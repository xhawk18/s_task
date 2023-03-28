/**
  ******************************************************************************
  * @file    stm8s_flash.h
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   This file contains all functions prototype and macros for the FLASH peripheral.
   ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM8S_FLASH_H
#define __STM8S_FLASH_H

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"

/* Exported constants --------------------------------------------------------*/

/** @addtogroup FLASH_Exported_Constants
  * @{
  */

#define FLASH_PROG_START_PHYSICAL_ADDRESS ((uint32_t)0x008000) /*!< Program memory: start address */

#if defined (STM8S208) || defined(STM8S207) || defined(STM8S007) || defined (STM8AF52Ax) || defined (STM8AF62Ax)
 #define FLASH_PROG_END_PHYSICAL_ADDRESS   ((uint32_t)0x027FFF) /*!< Program memory: end address */
 #define FLASH_PROG_BLOCKS_NUMBER          ((uint16_t)1024)     /*!< Program memory: total number of blocks */
 #define FLASH_DATA_START_PHYSICAL_ADDRESS ((uint32_t)0x004000) /*!< Data EEPROM memory: start address */
 #define FLASH_DATA_END_PHYSICAL_ADDRESS   ((uint32_t)0x0047FF) /*!< Data EEPROM memory: end address */
 #define FLASH_DATA_BLOCKS_NUMBER          ((uint16_t)16)       /*!< Data EEPROM memory: total number of blocks */
 #define FLASH_BLOCK_SIZE                  ((uint8_t)128)       /*!< Number of bytes in a block (common for Program and Data memories) */
#endif /* STM8S208, STM8S207, STM8S007, STM8AF52Ax, STM8AF62Ax */

#if defined(STM8S105) || defined(STM8S005) || defined(STM8AF626x)
 #define FLASH_PROG_END_PHYSICAL_ADDRESS   ((uint32_t)0xFFFF)   /*!< Program memory: end address */
 #define FLASH_PROG_BLOCKS_NUMBER          ((uint16_t)256)      /*!< Program memory: total number of blocks */
 #define FLASH_DATA_START_PHYSICAL_ADDRESS ((uint32_t)0x004000) /*!< Data EEPROM memory: start address */
 #define FLASH_DATA_END_PHYSICAL_ADDRESS   ((uint32_t)0x0043FF) /*!< Data EEPROM memory: end address */
 #define FLASH_DATA_BLOCKS_NUMBER          ((uint16_t)8)        /*!< Data EEPROM memory: total number of blocks */
 #define FLASH_BLOCK_SIZE                  ((uint8_t)128)       /*!< Number of bytes in a block (common for Program and Data memories) */
#endif /* STM8S105 or STM8AF626x */

#if defined(STM8S103) || defined(STM8S003) || defined(STM8S903) || defined(STM8AF622x)
 #define FLASH_PROG_END_PHYSICAL_ADDRESS   ((uint32_t)0x9FFF)   /*!< Program memory: end address */
 #define FLASH_PROG_BLOCKS_NUMBER          ((uint16_t)128)      /*!< Program memory: total number of blocks */
 #define FLASH_DATA_START_PHYSICAL_ADDRESS ((uint32_t)0x004000) /*!< Data EEPROM memory: start address */
 #define FLASH_DATA_END_PHYSICAL_ADDRESS   ((uint32_t)0x00427F) /*!< Data EEPROM memory: end address */
 #define FLASH_DATA_BLOCKS_NUMBER          ((uint16_t)10)       /*!< Data EEPROM memory: total number of blocks */
 #define FLASH_BLOCK_SIZE                  ((uint8_t)64)        /*!< Number of bytes in a block (common for Program and Data memories) */
#endif /* STM8S103 or STM8S003 or STM8S903 or STM8AF622x*/

#define FLASH_RASS_KEY1 ((uint8_t)0x56) /*!< First RASS key */
#define FLASH_RASS_KEY2 ((uint8_t)0xAE) /*!< Second RASS key */

