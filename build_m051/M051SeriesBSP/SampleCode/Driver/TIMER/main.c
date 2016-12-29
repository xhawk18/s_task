/**************************************************************************//**
 * @file     main.c
 * @version  V2.10
 * $Revision: 7 $
 * $Date: 12/08/09 7:05p $ 
 * @brief    M051 Series Timer Controller and Watchdog Timer Driver Sample Code
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
/* Global Interface Variables Declarations                                                                 */                                                                            
/*---------------------------------------------------------------------------------------------------------*/
extern int IsDebugFifoEmpty(void);
volatile uint8_t  g_u8IsWDTTimeoutINT = 0, g_u8IsWDTWakeupINT = 0;
volatile uint32_t g_au32TMRINTCount[4] = {0};
                         

/*---------------------------------------------------------------------------------------------------------*/
/*  Function for System Entry to Power Down Mode                                                           */
/*---------------------------------------------------------------------------------------------------------*/
void PowerDownFunction(void)
{
    SYS_UnlockReg();
    
    /* To check if all the debug messages are finished */
    while (IsDebugFifoEmpty() == 0);

    SCB->SCR = 4;

    SYSCLK->PWRCON = (SYSCLK->PWRCON & ~(SYSCLK_PWRCON_PWR_DOWN_EN_Msk | SYSCLK_PWRCON_PD_WAIT_CPU_Msk)) |
                        SYSCLK_PWRCON_PD_WAIT_CPU_Msk | SYSCLK_PWRCON_PD_WU_INT_EN_Msk;
    SYSCLK->PWRCON |= SYSCLK_PWRCON_PWR_DOWN_EN_Msk;                     
       
    __WFI();
}


/*---------------------------------------------------------------------------------------------------------*/
/*  Create Counter Source by GPIO PORT2                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
void GeneratePORT2Counter(uint32_t u32Pin, uint32_t u32Counts)
{
    while (u32Counts--)
    {
        GPIO_PIN_DATA(2, u32Pin) = 1; 
        GPIO_PIN_DATA(2, u32Pin) = 0;  
    }
}


/**
 * @brief       Timer-0 IRQ
 *
 * @param       None
 *
 * @return      None
 *
 * @details     The TIMER0 default IRQ, declared in startup_M051Series.s.
 */
void TMR0_IRQHandler(void)
{
    /* Clear TIMER0 Timeout Interrupt Flag */
    _TIMER_CLEAR_CMP_INT_FLAG(TIMER0);      

    g_au32TMRINTCount[0]++;
}


/**
 * @brief       Timer-1 IRQ
 *
 * @param       None
 *
 * @return      None
 *
 * @details     The TIMER1 default IRQ, declared in startup_M051Series.s.
 */
void TMR1_IRQHandler(void)
{
    if (_TIMER_GET_CMP_INT_FLAG(TIMER1) == 1)
    {
        /* Clear TIMER1 Timeout Interrupt Flag */
        _TIMER_CLEAR_CMP_INT_FLAG(TIMER1);      
    }else
    if (_TIMER_GET_CAP_INT_FLAG(TIMER1) == 1)
    {
        /* Clear TIMER1 Capture Interrupt Flag */
        _TIMER_CLEAR_CAP_INT_FLAG(TIMER1);      
    }

    g_au32TMRINTCount[1]++;
}


/**
 * @brief       Timer-2 IRQ
 *
 * @param       None
 *
 * @return      None
 *
 * @details     The TIMER2 default IRQ, declared in startup_M051Series.s.
 */
void TMR2_IRQHandler(void)
{
    /* Clear TIMER2 Timeout Interrupt Flag */
    _TIMER_CLEAR_CMP_INT_FLAG(TIMER2);      

    g_au32TMRINTCount[2]++;
}


/**
 * @brief       Timer-3 IRQ
 *
 * @param       None
 *
 * @return      None
 *
 * @details     The TIMER3 default IRQ, declared in startup_M051Series.s.
 */
void TMR3_IRQHandler(void)
{
    /* Clear TIMER3 Timeout Interrupt Flag */
    _TIMER_CLEAR_CMP_INT_FLAG(TIMER3);      

    g_au32TMRINTCount[3]++;
}


