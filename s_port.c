/* Copyright xhawk, MIT license */

/* Size for stack for each stask. */
#define STACK_SIZE 14096

/* Timer functions need to be implemented on a new porting. */

#if defined __ARMCC_VERSION
/* === Timer functions on nuvoton M051. === */
#ifdef  STM32F10X_MD
#include "stm32f10x.h"
#else
#include "M051Series.h"
#endif

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

/* sleep at most ms millseconds */
void my_sleep_ms(uint64_t ms) {
    __WFE();
}



#elif defined _WIN32
/* === Timer functions on windows === */
#include <Windows.h>
#include <time.h>

#define MY_CLOCKS_PER_SEC 1000
typedef DWORD my_clock_t;

void my_clock_init(){
}

my_clock_t my_clock() {
    return GetTickCount();
}

void my_sleep_ms(uint64_t ms) {
    Sleep((DWORD)ms);
}



#else
/* === Timer functions on linux === */
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