#define OPTION_BYTE_START_PHYSICAL_ADDRESS  ((uint16_t)0x4800)
#define OPTION_BYTE_END_PHYSICAL_ADDRESS    ((uint16_t)0x487F)
#define FLASH_OPTIONBYTE_ERROR              ((uint16_t)0x5555) /*!< Error code option byte 
                                                                   (if value read is not equal to complement value read) */
/**
  * @}
  */

/* Exported types ------------------------------------------------------------*/

/** @addtogroup FLASH_Exported_Types
  * @{
  */

/**
  * @brief  FLASH Memory types
  */
typedef enum {
    FLASH_MEMTYPE_PROG      = (uint8_t)0xFD, /*!< Program memory */
    FLASH_MEMTYPE_DATA      = (uint8_t)0xF7  /*!< Data EEPROM memory */
} FLASH_MemType_TypeDef;

/**
  * @brief  FLASH programming modes
  */
typedef enum {
    FLASH_PROGRAMMODE_STANDARD = (uint8_t)0x00, /*!< Standard programming mode */
    FLASH_PROGRAMMODE_FAST     = (uint8_t)0x10  /*!< Fast programming mode */
} FLASH_ProgramMode_TypeDef;

/**
  * @brief  FLASH fixed programming time
  */
typedef enum {
    FLASH_PROGRAMTIME_STANDARD = (uint8_t)0x00, /*!< Standard programming time fixed at 1/2 tprog */
    FLASH_PROGRAMTIME_TPROG    = (uint8_t)0x01  /*!< Programming time fixed at tprog */
} FLASH_ProgramTime_TypeDef;

/**
  * @brief  FLASH Low Power mode select
  */
typedef enum {
    FLASH_LPMODE_POWERDOWN         = (uint8_t)0x04, /*!< HALT: Power-Down / ACTIVE-HALT: Power-Down */
    FLASH_LPMODE_STANDBY           = (uint8_t)0x08, /*!< HALT: Standby    / ACTIVE-HALT: Standby */
    FLASH_LPMODE_POWERDOWN_STANDBY = (uint8_t)0x00, /*!< HALT: Power-Down / ACTIVE-HALT: Standby */
    FLASH_LPMODE_STANDBY_POWERDOWN = (uint8_t)0x0C  /*!< HALT: Standby    / ACTIVE-HALT: Power-Down */
}
FLASH_LPMode_TypeDef;

/**
  * @brief  FLASH status of the last operation
  */
typedef enum {
#if defined (STM8S208) || defined(STM8S207) || defined(STM8S007) || defined(STM8S105) || \
    defined(STM8S005) || defined (STM8AF52Ax) || defined (STM8AF62Ax) || defined(STM8AF626x)		
		FLASH_STATUS_END_HIGH_VOLTAGE           = (uint8_t)0x40, /*!< End of high voltage */
#endif /* STM8S208, STM8S207, STM8S105, STM8AF62Ax, STM8AF52Ax, STM8AF626x */
		FLASH_STATUS_SUCCESSFUL_OPERATION       = (uint8_t)0x04, /*!< End of operation flag */
		FLASH_STATUS_TIMEOUT = (uint8_t)0x02, /*!< Time out error */
    FLASH_STATUS_WRITE_PROTECTION_ERROR     = (uint8_t)0x01 /*!< Write attempted to protected page */
} FLASH_Status_TypeDef;

/**
  * @brief  FLASH flags definition
 * - Warning : FLAG value = mapping position register
  */
typedef enum {
#if defined (STM8S208) || defined(STM8S207) || defined(STM8S007) || defined(STM8S105) || \
    defined(STM8S005) || defined (STM8AF52Ax) || defined (STM8AF62Ax) || defined(STM8AF626x)
    FLASH_FLAG_HVOFF     = (uint8_t)0x40,     /*!< End of high voltage flag */
#endif /* STM8S208, STM8S207, STM8S105, STM8AF62Ax, STM8AF52Ax, STM8AF626x */
    FLASH_FLAG_DUL       = (uint8_t)0x08,     /*!< Data EEPROM unlocked flag */
    FLASH_FLAG_EOP       = (uint8_t)0x04,     /*!< End of programming (write or erase operation) flag */
    FLASH_FLAG_PUL       = (uint8_t)0x02,     /*!< Flash Program memory unlocked flag */
    FLASH_FLAG_WR_PG_DIS = (uint8_t)0x01      /*!< Write attempted to protected page flag */
} FLASH_Flag_TypeDef;

