/* Copyright xhawk, MIT license */

#include <setjmp.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "s_task.h"
#include "s_list.h"

void task_helper_entry(task_fn_t task_entry, void *task_arg, __async__);

/* Run next task, but not set myself for ready to run */
static void task_next(__async__);

#if defined __ARMCC_VERSION
#   if defined STM32F10X_MD
#       include "s_port_cortex_m3.inc"
#   elif defined STM32F302x8
#       include "s_port_cortex_m3.inc"
#   elif defined STM32L1XX_MD
#       include "s_port_cortex_m3.inc"
#   else
#       include "s_port_cortex_m0.inc"
#   endif
#elif defined STM8S103
#   include "s_port_stm8s103.inc"
#elif defined _WIN32
#   include "s_port_win32.inc"
#else
#   include "s_port_posix.inc"
#endif

/*******************************************************************/
/* tasks                                                           */
/*******************************************************************/

typedef struct {
    list_t node;
    ucontext_t uc;
    size_t stack_size;
} task_t;

typedef struct {
    list_t node;
    task_t *task;
    my_clock_t wakeup_ticks;
} s_timer_t;


static list_t      g_active_tasks;
static task_t     *g_current_task = NULL;
static list_t      g_timers;


/*******************************************************************/
/* timer                                                           */
/*******************************************************************/

static void timer_run() {
    my_clock_t current_ticks = my_clock();

    list_t *node = list_get_next(&g_timers);
    while (node != &g_timers) {
        //task_t *task = GET_PARENT_ADDR(node, task_t, node);
        s_timer_t *timer = GET_PARENT_ADDR(node, s_timer_t, node);

        int ticks_to_wakeup = (int)(timer->wakeup_ticks - current_ticks);
        if (ticks_to_wakeup <= 0) {
            list_t *node_next = list_get_next(node);
            list_detach(node);
            //list_attach(&g_active_tasks, node);
            
            list_detach(&timer->task->node);
            list_attach(&g_active_tasks, &timer->task->node);
            node = node_next;
        }
        else break;
    }
}

static void timer_wait_recent() {
    my_clock_t current_ticks = my_clock();
    list_t *node = list_get_next(&g_timers);
    if (node != &g_timers) {
        //task_t *task = GET_PARENT_ADDR(node, task_t, node);
        s_timer_t *timer = GET_PARENT_ADDR(node, s_timer_t, node);
        
        int ticks_to_wakeup = (int)(timer->wakeup_ticks - current_ticks);
        if (ticks_to_wakeup > 0)
            my_on_idle((uint64_t)ticks_to_wakeup * 1000  / MY_CLOCKS_PER_SEC);
    }
}


void task_sleep_ticks(__async__, my_clock_t ticks) {
    my_clock_t current_ticks;
    list_t *node;
    s_timer_t timer;
    
    current_ticks = my_clock();
    list_init(&timer.node);
    timer.task = g_current_task;
    timer.wakeup_ticks = current_ticks + ticks;
    //g_current_task->wakeup_ticks = current_ticks + ticks;

    for(node = list_get_next(&g_timers);
        node != &g_timers;
        node = list_get_next(node)) {
        s_timer_t *timer = GET_PARENT_ADDR(node, s_timer_t, node);

        int ticks_to_wakeup = (int)(timer->wakeup_ticks - current_ticks);
        if (ticks_to_wakeup >= 0 && (my_clock_t)ticks_to_wakeup > ticks)
            break;
    }

    //list_attach(node, &g_current_task->node);
    list_detach(&timer.task->node);
    list_attach(node, &timer.node);
    task_next(__await__);
    list_detach(&timer.node);
}

void task_msleep(__async__, uint32_t msec) {
    my_clock_t ticks = msec_to_ticks(msec);
    task_sleep_ticks(__await__, ticks);
}

void task_sleep(__async__, uint32_t sec) {
    my_clock_t ticks = sec_to_ticks(sec);
    task_sleep_ticks(__await__, ticks);
}

my_clock_t msec_to_ticks(uint32_t msec) {
    return (my_clock_t)((uint64_t)msec * (1024 * (uint64_t)MY_CLOCKS_PER_SEC / 1000) / 1024);
}

my_clock_t sec_to_ticks(uint32_t sec) {
    return (my_clock_t)((uint64_t)sec * (uint64_t)MY_CLOCKS_PER_SEC);
}

#define TICKS_DEVIDER   (4096*1024)
#define TICKS_PER_SEC_1 (TICKS_DEVIDER / MY_CLOCKS_PER_SEC)

uint32_t ticks_to_msec(my_clock_t ticks) {
    uint64_t u64_msec = 1000 * (uint64_t)ticks * TICKS_PER_SEC_1 / TICKS_DEVIDER;
    uint32_t msec = (u64_msec > (uint64_t)~(uint32_t)0
        ? ~(uint32_t)0 : (uint32_t)u64_msec);
    return msec;
}

uint32_t ticks_to_sec(my_clock_t ticks) {
    uint64_t u64_sec = (uint64_t)ticks * TICKS_PER_SEC_1 / TICKS_DEVIDER;
    uint32_t sec = (u64_sec > (uint64_t)~(uint32_t)0
        ? ~(uint32_t)0 : (uint32_t)u64_sec);
    return sec;
}

