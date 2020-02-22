/* Copyright xhawk, MIT license */

#include <stdio.h>
#include "s_task.h"

//#define PRINTF(fmt...)
#define PRINTF printf



s_event_t g_event;

void sub_task(__async__, void *arg) {
    size_t n = (size_t)arg;

    while (1) {
        s_event_wait_msec(__await__, &g_event, 300);
        //s_task_msleep(__await__, 500);
        PRINTF("task %d wait event OK\n", (int)n);
    }
}

#if defined __ARMCC_VERSION
void *stack_main[256];
void *stack0[256];
void *stack1[256];
#elif defined STM8S103
void *stack_main[256];
void *stack0[256];
void *stack1[256];
#elif defined _WIN32
void *stack_main[512*1024];
void *stack0[512*1024];
void *stack1[512*1024];
#else
void *stack_main[512*1024];
void *stack0[512*1024];
void *stack1[512*1024];
#endif

void main_task(__async__, void *arg) {
    int i;
    s_event_init(&g_event);

    s_task_create(stack0, sizeof(stack0), sub_task, (void *)1);
    s_task_create(stack1, sizeof(stack1), sub_task, (void *)2);

    for (i = 0; i < 12; ++i) {
        PRINTF("task_main arg = %p, i = %d\n", arg, i);
        s_task_msleep(__await__, 500);
        if (i % 3 == 0) {
            PRINTF("task main set event\n");
            s_event_set(&g_event);
            //s_task_yield(__await__);
        }
    }
    
    while (0) {
        PRINTF("loop in main_task\n");
        s_task_msleep(__await__, 1000);
    }
}

int main(int argc, char *argv) {
    __init_async__;

    s_task_init_system();
    s_task_create(stack_main, sizeof(stack_main), main_task, (void *)(size_t)argc);
    
    //while(1){
    //    s_task_yield(__await__);
    //}
    s_task_join(__await__, stack_main);
    PRINTF("all task is over\n");
    return 0;
}
