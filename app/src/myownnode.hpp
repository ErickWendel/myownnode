#include <libplatform/libplatform.h>
#include <uv.h>
#include "v8.h"

#include "./fs.hpp"
#include "./util.hpp"
#include "./timer.hpp"
// #include "./thread.hpp"

uv_loop_t *DEFAULT_LOOP = uv_default_loop();

class MyOwnNode
{
private:
    v8::Isolate *isolate;
    v8::Local<v8::Context> context;
    std::unique_ptr<v8::Platform> *platform;
    v8::Isolate::CreateParams create_params;

    void WaitForEvents()
    {
        // https://groups.google.com/g/v8-users/c/xaSx3ATgFbg
        // while(v8::platform::PumpMessageLoop(this->platform->get(), this->isolate)){}
        uv_run(DEFAULT_LOOP, UV_RUN_DEFAULT);
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
        Timer t;
        t.Initialize(DEFAULT_LOOP);

        global->Set(isolate, "print", v8::FunctionTemplate::New(isolate, Print));
        global->Set(isolate, "timeout", v8::FunctionTemplate::New(isolate, t.Timeout));
        // global->Set(isolate, "thread", v8::FunctionTemplate::New(isolate, MyThread::Thread));

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
