/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2010 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include "M051Series.h"
#include "LCD_Driver.h"

#define PLLCON_SETTING      SYSCLK_PLLCON_50MHz_XTAL
#define PLL_CLOCK           50000000



volatile int8_t gi8Key = 1;
char g_strBuf[32] = {0};

void EINT0_IRQHandler(void)
{
    /* Clear P3.2 interrupt flag */
    P3->ISRC = 1 << 2;

    if(P32)
    {
        /* P3.2 is from low to high */
        gi8Key = 1;
    }
    else
    {
        /* P3.2 is from high to low */
        gi8Key = 0;
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
    /* Set P1.4, P1.5, P1.6, P1.7 for SPI0 to driver LCD */
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
    /* Enable debunce function of P3.2 (EINT0) */
    P3->DBEN = GPIO_DBEN_ENABLE(2);

    /* Set debounce time. it is about 6.4 ms */
    GPIO->DBNCECON = GPIO_DBNCECON_DBCLKSRC_IRC10K | GPIO_DBNCECON_DBCLKSEL_64;

    /* Enable P3.2 to be EINT0 */
    GPIO_EnableInt(P3, 2, GPIO_INT_BOTH_EDGE);
    NVIC_EnableIRQ(EINT0_IRQn);
}


/*---------------------------------------------------------------------------------------------------------*/
/* MAIN function                                                                                           */
/*---------------------------------------------------------------------------------------------------------*/

int main(void)
{
    uint32_t u32Cnt;

    /* Init system, IP clock and multi-function I/O */
    SYS_Init();

    /* Init UART0 for printf */
    UART0_Init();

    /* Init SPI0 and LCD */
    LCD_Init();
    LCD_EnableBackLight();
    LCD_ClearScreen();
    
    printf("CPU @ %d Hz\n", SystemCoreClock);

    LCD_Print(0, "Smpl_DeepSleep");

    /*Initialize external interrupt*/
    GPIO_Init();

    /* 
        P3.2 is used as EINT0 for deep sleep (power down) control.
        Press P3.2 will toggle power down/wakeup state to show how to enter power down.
    */
    LCD_Print(1, "Press INT ");
    while (1)
    {
        char strClearVal[15] = "Count:         ";
        /* Enter power when key change from low to high */
        u32Cnt = 0;
        while (gi8Key == 1)
        {
            sprintf(g_strBuf, "Count:%d", u32Cnt++);
            LCD_Print(3, strClearVal);
            LCD_Print(3, g_strBuf);
        }
        while (gi8Key == 0)
        {
            sprintf(g_strBuf, "Count:%d", u32Cnt++);
            LCD_Print(3, strClearVal);
            LCD_Print(3, g_strBuf);
        }

        LCD_Print(2, "Deep Sleeping...");
        
        /* Unlock protected registers */
        SYS_UnlockReg();
            
        /* Prepare to enter power down */
        SYS_PowerDownInit();

        /* Lock protected registers */
        SYS_LockReg();

        /* We need to disable debounce function before power down, otherwise, there would be twice interrupt when
           wakeup */
        P3->DBEN = 0;

        /* Use WFI instruction to idle the CPU. NOTE:
           If ICE is attached, system will wakeup immediately because ICE is a wakeup event. */
        __WFI();

        /* Re-enable debounce function if necessary */
        P3->DBEN = GPIO_DBEN_ENABLE(2);

        LCD_Print(2, "Working...      ");

        /* Make sure the key is return to high before next key action */
        while (gi8Key == 0);
        
        gi8Key = 0;
    }
}

