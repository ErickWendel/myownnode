#include <v8.h>
#include <uv.h>


class MyThread {
    public:
        static void RunThread(void *arg) 
        {
            int counter = * ((int *)arg);
            while(counter) {
                counter --;
                printf(".\n");
                uv_sleep(100);
            }
            printf("done running out thread!!\n");
        }

        static void Thread(const v8::FunctionCallbackInfo<v8::Value> &args) 
        {
            auto isolate = args.GetIsolate();
            auto context = isolate->GetCurrentContext();
            int64_t counter = args[0]->IntegerValue(context).ToChecked();
            uv_thread_t taskId;
            uv_thread_create(&taskId, RunThread, &counter);
            uv_thread_join(&taskId);
        }
};