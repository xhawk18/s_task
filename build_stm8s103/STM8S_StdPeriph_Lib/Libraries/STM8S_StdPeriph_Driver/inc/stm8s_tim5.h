/**
  ******************************************************************************
  * @file    stm8s_tim5.h
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   This file contains all functions prototype and macros for the TIM5 peripheral.
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
#ifndef __STM8S_TIM5_H
#define __STM8S_TIM5_H

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"

/** @addtogroup STM8S_StdPeriph_Driver
  * @{
  */

/* Exported types ------------------------------------------------------------*/


/** TIM5 Forced Action */
typedef enum
{
    TIM5_FORCEDACTION_ACTIVE           =((uint8_t)0x50),
    TIM5_FORCEDACTION_INACTIVE         =((uint8_t)0x40)
}TIM5_ForcedAction_TypeDef;

#define IS_TIM5_FORCED_ACTION_OK(ACTION) (((ACTION) == TIM5_FORCEDACTION_ACTIVE) || \
                                       ((ACTION) == TIM5_FORCEDACTION_INACTIVE))

/** TIM5 Prescaler */
typedef enum
{
    TIM5_PRESCALER_1     =((uint8_t)0x00),
    TIM5_PRESCALER_2      =((uint8_t)0x01),
    TIM5_PRESCALER_4      =((uint8_t)0x02),
    TIM5_PRESCALER_8      =((uint8_t)0x03),
    TIM5_PRESCALER_16     =((uint8_t)0x04),
    TIM5_PRESCALER_32     =((uint8_t)0x05),
    TIM5_PRESCALER_64     =((uint8_t)0x06),
    TIM5_PRESCALER_128    =((uint8_t)0x07),
    TIM5_PRESCALER_256    =((uint8_t)0x08),
    TIM5_PRESCALER_512    =((uint8_t)0x09),
    TIM5_PRESCALER_1024   =((uint8_t)0x0A),
    TIM5_PRESCALER_2048   =((uint8_t)0x0B),
    TIM5_PRESCALER_4096   =((uint8_t)0x0C),
    TIM5_PRESCALER_8192   =((uint8_t)0x0D),
    TIM5_PRESCALER_16384  =((uint8_t)0x0E),
    TIM5_PRESCALER_32768  =((uint8_t)0x0F)
}TIM5_Prescaler_TypeDef;

#define IS_TIM5_PRESCALER_OK(PRESCALER) (((PRESCALER) == TIM5_PRESCALER_1) || \
                 ((PRESCALER) == TIM5_PRESCALER_2     ) || \
                 ((PRESCALER) == TIM5_PRESCALER_4     ) || \
                 ((PRESCALER) == TIM5_PRESCALER_8    ) || \
                 ((PRESCALER) == TIM5_PRESCALER_16    ) || \
                 ((PRESCALER) == TIM5_PRESCALER_32      ) || \
                 ((PRESCALER) == TIM5_PRESCALER_64     ) || \
                 ((PRESCALER) == TIM5_PRESCALER_128    ) || \
                 ((PRESCALER) == TIM5_PRESCALER_256    ) || \
                 ((PRESCALER) == TIM5_PRESCALER_512    ) || \
                 ((PRESCALER) == TIM5_PRESCALER_1024   ) || \
                 ((PRESCALER) == TIM5_PRESCALER_2048  ) || \
                 ((PRESCALER) == TIM5_PRESCALER_4096  ) || \
                 ((PRESCALER) == TIM5_PRESCALER_8192  ) || \
                 ((PRESCALER) == TIM5_PRESCALER_16384  ) || \
                 ((PRESCALER) == TIM5_PRESCALER_32768  ))

