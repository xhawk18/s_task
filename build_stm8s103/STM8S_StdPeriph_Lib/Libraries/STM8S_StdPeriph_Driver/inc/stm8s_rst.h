/**
  ******************************************************************************
  * @file    stm8s_rst.h
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   This file contains all functions prototype and macros for the RST peripheral.
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
#ifndef __STM8S_RST_H
#define __STM8S_RST_H

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"

/** @addtogroup STM8S_StdPeriph_Driver
  * @{
  */

/** @addtogroup RST_Exported_Types
  * @{
  */
typedef enum {
  RST_FLAG_EMCF    = (uint8_t)0x10, /*!< EMC reset flag */
  RST_FLAG_SWIMF   = (uint8_t)0x08, /*!< SWIM reset flag */
  RST_FLAG_ILLOPF  = (uint8_t)0x04, /*!< Illigal opcode reset flag */
  RST_FLAG_IWDGF   = (uint8_t)0x02, /*!< Independent watchdog reset flag */
  RST_FLAG_WWDGF   = (uint8_t)0x01  /*!< Window watchdog reset flag */
}RST_Flag_TypeDef;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/

/** @addtogroup RST_Private_Macros
  * @{
  */

/**
  * @brief  Macro used by the assert function to check the different functions parameters.
  */
/**
  * @brief  Macro used by the assert function to check the different RST flags.
  */
#define IS_RST_FLAG_OK(FLAG) (((FLAG) == RST_FLAG_EMCF) || \
                              ((FLAG) == RST_FLAG_SWIMF)  ||\
                              ((FLAG) == RST_FLAG_ILLOPF) ||\
                              ((FLAG) == RST_FLAG_IWDGF)  ||\
                              ((FLAG) == RST_FLAG_WWDGF))

/**
  * @}
  */

/** @addtogroup RST_Exported_functions
  * @{
  */
FlagStatus RST_GetFlagStatus(RST_Flag_TypeDef RST_Flag);
void RST_ClearFlag(RST_Flag_TypeDef RST_Flag);

/**
  * @}
  */

#endif /* __STM8S_RST_H */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
