/**************************************************************************//**
 * @file     FMC.h
 * @version  V2.1
 * $Revision: 4 $
 * $Date: 12/08/10 3:12p $
 * @brief    M051 Series Flash Memory Controller Driver Header File
 *
 * @note
 * Copyright (C) 2011 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#ifndef __FMC_H__
#define __FMC_H__

#include "M051Series.h"


/** @addtogroup M051_FUNC M051 Function Interface
  * @{
  */

/** @addtogroup FMC_FUNC FMC Device Function Interface
  * @{
  */


/*---------------------------------------------------------------------------------------------------------*/
/* Define Base Address                                                                                     */
/*---------------------------------------------------------------------------------------------------------*/
#define FMC_APROM_BASE          0x00000000UL    /*!< APROM  Base Address         */  
#define FMC_LDROM_BASE          0x00100000UL    /*!< LDROM  Base Address         */
#define FMC_CONFIG_BASE         0x00300000UL    /*!< CONFIG Base Address         */

#define FMC_FLASH_PAGE_SIZE     0x200           /*!< Flash Page Size (512 Bytes) */
#define FMC_LDROM_SIZE          0x1000          /*!< LDROM Size (4 kBytes)       */

/*---------------------------------------------------------------------------------------------------------*/
/*  ISPCON constant definitions                                                                            */
/*---------------------------------------------------------------------------------------------------------*/
#define FMC_ISPCON_BS_LDROM     0x2     /*!< ISPCON setting to select to boot from LDROM */
#define FMC_ISPCON_BS_APROM     0x0     /*!< ISPCON setting to select to boot from APROM */

/*---------------------------------------------------------------------------------------------------------*/
/*  ISPCMD constant definitions                                                                            */
/*---------------------------------------------------------------------------------------------------------*/
#define FMC_ISPCMD_READ        0x00     /*!< ISP Command: Read Flash       */
#define FMC_ISPCMD_PROGRAM     0x21     /*!< ISP Command: Program Flash    */
#define FMC_ISPCMD_PAGE_ERASE  0x22     /*!< ISP Command: Page Erase Flash */
#define FMC_ISPCMD_READ_UID    0x04     /*!< ISP Command: Read Unique ID   */

/*---------------------------------------------------------------------------------------------------------*/
/*  FMC Macro Definitions                                                                                  */
/*---------------------------------------------------------------------------------------------------------*/
#define _FMC_ENABLE_ISP()          (FMC->ISPCON |=  FMC_ISPCON_ISPEN_Msk)  /*!< Enable ISP Function  */
#define _FMC_DISABLE_ISP()         (FMC->ISPCON &= ~FMC_ISPCON_ISPEN_Msk)  /*!< Disable ISP Function */
#define _FMC_ENABLE_LD_UPDATE()    (FMC->ISPCON |=  FMC_ISPCON_LDUEN_Msk)  /*!< Enable LDROM Update Function   */
#define _FMC_DISABLE_LD_UPDATE()   (FMC->ISPCON &= ~FMC_ISPCON_LDUEN_Msk)  /*!< Disable LDROM Update Function  */
#define _FMC_ENABLE_CFG_UPDATE()   (FMC->ISPCON |=  FMC_ISPCON_CFGUEN_Msk) /*!< Enable CONFIG Update Function  */
#define _FMC_DISABLE_CFG_UPDATE()  (FMC->ISPCON &= ~FMC_ISPCON_CFGUEN_Msk) /*!< Disable CONFIG Update Function */
#define _FMC_ENABLE_AP_UPDATE()    (FMC->ISPCON |=  FMC_ISPCON_APUEN_Msk)  /*!< Enable APROM Update Function   */
#define _FMC_DISABLE_AP_UPDATE()   (FMC->ISPCON &= ~FMC_ISPCON_APUEN_Msk)  /*!< Disable APROM Update Function  */
#define _FMC_ENABLE_LOW_FREQ_OPTIMIZE_MODE()  (FMC->FATCON |=  FMC_FATCON_LFOM_Msk) /*!< Enable Flash Access Low Frequency Optimization Mode when HCLK <= 25MHz */
#define _FMC_DISABLE_LOW_FREQ_OPTIMIZE_MODE() (FMC->FATCON &= ~FMC_FATCON_LFOM_Msk) /*!< Disable Flash Access Low Frequency Optimization Mode */

