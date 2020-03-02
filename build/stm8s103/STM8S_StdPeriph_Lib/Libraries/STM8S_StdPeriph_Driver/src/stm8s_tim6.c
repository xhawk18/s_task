/**
  ******************************************************************************
  * @file    stm8s_tim6.c
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   This file contains all the functions for the TIM6 peripheral.
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
#include "stm8s_tim6.h"

/** @addtogroup STM8S_StdPeriph_Driver
  * @{
  */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/


/** @addtogroup STM8S_StdPeriph_Driver
  * @{
  */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/**
  * @addtogroup TIM6_Public_Functions
  * @{
  */

/**
  * @brief  Deinitializes the TIM6 peripheral registers to their default reset values.
  * @param  None
  * @retval None
  */
void TIM6_DeInit(void)
{
  TIM6->CR1 	= TIM6_CR1_RESET_VALUE;
  TIM6->CR2 	= TIM6_CR2_RESET_VALUE;
  TIM6->SMCR 	= TIM6_SMCR_RESET_VALUE;
  TIM6->IER 	= TIM6_IER_RESET_VALUE;
  TIM6->CNTR 	= TIM6_CNTR_RESET_VALUE;
  TIM6->PSCR	= TIM6_PSCR_RESET_VALUE;
  TIM6->ARR 	= TIM6_ARR_RESET_VALUE;
  TIM6->SR1 	= TIM6_SR1_RESET_VALUE;
}

/**
  * @brief  Initializes the TIM6 Time Base Unit according to the specified
  * parameters.
  * @param   TIM6_Prescaler : This parameter can be any of the @Ref TIM5_Prescaler_TypeDef enumeration.
  * @param   TIM6_Period : This parameter must be a value between 0x00 and 0xFF.
  * @retval None
  */
void TIM6_TimeBaseInit(TIM6_Prescaler_TypeDef TIM6_Prescaler,
                       uint8_t TIM6_Period)
{
  /* Check TIM6 prescaler value */
  assert_param(IS_TIM6_PRESCALER_OK(TIM6_Prescaler));
  /* Set the Autoreload value */
  TIM6->ARR = (uint8_t)(TIM6_Period);
  /* Set the Prescaler value */
  TIM6->PSCR = (uint8_t)(TIM6_Prescaler);
}

/**
  * @brief  Enables or disables the TIM6 peripheral.
  * @param   NewState : The new state of the TIM6 peripheral.
  * This parameter can be any of the @ref FunctionalState enumeration.
  * @retval None
  */
void TIM6_Cmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
  
  /* set or Reset the CEN Bit */
  if (NewState == ENABLE)
  {
    TIM6->CR1 |= TIM6_CR1_CEN ;
  }
  else
  {
    TIM6->CR1 &= (uint8_t)(~TIM6_CR1_CEN) ;
  }
}

/**
  * @brief  Enables or Disables the TIM6 Update event.
  * @param   NewState : The new state of the TIM6 peripheral Preload register.
  * This parameter can be any of the @ref FunctionalState enumeration.
  * @retval None
  */
void TIM6_UpdateDisableConfig(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
  
  /* Set or Reset the UDIS Bit */
  if (NewState == ENABLE)
  {
    TIM6->CR1 |= TIM6_CR1_UDIS ;
  }
  else
  {
    TIM6->CR1 &= (uint8_t)(~TIM6_CR1_UDIS) ;
  }
}

/**
  * @brief  Selects the TIM6 Update Request Interrupt source.
  * @param   TIM6_UpdateSource : Specifies the Update source.
  * This parameter can be one of the @ref TIM6_UpdateSource_TypeDef enumeration.
  * @retval None
  */
void TIM6_UpdateRequestConfig(TIM6_UpdateSource_TypeDef TIM6_UpdateSource)
{
  /* Check the parameters */
  assert_param(IS_TIM6_UPDATE_SOURCE_OK(TIM6_UpdateSource));
  
  /* Set or Reset the URS Bit */
  if (TIM6_UpdateSource == TIM6_UPDATESOURCE_REGULAR)
  {
    TIM6->CR1 |= TIM6_CR1_URS ;
  }
  else
  {
    TIM6->CR1 &= (uint8_t)(~TIM6_CR1_URS) ;
  }
}

