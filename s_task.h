#ifndef INC_S_TASK_H_
#define INC_S_TASK_H_

/* Copyright xhawk, MIT license */

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void(*task_fn_t)(void *arg);

/* Initialize the task system. */
void task_init_system(task_fn_t main_entry, void *arg);

/* Create a new task */
void task_create(task_fn_t entry, void *arg);

/* Sleep in milliseconds */
void task_msleep(uint32_t msec);

/* Yield current task */
void task_yield(void);

#ifdef __cplusplus
}
#endif
#endif