static void task_next(__async__) {
    //get next task and run it
    list_t *next;
    task_t *old_task;

    while (1){
        int is_wait_empty;
        int is_timer_empty;

        timer_run();
        is_wait_empty = list_is_empty(&g_active_tasks);
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
    next = list_get_next(&g_active_tasks);
    g_current_task = GET_PARENT_ADDR(next, task_t, node);
    list_detach(next);

    swapcontext(&old_task->uc, &g_current_task->uc);
}

void task_yield(__async__) {
    //Put current task to the waiting list
    list_attach(&g_active_tasks, &g_current_task->node);
    task_next(__await__);
}

void task_init_system() {
    static task_t main_task;

    list_init(&g_active_tasks);
    list_init(&g_timers);
    my_clock_init();

    list_init(&main_task.node);
    main_task.stack_size = 0;
    g_current_task = &main_task;

}

void task_create(void *stack, size_t stack_size, task_fn_t task_entry, void *task_arg) {
    void *real_stack;
    size_t real_stack_size;

    task_t *task = (task_t *)stack;
    list_init(&task->node);
    list_attach(&g_active_tasks, &task->node);
    task->stack_size = stack_size;
    
    //填全1检查stack大小
    real_stack = (void *)&task[1];
    real_stack_size = stack_size - sizeof(task[0]);
    memset(real_stack, '\xff', real_stack_size);
    ((char *)real_stack)[real_stack_size - 1] = 0;    //最后填0防止stack检查越过界
    
    createcontext(&task->uc, real_stack, real_stack_size, task_entry, task_arg);
}

//timer conflict with this function!!!
void task_kill__remove(void *stack) {
    task_t *task = (task_t *)stack;
    list_detach(&task->node);
}


static size_t task_get_stack_free_size_ex_by_stack(void *stack) {
    uint32_t *check;
    for(check = (uint32_t *)stack; ; ++check){
        if(*check != 0xFFFFFFFF)
            return (char *)check - (char *)stack;
    }
}

static size_t task_get_stack_free_size_by_task(task_t *task) {
    return task_get_stack_free_size_ex_by_stack(&task[1]);
}

size_t task_get_stack_free_size() {
    return task_get_stack_free_size_by_task(g_current_task);
}


void task_helper_entry(task_fn_t task_entry, void *task_arg, __async__) {
    (*task_entry)(__await__, task_arg);
    task_next(__await__);
}



/* Initialize a mutex */
void mutex_init(mutex_t *mutex) {
    list_init(&mutex->wait_list);
    mutex->locked = false;
}

/* Lock the mutex */
void mutex_lock(__async__, mutex_t *mutex) {
    if(mutex->locked) {
        //Put current task to the event's waiting list
        list_attach(&mutex->wait_list, &g_current_task->node);
        task_next(__await__);
    }
    else
        mutex->locked = true;
}

/* Unlock the mutex */
void mutex_unlock(mutex_t *mutex) {
    if(list_is_empty(&mutex->wait_list))
        mutex->locked = false;
    else {
        list_t *next = list_get_next(&mutex->wait_list);
        list_detach(next);
        list_attach(&g_active_tasks, next);
    }
}


/* Initialize a wait event */
void event_init(event_t *event) {
    list_init(&event->wait_list);
}

/* Wait event */
void event_wait(__async__, event_t *event) {
    //Put current task to the event's waiting list
    list_attach(&event->wait_list, &g_current_task->node);
    task_next(__await__);
}

/* Set event */
void event_set(event_t *event) {
    list_attach(&g_active_tasks, &event->wait_list);
    list_detach(&event->wait_list);
}

/* Wait event */
static void event_wait_ticks(__async__, event_t *event, my_clock_t ticks) {
    my_clock_t current_ticks;
    list_t *node;
    s_timer_t timer;
    
    current_ticks = my_clock();
    list_init(&timer.node);
    timer.task = g_current_task;
    timer.wakeup_ticks = current_ticks + ticks;
    //g_current_task->wakeup_ticks = current_ticks + ticks;

    for(node = list_get_next(&g_timers);
        node != &g_timers;
        node = list_get_next(node)) {
        s_timer_t *timer = GET_PARENT_ADDR(node, s_timer_t, node);

        int ticks_to_wakeup = (int)(timer->wakeup_ticks - current_ticks);
        if (ticks_to_wakeup >= 0 && (my_clock_t)ticks_to_wakeup > ticks)
            break;
    }

    //list_attach(node, &g_current_task->node);
    list_detach(&timer.task->node);
    list_attach(node, &timer.node);
    
    //Put current task to the event's waiting list
    list_attach(&event->wait_list, &g_current_task->node);
    task_next(__await__);
    
    list_detach(&timer.node);
}

void event_wait_msec(__async__, event_t *event, uint32_t msec) {
    my_clock_t ticks = msec_to_ticks(msec);
    event_wait_ticks(__await__, event, ticks);
}
void event_wait_sec(__async__, event_t *event, uint32_t msec) {
    my_clock_t ticks = sec_to_ticks(msec);
    event_wait_ticks(__await__, event, ticks);
}