/** TIM5 Output Compare and PWM modes */
typedef enum
{
    TIM5_OCMODE_TIMING     =((uint8_t)0x00),
    TIM5_OCMODE_ACTIVE     =((uint8_t)0x10),
    TIM5_OCMODE_INACTIVE   =((uint8_t)0x20),
    TIM5_OCMODE_TOGGLE     =((uint8_t)0x30),
    TIM5_OCMODE_PWM1       =((uint8_t)0x60),
    TIM5_OCMODE_PWM2       =((uint8_t)0x70)
}TIM5_OCMode_TypeDef;

#define IS_TIM5_OC_MODE_OK(MODE) (((MODE) ==  TIM5_OCMODE_TIMING) || \
                                  ((MODE) == TIM5_OCMODE_ACTIVE) || \
                                  ((MODE) == TIM5_OCMODE_INACTIVE) || \
                                  ((MODE) == TIM5_OCMODE_TOGGLE)|| \
                                  ((MODE) == TIM5_OCMODE_PWM1) || \
                                  ((MODE) == TIM5_OCMODE_PWM2))

#define IS_TIM5_OCM_OK(MODE)(((MODE) ==  TIM5_OCMODE_TIMING) || \
                             ((MODE) == TIM5_OCMODE_ACTIVE) || \
                             ((MODE) == TIM5_OCMODE_INACTIVE) || \
                             ((MODE) == TIM5_OCMODE_TOGGLE)|| \
                             ((MODE) == TIM5_OCMODE_PWM1) || \
                             ((MODE) == TIM5_OCMODE_PWM2) ||  \
                             ((MODE) == (uint8_t)TIM5_FORCEDACTION_ACTIVE) || \
                             ((MODE) == (uint8_t)TIM5_FORCEDACTION_INACTIVE))

/** TIM5 One Pulse Mode */
typedef enum
{
    TIM5_OPMODE_SINGLE                 =((uint8_t)0x01),
    TIM5_OPMODE_REPETITIVE             =((uint8_t)0x00)
}TIM5_OPMode_TypeDef;

#define IS_TIM5_OPM_MODE_OK(MODE) (((MODE) == TIM5_OPMODE_SINGLE) || \
                                   ((MODE) == TIM5_OPMODE_REPETITIVE))

/** TIM5 Channel */
typedef enum
{
    TIM5_CHANNEL_1                     =((uint8_t)0x00),
    TIM5_CHANNEL_2                     =((uint8_t)0x01),
    TIM5_CHANNEL_3                     =((uint8_t)0x02)
}TIM5_Channel_TypeDef;

#define IS_TIM5_CHANNEL_OK(CHANNEL) (((CHANNEL) == TIM5_CHANNEL_1) || \
                                     ((CHANNEL) == TIM5_CHANNEL_2) || \
                                     ((CHANNEL) == TIM5_CHANNEL_3))

#define IS_TIM5_PWMI_CHANNEL_OK(CHANNEL) (((CHANNEL) == TIM5_CHANNEL_1) || \
                                          ((CHANNEL) == TIM5_CHANNEL_2))

/** TIM5 Output Compare Polarity */
typedef enum
{
    TIM5_OCPOLARITY_HIGH               =((uint8_t)0x00),
    TIM5_OCPOLARITY_LOW                =((uint8_t)0x22)
}TIM5_OCPolarity_TypeDef;

#define IS_TIM5_OC_POLARITY_OK(POLARITY) (((POLARITY) == TIM5_OCPOLARITY_HIGH) || \
                                       ((POLARITY) == TIM5_OCPOLARITY_LOW))

/** TIM5 Output Compare states */
typedef enum
{
    TIM5_OUTPUTSTATE_DISABLE           =((uint8_t)0x00),
    TIM5_OUTPUTSTATE_ENABLE            =((uint8_t)0x11)
}TIM5_OutputState_TypeDef;

#define IS_TIM5_OUTPUT_STATE_OK(STATE) (((STATE) == TIM5_OUTPUTSTATE_DISABLE) || \
                                     ((STATE) == TIM5_OUTPUTSTATE_ENABLE))

