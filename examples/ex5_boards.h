/* Copyright xhawk, MIT license */
#ifndef INC_EX_BOARDS_H_
#define INC_EX_BOARDS_H_

#include <stdio.h>
#include "s_task.h"

/* Stack and GPIO blink settings for supported boards */

#if defined __ARMCC_VERSION || (defined __GNUC__ && __USES_INITFINI__ && defined __ARM_ARCH)

/* For board STM32F10X */
#   if defined STM32F10X_MD
#       define LED_GPIO       1
#       define LED_INIT()     do {                                \
            GPIO_InitTypeDef GPIO_InitStructure;                  \
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); \
            GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;             \
            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     \
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      \
            GPIO_Init(GPIOB, &GPIO_InitStructure);                \
        } while(0)
#       define LED_SET_HIGH() do {GPIOB->BSRR |= (1 << LED_GPIO);} while(0)
#       define LED_SET_LOW()  do {GPIOB->BRR  |= (1 << LED_GPIO);} while(0)

/* For board STM32F302x8 */
#   elif defined STM32F302x8
#       error "not supported"

/* For board STM32L1XX_MD */
#   elif defined STM32L1XX_MD
#       error "not supported"

/* For board Raspberry Pico */
#   elif defined PICO_BUILD
#       define LED_GPIO 25
#       define LED_INIT()     do {                                \
            gpio_init(LED_GPIO);                                  \
            gpio_set_dir(LED_GPIO, GPIO_OUT);                     \
        } while(0)
#       define LED_SET_HIGH() gpio_put(LED_GPIO, 1)
#       define LED_SET_LOW()  gpio_put(LED_GPIO, 0);

/* For board M051 */
#   else /* M051 */
#       define LED_GPIO       6
#       define LED_INIT()     _GPIO_SET_PIN_MODE(P3, LED_GPIO, GPIO_PMD_OUTPUT)
#       define LED_SET_HIGH() do {P3->DOUT &= ~(1<<LED_GPIO);} while(0)
#       define LED_SET_LOW()  do {P3->DOUT |= (1<<LED_GPIO);} while(0)
#   endif
int g_stack0[512 / sizeof(int)];
int g_stack1[512 / sizeof(int)];

/* For board STM8S103 */
#elif defined STM8S103 
#   define LED_GPIO       GPIO_PIN_3
#   define LED_INIT()     GPIO_Init(GPIOA, LED_GPIO, GPIO_MODE_OUT_PP_LOW_FAST)
#   define LED_SET_HIGH() do {GPIOA->ODR |= LED_GPIO;} while(0)
#   define LED_SET_LOW()  do {GPIOA->ODR &= ~LED_GPIO;} while(0)
int g_stack0[350 / sizeof(int)];
int g_stack1[350 / sizeof(int)];

/* For board STM8L05X_LD_VL */
#elif defined STM8L05X_LD_VL 
#   define LED_GPIO       GPIO_Pin_0
#   define LED_INIT()     GPIO_Init(GPIOD, LED_GPIO, GPIO_Mode_Out_PP_Low_Fast)
#   define LED_SET_HIGH() do {GPIOD->ODR &= ~LED_GPIO;} while(0)
#   define LED_SET_LOW()  do {GPIOD->ODR |= LED_GPIO;} while(0)
int g_stack0[350 / sizeof(int)];
int g_stack1[350 / sizeof(int)];

/* For board atmega328p */
#elif defined __AVR__
#   define LED_GPIO       (1<<PB5)
#   define LED_INIT()     do {DDRB |= LED_GPIO;} while(0)
#   define LED_SET_HIGH() do {PORTB |= LED_GPIO;} while(0)
#   define LED_SET_LOW()  do {PORTB &= ~LED_GPIO;} while(0)
int g_stack0[384 / sizeof(int)];
int g_stack1[384 / sizeof(int)];

#else
#   error "not supported"
#endif


#endif