/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/

/**
  * @brief  Macros used by the assert function in order to check the different functions parameters.
  * @addtogroup FLASH_Private_Macros
  * @{
  */

/**
  * @brief  Macro used by the assert function in order to check the different sensitivity values for the flash program Address
  */

#define IS_FLASH_PROG_ADDRESS_OK(ADDRESS) (((ADDRESS) >= FLASH_PROG_START_PHYSICAL_ADDRESS) && \
    ((ADDRESS) <= FLASH_PROG_END_PHYSICAL_ADDRESS))

/**
  * @brief  Macro used by the assert function in order to check the different sensitivity values for the data eeprom Address
  */

#define IS_FLASH_DATA_ADDRESS_OK(ADDRESS) (((ADDRESS) >= FLASH_DATA_START_PHYSICAL_ADDRESS) && \
    ((ADDRESS) <= FLASH_DATA_END_PHYSICAL_ADDRESS))

/**
  * @brief  Macro used by the assert function in order to check the different sensitivity values for the data eeprom and flash program Address
  */
#define IS_FLASH_ADDRESS_OK(ADDRESS)((((ADDRESS) >= FLASH_PROG_START_PHYSICAL_ADDRESS) && ((ADDRESS) <= FLASH_PROG_END_PHYSICAL_ADDRESS)) || \
                                     (((ADDRESS) >= FLASH_DATA_START_PHYSICAL_ADDRESS) && ((ADDRESS) <= FLASH_DATA_END_PHYSICAL_ADDRESS)))

/**
  * @brief  Macro used by the assert function in order to check the different sensitivity values for the flash program Block number
  */
#define IS_FLASH_PROG_BLOCK_NUMBER_OK(BLOCKNUM) ((BLOCKNUM) < FLASH_PROG_BLOCKS_NUMBER)

/**
  * @brief  Macro used by the assert function in order to check the different sensitivity values for the data eeprom Block number
  */
#define IS_FLASH_DATA_BLOCK_NUMBER_OK(BLOCKNUM) ((BLOCKNUM) < FLASH_DATA_BLOCKS_NUMBER)

/**
  * @brief  Macro used by the assert function in order to check the different sensitivity values for the flash memory type
  */

#define IS_MEMORY_TYPE_OK(MEMTYPE) (((MEMTYPE) == FLASH_MEMTYPE_PROG) || \
                                    ((MEMTYPE) == FLASH_MEMTYPE_DATA))

/**
  * @brief  Macro used by the assert function in order to check the different sensitivity values for the flash program mode
  */

#define IS_FLASH_PROGRAM_MODE_OK(MODE) (((MODE) == FLASH_PROGRAMMODE_STANDARD) || \
                                        ((MODE) == FLASH_PROGRAMMODE_FAST))

/**
  * @brief  Macro used by the assert function in order to check the program time mode
  */

#define IS_FLASH_PROGRAM_TIME_OK(TIME) (((TIME) == FLASH_PROGRAMTIME_STANDARD) || \
                                        ((TIME) == FLASH_PROGRAMTIME_TPROG))

/**
  * @brief  Macro used by the assert function in order to check the different 
  *         sensitivity values for the low power mode
  */

#define IS_FLASH_LOW_POWER_MODE_OK(LPMODE) (((LPMODE) == FLASH_LPMODE_POWERDOWN) || \
    ((LPMODE) == FLASH_LPMODE_STANDBY) || \
    ((LPMODE) == FLASH_LPMODE_POWERDOWN_STANDBY) || \
    ((LPMODE) == FLASH_LPMODE_STANDBY_POWERDOWN))

/**
  * @brief  Macro used by the assert function in order to check the different 
  *         sensitivity values for the option bytes Address
  */
