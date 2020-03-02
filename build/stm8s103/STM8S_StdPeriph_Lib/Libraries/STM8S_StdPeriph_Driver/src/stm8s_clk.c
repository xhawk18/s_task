/**
  ******************************************************************************
  * @file    stm8s_clk.c
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   This file contains all the functions for the CLK peripheral.
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

#include "stm8s_clk.h"

/** @addtogroup STM8S_StdPeriph_Driver
  * @{
  */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Private Constants ---------------------------------------------------------*/

/**
  * @addtogroup CLK_Private_Constants
  * @{
  */

CONST uint8_t HSIDivFactor[4] = {1, 2, 4, 8}; /*!< Holds the different HSI Divider factors */
CONST uint8_t CLKPrescTable[8] = {1, 2, 4, 8, 10, 16, 20, 40}; /*!< Holds the different CLK prescaler values */

/**
  * @}
  */

/* Public functions ----------------------------------------------------------*/
/**
  * @addtogroup CLK_Public_Functions
  * @{
  */

/**
  * @brief  Deinitializes the CLK peripheral registers to their default reset
  * values.
  * @param  None
  * @retval None
  * @par Warning:
  * Resetting the CCOR register: \n
  * When the CCOEN bit is set, the reset of the CCOR register require
  * two consecutive write instructions in order to reset first the CCOEN bit
  * and the second one is to reset the CCOSEL bits.
  */
void CLK_DeInit(void)
{
  CLK->ICKR = CLK_ICKR_RESET_VALUE;
  CLK->ECKR = CLK_ECKR_RESET_VALUE;
  CLK->SWR  = CLK_SWR_RESET_VALUE;
  CLK->SWCR = CLK_SWCR_RESET_VALUE;
  CLK->CKDIVR = CLK_CKDIVR_RESET_VALUE;
  CLK->PCKENR1 = CLK_PCKENR1_RESET_VALUE;
  CLK->PCKENR2 = CLK_PCKENR2_RESET_VALUE;
  CLK->CSSR = CLK_CSSR_RESET_VALUE;
  CLK->CCOR = CLK_CCOR_RESET_VALUE;
  while ((CLK->CCOR & CLK_CCOR_CCOEN)!= 0)
  {}
  CLK->CCOR = CLK_CCOR_RESET_VALUE;
  CLK->HSITRIMR = CLK_HSITRIMR_RESET_VALUE;
  CLK->SWIMCCR = CLK_SWIMCCR_RESET_VALUE;
}

/**
  * @brief   Configures the High Speed Internal oscillator (HSI).
  * @par Full description:
  * If CLK_FastHaltWakeup is enabled, HSI oscillator is automatically
  * switched-on (HSIEN=1) and selected as next clock master
  * (CKM=SWI=HSI) when resuming from HALT/ActiveHalt modes.\n
  * @param   NewState this parameter is the Wake-up Mode state.
  * @retval None
  */
void CLK_FastHaltWakeUpCmd(FunctionalState NewState)
{
  /* check the parameters */
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
  
  if (NewState != DISABLE)
  {
    /* Set FHWU bit (HSI oscillator is automatically switched-on) */
    CLK->ICKR |= CLK_ICKR_FHWU;
  }
  else  /* FastHaltWakeup = DISABLE */
  {
    /* Reset FHWU bit */
    CLK->ICKR &= (uint8_t)(~CLK_ICKR_FHWU);
  }
}

/**
  * @brief  Enable or Disable the External High Speed oscillator (HSE).
  * @param   NewState new state of HSEEN, value accepted ENABLE, DISABLE.
  * @retval None
  */
void CLK_HSECmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
  
  if (NewState != DISABLE)
  {
    /* Set HSEEN bit */
    CLK->ECKR |= CLK_ECKR_HSEEN;
  }
  else
  {
    /* Reset HSEEN bit */
    CLK->ECKR &= (uint8_t)(~CLK_ECKR_HSEEN);
  }
}

/**
  * @brief  Enables or disables the Internal High Speed oscillator (HSI).
  * @param   NewState new state of HSIEN, value accepted ENABLE, DISABLE.
  * @retval None
  */
