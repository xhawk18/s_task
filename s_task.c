/* Copyright xhawk, MIT license */

#include <setjmp.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "s_task.h"

void task_helper_entry(task_fn_t task_entry, void *task_arg, __async__);
static void run_next_task(__async__);

#if defined __ARMCC_VERSION
#   if defined  STM32F10X_MD
#       include "s_port_stm32f10x.h"
#   elif defined  STM32F302x8
#       include "s_port_stm32f30x.h"
#   else
#       include "s_port_m051.h"
#   endif
#elif defined STM8S103
#   include "s_port_stm8s103.h"
#elif defined _WIN32
#   include "s_port_win32.h"
#else
#   include "s_port_posix.h"
#endif

/*******************************************************************/
/* list                                                            */
/*******************************************************************/

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


/*******************************************************************/
/* tasks                                                           */
/*******************************************************************/

typedef struct {
    list_t node;
    my_clock_t wakeup_ticks;
    ucontext_t uc;
} task_t;


static list_t      g_waiting_tasks;
static task_t     *g_current_task = NULL;
static list_t      g_timers;


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
            my_on_idle((uint64_t)ticks_to_wakeup * 1000  / MY_CLOCKS_PER_SEC);
    }
}


void task_sleep_ticks(__async__, my_clock_t ticks) {
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
    run_next_task(__await__);
}

void task_msleep(__async__, uint32_t msec) {
    my_clock_t ticks = (my_clock_t)((uint64_t)msec * (1024 * (uint64_t)MY_CLOCKS_PER_SEC / 1000) / 1024);
    task_sleep_ticks(__await__, ticks);
}

static void run_next_task(__async__) {
    //get next task and run it
    list_t *next;
    task_t *old_task;

    while (1){
        int is_wait_empty;
        int is_timer_empty;

        timer_run();
        is_wait_empty = list_is_empty(&g_waiting_tasks);
        is_timer_empty = list_is_empty(&g_timers);
        if (!is_wait_empty)
            break;
        else if(!is_timer_empty) {
            //Wait for the recent timer
            timer_wait_recent();
        }else{
            //Wati CPU
            my_on_idle(0);
        }
    }

    old_task = g_current_task;
    next = list_get_next(&g_waiting_tasks);
    g_current_task = GET_PARENT_ADDR(next, task_t, node);
    list_detach(next);

    swapcontext(&old_task->uc, &g_current_task->uc);
}

void task_yield(__async__) {
    //Put current task to the waiting list
    list_attach(&g_waiting_tasks, &g_current_task->node);
    run_next_task(__await__);
}

void task_init_system(void *stack, size_t stack_size, task_fn_t main_entry, void *task_arg) {
    static task_t main_task;

    list_init(&g_waiting_tasks);
    list_init(&g_timers);
    my_clock_init();

    list_init(&main_task.node);
    g_current_task = &main_task;

    task_create(stack, stack_size, main_entry, task_arg);
}

void task_create(void *stack, size_t stack_size, task_fn_t task_entry, void *task_arg) {
    void *real_stack;
    size_t real_stack_size;

    task_t *task = (task_t *)stack;
    list_init(&task->node);
    list_attach(&g_waiting_tasks, &task->node);
    
    //填全1检查stack大小
    real_stack = (void *)&task[1];
    real_stack_size = stack_size - sizeof(task[0]);
    memset(real_stack, '\xff', real_stack_size);
    
    createcontext(&task->uc, real_stack, real_stack_size, task_entry, task_arg);
}

size_t task_get_stack_free_size() {
    task_t *task = g_current_task;
    uint32_t *check;
    for(check = (uint32_t *)&task[1]; ; ++check){
        if(*check != 0xFFFFFFFF)
            return (char *)check - (char *)&task[1];
    }
}

void task_helper_entry(task_fn_t task_entry, void *task_arg, __async__) {
    (*task_entry)(__await__, task_arg);
    run_next_task(__await__);
}


/* Initialize a wait event */
void event_init(event_t *event) {
    list_init(&event->wait_list);
}

/* Wait event */
void event_wait(__async__, event_t *event) {
    //Put current task to the event's waiting list
    list_attach(&event->wait_list, &g_current_task->node);
    run_next_task(__await__);
}

/* Set event */
void event_set(event_t *event) {
    list_attach(&g_waiting_tasks, &event->wait_list);
    list_detach(&event->wait_list);
}
