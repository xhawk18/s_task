/**
  ******************************************************************************
  * @file    stm8_eval_i2c_ee.h
  * @author  MCD Application Team
  * @version V2.1.3
  * @date    28-June-2013
  * @brief   This file contains all the functions prototypes for the stm8_eval_i2c_ee
  *          firmware driver.
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
#ifndef __STM8_EVAL_I2C_EE_H
#define __STM8_EVAL_I2C_EE_H

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
  
/** @addtogroup STM8_EVAL_I2C_EE
  * @{
  */  

/* Private types -------------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
  
/** @defgroup STM8_EVAL_I2C_EE_Private_define
  * @{
  */
#if !defined (sEE_M24C64_32)
/* Use the defines below the choose the EEPROM type */
#define sEE_M24C64_32  /* Support the devices: M24C32 and M24C64 */
#endif

#ifdef sEE_M24C64_32
/* For M24C32 and M24C64 devices, E0,E1 and E2 pins are all used for device 
  address selection (ne need for additional address lines). According to the 
  Harware connection on the board (on STM810C-EVAL board E0 = E1 = E2 = 0) */

 #define sEE_HW_ADDRESS     0xA0   /* E0 = E1 = E2 = 0 */ 
#endif /* sEE_M24C64_32 */

#define I2C_SPEED              200000
#define I2C_SLAVE_ADDRESS7     0xA0

#if defined (sEE_M24C64_32)
 #define sEE_PAGESIZE    32
#endif
 
/* Defintions for the state of the DMA transfer */   
#define sEE_STATE_READY         0
#define sEE_STATE_BUSY          1
   
/* Maximum timeout value for counting before exiting waiting loop on DMA 
   Trasnfer Complete. This value depends directly on the maximum page size and
   the sytem clock frequency. */
#define sEE_TIMEOUT_MAX         0x10000
/**
  * @}
  */ 
  
/* Private macro -------------------------------------------------------------*/

/** @defgroup STM8_EVAL_I2C_EE_Exported_Functions
  * @{
  */ 
void sEE_DeInit(void);
void sEE_Init(void);
void sEE_WriteByte(uint8_t* pBuffer, uint16_t WriteAddr);
void sEE_WritePage(uint8_t* pBuffer, uint16_t WriteAddr, uint8_t* NumByteToWrite);
void sEE_WriteBuffer(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite);
void sEE_ReadBuffer(uint8_t* pBuffer, uint16_t ReadAddr, uint16_t* NumByteToRead);
void sEE_WaitEepromStandbyState(void);
void sEE_I2C_DMA_RX_IRQHandler(void);
void sEE_I2C_DMA_TX_IRQHandler(void);
#endif /* __STM8_EVAL_I2C_EE_H */
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

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/