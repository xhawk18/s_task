/* Copyright xhawk, MIT license */

#include "s_port_posix.h"

/* Timer functions need to be implemented on a new porting. */

void my_clock_init(){
}

my_clock_t my_clock() {
    return clock();
}

void my_on_idle(uint64_t max_idle_ms) {
    usleep(max_idle_ms * 1000);
}

void createcontext(ucontext_t *oucp, void *stack, size_t stack_size, task_fn_t task_entry, void *task_arg) {
    getcontext(oucp);
    oucp->uc_stack.ss_sp = stack;
    oucp->uc_stack.ss_size = stack_size;
    //oucp->uc_link = &schedule_task_->ucontext_;
    makecontext(oucp, (void (*)())&task_helper_entry, 3, task_entry, task_arg, NULL);
}
