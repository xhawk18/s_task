/**
  ******************************************************************************
  * @file    stm8_eval_i2c_tsensor.h
  * @author  MCD Application Team
  * @version V2.1.3
  * @date    28-June-2013
  * @brief   This file contains all the functions prototypes for the 
  *          stm8_eval_i2c_tsensor firmware driver.
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
#ifndef __STM8_EVAL_I2C_TSENSOR_H
#define __STM8_EVAL_I2C_TSENSOR_H

/* Includes ------------------------------------------------------------------*/
#include "stm8_eval.h"


/** @addtogroup Utilities
  * @{
  */
  
/** @addtogroup STM8_EVAL
  * @{
  */ 

/** @addtogroup Common
  * @{
  */
  
/** @addtogroup STM8_EVAL_I2C_TSENSOR
  * @{
  */  

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/**
  * @brief  Block Size
  */
#define LM75_REG_TEMP        0x00  /* Temperature Register of LM75 */
#define LM75_REG_CONF        0x01  /* Configuration Register of LM75 */
#define LM75_REG_THYS        0x02  /* Temperature Register of LM75 */
#define LM75_REG_TOS         0x03  /* Over-temp Shutdown threshold Register of LM75 */
#define I2C_TIMEOUT         (uint32_t)0x3FFFF /*!< I2C Time out */
#define LM75_ADDR           0x90   /*!< LM75 address */
#define LM75_I2C_SPEED      100000 /*!< I2C Speed */

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/


void LM75_DeInit(void);
void LM75_Init(void);
ErrorStatus LM75_GetStatus(void);
uint16_t LM75_ReadTemp(void);
uint16_t LM75_ReadReg(uint8_t RegName);
void LM75_WriteReg(uint8_t RegName, uint16_t RegValue);
uint8_t LM75_ReadConfReg(void);
void LM75_WriteConfReg(uint8_t RegValue);
void LM75_ShutDown(FunctionalState NewState);  

#endif /* __STM8_EVAL_I2C_TSENSOR_H */

/**
  * @}
  */

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
