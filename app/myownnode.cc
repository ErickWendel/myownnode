

#include <libplatform/libplatform.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

#include <uv.h>
#include "v8-context.h"
#include "v8-exception.h"
#include "v8-initialization.h"
#include "v8-isolate.h"
#include "v8-local-handle.h"
#include "v8-script.h"
#include "v8-template.h"
#include "v8.h"

#include "./fs.h"

uv_loop_t *DEFAULT_LOOP = uv_default_loop();

struct timer
{
    uv_timer_t req;
    v8::Isolate *isolate;
    v8::Global<v8::Function> cb;
};

static inline v8::Local<v8::String> v8_str(const char *x)
{
    return v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), x).ToLocalChecked();
}

class Timer
{
public:
    static void OnTimerCb(uv_timer_t *handle)
    {
        timer *timerWrap = (timer *)handle->data;

        v8::Isolate *isolate = timerWrap->isolate;
        v8::Local<v8::Context> context = isolate->GetCurrentContext();

        if (isolate->IsDead())
        {
            printf("isolate is dead\n");
            return;
        }

        v8::Local<v8::Value> result;
        v8::Handle<v8::Value> resultr[] = {v8::Undefined(isolate), v8_str("hello world")};
        v8::Local<v8::Function> callback = v8::Local<v8::Function>::New(isolate, timerWrap->cb);

        if (callback->Call(
                        context,
                        v8::Undefined(isolate),
                        2,
                        resultr)
                .ToLocal(&result))
        {
            // Ok, the callback succeded
        }
        else
        {
            // some exception happened
        }
    }

    static void Timeout(const v8::FunctionCallbackInfo<v8::Value> &args)
    {
        auto isolate = args.GetIsolate();

        auto context = isolate->GetCurrentContext();
        int64_t delay = args[0]->IntegerValue(context).ToChecked();
        int64_t repeat = args[1]->IntegerValue(context).ToChecked();
        v8::Local<v8::Value> callback = args[2];

        if (!callback->IsFunction())
        {
            printf("callback not declared!");
            return;
        }

        timer *timerWrap = new timer();

        timerWrap->isolate = isolate;
        timerWrap->cb.Reset(isolate, callback.As<v8::Function>());
        timerWrap->req.data = (void *)timerWrap;

        uv_timer_init(DEFAULT_LOOP, &timerWrap->req);
        uv_timer_start(&timerWrap->req, OnTimerCb, delay, repeat);
    }
};

class MyOwnNode
{
private:
    v8::Isolate *isolate;
    v8::Local<v8::Context> context;
    std::unique_ptr<v8::Platform> *platform;
    v8::Isolate::CreateParams create_params;

    void WaitForEvents()
    {
        bool more;
        do
        {
            v8::platform::PumpMessageLoop(this->platform->get(), this->isolate);
            more = uv_run(DEFAULT_LOOP, UV_RUN_DEFAULT);
            if (more == false)
            {
                v8::platform::PumpMessageLoop(this->platform->get(), this->isolate);
                more = uv_loop_alive(DEFAULT_LOOP);
                int isRun = uv_run(DEFAULT_LOOP, UV_RUN_NOWAIT);
                if (uv_run(DEFAULT_LOOP, UV_RUN_NOWAIT) != 0)
                {
                    more = true;
                }
            }

        } while (more == true);
    }

    void ExecuteScriptAndWaitForEvents(char *filename)
    {
        // Enter the context for compiling and running the hello world script.
        v8::Context::Scope context_scope(this->context);
        {
            // Read file from args
            v8::Local<v8::String> source;
            if (!Fs::ReadFile(isolate, filename).ToLocal(&source))
            {
                fprintf(stderr, "Error reading file\n");
                return;
            }

            // Create a string containing the JavaScript source code.
            v8::ScriptOrigin origin(isolate, v8_str(filename));

            // Compile the source code.
            v8::Local<v8::Script> script =
                v8::Script::Compile(this->context, source, &origin).ToLocalChecked();

            // Run the script to get the result.
            v8::Local<v8::Value> result = script->Run(this->context).ToLocalChecked();

            // Convert the result to an UTF8 string and print it.
            v8::String::Utf8Value utf8(this->isolate, result);

            // fprintf(stderr, "result %s\n", *utf8);
            WaitForEvents();
        }
    }

    static void Print(const v8::FunctionCallbackInfo<v8::Value> &args)
    {

        bool first = true;
        for (int i = 0; i < args.Length(); i++)
        {
            v8::HandleScope handle_scope(args.GetIsolate());
            if (first)
            {
                first = false;
            }
            else
            {
                printf(" ");
            }
            v8::String::Utf8Value str(args.GetIsolate(), args[i]);
            printf("%s", *str);
        }
        printf("\n");
        fflush(stdout);
    }

public:
    std::unique_ptr<v8::Platform> initializeV8(int argc, char *argv[])
    {
        v8::V8::InitializeICUDefaultLocation(argv[0]);
        v8::V8::InitializeExternalStartupData(argv[0]);
        std::unique_ptr<v8::Platform> platform = v8::platform::NewDefaultPlatform();
        v8::V8::InitializePlatform(platform.get());
        v8::V8::Initialize();
        // v8::V8::SetFlagsFromCommandLine(&argc, argv, true);

        this->platform = &platform;
        return platform;
    }

    void initializeVM()
    {
        // Create a new Isolate and make it the current one.
        v8::Isolate::CreateParams create_params;
        create_params.array_buffer_allocator =
            v8::ArrayBuffer::Allocator::NewDefaultAllocator();
        this->isolate = v8::Isolate::New(create_params);
        this->create_params = create_params;
    }

    void InitializeProgram(char *filename)
    {
        v8::Isolate::Scope isolate_scope(this->isolate);

        // Create a stack-allocated handle scope.
        v8::HandleScope handle_scope(this->isolate);

        // Create a template for the global object.
        v8::Local<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);
        // Bind the global 'print' function to the C++ Print callback.
        global->Set(isolate, "print", v8::FunctionTemplate::New(isolate, Print));

        global->Set(isolate, "timeout", v8::FunctionTemplate::New(isolate, Timer::Timeout));

        // Create a new context.
        this->context = v8::Context::New(this->isolate, NULL, global);
        if (this->context.IsEmpty())
        {
            fprintf(stderr, "Error creating context\n");
            return;
        }

        ExecuteScriptAndWaitForEvents(filename);
    }

    void Shutdown()
    {
        this->isolate->Dispose();
        v8::V8::Dispose();
        v8::V8::DisposePlatform();
        delete this->create_params.array_buffer_allocator;
    }
};

int main(int argc, char *argv[])
{
    char *filename = argv[1];
    MyOwnNode *myOwnNode = new MyOwnNode();
    std::unique_ptr<v8::Platform> platform =
        myOwnNode->initializeV8(argc, argv);

    myOwnNode->initializeVM();
    myOwnNode->InitializeProgram(filename);
    myOwnNode->Shutdown();

    return 0;
}