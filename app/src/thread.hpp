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
    v8::Isolate *isolate;
};

class MyThread
{

public:
    static void RunThread(void *arg)
    {
        thread *task = (thread *)arg;

        v8::Isolate *isolate = task->isolate;

        if (isolate->IsDead())
        {
            printf("isolate is dead\n");
            return;
        }

        {
            v8::Locker locker(isolate);
            
            v8::Isolate::Scope isolate_scope(isolate);
            
            v8::HandleScope handle_scope(isolate);
            
            v8::Local<v8::Context> context = v8::Local<v8::Context>::New(isolate, isolate->GetCurrentContext());

            v8::Context::Scope context_scope(v8::Local<v8::Context>::New(isolate, context));

            v8::Local<v8::Function> callback = v8::Local<v8::Function>::New(isolate, task->cb);

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

            std::cout << "do_work 6" << std::endl;
        }

        // int tracklen = 10;
        // while (tracklen)
        // {
        //     tracklen--;
        //     fprintf(stderr, ".\n");
        //     uv_sleep(100);
        // }
        // fprintf(stderr, "Tortoise done running!\n");

        // if (isolate->IsDead())
        // {
        //     printf("isolate is dead\n");
        //     return;
        // }
    }

    static void Thread(const v8::FunctionCallbackInfo<v8::Value> &args)
    {
        auto isolate = args.GetIsolate();
        auto context = isolate->GetCurrentContext();

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

        thread *task = new thread();
        task->isolate = isolate;
        task->cb.Reset(isolate, callback.As<v8::Function>());

        uv_thread_t taskId;
        uv_thread_create(&taskId, RunThread, &task);
        uv_thread_join(&taskId);

        // std::thread new_thread(do_work, std::move(task));
        // new_thread.detach();
    }
};