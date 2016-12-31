/**
  ******************************************************************************
  * @file    stm8_128_eval.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    30-September-2014
  * @brief   This file provides firmware functions to manage Leds, push-buttons
  *          and COM ports available on STM8S Evaluation Boards from STMicroelectronics.
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

/* Includes ------------------------------------------------------------------*/
#include "stm8_128_eval.h"
#include "stm8s_spi.h"
#include "stm8s_i2c.h"
#include "stm8s_clk.h"

/** @addtogroup Utilities
  * @{
  */

/** @addtogroup STM8S_EVAL
  * @{
  */

/** @addtogroup STM8_128_EVAL
  * @{
  */

/** @defgroup STM8_128_EVAL_LOW_LEVEL
  * @brief This file provides firmware functions to manage Leds, push-buttons,
  *        COM ports, SD card on SPI and EEPROM (sEE) available on STM8_128-EVAL
  *        evaluation board from STMicroelectronics.
  * @{
  */

/* Private types definition --------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/


/** @defgroup STM8_128_EVAL_LOW_LEVEL_Private_Variables
  * @{
  */
GPIO_TypeDef* LED_PORT[LEDn] = {LED1_GPIO_PORT, LED2_GPIO_PORT, LED3_GPIO_PORT,
                                LED4_GPIO_PORT};
const uint8_t LED_PIN[LEDn] =
  {
    LED1_GPIO_PIN, LED2_GPIO_PIN, LED3_GPIO_PIN,
    LED4_GPIO_PIN
  };

GPIO_TypeDef* BUTTON_PORT[BUTTONn] =
  {
    KEY_BUTTON_PORT, RIGHT_BUTTON_PORT,
    LEFT_BUTTON_PORT, UP_BUTTON_PORT,
    DOWN_BUTTON_PORT, SEL_BUTTON_PORT
  };
const uint8_t BUTTON_PIN[BUTTONn] =
  {
    KEY_BUTTON_PIN, RIGHT_BUTTON_PIN,
    LEFT_BUTTON_PIN, UP_BUTTON_PIN,
    DOWN_BUTTON_PIN, SEL_BUTTON_PIN
  };

const uint8_t BUTTON_EXTI[BUTTONn] =
  {
    KEY_BUTTON_EXTI_PORT, RIGHT_BUTTON_EXTI_PORT,
    LEFT_BUTTON_EXTI_PORT, UP_BUTTON_EXTI_PORT,
    DOWN_BUTTON_EXTI_PORT, SEL_BUTTON_EXTI_PORT
  };
/**
  * @}
  */

/* Private function prototypes -----------------------------------------------*/

/** @defgroup STM8_128_EVAL_LOW_LEVEL_Private_Functions
  * @{
  */

/**
  * @brief  Configures LED GPIO.
  * @param  Led: Specifies the Led to be configured.
  *   This parameter can be one of following parameters:
  *     @arg LED1
  *     @arg LED2
  *     @arg LED3
  *     @arg LED4
  * @retval None
  */
void STM_EVAL_LEDInit(Led_TypeDef Led)
{
  /* Configure the GPIO_LED pin */
  GPIO_Init(LED_PORT[Led], (GPIO_Pin_TypeDef)LED_PIN[Led], GPIO_MODE_OUT_PP_HIGH_FAST);
}

/**
  * @brief  Turns selected LED On.
  * @param  Led: Specifies the Led to be set on.
  *   This parameter can be one of following parameters:
  *     @arg LED1
  *     @arg LED2
  *     @arg LED3
  *     @arg LED4
  * @retval None
  */
void STM_EVAL_LEDOn(Led_TypeDef Led)
{
  LED_PORT[Led]->ODR |= (uint8_t)LED_PIN[Led];
}

/**
  * @brief  Turns selected LED Off.
  * @param  Led: Specifies the Led to be set off.
  *   This parameter can be one of following parameters:
  *     @arg LED1
  *     @arg LED2
  *     @arg LED3
  *     @arg LED4
  * @retval None
  */
void STM_EVAL_LEDOff(Led_TypeDef Led)
{
  LED_PORT[Led]->ODR &= (uint8_t)~LED_PIN[Led];
}

/**
  * @brief  Toggles the selected LED.
  * @param  Led: Specifies the Led to be toggled.
  *   This parameter can be one of following parameters:
  *     @arg LED1
  *     @arg LED2
  *     @arg LED3
  *     @arg LED4
  * @retval None
  */
void STM_EVAL_LEDToggle(Led_TypeDef Led)
{
  LED_PORT[Led]->ODR ^= (uint8_t)LED_PIN[Led];
}

/**
  * @brief  Configures Button GPIO and EXTI Line.
  * @param  Button: Specifies the Button to be configured.
  *   This parameter can be one of following parameters:
  *     @arg BUTTON_KEY: Key Push Button
  *     @arg BUTTON_RIGHT: Joystick Right Push Button
  *     @arg BUTTON_LEFT: Joystick Left Push Button
  *     @arg BUTTON_UP: Joystick Up Push Button
  *     @arg BUTTON_DOWN: Joystick Down Push Button
  *     @arg BUTTON_SEL: Joystick Sel Push Button
  * @param  Button_Mode: Specifies Button mode.
  *   This parameter can be one of following parameters:
  *     @arg BUTTON_MODE_GPIO: Button will be used as simple IO
  *     @arg BUTTON_MODE_EXTI: Button will be connected to EXTI pin (sensitive to falling trigger)
  * @retval None
  */