/*---------------------------------------------------------------------------------------------------------*/
/*  To Delay Specified Interrupt Ticks                                                                     */
/*---------------------------------------------------------------------------------------------------------*/
void TIMER_DelayIntTicks(uint32_t u32TimerCH, uint32_t u32DelayTicks)
{
    g_au32TMRINTCount[u32TimerCH] = 0;
    
    while (g_au32TMRINTCount[u32TimerCH] < u32DelayTicks);
}


/**
 * @brief       IRQ Handler for WDT and WWDT Interrupt
 *
 * @param       None
 *
 * @return      None
 *
 * @details     The WDT_IRQHandler is default IRQ of WDT and WWDT, declared in startup_M051Series.s.
 */
void WDT_IRQHandler(void)
{
    if ((_WDT_GET_TIMEOUT_INT_FLAG() == 1) && (_WDT_GET_TIMEOUT_WAKEUP_FLAG() == 1))
    {
        /* Clear WDT Timeout Interrupt and Wake-up Flag */
        _WDT_CLEAR_TIMEOUT_INT_FLAG();
        _WDT_CLEAR_TIMEOUT_WAKEUP_FLAG();
        
        g_u8IsWDTWakeupINT = 1;

        printf("WDT Timeout Wake-up occurred. \n");
    }

    if (_WDT_GET_TIMEOUT_INT_FLAG() == 1)
    {
        /* Clear WDT Timeout Interrupt Flag */
        _WDT_CLEAR_TIMEOUT_INT_FLAG();
        
        g_u8IsWDTTimeoutINT = 1;

        printf("WDT Timeout occurred. \n");
    }
}


/*---------------------------------------------------------------------------------------------------------*/
/*  Timer Interrupt Ticks and Frequency Test                                                               */
/*---------------------------------------------------------------------------------------------------------*/
void TIMER_InterruptAndFrequencyTest(void)
{
    uint32_t u32InitCount;

    printf("# TIMER Settings: \n");
    printf("  TIMER0: Clock source 12 MHz;     Periodic mode; Enable interrupt; 1 interrupt tick/sec.  \n");
    printf("  TIMER1: Clock source 48 MHz;     Periodic mode; Enable interrupt; 2 interrupt ticks/sec. \n");
    printf("  TIMER2: Clock source IRC 22 MHz; Periodic mode; Enable interrupt; 4 interrupt ticks/sec. \n");
    printf("  TIMER3: Clock source 12 MHz;     Periodic mode; Enable interrupt; 8 interrupt ticks/sec. \n");
    printf("# Check all Timer interrupt counts are reasonable or not. \n");
    printf("Press any key to start test ... \n");
    getchar();
    printf("\n");
    
    /* Reset and stop TIMER0, TIMER1, TIMER2 and TIMER3 counting first */
    _TIMER_RESET(TIMER0);
    _TIMER_RESET(TIMER1);
    _TIMER_RESET(TIMER2);
    _TIMER_RESET(TIMER3);

    /* Enable TIMER0, TIMER1, TIMER2 and TIMER3 NVIC */
    NVIC_EnableIRQ(TMR0_IRQn);
    NVIC_EnableIRQ(TMR1_IRQn);
    NVIC_EnableIRQ(TMR2_IRQn);
    NVIC_EnableIRQ(TMR3_IRQn);

    /* To Configure TCMPR values based on Timer clock source and pre-scale value */
    TIMER0->TCMPR = __XTAL;                  // For 1 tick per second when using external 12MHz (Prescale = 1)
    TIMER1->TCMPR = ((SystemCoreClock/4)/2); // For 2 ticks per second when using 48MHz HCLK (Prescale = 4)
    TIMER2->TCMPR = ((__IRC22M/1)/4);        // For 4 ticks per second when using IRC 22.1184MHz (Prescale = 1)
    TIMER3->TCMPR = ((__XTAL/1)/8);          // For 8 ticks per second when using external 12MHz (Prescale = 1)

    /* Start TIMER0 counting */
    TIMER0->TCSR = TIMER_TCSR_CEN_Msk | TIMER_TCSR_IE_Msk | TIMER_TCSR_MODE_PERIODIC | TIMER_TCSR_TDR_EN_Msk |
                      TIMER_TCSR_PRESCALE(1);

    /* Start TIMER1 counting */
    TIMER1->TCSR = TIMER_TCSR_CEN_Msk | TIMER_TCSR_IE_Msk | TIMER_TCSR_MODE_PERIODIC | TIMER_TCSR_TDR_EN_Msk |
                      TIMER_TCSR_PRESCALE(4);

    /* Start TIMER2 counting */
    TIMER2->TCSR = TIMER_TCSR_CEN_Msk | TIMER_TCSR_IE_Msk | TIMER_TCSR_MODE_PERIODIC | TIMER_TCSR_TDR_EN_Msk |
                      TIMER_TCSR_PRESCALE(1);

    /* Start TIMER3 counting */
    TIMER3->TCSR = TIMER_TCSR_CEN_Msk | TIMER_TCSR_IE_Msk | TIMER_TCSR_MODE_PERIODIC | TIMER_TCSR_TDR_EN_Msk |
                      TIMER_TCSR_PRESCALE(1);
    
    /* Clear Timer interrupt ticks to 0 */
    g_au32TMRINTCount[0] = g_au32TMRINTCount[1] = g_au32TMRINTCount[2] = g_au32TMRINTCount[3] = 0;

    /* Check Timer interrupt counts */
    u32InitCount = g_au32TMRINTCount[0];
    printf("# Timer interrupt counts :  \n");
    while (g_au32TMRINTCount[0] <= 10)
    {
        if (g_au32TMRINTCount[0] != u32InitCount)
        {
            printf("TMR0:%d  TMR1:%d  TMR2:%d  TMR3:%d \n", 
                    g_au32TMRINTCount[0], g_au32TMRINTCount[1], g_au32TMRINTCount[2], g_au32TMRINTCount[3]);
            u32InitCount = g_au32TMRINTCount[0];
        }
    }
    
    /* Reset and stop TIMER0, TIMER1, TIMER2 and TIMER3 counting */
    _TIMER_RESET(TIMER0);
    _TIMER_RESET(TIMER1);
    _TIMER_RESET(TIMER2);
    _TIMER_RESET(TIMER3);
    
    printf("\n");
}


