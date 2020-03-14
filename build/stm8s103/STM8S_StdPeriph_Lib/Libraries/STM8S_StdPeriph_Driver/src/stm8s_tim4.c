/**
  ******************************************************************************
  * @file    stm8s_tim4.c
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   This file contains all the functions for the TIM4 peripheral.
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
#include "stm8s_tim4.h"

/** @addtogroup STM8S_StdPeriph_Driver
  * @{
  */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/**
  * @addtogroup TIM4_Public_Functions
  * @{
  */

/**
  * @brief  Deinitializes the TIM4 peripheral registers to their default reset values.
  * @param  None
  * @retval None
  */
void TIM4_DeInit(void)
{
  TIM4->CR1 = TIM4_CR1_RESET_VALUE;
  TIM4->IER = TIM4_IER_RESET_VALUE;
  TIM4->CNTR = TIM4_CNTR_RESET_VALUE;
  TIM4->PSCR = TIM4_PSCR_RESET_VALUE;
  TIM4->ARR = TIM4_ARR_RESET_VALUE;
  TIM4->SR1 = TIM4_SR1_RESET_VALUE;
}

/**
  * @brief  Initializes the TIM4 Time Base Unit according to the specified parameters.
  * @param    TIM4_Prescaler specifies the Prescaler from TIM4_Prescaler_TypeDef.
  * @param    TIM4_Period specifies the Period value.
  * @retval None
  */
void TIM4_TimeBaseInit(TIM4_Prescaler_TypeDef TIM4_Prescaler, uint8_t TIM4_Period)
{
  /* Check TIM4 prescaler value */
  assert_param(IS_TIM4_PRESCALER_OK(TIM4_Prescaler));
  /* Set the Prescaler value */
  TIM4->PSCR = (uint8_t)(TIM4_Prescaler);
  /* Set the Autoreload value */
  TIM4->ARR = (uint8_t)(TIM4_Period);
}

/**
  * @brief  Enables or disables the TIM4 peripheral.
  * @param   NewState new state of the TIM4 peripheral. This parameter can
  * be ENABLE or DISABLE.
  * @retval None
  */
void TIM4_Cmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
  
  /* set or Reset the CEN Bit */
  if (NewState != DISABLE)
  {
    TIM4->CR1 |= TIM4_CR1_CEN;
  }
  else
  {
    TIM4->CR1 &= (uint8_t)(~TIM4_CR1_CEN);
  }
}

/**
  * @brief  Enables or disables the specified TIM4 interrupts.
  * @param   NewState new state of the TIM4 peripheral.
  * This parameter can be: ENABLE or DISABLE.
  * @param   TIM4_IT specifies the TIM4 interrupts sources to be enabled or disabled.
  * This parameter can be any combination of the following values:
  * - TIM4_IT_UPDATE: TIM4 update Interrupt source
  * @param   NewState new state of the TIM4 peripheral.
  * @retval None
  */
void TIM4_ITConfig(TIM4_IT_TypeDef TIM4_IT, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_TIM4_IT_OK(TIM4_IT));
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
  
  if (NewState != DISABLE)
  {
    /* Enable the Interrupt sources */
    TIM4->IER |= (uint8_t)TIM4_IT;
  }
  else
  {
    /* Disable the Interrupt sources */
    TIM4->IER &= (uint8_t)(~TIM4_IT);
  }
}

/**
  * @brief  Enables or Disables the TIM4 Update event.
  * @param   NewState new state of the TIM4 peripheral Preload register. This parameter can
  * be ENABLE or DISABLE.
  * @retval None
  */
void TIM4_UpdateDisableConfig(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
  
  /* Set or Reset the UDIS Bit */
  if (NewState != DISABLE)
  {
    TIM4->CR1 |= TIM4_CR1_UDIS;
  }
  else
  {
    TIM4->CR1 &= (uint8_t)(~TIM4_CR1_UDIS);
  }
}

/**
  * @brief  Selects the TIM4 Update Request Interrupt source.
  * @param   TIM4_UpdateSource specifies the Update source.
  * This parameter can be one of the following values
  *                       - TIM4_UPDATESOURCE_REGULAR
  *                       - TIM4_UPDATESOURCE_GLOBAL
  * @retval None
  */
