/**
  ******************************************************************************
  * @file    stm8l1526_eval_glass_lcd.h
  * @author  MCD Application Team
  * @version V2.1.3
  * @date    28-June-2013
  * @brief   This file contains all the functions prototypes for the glass LCD
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
#ifndef __STM8L1526_EVAL_GLASS_LCD_H
#define __STM8L1526_EVAL_GLASS_LCD_H

/* Includes ------------------------------------------------------------------*/
#include "stm8l15x.h"

/* Private define ------------------------------------------------------------*/
typedef enum
{
  POINT_OFF = 0,
  POINT_ON = 1
}Point_Typedef;

typedef enum
{
  APOSTROPHE_OFF = 0,
  APOSTROPHE_ON = 1
}Apostrophe_Typedef;


#define LCD_CLEAR_BYTE (uint8_t)0x00
#define LCD_SET_BYTE   (uint8_t)0xFF
#define LCD_CLEAR_MSB  (uint8_t)0x0F
#define LCD_CLEAR_LSB  (uint8_t)0xF0

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void LCD_GLASS_Init(void);
void LCD_GLASS_WriteChar(uint8_t* ch, Point_Typedef point, Apostrophe_Typedef apostrophe, uint8_t position);
void LCD_GLASS_DisplayString(uint8_t* ptr);
void LCD_GLASS_ClearChar(uint8_t position);
void LCD_GLASS_Clear(void);
void LCD_GLASS_ScrollString(uint8_t* ptr, uint16_t nScroll, uint16_t ScrollSpeed);

#endif /* __STM8L1526_EVAL_GLASS_LCD_H*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
