#ifndef INC_S_PORT_RISCV_H_
#define INC_S_PORT_RISCV_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int sp;
} ucontext_t;

/* Copyright xhawk, MIT license */

/* Timer functions need to be implemented on a new porting. */

/* === Timer functions on posix/linux === */

/* 1. define a type for clock */
typedef uint64_t my_clock_t;
typedef int64_t my_clock_diff_t;

/* 2. define the clock ticks count for one second */
#define MY_CLOCKS_PER_SEC (SystemCoreClock/4)

/* 3. Implement the initilization function for clock. Leave it blank if not required. */
void my_clock_init(void);

/* 4. Implement the function of getting current clock ticks. */
my_clock_t my_clock(void);

/* 5. Implement the idle delay function. */
void my_on_idle(uint64_t max_idle_ms);

/* 6. Define irq enable/disable functions */

//    #define CSR_MSTATUS 0x300
//    #define MSTATUS_MIE 0x00000008

static inline void S_IRQ_DISABLE(){
    asm volatile ("csrc 0x300, 0x8");
}

static inline void S_IRQ_ENABLE(){
    asm volatile ("csrs 0x300, 0x8");
}


#ifdef __cplusplus
}
#endif
#endif
