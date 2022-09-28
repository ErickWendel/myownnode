#include <stdio.h>
#include <string.h>

#include <uv.h>

void dowork(void *arg) {
    int counter = *((int *) arg);
    while (counter) {
        counter--;
        uv_sleep(200);
        fprintf(stderr, "another step\n");
    }
    fprintf(stderr, "done running!\n");
} 

int main() {
    int counter = 10;
    uv_thread_t taskId;
    uv_thread_create(&taskId, dowork, &counter);

    uv_thread_join(&taskId);
    return 0;
}