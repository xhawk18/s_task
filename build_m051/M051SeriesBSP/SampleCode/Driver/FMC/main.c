/**************************************************************************//**
 * @file     Smpl_DrvFMC.c
 * @version  V2.00
 * $Revision: 4 $
 * $Date: 12/04/23 7:41p $ 
 * @brief    M051 Series Flash Memory Controller Driver Sample Code
 *
 * @note
 * Copyright (C) 2011 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#include <stdio.h>
#include "M051Series.h"

#define PLLCON_SETTING      SYSCLK_PLLCON_50MHz_XTAL
#define PLL_CLOCK           50000000

#define KEY_ADDR    0x20000FFC      /* The location of signature */
#define SIGNATURE   0x21557899      /* The signature word is used by AP code to check if simple LD is finished */

extern uint32_t loaderImageBase;
extern uint32_t loaderImageLimit;

uint32_t g_u32ImageSize;

void SYS_Init(void)
{
/*---------------------------------------------------------------------------------------------------------*/
/* Init System Clock                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Enable External XTAL (4~24 MHz) */
    SYSCLK->PWRCON |= SYSCLK_PWRCON_XTL12M_EN_Msk;

    SYSCLK->PLLCON = PLLCON_SETTING;

    /* Waiting for clock ready */
    SYS_WaitingForClockReady(SYSCLK_CLKSTATUS_PLL_STB_Msk | SYSCLK_CLKSTATUS_XTL12M_STB_Msk);

    /* Switch HCLK clock source to PLL */
    SYSCLK->CLKSEL0 = SYSCLK_CLKSEL0_HCLK_PLL;

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


void FMC_LDROM_Test(void)
{
    int32_t  i32Err;
    uint32_t u32Data, i, j, *pu32Loader;

    /* Enable LDROM Update */
    _FMC_ENABLE_LD_UPDATE();

    printf("  Erase LD ROM ............................... ");
    
    /* Page Erase LDROM */
    for (i = 0; i < FMC_LDROM_SIZE; i += FMC_FLASH_PAGE_SIZE)
        FMC_Erase(FMC_LDROM_BASE + i);
    
    /* Erase Verify */
    i32Err = 0;

    for (i = FMC_LDROM_BASE; i < (FMC_LDROM_BASE+FMC_LDROM_SIZE); i += 4) 
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
    
    printf("  Program LD ROM test ........................ ");
    
    /* Program LDROM and read out data to compare it */
    for(i = FMC_LDROM_BASE; i < (FMC_LDROM_BASE+FMC_LDROM_SIZE); i += 4) 
    {
        FMC_Write(i, i);
    }

    i32Err = 0;
    
    for(i = FMC_LDROM_BASE; i < (FMC_LDROM_BASE+FMC_LDROM_SIZE); i += 4) 
    {
        u32Data = FMC_Read(i);
        
        if (u32Data != i)
        { 
           i32Err = 1;
        }        
    }
    
    if (i32Err)
        printf("[FAIL]\n");
    else
        printf("[OK]\n");

    printf("  Program Simple LD Code ..................... ");
    pu32Loader = (uint32_t *)&loaderImageBase;
    
    for (i=0;i<g_u32ImageSize;i+=FMC_FLASH_PAGE_SIZE)
    {
        FMC_Erase(FMC_LDROM_BASE + i);
        
        for (j=0;j<FMC_FLASH_PAGE_SIZE;j+=4)
        {
            FMC_Write(FMC_LDROM_BASE + i + j, pu32Loader[(i + j) / 4]);
        }
    }

    /* Verify loader */
    i32Err = 0;
    
    for (i=0;i<g_u32ImageSize;i+=FMC_FLASH_PAGE_SIZE)
    {
        for (j=0;j<FMC_FLASH_PAGE_SIZE;j+=4)
        {
            u32Data = FMC_Read(FMC_LDROM_BASE + i + j);
            if (u32Data != pu32Loader[(i+j)/4])
                i32Err = 1;
            
            if (i + j >= g_u32ImageSize)
                break;
        }
    }

    if (i32Err)
    {
        printf("[FAIL]\n");
    }
    else
    {
        printf("[OK]\n");
        
        /* Reset CPU to boot to LD mode */
        printf("\n  >>> Reset to LD mode <<<\n");
        
        /* Make sure message has printed out */
        _UART_WAIT_TX_EMPTY(UART0);

        FMC->ISPCON |= FMC_ISPCON_BS_LDROM;
        
        /* CPU Reset */
        _SYS_RESET_CPU();
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/*  Main Function                                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
int32_t main (void)
{
    uint8_t ch;
    uint32_t u32Data;
  
    /* Init System, IP clock and multi-function I/O */
    SYS_Init();     

    /* Init UART0 for printf */
    UART0_Init();    
  
    /* Unlock protected registers for ISP function */
    SYS_UnlockReg();
    
    /* Enable ISP function */
    FMC->ISPCON |= FMC_ISPCON_ISPEN_Msk;

    /* Check the signature to check if Simple LD code is finished or not */
    if (inp32(KEY_ADDR) == SIGNATURE)
    {
        /* Just clear SIGNATURE and finish the sample code if Simple LD code has been executed. */
        outp32(KEY_ADDR, 0);
        
        /* Read BS */
        printf("  Boot Mode .................................. ");
        
        if ((FMC->ISPCON & FMC_ISPCON_BS_Msk) == FMC_ISPCON_BS_APROM)
            printf("[APROM]\n");
        else
        {
            printf("[LDROM]\n");
            printf("  WARNING: The driver sample code must execute in AP mode!\n");
        }
        goto lexit;
    }
    
    /*
        This sample code will demo some function about FMC:
        1. Check if CPU boots from APROM
        2. Read 96-bit UID 
        3. Erase LDROM, program LD sample code to LDROM, and verify LD sample code
        4. Select next booting from LDROM and run LD sample code
    */

    printf("\n\n");
    printf("+--------------------------------------------------------+\n");
    printf("|    M05xx Flash Memory Controller Driver Sample Code    |\n");
    printf("+--------------------------------------------------------+\n");

    printf("\nCPU @ %dHz\n\n", SystemCoreClock);

    /* Read BS */
    printf("  Boot Mode .................................. ");
    
    if ((FMC->ISPCON & FMC_ISPCON_BS_Msk) == FMC_ISPCON_BS_APROM)
        printf("[APROM]\n");
    else
    {
        printf("[LDROM]\n");
        printf("  WARNING: The driver sample code must execute in AP mode!\n");
        goto lexit;
    }

    /* Read UID */
    printf("  UID[ 0:31] ................................. [0x%x]\n", FMC_ReadUID(0)); 
    printf("  UID[32:63] ................................. [0x%x]\n", FMC_ReadUID(1));    
    printf("  UID[64:95] ................................. [0x%x]\n", FMC_ReadUID(2));

    /* Read Data Flash base address */
    printf("  Data Flash Base Address .................... [0x%x]\n", FMC->DFBADR);

    /* Check the data in LD ROM to avoid overwrite them */
    u32Data = FMC_Read(FMC_LDROM_BASE);
    
    if (u32Data != 0xFFFFFFFF)
    {
        printf("\n  WARNING: There is code in LD ROM.\n  If you proceed, the code in LD ROM will be corrupted.\n");
        printf("  Continue? [y/n]:");
        ch = getchar();
        putchar(ch);
        if (ch != 'y')
            goto lexit;
        printf("\n\n");
    }

    /* Check LD image size */
    g_u32ImageSize = (uint32_t)&loaderImageLimit - (uint32_t)&loaderImageBase;
    
    if (g_u32ImageSize == 0)
    {
        printf("  ERROR: Loader Image is 0 bytes!\n");
        goto lexit;
    }

    if (g_u32ImageSize > FMC_LDROM_SIZE)
    {
        printf("  ERROR: Loader Image is larger than 4KBytes!\n");
        goto lexit;    
    }

    /* Erase LDROM, program LD sample code to LDROM, and verify LD sample code */
    /* The chip will boot from LDROM and run LD sample code if LD sample code is downloaded successfully */
    FMC_LDROM_Test();

lexit:

    /* Disable ISP function */
    FMC->ISPCON &= ~FMC_ISPCON_ISPEN_Msk;

    /* Lock protected registers */
    SYS_LockReg();
    
    printf("\nFMC Sample Code Completed.\n");
}




