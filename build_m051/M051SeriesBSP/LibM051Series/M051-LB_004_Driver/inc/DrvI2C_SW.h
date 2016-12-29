/**************************************************************************//**
 * @file     DrvI2C_SW.h
 * @version  V2.1
 * $Revision: 2 $
 * $Date: 12/04/24 1:56p $
 * @brief    Software I2C Driver Header File
 *
 * @note
 * Copyright (C) 2011 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#ifndef _DRVI2C_SW_H
#define _DRVI2C_SW_H

#include "M051Series.h"


/*---------------------------------------------------------------------------------------------------------*/
/*  Define Version number								                                                   */
/*---------------------------------------------------------------------------------------------------------*/
#define DRVI2C_SW_MAJOR_NUM	1
#define DRVI2C_SW_MINOR_NUM	01
#define DRVI2C_SW_BUILD_NUM	001

/*---------------------------------------------------------------------------------------------------------*/
/*  Version define with SysInfra				                                                           */
/*---------------------------------------------------------------------------------------------------------*/
#define DRVI2C_SW_VERSION_NUM     _SYSINFRA_VERSION(DRVI2C_MAJOR_NUM, DRVI2C_MINOR_NUM, DRVI2C_BUILD_NUM)

int32_t DrvI2C_SW_Open(uint32_t u32BusClock);
int32_t DrvI2C_SW_Send(uint8_t u8Address, uint8_t* p8Data, uint32_t u32ByteSize);
int32_t DrvI2C_SW_Get(uint8_t u8Address, uint8_t* p8Data, uint32_t u32ByteSize);
#endif

