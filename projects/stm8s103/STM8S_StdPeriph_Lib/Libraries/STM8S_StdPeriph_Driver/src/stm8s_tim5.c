/**
  ******************************************************************************
  * @file    stm8s_tim5.c
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   This file contains all the functions for the TIM5 peripheral.
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
#include "stm8s_tim5.h"

/** @addtogroup STM8S_StdPeriph_Driver
  * @{
  */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void TI1_Config(uint8_t TIM5_ICPolarity, uint8_t TIM5_ICSelection, uint8_t TIM5_ICFilter);
static void TI2_Config(uint8_t TIM5_ICPolarity, uint8_t TIM5_ICSelection, uint8_t TIM5_ICFilter);
static void TI3_Config(uint8_t TIM5_ICPolarity, uint8_t TIM5_ICSelection, uint8_t TIM5_ICFilter);
/**
  * @addtogroup TIM5_Public_Functions
  * @{
  */

/**
  * @brief  Deinitializes the TIM5 peripheral registers to their default reset values.
  * @param  None
  * @retval None
  */
void TIM5_DeInit(void)
{
  TIM5->CR1 = (uint8_t)TIM5_CR1_RESET_VALUE;
  TIM5->CR2 = TIM5_CR2_RESET_VALUE;
  TIM5->SMCR = TIM5_SMCR_RESET_VALUE;
  TIM5->IER = (uint8_t)TIM5_IER_RESET_VALUE;
  TIM5->SR2 = (uint8_t)TIM5_SR2_RESET_VALUE;
  
  /* Disable channels */
  TIM5->CCER1 = (uint8_t)TIM5_CCER1_RESET_VALUE;
  TIM5->CCER2 = (uint8_t)TIM5_CCER2_RESET_VALUE;
  
  /* Then reset channel registers: it also works if lock level is equal to 2 or 3 */
  TIM5->CCER1 = (uint8_t)TIM5_CCER1_RESET_VALUE;
  TIM5->CCER2 = (uint8_t)TIM5_CCER2_RESET_VALUE;
  TIM5->CCMR1 = (uint8_t)TIM5_CCMR1_RESET_VALUE;
  TIM5->CCMR2 = (uint8_t)TIM5_CCMR2_RESET_VALUE;
  TIM5->CCMR3 = (uint8_t)TIM5_CCMR3_RESET_VALUE;
  TIM5->CNTRH = (uint8_t)TIM5_CNTRH_RESET_VALUE;
  TIM5->CNTRL = (uint8_t)TIM5_CNTRL_RESET_VALUE;
  TIM5->PSCR	= (uint8_t)TIM5_PSCR_RESET_VALUE;
  TIM5->ARRH 	= (uint8_t)TIM5_ARRH_RESET_VALUE;
  TIM5->ARRL 	= (uint8_t)TIM5_ARRL_RESET_VALUE;
  TIM5->CCR1H = (uint8_t)TIM5_CCR1H_RESET_VALUE;
  TIM5->CCR1L = (uint8_t)TIM5_CCR1L_RESET_VALUE;
  TIM5->CCR2H = (uint8_t)TIM5_CCR2H_RESET_VALUE;
  TIM5->CCR2L = (uint8_t)TIM5_CCR2L_RESET_VALUE;
  TIM5->CCR3H = (uint8_t)TIM5_CCR3H_RESET_VALUE;
  TIM5->CCR3L = (uint8_t)TIM5_CCR3L_RESET_VALUE;
  TIM5->SR1 = (uint8_t)TIM5_SR1_RESET_VALUE;
}

/**
  * @brief  Initializes the TIM5 Time Base Unit according to the specified parameters.
  * @param    TIM5_Prescaler specifies the Prescaler from TIM5_Prescaler_TypeDef.
  * @param    TIM5_Period specifies the Period value.
  * @retval None
  */
void TIM5_TimeBaseInit( TIM5_Prescaler_TypeDef TIM5_Prescaler,
                        uint16_t TIM5_Period)
{
  /* Set the Prescaler value */
  TIM5->PSCR = (uint8_t)(TIM5_Prescaler);
  /* Set the Autoreload value */
  TIM5->ARRH = (uint8_t)(TIM5_Period >> 8) ;
  TIM5->ARRL = (uint8_t)(TIM5_Period);
}

/**
  * @brief  Initializes the TIM5 Channel1 according to the specified parameters.
  * @param   TIM5_OCMode specifies the Output Compare mode  from @ref TIM5_OCMode_TypeDef.
  * @param   TIM5_OutputState specifies the Output State  from @ref TIM5_OutputState_TypeDef.
  * @param   TIM5_Pulse specifies the Pulse width  value.
  * @param   TIM5_OCPolarity specifies the Output Compare Polarity  from @ref TIM5_OCPolarity_TypeDef.
  * @retval None
  */
void TIM5_OC1Init(TIM5_OCMode_TypeDef TIM5_OCMode,
                  TIM5_OutputState_TypeDef TIM5_OutputState,
                  uint16_t TIM5_Pulse,
                  TIM5_OCPolarity_TypeDef TIM5_OCPolarity)
{
  /* Check the parameters */
  assert_param(IS_TIM5_OC_MODE_OK(TIM5_OCMode));
  assert_param(IS_TIM5_OUTPUT_STATE_OK(TIM5_OutputState));
  assert_param(IS_TIM5_OC_POLARITY_OK(TIM5_OCPolarity));
  
  /* Disable the Channel 1: Reset the CCE Bit, Set the Output State , the Output Polarity */
  TIM5->CCER1 &= (uint8_t)(~( TIM5_CCER1_CC1E | TIM5_CCER1_CC1P));
  /* Set the Output State &  Set the Output Polarity  */
  TIM5->CCER1 |= (uint8_t)((uint8_t)(TIM5_OutputState & TIM5_CCER1_CC1E )| 
                           (uint8_t)(TIM5_OCPolarity & TIM5_CCER1_CC1P));
  
  /* Reset the Output Compare Bits  & Set the Output Compare Mode */
  TIM5->CCMR1 = (uint8_t)((uint8_t)(TIM5->CCMR1 & (uint8_t)(~TIM5_CCMR_OCM)) | 
                          (uint8_t)TIM5_OCMode);
  
  /* Set the Pulse value */
  TIM5->CCR1H = (uint8_t)(TIM5_Pulse >> 8);
  TIM5->CCR1L = (uint8_t)(TIM5_Pulse);
}

/**
  * @brief  Initializes the TIM5 Channel2 according to the specified parameters.
  * @param   TIM5_OCMode specifies the Output Compare mode  from @ref TIM5_OCMode_TypeDef.
  * @param   TIM5_OutputState specifies the Output State  from @ref TIM5_OutputState_TypeDef.
  * @param   TIM5_Pulse specifies the Pulse width  value.
  * @param   TIM5_OCPolarity specifies the Output Compare Polarity  from @ref TIM5_OCPolarity_TypeDef.
  * @retval None
  */
void TIM5_OC2Init(TIM5_OCMode_TypeDef TIM5_OCMode,
                  TIM5_OutputState_TypeDef TIM5_OutputState,
                  uint16_t TIM5_Pulse,
                  TIM5_OCPolarity_TypeDef TIM5_OCPolarity)
{
  /* Check the parameters */
  assert_param(IS_TIM5_OC_MODE_OK(TIM5_OCMode));
  assert_param(IS_TIM5_OUTPUT_STATE_OK(TIM5_OutputState));
  assert_param(IS_TIM5_OC_POLARITY_OK(TIM5_OCPolarity));
  
  /* Disable the Channel 1: Reset the CCE Bit, Set the Output State , the Output Polarity */
  TIM5->CCER1 &= (uint8_t)(~( TIM5_CCER1_CC2E |  TIM5_CCER1_CC2P ));
  /* Set the Output State & Set the Output Polarity  */
  TIM5->CCER1 |= (uint8_t)((uint8_t)(TIM5_OutputState  & TIM5_CCER1_CC2E )| \
    (uint8_t)(TIM5_OCPolarity & TIM5_CCER1_CC2P));
  
  
  /* Reset the Output Compare Bits  & Set the Output Compare Mode */
  TIM5->CCMR2 = (uint8_t)((uint8_t)(TIM5->CCMR2 & (uint8_t)(~TIM5_CCMR_OCM)) |
                          (uint8_t)TIM5_OCMode);
  
  /* Set the Pulse value */
  TIM5->CCR2H = (uint8_t)(TIM5_Pulse >> 8);
  TIM5->CCR2L = (uint8_t)(TIM5_Pulse);
}