void CLK_HSICmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
  
  if (NewState != DISABLE)
  {
    /* Set HSIEN bit */
    CLK->ICKR |= CLK_ICKR_HSIEN;
  }
  else
  {
    /* Reset HSIEN bit */
    CLK->ICKR &= (uint8_t)(~CLK_ICKR_HSIEN);
  }
}

/**
  * @brief  Enables or disables the Internal Low Speed oscillator (LSI).
  * @param  NewState new state of LSIEN, value accepted ENABLE, DISABLE.
  * @note   Before using the LSI clock you have to enable the option bytes (LSI_EN option bit is set).
  * @retval None
  */
void CLK_LSICmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
  
  if (NewState != DISABLE)
  {
    /* Set LSIEN bit */
    CLK->ICKR |= CLK_ICKR_LSIEN;
  }
  else
  {
    /* Reset LSIEN bit */
    CLK->ICKR &= (uint8_t)(~CLK_ICKR_LSIEN);
  }
}

/**
  * @brief  Enables or disablle the Configurable Clock Output (CCO).
  * @param   NewState : New state of CCEN bit (CCO register).
  * This parameter can be any of the @ref FunctionalState enumeration.
  * @retval None
  */
void CLK_CCOCmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
  
  if (NewState != DISABLE)
  {
    /* Set CCOEN bit */
    CLK->CCOR |= CLK_CCOR_CCOEN;
  }
  else
  {
    /* Reset CCOEN bit */
    CLK->CCOR &= (uint8_t)(~CLK_CCOR_CCOEN);
  }
}

/**
  * @brief  Starts or Stops manually the clock switch execution.
  * @par Full description:
  * NewState parameter set the SWEN.
  * @param   NewState new state of SWEN, value accepted ENABLE, DISABLE.
  * @retval None
  */
void CLK_ClockSwitchCmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
  
  if (NewState != DISABLE )
  {
    /* Enable the Clock Switch */
    CLK->SWCR |= CLK_SWCR_SWEN;
  }
  else
  {
    /* Disable the Clock Switch */
    CLK->SWCR &= (uint8_t)(~CLK_SWCR_SWEN);
  }
}

/**
  * @brief  Configures the slow active halt wake up
  * @param   NewState: specifies the Slow Active Halt wake up state.
  * can be set of the following values:
  * - DISABLE: Slow Active Halt mode disabled;
  * - ENABLE:  Slow Active Halt mode enabled.
  * @retval None
  */
void CLK_SlowActiveHaltWakeUpCmd(FunctionalState NewState)
{
  /* check the parameters */
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
  
  if (NewState != DISABLE)
  {
    /* Set S_ACTHALT bit */
    CLK->ICKR |= CLK_ICKR_SWUAH;
  }
  else
  {
    /* Reset S_ACTHALT bit */
    CLK->ICKR &= (uint8_t)(~CLK_ICKR_SWUAH);
  }
}

/**
  * @brief   Enables or disables the specified peripheral CLK.
  * @param   CLK_Peripheral : This parameter specifies the peripheral clock to gate.
  * This parameter can be any of the  @ref CLK_Peripheral_TypeDef enumeration.
  * @param   NewState : New state of specified peripheral clock.
  * This parameter can be any of the @ref FunctionalState enumeration.
  * @retval None
  */
void CLK_PeripheralClockConfig(CLK_Peripheral_TypeDef CLK_Peripheral, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
  assert_param(IS_CLK_PERIPHERAL_OK(CLK_Peripheral));
  
  if (((uint8_t)CLK_Peripheral & (uint8_t)0x10) == 0x00)
  {
    if (NewState != DISABLE)
    {
      /* Enable the peripheral Clock */
      CLK->PCKENR1 |= (uint8_t)((uint8_t)1 << ((uint8_t)CLK_Peripheral & (uint8_t)0x0F));
    }
    else
    {
      /* Disable the peripheral Clock */
      CLK->PCKENR1 &= (uint8_t)(~(uint8_t)(((uint8_t)1 << ((uint8_t)CLK_Peripheral & (uint8_t)0x0F))));
    }
  }
  else
  {
    if (NewState != DISABLE)
    {
      /* Enable the peripheral Clock */
      CLK->PCKENR2 |= (uint8_t)((uint8_t)1 << ((uint8_t)CLK_Peripheral & (uint8_t)0x0F));
    }
    else
    {
      /* Disable the peripheral Clock */
      CLK->PCKENR2 &= (uint8_t)(~(uint8_t)(((uint8_t)1 << ((uint8_t)CLK_Peripheral & (uint8_t)0x0F))));
    }
  }
}

