/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include "M051Series.h"
#include "LCD_Driver.h"

#define PLLCON_SETTING      SYSCLK_PLLCON_50MHz_XTAL
#define PLL_CLOCK           50000000


void GPIOP0P1_IRQHandler(void)
{
    /* Re-enable debounce function */
    P1->DBEN |= GPIO_DBEN_ENABLE(3);

    /* Clear the interrupt */
    P1->ISRC = 1 << 3;

    printf("P1.3 Interrupt!\n");
    
    LCD_Print(3, "P1.3 Interrupt!");

    /* Toggle LED */
    P20 = P20 ^ 1;
}

void GPIOP2P3P4_IRQHandler(void)
{
    /* Re-enable debounce function */
    P4->DBEN |= GPIO_DBEN_ENABLE(5);

    /* Clear the interrupt */
	P4->ISRC = 1 << 5;

    printf("P2P3P4 Interrupt!\n");
    
    LCD_Print(3, "P4.5 Interrupt!");
    
    /* Toggle LED */
    P20 = P20 ^ 1;
}

void EINT0_IRQHandler(void)
{
    /* Re-enable debounce function */
    P3->DBEN |= GPIO_DBEN_ENABLE(2);

    P3->ISRC = 1 << 2;

    /* Toggle LED */
    P20 = P20 ^ 1;

    printf("EINT0 Interrupt!\n");

    LCD_Print(3, "EINT0 Interrupt!");
}

void EINT1_IRQHandler(void)
{
    /* Re-enable debounce function */
    P3->DBEN |= GPIO_DBEN_ENABLE(3);

    P3->ISRC = 1 << 3;

    /* Toggle LED */
    P20 = P20 ^ 1;

    printf("EINT1 Interrupt!\n");

    LCD_Print(3, "EINT1 Interrupt!");
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

    /* Enable external 12MHz XTAL, 10kHz */
    SYSCLK->PWRCON |= SYSCLK_PWRCON_XTL12M_EN_Msk | SYSCLK_PWRCON_IRC10K_EN_Msk;

    /* Enable PLL and Set PLL frequency */        
    SYSCLK->PLLCON = PLLCON_SETTING;
    
    /* Waiting for clock ready */
    SYS_WaitingForClockReady(SYSCLK_CLKSTATUS_PLL_STB_Msk | SYSCLK_CLKSTATUS_XTL12M_STB_Msk);

    /* Switch HCLK clock source to PLL, STCLK to HCLK/2 */
    SYSCLK->CLKSEL0 = SYSCLK_CLKSEL0_STCLK_HCLK_DIV2 | SYSCLK_CLKSEL0_HCLK_PLL;

    /* Enable IP clock */        
    SYSCLK->APBCLK = SYSCLK_APBCLK_UART0_EN_Msk | SYSCLK_APBCLK_SPI0_EN_Msk;
    /* IP clock source */
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
    /* Set P3 multi-function pins for UART0 RXD and TXD  */
    SYS->P3_MFP = SYS_MFP_P30_RXD0 | SYS_MFP_P31_TXD0;
    /* Set P1.4, P1.5, P1.6, P1.7 for SPI0 */
    SYS->P1_MFP = SYS_MFP_P14_SPISS0 | SYS_MFP_P15_MOSI_0 | SYS_MFP_P16_MISO_0 | SYS_MFP_P17_SPICLK0;

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

void GPIO_Init(void)
{
    /* Set p2.0 as output pin */
    P2->PMD = GPIO_PMD_PMD0_OUTPUT;

    /* Set p1.3, p4.5 as Quasi-bidirectional mode */
    P1->PMD = GPIO_PMD_PMD3_QUASI;
    P4->PMD = GPIO_PMD_PMD5_QUASI;

    /* Set p1.3 as falling edge trigger and enable its interrupt */
    GPIO_EnableInt(P1, 3, GPIO_INT_FALLING);
    NVIC_EnableIRQ(GPIO_P0P1_IRQn);
    
    /* Set p4.5 as low level trigger */
    GPIO_EnableInt(P4, 5, GPIO_INT_LOW);
    NVIC_EnableIRQ(GPIO_P2P3P4_IRQn);

    /* Debounce function control */
    GPIO->DBNCECON = GPIO_DBNCECON_ICLK_ON | GPIO_DBNCECON_DBCLKSRC_HCLK | GPIO_DBNCECON_DBCLKSEL_32768;
    P1->DBEN = GPIO_DBEN_ENABLE(3);
    P3->DBEN = GPIO_DBEN_ENABLE(2) | GPIO_DBEN_ENABLE(3);
    P4->DBEN = GPIO_DBEN_ENABLE(5);

    /* Configure external interrupt */

    GPIO_EnableInt(P3, 2, GPIO_INT_FALLING);
    NVIC_EnableIRQ(EINT0_IRQn);

    GPIO_EnableInt(P3, 3, GPIO_INT_BOTH_EDGE);
    NVIC_EnableIRQ(EINT1_IRQn);

}

/*---------------------------------------------------------------------------------------------------------*/
/* MAIN function                                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
int main(void)
{
    /* Init System, IP clock and multi-function I/O */
    SYS_Init();     

    /* Init UART0 for printf */
    UART0_Init();   

    printf("CPU @ %dHz\n", SystemCoreClock);
    printf("This sample code uses PWM0, PWM1 to drive LED and uses ADC to control PWM1 duty.\n");

    /* Init SPI0 and LCD */
    LCD_Init();
    LCD_EnableBackLight();
    LCD_ClearScreen();

    LCD_Print(0, "Welcome! Nuvoton");
    LCD_Print(1, "This is INT test");

    /*-----------------------------------------------------------------------------------------------------*/
    /* GPIO Interrupt Test                                                                                 */
    /*-----------------------------------------------------------------------------------------------------*/

    printf("P13, P45, P32(INT0) and P33(INT1) are used to test interrupt\n  and control LEDs(P20)\n");

    /* Init P2.0 (output), P1.3, P4.5 (Quasi-bidirectional) and relative interrupts */
    GPIO_Init();

    /* Unlock protected registers */
    SYS_UnlockReg();
    /* Settings for power down (deep sleep) */
    SYS_PowerDownInit();
    /* Lock protected registers */
    SYS_LockReg();

    /* Waiting for interrupts */
    while (1)
    {
        printf("Deep Sleep\n");

        LCD_Print(2, "Deep Sleep");
        while ((UART0->FSR & UART_FSR_TE_FLAG_Msk) == 0);

        
        /* Disable P3.2, P3.3, p1.3, p4.5 debounce to avoid double interrupts when wakeup */
        P3->DBEN = 0;
        P1->DBEN = 0;
        P4->DBEN = 0;
        
        SYS_UnlockReg();
        /* Settings for power down (deep sleep) */
        /* Prepare to enter power down */
        SYS_PowerDownInit();
        /* Lock protected registers */
        SYS_LockReg();
    
        /* Hold in wakeup state when P3.2 or P1.3 or P4.5 is low. */
        while((P32 == 0) || (P13 == 0) || (P45 == 0));

        LCD_Print(3, "                ");

        /* Enter power down. Only INT0, INT1 can used to wakeup system */
        __WFI();

    }

}




