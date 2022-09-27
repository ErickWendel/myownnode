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
    v8::Isolate *isolate;
    v8::Global<v8::Function> cb;
};

class MyThread
{

public:
    static v8::Global<v8::Context> g_context;
    static v8::Isolate *isolate;

    // static void Task(void *arg)
    // {

    //     thread *task = (thread *)arg;

    //     v8::Isolate *isolate = task->isolate;
    //     v8::Local<v8::Context> context = isolate->GetCurrentContext();

    //     if (isolate->IsDead())
    //     {
    //         printf("isolate is dead\n");
    //         return;
    //     }

    //     v8::Isolate::CreateParams create_params;
    //     create_params.array_buffer_allocator =
    //         v8::ArrayBuffer::Allocator::NewDefaultAllocator();

    //     v8::Isolate *isolate2 = v8::Isolate::New(create_params);
    //     {
    //         v8::Isolate::Scope isolate_scope(isolate2);
    //         // Create a stack-allocated handle scope.
    //         v8::HandleScope handle_scope(isolate2);
    //         // Create a new context.
    //         v8::Local<v8::Context> context2 = v8::Context::New(isolate2);
    //         // // Enter the context for compiling and running the hello world script.
    //         v8::Context::Scope context_scope(context2);
    //         v8::Local<v8::Function> callback = v8::Local<v8::Function>::New(isolate2, task->cb);
    //         v8::Handle<v8::Value> resultr[] = {v8::Undefined(isolate2), v8_str("hello world")};
    //         printf("here\n");

    //         v8::Local<v8::Value> result;
    //         if (callback->Call(
    //                         context2,
    //                         v8::Undefined(isolate2),
    //                         2,
    //                         resultr)
    //                 .ToLocal(&result))
    //         {
    //             printf("succeded\n");
    //             // Ok, the callback succeded
    //         }
    //         else
    //         {
    //             printf("exception\n");
    //             // some exception happened
    //         }
    //     }

    //     // Dispose the isolate and tear down V8.
    //     isolate2->Dispose();
    // }
    static void do_work(v8::Global<v8::Function> func)
    {
        // std::cout << "do_work 0" << std::endl;

        v8::Locker locker(isolate);
        // std::cout << "do_work 1" << std::endl;

        v8::Isolate::Scope isolate_scope(isolate);
        // std::cout << "do_work 2" << std::endl;

        v8::HandleScope handle_scope(isolate);
        // std::cout << "do_work 3" << std::endl;

        v8::Local<v8::Context> context = v8::Local<v8::Context>::New(isolate, g_context);

        v8::Context::Scope context_scope(
            v8::Local<v8::Context>::New(isolate, context));
        // std::cout << "do_work 4" << std::endl;

        v8::Local<v8::Function> fn = v8::Local<v8::Function>::New(isolate, func);
        // std::cout << "do_work 5" << std::endl;

        // v8::Local<v8::Value> v = fn->Call(v8::Undefined(isolate), 0, NULL);
        v8::Local<v8::Value> result;

        if (fn->Call(
                  context,
                  v8::Undefined(isolate),
                  0,
                  NULL)
                .ToLocal(&result))
        {
            // Ok, the callback succeded
        }
        else
        {
            // some exception happened
        }

        // std::cout << "do_work 6" << std::endl;
    }

    static void Thread(const v8::FunctionCallbackInfo<v8::Value> &args)
    {
        // std::cout << "Start start" << std::endl;
        isolate = args.GetIsolate();
        v8::Global<v8::Function> fn;
        v8::Local<v8::Function> f = v8::Local<v8::Function>::Cast(args[0]);
        fn.Reset(isolate, f);

        std::thread new_thread(do_work, std::move(fn));
        new_thread.detach();
        // std::cout << "Start end" << std::endl;
    }
    // static void Thread(const v8::FunctionCallbackInfo<v8::Value> &args)
    // {
    //     auto isolate = args.GetIsolate();

    //     v8::Local<v8::Value> callback = args[0];

    //     if (!callback->IsFunction())
    //     {
    //         printf("callback not declared!");
    //         return;
    //     }

    //     uv_thread_t task_id;

    //     thread *task = new thread();

    //     task->isolate = isolate;
    //     task->cb.Reset(isolate, callback.As<v8::Function>());
    //     uv_thread_create(&task_id, Task, &task);
    //     uv_thread_join(&task_id);
    // };
};