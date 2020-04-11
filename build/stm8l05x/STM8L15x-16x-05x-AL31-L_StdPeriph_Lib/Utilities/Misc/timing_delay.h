/**
  ******************************************************************************
  * @file    timing_delay.h
  * @author  MCD Application Team
  * @version V2.1.3
  * @date    28-June-2013
  * @brief   Header for timing_delay.c
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
#ifndef __TIMING_DELAY_H
#define __TIMING_DELAY_H

/* Includes ------------------------------------------------------------------*/
#include "stm8l15x.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void Delay(__IO uint32_t nTime);
void TimingDelay_Decrement(void);
void TimingDelay_Init(void);

#endif /* __TIMING_DELAY_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
