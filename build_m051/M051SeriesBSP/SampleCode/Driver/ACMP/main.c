/**************************************************************************//**
 * @file     Smpl_DrvACMP.c
 * @version  V2.00
 * $Revision: 6 $
 * $Date: 8/09/12 7:11p $ 
 * @brief    M051 Series ACMP Driver Sample Code
 *
 * @note
 * Copyright (C) 2011 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#include <stdio.h>
#include "M051Series.h"

#define PLLCON_SETTING      SYSCLK_PLLCON_50MHz_XTAL
#define PLL_CLOCK           50000000



/*---------------------------------------------------------------------------------------------------------*/
/* ACMP0 IRQ Handler                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
static uint32_t s_u32Acmp0IntCnt = 0;

void ACMP0_IRQHandler(void)
{
    uint32_t u32DelayCount;
    
    /* input de-bounce */
    for(u32DelayCount=0; u32DelayCount<10000; u32DelayCount++);
    
    s_u32Acmp0IntCnt++;

    if (ACMP0->CMPSR & ACMP_CMPSR_CMPF0_Msk)
    {
        /* Check Comparator 0 Output Status */
        if (ACMP0->CMPSR & ACMP_CMPSR_CO0_Msk)
            printf("CPP0 > CPN0 (%d)\n", s_u32Acmp0IntCnt);
        else
            printf("CPP0 <= CPN0 (%d)\n", s_u32Acmp0IntCnt);
        
        /* Clear Comparator 0 Flag */
        ACMP0->CMPSR = ACMP_CMPSR_CMPF0_Msk;
    }

    if (ACMP0->CMPSR & ACMP_CMPSR_CMPF1_Msk)
    {
        /* Check Comparator 1 Output Status */
        if (ACMP0->CMPSR & ACMP_CMPSR_CO1_Msk)
            printf("CPP1 > CPN1 (%d)\n", s_u32Acmp0IntCnt);
        else
            printf("CPP1 <= CPN1 (%d)\n", s_u32Acmp0IntCnt);
        
        /* Clear Comparator 1 Flag */
        ACMP0->CMPSR = ACMP_CMPSR_CMPF1_Msk;
    }
}

void SYS_Init(void)
{
/*---------------------------------------------------------------------------------------------------------*/
/* Init System Clock                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Enable Internal RC clock */
    SYSCLK->PWRCON |= SYSCLK_PWRCON_IRC22M_EN_Msk;

    /* Waiting for IRC22M clock ready */
    SYS_WaitingForClockReady(SYSCLK_CLKSTATUS_IRC22M_STB_Msk);

    /* Switch HCLK clock source to internal RC */
    SYSCLK->CLKSEL0 = SYSCLK_CLKSEL0_HCLK_IRC22M;

    /* Set PLL to power down mode and PLL_STB bit in CLKSTATUS register will be cleared by hardware.*/
    SYSCLK->PLLCON |= SYSCLK_PLLCON_PD_Msk;

    /* Enable external 12MHz XTAL, internal 22.1184MHz */
    SYSCLK->PWRCON |= SYSCLK_PWRCON_XTL12M_EN_Msk | SYSCLK_PWRCON_IRC22M_EN_Msk;

    /* Enable PLL and Set PLL frequency */
    SYSCLK->PLLCON = PLLCON_SETTING;
    
    /* Waiting for clock ready */
    SYS_WaitingForClockReady(SYSCLK_CLKSTATUS_PLL_STB_Msk | SYSCLK_CLKSTATUS_XTL12M_STB_Msk  |
                             SYSCLK_CLKSTATUS_IRC22M_STB_Msk);

    /* Switch HCLK clock source to PLL, STCLK to HCLK/2 */
    SYSCLK->CLKSEL0 = SYSCLK_CLKSEL0_STCLK_HCLK_DIV2 | SYSCLK_CLKSEL0_HCLK_PLL;

    /* Enable IP clock */        
    SYSCLK->APBCLK = SYSCLK_APBCLK_UART0_EN_Msk | SYSCLK_APBCLK_ACMP0_EN_Msk;
    SYSCLK->CLKSEL1 = SYSCLK_CLKSEL1_UART_PLL;

    /* Update System Core Clock */
    /* User can use SystemCoreClockUpdate() to calculate PllClock, SystemCoreClock and CycylesPerUs automatically. */
    //SystemCoreClockUpdate(); 
    PllClock        = PLL_CLOCK;            // PLL
    SystemCoreClock = PLL_CLOCK / 1;        // HCLK
    CyclesPerUs     = PLL_CLOCK / 1000000;  // For SYS_SysTickDelay()

/*---------------------------------------------------------------------------------------------------------*/
/* Init I/O Multi-function                                                                                 */
/*---------------------------------------------------------------------------------------------------------*/
    /* Set P0 multi-function pins for UART0 RXD and TXD */
    SYS->P0_MFP = SYS_MFP_P02_TXD0 | SYS_MFP_P03_RXD0;

    /* Set P1 multi-function pins for ACMP0 CPP0 */
    SYS->P1_MFP = SYS_MFP_P15_CPP0;

    /* Set P2 multi-function pins for ACMP0 CPO1 */
    SYS->P2_MFP = SYS_MFP_P26_CPO1;

    /* Set P3 multi-function pins for ACMP0 CPP1 and CPO0 */
    SYS->P3_MFP = SYS_MFP_P31_CPP1 | SYS_MFP_P36_CPO0;

    /* Lock protected registers */
    SYS_LockReg();
}

