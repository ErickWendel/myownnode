#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

#include <uv.h>

uv_loop_t *loop;
uv_timer_t gc_req;
uv_timer_t fake_job_req;

void callbackFn()
{
    printf("callback executed!\n");
}

typedef void (*functionTemplate)(void);

struct timer
{
    uv_timer_t req;
    std::string text;
    functionTemplate *callback;
};

void work(uv_timer_t *handle)
{
    timer *timerWrap = (timer *)handle->data;

    ((functionTemplate)timerWrap->callback)();

    printf("%s", timerWrap->text.c_str());
}

int main()
{
    loop = uv_default_loop();
    for (size_t i = 0; i < 10; i++)
    {
        timer *timerWrap = new timer();
        timerWrap->callback = (functionTemplate *)callbackFn;
        timerWrap->text = "hello\n";

        timerWrap->req.data = (void *)timerWrap;

        // could actually be a TCP download or something
        uv_timer_init(loop, &timerWrap->req);
        int delay = 500 + i;
        int interval = 0;
        uv_timer_start(&timerWrap->req, work, delay, interval);
    }

    return uv_run(loop, UV_RUN_DEFAULT);
}