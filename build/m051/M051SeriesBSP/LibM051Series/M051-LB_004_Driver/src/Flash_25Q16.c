/**************************************************************************//**
 * @file     Flash_25Q16.c
 * @version  V2.1
 * $Revision: 2 $
 * $Date: 12/08/09 2:23p $
 * @brief    SPI Flash 25Q16 Driver
 *
 * @note
 * Copyright (C) 2011 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#include <stdio.h>
#include "M051Series.h"
#include "Driver\DrvGPIO.h"
#include "Driver\DrvSYS.h"
#include "DrvSPI.h"
#include "Flash_25Q16.h"
#define TEST_NUMBER 1 /* page numbers */
// **************************************
// For W25Q16BV, Manufacturer ID: 0xEF; Device ID: 0x14
// For W26X16, Manufacturer ID: 0xEF; Device ID: 0x14
uint32_t SpiFlash_ReadMidDid(void)
{
    uint32_t au32SourceData;
    uint32_t au32DestinationData;
    
    // configure transaction length as 8 bits
    DrvSPI_SetBitLength(eDRVSPI_PORT1, 8);

    // /CS: active
    DrvSPI_SetSS(eDRVSPI_PORT1);

    // send Command: 0x90, Read Manufacturer/Device ID
    au32SourceData = 0x90;
    DrvSPI_SingleWrite(eDRVSPI_PORT1, &au32SourceData);

    // wait
    while (DrvSPI_IsBusy(eDRVSPI_PORT1));

    // configure transaction length as 24 bits
    DrvSPI_SetBitLength(eDRVSPI_PORT1, 24);

    // send 24-bit '0', dummy
    au32SourceData = 0x0;
    DrvSPI_SingleWrite(eDRVSPI_PORT1, &au32SourceData);

    // wait
    while (DrvSPI_IsBusy(eDRVSPI_PORT1));

    // configure transaction length as 16 bits
    DrvSPI_SetBitLength(eDRVSPI_PORT1, 16);

    // receive
    au32SourceData = 0x0;
    DrvSPI_SingleWrite(eDRVSPI_PORT1, &au32SourceData);

    // wait
    while (DrvSPI_IsBusy(eDRVSPI_PORT1));

    // /CS: de-active
    DrvSPI_ClrSS(eDRVSPI_PORT1);

    DrvSPI_DumpRxRegister(eDRVSPI_PORT1, &au32DestinationData, 1);

    if ((au32DestinationData & 0xffff) == 0xEF14)
        printf("MID & DID=0xEF14\n");
    else
        printf("MID & DID Error!\n");
    return  (au32DestinationData & 0xffff);

}
// **************************************
void SpiFlash_ChipErase(void)
{
    uint32_t au32SourceData;

    // configure transaction length as 8 bits
    DrvSPI_SetBitLength(eDRVSPI_PORT1, 8);

    // /CS: active
    DrvSPI_SetSS(eDRVSPI_PORT1);

    // send Command: 0x06, Write enable
    au32SourceData = 0x06;
    DrvSPI_SingleWrite(eDRVSPI_PORT1, &au32SourceData);

    // wait
    while (DrvSPI_IsBusy(eDRVSPI_PORT1));

    // /CS: de-active
    DrvSPI_ClrSS(eDRVSPI_PORT1);

    // /CS: active
    DrvSPI_SetSS(eDRVSPI_PORT1);

    // send Command: 0xC7, Chip Erase
    au32SourceData = 0xc7;
    DrvSPI_SingleWrite(eDRVSPI_PORT1, &au32SourceData);

    // wait
    while (DrvSPI_IsBusy(eDRVSPI_PORT1));

    // /CS: de-active
    DrvSPI_ClrSS(eDRVSPI_PORT1);
}
// **************************************
uint32_t SpiFlash_ReadStatusReg1(void)
{
    uint32_t au32SourceData;
    uint32_t au32DestinationData;

    // configure transaction length as 16 bits
    DrvSPI_SetBitLength(eDRVSPI_PORT1, 16);

    // /CS: active
    DrvSPI_SetSS(eDRVSPI_PORT1);

    // send Command: 0x05, Read status register 1
    au32SourceData = 0x0500;
    DrvSPI_SingleWrite(eDRVSPI_PORT1, &au32SourceData);

    // wait
    while (DrvSPI_IsBusy(eDRVSPI_PORT1));

    // /CS: de-active
    DrvSPI_ClrSS(eDRVSPI_PORT1);

    // dump Rx register
    DrvSPI_DumpRxRegister(eDRVSPI_PORT1, &au32DestinationData, 1);

    return (au32DestinationData & 0xFF);
}
// **************************************
void SpiFlash_WaitReady(void)
{
    uint32_t ReturnValue;

    do
    {
        ReturnValue = SpiFlash_ReadStatusReg1();
        ReturnValue = ReturnValue & 1;
    }
    while (ReturnValue != 0); // check the BUSY bit

}
// **************************************
void SpiFlash_PageProgram(uint8_t *DataBuffer, uint32_t StartAddress, uint32_t ByteCount)
{
    uint32_t au32SourceData;
    uint32_t Counter;

    // configure transaction length as 8 bits
    DrvSPI_SetBitLength(eDRVSPI_PORT1, 8);

    // /CS: active
    DrvSPI_SetSS(eDRVSPI_PORT1);

    // send Command: 0x06, Write enable
    au32SourceData = 0x06;
    DrvSPI_SingleWrite(eDRVSPI_PORT1, &au32SourceData);

    // wait
    while (DrvSPI_IsBusy(eDRVSPI_PORT1));

    // /CS: de-active
    DrvSPI_ClrSS(eDRVSPI_PORT1);

    // /CS: active
    DrvSPI_SetSS(eDRVSPI_PORT1);

    // send Command: 0x02, Page program
    au32SourceData = 0x02;
    DrvSPI_SingleWrite(eDRVSPI_PORT1    , &au32SourceData);

    // wait
    while (DrvSPI_IsBusy(eDRVSPI_PORT1));

    // configure transaction length as 24 bits
    DrvSPI_SetBitLength(eDRVSPI_PORT1, 24);

    // send 24-bit start address
    au32SourceData = StartAddress;
    DrvSPI_SingleWrite(eDRVSPI_PORT1, &au32SourceData);

    // wait
    while (DrvSPI_IsBusy(eDRVSPI_PORT1));

    // configure transaction length as 8 bits
    DrvSPI_SetBitLength(eDRVSPI_PORT1, 8);

    for (Counter = 0; Counter < ByteCount; Counter++)
    {
        // send data to program
        au32SourceData = DataBuffer[Counter];
        DrvSPI_SingleWrite(eDRVSPI_PORT1, &au32SourceData);

        // wait
        while (DrvSPI_IsBusy(eDRVSPI_PORT1));
    }

    // /CS: de-active
    DrvSPI_ClrSS(eDRVSPI_PORT1);
}
// **************************************
void SpiFlash_ReadData(uint8_t *DataBuffer0, uint32_t StartAddress, uint32_t ByteCount)
{
    uint32_t au32SourceData;
    uint32_t au32DestinationData;
    uint32_t Counter;

    // configure transaction length as 8 bits
    DrvSPI_SetBitLength(eDRVSPI_PORT1, 8);

    // /CS: active
    DrvSPI_SetSS(eDRVSPI_PORT1);

    // send Command: 0x03, Read data
    au32SourceData = 0x03;
    DrvSPI_SingleWrite(eDRVSPI_PORT1, &au32SourceData);

    // wait
    while (DrvSPI_IsBusy(eDRVSPI_PORT1));

    // configure transaction length as 24 bits
    DrvSPI_SetBitLength(eDRVSPI_PORT1, 24);

    // send 24-bit start address
    au32SourceData = StartAddress;
    DrvSPI_SingleWrite(eDRVSPI_PORT1, &au32SourceData);

    // wait
    while (DrvSPI_IsBusy(eDRVSPI_PORT1));

    // configure transaction length as 8 bits
    DrvSPI_SetBitLength(eDRVSPI_PORT1, 8);

    for (Counter = 0; Counter < ByteCount; Counter++)
    {
        // receive
        au32SourceData = 0x0;
        DrvSPI_SingleWrite(eDRVSPI_PORT1, &au32SourceData);

        // wait
        while (DrvSPI_IsBusy(eDRVSPI_PORT1));

        // dump Rx register
        DrvSPI_DumpRxRegister(eDRVSPI_PORT1, &au32DestinationData, 1);
        DataBuffer0[Counter] = (uint8_t) au32DestinationData;
    }

    // /CS: de-active
    DrvSPI_ClrSS(eDRVSPI_PORT1);
}
void SPItest(uint32_t Counter)
{
    uint32_t u32ByteCount, u32FlashAddress, u32PageNumber, Temp, Temp1;
    uint8_t DataBuffer0[256];
    /*All chip erase*/
    SpiFlash_ChipErase();

    /* Wait ready */
    SpiFlash_WaitReady();
    /* source data */
    for (u32ByteCount = 0; u32ByteCount < 256; u32ByteCount++)
    {
        DataBuffer0[u32ByteCount] = (u32ByteCount + Counter) & 0xff;
    }

    for (u32PageNumber = 0, u32FlashAddress = 0; u32PageNumber < TEST_NUMBER; u32PageNumber++)
    {
        /* page program */
        SpiFlash_PageProgram(DataBuffer0, u32FlashAddress, 256);
        SpiFlash_WaitReady();
        u32FlashAddress += 0x100;
    }
    /* clear data buffer */
    for (u32ByteCount = 0; u32ByteCount < 256; u32ByteCount++)
    {
        DataBuffer0[u32ByteCount] = 0;
    }
    /* read back and compare */

    for (u32PageNumber = 0, u32FlashAddress = 0; u32PageNumber < TEST_NUMBER; u32PageNumber++)
    {
        SpiFlash_ReadData(DataBuffer0, u32FlashAddress, 256);
        u32FlashAddress += 0x100;

        for (u32ByteCount = 0; u32ByteCount < 256; u32ByteCount++)
        {
            Temp = DataBuffer0[u32ByteCount];
            Temp1 = (u32ByteCount + Counter) & 0xff;
            if (Temp != Temp1)
            {
                printf("F0 Verify Error!\n");
                while (1);
            }
        }
        /* clear data buffer */
        for (u32ByteCount = 0; u32ByteCount < 256; u32ByteCount++)
        {
            DataBuffer0[u32ByteCount] = 0;
        }
    }

}