/**
  * @brief  Initializes the TIM5 Channel3 according to the specified parameters.
  * @param   TIM5_OCMode specifies the Output Compare mode from @ref TIM5_OCMode_TypeDef.
  * @param   TIM5_OutputState specifies the Output State from @ref TIM5_OutputState_TypeDef.
  * @param   TIM5_Pulse specifies the Pulse width value.
  * @param   TIM5_OCPolarity specifies the Output Compare Polarity  from @ref TIM5_OCPolarity_TypeDef.
  * @retval None
  */
void TIM5_OC3Init(TIM5_OCMode_TypeDef TIM5_OCMode,
                  TIM5_OutputState_TypeDef TIM5_OutputState,
                  uint16_t TIM5_Pulse,
                  TIM5_OCPolarity_TypeDef TIM5_OCPolarity)
{
  /* Check the parameters */
  assert_param(IS_TIM5_OC_MODE_OK(TIM5_OCMode));
  assert_param(IS_TIM5_OUTPUT_STATE_OK(TIM5_OutputState));
  assert_param(IS_TIM5_OC_POLARITY_OK(TIM5_OCPolarity));
  /* Disable the Channel 1: Reset the CCE Bit, Set the Output State, the Output Polarity */
  TIM5->CCER2 &= (uint8_t)(~( TIM5_CCER2_CC3E  | TIM5_CCER2_CC3P));
  /* Set the Output State & Set the Output Polarity  */
  TIM5->CCER2 |= (uint8_t)((uint8_t)(TIM5_OutputState  & TIM5_CCER2_CC3E   )|
                           (uint8_t)(TIM5_OCPolarity   & TIM5_CCER2_CC3P   ));
  
  /* Reset the Output Compare Bits  & Set the Output Compare Mode */
  TIM5->CCMR3 = (uint8_t)((uint8_t)(TIM5->CCMR3 & (uint8_t)(~TIM5_CCMR_OCM)) | (uint8_t)TIM5_OCMode);
  
  /* Set the Pulse value */
  TIM5->CCR3H = (uint8_t)(TIM5_Pulse >> 8);
  TIM5->CCR3L = (uint8_t)(TIM5_Pulse);
}

/**
  * @brief  Initializes the TIM5 peripheral according to the specified parameters.
  * @param    TIM5_Channel specifies the Input Capture Channel from @ref TIM5_Channel_TypeDef.
  * @param   TIM5_ICPolarity specifies the Input Capture Polarity from @ref TIM5_ICPolarity_TypeDef.
  * @param   TIM5_ICSelection specifies theInput Capture Selection from @ref TIM5_ICSelection_TypeDef.
  * @param   TIM5_ICPrescaler specifies the Input Capture Prescaler from @ref TIM5_ICPSC_TypeDef.
  * @param   TIM5_ICFilter specifies the Input Capture Filter value (value can be an integer from 0x00 to 0x0F).
  * @retval None
  */
void TIM5_ICInit(TIM5_Channel_TypeDef TIM5_Channel,
                 TIM5_ICPolarity_TypeDef TIM5_ICPolarity,
                 TIM5_ICSelection_TypeDef TIM5_ICSelection,
                 TIM5_ICPSC_TypeDef TIM5_ICPrescaler,
                 uint8_t TIM5_ICFilter)
{
  /* Check the parameters */
  assert_param(IS_TIM5_CHANNEL_OK(TIM5_Channel));
  assert_param(IS_TIM5_IC_POLARITY_OK(TIM5_ICPolarity));
  assert_param(IS_TIM5_IC_SELECTION_OK(TIM5_ICSelection));
  assert_param(IS_TIM5_IC_PRESCALER_OK(TIM5_ICPrescaler));
  assert_param(IS_TIM5_IC_FILTER_OK(TIM5_ICFilter));
  
  if (TIM5_Channel == TIM5_CHANNEL_1)
  {
    /* TI1 Configuration */
    TI1_Config((uint8_t)TIM5_ICPolarity,
               (uint8_t)TIM5_ICSelection,
               (uint8_t)TIM5_ICFilter);
    
    /* Set the Input Capture Prescaler value */
    TIM5_SetIC1Prescaler(TIM5_ICPrescaler);
  }
  else if (TIM5_Channel == TIM5_CHANNEL_2)
  {
    /* TI2 Configuration */
    TI2_Config((uint8_t)TIM5_ICPolarity,
               (uint8_t)TIM5_ICSelection,
               (uint8_t)TIM5_ICFilter);
    
    /* Set the Input Capture Prescaler value */
    TIM5_SetIC2Prescaler(TIM5_ICPrescaler);
  }
  else
  {
    /* TI3 Configuration */
    TI3_Config((uint8_t)TIM5_ICPolarity,
               (uint8_t)TIM5_ICSelection,
               (uint8_t)TIM5_ICFilter);
    
    /* Set the Input Capture Prescaler value */
    TIM5_SetIC3Prescaler(TIM5_ICPrescaler);
  }
}

/**
  * @brief  Configures the TIM5 peripheral in PWM Input Mode according to the specified parameters.
    * @param    TIM5_Channel specifies the Input Capture Channel from @ref TIM5_Channel_TypeDef.
  * @param   TIM5_ICPolarity specifies the Input Capture Polarity from @ref TIM5_ICPolarity_TypeDef.
  * @param   TIM5_ICSelection specifies theInput Capture Selection from @ref TIM5_ICSelection_TypeDef.
  * @param   TIM5_ICPrescaler specifies the Input Capture Prescaler from @ref TIM5_ICPSC_TypeDef.
  * @param   TIM5_ICFilter specifies the Input Capture Filter value (value can be an integer from 0x00 to 0x0F).
  * @retval None
  */
