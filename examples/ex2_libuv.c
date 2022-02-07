/* Copyright xhawk, MIT license */

#include <stdio.h>
#include "s_task.h"


s_event_t  g_event;
bool       g_closed = false;
void      *g_stack_main[512*1024];
void      *g_stack0[512*1024];
void      *g_stack1[512*1024];


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

    s_task_create(g_stack0, sizeof(g_stack0), sub_task, (void *)1);
    s_task_create(g_stack1, sizeof(g_stack1), sub_task, (void *)2);

    for (i = 0; i < 5; ++i) {
        printf("task_main arg = %p, i = %d\n", arg, i);
        s_task_msleep(__await__, 500);
        if (i % 3 == 0) {
            printf("task main set event\n");
            s_event_set(&g_event);
        }
    }

    g_closed = true;
}


int main(int argc, char *argv[]) {
    uv_loop_t *loop = uv_default_loop();
    s_task_init_uv_system(loop);

    s_task_create(g_stack_main, sizeof(g_stack_main), main_task, (void *)(size_t)argc);
    
    uv_run(loop, UV_RUN_DEFAULT);
    printf("all task is over\n");
    return 0;
}
