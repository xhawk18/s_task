/**
  ******************************************************************************
  * @file    stm8s_adc2.h
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   This file contains all the prototypes/macros for the ADC2 peripheral.
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
#ifndef __STM8S_ADC2_H
#define __STM8S_ADC2_H

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"

/* Exported types ------------------------------------------------------------*/

/** @addtogroup ADC2_Exported_Types
  * @{
  */

/**
  * @brief  ADC2 clock prescaler selection
  */

typedef enum {
  ADC2_PRESSEL_FCPU_D2  = (uint8_t)0x00, /**< Prescaler selection fADC2 = fcpu/2 */
  ADC2_PRESSEL_FCPU_D3  = (uint8_t)0x10, /**< Prescaler selection fADC2 = fcpu/3 */
  ADC2_PRESSEL_FCPU_D4  = (uint8_t)0x20, /**< Prescaler selection fADC2 = fcpu/4 */
  ADC2_PRESSEL_FCPU_D6  = (uint8_t)0x30, /**< Prescaler selection fADC2 = fcpu/6 */
  ADC2_PRESSEL_FCPU_D8  = (uint8_t)0x40, /**< Prescaler selection fADC2 = fcpu/8 */
  ADC2_PRESSEL_FCPU_D10 = (uint8_t)0x50, /**< Prescaler selection fADC2 = fcpu/10 */
  ADC2_PRESSEL_FCPU_D12 = (uint8_t)0x60, /**< Prescaler selection fADC2 = fcpu/12 */
  ADC2_PRESSEL_FCPU_D18 = (uint8_t)0x70  /**< Prescaler selection fADC2 = fcpu/18 */
} ADC2_PresSel_TypeDef;

/**
  * @brief   ADC2 External conversion trigger event selection
  */
typedef enum {
  ADC2_EXTTRIG_TIM   = (uint8_t)0x00, /**< Conversion from Internal TIM TRGO event */
  ADC2_EXTTRIG_GPIO  = (uint8_t)0x01  /**< Conversion from External interrupt on ADC_ETR pin*/
} ADC2_ExtTrig_TypeDef;

/**
  * @brief  ADC2 data alignment
  */
typedef enum {
  ADC2_ALIGN_LEFT  = (uint8_t)0x00, /**< Data alignment left */
  ADC2_ALIGN_RIGHT = (uint8_t)0x08  /**< Data alignment right */
} ADC2_Align_TypeDef;

/**
  * @brief  ADC2 schmitt Trigger
  */
typedef enum {
  ADC2_SCHMITTTRIG_CHANNEL0  = (uint8_t)0x00, /**< Schmitt trigger disable on AIN0 */
  ADC2_SCHMITTTRIG_CHANNEL1  = (uint8_t)0x01, /**< Schmitt trigger disable on AIN1 */
  ADC2_SCHMITTTRIG_CHANNEL2  = (uint8_t)0x02, /**< Schmitt trigger disable on AIN2 */
  ADC2_SCHMITTTRIG_CHANNEL3  = (uint8_t)0x03, /**< Schmitt trigger disable on AIN3 */
  ADC2_SCHMITTTRIG_CHANNEL4  = (uint8_t)0x04, /**< Schmitt trigger disable on AIN4 */
  ADC2_SCHMITTTRIG_CHANNEL5  = (uint8_t)0x05, /**< Schmitt trigger disable on AIN5 */
  ADC2_SCHMITTTRIG_CHANNEL6  = (uint8_t)0x06, /**< Schmitt trigger disable on AIN6 */
  ADC2_SCHMITTTRIG_CHANNEL7  = (uint8_t)0x07, /**< Schmitt trigger disable on AIN7 */
  ADC2_SCHMITTTRIG_CHANNEL8  = (uint8_t)0x08, /**< Schmitt trigger disable on AIN8 */
  ADC2_SCHMITTTRIG_CHANNEL9  = (uint8_t)0x09, /**< Schmitt trigger disable on AIN9 */
  ADC2_SCHMITTTRIG_CHANNEL10 = (uint8_t)0x0A, /**< Schmitt trigger disable on AIN10 */
  ADC2_SCHMITTTRIG_CHANNEL11 = (uint8_t)0x0B, /**< Schmitt trigger disable on AIN11 */
  ADC2_SCHMITTTRIG_CHANNEL12 = (uint8_t)0x0C, /**< Schmitt trigger disable on AIN12 */
  ADC2_SCHMITTTRIG_CHANNEL13 = (uint8_t)0x0D, /**< Schmitt trigger disable on AIN13 */
  ADC2_SCHMITTTRIG_CHANNEL14 = (uint8_t)0x0E, /**< Schmitt trigger disable on AIN14 */
  ADC2_SCHMITTTRIG_CHANNEL15 = (uint8_t)0x0F, /**< Schmitt trigger disable on AIN15 */
  ADC2_SCHMITTTRIG_ALL       = (uint8_t)0x1F /**< Schmitt trigger disable on all channels */

} ADC2_SchmittTrigg_TypeDef;