/*---------------------------------------------------------------------------------------------------------*/
/*  Delay Time By Timer Interrupt Ticks Test                                                               */
/*---------------------------------------------------------------------------------------------------------*/
void TIMER_DelayTimeByInterruptTicksTest(void)
{
    printf("# TIMER Settings: \n");
    printf("  TIMER0: Clock source 12 MHz; Periodic mode; Enable interrupt; Pre-scale 1; 100000 interrupt ticks/sec. \n");
    printf("# Check delay time period is 2.5 s. \n");
    printf("Press any key to start test ... \n");
    getchar();
    printf("\n");

    /* Reset and stop TIMER0 counting first */
    _TIMER_RESET(TIMER0);

    /* Enable TIMER0 NVIC */
    NVIC_EnableIRQ(TMR0_IRQn);

    /* Configure TCMP values of TIMER0 */
    TIMER0->TCMPR = __XTAL/100000;  // For 100kHz when using external 12MHz

    /* Start TIMER0 counting */
    TIMER0->TCSR = TIMER_TCSR_CEN_Msk | TIMER_TCSR_IE_Msk | TIMER_TCSR_MODE_PERIODIC | TIMER_TCSR_TDR_EN_Msk |
                      TIMER_TCSR_PRESCALE(1);
    
    printf("Start Timer counting ...... \n");

    /* Delay 2.5 sec. (1 interrupt tick = 10 us) */
    TIMER_DelayIntTicks(0, 250000); 

    printf("Sotp Timer counting ...... \n");

    /* Reset and stop TIMER0 counting */
    _TIMER_RESET(TIMER0);

    printf("\n");
}


