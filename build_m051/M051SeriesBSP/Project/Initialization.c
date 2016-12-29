/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2010 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include "M051Series.h"

#define PLLCON_SETTING      SYSCLK_PLLCON_50MHz_XTAL
#define PLL_CLOCK           50000000

void CKO_Init(void)
{
    /* CKO = HCLK / 2^(1+1) */
    SYS_EnableCKO(SYSCLK_CLKSEL2_FRQDIV_HCLK, 3);

}

void ADC_Init(void)
{
    /* Set the ADC operation mode as single, input mode as single-end and enable the ADC converter */
    ADC->ADCR = (ADC_ADCR_ADMD_SINGLE | ADC_ADCR_DIFFEN_SINGLE_END | ADC_ADCR_ADEN_CONVERTER_ENABLE);
    /* Enable analog input channel 2 */
    ADC->ADCHER = 0x4;
}


void SPI0_Init(void)
{
    /* Initial SPI data format and SPI clock */
    /* SPI clock idle low. 32-bit data output at clock falling and latched at clock rising. */    
    SPI0->CNTRL   = SPI_CNTRL_CLK_IDLE_LOW | SPI_CNTRL_TX_FALLING | 
                    SPI_CNTRL_RX_RISING | SPI_CNTRL_TX_BIT_LEN(32);

    /* SPI clock freq = system clock / ((3+1)*2) */
    SPI0->DIVIDER = SPI_DIVIDER_DIV(3); 
    
    /* Enable automatic slave select control and set it as active low */
    SPI0->SSR = SPI_SSR_HW_AUTO_ACTIVE_LOW;
}


void SPI1_Init(void)
{
    /* Initial SPI data format and SPI clock */
    /* SPI clock idle high. 16-bit data output at clock falling and latched at clock rising. */    
    SPI1->CNTRL   = SPI_CNTRL_CLK_IDLE_HIGH | SPI_CNTRL_TX_FALLING | 
                    SPI_CNTRL_RX_RISING | SPI_CNTRL_TX_BIT_LEN(16);

    /* SPI clock freq = system clock / ((3+1)*2) */
    SPI1->DIVIDER = SPI_DIVIDER_DIV(3); 
    
    /* Enable automatic slave select control and set it as active low */
    SPI1->SSR = SPI_SSR_HW_AUTO_ACTIVE_LOW;
}


void I2C0_Init(void)
{
    /* Enable I2C0 Controller */
    I2C0->I2CON = I2C_I2CON_ENS1_Msk;
    
    /* I2C0 clock divider, I2C Bus Clock = PCLK / (4*125) */
    I2C0->I2CLK = I2C_I2CLK_DIV4(125);

    /* Set I2C0 4 Slave Addresses */            
    _I2C_SET_SLAVE_ADDRESS_0(I2C0, 0x15);   /* Slave Address : 0x15 */
    _I2C_SET_SLAVE_ADDRESS_1(I2C0, 0x35);   /* Slave Address : 0x35 */
    _I2C_SET_SLAVE_ADDRESS_2(I2C0, 0x55);   /* Slave Address : 0x55 */
    _I2C_SET_SLAVE_ADDRESS_3(I2C0, 0x75);   /* Slave Address : 0x75 */

    /* Enable I2C0 interrupt and set corresponding NVIC bit */
    //I2C->I2CON |= I2C_I2CON_EI_Msk;
    //NVIC_EnableIRQ(I2C0_IRQn);
}

void EBI_Init(void)
{
    /* Enable EBI, data width = 16-bit, EBI MCLK = HCLK / 4 */
    EBI->EBICON = EBI_EBICON_ExtEN_Msk | EBI_EBICON_ExtBW16_Msk | EBI_EBICON_MCLKDIV_4;
}

void PWMA_Init(void)
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

    /* Enable PWM channle 0, 1, 2, 3 Output */
    PWMA->POE = PWM_POE_PWM0_Msk | PWM_POE_PWM1_Msk | PWM_POE_PWM2_Msk | PWM_POE_PWM3_Msk;
}


