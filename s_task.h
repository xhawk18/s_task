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
} awaiter_t;

#   define __await__ __awaiter_dummy__
#   define __async__ awaiter_t *__awaiter_dummy__

/* Function type for task entrance */
typedef void(*task_fn_t)(__async__, void *arg);

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
    list_t wait_list;
    bool locked;
} mutex_t;

typedef struct {
    list_t wait_list;
} event_t;






/* Initialize the task system. */
void task_init_system(void);

/* Create a new task */
void task_create(void *stack, size_t stack_size, task_fn_t entry, void *arg);

/* Kill a task */
void task_kill(void *stack);

/* Sleep in milliseconds */
void task_msleep(__async__, uint32_t msec);

/* Sleep in seconds */
void task_sleep(__async__, uint32_t sec);

/* Yield current task */
void task_yield(__async__);

/* Get free stack size (for debug) */
size_t task_get_stack_free_size(void);

/* Dump task information */
void dump_tasks(__async__);

/* Initialize a mutex */
void mutex_init(mutex_t *mutex);

/* Lock the mutex */
void mutex_lock(__async__, mutex_t *mutex);

/* Unlock the mutex */
void mutex_unlock(mutex_t *mutex);

/* Initialize a wait event */
void event_init(event_t *event);

/* Wait event */
void event_wait(__async__, event_t *event);

/* Set event */
void event_set(event_t *event);

void event_wait_msec(__async__, event_t *event, uint32_t msec);
void event_wait_sec(__async__, event_t *event, uint32_t msec);

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