void TIM4_UpdateRequestConfig(TIM4_UpdateSource_TypeDef TIM4_UpdateSource)
{
  /* Check the parameters */
  assert_param(IS_TIM4_UPDATE_SOURCE_OK(TIM4_UpdateSource));
  
  /* Set or Reset the URS Bit */
  if (TIM4_UpdateSource != TIM4_UPDATESOURCE_GLOBAL)
  {
    TIM4->CR1 |= TIM4_CR1_URS;
  }
  else
  {
    TIM4->CR1 &= (uint8_t)(~TIM4_CR1_URS);
  }
}

/**
  * @brief  Selects the TIM4’s One Pulse Mode.
  * @param   TIM4_OPMode specifies the OPM Mode to be used.
  * This parameter can be one of the following values
  *                    - TIM4_OPMODE_SINGLE
  *                    - TIM4_OPMODE_REPETITIVE
  * @retval None
  */
void TIM4_SelectOnePulseMode(TIM4_OPMode_TypeDef TIM4_OPMode)
{
  /* Check the parameters */
  assert_param(IS_TIM4_OPM_MODE_OK(TIM4_OPMode));
  
  /* Set or Reset the OPM Bit */
  if (TIM4_OPMode != TIM4_OPMODE_REPETITIVE)
  {
    TIM4->CR1 |= TIM4_CR1_OPM;
  }
  else
  {
    TIM4->CR1 &= (uint8_t)(~TIM4_CR1_OPM);
  }
}

/**
  * @brief  Configures the TIM4 Prescaler.
  * @param   Prescaler specifies the Prescaler Register value
  * This parameter can be one of the following values
  *                       -  TIM4_PRESCALER_1
  *                       -  TIM4_PRESCALER_2
  *                       -  TIM4_PRESCALER_4
  *                       -  TIM4_PRESCALER_8
  *                       -  TIM4_PRESCALER_16
  *                       -  TIM4_PRESCALER_32
  *                       -  TIM4_PRESCALER_64
  *                       -  TIM4_PRESCALER_128
  * @param   TIM4_PSCReloadMode specifies the TIM4 Prescaler Reload mode.
  * This parameter can be one of the following values
  *                       - TIM4_PSCRELOADMODE_IMMEDIATE: The Prescaler is loaded
  *                         immediately.
  *                       - TIM4_PSCRELOADMODE_UPDATE: The Prescaler is loaded at
  *                         the update event.
  * @retval None
  */
void TIM4_PrescalerConfig(TIM4_Prescaler_TypeDef Prescaler, TIM4_PSCReloadMode_TypeDef TIM4_PSCReloadMode)
{
  /* Check the parameters */
  assert_param(IS_TIM4_PRESCALER_RELOAD_OK(TIM4_PSCReloadMode));
  assert_param(IS_TIM4_PRESCALER_OK(Prescaler));
  
  /* Set the Prescaler value */
  TIM4->PSCR = (uint8_t)Prescaler;
  
  /* Set or reset the UG Bit */
  TIM4->EGR = (uint8_t)TIM4_PSCReloadMode;
}

/**
  * @brief  Enables or disables TIM4 peripheral Preload register on ARR.
  * @param   NewState new state of the TIM4 peripheral Preload register.
  * This parameter can be ENABLE or DISABLE.
  * @retval None
  */
void TIM4_ARRPreloadConfig(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
  
  /* Set or Reset the ARPE Bit */
  if (NewState != DISABLE)
  {
    TIM4->CR1 |= TIM4_CR1_ARPE;
  }
  else
  {
    TIM4->CR1 &= (uint8_t)(~TIM4_CR1_ARPE);
  }
}

/**
  * @brief  Configures the TIM4 event to be generated by software.
  * @param   TIM4_EventSource specifies the event source.
  * This parameter can be one of the following values:
  *                       - TIM4_EVENTSOURCE_UPDATE: TIM4 update Event source
  * @retval None
  */
void TIM4_GenerateEvent(TIM4_EventSource_TypeDef TIM4_EventSource)
{
  /* Check the parameters */
  assert_param(IS_TIM4_EVENT_SOURCE_OK(TIM4_EventSource));
  
  /* Set the event sources */
  TIM4->EGR = (uint8_t)(TIM4_EventSource);
}

/**
  * @brief  Sets the TIM4 Counter Register value.
  * @param   Counter specifies the Counter register new value.
  * This parameter is between 0x00 and 0xFF.
  * @retval None
  */
void TIM4_SetCounter(uint8_t Counter)
{
  /* Set the Counter Register value */
  TIM4->CNTR = (uint8_t)(Counter);
}

