#ifndef INC_S_TASK_H_
#define INC_S_TASK_H_

/* Copyright xhawk, MIT license */

#include <stdint.h>
#include <stddef.h>

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


typedef struct{
    int dummy;
} awaiter_t;

#   define __await__ __awaiter_dummy__
#   define __async__ awaiter_t *__awaiter_dummy__


/* Function type for task entrance */
typedef void(*task_fn_t)(__async__, void *arg);

/* Initialize the task system. */
void task_init_system(void *stack, size_t stack_size, task_fn_t main_entry, void *arg);

/* Create a new task */
void task_create(void *stack, size_t stack_size, task_fn_t entry, void *arg);

/* Sleep in milliseconds */
void task_msleep(__async__, uint32_t msec);

/* Yield current task */
void task_yield(__async__);

/* Get free stack size (for debug) */
size_t task_get_stack_free_size(void);

/* Initialize a wait event */
void event_init(event_t *event);

/* Wait event */
void event_wait(__async__, event_t *event);

/* Set event */
void event_set(event_t *event);

#ifdef __cplusplus
}
#endif
#endif
