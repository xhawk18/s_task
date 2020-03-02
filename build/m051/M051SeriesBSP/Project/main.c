/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2010 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include "M051Series.h"
#include "initialization.h"

#define DBG_PRINTF      printf

/*---------------------------------------------------------------------------------------------------------*/
/* MAIN function                                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
int main(void)
{
    SYS_Init();
    UART0_Init();
    UART1_Init();
    GPIO_Init();
    TMR0_Init();
    TMR1_Init();
    PWMA_Init();
    PWMB_Init();
    CKO_Init();
    ADC_Init();
    SPI0_Init();
    SPI1_Init();

    DBG_PRINTF("M051 Series Code Template\n");
    DBG_PRINTF("System Frequency: %d\n", SystemCoreClock);

    __WFI();

}




