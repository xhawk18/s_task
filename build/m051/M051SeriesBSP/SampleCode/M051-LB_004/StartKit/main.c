/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2010 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include "M051Series.h"
#include "LCD_Driver.h"
#include "EEPROM_24LC64.h"
#include "SPIFLASH_Driver.h"

#define PLLCON_SETTING      SYSCLK_PLLCON_50MHz_XTAL
#define PLL_CLOCK           50000000

void Delay(int32_t ms)
{
    int32_t i;

    for(i=0;i<ms;i++)
        SYS_SysTickDelay(1000);
}

void EINT0_IRQHandler(void)
{
    P3->ISRC = 1 << 2;

    /* Toggle PWM3 enable/disable */
    PWMA->POE |= PWM_POE_PWM3_Msk;

    printf("EINT0 Interrupt!\n");

}

void TMR0_IRQHandler(void)
{
    /* Clear Interrupt */
    TIMER0->TISR = 1;    

}


void PWM_Init(void)
{

    /* PWM Timer0: Clk = HCLK / 120 / 16, Freq = clk / 6250, dute cycle = 3125/6250 % */
    /* PWM Timer1: Clk = HCLK / 120 / 16, Freq = clk / 3125, dute cycle = 1563/3125 % */
    /* PWM Timer2: Clk = HCLK / 60 / 16, Freq = clk / 3125, dute cycle = 1563/3125 % */
    /* PWM Timer3: Clk = HCLK / 60 / 1, Freq = clk / 50, dute cycle = 25/50 % */
    /* PWM0 = 12000000 / 120 / 16 / 6250 =    1Hz */
    /* PWM1 = 12000000 / 120 / 16 / 3125 =    2Hz */
    /* PWM2 = 12000000 /  60 / 16 / 3125 =    4Hz */
    /* PWM3 = 12000000 /  60 /  1 /   50 = 4000Hz */
    PWMA->PPR = PWM_PPR_CP01(120) | PWM_PPR_CP23(60);
    PWMA->CSR = PWM_CSR_CSR0(PWM_CSR_DIV16) | PWM_CSR_CSR1(PWM_CSR_DIV16) |
                   PWM_CSR_CSR2(PWM_CSR_DIV16) | PWM_CSR_CSR3(PWM_CSR_DIV1);

    /* Enable PWM0, PWM1, PWM2, PWM3 counter. We must set PWM mode before setting CNR, CMR. */
    PWMA->PCR = PWM_PCR_CH0EN_Msk | PWM_PCR_CH0MOD_AUTO_RELOAD | 
                   PWM_PCR_CH1EN_Msk | PWM_PCR_CH1MOD_AUTO_RELOAD |
                   PWM_PCR_CH2EN_Msk | PWM_PCR_CH2MOD_AUTO_RELOAD |
                   PWM_PCR_CH3EN_Msk | PWM_PCR_CH3MOD_AUTO_RELOAD;

    PWMA->CNR0= 6250 - 1;
    PWMA->CMR0= 3125 - 1;
    PWMA->CNR1= 3125 - 1;
    PWMA->CMR1= 1563 - 1;
    PWMA->CNR2= 3125 - 1;
    PWMA->CMR2= 1563 - 1;
    PWMA->CNR3= 50 - 1;
    PWMA->CMR3= 25 - 1;

    /* Enable PWM0, PWM1, PWM2, PWM3 Output */
    PWMA->POE = PWM_POE_PWM0_Msk | PWM_POE_PWM1_Msk | PWM_POE_PWM2_Msk | PWM_POE_PWM3_Msk;
}

void TMR0_Init(void)
{
    TIMER0->TCSR = TIMER_TCSR_INIT_IE(12);
    TIMER0->TCMPR = 100000; // 12MHz / 12 / 100000 = 10Hz

    NVIC_EnableIRQ(TMR0_IRQn);
}

