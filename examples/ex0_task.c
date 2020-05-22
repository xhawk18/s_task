/* Copyright xhawk, MIT license */

#include <stdio.h>
#include "s_task.h"

void* g_stack_main[64 * 1024];
void* g_stack0[64 * 1024];
void* g_stack1[64 * 1024];

void sub_task(__async__, void* arg) {
    int i;
    int n = (int)(size_t)arg;
    for (i = 0; i < 5; ++i) {
        printf("task %d, delay seconds = %d, i = %d\n", n, n, i);
        s_task_msleep(__await__, n * 1000);
        /* s_task_yield(__await__); */
    }
}

void main_task(__async__, void* arg) {
    int i;
    s_task_create(g_stack0, sizeof(g_stack0), sub_task, (void*)1);
    s_task_create(g_stack1, sizeof(g_stack1), sub_task, (void*)2);

    for (i = 0; i < 4; ++i) {
        printf("task_main arg = %p, i = %d\n", arg, i);
        s_task_yield(__await__);
    }

    s_task_join(__await__, g_stack0);
    s_task_join(__await__, g_stack1);
}

int main(int argc, char *argv[]) {
    (void)argv;

    s_task_init_system();
    s_task_create(g_stack_main, sizeof(g_stack_main), main_task, (void*)(size_t)argc);
    s_task_join(__await__, g_stack_main);

    printf("all task is over\n");
    return 0;
}
