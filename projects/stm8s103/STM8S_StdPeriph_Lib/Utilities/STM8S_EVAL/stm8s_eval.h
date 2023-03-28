/**
  ******************************************************************************
  * @file    stm8s_eval.h
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    30-September-2014
  * @brief   Header file for stm8s_eval.c module.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM8S_EVAL_H
#define __STM8S_EVAL_H

/* Includes ------------------------------------------------------------------*/

/** @addtogroup Utilities
  * @{
  */

/** @addtogroup STM8S_EVAL
  * @{
  */

/** @defgroup STM8S_EVAL_Abstraction_Layer
  * @{
  */

/** @defgroup STM8S_EVAL_HARDWARE_RESOURCES
  * @{
  */

/**
@code
 The table below gives an overview of the hardware resources supported by each
 STM8S EVAL board.
     - LCD: Mono LCD (Glass and Serial (SPI))
     - sFLASH: serial SPI FLASH (M25Pxxx)
     - sEE: serial I2C EEPROM (M24C32, M24C64)
     - SD: SD Card memory (SPI (SD Card MODE))
  =============================================================================================+
    STM8S EVAL     | LED | Buttons  | Com Ports |    LCD    | sFLASH | sEE | TSENSOR | SD (SPI) |
  -============================================================================================+
   STM8-128-EVAL   |  5  |    6     |     é     | YES (SPI) |  YES   | YES |   No    |    YES   |
  =============================================================================================+
@endcode
*/
/**
  * @}
  */


/** @defgroup STM8S_EVAL_Exported_Types
  * @{
  */
typedef enum
{
  LED1 = 0,
  LED2 = 1,
  LED3 = 2,
  LED4 = 3,
  LED5 = 4  /* Bicolor Led */
} Led_TypeDef;

typedef enum
{
  BUTTON_KEY = 0,
  BUTTON_RIGHT = 1,
  BUTTON_LEFT = 2,
  BUTTON_UP = 3,
  BUTTON_DOWN = 4,
  BUTTON_SEL = 5
} Button_TypeDef;

typedef enum
{
  BUTTON_MODE_GPIO = 0,
  BUTTON_MODE_EXTI = 1
} ButtonMode_TypeDef;

typedef enum
{
  JOY_NONE = 0,
  JOY_SEL = 1,
  JOY_DOWN = 2,
  JOY_LEFT = 3,
  JOY_RIGHT = 4,
  JOY_UP = 5
} JOYState_TypeDef;

/**
  * @}
  */

/** @defgroup STM8S_EVAL_Exported_Constants
  * @{
  */
/**
  * @brief  Uncomment the line corresponding to the STMicroelectronics evaluation
  *   board used in your application.
  *
  *  Tip: To avoid modifying this file each time you need to switch between these
  *       boards, you can define the board in your toolchain compiler preprocessor.
  */
#if !defined (USE_STM8_128_EVAL)
//#define USE_STM8_128_EVAL
#endif

#ifdef USE_STM8_128_EVAL
 #include "stm8s.h"
 #include "stm8-128_eval/stm8_128_eval.h"
#else
 #error "Please select first the STM8S EVAL board to be used (in stm8s_eval.h)"
#endif

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __STM8S_EVAL_H */


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