/**
  * @brief  configures the Switch from one clock to another
  * @param   CLK_SwitchMode select the clock switch mode.
  * It can be set of the values of @ref CLK_SwitchMode_TypeDef
  * @param   CLK_NewClock choice of the future clock.
  * It can be set of the values of @ref CLK_Source_TypeDef
  * @param   NewState Enable or Disable the Clock Switch interrupt.
  * @param   CLK_CurrentClockState current clock to switch OFF or to keep ON.
  * It can be set of the values of @ref CLK_CurrentClockState_TypeDef
  * @note LSI selected as master clock source only if LSI_EN option bit is set.
  * @retval ErrorStatus this shows the clock switch status (ERROR/SUCCESS).
  */
ErrorStatus CLK_ClockSwitchConfig(CLK_SwitchMode_TypeDef CLK_SwitchMode, CLK_Source_TypeDef CLK_NewClock, FunctionalState ITState, CLK_CurrentClockState_TypeDef CLK_CurrentClockState)
{
  CLK_Source_TypeDef clock_master;
  uint16_t DownCounter = CLK_TIMEOUT;
  ErrorStatus Swif = ERROR;
  
  /* Check the parameters */
  assert_param(IS_CLK_SOURCE_OK(CLK_NewClock));
  assert_param(IS_CLK_SWITCHMODE_OK(CLK_SwitchMode));
  assert_param(IS_FUNCTIONALSTATE_OK(ITState));
  assert_param(IS_CLK_CURRENTCLOCKSTATE_OK(CLK_CurrentClockState));
  
  /* Current clock master saving */
  clock_master = (CLK_Source_TypeDef)CLK->CMSR;
  
  /* Automatic switch mode management */
  if (CLK_SwitchMode == CLK_SWITCHMODE_AUTO)
  {
    /* Enables Clock switch */
    CLK->SWCR |= CLK_SWCR_SWEN;
    
    /* Enables or Disables Switch interrupt */
    if (ITState != DISABLE)
    {
      CLK->SWCR |= CLK_SWCR_SWIEN;
    }
    else
    {
      CLK->SWCR &= (uint8_t)(~CLK_SWCR_SWIEN);
    }
    
    /* Selection of the target clock source */
    CLK->SWR = (uint8_t)CLK_NewClock;
    
    /* Wait until the target clock source is ready */
    while((((CLK->SWCR & CLK_SWCR_SWBSY) != 0 )&& (DownCounter != 0)))
    {
      DownCounter--;
    }
    
    if(DownCounter != 0)
    {
      Swif = SUCCESS;
    }
    else
    {
      Swif = ERROR;
    }
  }
  else /* CLK_SwitchMode == CLK_SWITCHMODE_MANUAL */
  {
    /* Enables or Disables Switch interrupt  if required  */
    if (ITState != DISABLE)
    {
      CLK->SWCR |= CLK_SWCR_SWIEN;
    }
    else
    {
      CLK->SWCR &= (uint8_t)(~CLK_SWCR_SWIEN);
    }
    
    /* Selection of the target clock source */
    CLK->SWR = (uint8_t)CLK_NewClock;
    
    /* Wait until the target clock source is ready */
    while((((CLK->SWCR & CLK_SWCR_SWIF) != 0 ) && (DownCounter != 0)))
    {
      DownCounter--;
    }
    
    if(DownCounter != 0)
    {
      /* Enables Clock switch */
      CLK->SWCR |= CLK_SWCR_SWEN;
      Swif = SUCCESS;
    }
    else
    {
      Swif = ERROR;
    }
  }
  if(Swif != ERROR)
  {
    /* Switch OFF current clock if required */
    if((CLK_CurrentClockState == CLK_CURRENTCLOCKSTATE_DISABLE) && ( clock_master == CLK_SOURCE_HSI))
    {
      CLK->ICKR &= (uint8_t)(~CLK_ICKR_HSIEN);
    }
    else if((CLK_CurrentClockState == CLK_CURRENTCLOCKSTATE_DISABLE) && ( clock_master == CLK_SOURCE_LSI))
    {
      CLK->ICKR &= (uint8_t)(~CLK_ICKR_LSIEN);
    }
    else if ((CLK_CurrentClockState == CLK_CURRENTCLOCKSTATE_DISABLE) && ( clock_master == CLK_SOURCE_HSE))
    {
      CLK->ECKR &= (uint8_t)(~CLK_ECKR_HSEEN);
    }
  }
  return(Swif);
}

