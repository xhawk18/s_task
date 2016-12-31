/**
  ********************************************************************************
  * @file    stm8s_iwdg.c
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   This file contains all the functions for the IWDG peripheral.
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
#include "stm8s_iwdg.h"

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

/** @addtogroup IWDG_Public_Functions
  * @{
  */

/**
  * @brief  Enables or disables write access to Prescaler and Reload registers.
  * @param  IWDG_WriteAccess : New state of write access to Prescaler and Reload
  *         registers.  This parameter can be a value of @ref IWDG_WriteAccess_TypeDef.
  * @retval None
  */
void IWDG_WriteAccessCmd(IWDG_WriteAccess_TypeDef IWDG_WriteAccess)
{
  /* Check the parameters */
  assert_param(IS_IWDG_WRITEACCESS_MODE_OK(IWDG_WriteAccess));
  
  IWDG->KR = (uint8_t)IWDG_WriteAccess; /* Write Access */
}

/**
  * @brief  Sets IWDG Prescaler value.
  * @note   Write access should be enabled
  * @param  IWDG_Prescaler : Specifies the IWDG Prescaler value.
  *         This parameter can be a value of @ref IWDG_Prescaler_TypeDef.
  * @retval None
  */
void IWDG_SetPrescaler(IWDG_Prescaler_TypeDef IWDG_Prescaler)
{
  /* Check the parameters */
  assert_param(IS_IWDG_PRESCALER_OK(IWDG_Prescaler));
  
  IWDG->PR = (uint8_t)IWDG_Prescaler;
}

/**
  * @brief  Sets IWDG Reload value.
  * @note   Write access should be enabled
  * @param  IWDG_Reload : Reload register value.
  *         This parameter must be a number between 0 and 0xFF.
  * @retval None
  */
void IWDG_SetReload(uint8_t IWDG_Reload)
{
  IWDG->RLR = IWDG_Reload;
}

/**
  * @brief  Reloads IWDG counter
  * @note   Write access should be enabled
  * @param  None
  * @retval None
  */
void IWDG_ReloadCounter(void)
{
  IWDG->KR = IWDG_KEY_REFRESH;
}

/**
  * @brief  Enables IWDG.
  * @param  None
  * @retval None
  */
void IWDG_Enable(void)
{
  IWDG->KR = IWDG_KEY_ENABLE;
}

/**
  * @}
  */
  
/**
  * @}
  */
  

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
