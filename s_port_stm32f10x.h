#ifndef INC_S_PORT_H_
#define INC_S_PORT_H_

/* Copyright xhawk, MIT license */

/* Timer functions need to be implemented on a new porting. */

/* === Timer functions on STM32F103 === */
#include "stm32f10x.h"

//1. define a type for clock
typedef uint32_t my_clock_t;

//2. define the clock ticks count for one second
#define MY_CLOCKS_PER_SEC 1000

#include "s_port_cortex_m3.h"

#endif