/** TIM5 Input Capture Polarity */
typedef enum
{
    TIM5_ICPOLARITY_RISING            =((uint8_t)0x00),
    TIM5_ICPOLARITY_FALLING           =((uint8_t)0x44)
}TIM5_ICPolarity_TypeDef;

#define IS_TIM5_IC_POLARITY_OK(POLARITY) (((POLARITY) == TIM5_ICPOLARITY_RISING) || \
                                       ((POLARITY) == TIM5_ICPOLARITY_FALLING))

/** TIM5 Input Capture Selection */
typedef enum
{
    TIM5_ICSELECTION_DIRECTTI          =((uint8_t)0x01),
    TIM5_ICSELECTION_INDIRECTTI        =((uint8_t)0x02),
    TIM5_ICSELECTION_TRGI              =((uint8_t)0x03)
}TIM5_ICSelection_TypeDef;

#define IS_TIM5_IC_SELECTION_OK(SELECTION) (((SELECTION) == TIM5_ICSELECTION_DIRECTTI) || \
                                           ((SELECTION) == TIM5_ICSELECTION_INDIRECTTI) || \
                                           ((SELECTION) == TIM5_ICSELECTION_TRGI))

#define IS_TIM5_IC_SELECTION1_OK(SELECTION) (((SELECTION) == TIM5_ICSELECTION_DIRECTTI) || \
                                             ((SELECTION) == TIM5_ICSELECTION_TRGI))

/** TIM5 Input Capture Prescaler */
typedef enum
{
    TIM5_ICPSC_DIV1                    =((uint8_t)0x00),
    TIM5_ICPSC_DIV2                    =((uint8_t)0x04),
    TIM5_ICPSC_DIV4                    =((uint8_t)0x08),
    TIM5_ICPSC_DIV8                    =((uint8_t)0x0C)
}TIM5_ICPSC_TypeDef;

#define IS_TIM5_IC_PRESCALER_OK(PRESCALER) (((PRESCALER) == TIM5_ICPSC_DIV1) || \
                                         ((PRESCALER) == TIM5_ICPSC_DIV2) || \
                                         ((PRESCALER) == TIM5_ICPSC_DIV4) || \
                                         ((PRESCALER) == TIM5_ICPSC_DIV8))

/** TIM5 Input Capture Filer Value */
#define IS_TIM5_IC_FILTER_OK(ICFILTER) ((ICFILTER) <= 0x0F)

/** TIM5 interrupt sources */
typedef enum
{
    TIM5_IT_UPDATE                     =((uint8_t)0x01),
    TIM5_IT_CC1                        =((uint8_t)0x02),
    TIM5_IT_CC2                        =((uint8_t)0x04),
    TIM5_IT_CC3                        =((uint8_t)0x08),
    TIM5_IT_TRIGGER                    = ((uint8_t)0x40)
}TIM5_IT_TypeDef;

#define IS_TIM5_IT_OK(IT) (((IT) != 0x00) && ((IT) <= 0x4F))

#define IS_TIM5_GET_IT_OK(IT) (((IT) == TIM5_IT_UPDATE) || \
                            ((IT) == TIM5_IT_CC1) || \
                            ((IT) == TIM5_IT_CC2) || \
                            ((IT) == TIM5_IT_CC3) || \
                            ((IT) == TIM5_IT_TRIGGER))

/** TIM5 Prescaler Reload Mode */
typedef enum
{
    TIM5_PSCRELOADMODE_UPDATE          =((uint8_t)0x00),
    TIM5_PSCRELOADMODE_IMMEDIATE       =((uint8_t)0x01)
}TIM5_PSCReloadMode_TypeDef;

#define IS_TIM5_PRESCALER_RELOAD_OK(RELOAD) (((RELOAD) == TIM5_PSCRELOADMODE_UPDATE) || \
                                          ((RELOAD) == TIM5_PSCRELOADMODE_IMMEDIATE))