/**
  * @brief  Configures the HSI clock dividers.
  * @param   HSIPrescaler : Specifies the HSI clock divider to apply.
  * This parameter can be any of the @ref CLK_Prescaler_TypeDef enumeration.
  * @retval None
  */
void CLK_HSIPrescalerConfig(CLK_Prescaler_TypeDef HSIPrescaler)
{
  /* check the parameters */
  assert_param(IS_CLK_HSIPRESCALER_OK(HSIPrescaler));
  
  /* Clear High speed internal clock prescaler */
  CLK->CKDIVR &= (uint8_t)(~CLK_CKDIVR_HSIDIV);
  
  /* Set High speed internal clock prescaler */
  CLK->CKDIVR |= (uint8_t)HSIPrescaler;
}

/**
  * @brief  Output the selected clock on a dedicated I/O pin.
  * @param   CLK_CCO : Specifies the clock source.
  * This parameter can be any of the  @ref CLK_Output_TypeDef enumeration.
  * @retval None
  * @par Required preconditions:
  * The dedicated I/O pin must be set at 1 in the corresponding Px_CR1 register \n
  * to be set as input with pull-up or push-pull output.
  */
void CLK_CCOConfig(CLK_Output_TypeDef CLK_CCO)
{
  /* check the parameters */
  assert_param(IS_CLK_OUTPUT_OK(CLK_CCO));
  
  /* Clears of the CCO type bits part */
  CLK->CCOR &= (uint8_t)(~CLK_CCOR_CCOSEL);
  
  /* Selects the source provided on cco_ck output */
  CLK->CCOR |= (uint8_t)CLK_CCO;
  
  /* Enable the clock output */
  CLK->CCOR |= CLK_CCOR_CCOEN;
}

/**
  * @brief   Enables or disables the specified CLK interrupts.
  * @param   CLK_IT This parameter specifies the interrupt sources.
  * It can be one of the values of @ref CLK_IT_TypeDef.
  * @param   NewState New state of the Interrupt.
  * Value accepted ENABLE, DISABLE.
  * @retval None
  */
void CLK_ITConfig(CLK_IT_TypeDef CLK_IT, FunctionalState NewState)
{
  /* check the parameters */
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
  assert_param(IS_CLK_IT_OK(CLK_IT));
  
  if (NewState != DISABLE)
  {
    switch (CLK_IT)
    {
    case CLK_IT_SWIF: /* Enable the clock switch interrupt */
      CLK->SWCR |= CLK_SWCR_SWIEN;
      break;
    case CLK_IT_CSSD: /* Enable the clock security system detection interrupt */
      CLK->CSSR |= CLK_CSSR_CSSDIE;
      break;
    default:
      break;
    }
  }
  else  /*(NewState == DISABLE)*/
  {
    switch (CLK_IT)
    {
    case CLK_IT_SWIF: /* Disable the clock switch interrupt */
      CLK->SWCR  &= (uint8_t)(~CLK_SWCR_SWIEN);
      break;
    case CLK_IT_CSSD: /* Disable the clock security system detection interrupt */
      CLK->CSSR &= (uint8_t)(~CLK_CSSR_CSSDIE);
      break;
    default:
      break;
    }
  }
}

/**
  * @brief  Configures the HSI and CPU clock dividers.
  * @param   ClockPrescaler Specifies the HSI or CPU clock divider to apply.
  * @retval None
  */
