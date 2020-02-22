/* Copyright xhawk, MIT license */

#include <stdio.h>
#include "s_task.h"

void* stack_main[64 * 1024];
void* stack0[64 * 1024];
void* stack1[64 * 1024];

void sub_task(__async__, void* arg) {
    int i;
    int n = (int)(size_t)arg;
    for (i = 0; i < 5; ++i) {
        printf("task %d, delay seconds = %d, i = %d\n", n, n, i);
        s_task_msleep(__await__, n * 1000);
        //s_task_yield(__await__);
    }
}

void main_task(__async__, void* arg) {
    int i;
    s_task_create(stack0, sizeof(stack0), sub_task, (void*)1);
    s_task_create(stack1, sizeof(stack1), sub_task, (void*)2);

    for (i = 0; i < 4; ++i) {
        printf("task_main arg = %p, i = %d\n", arg, i);
        s_task_yield(__await__);
    }

    s_task_join(__await__, stack0);
    s_task_join(__await__, stack1);
}

int main(int argc, char* argv) {
    __init_async__;

    s_task_init_system();
    s_task_create(stack_main, sizeof(stack_main), main_task, (void*)(size_t)argc);
    s_task_join(__await__, stack_main);
    printf("all task is over\n");
    return 0;
}