/*---------------------------------------------------------------------------------------------------------*/
/*  Timer External Counter Function Test                                                                   */
/*---------------------------------------------------------------------------------------------------------*/
void TIMER_ExternalCounterFunctionTest(void)
{
    printf("# TIMER Settings: \n");
    printf("  a) TIMER1: Clock source is HCLK(48 MHz); Counter pin enable; One-Shot mode; Enable interrupt; Pre-scale 1; TCMP is 1234.  \n");
    printf("  b) TIMER1: Clock source is HCLK(48 MHz); Counter pin enable; Periodic mode; Enable interrupt; Pre-scale 1; TCMP is 56789. \n");
    printf("# Connect P2.0 to T1 pin. and pull P2.0 High/Low as T1 counter input source. \n");
    printf("Press any key to start test ... \n");
    getchar();
    printf("\n");
                                                
    /* Configure P2.0 as GPIO output pins and pull pin status to Low first */
    _GPIO_SET_PIN_MODE(P2, 0, GPIO_PMD_OUTPUT);
    P20 = 0;
                                                                
    /* Reset and stop TIMER1 counting first */
    _TIMER_RESET(TIMER1);

    /* Enable TIMER1 NVIC */
    NVIC_EnableIRQ(TMR1_IRQn);

    /* Configure TCMP values of TIMER1 */
    TIMER1->TCMPR = 1234;

    /* Clear Timer interrupt ticks to 0 */
    g_au32TMRINTCount[1] = 0;

    /* Start TIMER1 counting as Timer Counter mode */
    TIMER1->TCSR = TIMER_TCSR_CEN_Msk | TIMER_TCSR_IE_Msk | TIMER_TCSR_MODE_ONESHOT | TIMER_TCSR_CTB_ENABLE | 
                      TIMER_TCSR_TDR_EN_Msk | TIMER_TCSR_PRESCALE(1);
    
    /* To check if TDR of TIMER1 must be 0 as default value */
    if (_TIMER_GET_TDR(TIMER1) != 0)
    {
        printf("Get unexpect counter value and exit test. \n");

        /* Reset and stop TIMER1 */
        _TIMER_RESET(TIMER1);
        return ;
    }

    /* To generate 1 count to T1 pin */
    GeneratePORT2Counter(0, 1);

    /* To check if TIMER1 TDRmust be 1 after generate 1 count */
    if (_TIMER_GET_TDR(TIMER1) == 1)
    {
        printf("T1 get 1 count ... \n");
    }else
    {
        printf("Can not get correct counter value and exit test. \n");

        /* Reset and stop TIMER1 */
        _TIMER_RESET(TIMER1);
        return ;
    }

    /* To generate remains counts to T1 pin */
    GeneratePORT2Counter(0, (1234 - 1));

    while (1)
    {
        if ((g_au32TMRINTCount[1] == 1) && (_TIMER_GET_TDR(TIMER1) == 0) && ((TIMER1->TCSR&TIMER_TCSR_CEN_ENABLE) == 0))// && 
        {
            printf("TIMER1 External Counter Pin(T1) test pass at Timer One-Shot mode. \n");
            break;
        }
    }

    /* Reset and stop TIMER1 counting */
    _TIMER_RESET(TIMER1);

    /* Configure TCMP values of TIMER1 */
    TIMER1->TCMPR = 56789;

    /* Clear Timer interrupt ticks to 0 */
    g_au32TMRINTCount[1] = 0;

    /* Start TIMER1 counting as Timer Counter mode */
    TIMER1->TCSR = TIMER_TCSR_CEN_Msk | TIMER_TCSR_IE_Msk | TIMER_TCSR_MODE_PERIODIC | TIMER_TCSR_CTB_ENABLE | 
                      TIMER_TCSR_TDR_EN_Msk | TIMER_TCSR_PRESCALE(1);
    
    /* To check if TDR of TIMER1 must be 0 as default value */
    if (_TIMER_GET_TDR(TIMER1) != 0)
    {
        printf("Get unexpect counter value and exit test. \n");

        /* Reset and stop TIMER1 */
        _TIMER_RESET(TIMER1);
        return ;
    }

    /* To generate remains counts to T1 pin */
    GeneratePORT2Counter(0, (56789 - 1));
    GeneratePORT2Counter(0, 1);

    while (1)
    {
        if ((g_au32TMRINTCount[1] == 1) && (_TIMER_GET_TDR(TIMER1) == 0) && ((TIMER1->TCSR&TIMER_TCSR_CEN_ENABLE) == TIMER_TCSR_CEN_ENABLE))
        {
            printf("TIMER1 External Counter Pin(T1) test pass at Timer Periodic mode. \n");
            break;
        }
    }

    /* Reset and stop TIMER1 counting */
    _TIMER_RESET(TIMER1);

    printf("\n");
}