void TIM5_PWMIConfig(TIM5_Channel_TypeDef TIM5_Channel,
                     TIM5_ICPolarity_TypeDef TIM5_ICPolarity,
                     TIM5_ICSelection_TypeDef TIM5_ICSelection,
                     TIM5_ICPSC_TypeDef TIM5_ICPrescaler,
                     uint8_t TIM5_ICFilter)
{
  uint8_t icpolarity = (uint8_t)TIM5_ICPOLARITY_RISING;
  uint8_t icselection = (uint8_t)TIM5_ICSELECTION_DIRECTTI;
  
  /* Check the parameters */
  assert_param(IS_TIM5_PWMI_CHANNEL_OK(TIM5_Channel));
  assert_param(IS_TIM5_IC_POLARITY_OK(TIM5_ICPolarity));
  assert_param(IS_TIM5_IC_SELECTION_OK(TIM5_ICSelection));
  assert_param(IS_TIM5_IC_PRESCALER_OK(TIM5_ICPrescaler));
  
  /* Select the Opposite Input Polarity */
  if (TIM5_ICPolarity != TIM5_ICPOLARITY_FALLING)
  {
    icpolarity = (uint8_t)TIM5_ICPOLARITY_FALLING;
  }
  else
  {
    icpolarity = (uint8_t)TIM5_ICPOLARITY_RISING;
  }
  
  /* Select the Opposite Input */
  if (TIM5_ICSelection == TIM5_ICSELECTION_DIRECTTI)
  {
    icselection = (uint8_t)TIM5_ICSELECTION_INDIRECTTI;
  }
  else
  {
    icselection = (uint8_t)TIM5_ICSELECTION_DIRECTTI;
  }
  
  if (TIM5_Channel == TIM5_CHANNEL_1)
  {
    /* TI1 Configuration */
    TI1_Config((uint8_t)TIM5_ICPolarity, (uint8_t)TIM5_ICSelection,
               (uint8_t)TIM5_ICFilter);
    
    /* Set the Input Capture Prescaler value */
    TIM5_SetIC1Prescaler(TIM5_ICPrescaler);
    
    /* TI2 Configuration */
    TI2_Config((uint8_t)icpolarity, (uint8_t)icselection, (uint8_t)TIM5_ICFilter);
    
    /* Set the Input Capture Prescaler value */
    TIM5_SetIC2Prescaler(TIM5_ICPrescaler);
  }
  else
  {
    /* TI2 Configuration */
    TI2_Config((uint8_t)TIM5_ICPolarity, (uint8_t)TIM5_ICSelection,
               (uint8_t)TIM5_ICFilter);
    
    /* Set the Input Capture Prescaler value */
    TIM5_SetIC2Prescaler(TIM5_ICPrescaler);
    
    /* TI1 Configuration */
    TI1_Config((uint8_t)icpolarity, (uint8_t)icselection, (uint8_t)TIM5_ICFilter);
    
    /* Set the Input Capture Prescaler value */
    TIM5_SetIC1Prescaler(TIM5_ICPrescaler);
  }
}

/**
  * @brief  Enables or disables the TIM5 peripheral.
  * @param   NewState new state of the TIM5 peripheral.This parameter can
  * be ENABLE or DISABLE.
  * @retval None
  */
void TIM5_Cmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
  
  /* set or Reset the CEN Bit */
  if (NewState != DISABLE)
  {
    TIM5->CR1 |= TIM5_CR1_CEN ;
  }
  else
  {
    TIM5->CR1 &= (uint8_t)(~TIM5_CR1_CEN) ;
  }
}

/**
  * @brief  Enables or disables the specified TIM5 interrupts.
  * @param   NewState new state of the TIM5 peripheral.
  * This parameter can be: ENABLE or DISABLE.
  * @param   TIM5_IT specifies the TIM5 interrupts sources to be enabled or disabled.
  * This parameter can be any combination of the following values:
  *                       - TIM5_IT_UPDATE: TIM5 update Interrupt source
  *                       - TIM5_IT_CC1: TIM5 Capture Compare 1 Interrupt source
  *                       - TIM5_IT_CC2: TIM5 Capture Compare 2 Interrupt source
  *                       - TIM5_IT_CC3: TIM5 Capture Compare 3 Interrupt source
  * @param   NewState new state of the TIM5 peripheral.
  * @retval None
  */
void TIM5_ITConfig(TIM5_IT_TypeDef TIM5_IT, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_TIM5_IT_OK(TIM5_IT));
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
  
  if (NewState != DISABLE)
  {
    /* Enable the Interrupt sources */
    TIM5->IER |= (uint8_t)TIM5_IT;
  }
  else
  {
    /* Disable the Interrupt sources */
    TIM5->IER &= (uint8_t)(~TIM5_IT);
  }
}

/**
  * @brief  Enables or Disables the TIM5 Update event.
  * @param   NewState new state of the TIM5 peripheral Preload register.This parameter can
  * be ENABLE or DISABLE.
  * @retval None
  */
void TIM5_UpdateDisableConfig(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
  
  /* Set or Reset the UDIS Bit */
  if (NewState != DISABLE)
  {
    TIM5->CR1 |= TIM5_CR1_UDIS ;
  }
  else
  {
    TIM5->CR1 &= (uint8_t)(~TIM5_CR1_UDIS) ;
  }
}

/**
  * @brief  Selects the TIM5 Update Request Interrupt source.
  * @param   TIM5_UpdateSource specifies the Update source.
  * This parameter can be one of the following values
  *                       - TIM5_UPDATESOURCE_REGULAR
  *                       - TIM5_UPDATESOURCE_GLOBAL
  * @retval None
  */
void TIM5_UpdateRequestConfig(TIM5_UpdateSource_TypeDef TIM5_UpdateSource)
{
  /* Check the parameters */
  assert_param(IS_TIM5_UPDATE_SOURCE_OK(TIM5_UpdateSource));
  
  /* Set or Reset the URS Bit */
  if (TIM5_UpdateSource != TIM5_UPDATESOURCE_GLOBAL)
  {
    TIM5->CR1 |= TIM5_CR1_URS ;
  }
  else
  {
    TIM5->CR1 &= (uint8_t)(~TIM5_CR1_URS) ;
  }
}

/**
  * @brief  Selects the TIM5’s One Pulse Mode.
  * @param   TIM5_OPMode specifies the OPM Mode to be used.
  * This parameter can be one of the following values
  *                    - TIM5_OPMODE_SINGLE
  *                    - TIM5_OPMODE_REPETITIVE
  * @retval None
  */
void TIM5_SelectOnePulseMode(TIM5_OPMode_TypeDef TIM5_OPMode)
{
  /* Check the parameters */
  assert_param(IS_TIM5_OPM_MODE_OK(TIM5_OPMode));
  
  /* Set or Reset the OPM Bit */
  if (TIM5_OPMode != TIM5_OPMODE_REPETITIVE)
  {
    TIM5->CR1 |= TIM5_CR1_OPM ;
  }
  else
  {
    TIM5->CR1 &= (uint8_t)(~TIM5_CR1_OPM) ;
  }
}

/**
  * @brief  Configures the TIM5 Prescaler.
  * @param   Prescaler specifies the Prescaler Register value
  * This parameter can be one of the following values
  *                       -  TIM5_PRESCALER_1
  *                       -  TIM5_PRESCALER_2
  *                       -  TIM5_PRESCALER_4
  *                       -  TIM5_PRESCALER_8
  *                       -  TIM5_PRESCALER_16
  *                       -  TIM5_PRESCALER_32
  *                       -  TIM5_PRESCALER_64
  *                       -  TIM5_PRESCALER_128
  *                       -  TIM5_PRESCALER_256
  *                       -  TIM5_PRESCALER_512
  *                       -  TIM5_PRESCALER_1024
  *                       -  TIM5_PRESCALER_2048
  *                       -  TIM5_PRESCALER_4096
  *                       -  TIM5_PRESCALER_8192
  *                       -  TIM5_PRESCALER_16384
  *                       -  TIM5_PRESCALER_32768
  * @param   TIM5_PSCReloadMode specifies the TIM5 Prescaler Reload mode.
  * This parameter can be one of the following values
  *                       - TIM5_PSCRELOADMODE_IMMEDIATE: The Prescaler is loaded
  *                         immediately.
  *                       - TIM5_PSCRELOADMODE_UPDATE: The Prescaler is loaded at
  *                         the update event.
  * @retval None
  */
void TIM5_PrescalerConfig(TIM5_Prescaler_TypeDef Prescaler,
                          TIM5_PSCReloadMode_TypeDef TIM5_PSCReloadMode)
{
  /* Check the parameters */
  assert_param(IS_TIM5_PRESCALER_RELOAD_OK(TIM5_PSCReloadMode));
  assert_param(IS_TIM5_PRESCALER_OK(Prescaler));
  
  /* Set the Prescaler value */
  TIM5->PSCR = (uint8_t)Prescaler;
  
  /* Set or reset the UG Bit */
  TIM5->EGR = (uint8_t)TIM5_PSCReloadMode ;
}

