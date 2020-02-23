/* Copyright xhawk, MIT license */

#include <setjmp.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "s_task.h"
#include "s_list.h"
#include "s_rbtree.h"

struct tag_s_task_t;

typedef void* fcontext_t;
typedef struct {
    fcontext_t  fctx;
    void* data;
} transfer_t;

void s_task_context_entry(struct tag_s_task_t* task);
void s_task_fcontext_entry(transfer_t arg);

/* Run next task, but not set myself for ready to run */
static void s_task_next(__async__);

//#define USE_SWAP_CONTEXT
//#define USE_JUMP_FCONTEXT


#if defined __ARMCC_VERSION
#   define USE_SWAP_CONTEXT
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
#   define USE_SWAP_CONTEXT
#   include "s_port_stm8s103.inc"
#elif defined USE_LIBUV
#   define USE_JUMP_FCONTEXT
#   include "s_port_libuv.inc"
#elif defined _WIN32
#   define USE_JUMP_FCONTEXT
#   include "s_port_windows.inc"
#else
#   define USE_JUMP_FCONTEXT
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
#if defined   USE_SWAP_CONTEXT
    ucontext_t   uc;
#elif defined USE_JUMP_FCONTEXT
	fcontext_t   fc;
#endif
    size_t       stack_size;
    bool         closed;
} s_task_t;

typedef struct {
    RBTNode    rbt_node;
    s_task_t  *task;
    my_clock_t wakeup_ticks;
} s_timer_t;

typedef struct {
    s_task_t *from;
    s_task_t *to;
} s_jump_t;


static s_list_t  g_active_tasks;
static s_task_t *g_current_task = NULL;
static RBTree    g_timers;

/*******************************************************************/
/* timer                                                           */
/*******************************************************************/


static int s_timer_comparator(const RBTNode* a, const RBTNode* b, void* arg) {
    s_timer_t* timer_a = GET_PARENT_ADDR(a, s_timer_t, rbt_node);
    s_timer_t* timer_b = GET_PARENT_ADDR(b, s_timer_t, rbt_node);

    my_clock_diff_t diff = (my_clock_diff_t)(timer_a->wakeup_ticks - timer_b->wakeup_ticks);
    if (diff != 0) {
        return (int)diff;
    }
    else {
        if (timer_a->task < timer_b->task)
            return -1;
        else if (timer_b->task < timer_a->task)
            return 1;
        else
            return 0;
    }
}

void dump_timers(int line) {
#if 0
    RBTreeIterator itr;
    RBTNode* node;
    RBTNode* node_next;
    my_clock_t current_ticks = my_clock();
    printf("=========================== %d\n", line);
    rbt_begin_iterate(&g_timers, LeftRightWalk, &itr);
    node_next = rbt_iterate(&itr);
    while ((node = node_next) != NULL) {
        s_timer_t* timer = GET_PARENT_ADDR(node, s_timer_t, rbt_node);
        printf("timer: %p %d %d\n", timer->task, (int)timer->wakeup_ticks, (int)(timer->wakeup_ticks - current_ticks));
        node_next = rbt_iterate(&itr);
    }

    s_list_t* list;
    for (list = &g_active_tasks.next; list != &g_active_tasks; list = list->next) {
        s_task_t *task = GET_PARENT_ADDR(list, s_task_t, node);
        printf("task: %p\n", task);
    }
#endif
}

static void s_timer_run(void) {
    my_clock_t current_ticks = my_clock();

    RBTreeIterator itr;
    RBTNode* node;
    RBTNode* node_next;
	
    dump_timers(__LINE__);

    rbt_begin_iterate(&g_timers, LeftRightWalk, &itr);
    node_next = rbt_iterate(&itr);
    while ((node = node_next) != NULL) {

        s_timer_t* timer = GET_PARENT_ADDR(node, s_timer_t, rbt_node);

        my_clock_diff_t ticks_to_wakeup = (my_clock_diff_t)(timer->wakeup_ticks - current_ticks);
        if (ticks_to_wakeup > 0) break;

        node_next = rbt_iterate(&itr);
        
        s_list_detach(&timer->task->node);
        s_list_attach(&g_active_tasks, &timer->task->node);

        timer->task = NULL;
        rbt_delete(&g_timers, node);
    }

    dump_timers(__LINE__);

}

