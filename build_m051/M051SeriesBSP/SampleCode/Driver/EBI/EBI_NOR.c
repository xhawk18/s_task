/**************************************************************************//**
 * @file     EBI_NOR.c
 * @version  V2.00
 * $Revision: 1 $
 * $Date: 12/04/05 7:07p $
 * @brief    NOR Flash - W39L010 Driver Source File
 *
 * @note
 * Copyright (C) 2011 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#include <stdio.h>
#include "M051Series.h"

#include "EBI_NOR.h"


/*---------------------------------------------------------------------------------------------------------*/
/* Global file scope (static) variables                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
volatile uint8_t *g_pu8NorBaseAddr;


/**
 * @brief       NOR Flash Initial
 *
 * @param[in]   None
 *
 * @return      None
 *
 * @details     Initial and reset NOR Flash - W39L010.
 */
void NOR_Init(void)
{
	g_pu8NorBaseAddr = (uint8_t *)EBI_START_BASE_ADDR;
	
	// Reset NOR Flash
	NOR_Reset();
}


/**
 * @brief       Reset NOR Flash
 *
 * @param[in]   None
 *
 * @return      None
 *
 * @details     Reset NOR Flash - W39L010.
 */
void NOR_Reset(void)
{
	*(g_pu8NorBaseAddr+0x5555) = 0xAA;
	*(g_pu8NorBaseAddr+0x2AAA) = 0x55;
	*(g_pu8NorBaseAddr+0x0000) = 0xF0;
}


/**
 * @brief       Get ID of NOR Flash
 *
 * @param[in]   None
 *
 * @return      Return Manufacture ID and Device ID
 *
 * @details     Return the ID of W39L010 and Manufacture ID sbould be 0xDA, Device ID should be 0x31.
 */
uint32_t NOR_GetID(void)
{
	uint8_t u8ManuFactureID, u8DeviceID;
	
	*(g_pu8NorBaseAddr+0x5555) = 0xAA;
	*(g_pu8NorBaseAddr+0x2AAA) = 0x55;
	*(g_pu8NorBaseAddr+0x5555) = 0x90;
    SYS_SysTickDelay(10);
	
	u8ManuFactureID = *(g_pu8NorBaseAddr+0x0);
	u8DeviceID 		= *(g_pu8NorBaseAddr+0x1);
		
	*(g_pu8NorBaseAddr+0x5555) = 0xAA;
	*(g_pu8NorBaseAddr+0x2AAA) = 0x55;
	*(g_pu8NorBaseAddr+0x5555) = 0xF0;
    SYS_SysTickDelay(10);
	
	return ((u8ManuFactureID<<8) | u8DeviceID);
}


/**
 * @brief       NOR Flash Chip Erase
 *
 * @param[in]   None
 *
 * @retval      FALSE       Erase command fail
 * @retval      TRUE        Erase command pass
 *
 * @details     W39L010 whole chip erase.
 */
uint8_t NOR_Erase(void)
{
	uint8_t u8Status;
	
	*(g_pu8NorBaseAddr+0x5555) = 0xAA;
	*(g_pu8NorBaseAddr+0x2AAA) = 0x55;
	*(g_pu8NorBaseAddr+0x5555) = 0x80;
	*(g_pu8NorBaseAddr+0x5555) = 0xAA;
	*(g_pu8NorBaseAddr+0x2AAA) = 0x55;
	*(g_pu8NorBaseAddr+0x5555) = 0x10;
	
	u8Status = NOR_CheckCMDComplete(0x0, 0x0);
	if (u8Status == FALSE)
		return FALSE;
	
    SYS_SysTickDelay(200000);
		
	return TRUE;
}


/**
 * @brief       Check Command Complele
 *
 * @param[in]   u32Addr     The address to read out data to check if command compelte
 * @param[in]   u8Data      The data to be compared with the read out data
 *
 * @retval      FALSE       Command fail
 * @retval      TRUE        Command complete
 *
 * @details     Check if the specified command is complete or not.
 */
uint8_t NOR_CheckCMDComplete(uint32_t u32Addr, uint8_t u8Data)
{
	/* Using Data Polling Algorithm to check if command is complete or not */
	uint8_t u8CurData;
    volatile uint32_t u32TimeOutCnts = 0;

    /* Command timeout period is 200 ms */
    u8Data = u8Data & (1<<7); 	// read D7
    while (u32TimeOutCnts < 200000)	
    {
		u8CurData = NOR_ReadData(u32Addr);
		u8CurData = u8CurData & (1<<7);	// read DQ7
		if (u8Data == u8CurData)
		{
			return TRUE;
		}
        SYS_SysTickDelay(1);
		u32TimeOutCnts++;
    }

    return FALSE;
}


/**
 * @brief       Read Data from NOR Flash
 *
 * @param[in]   u32DestAddr The target addres of read command
 *
 * @return      Return the data
 *
 * @details     Read the date from spcified address.
 */
uint8_t NOR_ReadData(uint32_t u32DestAddr)
{
	return *(g_pu8NorBaseAddr + u32DestAddr);
}


/**
 * @brief       Write Data to NOR Flash
 *
 * @param[in]   u32DestAddr The target addres of write command
 * @param[in]   u8Data      The target data for write command
 *
 * @retval      FALSE       Write command fail
 * @retval      TRUE        Write command pass
 *
 * @details     Write the date to spcified address.
 */
uint8_t NOR_WriteData(uint32_t u32DestAddr, uint8_t u8Data)
{
	*(g_pu8NorBaseAddr+0x5555) = 0xAA;
	*(g_pu8NorBaseAddr+0x2AAA) = 0x55;
	*(g_pu8NorBaseAddr+0x5555) = 0xA0;
	
	*(g_pu8NorBaseAddr + u32DestAddr) = u8Data;
	
 	return NOR_CheckCMDComplete(u32DestAddr, u8Data);
}