/**
  * @brief  Forces the TIM5 Channel1 output waveform to active or inactive level.
  * @param   TIM5_ForcedAction specifies the forced Action to be set to the output waveform.
  * This parameter can be one of the following values:
  *                       - TIM5_FORCEDACTION_ACTIVE: Force active level on OC1REF
  *                       - TIM5_FORCEDACTION_INACTIVE: Force inactive level on
  *                         OC1REF.
  * @retval None
  */
void TIM5_ForcedOC1Config(TIM5_ForcedAction_TypeDef TIM5_ForcedAction)
{
  /* Check the parameters */
  assert_param(IS_TIM5_FORCED_ACTION_OK(TIM5_ForcedAction));
  
  /* Reset the OCM Bits */ /* Configure The Forced output Mode */
  TIM5->CCMR1  =  (uint8_t)((uint8_t)(TIM5->CCMR1 & (uint8_t)(~TIM5_CCMR_OCM))
                            | (uint8_t)TIM5_ForcedAction);
}

/**
  * @brief  Forces the TIM5 Channel2 output waveform to active or inactive level.
  * @param   TIM5_ForcedAction specifies the forced Action to be set to the output waveform.
  * This parameter can be one of the following values:
  *                       - TIM5_FORCEDACTION_ACTIVE: Force active level on OC2REF
  *                       - TIM5_FORCEDACTION_INACTIVE: Force inactive level on
  *                         OC2REF.
  * @retval None
  */
void TIM5_ForcedOC2Config(TIM5_ForcedAction_TypeDef TIM5_ForcedAction)
{
  /* Check the parameters */
  assert_param(IS_TIM5_FORCED_ACTION_OK(TIM5_ForcedAction));
  
  /* Reset the OCM Bits */ /* Configure The Forced output Mode */
  TIM5->CCMR2  =  (uint8_t)((uint8_t)(TIM5->CCMR2 & (uint8_t)(~TIM5_CCMR_OCM))
                            | (uint8_t)TIM5_ForcedAction);
}

/**
  * @brief  Forces the TIM5 Channel3 output waveform to active or inactive level.
  * @param   TIM5_ForcedAction specifies the forced Action to be set to the output waveform.
  * This parameter can be one of the following values:
  *                       - TIM5_FORCEDACTION_ACTIVE: Force active level on OC3REF
  *                       - TIM5_FORCEDACTION_INACTIVE: Force inactive level on
  *                         OC3REF.
  * @retval None
  */
void TIM5_ForcedOC3Config(TIM5_ForcedAction_TypeDef TIM5_ForcedAction)
{
  /* Check the parameters */
  assert_param(IS_TIM5_FORCED_ACTION_OK(TIM5_ForcedAction));
  
  /* Reset the OCM Bits */ /* Configure The Forced output Mode */
  TIM5->CCMR3  =  (uint8_t)((uint8_t)(TIM5->CCMR3 & (uint8_t)(~TIM5_CCMR_OCM))  
                            | (uint8_t)TIM5_ForcedAction);
}

/**
  * @brief  Enables or disables TIM5 peripheral Preload register on ARR.
  * @param   NewState new state of the TIM5 peripheral Preload register.
  * This parameter can be ENABLE or DISABLE.
  * @retval None
  */
void TIM5_ARRPreloadConfig(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
  
  /* Set or Reset the ARPE Bit */
  if (NewState != DISABLE)
  {
    TIM5->CR1 |= TIM5_CR1_ARPE ;
  }
  else
  {
    TIM5->CR1 &= (uint8_t)(~TIM5_CR1_ARPE) ;
  }
}

/**
  * @brief  Enables or disables the TIM5 peripheral Preload Register on CCR1.
  * @param   NewState new state of the Capture Compare Preload register.
  * This parameter can be ENABLE or DISABLE.
  * @retval None
  */
void TIM5_OC1PreloadConfig(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
  
  /* Set or Reset the OC1PE Bit */
  if (NewState != DISABLE)
  {
    TIM5->CCMR1 |= TIM5_CCMR_OCxPE ;
  }
  else
  {
    TIM5->CCMR1 &= (uint8_t)(~TIM5_CCMR_OCxPE) ;
  }
}

/**
  * @brief  Enables or disables the TIM5 peripheral Preload Register on CCR2.
  * @param   NewState new state of the Capture Compare Preload register.
  * This parameter can be ENABLE or DISABLE.
  * @retval None
  */
void TIM5_OC2PreloadConfig(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
  
  /* Set or Reset the OC2PE Bit */
  if (NewState != DISABLE)
  {
    TIM5->CCMR2 |= TIM5_CCMR_OCxPE ;
  }
  else
  {
    TIM5->CCMR2 &= (uint8_t)(~TIM5_CCMR_OCxPE) ;
  }
}

/**
  * @brief  Enables or disables the TIM5 peripheral Preload Register on CCR3.
  * @param   NewState new state of the Capture Compare Preload register.
  * This parameter can be ENABLE or DISABLE.
  * @retval None
  */
void TIM5_OC3PreloadConfig(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
  
  /* Set or Reset the OC3PE Bit */
  if (NewState != DISABLE)
  {
    TIM5->CCMR3 |= TIM5_CCMR_OCxPE ;
  }
  else
  {
    TIM5->CCMR3 &= (uint8_t)(~TIM5_CCMR_OCxPE) ;
  }
}

/**
  * @brief  Configures the TIM5 event to be generated by software.
  * @param   TIM5_EventSource specifies the event source.
  * This parameter can be one of the following values:
  *                       - TIM5_EVENTSOURCE_UPDATE: TIM5 update Event source
  *                       - TIM5_EVENTSOURCE_CC1: TIM5 Capture Compare 1 Event source
  *                       - TIM5_EVENTSOURCE_CC2: TIM5 Capture Compare 2 Event source
  *                       - TIM5_EVENTSOURCE_CC3: TIM5 Capture Compare 3 Event source
  * @retval None
  */
void TIM5_GenerateEvent(TIM5_EventSource_TypeDef TIM5_EventSource)
{
  /* Check the parameters */
  assert_param(IS_TIM5_EVENT_SOURCE_OK(TIM5_EventSource));
  
  /* Set the event sources */
  TIM5->EGR = (uint8_t)TIM5_EventSource;
}

/**
  * @brief  Configures the TIM5 Channel 1 polarity.
  * @param   TIM5_OCPolarity specifies the OC1 Polarity.
  * This parameter can be one of the following values:
  *                       - TIM5_OCPOLARITY_LOW: Output Compare active low
  *                       - TIM5_OCPOLARITY_HIGH: Output Compare active high
  * @retval None
  */
void TIM5_OC1PolarityConfig(TIM5_OCPolarity_TypeDef TIM5_OCPolarity)
{
    /* Check the parameters */
    assert_param(IS_TIM5_OC_POLARITY_OK(TIM5_OCPolarity));

    /* Set or Reset the CC1P Bit */
    if (TIM5_OCPolarity != TIM5_OCPOLARITY_HIGH)
    {
        TIM5->CCER1 |= TIM5_CCER1_CC1P ;
    }
    else
    {
        TIM5->CCER1 &= (uint8_t)(~TIM5_CCER1_CC1P) ;
    }
}


/**
  * @brief  Configures the TIM5 Channel 2 polarity.
  * @param   TIM5_OCPolarity specifies the OC2 Polarity.
  * This parameter can be one of the following values:
  *                       - TIM5_OCPOLARITY_LOW: Output Compare active low
  *                       - TIM5_OCPOLARITY_HIGH: Output Compare active high
  * @retval None
  */
