/**
  ******************************************************************************
  * @file UART1_IrDA\Transmit\main.c
  * @brief This file contains the main function for UART1 in IrDA transmit mode.
  * @author  MCD Application Team
  * @version  V2.2.0
  * @date     30-September-2014
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

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "stm8s_eval.h"

/**
  * @addtogroup UART1_IrDA_Transmit
  * @{
  */
 
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Evalboard I/Os configuration */
#define Bit_RESET  0
#define Bit_SET    1
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void GPIO_Config(void);
static void UART1_Config(void);
JOYState_TypeDef ReadJoystick(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{

  JOYState_TypeDef Key = JOY_NONE;

  /* GPIO configuration -----------------------------------------*/
  GPIO_Config();

  /* UART1 configuration -----------------------------------------*/
  UART1_Config();  
  
  while (1)
  {
    /* Read Key */
    while (Key == JOY_NONE)
    {
      Key = (JOYState_TypeDef)ReadJoystick();
    }

    switch (Key)
    {
      case JOY_UP:
        UART1_SendData8(JOY_UP);
        while (UART1_GetFlagStatus(UART1_FLAG_TC) == RESET)
        {}
        Key = JOY_NONE;
        break;
      case JOY_DOWN:
        UART1_SendData8(JOY_DOWN);
        while (UART1_GetFlagStatus(UART1_FLAG_TC) == RESET)
        {}
        Key = JOY_NONE;
        break;
      case JOY_LEFT:
        UART1_SendData8(JOY_LEFT);
        while (UART1_GetFlagStatus(UART1_FLAG_TC) == RESET)
        {}
        Key = JOY_NONE;
        break;
      case JOY_RIGHT:
        UART1_SendData8(JOY_RIGHT);
        while (UART1_GetFlagStatus(UART1_FLAG_TC) == RESET)
        {}
        Key = JOY_NONE;
        break;
      case JOY_SEL:
        UART1_SendData8(JOY_SEL);
        while (UART1_GetFlagStatus(UART1_FLAG_TC) == RESET)
        {}
        Key = JOY_NONE;
        break;
      default:
        break;
    }
  }

}

/**
  * @brief  Configures the Multiplexer on the evaluation board  to select the IrDA
  * @param  None
  * @retval None
  */
static void GPIO_Config(void)
{
  /* Configures the Multiplexer on the evaluation board to select the IrDA */
  /* Initialize I/Os in Output PP Mode */
  GPIO_Init(GPIOF, GPIO_PIN_6 ,GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(GPIOF, GPIO_PIN_5 ,GPIO_MODE_OUT_PP_HIGH_FAST);
}

/**
  * @brief  Configure UART1 for the communication with IrDA
  * @param  None
  * @retval None
  */
static void UART1_Config(void)
{
  UART1_DeInit();
  
  /* UART1 configured as follow:
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - BaudRate = 9600 baud
        - Tx and Rx enabled
        - UART1 Clock disabled
  */
  UART1_Init((uint32_t)9600, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO,
              UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);

  /* Set Prescaler*/
  UART1_SetPrescaler(0x1);

  UART1_IrDAConfig(UART1_IRDAMODE_NORMAL);

  UART1_IrDACmd(ENABLE);
}

/**
  * @brief  Reads joystick position from evalboard.
  * @param  None.
  * @retval Joystick position.
  *  This parameter can be a value of @ref JOYState_TypeDef enumeration.
  * @par Required preconditions:
  * None
  */
JOYState_TypeDef ReadJoystick(void)
{
  /* "right" key is pressed */
  if (!STM_EVAL_PBGetState(BUTTON_RIGHT))
  {
    while (STM_EVAL_PBGetState(BUTTON_RIGHT) == Bit_RESET);
    return JOY_RIGHT;
  }
  /* "left" key is pressed */
  if (!STM_EVAL_PBGetState(BUTTON_LEFT))
  {
    while (STM_EVAL_PBGetState(BUTTON_LEFT) == Bit_RESET);
    return JOY_LEFT;
  }
  /* "up" key is pressed */
  if (!STM_EVAL_PBGetState(BUTTON_UP))
  {
    while (STM_EVAL_PBGetState(BUTTON_UP) == Bit_RESET);
    return JOY_UP;
  }
  /* "down" key is pressed */
  if (!STM_EVAL_PBGetState(BUTTON_DOWN))
  {
    while (STM_EVAL_PBGetState(BUTTON_DOWN) == Bit_RESET);
    return JOY_DOWN;
  }
  /* "sel" key is pressed */
  if (!STM_EVAL_PBGetState(BUTTON_SEL))
  {
    while (STM_EVAL_PBGetState(BUTTON_SEL) == Bit_RESET);
    return JOY_SEL;
  }
  /* No key is pressed */
  else
  {
    return JOY_NONE;
  }
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif
/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
