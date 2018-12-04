/* Copyright xhawk, MIT license */

#include <stdio.h>
#include "s_task.h"

//#define PRINTF(fmt...)
#define PRINTF printf



event_t g_event;

void sub_task(__async__, void *arg) {
    int n = (int)arg;

    while (1) {
        event_wait(__await__, &g_event);
        PRINTF("task %d wait event OK\n", n);
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
    event_init(&g_event);

    task_create(stack0, sizeof(stack0), sub_task, (void *)1);
    task_create(stack1, sizeof(stack1), sub_task, (void *)2);

    for (i = 0; i < 24; ++i) {
        PRINTF("task_main arg = %p, i = %d\n", arg, i);
        task_msleep(__await__, 500);
        if (i % 3 == 0) {
            PRINTF("task main set event\n");
            event_set(&g_event);
            task_yield(__await__);
        }
    }
    
    while (0) {
        PRINTF("loop in main_task\n");
        task_msleep(__await__, 1000);
    }
}

int main(int argc, char *argv) {
    task_init_system(stack_main, sizeof(stack_main), main_task, (void *)argc);
    while(1){
        __async__;
        task_yield(__await__);
    }
    //PRINTF("all task is over\n");
    //return 0;
}

