/**
  ******************************************************************************
  * @file    stm8l1526_eval.h
  * @author  MCD Application Team
  * @version V2.1.3
  * @date    28-June-2013
  * @brief   This file contains definitions for STM8L1526_EVAL's Leds, push-buttons
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
#ifndef __STM8L1526_EVAL_H
#define __STM8L1526_EVAL_H

/* Includes ------------------------------------------------------------------*/
#include "stm8_eval.h"

/** @addtogroup Utilities
  * @{
  */

/** @addtogroup STM8_EVAL
  * @{
  */

/** @addtogroup STM8L1526_EVAL
  * @{
  */

/** @addtogroup STM8L1526_EVAL_LOW_LEVEL
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/** @defgroup STM8L1526_EVAL_LOW_LEVEL_Exported_Constants
  * @{
  */
/** @addtogroup STM8L1526_EVAL_LOW_LEVEL_LED
  * @{
  */

#define LEDn                        5
#define LED1_GPIO_PORT              GPIOA
#define LED1_GPIO_PIN               GPIO_Pin_4

#define LED2_GPIO_PORT              GPIOA
#define LED2_GPIO_PIN               GPIO_Pin_5

#define LED3_GPIO_PORT              GPIOA
#define LED3_GPIO_PIN               GPIO_Pin_6

#define LED4_GPIO_PORT              GPIOA
#define LED4_GPIO_PIN               GPIO_Pin_7

#define LED5_GPIO_PORT              GPIOD
#define LED5_GPIO_PIN               GPIO_Pin_2

/**
  * @}
  */

/** @addtogroup STM8L1526_EVAL_LOW_LEVEL_BUTTON
  * @{
  */
#define BUTTONn                     6

/**
 * @brief Key push-button
 */
#define KEY_BUTTON_PORT             GPIOC
#define KEY_BUTTON_PIN              GPIO_Pin_1

/**
 * @brief Joystick Right push-button
 */
#define RIGHT_BUTTON_PORT           GPIOD
#define RIGHT_BUTTON_PIN            GPIO_Pin_5

/**
 * @brief Joystick Left push-button
 */
#define LEFT_BUTTON_PORT            GPIOE
#define LEFT_BUTTON_PIN             GPIO_Pin_1

/**
 * @brief Joystick Up push-button
 */
#define UP_BUTTON_PORT              GPIOD
#define UP_BUTTON_PIN               GPIO_Pin_7

/**
 * @brief Joystick Down push-button
 */
#define DOWN_BUTTON_PORT            GPIOD
#define DOWN_BUTTON_PIN             GPIO_Pin_4

/**
 * @brief Joystick Sel push-button
 */
#define SEL_BUTTON_PORT             GPIOE
#define SEL_BUTTON_PIN              GPIO_Pin_2


/**
 * @brief Joystick EXTI push-button definition
 */
#define KEY_BUTTON_EXTI             EXTI_Pin_1
#define RIGHT_BUTTON_EXTI           EXTI_Pin_5
#define LEFT_BUTTON_EXTI            EXTI_Pin_1
#define UP_BUTTON_EXTI              EXTI_Pin_7
#define DOWN_BUTTON_EXTI            EXTI_Pin_4
#define SEL_BUTTON_EXTI             EXTI_Pin_2

/**
  * @}
  */

/** @addtogroup STM8L1526_EVAL_LOW_LEVEL_COM
  * @{
  */
#define COMn                        1

/**
 * @brief Definition for COM port1
 */
#define EVAL_COM1                   USART1
#define EVAL_COM1_GPIO              GPIOC
#define EVAL_COM1_CLK               CLK_Peripheral_USART1
#define EVAL_COM1_RxPin             GPIO_Pin_2
#define EVAL_COM1_TxPin             GPIO_Pin_3

/**
  * @}
  */
/** @addtogroup STM8L1526_EVAL_SD_SPI
  * @{
  */
/**
  * @brief  SD SPI Interface pins
  */
#define SD_SPI                           SPI1
#define SD_SPI_CLK                       CLK_Peripheral_SPI1
#define SD_SPI_SCK_PIN                   GPIO_Pin_5                 /* PB.05 */
#define SD_SPI_SCK_GPIO_PORT             GPIOB                       /* GPIOB */
#define SD_SPI_MISO_PIN                  GPIO_Pin_7                  /* PB.07 */
#define SD_SPI_MISO_GPIO_PORT            GPIOB                       /* GPIOB */
#define SD_SPI_MOSI_PIN                  GPIO_Pin_6                  /* PB.06 */
#define SD_SPI_MOSI_GPIO_PORT            GPIOB                       /* GPIOB */
#define SD_CS_PIN                        GPIO_Pin_3                  /* PE.03 */
#define SD_CS_GPIO_PORT                  GPIOE                       /* GPIOE */
#define SD_DETECT_PIN                    GPIO_Pin_6                  /* PD.06 */
#define SD_DETECT_GPIO_PORT              GPIOD                       /* GPIOD */

/**
  * @}
  */



