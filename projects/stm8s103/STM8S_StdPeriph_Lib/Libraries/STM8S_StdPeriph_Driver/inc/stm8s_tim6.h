/**
  ******************************************************************************
  * @file    stm8s_tim6.h
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   This file contains all functions prototype and macros for the TIM6 peripheral.
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
#ifndef __STM8S_TIM6_H
#define __STM8S_TIM6_H

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"

/** @addtogroup STM8S_StdPeriph_Driver
  * @{
  */

/* Exported variables ------------------------------------------------------- */
/* Exported types ------------------------------------------------------------*/

/** @addtogroup TIM6_Exported_Types
  * @{
  */


/**
  * @brief  TIM6 Prescaler
  */
typedef enum
{
    TIM6_PRESCALER_1      = ((uint8_t)0x00),  /*!< Time base Prescaler = 1 (No effect)*/
    TIM6_PRESCALER_2      = ((uint8_t)0x01),  /*!< Time base Prescaler = 2 */
    TIM6_PRESCALER_4      = ((uint8_t)0x02),  /*!< Time base Prescaler = 4 */
    TIM6_PRESCALER_8      = ((uint8_t)0x03),  /*!< Time base Prescaler = 8 */
    TIM6_PRESCALER_16     = ((uint8_t)0x04),  /*!< Time base Prescaler = 16 */
    TIM6_PRESCALER_32     = ((uint8_t)0x05),  /*!< Time base Prescaler = 32 */
    TIM6_PRESCALER_64     = ((uint8_t)0x06),  /*!< Time base Prescaler = 64 */
    TIM6_PRESCALER_128    = ((uint8_t)0x07)  /*!< Time base Prescaler = 128 */
}TIM6_Prescaler_TypeDef;

/**
  * @brief  TIM6 One Pulse Mode
  */
typedef enum
{
    TIM6_OPMODE_SINGLE      = ((uint8_t)0x01),  /*!< Single one Pulse mode (OPM Active) */
    TIM6_OPMODE_REPETITIVE  = ((uint8_t)0x00)   /*!< Repetitive Pulse mode (OPM inactive) */
}TIM6_OPMode_TypeDef;

/**
  * @brief  TIM6 Prescaler Reload Mode
  */
typedef enum
{
    TIM6_PSCRELOADMODE_UPDATE      =((uint8_t)0x00),  /*!< Prescaler value is reloaded at every update*/
    TIM6_PSCRELOADMODE_IMMEDIATE   =((uint8_t)0x01)    /*!< Prescaler value is reloaded immediately*/
}TIM6_PSCReloadMode_TypeDef;

/**
  * @brief  TIM6 Update Source
  */
typedef enum
{
    TIM6_UPDATESOURCE_GLOBAL   =((uint8_t)0x00),  /*!< Global Update request source */
    TIM6_UPDATESOURCE_REGULAR  =((uint8_t)0x01)    /*!< Regular Update request source */
}TIM6_UpdateSource_TypeDef;

/**
  * @brief  TIM6 Event Source
  */
typedef enum
{
    TIM6_EVENTSOURCE_UPDATE    = ((uint8_t)0x01),    /*!< Update Event*/
    TIM6_EVENTSOURCE_TRIGGER   = ((uint8_t)0x40)    /*!< Trigger Event*/
}TIM6_EventSource_TypeDef;

/**
  * @brief  TIM6 Trigger Output Source
  */
typedef enum
{
    TIM6_TRGOSOURCE_RESET    = ((uint8_t)0x00),    /*!< Trigger Output source = Reset*/
    TIM6_TRGOSOURCE_ENABLE   = ((uint8_t)0x10),    /*!< Trigger Output source = TIM5 is enabled*/
    TIM6_TRGOSOURCE_UPDATE   = ((uint8_t)0x20)    /*!< Trigger Output source = Update event*/
}TIM6_TRGOSource_TypeDef;

/**
  * @brief  TIM6 Slave Mode
  */
