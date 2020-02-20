/* Copyright xhawk, MIT license */

#include <setjmp.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "s_task.h"
#include "s_list.h"

struct tag_s_task_t;

void s_task_helper_entry(struct tag_s_task_t *task);

/* Run next task, but not set myself for ready to run */
static void s_task_next(__async__);

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

typedef struct tag_s_task_t {
    s_list_t     node;
    s_event_t    join_event;
    s_task_fn_t  task_entry;
    void        *task_arg;
    ucontext_t   uc;
    size_t       stack_size;
} s_task_t;

typedef struct {
    s_list_t   node;
    s_task_t  *task;
    my_clock_t wakeup_ticks;
} s_timer_t;


static s_list_t  g_active_tasks;
static s_task_t *g_current_task = NULL;
static s_list_t  g_timers;


/*******************************************************************/
/* timer                                                           */
/*******************************************************************/

static void s_timer_run() {
    my_clock_t current_ticks = my_clock();

    s_list_t *node = s_list_get_next(&g_timers);
    while (node != &g_timers) {
        //s_task_t *task = GET_PARENT_ADDR(node, s_task_t, node);
        s_timer_t *timer = GET_PARENT_ADDR(node, s_timer_t, node);

        int ticks_to_wakeup = (int)(timer->wakeup_ticks - current_ticks);
        if (ticks_to_wakeup <= 0) {
            s_list_t *node_next = s_list_get_next(node);
            s_list_detach(node);
            //s_list_attach(&g_active_tasks, node);
            
            s_list_detach(&timer->task->node);
            s_list_attach(&g_active_tasks, &timer->task->node);
            node = node_next;
        }
        else break;
    }
}

static void s_timer_wait_recent() {
    my_clock_t current_ticks = my_clock();
    s_list_t *node = s_list_get_next(&g_timers);
    if (node != &g_timers) {
        //s_task_t *task = GET_PARENT_ADDR(node, s_task_t, node);
        s_timer_t *timer = GET_PARENT_ADDR(node, s_timer_t, node);
        
        int ticks_to_wakeup = (int)(timer->wakeup_ticks - current_ticks);
        //printf("ticks_to_wakeup = %d %d %d \n", ticks_to_wakeup, (int)current_ticks, (int)timer->wakeup_ticks);
        if (ticks_to_wakeup > 0)
            my_on_idle((uint64_t)ticks_to_wakeup * 1000  / MY_CLOCKS_PER_SEC);
    }
}


void s_task_sleep_ticks(__async__, my_clock_t ticks) {
    my_clock_t current_ticks;
    s_list_t *node;
    s_timer_t timer;
    
    current_ticks = my_clock();
    s_list_init(&timer.node);
    timer.task = g_current_task;
    timer.wakeup_ticks = current_ticks + ticks;
    //g_current_task->wakeup_ticks = current_ticks + ticks;

    for(node = s_list_get_next(&g_timers);
        node != &g_timers;
        node = s_list_get_next(node)) {
        s_timer_t *timer = GET_PARENT_ADDR(node, s_timer_t, node);

        int ticks_to_wakeup = (int)(timer->wakeup_ticks - current_ticks);
        if (ticks_to_wakeup >= 0 && (my_clock_t)ticks_to_wakeup > ticks)
            break;
    }

    //s_list_attach(node, &g_current_task->node);
    s_list_detach(&timer.task->node);
    s_list_attach(node, &timer.node);
    s_task_next(__await__);
    s_list_detach(&timer.node);
}

void s_task_msleep(__async__, uint32_t msec) {
    my_clock_t ticks = msec_to_ticks(msec);
    s_task_sleep_ticks(__await__, ticks);
}

