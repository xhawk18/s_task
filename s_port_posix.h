#ifndef INC_S_PORT_H_
#define INC_S_PORT_H_

#ifdef __cplusplus
extern "C" {
#endif


/* Copyright xhawk, MIT license */

/* Timer functions need to be implemented on a new porting. */

/* === Timer functions on posix/linux === */
#include <time.h>
#define _XOPEN_SOURCE
#include <ucontext.h>
#include <unistd.h>

//1. define a type for clock
typedef clock_t my_clock_t;

//2. define the clock ticks count for one second
#define MY_CLOCKS_PER_SEC CLOCKS_PER_SEC

//3. Implement the initilization function for clock. Leave it blank if not required.
void my_clock_init();

//4. Implement the function of getting current clock ticks.
my_clock_t my_clock();

//5. Implement the idle delay function.
void my_on_idle(uint64_t max_idle_ms);

#ifdef __cplusplus
}
#endif
#endif