#define IS_OPTION_BYTE_ADDRESS_OK(ADDRESS) (((ADDRESS) >= OPTION_BYTE_START_PHYSICAL_ADDRESS) && \
    ((ADDRESS) <= OPTION_BYTE_END_PHYSICAL_ADDRESS))


/**
  * @brief  Macro used by the assert function in order to check the different flags values
  */
#if defined (STM8S208) || defined(STM8S207) || defined(STM8S007) || defined(STM8S105) || \
    defined(STM8S005) || defined (STM8AF52Ax) || defined (STM8AF62Ax) || defined(STM8AF626x)
 #define IS_FLASH_FLAGS_OK(FLAG) (((FLAG) == FLASH_FLAG_HVOFF) || \
                                 ((FLAG) == FLASH_FLAG_DUL) || \
                                 ((FLAG) == FLASH_FLAG_EOP) || \
                                 ((FLAG) == FLASH_FLAG_PUL) || \
                                 ((FLAG) == FLASH_FLAG_WR_PG_DIS))
#else /* STM8S103, STM8S903, STM8AF622x */
 #define IS_FLASH_FLAGS_OK(FLAG) (((FLAG) == FLASH_FLAG_DUL) || \
                                 ((FLAG) == FLASH_FLAG_EOP) || \
                                 ((FLAG) == FLASH_FLAG_PUL) || \
                                 ((FLAG) == FLASH_FLAG_WR_PG_DIS))
#endif /* STM8S208, STM8S207, STM8S105, STM8AF62Ax, STM8AF52Ax, STM8AF626x */
/**
  * @}
  */

/* Exported functions ------------------------------------------------------- */

/** @addtogroup FLASH_Exported_Functions
  * @{
  */
void FLASH_Unlock(FLASH_MemType_TypeDef FLASH_MemType);
void FLASH_Lock(FLASH_MemType_TypeDef FLASH_MemType);
void FLASH_DeInit(void);
void FLASH_ITConfig(FunctionalState NewState);
void FLASH_EraseByte(uint32_t Address);
void FLASH_ProgramByte(uint32_t Address, uint8_t Data);
uint8_t FLASH_ReadByte(uint32_t Address);
IN_RAM(void FLASH_ProgramWord(uint32_t Address, uint32_t Data));
uint16_t FLASH_ReadOptionByte(uint16_t Address);
void FLASH_ProgramOptionByte(uint16_t Address, uint8_t Data);
void FLASH_EraseOptionByte(uint16_t Address);
void FLASH_SetLowPowerMode(FLASH_LPMode_TypeDef FLASH_LPMode);
void FLASH_SetProgrammingTime(FLASH_ProgramTime_TypeDef FLASH_ProgTime);
FLASH_LPMode_TypeDef FLASH_GetLowPowerMode(void);
FLASH_ProgramTime_TypeDef FLASH_GetProgrammingTime(void);
uint32_t FLASH_GetBootSize(void);
FlagStatus FLASH_GetFlagStatus(FLASH_Flag_TypeDef FLASH_FLAG);

/**
@code
 All the functions declared below must be executed from RAM exclusively, except 
 for the FLASH_WaitForLastOperation function which can be executed from Flash.
 
 Steps of the execution from RAM differs from one toolchain to another.
 for more details refer to stm8s_flash.c file.
 
 To enable execution from RAM you can either uncomment the following define 
 in the stm8s.h file or define it in your toolchain compiler preprocessor
 - #define RAM_EXECUTION  (1) 

@endcode
*/
IN_RAM(void FLASH_EraseBlock(uint16_t BlockNum, FLASH_MemType_TypeDef FLASH_MemType));
IN_RAM(void FLASH_ProgramBlock(uint16_t BlockNum, FLASH_MemType_TypeDef FLASH_MemType, 
                        FLASH_ProgramMode_TypeDef FLASH_ProgMode, uint8_t *Buffer));
IN_RAM(FLASH_Status_TypeDef FLASH_WaitForLastOperation(FLASH_MemType_TypeDef FLASH_MemType));

/**
  * @}
  */

#endif /*__STM8S_FLASH_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
