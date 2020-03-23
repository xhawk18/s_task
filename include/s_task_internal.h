#ifndef INC_S_TASK_INTERNAL_H_
#define INC_S_TASK_INTERNAL_H_

/* Copyright xhawk, MIT license */

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************/
/* s_task type definitions                                         */
/*******************************************************************/

#ifdef USE_JUMP_FCONTEXT
typedef void* fcontext_t;
typedef struct {
    fcontext_t  fctx;
    void* data;
} transfer_t;
#endif

typedef struct tag_s_task_t {
    s_list_t     node;
    s_event_t    join_event;
    s_task_fn_t  task_entry;
    void        *task_arg;
#if defined   USE_SWAP_CONTEXT
    ucontext_t   uc;
#   ifdef __APPLE__
    char dummy[512]; //it seems darwin ucontext has no enough memory ?
#   endif
#elif defined USE_JUMP_FCONTEXT
    fcontext_t   fc;
#endif
    size_t       stack_size;
    bool         closed;
} s_task_t;

typedef struct {
#ifndef USE_LIST_TIMER_CONTAINER
    RBTNode    rbt_node;
#else
    s_list_t   node;
#endif
    s_task_t  *task;
    my_clock_t wakeup_ticks;
} s_timer_t;

typedef struct {
    s_task_t *from;
    s_task_t *to;
} s_jump_t;

typedef struct {
    s_task_t    main_task;
    s_list_t    active_tasks;
    s_task_t   *current_task;

#ifndef USE_LIST_TIMER_CONTAINER
    RBTree      timers;
#else
    s_list_t    timers;
#endif

#if defined USE_LIBUV
    uv_loop_t  *uv_loop;
    uv_timer_t  uv_timer;
#endif

#if defined USE_IN_EMBEDDED    
    s_list_t         irq_active_tasks;
    volatile uint8_t irq_actived;
#endif
} s_task_globals_t;

#if defined __STDC_VERSION__ && __STDC_VERSION__ >= 201112L
#   define THREAD_LOCAL _Thread_local
#elif defined _MSC_VER
#   define THREAD_LOCAL __declspec(thread)
#elif defined __GNUC__ || defined __clang__
#   define THREAD_LOCAL __thread
#else
#   define THREAD_LOCAL
#endif
extern THREAD_LOCAL s_task_globals_t g_globals;


struct tag_s_task_t;
/* */
#ifdef USE_SWAP_CONTEXT
void s_task_context_entry(struct tag_s_task_t* task);
#endif
#ifdef USE_JUMP_FCONTEXT
void s_task_fcontext_entry(transfer_t arg);
#endif

/* Run next task, but not set myself for ready to run */
void s_task_next(__async__);

void s_timer_run(void);
uint64_t s_timer_wait_recent(void);
int s_timer_comparator(const RBTNode* a, const RBTNode* b, void* arg);

#ifdef __cplusplus
}
#endif
#endif
