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

    /* Enable external 12MHz XTAL */
    SYSCLK->PWRCON |= SYSCLK_PWRCON_XTL12M_EN_Msk;

    /* Enable PLL and Set PLL frequency */        
    SYSCLK->PLLCON = PLLCON_SETTING;
    
    /* Waiting for clock ready */
    SYS_WaitingForClockReady(SYSCLK_CLKSTATUS_PLL_STB_Msk | SYSCLK_CLKSTATUS_XTL12M_STB_Msk);

    /* Switch HCLK clock source to PLL, STCLK to HCLK/2 */
    SYSCLK->CLKSEL0 = SYSCLK_CLKSEL0_STCLK_HCLK_DIV2 | SYSCLK_CLKSEL0_HCLK_PLL;

    /* Enable IP clock */        
    SYSCLK->APBCLK = SYSCLK_APBCLK_UART0_EN_Msk | SYSCLK_APBCLK_SPI0_EN_Msk |
                        SYSCLK_APBCLK_PWM01_EN_Msk | SYSCLK_APBCLK_ADC_EN_Msk;
    /* IP clock source */
    SYSCLK->CLKSEL1 = SYSCLK_CLKSEL1_UART_PLL | SYSCLK_CLKSEL1_PWM01_HCLK |
                         SYSCLK_CLKSEL1_ADC_HCLK;
    
    /* IP clock divider */
    SYSCLK->CLKDIV = SYSCLK_CLKDIV_ADC(4);   // ADC clock = HCLK / 4 = 12MHz

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
    /* Set P4.0 => PWM0, P4.1 => PWM1 */
    SYS->P4_MFP = SYS_MFP_P40_PWM0 | SYS_MFP_P41_PWM1;
    /* Set P0.0 => AD0 */
    SYS->P0_MFP = SYS_MFP_P00_AD0;

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

void PWMA_Init(void)
{

    /* PWM Timer0: clk = HCLK / 60 / 16, Freq = clk / 12500, dute cycle = 1240/1250 % */
    /* PWM Timer1: clk = HCLK / 60 / 1, Freq = clk / 1000, dute cycle = 500/1000 % */
    /* PWM0 = PLL_CLOCK / 60 / 16 / 12500 */
    /* PWM1 = PLL_CLOCK / 60 /  1 / 1000 */
    PWMA->PPR = PWM_PPR_CP01(60);
    PWMA->CSR = PWM_CSR_CSR0(PWM_CSR_DIV16) | PWM_CSR_CSR1(PWM_CSR_DIV1);

    /* Enable PWM0, PWM1 counter. We must set PWM mode before setting CNR, CMR. */
    PWMA->PCR = PWM_PCR_CH0EN_Msk | PWM_PCR_CH0MOD_AUTO_RELOAD | 
                   PWM_PCR_CH1EN_Msk | PWM_PCR_CH1MOD_AUTO_RELOAD;
    PWMA->CNR0= 12500 - 1;
    PWMA->CMR0= 12400 - 1;
    PWMA->CNR1= 1000 - 1;
    PWMA->CMR1= 500 - 1;
    /* Enable PWM0, PWM1 Output */
    PWMA->POE = PWM_POE_PWM0_Msk | PWM_POE_PWM1_Msk;

}

void ADC_Init(void)
{
    /* Set to convert ADC channel 0 */
    ADC->ADCHER = 0x1;

}


/*---------------------------------------------------------------------------------------------------------*/
/* MAIN function                                                                                           */
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
    LCD_Print(0, "ADC_PWM");
    LCD_Print(1, "Sample code");

    /* Init PWMA channel 0,1 to drive LED */    
    PWMA_Init(); 

    /* Init ADC to get the value of variable resistor */
    ADC_Init();

    while(1)
    {
        uint32_t u32AdcData, u32CMR;
        char strAdcValue[15] = "ADC Value:";
        char strClearVal[15] = "ADC Value:     ";

        /* Single end, single mode, start convert */
        ADC->ADCR = ADC_ADCR_ADEN_Msk | ADC_ADCR_ADST_Msk;

        /* Waiting for convert ready */
        while(ADC->ADCR & ADC_ADCR_ADST_Msk);
        
        /* Read the result from ADC */
        u32AdcData = (ADC->ADDR[0] & 0xFFFUL);
        printf("ADC value: %d\n", u32AdcData);
        LCD_Print(3, strClearVal);
        sprintf(strAdcValue + 10, "%d", u32AdcData);
        LCD_Print(3, strAdcValue);
        /* Adjust the duty cycle of PWM1 according to ADC value. */
        u32CMR = 1000 - u32AdcData * 1000 / 4096;
        PWMA->CMR1 = u32CMR;
    }

}




