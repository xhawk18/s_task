/**************************************************************************//**
 * @file     main.c
 * @version  V2.10
 * $Revision: 10 $
 * $Date: 12/08/09 7:24p $ 
 * @brief    M051 Series General Purpose I/O Driver Sample Code
 *
 * @note
 * Copyright (C) 2011 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#include <stdio.h>
#include "M051Series.h"

#define PLLCON_SETTING      SYSCLK_PLLCON_50MHz_XTAL
#define PLL_CLOCK           50000000



/**
 * @brief       Port0/Port1 IRQ
 *
 * @param       None
 *
 * @return      None
 *
 * @details     The Port0/Port1 default IRQ, declared in startup_M051Series.s.
 */
void GPIOP0P1_IRQHandler(void)
{	
    /* To check if P1.3 interrupt occurred */
    if (P1->ISRC & GPIO_ISRC_ISRC3)
    {
        P1->ISRC = GPIO_ISRC_ISRC3;
        printf("P1.3 INT occurred. \n");
    }else
    {
        /* Un-expected interrupt. Just clear all PORT0, PORT1 interrupts */
        P0->ISRC = P0->ISRC;
        P1->ISRC = P1->ISRC;
        printf("Un-expected interrupts. \n");
    }
}


/**
 * @brief       Port2/Port3/Port4 IRQ
 *
 * @param       None
 *
 * @return      None
 *
 * @details     The Port2/Port3/Port4 default IRQ, declared in startup_M051Series.s.
 */
void GPIOP2P3P4_IRQHandler(void)
{
    /* To check if P4.5 interrupt occurred */
    if (P4->ISRC & GPIO_ISRC_ISRC5)
    {
	    P4->ISRC = GPIO_ISRC_ISRC5;
        printf("P4.5 INT occurred. \n");
    }else
    {
        /* Un-expected interrupt. Just clear all PORT2, PORT3 and PORT4 interrupts */
    	P2->ISRC = P2->ISRC;
    	P3->ISRC = P3->ISRC;
    	P4->ISRC = P4->ISRC;
        printf("Un-expected interrupts. \n");
    }
}


/**
 * @brief       External INT0 IRQ
 *
 * @param       None
 *
 * @return      None
 *
 * @details     The External INT0(P3.2) default IRQ, declared in startup_M051Series.s.
 */
void EINT0_IRQHandler(void)
{
	/* For P3.2, clear the INT flag */
    P3->ISRC = GPIO_ISRC_ISRC2;

    printf("P3.2 EINT0 occurred. \n");
}


/**
 * @brief       External INT1 IRQ
 *
 * @param       None
 *
 * @return      None
 *
 * @details     The External INT1(P3.3) default IRQ, declared in startup_M051Series.s.
 */
