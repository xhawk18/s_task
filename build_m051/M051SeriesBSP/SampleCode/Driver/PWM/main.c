/**************************************************************************//**
 * @file     Smpl_DrvPWM.c
 * @version  V1.00
 * $Revision: 11 $
 * $Date: 12/08/09 7:27p $ 
 * @brief    M051 Series PWM Generator and Capture Timer Driver Sample Code
 *
 * @note
 * Copyright (C) 2011 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#include <stdio.h>
#include "M051Series.h"

/*---------------------------------------------------------------------------------------------------------*/
/* Macro, type and constant definitions                                                                    */
/*---------------------------------------------------------------------------------------------------------*/

#define PLLCON_SETTING      SYSCLK_PLLCON_50MHz_XTAL
#define PLL_CLOCK           50000000



// Scale frequency and unit is Hz
#define TENOR_C 523 
#define TENOR_D 587
#define TENOR_E 659
#define TENOR_F 698
#define TENOR_G 784
#define TENOR_A 880
#define TENOR_B 988

void PWM_PwmIRQHandler(void);
/*---------------------------------------------------------------------------------------------------------*/
/* Global variables                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
volatile uint8_t g_u8PWMCount = 1;
volatile uint16_t g_u16Frequency;
volatile uint32_t g_u32Pulse = 0;

/* Assume PWM output frequency is 523Hz and duty ratio is 60%, user can calculate PWM settings by follows.
   PWM clock source frequency = __XTAL = 12000000 in the sample code.
   (CNR+1) = PWM clock source frequency/prescaler/clock source divider/PWM output frequency 
           = 12000000/2/1/523 = 11472 < 65536  (Note: If calculated value is larger than 65536, user should increase prescale value.)
   CNR = 11471 =>g_au16ScaleCnr[0] = 11471
   duty ratio = 60% = (CMR+1)/(CNR+1) ==> CMR = (CNR+1)*60/100-1 = 11472*60/100-1
   CMR = 6882 =>g_au16ScaleCmr[0] = 6882
*/ 
static const uint16_t g_au16ScaleFreq[7] = {TENOR_C, TENOR_D, TENOR_E, TENOR_F, TENOR_G, TENOR_A, TENOR_B};
static const uint16_t g_au16ScaleCnr[7] =  {11471,10220,9103,8594,7652,6817,6071};            
static const uint16_t g_au16ScaleCmr[7] =  {6882 ,6131 ,5461,5156,4590,4089,3642};

/**
 * @brief       PWMA IRQ Handler
 *
 * @param       None
 *
 * @return      None
 *
 * @details     ISR to handle PWMA interrupt event
 */
void PWMA_IRQHandler(void)
{    
    /*In this sample code, user will not use PWMA channel 0~3 PIIR interrupt and CAPIF capture interrupt. 
      Defined following code as 0 for reducing interrupt execution time and code size. */  
#if 0  
    uint32_t u32PwmIntFlag, u32CapIntFlag0, u32CapIntFlag1;

    /* Handle PWMA Capture function */
    u32CapIntFlag0 = PWMA->CCR0;
    u32CapIntFlag1 = PWMA->CCR2;
    
    if (u32CapIntFlag0 & PWM_CCR0_CAPIF0_Msk) 
    {
        PWMA->CCR0&=(PWM_CCR_MASK|PWM_CCR0_CAPIF0_Msk);        
    }
    
    else if (u32CapIntFlag0 & PWM_CCR0_CAPIF1_Msk) 
    {
        PWMA->CCR0&=(PWM_CCR_MASK|PWM_CCR0_CAPIF1_Msk);
    }
    
    else if (u32CapIntFlag1 & PWM_CCR2_CAPIF2_Msk) 
    {
        PWMA->CCR2&=(PWM_CCR_MASK|PWM_CCR2_CAPIF2_Msk);
    }
        
    else if (u32CapIntFlag1 & PWM_CCR2_CAPIF3_Msk) 
    {
        PWMA->CCR2&=(PWM_CCR_MASK|PWM_CCR2_CAPIF3_Msk);    
    }  
        
    /* Handle PWMA Timer function */
    u32PwmIntFlag = PWMA->PIIR;
   
    if (u32PwmIntFlag & PWM_PIIR_PWMIF0_Msk)
    {  
        PWMA->PIIR = PWM_PIIR_PWMIF0_Msk;      
    }

    else if (u32PwmIntFlag & PWM_PIIR_PWMIF1_Msk)
    {
        PWMA->PIIR = PWM_PIIR_PWMIF1_Msk;        
    }
    
    else if (u32PwmIntFlag & PWM_PIIR_PWMIF2_Msk)
    {
        PWMA->PIIR = PWM_PIIR_PWMIF2_Msk;          
    }

    else if (u32PwmIntFlag & PWM_PIIR_PWMIF3_Msk)
    {
        PWMA->PIIR = PWM_PIIR_PWMIF3_Msk;           
    }  
#endif     
}

