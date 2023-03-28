/**
  ******************************************************************************
  * @file    stm8_eval_lcd.c
  * @author  MCD Application Team
  * @version V2.1.3
  * @date    28-June-2013
  * @brief   This file includes driver for the dot matrix LCD Module mounted on
  *          STM8L15xx-EVAL board.
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
#include "stm8_eval_lcd.h"

/** @addtogroup Utilities
  * @{
  */

/** @defgroup STM8_EVAL_LCD
  * @{
  */

/** @defgroup STM8_EVAL_LCD_Private_Types
  * @{
  */
/**
  * @}
  */

/** @defgroup STM8_EVAL_LCD_Private_Defines
  * @{
  */

#define STATUS_TYPE 0xFC

/* This table contains the "S" of ST logo */
CONST uint8_t S_CGRAM[] =
  {
    /* 0~7 */
    0x03, 0xff,
    0x02, 0x00,
    0x04, 0x00,
    0x04, 0x00,
    0x0c, 0x7f,
    0x0c, 0x7f,
    0x1c, 0x3f,
    0x1e, 0x1f,
    /* 8~15 */
    0x3f, 0x0f,
    0x3f, 0x87,
    0x7f, 0xc3,
    0x7f, 0xe3,
    0x00, 0x03,
    0x00, 0x03,
    0x00, 0x07,
    0xff, 0xfe,
  };

/* This table contains the "T" of ST logo */
CONST uint8_t T_CGRAM[] =
  {
    /* 0~7 */
    0xff, 0xff,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0xf8, 0xf8,
    0xf0, 0xf8,
    0xf0, 0xf0,
    0xf0, 0xf0,
    /* 8~15 */
    0xe1, 0xe0,
    0xe3, 0xe0,
    0xc3, 0xc0,
    0xc7, 0xc0,
    0x87, 0xc0,
    0x8f, 0x80,
    0x0f, 0x80,
    0x1f, 0x00
  };

/**
  * @}
  */


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void LCD_SPISendByte(uint8_t DataToSend);
static void LCD_DisplayCGRAM0(uint8_t address, uint8_t *ptrTable);
static void LCD_DisplayCGRAM1(uint8_t address, uint8_t *ptrTable);

/**
  * @brief  The delay function implemented in this driver is not a precise one,
  *         however it allows the insertion of 1ms delay when Fcpu is 16Mhz if 
  *         the passed parameter is 0x4000.
  *         Any change in system clock frequency will impact this delay duration.
  *         
  *         User is given the possibility to develop a customized and accurate
  *         delay function by the mean of timers for example. 
  *         Uncommenting " #define USE_Delay" line in the stm8_eval_lcd.h file 
  *         will allow the consideration of the new function by this driver. 
  */      
static void delay(__IO uint32_t nCount);
/* Private functions ---------------------------------------------------------*/

/** @defgroup STM8_EVAL_LCD_Private_Functions
  * @{
  */

/**
  * @brief  Initialize the LCD
  * @param  None
  * @retval None
  */
