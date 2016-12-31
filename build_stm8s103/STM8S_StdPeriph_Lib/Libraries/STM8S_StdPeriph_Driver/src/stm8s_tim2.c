/**
  ******************************************************************************
  * @file    stm8s_tim2.c
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   This file contains all the functions for the TIM2 peripheral.
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
#include "stm8s_tim2.h"

/** @addtogroup STM8S_StdPeriph_Driver
  * @{
  */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void TI1_Config(uint8_t TIM2_ICPolarity, uint8_t TIM2_ICSelection, uint8_t TIM2_ICFilter);
static void TI2_Config(uint8_t TIM2_ICPolarity, uint8_t TIM2_ICSelection, uint8_t TIM2_ICFilter);
static void TI3_Config(uint8_t TIM2_ICPolarity, uint8_t TIM2_ICSelection, uint8_t TIM2_ICFilter);
/**
  * @addtogroup TIM2_Public_Functions
  * @{
  */

/**
  * @brief  Deinitializes the TIM2 peripheral registers to their default reset values.
  * @param  None
  * @retval None
  */
void TIM2_DeInit(void)
{
  TIM2->CR1 = (uint8_t)TIM2_CR1_RESET_VALUE;
  TIM2->IER = (uint8_t)TIM2_IER_RESET_VALUE;
  TIM2->SR2 = (uint8_t)TIM2_SR2_RESET_VALUE;
  
  /* Disable channels */
  TIM2->CCER1 = (uint8_t)TIM2_CCER1_RESET_VALUE;
  TIM2->CCER2 = (uint8_t)TIM2_CCER2_RESET_VALUE;
  
  
  /* Then reset channel registers: it also works if lock level is equal to 2 or 3 */
  TIM2->CCER1 = (uint8_t)TIM2_CCER1_RESET_VALUE;
  TIM2->CCER2 = (uint8_t)TIM2_CCER2_RESET_VALUE;
  TIM2->CCMR1 = (uint8_t)TIM2_CCMR1_RESET_VALUE;
  TIM2->CCMR2 = (uint8_t)TIM2_CCMR2_RESET_VALUE;
  TIM2->CCMR3 = (uint8_t)TIM2_CCMR3_RESET_VALUE;
  TIM2->CNTRH = (uint8_t)TIM2_CNTRH_RESET_VALUE;
  TIM2->CNTRL = (uint8_t)TIM2_CNTRL_RESET_VALUE;
  TIM2->PSCR = (uint8_t)TIM2_PSCR_RESET_VALUE;
  TIM2->ARRH  = (uint8_t)TIM2_ARRH_RESET_VALUE;
  TIM2->ARRL  = (uint8_t)TIM2_ARRL_RESET_VALUE;
  TIM2->CCR1H = (uint8_t)TIM2_CCR1H_RESET_VALUE;
  TIM2->CCR1L = (uint8_t)TIM2_CCR1L_RESET_VALUE;
  TIM2->CCR2H = (uint8_t)TIM2_CCR2H_RESET_VALUE;
  TIM2->CCR2L = (uint8_t)TIM2_CCR2L_RESET_VALUE;
  TIM2->CCR3H = (uint8_t)TIM2_CCR3H_RESET_VALUE;
  TIM2->CCR3L = (uint8_t)TIM2_CCR3L_RESET_VALUE;
  TIM2->SR1 = (uint8_t)TIM2_SR1_RESET_VALUE;
}

/**
  * @brief  Initializes the TIM2 Time Base Unit according to the specified parameters.
  * @param    TIM2_Prescaler specifies the Prescaler from TIM2_Prescaler_TypeDef.
  * @param    TIM2_Period specifies the Period value.
  * @retval None
  */
void TIM2_TimeBaseInit( TIM2_Prescaler_TypeDef TIM2_Prescaler,
                        uint16_t TIM2_Period)
{
  /* Set the Prescaler value */
  TIM2->PSCR = (uint8_t)(TIM2_Prescaler);
  /* Set the Autoreload value */
  TIM2->ARRH = (uint8_t)(TIM2_Period >> 8);
  TIM2->ARRL = (uint8_t)(TIM2_Period);
}


/**
  * @brief  Initializes the TIM2 Channel1 according to the specified parameters.
  * @param   TIM2_OCMode specifies the Output Compare mode  from @ref TIM2_OCMode_TypeDef.
  * @param   TIM2_OutputState specifies the Output State  from @ref TIM2_OutputState_TypeDef.
  * @param   TIM2_Pulse specifies the Pulse width  value.
  * @param   TIM2_OCPolarity specifies the Output Compare Polarity  from @ref TIM2_OCPolarity_TypeDef.
  * @retval None
  */
void TIM2_OC1Init(TIM2_OCMode_TypeDef TIM2_OCMode,
                  TIM2_OutputState_TypeDef TIM2_OutputState,
                  uint16_t TIM2_Pulse,
                  TIM2_OCPolarity_TypeDef TIM2_OCPolarity)
{
  /* Check the parameters */
  assert_param(IS_TIM2_OC_MODE_OK(TIM2_OCMode));
  assert_param(IS_TIM2_OUTPUT_STATE_OK(TIM2_OutputState));
  assert_param(IS_TIM2_OC_POLARITY_OK(TIM2_OCPolarity));
  
  /* Disable the Channel 1: Reset the CCE Bit, Set the Output State , the Output Polarity */
  TIM2->CCER1 &= (uint8_t)(~( TIM2_CCER1_CC1E | TIM2_CCER1_CC1P));
  /* Set the Output State &  Set the Output Polarity  */
  TIM2->CCER1 |= (uint8_t)((uint8_t)(TIM2_OutputState & TIM2_CCER1_CC1E ) | 
                           (uint8_t)(TIM2_OCPolarity & TIM2_CCER1_CC1P));
  
  /* Reset the Output Compare Bits  & Set the Ouput Compare Mode */
  TIM2->CCMR1 = (uint8_t)((uint8_t)(TIM2->CCMR1 & (uint8_t)(~TIM2_CCMR_OCM)) |
                          (uint8_t)TIM2_OCMode);
  
  /* Set the Pulse value */
  TIM2->CCR1H = (uint8_t)(TIM2_Pulse >> 8);
  TIM2->CCR1L = (uint8_t)(TIM2_Pulse);
}


/**
  * @brief  Initializes the TIM2 Channel2 according to the specified parameters.
  * @param   TIM2_OCMode specifies the Output Compare mode  from @ref TIM2_OCMode_TypeDef.
  * @param   TIM2_OutputState specifies the Output State  from @ref TIM2_OutputState_TypeDef.
  * @param   TIM2_Pulse specifies the Pulse width  value.
  * @param   TIM2_OCPolarity specifies the Output Compare Polarity  from @ref TIM2_OCPolarity_TypeDef.
  * @retval None
  */
