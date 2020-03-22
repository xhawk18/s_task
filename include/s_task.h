#ifndef INC_S_TASK_H_
#define INC_S_TASK_H_

/* Copyright xhawk, MIT license */

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "s_list.h"
#include "s_rbtree.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct{
    int dummy;
} s_awaiter_t;

#   define __await__      __awaiter_dummy__
#   define __async__      s_awaiter_t *__awaiter_dummy__
#   define __init_async__ s_awaiter_t *__awaiter_dummy__ = 0

/* Function type for task entrance */
typedef void(*s_task_fn_t)(__async__, void *arg);


//#define USE_SWAP_CONTEXT
//#define USE_JUMP_FCONTEXT
//#define USE_LIST_TIMER_CONTAINER	//for very small memory footprint


#if defined __ARMCC_VERSION
#   define USE_SWAP_CONTEXT
#   define USE_LIST_TIMER_CONTAINER
#   if defined STM32F10X_MD
#       include "s_port_stm32f10x.h"
#   elif defined STM32F302x8
#       include "s_port_stm32f30x.h"
#   elif defined STM32L1XX_MD
#       include "s_port_stm32l1xx.h"
#   else
#       include "s_port_m051.h"
#   endif
#elif defined __ICCSTM8__
#   define USE_SWAP_CONTEXT
#   define USE_LIST_TIMER_CONTAINER
#   include "s_port_stm8s.h"
#elif defined USE_LIBUV
#   define USE_JUMP_FCONTEXT
#   include "s_port_libuv.h"
#elif defined _WIN32
#   define USE_JUMP_FCONTEXT
#   include "s_port_windows.h"
#else
#   define USE_JUMP_FCONTEXT
#   include "s_port_posix.h"
#endif

typedef struct {
    s_list_t wait_list;
    bool locked;
} s_mutex_t;

typedef struct {
    s_list_t wait_list;
} s_event_t;


#include "s_task_internal.h"



/* Initialize the task system. */
#if defined USE_LIBUV
void s_task_init_system(uv_loop_t* uv_loop);
#else
void s_task_init_system(void);
#endif

/* Create a new task */
void s_task_create(void *stack, size_t stack_size, s_task_fn_t entry, void *arg);

/* Wait a task to exit */
void s_task_join(__async__, void *stack);

/* Kill a task */
void s_task_kill(void *stack);

/* Sleep in ticks */
void s_task_sleep_ticks(__async__, my_clock_t ticks);

/* Sleep in milliseconds */
void s_task_msleep(__async__, uint32_t msec);

/* Sleep in seconds */
void s_task_sleep(__async__, uint32_t sec);

/* Yield current task */
void s_task_yield(__async__);

#if defined USE_LIBUV
/* Try to run next tasks, and return wait time if none need to run */
void s_task_main_loop_once(__async__);
#endif

/* Get free stack size (for debug) */
size_t s_task_get_stack_free_size(void);

/* Dump task information */
//void dump_tasks(__async__);

/* Initialize a mutex */
void s_mutex_init(s_mutex_t *mutex);

/* Lock the mutex */
void s_mutex_lock(__async__, s_mutex_t *mutex);

/* Unlock the mutex */
void s_mutex_unlock(s_mutex_t *mutex);

/* Initialize a wait event */
void s_event_init(s_event_t *event);

/* Wait event */
void s_event_wait(__async__, s_event_t *event);

/* Set event */
void s_event_set(s_event_t *event);

/* Wait event with timeout */
void s_event_wait_msec(__async__, s_event_t *event, uint32_t msec);

/* Wait event with timeout */
void s_event_wait_sec(__async__, s_event_t *event, uint32_t msec);

/* milliseconds to ticks */
my_clock_t msec_to_ticks(uint32_t msec);
/* seconds to ticks */
my_clock_t sec_to_ticks(uint32_t sec);
/* ticks to milliseconds */
uint32_t ticks_to_msec(my_clock_t ticks);
/* ticks to seconds */
uint32_t ticks_to_sec(my_clock_t ticks);

#ifdef __cplusplus
}
#endif
#endif