void EINT1_IRQHandler(void)
{	
	/* For P3.3, clear the INT flag */
    P3->ISRC = GPIO_ISRC_ISRC3;
	
    printf("P3.3 EINT1 occurred. \n");
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

    /* Enable external 12MHz XTAL */
    SYSCLK->PWRCON |= SYSCLK_PWRCON_XTL12M_EN_Msk;

    /* Enable PLL and Set PLL frequency */        
    SYSCLK->PLLCON = PLLCON_SETTING;

    /* Waiting for clock ready */
    SYS_WaitingForClockReady(SYSCLK_CLKSTATUS_PLL_STB_Msk | SYSCLK_CLKSTATUS_XTL12M_STB_Msk);

    /* Switch HCLK clock source to PLL, STCLK to HCLK/2 */
    SYSCLK->CLKSEL0 = SYSCLK_CLKSEL0_STCLK_HCLK_DIV2 | SYSCLK_CLKSEL0_HCLK_PLL;

    /* Enable IP clock */        
    SYSCLK->APBCLK = SYSCLK_APBCLK_UART0_EN_Msk;

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
    /* Set P3 multi-function pins for UART0 RXD, UART0 TXD, EINT0 and EINT1 */
    SYS->P3_MFP = SYS_MFP_P30_RXD0 | SYS_MFP_P31_TXD0 | SYS_MFP_P32_INT0 | SYS_MFP_P33_INT1;

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
/* MAIN function                                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
int main (void)
{
    int32_t i32Err;

    /* Init System, IP clock and multi-function I/O */
    SYS_Init();     

    /* Init UART0 for printf */
    UART0_Init();

    printf("\n\nCPU @ %dHz\n", SystemCoreClock);
                                
    printf("+-------------------------------------+ \n");
    printf("|    M05xx GPIO Driver Sample Code    | \n");
    printf("+-------------------------------------+ \n");

    /*-----------------------------------------------------------------------------------------------------*/
    /* GPIO Basic Mode Test --- Use Pin Data Input/Output to control GPIO pin                              */
    /*-----------------------------------------------------------------------------------------------------*/    
    printf("  >> Please connect P1.2 and P4.1 first << \n");
    printf("     Press any key to start test by using [Pin Data Input/Output Control] \n\n");
	getchar();
	
    /* Configure P1.2 as Output mode and P4.1 as Input mode then close it */
    _GPIO_SET_PIN_MODE(P1, 2, GPIO_PMD_OUTPUT);
    _GPIO_SET_PIN_MODE(P4, 1, GPIO_PMD_INPUT);
	
    i32Err = 0;
    printf("  GPIO Output/Input test ...... \n");

    /* Use Pin Data Input/Output Control to pull specified I/O or get I/O pin status */
    P12 = 0;
    if (P41 != 0)
	{
		i32Err = 1;
	}

    P12 = 1;
    if (P41 != 1)
	{
		i32Err = 1;
	}

    if ( i32Err )
    { 
	    printf("  [FAIL] --- Please make sure P1.2 and P4.1 are connected. \n");
    }else
    {
        printf("  [OK] \n");
    }
    
    /* Configure P1.2 amd P4.1 to default Quasi-bidirectional mode */
    _GPIO_SET_PIN_MODE(P1, 2, GPIO_PMD_QUASI);
    _GPIO_SET_PIN_MODE(P4, 1, GPIO_PMD_QUASI);


    /*-----------------------------------------------------------------------------------------------------*/
    /* GPIO Interrupt Function Test                                                                        */
    /*-----------------------------------------------------------------------------------------------------*/    
    printf("\n  P1.3, P4.5, P3.2(INT0) and P3.3(INT1) are used to test interrupt \n");    

    /* Configure P1.3 as Input mode and enable interrupt by rising edge trigger */
    _GPIO_SET_PIN_MODE(P1, 3, GPIO_PMD_INPUT);
    GPIO_EnableInt(P1, 3, GPIO_INT_RISING);
    NVIC_EnableIRQ(GPIO_P0P1_IRQn);

    /*  Configure P4.5 as Quasi-bidirection mode and enable interrupt by falling edge trigger */
    _GPIO_SET_PIN_MODE(P4, 5, GPIO_PMD_QUASI);
    GPIO_EnableInt(P4, 5, GPIO_INT_FALLING);
    NVIC_EnableIRQ(GPIO_P2P3P4_IRQn);

    /* Configure P3.2 as EINT0 pin and enable interrupt by falling edge trigger */
    _GPIO_SET_PIN_MODE(P3, 2, GPIO_PMD_INPUT);
    GPIO_EnableInt(P3, 2, GPIO_INT_FALLING);
    NVIC_EnableIRQ(EINT0_IRQn);

    /* Configure P3.3 as EINT1 pin and enable interrupt by rising and falling edge trigger */
    _GPIO_SET_PIN_MODE(P3, 3, GPIO_PMD_INPUT);
    GPIO_EnableInt(P3, 3, GPIO_INT_BOTH_EDGE);
    NVIC_EnableIRQ(EINT1_IRQn);

    /* Enable interrupt de-bounce function and select de-bounce sampling cycle time is 1024 * 10 KHz clock */
    _GPIO_SET_DEBOUNCE_TIME(GPIO_DBNCECON_DBCLKSRC_IRC10K, GPIO_DBNCECON_DBCLKSEL_1024);
    _GPIO_ENABLE_DEBOUNCE(P1, 3);
    _GPIO_ENABLE_DEBOUNCE(P4, 5);
    _GPIO_ENABLE_DEBOUNCE(P3, 2);
    _GPIO_ENABLE_DEBOUNCE(P3, 3);

    /* Waiting for interrupts */
    while (1);
}




