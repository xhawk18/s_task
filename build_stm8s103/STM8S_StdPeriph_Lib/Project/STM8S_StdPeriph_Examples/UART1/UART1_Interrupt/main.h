/**
  ******************************************************************************
  * @file    UART1_Interrupt\main.h
  * @author  MCD Application Team
  * @version V2.0.1
  * @date    18-November-2011
  * @brief   This file contains the external declaration of main.c file.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define TxBufferSize1   (countof(TxBuffer1) - 1)
#define TxBufferSize2   (countof(TxBuffer2) - 1)
#define RxBufferSize1   TxBufferSize2
#define RxBufferSize2   TxBufferSize1

/* Private macro -------------------------------------------------------------*/
#define countof(a)   (sizeof(a) / sizeof(*(a)))

/* Exported functions ------------------------------------------------------- */
uint8_t GetVar_TxCounter1(void);
uint8_t IncrementVar_TxCounter1(void);
uint8_t GetVar_TxCounter2(void);
uint8_t IncrementVar_TxCounter2(void);
uint8_t GetVar_RxCounter1(void);
uint8_t IncrementVar_RxCounter1(void);
uint8_t GetVar_RxCounter2(void);
uint8_t IncrementVar_RxCounter2(void);
uint8_t GetVar_NbrOfDataToTransfer1(void);
uint8_t GetVar_NbrOfDataToTransfer2(void);
uint8_t GetVar_NbrOfDataToRead1(void);
uint8_t GetVar_NbrOfDataToRead2(void);


#endif /* __MAIN_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