void TIM2_OC2Init(TIM2_OCMode_TypeDef TIM2_OCMode,
                  TIM2_OutputState_TypeDef TIM2_OutputState,
                  uint16_t TIM2_Pulse,
                  TIM2_OCPolarity_TypeDef TIM2_OCPolarity)
{
  /* Check the parameters */
  assert_param(IS_TIM2_OC_MODE_OK(TIM2_OCMode));
  assert_param(IS_TIM2_OUTPUT_STATE_OK(TIM2_OutputState));
  assert_param(IS_TIM2_OC_POLARITY_OK(TIM2_OCPolarity));
  
  
  /* Disable the Channel 1: Reset the CCE Bit, Set the Output State, the Output Polarity */
  TIM2->CCER1 &= (uint8_t)(~( TIM2_CCER1_CC2E |  TIM2_CCER1_CC2P ));
  /* Set the Output State & Set the Output Polarity */
  TIM2->CCER1 |= (uint8_t)((uint8_t)(TIM2_OutputState  & TIM2_CCER1_CC2E ) |
                           (uint8_t)(TIM2_OCPolarity & TIM2_CCER1_CC2P));
  
  
  /* Reset the Output Compare Bits & Set the Output Compare Mode */
  TIM2->CCMR2 = (uint8_t)((uint8_t)(TIM2->CCMR2 & (uint8_t)(~TIM2_CCMR_OCM)) | 
                          (uint8_t)TIM2_OCMode);
  
  
  /* Set the Pulse value */
  TIM2->CCR2H = (uint8_t)(TIM2_Pulse >> 8);
  TIM2->CCR2L = (uint8_t)(TIM2_Pulse);
}


/**
  * @brief  Initializes the TIM2 Channel3 according to the specified parameters.
  * @param   TIM2_OCMode specifies the Output Compare mode from @ref TIM2_OCMode_TypeDef.
  * @param   TIM2_OutputState specifies the Output State from @ref TIM2_OutputState_TypeDef.
  * @param   TIM2_Pulse specifies the Pulse width value.
  * @param   TIM2_OCPolarity specifies the Output Compare Polarity  from @ref TIM2_OCPolarity_TypeDef.
  * @retval None
  */
void TIM2_OC3Init(TIM2_OCMode_TypeDef TIM2_OCMode,
                  TIM2_OutputState_TypeDef TIM2_OutputState,
                  uint16_t TIM2_Pulse,
                  TIM2_OCPolarity_TypeDef TIM2_OCPolarity)
{
  /* Check the parameters */
  assert_param(IS_TIM2_OC_MODE_OK(TIM2_OCMode));
  assert_param(IS_TIM2_OUTPUT_STATE_OK(TIM2_OutputState));
  assert_param(IS_TIM2_OC_POLARITY_OK(TIM2_OCPolarity));
  /* Disable the Channel 1: Reset the CCE Bit, Set the Output State, the Output Polarity */
  TIM2->CCER2 &= (uint8_t)(~( TIM2_CCER2_CC3E  | TIM2_CCER2_CC3P));
  /* Set the Output State & Set the Output Polarity */
  TIM2->CCER2 |= (uint8_t)((uint8_t)(TIM2_OutputState & TIM2_CCER2_CC3E) |  
                           (uint8_t)(TIM2_OCPolarity & TIM2_CCER2_CC3P));
  
  /* Reset the Output Compare Bits & Set the Output Compare Mode */
  TIM2->CCMR3 = (uint8_t)((uint8_t)(TIM2->CCMR3 & (uint8_t)(~TIM2_CCMR_OCM)) |
                          (uint8_t)TIM2_OCMode);
  
  /* Set the Pulse value */
  TIM2->CCR3H = (uint8_t)(TIM2_Pulse >> 8);
  TIM2->CCR3L = (uint8_t)(TIM2_Pulse);
}

/**
  * @brief  Initializes the TIM2 peripheral according to the specified parameters.
  * @param    TIM2_Channel specifies the Input Capture Channel from @ref TIM2_Channel_TypeDef.
  * @param   TIM2_ICPolarity specifies the Input Capture Polarity from @ref TIM2_ICPolarity_TypeDef.
  * @param   TIM2_ICSelection specifies the Input Capture Selection from @ref TIM2_ICSelection_TypeDef.
  * @param   TIM2_ICPrescaler specifies the Input Capture Prescaler from @ref TIM2_ICPSC_TypeDef.
  * @param   TIM2_ICFilter specifies the Input Capture Filter value (value can be an integer from 0x00 to 0x0F).
  * @retval None
  */
void TIM2_ICInit(TIM2_Channel_TypeDef TIM2_Channel,
                 TIM2_ICPolarity_TypeDef TIM2_ICPolarity,
                 TIM2_ICSelection_TypeDef TIM2_ICSelection,
                 TIM2_ICPSC_TypeDef TIM2_ICPrescaler,
                 uint8_t TIM2_ICFilter)
{
  /* Check the parameters */
  assert_param(IS_TIM2_CHANNEL_OK(TIM2_Channel));
  assert_param(IS_TIM2_IC_POLARITY_OK(TIM2_ICPolarity));
  assert_param(IS_TIM2_IC_SELECTION_OK(TIM2_ICSelection));
  assert_param(IS_TIM2_IC_PRESCALER_OK(TIM2_ICPrescaler));
  assert_param(IS_TIM2_IC_FILTER_OK(TIM2_ICFilter));
  
  if (TIM2_Channel == TIM2_CHANNEL_1)
  {
    /* TI1 Configuration */
    TI1_Config((uint8_t)TIM2_ICPolarity,
               (uint8_t)TIM2_ICSelection,
               (uint8_t)TIM2_ICFilter);
    
    /* Set the Input Capture Prescaler value */
    TIM2_SetIC1Prescaler(TIM2_ICPrescaler);
  }
  else if (TIM2_Channel == TIM2_CHANNEL_2)
  {
    /* TI2 Configuration */
    TI2_Config((uint8_t)TIM2_ICPolarity,
               (uint8_t)TIM2_ICSelection,
               (uint8_t)TIM2_ICFilter);
    
    /* Set the Input Capture Prescaler value */
    TIM2_SetIC2Prescaler(TIM2_ICPrescaler);
  }
  else
  {
    /* TI3 Configuration */
    TI3_Config((uint8_t)TIM2_ICPolarity,
               (uint8_t)TIM2_ICSelection,
               (uint8_t)TIM2_ICFilter);
    
    /* Set the Input Capture Prescaler value */
    TIM2_SetIC3Prescaler(TIM2_ICPrescaler);
  }
}

/**
  * @brief  Configures the TIM2 peripheral in PWM Input Mode according to the specified parameters.
    * @param    TIM2_Channel specifies the Input Capture Channel from @ref TIM2_Channel_TypeDef.
  * @param   TIM2_ICPolarity specifies the Input Capture Polarity from @ref TIM2_ICPolarity_TypeDef.
  * @param   TIM2_ICSelection specifies the Input Capture Selection from @ref TIM2_ICSelection_TypeDef.
  * @param   TIM2_ICPrescaler specifies the Input Capture Prescaler from @ref TIM2_ICPSC_TypeDef.
  * @param   TIM2_ICFilter specifies the Input Capture Filter value (value can be an integer from 0x00 to 0x0F).
  * @retval None
  */
