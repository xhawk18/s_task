/**************************************************************************//**
 * @file     system_M051Series.c
 * @version  V2.00
 * $Revision: 2 $
 * $Date: 12/04/19 7:37p $
 * @brief    M051 Series System Setting Source File
 *
 * @note
 * Copyright (C) 2011 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include "M051Series.h"



/*----------------------------------------------------------------------------
  Clock Variable definitions
 *----------------------------------------------------------------------------*/
uint32_t SystemCoreClock  = __HSI;              /*!< System Clock Frequency (Core Clock) */
uint32_t CyclesPerUs      = (__HSI / 1000000);  /*!< Cycles per micro second             */
uint32_t PllClock         = __HSI;              /*!< PLL Output Clock Frequency          */
const uint32_t gau32ClkSrcTbl[] = {__XTAL, 0, __HSI, __IRC10K, 0, 0, 0, __IRC22M};


/**
 * @brief    Update the Variable SystemCoreClock
 *
 * @param    None
 *
 * @return   None
 *
 * @details  This function is used to update the variable SystemCoreClock
 *           and must be called whenever the core clock is changed.
 */
void SystemCoreClockUpdate (void) 
{
    uint32_t u32Freq, u32ClkSrc;
    uint32_t u32HclkDiv;

    /* Update PLL Clock */
    PllClock = DrvSYS_GetPLLClockFreq();
 
    u32ClkSrc = SYSCLK->CLKSEL0 & SYSCLK_CLKSEL0_HCLK_S_Msk;

    if(u32ClkSrc != SYSCLK_CLKSEL0_HCLK_PLL)
    {
        /* Use the clock sources directly */
        u32Freq = gau32ClkSrcTbl[u32ClkSrc];
    }
    else
    {
        /* Use PLL clock */
        u32Freq = PllClock;
    }
 
    u32HclkDiv = (SYSCLK->CLKDIV & SYSCLK_CLKDIV_HCLK_N_Msk) + 1; 
    
    /* Update System Core Clock */
    SystemCoreClock = u32Freq/u32HclkDiv;

    CyclesPerUs = (SystemCoreClock + 500000) / 1000000;
}


/**
 * @brief    System Initializaiton
 *
 * @param    None
 *
 * @return   None
 *
 * @details  The necessary initializaiton of system. Global variables are forbidden here.
 */
void SystemInit(void)
{
#ifdef INIT_SYSCLK_AT_BOOTING
    int32_t i32TimeoutCnt;
    uint32_t u32HclkSelect;
    int8_t i8IsPllEn;

    PllClock = 0;
    i8IsPllEn = 0;
    u32HclkSelect = SYSCLK->CLKSEL0 & SYSCLK_CLKSEL0_HCLK_S_Msk;
    if(u32HclkSelect == SYSCLK_CLKSEL0_HCLK_XTAL)
    {
        /* Set to 50MHz system clock frequency when clock source is from external 12MHz */
        SYSCLK->PLLCON = SYSCLK_PLLCON_50MHz_XTAL;       
        
        /* Waiting for PLL ready */
        i32TimeoutCnt = (__XTAL / 1000); /* Timeout is about 1ms */
        while((SYSCLK->CLKSTATUS & SYSCLK_CLKSTATUS_PLL_STB_Msk) == 0 )
        {
            if(i32TimeoutCnt-- <= 0)
                break;    
        }

        i8IsPllEn = 1;
    }
    else if(u32HclkSelect == SYSCLK_CLKSEL0_HCLK_IRC22M)
    {
        /* Set to 50.1918MHz system clock frequency when clock source is from internal 22.1184MHz RC clock */
        SYSCLK->PLLCON = SYSCLK_PLLCON_50MHz_IRC22M;       
        
        /* Waiting for PLL ready */
        i32TimeoutCnt = (__IRC22M / 1000); /* Timeout is about 1ms */
        while((SYSCLK->CLKSTATUS & SYSCLK_CLKSTATUS_PLL_STB_Msk) == 0 )
        {
            if(i32TimeoutCnt-- <= 0)
                break;    
        }

        i8IsPllEn = 1;
    }

    if(i8IsPllEn)    
    {
        /* Set PLL as HCLK clock source (HCLK_S is locked setting)*/
        SYS_UnlockReg();
        SYSCLK->CLKSEL0 = SYSCLK_CLKSEL0_HCLK_PLL;
        SYS_LockReg();
    }
#endif
}

#if USE_ASSERT

/**
 * @brief      Assert Error Message
 *
 * @param[in]  file  the source file name
 * @param[in]  line  line number
 *
 * @return     None
 *
 * @details    The function prints the source file name and line number where
 *             the ASSERT_PARAM() error occurs, and then stops in an infinite loop.
 */
void AssertError(uint8_t * file, uint32_t line)
{
  
    printf("[%s] line %d : wrong parameters.\r\n", file, line);

    /* Infinite loop */
    while(1) ;
}
#endif
