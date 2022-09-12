

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

// Extracts a C string from a V8 Utf8Value.
const char *ToCString(const v8::String::Utf8Value &value)
{
    return *value ? *value : "<string conversion failed>";
}

static inline v8::Local<v8::String> v8_str(const char *x)
{
    return v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), x).ToLocalChecked();
}


struct timer
{
    uv_timer_t req;
    v8::Isolate *isolate;
    v8::FunctionCallbackInfo<v8::Value> pargs;
};

uv_loop_t *loop = uv_default_loop();

void OnTimerCb(uv_timer_t *handle)
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
    v8::Local<v8::Function> callback = timerWrap->pargs[2].As<v8::Function>();
    if (callback->Call(
                    context,
                    v8::Undefined(isolate),
                    2,
                    resultr)
            .ToLocal(&result))
    {
        timerWrap->pargs.GetReturnValue().Set(result);
    }
}

void Timeout(const v8::FunctionCallbackInfo<v8::Value> &args)
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

    timer *timerWrap = (timer *)malloc(sizeof(timer));

    timerWrap->isolate = args.GetIsolate();
    timerWrap->pargs = args;
    timerWrap->req.data = (void *)timerWrap;

    uv_timer_init(loop, &timerWrap->req);
    uv_timer_start(&timerWrap->req, OnTimerCb, delay, repeat);
}

void Print(const v8::FunctionCallbackInfo<v8::Value> &args)
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
        const char *cstr = ToCString(str);
        printf("%s", cstr);
    }
    printf("\n");
    fflush(stdout);
}

// Creates a new execution environment containing the built-in
// functions.
v8::Local<v8::Context> BindFunctionsAndCreateContext(v8::Isolate *isolate)
{
    // Create a template for the global object.
    v8::Local<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);
    // Bind the global 'print' function to the C++ Print callback.
    global->Set(isolate, "print", v8::FunctionTemplate::New(isolate, Print));
    global->Set(isolate, "timeout", v8::FunctionTemplate::New(isolate, Timeout));

    // Bind the global 'read' function to the C++ Read callback.
    return v8::Context::New(isolate, NULL, global);
}

// Reads a file into a v8 string.
v8::MaybeLocal<v8::String> ReadFile(v8::Isolate *isolate, const char *name)
{
    FILE *file = fopen(name, "rb");
    if (file == NULL)
        return v8::MaybeLocal<v8::String>();

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    rewind(file);

    char *chars = new char[size + 1];
    chars[size] = '\0';
    for (size_t i = 0; i < size;)
    {
        i += fread(&chars[i], 1, size - i, file);
        if (ferror(file))
        {
            fclose(file);
            return v8::MaybeLocal<v8::String>();
        }
    }
    fclose(file);
    v8::MaybeLocal<v8::String> result = v8::String::NewFromUtf8(
        isolate, chars, v8::NewStringType::kNormal, static_cast<int>(size));
    delete[] chars;
    return result;
}

int main(int argc, char *argv[])
{
    loop = uv_default_loop();

    v8::V8::InitializeICUDefaultLocation(argv[0]);
    v8::V8::InitializeExternalStartupData(argv[0]);
    std::unique_ptr<v8::Platform> platform = v8::platform::NewDefaultPlatform();
    v8::V8::InitializePlatform(platform.get());
    v8::V8::Initialize();
    v8::V8::SetFlagsFromCommandLine(&argc, argv, true);
    v8::Isolate::CreateParams create_params;
    create_params.array_buffer_allocator =
        v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    v8::Isolate *isolate = v8::Isolate::New(create_params);

    int result;
    {
        v8::Isolate::Scope isolate_scope(isolate);
        v8::HandleScope handle_scope(isolate);
        v8::Local<v8::Context> context = BindFunctionsAndCreateContext(isolate);
        if (context.IsEmpty())
        {
            fprintf(stderr, "Error creating context\n");
            return 1;
        }
        v8::Context::Scope context_scope(context);
        {

            v8::Local<v8::String> source;
            if (!ReadFile(isolate, argv[1]).ToLocal(&source))
            {
                fprintf(stderr, "Error reading file\n");
                return 1;
            }

            v8::ScriptOrigin origin(isolate, v8::String::NewFromUtf8(context->GetIsolate(), argv[1]).ToLocalChecked());

            v8::Local<v8::Script> script =
                v8::Script::Compile(context, source, &origin).ToLocalChecked();

            // Run the script to get the result.
            v8::Local<v8::Value> result = script->Run(context).ToLocalChecked();

            // Convert the result to an UTF8 string and print it.
            v8::String::Utf8Value utf8(isolate, result);
            printf("%s\n", *utf8);

            bool more;
            do
            {
                v8::platform::PumpMessageLoop(platform.get(), isolate);
                more = uv_run(loop, UV_RUN_DEFAULT);
                if (more == false)
                {
                    v8::platform::PumpMessageLoop(platform.get(), isolate);
                    more = uv_loop_alive(loop);
                    int isRun = uv_run(loop, UV_RUN_NOWAIT);
                    if (uv_run(loop, UV_RUN_NOWAIT) != 0)
                    {
                        more = true;
                    }
                }

            } while (more == true);
        }
    }

    isolate->Dispose();
    v8::V8::Dispose();
    v8::V8::DisposePlatform();
    delete create_params.array_buffer_allocator;

    return result;
}