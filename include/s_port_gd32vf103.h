#ifndef INC_S_PORT_H_
#define INC_S_PORT_H_

/* Copyright xhawk, MIT license */

/* Timer functions need to be implemented on a new porting. */

#undef bool
#include "gd32vf103.h"

/* 2. define the clock ticks count for one second */
#define MY_CLOCKS_PER_SEC (SystemCoreClock/4)

#include "s_port_riscv.h"

#endif

