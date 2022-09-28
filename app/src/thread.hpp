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
    v8::Global<v8::Context> context;
    v8::Isolate *isolate;
};

class MyThread
{

public:
    static void do_work(thread *task)
    {
        int tracklen = 10;
        while (tracklen)
        {
            tracklen--;
            fprintf(stderr, ".\n");
            uv_sleep(100);
        }
        fprintf(stderr, "Tortoise done running!\n");
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
        task->context.Reset(isolate, context);

        std::thread new_thread(do_work, std::move(task));
        new_thread.detach();
    }
};