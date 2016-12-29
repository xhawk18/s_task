/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include "M051Series.h"
#include "LCD_Driver.h"


#define PLLCON_SETTING      SYSCLK_PLLCON_50MHz_IRC22M
#define PLL_CLOCK           50191800



void WDT_IRQHandler(void)
{

    /* Clear WDT interrupt flag */
    WDT->WTCR |= WDT_WTCR_WTIF_Msk;
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
    SYS_WaitingForClockReady(SYSCLK_CLKSTATUS_PLL_STB_Msk | SYSCLK_CLKSTATUS_XTL12M_STB_Msk  | SYSCLK_CLKSTATUS_IRC10K_STB_Msk);

    /* Use IRC22M as system clock source */
	SYSCLK->CLKSEL0 = SYSCLK_CLKSEL0_HCLK_PLL;

    /* Enable clock  of WDT, Timer0 and PWM01 */
	SYSCLK->APBCLK = SYSCLK_APBCLK_WDT_EN_Msk | SYSCLK_APBCLK_TMR0_EN_Msk | 
                        SYSCLK_APBCLK_PWM01_EN_Msk | SYSCLK_APBCLK_SPI0_EN_Msk;

    /* Enable IP clock */        
    SYSCLK->CLKSEL1 = SYSCLK_CLKSEL1_WDT_IRC10K | SYSCLK_CLKSEL1_TMR0_HCLK | 
                         SYSCLK_CLKSEL1_PWM01_HCLK;
    
    /* Update System Core Clock */
    /* User can use SystemCoreClockUpdate() to calculate PllClock, SystemCoreClock and CycylesPerUs automatically. */
    //SystemCoreClockUpdate(); 
    PllClock        = PLL_CLOCK;            // PLL
    SystemCoreClock = PLL_CLOCK / 1;        // HCLK
    CyclesPerUs     = PLL_CLOCK / 1000000;  // For SYS_SysTickDelay()

/*---------------------------------------------------------------------------------------------------------*/
/* Init I/O Multi-function                                                                                 */
/*---------------------------------------------------------------------------------------------------------*/
    /* Set P1.4, P1.5, P1.6, P1.7 for SPI0 */
    SYS->P1_MFP = SYS_MFP_P14_SPISS0 | SYS_MFP_P15_MOSI_0 | SYS_MFP_P16_MISO_0 | SYS_MFP_P17_SPICLK0;
    /* Set P4.0 => PWM0 */
    SYS->P4_MFP = SYS_MFP_P40_PWM0;

    /* Lock protected registers */
    SYS_LockReg();
}

void WDT_Init(void)
{
    /* Unlock protected registers */
    SYS_UnlockReg();
    /* start WDT. timeout = (2^14 + 1024) * (1 / 10kHz)*/
    WDT->WTCR = WDT_WTCR_WAKEUP_INT_2POW14;
    /* Lock protected registers */
    SYS_LockReg();    
	NVIC_EnableIRQ(WDT_IRQn);
}

void PWMA_Init(void)
{
    /* Enable PWM Timer 0 and set to auto-reload mode */
    PWMA->PCR = PWM_PCR_CH0EN_Msk | PWM_PCR_CH0MOD_AUTO_RELOAD;
    
    /* Set pre-scale for PWM01 */
    PWMA->PPR = PWM_PPR_CP01(2);

    /* Select the divider of PWM01 clock */
    PWMA->CSR = PWM_CSR_CSR0(PWM_CSR_DIV1);

    /* PWM0 output is HCLK / 2 / 2 */
	PWMA->CNR0 = 2 - 1;
	PWMA->CMR0 = 0;

    /* PWM output enable */
    PWMA->POE = PWM_POE_PWM0_Msk;
}

/*---------------------------------------------------------------------------------------------------------*/
/*  Main Function									                                           			   */
/*---------------------------------------------------------------------------------------------------------*/
int32_t main (void)
{
    /* Init system, IP clock and multi-function I/O */
    SYS_Init();

    /* Init SPI0 and LCD */
    LCD_Init();
    LCD_EnableBackLight();
    LCD_ClearScreen();
    LCD_Print(0, "Idle Sample Code");

/*----------------------- Enable WDT to wakeup CPU from idle ------------------*/
    WDT_Init();

/*----------------------- Enable PWM to toggle output -------------------------*/
    PWMA_Init();


    /* 
        The Idle sample uses __WFI() instruction to disable CPU clock to enter idle mode.
        In the sample code, CPU will enter idle mode to wait WDT timeout. User may check 
        LCD display to know if it is wakeup.
    */

    /* Unlock protected registers */
    SYS_UnlockReg();
	while(1)
	{
        /* Select system clock source as IRC10K */
        SYSCLK->CLKSEL0 = SYSCLK_CLKSEL0_HCLK_IRC10K;
    
        /* Update current system core clock */
        SystemCoreClockUpdate();
        
        /* Disable 22.1184MHz IRC */
    	SYSCLK->PWRCON &= ~SYSCLK_PWRCON_IRC22M_EN_Msk;

        LCD_Print(1, "Idle  ");    
        /* Idle to WDT timeout. HCLK should be 10KHz here. PWM0 = 2.5KHz*/
    	__WFI();
        LCD_Print(1, "Wakeup");    

        /* Enable 22.1184MHz IRC */
    	SYSCLK->PWRCON |= SYSCLK_PWRCON_IRC22M_EN_Msk;
        
        /* Waiting for clock ready */
        SYS_WaitingForClockReady(SYSCLK_CLKSTATUS_IRC22M_STB_Msk);
    
        /* Use IRC22M as system clock source */
    	SYSCLK->CLKSEL0 = SYSCLK_CLKSEL0_HCLK_IRC22M;
    
        /* Update System Core Clock */
        SystemCoreClockUpdate();
    
        LCD_Print(1, "Idle  ");    
        /* Idle to WDT timeout. HCLK should be 22.1184MHz here. PWM0 = 5.5MHz*/
        __WFI();
        LCD_Print(1, "Wakeup");    
	}
}




