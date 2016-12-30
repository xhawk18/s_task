#ifndef INC_S_TASK_H_
#define INC_S_TASK_H_

/* Copyright xhawk, MIT license */

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct tag_list;
typedef struct tag_list {
    struct tag_list *next;
    struct tag_list *prev;
} list_t;

typedef struct {
    list_t wait_list;
} event_t;

/* Function type for task entrance */
typedef void(*task_fn_t)(void *arg);

/* Initialize the task system. */
void task_init_system(task_fn_t main_entry, void *arg);

/* Create a new task */
void task_create(task_fn_t entry, void *arg);

/* Sleep in milliseconds */
void task_msleep(uint32_t msec);

/* Yield current task */
void task_yield(void);

/* Initialize a wait event */
void event_init(event_t *event);

/* Wait event */
void event_wait(event_t *event);

/* Set event */
void event_set(event_t *event);

#ifdef __cplusplus
}
#endif
#endif
