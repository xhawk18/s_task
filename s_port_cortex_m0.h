#ifndef INC_S_PORT_CORTEX_M3_H_
#define INC_S_PORT_CORTEX_M3_H_


typedef struct {
    int regs[12];   //psp,r4-r12,lr,sp
} ucontext_t;


__asm static void swapcontext(ucontext_t *oucp, const ucontext_t *ucp) {
    MOV     r3, lr
    STM     r0!, {r3-r7}
    MOV     r2, r8
    MOV     r3, r9
    MOV     r4, r10
    MOV     r5, r11
    MOV     r6, r12
    MOV     r7, sp
    STM     r0!, {r2-r7}

    ADDS    r1, r1, #20
    LDM     r1!, {r2-r7}
    MOV     r8, r2
    MOV     r9, r3
    MOV     r10, r4
    MOV     r11, r5
    MOV     r12, r6
    MOV     sp, r7

    SUBS    r1, r1, #44
    LDM     r1!, {r3-r7}
    MOV     lr, r3

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
    PUSH    {r2, r3}
    LDR     r3, =task_helper_entry_asm  //将恢复为lr
    STM     r0!, {r3-r7}
    MOV     r2, r8
    MOV     r3, r9
    MOV     r4, r10
    MOV     r5, r11
    MOV     r6, r12
    MOV     r7, r1
    SUBS    r7, r7, #8                  //将恢复为sp
    STM     r0!, {r2-r7}

    POP     {r2, r3}
    SUBS    r1, r1, #8
    STM     r1!, {r2, r3}

    BX      lr
    NOP
}


static void createcontext(ucontext_t *oucp, void *stack, size_t stack_size, task_fn_t task_entry, void *task_arg) {
    makecontext(oucp, (char *)stack + stack_size, task_entry, task_arg);
}

#endif