/**
  * @brief  Sets the TIM4 Autoreload Register value.
  * @param   Autoreload specifies the Autoreload register new value.
  * This parameter is between 0x00 and 0xFF.
  * @retval None
  */
void TIM4_SetAutoreload(uint8_t Autoreload)
{
  /* Set the Autoreload Register value */
  TIM4->ARR = (uint8_t)(Autoreload);
}

/**
  * @brief  Gets the TIM4 Counter value.
  * @param  None
  * @retval Counter Register value.
  */
uint8_t TIM4_GetCounter(void)
{
  /* Get the Counter Register value */
  return (uint8_t)(TIM4->CNTR);
}

/**
  * @brief  Gets the TIM4 Prescaler value.
  * @param  None
  * @retval Prescaler Register configuration value.
  */
TIM4_Prescaler_TypeDef TIM4_GetPrescaler(void)
{
  /* Get the Prescaler Register value */
  return (TIM4_Prescaler_TypeDef)(TIM4->PSCR);
}

/**
  * @brief  Checks whether the specified TIM4 flag is set or not.
  * @param   TIM4_FLAG specifies the flag to check.
  * This parameter can be one of the following values:
  *                       - TIM4_FLAG_UPDATE: TIM4 update Flag
  * @retval FlagStatus The new state of TIM4_FLAG (SET or RESET).
  */
FlagStatus TIM4_GetFlagStatus(TIM4_FLAG_TypeDef TIM4_FLAG)
{
  FlagStatus bitstatus = RESET;
  
  /* Check the parameters */
  assert_param(IS_TIM4_GET_FLAG_OK(TIM4_FLAG));
  
  if ((TIM4->SR1 & (uint8_t)TIM4_FLAG)  != 0)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  return ((FlagStatus)bitstatus);
}

/**
  * @brief  Clears the TIM4’s pending flags.
  * @param   TIM4_FLAG specifies the flag to clear.
  * This parameter can be one of the following values:
  *                       - TIM4_FLAG_UPDATE: TIM4 update Flag
  * @retval None.
  */
void TIM4_ClearFlag(TIM4_FLAG_TypeDef TIM4_FLAG)
{
  /* Check the parameters */
  assert_param(IS_TIM4_GET_FLAG_OK(TIM4_FLAG));
  
  /* Clear the flags (rc_w0) clear this bit by writing 0. Writing ‘1’ has no effect*/
  TIM4->SR1 = (uint8_t)(~TIM4_FLAG);
}

/**
  * @brief  Checks whether the TIM4 interrupt has occurred or not.
  * @param  TIM4_IT specifies the TIM4 interrupt source to check.
  * This parameter can be one of the following values:
  *                       - TIM4_IT_UPDATE: TIM4 update Interrupt source
  * @retval ITStatus The new state of the TIM4_IT (SET or RESET).
  */
ITStatus TIM4_GetITStatus(TIM4_IT_TypeDef TIM4_IT)
{
  ITStatus bitstatus = RESET;
  
  uint8_t itstatus = 0x0, itenable = 0x0;
  
  /* Check the parameters */
  assert_param(IS_TIM4_IT_OK(TIM4_IT));
  
  itstatus = (uint8_t)(TIM4->SR1 & (uint8_t)TIM4_IT);
  
  itenable = (uint8_t)(TIM4->IER & (uint8_t)TIM4_IT);
  
  if ((itstatus != (uint8_t)RESET ) && (itenable != (uint8_t)RESET ))
  {
    bitstatus = (ITStatus)SET;
  }
  else
  {
    bitstatus = (ITStatus)RESET;
  }
  return ((ITStatus)bitstatus);
}

/**
  * @brief Clears the TIM4's interrupt pending bits.
  * @param TIM4_IT specifies the pending bit to clear.
  * This parameter can be one of the following values:
  *                       - TIM4_IT_UPDATE: TIM4 update Interrupt source
  * @retval None.
  */
void TIM4_ClearITPendingBit(TIM4_IT_TypeDef TIM4_IT)
{
  /* Check the parameters */
  assert_param(IS_TIM4_IT_OK(TIM4_IT));
  
  /* Clear the IT pending Bit */
  TIM4->SR1 = (uint8_t)(~TIM4_IT);
}

/**
  * @}
  */
  
  /**
  * @}
  */
  

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
