/**
  ******************************************************************************
  * @file    stm8_eval.c
  * @author  MCD Application Team
  * @version V2.1.3
  * @date    28-June-2013
  * @brief   This file provides firmware functions to manage Leds, push-buttons
  *          and COM ports available on STM8L Evaluation Boards from STMicroelectronics.
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
#include "stm8_eval.h"

/** @addtogroup Utilities
  * @{
  */

/** @defgroup STM8_EVAL
  * @brief This file provides firmware functions to manage Leds, push-buttons
  *        and COM ports available on STM8 Evaluation Boards from STMicroelectronics.
  * @{
  */

/** @defgroup STM8_EVAL_Abstraction_Layer
  * @{
  */

#ifdef USE_STM8L1526_EVAL
#include "stm8l1526_eval/stm8l1526_eval.c"
#elif defined USE_STM8L1528_EVAL
#include "stm8l1528_eval/stm8l1528_eval.c"
#else
#error "Please select first the STM8 EVAL board to be used (in stm8_eval.h)"
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


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