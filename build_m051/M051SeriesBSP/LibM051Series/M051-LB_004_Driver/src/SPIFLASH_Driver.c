/**************************************************************************//**
 * @file     SPIFLASH_Driver.c
 * @version  V2.1
 * $Revision: 2 $
 * $Date: 12/08/09 2:23p $
 * @brief    SPI Flash Driver
 *
 * @note
 * Copyright (C) 2011 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#include <stdio.h>
#include "M051Series.h"
#include "LCD_Driver.h"


/* Status */
#define SPIFLASH_STATUS_BUSY       (0x01UL)
#define SPIFLASH_STATUS_WEL        (0x02UL)
#define SPIFLASH_STATUS_BP0        (0x04UL)
#define SPIFLASH_STATUS_BP1        (0x08UL)
#define SPIFLASH_STATUS_BP2        (0x10UL)
#define SPIFLASH_STATUS_TB         (0x20UL)
#define SPIFLASH_STATUS_SEC        (0x40UL)
#define SPIFLASH_STATUS_SRP0       (0x80UL)

#define SPI_PORT     SPI1

#define SPIFLASH_CNTRL(x)  (uint32_t)(SPI_CNTRL_CLK_IDLE_LOW | SPI_CNTRL_TX_FALLING | SPI_CNTRL_RX_RISING | SPI_CNTRL_TX_BIT_LEN(x))

/* Init SPI interface for SPI Flash */
void SPIFLASH_Init(void)
{
    /* Initial SPI1 data format and SPI1 clock */    
    SPI_PORT->CNTRL   = SPIFLASH_CNTRL(8);
    SPI_PORT->DIVIDER = SPI_DIVIDER_DIV(4096) | SPI_DIVIDER_DIV2(4096); /* SPI clock freq = system clock / 4 */
    SPI_PORT->SSR     = SPI_SSR_SW_SS_PIN_HIGH;
}

/* Read manufacturer / device ID */
uint32_t SPIFLASH_ReadId(void)
{
    SPI_PORT->SSR = SPI_SSR_SW_SS_PIN_LOW;
    SPI_PORT->TX[0] = 0x90000000UL;
    SPI_PORT->CNTRL = SPIFLASH_CNTRL(32) | SPI_CNTRL_GO_BUSY;
    while(SPI_PORT->CNTRL & SPI_CNTRL_GO_BUSY);
    SPI_PORT->CNTRL = SPIFLASH_CNTRL(16) | SPI_CNTRL_GO_BUSY;
    while(SPI_PORT->CNTRL & SPI_CNTRL_GO_BUSY);
    SPI_PORT->SSR = SPI_SSR_SW_SS_PIN_HIGH;

    return SPI_PORT->RX[0];

}

/* To set write enable latch (WEL) which is required before page program, sector erase, chip erase and
   write status register */
void SPIFLASH_WriteEnable(void)
{
    SPI_PORT->SSR = SPI_SSR_HW_AUTO_ACTIVE_LOW;
    SPI_PORT->TX[0] = 0x06;
    SPI_PORT->CNTRL = SPIFLASH_CNTRL(8) | SPI_CNTRL_GO_BUSY;
    while(SPI_PORT->CNTRL & SPI_CNTRL_GO_BUSY);
}

/* To disable write enable latch */
void SPIFLASH_WriteDisable(void)
{
    SPI_PORT->SSR = SPI_SSR_HW_AUTO_ACTIVE_LOW;
    SPI_PORT->TX[0] = 0x04;
    SPI_PORT->CNTRL = SPIFLASH_CNTRL(8) | SPI_CNTRL_GO_BUSY;
    while(SPI_PORT->CNTRL & SPI_CNTRL_GO_BUSY);
}

/* To read 8-bit status register 1 */
uint32_t SPIFLASH_ReadStatusReg1(void)
{
    SPI_PORT->SSR = SPI_SSR_HW_AUTO_ACTIVE_LOW;
    SPI_PORT->TX[0] = 0x050000;
    SPI_PORT->CNTRL = SPIFLASH_CNTRL(24) | SPI_CNTRL_GO_BUSY;
    while(SPI_PORT->CNTRL & SPI_CNTRL_GO_BUSY);

    return SPI_PORT->RX[0];
}

/* To read 8-bit status register 2 */
uint32_t SPIFLASH_ReadStatusReg2(void)
{
    SPI_PORT->SSR = SPI_SSR_HW_AUTO_ACTIVE_LOW;
    SPI_PORT->TX[0] = 0x350000;
    SPI_PORT->CNTRL = SPIFLASH_CNTRL(24) | SPI_CNTRL_GO_BUSY;
    while(SPI_PORT->CNTRL & SPI_CNTRL_GO_BUSY);

    return SPI_PORT->RX[0];
}

/* To write status register. Write enable must be set before calling this function */
uint32_t SPIFLASH_WriteStatusReg(void)
{
    SPI_PORT->SSR = SPI_SSR_HW_AUTO_ACTIVE_LOW;
    SPI_PORT->TX[0] = 0x010000;
    SPI_PORT->CNTRL = SPIFLASH_CNTRL(24) | SPI_CNTRL_GO_BUSY;
    while(SPI_PORT->CNTRL & SPI_CNTRL_GO_BUSY);

    return SPI_PORT->RX[0];
}