void s_task_sleep(__async__, uint32_t sec) {
    my_clock_t ticks = sec_to_ticks(sec);
    s_task_sleep_ticks(__await__, ticks);
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

static void s_task_next(__async__) {
    //get next task and run it
    s_list_t *next;
    s_task_t *old_task;

    while (1){
        int is_wait_empty;
        int is_timer_empty;

        s_timer_run();
        is_wait_empty = s_list_is_empty(&g_active_tasks);
        is_timer_empty = s_list_is_empty(&g_timers);
        
        //printf("is %d %d\n", is_timer_empty, is_wait_empty);
        if (!is_wait_empty)
            break;
        else if(!is_timer_empty) {
            //Wait for the recent timer
            s_timer_wait_recent();
        }else{
            //Wait CPU
            my_on_idle(0);
        }
    }

    old_task = g_current_task;
    next = s_list_get_next(&g_active_tasks);
    
    //printf("next = %p %p\n", g_current_task, next);
    
    g_current_task = GET_PARENT_ADDR(next, s_task_t, node);
    s_list_detach(next);

    swapcontext(&old_task->uc, &g_current_task->uc);
}

void s_task_yield(__async__) {
    //Put current task to the waiting list
    s_list_attach(&g_active_tasks, &g_current_task->node);
    s_task_next(__await__);
}

void s_task_init_system() {
    static s_task_t main_task;

    s_list_init(&g_active_tasks);
    s_list_init(&g_timers);
    my_clock_init();

    s_list_init(&main_task.node);
    s_event_init(&main_task.join_event);
    main_task.stack_size = 0;
    g_current_task = &main_task;

}

void s_task_create(void *stack, size_t stack_size, s_task_fn_t task_entry, void *task_arg) {
    void *real_stack;
    size_t real_stack_size;

    s_task_t *task = (s_task_t *)stack;
    s_list_init(&task->node);
    s_event_init(&task->join_event);
    task->task_entry = task_entry;
    task->task_arg   = task_arg;
    task->stack_size = stack_size;
    s_list_attach(&g_active_tasks, &task->node);

    //填全1检查stack大小
    real_stack = (void *)&task[1];
    real_stack_size = stack_size - sizeof(task[0]);
    memset(real_stack, '\xff', real_stack_size);
    ((char *)real_stack)[real_stack_size - 1] = 0;    //最后填0防止stack检查越过界
    
    createcontext(&task->uc, real_stack, real_stack_size, task);
}

void s_task_join(__async__, void *stack) {
    s_task_t *task = (s_task_t *)stack;
    s_event_wait(__await__, &task->join_event);
}

//timer conflict with this function!!!
//Do NOT call s_task_kill, and let the task exit by itself!
void s_task_kill__remove(void *stack) {
    s_task_t *task = (s_task_t *)stack;
    s_list_detach(&task->node);
}


static size_t s_task_get_stack_free_size_ex_by_stack(void *stack) {
    uint32_t *check;
    for(check = (uint32_t *)stack; ; ++check){
        if(*check != 0xFFFFFFFF)
            return (char *)check - (char *)stack;
    }
}

static size_t s_task_get_stack_free_size_by_task(s_task_t *task) {
    return s_task_get_stack_free_size_ex_by_stack(&task[1]);
}

size_t s_task_get_stack_free_size() {
    return s_task_get_stack_free_size_by_task(g_current_task);
}

void s_task_helper_entry(struct tag_s_task_t *task) {
    s_task_fn_t task_entry = task->task_entry;
    void *task_arg         = task->task_arg;
    
    __async__;
    (*task_entry)(__await__, task_arg);
    s_event_set(&task->join_event);
    s_task_next(__await__);
}



/* Initialize a mutex */
void s_mutex_init(s_mutex_t *mutex) {
    s_list_init(&mutex->wait_list);
    mutex->locked = false;
}

/* Lock the mutex */
void s_mutex_lock(__async__, s_mutex_t *mutex) {
    if(mutex->locked) {
        //Put current task to the event's waiting list
        s_list_attach(&mutex->wait_list, &g_current_task->node);
        s_task_next(__await__);
    }
    else
        mutex->locked = true;
}

/* Unlock the mutex */
void s_mutex_unlock(s_mutex_t *mutex) {
    if(s_list_is_empty(&mutex->wait_list))
        mutex->locked = false;
    else {
        s_list_t *next = s_list_get_next(&mutex->wait_list);
        s_list_detach(next);
        s_list_attach(&g_active_tasks, next);
    }
}


/* Initialize a wait event */
void s_event_init(s_event_t *event) {
    s_list_init(&event->wait_list);
}

/* Wait event */
void s_event_wait(__async__, s_event_t *event) {
    //Put current task to the event's waiting list
    s_list_attach(&event->wait_list, &g_current_task->node);
    s_task_next(__await__);
}

/* Set event */
void s_event_set(s_event_t *event) {
    s_list_attach(&g_active_tasks, &event->wait_list);
    s_list_detach(&event->wait_list);
}

/* Wait event */
static void s_event_wait_ticks(__async__, s_event_t *event, my_clock_t ticks) {
    my_clock_t current_ticks;
    s_list_t *node;
    s_timer_t timer;
    
    current_ticks = my_clock();
    s_list_init(&timer.node);
    timer.task = g_current_task;
    timer.wakeup_ticks = current_ticks + ticks;
    //g_current_task->wakeup_ticks = current_ticks + ticks;

    for(node = s_list_get_next(&g_timers);
        node != &g_timers;
        node = s_list_get_next(node)) {
        s_timer_t *timer = GET_PARENT_ADDR(node, s_timer_t, node);

        int ticks_to_wakeup = (int)(timer->wakeup_ticks - current_ticks);
        if (ticks_to_wakeup >= 0 && (my_clock_t)ticks_to_wakeup > ticks)
            break;
    }

    //s_list_attach(node, &g_current_task->node);
    s_list_detach(&timer.task->node);
    s_list_attach(node, &timer.node);
    
    //Put current task to the event's waiting list
    s_list_attach(&event->wait_list, &g_current_task->node);
    s_task_next(__await__);
    
    s_list_detach(&timer.node);
}

void s_event_wait_msec(__async__, s_event_t *event, uint32_t msec) {
    my_clock_t ticks = msec_to_ticks(msec);
    s_event_wait_ticks(__await__, event, ticks);
}
void s_event_wait_sec(__async__, s_event_t *event, uint32_t msec) {
    my_clock_t ticks = sec_to_ticks(msec);
    s_event_wait_ticks(__await__, event, ticks);
}
