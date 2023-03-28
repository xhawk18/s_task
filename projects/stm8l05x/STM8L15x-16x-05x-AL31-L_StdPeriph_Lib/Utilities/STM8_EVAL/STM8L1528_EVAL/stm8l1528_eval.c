/**
  ******************************************************************************
  * @file    stm8_eval.c
  * @author  MCD Application Team
  * @version V2.1.3
  * @date    28-June-2013
  * @brief   This file provides firmware functions to manage Leds, push-buttons
  *          and COM ports available on STM8L Evaluation Boards from STMicroelectronics.
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
#include "stm8l1528_eval.h"
#include "stm8l15x_spi.h"
#include "stm8l15x_i2c.h"
#include "stm8l15x_dma.h"
#include "stm8l15x_syscfg.h"

/** @addtogroup Utilities
  * @{
  */

/** @addtogroup STM8_EVAL
  * @{
  */

/** @addtogroup STM8L1528_EVAL
  * @{
  */

/** @defgroup STM1528_EVAL_LOW_LEVEL
  * @brief This file provides firmware functions to manage Leds, push-buttons,
  *        COM ports, SD card on SPI and EEPROM (sEE) available on STM1528-EVAL
  *        evaluation board from STMicroelectronics.
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

GPIO_TypeDef* LED_PORT[LEDn] = {LED1_GPIO_PORT, LED2_GPIO_PORT, LED3_GPIO_PORT,
                                LED4_GPIO_PORT, LED5_GPIO_PORT};
const uint8_t LED_PIN[LEDn] =
  {
    LED1_GPIO_PIN, LED2_GPIO_PIN, LED3_GPIO_PIN,
    LED4_GPIO_PIN, LED5_GPIO_PIN
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
    KEY_BUTTON_EXTI, RIGHT_BUTTON_EXTI,
    LEFT_BUTTON_EXTI, UP_BUTTON_EXTI,
    DOWN_BUTTON_EXTI, SEL_BUTTON_EXTI
  };


USART_TypeDef* COM_USART[COMn] =
  {
    EVAL_COM1
  };

GPIO_TypeDef* COM_PORT[COMn] =
  {
    EVAL_COM1_GPIO
  };
const uint8_t COM_USART_CLK[COMn] =
  {
    EVAL_COM1_CLK
  };
const uint8_t COM_TX_PIN[COMn] =
  {
    EVAL_COM1_TxPin
  };
const uint8_t COM_RX_PIN[COMn] =
  {
    EVAL_COM1_RxPin
  };
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/** @defgroup STM8L1528_EVAL_LOW_LEVEL_Private_Functions
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
  *     @arg LED5
  * @retval None
  */
void STM_EVAL_LEDInit(Led_TypeDef Led)
{
  /* Configure the GPIO_LED pin */
  GPIO_Init(LED_PORT[Led], LED_PIN[Led], GPIO_Mode_Out_PP_High_Fast);
}

/**
  * @brief  Turns selected LED On.
  * @param  Led: Specifies the Led to be set on.
  *   This parameter can be one of following parameters:
  *     @arg LED1
  *     @arg LED2
  *     @arg LED3
  *     @arg LED4
  *     @arg LED5
  * @retval None
  */
void STM_EVAL_LEDOn(Led_TypeDef Led)
{
  LED_PORT[Led]->ODR &= (uint8_t)~LED_PIN[Led];
}

/**
  * @brief  Turns selected LED Off.
  * @param  Led: Specifies the Led to be set off.
  *   This parameter can be one of following parameters:
  *     @arg LED1
  *     @arg LED2
  *     @arg LED3
  *     @arg LED4
  *     @arg LED5
  * @retval None
  */
void STM_EVAL_LEDOff(Led_TypeDef Led)
{
  LED_PORT[Led]->ODR |= (uint8_t)LED_PIN[Led];
}

/**
  * @brief  Toggles the selected LED.
  * @param  Led: Specifies the Led to be toggled.
  *   This parameter can be one of following parameters:
  *     @arg LED1
  *     @arg LED2
  *     @arg LED3
  *     @arg LED4
  *     @arg LED5
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
    GPIO_Init(BUTTON_PORT[Button], BUTTON_PIN[Button], GPIO_Mode_In_FL_IT);
    EXTI_SetPinSensitivity((EXTI_Pin_TypeDef)BUTTON_EXTI[Button], EXTI_Trigger_Falling);
  }
  else
  { /* Pin configured in input floating mode with interrupt disabled */
    GPIO_Init(BUTTON_PORT[Button], BUTTON_PIN[Button], GPIO_Mode_In_FL_No_IT);
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
  return GPIO_ReadInputDataBit(BUTTON_PORT[Button], (GPIO_Pin_TypeDef)BUTTON_PIN[Button]);
}