#define _FMC_SELECT_NEXT_BOOT(x)   (FMC->ISPCON = (FMC->ISPCON & ~FMC_ISPCON_BS_Msk) | (x << FMC_ISPCON_BS_Pos)) /*!< Select Next Booting, x = 0 or 1 */
#define _FMC_GET_BOOT_STATUS()     ((FMC->ISPCON & FMC_ISPCON_BS_Msk)?1:0) /*!< Get MCU Booting Status */

/*---------------------------------------------------------------------------------------------------------*/
/* inline functions                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
/**
 * @brief      Program 32-bit data into specified address of flash
 * 
 * @param[in]  u32addr  Flash address include APROM, LDROM, Data Flash, and CONFIG
 * @param[in]  u32data  32-bit Data to program
 *
 * @details    To program word data into Flash include APROM, LDROM, Data Flash, and CONFIG.
 *             The corresponding functions in CONFIG are listed in FMC section of TRM.
 *
 * @note   
 *             Please make sure that Register Write-Protection Function has been disabled 
 *             before using this function. User can check the status of 
 *             Register Write-Protection Function with DrvSYS_IsProtectedRegLocked().
 */
static __INLINE void FMC_Write(uint32_t u32addr, uint32_t u32data)
{
    FMC->ISPCMD = FMC_ISPCMD_PROGRAM;
    FMC->ISPADR = u32addr;
    FMC->ISPDAT = u32data;
    FMC->ISPTRG = 0x1;  
    __ISB();
    while (FMC->ISPTRG);
}

/**
 * @brief       Read 32-bit Data from specified address of flash
 * 
 * @param[in]   u32addr  Flash address include APROM, LDROM, Data Flash, and CONFIG
 *
 * @return      The data of specified address
 *
 * @details     To read word data from Flash include APROM, LDROM, Data Flash, and CONFIG.
 *
 * @note   
 *              Please make sure that Register Write-Protection Function has been disabled 
 *              before using this function. User can check the status of 
 *              Register Write-Protection Function with DrvSYS_IsProtectedRegLocked().
 */
static __INLINE uint32_t FMC_Read(uint32_t u32addr)
{ 
    FMC->ISPCMD = FMC_ISPCMD_READ;
    FMC->ISPADR = u32addr;
    FMC->ISPDAT = 0;
    FMC->ISPTRG = 0x1;
    __ISB();
    while (FMC->ISPTRG);
    
    return FMC->ISPDAT;
}


/**
 * @brief      Flash page erase
 * 
 * @param[in]  u32addr  Flash address including APROM, LDROM, Data Flash, and CONFIG
 *
 * @details    To do flash page erase. The target address could be APROM, LDROM, Data Flash, or CONFIG. 
 *             The page size is 512 bytes.
 *
 * @note   
 *             Please make sure that Register Write-Protection Function has been disabled 
 *             before using this function. User can check the status of 
 *             Register Write-Protection Function with DrvSYS_IsProtectedRegLocked().
 */
static __INLINE void FMC_Erase(uint32_t u32addr)
{
    FMC->ISPCMD = FMC_ISPCMD_PAGE_ERASE;
    FMC->ISPADR = u32addr;
    FMC->ISPTRG = 0x1;
    __ISB();
    while (FMC->ISPTRG);
}

/**
 * @brief       Read Unique ID
 * 
 * @param[in]   u8index  UID index. 0 = UID[0:31], 1 = UID[32:63], 2 = UID[64:95]
 *
 * @return      The 32-bit unique ID data of specified UID index
 *
 * @details     To read out 96-bit Unique ID.
 *
 * @note   
 *              Please make sure that Register Write-Protection Function has been disabled 
 *              before using this function. User can check the status of 
 *              Register Write-Protection Function with DrvSYS_IsProtectedRegLocked().
 */
static __INLINE uint32_t FMC_ReadUID(uint8_t u8index)
{ 
    FMC->ISPCMD = FMC_ISPCMD_READ_UID;
    FMC->ISPADR = (u8index << 2);
    FMC->ISPDAT = 0;
    FMC->ISPTRG = 0x1;
    __ISB();
    while (FMC->ISPTRG);
    
    return FMC->ISPDAT;
}

/**
  * @} End of FMC Device Function Interface
  */ 

/**
  * @} End of M051 Function Interface
  */ 



#endif

