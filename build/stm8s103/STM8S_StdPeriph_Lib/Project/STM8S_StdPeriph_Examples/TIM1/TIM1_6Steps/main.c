/**
  ******************************************************************************
  * @file    TIM1_6Steps\main.c
  * @author  MCD Application Team
  * @version  V2.2.0
  * @date     30-September-2014
  * @brief   This file contains the main function for TIM1 6 Steps example.
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
  * @addtogroup TIM1_6Steps
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void TIM1_Config(void);
static void TIM4_Config(void);
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{
  /* TIM1 configuration -----------------------------------------*/
    TIM1_Config();
    
  /* TIM4 configuration -----------------------------------------*/
    TIM4_Config();
   
  /* Main Output Enable */
  TIM1_CtrlPWMOutputs(ENABLE);
  
  enableInterrupts();
  
  while (1)
  {} 
}

/**
  * @brief  Configure TIM1 to generate 6 Steps PWM signal
  * @param  None
  * @retval None
  */
static void TIM1_Config(void)
{
  /* TIM1 Peripheral Configuration */ 
  TIM1_DeInit();

  /* Time Base configuration */
  TIM1_TimeBaseInit(0, TIM1_COUNTERMODE_UP, 4095, 0);

  /* Channel 1, 2 and 3 Configuration in TIMING mode */  
  
  /* TIM1_Pulse = 2047 */
  TIM1_OC1Init(TIM1_OCMODE_TIMING, TIM1_OUTPUTSTATE_ENABLE, TIM1_OUTPUTNSTATE_ENABLE,
               2047, TIM1_OCPOLARITY_HIGH, TIM1_OCNPOLARITY_HIGH, TIM1_OCIDLESTATE_SET,
               TIM1_OCNIDLESTATE_SET);  

  /* TIM1_Pulse = 1023 */
  TIM1_OC2Init(TIM1_OCMODE_TIMING, TIM1_OUTPUTSTATE_ENABLE, TIM1_OUTPUTNSTATE_ENABLE, 1023,
               TIM1_OCPOLARITY_HIGH, TIM1_OCNPOLARITY_HIGH, TIM1_OCIDLESTATE_SET, 
               TIM1_OCNIDLESTATE_SET); 

  /* TIM1_Pulse = 511 */
  TIM1_OC3Init(TIM1_OCMODE_TIMING, TIM1_OUTPUTSTATE_ENABLE, TIM1_OUTPUTNSTATE_ENABLE,
               511, TIM1_OCPOLARITY_HIGH, TIM1_OCNPOLARITY_HIGH, TIM1_OCIDLESTATE_SET,
               TIM1_OCNIDLESTATE_SET); 

  /* Automatic Output enable, Break, dead time and lock configuration*/
  TIM1_BDTRConfig( TIM1_OSSISTATE_ENABLE,  TIM1_LOCKLEVEL_OFF, 1,  TIM1_BREAK_DISABLE,
                   TIM1_BREAKPOLARITY_LOW,  TIM1_AUTOMATICOUTPUT_ENABLE);
  TIM1_CCPreloadControl(ENABLE);
  TIM1_ITConfig(TIM1_IT_COM, ENABLE);

  /* TIM1 counter enable */
  TIM1_Cmd(ENABLE);
}

/**
  * @brief  Configure TIM4 to generate a software COM event each 180 ms
  * @param  None
  * @retval None
  */
static void TIM4_Config(void)
{
  /* TIM4 Peripheral Configuration */ 
  /* Time Base configuration */
  TIM4_TimeBaseInit(TIM4_PRESCALER_128, 0xFF);
  
  /*TIM4 counter enable */
  TIM4_Cmd(ENABLE);
  
  TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);
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