/* Erase chip. Write enable must be set before calling this function */
void SPIFLASH_ChipErase(void)
{
    SPI_PORT->SSR = SPI_SSR_HW_AUTO_ACTIVE_LOW;
    SPI_PORT->TX[0] = 0x60;
    SPI_PORT->CNTRL = SPIFLASH_CNTRL(8) | SPI_CNTRL_GO_BUSY;
    while(SPI_PORT->CNTRL & SPI_CNTRL_GO_BUSY);
}

/* Polling busy flag of status register */
void SPIFLASH_WaitReady(void)
{
    while (SPIFLASH_ReadStatusReg1()&SPIFLASH_STATUS_BUSY); // check the BUSY bit
}

/* u32Cnt is read count. Each read will get 8 bytes data */
uint32_t SPIFLASH_Read(uint32_t u32Addr, uint32_t *pu32Buf, uint32_t u32Cnt)
{
    int32_t i,j;

    SPI_PORT->SSR = SPI_SSR_SW_SS_PIN_LOW;
    SPI_PORT->TX[0] = 0x03000000UL | u32Addr;
    SPI_PORT->CNTRL = SPIFLASH_CNTRL(32) | SPI_CNTRL_GO_BUSY;
    while(SPI_PORT->CNTRL & SPI_CNTRL_GO_BUSY);
    
    SPI_PORT->TX[0] = 0xFFFFFFFFUL;
    SPI_PORT->TX[1] = 0xFFFFFFFFUL;
    i = 0;
    j = 0;
    do
    {
        SPI_PORT->CNTRL = SPIFLASH_CNTRL(32) | SPI_CNTRL_TX_NUM(2) | SPI_CNTRL_GO_BUSY;
        while(SPI_PORT->CNTRL & SPI_CNTRL_GO_BUSY);
        pu32Buf[i++] = SPI_PORT->RX[0];
        pu32Buf[i++] = SPI_PORT->RX[1];
        j++;
    }while(u32Cnt > j);

    SPI_PORT->SSR = SPI_SSR_SW_SS_PIN_HIGH;

    return i;
}


/* To do page program. Page size is fixed to be 256 bytes. Input buffer must be 4 bytes alignment. 
   Write enable must be called before calling this function. */
uint32_t SPIFLASH_PageProg(uint32_t u32Addr, uint32_t *pu32Buf)
{
    int32_t i,j;
    

    SPI_PORT->SSR = SPI_SSR_SW_SS_PIN_LOW;
    SPI_PORT->TX[0] = 0x02000000UL | u32Addr;
    SPI_PORT->CNTRL = SPIFLASH_CNTRL(32) | SPI_CNTRL_GO_BUSY;
    while(SPI_PORT->CNTRL & SPI_CNTRL_GO_BUSY);
    
    i = 0;
    j = 0;
    do
    {
        SPI_PORT->TX[0] = pu32Buf[i++];
        SPI_PORT->TX[1] = pu32Buf[i++];
        SPI_PORT->CNTRL = SPIFLASH_CNTRL(32) | SPI_CNTRL_TX_NUM(2) | SPI_CNTRL_GO_BUSY;
        while(SPI_PORT->CNTRL & SPI_CNTRL_GO_BUSY);
        j++;
    }while(256/8 > j);

    SPI_PORT->SSR = SPI_SSR_SW_SS_PIN_HIGH;

    return i;
}


/* To erase a sector. One sector is 4k bytes. Write enable must be called before calling this function. */
void SPIFLASH_SectorErase(uint32_t u32Addr)
{
    SPI_PORT->SSR = SPI_SSR_HW_AUTO_ACTIVE_LOW;
    SPI_PORT->TX[0] = 0x20000000 | u32Addr;
    SPI_PORT->CNTRL = SPIFLASH_CNTRL(32) | SPI_CNTRL_GO_BUSY;
    while(SPI_PORT->CNTRL & SPI_CNTRL_GO_BUSY);
}


/* To erase a 32kbytes block. Write enable must be called before calling this function. */
void SPIFLASH_32kBlockErase(uint32_t u32Addr)
{
    SPI_PORT->SSR = SPI_SSR_HW_AUTO_ACTIVE_LOW;
    SPI_PORT->TX[0] = 0x52000000 | u32Addr;
    SPI_PORT->CNTRL = SPIFLASH_CNTRL(32) | SPI_CNTRL_GO_BUSY;
    while(SPI_PORT->CNTRL & SPI_CNTRL_GO_BUSY);
}


/* To erase a 64kbytes block. Write enable must be called before calling this function. */
void SPIFLASH_64kBlockErase(uint32_t u32Addr)
{
    SPI_PORT->SSR = SPI_SSR_HW_AUTO_ACTIVE_LOW;
    SPI_PORT->TX[0] = 0xD8000000 | u32Addr;
    SPI_PORT->CNTRL = SPIFLASH_CNTRL(32) | SPI_CNTRL_GO_BUSY;
    while(SPI_PORT->CNTRL & SPI_CNTRL_GO_BUSY);
}