/**
  * @brief  Configures COM port.
  * @param  COM: Specifies the COM port to be configured.
  *   This parameter should be COM1.
  * @param  USART_InitStruct: pointer to a USART_InitTypeDef structure that
  *   contains the configuration information for the specified USART peripheral.
  * @retval None
  */
void STM_EVAL_COMInit(COM_TypeDef COM, uint32_t USART_BaudRate,
                      USART_WordLength_TypeDef USART_WordLength,
                      USART_StopBits_TypeDef USART_StopBits,
                      USART_Parity_TypeDef USART_Parity,
                      USART_Mode_TypeDef USART_Mode)
{
  /* Enable USART clock */
  CLK_PeripheralClockConfig((CLK_Peripheral_TypeDef)COM_USART_CLK[COM], ENABLE);

  /* Configure USART Tx as alternate function push-pull  (software pull up)*/
  GPIO_ExternalPullUpConfig(COM_PORT[COM], COM_TX_PIN[COM], ENABLE);

  /* Configure USART Rx as alternate function push-pull  (software pull up)*/
  GPIO_ExternalPullUpConfig(COM_PORT[COM], COM_RX_PIN[COM], ENABLE);

  /* USART configuration */
  USART_Init(COM_USART[COM], USART_BaudRate,
             USART_WordLength,
             USART_StopBits,
             USART_Parity,
             USART_Mode);
}

/**
  * @brief  DeInitializes the SD/SD communication.
  * @param  None
  * @retval None
  */
void SD_LowLevel_DeInit(void)
{
  SPI_Cmd(SD_SPI, DISABLE); /*!< SD_SPI disable */

  /*!< SD_SPI Periph clock disable */
  CLK_PeripheralClockConfig(SD_SPI_CLK, DISABLE);

  /*!< Configure SD_SPI pins: SCK */
  GPIO_Init(SD_SPI_SCK_GPIO_PORT, SD_SPI_SCK_PIN, GPIO_Mode_In_FL_No_IT);

  /*!< Configure SD_SPI pins: MISO */
  GPIO_Init(SD_SPI_MISO_GPIO_PORT, SD_SPI_MISO_PIN, GPIO_Mode_In_FL_No_IT);

  /*!< Configure SD_SPI pins: MOSI */
  GPIO_Init(SD_SPI_MOSI_GPIO_PORT, SD_SPI_MOSI_PIN, GPIO_Mode_In_FL_No_IT);

  /*!< Configure SD_SPI_CS_PIN pin: SD Card CS pin */
  GPIO_Init(SD_CS_GPIO_PORT, SD_CS_PIN, GPIO_Mode_In_FL_No_IT);

  /*!< Configure SD_SPI_DETECT_PIN pin: SD Card detect pin */
  GPIO_Init(SD_DETECT_GPIO_PORT, SD_DETECT_PIN, GPIO_Mode_In_FL_No_IT);
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
  GPIO_ExternalPullUpConfig(SD_SPI_SCK_GPIO_PORT, SD_SPI_MISO_PIN | SD_SPI_MOSI_PIN | \
                            SD_SPI_SCK_PIN, ENABLE);

  /* SPI2 pin remap on Port I*/
  SYSCFG_REMAPPinConfig(REMAP_Pin_SPI2Full, ENABLE);

  /* SD_SPI Config */
  SPI_Init(SD_SPI, SPI_FirstBit_MSB, SPI_BaudRatePrescaler_4, SPI_Mode_Master,
           SPI_CPOL_High, SPI_CPHA_2Edge, SPI_Direction_2Lines_FullDuplex,
           SPI_NSS_Soft, 0x07);


  /* SD_SPI enable */
  SPI_Cmd(SD_SPI, ENABLE);

  /* Configure the SD_Detect pin */
  GPIO_Init(SD_DETECT_GPIO_PORT, SD_DETECT_PIN, GPIO_Mode_In_PU_No_IT);

  /* Set MSD ChipSelect pin in Output push-pull high level */
  GPIO_Init(SD_CS_GPIO_PORT, SD_CS_PIN, GPIO_Mode_Out_PP_High_Slow);
}

