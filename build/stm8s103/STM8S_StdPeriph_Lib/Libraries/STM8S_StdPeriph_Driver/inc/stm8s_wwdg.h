/**
  ********************************************************************************
  * @file    stm8s_wwdg.h
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   This file contains all functions prototype and macros for the WWDG peripheral.
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
#ifndef __STM8S_WWDG_H
#define __STM8S_WWDG_H

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"

/** @addtogroup STM8S_StdPeriph_Driver
  * @{
  */

/* Private macros ------------------------------------------------------------*/

/** @addtogroup WWDG_Private_Macros
  * @{
  */

/**
  * @brief  Macro used by the assert function in order to check the
  * values of the window register.
  */
#define IS_WWDG_WINDOWLIMITVALUE_OK(WindowLimitValue) ((WindowLimitValue) <= 0x7F)

/**
  * @brief  Macro used by the assert function in order to check the different
  * values of the counter register.
  */
#define IS_WWDG_COUNTERVALUE_OK(CounterValue) ((CounterValue) <= 0x7F)

/**
  * @}
  */

/* Exported types ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */

/** @addtogroup WWDG_Exported_Functions
  * @{
  */

void WWDG_Init(uint8_t Counter, uint8_t WindowValue);
void WWDG_SetCounter(uint8_t Counter);
uint8_t WWDG_GetCounter(void);
void WWDG_SWReset(void);
void WWDG_SetWindowValue(uint8_t WindowValue);


/**
  * @}
  */

#endif /* __STM8S_WWDG_H */

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