void TIM5_OC2PolarityConfig(TIM5_OCPolarity_TypeDef TIM5_OCPolarity)
{
  /* Check the parameters */
  assert_param(IS_TIM5_OC_POLARITY_OK(TIM5_OCPolarity));
  
  /* Set or Reset the CC2P Bit */
  if (TIM5_OCPolarity != TIM5_OCPOLARITY_HIGH)
  {
    TIM5->CCER1 |= TIM5_CCER1_CC2P ;
  }
  else
  {
    TIM5->CCER1 &= (uint8_t)(~TIM5_CCER1_CC2P) ;
  }
}

/**
  * @brief  Configures the TIM5 Channel 3 polarity.
  * @param   TIM5_OCPolarity specifies the OC3 Polarity.
  * This parameter can be one of the following values:
  *                       - TIM5_OCPOLARITY_LOW: Output Compare active low
  *                       - TIM5_OCPOLARITY_HIGH: Output Compare active high
  * @retval None
  */
void TIM5_OC3PolarityConfig(TIM5_OCPolarity_TypeDef TIM5_OCPolarity)
{
  /* Check the parameters */
  assert_param(IS_TIM5_OC_POLARITY_OK(TIM5_OCPolarity));
  
  /* Set or Reset the CC3P Bit */
  if (TIM5_OCPolarity != TIM5_OCPOLARITY_HIGH)
  {
    TIM5->CCER2 |= TIM5_CCER2_CC3P ;
  }
  else
  {
    TIM5->CCER2 &= (uint8_t)(~TIM5_CCER2_CC3P) ;
  }
}

/**
  * @brief  Enables or disables the TIM5 Capture Compare Channel x.
  * @param   TIM5_Channel specifies the TIM5 Channel.
  * This parameter can be one of the following values:
  *                       - TIM5_Channel1: TIM5 Channel1
  *                       - TIM5_Channel2: TIM5 Channel2
  *                       - TIM5_Channel3: TIM5 Channel3
  * @param   NewState specifies the TIM5 Channel CCxE bit new state.
  * This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void TIM5_CCxCmd(TIM5_Channel_TypeDef TIM5_Channel, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_TIM5_CHANNEL_OK(TIM5_Channel));
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
  
  if (TIM5_Channel == TIM5_CHANNEL_1)
  {
    /* Set or Reset the CC1E Bit */
    if (NewState != DISABLE)
    {
      TIM5->CCER1 |= TIM5_CCER1_CC1E ;
    }
    else
    {
      TIM5->CCER1 &= (uint8_t)(~TIM5_CCER1_CC1E) ;
    }
    
  }
  else if (TIM5_Channel == TIM5_CHANNEL_2)
  {
    /* Set or Reset the CC2E Bit */
    if (NewState != DISABLE)
    {
      TIM5->CCER1 |= TIM5_CCER1_CC2E;
    }
    else
    {
      TIM5->CCER1 &= (uint8_t)(~TIM5_CCER1_CC2E) ;
    }
  }
  else
  {
    /* Set or Reset the CC3E Bit */
    if (NewState != DISABLE)
    {
      TIM5->CCER2 |= TIM5_CCER2_CC3E;
    }
    else
    {
      TIM5->CCER2 &= (uint8_t)(~TIM5_CCER2_CC3E) ;
    }
  }
}

/**
  * @brief  Selects the TIM5 Output Compare Mode. This function disables the
  * selected channel before changing the Output Compare Mode. User has to
  * enable this channel using TIM5_CCxCmd and TIM5_CCxNCmd functions.
  * @param   TIM5_Channel specifies the TIM5 Channel.
  * This parameter can be one of the following values:
  *                       - TIM5_Channel1: TIM5 Channel1
  *                       - TIM5_Channel2: TIM5 Channel2
  *                       - TIM5_Channel3: TIM5 Channel3
  * @param   TIM5_OCMode specifies the TIM5 Output Compare Mode.
  * This parameter can be one of the following values:
  *                       - TIM5_OCMODE_TIMING
  *                       - TIM5_OCMODE_ACTIVE
  *                       - TIM5_OCMODE_TOGGLE
  *                       - TIM5_OCMODE_PWM1
  *                       - TIM5_OCMODE_PWM2
  *                       - TIM5_FORCEDACTION_ACTIVE
  *                       - TIM5_FORCEDACTION_INACTIVE
  * @retval None
  */
void TIM5_SelectOCxM(TIM5_Channel_TypeDef TIM5_Channel, TIM5_OCMode_TypeDef TIM5_OCMode)
{
  /* Check the parameters */
  assert_param(IS_TIM5_CHANNEL_OK(TIM5_Channel));
  assert_param(IS_TIM5_OCM_OK(TIM5_OCMode));
  
  if (TIM5_Channel == TIM5_CHANNEL_1)
  {
    /* Disable the Channel 1: Reset the CCE Bit */
    TIM5->CCER1 &= (uint8_t)(~TIM5_CCER1_CC1E);
    
    /* Reset the Output Compare Bits  Set the Output Compare Mode */
    TIM5->CCMR1 = (uint8_t)((uint8_t)(TIM5->CCMR1 & (uint8_t)(~TIM5_CCMR_OCM)) 
                            | (uint8_t)TIM5_OCMode);
  }
  else if (TIM5_Channel == TIM5_CHANNEL_2)
  {
    /* Disable the Channel 2: Reset the CCE Bit */
    TIM5->CCER1 &= (uint8_t)(~TIM5_CCER1_CC2E);
    
    /* Reset the Output Compare Bits ** Set the Output Compare Mode */
    TIM5->CCMR2 = (uint8_t)((uint8_t)(TIM5->CCMR2 & (uint8_t)(~TIM5_CCMR_OCM))
                            | (uint8_t)TIM5_OCMode);
  }
  else
  {
    /* Disable the Channel 3: Reset the CCE Bit */
    TIM5->CCER2 &= (uint8_t)(~TIM5_CCER2_CC3E);
    
    /* Reset the Output Compare Bits ** Set the Output Compare Mode */
    TIM5->CCMR3 = (uint8_t)((uint8_t)(TIM5->CCMR3 & (uint8_t)(~TIM5_CCMR_OCM))
                            | (uint8_t)TIM5_OCMode);
  }
}

/**
  * @brief  Sets the TIM5 Counter Register value.
  * @param   Counter specifies the Counter register new value.
  * This parameter is between 0x0000 and 0xFFFF.
  * @retval None
  */
void TIM5_SetCounter(uint16_t Counter)
{
  /* Set the Counter Register value */
  TIM5->CNTRH = (uint8_t)(Counter >> 8);
  TIM5->CNTRL = (uint8_t)(Counter);
}

/**
  * @brief  Sets the TIM5 Autoreload Register value.
  * @param   Autoreload specifies the Autoreload register new value.
  * This parameter is between 0x0000 and 0xFFFF.
  * @retval None
  */
void TIM5_SetAutoreload(uint16_t Autoreload)
{
  /* Set the Autoreload Register value */
  TIM5->ARRH = (uint8_t)(Autoreload >> 8);
  TIM5->ARRL = (uint8_t)(Autoreload);
}

/**
  * @brief  Sets the TIM5 Capture Compare1 Register value.
  * @param   Compare1 specifies the Capture Compare1 register new value.
  * This parameter is between 0x0000 and 0xFFFF.
  * @retval None
  */
void TIM5_SetCompare1(uint16_t Compare1)
{
  /* Set the Capture Compare1 Register value */
  TIM5->CCR1H = (uint8_t)(Compare1 >> 8);
  TIM5->CCR1L = (uint8_t)(Compare1);
}

/**
  * @brief  Sets the TIM5 Capture Compare2 Register value.
  * @param   Compare2 specifies the Capture Compare2 register new value.
  * This parameter is between 0x0000 and 0xFFFF.
  * @retval None
  */
