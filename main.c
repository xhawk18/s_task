/* Copyright xhawk, MIT license */

#include <stdio.h>
#include "s_task.h"

//#define PRINTF(fmt...)
#define PRINTF printf

void sub_task(void *arg) {
    int i;
    int n = (int)arg;
    for (i = 0; i < 5; ++i) {
        PRINTF("task %d, delay seconds = %d, i = %d\n", n, n, i);
        task_msleep(n * 1000);
        //task_yield();
    }
}

void main_task(void *arg) {
    int i;
    task_create(sub_task, (void *)1);
    task_create(sub_task, (void *)2);

    for (i = 0; i < 4; ++i) {
        PRINTF("task_main arg = %p, i = %d\n", arg, i);
        task_yield();
    }
}

int main(int argc, char *argv) {
    task_init_system(main_task, (void *)argc);
    PRINTF("all task is over\n");
    return 0;
}

