/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include "M051Series.h"
#include "LCD_Driver.h"

#define PLLCON_SETTING      SYSCLK_PLLCON_50MHz_XTAL
#define PLL_CLOCK           50000000



#define LDROM_BASE      FMC_LDROM_BASE
#define PAGE_SIZE       FMC_FLASH_PAGE_SIZE

volatile uint8_t g_u8IsPress = FALSE;

#ifndef LDROM
extern uint32_t g_u32LoaderImageBase;
extern uint32_t g_u32LoaderImageLimit;
#endif

void EINT0_IRQHandler(void)
{
    /* Clear P3.2 interrupt flag */
    P3->ISRC = 1 << 2;

    g_u8IsPress = TRUE;
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
    SYSCLK->AHBCLK = SYSCLK_AHBCLK_ISP_EN_Msk;
    SYSCLK->APBCLK = SYSCLK_APBCLK_UART0_EN_Msk | SYSCLK_APBCLK_SPI0_EN_Msk;
                        
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
    /* Set P3 multi-function pins for UART0 RXD and TXD  */
    SYS->P3_MFP = SYS_MFP_P30_RXD0 | SYS_MFP_P31_TXD0;
    /* Set P1.4, P1.5, P1.6, P1.7 for SPI0 */
    SYS->P1_MFP = SYS_MFP_P14_SPISS0 | SYS_MFP_P15_MOSI_0 | SYS_MFP_P16_MISO_0 | SYS_MFP_P17_SPICLK0;

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


void GPIO_Init(void)
{
    /*set P2.0 as output*/
    P2->PMD = GPIO_PMD_PMD0_OUTPUT;

    /* Enable debunce function of P3.2 (EINT0) */
    P3->DBEN = GPIO_DBEN_ENABLE(2);
    /* Set debounce time. it is about 6.4 ms */
    GPIO->DBNCECON = GPIO_DBNCECON_DBCLKSRC_IRC10K | GPIO_DBNCECON_DBCLKSEL_64;
    /*Initialize external interrupt*/
    GPIO_EnableInt(P3, 2, GPIO_INT_FALLING);
    NVIC_EnableIRQ(EINT0_IRQn);
}

#ifndef LDROM

/*---------------------------------------------------------------------------------------------------------*/
/*  Main Function                                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
int32_t main(void)
{
    int32_t  i32Err = 0;
    uint32_t u32Data, u32ImageSize, i, j, *pu32Loader;

    /* Init system, IP clock and multi-function I/O */
    SYS_Init();

    /* Init UART0 for printf */
    UART0_Init();

    /* Init GPIO P2.0 (output) and P3.2 (EINT0) */
    GPIO_Init();

    /* Init SPI0 and LCD */
    LCD_Init();
    LCD_EnableBackLight();
    LCD_ClearScreen();


    /* 
        This sample shows how to switch between APROM and LDROM. 
        Target: 
            Smpl_FMC
                Smpl_FMC is the code for APROM. It will program the firmware to LDROM and
                user can press SW_INT to change to boot from LDROM. 
                In APROM, the LED blanking interval is 200ms.
            Smpl_LDROM
                Smpl_LDROM is the code for LDROM. User can press SW_INT to change to boot from APROM.
                In LDROM, the LED blanking interval is 1000ms.
    */
    LCD_Print(0, "Boot from APROM");
    LCD_Print(1, "Press SW_INT   ");
    while (1)
    {
        if(g_u8IsPress)
        {
            g_u8IsPress = FALSE;

            /* Unlock protected registers to write ISP Control Register (ISPCON) */
            SYS_UnlockReg();

            /* Program sample LD code to LDROM */

            /* Enable ISP LDROM update function */
            FMC->ISPCON = FMC_ISPCON_LDUEN_Msk | FMC_ISPCON_ISPEN_Msk;

            /* Page Erase LDROM */
            for (i = 0;i < 4096;i += PAGE_SIZE)
                FMC_Erase(LDROM_BASE + i);


            /* Erase Verify */
            i32Err = 0;
            for (i = LDROM_BASE; i < (LDROM_BASE + 4096); i += 4)
            {
                u32Data = FMC_Read(i);

                if (u32Data != 0xFFFFFFFF)
                {
                    i32Err = 1;
                }
            }


            u32ImageSize = (uint32_t)&g_u32LoaderImageLimit - (uint32_t)&g_u32LoaderImageBase;

            pu32Loader = (uint32_t *)&g_u32LoaderImageBase;
            for (i = 0;i < u32ImageSize;i += PAGE_SIZE)
            {
                FMC_Erase(LDROM_BASE + i);
                for (j = 0;j < PAGE_SIZE;j += 4)
                {
                    FMC_Write(LDROM_BASE + i + j, pu32Loader[(i + j) / 4]);
                }
            }

            /* Verify loader */
            i32Err = 0;
            for (i = 0;i < u32ImageSize;i += PAGE_SIZE)
            {
                for (j = 0;j < PAGE_SIZE;j += 4)
                {
                    u32Data = FMC_Read(LDROM_BASE + i + j);
                    if (u32Data != pu32Loader[(i+j)/4])
                        i32Err = 1;

                    if (i + j >= u32ImageSize)
                        break;
                }


            }

            if(i32Err)
            {
                LCD_ClearScreen();
                LCD_Print(0,"LDROM write fail");
            }
            else
            {
                /* Switch to boot from LDROM */
                FMC->ISPCON = FMC_ISPCON_BS_LDROM;
                _SYS_RESET_CPU();
            }

            while (1);
        }
        else
        {
            /* LED blanking for 200ms */
            P2->DOUT ^= 1;
            SYS_SysTickDelay(100000);
        }

        

    }
}

#else
/*---------------------------------------------------------------------------------------------------------*/
/*  Main Function for LDROM                                                                                */
/*---------------------------------------------------------------------------------------------------------*/
int32_t main(void)
{

    /* Init system, IP clock and multi-function I/O */
    SYS_Init();

    /* Init UART0 for printf */
    UART0_Init();

    /* Init GPIO P2.0 (output) and P3.2 (EINT0) */
    GPIO_Init();

    /* Init SPI0 and LCD */
    LCD_Init();
    LCD_EnableBackLight();
    LCD_ClearScreen();

    LCD_Print(0, "Boot from LDROM");
    LCD_Print(1, "Press SW_INT   ");

    while (1)
    {
        if (g_u8IsPress)
        {
            g_u8IsPress = FALSE;
            
            /* Switch to boot from APROM */
            SYS_UnlockReg();
            FMC->ISPCON = FMC_ISPCON_BS_APROM;
            _SYS_RESET_CPU();
            while (1);
        }
        else
        {
            /* LED blanking for 1000ms */
            P2->DOUT ^= 1;
            SYS_SysTickDelay(300000);
            SYS_SysTickDelay(200000);
        }
    }
}

#endif