/**
 * @brief       PWMB IRQ Handler
 *
 * @param       None
 *
 * @return      None
 *
 * @details     ISR to handle PWMB interrupt event
 */
void PWMB_IRQHandler(void)
{ 
    uint32_t u32PwmIntFlag, u32CapIntFlag1;

    /* Handle PWMB Capture function */
    u32CapIntFlag1 = PWMB->CCR2;
    
    /* PWMB channel 2 Capture interrupt */
    if (u32CapIntFlag1 & PWM_CCR2_CAPIF2_Msk) 
    {
        PWMB->CCR2&=(PWM_CCR_MASK|PWM_CCR2_CAPIF2_Msk);
    }
    
    /* Handle PWMB Timer function */
    u32PwmIntFlag = PWMB->PIIR;
    
    /* PWMB channel 0 PWM timer interrupt */ 
    if (u32PwmIntFlag & PWM_PIIR_PWMIF0_Msk)
    {
        PWMB->PIIR = PWM_PIIR_PWMIF0_Msk; 
        PWM_PwmIRQHandler();          
    }       
}


/*---------------------------------------------------------------------------------------------------------*/
/* PWM Timer function                                                                                      */
/*---------------------------------------------------------------------------------------------------------*/
void PWM_PwmIRQHandler(void)
{   
    if (g_u32Pulse == 1 * g_u16Frequency /10)
    {
        /*--------------------------------------------------------------------------------------*/
        /* Stop PWMB channel 0 Timer (Recommended procedure method 2)                           */
        /* Set PWM Timer counter as 0, When interrupt request happen, disable PWM Timer         */                  
        /*--------------------------------------------------------------------------------------*/          
        PWMB->CNR0 = 0;
    }
        
    if (g_u32Pulse == 1 * g_u16Frequency /10 + 1)
        g_u8PWMCount = 0;
    g_u32Pulse++;   
} 