void STM_EVAL_PBInit(Button_TypeDef Button, ButtonMode_TypeDef Button_Mode)
{

  if (Button_Mode == BUTTON_MODE_EXTI)
  { /* Pin configured in input floating mode with interrupt enabled
                       --> connected to EXTIx Interrupt, where x:0..7 */
    GPIO_Init(BUTTON_PORT[Button], (GPIO_Pin_TypeDef)BUTTON_PIN[Button], GPIO_MODE_IN_FL_IT);
    EXTI_SetExtIntSensitivity((EXTI_Port_TypeDef)BUTTON_EXTI[Button], EXTI_SENSITIVITY_FALL_LOW);
  }
  else
  { /* Pin configured in input floating mode with interrupt disabled */
    GPIO_Init(BUTTON_PORT[Button], (GPIO_Pin_TypeDef)BUTTON_PIN[Button], GPIO_MODE_IN_FL_NO_IT);
  }
}

/**
  * @brief  Returns the selected Button state.
  * @param  Button: Specifies the Button to be checked.
  *   This parameter can be one of following parameters:
  *     @arg BUTTON_KEY: Key Push Button
  *     @arg BUTTON_RIGHT: Joystick Right Push Button
  *     @arg BUTTON_LEFT: Joystick Left Push Button
  *     @arg BUTTON_UP: Joystick Up Push Button
  *     @arg BUTTON_DOWN: Joystick Down Push Button
  *     @arg BUTTON_SEL: Joystick Sel Push Button
  * @retval The Button GPIO pin value.
  */
uint8_t STM_EVAL_PBGetState(Button_TypeDef Button)
{
  return GPIO_ReadInputPin(BUTTON_PORT[Button], (GPIO_Pin_TypeDef)BUTTON_PIN[Button]);
}


/**
  * @brief  DeInitializes the SD/SD communication.
  * @param  None
  * @retval None
  */
void SD_LowLevel_DeInit(void)
{
  SPI_Cmd(DISABLE); /*!< SD_SPI disable */

  /*!< SD_SPI Peripheral clock disable */
  CLK_PeripheralClockConfig(SD_SPI_CLK, DISABLE);

  /*!< Configure SD_SPI pins: SCK */
  GPIO_Init(SD_SPI_SCK_GPIO_PORT, SD_SPI_SCK_PIN, GPIO_MODE_IN_FL_NO_IT);

  /*!< Configure SD_SPI pins: MISO */
  GPIO_Init(SD_SPI_MISO_GPIO_PORT, SD_SPI_MISO_PIN, GPIO_MODE_IN_FL_NO_IT);

  /*!< Configure SD_SPI pins: MOSI */
  GPIO_Init(SD_SPI_MOSI_GPIO_PORT, SD_SPI_MOSI_PIN, GPIO_MODE_IN_FL_NO_IT);

  /*!< Configure SD_SPI_CS_PIN pin: SD Card CS pin */
  GPIO_Init(SD_CS_GPIO_PORT, SD_CS_PIN, GPIO_MODE_IN_FL_NO_IT);

  /*!< Configure SD_SPI_DETECT_PIN pin: SD Card detect pin */
  GPIO_Init(SD_DETECT_GPIO_PORT, SD_DETECT_PIN, GPIO_MODE_IN_FL_NO_IT);
}

/**
  * @brief  Initializes the SD_SPI and CS pins.
  * @param  None
  * @retval None
  */
void SD_LowLevel_Init(void)
{
  /* Enable SPI clock */
  CLK_PeripheralClockConfig(SD_SPI_CLK, ENABLE);

  /* Set the MOSI,MISO and SCK at high level */
  GPIO_ExternalPullUpConfig(SD_SPI_SCK_GPIO_PORT, (GPIO_Pin_TypeDef)(SD_SPI_MISO_PIN | SD_SPI_MOSI_PIN | \
                            SD_SPI_SCK_PIN), ENABLE);

  /* SD_SPI Configuration */
  SPI_Init( SPI_FIRSTBIT_MSB, SPI_BAUDRATEPRESCALER_4, SPI_MODE_MASTER,
           SPI_CLOCKPOLARITY_HIGH, SPI_CLOCKPHASE_2EDGE, SPI_DATADIRECTION_2LINES_FULLDUPLEX,
           SPI_NSS_SOFT, 0x07);


  /* SD_SPI enable */
  SPI_Cmd( ENABLE);

  /* Set MSD ChipSelect pin in Output push-pull high level */
  GPIO_Init(SD_CS_GPIO_PORT, SD_CS_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
}

/**
  * @brief  DeInitializes peripherals used by the I2C EEPROM driver.
  * @param  None
  * @retval None
  */
void sEE_LowLevel_DeInit(void)
{
  /* sEE_I2C Peripheral Disable */
  I2C_Cmd(DISABLE);

  /* sEE_I2C DeInit */
  I2C_DeInit();

  /*!< sEE_I2C Peripheral clock disable */
  CLK_PeripheralClockConfig(sEE_I2C_CLK, DISABLE);

  /*!< GPIO configuration */
  /*!< Configure sEE_I2C pins: SCL */
  GPIO_Init(sEE_I2C_SCL_GPIO_PORT, sEE_I2C_SCL_PIN, GPIO_MODE_IN_PU_NO_IT);

  /*!< Configure sEE_I2C pins: SDA */
  GPIO_Init(sEE_I2C_SDA_GPIO_PORT, sEE_I2C_SDA_PIN, GPIO_MODE_IN_PU_NO_IT);
}

/**
  * @brief  Initializes peripherals used by the I2C EEPROM driver.
  * @param  None
  * @retval None
  */
void sEE_LowLevel_Init(void)
{
  /*!< sEE_I2C Peripheral clock enable */
  CLK_PeripheralClockConfig(sEE_I2C_CLK, ENABLE);

}

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

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
