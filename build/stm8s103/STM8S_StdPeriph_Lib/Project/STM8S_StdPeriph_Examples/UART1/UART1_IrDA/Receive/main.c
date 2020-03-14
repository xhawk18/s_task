/**
  ******************************************************************************
  * @file UART1_IrDA\Receive\main.c
  * @brief This file contains the main function for UART1 in IrDA receive mode.
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
  * @addtogroup UART1_IrDA_Receive
  * @{
  */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
uint8_t ReceivedData = 0;
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void GPIO_Config(void);
static void UART1_Config(void);
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{
  /* GPIO configuration -----------------------------------------*/
  GPIO_Config();  
  
  /* UART1 configuration -----------------------------------------*/
  UART1_Config();  
    
    while (1)
    {
      /* Wait until a byte is received */
      while (UART1_GetFlagStatus(UART1_FLAG_RXNE) == RESET)
      {}
      /* Read the received byte */
      ReceivedData = UART1_ReceiveData8();

      switch (ReceivedData)
      {
        /* LED4 toggle */
        case JOY_UP:
          STM_EVAL_LEDToggle(LED4);
          break;
        /* LED3 toggle */
        case JOY_DOWN:
          STM_EVAL_LEDToggle(LED3);
          break;
        /* LED2 toggle */
        case JOY_LEFT:
          STM_EVAL_LEDToggle(LED2);
          break;
        /* LED1 toggle */
        case JOY_RIGHT:
          STM_EVAL_LEDToggle(LED1);
          break;

        case JOY_SEL:
          STM_EVAL_LEDToggle(LED1);
          STM_EVAL_LEDToggle(LED2);
          STM_EVAL_LEDToggle(LED3);
          STM_EVAL_LEDToggle(LED4);
          break;
        default:
          break;
    }
    }

}
/**
  * @brief  Configures the Multiplexer on the evaluation board  to select the IrDA
  *         and initialize GPIO for LEDs. 
  * @param  None
  * @retval None
  */
static void GPIO_Config(void)
{
  /* Configures the Multiplexer on the evaluation board to select the IrDA */
  /* Initialize I/Os in Output PP Mode */
  GPIO_Init(GPIOF, GPIO_PIN_6 ,GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(GPIOF, GPIO_PIN_5 ,GPIO_MODE_OUT_PP_HIGH_FAST);
    
  /* Initialize I/Os in Output Mode for LEDs */
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);

  /* Turn on LEDs */
  STM_EVAL_LEDOn(LED1);
  STM_EVAL_LEDOn(LED2);
  STM_EVAL_LEDOn(LED3);
  STM_EVAL_LEDOn(LED4);
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
