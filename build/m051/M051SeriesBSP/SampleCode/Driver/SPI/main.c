/**************************************************************************//**
 * @file     Smpl_DrvSPI.c
 * @version  V2.00
 * $Revision: 4 $
 * $Date: 8/09/12 7:19p $ 
 * @brief    M051 Series SPI Driver Sample Code
 *
 * @note
 * Copyright (C) 2011 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#include <stdio.h>
#include "M051Series.h"

#define PLLCON_SETTING      SYSCLK_PLLCON_50MHz_XTAL
#define PLL_CLOCK           50000000



#define TEST_COUNT 64

uint32_t g_au32SourceData[TEST_COUNT];
uint32_t g_au32DestinationData[TEST_COUNT];

void SYS_Init(void);
void UART0_Init(void);
void SPI_Init(void);
void SpiLoopbackTest(void);
volatile uint32_t SPI1_INT_Flag;

int main(void)
{
    /* Init System, IP clock and multi-function I/O */
    SYS_Init();

    /* Init UART0 for printf */
    UART0_Init();

    /* Init SPI */
    SPI_Init();

    printf("\n\n");
    printf("+----------------------------------------------------------------------+\n");       
    printf("|                       SPI Driver Sample Code                         |\n");
    printf("|                                                                      |\n");
    printf("+----------------------------------------------------------------------+\n");
    printf("\n");
    printf("Configure SPI0 as a master and SPI1 as a slave.\n");
    printf("Bit length of a transaction: 32\n");
    printf("SPI clock rate: %d Hz\n", SPI_GetClock1Freq(SPI0));
    printf("The I/O connection for SPI0/SPI1 loopback:\n");
    printf("    SPI0_SS(P1.4)   <--> SPI1_SS0(P0.4)\n    SPI0_CLK(P1.7)  <--> SPI1_CLK(P0.7)\n");
    printf("    SPI0_MISO(P1.6) <--> SPI1_MISO(P0.6)\n    SPI0_MOSI(P1.5) <--> SPI1_MOSI(P0.5)\n\n");
    printf("Please connect SPI0 with SPI1, and press any key to start transmission ...");
    getchar();
    printf("\n");
    SpiLoopbackTest();

    printf("\n\nExit SPI driver sample code.\n");

    /* Disable SPI0 and SPI1 IP clock */        
    SYSCLK->APBCLK &= (~(SYSCLK_APBCLK_SPI0_EN_Msk | SYSCLK_APBCLK_SPI1_EN_Msk));
    return 0;
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

    /* Enable external 12MHz XTAL, internal 22.1184MHz */
    SYSCLK->PWRCON |= SYSCLK_PWRCON_XTL12M_EN_Msk | SYSCLK_PWRCON_IRC22M_EN_Msk;

    /* Enable PLL and Set PLL frequency */
    SYSCLK->PLLCON = PLLCON_SETTING;
    
    /* Waiting for clock ready */
    SYS_WaitingForClockReady(SYSCLK_CLKSTATUS_PLL_STB_Msk | SYSCLK_CLKSTATUS_XTL12M_STB_Msk  |
                             SYSCLK_CLKSTATUS_IRC22M_STB_Msk);

    /* Switch HCLK clock source to PLL, STCLK to HCLK/2 */
    SYSCLK->CLKSEL0 = SYSCLK_CLKSEL0_STCLK_HCLK_DIV2 | SYSCLK_CLKSEL0_HCLK_PLL;

    /* Enable IP clock */
    SYSCLK->APBCLK = SYSCLK_APBCLK_UART0_EN_Msk | SYSCLK_APBCLK_SPI0_EN_Msk | SYSCLK_APBCLK_SPI1_EN_Msk;
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
    /* Set P0.4, P0.5, P0.6, P0.7 for SPI1 */
    SYS->P0_MFP = SYS_MFP_P04_SPISS1 | SYS_MFP_P05_MOSI_1 | SYS_MFP_P06_MISO_1 | SYS_MFP_P07_SPICLK1;

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

