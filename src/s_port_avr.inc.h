// The context swapping code from
//    https://github.com/LeeReindeer/ROS
// Thanks LeeReindeer for the great work!

#include <avr/interrupt.h>
#include <avr/io.h>

/* Timer functions need to be implemented on a new porting. */

void my_clock_init(){
}

my_clock_t my_clock() {
    return 0;
}

void my_on_idle(uint64_t max_idle_ms) {
}

static void create_context(ucontext_t *uc, void *stack, size_t stack_size) {
    uint8_t *stack_top = (uint8_t *)stack + stack_size;
    // pc
    // the function pointer is uint16_t in avr
#if 0
    *--stack_top = (uint8_t)((uint16_t)s_task_context_entry & 0xFF);         // the LSB
    *--stack_top = (uint8_t)(((uint16_t)s_task_context_entry >> 8) & 0xFF);  // THE MSB
    // Make space for R2-R17, R28-R29
    *--stack_top = 0x00; // R2
    *--stack_top = 0x00; // R3
    *--stack_top = 0x00; // R4
    *--stack_top = 0x00; // R5
    *--stack_top = 0x00; // R6
    
    *--stack_top = 0x00; // R7
    *--stack_top = 0x00; // R8
    *--stack_top = 0x00; // R9
    *--stack_top = 0x00; // R10
    *--stack_top = 0x00; // R11
    
    *--stack_top = 0x00; // R12
    *--stack_top = 0x00; // R13
    *--stack_top = 0x00; // R14
    *--stack_top = 0x00; // R15
    *--stack_top = 0x00; // R16
    
    *--stack_top = 0x00; // R17
    *--stack_top = 0x00; // R28
    *--stack_top = 0x00; // R29
    uc->sp = --stack_top;
#else
    stack_top[-1] = (uint8_t)((uint16_t)s_task_context_entry & 0xFF);         // the LSB
    stack_top[-2] = (uint8_t)(((uint16_t)s_task_context_entry >> 8) & 0xFF);  // THE MSB
    uc->sp = (int)(stack_top - 21);
#endif
}

static void swapcontext(ucontext_t *old_tcb, const ucontext_t *new_tcb) {
  // The assembly code is in intel style, source is always on the right
  // Y-reg is R28 and R29
  __asm__ __volatile__(
      "push r2\n\t"
      "push r3\n\t"
      "push r4\n\t"
      "push r5\n\t"
      "push r6\n\t"
      "push r7\n\t"
      "push r8\n\t"
      "push r9\n\t"
      "push r10\n\t"
      "push r11\n\t"
      "push r12\n\t"
      "push r13\n\t"
      "push r14\n\t"
      "push r15\n\t"
      "push r16\n\t"
      "push r17\n\t"
      "push r28\n\t"
      "push r29\n\t"
      // r16, r17, r28 and r29 is saved, we're safe to use them
      "mov r28, %A[_old_tcb_]\n\t" // move old tcb(LSB) to Y-regs
      "mov r29, %B[_old_tcb_]\n\t" // MSB
      "sbiw r28, 0\n\t"        // subract 0 from r29:r28, we need this to set SREG-Z if result is zero
      "breq restore\n\t"           // if old_tcb is NULL, jump to restore
      "in r16, %[_SPL_]\n\t"       // get stack pointer to r17:r16
      "in r17, %[_SPH_]\n\t"
      "st Y, r16\n\t"              // set old_tcb->sp to stack pointer
      "std Y+1, r17\n\t"           // because sp is the first member of the TCB struct
      "restore:"
      "mov r28, %A[_new_tcb_]\n\t"
      "mov r29, %B[_new_tcb_]\n\t"
      "ld r16, Y\n\t"              //load new_tcb->sp to r17:r16
      "ldd r17, Y+1\n\t"
      "out %[_SPL_], r16\n\t"      //change the stack pointer to new_tcb->sp
      "out %[_SPH_], r17\n\t"
      "pop r29\n\t"                // restore new_tcb's context
      "pop r28\n\t"
      "pop r17\n\t"
      "pop r16\n\t"
      "pop r15\n\t"
      "pop r14\n\t"
      "pop r13\n\t"
      "pop r12\n\t"
      "pop r11\n\t"
      "pop r10\n\t"
      "pop r9\n\t"
      "pop r8\n\t"
      "pop r7\n\t"
      "pop r6\n\t"
      "pop r5\n\t"
      "pop r4\n\t"
      "pop r3\n\t"
      "pop r2\n\t"
      "ret\n\t"
      "" ::
      [_SPL_] "i" _SFR_IO_ADDR(SPL),
      [_SPH_] "i" _SFR_IO_ADDR(SPH),
      [_old_tcb_] "r"(old_tcb),
      [_new_tcb_] "r" (new_tcb)
  );
}