/**
  * @brief  Selects the TIM6’s One Pulse Mode.
  * @param   TIM6_OPMode : Specifies the OPM Mode to be used.
  * This parameter can be one of the @ref TIM6_OPMode_TypeDef enumeration.
  * @retval None
  */
void TIM6_SelectOnePulseMode(TIM6_OPMode_TypeDef TIM6_OPMode)
{
  /* Check the parameters */
  assert_param(IS_TIM6_OPM_MODE_OK(TIM6_OPMode));
  
  /* Set or Reset the OPM Bit */
  if (TIM6_OPMode == TIM6_OPMODE_SINGLE)
  {
    TIM6->CR1 |= TIM6_CR1_OPM ;
  }
  else
  {
    TIM6->CR1 &= (uint8_t)(~TIM6_CR1_OPM) ;
  }
}

/**
  * @brief  Configures the TIM6 Prescaler.
  * @param   Prescaler : Specifies the Prescaler Register value
  * This parameter can be one of the @ref TIM6_Prescaler_TypeDef enumeration.
  * @param   TIM6_PSCReloadMode : Specifies the TIM6 Prescaler Reload mode.
  * This parameter can be one of the @ref TIM6_PSCReloadMode_TypeDef enumeration.
  * @retval None
  */
void TIM6_PrescalerConfig(TIM6_Prescaler_TypeDef Prescaler,
                          TIM6_PSCReloadMode_TypeDef TIM6_PSCReloadMode)
{
  /* Check the parameters */
  assert_param(IS_TIM6_PRESCALER_RELOAD_OK(TIM6_PSCReloadMode));
  assert_param(IS_TIM6_PRESCALER_OK(Prescaler));
  
  /* Set the Prescaler value */
  TIM6->PSCR = (uint8_t)Prescaler;
  
  /* Set or reset the UG Bit */
  if (TIM6_PSCReloadMode == TIM6_PSCRELOADMODE_IMMEDIATE)
  {
    TIM6->EGR |= TIM6_EGR_UG ;
  }
  else
  {
    TIM6->EGR &= (uint8_t)(~TIM6_EGR_UG) ;
  }
}

/**
  * @brief  Enables or disables TIM6 peripheral Preload register on ARR.
  * @param   NewState : The new state of the TIM6 peripheral Preload register.
  * This parameter can be any of the @ref FunctionalState enumeration.
  * @retval None
  */
void TIM6_ARRPreloadConfig(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
  
  /* Set or Reset the ARPE Bit */
  if (NewState == ENABLE)
  {
    TIM6->CR1 |= TIM6_CR1_ARPE ;
  }
  else
  {
    TIM6->CR1 &= (uint8_t)(~TIM6_CR1_ARPE) ;
  }
}

/**
  * @brief  Sets the TIM6 Counter Register value.
  * @param   Counter : Specifies the Counter register new value.
  * This parameter is between 0x00 and 0xFF.
  * @retval None
  */
void TIM6_SetCounter(uint8_t Counter)
{
  /* Set the Counter Register value */
  TIM6->CNTR = (uint8_t)(Counter);
}

/**
  * @brief  Sets the TIM6 Autoreload Register value.
  * @param   Autoreload : Specifies the Autoreload register new value.
  * This parameter is between 0x00 and 0xFF.
  * @retval None
  */
void TIM6_SetAutoreload(uint8_t Autoreload)
{
  /* Set the Autoreload Register value */
  TIM6->ARR = (uint8_t)(Autoreload);
}

/**
  * @brief  Gets the TIM6 Counter value.
  * @param  None
  * @retval uint8_t: Counter Register value.
  */
uint8_t TIM6_GetCounter(void)
{
  uint8_t tmpcntr=0;
  tmpcntr = TIM6->CNTR;
  /* Get the Counter Register value */
  return ((uint8_t)tmpcntr);
}

/**
  * @brief  Gets the TIM6 Prescaler value.
  * @param  None
  * @retval TIM6_Prescaler_TypeDef : Prescaler Register configuration value.
  */
TIM6_Prescaler_TypeDef TIM6_GetPrescaler(void)
{
  /* Get the Prescaler Register value */
  return ((TIM6_Prescaler_TypeDef)TIM6->PSCR);
}