void PWMB_Init(void)
{

    /* PWM Timer0: Clk = HCLK / 120 / 16, Freq = clk / 6250, dute cycle = 3125/6250 % */
    /* PWM Timer1: Clk = HCLK / 120 / 16, Freq = clk / 3125, dute cycle = 1563/3125 % */
    /* PWM Timer2: Clk = HCLK / 60 / 16, Freq = clk / 3125, dute cycle = 1563/3125 % */
    /* PWM Timer3: Clk = HCLK / 60 / 1, Freq = clk / 50, dute cycle = 25/50 % */
    /* PWM0 = 12000000 / 120 / 16 / 6250 =    1Hz */
    /* PWM1 = 12000000 / 120 / 16 / 3125 =    2Hz */
    /* PWM2 = 12000000 /  60 / 16 / 3125 =    4Hz */
    /* PWM3 = 12000000 /  60 /  1 /   50 = 4000Hz */
    PWMB->PPR = PWM_PPR_CP01(120) | PWM_PPR_CP23(60);
    PWMB->CSR = PWM_CSR_CSR0(PWM_CSR_DIV16) | PWM_CSR_CSR1(PWM_CSR_DIV16) |
                   PWM_CSR_CSR2(PWM_CSR_DIV16) | PWM_CSR_CSR3(PWM_CSR_DIV1);

    /* Enable PWM0, PWM1, PWM2, PWM3 counter. We must set PWM mode before setting CNR, CMR. */
    PWMB->PCR = PWM_PCR_CH0EN_Msk | PWM_PCR_CH0MOD_AUTO_RELOAD | 
                   PWM_PCR_CH1EN_Msk | PWM_PCR_CH1MOD_AUTO_RELOAD |
                   PWM_PCR_CH2EN_Msk | PWM_PCR_CH2MOD_AUTO_RELOAD |
                   PWM_PCR_CH3EN_Msk | PWM_PCR_CH3MOD_AUTO_RELOAD;

    PWMB->CNR0= 6250 - 1;
    PWMB->CMR0= 3125 - 1;
    PWMB->CNR1= 3125 - 1;
    PWMB->CMR1= 1563 - 1;
    PWMB->CNR2= 3125 - 1;
    PWMB->CMR2= 1563 - 1;
    PWMB->CNR3= 50 - 1;
    PWMB->CMR3= 25 - 1;

    /* Enable PWM channle 0, 1, 2, 3 Output */
    PWMB->POE = PWM_POE_PWM0_Msk | PWM_POE_PWM1_Msk | PWM_POE_PWM2_Msk | PWM_POE_PWM3_Msk;
}


void TMR0_Init(void)
{
    TIMER0->TCSR = TIMER_TCSR_INIT_IE(12);
    TIMER0->TCMPR = 10000; // 12MHz / 12 / 10000 = 100Hz

    //NVIC_EnableIRQ(TMR0_IRQn);
}

void TMR1_Init(void)
{
    TIMER1->TCSR = TIMER_TCSR_INIT_IE(12);
    TIMER1->TCMPR = 10000; // 12MHz / 12 / 10000 = 100Hz

    //NVIC_EnableIRQ(TMR0_IRQn);
}


void GPIO_Init(void)
{
    /* Debounce function control */
    GPIO->DBNCECON = GPIO_DBNCECON_ICLK_ON | GPIO_DBNCECON_DBCLKSRC_HCLK | GPIO_DBNCECON_DBCLKSEL_32768;

    /* Enable P3.2 debounce function */
    P3->DBEN = GPIO_DBEN_ENABLE(2);

    /* Configure external interrupt EINT0 (P3.2) */
    GPIO_EnableInt(P3, 2, GPIO_INT_FALLING);
    NVIC_EnableIRQ(EINT0_IRQn);
}

void UART0_Init(void)
{
    /* Set 115200 baudrate according to 50MHz system clock */
    UART0->BAUD = UART_BAUD_MODE2 | UART_BAUD_DIV_MODE2(PLL_CLOCK, 115200);
    _UART_SET_DATA_FORMAT(UART0, UART_WORD_LEN_8 | UART_PARITY_NONE | UART_STOP_BIT_1);
}

