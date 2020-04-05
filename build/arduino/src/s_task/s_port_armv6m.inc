
static my_clock_t g_ticks;
void SysTick_Handler(){
    ++g_ticks;
}

//3. Implement the initilization function for clock. Leave it blank if not required.
void my_clock_init(){
    SysTick_Config(SystemCoreClock / MY_CLOCKS_PER_SEC);
}

//4. Implement the function of getting current clock ticks.
my_clock_t my_clock() {
    return g_ticks;
}

//5. Implement the idle delay function.
void my_on_idle(uint64_t max_idle_ms) {
    __WFE();
}


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


__asm static void makecontext(ucontext_t *oucp, void *stack) {
    PRESERVE8
    import s_task_context_entry
    LDR     r3, =s_task_context_entry  //将恢复为lr
    STM     r0!, {r3-r7}
    MOV     r2, r8
    MOV     r3, r9
    MOV     r4, r10
    MOV     r5, r11
    MOV     r6, r12
    MOV     r7, r1
    SUBS    r7, r7, #8                  //将恢复为sp
    STM     r0!, {r2-r7}

    SUBS    r1, r1, #8
    STM     r1!, {r2, r3}

    BX      lr
}


static void create_context(ucontext_t *oucp, void *stack, size_t stack_size) {
    oucp->regs[0] = (int)&s_task_context_entry;
    oucp->regs[10] = (int)((char *)stack + stack_size);
}