void STM8_EVAL_LCD_Init(void)
{
  /* Enable SPI clock */
  CLK_PeripheralClockConfig(LCD_SPI_CLK, ENABLE);

  /* Configure SPI pins: SCK and MOSI */
  GPIO_Init(LCD_SPI_GPIO_PORT, LCD_SPI_SCK_PIN | LCD_SPI_MOSI_PIN, GPIO_Mode_Out_PP_Low_Fast);

#ifdef USE_STM8L1528_EVAL
  /* SPI2 pin remap on Port I*/
  SYSCFG_REMAPPinConfig(REMAP_Pin_SPI2Full, ENABLE);
#endif /* USE_STM8L1528_EVAL */

  /* Initialize SPI */
  SPI_Init(LCD_SPI, SPI_FirstBit_MSB, SPI_BaudRatePrescaler_64, SPI_Mode_Master,
           SPI_CPOL_High, SPI_CPHA_2Edge,  SPI_Direction_1Line_Tx, SPI_NSS_Soft, 0x07);
  SPI_Cmd(LCD_SPI, ENABLE);

  /* Required to ensure proper LCD display when the board is powered-on ... */
  _delay_(0x4000); /* 1ms _delay_ using Fcpu = 16Mhz*/

  /* Configure LCD ChipSelect pin (NCS) in Output push-pull mode */
  GPIO_Init(LCD_NCS_GPIO_PORT, LCD_NCS_PIN, GPIO_Mode_Out_PP_Low_Fast);


  /* Set the LCD in TEXT mode */
  LCD_SendByte(COMMAND_TYPE, SET_TEXT_MODE);

  /* Enable the display */
  LCD_SendByte(COMMAND_TYPE, DISPLAY_ON);

  /* Clear the LCD */
  LCD_SendByte(COMMAND_TYPE, DISPLAY_CLR);

  /* Delay required to complete LCD clear command */
  _delay_(0x4000); /* 1ms _delay_ using Fcpu = 16Mhz*/

  /* Select the entry mode type */
  LCD_SendByte(COMMAND_TYPE, ENTRY_MODE_SET_INC);
}

/**
  * @brief  Set the LCD cursor to the specified location
  * @param  Line : line where the cursor will be set (LCD_LINE1 or LCD_LINE2)
  * @param  Offset : is the position offset (only even position are supported)
  * @retval None
  */
void LCD_SetCursorPos(uint8_t Line, uint8_t Offset)
{
  LCD_SendByte(COMMAND_TYPE, (uint8_t)(Line + Offset));
}

/**
  * @brief  Send a byte to LCD
  * @param  DataType Type of Data to be sent
  * @param  DataToSend Data to be sent
  * @retval None
  */
void LCD_SendByte(uint8_t DataType, uint8_t DataToSend)
{
  /* Enable access to LCD */
  LCD_NCS_HIGH();

  /* Send Synchro/Mode byte */
  LCD_SPISendByte(DataType);

  /* Send byte high nibble */
  LCD_SPISendByte((uint8_t)(DataToSend & (uint8_t)0xF0));

  /* Send byte low nibble */
  LCD_SPISendByte((uint8_t)((uint8_t)(DataToSend << 4) & (uint8_t)0xF0));
  _delay_(80);

  /* Disable access to LCD */
  LCD_NCS_LOW();
}

/**
  * @brief  Clear the LCD
  * @param  None
  * @retval None
  */
void LCD_Clear(void)
{
  LCD_SendByte(COMMAND_TYPE, DISPLAY_CLR); /* Clear the LCD */

  /* Delay required to complete LCD clear command */
  _delay_(0x4000); /* 1ms _delay_ using Fcpu = 16Mhz*/

}

/**
  * @brief  Display a string from current position of the LCD cursor
  * @param  ptr : Pointer to the string to display
  * @retval None
  */
void LCD_Print(uint8_t *ptr)
{
  __IO uint8_t charindex = 0x00;

  /* Display the string */
  while ((*ptr) && (charindex < 0x0F))
  {
    LCD_SendByte(DATA_TYPE, *ptr++);
    charindex++;
  }
}

/**
  * @brief  Display a string in rolling mode
  * @param  Line : line used for displaying the text (LCD_LINE1 or LCD_LINE2)
  * @param  ptr : Pointer to the text to display
  * @param  speed : Rolling speed
  * @retval
  * None
  */
void LCD_RollString(uint8_t Line, uint8_t *ptr, uint16_t speed)
{

  uint8_t CharPos = 0;
  uint8_t *ptr2;

  /* Set cursor position at beginning of line */
  LCD_SendByte(COMMAND_TYPE, Line);

  ptr2 = ptr;

  /* Display each character of the string */
  while (*ptr2 != 0)
  {
    if (*ptr != 0)
    {
      LCD_SendByte(DATA_TYPE, *ptr);
      ptr++;
    }
    else
    {
      LCD_SendByte(DATA_TYPE, ' ');
    }

    CharPos++;

    if (CharPos == LCD_LINE_MAX_CHAR)
    {
      _delay_(speed);

      /* Select the line to be cleared */
      LCD_SendByte(COMMAND_TYPE, Line);

      /* Clear the selected line */
      for (CharPos = 0; CharPos < LCD_LINE_MAX_CHAR; CharPos++)
      {
        LCD_SendByte(DATA_TYPE, ' ');
      }
      LCD_SendByte(COMMAND_TYPE, Line);
      CharPos = 0;
      ptr2++;
      ptr = ptr2;
    }
  }
}