void TIM2_PWMIConfig(TIM2_Channel_TypeDef TIM2_Channel,
                     TIM2_ICPolarity_TypeDef TIM2_ICPolarity,
                     TIM2_ICSelection_TypeDef TIM2_ICSelection,
                     TIM2_ICPSC_TypeDef TIM2_ICPrescaler,
                     uint8_t TIM2_ICFilter)
{
  uint8_t icpolarity = (uint8_t)TIM2_ICPOLARITY_RISING;
  uint8_t icselection = (uint8_t)TIM2_ICSELECTION_DIRECTTI;
  
  /* Check the parameters */
  assert_param(IS_TIM2_PWMI_CHANNEL_OK(TIM2_Channel));
  assert_param(IS_TIM2_IC_POLARITY_OK(TIM2_ICPolarity));
  assert_param(IS_TIM2_IC_SELECTION_OK(TIM2_ICSelection));
  assert_param(IS_TIM2_IC_PRESCALER_OK(TIM2_ICPrescaler));
  
  /* Select the Opposite Input Polarity */
  if (TIM2_ICPolarity != TIM2_ICPOLARITY_FALLING)
  {
    icpolarity = (uint8_t)TIM2_ICPOLARITY_FALLING;
  }
  else
  {
    icpolarity = (uint8_t)TIM2_ICPOLARITY_RISING;
  }
  
  /* Select the Opposite Input */
  if (TIM2_ICSelection == TIM2_ICSELECTION_DIRECTTI)
  {
    icselection = (uint8_t)TIM2_ICSELECTION_INDIRECTTI;
  }
  else
  {
    icselection = (uint8_t)TIM2_ICSELECTION_DIRECTTI;
  }
  
  if (TIM2_Channel == TIM2_CHANNEL_1)
  {
    /* TI1 Configuration */
    TI1_Config((uint8_t)TIM2_ICPolarity, (uint8_t)TIM2_ICSelection,
               (uint8_t)TIM2_ICFilter);
    
    /* Set the Input Capture Prescaler value */
    TIM2_SetIC1Prescaler(TIM2_ICPrescaler);
    
    /* TI2 Configuration */
    TI2_Config(icpolarity, icselection, TIM2_ICFilter);
    
    /* Set the Input Capture Prescaler value */
    TIM2_SetIC2Prescaler(TIM2_ICPrescaler);
  }
  else
  {
    /* TI2 Configuration */
    TI2_Config((uint8_t)TIM2_ICPolarity, (uint8_t)TIM2_ICSelection,
               (uint8_t)TIM2_ICFilter);
    
    /* Set the Input Capture Prescaler value */
    TIM2_SetIC2Prescaler(TIM2_ICPrescaler);
    
    /* TI1 Configuration */
    TI1_Config((uint8_t)icpolarity, icselection, (uint8_t)TIM2_ICFilter);
    
    /* Set the Input Capture Prescaler value */
    TIM2_SetIC1Prescaler(TIM2_ICPrescaler);
  }
}

/**
  * @brief  Enables or disables the TIM2 peripheral.
  * @param   NewState new state of the TIM2 peripheral. This parameter can
  * be ENABLE or DISABLE.
  * @retval None
  */
void TIM2_Cmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
  
  /* set or Reset the CEN Bit */
  if (NewState != DISABLE)
  {
    TIM2->CR1 |= (uint8_t)TIM2_CR1_CEN;
  }
  else
  {
    TIM2->CR1 &= (uint8_t)(~TIM2_CR1_CEN);
  }
}

/**
  * @brief  Enables or disables the specified TIM2 interrupts.
  * @param   NewState new state of the TIM2 peripheral.
  * This parameter can be: ENABLE or DISABLE.
  * @param   TIM2_IT specifies the TIM2 interrupts sources to be enabled or disabled.
  * This parameter can be any combination of the following values:
  *                       - TIM2_IT_UPDATE: TIM2 update Interrupt source
  *                       - TIM2_IT_CC1: TIM2 Capture Compare 1 Interrupt source
  *                       - TIM2_IT_CC2: TIM2 Capture Compare 2 Interrupt source
  *                       - TIM2_IT_CC3: TIM2 Capture Compare 3 Interrupt source
  * @param   NewState new state of the TIM2 peripheral.
  * @retval None
  */
void TIM2_ITConfig(TIM2_IT_TypeDef TIM2_IT, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_TIM2_IT_OK(TIM2_IT));
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
  
  if (NewState != DISABLE)
  {
    /* Enable the Interrupt sources */
    TIM2->IER |= (uint8_t)TIM2_IT;
  }
  else
  {
    /* Disable the Interrupt sources */
    TIM2->IER &= (uint8_t)(~TIM2_IT);
  }
}

/**
  * @brief  Enables or Disables the TIM2 Update event.
  * @param   NewState new state of the TIM2 peripheral Preload register. This parameter can
  * be ENABLE or DISABLE.
  * @retval None
  */
void TIM2_UpdateDisableConfig(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
  
  /* Set or Reset the UDIS Bit */
  if (NewState != DISABLE)
  {
    TIM2->CR1 |= (uint8_t)TIM2_CR1_UDIS;
  }
  else
  {
    TIM2->CR1 &= (uint8_t)(~TIM2_CR1_UDIS);
  }
}

/**
  * @brief  Selects the TIM2 Update Request Interrupt source.
  * @param   TIM2_UpdateSource specifies the Update source.
  * This parameter can be one of the following values
  *                       - TIM2_UPDATESOURCE_REGULAR
  *                       - TIM2_UPDATESOURCE_GLOBAL
  * @retval None
  */
void TIM2_UpdateRequestConfig(TIM2_UpdateSource_TypeDef TIM2_UpdateSource)
{
  /* Check the parameters */
  assert_param(IS_TIM2_UPDATE_SOURCE_OK(TIM2_UpdateSource));
  
  /* Set or Reset the URS Bit */
  if (TIM2_UpdateSource != TIM2_UPDATESOURCE_GLOBAL)
  {
    TIM2->CR1 |= (uint8_t)TIM2_CR1_URS;
  }
  else
  {
    TIM2->CR1 &= (uint8_t)(~TIM2_CR1_URS);
  }
}

/**
  * @brief  Selects the TIM2’s One Pulse Mode.
  * @param   TIM2_OPMode specifies the OPM Mode to be used.
  * This parameter can be one of the following values
  *                    - TIM2_OPMODE_SINGLE
  *                    - TIM2_OPMODE_REPETITIVE
  * @retval None
  */
void TIM2_SelectOnePulseMode(TIM2_OPMode_TypeDef TIM2_OPMode)
{
  /* Check the parameters */
  assert_param(IS_TIM2_OPM_MODE_OK(TIM2_OPMode));
  
  /* Set or Reset the OPM Bit */
  if (TIM2_OPMode != TIM2_OPMODE_REPETITIVE)
  {
    TIM2->CR1 |= (uint8_t)TIM2_CR1_OPM;
  }
  else
  {
    TIM2->CR1 &= (uint8_t)(~TIM2_CR1_OPM);
  }
}