void TIM5_SetCompare2(uint16_t Compare2)
{
  /* Set the Capture Compare2 Register value */
  TIM5->CCR2H = (uint8_t)(Compare2 >> 8);
  TIM5->CCR2L = (uint8_t)(Compare2);
}

/**
  * @brief  Sets the TIM5 Capture Compare3 Register value.
  * @param   Compare3 specifies the Capture Compare3 register new value.
  * This parameter is between 0x0000 and 0xFFFF.
  * @retval None
  */
void TIM5_SetCompare3(uint16_t Compare3)
{
  /* Set the Capture Compare3 Register value */
  TIM5->CCR3H = (uint8_t)(Compare3 >> 8);
  TIM5->CCR3L = (uint8_t)(Compare3);
}

/**
  * @brief  Sets the TIM5 Input Capture 1 prescaler.
  * @param   TIM5_IC1Prescaler specifies the Input Capture prescaler new value
  * This parameter can be one of the following values:
  *                       - TIM5_ICPSC_DIV1: no prescaler
  *                       - TIM5_ICPSC_DIV2: capture is done once every 2 events
  *                       - TIM5_ICPSC_DIV4: capture is done once every 4 events
  *                       - TIM5_ICPSC_DIV8: capture is done once every 8 events
  * @retval None
  */
void TIM5_SetIC1Prescaler(TIM5_ICPSC_TypeDef TIM5_IC1Prescaler)
{
  /* Check the parameters */
  assert_param(IS_TIM5_IC_PRESCALER_OK(TIM5_IC1Prescaler));
  
  /* Reset the IC1PSC Bits */ /* Set the IC1PSC value */
  TIM5->CCMR1 = (uint8_t)((uint8_t)(TIM5->CCMR1 & (uint8_t)(~TIM5_CCMR_ICxPSC))|
                          (uint8_t)TIM5_IC1Prescaler);
}

/**
  * @brief  Sets the TIM5 Input Capture 2 prescaler.
  * @param   TIM5_IC2Prescaler specifies the Input Capture prescaler new value
  * This parameter can be one of the following values:
  *                       - TIM5_ICPSC_DIV1: no prescaler
  *                       - TIM5_ICPSC_DIV2: capture is done once every 2 events
  *                       - TIM5_ICPSC_DIV4: capture is done once every 4 events
  *                       - TIM5_ICPSC_DIV8: capture is done once every 8 events
  * @retval None
  */
void TIM5_SetIC2Prescaler(TIM5_ICPSC_TypeDef TIM5_IC2Prescaler)
{
  /* Check the parameters */
  assert_param(IS_TIM5_IC_PRESCALER_OK(TIM5_IC2Prescaler));
  
  /* Reset the IC1PSC Bits */ /* Set the IC1PSC value */
  TIM5->CCMR2 = (uint8_t)((uint8_t)(TIM5->CCMR2 & (uint8_t)(~TIM5_CCMR_ICxPSC))
                          | (uint8_t)TIM5_IC2Prescaler);
}

/**
  * @brief  Sets the TIM5 Input Capture 3 prescaler.
  * @param   TIM5_IC3Prescaler specifies the Input Capture prescaler new value
  * This parameter can be one of the following values:
  *                       - TIM5_ICPSC_DIV1: no prescaler
  *                       - TIM5_ICPSC_DIV2: capture is done once every 2 events
  *                       - TIM5_ICPSC_DIV4: capture is done once every 4 events
  *                       - TIM5_ICPSC_DIV8: capture is done once every 8 events
  * @retval None
  */
void TIM5_SetIC3Prescaler(TIM5_ICPSC_TypeDef TIM5_IC3Prescaler)
{
  /* Check the parameters */
  assert_param(IS_TIM5_IC_PRESCALER_OK(TIM5_IC3Prescaler));
  /* Reset the IC1PSC Bits */ /* Set the IC1PSC value */
  TIM5->CCMR3 = (uint8_t)((uint8_t)(TIM5->CCMR3 & (uint8_t)(~TIM5_CCMR_ICxPSC)) |
                          (uint8_t)TIM5_IC3Prescaler);
}

/**
  * @brief  Gets the TIM5 Input Capture 1 value.
  * @param  None
  * @retval Capture Compare 1 Register value.
  */
uint16_t TIM5_GetCapture1(void)
{
  uint16_t temp = 0; 
  
  temp = ((uint16_t)TIM5->CCR1H << 8); 
  
  /* Get the Capture 1 Register value */
  return (uint16_t)(temp | (uint16_t)(TIM5->CCR1L));
}

/**
  * @brief  Gets the TIM5 Input Capture 2 value.
  * @param  None
  * @retval Capture Compare 2 Register value.
  */
uint16_t TIM5_GetCapture2(void)
{
  uint16_t temp = 0; 
  
  temp = ((uint16_t)TIM5->CCR2H << 8);  
  
  /* Get the Capture 2 Register value */
  return (uint16_t)(temp | (uint16_t)(TIM5->CCR2L));
}

/**
  * @brief  Gets the TIM5 Input Capture 3 value.
  * @param  None
  * @retval Capture Compare 3 Register value.
  */
uint16_t TIM5_GetCapture3(void)
{
  uint16_t temp = 0; 
  
  temp = ((uint16_t)TIM5->CCR3H << 8);
  /* Get the Capture 1 Register value */
  return (uint16_t)(temp | (uint16_t)(TIM5->CCR3L));
}

/**
  * @brief  Gets the TIM5 Counter value.
  * @param  None
  * @retval Counter Register value.
  */
uint16_t TIM5_GetCounter(void)
{
  uint16_t tmpcntr = 0;
  
  tmpcntr = ((uint16_t)TIM5->CNTRH << 8); 
  /* Get the Counter Register value */
  return (uint16_t)(tmpcntr | (uint16_t)(TIM5->CNTRL));
}

/**
  * @brief  Gets the TIM5 Prescaler value.
  * @param  None
  * @retval Prescaler Register configuration value  @ref TIM5_Prescaler_TypeDef .
  */
TIM5_Prescaler_TypeDef TIM5_GetPrescaler(void)
{
  /* Get the Prescaler Register value */
  return (TIM5_Prescaler_TypeDef)(TIM5->PSCR);
}

/**
  * @brief  Checks whether the specified TIM5 flag is set or not.
  * @param   TIM5_FLAG specifies the flag to check.
  * This parameter can be one of the following values:
  *                       - TIM5_FLAG_UPDATE: TIM5 update Flag
  *                       - TIM5_FLAG_CC1: TIM5 Capture Compare 1 Flag
  *                       - TIM5_FLAG_CC2: TIM5 Capture Compare 2 Flag
  *                       - TIM5_FLAG_CC3: TIM5 Capture Compare 3 Flag
  *                       - TIM5_FLAG_CC1OF: TIM5 Capture Compare 1 overcapture Flag
  *                       - TIM5_FLAG_CC2OF: TIM5 Capture Compare 2 overcapture Flag
  *                       - TIM5_FLAG_CC3OF: TIM5 Capture Compare 3 overcapture Flag
  * @retval FlagStatus The new state of TIM5_FLAG (SET or RESET).
  */
