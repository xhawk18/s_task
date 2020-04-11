/* Copyright xhawk, MIT license */
#include <stdio.h>
#include "s_task.h"

/*
This program demonstrates three tasks:
 1) main_task - 
    Wait 10 seconds and set flag g_exit. 
    After all tasks finished, set LED on always.
 2) sub_task_fast_blinking -
    Set led blinking fast
 3) sub_task_set_low -
    Set led off for 1 second, and then blinking for 3 seconds.
*/

#if defined STM8S103 
#   define LED_GPIO       GPIO_PIN_3
#   define LED_INIT()     GPIO_Init(GPIOA, LED_GPIO, GPIO_MODE_OUT_PP_LOW_FAST)
#   define LED_SET_HIGH() do {GPIOA->ODR |= LED_GPIO;} while(0)
#   define LED_SET_LOW()  do {GPIOA->ODR &= ~LED_GPIO;} while(0)
char g_stack0[350];
char g_stack1[350];

#elif defined STM8L05X_LD_VL 
#   define LED_GPIO       GPIO_Pin_0
#   define LED_INIT()     GPIO_Init(GPIOD, LED_GPIO, GPIO_Mode_Out_PP_Low_Fast)
#   define LED_SET_HIGH() do {GPIOD->ODR &= ~LED_GPIO;} while(0)
#   define LED_SET_LOW()  do {GPIOD->ODR |= LED_GPIO;} while(0)
char g_stack0[350];
char g_stack1[350];

#elif defined __AVR__
#   define LED_GPIO       (1<<PB5)
#   define LED_INIT()     do {DDRB |= LED_GPIO;} while(0)
#   define LED_SET_HIGH() do {PORTB |= LED_GPIO;} while(0)
#   define LED_SET_LOW()  do {PORTB &= ~LED_GPIO;} while(0)
char g_stack0[384];
char g_stack1[384];

#else
#   error "not supported"
#endif

volatile bool g_is_low = false;
volatile bool g_exit = false;

void sub_task_fast_blinking(__async__, void* arg) {
    while(!g_exit) {
        if(!g_is_low)
            LED_SET_HIGH();                  /* turn the LED on */

        s_task_msleep(__await__, 50);        /* wait for 50 milliseconds */
        LED_SET_LOW();                       /* turn the LED off */
        s_task_msleep(__await__, 50);        /* wait for 50 milliseconds */
    }    
}

void sub_task_set_low(__async__, void* arg) {
    while(!g_exit) {
        g_is_low = true;                     /* stop fast blinking */
        LED_SET_LOW();                       /* turn the LED off */
        s_task_sleep(__await__, 1);          /* wait for 1 second */
        g_is_low = false;                    /* start fast blinking */
        s_task_sleep(__await__, 3);          /* wait for 3 seconds */
    }    
}

void main_task(__async__, void* arg) {
    
    LED_INIT();

    /* create two sub tasks */
    s_task_create(g_stack0, sizeof(g_stack0), sub_task_fast_blinking, NULL);
    s_task_create(g_stack1, sizeof(g_stack1), sub_task_set_low, NULL);

    /* wait for 10 seconds */
    s_task_sleep(__await__, 10);

    g_exit = true;

    /* wait two sub tasks return */
    s_task_join(__await__, g_stack0);
    s_task_join(__await__, g_stack1);
}

int main() {
    __init_async__;
    
    s_task_init_system();
    main_task(__await__, NULL);

    /* turn the LED on always */
    LED_SET_HIGH();
    return 0;
}
