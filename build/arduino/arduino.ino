/* Copyright xhawk, MIT license */
#include <stdio.h>
#include "src/s_task/s_task.h"

//This program demonstrates three tasks:
// 1) main_task - 
//    Wait 10 seconds and set flag g_exit. 
//    After all tasks finished, set LED on always.
// 2) sub_task_fast_blinking -
//    Set led blinking fast
// 3) sub_task_set_low -
//    Set led off for 1 second, and then blinking for 3 seconds.


void setup() {
    // put your setup code here, to run once:
    pinMode(LED_BUILTIN, OUTPUT);
}


char g_stack0[384];
char g_stack1[384];
volatile bool g_is_low = false;
volatile bool g_exit = false;

void sub_task_fast_blinking(__async__, void* arg) {
    while(!g_exit) {
        if(!g_is_low)
            digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)

        s_task_msleep(__await__, 50);                       // wait for a second
        //s_task_yield(__await__);
        digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
        s_task_msleep(__await__, 50);                       // wait for a second
        //s_task_yield(__await__);
    }    
}

void sub_task_set_low(__async__, void* arg) {
    while(!g_exit) {
        g_is_low = true;
        digitalWrite(LED_BUILTIN, LOW);
        s_task_sleep(__await__, 1);
        g_is_low = false;
        s_task_sleep(__await__, 3);
    }    
}

void main_task(__async__, void* arg) {
    int i;
    s_task_create(g_stack0, sizeof(g_stack0), sub_task_fast_blinking, NULL);
    s_task_create(g_stack1, sizeof(g_stack1), sub_task_set_low, NULL);

    for (i = 0; i < 10; ++i) {
        //s_task_yield(__await__);
        s_task_sleep(__await__, 1);
    }
    g_exit = true;

    s_task_join(__await__, g_stack0);
    s_task_join(__await__, g_stack1);
}

void loop() {
    __init_async__;
    
    s_task_init_system();
    main_task(__await__, NULL);

    digitalWrite(LED_BUILTIN, HIGH);
    while(1);
}
