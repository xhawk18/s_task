#ifndef INC_S_PORT_H_
#define INC_S_PORT_H_

/* Copyright xhawk, MIT license */

/* Size for stack for each stask. */
#define STACK_SIZE 1024

/* Timer functions need to be implemented on a new porting. */

/* === Timer functions on nuvoton M051 === */
#include "M051Series.h"

#define MY_CLOCKS_PER_SEC 1024
typedef uint32_t my_clock_t;

static my_clock_t g_ticks;
void SysTick_Handler(){
    ++g_ticks;
}

void my_clock_init(){
    SysTick_Config(SystemCoreClock / MY_CLOCKS_PER_SEC);
}

my_clock_t my_clock() {
    return g_ticks;
}

void my_on_idle(uint64_t max_idle_ms) {
    __WFE();
}

#endif