/**
  * @brief  DeInitializes the peripherals used by the SPI FLASH driver.
  * @param  None
  * @retval None
  */
void sFLASH_LowLevel_DeInit(void)
{
  /*!< Disable the sFLASH_SPI  */
  SPI_Cmd(sFLASH_SPI, DISABLE);

  /*!< sFLASH_SPI Periph clock disable */
  CLK_PeripheralClockConfig(sFLASH_SPI_CLK, DISABLE);

  /*!< Configure sFLASH_SPI pins: SCK */
  GPIO_Init(sFLASH_SPI_SCK_GPIO_PORT, sFLASH_SPI_SCK_PIN, GPIO_Mode_In_FL_No_IT);

  /*!< Configure sFLASH_SPI pins: MISO */
  GPIO_Init(sFLASH_SPI_MISO_GPIO_PORT, sFLASH_SPI_MISO_PIN, GPIO_Mode_In_FL_No_IT);

  /*!< Configure sFLASH_SPI pins: MOSI */
  GPIO_Init(sFLASH_SPI_MOSI_GPIO_PORT, sFLASH_SPI_MOSI_PIN, GPIO_Mode_In_FL_No_IT);

  /*!< Configure sFLASH_CS_PIN pin: sFLASH Card CS pin */
  GPIO_Init(sFLASH_CS_GPIO_PORT, sFLASH_CS_PIN, GPIO_Mode_In_FL_No_IT);
}

/**
  * @brief  Initializes the peripherals used by the SPI FLASH driver.
  * @param  None
  * @retval None
  */
void sFLASH_LowLevel_Init(void)
{

  /* sFLASH_SPI Periph clock enable */
  CLK_PeripheralClockConfig(sFLASH_SPI_CLK, ENABLE);

  /* Set the MOSI,MISO and SCK at high level */
  GPIO_ExternalPullUpConfig(sFLASH_SPI_SCK_GPIO_PORT, sFLASH_SPI_SCK_PIN | \
                            sFLASH_SPI_MISO_PIN | sFLASH_SPI_MOSI_PIN, ENABLE);

  /* SPI2 pin remap on Port I*/
  SYSCFG_REMAPPinConfig(REMAP_Pin_SPI2Full, ENABLE);

  /* Configure FLASH_CS as Output push-pull, used as Flash Chip select */
  GPIO_Init(sFLASH_CS_GPIO_PORT, sFLASH_CS_PIN, GPIO_Mode_Out_PP_High_Slow);
}

/**
  * @brief  DeInitializes peripherals used by the I2C EEPROM driver.
  * @param  None
  * @retval None
  */
void sEE_LowLevel_DeInit(void)
{
  /* sEE_I2C Peripheral Disable */
  I2C_Cmd(sEE_I2C, DISABLE);

  /* sEE_I2C DeInit */
  I2C_DeInit(sEE_I2C);

  /*!< sEE_I2C Periph clock disable */
  CLK_PeripheralClockConfig(sEE_I2C_CLK, DISABLE);

  /*!< GPIO configuration */
  /*!< Configure sEE_I2C pins: SCL */
  GPIO_Init(sEE_I2C_SCL_GPIO_PORT, sEE_I2C_SCL_PIN, GPIO_Mode_In_PU_No_IT);

  /*!< Configure sEE_I2C pins: SDA */
  GPIO_Init(sEE_I2C_SDA_GPIO_PORT, sEE_I2C_SDA_PIN, GPIO_Mode_In_PU_No_IT);

  /* Disable and Deinitialize the DMA channels */
  DMA_Cmd(sEE_I2C_DMA_CHANNEL_TX, DISABLE);
  DMA_Cmd(sEE_I2C_DMA_CHANNEL_RX, DISABLE);
  DMA_DeInit(sEE_I2C_DMA_CHANNEL_TX);
  DMA_DeInit(sEE_I2C_DMA_CHANNEL_RX);
}

/**
  * @brief  Initializes peripherals used by the I2C EEPROM driver.
  * @param  None
  * @retval None
  */
