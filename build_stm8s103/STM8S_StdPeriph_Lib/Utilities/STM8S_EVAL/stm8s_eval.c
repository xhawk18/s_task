/**
  ******************************************************************************
  * @file    stm8s_eval.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    30-September-2014
  * @brief   This file provides firmware functions to manage Leds, push-buttons
  *          and COM ports available on STM8S Evaluation Boards from STMicroelectronics.
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
#include "stm8s_eval.h"

/** @addtogroup Utilities
  * @{
  */

/** @defgroup STM8S_EVAL
  * @brief This file provides firmware functions to manage Leds, push-buttons
  *        and COM ports available on STM8 Evaluation Boards from STMicroelectronics.
  * @{
  */

/** @defgroup STM8S_EVAL_Abstraction_Layer
  * @{
  */

#ifdef USE_STM8_128_EVAL
 #include "stm8-128_eval/stm8_128_eval.c"
#else
 #error "Please select first the STM8 EVAL board to be used (in stm8s_eval.h)"
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private function ----------------------------------------------------------*/

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