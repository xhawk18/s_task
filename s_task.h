#ifndef INC_S_TASK_H_
#define INC_S_TASK_H_

/* Copyright xhawk, MIT license */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "s_list.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct{
    int dummy;
} s_awaiter_t;

#   define __await__ __awaiter_dummy__
#   define __async__ s_awaiter_t *__awaiter_dummy__

/* Function type for task entrance */
typedef void(*s_task_fn_t)(__async__, void *arg);

#if defined __ARMCC_VERSION
#   if defined STM32F10X_MD
#       include "s_port_stm32f10x.h"
#   elif defined STM32F302x8
#       include "s_port_stm32f30x.h"
#   elif defined STM32L1XX_MD
#       include "s_port_stm32l1xx.h"
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

typedef struct {
    s_list_t wait_list;
    bool locked;
} s_mutex_t;

typedef struct {
    s_list_t wait_list;
} s_event_t;






/* Initialize the task system. */
void s_task_init_system(void);

/* Create a new task */
void s_task_create(void *stack, size_t stack_size, s_task_fn_t entry, void *arg);

/* Kill a task */
void s_task_kill(void *stack);

/* Sleep in milliseconds */
void s_task_msleep(__async__, uint32_t msec);

/* Sleep in seconds */
void s_task_sleep(__async__, uint32_t sec);

/* Yield current task */
void s_task_yield(__async__);

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

void s_event_wait_msec(__async__, s_event_t *event, uint32_t msec);
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
