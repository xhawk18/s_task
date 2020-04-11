
static my_clock_t g_ticks;
void SysTick_Handler(){
    ++g_ticks;
}

/* 3. Implement the initilization function for clock. Leave it blank if not required. */
void my_clock_init(){
    SysTick_Config(SystemCoreClock / MY_CLOCKS_PER_SEC);
}

/* 4. Implement the function of getting current clock ticks. */
my_clock_t my_clock() {
    return g_ticks;
}

/* 5. Implement the idle delay function. */
void my_on_idle(uint64_t max_idle_ms) {
    __WFE();
}

__asm static void swapcontext(ucontext_t *oucp, const ucontext_t *ucp) {
    STM     r0!, {r4-r12,lr}
    STR     sp, [r0]
    
    LDM     r1!, {r4-r12,lr}
    LDR     r2, [r1]
    MOV     sp, r2
    /* LDR     sp, [r1],#04 */
    /* LDR     sp, [r1]     */

    BX      lr
}


static void create_context(ucontext_t *oucp, void *stack, size_t stack_size) {
    oucp->regs[9] = (int)&s_task_context_entry;
    oucp->regs[10] = (int)((char *)stack + stack_size);
}

