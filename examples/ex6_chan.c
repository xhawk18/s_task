/* Copyright xhawk, MIT license */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "s_task.h"

/* Declares for chan1 */
typedef struct {
    int i;
    int n;
} my_element_t;
s_chan_declare(g_my_chan1, my_element_t, 3);
void* g_stack_chan1_get[64 * 1024];
void* g_stack_chan1_put_a[64 * 1024];
void* g_stack_chan1_put_b[64 * 1024];
bool  g_chan1_closed = false;

/* Declares for chan2 */
s_chan_declare(g_my_chan2, char, 4096);
void* g_stack_chan2_get[64 * 1024];
void* g_stack_chan2_put[64 * 1024];
bool  g_chan2_closed = false;



void task_chan1_put(__async__, void* arg) {
    int n = (int)(size_t)arg;
    int i;

    for (i = 0; !g_chan1_closed; ++i) {
        my_element_t element;
        element.i = i;
        element.n = n;
        printf("s_chan_put = %d %d\n", element.i, element.n);

        /* put element into chain */
        s_chan_put(__await__, g_my_chan1, &element);

        s_task_sleep(__await__, 1);
    }
}

void task_chan1_get(__async__, void* arg) {
    (void)arg;
    my_element_t element;
    int sum;

    s_chan_init(g_my_chan1, my_element_t, 3);

    s_task_create(g_stack_chan1_put_a, sizeof(g_stack_chan1_put_a), task_chan1_put, (void*)1);
    s_task_create(g_stack_chan1_put_b, sizeof(g_stack_chan1_put_b), task_chan1_put, (void*)2);

    sum = 0;
    while (sum < 100) {

        /* get element from chan */
        s_chan_get(__await__, g_my_chan1, &element);
        printf("s_chan_get = %d %d\n", element.i, element.n);

        sum += element.i;
        sum += element.n;
    }
    g_chan1_closed = true;

    s_task_join(__await__, g_stack_chan1_put_a);
    s_task_join(__await__, g_stack_chan1_put_b);
}


void task_chan2_put(__async__, void* arg) {
    (void)arg;
    
    char x = 0;
    while(true) {
        char buf[10];
        uint16_t number = (uint16_t)(rand() % 10);
        uint16_t i;
        for (i = 0; i < number; ++i)
            buf[i] = x++;

        s_chan_put_n(__await__, g_my_chan2, buf, number);
    }
}

void task_chan2_get(__async__, void* arg) {
    (void)arg;

    s_chan_init(g_my_chan2, char, 512);
    s_task_create(g_stack_chan2_put, sizeof(g_stack_chan2_put), task_chan2_put, NULL);

    char x = 0;
    while (true) {
        char buf[10];
        uint16_t i;
        uint16_t number = (uint16_t)(sizeof(buf) / sizeof(buf[0]));

        s_chan_get_n(__await__, g_my_chan2, buf, number);

        for (i = 0; i < number; ++i) {
            if (buf[i] != x++) {
                fprintf(stderr, "buf check error\n");
                goto end;
            }
        }
        printf("get %d at line %d\n", (int)x, __LINE__);
    }

end:
    s_task_join(__await__, g_stack_chan2_put);
}




int main(void) {

    s_task_init_system();

    srand((unsigned int)time(NULL));
    
    s_task_create(g_stack_chan1_get, sizeof(g_stack_chan1_get), task_chan1_get, NULL);
    s_task_create(g_stack_chan2_get, sizeof(g_stack_chan2_get), task_chan2_get, NULL);

    s_task_join(__await__, g_stack_chan1_get);
    printf("task for chan 1 is finished\n");
    s_task_join(__await__, g_stack_chan2_get);
    printf("task for chan 2 is finished\n");
    return 0;
}
