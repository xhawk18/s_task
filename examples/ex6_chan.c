/* Copyright xhawk, MIT license */

#include <stdio.h>
#include "s_task.h"

void* g_stack_main[64 * 1024];
void* g_stack0[64 * 1024];
void* g_stack1[64 * 1024];
bool  g_closed = false;

typedef struct {
    int i;
    int n;
} my_element_t;

s_chan_declare(g_my_chan, my_element_t, 3);


void sub_task(__async__, void* arg) {
    int n = (int)(size_t)arg;
    int i;

    for (i = 0; !g_closed; ++i) {
        my_element_t element;
        element.i = i;
        element.n = n;
        printf("s_chan_put = %d %d\n", element.i, element.n);
        fflush(stdout);

        /* put element into chain */
        s_chan_put(__await__, g_my_chan, &element);

        s_task_sleep(__await__, 1);
    }
}

void main_task(__async__, void* arg) {
    my_element_t element;
    int sum;

    s_chan_init(g_my_chan, my_element_t, 3);

    s_task_create(g_stack0, sizeof(g_stack0), sub_task, (void*)1);
    s_task_create(g_stack1, sizeof(g_stack1), sub_task, (void*)2);

    sum = 0;
    while (sum < 100) {

        /* get element from chan */
        s_chan_get(__await__, g_my_chan, &element);
        printf("s_chan_get = %d %d\n", element.i, element.n);
        fflush(stdout);

        sum += element.i;
        sum += element.n;
    }
    g_closed = true;

    s_task_join(__await__, g_stack0);
    s_task_join(__await__, g_stack1);
}

int main(int argc, char* argv[]) {
    __init_async__;

    s_task_init_system();
    s_task_create(g_stack_main, sizeof(g_stack_main), main_task, (void*)(size_t)argc);
    s_task_join(__await__, g_stack_main);
    printf("all task is over\n");
    return 0;
}