/** TIM5 Event Source */
typedef enum
{
    TIM5_EVENTSOURCE_UPDATE            =((uint8_t)0x01),
    TIM5_EVENTSOURCE_CC1               =((uint8_t)0x02),
    TIM5_EVENTSOURCE_CC2               =((uint8_t)0x04),
    TIM5_EVENTSOURCE_CC3               =((uint8_t)0x08),
    TIM5_EVENTSOURCE_TRIGGER           = ((uint8_t)0x40)
}TIM5_EventSource_TypeDef;

#define IS_TIM5_EVENT_SOURCE_OK(SOURCE) (((SOURCE) != 0x00))

/** TIM5 Update Source */
typedef enum
{
    TIM5_UPDATESOURCE_GLOBAL           =((uint8_t)0x00),
    TIM5_UPDATESOURCE_REGULAR          =((uint8_t)0x01)
}TIM5_UpdateSource_TypeDef;


#define IS_TIM5_UPDATE_SOURCE_OK(SOURCE) (((SOURCE) == TIM5_UPDATESOURCE_GLOBAL) || \
                                       ((SOURCE) == TIM5_UPDATESOURCE_REGULAR))

/**
  * @brief  TIM5 Trigger Output Source
  */
typedef enum
{
    TIM5_TRGOSOURCE_RESET   = ((uint8_t)0x00),   /*!< Trigger Output source = Reset*/
    TIM5_TRGOSOURCE_ENABLE  = ((uint8_t)0x10),   /*!< Trigger Output source = TIM5 is enabled*/
    TIM5_TRGOSOURCE_UPDATE  = ((uint8_t)0x20),   /*!< Trigger Output source = Update event*/
    TIM5_TRGOSOURCE_OC1     = ((uint8_t)0x30),   /*!< Trigger Output source = output compare channel1  */
    TIM5_TRGOSOURCE_OC1REF  = ((uint8_t)0x40),   /*!< Trigger Output source = output compare channel 1 reference */
    TIM5_TRGOSOURCE_OC2REF  = ((uint8_t)0x50)    /*!< Trigger Output source = output compare channel 2 reference */
}TIM5_TRGOSource_TypeDef;

/**
  * @brief  Macro TIM5 TRGO source
  */
#define IS_TIM5_TRGO_SOURCE_OK(SOURCE) \
   (((SOURCE) == TIM5_TRGOSOURCE_RESET)  || \
    ((SOURCE) == TIM5_TRGOSOURCE_ENABLE) || \
    ((SOURCE) == TIM5_TRGOSOURCE_UPDATE) || \
    ((SOURCE) == TIM5_TRGOSOURCE_OC1)    || \
    ((SOURCE) == TIM5_TRGOSOURCE_OC1REF) || \
    ((SOURCE) == TIM5_TRGOSOURCE_OC2REF))
  
/** TIM5 Flags */
typedef enum
{
    TIM5_FLAG_UPDATE                   =((uint16_t)0x0001),
    TIM5_FLAG_CC1                      =((uint16_t)0x0002),
    TIM5_FLAG_CC2                      =((uint16_t)0x0004),
    TIM5_FLAG_CC3                      =((uint16_t)0x0008),
    TIM5_FLAG_TRIGGER                  = ((uint16_t)0x0040),
    TIM5_FLAG_CC1OF                    =((uint16_t)0x0200),
    TIM5_FLAG_CC2OF                    =((uint16_t)0x0400),
    TIM5_FLAG_CC3OF                    =((uint16_t)0x0800)
}TIM5_FLAG_TypeDef;

#define IS_TIM5_GET_FLAG_OK(FLAG) (((FLAG) == TIM5_FLAG_UPDATE) || \
                                ((FLAG) == TIM5_FLAG_CC1) || \
                                ((FLAG) == TIM5_FLAG_CC2) || \
                                ((FLAG) == TIM5_FLAG_CC3) || \
                                ((FLAG) == TIM5_FLAG_TRIGGER) || \
                                ((FLAG) == TIM5_FLAG_CC1OF) || \
                                ((FLAG) == TIM5_FLAG_CC2OF) || \
                                ((FLAG) == TIM5_FLAG_CC3OF))

