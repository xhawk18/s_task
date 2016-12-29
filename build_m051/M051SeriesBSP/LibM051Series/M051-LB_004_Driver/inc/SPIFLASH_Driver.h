/**************************************************************************//**
 * @file     SPIFLASH_Driver.h
 * @version  V2.1
 * $Revision: 2 $
 * $Date: 12/04/24 1:56p $
 * @brief    SPI Flash Driver Header File
 *
 * @note
 * Copyright (C) 2011 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#ifndef __SPI_FLASH_H__
#define __SPI_FLASH_H__

void SPIFLASH_Init(void);
uint32_t SPIFLASH_ReadId(void);
void SPIFLASH_WriteEnable(void);
void SPIFLASH_64kBlockErase(uint32_t u32Addr);
void SPIFLASH_32kBlockErase(uint32_t u32Addr);
void SPIFLASH_SectorErase(uint32_t u32Addr);
uint32_t SPIFLASH_PageProg(uint32_t u32Addr, uint32_t *pu32Buf);
uint32_t SPIFLASH_Read(uint32_t u32Addr, uint32_t *pu32Buf, uint32_t u32Cnt);
void SPIFLASH_WaitReady(void);
void SPIFLASH_ChipErase(void);
uint32_t SPIFLASH_WriteStatusReg(void);
uint32_t SPIFLASH_ReadStatusReg2(void);
uint32_t SPIFLASH_ReadStatusReg1(void);
void SPIFLASH_WriteDisable(void);


#endif //__SPI_FLASH_H__


