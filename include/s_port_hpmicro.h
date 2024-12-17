#ifndef INC_S_PORT_H_
#define INC_S_PORT_H_

/* 2. define the clock ticks count for one second */
#define MY_CLOCKS_PER_SEC (24000000)

extern uint64_t get_timer_value(void);

#include "s_port_riscv.h"

#endif