void UART1_Init(void)
{
    /* Set 115200 baudrate according to 50MHz system clock */
    UART1->BAUD = UART_BAUD_MODE2 | UART_BAUD_DIV_MODE2(PLL_CLOCK, 115200);
    _UART_SET_DATA_FORMAT(UART1, UART_WORD_LEN_8 | UART_PARITY_NONE | UART_STOP_BIT_1);
}

void SYS_Init(void)
{
    /* Unlock protected registers */
    SYS_UnlockReg();

/*---------------------------------------------------------------------------------------------------------*/
/* Init System Clock                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/

    /* Enable Internal RC clock */
    SYSCLK->PWRCON |= SYSCLK_PWRCON_IRC22M_EN_Msk;

    /* Waiting for IRC22M clock ready */
    SYS_WaitingForClockReady(SYSCLK_CLKSTATUS_IRC22M_STB_Msk);

    /* Switch HCLK clock source to internal RC */
    SYSCLK->CLKSEL0 = SYSCLK_CLKSEL0_HCLK_IRC22M;

    /* Set PLL to power down mode and PLL_STB bit in CLKSTATUS register will be cleared by hardware.*/
    SYSCLK->PLLCON |= SYSCLK_PLLCON_PD_Msk;

    /* Enable external 12MHz XTAL, internal 22.1184MHz, 10kHz */
    SYSCLK->PWRCON |= SYSCLK_PWRCON_XTL12M_EN_Msk | SYSCLK_PWRCON_IRC22M_EN_Msk | SYSCLK_PWRCON_IRC10K_EN_Msk;

    /* Enable PLL and Set PLL frequency */        
    SYSCLK->PLLCON = PLLCON_SETTING;
    
    /* Waiting for clock ready */
    SYS_WaitingForClockReady(SYSCLK_CLKSTATUS_PLL_STB_Msk | SYSCLK_CLKSTATUS_XTL12M_STB_Msk  | SYSCLK_CLKSTATUS_IRC10K_STB_Msk |
                             SYSCLK_CLKSTATUS_IRC22M_STB_Msk);

    /* Switch HCLK clock source to PLL, STCLK to HCLK/2 */
    SYSCLK->CLKSEL0 = SYSCLK_CLKSEL0_STCLK_HCLK_DIV2 | SYSCLK_CLKSEL0_HCLK_PLL;

    /* Enable IP clock */        
    SYSCLK->APBCLK = SYSCLK_APBCLK_ACMP_EN_Msk | SYSCLK_APBCLK_ADC_EN_Msk | SYSCLK_APBCLK_PWM67_EN_Msk | 
                        SYSCLK_APBCLK_PWM45_EN_Msk | SYSCLK_APBCLK_PWM23_EN_Msk | SYSCLK_APBCLK_PWM01_EN_Msk |
                        SYSCLK_APBCLK_UART1_EN_Msk | SYSCLK_APBCLK_UART0_EN_Msk | SYSCLK_APBCLK_SPI1_EN_Msk |
                        SYSCLK_APBCLK_SPI0_EN_Msk | SYSCLK_APBCLK_I2C0_EN_Msk | SYSCLK_APBCLK_FDIV_EN_Msk |
                        SYSCLK_APBCLK_TMR3_EN_Msk | SYSCLK_APBCLK_TMR2_EN_Msk | SYSCLK_APBCLK_TMR1_EN_Msk |
                        SYSCLK_APBCLK_TMR0_EN_Msk | SYSCLK_APBCLK_WDT_EN_Msk; 

    /* Select IP clock source */
    SYSCLK->CLKSEL1 = SYSCLK_CLKSEL1_PWM23_HCLK | SYSCLK_CLKSEL1_PWM01_HCLK | SYSCLK_CLKSEL1_UART_PLL |
                         SYSCLK_CLKSEL1_TMR3_XTAL | SYSCLK_CLKSEL1_TMR2_XTAL | SYSCLK_CLKSEL1_TMR1_XTAL |
                         SYSCLK_CLKSEL1_TMR0_XTAL | SYSCLK_CLKSEL1_ADC_HCLK | SYSCLK_CLKSEL1_WDT_IRC10K;

    SYSCLK->CLKSEL2 = SYSCLK_CLKSEL2_PWM67_HCLK | SYSCLK_CLKSEL2_PWM45_HCLK | SYSCLK_CLKSEL2_FRQDIV_HCLK;
    
        
    /* IP clock divider */
    SYSCLK->CLKDIV = SYSCLK_CLKDIV_ADC(7) |  // ADC clock = ADC clock source / 7
                        SYSCLK_CLKDIV_UART(1) | // UART clock = UART clock source / 1
                        SYSCLK_CLKDIV_HCLK(1);  // HCLK clock = HCLK clock source / 1


    /* Update System Core Clock */
    /* User can use SystemCoreClockUpdate() to calculate PllClock, SystemCoreClock and CycylesPerUs automatically. */
    //SystemCoreClockUpdate(); 
    PllClock        = PLL_CLOCK;            // PLL
    SystemCoreClock = PLL_CLOCK / 1;        // HCLK
    CyclesPerUs     = PLL_CLOCK / 1000000;  // For SYS_SysTickDelay()


/*---------------------------------------------------------------------------------------------------------*/
/* Init I/O Multi-function                                                                                 */
/*---------------------------------------------------------------------------------------------------------*/
    /* 
       P0.0, P0.1  for UART1 - TXD1, RXD1
       P0.2, P0.3  for UART0 - CTS0, RTS0
       P0.4 ~ P0.7 for SPI1  - SPISS1, MOSI_1, MISO_1, SPICLK1
    */
    SYS->P0_MFP = SYS_MFP_P00_TXD1 | SYS_MFP_P01_RXD1 | SYS_MFP_P02_CTS0 | SYS_MFP_P03_RTS0 |
                     SYS_MFP_P04_SPISS1 | SYS_MFP_P05_MOSI_1 | SYS_MFP_P06_MISO_1 | SYS_MFP_P07_SPICLK1;

    /* 
       P1.0        for TIMER2 - T2 (external event counter input)
       P1.1        for TIMER3 - T3 (external event counter input)
       P1.2, P1.3  for ADC    - analog to digital converter channel 2, 3.
       P1.4 ~ P1.7 for SPI0   - SPISS0, MOSI_0, MISO_0, SPICLK0
    */
    SYS->P1_MFP = SYS_MFP_P10_T2 | SYS_MFP_P11_T3 | SYS_MFP_P12_AIN2 | SYS_MFP_P13_AIN3 |
                     SYS_MFP_P14_SPISS0 | SYS_MFP_P15_MOSI_0 | SYS_MFP_P16_MISO_0 | SYS_MFP_P17_SPICLK0;

    /* P2.0 ~ P2.7 for PWM0 ~ PWM7 */
    SYS->P2_MFP = SYS_MFP_P20_PWM0 | SYS_MFP_P21_PWM1 | SYS_MFP_P22_PWM2 | SYS_MFP_P23_PWM3 |
                     SYS_MFP_P24_PWM4 | SYS_MFP_P25_PWM5 | SYS_MFP_P26_PWM6 | SYS_MFP_P27_PWM7;

    /* 
       P3.0, P3.1 for UART0  - RXD0, TXD0
       P3.2       for TIMER1 - T1EX (external capture/reset trigger input)
       P3.3       for TIMER0 - T0EX (external capture/reset trigger input)
       P3.4, P3.5 for I2C0   - SDA0, SCL0
       P3.6       for CKO
       P3.7       for GPIO
    */
    SYS->P3_MFP = SYS_MFP_P30_RXD0 | SYS_MFP_P31_TXD0 | SYS_MFP_P32_T0EX | SYS_MFP_P33_T1EX |
                     SYS_MFP_P34_SDA0 | SYS_MFP_P35_SCL0 | SYS_MFP_P36_CKO | SYS_MFP_P37_GPIO;
                     
    /* 
       P4.0 ~ P4.5 for GPIO
       P4.6, P4.7  for ICE 
    */
    SYS->P4_MFP = SYS_MFP_P40_PWM0 | SYS_MFP_P41_PWM1 | SYS_MFP_P42_PWM2 | SYS_MFP_P43_PWM3 |
                     SYS_MFP_P46_ICE_CLK | SYS_MFP_P47_ICE_DAT;
                     
    /* Lock protected registers */
    SYS_LockReg();
                         
}