/*---------------------------------------------------------------------------------------------------------*/
/*  Timer External Capture Function Test                                                                   */
/*---------------------------------------------------------------------------------------------------------*/
void TIMER_ExternalCaptureFunctionTest(void)
{
    uint32_t u32InitCount;

    printf("# TIMER Settings: \n");
    printf("  TIMER0: Clock source is 12 MHz; Toggle-Out mode and frequency is 500 Hz; Disable interrupt. \n");
    printf("  TIMER3: Clock source is 12 MHz; Toggle-Out mode and frequency is 1 Hz; Disable interrupt.   \n");
    printf("  TIMER1: Clock source is HCLK(48 MHz); Periodic mode; Disable interrupt; Pre-scale 1; TCMP is 0xFFFFFF; \n");
    printf("          Counter pin enable; Capture pin enable; \n");
    printf("# Generate 500 Hz frequency of T0 and connect T0 to T1 counter pin. \n");
    printf("# Generate 1 Hz frequency of T3 and connect T3 to T1EX capture pin. \n");
    printf("# Get 500 ccounts from T1 pin when each T1EX pin interrupt occurred.  \n");
    printf("Press any key to start test ... \n");
    getchar();
    printf("\n");
                                                
    /* Reset and stop TIMER0, TIMER1 and TIMER3 counting first */
    _TIMER_RESET(TIMER0);
    _TIMER_RESET(TIMER1);
    _TIMER_RESET(TIMER3);

    /* Enable TIMER1 NVIC for T1EX interrupt */
    NVIC_EnableIRQ(TMR1_IRQn);

    /* Configure TCMP values of TIMER0 and TIMER3 */
    TIMER0->TCMPR = (__XTAL/1000);  // For 1000Hz (Equal to 500Hz toggle rate)
    TIMER3->TCMPR = __XTAL/2;       // For 2Hz (Prescale = 1)

    /* Start TIMER0 counting and output T0 frequency - 500 Hz*/
    TIMER0->TCSR = TIMER_TCSR_CEN_Msk | TIMER_TCSR_MODE_TOGGLE | TIMER_TCSR_TDR_EN_Msk | TIMER_TCSR_PRESCALE(1);

    /* Start TIMER3 counting and output T3 frequency - 1 Hz */
    TIMER3->TCSR = TIMER_TCSR_CEN_Msk | TIMER_TCSR_MODE_TOGGLE | TIMER_TCSR_TDR_EN_Msk | TIMER_TCSR_PRESCALE(1);

    /* Enable TIMER1 counter and capture function */
    TIMER1->TCMPR = 0xFFFFFF;
    TIMER1->TCSR = TIMER_TCSR_CEN_Msk | TIMER_TCSR_IE_Msk | TIMER_TCSR_MODE_CONTINUOUS | TIMER_TCSR_CTB_ENABLE | 
                      TIMER_TCSR_TDR_EN_Msk | TIMER_TCSR_PRESCALE(1);
    TIMER1->TEXCON = TIMER_TEXCON_MODE_CAP | TIMER_TEXCON_TEXIEN_ENABLE | TIMER_TEXCON_TEXEN_ENABLE;

    /* Check T1EX interrupt counts */
    u32InitCount = g_au32TMRINTCount[1] = 0;
    while (g_au32TMRINTCount[1] <= 10)
    {
        if (g_au32TMRINTCount[1] != u32InitCount)
        {
            printf("   [%d]- %d \n", g_au32TMRINTCount[1], _TIMER_GET_TCAP(TIMER1));
            u32InitCount = g_au32TMRINTCount[1];
        }
    }

    /* Reset and stop TIMER0, TIMER1 and TIMER3 counting */
    _TIMER_RESET(TIMER0);
    _TIMER_RESET(TIMER1);
    _TIMER_RESET(TIMER3);

    printf("\n");
}


/*---------------------------------------------------------------------------------------------------------*/
/*  WDT Timeout Interrupt Test                                                                             */
/*---------------------------------------------------------------------------------------------------------*/
void WDT_TimeoutInterruptTest(void)
{
    printf("# WDT Settings: \n");
    printf("  Clock source is 10 kHz; Enable interrupt; Timeout interval is 2^14 * WDT clock. \n");
    printf("# When WDT statr counting, system will generate a WDT timeout interrupt after 1.6384 ~ 1.7408 s. \n");
    printf("  Measure P0.0 Low period to check timeout interval. \n");
    printf("Press any key to start test ... \n");
    getchar();
                                
    /* Because of all bits can be written in WDT Control Register are write-protected;
       To program it needs to disable register protection first. */
    SYS_UnlockReg();

    g_u8IsWDTTimeoutINT = 0;

    /* Enable WDT NVIC */
    NVIC_EnableIRQ(WDT_IRQn);

    P00 = 0;

    /* Enable WDT interrupt and select timeout interval to 2^14 * WDT clock then start WDT counting */
    WDT->WTCR = WDT_WTCR_INT_2POW14;               
    
    /* Check if WDT timeout interrupt occurred */
    while (g_u8IsWDTTimeoutINT == 0);

    P00 = 1;

    /* Disable WDT counting */
    _WDT_DISABLE_COUNTING();

    g_u8IsWDTTimeoutINT = 0;

    /* Lock protected registers */
    SYS_LockReg();

    printf("\n");
}