/**
  * @brief  ADC2 conversion mode selection
  */

typedef enum {
  ADC2_CONVERSIONMODE_SINGLE     = (uint8_t)0x00, /**< Single conversion mode */
  ADC2_CONVERSIONMODE_CONTINUOUS = (uint8_t)0x01  /**< Continuous conversion mode */
} ADC2_ConvMode_TypeDef;

/**
  * @brief  ADC2 analog channel selection
  */

typedef enum {
  ADC2_CHANNEL_0  = (uint8_t)0x00, /**< Analog channel 0 */
  ADC2_CHANNEL_1  = (uint8_t)0x01, /**< Analog channel 1 */
  ADC2_CHANNEL_2  = (uint8_t)0x02, /**< Analog channel 2 */
  ADC2_CHANNEL_3  = (uint8_t)0x03, /**< Analog channel 3 */
  ADC2_CHANNEL_4  = (uint8_t)0x04, /**< Analog channel 4 */
  ADC2_CHANNEL_5  = (uint8_t)0x05, /**< Analog channel 5 */
  ADC2_CHANNEL_6  = (uint8_t)0x06, /**< Analog channel 6 */
  ADC2_CHANNEL_7  = (uint8_t)0x07, /**< Analog channel 7 */
  ADC2_CHANNEL_8  = (uint8_t)0x08, /**< Analog channel 8 */
  ADC2_CHANNEL_9  = (uint8_t)0x09, /**< Analog channel 9 */
  ADC2_CHANNEL_10 = (uint8_t)0x0A, /**< Analog channel 10 */
  ADC2_CHANNEL_11 = (uint8_t)0x0B, /**< Analog channel 11 */
  ADC2_CHANNEL_12 = (uint8_t)0x0C, /**< Analog channel 12 */
  ADC2_CHANNEL_13 = (uint8_t)0x0D, /**< Analog channel 13 */
  ADC2_CHANNEL_14 = (uint8_t)0x0E, /**< Analog channel 14 */
  ADC2_CHANNEL_15 = (uint8_t)0x0F  /**< Analog channel 15 */
} ADC2_Channel_TypeDef;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/

/* Exported macros ------------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/

/** @addtogroup ADC2_Private_Macros
  * @brief  Macros used by the assert function to check the different functions parameters.
  * @{
  */

/**
  * @brief  Macro used by the assert function to check the different prescaler's values.
  */
#define IS_ADC2_PRESSEL_OK(PRESCALER) (((PRESCALER) == ADC2_PRESSEL_FCPU_D2) || \
                                      ((PRESCALER) == ADC2_PRESSEL_FCPU_D3) || \
                                      ((PRESCALER) == ADC2_PRESSEL_FCPU_D4) || \
                                      ((PRESCALER) == ADC2_PRESSEL_FCPU_D6) || \
                                      ((PRESCALER) == ADC2_PRESSEL_FCPU_D8) || \
                                      ((PRESCALER) == ADC2_PRESSEL_FCPU_D10) || \
                                      ((PRESCALER) == ADC2_PRESSEL_FCPU_D12) || \
                                      ((PRESCALER) == ADC2_PRESSEL_FCPU_D18))

/**
  * @brief  Macro used by the assert function to check the different external trigger values.
  */
#define IS_ADC2_EXTTRIG_OK(EXTRIG) (((EXTRIG) == ADC2_EXTTRIG_TIM) || \
								   ((EXTRIG) == ADC2_EXTTRIG_GPIO))

/**
  * @brief  Macro used by the assert function to check the different alignment modes.
  */
#define IS_ADC2_ALIGN_OK(ALIGN) (((ALIGN) == ADC2_ALIGN_LEFT) || \
                                ((ALIGN) == ADC2_ALIGN_RIGHT))


/**
  * @brief  Macro used by the assert function to check the different schmitt trigger values.
  */
