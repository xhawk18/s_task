/**
  ******************************************************************************
  * @file    stm8s_exti.c
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   This file contains all the functions for the EXTI peripheral.
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
#include "stm8s_exti.h"

/** @addtogroup STM8S_StdPeriph_Driver
  * @{
  */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* Public functions ----------------------------------------------------------*/

/**
  * @addtogroup EXTI_Public_Functions
  * @{
  */

/**
  * @brief  Deinitializes the external interrupt control registers to their default reset value.
  * @param  None
  * @retval None
  */
void EXTI_DeInit(void)
{
  EXTI->CR1 = EXTI_CR1_RESET_VALUE;
  EXTI->CR2 = EXTI_CR2_RESET_VALUE;
}

/**
  * @brief  Set the external interrupt sensitivity of the selected port.
  * @warning
  * - The modification of external interrupt sensitivity is only possible when the interrupts are disabled.
  * - The normal behavior is to disable the interrupts before calling this function, and re-enable them after.
  * @param   Port The port number to access.
  * @param   SensitivityValue The external interrupt sensitivity value to set.
  * @retval None
  * @par Required preconditions:
  * Global interrupts must be disabled before calling this function.
  */
void EXTI_SetExtIntSensitivity(EXTI_Port_TypeDef Port, EXTI_Sensitivity_TypeDef SensitivityValue)
{
  /* Check function parameters */
  assert_param(IS_EXTI_PORT_OK(Port));
  assert_param(IS_EXTI_SENSITIVITY_OK(SensitivityValue));
  
  /* Set external interrupt sensitivity */
  switch (Port)
  {
  case EXTI_PORT_GPIOA:
    EXTI->CR1 &= (uint8_t)(~EXTI_CR1_PAIS);
    EXTI->CR1 |= (uint8_t)(SensitivityValue);
    break;
  case EXTI_PORT_GPIOB:
    EXTI->CR1 &= (uint8_t)(~EXTI_CR1_PBIS);
    EXTI->CR1 |= (uint8_t)((uint8_t)(SensitivityValue) << 2);
    break;
  case EXTI_PORT_GPIOC:
    EXTI->CR1 &= (uint8_t)(~EXTI_CR1_PCIS);
    EXTI->CR1 |= (uint8_t)((uint8_t)(SensitivityValue) << 4);
    break;
  case EXTI_PORT_GPIOD:
    EXTI->CR1 &= (uint8_t)(~EXTI_CR1_PDIS);
    EXTI->CR1 |= (uint8_t)((uint8_t)(SensitivityValue) << 6);
    break;
  case EXTI_PORT_GPIOE:
    EXTI->CR2 &= (uint8_t)(~EXTI_CR2_PEIS);
    EXTI->CR2 |= (uint8_t)(SensitivityValue);
    break;
  default:
    break;
  }
}

/**
  * @brief  Set the TLI interrupt sensitivity.
  * @param   SensitivityValue The TLI interrupt sensitivity value.
  * @retval None
  * @par Required preconditions:
  * Global interrupts must be disabled before calling this function.
  */
void EXTI_SetTLISensitivity(EXTI_TLISensitivity_TypeDef SensitivityValue)
{
  /* Check function parameters */
  assert_param(IS_EXTI_TLISENSITIVITY_OK(SensitivityValue));
  
  /* Set TLI interrupt sensitivity */
  EXTI->CR2 &= (uint8_t)(~EXTI_CR2_TLIS);
  EXTI->CR2 |= (uint8_t)(SensitivityValue);
}

/**
  * @brief  Get the external interrupt sensitivity of the selected port.
  * @param   Port The port number to access.
  * @retval EXTI_Sensitivity_TypeDef The external interrupt sensitivity of the selected port.
  */
EXTI_Sensitivity_TypeDef EXTI_GetExtIntSensitivity(EXTI_Port_TypeDef Port)
{
  uint8_t value = 0;
  
  /* Check function parameters */
  assert_param(IS_EXTI_PORT_OK(Port));
  
  switch (Port)
  {
  case EXTI_PORT_GPIOA:
    value = (uint8_t)(EXTI->CR1 & EXTI_CR1_PAIS);
    break;
  case EXTI_PORT_GPIOB:
    value = (uint8_t)((uint8_t)(EXTI->CR1 & EXTI_CR1_PBIS) >> 2);
    break;
  case EXTI_PORT_GPIOC:
    value = (uint8_t)((uint8_t)(EXTI->CR1 & EXTI_CR1_PCIS) >> 4);
    break;
  case EXTI_PORT_GPIOD:
    value = (uint8_t)((uint8_t)(EXTI->CR1 & EXTI_CR1_PDIS) >> 6);
    break;
  case EXTI_PORT_GPIOE:
    value = (uint8_t)(EXTI->CR2 & EXTI_CR2_PEIS);
    break;
  default:
    break;
  }
  
  return((EXTI_Sensitivity_TypeDef)value);
}

/**
  * @brief  Get the TLI interrupt sensitivity.
  * @param  None
  * @retval EXTI_TLISensitivity_TypeDef The TLI interrupt sensitivity read.
  */
EXTI_TLISensitivity_TypeDef EXTI_GetTLISensitivity(void)
{
  uint8_t value = 0;
  
  /* Get TLI interrupt sensitivity */
  value = (uint8_t)(EXTI->CR2 & EXTI_CR2_TLIS);
  
  return((EXTI_TLISensitivity_TypeDef)value);
}

/**
  * @}
  */
  
/**
  * @}
  */
  

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