typedef enum
{
    TIM6_SLAVEMODE_DISABLE    = ((uint8_t)0x00),   /*!< Disable slave mode to clock the prescaler directly with the internal clock */
    TIM6_SLAVEMODE_RESET      = ((uint8_t)0x04),   /*!< Slave Mode Selection  = Reset*/
    TIM6_SLAVEMODE_GATED      = ((uint8_t)0x05),   /*!< Slave Mode Selection  = Gated*/
    TIM6_SLAVEMODE_TRIGGER    = ((uint8_t)0x06),    /*!< Slave Mode Selection  = Trigger*/
    TIM6_SLAVEMODE_EXTERNAL1  = ((uint8_t)0x07)    /*!< Slave Mode Selection  = External 1*/
}TIM6_SlaveMode_TypeDef;

/**
  * @brief  TIM6 Flags
  */
typedef enum
{
    TIM6_FLAG_UPDATE   = ((uint8_t)0x01),    /*!< Update Flag */
    TIM6_FLAG_TRIGGER  = ((uint8_t)0x40)    /*!< Trigger Flag */
}TIM6_FLAG_TypeDef;

/**
  * @brief  TIM6 interrupt sources
  */
typedef enum
{
    TIM6_IT_UPDATE   = ((uint8_t)0x01),    /*!< Update Interrupt*/
    TIM6_IT_TRIGGER  = ((uint8_t)0x40)    /*!< Trigger  Interrupt*/
}TIM6_IT_TypeDef;

/**
  * @brief  TIM6 Internal Trigger Selection
  */
typedef enum
{
    TIM6_TS_TIM1  = ((uint8_t)0x20),/*!< TRIG Input source =  TIM1 TRIG Output  */
    TIM6_TS_TIM5  = ((uint8_t)0x30) /*!< TRIG Input source =  TIM5 TRIG Output  */
}TIM6_TS_TypeDef;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

/** @addtogroup TIM6_Private_Macros
  * @{
  */

/**
  * @brief  Macro used by the assert function to check the different functions parameters.
  */

/**
  * @brief  Macro TIM6 Prescaler
  */
#define IS_TIM6_PRESCALER_OK(PRESCALER)  \
   (((PRESCALER) == TIM6_PRESCALER_1)    || \
    ((PRESCALER) == TIM6_PRESCALER_2)    || \
    ((PRESCALER) == TIM6_PRESCALER_4)    || \
    ((PRESCALER) == TIM6_PRESCALER_8)    || \
    ((PRESCALER) == TIM6_PRESCALER_16)   || \
    ((PRESCALER) == TIM6_PRESCALER_32)   || \
    ((PRESCALER) == TIM6_PRESCALER_64)   || \
    ((PRESCALER) == TIM6_PRESCALER_128))
/**
  * @brief  Macro TIM6 One Pulse Mode
  */
#define IS_TIM6_OPM_MODE_OK(MODE)  \
   (((MODE) == TIM6_OPMODE_SINGLE) || \
    ((MODE) == TIM6_OPMODE_REPETITIVE))

/**
  * @brief  Macro TIM6 Prescaler reload
  */
#define IS_TIM6_PRESCALER_RELOAD_OK(RELOAD) \
   (((RELOAD) == TIM6_PSCRELOADMODE_UPDATE) || \
    ((RELOAD) == TIM6_PSCRELOADMODE_IMMEDIATE))
/**
  * @brief  Macro TIM6 Update source
  */
#define IS_TIM6_UPDATE_SOURCE_OK(SOURCE) \
   (((SOURCE) == TIM6_UPDATESOURCE_GLOBAL) || \
    ((SOURCE) == TIM6_UPDATESOURCE_REGULAR))
/**
  * @brief  Macro TIM6 Event source
  */
#define IS_TIM6_EVENT_SOURCE_OK(SOURCE) \
   ((((SOURCE) & (uint8_t)0xBE) == 0x00) && \
    ((SOURCE) != 0x00))

/**
  * @brief  Macro TIM6 TRGO source
  */
#define IS_TIM6_TRGO_SOURCE_OK(SOURCE) \
   (((SOURCE) == TIM6_TRGOSOURCE_RESET) || \
    ((SOURCE) == TIM6_TRGOSOURCE_ENABLE)|| \
    ((SOURCE) == TIM6_TRGOSOURCE_UPDATE))