static uint64_t s_timer_wait_recent(void) {
    my_clock_t current_ticks = my_clock();

    RBTreeIterator itr;
    RBTNode* node;
    rbt_begin_iterate(&g_timers, LeftRightWalk, &itr);

    if ((node = rbt_iterate(&itr)) != NULL) {
        s_timer_t* timer = GET_PARENT_ADDR(node, s_timer_t, rbt_node);

        my_clock_diff_t ticks_to_wakeup = (my_clock_diff_t)(timer->wakeup_ticks - current_ticks);
        //printf("ticks_to_wakeup = %d %d %d \n", ticks_to_wakeup, (int)current_ticks, (int)timer->wakeup_ticks);
        if (ticks_to_wakeup > 0) {
            uint64_t wait = ((uint64_t)ticks_to_wakeup * 1000 / MY_CLOCKS_PER_SEC);
            return wait;
        }
        else
            return 0;
    }
    return (uint64_t)-1;    //max
}


void s_task_sleep_ticks(__async__, my_clock_t ticks) {
    my_clock_t current_ticks;
    s_timer_t timer;
    
    current_ticks = my_clock();
    
    timer.task = g_current_task;
    timer.wakeup_ticks = current_ticks + ticks;

    dump_timers(__LINE__);

    if (!rbt_insert(&g_timers, &timer.rbt_node)) {
        fprintf(stderr, "timer insert failed!\n");
        return;
    }

    dump_timers(__LINE__);

    s_list_detach(&g_current_task->node);
    s_task_next(__await__);

    dump_timers(__LINE__);

    if (timer.task != NULL) {
        timer.task = NULL;
        rbt_delete(&g_timers, &timer.rbt_node);
    }
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

/* 
    *timeout, wait timeout
    return, true on task run
 */
static void s_task_call_next(__async__) {
    //get next task and run it
    s_list_t* next;
    s_task_t* old_task;

    //Check active tasks
    if (s_list_is_empty(&g_active_tasks)) {
        fprintf(stderr, "error: must has one task to run\n");
        return;
    }

    old_task = g_current_task;
    next = s_list_get_next(&g_active_tasks);
    
    //printf("next = %p %p\n", g_current_task, next);

    g_current_task = GET_PARENT_ADDR(next, s_task_t, node);
    s_list_detach(next);

    if (old_task != g_current_task) {
#if defined   USE_SWAP_CONTEXT
        swapcontext(&old_task->uc, &g_current_task->uc);
#elif defined USE_JUMP_FCONTEXT
        s_jump_t jump;
        jump.from = old_task;
        jump.to = g_current_task;
        transfer_t t = jump_fcontext(g_current_task->fc, (void*)&jump);
        s_jump_t* ret = (s_jump_t*)t.data;
        ret->from->fc = t.fctx;
#endif
    }
}

#if defined USE_LIBUV
static void s_task_next(__async__) {
    s_timer_run();
    s_task_call_next(__await__);
}

void s_task_main_loop_once(__async__) {
    s_timer_run();
    while (!s_list_is_empty(&g_active_tasks)) {
        //Put current task to the waiting list
        s_list_attach(&g_active_tasks, &g_current_task->node);
        s_task_call_next(__await__);
        s_timer_run();
    }

    //Check timers
    if (!rbt_is_empty(&g_timers)) {
        //Wait for the recent timer
        uint64_t timeout = s_timer_wait_recent();
        my_on_idle(timeout);
    }
}

#else

static void s_task_next(__async__) {
    while (true) {
        s_timer_run();
        if (!s_list_is_empty(&g_active_tasks)) {
            s_task_call_next(__await__);
            return;
        }

        //Check timers
        if (!rbt_is_empty(&g_timers)) {
            //Wait for the recent timer
            uint64_t timeout = s_timer_wait_recent();
            my_on_idle(timeout);
        }
        else {
            fprintf(stderr, "error: must not wait so long!\n");
            my_on_idle((uint64_t)-1);
        }
    }
}

#endif

void s_task_yield(__async__) {
    //Put current task to the waiting list
    s_list_attach(&g_active_tasks, &g_current_task->node);
    s_task_next(__await__);
}

void s_task_init_system() {
    static s_task_t main_task;

    s_list_init(&g_active_tasks);
    rbt_create(&g_timers,
        s_timer_comparator,
        NULL
    );
    my_clock_init();

    s_list_init(&main_task.node);
    s_event_init(&main_task.join_event);
    main_task.stack_size = 0;
    main_task.closed = false;
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
    task->closed = false;
    s_list_attach(&g_active_tasks, &task->node);

    //填全1检查stack大小
    real_stack = (void *)&task[1];
    real_stack_size = stack_size - sizeof(task[0]);
    memset(real_stack, '\xff', real_stack_size);
    ((char *)real_stack)[real_stack_size - 1] = 0;    //最后填0防止stack检查越过界
    

#if defined   USE_SWAP_CONTEXT
    create_context(&task->uc, real_stack, real_stack_size, task);
#elif defined USE_JUMP_FCONTEXT
    create_fcontext(&task->fc, real_stack, real_stack_size, s_task_fcontext_entry);
#endif
}

void s_task_join(__async__, void *stack) {
    s_task_t *task = (s_task_t *)stack;
    while(!task->closed)
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

void s_task_context_entry(struct tag_s_task_t *task) {
    s_task_fn_t task_entry = task->task_entry;
    void *task_arg         = task->task_arg;
    
    __async__ = 0;
    (*task_entry)(__await__, task_arg);

    task->closed = true;
    s_event_set(&task->join_event);
    s_task_next(__await__);
}


#ifdef USE_JUMP_FCONTEXT
void s_task_fcontext_entry(transfer_t arg) {
    //printf("=== s_task_helper_entry = %p\n", arg.fctx);

    s_jump_t* jump = (s_jump_t*)arg.data;
    jump->from->fc = arg.fctx;
    //printf("%p %p %p\n", jump, jump->to, g_current_task);

    s_task_context_entry(jump->to);
}
#endif


/* Initialize a mutex */
void s_mutex_init(s_mutex_t *mutex) {
    s_list_init(&mutex->wait_list);
    mutex->locked = false;
}

/* Lock the mutex */
void s_mutex_lock(__async__, s_mutex_t *mutex) {
    if(mutex->locked) {
        //Put current task to the event's waiting list
        s_list_detach(&g_current_task->node);   //no need, for safe
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
    s_list_detach(&g_current_task->node);   //no need, for safe
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
    s_timer_t timer;
    
    current_ticks = my_clock();
    timer.task = g_current_task;
    timer.wakeup_ticks = current_ticks + ticks;

    if (!rbt_insert(&g_timers, &timer.rbt_node)) {
        fprintf(stderr, "timer insert failed!\n");
        return;
    }

    s_list_detach(&g_current_task->node);   //no need, for safe
    s_list_attach(&event->wait_list, &g_current_task->node);
    s_task_next(__await__);

    if (timer.task != NULL) {
        timer.task = NULL;
        rbt_delete(&g_timers, &timer.rbt_node);
    }
}

void s_event_wait_msec(__async__, s_event_t *event, uint32_t msec) {
    my_clock_t ticks = msec_to_ticks(msec);
    s_event_wait_ticks(__await__, event, ticks);
}
void s_event_wait_sec(__async__, s_event_t *event, uint32_t msec) {
    my_clock_t ticks = sec_to_ticks(msec);
    s_event_wait_ticks(__await__, event, ticks);
}
