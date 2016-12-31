/**
  ******************************************************************************
  * @file    stm8s_iwdg.h
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   This file contains all functions prototypes and macros for the IWDG peripheral.
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
#ifndef __STM8S_IWDG_H
#define __STM8S_IWDG_H

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"

/** @addtogroup STM8S_StdPeriph_Driver
  * @{
  */

/** @addtogroup IWDG_Private_Define
  * @{
  */

/**
  * @brief  Define used to prevent watchdog reset
  */
#define IWDG_KEY_REFRESH    ((uint8_t)0xAA)  /*!<  This value written in the Key register prevent the watchdog reset */

/**
  * @brief  Define used to start the watchdog counter down
  */
#define IWDG_KEY_ENABLE     ((uint8_t)0xCC)  /*!<  This value written in the Key register start the watchdog counting down*/

/**
 * @}
 */

/** @addtogroup IWDG_Private_Macros
  * @{
  */

/**
  * @brief  Macro used by the assert function in order to check the different
  * values of the prescaler.
  */
#define IS_IWDG_PRESCALER_OK(VALUE) (((VALUE) == IWDG_Prescaler_4   ) || \
                                     ((VALUE) == IWDG_Prescaler_8   ) || \
                                     ((VALUE) == IWDG_Prescaler_16  ) || \
                                     ((VALUE) == IWDG_Prescaler_32  ) || \
                                     ((VALUE) == IWDG_Prescaler_64  ) || \
                                     ((VALUE) == IWDG_Prescaler_128 ) || \
                                     ((VALUE) == IWDG_Prescaler_256))

/**
  * @brief  Macro used by the assert function in order to check the different
  * values of the counter register.
  */
#define IS_IWDG_WRITEACCESS_MODE_OK(MODE) (((MODE) == IWDG_WriteAccess_Enable) || ((MODE) == IWDG_WriteAccess_Disable))

/**
  * @}
  */

/** @addtogroup IWDG_Exported_Types
  * @{
  */

/** IWDG write  access enumeration */
typedef enum
{
  IWDG_WriteAccess_Enable  = (uint8_t)0x55, /*!< Code 0x55 in Key register, allow write access to Prescaler and Reload registers */
  IWDG_WriteAccess_Disable = (uint8_t)0x00  /*!< Code 0x00 in Key register, not allow write access to Prescaler and Reload registers */
} IWDG_WriteAccess_TypeDef;

/** IWDG prescaler enumaration */
typedef enum
{
  IWDG_Prescaler_4   = (uint8_t)0x00, /*!< Used to set prescaler register to 4 */
  IWDG_Prescaler_8   = (uint8_t)0x01, /*!< Used to set prescaler register to 8 */
  IWDG_Prescaler_16  = (uint8_t)0x02, /*!< Used to set prescaler register to 16 */
  IWDG_Prescaler_32  = (uint8_t)0x03, /*!< Used to set prescaler register to 32 */
  IWDG_Prescaler_64  = (uint8_t)0x04, /*!< Used to set prescaler register to 64 */
  IWDG_Prescaler_128 = (uint8_t)0x05, /*!< Used to set prescaler register to 128 */
  IWDG_Prescaler_256 = (uint8_t)0x06  /*!< Used to set prescaler register to 256 */
} IWDG_Prescaler_TypeDef;

/**
  * @}
  */

/** @addtogroup IWDG_Exported_Functions
  * @{
  */

void IWDG_WriteAccessCmd(IWDG_WriteAccess_TypeDef IWDG_WriteAccess);
void IWDG_SetPrescaler(IWDG_Prescaler_TypeDef IWDG_Prescaler);
void IWDG_SetReload(uint8_t IWDG_Reload);
void IWDG_ReloadCounter(void);
void IWDG_Enable(void);

/**
  * @}
  */

#endif /* __STM8S_IWDG_H */

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