/**
  * @brief  Macro TIM6 Slave mode
  */
#define IS_TIM6_SLAVE_MODE_OK(MODE) \
   (((MODE) == TIM6_SLAVEMODE_DISABLE) || \
    ((MODE) == TIM6_SLAVEMODE_RESET)   || \
    ((MODE) == TIM6_SLAVEMODE_GATED)   || \
    ((MODE) == TIM6_SLAVEMODE_TRIGGER) || \
    ((MODE) == TIM6_SLAVEMODE_EXTERNAL1))
/**
  * @brief  Macro TIM6 Flags
  */
#define IS_TIM6_GET_FLAG_OK(FLAG) \
   (((FLAG) == TIM6_FLAG_UPDATE) || \
    ((FLAG) == TIM6_FLAG_TRIGGER))

#define IS_TIM6_CLEAR_FLAG_OK(FLAG) \
   ((((FLAG) & (uint8_t)0xBE) == 0x00) && ((FLAG) != 0x00))
/**
  * @brief  Macro TIM6 interrupts
  */
#define IS_TIM6_IT_OK(IT) \
   (((IT) != 0x00) && \
    (((IT) & (uint8_t)(~(uint8_t)(0x41)))== 0x00))

#define IS_TIM6_GET_IT_OK(IT) \
   (((IT) == TIM6_IT_UPDATE) || \
    ((IT) == TIM6_IT_TRIGGER))
/**
  * @brief  Macro TIM6 Trigger selection
  */
#define IS_TIM6_TRIGGER_SELECTION_OK(SELECTION) \
   (((SELECTION) == TIM6_TS_TIM5) || \
    ((SELECTION) == TIM6_TS_TIM1))
/**
 * @}
 */

/* Exported functions --------------------------------------------------------*/

/** @addtogroup TIM6_Exported_Functions
  * @{
  */

void TIM6_DeInit(void);
void TIM6_TimeBaseInit(TIM6_Prescaler_TypeDef TIM6_Prescaler, uint8_t TIM6_Period);
void TIM6_Cmd(FunctionalState NewState);
void TIM6_UpdateDisableConfig(FunctionalState NewState);
void TIM6_UpdateRequestConfig(TIM6_UpdateSource_TypeDef TIM6_UpdateSource);
void TIM6_SelectOnePulseMode(TIM6_OPMode_TypeDef TIM6_OPMode);
void TIM6_PrescalerConfig(TIM6_Prescaler_TypeDef Prescaler, TIM6_PSCReloadMode_TypeDef TIM6_PSCReloadMode);
void TIM6_ARRPreloadConfig(FunctionalState NewState);
void TIM6_SetCounter(uint8_t Counter);
void TIM6_SetAutoreload(uint8_t Autoreload);
uint8_t TIM6_GetCounter(void);
TIM6_Prescaler_TypeDef TIM6_GetPrescaler(void);
void TIM6_ITConfig(TIM6_IT_TypeDef TIM6_IT, FunctionalState NewState);
void TIM6_ClearFlag(TIM6_FLAG_TypeDef TIM6_FLAG);
ITStatus TIM6_GetITStatus(TIM6_IT_TypeDef TIM6_IT);
void TIM6_GenerateEvent(TIM6_EventSource_TypeDef TIM6_EventSource);
FlagStatus TIM6_GetFlagStatus(TIM6_FLAG_TypeDef TIM6_FLAG);
void TIM6_ClearITPendingBit(TIM6_IT_TypeDef TIM6_IT);
void TIM6_SelectOutputTrigger(TIM6_TRGOSource_TypeDef TIM6_TRGOSource);
void TIM6_SelectMasterSlaveMode(FunctionalState NewState);
void TIM6_SelectInputTrigger(TIM6_TS_TypeDef TIM6_InputTriggerSource);
void TIM6_InternalClockConfig(void);
void TIM6_SelectSlaveMode(TIM6_SlaveMode_TypeDef TIM6_SlaveMode);

/**
 * @}
 */

#endif /* __STM8S_TIM6_H */

/**
  * @}
  */
  

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
