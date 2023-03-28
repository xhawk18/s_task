/**
  ******************************************************************************
  * @file    stm8s_tim4.h
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   This file contains all functions prototype and macros for the TIM4 peripheral.
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
#ifndef __STM8S_TIM4_H
#define __STM8S_TIM4_H

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"

/** @addtogroup STM8S_StdPeriph_Driver
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/** @addtogroup TIM4_Exported_Types
  * @{
  */



/** TIM4 Prescaler */
typedef enum
{
  TIM4_PRESCALER_1  = ((uint8_t)0x00),
  TIM4_PRESCALER_2    = ((uint8_t)0x01),
  TIM4_PRESCALER_4    = ((uint8_t)0x02),
  TIM4_PRESCALER_8     = ((uint8_t)0x03),
  TIM4_PRESCALER_16   = ((uint8_t)0x04),
  TIM4_PRESCALER_32     = ((uint8_t)0x05),
  TIM4_PRESCALER_64    = ((uint8_t)0x06),
  TIM4_PRESCALER_128   = ((uint8_t)0x07)
} TIM4_Prescaler_TypeDef;

#define IS_TIM4_PRESCALER_OK(PRESCALER) (((PRESCALER) == TIM4_PRESCALER_1  ) || \
    ((PRESCALER) == TIM4_PRESCALER_2    ) || \
    ((PRESCALER) == TIM4_PRESCALER_4    ) || \
    ((PRESCALER) == TIM4_PRESCALER_8  ) || \
    ((PRESCALER) == TIM4_PRESCALER_16   ) || \
    ((PRESCALER) == TIM4_PRESCALER_32     ) || \
    ((PRESCALER) == TIM4_PRESCALER_64    ) || \
    ((PRESCALER) == TIM4_PRESCALER_128   ) )

/** TIM4 One Pulse Mode */
typedef enum
{
  TIM4_OPMODE_SINGLE                 = ((uint8_t)0x01),
  TIM4_OPMODE_REPETITIVE             = ((uint8_t)0x00)
} TIM4_OPMode_TypeDef;

#define IS_TIM4_OPM_MODE_OK(MODE) (((MODE) == TIM4_OPMODE_SINGLE) || \
                                   ((MODE) == TIM4_OPMODE_REPETITIVE))

/** TIM4 Prescaler Reload Mode */
typedef enum
{
  TIM4_PSCRELOADMODE_UPDATE          = ((uint8_t)0x00),
  TIM4_PSCRELOADMODE_IMMEDIATE       = ((uint8_t)0x01)
} TIM4_PSCReloadMode_TypeDef;

#define IS_TIM4_PRESCALER_RELOAD_OK(RELOAD) (((RELOAD) == TIM4_PSCRELOADMODE_UPDATE) || \
    ((RELOAD) == TIM4_PSCRELOADMODE_IMMEDIATE))

/** TIM4 Update Source */
typedef enum
{
  TIM4_UPDATESOURCE_GLOBAL           = ((uint8_t)0x00),
  TIM4_UPDATESOURCE_REGULAR          = ((uint8_t)0x01)
} TIM4_UpdateSource_TypeDef;

#define IS_TIM4_UPDATE_SOURCE_OK(SOURCE) (((SOURCE) == TIM4_UPDATESOURCE_GLOBAL) || \
    ((SOURCE) == TIM4_UPDATESOURCE_REGULAR))

/** TIM4 Event Source */
typedef enum
{
  TIM4_EVENTSOURCE_UPDATE            = ((uint8_t)0x01)
}TIM4_EventSource_TypeDef;

#define IS_TIM4_EVENT_SOURCE_OK(SOURCE) (((SOURCE) == 0x01))

/** TIM4 Flags */
typedef enum
{
  TIM4_FLAG_UPDATE                   = ((uint8_t)0x01)
}TIM4_FLAG_TypeDef;

#define IS_TIM4_GET_FLAG_OK(FLAG) ((FLAG) == TIM4_FLAG_UPDATE)



/** TIM4 interrupt sources */
typedef enum
{
  TIM4_IT_UPDATE                     = ((uint8_t)0x01)
}TIM4_IT_TypeDef;

#define IS_TIM4_IT_OK(IT) ((IT) == TIM4_IT_UPDATE)



/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/** @addtogroup TIM4_Exported_Functions
  * @{
  */
void TIM4_DeInit(void);
void TIM4_TimeBaseInit(TIM4_Prescaler_TypeDef TIM4_Prescaler, uint8_t TIM4_Period);
void TIM4_Cmd(FunctionalState NewState);
void TIM4_ITConfig(TIM4_IT_TypeDef TIM4_IT, FunctionalState NewState);
void TIM4_UpdateDisableConfig(FunctionalState NewState);
void TIM4_UpdateRequestConfig(TIM4_UpdateSource_TypeDef TIM4_UpdateSource);
void TIM4_SelectOnePulseMode(TIM4_OPMode_TypeDef TIM4_OPMode);
void TIM4_PrescalerConfig(TIM4_Prescaler_TypeDef Prescaler, TIM4_PSCReloadMode_TypeDef TIM4_PSCReloadMode);
void TIM4_ARRPreloadConfig(FunctionalState NewState);
void TIM4_GenerateEvent(TIM4_EventSource_TypeDef TIM4_EventSource);
void TIM4_SetCounter(uint8_t Counter);
void TIM4_SetAutoreload(uint8_t Autoreload);
uint8_t TIM4_GetCounter(void);
TIM4_Prescaler_TypeDef TIM4_GetPrescaler(void);
FlagStatus TIM4_GetFlagStatus(TIM4_FLAG_TypeDef TIM4_FLAG);
void TIM4_ClearFlag(TIM4_FLAG_TypeDef TIM4_FLAG);
ITStatus TIM4_GetITStatus(TIM4_IT_TypeDef TIM4_IT);
void TIM4_ClearITPendingBit(TIM4_IT_TypeDef TIM4_IT);


/**
  * @}
  */

#endif /* __STM8S_TIM4_H */

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