void sEE_LowLevel_Init(void)
{
  /*!< sEE_I2C Periph clock enable */
  CLK_PeripheralClockConfig(sEE_I2C_CLK, ENABLE);

  /*!< Enable the DMA clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_DMA1, ENABLE);

  /* I2C TX DMA Channel configuration */
  DMA_DeInit(sEE_I2C_DMA_CHANNEL_TX);
  DMA_Init(sEE_I2C_DMA_CHANNEL_TX,
           0, /* This parameter will be configured durig communication */
           sEE_I2C_DR_Address,
           0xFF, /* This parameter will be configured durig communication */
           DMA_DIR_PeripheralToMemory,/* This parameter will be configured durig communication */
           DMA_Mode_Normal,
           DMA_MemoryIncMode_Inc,
           DMA_Priority_VeryHigh,
           DMA_MemoryDataSize_Byte);

  /* I2C RX DMA Channel configuration */
  DMA_DeInit(sEE_I2C_DMA_CHANNEL_RX);
  DMA_Init(sEE_I2C_DMA_CHANNEL_RX, 0, /* This parameter will be configured durig communication */
           sEE_I2C_DR_Address,
           0xFF, /* This parameter will be configured durig communication */
           DMA_DIR_PeripheralToMemory,/* This parameter will be configured durig communication */
           DMA_Mode_Normal,
           DMA_MemoryIncMode_Inc,
           DMA_Priority_VeryHigh,
           DMA_MemoryDataSize_Byte);


}


/**
  * @brief  Initializes DMA channel used by the I2C EEPROM driver.
  * @param  None
  * @retval None
  */
void sEE_LowLevel_DMAConfig(uint16_t pBuffer, uint8_t BufferSize, uint16_t Direction)
{
  /* Initialize the DMA with the new parameters */
  if (Direction == sEE_DIRECTION_TX)
  {
    /* Configure the DMA Tx Channel with the buffer address and the buffer size */
    DMA_Init(sEE_I2C_DMA_CHANNEL_TX, pBuffer, sEE_I2C_DR_Address, BufferSize,
             DMA_DIR_MemoryToPeripheral, DMA_Mode_Normal, DMA_MemoryIncMode_Inc,
             DMA_Priority_VeryHigh, DMA_MemoryDataSize_Byte);
  }
  else
  {
    /* Configure the DMA Rx Channel with the buffer address and the buffer size */
    DMA_Init(sEE_I2C_DMA_CHANNEL_RX, pBuffer, sEE_I2C_DR_Address, BufferSize,
             DMA_DIR_PeripheralToMemory, DMA_Mode_Normal, DMA_MemoryIncMode_Inc,
             DMA_Priority_VeryHigh, DMA_MemoryDataSize_Byte);
  }

  /* Enable the DMA Channels Interrupts */
  DMA_ITConfig(sEE_I2C_DMA_CHANNEL_TX, DMA_ITx_TC, ENABLE);
  DMA_ITConfig(sEE_I2C_DMA_CHANNEL_RX, DMA_ITx_TC, ENABLE);
}

/**
  * @brief  DeInitializes the LM75_I2C.
  * @param  None
  * @retval None
  */
void LM75_LowLevel_DeInit(void)
{
  /*!< Disable LM75_I2C */
  I2C_Cmd(LM75_I2C, DISABLE);
  /*!< DeInitializes the LM75_I2C */
  I2C_DeInit(LM75_I2C);

  /*!< LM75_I2C Periph clock disable */
  CLK_PeripheralClockConfig(LM75_I2C_CLK, DISABLE);

  /*!< Configure LM75_I2C pins: SCL */
  GPIO_Init(LM75_I2C_SCL_GPIO_PORT, LM75_I2C_SCL_PIN, GPIO_Mode_In_PU_No_IT);

  /*!< Configure LM75_I2C pins: SDA */
  GPIO_Init(LM75_I2C_SDA_GPIO_PORT, LM75_I2C_SDA_PIN, GPIO_Mode_In_PU_No_IT);

  /*!< Configure LM75_I2C pin: SMBUS ALERT */
  GPIO_Init(LM75_I2C_SMBUSALERT_GPIO_PORT, LM75_I2C_SMBUSALERT_PIN, GPIO_Mode_In_FL_No_IT);
}

/**
  * @brief  Initializes the LM75_I2C..
  * @param  None
  * @retval None
  */
void LM75_LowLevel_Init(void)
{
  /*!< LM75_I2C Periph clock enable */
  CLK_PeripheralClockConfig(LM75_I2C_CLK, ENABLE);

  /* Configure PC.4 as Input pull-up, used as TemperatureSensor_INT */
  GPIO_Init(LM75_I2C_SMBUSALERT_GPIO_PORT, LM75_I2C_SMBUSALERT_PIN, GPIO_Mode_In_FL_No_IT);

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