/** @addtogroup STM8L1526_EVAL_LOW_LEVEL_M25P_FLASH_SPI
  * @{
  */
/**
  * @brief  M25P FLASH SPI Interface pins
  */
#define sFLASH_SPI                       SPI1
#define sFLASH_SPI_CLK                   CLK_Peripheral_SPI1
#define sFLASH_SPI_SCK_PIN               GPIO_Pin_5                  /* PB.05 */
#define sFLASH_SPI_SCK_GPIO_PORT         GPIOB                       /* GPIOB */
#define sFLASH_SPI_MISO_PIN              GPIO_Pin_7                  /* PB.07 */
#define sFLASH_SPI_MISO_GPIO_PORT        GPIOB                       /* GPIOB */
#define sFLASH_SPI_MOSI_PIN              GPIO_Pin_6                  /* PB.06 */
#define sFLASH_SPI_MOSI_GPIO_PORT        GPIOB                       /* GPIOAB */
#define sFLASH_CS_PIN                    GPIO_Pin_4                  /* PE.04 */
#define sFLASH_CS_GPIO_PORT              GPIOE                       /* GPIOE */

/**
  * @}
  */

/** @addtogroup STM8L1526_EVAL_LOW_LEVEL_I2C_EE
  * @{
  */
/**
  * @brief  I2C EEPROM Interface pins
  */
#define sEE_I2C                          I2C1
#define sEE_I2C_CLK                      CLK_Peripheral_I2C1
#define sEE_I2C_SCL_PIN                  GPIO_Pin_1                  /* PC.01 */
#define sEE_I2C_SCL_GPIO_PORT            GPIOC                       /* GPIOC */
#define sEE_I2C_SDA_PIN                  GPIO_Pin_0                  /* PC.00 */
#define sEE_I2C_SDA_GPIO_PORT            GPIOC                       /* GPIOC */
#define sEE_M24C64_32

#define sEE_I2C_DMA                      DMA1
#define sEE_I2C_DMA_CHANNEL_TX           DMA1_Channel3
#define sEE_I2C_DMA_CHANNEL_RX           DMA1_Channel0
#define sEE_I2C_DMA_FLAG_TX_TC           DMA1_FLAG_TC3
#define sEE_I2C_DMA_FLAG_RX_TC           DMA1_FLAG_TC0
#define sEE_I2C_DR_Address               ((uint16_t)0x005216)
#define sEE_USE_DMA

#define sEE_DIRECTION_TX                 0
#define sEE_DIRECTION_RX                 1

/**
  * @}
  */

/** @addtogroup STM81526_EVAL_LOW_LEVEL_TSENSOR_I2C
  * @{
  */
/**
  * @brief  LM75 Temperature Sensor I2C Interface pins
  */
#define LM75_I2C                         I2C1
#define LM75_I2C_CLK                     CLK_Peripheral_I2C1
#define LM75_I2C_SCL_PIN                 GPIO_Pin_1                  /* PC.01 */
#define LM75_I2C_SCL_GPIO_PORT           GPIOC                       /* GPIOC */
#define LM75_I2C_SDA_PIN                 GPIO_Pin_0                  /* PC.00 */
#define LM75_I2C_SDA_GPIO_PORT           GPIOC                       /* GPIOC */
#define LM75_I2C_SMBUSALERT_PIN          GPIO_Pin_4                  /* PC.04 */
#define LM75_I2C_SMBUSALERT_GPIO_PORT    GPIOC                       /* GPIOC */

/**
  * @}
  */

/* Exported Macros ------------------------------------------------------------*/

/** @defgroup STM8L1526_EVAL_LOW_LEVEL_Exported_Functions
  * @{
  */
void STM_EVAL_LEDInit(Led_TypeDef Led);
void STM_EVAL_LEDOn(Led_TypeDef Led);
void STM_EVAL_LEDOff(Led_TypeDef Led);
void STM_EVAL_LEDToggle(Led_TypeDef Led);
void STM_EVAL_PBInit(Button_TypeDef Button, ButtonMode_TypeDef Button_Mode);
uint8_t STM_EVAL_PBGetState(Button_TypeDef Button);
void STM_EVAL_COMInit(COM_TypeDef COM, uint32_t USART_BaudRate, USART_WordLength_TypeDef USART_WordLength,\
                      USART_StopBits_TypeDef USART_StopBits,\
                      USART_Parity_TypeDef USART_Parity,\
                      USART_Mode_TypeDef USART_Mode);
void SD_LowLevel_DeInit(void);
void SD_LowLevel_Init(void);
void sFLASH_LowLevel_DeInit(void);
void sFLASH_LowLevel_Init(void);
void sEE_LowLevel_DeInit(void);
void sEE_LowLevel_Init(void);
void sEE_LowLevel_DMAConfig(uint16_t pBuffer, uint8_t BufferSize, uint16_t Direction);
void LM75_LowLevel_DeInit(void);
void LM75_LowLevel_Init(void);
/**
  * @}
  */

#endif /* __STM8L1526_EVAL_H */

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
