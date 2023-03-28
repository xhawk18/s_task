/**
  ******************************************************************************
  * @file    timing_delay.c
  * @author  MCD Application Team
  * @version V2.1.3
  * @date    28-June-2013
  * @brief   This file contains a set of functions needed to generate 1ms time 
  *          base delay using TIM2 update interrupt.
  *          TimingDelay_Init() function, should be called in the main.c file to
  *          ensure TIM2 initialization and configuration.
  *          The timing accuracy is based on the use of the external low speed
  *          clock source(LSE).
  *          Delay() function should be called in main.c file to specify the 
  *          duration of the desired delay in ms.   
  *          Counter decrementation is performed in TIM2 Update interrupt Handler
  *          by the mean of TimingDelay_Decrement() function. 
  *          This function should be called in the stm8l15x_it.c file in the 
  *          TIM2_UPD_OVF_TRG_BRK_USART2_TX_IRQHandler handler.
  *            
  *          @note TimingDelay_Init() function should be tailored in case user 
  *                wants to use a different clock source.
  *           
  *          For more details on timing_delay driver use, you can refer to 
  *          CLK_SYSCLKSwitch example in the STM8L15x_StdPeriph_Lib package.         
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
#include "timing_delay.h"

/** @addtogroup Utilities
  * @{
  */

/** @addtogroup Misc
  * @{
  */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define TIM2_PERIOD  (uint8_t) 7

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t TimingDelay;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @addtogroup TIMING_DELAY_Functions
  * @{
  */

/**
  * @brief  timing delay init:to generate 1 ms time base using TIM2 update interrupt
  * @note   The external low speed clock (LSE) is used to ensure timing accuracy.
  *         This function should be updated in case of use of other clock source.      
  * @param  None
  * @retval None
  */
void TimingDelay_Init(void)
{
  /* Enable TIM2 clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM2, ENABLE);

  /* Remap TIM2 ETR to LSE: TIM2 external trigger becomes controlled by LSE clock */
  SYSCFG_REMAPPinConfig(REMAP_Pin_TIM2TRIGLSE, ENABLE);

  /* Enable LSE clock */
  CLK_LSEConfig(CLK_LSE_ON);
  /* Wait for LSERDY flag to be reset */
  while (CLK_GetFlagStatus(CLK_FLAG_LSERDY) == RESET);

  /* TIM2 configuration:
     - TIM2 ETR is mapped to LSE
     - TIM2 counter is clocked by LSE div 4
      so the TIM2 counter clock used is LSE / 4 = 32.768 / 4 = 8.192 KHz
     TIM2 Channel1 output frequency = TIM2CLK / (TIM2 Prescaler * (TIM2_PERIOD + 1))
                                    = 8192 / (1 * 8) = 1024 Hz                */
                                    
  /* Time Base configuration */
  TIM2_TimeBaseInit(TIM2_Prescaler_1, TIM2_CounterMode_Up, TIM2_PERIOD);
  TIM2_ETRClockMode2Config(TIM2_ExtTRGPSC_DIV4, TIM2_ExtTRGPolarity_NonInverted, 0);

  TIM2_UpdateRequestConfig(TIM2_UpdateSource_Global);

  /* Clear TIM2 update flag */
  TIM2_ClearFlag(TIM2_FLAG_Update);

  /* Enable update interrupt */
  TIM2_ITConfig(TIM2_IT_Update, ENABLE);

  TIM2_Cmd(ENABLE);
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{
  TimingDelay = nTime;
  while (TimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @note   This function should be called in the
  *         TIM2_UPD_OVF_TRG_BRK_USART2_TX_IRQHandler in the stm8l15x_it.c file.
  *
  *       // INTERRUPT_HANDLER(TIM2_UPD_OVF_TRG_BRK_USART2_TX_IRQHandler, 19)
  *       // {
  *           // TimingDelay_Decrement(); 
  *           // TIM2_ClearITPendingBit(TIM2_IT_Update);
  *             
  *       // }  
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  {
    TimingDelay--;
  }
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
