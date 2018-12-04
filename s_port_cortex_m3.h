#ifndef INC_S_PORT_CORTEX_M3_H_
#define INC_S_PORT_CORTEX_M3_H_


typedef struct {
    int regs[12];   //psp,r4-r12,lr,sp
} ucontext_t;


__asm static void swapcontext(ucontext_t *oucp, const ucontext_t *ucp) {
    STM     r0!, {r4-r12,lr}
    STR     sp, [r0],#0x04
    
    LDM     r1!, {r4-r12,lr}
    LDR     sp, [r1],#0x04
    BX      lr
}

__asm void task_helper_entry_asm() {
    PRESERVE8
    import task_helper_entry
    POP     {r0, r1}
    LDR     r2, =task_helper_entry
    BX      r2
}


__asm static void makecontext(ucontext_t *oucp, void *stack, task_fn_t task_entry, void *task_arg) {
    STM     r0!, {r4-r12}
    
    STMDB   r1!, {r2, r3}
    
    LDR     r3, =task_helper_entry_asm
    STR     r3, [r0],#0x04    //将恢复为lr
    STR     r1, [r0],#0x04    //将恢复为sp

    BX      lr
}


static void createcontext(ucontext_t *oucp, void *stack, size_t stack_size, task_fn_t task_entry, void *task_arg) {
    makecontext(oucp, (char *)stack + stack_size, task_entry, task_arg);
}

#endif
