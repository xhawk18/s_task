/**
  ******************************************************************************
  * @file    stm8_128_eval.h
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    30-September-2014
  * @brief   This file contains definitions for STM8_128_EVAL's Leds, push-buttons
  *          and COM ports hardware resources.
  ******************************************************************************
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
#ifndef __STM8_128_EVAL_H
#define __STM8_128_EVAL_H

/* Includes ------------------------------------------------------------------*/
#include "stm8s_eval.h"

/** @addtogroup Utilities
  * @{
  */

/** @addtogroup STM8S_EVAL
  * @{
  */

/** @addtogroup STM8_128_EVAL
  * @{
  */
/** @addtogroup STM8_128_EVAL_LOW_LEVEL
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/** @defgroup STM8_128_EVAL_LOW_LEVEL_Exported_Constants
  * @{
  */
/** @addtogroup STM8_128_EVAL_LOW_LEVEL_LED
  * @{
  */

#define LEDn                        4
#define LED1_GPIO_PORT              GPIOH
#define LED1_GPIO_PIN               GPIO_PIN_3


#define LED2_GPIO_PORT              GPIOH
#define LED2_GPIO_PIN               GPIO_PIN_2

#define LED3_GPIO_PORT              GPIOH
#define LED3_GPIO_PIN               GPIO_PIN_1

#define LED4_GPIO_PORT              GPIOH
#define LED4_GPIO_PIN               GPIO_PIN_0


/**
  * @}
  */

/** @addtogroup STM8_128_EVAL_LOW_LEVEL_BUTTON
  * @{
  */
#define BUTTONn                     6

/**
 * @brief Key push-button
 */
#define KEY_BUTTON_PORT             GPIOC
#define KEY_BUTTON_PIN              GPIO_PIN_0

/**
 * @brief Joystick Right push-button
 */
#define RIGHT_BUTTON_PORT           GPIOB
#define RIGHT_BUTTON_PIN            GPIO_PIN_5

/**
 * @brief Joystick Left push-button
 */
#define LEFT_BUTTON_PORT            GPIOB
#define LEFT_BUTTON_PIN             GPIO_PIN_4

/**
 * @brief Joystick Up push-button
 */
#define UP_BUTTON_PORT              GPIOB
#define UP_BUTTON_PIN               GPIO_PIN_6

/**
 * @brief Joystick Down push-button
 */
#define DOWN_BUTTON_PORT            GPIOB
#define DOWN_BUTTON_PIN             GPIO_PIN_7

/**
 * @brief Joystick Sel push-button
 */
#define SEL_BUTTON_PORT             GPIOD
#define SEL_BUTTON_PIN              GPIO_PIN_7

/**
 * @brief EXTI push-button definition
 */
#define KEY_BUTTON_EXTI_PORT        EXTI_PORT_GPIOC
#define RIGHT_BUTTON_EXTI_PORT      EXTI_PORT_GPIOB
#define LEFT_BUTTON_EXTI_PORT       EXTI_PORT_GPIOB
#define UP_BUTTON_EXTI_PORT         EXTI_PORT_GPIOB
#define DOWN_BUTTON_EXTI_PORT       EXTI_PORT_GPIOB
#define SEL_BUTTON_EXTI_PORT        EXTI_PORT_GPIOD

/**
  * @}
  */

/** @addtogroup STM8_128_EVAL_SD_SPI
  * @{
  */
/**
  * @brief  SD SPI Interface pins
  */
#define SD_SPI_CLK                       CLK_PERIPHERAL_SPI
#define SD_SPI_SCK_PIN                   GPIO_PIN_5                  /* PC.05 */
#define SD_SPI_SCK_GPIO_PORT             GPIOC                       /* GPIOC */
#define SD_SPI_MISO_PIN                  GPIO_PIN_7                  /* PC.05 */
#define SD_SPI_MISO_GPIO_PORT            GPIOC                       /* GPIOC */
#define SD_SPI_MOSI_PIN                  GPIO_PIN_6                  /* PC.06 */
#define SD_SPI_MOSI_GPIO_PORT            GPIOC                       /* GPIOC */
#define SD_CS_PIN                        GPIO_PIN_5                  /* PE.05 */
#define SD_CS_GPIO_PORT                  GPIOE                       /* GPIOE */
#define SD_DETECT_PIN                    GPIO_PIN_4                  /* PE.04 */
#define SD_DETECT_GPIO_PORT              GPIOE                       /* GPIOE */

/**
  * @}
  */

/** @addtogroup STM8_128_EVAL_LOW_LEVEL_I2C_EE
  * @{
  */
/**
  * @brief  I2C EEPROM Interface pins
  */
#define sEE_I2C                          I2C  
#define sEE_I2C_CLK                      CLK_PERIPHERAL_I2C
#define sEE_I2C_SCL_PIN                  GPIO_PIN_1                  /* PC.01 */
#define sEE_I2C_SCL_GPIO_PORT            GPIOE                       /* GPIOE */
#define sEE_I2C_SDA_PIN                  GPIO_PIN_2                  /* PC.00 */
#define sEE_I2C_SDA_GPIO_PORT            GPIOE                       /* GPIOE */
#define sEE_M24C64_32


#define sEE_DIRECTION_TX                 0
#define sEE_DIRECTION_RX                 1

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/

/** @defgroup STM8_128_EVAL_LOW_LEVEL_Exported_Functions
  * @{
  */
void STM_EVAL_LEDInit(Led_TypeDef Led);
void STM_EVAL_LEDOn(Led_TypeDef Led);
void STM_EVAL_LEDOff(Led_TypeDef Led);
void STM_EVAL_LEDToggle(Led_TypeDef Led);
void STM_EVAL_PBInit(Button_TypeDef Button, ButtonMode_TypeDef Button_Mode);
uint8_t STM_EVAL_PBGetState(Button_TypeDef Button);
void SD_LowLevel_DeInit(void);
void SD_LowLevel_Init(void);
void sEE_LowLevel_DeInit(void);
void sEE_LowLevel_Init(void);


/**
  * @}
  */

#endif /* __STM8_128_EVAL_H */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
