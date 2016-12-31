/**
  ******************************************************************************
  * @file    stm8s_exti.h
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   This file contains all functions prototype and macros for the EXTI peripheral.
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
#ifndef __STM8S_EXTI_H
#define __STM8S_EXTI_H

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"

/* Exported types ------------------------------------------------------------*/

/** @addtogroup EXTI_Exported_Types
  * @{
  */

/**
  * @brief  EXTI Sensitivity values for PORTA to PORTE
  */
typedef enum {
  EXTI_SENSITIVITY_FALL_LOW  = (uint8_t)0x00, /*!< Interrupt on Falling edge and Low level */
  EXTI_SENSITIVITY_RISE_ONLY = (uint8_t)0x01, /*!< Interrupt on Rising edge only */
  EXTI_SENSITIVITY_FALL_ONLY = (uint8_t)0x02, /*!< Interrupt on Falling edge only */
  EXTI_SENSITIVITY_RISE_FALL = (uint8_t)0x03  /*!< Interrupt on Rising and Falling edges */
} EXTI_Sensitivity_TypeDef;

/**
  * @brief  EXTI Sensitivity values for TLI
  */
typedef enum {
  EXTI_TLISENSITIVITY_FALL_ONLY = (uint8_t)0x00, /*!< Top Level Interrupt on Falling edge only */
  EXTI_TLISENSITIVITY_RISE_ONLY = (uint8_t)0x04  /*!< Top Level Interrupt on Rising edge only */
} EXTI_TLISensitivity_TypeDef;

/**
  * @brief  EXTI PortNum possible values
  */
typedef enum {
  EXTI_PORT_GPIOA = (uint8_t)0x00, /*!< GPIO Port A */
  EXTI_PORT_GPIOB = (uint8_t)0x01, /*!< GPIO Port B */
  EXTI_PORT_GPIOC = (uint8_t)0x02, /*!< GPIO Port C */
  EXTI_PORT_GPIOD = (uint8_t)0x03, /*!< GPIO Port D */
  EXTI_PORT_GPIOE = (uint8_t)0x04  /*!< GPIO Port E */
} EXTI_Port_TypeDef;

/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/

/** @addtogroup EXTI_Private_Macros
  * @{
  */

/**
  * @brief  Macro used by the assert function in order to check the different sensitivity values for PORTA to PORTE.
  */
#define IS_EXTI_SENSITIVITY_OK(SensitivityValue) \
  (((SensitivityValue) == EXTI_SENSITIVITY_FALL_LOW) || \
   ((SensitivityValue) == EXTI_SENSITIVITY_RISE_ONLY) || \
   ((SensitivityValue) == EXTI_SENSITIVITY_FALL_ONLY) || \
   ((SensitivityValue) == EXTI_SENSITIVITY_RISE_FALL))

/**
  * @brief  Macro used by the assert function in order to check the different sensitivity values for TLI.
  */
#define IS_EXTI_TLISENSITIVITY_OK(SensitivityValue) \
  (((SensitivityValue) == EXTI_TLISENSITIVITY_FALL_ONLY) || \
   ((SensitivityValue) == EXTI_TLISENSITIVITY_RISE_ONLY))

/**
  * @brief  Macro used by the assert function in order to check the different Port values
  */
#define IS_EXTI_PORT_OK(PORT) \
  (((PORT) == EXTI_PORT_GPIOA) ||\
   ((PORT) == EXTI_PORT_GPIOB) ||\
   ((PORT) == EXTI_PORT_GPIOC) ||\
   ((PORT) == EXTI_PORT_GPIOD) ||\
   ((PORT) == EXTI_PORT_GPIOE))

/**
  * @brief  Macro used by the assert function in order to check the different values of the EXTI PinMask
  */
#define IS_EXTI_PINMASK_OK(PinMask) ((((PinMask) & (uint8_t)0x00) == (uint8_t)0x00) && ((PinMask) != (uint8_t)0x00))

/**
  * @}
  */

/* Exported functions ------------------------------------------------------- */

/** @addtogroup EXTI_Exported_Functions
  * @{
  */

void EXTI_DeInit(void);
void EXTI_SetExtIntSensitivity(EXTI_Port_TypeDef Port, EXTI_Sensitivity_TypeDef SensitivityValue);
void EXTI_SetTLISensitivity(EXTI_TLISensitivity_TypeDef SensitivityValue);
EXTI_Sensitivity_TypeDef EXTI_GetExtIntSensitivity(EXTI_Port_TypeDef Port);
EXTI_TLISensitivity_TypeDef EXTI_GetTLISensitivity(void);

/**
  * @}
  */

#endif /* __STM8S_EXTI_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