#define IS_ADC2_SCHMITTTRIG_OK(SCHMITTTRIG) (((SCHMITTTRIG) == ADC2_SCHMITTTRIG_CHANNEL0) || \
    ((SCHMITTTRIG) == ADC2_SCHMITTTRIG_CHANNEL1) || \
    ((SCHMITTTRIG) == ADC2_SCHMITTTRIG_CHANNEL2) || \
    ((SCHMITTTRIG) == ADC2_SCHMITTTRIG_CHANNEL3) || \
    ((SCHMITTTRIG) == ADC2_SCHMITTTRIG_CHANNEL4) || \
    ((SCHMITTTRIG) == ADC2_SCHMITTTRIG_CHANNEL5) || \
    ((SCHMITTTRIG) == ADC2_SCHMITTTRIG_CHANNEL6) || \
    ((SCHMITTTRIG) == ADC2_SCHMITTTRIG_CHANNEL7) || \
    ((SCHMITTTRIG) == ADC2_SCHMITTTRIG_CHANNEL8) || \
    ((SCHMITTTRIG) == ADC2_SCHMITTTRIG_CHANNEL9) || \
    ((SCHMITTTRIG) == ADC2_SCHMITTTRIG_CHANNEL10) || \
    ((SCHMITTTRIG) == ADC2_SCHMITTTRIG_CHANNEL11) || \
    ((SCHMITTTRIG) == ADC2_SCHMITTTRIG_CHANNEL12) || \
    ((SCHMITTTRIG) == ADC2_SCHMITTTRIG_CHANNEL13) || \
    ((SCHMITTTRIG) == ADC2_SCHMITTTRIG_CHANNEL14) || \
    ((SCHMITTTRIG) == ADC2_SCHMITTTRIG_CHANNEL15) || \
    ((SCHMITTTRIG) == ADC2_SCHMITTTRIG_ALL))

/**
  * @brief  Macro used by the assert function to check the different conversion modes.
  */
#define IS_ADC2_CONVERSIONMODE_OK(MODE) (((MODE) == ADC2_CONVERSIONMODE_SINGLE) || \
                                        ((MODE) == ADC2_CONVERSIONMODE_CONTINUOUS))

/**
  * @brief  Macro used by the assert function to check the different channels values.
  */
#define IS_ADC2_CHANNEL_OK(CHANNEL) (((CHANNEL) == ADC2_CHANNEL_0) || \
                                    ((CHANNEL) == ADC2_CHANNEL_1) || \
                                    ((CHANNEL) == ADC2_CHANNEL_2) || \
                                    ((CHANNEL) == ADC2_CHANNEL_3) || \
                                    ((CHANNEL) == ADC2_CHANNEL_4) || \
                                    ((CHANNEL) == ADC2_CHANNEL_5) || \
                                    ((CHANNEL) == ADC2_CHANNEL_6) || \
                                    ((CHANNEL) == ADC2_CHANNEL_7) || \
                                    ((CHANNEL) == ADC2_CHANNEL_8) || \
                                    ((CHANNEL) == ADC2_CHANNEL_9) || \
                                    ((CHANNEL) == ADC2_CHANNEL_10) || \
                                    ((CHANNEL) == ADC2_CHANNEL_11) || \
                                    ((CHANNEL) == ADC2_CHANNEL_12) || \
                                    ((CHANNEL) == ADC2_CHANNEL_13) || \
                                    ((CHANNEL) == ADC2_CHANNEL_14) || \
                                    ((CHANNEL) == ADC2_CHANNEL_15))

/**
  * @}
  */

/* Exported functions ------------------------------------------------------- */

/** @addtogroup ADC2_Exported_Functions
  * @{
  */
void ADC2_DeInit(void);
void ADC2_Init(ADC2_ConvMode_TypeDef ADC2_ConversionMode, 
               ADC2_Channel_TypeDef ADC2_Channel, 
               ADC2_PresSel_TypeDef ADC2_PrescalerSelection, 
               ADC2_ExtTrig_TypeDef ADC2_ExtTrigger, 
               FunctionalState ADC2_ExtTriggerState, 
               ADC2_Align_TypeDef ADC2_Align, 
               ADC2_SchmittTrigg_TypeDef ADC2_SchmittTriggerChannel, 
               FunctionalState ADC2_SchmittTriggerState);
void ADC2_Cmd(FunctionalState NewState);
void ADC2_ITConfig(FunctionalState NewState);
void ADC2_PrescalerConfig(ADC2_PresSel_TypeDef ADC2_Prescaler);
void ADC2_SchmittTriggerConfig(ADC2_SchmittTrigg_TypeDef ADC2_SchmittTriggerChannel, 
                              FunctionalState NewState);
void ADC2_ConversionConfig(ADC2_ConvMode_TypeDef ADC2_ConversionMode, 
                           ADC2_Channel_TypeDef ADC2_Channel, 
                           ADC2_Align_TypeDef ADC2_Align);
void ADC2_ExternalTriggerConfig(ADC2_ExtTrig_TypeDef ADC2_ExtTrigger, FunctionalState NewState);
void ADC2_StartConversion(void);
uint16_t ADC2_GetConversionValue(void);
FlagStatus ADC2_GetFlagStatus(void);
void ADC2_ClearFlag(void);
ITStatus ADC2_GetITStatus(void);
void ADC2_ClearITPendingBit(void);
/**
  * @}
  */

#endif /* __STM8S_ADC2_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
