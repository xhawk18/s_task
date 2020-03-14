/* Copyright xhawk, MIT license */

#include <stdio.h>
#include "s_task.h"


s_event_t  g_event;
bool       g_closed = false;
#if defined __ARMCC_VERSION
void      *stack_main[256];
void      *stack0[256];
void      *stack1[256];
#elif defined STM8S103
char       stack_main[192];
char       stack0[192];
char       stack1[192];
#else
void      *stack_main[512*1024];
void      *stack0[512*1024];
void      *stack1[512*1024];
#endif


void sub_task(__async__, void *arg) {
    size_t n = (size_t)arg;

    while (!g_closed) {
        s_event_wait_msec(__await__, &g_event, 300);
        printf("task %d wait event OK\n", (int)n);
    }
}


void main_task(__async__, void *arg) {
    int i;
    s_event_init(&g_event);

    s_task_create(stack0, sizeof(stack0), sub_task, (void *)1);
    s_task_create(stack1, sizeof(stack1), sub_task, (void *)2);

    while(true) {
        ++i;
        printf("task_main arg = %p, i = %d\n", arg, i);

        s_task_yield(__await__);
        //s_task_msleep(__await__, 500);

        if (i % 3 == 0) {
            printf("task main set event\n");
            s_event_set(&g_event);
        }
    }

    g_closed = true;
}

int main(int argc, char *argv[]) {
    s_task_init_system();

    s_task_create(stack_main, sizeof(stack_main), main_task, (void *)(size_t)argc);
    
    __init_async__;
    s_task_join(__await__, stack_main);
    printf("all task is over\n");
    return 0;
}