/**
  * @brief  Configures the TIM2 Prescaler.
  * @param   Prescaler specifies the Prescaler Register value
  * This parameter can be one of the following values
  *                       -  TIM2_PRESCALER_1
  *                       -  TIM2_PRESCALER_2
  *                       -  TIM2_PRESCALER_4
  *                       -  TIM2_PRESCALER_8
  *                       -  TIM2_PRESCALER_16
  *                       -  TIM2_PRESCALER_32
  *                       -  TIM2_PRESCALER_64
  *                       -  TIM2_PRESCALER_128
  *                       -  TIM2_PRESCALER_256
  *                       -  TIM2_PRESCALER_512
  *                       -  TIM2_PRESCALER_1024
  *                       -  TIM2_PRESCALER_2048
  *                       -  TIM2_PRESCALER_4096
  *                       -  TIM2_PRESCALER_8192
  *                       -  TIM2_PRESCALER_16384
  *                       -  TIM2_PRESCALER_32768
  * @param   TIM2_PSCReloadMode specifies the TIM2 Prescaler Reload mode.
  * This parameter can be one of the following values
  *                       - TIM2_PSCRELOADMODE_IMMEDIATE: The Prescaler is loaded
  *                         immediately.
  *                       - TIM2_PSCRELOADMODE_UPDATE: The Prescaler is loaded at
  *                         the update event.
  * @retval None
  */
void TIM2_PrescalerConfig(TIM2_Prescaler_TypeDef Prescaler,
                          TIM2_PSCReloadMode_TypeDef TIM2_PSCReloadMode)
{
  /* Check the parameters */
  assert_param(IS_TIM2_PRESCALER_RELOAD_OK(TIM2_PSCReloadMode));
  assert_param(IS_TIM2_PRESCALER_OK(Prescaler));
  
  /* Set the Prescaler value */
  TIM2->PSCR = (uint8_t)Prescaler;
  
  /* Set or reset the UG Bit */
  TIM2->EGR = (uint8_t)TIM2_PSCReloadMode;
}

/**
  * @brief  Forces the TIM2 Channel1 output waveform to active or inactive level.
  * @param   TIM2_ForcedAction specifies the forced Action to be set to the output waveform.
  * This parameter can be one of the following values:
  *                       - TIM2_FORCEDACTION_ACTIVE: Force active level on OC1REF
  *                       - TIM2_FORCEDACTION_INACTIVE: Force inactive level on
  *                         OC1REF.
  * @retval None
  */
void TIM2_ForcedOC1Config(TIM2_ForcedAction_TypeDef TIM2_ForcedAction)
{
  /* Check the parameters */
  assert_param(IS_TIM2_FORCED_ACTION_OK(TIM2_ForcedAction));
  
  /* Reset the OCM Bits & Configure the Forced output Mode */
  TIM2->CCMR1  =  (uint8_t)((uint8_t)(TIM2->CCMR1 & (uint8_t)(~TIM2_CCMR_OCM))  
                            | (uint8_t)TIM2_ForcedAction);
}

/**
  * @brief  Forces the TIM2 Channel2 output waveform to active or inactive level.
  * @param   TIM2_ForcedAction specifies the forced Action to be set to the output waveform.
  * This parameter can be one of the following values:
  *                       - TIM2_FORCEDACTION_ACTIVE: Force active level on OC2REF
  *                       - TIM2_FORCEDACTION_INACTIVE: Force inactive level on
  *                         OC2REF.
  * @retval None
  */
void TIM2_ForcedOC2Config(TIM2_ForcedAction_TypeDef TIM2_ForcedAction)
{
  /* Check the parameters */
  assert_param(IS_TIM2_FORCED_ACTION_OK(TIM2_ForcedAction));
  
  /* Reset the OCM Bits & Configure the Forced output Mode */
  TIM2->CCMR2 = (uint8_t)((uint8_t)(TIM2->CCMR2 & (uint8_t)(~TIM2_CCMR_OCM))  
                          | (uint8_t)TIM2_ForcedAction);
}

/**
  * @brief  Forces the TIM2 Channel3 output waveform to active or inactive level.
  * @param   TIM2_ForcedAction specifies the forced Action to be set to the output waveform.
  * This parameter can be one of the following values:
  *                       - TIM2_FORCEDACTION_ACTIVE: Force active level on OC3REF
  *                       - TIM2_FORCEDACTION_INACTIVE: Force inactive level on
  *                         OC3REF.
  * @retval None
  */
void TIM2_ForcedOC3Config(TIM2_ForcedAction_TypeDef TIM2_ForcedAction)
{
  /* Check the parameters */
  assert_param(IS_TIM2_FORCED_ACTION_OK(TIM2_ForcedAction));
  
  /* Reset the OCM Bits & Configure the Forced output Mode */
  TIM2->CCMR3  =  (uint8_t)((uint8_t)(TIM2->CCMR3 & (uint8_t)(~TIM2_CCMR_OCM))
                            | (uint8_t)TIM2_ForcedAction);
}

/**
  * @brief  Enables or disables TIM2 peripheral Preload register on ARR.
  * @param   NewState new state of the TIM2 peripheral Preload register.
  * This parameter can be ENABLE or DISABLE.
  * @retval None
  */
void TIM2_ARRPreloadConfig(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
  
  /* Set or Reset the ARPE Bit */
  if (NewState != DISABLE)
  {
    TIM2->CR1 |= (uint8_t)TIM2_CR1_ARPE;
  }
  else
  {
    TIM2->CR1 &= (uint8_t)(~TIM2_CR1_ARPE);
  }
}

/**
  * @brief  Enables or disables the TIM2 peripheral Preload Register on CCR1.
  * @param   NewState new state of the Capture Compare Preload register.
  * This parameter can be ENABLE or DISABLE.
  * @retval None
  */
void TIM2_OC1PreloadConfig(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
  
  /* Set or Reset the OC1PE Bit */
  if (NewState != DISABLE)
  {
    TIM2->CCMR1 |= (uint8_t)TIM2_CCMR_OCxPE;
  }
  else
  {
    TIM2->CCMR1 &= (uint8_t)(~TIM2_CCMR_OCxPE);
  }
}

/**
  * @brief  Enables or disables the TIM2 peripheral Preload Register on CCR2.
  * @param   NewState new state of the Capture Compare Preload register.
  * This parameter can be ENABLE or DISABLE.
  * @retval None
  */
void TIM2_OC2PreloadConfig(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
  
  /* Set or Reset the OC2PE Bit */
  if (NewState != DISABLE)
  {
    TIM2->CCMR2 |= (uint8_t)TIM2_CCMR_OCxPE;
  }
  else
  {
    TIM2->CCMR2 &= (uint8_t)(~TIM2_CCMR_OCxPE);
  }
}

/**
  * @brief  Enables or disables the TIM2 peripheral Preload Register on CCR3.
  * @param   NewState new state of the Capture Compare Preload register.
  * This parameter can be ENABLE or DISABLE.
  * @retval None
  */
void TIM2_OC3PreloadConfig(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
  
  /* Set or Reset the OC3PE Bit */
  if (NewState != DISABLE)
  {
    TIM2->CCMR3 |= (uint8_t)TIM2_CCMR_OCxPE;
  }
  else
  {
    TIM2->CCMR3 &= (uint8_t)(~TIM2_CCMR_OCxPE);
  }
}

