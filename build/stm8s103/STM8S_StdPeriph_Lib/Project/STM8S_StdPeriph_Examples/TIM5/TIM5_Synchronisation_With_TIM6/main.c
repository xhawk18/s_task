/**
  ******************************************************************************
  * @file    TIM5_Synchronisation_with_TIM6\main.c
  * @author  MCD Application Team
  * @version  V2.2.0
  * @date     30-September-2014
  * @brief   This file contains the main function for TIM5 example.
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

/**
  * @addtogroup TIM5_Synchronisation_with_TIM6
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define TIMEOUT ((uint8_t)0xFF)
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void TIM5_Config(void);
static void TIM6_Config(void);
static FunctionalState TIM5_Status(void);
/* Private functions ---------------------------------------------------------*/
static FunctionalState TIM5_Status(void) 
{
   return ((FunctionalState)(TIM5->CR1 & TIM5_CR1_CEN));
}
/* Public functions ----------------------------------------------------------*/
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{
  
  __IO uint8_t Result = 0;
  __IO uint8_t timeout = TIMEOUT;
  
  /* TIM5 configuration -----------------------------------------*/
  TIM5_Config();   

  /* TIM6 configuration -----------------------------------------*/
  TIM6_Config();   

  /*Enable TIM6*/
  TIM6_Cmd(ENABLE);/* if this line is commented, TIM6 will not be enabled and */
                   /* TIM5  will not detect an Edge on its TRGI => Result =0  */
  
  while((TIM5_Status() == DISABLE) && (timeout != 0))
  {
    /* Decrement timeout */
    timeout--;
  }
  
  if (TIM5_Status() != DISABLE)
  {
    /*If the program counter reaches this section, 
    the TIM5 is correctly triggered by TIM6 */
    /*Insert break point in the following line */
    Result = 1;
  }
  else
  {
    /*If the program counter reaches this section, 
      the TIM5 is not correctly triggered by TIM6 */
    /*Insert break point in the following line */
    Result = 0;
  }
  /* infinite loop */
  while(1)
  {
  }
}

/**
  * @brief  TIM5 Configuration.
  * @param  None
  * @retval None
  */
static void TIM5_Config(void)
{
  /*DeInit TIM5 registers*/
  TIM5_DeInit();
  
  /* Time base configuration */
  TIM5_TimeBaseInit(TIM5_PRESCALER_1, 0xFF );
  
  /* Select TIM6 as source of TIM5 Trigger Input*/
  TIM5_SelectInputTrigger(TIM5_TS_TIM6);
  
  /* configurate TIM5 as slave of Trigger input*/
  TIM5_SelectSlaveMode(TIM5_SLAVEMODE_TRIGGER);
}

/**
  * @brief  TIM6 Configuration.
  * @param  None
  * @retval None
  */
static void TIM6_Config(void)
{
  /*DeInit TIM6 registers*/
  TIM6_DeInit();
  
  /* Time base configuration */  
  TIM6_TimeBaseInit(TIM6_PRESCALER_1, 0xFF );  
  
  /* Select TIM6 Update event as source of TIM6 Trigger Output*/
  TIM6_SelectOutputTrigger(TIM6_TRGOSOURCE_UPDATE);  
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
