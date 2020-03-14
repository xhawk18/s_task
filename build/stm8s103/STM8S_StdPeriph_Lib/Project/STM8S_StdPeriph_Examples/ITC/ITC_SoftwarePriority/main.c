/**
  ******************************************************************************
  * @file    ITC_SoftwarePriority\main.c
  * @author  MCD Application Team
  * @version  V2.2.0
  * @date     30-September-2014
  * @brief   This file contains the main function for the ITC software priority example.
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
  * @addtogroup ITC_SoftwarePriority
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void GPIO_Config(void);
void ITC_Config(void);
void Delay (uint16_t nCount);
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

/**
  * @brief Example main entry point.
  * @param  None
  * @retval None
  */
void main(void)
{
  
    /* GPIO Configuration  -----------------------------------------*/
    GPIO_Config();  
    
    /* ITC Configuration  -----------------------------------------*/
    ITC_Config();  

  while (1)
  {}
}

/**
  * @brief  Configure GPIO for buttons and LEDs available on the evaluation board
  * @param  None
  * @retval None
  */
void GPIO_Config(void)
{
  
  /* Configure GPIO to drive LEDs on Eval board */
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  
  /* LED1 & LED2 are on oer default */
  STM_EVAL_LEDOn(LED1);
  STM_EVAL_LEDOn(LED2);
  
  /* Configure GPIO used to drive the joystick Down and the Key button */
    STM_EVAL_PBInit(BUTTON_KEY, BUTTON_MODE_EXTI);
    STM_EVAL_PBInit(BUTTON_DOWN, BUTTON_MODE_EXTI);
}

/**
  * @brief  Change the software priority between Key button and joystick Down
  * @param  None
  * @retval None
  */
void ITC_Config(void)
{
  /*
    Change the software priority between button and joystick:
    - By hardware the PORTB (joystick) has a higher priority than PORTC (button)
    - We configure the PORTC (button) to have a higher sw priority than PORTB (joystick)
  */
  ITC_SetSoftwarePriority(ITC_IRQ_PORTB, ITC_PRIORITYLEVEL_1); /* joystick Down*/
  ITC_SetSoftwarePriority(ITC_IRQ_PORTC, ITC_PRIORITYLEVEL_2); /* button = higher sw priority */
  
  enableInterrupts();
}


/**
  * @brief Delay.
  * @param nCount
  * @retval None
   */
void Delay(uint16_t nCount)
{
    /* Decrement nCount value */
    while (nCount != 0)
    {
        nCount--;
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
