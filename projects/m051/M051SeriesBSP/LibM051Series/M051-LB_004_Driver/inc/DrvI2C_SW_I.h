/**************************************************************************//**
 * @file     Flash_25Q16.h
 * @version  V2.1
 * $Revision: 2 $
 * $Date: 12/04/24 1:56p $
 * @brief    Flash 25Q16 Driver Header File
 *
 * @note
 * Copyright (C) 2011 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#ifndef _DRVI2C_SW_I_H
#define _DRVI2C_SW_I_H

#include "M051Series.h"


/*---------------------------------------------------------------------------------------------------------*/
/*  Define Version number								                                                   */
/*---------------------------------------------------------------------------------------------------------*/
#define DRVI2C_SW_I_MAJOR_NUM	1
#define DRVI2C_SW_I_MINOR_NUM	01
#define DRVI2C_SW_I_BUILD_NUM	001

/*---------------------------------------------------------------------------------------------------------*/
/*  Version define with SysInfra				                                                           */
/*---------------------------------------------------------------------------------------------------------*/
#define DRVI2C_SW_I_VERSION_NUM     _SYSINFRA_VERSION(DRVI2C_MAJOR_NUM, DRVI2C_MINOR_NUM, DRVI2C_BUILD_NUM)

uint32_t DrvI2C_SW_I_Open(uint32_t u32BusClock);
uint32_t DrvI2C_SW_I_Send(uint8_t u8Address, uint8_t* p8Data, uint32_t u32ByteSize);
uint32_t DrvI2C_SW_I_Get(uint8_t u8Address, uint8_t* p8Data, uint32_t u32ByteSize);
uint32_t DrvI2C_SW_I_IsBZ(void);
uint32_t DrvI2C_SW_I_Count(void);
#endif