#define IS_TIM5_CLEAR_FLAG_OK(FLAG) ((((uint16_t)(FLAG) & 0xF1F0) == 0x0000) && ((uint16_t)(FLAG) != 0x0000))


/**
  * @brief  TIM5 Slave Mode
  */
typedef enum
{
    TIM5_SLAVEMODE_RESET      = ((uint8_t)0x04),   /*!< Slave Mode Selection  = Reset*/
    TIM5_SLAVEMODE_GATED      = ((uint8_t)0x05),   /*!< Slave Mode Selection  = Gated*/
    TIM5_SLAVEMODE_TRIGGER    = ((uint8_t)0x06),   /*!< Slave Mode Selection  = Trigger*/
    TIM5_SLAVEMODE_EXTERNAL1  = ((uint8_t)0x07)  /*!< Slave Mode Selection  = External 1*/
}TIM5_SlaveMode_TypeDef;

/**
  * @brief  Macro TIM5 Slave mode
  */
#define IS_TIM5_SLAVE_MODE_OK(MODE) \
   (((MODE) == TIM5_SLAVEMODE_RESET)   || \
    ((MODE) == TIM5_SLAVEMODE_GATED)   || \
    ((MODE) == TIM5_SLAVEMODE_TRIGGER) || \
    ((MODE) == TIM5_SLAVEMODE_EXTERNAL1))
    
/**
  * @brief  TIM5 Internal Trigger Selection
  */
typedef enum
{
    TIM5_TS_TIM6  = ((uint8_t)0x00), /*!< TRIG Input source =  TIM6 TRIG Output  */
    TIM5_TS_TIM1  = ((uint8_t)0x03) /*!< TRIG Input source =  TIM1 TRIG Output  */
}TIM5_TS_TypeDef;

/**
  * @brief  Macro TIM5  Trigger Selection
  */
#define IS_TIM5_TRIGGER_SELECTION_OK(SELECTION) \
   (((SELECTION) == TIM5_TS_TIM6)  || \
    ((SELECTION) == TIM5_TS_TIM1)  )


#define IS_TIM5_TIX_TRIGGER_SELECTION_OK(SELECTION) \
   (((SELECTION) == TIM5_TS_TI1F_ED) || \
    ((SELECTION) == TIM5_TS_TI1FP1)  || \
    ((SELECTION) == TIM5_TS_TI2FP2))


/**
  * @brief  TIM5 Encoder Mode
  */
typedef enum
{
    TIM5_ENCODERMODE_TI1    = ((uint8_t)0x01),   /*!< Encoder mode 1*/
    TIM5_ENCODERMODE_TI2    = ((uint8_t)0x02),   /*!< Encoder mode 2*/
    TIM5_ENCODERMODE_TI12   = ((uint8_t)0x03)    /*!< Encoder mode 3*/
}TIM5_EncoderMode_TypeDef;
/**
  * @brief  Macro TIM5 encoder mode
  */
#define IS_TIM5_ENCODER_MODE_OK(MODE) \
   (((MODE) == TIM5_ENCODERMODE_TI1) || \
    ((MODE) == TIM5_ENCODERMODE_TI2) || \
    ((MODE) == TIM5_ENCODERMODE_TI12))
    
/**
  * @brief  TIM5 External Trigger Prescaler
  */
typedef enum
{
    TIM5_EXTTRGPSC_OFF   = ((uint8_t)0x00),   /*!< No External Trigger prescaler  */
    TIM5_EXTTRGPSC_DIV2  = ((uint8_t)0x10),   /*!< External Trigger prescaler = 2 (ETRP frequency divided by 2) */
    TIM5_EXTTRGPSC_DIV4  = ((uint8_t)0x20),   /*!< External Trigger prescaler = 4 (ETRP frequency divided by 4) */
    TIM5_EXTTRGPSC_DIV8  = ((uint8_t)0x30)    /*!< External Trigger prescaler = 8 (ETRP frequency divided by 8) */
}TIM5_ExtTRGPSC_TypeDef;

