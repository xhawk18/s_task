/**************************************************************************//**
 * @file     EBI_NOR.h
 * @version  V2.00
 * $Revision: 1 $
 * $Date: 12/04/05 7:07p $
 * @brief    NOR Flash - W39L010 Driver Header File
 *
 * @note
 * Copyright (C) 2011 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#ifndef __EBI_NOR_H__
#define __EBI_NOR_H__


/*---------------------------------------------------------------------------------------------------------*/
/* Define EBI_NOR Functions Prototype                                                                      */
/*---------------------------------------------------------------------------------------------------------*/
void NOR_Init(void);
void NOR_Reset(void);
uint32_t NOR_GetID(void);
uint8_t NOR_Erase(void);
uint8_t NOR_CheckCMDComplete(uint32_t u32DestAddr, uint8_t u8Data);
uint8_t NOR_ReadData(uint32_t u32DestAddr);
uint8_t NOR_WriteData(uint32_t u32DestAddr, uint8_t u8Data);

#endif  // __EBI_NOR_H__