void CLK_SYSCLKConfig(CLK_Prescaler_TypeDef CLK_Prescaler)
{
  /* check the parameters */
  assert_param(IS_CLK_PRESCALER_OK(CLK_Prescaler));
  
  if (((uint8_t)CLK_Prescaler & (uint8_t)0x80) == 0x00) /* Bit7 = 0 means HSI divider */
  {
    CLK->CKDIVR &= (uint8_t)(~CLK_CKDIVR_HSIDIV);
    CLK->CKDIVR |= (uint8_t)((uint8_t)CLK_Prescaler & (uint8_t)CLK_CKDIVR_HSIDIV);
  }
  else /* Bit7 = 1 means CPU divider */
  {
    CLK->CKDIVR &= (uint8_t)(~CLK_CKDIVR_CPUDIV);
    CLK->CKDIVR |= (uint8_t)((uint8_t)CLK_Prescaler & (uint8_t)CLK_CKDIVR_CPUDIV);
  }
}

/**
  * @brief  Configures the SWIM clock frequency on the fly.
  * @param   CLK_SWIMDivider Specifies the SWIM clock divider to apply.
  * can be one of the value of @ref CLK_SWIMDivider_TypeDef
  * @retval None
  */
void CLK_SWIMConfig(CLK_SWIMDivider_TypeDef CLK_SWIMDivider)
{
  /* check the parameters */
  assert_param(IS_CLK_SWIMDIVIDER_OK(CLK_SWIMDivider));
  
  if (CLK_SWIMDivider != CLK_SWIMDIVIDER_2)
  {
    /* SWIM clock is not divided by 2 */
    CLK->SWIMCCR |= CLK_SWIMCCR_SWIMDIV;
  }
  else /* CLK_SWIMDivider == CLK_SWIMDIVIDER_2 */
  {
    /* SWIM clock is divided by 2 */
    CLK->SWIMCCR &= (uint8_t)(~CLK_SWIMCCR_SWIMDIV);
  }
}

/**
  * @brief  Enables the Clock Security System.
  * @par Full description:
  * once CSS is enabled it cannot be disabled until the next reset.
  * @param  None
  * @retval None
  */
void CLK_ClockSecuritySystemEnable(void)
{
  /* Set CSSEN bit */
  CLK->CSSR |= CLK_CSSR_CSSEN;
}

/**
  * @brief  Returns the clock source used as system clock.
  * @param  None
  * @retval  Clock source used.
  * can be one of the values of @ref CLK_Source_TypeDef
  */
CLK_Source_TypeDef CLK_GetSYSCLKSource(void)
{
  return((CLK_Source_TypeDef)CLK->CMSR);
}

/**
  * @brief  This function returns the frequencies of different on chip clocks.
  * @param  None
  * @retval the master clock frequency
  */
uint32_t CLK_GetClockFreq(void)
{
  uint32_t clockfrequency = 0;
  CLK_Source_TypeDef clocksource = CLK_SOURCE_HSI;
  uint8_t tmp = 0, presc = 0;
  
  /* Get CLK source. */
  clocksource = (CLK_Source_TypeDef)CLK->CMSR;
  
  if (clocksource == CLK_SOURCE_HSI)
  {
    tmp = (uint8_t)(CLK->CKDIVR & CLK_CKDIVR_HSIDIV);
    tmp = (uint8_t)(tmp >> 3);
    presc = HSIDivFactor[tmp];
    clockfrequency = HSI_VALUE / presc;
  }
  else if ( clocksource == CLK_SOURCE_LSI)
  {
    clockfrequency = LSI_VALUE;
  }
  else
  {
    clockfrequency = HSE_VALUE;
  }
  
  return((uint32_t)clockfrequency);
}

/**
  * @brief  Adjusts the Internal High Speed oscillator (HSI) calibration value.
  * @par Full description:
  * @param   CLK_HSICalibrationValue calibration trimming value.
  * can be one of the values of @ref CLK_HSITrimValue_TypeDef
  * @retval None
  */
void CLK_AdjustHSICalibrationValue(CLK_HSITrimValue_TypeDef CLK_HSICalibrationValue)
{
  /* check the parameters */
  assert_param(IS_CLK_HSITRIMVALUE_OK(CLK_HSICalibrationValue));
  
  /* Store the new value */
  CLK->HSITRIMR = (uint8_t)( (uint8_t)(CLK->HSITRIMR & (uint8_t)(~CLK_HSITRIMR_HSITRIM))|((uint8_t)CLK_HSICalibrationValue));
}

