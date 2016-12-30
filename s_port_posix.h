#ifndef INC_S_PORT_H_
#define INC_S_PORT_H_

/* Copyright xhawk, MIT license */

/* Size for stack for each stask. */
#define STACK_SIZE 4096

/* Timer functions need to be implemented on a new porting. */

/* === Timer functions on posix/linux === */
#include <time.h>
#define MY_CLOCKS_PER_SEC CLOCKS_PER_SEC
typedef clock_t my_clock_t;

void my_clock_init(){
}

my_clock_t my_clock() {
    return clock();
}

void my_sleep_ms(uint64_t ms) {
    usleep(ms / 1000);
}

#endif

