/* Copyright xhawk, MIT license */

#include "s_port_win32.h"

void my_clock_init(){
}

my_clock_t my_clock() {
    return GetTickCount();
}

void my_on_idle(uint64_t max_idle_ms) {
    Sleep((DWORD)max_idle_ms);
}


typedef struct {
    int regs[12];   //psp,r4-r12,lr,sp
} ucontext_t;

void createcontext(ucontext_t *oucp, void *stack, size_t stack_size, task_fn_t task_entry, void *task_arg) {
}

void swapcontext(ucontext_t *oucp, const ucontext_t *ucp) {
}