void UART0_Init(void)
{
/*---------------------------------------------------------------------------------------------------------*/
/* Init UART                                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
    UART0->BAUD = UART_BAUD_MODE2 | UART_BAUD_DIV_MODE2(PLL_CLOCK, 115200);
    _UART_SET_DATA_FORMAT(UART0, UART_WORD_LEN_8 | UART_PARITY_NONE | UART_STOP_BIT_1);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Main function                                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
/* Note:  In this sample code, the software configure P0.2 as UART0 TX pin and P0.3 as UART0 RX pin.     */
int32_t main()
{
    /* Init System, IP clock and multi-function I/O */
    SYS_Init();     

    /* Init UART0 for printf */
    UART0_Init();

    printf("\n");
    printf("+----------------------------------------------------------------+\n");
    printf("|                     ACMP Sample Code                           |\n");           
    printf("+----------------------------------------------------------------+\n");
    printf("  P1.5 (CPP0): ACMP0 comparator 0 positive input\n");
    printf("  P3.1 (CPP1): ACMP0 comparator 1 positive input\n");
    printf("  The internal bandgap voltage is selected as the negative input of both comparators (CPN0 & CPN1).\n");
    printf("  P3.6 (CPO0): ACMP0 comparator 0 output\n");
    printf("  P2.6 (CPO1): ACMP0 comparator 1 output\n");
    printf("When the level of the positive input is greater than the level of the negative input,\n");
    printf("the analog comparator outputs a logical one; otherwise, it outputs a logical zero.\n");
    printf("This sample code will show the expression of the comparator's inputs and a sequence "); 
    printf("number when detecting a transition of analog comparator's output.\n");
    printf("Press any key to start ...");
    getchar();
    printf("\n");

    /* Disable digital input path of P1.5 (CPP0) and P3.1 (CPP1) to avoid the leakage current */
    _GPIO_DISABLE_DIGITAL_PATH(P1, 5);
    _GPIO_DISABLE_DIGITAL_PATH(P3, 1);

    /* Configure ACMP0 Comparator 0 */
    ACMP0->CMPCR[0] = ACMP_CMPCR_CMPEN_Msk | ACMP_CMPCR_CMPIE_Msk | ACMP_CMPCR_CMPCN_Msk;
    
    /* Configure ACMP0 Comparator 1 */
    ACMP0->CMPCR[1] = ACMP_CMPCR_CMPEN_Msk | ACMP_CMPCR_CMPIE_Msk | ACMP_CMPCR_CMPCN_Msk;

    /* Enable NVIC bit for ACMP0 */
    NVIC_EnableIRQ(ACMP0_IRQn);

    while(1);
}   