/**
  * @brief  Enables or disables the specified TIM6 interrupts.
  * @param   TIM6_IT : Specifies the TIM6 interrupts sources to be enabled or disabled.
  * This parameter can be any combination of the @ref TIM6_IT_TypeDef enumeration.
  * @param   NewState : The new state of the TIM6 peripheral.
  * This parameter can be any of the @ref FunctionalState enumeration.
  * @retval None
  * @par Required preconditions:
  * If QST option bit is enabled, the TIM6 Interrupt vector will be mapped on IRQ number 2 (irq0).
  * Otherwise, it will be mapped on IRQ number 27 (irq25).
  */
void TIM6_ITConfig(TIM6_IT_TypeDef TIM6_IT, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_TIM6_IT_OK(TIM6_IT));
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
  
  if (NewState == ENABLE)
  {
    /* Enable the Interrupt sources */
    TIM6->IER |= (uint8_t)TIM6_IT;
  }
  else
  {
    /* Disable the Interrupt sources */
    TIM6->IER &= (uint8_t)(~(uint8_t)TIM6_IT);
  }
}

/**
  * @brief  Clears the TIM’s pending flags.
  * @param   TIM6_FLAG : Specifies the flag to clear.
  * This parameter can be one of the @ref TIM6_FLAG_TypeDef enumeration.
  * @retval None
  */
void TIM6_ClearFlag(TIM6_FLAG_TypeDef TIM6_FLAG)
{
  /* Check the parameters */
  assert_param(IS_TIM6_CLEAR_FLAG_OK((uint8_t)TIM6_FLAG));
  /* Clear the flags (rc_w0) clear this bit by writing 0. Writing ‘1’ has no effect*/
  TIM6->SR1 &= (uint8_t)(~((uint8_t)TIM6_FLAG));
}

/**
  * @brief  Checks whether the TIM6 interrupt has occurred or not.
  * @param   TIM6_IT : Specifies the TIM6 interrupt source to check.
  * This parameter can be one of the @ref TIM6_IT_TypeDef enumeration.
  * @retval ITStatus : The new state of the TIM6_IT.
  * This parameter can be any of the @ref ITStatus enumeration.
  */