/**
  * @brief  Display ST logo
  * @param  address : Display address (LINE1:0x80-0x87 and LINE2:0x90-0x97)
  * @retval None
  */
void LCD_DisplayLogo(uint8_t address)
{
  LCD_DisplayCGRAM0(address, (uint8_t*)S_CGRAM);
  LCD_DisplayCGRAM1(address, (uint8_t*)T_CGRAM);
}

/**
  * @}
  */

/** @addtogroup LCD_Private_Functions
  * @{
  */

/**
  * @brief  Send a byte to LCD through the SPI peripheral
  * @param  DataToSend : Data to be sent
  * @retval None
  */
static void LCD_SPISendByte(uint8_t DataToSend)
{
  while ((LCD_SPI->SR & SPI_SR_TXE) == 0)
  {
    /* Wait while the byte is transmitted */
  }

  /* Send byte through the SPI peripheral */
  LCD_SPI->DR = DataToSend;

}

/**
  * @brief  Display CGRAM on even address
  * @param  address : Display address
  * @param  ptrTable : Pointer a the CGRAM table to be displayed
  * @retval None
  */
static void LCD_DisplayCGRAM0(uint8_t address, uint8_t *ptrTable)
{

  uint8_t u = 32; /* Nb byte in the table */

  /* Set CGRAM Address */
  LCD_SendByte(COMMAND_TYPE, (uint8_t)0x40);

  while (u)
  {
    LCD_SendByte(DATA_TYPE, ptrTable[32 - u]);
    u--;
  }

  /* Setup Display Address */
  LCD_SendByte(COMMAND_TYPE, address);
  LCD_SendByte(DATA_TYPE, (uint8_t)0x00);
  LCD_SendByte(DATA_TYPE, (uint8_t)0x00);

}

/**
  * @brief  Display CGRAM on odd address
  * @param  address : Display address
  * @param  ptrTable : Pointer a the CGRAM table to be displayed
  * @retval None
  */
static void LCD_DisplayCGRAM1(uint8_t address, uint8_t *ptrTable)
{

  uint8_t u = 32; /* Nb byte in the table */

  /* Set CGRAM Address */
  LCD_SendByte(COMMAND_TYPE, (uint8_t)((uint8_t)0x40 | (uint8_t)0x10));

  while (u)
  {
    LCD_SendByte(DATA_TYPE, ptrTable[32 - u]);
    u--;
  }

  /* Setup Display Address */
  LCD_SendByte(COMMAND_TYPE, (uint8_t)(address + 1));
  LCD_SendByte(DATA_TYPE, (uint8_t)0x00);
  LCD_SendByte(DATA_TYPE, (uint8_t)0x02);

}

#ifndef USE_Delay
/**
  * @brief  Inserts a delay time.
  *         The delay function implemented in this driver is not a precise one,
  *         however it allows the insertion of 1ms delay when Fcpu is 16Mhz if 
  *         the passed parameter is 0x4000.
  *         Any change in system clock frequency will impact this delay duration.
  *         
  *         User is given the possibility to develop a customized and accurate
  *         delay function by the mean of timers for example. 
  *         Uncommenting " #define USE_Delay" line in the stm8_eval_lcd.h file 
  *         will allow the consideration of the new function by this driver. 
  *    
  * @param  nCount: specifies the _delay_ time length.
  * @retval None
  */
static void delay(__IO uint32_t nCount)
{
  /* Decrement nCount value */
  while (nCount != 0)
  {
    nCount--;
  }
}
#endif /* USE_Delay*/

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