/*---------------------------------------------------------------------------------------------------------*/
/*  WDT Timeout Wake-up Function Test                                                                      */
/*---------------------------------------------------------------------------------------------------------*/
void WDT_TimeoutWakeupTest(void)
{
    printf("# WDT Settings: \n");
    printf("  Clock source is 10 kHz; Enable interrupt; Enable Wake-up; Timeout interval is 2^16 * WDT clock. \n");
    printf("# When WDT statr counting, system will generate a WDT timeout interrupt after 6.5536 ~ 6.656 s. \n");
    printf("  Measure P0.0 Low period to check timeout interval and system can be waken-up by WDT timeout event. \n");
    printf("Press any key to start test ... \n");
    getchar();
                
    /* Because of all bits can be written in WDT Control Register are write-protected;
       To program it needs to disable register protection first. */
    SYS_UnlockReg();

    g_u8IsWDTWakeupINT = 0;

    /* Enable WDT NVIC */
    NVIC_EnableIRQ(WDT_IRQn);

    P00 = 0;

    /* Enable WDT interrupt and select timeout interval to 2^16 * WDT clock then start WDT counting */
    WDT->WTCR = WDT_WTCR_WAKEUP_INT_2POW16;               
    
    /* System entry into Power Down Mode */
    PowerDownFunction();
    
    /* Check if WDT timeout interrupt and Power Down Wake-up Interrupt Flag occurred */
    while (1)
    {
        if (((SYSCLK->PWRCON & SYSCLK_PWRCON_PD_WU_STS_Msk) == SYSCLK_PWRCON_PD_WU_STS_Msk) && (g_u8IsWDTWakeupINT == 1))
            break;
        
    }

    /* Clear Power Down Wake-up Interrupt Flag */
    SYSCLK->PWRCON |= SYSCLK_PWRCON_PD_WU_STS_Msk;

    P00 = 1;

    /* Disable WDT counting */
    _WDT_DISABLE_COUNTING();

    g_u8IsWDTWakeupINT = 0;

    /* Lock protected registers */
    SYS_LockReg();

    printf("\n");
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

    /* Enable external 12MHz XTAL, internal 10kHz */
    SYSCLK->PWRCON |= SYSCLK_PWRCON_XTL12M_EN_Msk | SYSCLK_PWRCON_OSC10K_EN_Msk;

    /* Enable PLL and Set PLL frequency */        
    SYSCLK->PLLCON = PLLCON_SETTING;

    /* Waiting for clock ready */
    SYS_WaitingForClockReady(SYSCLK_CLKSTATUS_PLL_STB_Msk | SYSCLK_CLKSTATUS_XTL12M_STB_Msk |SYSCLK_CLKSTATUS_IRC10K_STB_Msk);

    /* Switch HCLK clock source to PLL, STCLK to HCLK/2 */
    SYSCLK->CLKSEL0 = SYSCLK_CLKSEL0_STCLK_HCLK_DIV2 | SYSCLK_CLKSEL0_HCLK_PLL;

    /* Enable IP clock */        
    SYSCLK->APBCLK = SYSCLK_APBCLK_UART0_EN_Msk | SYSCLK_APBCLK_WDT_EN_Msk | 
                        SYSCLK_APBCLK_TMR0_EN_Msk | SYSCLK_APBCLK_TMR1_EN_Msk | SYSCLK_APBCLK_TMR2_EN_Msk | SYSCLK_APBCLK_TMR3_EN_Msk;

    /* IP clock source */
    SYSCLK->CLKSEL1 = SYSCLK_CLKSEL1_UART_PLL | SYSCLK_CLKSEL1_WDT_IRC10K |
                         SYSCLK_CLKSEL1_TMR0_XTAL | SYSCLK_CLKSEL1_TMR1_HCLK | SYSCLK_CLKSEL1_TMR2_IRC22M | SYSCLK_CLKSEL1_TMR3_XTAL;

    /* Update System Core Clock */
    /* User can use SystemCoreClockUpdate() to calculate PllClock, SystemCoreClock and CycylesPerUs automatically. */
    //SystemCoreClockUpdate(); 
    PllClock        = PLL_CLOCK;            // PLL
    SystemCoreClock = PLL_CLOCK / 1;        // HCLK
    CyclesPerUs     = PLL_CLOCK / 1000000;  // For SYS_SysTickDelay()

/*---------------------------------------------------------------------------------------------------------*/
/* Init I/O Multi-function                                                                                 */
/*---------------------------------------------------------------------------------------------------------*/
    /* Set P3 multi-function pins for UART0 RXD, TXD, T0, T1 and T1EX */
    SYS->P3_MFP = SYS_MFP_P30_RXD0 | SYS_MFP_P31_TXD0 | SYS_MFP_P34_T0 | SYS_MFP_P35_T1 | SYS_MFP_P33_T1EX;

    /* Set P1 multi-function pins for T3 */
    SYS->P1_MFP = SYS_MFP_P11_T3;

    /* Lock protected registers */
    SYS_LockReg();
}