/**
  * @brief  Macro TIM5 external trigger prescaler
  */
#define IS_TIM5_EXT_PRESCALER_OK(PRESCALER) \
   (((PRESCALER) == TIM5_EXTTRGPSC_OFF)  || \
    ((PRESCALER) == TIM5_EXTTRGPSC_DIV2) || \
    ((PRESCALER) == TIM5_EXTTRGPSC_DIV4) || \
    ((PRESCALER) == TIM5_EXTTRGPSC_DIV8))
    
/**
  * @brief  TIM5 External Trigger Polarity
  */
typedef enum
{
    TIM5_EXTTRGPOLARITY_INVERTED    = ((uint8_t)0x80),   /*!< External Trigger Polarity = inverted */
    TIM5_EXTTRGPOLARITY_NONINVERTED  = ((uint8_t)0x00)    /*!< External Trigger Polarity = non inverted */
}TIM5_ExtTRGPolarity_TypeDef;

/**
  * @brief  Macro TIM5  Trigger Polarity
  */
#define IS_TIM5_EXT_POLARITY_OK(POLARITY) \
   (((POLARITY) == TIM5_EXTTRGPOLARITY_INVERTED) || \
    ((POLARITY) == TIM5_EXTTRGPOLARITY_NONINVERTED))
    
/**
  * @brief  Macro TIM5 External Trigger Filter
  */
#define IS_TIM5_EXT_FILTER_OK(EXTFILTER) ((EXTFILTER) <= 0x0F)
/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/** @addtogroup TIM5_Exported_Functions
  * @{
  */

