#include "v8.h"
#include "uv.h"

#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <chrono>
#include <thread>
#include <iostream>

struct thread
{
    v8::Global<v8::Function> cb;
};

v8::Global<v8::Function> cb;
v8::Global<v8::Context> g_context;
v8::Isolate *isolate = nullptr;

class MyThread
{

public:
    static void do_work(v8::Global<v8::Function> func)
    {
        // auto func = (v8::Global<v8::Function> *)arg;

        if (isolate->IsDead())
        {
            printf("isolate is dead\n");
            return;
        }

        v8::Locker locker(isolate);
        v8::Isolate::Scope isolate_scope(isolate);
        v8::HandleScope handle_scope(isolate);
        v8::Local<v8::Context> context = v8::Local<v8::Context>::New(isolate, g_context);

        v8::Context::Scope context_scope(
            v8::Local<v8::Context>::New(isolate, context));
        v8::Local<v8::Function> callback = v8::Local<v8::Function>::New(isolate, func);

        v8::Local<v8::Value> result;

        v8::Handle<v8::Value> resultr[] = {v8::Undefined(isolate), v8_str("hello world")};
        v8::TryCatch try_catch(isolate);

        if (callback->Call(
                        context,
                        v8::Undefined(isolate),
                        2,
                        resultr)
                .ToLocal(&result))
        {
            // Ok, the callback succeded
            std::cout << "succeeded" << std::endl;
        }
        else
        {
            std::cout << "error" << std::endl;
            assert(try_catch.HasCaught());
            ReportException(isolate, &try_catch);
            // some exception happened
        }
        v8::Unlocker unlocker(isolate);

        std::cout << "do_work 6" << std::endl;
    }

    static void Thread(const v8::FunctionCallbackInfo<v8::Value> &args)
    {
        std::cout << "Start start" << std::endl;
        isolate = args.GetIsolate();
        auto context = isolate->GetCurrentContext();
        g_context.Reset(isolate, context);
        if (context.IsEmpty())
        {
            fprintf(stderr, "Error creating context\n");
            return;
        }
        v8::Local<v8::Value> callback = args[0];

        if (!callback->IsFunction())
        {
            printf("callback not declared!");
            return;
        }

        cb.Reset(isolate, callback.As<v8::Function>());

        // uv_thread_t task_id;
        // uv_thread_create(&task_id, do_work, &cb);
        // uv_thread_join(&task_id);

        std::thread new_thread(do_work, std::move(cb));
        new_thread.detach();
        std::cout << "Start end" << std::endl;
    }
};