void SPI_Init(void)
{
/*---------------------------------------------------------------------------------------------------------*/
/* Init SPI                                                                                                */
/*---------------------------------------------------------------------------------------------------------*/
    /* Configure SPI0 as a master, clock idle low, falling clock edge Tx, rising edge Rx and 32-bit transaction */
    SPI0->CNTRL = SPI_CNTRL_MASTER_MODE | SPI_CNTRL_CLK_IDLE_LOW | SPI_CNTRL_TX_FALLING |
                     SPI_CNTRL_RX_RISING | SPI_CNTRL_TX_BIT_LEN(32);
    /* Enable the automatic hardware slave select function. Select the SS pin and configure as low-active. */
    SPI0->SSR = SPI_SSR_HW_AUTO_ACTIVE_LOW;
    /* Set IP clock divider. SPI clock rate = HCLK / ((11+1)*2) = 2MHz */
    SPI0->DIVIDER = SPI0->DIVIDER & (~SPI_DIVIDER_DIVIDER_Msk) | SPI_DIVIDER_DIV(11);

    /* Configure SPI1 as a slave, clock idle low, falling clock edge Tx, rising edge Rx and 32-bit transaction */
    SPI1->CNTRL = SPI_CNTRL_SLAVE_MODE | SPI_CNTRL_CLK_IDLE_LOW | SPI_CNTRL_TX_FALLING |
                     SPI_CNTRL_RX_RISING | SPI_CNTRL_TX_BIT_LEN(32);
    /* Configure SPI1's slave select signal as a falling edge trigger device. */
    SPI1->SSR = SPI_SSR_SLAVE_FALLING_EDGE_TRIGGER;
}

void SpiLoopbackTest(void)
{
    uint32_t u32DataCount, u32TestCount, u32Err;
    
    printf("\nSPI0/1 Loopback test ");
    
    /* Enable the SPI1 unit transfer interrupt. */
    SPI1->CNTRL |= SPI_CNTRL_IE_Msk;
    NVIC_EnableIRQ(SPI1_IRQn);
    
    /* Clear Tx register of SPI1 to avoid send non-zero data to Master. Just for safe. */
    _SPI_WRITE_TX_BUFFER0(SPI1, 0);
    
    u32Err = 0;
    for(u32TestCount=0; u32TestCount<10000; u32TestCount++)
    {
        /* set the soure data and clear the destination buffer */
        for(u32DataCount=0; u32DataCount<TEST_COUNT; u32DataCount++)
        {
            g_au32SourceData[u32DataCount] = u32DataCount;
            g_au32DestinationData[u32DataCount] = 0;
        }
        
        u32DataCount=0;
        SPI1_INT_Flag = 0;

        if((u32TestCount&0x1FF) == 0)
        {
            putchar('.');
        }
        /* set the GO_BUSY bit of SPI1 */
        _SPI_SET_GO(SPI1);
        /* write the first data of source buffer to Tx register of SPI0. And start transmission. */
        _SPI_WRITE_TX_BUFFER0(SPI0, g_au32SourceData[0]);
        _SPI_SET_GO(SPI0);
    
        while(1)
        {
            if(SPI1_INT_Flag==1)
            {
                SPI1_INT_Flag = 0;
                
                if(u32DataCount<(TEST_COUNT-1))
                {
                    /* Read the previous retrived data and trigger next transfer. */
                    g_au32DestinationData[u32DataCount] = _SPI_GET_RX0_DATA(SPI1);
                    _SPI_SET_GO(SPI1);
                    u32DataCount++;
                    /* Write data to SPI0 Tx buffer and trigger the transfer */
                    _SPI_WRITE_TX_BUFFER0(SPI0, g_au32SourceData[u32DataCount]);
                    _SPI_SET_GO(SPI0);
                }
                else
                {
                    /* Just read the previous retrieved data but trigger next transfer, because this is the last transfer. */
                    g_au32DestinationData[u32DataCount] = _SPI_GET_RX0_DATA(SPI1);
                    break;
                }
            }
        }
        
        /*  Check the received data */
        for(u32DataCount=0; u32DataCount<TEST_COUNT; u32DataCount++)
        {
            if(g_au32DestinationData[u32DataCount]!=g_au32SourceData[u32DataCount])
                u32Err = 1;
        }

        if(u32Err)
            break;
    }

    if(u32Err)
        printf(" [FAIL]\n\n");
    else
        printf(" [PASS]\n\n");

    return ;
}

void SPI1_IRQHandler(void)
{
    if( _SPI_GET_UNIT_TRANS_INT_FLAG(SPI1) == 1 ) /* Check the one transaction done interrupt flag */
    {
        _SPI_CLR_UNIT_TRANS_INT_FLAG(SPI1);   /* write '1' to clear SPI1 interrupt flag */
        SPI1_INT_Flag = 1;
    }
}

