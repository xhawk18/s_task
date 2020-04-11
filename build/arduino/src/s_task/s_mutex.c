/* Copyright xhawk, MIT license */

#include "s_task.h"

/*******************************************************************/
/* mutex                                                           */
/*******************************************************************/


/* Initialize a mutex */
void s_mutex_init(s_mutex_t *mutex) {
    s_list_init(&mutex->wait_list);
    mutex->locked = false;
}

/* Lock the mutex */
int s_mutex_lock(__async__, s_mutex_t *mutex) {
    if(mutex->locked) {
        int ret;
        /* Put current task to the event's waiting list */
        s_list_detach(&g_globals.current_task->node);   /* no need, for safe */
        s_list_attach(&mutex->wait_list, &g_globals.current_task->node);
        s_task_next(__await__);

        ret = (g_globals.current_task->waiting_cancelled ? -1 : 0);
        g_globals.current_task->waiting_cancelled = false;
        return ret;
    }
    else {
        mutex->locked = true;
        return 0;
    }
}

/* Unlock the mutex */
void s_mutex_unlock(s_mutex_t *mutex) {
    if(s_list_is_empty(&mutex->wait_list))
        mutex->locked = false;
    else {
        s_list_t *next = s_list_get_next(&mutex->wait_list);
        s_list_detach(next);
        s_list_attach(&g_globals.active_tasks, next);
    }
}