/**
  * @brief  Configures the TIM2 event to be generated by software.
  * @param   TIM2_EventSource specifies the event source.
  * This parameter can be one of the following values:
  *                       - TIM2_EVENTSOURCE_UPDATE: TIM2 update Event source
  *                       - TIM2_EVENTSOURCE_CC1: TIM2 Capture Compare 1 Event source
  *                       - TIM2_EVENTSOURCE_CC2: TIM2 Capture Compare 2 Event source
  *                       - TIM2_EVENTSOURCE_CC3: TIM2 Capture Compare 3 Event source
  * @retval None
  */
void TIM2_GenerateEvent(TIM2_EventSource_TypeDef TIM2_EventSource)
{
  /* Check the parameters */
  assert_param(IS_TIM2_EVENT_SOURCE_OK(TIM2_EventSource));
  
  /* Set the event sources */
  TIM2->EGR = (uint8_t)TIM2_EventSource;
}

/**
  * @brief  Configures the TIM2 Channel 1 polarity.
  * @param   TIM2_OCPolarity specifies the OC1 Polarity.
  * This parameter can be one of the following values:
  *                       - TIM2_OCPOLARITY_LOW: Output Compare active low
  *                       - TIM2_OCPOLARITY_HIGH: Output Compare active high
  * @retval None
  */
void TIM2_OC1PolarityConfig(TIM2_OCPolarity_TypeDef TIM2_OCPolarity)
{
  /* Check the parameters */
  assert_param(IS_TIM2_OC_POLARITY_OK(TIM2_OCPolarity));
  
  /* Set or Reset the CC1P Bit */
  if (TIM2_OCPolarity != TIM2_OCPOLARITY_HIGH)
  {
    TIM2->CCER1 |= (uint8_t)TIM2_CCER1_CC1P;
  }
  else
  {
    TIM2->CCER1 &= (uint8_t)(~TIM2_CCER1_CC1P);
  }
}

/**
  * @brief  Configures the TIM2 Channel 2 polarity.
  * @param   TIM2_OCPolarity specifies the OC2 Polarity.
  * This parameter can be one of the following values:
  *                       - TIM2_OCPOLARITY_LOW: Output Compare active low
  *                       - TIM2_OCPOLARITY_HIGH: Output Compare active high
  * @retval None
  */
void TIM2_OC2PolarityConfig(TIM2_OCPolarity_TypeDef TIM2_OCPolarity)
{
  /* Check the parameters */
  assert_param(IS_TIM2_OC_POLARITY_OK(TIM2_OCPolarity));
  
  /* Set or Reset the CC2P Bit */
  if (TIM2_OCPolarity != TIM2_OCPOLARITY_HIGH)
  {
    TIM2->CCER1 |= TIM2_CCER1_CC2P;
  }
  else
  {
    TIM2->CCER1 &= (uint8_t)(~TIM2_CCER1_CC2P);
  }
}

/**
  * @brief  Configures the TIM2 Channel 3 polarity.
  * @param   TIM2_OCPolarity specifies the OC3 Polarity.
  * This parameter can be one of the following values:
  *                       - TIM2_OCPOLARITY_LOW: Output Compare active low
  *                       - TIM2_OCPOLARITY_HIGH: Output Compare active high
  * @retval None
  */
void TIM2_OC3PolarityConfig(TIM2_OCPolarity_TypeDef TIM2_OCPolarity)
{
  /* Check the parameters */
  assert_param(IS_TIM2_OC_POLARITY_OK(TIM2_OCPolarity));
  
  /* Set or Reset the CC3P Bit */
  if (TIM2_OCPolarity != TIM2_OCPOLARITY_HIGH)
  {
    TIM2->CCER2 |= (uint8_t)TIM2_CCER2_CC3P;
  }
  else
  {
    TIM2->CCER2 &= (uint8_t)(~TIM2_CCER2_CC3P);
  }
}

/**
  * @brief  Enables or disables the TIM2 Capture Compare Channel x.
  * @param   TIM2_Channel specifies the TIM2 Channel.
  * This parameter can be one of the following values:
  *                       - TIM2_CHANNEL_1: TIM2 Channel1
  *                       - TIM2_CHANNEL_2: TIM2 Channel2
  *                       - TIM2_CHANNEL_3: TIM2 Channel3
  * @param   NewState specifies the TIM2 Channel CCxE bit new state.
  * This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void TIM2_CCxCmd(TIM2_Channel_TypeDef TIM2_Channel, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_TIM2_CHANNEL_OK(TIM2_Channel));
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
  
  if (TIM2_Channel == TIM2_CHANNEL_1)
  {
    /* Set or Reset the CC1E Bit */
    if (NewState != DISABLE)
    {
      TIM2->CCER1 |= (uint8_t)TIM2_CCER1_CC1E;
    }
    else
    {
      TIM2->CCER1 &= (uint8_t)(~TIM2_CCER1_CC1E);
    }
    
  }
  else if (TIM2_Channel == TIM2_CHANNEL_2)
  {
    /* Set or Reset the CC2E Bit */
    if (NewState != DISABLE)
    {
      TIM2->CCER1 |= (uint8_t)TIM2_CCER1_CC2E;
    }
    else
    {
      TIM2->CCER1 &= (uint8_t)(~TIM2_CCER1_CC2E);
    }
  }
  else
  {
    /* Set or Reset the CC3E Bit */
    if (NewState != DISABLE)
    {
      TIM2->CCER2 |= (uint8_t)TIM2_CCER2_CC3E;
    }
    else
    {
      TIM2->CCER2 &= (uint8_t)(~TIM2_CCER2_CC3E);
    }
  }
}

/**
  * @brief  Selects the TIM2 Output Compare Mode. This function disables the
  * selected channel before changing the Output Compare Mode. User has to
  * enable this channel using TIM2_CCxCmd and TIM2_CCxNCmd functions.
  * @param   TIM2_Channel specifies the TIM2 Channel.
  * This parameter can be one of the following values:
  *                       - TIM2_CHANNEL_1: TIM2 Channel1
  *                       - TIM2_CHANNEL_2: TIM2 Channel2
  *                       - TIM2_CHANNEL_3: TIM2 Channel3
  * @param   TIM2_OCMode specifies the TIM2 Output Compare Mode.
  * This parameter can be one of the following values:
  *                       - TIM2_OCMODE_TIMING
  *                       - TIM2_OCMODE_ACTIVE
  *                       - TIM2_OCMODE_TOGGLE
  *                       - TIM2_OCMODE_PWM1
  *                       - TIM2_OCMODE_PWM2
  *                       - TIM2_FORCEDACTION_ACTIVE
  *                       - TIM2_FORCEDACTION_INACTIVE
  * @retval None
  */
