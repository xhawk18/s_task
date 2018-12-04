#ifndef INC_S_PORT_H_
#define INC_S_PORT_H_

/* Copyright xhawk, MIT license */

/* Timer functions need to be implemented on a new porting. */

/* === Timer functions on posix/linux === */
#include <time.h>
#define _XOPEN_SOURCE
#include <ucontext.h>
#include <unistd.h>

//1. define a type for clock
typedef clock_t my_clock_t;

//2. define the clock ticks count for one second
#define MY_CLOCKS_PER_SEC CLOCKS_PER_SEC

//3. Implement the initilization function for clock. Leave it blank if not required.
void my_clock_init(){
}

//4. Implement the function of getting current clock ticks.
my_clock_t my_clock() {
    return clock();
}

//5. Implement the idle delay function.
void my_on_idle(uint64_t max_idle_ms) {
    usleep(max_idle_ms * 1000);
}

void task_main(task_fn_t task_entry, void *task_arg, __async__){
    task_helper_entry(task_entry, task_arg, __await__);
}

void createcontext(ucontext_t *oucp, void *stack, size_t stack_size, task_fn_t task_entry, void *task_arg) {
    getcontext(oucp);
    oucp->uc_stack.ss_sp = stack;
    oucp->uc_stack.ss_size = stack_size;
    //oucp->uc_link = &schedule_task_->ucontext_;
    makecontext(oucp, (void (*)())&task_main, 3, task_entry, task_arg, NULL);
}

#endif

