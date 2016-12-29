/* Copyright xhawk, MIT license */

#include <setjmp.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include "s_task.h"

#include "s_port.c"

/*******************************************************************/
/* list                                                            */
/*******************************************************************/
struct tag_list;
typedef struct tag_list {
    struct tag_list *next;
    struct tag_list *prev;
} list_t;


#define GET_PARENT_ADDR(pMe,tParent,eMyName) \
    ((tParent *)((char *)(pMe) - (ptrdiff_t)&((tParent *)0)->eMyName))

list_t *list_get_prev (list_t *list) {
    return list->prev;
}

list_t *list_get_next (list_t *list) {
    return list->next;
}

void list_set_prev (list_t *list, list_t *other) {
    list->prev = other;
}

void list_set_next (list_t *list, list_t *other) {
    list->next = other;
}

/* Initilization a list */
void list_init(list_t *list) {
    list_set_prev(list, list);
    list_set_next(list, list);
}

/* Connect or disconnect two lists. */
void list_toggle_connect(list_t *list1, list_t *list2) {
    list_t *prev1 = list_get_prev(list1);
    list_t *prev2 = list_get_prev(list2);
    list_set_next(prev1, list2);
    list_set_next(prev2, list1);
    list_set_prev(list1, prev2);
    list_set_prev(list2, prev1);
}

/* Connect two lists. */
void list_connect (list_t *list1, list_t *list2) {
    list_toggle_connect (list1, list2);
}

/* Disconnect tow lists. */
void list_disconnect (list_t *list1, list_t *list2) {
    list_toggle_connect (list1, list2);
}

/* Same as list_connect */
void list_attach (list_t *node1, list_t *node2) {
    list_connect (node1, node2);
}

/* Make node in detach mode */
void list_detach (list_t *node) {
    list_disconnect (node, list_get_next(node));
}

/* Check if list is empty */
int list_is_empty (list_t *list) {
    return (list_get_next(list) == list);
}



typedef struct {
    list_t node;
    jmp_buf jbuf;
    my_clock_t wakeup_ticks;
} task_t;


typedef void *align_data_t;

static jmp_buf     g_jbuf_init;
static jmp_buf     g_jbuf;
static jmp_buf     g_jbuf_next;
static task_fn_t   g_entry;
static void       *g_arg;
static list_t      g_exit_tasks;
static list_t      g_waiting_tasks;
static task_t     *g_current_task = NULL;
static list_t      g_timers;

static void run_next_task(void);
static void run_task(align_data_t * prev_task);


/*******************************************************************/
/* timer                                                           */
/*******************************************************************/

static void timer_run() {
    my_clock_t current_ticks = my_clock();

    list_t *node = list_get_next(&g_timers);
    while (node != &g_timers) {
        task_t *task = GET_PARENT_ADDR(node, task_t, node);

        int ticks_to_wakeup = (int)(task->wakeup_ticks - current_ticks);
        if (ticks_to_wakeup <= 0) {
            list_t *node_next = list_get_next(node);
            list_detach(node);
            list_attach(&g_waiting_tasks, node);
            node = node_next;
        }
        else break;
    }
}

static void timer_wait_recent() {
    my_clock_t current_ticks = my_clock();
    list_t *node = list_get_next(&g_timers);
    if (node != &g_timers) {
        task_t *task = GET_PARENT_ADDR(node, task_t, node);
        int ticks_to_wakeup = (int)(task->wakeup_ticks - current_ticks);
        if (ticks_to_wakeup > 0)
            my_sleep_ms((uint64_t)ticks_to_wakeup * 1000  / MY_CLOCKS_PER_SEC);
    }
}

void task_sleep_ticks(my_clock_t ticks) {
    my_clock_t current_ticks;
    list_t *node;
    
    current_ticks = my_clock();
    g_current_task->wakeup_ticks = current_ticks + ticks;

    for(node = list_get_next(&g_timers);
        node != &g_timers;
        node = list_get_next(node)) {
        task_t *task = GET_PARENT_ADDR(node, task_t, node);

        int ticks_to_wakeup = (int)(task->wakeup_ticks - current_ticks);
        if (ticks_to_wakeup >= 0 && (my_clock_t)ticks_to_wakeup > ticks)
            break;
    }

    list_attach(node, &g_current_task->node);

    if(setjmp(g_current_task->jbuf) == 0)
        run_next_task();
}

void task_msleep(uint32_t msec) {
    my_clock_t ticks = (my_clock_t)((uint64_t)msec * (1024 * (uint64_t)MY_CLOCKS_PER_SEC / 1000) / 1024);
    task_sleep_ticks(ticks);
}

static void run_next_task() {
    //get next task and run it
    list_t *next;

    while (1){
        int is_wait_empty;
        int is_timer_empty;

        timer_run();
        is_wait_empty = list_is_empty(&g_waiting_tasks);
        is_timer_empty = list_is_empty(&g_timers);
        if (is_wait_empty && is_timer_empty)
            break;
        else if(is_wait_empty) {
            //Wait for the recent timer
            timer_wait_recent();
            continue;
        }

        next = list_get_next(&g_waiting_tasks);
        g_current_task = GET_PARENT_ADDR(next, task_t, node);
        list_detach(next);

        longjmp(g_current_task->jbuf, 1);
    }

    //No more waiting task
    longjmp(g_jbuf_init, 1);
}

void task_yield() {
    if (list_is_empty(&g_waiting_tasks)) return;

    if (setjmp(g_current_task->jbuf) == 0) {
        //Put current task to the waiting list
        list_attach(&g_waiting_tasks, &g_current_task->node);
        run_next_task();
    }
}

void allocate_stack() {
    align_data_t stack[STACK_SIZE / sizeof(align_data_t)];

    if (setjmp(g_jbuf_next) == 0) {
        longjmp(g_jbuf, 1);  //Jump to the entry of created task
    }
    else
        run_task(stack /* Not use, just avoid warning */);
}

static void run_task(align_data_t *prev_task) {
    task_t task;
    (void) prev_task;

    list_init(&task.node);

    if (setjmp(g_jbuf) == 0) {
        //allocate stack for the new task
        allocate_stack();
    }
    else {
        //run the new created task
        while (1) {
            g_current_task = &task;
            (*g_entry)(g_arg);

            list_attach(&g_exit_tasks, &task.node);
            if (setjmp(task.jbuf) == 0)
                run_next_task();
            else {
                //Reuse the stack buffer
                list_detach(&task.node);
            }
        }
    }
}

void task_init_system(task_fn_t main_entry, void *arg) {
    list_init(&g_exit_tasks);
    list_init(&g_waiting_tasks);
    list_init(&g_timers);
    my_clock_init();
    if(setjmp(g_jbuf_init) == 0){
        g_entry = main_entry;
        g_arg = arg;
        run_task(NULL);
    }
}

void task_create(task_fn_t entry, void *arg) {
    g_entry = entry;
    g_arg = arg;

    if (setjmp(g_current_task->jbuf) == 0) {
        list_attach(&g_waiting_tasks, &g_current_task->node);

        if(list_is_empty(&g_exit_tasks))
            longjmp(g_jbuf_next, 1);
        else {
            list_t *node = list_get_next(&g_exit_tasks);
            task_t *task = GET_PARENT_ADDR(node, task_t, node);
            //Jump the the previous exit task, and let it reuse the stack buffer
            longjmp(task->jbuf, 1);
        }
    }
}