void TIM2_SelectOCxM(TIM2_Channel_TypeDef TIM2_Channel, TIM2_OCMode_TypeDef TIM2_OCMode)
{
  /* Check the parameters */
  assert_param(IS_TIM2_CHANNEL_OK(TIM2_Channel));
  assert_param(IS_TIM2_OCM_OK(TIM2_OCMode));
  
  if (TIM2_Channel == TIM2_CHANNEL_1)
  {
    /* Disable the Channel 1: Reset the CCE Bit */
    TIM2->CCER1 &= (uint8_t)(~TIM2_CCER1_CC1E);
    
    /* Reset the Output Compare Bits & Set the Output Compare Mode */
    TIM2->CCMR1 = (uint8_t)((uint8_t)(TIM2->CCMR1 & (uint8_t)(~TIM2_CCMR_OCM))
                            | (uint8_t)TIM2_OCMode);
  }
  else if (TIM2_Channel == TIM2_CHANNEL_2)
  {
    /* Disable the Channel 2: Reset the CCE Bit */
    TIM2->CCER1 &= (uint8_t)(~TIM2_CCER1_CC2E);
    
    /* Reset the Output Compare Bits & Set the Output Compare Mode */
    TIM2->CCMR2 = (uint8_t)((uint8_t)(TIM2->CCMR2 & (uint8_t)(~TIM2_CCMR_OCM))
                            | (uint8_t)TIM2_OCMode);
  }
  else
  {
    /* Disable the Channel 3: Reset the CCE Bit */
    TIM2->CCER2 &= (uint8_t)(~TIM2_CCER2_CC3E);
    
    /* Reset the Output Compare Bits & Set the Output Compare Mode */
    TIM2->CCMR3 = (uint8_t)((uint8_t)(TIM2->CCMR3 & (uint8_t)(~TIM2_CCMR_OCM))
                            | (uint8_t)TIM2_OCMode);
  }
}

/**
  * @brief  Sets the TIM2 Counter Register value.
  * @param   Counter specifies the Counter register new value.
  * This parameter is between 0x0000 and 0xFFFF.
  * @retval None
  */
void TIM2_SetCounter(uint16_t Counter)
{
  /* Set the Counter Register value */
  TIM2->CNTRH = (uint8_t)(Counter >> 8);
  TIM2->CNTRL = (uint8_t)(Counter);
}

/**
  * @brief  Sets the TIM2 Autoreload Register value.
  * @param   Autoreload specifies the Autoreload register new value.
  * This parameter is between 0x0000 and 0xFFFF.
  * @retval None
  */
void TIM2_SetAutoreload(uint16_t Autoreload)
{
  /* Set the Autoreload Register value */
  TIM2->ARRH = (uint8_t)(Autoreload >> 8);
  TIM2->ARRL = (uint8_t)(Autoreload);
}

/**
  * @brief  Sets the TIM2 Capture Compare1 Register value.
  * @param   Compare1 specifies the Capture Compare1 register new value.
  * This parameter is between 0x0000 and 0xFFFF.
  * @retval None
  */
void TIM2_SetCompare1(uint16_t Compare1)
{
  /* Set the Capture Compare1 Register value */
  TIM2->CCR1H = (uint8_t)(Compare1 >> 8);
  TIM2->CCR1L = (uint8_t)(Compare1);
}

/**
  * @brief  Sets the TIM2 Capture Compare2 Register value.
  * @param   Compare2 specifies the Capture Compare2 register new value.
  * This parameter is between 0x0000 and 0xFFFF.
  * @retval None
  */
void TIM2_SetCompare2(uint16_t Compare2)
{
  /* Set the Capture Compare2 Register value */
  TIM2->CCR2H = (uint8_t)(Compare2 >> 8);
  TIM2->CCR2L = (uint8_t)(Compare2);
}

/**
  * @brief  Sets the TIM2 Capture Compare3 Register value.
  * @param   Compare3 specifies the Capture Compare3 register new value.
  * This parameter is between 0x0000 and 0xFFFF.
  * @retval None
  */
void TIM2_SetCompare3(uint16_t Compare3)
{
  /* Set the Capture Compare3 Register value */
  TIM2->CCR3H = (uint8_t)(Compare3 >> 8);
  TIM2->CCR3L = (uint8_t)(Compare3);
}

/**
  * @brief  Sets the TIM2 Input Capture 1 Prescaler.
  * @param   TIM2_IC1Prescaler specifies the Input Capture prescaler new value
  * This parameter can be one of the following values:
  *                       - TIM2_ICPSC_DIV1: no prescaler
  *                       - TIM2_ICPSC_DIV2: capture is done once every 2 events
  *                       - TIM2_ICPSC_DIV4: capture is done once every 4 events
  *                       - TIM2_ICPSC_DIV8: capture is done once every 8 events
  * @retval None
  */
void TIM2_SetIC1Prescaler(TIM2_ICPSC_TypeDef TIM2_IC1Prescaler)
{
  /* Check the parameters */
  assert_param(IS_TIM2_IC_PRESCALER_OK(TIM2_IC1Prescaler));
  
  /* Reset the IC1PSC Bits &Set the IC1PSC value */
  TIM2->CCMR1 = (uint8_t)((uint8_t)(TIM2->CCMR1 & (uint8_t)(~TIM2_CCMR_ICxPSC))
                          | (uint8_t)TIM2_IC1Prescaler);
}

/**
  * @brief  Sets the TIM2 Input Capture 2 prescaler.
  * @param   TIM2_IC2Prescaler specifies the Input Capture prescaler new value
  * This parameter can be one of the following values:
  *                       - TIM2_ICPSC_DIV1: no prescaler
  *                       - TIM2_ICPSC_DIV2: capture is done once every 2 events
  *                       - TIM2_ICPSC_DIV4: capture is done once every 4 events
  *                       - TIM2_ICPSC_DIV8: capture is done once every 8 events
  * @retval None
  */
void TIM2_SetIC2Prescaler(TIM2_ICPSC_TypeDef TIM2_IC2Prescaler)
{
  /* Check the parameters */
  assert_param(IS_TIM2_IC_PRESCALER_OK(TIM2_IC2Prescaler));
  
  /* Reset the IC1PSC Bits &Set the IC1PSC value */
  TIM2->CCMR2 = (uint8_t)((uint8_t)(TIM2->CCMR2 & (uint8_t)(~TIM2_CCMR_ICxPSC))
                          | (uint8_t)TIM2_IC2Prescaler);
}

/**
  * @brief  Sets the TIM2 Input Capture 3 prescaler.
  * @param   TIM2_IC3Prescaler specifies the Input Capture prescaler new value
  * This parameter can be one of the following values:
  *                       - TIM2_ICPSC_DIV1: no prescaler
  *                       - TIM2_ICPSC_DIV2: capture is done once every 2 events
  *                       - TIM2_ICPSC_DIV4: capture is done once every 4 events
  *                       - TIM2_ICPSC_DIV8: capture is done once every 8 events
  * @retval None
  */
void TIM2_SetIC3Prescaler(TIM2_ICPSC_TypeDef TIM2_IC3Prescaler)
{
  
  /* Check the parameters */
  assert_param(IS_TIM2_IC_PRESCALER_OK(TIM2_IC3Prescaler));
  /* Reset the IC1PSC Bits &Set the IC1PSC value */
  TIM2->CCMR3 = (uint8_t)((uint8_t)(TIM2->CCMR3 & (uint8_t)(~TIM2_CCMR_ICxPSC))
                          | (uint8_t)TIM2_IC3Prescaler);
}