/**
  * @brief  Reset the SWBSY flag (SWICR Register)
  * @par Full description:
  * This function reset SWBSY flag in order to reset clock switch operations (target
  * oscillator is broken, stabilization is longing too much, etc.).  If at the same time \n
  * software attempts to set SWEN and clear SWBSY, SWBSY action takes precedence.
  * @param  None
  * @retval None
  */
void CLK_SYSCLKEmergencyClear(void)
{
  CLK->SWCR &= (uint8_t)(~CLK_SWCR_SWBSY);
}

/**
  * @brief  Checks whether the specified CLK flag is set or not.
  * @par Full description:
  * @param   CLK_FLAG Flag to check.
  * can be one of the values of @ref CLK_Flag_TypeDef
  * @retval FlagStatus, status of the checked flag
  */
FlagStatus CLK_GetFlagStatus(CLK_Flag_TypeDef CLK_FLAG)
{
  uint16_t statusreg = 0;
  uint8_t tmpreg = 0;
  FlagStatus bitstatus = RESET;
  
  /* check the parameters */
  assert_param(IS_CLK_FLAG_OK(CLK_FLAG));
  
  /* Get the CLK register index */
  statusreg = (uint16_t)((uint16_t)CLK_FLAG & (uint16_t)0xFF00);
  
  
  if (statusreg == 0x0100) /* The flag to check is in ICKRregister */
  {
    tmpreg = CLK->ICKR;
  }
  else if (statusreg == 0x0200) /* The flag to check is in ECKRregister */
  {
    tmpreg = CLK->ECKR;
  }
  else if (statusreg == 0x0300) /* The flag to check is in SWIC register */
  {
    tmpreg = CLK->SWCR;
  }
  else if (statusreg == 0x0400) /* The flag to check is in CSS register */
  {
    tmpreg = CLK->CSSR;
  }
  else /* The flag to check is in CCO register */
  {
    tmpreg = CLK->CCOR;
  }
  
  if ((tmpreg & (uint8_t)CLK_FLAG) != (uint8_t)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  
  /* Return the flag status */
  return((FlagStatus)bitstatus);
}

/**
  * @brief  Checks whether the specified CLK interrupt has is enabled or not.
  * @param   CLK_IT specifies the CLK interrupt.
  * can be one of the values of @ref CLK_IT_TypeDef
  * @retval ITStatus, new state of CLK_IT (SET or RESET).
  */
ITStatus CLK_GetITStatus(CLK_IT_TypeDef CLK_IT)
{
  ITStatus bitstatus = RESET;
  
  /* check the parameters */
  assert_param(IS_CLK_IT_OK(CLK_IT));
  
  if (CLK_IT == CLK_IT_SWIF)
  {
    /* Check the status of the clock switch interrupt */
    if ((CLK->SWCR & (uint8_t)CLK_IT) == (uint8_t)0x0C)
    {
      bitstatus = SET;
    }
    else
    {
      bitstatus = RESET;
    }
  }
  else /* CLK_IT == CLK_IT_CSSDIE */
  {
    /* Check the status of the security system detection interrupt */
    if ((CLK->CSSR & (uint8_t)CLK_IT) == (uint8_t)0x0C)
    {
      bitstatus = SET;
    }
    else
    {
      bitstatus = RESET;
    }
  }
  
  /* Return the CLK_IT status */
  return bitstatus;
}

/**
  * @brief  Clears the CLK’s interrupt pending bits.
  * @param   CLK_IT specifies the interrupt pending bits.
  * can be one of the values of @ref CLK_IT_TypeDef
  * @retval None
  */
void CLK_ClearITPendingBit(CLK_IT_TypeDef CLK_IT)
{
  /* check the parameters */
  assert_param(IS_CLK_IT_OK(CLK_IT));
  
  if (CLK_IT == (uint8_t)CLK_IT_CSSD)
  {
    /* Clear the status of the security system detection interrupt */
    CLK->CSSR &= (uint8_t)(~CLK_CSSR_CSSD);
  }
  else /* CLK_PendingBit == (uint8_t)CLK_IT_SWIF */
  {
    /* Clear the status of the clock switch interrupt */
    CLK->SWCR &= (uint8_t)(~CLK_SWCR_SWIF);
  }
  
}

/**
  * @}
  */
  
/**
  * @}
  */
  

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
