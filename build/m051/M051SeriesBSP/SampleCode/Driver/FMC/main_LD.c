/**************************************************************************//**
 * @file     Smpl_DrvFMC_SimpleLD.c
 * @version  V2.00
 * $Revision: 3 $
 * $Date: 12/04/19 7:21p $ 
 * @brief    M051 Series Flash Memory Controller Driver Sample Code on LDROM
 *
 * @note
 * Copyright (C) 2011 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#include <stdio.h>
#include "M051Series.h"

#define PLLCON_SETTING      SYSCLK_PLLCON_50MHz_XTAL
#define PLL_CLOCK           50000000

#define KEY_ADDR            0x20000FFC  /* The location of signature */
#define SIGNATURE           0x21557899  /* The signature word is used by AP code to check if simple LD is finished */

#define CONFIG0_TEST_CODE   0x0F9000FF


void SYS_Init(void)
{
/*---------------------------------------------------------------------------------------------------------*/
/* Init System Clock                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Enable External XTAL (4~24 MHz) and IRC 22.1184 MHz */
    SYSCLK->PWRCON |= (SYSCLK_PWRCON_XTL12M_EN_Msk | SYSCLK_PWRCON_IRC22M_EN_Msk);

    SYSCLK->PLLCON = PLLCON_SETTING;

    /* Waiting for clock ready */
    SYS_WaitingForClockReady(SYSCLK_CLKSTATUS_PLL_STB_Msk | SYSCLK_CLKSTATUS_XTL12M_STB_Msk | SYSCLK_CLKSTATUS_IRC22M_STB_Msk);

    /* Switch HCLK clock source to IRC 22.1184 MHz */
    SYSCLK->CLKSEL0 = SYSCLK_CLKSEL0_HCLK_IRC22M;

    /* Enable IP clock */        
    SYSCLK->APBCLK = SYSCLK_APBCLK_UART0_EN_Msk;
    
    /* Select IP clock source */
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

void FMC_APROM_Test(void)
{
    int32_t  i32Err;
    uint32_t u32Data, i;
    uint32_t u32apBase, u32apLimit;

    u32apBase = 0x4000;
    u32apLimit= 0x8000;

    printf("  Erase AP ROM:0x%x..0x%x ................ ", u32apBase, u32apLimit-1);
    
    /* Page Erase APROM */
    for (i = u32apBase;i < u32apLimit; i+=FMC_FLASH_PAGE_SIZE)
        FMC_Erase(i);
    
    /* Erase Verify */
    i32Err = 0;
    
    for (i = u32apBase; i < u32apLimit; i += 4) 
    {     
        u32Data = FMC_Read(i);

        if (u32Data != 0xFFFFFFFF)
        { 
            i32Err = 1;
        }
    }
    
    if (i32Err)
        printf("[FAIL]\n");
    else
        printf("[OK]\n");
    

    printf("  Program AP ROM:0x%x..0x%x .............. ", u32apBase, u32apLimit-1);
    /* Program AP ROM and read out data to compare it */
    for (i = u32apBase; i < u32apLimit; i += 4) 
    {
        FMC_Write(i, i);
    }

    i32Err = 0;
    
    for (i = u32apBase; i < u32apLimit; i += 4) 
    {
        u32Data = FMC_Read(i);
        
        if(u32Data != i)
        { 
           i32Err = 1;
        }
    }
    
    if(i32Err)
        printf("[FAIL]\n");
    else
        printf("[OK]\n");
}

void FMC_CONFIG_Test(void)
{
    int32_t  i32Err;
    uint32_t u32Data, u32config0Bak;

    /* Read and backup the configuration settings */
    printf("  Read config0 ............................... ");
    u32config0Bak = FMC_Read(FMC_CONFIG_BASE);
    printf("[0x%x]\n", u32config0Bak);

    /* Configuration region write test */
    printf("  Erase config region ........................ ");
    
    /* Enable CONFIG Update Function */
    _FMC_ENABLE_CFG_UPDATE();
    FMC_Erase(FMC_CONFIG_BASE);
    u32Data = FMC_Read(FMC_CONFIG_BASE);

    if (u32Data != 0xFFFFFFFF)
        printf("[FAIL]\n");
    else
        printf("[OK]\n");

    printf("  Program config region ...................... ");
    FMC_Write(FMC_CONFIG_BASE, CONFIG0_TEST_CODE);
    
    i32Err = 0;    
    u32Data = FMC_Read(FMC_CONFIG_BASE);
    if (u32Data != CONFIG0_TEST_CODE)
        i32Err = 1;

    if(i32Err)
        printf("[FAIL]\n");
    else
        printf("[OK]\n");

    printf("  Restore CONFIG settings .................... ");
    FMC_Erase(FMC_CONFIG_BASE);
    FMC_Write(FMC_CONFIG_BASE, u32config0Bak);
    
    /* Disable CONFIG Update Function */
    _FMC_DISABLE_CFG_UPDATE();

    i32Err = 0;
    u32Data = FMC_Read(FMC_CONFIG_BASE);
    
    if (u32Data != u32config0Bak)
        i32Err = 1;
    
    if (i32Err)
        printf("[FAIL]\n");
    else
        printf("[OK]\n");
}

/*---------------------------------------------------------------------------------------------------------*/
/*  Main Function                                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
int32_t main (void)
{
    /* Init System, IP clock and multi-function I/O */
    SYS_Init();     

    /* Init UART0 for printf */
    UART0_Init();  

    /*
        This sample code will excute sample code on LDROM and demo some function about FMC:
        1. Change HCLK clock from PLL to IRC, and enable low frequency optimization mode 
           to improve efficiency when chip work at lower frequency.  
        2. Check if CPU boots from LDROM
        3. Erase/Program/Verify APROM from address 0x4000 to 0x8000
        3. Erase/Program/Verify CONFIG0, and restore CONFIG0 value
        4. Select next booting from APROM and re-run APROM sample code 
    */

    printf("+------------------------------------------------------------------+\n");
    printf("|    M05xx Flash Memory Controller Driver Sample Code for LDROM    |\n");
    printf("+------------------------------------------------------------------+\n");

    printf("\nCPU @ %dHz\n\n", SystemCoreClock);

    /* Unlock protected registers for ISP function */
    SYS_UnlockReg();
    
    /* Enable Flash Access Low Frequency Optimization Mode */
    _FMC_ENABLE_LOW_FREQ_OPTIMIZE_MODE();

    /* Enable ISP function */
    FMC->ISPCON |= FMC_ISPCON_ISPEN_Msk;

    /* Read BS */
    printf("  Boot Mode .................................. ");
    
    if ((FMC->ISPCON & FMC_ISPCON_BS_Msk) == FMC_ISPCON_BS_APROM)
    {
        printf("[APROM]\n");
        
        printf("  WARNING: This code should be in LD mode but it is in AP mode now.\n");
        while(1); 
    }
    else
        printf("[LDROM]\n");

    /* Read Data Flash base address */
    printf("  Data Flash Base Address .................... [0x%x]\n", FMC->DFBADR);
   
    /* Erase/Program/Verify APROM form address 0x4000 to 0x8000 */
    FMC_APROM_Test();

    /* Erase/Program/Verify CONFIG0, and restore CONFIG0 value */
    FMC_CONFIG_Test();
   
    /* Write the signature to tell AP code that LD code has finished */
    outp32(KEY_ADDR, SIGNATURE);

    printf("\nFMC Simple LD Code Completed.\n");

    printf("\n  >>> Reset to back to AP mode <<< \n");

    /* Make sure message has printed out */
    _UART_WAIT_TX_EMPTY(UART0);
    
    /* Disable Flash Access Low Frequency Optimization Mode */
    _FMC_DISABLE_LOW_FREQ_OPTIMIZE_MODE();

    FMC->ISPCON &= ~FMC_ISPCON_BS_Msk;
    
    /* Disable ISP function */
    FMC->ISPCON &= ~FMC_ISPCON_ISPEN_Msk;

    /* CPU Reset */
    _SYS_RESET_CPU();

    /* Lock protected registers */
    SYS_LockReg();
}




