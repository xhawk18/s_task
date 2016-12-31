#ifndef INC_S_PORT_H_
#define INC_S_PORT_H_

/* Copyright xhawk, MIT license */

/* Size for stack for each stask. */
#define STACK_SIZE 1024

/* The original jmp_buf is too large and waste RAM, Use smaller jmb_buf instead! */
typedef struct {
    int buf[44 / sizeof(int)];
} my_jmp_buf_t;
#define MY_JMP_BUF my_jmp_buf_t

/* Timer functions need to be implemented on a new porting. */

/* === Timer functions on STM32F302 === */
#include "stm32f30x.h"

//1. define a type for clock
typedef uint32_t my_clock_t;

//2. define the clock ticks count for one second
#define MY_CLOCKS_PER_SEC 1024


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

#endif