void EINT0_Init(void)
{
    /* Debounce function control */
    GPIO->DBNCECON = GPIO_DBNCECON_ICLK_ON | GPIO_DBNCECON_DBCLKSRC_HCLK | GPIO_DBNCECON_DBCLKSEL_32768;
    P3->DBEN = GPIO_DBEN_ENABLE(2);

    /* Configure external interrupt */
    GPIO_EnableInt(P3, 2, GPIO_INT_FALLING);
    NVIC_EnableIRQ(EINT0_IRQn);
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

    /* Switch HCLK clock source to PLL, STCLK to HCLK/2 */
    SYSCLK->CLKSEL0 = SYSCLK_CLKSEL0_STCLK_HCLK_DIV2 | SYSCLK_CLKSEL0_HCLK_PLL;

    /* Enable IP clock */        
    SYSCLK->APBCLK = SYSCLK_APBCLK_UART0_EN_Msk | SYSCLK_APBCLK_SPI0_EN_Msk |
                        SYSCLK_APBCLK_SPI1_EN_Msk | SYSCLK_APBCLK_ADC_EN_Msk |
                        SYSCLK_APBCLK_I2C_EN_Msk | SYSCLK_APBCLK_PWM01_EN_Msk |
                        SYSCLK_APBCLK_PWM23_EN_Msk | SYSCLK_APBCLK_TMR0_EN_Msk; 
    /* IP clock source */
    SYSCLK->CLKSEL1 = SYSCLK_CLKSEL1_UART_PLL | SYSCLK_CLKSEL1_ADC_HCLK |
                         SYSCLK_CLKSEL1_PWM01_XTAL | SYSCLK_CLKSEL1_PWM23_XTAL |
                         SYSCLK_CLKSEL1_TMR0_XTAL;
    
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
    /* Set P3 multi-function pins for UART0 RXD and TXD. Set P3.4 P3.5 for I2C  */
    SYS->P3_MFP = SYS_MFP_P30_RXD0 | SYS_MFP_P31_TXD0 | 
                     SYS_MFP_P34_SDA0 | SYS_MFP_P35_SCL0;
    /* Set P1.4, P1.5, P1.6, P1.7 for SPI0 */
    SYS->P1_MFP = SYS_MFP_P14_SPISS0 | SYS_MFP_P15_MOSI_0 | SYS_MFP_P16_MISO_0 | SYS_MFP_P17_SPICLK0;
    /* Set P0.4, P0.5, P0.6, P0.7 for SPI1. Set P0.0 for AD0 */
    SYS->P0_MFP = SYS_MFP_P04_SPISS1 | SYS_MFP_P05_MOSI_1 | SYS_MFP_P06_MISO_1 | SYS_MFP_P07_SPICLK1 |
                     SYS_MFP_P00_AD0;
    /* Set P4.0, P4.1 for PWM0, PWM1, PWM2, PWM3. Set P4.6, P4.7 for ICE */
    SYS->P4_MFP = SYS_MFP_P40_PWM0 | SYS_MFP_P41_PWM1 | SYS_MFP_P42_PWM2 | SYS_MFP_P43_PWM3 |
                     SYS_MFP_P46_ICE_CLK | SYS_MFP_P47_ICE_DAT;


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
/* MAIN function                                                                                           */
/*---------------------------------------------------------------------------------------------------------*/

int main(void)
{
    uint32_t u32Counter = 0, u32AdcData = 0, u32I2cData, u32LedOn = 0;
    uint32_t u32Id;
    int32_t i32BuzzerCnt;
    char AdcValue[15] = "ADC Value:";
    char TestOk[15] = "Test OK:";
    char strClearAdcValue[15] = "ADC Value:     ";

    /* Init System, IP clock and multi-function I/O */
    SYS_Init();     

    /* Init UART0 for printf */
    UART0_Init();   

/*---------------------------------------------------------------------------------------------------------*/
/* Init Other peripherals                                                                                  */
/*---------------------------------------------------------------------------------------------------------*/
    LCD_Init();
    SPIFLASH_Init();
    EEPROM_Init();
    PWM_Init();
    TMR0_Init();
    EINT0_Init();


/*
    Smpl_StartKit is used to demo Nu-LB_004 learning board. This sample will test EEPROM, SPI Flash read/write by
    I2C, SPI. And test LED by GPIO and PWM. Test KEY by EINT0. Use ADC to convert the voltage of 
    variable resistor by AD0. Generate 4kHz key sound by buzzer with PWM. LCD display is enabled to show working
    message. CPU will be idle by __WFI() when a loop has been done. Timer events (every 100ms) or key events are
    used to wakeup CPU to excute next loop.
*/

    LCD_EnableBackLight();
    LCD_ClearScreen();

    printf("CPU @ %dHz\n", SystemCoreClock);

    // SPI0 test
    LCD_Print(0, "Welcome! Nuvoton");
    LCD_Print(1, "This is LB test ");

    i32BuzzerCnt = 1;
    while (1)
    {
        /* Key sound */
        if(PWMA->POE & PWM_POE_PWM3_Msk)
        {
            if(i32BuzzerCnt-- <= 0)
            {            
                /* Disable Buzzer */
                PWMA->POE &= ~PWM_POE_PWM3_Msk;
            }
        }
        else
            i32BuzzerCnt = 1;

        printf("\nTest time: %d\n", u32Counter++);

        /* LED test */
        if (u32Counter % 8 == 0)
            u32LedOn = 0x01;
        P2->DOUT = ~u32LedOn;
        u32LedOn <<= 1;

        /* Check ADC is idle */
        if((ADC->ADCR & ADC_ADCR_ADST_Msk) == 0)
        {
            /* Read the result from last ADC convert */
            u32AdcData = (ADC->ADDR[0] & 0xFFFUL);
            printf("ADC value: %d\n", u32AdcData);
            sprintf(AdcValue + 10, "%d", u32AdcData);        
            LCD_Print(3, strClearAdcValue);
            LCD_Print(3, AdcValue);

            /* Single end, single mode, start convert */
            ADC->ADCR = ADC_ADCR_ADEN_Msk | ADC_ADCR_ADST_Msk;
        }

        /*I2C test*/
        u32AdcData = (u32Counter + u32AdcData) & 0xff;
        EEPROM_Write(u32Counter, u32AdcData);
        u32I2cData = EEPROM_Read(u32Counter);
        if (u32I2cData != u32AdcData)
        {
            LCD_Print(2, "I2C fail ");
            while (1);
        }
        else
            printf("I2C address:0x%x, Data:0x%x\n", u32Counter, u32I2cData);

        /*SPI1 test*/
        u32Id = SPIFLASH_ReadId() & 0xFFFFUL;
        if(u32Id != 0xEF14)  /*Get flash ID*/
        {
            LCD_Print(2, "SPI fail ");
            while (1);
        }

        //Test OK
        sprintf(TestOk + 8, "%d", u32Counter);
        LCD_Print(2, TestOk);

        /* Waiting for events */
        __WFI();
    }
}