/**
  * @brief  Gets the TIM2 Input Capture 1 value.
  * @param  None
  * @retval Capture Compare 1 Register value.
  */
uint16_t TIM2_GetCapture1(void)
{
  /* Get the Capture 1 Register value */
  uint16_t tmpccr1 = 0;
  uint8_t tmpccr1l=0, tmpccr1h=0;
  
  tmpccr1h = TIM2->CCR1H;
  tmpccr1l = TIM2->CCR1L;
  
  tmpccr1 = (uint16_t)(tmpccr1l);
  tmpccr1 |= (uint16_t)((uint16_t)tmpccr1h << 8);
  /* Get the Capture 1 Register value */
  return (uint16_t)tmpccr1;
}

/**
  * @brief  Gets the TIM2 Input Capture 2 value.
  * @param  None
  * @retval Capture Compare 2 Register value.
  */
uint16_t TIM2_GetCapture2(void)
{
  /* Get the Capture 2 Register value */
  uint16_t tmpccr2 = 0;
  uint8_t tmpccr2l=0, tmpccr2h=0;
  
  tmpccr2h = TIM2->CCR2H;
  tmpccr2l = TIM2->CCR2L;
  
  tmpccr2 = (uint16_t)(tmpccr2l);
  tmpccr2 |= (uint16_t)((uint16_t)tmpccr2h << 8);
  /* Get the Capture 2 Register value */
  return (uint16_t)tmpccr2;
}

/**
  * @brief  Gets the TIM2 Input Capture 3 value.
  * @param  None
  * @retval Capture Compare 3 Register value.
  */
uint16_t TIM2_GetCapture3(void)
{
  /* Get the Capture 3 Register value */
  uint16_t tmpccr3 = 0;
  uint8_t tmpccr3l=0, tmpccr3h=0;
  
  tmpccr3h = TIM2->CCR3H;
  tmpccr3l = TIM2->CCR3L;
  
  tmpccr3 = (uint16_t)(tmpccr3l);
  tmpccr3 |= (uint16_t)((uint16_t)tmpccr3h << 8);
  /* Get the Capture 3 Register value */
  return (uint16_t)tmpccr3;
}

/**
  * @brief  Gets the TIM2 Counter value.
  * @param  None
  * @retval Counter Register value.
  */
uint16_t TIM2_GetCounter(void)
{
  uint16_t tmpcntr = 0;
  
  tmpcntr =  ((uint16_t)TIM2->CNTRH << 8);
  /* Get the Counter Register value */
  return (uint16_t)( tmpcntr| (uint16_t)(TIM2->CNTRL));
}

/**
  * @brief  Gets the TIM2 Prescaler value.
  * @param  None
  * @retval Prescaler Register configuration value  @ref TIM2_Prescaler_TypeDef.
  */
TIM2_Prescaler_TypeDef TIM2_GetPrescaler(void)
{
  /* Get the Prescaler Register value */
  return (TIM2_Prescaler_TypeDef)(TIM2->PSCR);
}

/**
  * @brief  Checks whether the specified TIM2 flag is set or not.
  * @param   TIM2_FLAG specifies the flag to check.
  * This parameter can be one of the following values:
  *                       - TIM2_FLAG_UPDATE: TIM2 update Flag
  *                       - TIM2_FLAG_CC1: TIM2 Capture Compare 1 Flag
  *                       - TIM2_FLAG_CC2: TIM2 Capture Compare 2 Flag
  *                       - TIM2_FLAG_CC3: TIM2 Capture Compare 3 Flag
  *                       - TIM2_FLAG_CC1OF: TIM2 Capture Compare 1 over capture Flag
  *                       - TIM2_FLAG_CC2OF: TIM2 Capture Compare 2 over capture Flag
  *                       - TIM2_FLAG_CC3OF: TIM2 Capture Compare 3 over capture Flag
  * @retval FlagStatus The new state of TIM2_FLAG (SET or RESET).
  */
FlagStatus TIM2_GetFlagStatus(TIM2_FLAG_TypeDef TIM2_FLAG)
{
  FlagStatus bitstatus = RESET;
  uint8_t tim2_flag_l = 0, tim2_flag_h = 0;
  
  /* Check the parameters */
  assert_param(IS_TIM2_GET_FLAG_OK(TIM2_FLAG));
  
  tim2_flag_l = (uint8_t)(TIM2->SR1 & (uint8_t)TIM2_FLAG);
  tim2_flag_h = (uint8_t)((uint16_t)TIM2_FLAG >> 8);
  
  if ((tim2_flag_l | (uint8_t)(TIM2->SR2 & tim2_flag_h)) != (uint8_t)RESET )
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  return (FlagStatus)bitstatus;
}

/**
  * @brief  Clears the TIM2’s pending flags.
  * @param   TIM2_FLAG specifies the flag to clear.
  * This parameter can be one of the following values:
  *                       - TIM2_FLAG_UPDATE: TIM2 update Flag
  *                       - TIM2_FLAG_CC1: TIM2 Capture Compare 1 Flag
  *                       - TIM2_FLAG_CC2: TIM2 Capture Compare 2 Flag
  *                       - TIM2_FLAG_CC3: TIM2 Capture Compare 3 Flag
  *                       - TIM2_FLAG_CC1OF: TIM2 Capture Compare 1 over capture Flag
  *                       - TIM2_FLAG_CC2OF: TIM2 Capture Compare 2 over capture Flag
  *                       - TIM2_FLAG_CC3OF: TIM2 Capture Compare 3 over capture Flag
  * @retval None.
  */
void TIM2_ClearFlag(TIM2_FLAG_TypeDef TIM2_FLAG)
{
  /* Check the parameters */
  assert_param(IS_TIM2_CLEAR_FLAG_OK(TIM2_FLAG));
  
  /* Clear the flags (rc_w0) clear this bit by writing 0. Writing ‘1’ has no effect*/
  TIM2->SR1 = (uint8_t)(~((uint8_t)(TIM2_FLAG)));
  TIM2->SR2 = (uint8_t)(~((uint8_t)((uint8_t)TIM2_FLAG >> 8)));
}

/**
  * @brief  Checks whether the TIM2 interrupt has occurred or not.
  * @param   TIM2_IT specifies the TIM2 interrupt source to check.
  * This parameter can be one of the following values:
  *                       - TIM2_IT_UPDATE: TIM2 update Interrupt source
  *                       - TIM2_IT_CC1: TIM2 Capture Compare 1 Interrupt source
  *                       - TIM2_IT_CC2: TIM2 Capture Compare 2 Interrupt source
  *                       - TIM2_IT_CC3: TIM2 Capture Compare 3 Interrupt source
  * @retval ITStatus The new state of the TIM2_IT(SET or RESET).
  */
