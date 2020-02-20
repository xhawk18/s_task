
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

typedef struct {
    int regs[12];   //psp,r4-r12,lr,sp
} ucontext_t;


__asm static void swapcontext(ucontext_t *oucp, const ucontext_t *ucp) {
    STM     r0!, {r4-r12,lr}
    STR     sp, [r0]
    
    LDM     r1!, {r4-r12,lr}
    LDR     r2, [r1]
    MOV     sp, r2
    //LDR     sp, [r1],#04
	//LDR     sp, [r1]

    BX      lr
}

__asm void task_helper_entry_asm() {
    PRESERVE8
    import s_task_helper_entry
    POP     {r0, r1}
    LDR     r2, =s_task_helper_entry
    BX      r2
}

//dummy_arg is not used now
__asm static void makecontext(ucontext_t *oucp, void *stack, void *task, void *dummy_arg) {
    STM     r0!, {r4-r12}
    
    STMDB   r1!, {r2, r3}
    
    LDR     r3, =task_helper_entry_asm
    STR     r3, [r0],#0x04    //将恢复为lr
    STR     r1, [r0],#0x04    //将恢复为sp

    BX      lr
}


static void createcontext(ucontext_t *oucp, void *stack, size_t stack_size, void *task) {
    makecontext(oucp, (char *)stack + stack_size, task, NULL);
}

