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
        int counter = *((int *)arg);
        while (counter)
        {
            counter--;
            fprintf(stderr, ".\n");
            uv_sleep(100);
        }
        fprintf(stderr, "Tortoise done running!\n");
    }

    static void Thread(const v8::FunctionCallbackInfo<v8::Value> &args)
    {
        auto isolate = args.GetIsolate();
        auto context = isolate->GetCurrentContext();
        int64_t counter = args[0]->IntegerValue(context).ToChecked();

        uv_thread_t taskId;
        uv_thread_create(&taskId, RunThread, &counter);
        // uv_thread_join(&taskId);
    }
};