ITStatus TIM2_GetITStatus(TIM2_IT_TypeDef TIM2_IT)
{
  ITStatus bitstatus = RESET;
  uint8_t TIM2_itStatus = 0, TIM2_itEnable = 0;
  
  /* Check the parameters */
  assert_param(IS_TIM2_GET_IT_OK(TIM2_IT));
  
  TIM2_itStatus = (uint8_t)(TIM2->SR1 & TIM2_IT);
  
  TIM2_itEnable = (uint8_t)(TIM2->IER & TIM2_IT);
  
  if ((TIM2_itStatus != (uint8_t)RESET ) && (TIM2_itEnable != (uint8_t)RESET ))
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  return (ITStatus)(bitstatus);
}

/**
  * @brief  Clears the TIM2's interrupt pending bits.
  * @param   TIM2_IT specifies the pending bit to clear.
  * This parameter can be one of the following values:
  *                       - TIM2_IT_UPDATE: TIM2 update Interrupt source
  *                       - TIM2_IT_CC1: TIM2 Capture Compare 1 Interrupt source
  *                       - TIM2_IT_CC2: TIM2 Capture Compare 2 Interrupt source
  *                       - TIM2_IT_CC3: TIM2 Capture Compare 3 Interrupt source
  * @retval None.
  */
void TIM2_ClearITPendingBit(TIM2_IT_TypeDef TIM2_IT)
{
  /* Check the parameters */
  assert_param(IS_TIM2_IT_OK(TIM2_IT));
  
  /* Clear the IT pending Bit */
  TIM2->SR1 = (uint8_t)(~TIM2_IT);
}

/**
  * @brief  Configure the TI1 as Input.
  * @param   TIM2_ICPolarity  The Input Polarity.
  * This parameter can be one of the following values:
  *                       - TIM2_ICPOLARITY_FALLING
  *                       - TIM2_ICPOLARITY_RISING
  * @param   TIM2_ICSelection specifies the input to be used.
  * This parameter can be one of the following values:
  *                       - TIM2_ICSELECTION_DIRECTTI: TIM2 Input 1 is selected to
  *                         be connected to IC1.
  *                       - TIM2_ICSELECTION_INDIRECTTI: TIM2 Input 1 is selected to
  *                         be connected to IC2.
  * @param   TIM2_ICFilter Specifies the Input Capture Filter.
  * This parameter must be a value between 0x00 and 0x0F.
  * @retval None
  */
static void TI1_Config(uint8_t TIM2_ICPolarity,
                       uint8_t TIM2_ICSelection,
                       uint8_t TIM2_ICFilter)
{
  /* Disable the Channel 1: Reset the CCE Bit */
  TIM2->CCER1 &= (uint8_t)(~TIM2_CCER1_CC1E);
  
  /* Select the Input and set the filter */
  TIM2->CCMR1  = (uint8_t)((uint8_t)(TIM2->CCMR1 & (uint8_t)(~(uint8_t)( TIM2_CCMR_CCxS | TIM2_CCMR_ICxF )))
                           | (uint8_t)(((TIM2_ICSelection)) | ((uint8_t)( TIM2_ICFilter << 4))));
  
  /* Select the Polarity */
  if (TIM2_ICPolarity != TIM2_ICPOLARITY_RISING)
  {
    TIM2->CCER1 |= TIM2_CCER1_CC1P;
  }
  else
  {
    TIM2->CCER1 &= (uint8_t)(~TIM2_CCER1_CC1P);
  }
  /* Set the CCE Bit */
  TIM2->CCER1 |= TIM2_CCER1_CC1E;
}

/**
  * @brief  Configure the TI2 as Input.
  * @param   TIM2_ICPolarity  The Input Polarity.
  * This parameter can be one of the following values:
  *                       - TIM2_ICPOLARITY_FALLING
  *                       - TIM2_ICPOLARITY_RISING
  * @param   TIM2_ICSelection specifies the input to be used.
  * This parameter can be one of the following values:
  *                       - TIM2_ICSELECTION_DIRECTTI: TIM2 Input 2 is selected to
  *                         be connected to IC2.
  *                       - TIM2_ICSELECTION_INDIRECTTI: TIM2 Input 2 is selected to
  *                         be connected to IC1.
  * @param   TIM2_ICFilter Specifies the Input Capture Filter.
  * This parameter must be a value between 0x00 and 0x0F.
  * @retval None
  */
static void TI2_Config(uint8_t TIM2_ICPolarity,
                       uint8_t TIM2_ICSelection,
                       uint8_t TIM2_ICFilter)
{
  /* Disable the Channel 2: Reset the CCE Bit */
  TIM2->CCER1 &= (uint8_t)(~TIM2_CCER1_CC2E);
  
  /* Select the Input and set the filter */
  TIM2->CCMR2 = (uint8_t)((uint8_t)(TIM2->CCMR2 & (uint8_t)(~(uint8_t)( TIM2_CCMR_CCxS | TIM2_CCMR_ICxF ))) 
                          | (uint8_t)(( (TIM2_ICSelection)) | ((uint8_t)( TIM2_ICFilter << 4))));
  
  
  /* Select the Polarity */
  if (TIM2_ICPolarity != TIM2_ICPOLARITY_RISING)
  {
    TIM2->CCER1 |= TIM2_CCER1_CC2P;
  }
  else
  {
    TIM2->CCER1 &= (uint8_t)(~TIM2_CCER1_CC2P);
  }
  
  /* Set the CCE Bit */
  TIM2->CCER1 |= TIM2_CCER1_CC2E;
}

/**
  * @brief  Configure the TI3 as Input.
  * @param   TIM2_ICPolarity  The Input Polarity.
  * This parameter can be one of the following values:
  *                       - TIM2_ICPOLARITY_FALLING
  *                       - TIM2_ICPOLARITY_RISING
  * @param   TIM2_ICSelection specifies the input to be used.
  * This parameter can be one of the following values:
  *                       - TIM2_ICSELECTION_DIRECTTI: TIM2 Input 3 is selected to
  *                         be connected to IC3.
  * @param   TIM2_ICFilter Specifies the Input Capture Filter.
  * This parameter must be a value between 0x00 and 0x0F.
  * @retval None
  */
static void TI3_Config(uint8_t TIM2_ICPolarity, uint8_t TIM2_ICSelection,
                       uint8_t TIM2_ICFilter)
{
  /* Disable the Channel 3: Reset the CCE Bit */
  TIM2->CCER2 &=  (uint8_t)(~TIM2_CCER2_CC3E);
  
  /* Select the Input and set the filter */
  TIM2->CCMR3 = (uint8_t)((uint8_t)(TIM2->CCMR3 & (uint8_t)(~( TIM2_CCMR_CCxS | TIM2_CCMR_ICxF))) 
                          | (uint8_t)(( (TIM2_ICSelection)) | ((uint8_t)( TIM2_ICFilter << 4))));
  
  
  /* Select the Polarity */
  if (TIM2_ICPolarity != TIM2_ICPOLARITY_RISING)
  {
    TIM2->CCER2 |= TIM2_CCER2_CC3P;
  }
  else
  {
    TIM2->CCER2 &= (uint8_t)(~TIM2_CCER2_CC3P);
  }
  /* Set the CCE Bit */
  TIM2->CCER2 |= TIM2_CCER2_CC3E;
}

/**
  * @}
  */
  
  /**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
