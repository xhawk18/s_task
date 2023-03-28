/**
  ******************************************************************************
  * @file    stm8s_eval_lcd.h
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    30-September-2014
  * @brief   This file contains all the functions prototypes for the Dot matrix 
  *          LCD firmware driver.
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
#ifndef __STM8S_EVAL_LCD_H
#define __STM8S_EVAL_LCD_H

/* Includes ------------------------------------------------------------------*/
#include "stm8s_eval.h"
/** @addtogroup Utilities
  * @{
  */
  
/** @addtogroup STM8S_EVAL
  * @{
  */ 

/** @addtogroup Common
  * @{
  */

/* Exported constants --------------------------------------------------------*/

/*
 * Uncomment the line below if you want to use user defined Delay function
 * (for precise timing), otherwise default _delay_ function defined within
 *  this driver is used (less precise timing).  
 */
 
/* #define USE_Delay */

#ifdef USE_Delay
#include "main.h"
 
  #define _delay_     Delay  /* !< User can provide more timing precise _delay_ 
                                   function (with at least 1ms time base), using
                                   Timer for example */
#else
  #define _delay_     delay      /* !< Default _delay_ function with less precise timing */
#endif 	


/** @addtogroup STM8-128_EVAL_LOW_LEVEL_DOT_MATRIX_LCD
  * @{
  */
/**
  * @brief  Defines for the SPI and GPIO pins used to drive the SPI Flash
  */

 #define LCD_SPI                          SPI
 #define LCD_NCS_GPIO_PORT                GPIOF /* LCD Chip Select I/O definition */
 #define LCD_NCS_PIN                      GPIO_PIN_0
 #define LCD_SPI_CLK                      CLK_PERIPHERAL_SPI
 #define LCD_BACKLIGHT_PORT               GPIOH
 #define LCD_BACKLIGHT_PIN                GPIO_PIN_4
 #define LCD_SPI_SCK_PIN                  GPIO_PIN_5
 #define LCD_SPI_MOSI_PIN                 GPIO_PIN_6
 #define LCD_SPI_MISO_PIN                 GPIO_PIN_7
 #define LCD_SPI_GPIO_PORT                GPIOC

/**
  * @}
  */


/* LCD managed as 2 Lines of 15 characters (2Lines * 15Char) */
#define LCD_LINE_MAX_CHAR    ((uint8_t)15)
#define LCD_LINE1            ((uint8_t)0x80)
#define LCD_LINE2            ((uint8_t)0x90)

/* LCD Commands */
#define COMMAND_TYPE         ((uint8_t)0xF8)
#define DATA_TYPE            ((uint8_t)0xFA)
#define SET_TEXT_MODE        ((uint8_t)0x30) /* 8-Bits Interface, Normal inst., Text mode */
#define SET_EXTENDED_INST    ((uint8_t)0x34)
#define SET_GRAPHIC_MODE     ((uint8_t)0x36) /* 8-Bits Interface, Extended inst., Graphic mode */
#define DISPLAY_ON           ((uint8_t)0x0C) /* Cursor and blink off */
#define DISPLAY_OFF          ((uint8_t)0x08)
#define DISPLAY_CLR          ((uint8_t)0x01)
#define ENTRY_MODE_SET_INC   ((uint8_t)0x06)


/** @defgroup STM8S_EVAL_LCD_Exported_Macros
  * @{
  */

/** 
  * @brief  Deselect LCD: ChipSelect pin low : LCD disabled
  */  
#define LCD_NCS_LOW()     GPIO_WriteLow(LCD_NCS_GPIO_PORT, LCD_NCS_PIN)
/** 
  * @brief  Select LCD : ChipSelect pin high : LCD enabled   
  */ 
#define LCD_NCS_HIGH()    GPIO_WriteHigh(LCD_NCS_GPIO_PORT, LCD_NCS_PIN)
/**
  * @}
  */
  
/* Exported functions --------------------------------------------------------*/
void STM8S_EVAL_LCD_Init(void);
void LCD_BacklightCmd(FunctionalState NewState);
void LCD_SetCursorPos(uint8_t Line, uint8_t Offset);
void LCD_SendByte(uint8_t DataType, uint8_t DataToSend);
void LCD_Clear(void);
void LCD_Print(uint8_t *ptr);
void LCD_DisplayLogo(uint8_t address);
void LCD_RollString(uint8_t Line, uint8_t *ptr, uint16_t speed);

#endif /* __STM8S_EVAL_LCD_H */

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