void UART0_Init(void)
{
/*---------------------------------------------------------------------------------------------------------*/
/* Init UART                                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
    /* Reset IP */
    SYS->IPRSTC2 |=  SYS_IPRSTC2_UART0_RST_Msk;
    SYS->IPRSTC2 &= ~SYS_IPRSTC2_UART0_RST_Msk;

    /* Configure UART0 and set UART0 Baudrate */
    UART0->BAUD = UART_BAUD_MODE2 | UART_BAUD_DIV_MODE2(PLL_CLOCK, 115200);
    _UART_SET_DATA_FORMAT(UART0, UART_WORD_LEN_8 | UART_PARITY_NONE | UART_STOP_BIT_1);
}


/*---------------------------------------------------------------------------------------------------------*/
/*  MAIN function                                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
int main (void)
{
	uint8_t	u8Item = 0x0;

    /* Init System, IP clock and multi-function I/O */
    SYS_Init();     

    /* Init UART0 for printf */
    UART0_Init();

    printf("+----------------------------------------------+\n");
    printf("|    M05xx TIMER and WDT Driver Sample Code    |\n");
    printf("+----------------------------------------------+\n");
	printf("\n");

    /* Use P0.0 to check period time of specify test item */
    _GPIO_SET_PIN_MODE(P0, 0, GPIO_PMD_OUTPUT);
    P00 = 1;

    do
    {
        /* 
            The clock source of Timer and WDT are 
                TIMER0: 12 MHz
                TIMER1: 48 MHz from HCLK
                TIMER2: IRC 22.1184 MHz
                TIMER3: 12 MHz
                WDT:    IRC 10 kHz  
        */
        printf("+-------------------------------------------------------+ \n");
        printf("| Select an item to test                                | \n");
        printf("+-------------------------------------------------------+ \n");
        printf("| [1] TIMER0/1/2/3 timeout interrupt and frequency test | \n");
        printf("| [2] Delay time by TIMER0 interrupt ticks test         | \n");
        printf("| [3] TIMER1 External Counter Pin and Function test     | \n");
        printf("| [4] TIMER1 External Capture Pin and Function test     | \n");
        printf("|     Only supported on M05xxBN series, ex:M0516LBN.    | \n");
        printf("| [5] WDT timeout interrupt test                        | \n");
        printf("| [6] Power Down and waken-up by WDT timeout interrupt  | \n");
        printf("| [ESC] Quit                                            | \n");
        printf("+-------------------------------------------------------+ \n");
        printf("\n");
        u8Item = getchar();

        switch (u8Item)
        {
            case '1':
                TIMER_InterruptAndFrequencyTest();      break;    

            case '2':
                TIMER_DelayTimeByInterruptTicksTest();  break;    

            case '3':
                TIMER_ExternalCounterFunctionTest();    break;    

            case '4':
                TIMER_ExternalCaptureFunctionTest();    break;    

            case '5':
                WDT_TimeoutInterruptTest();             break;    

            case '6':
                WDT_TimeoutWakeupTest();                break;    
        }
    } while (u8Item != 27);
  
    printf("\nExit TIMER and WDT Sample Code. \n");

    return 0;
}
