/* Copyright xhawk, MIT license */

#include <stdio.h>
#include "s_task.h"

//#define PRINTF(fmt...)
#define PRINTF printf

event_t g_event;

void sub_task(void *arg) {
    int n = (int)arg;

    while (1) {
        event_wait(&g_event);
        PRINTF("task %d wait event OK\n", n);
    }
}

void main_task(void *arg) {
    int i;
    event_init(&g_event);

    task_create(sub_task, (void *)1);
    task_create(sub_task, (void *)2);

    for (i = 0; i < 24; ++i) {
        PRINTF("task_main arg = %p, i = %d\n", arg, i);
        task_msleep(500);
        if (i % 3 == 0) {
            PRINTF("task main set event\n");
            event_set(&g_event);
            task_yield();
        }
    }
    
    while (1) {
        PRINTF("loop in main_task\n");
        task_msleep(1000);
    }
}

int main(int argc, char *argv) {
    task_init_system(main_task, (void *)argc);
    PRINTF("all task is over\n");
    return 0;
}