void TIM5_DeInit(void);
void TIM5_TimeBaseInit(TIM5_Prescaler_TypeDef TIM5_Prescaler, uint16_t TIM5_Period);
void TIM5_OC1Init(TIM5_OCMode_TypeDef TIM5_OCMode, TIM5_OutputState_TypeDef TIM5_OutputState,uint16_t TIM5_Pulse, TIM5_OCPolarity_TypeDef TIM5_OCPolarity);
void TIM5_OC2Init(TIM5_OCMode_TypeDef TIM5_OCMode, TIM5_OutputState_TypeDef TIM5_OutputState,uint16_t TIM5_Pulse, TIM5_OCPolarity_TypeDef TIM5_OCPolarity);
void TIM5_OC3Init(TIM5_OCMode_TypeDef TIM5_OCMode, TIM5_OutputState_TypeDef TIM5_OutputState,uint16_t TIM5_Pulse, TIM5_OCPolarity_TypeDef TIM5_OCPolarity);
void TIM5_ICInit(TIM5_Channel_TypeDef TIM5_Channel, TIM5_ICPolarity_TypeDef TIM5_ICPolarity, TIM5_ICSelection_TypeDef TIM5_ICSelection,  TIM5_ICPSC_TypeDef TIM5_ICPrescaler, uint8_t TIM5_ICFilter);
void TIM5_PWMIConfig(TIM5_Channel_TypeDef TIM5_Channel, TIM5_ICPolarity_TypeDef TIM5_ICPolarity, TIM5_ICSelection_TypeDef TIM5_ICSelection,  TIM5_ICPSC_TypeDef TIM5_ICPrescaler, uint8_t TIM5_ICFilter);
void TIM5_Cmd(FunctionalState NewState);
void TIM5_ITConfig(TIM5_IT_TypeDef TIM5_IT, FunctionalState NewState);
void TIM5_InternalClockConfig(void);
void TIM5_UpdateDisableConfig(FunctionalState NewState);
void TIM5_UpdateRequestConfig(TIM5_UpdateSource_TypeDef TIM5_UpdateSource);
void TIM5_SelectOnePulseMode(TIM5_OPMode_TypeDef TIM5_OPMode);
void TIM5_PrescalerConfig(TIM5_Prescaler_TypeDef Prescaler, TIM5_PSCReloadMode_TypeDef TIM5_PSCReloadMode);
void TIM5_SelectOutputTrigger(TIM5_TRGOSource_TypeDef TIM5_TRGOSource);
void TIM5_ForcedOC1Config(TIM5_ForcedAction_TypeDef TIM5_ForcedAction);
void TIM5_ForcedOC2Config(TIM5_ForcedAction_TypeDef TIM5_ForcedAction);
void TIM5_ForcedOC3Config(TIM5_ForcedAction_TypeDef TIM5_ForcedAction);
void TIM5_ARRPreloadConfig(FunctionalState NewState);
void TIM5_CCPreloadControl(FunctionalState NewState);
void TIM5_OC1PreloadConfig(FunctionalState NewState);
void TIM5_OC2PreloadConfig(FunctionalState NewState);
void TIM5_OC3PreloadConfig(FunctionalState NewState);
void TIM5_GenerateEvent(TIM5_EventSource_TypeDef TIM5_EventSource);
void TIM5_OC1PolarityConfig(TIM5_OCPolarity_TypeDef TIM5_OCPolarity);
void TIM5_OC2PolarityConfig(TIM5_OCPolarity_TypeDef TIM5_OCPolarity);
void TIM5_OC3PolarityConfig(TIM5_OCPolarity_TypeDef TIM5_OCPolarity);
void TIM5_CCxCmd(TIM5_Channel_TypeDef TIM5_Channel, FunctionalState NewState);
void TIM5_SelectOCxM(TIM5_Channel_TypeDef TIM5_Channel, TIM5_OCMode_TypeDef TIM5_OCMode);
void TIM5_SetCounter(uint16_t Counter);
void TIM5_SetAutoreload(uint16_t Autoreload);
void TIM5_SetCompare1(uint16_t Compare1);
void TIM5_SetCompare2(uint16_t Compare2);
void TIM5_SetCompare3(uint16_t Compare3);
void TIM5_SetIC1Prescaler(TIM5_ICPSC_TypeDef TIM5_IC1Prescaler);
void TIM5_SetIC2Prescaler(TIM5_ICPSC_TypeDef TIM5_IC2Prescaler);
void TIM5_SetIC3Prescaler(TIM5_ICPSC_TypeDef TIM5_IC3Prescaler);
uint16_t TIM5_GetCapture1(void);
uint16_t TIM5_GetCapture2(void);
uint16_t TIM5_GetCapture3(void);
uint16_t TIM5_GetCounter(void);
TIM5_Prescaler_TypeDef TIM5_GetPrescaler(void);
FlagStatus TIM5_GetFlagStatus(TIM5_FLAG_TypeDef TIM5_FLAG);
void TIM5_ClearFlag(TIM5_FLAG_TypeDef TIM5_FLAG);
ITStatus TIM5_GetITStatus(TIM5_IT_TypeDef TIM5_IT);
void TIM5_ClearITPendingBit(TIM5_IT_TypeDef TIM5_IT);
void TIM5_SelectInputTrigger(TIM5_TS_TypeDef TIM5_InputTriggerSource);
void TIM5_SelectSlaveMode(TIM5_SlaveMode_TypeDef TIM5_SlaveMode);
void TIM5_EncoderInterfaceConfig(TIM5_EncoderMode_TypeDef TIM5_EncoderMode, TIM5_ICPolarity_TypeDef TIM5_IC1Polarity,TIM5_ICPolarity_TypeDef TIM5_IC2Polarity);


/**
  * @}
  */

#endif /* __STM8S_TIM5_H */

/**
  * @}
  */
  

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