ITStatus TIM6_GetITStatus(TIM6_IT_TypeDef TIM6_IT)
{
  ITStatus bitstatus = RESET;
  uint8_t itStatus = 0, itEnable = 0;
  
  /* Check the parameters */
  assert_param(IS_TIM6_GET_IT_OK(TIM6_IT));
  
  itStatus = (uint8_t)(TIM6->SR1 & (uint8_t)TIM6_IT);
  
  itEnable = (uint8_t)(TIM6->IER & (uint8_t)TIM6_IT);
  
  if ((itStatus != (uint8_t)RESET ) && (itEnable != (uint8_t)RESET ))
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
  * @brief  Configures the TIM6 event to be generated by software.
  * @param   TIM6_EventSource : Specifies the event source.
  * This parameter can be one of the @ref TIM6_EventSource_TypeDef enumeration.
  * @retval None
  */
void TIM6_GenerateEvent(TIM6_EventSource_TypeDef TIM6_EventSource)
{
  /* Check the parameters */
  assert_param(IS_TIM6_EVENT_SOURCE_OK((uint8_t)TIM6_EventSource));
  
  /* Set the event sources */
  TIM6->EGR |= (uint8_t)TIM6_EventSource;
}

/**
  * @brief  Checks whether the specified TIM6 flag is set or not.
  * @param   TIM6_FLAG : Specifies the flag to check.
  * This parameter can be one of the @ref TIM6_FLAG_TypeDef enumeration.
  * @retval FlagStatus : The new state of TIM6_FLAG.
  * This parameter can be any of the @ref FlagStatus enumeration.
  */
FlagStatus TIM6_GetFlagStatus(TIM6_FLAG_TypeDef TIM6_FLAG)
{
  volatile FlagStatus bitstatus = RESET;
  
  /* Check the parameters */
  assert_param(IS_TIM6_GET_FLAG_OK(TIM6_FLAG));
  
  if ((TIM6->SR1 & (uint8_t)TIM6_FLAG)  != 0)
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
  * @brief  Clears the TIM6's interrupt pending bits.
  * @param   TIM6_IT : Specifies the pending bit to clear.
  * This parameter can be one of the @ref TIM6_IT_TypeDef enumeration.
  * @retval None
  */
void TIM6_ClearITPendingBit(TIM6_IT_TypeDef TIM6_IT)
{
  /* Check the parameters */
  assert_param(IS_TIM6_IT_OK(TIM6_IT));
  
  /* Clear the IT pending Bit */
  TIM6->SR1 &= (uint8_t)(~(uint8_t)TIM6_IT);
}

/**
  * @brief  Selects the TIM6 Trigger Output Mode.
  * @param   TIM6_TRGOSource : Specifies the Trigger Output source.
  * This parameter can be one of the @ref TIM6_TRGOSource_TypeDef enumeration.
  * @retval None
  */
void TIM6_SelectOutputTrigger(TIM6_TRGOSource_TypeDef TIM6_TRGOSource)
{
  uint8_t tmpcr2 = 0;
  
  /* Check the parameters */
  assert_param(IS_TIM6_TRGO_SOURCE_OK(TIM6_TRGOSource));
  
  tmpcr2 = TIM6->CR2;
  
  /* Reset the MMS Bits */
  tmpcr2 &= (uint8_t)(~TIM6_CR2_MMS);
  
  /* Select the TRGO source */
  tmpcr2 |=  (uint8_t)TIM6_TRGOSource;
  
  TIM6->CR2 = tmpcr2;
}

/**
  * @brief  Sets or Resets the TIM6 Master/Slave Mode.
  * @param   NewState : The new state of the synchronization between TIM6 and its slaves (through TRGO).
  * This parameter can be any of the @ref FunctionalState enumeration.
  * @retval None
  */

void TIM6_SelectMasterSlaveMode(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
  
  /* Set or Reset the MSM Bit */
  if (NewState == ENABLE)
  {
    TIM6->SMCR |= TIM6_SMCR_MSM;
  }
  else
  {
    TIM6->SMCR &= (uint8_t)(~TIM6_SMCR_MSM);
  }
}

/**
  * @brief  Selects the TIM6 Input Trigger source.
  * @param   TIM6_InputTriggerSource : Specifies Input Trigger source.
  * This parameter can be one of the @ref TIM6_TS_TypeDef enumeration.
  * @retval None
  */
void TIM6_SelectInputTrigger(TIM6_TS_TypeDef TIM6_InputTriggerSource)
{
  uint8_t tmpsmcr = 0;
  
  /* Check the parameters */
  assert_param(IS_TIM6_TRIGGER_SELECTION_OK(TIM6_InputTriggerSource));
  
  tmpsmcr = TIM6->SMCR;
  
  /* Select the Trigger Source */
  tmpsmcr &= (uint8_t)(~TIM6_SMCR_TS);
  tmpsmcr |= (uint8_t)TIM6_InputTriggerSource;
  
  TIM6->SMCR = (uint8_t)tmpsmcr;
}

/**
  * @brief  Enables the TIM6 internal Clock.
  * @param  None
  * @retval None
  */
void TIM6_InternalClockConfig(void)
{
  /* Disable slave mode to clock the prescaler directly with the internal clock */
  TIM6->SMCR &=  (uint8_t)(~TIM6_SMCR_SMS);
}

/**
  * @brief  Selects the TIM6 Slave Mode.
  * @param   TIM6_SlaveMode : Specifies the TIM6 Slave Mode.
  * This parameter can be one of the @ref TIM6_SlaveMode_TypeDef enumeration.
  * @retval None
  */
void TIM6_SelectSlaveMode(TIM6_SlaveMode_TypeDef TIM6_SlaveMode)
{
  uint8_t tmpsmcr = 0;
  
  /* Check the parameters */
  assert_param(IS_TIM6_SLAVE_MODE_OK(TIM6_SlaveMode));
  
  tmpsmcr = TIM6->SMCR;
  
  /* Reset the SMS Bits */
  tmpsmcr &= (uint8_t)(~TIM6_SMCR_SMS);
  
  /* Select the Slave Mode */
  tmpsmcr |= (uint8_t)TIM6_SlaveMode;
  
  TIM6->SMCR = tmpsmcr;
}

/**
  * @}
  */
  
/**
  * @}
  */
  

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