FlagStatus TIM5_GetFlagStatus(TIM5_FLAG_TypeDef TIM5_FLAG)
{
  FlagStatus bitstatus = RESET;
  uint8_t tim5_flag_l, tim5_flag_h;
  
  /* Check the parameters */
  assert_param(IS_TIM5_GET_FLAG_OK(TIM5_FLAG));
  
  tim5_flag_l= (uint8_t)(TIM5->SR1 & (uint8_t)TIM5_FLAG);
  tim5_flag_h= (uint8_t)((uint16_t)TIM5_FLAG >> 8);
  
  if (((tim5_flag_l)|(uint8_t)(TIM5->SR2 & tim5_flag_h)) != RESET )
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
  * @brief  Clears the TIM5’s pending flags.
  * @param   TIM5_FLAG specifies the flag to clear.
  * This parameter can be one of the following values:
  *                       - TIM5_FLAG_UPDATE: TIM5 update Flag
  *                       - TIM5_FLAG_CC1: TIM5 Capture Compare 1 Flag
  *                       - TIM5_FLAG_CC2: TIM5 Capture Compare 2 Flag
  *                       - TIM5_FLAG_CC3: TIM5 Capture Compare 3 Flag
  *                       - TIM5_FLAG_CC1OF: TIM5 Capture Compare 1 overcapture Flag
  *                       - TIM5_FLAG_CC2OF: TIM5 Capture Compare 2 overcapture Flag
  *                       - TIM5_FLAG_CC3OF: TIM5 Capture Compare 3 overcapture Flag
  * @retval None.
  */
void TIM5_ClearFlag(TIM5_FLAG_TypeDef TIM5_FLAG)
{
  /* Check the parameters */
  assert_param(IS_TIM5_CLEAR_FLAG_OK(TIM5_FLAG));
  
  /* Clear the flags (rc_w0) clear this bit by writing 0. Writing ‘1’ has no effect*/
  TIM5->SR1 = (uint8_t)(~((uint8_t)(TIM5_FLAG)));
  TIM5->SR2 &= (uint8_t)(~((uint8_t)((uint16_t)TIM5_FLAG >> 8)));
}

/**
  * @brief  Checks whether the TIM5 interrupt has occurred or not.
  * @param   TIM5_IT specifies the TIM5 interrupt source to check.
  * This parameter can be one of the following values:
  *                       - TIM5_IT_UPDATE: TIM5 update Interrupt source
  *                       - TIM5_IT_CC1: TIM5 Capture Compare 1 Interrupt source
  *                       - TIM5_IT_CC2: TIM5 Capture Compare 2 Interrupt source
  *                       - TIM5_IT_CC3: TIM5 Capture Compare 3 Interrupt source
  * @retval ITStatus The new state of the TIM5_IT(SET or RESET).
  */

ITStatus TIM5_GetITStatus(TIM5_IT_TypeDef TIM5_IT)
{
  ITStatus bitstatus = RESET;
  uint8_t TIM5_itStatus = 0, TIM5_itEnable = 0;
  
  /* Check the parameters */
  assert_param(IS_TIM5_GET_IT_OK(TIM5_IT));
  
  TIM5_itStatus = (uint8_t)(TIM5->SR1 & TIM5_IT);
  
  TIM5_itEnable = (uint8_t)(TIM5->IER & TIM5_IT);
  
  if ((TIM5_itStatus != (uint8_t)RESET ) && (TIM5_itEnable != (uint8_t)RESET ))
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
  * @brief  Clears the TIM5's interrupt pending bits.
  * @param   TIM5_IT specifies the pending bit to clear.
  * This parameter can be one of the following values:
  *                       - TIM5_IT_UPDATE: TIM5 update Interrupt source
  *                       - TIM5_IT_CC1: TIM5 Capture Compare 1 Interrupt source
  *                       - TIM5_IT_CC2: TIM5 Capture Compare 2 Interrupt source
  *                       - TIM5_IT_CC3: TIM5 Capture Compare 3 Interrupt source
  * @retval None.
  */
void TIM5_ClearITPendingBit(TIM5_IT_TypeDef TIM5_IT)
{
  /* Check the parameters */
  assert_param(IS_TIM5_IT_OK(TIM5_IT));
  
  /* Clear the IT pending Bit */
  TIM5->SR1 = (uint8_t)(~TIM5_IT);
}

/**
  * @brief  Configure the TI1 as Input.
  * @param   TIM5_ICPolarity  The Input Polarity.
  * This parameter can be one of the following values:
  *                       - TIM5_ICPOLARITY_FALLING
  *                       - TIM5_ICPOLARITY_RISING
  * @param   TIM5_ICSelection specifies the input to be used.
  * This parameter can be one of the following values:
  *                       - TIM5_ICSELECTION_DIRECTTI: TIM5 Input 1 is selected to
  *                         be connected to IC1.
  *                       - TIM5_ICSELECTION_INDIRECTTI: TIM5 Input 1 is selected to
  *                         be connected to IC2.
  * @param   TIM5_ICFilter Specifies the Input Capture Filter.
  * This parameter must be a value between 0x00 and 0x0F.
  * @retval None
  */
static void TI1_Config(uint8_t TIM5_ICPolarity,
                       uint8_t TIM5_ICSelection,
                       uint8_t TIM5_ICFilter)
{
  /* Disable the Channel 1: Reset the CCE Bit */
  TIM5->CCER1 &= (uint8_t)(~TIM5_CCER1_CC1E);
  
  /* Select the Input and set the filter */
  TIM5->CCMR1  = (uint8_t)((uint8_t)(TIM5->CCMR1 & (uint8_t)(~( TIM5_CCMR_CCxS | TIM5_CCMR_ICxF )))
                           | (uint8_t)(( (TIM5_ICSelection)) | ((uint8_t)( TIM5_ICFilter << 4))));
  
  /* Select the Polarity */
  if (TIM5_ICPolarity != TIM5_ICPOLARITY_RISING)
  {
    TIM5->CCER1 |= TIM5_CCER1_CC1P ;
  }
  else
  {
    TIM5->CCER1 &= (uint8_t)(~TIM5_CCER1_CC1P) ;
  }
  /* Set the CCE Bit */
  TIM5->CCER1 |=  TIM5_CCER1_CC1E;
}

/**
  * @brief  Configure the TI2 as Input.
  * @param   TIM5_ICPolarity  The Input Polarity.
  * This parameter can be one of the following values:
  *                       - TIM5_ICPOLARITY_FALLING
  *                       - TIM5_ICPOLARITY_RISING
  * @param   TIM5_ICSelection specifies the input to be used.
  * This parameter can be one of the following values:
  *                       - TIM5_ICSELECTION_DIRECTTI: TIM5 Input 2 is selected to
  *                         be connected to IC2.
  *                       - TIM5_ICSELECTION_INDIRECTTI: TIM5 Input 2 is selected to
  *                         be connected to IC1.
  * @param   TIM5_ICFilter Specifies the Input Capture Filter.
  * This parameter must be a value between 0x00 and 0x0F.
  * @retval None
  */
static void TI2_Config(uint8_t TIM5_ICPolarity,
                       uint8_t TIM5_ICSelection,
                       uint8_t TIM5_ICFilter)
{
  /* Disable the Channel 2: Reset the CCE Bit */
  TIM5->CCER1 &=  (uint8_t)(~TIM5_CCER1_CC2E);
  
  /* Select the Input and set the filter */
  TIM5->CCMR2  = (uint8_t)((uint8_t)(TIM5->CCMR2 & (uint8_t)(~( TIM5_CCMR_CCxS | TIM5_CCMR_ICxF)))
                           | (uint8_t)(( (TIM5_ICSelection)) | ((uint8_t)( TIM5_ICFilter << 4))));
  
  
  /* Select the Polarity */
  if (TIM5_ICPolarity != TIM5_ICPOLARITY_RISING)
  {
    TIM5->CCER1 |= TIM5_CCER1_CC2P ;
  }
  else
  {
    TIM5->CCER1 &= (uint8_t)(~TIM5_CCER1_CC2P) ;
  }
  
  /* Set the CCE Bit */
  TIM5->CCER1 |=  TIM5_CCER1_CC2E;
}

/**
  * @brief  Configure the TI3 as Input.
  * @param   TIM5_ICPolarity  The Input Polarity.
  * This parameter can be one of the following values:
  *                       - TIM5_ICPOLARITY_FALLING
  *                       - TIM5_ICPOLARITY_RISING
  * @param   TIM5_ICSelection specifies the input to be used.
  * This parameter can be one of the following values:
  *                       - TIM5_ICSELECTION_DIRECTTI: TIM5 Input 3 is selected to
  *                         be connected to IC3.
  * @param   TIM5_ICFilter Specifies the Input Capture Filter.
  * This parameter must be a value between 0x00 and 0x0F.
  * @retval None
  */
static void TI3_Config(uint8_t TIM5_ICPolarity, uint8_t TIM5_ICSelection,
                       uint8_t TIM5_ICFilter)
{
  /* Disable the Channel 3: Reset the CCE Bit */
  TIM5->CCER2 &=  (uint8_t)(~TIM5_CCER2_CC3E);
  
  /* Select the Input and set the filter */
  TIM5->CCMR3 = (uint8_t)((uint8_t)(TIM5->CCMR3 & (uint8_t)(~( TIM5_CCMR_CCxS | TIM5_CCMR_ICxF))) 
                          | (uint8_t)(( (TIM5_ICSelection)) | ((uint8_t)( TIM5_ICFilter << 4))));
  
  
  /* Select the Polarity */
  if (TIM5_ICPolarity != TIM5_ICPOLARITY_RISING)
  {
    TIM5->CCER2 |= TIM5_CCER2_CC3P ;
  }
  else
  {
    TIM5->CCER2 &= (uint8_t)(~TIM5_CCER2_CC3P) ;
  }
  /* Set the CCE Bit */
  TIM5->CCER2 |=  TIM5_CCER2_CC3E;
}

/**
  * @brief  Enables the TIM5 internal Clock.
  * @par Parameters:
	* None
  * @retval None
  */
void TIM5_InternalClockConfig(void)
{
  /* Disable slave mode to clock the prescaler directly with the internal clock */
  TIM5->SMCR &=  (uint8_t)(~TIM5_SMCR_SMS);
}

/**
  * @brief  Selects the TIM5 Trigger Output Mode.
  * @param   TIM5_TRGOSource : Specifies the Trigger Output source.
  * This parameter can be one of the @ref TIM5_TRGOSource_TypeDef enumeration.
  * @retval None
  */
void TIM5_SelectOutputTrigger(TIM5_TRGOSource_TypeDef TIM5_TRGOSource)
{
  uint8_t tmpcr2 = 0;
  
  /* Check the parameters */
  assert_param(IS_TIM5_TRGO_SOURCE_OK(TIM5_TRGOSource));
  
  tmpcr2 = TIM5->CR2;
  
  /* Reset the MMS Bits */
  tmpcr2 &= (uint8_t)(~TIM5_CR2_MMS);
  
  /* Select the TRGO source */
  tmpcr2 |=  (uint8_t)TIM5_TRGOSource;
  
  TIM5->CR2 = tmpcr2;
}

/**
  * @brief  Selects the TIM5 Slave Mode.
  * @param   TIM5_SlaveMode : Specifies the TIM5 Slave Mode.
  * This parameter can be one of the @ref TIM5_SlaveMode_TypeDef enumeration.
  * @retval None
  */
void TIM5_SelectSlaveMode(TIM5_SlaveMode_TypeDef TIM5_SlaveMode)
{
  uint8_t tmpsmcr = 0;
  
  /* Check the parameters */
  assert_param(IS_TIM5_SLAVE_MODE_OK(TIM5_SlaveMode));
  
  tmpsmcr = TIM5->SMCR;
  
  /* Reset the SMS Bits */
  tmpsmcr &= (uint8_t)(~TIM5_SMCR_SMS);
  
  /* Select the Slave Mode */
  tmpsmcr |= (uint8_t)TIM5_SlaveMode;
  
  TIM5->SMCR = tmpsmcr;
}

/**
  * @brief  Selects the TIM5 Input Trigger source.
  * @param   TIM5_InputTriggerSource : Specifies Input Trigger source.
  * This parameter can be one of the @ref TIM5_TS_TypeDef enumeration.
  * @retval None
  */
void TIM5_SelectInputTrigger(TIM5_TS_TypeDef TIM5_InputTriggerSource)
{
  uint8_t tmpsmcr = 0;
  
  /* Check the parameters */
  assert_param(IS_TIM5_TRIGGER_SELECTION_OK(TIM5_InputTriggerSource));
  
  tmpsmcr = TIM5->SMCR;
  
  /* Select the Trigger Source */
  tmpsmcr &= (uint8_t)(~TIM5_SMCR_TS);
  tmpsmcr |= (uint8_t)TIM5_InputTriggerSource;
  
  TIM5->SMCR = (uint8_t)tmpsmcr;
}

/**
  * @brief  Configures the TIM5 Encoder Interface.
  * @param   TIM5_EncoderMode : Specifies the TIM5 Encoder Mode.
  * This parameter can be one of the @ref TIM5_EncoderMode_TypeDef enumeration.
  * @param   TIM5_IC1Polarity : Specifies the IC1 Polarity.
  * This parameter can be one of the @ref TIM5_ICPolarity_TypeDef enumeration.
  * @param   TIM5_IC2Polarity : Specifies the IC2 Polarity.
  * This parameter can be one of the @ref TIM5_ICPolarity_TypeDef enumeration.
  * @retval None
  */
void TIM5_EncoderInterfaceConfig(TIM5_EncoderMode_TypeDef TIM5_EncoderMode,
                                 TIM5_ICPolarity_TypeDef TIM5_IC1Polarity,
                                 TIM5_ICPolarity_TypeDef TIM5_IC2Polarity)
{
  uint8_t tmpsmcr = 0;
  uint8_t tmpccmr1 = 0;
  uint8_t tmpccmr2 = 0;
  
  /* Check the parameters */
  assert_param(IS_TIM5_ENCODER_MODE_OK(TIM5_EncoderMode));
  assert_param(IS_TIM5_IC_POLARITY_OK(TIM5_IC1Polarity));
  assert_param(IS_TIM5_IC_POLARITY_OK(TIM5_IC2Polarity));
  
  tmpsmcr = TIM5->SMCR;
  tmpccmr1 = TIM5->CCMR1;
  tmpccmr2 = TIM5->CCMR2;
  
  /* Set the encoder Mode */
  tmpsmcr &= (uint8_t)(TIM5_SMCR_MSM | TIM5_SMCR_TS)  ;
  tmpsmcr |= (uint8_t)TIM5_EncoderMode;
  
  /* Select the Capture Compare 1 and the Capture Compare 2 as input */
  tmpccmr1 &= (uint8_t)(~TIM5_CCMR_CCxS);
  tmpccmr2 &= (uint8_t)(~TIM5_CCMR_CCxS);
  tmpccmr1 |= TIM5_CCMR_TIxDirect_Set;
  tmpccmr2 |= TIM5_CCMR_TIxDirect_Set;
  
  /* Set the TI1 and the TI2 Polarities */
  if (TIM5_IC1Polarity == TIM5_ICPOLARITY_FALLING)
  {
    TIM5->CCER1 |= TIM5_CCER1_CC1P ;
  }
  else
  {
    TIM5->CCER1 &= (uint8_t)(~TIM5_CCER1_CC1P) ;
  }
  
  if (TIM5_IC2Polarity == TIM5_ICPOLARITY_FALLING)
  {
    TIM5->CCER1 |= TIM5_CCER1_CC2P ;
  }
  else
  {
    TIM5->CCER1 &= (uint8_t)(~TIM5_CCER1_CC2P) ;
  }
  
  TIM5->SMCR = tmpsmcr;
  TIM5->CCMR1 = tmpccmr1;
  TIM5->CCMR2 = tmpccmr2;
}

/**
  * @}
  */

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