/*--------------------------------------------------------------------------------------*/
/* Capture function to calculate the input waveform information                         */
/* u32Count[4] : Keep the internal counter value when input signal rising / falling     */
/*               happens                                                                */
/*                                                                                      */
/* time    A    B     C     D                                                           */
/*           ___   ___   ___   ___   ___   ___   ___   ___                              */
/*      ____|   |_|   |_|   |_|   |_|   |_|   |_|   |_|   |_____                        */
/* index              0 1   2 3                                                         */
/*                                                                                      */
/* The capture internal counter down count from 0x10000, and reload to 0x10000 after    */
/* input signal falling happens (Time B/C/D)                                            */
/*--------------------------------------------------------------------------------------*/                              
void CalPeriodTime(PWM_T *PWM, uint32_t u32Ch)
{
    uint16_t u32Count[4];
    uint32_t u32i;
    uint16_t u16RisingTime, u16FallingTime, u16HighPeroid, u16LowPeroid, u16TotalPeroid;
    
    /* Clear Capture Falling Indicator (Time A) */
    _PWM_CLR_CAP_FALLING_INDICATOR(PWM, u32Ch);
    
    /* Wait for Capture Falling Indicator  */
    while(_PWM_GET_CAP_FALLING_INDICATOR(PWM, u32Ch)==0);    
    /* Clear Capture Falling Indicator (Time B)*/
    _PWM_CLR_CAP_FALLING_INDICATOR(PWM, u32Ch);
    
    u32i = 0;
    
    while (u32i < 4)
    {
        /* Wait for Capture Falling Indicator */
        while(_PWM_GET_CAP_FALLING_INDICATOR(PWM, u32Ch)==0);
        /* Clear Capture Falling Indicator */
        _PWM_CLR_CAP_FALLING_INDICATOR(PWM, u32Ch);

        /* Clear Capture Rising Indicator */
        _PWM_CLR_CAP_RISING_INDICATOR(PWM, u32Ch);
                
        /* Get Capture Falling Latch Counter Data */
        u32Count[u32i++] = _PWM_GET_CAP_FALLING_LATCH_VALUE(PWM, u32Ch);
        
        /* Wait for Capture Rising Indicator */
        while(_PWM_GET_CAP_RISING_INDICATOR(PWM, u32Ch)==0);
        
        /* Clear Capture Rising Indicator */
        _PWM_CLR_CAP_RISING_INDICATOR(PWM, u32Ch);
        
        /* Get Capture Rising Latch Counter Data */
        u32Count[u32i++] = _PWM_GET_CAP_RISING_LATCH_VALUE(PWM, u32Ch);
    }   
    
    u16RisingTime = u32Count[1];
    
    u16FallingTime = u32Count[0];
    
    u16HighPeroid = u32Count[1] - u32Count[2];
    
    u16LowPeroid = 0x10000 - u32Count[1];
    
    u16TotalPeroid = 0x10000 - u32Count[2];
        
    printf("\nTest Result:\nRising Time = %d, Falling Time = %d.\nHigh Period = %d, Low Period = %d, Total Period = %d.\n",
        u16RisingTime, u16FallingTime, u16HighPeroid, u16LowPeroid, u16TotalPeroid);
    printf("\nTest Result should be:\nHigh Period=7199~7201, Low Period=16799~16801, Total Period=23999~24001\n");
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

    /* Switch HCLK clock source to Internal RC */
    SYSCLK->CLKSEL0 = SYSCLK_CLKSEL0_HCLK_IRC22M;

    /* Set PLL to power down mode and PLL_STB bit in CLKSTATUS register will be cleared by hardware.*/
    SYSCLK->PLLCON |= SYSCLK_PLLCON_PD_Msk;

    /* Enable external 12MHz XTAL, internal 22.1184MHz */
    SYSCLK->PWRCON |= SYSCLK_PWRCON_XTL12M_EN_Msk | SYSCLK_PWRCON_IRC22M_EN_Msk;

    /* Enable PLL and Set PLL frequency */        
    SYSCLK->PLLCON = PLLCON_SETTING;
    
    /* Waiting for clock ready */
    SYS_WaitingForClockReady(SYSCLK_CLKSTATUS_PLL_STB_Msk | SYSCLK_CLKSTATUS_XTL12M_STB_Msk | SYSCLK_CLKSTATUS_IRC22M_STB_Msk);

    /* Switch HCLK clock source to PLL, STCLK to HCLK/2 */
    SYSCLK->CLKSEL0 = SYSCLK_CLKSEL0_STCLK_HCLK_DIV2 | SYSCLK_CLKSEL0_HCLK_PLL;

    /* Enable IP clock */        
    SYSCLK->APBCLK = SYSCLK_APBCLK_UART0_EN_Msk|SYSCLK_APBCLK_PWM45_EN_Msk|SYSCLK_APBCLK_PWM67_EN_Msk;
    /* IP clock source */
    SYSCLK->CLKSEL1 = SYSCLK_CLKSEL1_UART_PLL;    
    /* IP clock source */
    SYSCLK->CLKSEL2 = SYSCLK_CLKSEL2_PWM45_XTAL|SYSCLK_CLKSEL2_PWM67_XTAL;
    
    /* Reset PWMB channel0~channel3 */                    
    SYS->IPRSTC2 = SYS_IPRSTC2_PWM47_RST_Msk;                    
    SYS->IPRSTC2 = 0;      

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
    /* Set P2 multi-function pins for PWMB Channel0~3  */
    SYS->P2_MFP = SYS_MFP_P24_PWM4|SYS_MFP_P25_PWM5|SYS_MFP_P26_PWM6|SYS_MFP_P27_PWM7;

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
/*  Main Function                                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
int32_t main (void)
{   
    uint8_t u8Item;
    int32_t i32Loop = 1;
    int32_t i32TestLoop = 1;
                                    
    /* Init System, IP clock and multi-function I/O */
    SYS_Init(); //In the end of SYS_Init() will issue SYS_LockReg() to lock protected register. If user want to write protected register, please issue SYS_UnlockReg() to unlock protected register.    

    /* Init UART0 for printf */
    UART0_Init();   
             
    printf("+------------------------------------------------------------------------+\n");
    printf("|                          PWM Driver Sample Code                        |\n");
    printf("|                                                                        |\n");
    printf("+------------------------------------------------------------------------+\n");                    
    printf("  This sample code will use PWMB channel 0 to drive Buzzer or use PWMB channel 2 to capture\n  the signal from PWMB channel 1.\n");
    printf("  I/O configuration:\n");
    printf("    PWM4(P2.4 PWMB channel 0) <--> Buzzer\n");
    printf("    PWM5(P2.5 PWMB channel 1) <--> PWM6(P2.6 PWMB channel 2)\n\n");
    
    while (i32Loop)
    {       
        printf("\n  Please Select Test Item\n");
        printf("    0 : Exit\n");          
        printf("    1 : PWM Timer Waveform Test.\n");
        printf("    2 : PWM Caputre Test\n");
          
        u8Item = getchar();
    
        switch (u8Item)
        {
            case '0':
            {
                i32Loop = 0;
                break;          
            }                      
            case '1':       
            {           
                uint8_t u8ItemOK;       
                
                printf("\nPWM Timer Waveform Test. Waveform output(P2.4 PWMB channel 0) to Buzzer\n");  
                /* P2.4 PWMB channel 0 generates PWM frequency Do - Si */
                    
                i32TestLoop = 1; 
                                
                printf("Select Test Item\n");  
                printf(" 1: Do (523Hz)Tenor C\n");
                printf(" 2: Re (587Hz)\n");
                printf(" 3: Mi (659Hz)\n");
                printf(" 4: Fa (698Hz)\n");
                printf(" 5: Sol(784Hz) \n");
                printf(" 6: La (880Hz)\n");
                printf(" 7: Si (988Hz)\n");
                printf(" 0: Exit\n");   
                                            
                while (i32TestLoop)
                {                   
                    u8ItemOK = 1;
                    u8Item = getchar();                         
                                          
                    switch (u8Item)
                    {
                        case '1':
                        case '2':
                        case '3':
                        case '4':
                        case '5':
                        case '6':
                        case '7':
                            g_u16Frequency = g_au16ScaleFreq[(u8Item-'1')];
                            break;  
                        case '0':
                            i32TestLoop = 0;
                            break;      
                        default:
                            u8ItemOK = 0;
                            break;                                                                                                                                                                                                                                  
                    }
                        
                    if (i32TestLoop && u8ItemOK)
                    {
                        g_u32Pulse = 0;
                        g_u8PWMCount = 1;                                                                              

                    /* Assume PWM output frequency is 523Hz and duty ratio is 60%, user can calculate PWM settings by follows.
                       duty ratio = (CMR+1)/(CNR+1)
                       cycle time = CNR+1
                       High level = CMR+1
                       PWM clock source frequency = __XTAL = 12000000
                       (CNR+1) = PWM clock source frequency/prescaler/clock source divider/PWM output frequency 
                               = 12000000/2/1/523 = 11472 
                       (Note: CNR is 16 bits, so if calculated value is larger than 65536, user should increase prescale value.)
                       CNR = 11471
                       duty ratio = 60% ==> (CMR+1)/(CNR+1) = 60% ==> CMR = (CNR+1)*0.6-1 = 11472*60/100-1
                       CMR = 6882
                       Prescale value is 1 : prescaler= 2
                       Clock divider is PWM_CSR_DIV1 : clock divider =1             
                    */                                                                 
                        /*Set Pwm mode*/
                        _PWM_SET_TIMER_AUTO_RELOAD_MODE(PWMB,PWM_CH0);  
                            
                        /*Set PWM Timer clock prescaler*/
                        _PWM_SET_TIMER_PRESCALE(PWMB,PWM_CH0, 1); // Divided by 2  
                                                 
                        /*Set PWM Timer clock divider select*/
                        _PWM_SET_TIMER_CLOCK_DIV(PWMB,PWM_CH0,PWM_CSR_DIV1); 
        
                        /*Set PWM Timer duty*/
                        PWMB->CMR0 = g_au16ScaleCmr[(u8Item-'1')];
                                            
                        /*Set PWM Timer period*/
                        PWMB->CNR0 = g_au16ScaleCnr[(u8Item-'1')];
                            
                        /* Enable PWM Output pin */
                        _PWM_ENABLE_PWM_OUT(PWMB, PWM_CH0); 

                        /* Enable Timer period Interrupt */
                        _PWM_ENABLE_TIMER_PERIOD_INT(PWMB, PWM_CH0);
                        
                        /* Enable PWMB NVIC */
                        NVIC_EnableIRQ((IRQn_Type)(PWMB_IRQn)); 
                                                            
                        /* Enable PWM Timer */
                        _PWM_ENABLE_TIMER(PWMB, PWM_CH0); 
                                
                        while (g_u8PWMCount);   
                                                            
                        /*--------------------------------------------------------------------------------------*/
                        /* Stop PWM Timer (Recommended procedure method 2)                                      */
                        /* Set PWM Timer counter as 0, When interrupt request happen, disable PWM Timer         */          
                        /* Set PWM Timer counter as 0 in Call back function                                     */                              
                        /*--------------------------------------------------------------------------------------*/                          

                        /* Disable PWMB NVIC */
                        NVIC_DisableIRQ((IRQn_Type)(PWMB_IRQn)); 

                        /* Wait until PWMB channel 0 Timer Stop */                         
                        while(PWMB->PDR0!=0);
                                                                                        
                        /* Disable the PWM Timer */
                        _PWM_DISABLE_TIMER(PWMB, PWM_CH0); 
                        
                        /* Disable PWM Output pin */
                        _PWM_DISABLE_PWM_OUT(PWMB, PWM_CH0);                         
                    }
                }   
                break;
            }  
            
            case '2':
            {
                printf("PWM Capture Test\n");
                printf("Use PWMB Channel 2(P2.6) to capture the PWMB Channel 1(P2.5) Waveform\n");       

                /*--------------------------------------------------------------------------------------*/
                /* Set the PWMB Channel 1 as PWM output function.                                               */
                /*--------------------------------------------------------------------------------------*/             

                /* Assume PWM output frequency is 250Hz and duty ratio is 30%, user can calculate PWM settings by follows.
                   duty ratio = (CMR+1)/(CNR+1)
                   cycle time = CNR+1
                   High level = CMR+1
                   PWM clock source frequency = __XTAL = 12000000
                   (CNR+1) = PWM clock source frequency/prescaler/clock source divider/PWM output frequency 
                           = 12000000/2/1/250 = 24000
                   (Note: CNR is 16 bits, so if calculated value is larger than 65536, user should increase prescale value.)
                   CNR = 23999
                   duty ratio = 30% ==> (CMR+1)/(CNR+1) = 30%
                   CMR = 7199
                   Prescale value is 1 : prescaler= 2
                   Clock divider is PWM_CSR_DIV1 : clock divider =1                  
                */                  
                /*Set Pwm mode*/
                _PWM_SET_TIMER_AUTO_RELOAD_MODE(PWMB,PWM_CH1);  
                    
                /*Set PWM Timer clock prescaler*/
                _PWM_SET_TIMER_PRESCALE(PWMB,PWM_CH1, 1); // Divided by 2  
                                         
                /*Set PWM Timer clock divider select*/
                _PWM_SET_TIMER_CLOCK_DIV(PWMB,PWM_CH1,PWM_CSR_DIV1); 

                /*Set PWM Timer duty*/
                PWMB->CMR1 = 7199;
                                    
                /*Set PWM Timer period*/
                PWMB->CNR1 = 23999;
                                                                                                                  
                /* Enable PWM Output path for PWMB channel 1 */
                _PWM_ENABLE_PWM_OUT(PWMB, PWM_CH1);
                                                        
                /* Enable Timer for PWMB channel 1 */ 
                _PWM_ENABLE_TIMER(PWMB, PWM_CH1);
                    
                /*--------------------------------------------------------------------------------------*/
                /* Set the PWMB channel 2  for capture function                                         */
                /*--------------------------------------------------------------------------------------*/                  
                
                /* If input minimum frequency is 250Hz, user can calculate capture settings by follows.
                   Capture clock source frequency = __XTAL = 12000000 in the sample code.
                   (CNR+1) = Capture clock source frequency/prescaler/clock source divider/minimum input frequency 
                           = 12000000/2/1/250 = 24000
                   (Note: CNR is 16 bits, so if calculated value is larger than 65536, user should increase prescale value.)
                   CNR = 0xFFFF
                   (Note: In capture mode, user should set CNR to 0xFFFF to increase capture frequency range.)  
                */                                
                /*Set Pwm mode*/
                _PWM_SET_TIMER_AUTO_RELOAD_MODE(PWMB,PWM_CH2);  
                    
                /*Set PWM Timer clock prescaler*/
                _PWM_SET_TIMER_PRESCALE(PWMB,PWM_CH2, 1); // Divided by 2  
                                         
                /*Set PWM Timer clock divider select*/
                _PWM_SET_TIMER_CLOCK_DIV(PWMB,PWM_CH2,PWM_CSR_DIV1); 
                                
                /*Set PWM Timer loaded value*/
                PWMB->CNR2 = 0xFFFF;
                                                    
                /* Enable capture falling edge interrupt for PWMB channel 2 */
                _PWM_ENABLE_CAP_FALLING_INT(PWMB, PWM_CH2);
                
                /* Enable PWMB NVIC interrupt */
                NVIC_EnableIRQ((IRQn_Type)(PWMB_IRQn)); 

                /* Enable Capture Function for PWMB channel 2 */
                _PWM_ENABLE_CAP_FUNC(PWMB, PWM_CH2);

                /* Enable Timer for PWMB channel 2  */
                _PWM_ENABLE_TIMER(PWMB, PWM_CH2);
                
                /* Wait until PWMB channel 2 Timer start to count */
                while(PWMB->PDR2==0);
                                                
                /* Enable capture input path for PWMB channel 2 */
                _PWM_ENABLE_CAP_IN(PWMB, PWM_CH2);
                                        
                /* Capture the Input Waveform Data */
                CalPeriodTime(PWMB, PWM_CH2);                           
                /*------------------------------------------------------------------------------------------------------*/
                /* Stop PWMB channel 1 (Recommended procedure method 1)                                                 */
                /* Set PWM Timer loaded value(CNR) as 0. When PWM internal counter(PDR) reaches to 0, disable PWM Timer */                  
                /*------------------------------------------------------------------------------------------------------*/                  
                                    
                /* Set PWMB channel 1 loaded value as 0 */
                PWMB->CNR1 =0;

                /* Wait until PWMB channel 1 Timer Stop */                         
                while(PWMB->PDR1!=0);
                        
                /* Disable Timer for PWMB channel 1 */    
                _PWM_DISABLE_TIMER(PWMB, PWM_CH1);
                             
                /* Disable PWM Output path for PWMB channel 1 */
                _PWM_DISABLE_PWM_OUT(PWMB, PWM_CH1);
                
                /*------------------------------------------------------------------------------------------------------*/
                /* Stop PWMB channel 2 (Recommended procedure method 1)                                                 */
                /* Set PWM Timer loaded value(CNR) as 0. When PWM internal counter(PDR) reaches to 0, disable PWM Timer */                
                /*------------------------------------------------------------------------------------------------------*/                              

                /* Disable PWMB NVIC */
                NVIC_DisableIRQ((IRQn_Type)(PWMB_IRQn)); 
                                        
                /* Set loaded value as 0 for PWMB channel 2 */
                PWMB->CNR2 =0;
                
                /* Wait until PWMB channel 2 current counter reach to 0 */
                while(PWMB->PDR2!=0);

                /* Disable Timer for PWMB channel 2 */  
                _PWM_DISABLE_TIMER(PWMB, PWM_CH2);
                
                /* Disable Capture Function for  PWMB channel 2*/
                _PWM_DISABLE_CAP_FUNC(PWMB, PWM_CH2);
                                    
                /* Clear Capture Interrupt flag for PWMB channel 2*/
                _PWM_CLR_CAP_INT_FLAG(PWMB, PWM_CH2);
                                                                               
                /* Disable Capture Input path for PWMB channel 2 */
                _PWM_DISABLE_CAP_IN(PWMB, PWM_CH2);
                                        
                break;          
            }    
        }           
    }       
    
    printf("PWM sample is complete.\n");

    /* Disable PWMB clock engine */
    SYSCLK->APBCLK&=(~(SYSCLK_APBCLK_PWM45_EN_Msk|SYSCLK_APBCLK_PWM67_EN_Msk));          
    return 0;  
}




