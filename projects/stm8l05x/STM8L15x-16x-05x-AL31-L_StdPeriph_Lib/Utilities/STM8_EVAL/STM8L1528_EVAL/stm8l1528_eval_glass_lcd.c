/**
  ******************************************************************************
  * @file    stm8l1528_eval_glass_lcd.c
  * @author  MCD Application Team
  * @version V2.1.3
  * @date    28-June-2013
  * @brief   This file includes driver for the glass LCD Module mounted on
  *          Big-Falco EVAL board.
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
#include "stm8l1528_eval_glass_lcd.h"

/** @addtogroup Utilities
  * @{
  */

/** @defgroup STM8L1528_EVAL_GLASS_LCD
  * @brief This file includes the LCD Glass driver for Pacific Display
  *        (Custom LCD 8x40) Module of Big-Falco-EVAL board.
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/**
  @verbatim
================================================================================
                              GLASS LCD MAPPING
================================================================================
               A
       _  ----------
     P|_| |\   |H  /|
         F| G  |  I |B
          |  \ | /  |
          --M-- --N--
          |   /| \  |
         E|  L |  J |C
       _  | /  |K  \|
     Q|_| -----------
              D

A LCD character coding is based on the following matrix:
             COM0    COM1    COM4    COM5     COM6    COM7
  SEG(n)    { 0 ,     0 ,     I ,     B ,      C ,     J }
  SEG(n+1)  { D ,     K ,     A ,     H ,      M ,     N }
  SEG(n+2)  { Q ,     L ,     G ,     F ,      P ,     E }

The character A for example is:
-----------------------------------------------------------
             COM0    COM1     COM4    COM5    COM6     COM7
  SEG(n)    { 0 ,     0 ,      0 ,     1 ,     1 ,      0 }
  SEG(n+1)  { 0 ,     0 ,      1 ,     0 ,     1 ,      1 }
  SEG(n+2)  { 0 ,     0 ,      0 ,     1 ,     0 ,      1 }
   --------------------------------------------------------
           =  0       0        2       5       3        6 hex

   => 'A' = 0x002536

  @endverbatim
  */

/**
  * @brief LETTERS AND NUMBERS MAPPING DEFINITION
  */
uint16_t i;
CONST uint32_t mask [] =
  {
    0xF00000, 0x0F0000, 0x00F000, 0x000F00, 0x0000F0, 0x00000F
  };
CONST uint8_t shift[6] =
  {
    20, 16, 12, 8, 4, 0
  };

uint8_t digit[6];     /* Digit frame buffer */

/* Letters and number map of the custom LCD 8x40(Big-Falco Evaluation boad) */
CONST uint32_t LetterMap[26] =
  {
    /* A         B         C         D        E          F         G          H         I  */
    0x002536, 0x202536, 0x202404, 0x222310, 0x202426, 0x002426, 0x202416, 0x000536, 0x222200,
    /*  J        K         L         M         N         O         P          Q         R  */
    0x200114, 0x001425, 0x200404, 0x005514, 0x004515, 0x202514, 0x002526, 0x002532, 0x002527,
    /*  S        T         U         V         W         X         Y          Z  */
    0x202432, 0x022200, 0x200514, 0x041404, 0x050515, 0x045001, 0x025000, 0x243000
  };

CONST uint32_t NumberMap[10] =
  {
    /*  0         1          2        3        4   */
    0x202514, 0x000110, 0x202126, 0x202132, 0x000532,
    /*  5         6          7        8        9   */
    0x202432, 0x202436, 0x002110, 0x202536, 0x202532
  };

__IO uint8_t str[7] = "";
/* Private function prototypes -----------------------------------------------*/
static void Convert(uint8_t* c, Point_Typedef Point, DoublePoint_Typedef DoublePoint);
static void delay(__IO uint32_t nCount);
/* Private functions ---------------------------------------------------------*/

/** @defgroup STM8L1528_EVAL_GLASS_LCD_Private_Functions
  * @{
  */

/*  * @brief  Configures the LCD GLASS GPIO port IOs and LCD peripheral.
  * @param  None
  * @retval None
  */

void LCD_GLASS_Init(void)
{
  /*
    The LCD is configured as follow:
     - clock source = LSE (32.768 KHz)
     - Voltage source = Internal
     - Prescaler = 2
     - Divider = 18 (16 + 2)  
     - Mode = 1/8 Duty, 1/4 Bias
     - LCD frequency = (clock source * Duty) / (Prescaler * Divider)
                     = 114 Hz ==> Frame frequency = 28,5 Hz*/


  /* Enable LCD clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_LCD, ENABLE);
  CLK_RTCClockConfig(CLK_RTCCLKSource_LSE, CLK_RTCCLKDiv_1);

  /* Initialize the LCD */
  LCD_Init(LCD_Prescaler_2, LCD_Divider_18, LCD_Duty_1_8,
           LCD_Bias_1_4, LCD_VoltageSource_Internal);

  /* Mask register*/
  LCD_PortMaskConfig(LCD_PortMaskRegister_0, 0xFF);
  LCD_PortMaskConfig(LCD_PortMaskRegister_1, 0xFF);
  LCD_PortMaskConfig(LCD_PortMaskRegister_2, 0xFF);
  LCD_PortMaskConfig(LCD_PortMaskRegister_3, 0xFF);
  LCD_PortMaskConfig(LCD_PortMaskRegister_4, 0xFF);
  LCD_PortMaskConfig(LCD_PortMaskRegister_5, 0x0F);

  LCD_ContrastConfig(LCD_Contrast_Level_7);

  LCD_PulseOnDurationConfig(LCD_PulseOnDuration_7);

  LCD_Cmd(ENABLE); /*!< Enable LCD peripheral */
}

/**
  * @brief  This function writes a char on the LCD Glass.
  * @param  ch: the character to dispaly.
  * @param  point: a point to add in front of char
  *         This parameter can be: POINT_OFF or POINT_ON
  * @param  DoublePoint: flag indicating if a double point has to be added in front
  *         of the displayed character.
  *         This parameter can be: DOUBLEPOINT_OFF or DOUBLEPOINT_ON.
  * @param  position: position in the LCD of the character to write [0:6]
  * @retval None
  */
void LCD_GLASS_WriteChar(uint8_t* ch, Point_Typedef Point,
                         DoublePoint_Typedef DoublePoint, uint8_t Position)
{
  Convert(ch, Point, DoublePoint);
  /* Enable the write access on the LCD RAM first banck */
  LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
  switch (Position)
  {
      /* Position 0 on LCD (Digit1)*/
    case 0:
      /* Enable the write access on the LCD RAM First banck */
      LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;

      /*Write Digit 0 on COM0 */
      LCD->RAM[LCD_RAMRegister_1] &= (uint8_t)0xFB;
      LCD->RAM[LCD_RAMRegister_1] |= (uint8_t)(digit[0] << (uint8_t)0x01);

      /*Write Digit 1 on COM1 */
      LCD->RAM[LCD_RAMRegister_4] &= (uint8_t)0x3F;
      LCD->RAM[LCD_RAMRegister_4] |= (uint8_t)(digit[1] << (uint8_t)0x05);

      /* Enable the write access on the LCD RAM second banck */
      LCD->CR4 |= LCD_CR4_PAGECOM;

      /*Write Digit 2 on COM4 */
      LCD->RAM[LCD_RAMRegister_1] &= (uint8_t)0xF1;
      LCD->RAM[LCD_RAMRegister_1] |= (uint8_t)(digit[2] << (uint8_t)0x01);

      /*Write Digit 3 on COM5 */
      LCD->RAM[LCD_RAMRegister_4] &= (uint8_t)0x1F;
      LCD->RAM[LCD_RAMRegister_4] |= (uint8_t)(digit[3] << (uint8_t)0x05);

      /*Write Digit 4 on COM6 */
      LCD->RAM[LCD_RAMRegister_8] &= (uint8_t)0xF9;
      LCD->RAM[LCD_RAMRegister_8] |= (uint8_t)(digit[4] << (uint8_t)0x01);

      /*Write Digit 5 on COM7 */
      LCD->RAM[LCD_RAMRegister_11] &= (uint8_t)0x1F;
      LCD->RAM[LCD_RAMRegister_11] |= (uint8_t)(digit[5] << (uint8_t)0x05);
      break;

      /* Position 1 on LCD */
    case 1:
      /* Enable the write access on the LCD RAM First banck */
      LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;

      /*Write Digit 0 on COM0 */
      LCD->RAM[LCD_RAMRegister_1] &= (uint8_t)0x9F;
      LCD->RAM[LCD_RAMRegister_1] |= (uint8_t)(digit[0] << (uint8_t)0x04);

      /*Write Digit 1 on COM1 */
      LCD->RAM[LCD_RAMRegister_5] &= (uint8_t)0xF9;
      LCD->RAM[LCD_RAMRegister_5] |= (uint8_t)(digit[1]);

      /* Enable the write access on the LCD RAM second banck */
      LCD->CR4 |= LCD_CR4_PAGECOM;
      /*Write Digit 2 on COM4 */
      LCD->RAM[LCD_RAMRegister_1] &= (uint8_t)0x8F;
      LCD->RAM[LCD_RAMRegister_1] |= (uint8_t)(digit[2] << (uint8_t)0x04);

      /*Write Digit 3 on COM5 */
      LCD->RAM[LCD_RAMRegister_5] &= (uint8_t)0xF8;
      LCD->RAM[LCD_RAMRegister_5] |= (uint8_t)(digit[3]);

      /*Write Digit 4 on COM6 */
      LCD->RAM[LCD_RAMRegister_8] &= (uint8_t)0x8F;
      LCD->RAM[LCD_RAMRegister_8] |= (uint8_t)(digit[4] << (uint8_t)0x04);

      /*Write Digit 5 on COM7 */
      LCD->RAM[LCD_RAMRegister_12] &= (uint8_t)0xF8;
      LCD->RAM[LCD_RAMRegister_12] |= (uint8_t)(digit[5]);
      break;

      /* Position 2 on LCD (Digit3)*/
    case 2:
      /* Enable the write access on the LCD RAM First banck */
      LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;

      /*Write Digit 0 on COM0 */
      LCD->RAM[LCD_RAMRegister_1] &= (uint8_t)0xFF;
      LCD->RAM[LCD_RAMRegister_2] &= (uint8_t)0xFC;
      LCD->RAM[LCD_RAMRegister_1] |= (uint8_t)((digit[0] & (uint8_t)0x01) << (uint8_t)0x07);
      LCD->RAM[LCD_RAMRegister_2] |= (uint8_t)((digit[0] & (uint8_t)0x06) >> (uint8_t)0x01);

      /*Write Digit 1 on COM1 */
      LCD->RAM[LCD_RAMRegister_5] &= (uint8_t)0xCF;
      LCD->RAM[LCD_RAMRegister_5] |= (uint8_t)(digit[1] << (uint8_t)0x03);

      /* Enable the write access on the LCD RAM second banck */
      LCD->CR4 |= LCD_CR4_PAGECOM;
      /*Write Digit 2 on COM4 */
      LCD->RAM[LCD_RAMRegister_1] &= (uint8_t)0x7F;
      LCD->RAM[LCD_RAMRegister_2] &= (uint8_t)0xFC;
      LCD->RAM[LCD_RAMRegister_1] |= (uint8_t)((digit[2] & (uint8_t)0x1) << (uint8_t)0x07);
      LCD->RAM[LCD_RAMRegister_2] |= (uint8_t)((digit[2] & (uint8_t)0x6) >> (uint8_t)0x01);

      /*Write Digit 3 on COM5 */
      LCD->RAM[LCD_RAMRegister_5] &= (uint8_t)0xC7;
      LCD->RAM[LCD_RAMRegister_5] |= (uint8_t)(digit[3] << (uint8_t)0x03);

      /*Write Digit 4 on COM6 */
      LCD->RAM[LCD_RAMRegister_8] &= (uint8_t)0x7F;
      LCD->RAM[LCD_RAMRegister_9] &= (uint8_t)0xFC;
      LCD->RAM[LCD_RAMRegister_8] |= (uint8_t)((digit[4] & (uint8_t)0x1) << (uint8_t)0x07);
      LCD->RAM[LCD_RAMRegister_9] |= (uint8_t)((digit[4] & (uint8_t)0x6) >> (uint8_t)0x01);
      /*Write Digit 5 on COM7 */
      LCD->RAM[LCD_RAMRegister_12] &= (uint8_t)0xC7;
      LCD->RAM[LCD_RAMRegister_12] |= (uint8_t)(digit[5] << (uint8_t)0x03);
      break;

      /* Position 3 on LCD */
    case 3:
      /* Enable the write access on the LCD RAM First banck */
      LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;

      /*Write Digit 0 on COM0 */
      LCD->RAM[LCD_RAMRegister_2] &= (uint8_t)0xE7;
      LCD->RAM[LCD_RAMRegister_2] |= (uint8_t)(digit[0] << (uint8_t)0x02);

      /*Write Digit 1 on COM1 */
      LCD->RAM[LCD_RAMRegister_5] &= (uint8_t)0x7F;
      LCD->RAM[LCD_RAMRegister_6] &= (uint8_t)0xFE;
      LCD->RAM[LCD_RAMRegister_5] |= (uint8_t)((digit[1] & (uint8_t)0x03) << (uint8_t)0x06);
      LCD->RAM[LCD_RAMRegister_6] |= (uint8_t)((digit[1] & (uint8_t)0x04) >> (uint8_t)0x02);

      /* Enable the write access on the LCD RAM second banck */
      LCD->CR4 |= LCD_CR4_PAGECOM;

      /*Write Digit 2 on COM4 */
      LCD->RAM[LCD_RAMRegister_2] &= (uint8_t)0xE3;
      LCD->RAM[LCD_RAMRegister_2] |= (uint8_t)(digit[2] << (uint8_t)0x02);

      /*Write Digit 3 on COM5 */
      LCD->RAM[LCD_RAMRegister_5] &= (uint8_t)0x3F;
      LCD->RAM[LCD_RAMRegister_6] &= (uint8_t)0xFE;
      LCD->RAM[LCD_RAMRegister_5] |= (uint8_t)((digit[3] & (uint8_t)0x3) << (uint8_t)0x06);
      LCD->RAM[LCD_RAMRegister_6] |= (uint8_t)((digit[3] & (uint8_t)0x4) >> (uint8_t)0x02);

      /*Write Digit 4 on COM6 */
      LCD->RAM[LCD_RAMRegister_9] &= (uint8_t)0xE3;
      LCD->RAM[LCD_RAMRegister_9] |= (uint8_t)(digit[4] << (uint8_t)0x02);

      /*Write Digit 5 on COM7 */
      LCD->RAM[LCD_RAMRegister_12] &= (uint8_t)0x3F;
      LCD->RAM[LCD_RAMRegister_13] &= (uint8_t)0xFE;
      LCD->RAM[LCD_RAMRegister_12] |= (uint8_t)((digit[5] & (uint8_t)0x3) << (uint8_t)0x06);
      LCD->RAM[LCD_RAMRegister_13] |= (uint8_t)((digit[5] & (uint8_t)0x4) >> (uint8_t)0x02);
      break;

      /* Position 4 on LCD (Digit5)*/
    case 4:
      /* Enable the write access on the LCD RAM First banck */
      LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;

      /*Write Digit 0 on COM0 */
      LCD->RAM[LCD_RAMRegister_0] &= (uint8_t)0xF9;
      LCD->RAM[LCD_RAMRegister_0] |= (uint8_t)(digit[0]);

      /*Write Digit 1 on COM1 */
      LCD->RAM[LCD_RAMRegister_3] &= (uint8_t)0x9F;
      LCD->RAM[LCD_RAMRegister_3] |= (uint8_t)(digit[1] << (uint8_t)0x4);

      /* Enable the write access on the LCD RAM second banck */
      LCD->CR4 |= LCD_CR4_PAGECOM;
      /*Write Digit 2 on COM4 */
      LCD->RAM[LCD_RAMRegister_0] &= (uint8_t)0xF8;
      LCD->RAM[LCD_RAMRegister_0] |= (uint8_t)(digit[2]);

      /*Write Digit 3 on COM5 */
      LCD->RAM[LCD_RAMRegister_3] &= (uint8_t)0x8F;
      LCD->RAM[LCD_RAMRegister_3] |= (uint8_t)(digit[3] << (uint8_t)0x04);

      /*Write Digit 4 on COM6 */
      LCD->RAM[LCD_RAMRegister_7] &= (uint8_t)0xF8;
      LCD->RAM[LCD_RAMRegister_7] |= (uint8_t)(digit[4]);

      /*Write Digit 5 on COM7 */
      LCD->RAM[LCD_RAMRegister_10] &= (uint8_t)0x8F;
      LCD->RAM[LCD_RAMRegister_10] |= (uint8_t)(digit[5] << (uint8_t)0x04);
      break;

      /* Position 5 on LCD (Digit6)*/
    case 5:

      /* Enable the write access on the LCD RAM First banck */
      LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;

      /*Write Digit 0 on COM0 */
      LCD->RAM[LCD_RAMRegister_0] &= (uint8_t)0xCF;
      LCD->RAM[LCD_RAMRegister_0] |= (uint8_t)(digit[0] << (uint8_t)0x03);

      /*Write Digit 1 on COM1 */
      LCD->RAM[LCD_RAMRegister_3] &= (uint8_t)0xFF;
      LCD->RAM[LCD_RAMRegister_4] &= (uint8_t)0xFC;
      LCD->RAM[LCD_RAMRegister_3] |= (uint8_t)((digit[1] & (uint8_t)0x01) << (uint8_t)0x07);
      LCD->RAM[LCD_RAMRegister_4] |= (uint8_t)((digit[1] & (uint8_t)0x06) >> (uint8_t)0x01);

      /* Enable the write access on the LCD RAM second banck */
      LCD->CR4 |= LCD_CR4_PAGECOM;
      /*Write Digit 2 on COM4 */
      LCD->RAM[LCD_RAMRegister_0] &= (uint8_t)0xC7;
      LCD->RAM[LCD_RAMRegister_0] |= (uint8_t)(digit[2] << (uint8_t)0x03);

      /*Write Digit 3 on COM5 */
      LCD->RAM[LCD_RAMRegister_3] &= (uint8_t)0x7F;
      LCD->RAM[LCD_RAMRegister_4] &= (uint8_t)0xFC;
      LCD->RAM[LCD_RAMRegister_3] |= (uint8_t)((digit[3] & (uint8_t)0x01) << (uint8_t)0x07);
      LCD->RAM[LCD_RAMRegister_4] |= (uint8_t)((digit[3] & (uint8_t)0x06) >> (uint8_t)0x01);

      /*Write Digit 4 on COM6 */
      LCD->RAM[LCD_RAMRegister_7] &= (uint8_t)0xC7;
      LCD->RAM[LCD_RAMRegister_7] |= (uint8_t)(digit[4] << (uint8_t)0x03);

      /*Write Digit 5 on COM7 */
      LCD->RAM[LCD_RAMRegister_10] &= (uint8_t)0x7F;
      LCD->RAM[LCD_RAMRegister_11] &= (uint8_t)0xFC;
      LCD->RAM[LCD_RAMRegister_10] |= (uint8_t)((digit[5] & (uint8_t)0x01) << (uint8_t)0x07);
      LCD->RAM[LCD_RAMRegister_11] |= (uint8_t)((digit[5] & (uint8_t)0x06) >> (uint8_t)0x01);
      break;

      /* Position 6 on LCD (Digit7)*/
    case 6:

      /* Enable the write access on the LCD RAM First banck */
      LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;

      /*Write Digit 0 on COM0 */
      LCD->RAM[LCD_RAMRegister_0] &= (uint8_t)0x7F;
      LCD->RAM[LCD_RAMRegister_1] &= (uint8_t)0xFE;
      LCD->RAM[LCD_RAMRegister_0] |= (uint8_t)((digit[0] & (uint8_t)0x03) << (uint8_t)0x06);
      LCD->RAM[LCD_RAMRegister_1] |= (uint8_t)((digit[0] & (uint8_t)0x04) >> (uint8_t)0x02);

      /*Write Digit 1 on COM1 */
      LCD->RAM[LCD_RAMRegister_4] &= (uint8_t)0xE7;
      LCD->RAM[LCD_RAMRegister_4] |= (uint8_t)(digit[1] << (uint8_t)0x02);

      /* Enable the write access on the LCD RAM second banck */
      LCD->CR4 |= LCD_CR4_PAGECOM;
      /*Write Digit 2 on COM4 */
      LCD->RAM[LCD_RAMRegister_0] &= (uint8_t)0x3F;
      LCD->RAM[LCD_RAMRegister_1] &= (uint8_t)0xFE;
      LCD->RAM[LCD_RAMRegister_0] |= (uint8_t)((digit[2] & (uint8_t)0x03) << (uint8_t)0x06);
      LCD->RAM[LCD_RAMRegister_1] |= (uint8_t)((digit[2] & (uint8_t)0x04) >> (uint8_t)0x02);

      /*Write Digit 3 on COM5 */
      LCD->RAM[LCD_RAMRegister_4] &= (uint8_t)0xE3;
      LCD->RAM[LCD_RAMRegister_4] |= (uint8_t)(digit[3] << (uint8_t)0x02);

      /*Write Digit 4 on COM6 */
      LCD->RAM[LCD_RAMRegister_7] &= (uint8_t)0x3F;
      LCD->RAM[LCD_RAMRegister_8] &= (uint8_t)0xFE;
      LCD->RAM[LCD_RAMRegister_7] |= (uint8_t)((digit[4] & (uint8_t)0x03) << (uint8_t)0x06);
      LCD->RAM[LCD_RAMRegister_8] |= (uint8_t)((digit[4] & (uint8_t)0x04) >> (uint8_t)0x02);

      /*Write Digit 5 on COM7 */
      LCD->RAM[LCD_RAMRegister_11] &= (uint8_t)0xE3;
      LCD->RAM[LCD_RAMRegister_11] |= (uint8_t)(digit[5] << (uint8_t)0x02);
      break;

    default:
      break;
  }
}

/**
  * @brief  This function Clear a character on the LCD Glass.
  * @param  position: position in the LCD of the character to Clear [0:6]
  * @retval None
  */
void LCD_GLASS_ClearChar(uint8_t Position)
{
  switch (Position)
  {
      /* Position 0 on LCD Glass*/
    case 0:
      /* Enable the write access on the LCD RAM First banck */
      LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;

      LCD->RAM[LCD_RAMRegister_1] &= (uint8_t)0xFB;
      LCD->RAM[LCD_RAMRegister_4] &= (uint8_t)0x3F;

      /* Enable the write access on the LCD RAM second banck */
      LCD->CR4 |= LCD_CR4_PAGECOM;

      LCD->RAM[LCD_RAMRegister_1] &= (uint8_t)0xF1;
      LCD->RAM[LCD_RAMRegister_4] &= (uint8_t)0x1F;
      LCD->RAM[LCD_RAMRegister_8] &= (uint8_t)0xF9;
      LCD->RAM[LCD_RAMRegister_11] &= (uint8_t)0x1F;
      break;

      /* Position 1 on LCD Glass*/
    case 1:
      /* Enable the write access on the LCD RAM First banck */
      LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;

      LCD->RAM[LCD_RAMRegister_1] &= (uint8_t)0x9F;
      LCD->RAM[LCD_RAMRegister_5] &= (uint8_t)0xF9;

      /* Enable the write access on the LCD RAM second banck */
      LCD->CR4 |= LCD_CR4_PAGECOM;

      LCD->RAM[LCD_RAMRegister_1] &= (uint8_t)0x8F;
      LCD->RAM[LCD_RAMRegister_5] &= (uint8_t)0xF8;
      LCD->RAM[LCD_RAMRegister_8] &= (uint8_t)0x8F;
      LCD->RAM[LCD_RAMRegister_12] &= (uint8_t)0xF8;

      break;

      /* Position 2 on LCD Glass*/
    case 2:
      /* Enable the write access on the LCD RAM First banck */
      LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;

      LCD->RAM[LCD_RAMRegister_1] &= (uint8_t)0xFF;
      LCD->RAM[LCD_RAMRegister_2] &= (uint8_t)0xFC;
      LCD->RAM[LCD_RAMRegister_5] &= (uint8_t)0xCF;

      /* Enable the write access on the LCD RAM second banck */
      LCD->CR4 |= LCD_CR4_PAGECOM;

      LCD->RAM[LCD_RAMRegister_1] &= (uint8_t)0x7F;
      LCD->RAM[LCD_RAMRegister_2] &= (uint8_t)0xFC;
      LCD->RAM[LCD_RAMRegister_5] &= (uint8_t)0xC7;
      LCD->RAM[LCD_RAMRegister_8] &= (uint8_t)0x7F;
      LCD->RAM[LCD_RAMRegister_9] &= (uint8_t)0xFC;
      LCD->RAM[LCD_RAMRegister_12] &= (uint8_t)0xC7;

      break;

      /* Position 3 on LCD Glass*/
    case 3:
      /* Enable the write access on the LCD RAM First banck */
      LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;

      LCD->RAM[LCD_RAMRegister_2] &= (uint8_t)0xE7;
      LCD->RAM[LCD_RAMRegister_5] &= (uint8_t)0x7F;
      LCD->RAM[LCD_RAMRegister_6] &= (uint8_t)0xFE;

      /* Enable the write access on the LCD RAM second banck */
      LCD->CR4 |= LCD_CR4_PAGECOM;

      LCD->RAM[LCD_RAMRegister_2] &= (uint8_t)0xE3;
      LCD->RAM[LCD_RAMRegister_5] &= (uint8_t)0x3F;
      LCD->RAM[LCD_RAMRegister_6] &= (uint8_t)0xFE;
      LCD->RAM[LCD_RAMRegister_9] &= (uint8_t)0xE3;
      LCD->RAM[LCD_RAMRegister_12] &= (uint8_t)0x3F;
      LCD->RAM[LCD_RAMRegister_13] &= (uint8_t)0xFE;

      break;

      /* Position 4 on LCD Glass*/
    case 4:
      /* Enable the write access on the LCD RAM First banck */
      LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;

      LCD->RAM[LCD_RAMRegister_0] &= (uint8_t)0xF9;
      LCD->RAM[LCD_RAMRegister_3] &= (uint8_t)0x9F;

      /* Enable the write access on the LCD RAM second banck */
      LCD->CR4 |= LCD_CR4_PAGECOM;

      LCD->RAM[LCD_RAMRegister_0] &= (uint8_t)0xF8;
      LCD->RAM[LCD_RAMRegister_3] &= (uint8_t)0x8F;
      LCD->RAM[LCD_RAMRegister_7] &= (uint8_t)0xF8;
      LCD->RAM[LCD_RAMRegister_10] &= (uint8_t)0x8F;

      break;

      /* Position 5 on LCD Glass*/
    case 5:
      /* Enable the write access on the LCD RAM First banck */
      LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;

      LCD->RAM[LCD_RAMRegister_0] &= (uint8_t)0xCF;
      LCD->RAM[LCD_RAMRegister_3] &= (uint8_t)0xFF;
      LCD->RAM[LCD_RAMRegister_4] &= (uint8_t)0xFC;

      /* Enable the write access on the LCD RAM second banck */
      LCD->CR4 |= LCD_CR4_PAGECOM;

      LCD->RAM[LCD_RAMRegister_0] &= (uint8_t)0xC7;
      LCD->RAM[LCD_RAMRegister_3] &= (uint8_t)0x7F;
      LCD->RAM[LCD_RAMRegister_4] &= (uint8_t)0xFC;
      LCD->RAM[LCD_RAMRegister_7] &= (uint8_t)0xC7;
      LCD->RAM[LCD_RAMRegister_10] &= (uint8_t)0x7F;
      LCD->RAM[LCD_RAMRegister_11] &= (uint8_t)0xFC;

      break;

      /* Position 6 on LCD Glass*/
    case 6:
      /* Enable the write access on the LCD RAM First banck */
      LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;

      LCD->RAM[LCD_RAMRegister_0] &= (uint8_t)0x7F;
      LCD->RAM[LCD_RAMRegister_1] &= (uint8_t)0xFE;
      LCD->RAM[LCD_RAMRegister_4] &= (uint8_t)0xE7;

      /* Enable the write access on the LCD RAM second banck */
      LCD->CR4 |= LCD_CR4_PAGECOM;

      LCD->RAM[LCD_RAMRegister_0] &= (uint8_t)0x3F;
      LCD->RAM[LCD_RAMRegister_1] &= (uint8_t)0xFE;
      LCD->RAM[LCD_RAMRegister_4] &= (uint8_t)0xE3;
      LCD->RAM[LCD_RAMRegister_7] &= (uint8_t)0x3F;
      LCD->RAM[LCD_RAMRegister_8] &= (uint8_t)0xFE;
      LCD->RAM[LCD_RAMRegister_11] &= (uint8_t)0xE3;

      break;

    default:
      break;
  }
}
/**
  * @brief  This function Clears the LCD Glass Text Zone.
  * @param  None
  * @retval None
  */
void LCD_GLASS_ClearTextZone(void)
{
  /* Enable the write access on the LCD RAM First banck */
  LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;

  LCD->RAM[LCD_RAMRegister_1] &= (uint8_t)0xFB;
  LCD->RAM[LCD_RAMRegister_4] &= (uint8_t)0x3F;
  LCD->RAM[LCD_RAMRegister_1] &= (uint8_t)0x9F;
  LCD->RAM[LCD_RAMRegister_5] &= (uint8_t)0xF9;
  LCD->RAM[LCD_RAMRegister_1] &= (uint8_t)0xFF;
  LCD->RAM[LCD_RAMRegister_2] &= (uint8_t)0xFC;
  LCD->RAM[LCD_RAMRegister_5] &= (uint8_t)0xCF;
  LCD->RAM[LCD_RAMRegister_2] &= (uint8_t)0xE7;
  LCD->RAM[LCD_RAMRegister_5] &= (uint8_t)0x7F;
  LCD->RAM[LCD_RAMRegister_6] &= (uint8_t)0xFE;
  LCD->RAM[LCD_RAMRegister_0] &= (uint8_t)0xF9;
  LCD->RAM[LCD_RAMRegister_3] &= (uint8_t)0x9F;
  LCD->RAM[LCD_RAMRegister_0] &= (uint8_t)0xCF;
  LCD->RAM[LCD_RAMRegister_3] &= (uint8_t)0xFF;
  LCD->RAM[LCD_RAMRegister_4] &= (uint8_t)0xFC;
  LCD->RAM[LCD_RAMRegister_0] &= (uint8_t)0x7F;
  LCD->RAM[LCD_RAMRegister_1] &= (uint8_t)0xFE;
  LCD->RAM[LCD_RAMRegister_4] &= (uint8_t)0xE7;

  /* Enable the write access on the LCD RAM second banck */
  LCD->CR4 |= LCD_CR4_PAGECOM;

  LCD->RAM[LCD_RAMRegister_1]  &= (uint8_t)0xF1;
  LCD->RAM[LCD_RAMRegister_4]  &= (uint8_t)0x1F;
  LCD->RAM[LCD_RAMRegister_8]  &= (uint8_t)0xF9;
  LCD->RAM[LCD_RAMRegister_11] &= (uint8_t)0x1F;
  LCD->RAM[LCD_RAMRegister_1]  &= (uint8_t)0x8F;
  LCD->RAM[LCD_RAMRegister_5]  &= (uint8_t)0xF8;
  LCD->RAM[LCD_RAMRegister_8]  &= (uint8_t)0x8F;
  LCD->RAM[LCD_RAMRegister_12] &= (uint8_t)0xF8;
  LCD->RAM[LCD_RAMRegister_1]  &= (uint8_t)0x7F;
  LCD->RAM[LCD_RAMRegister_2]  &= (uint8_t)0xFC;
  LCD->RAM[LCD_RAMRegister_5]  &= (uint8_t)0xC7;
  LCD->RAM[LCD_RAMRegister_8]  &= (uint8_t)0x7F;
  LCD->RAM[LCD_RAMRegister_9]  &= (uint8_t)0xFC;
  LCD->RAM[LCD_RAMRegister_12] &= (uint8_t)0xC7;
  LCD->RAM[LCD_RAMRegister_2]  &= (uint8_t)0xE3;
  LCD->RAM[LCD_RAMRegister_5]  &= (uint8_t)0x3F;
  LCD->RAM[LCD_RAMRegister_6]  &= (uint8_t)0xFE;
  LCD->RAM[LCD_RAMRegister_9]  &= (uint8_t)0xE3;
  LCD->RAM[LCD_RAMRegister_12] &= (uint8_t)0x3F;
  LCD->RAM[LCD_RAMRegister_13] &= (uint8_t)0xFE;
  LCD->RAM[LCD_RAMRegister_0]  &= (uint8_t)0xF8;
  LCD->RAM[LCD_RAMRegister_3]  &= (uint8_t)0x8F;
  LCD->RAM[LCD_RAMRegister_7]  &= (uint8_t)0xF8;
  LCD->RAM[LCD_RAMRegister_10] &= (uint8_t)0x8F;
  LCD->RAM[LCD_RAMRegister_0]  &= (uint8_t)0xC7;
  LCD->RAM[LCD_RAMRegister_3]  &= (uint8_t)0x7F;
  LCD->RAM[LCD_RAMRegister_4]  &= (uint8_t)0xFC;
  LCD->RAM[LCD_RAMRegister_7]  &= (uint8_t)0xC7;
  LCD->RAM[LCD_RAMRegister_10] &= (uint8_t)0x7F;
  LCD->RAM[LCD_RAMRegister_11] &= (uint8_t)0xFC;
  LCD->RAM[LCD_RAMRegister_0]  &= (uint8_t)0x3F;
  LCD->RAM[LCD_RAMRegister_1]  &= (uint8_t)0xFE;
  LCD->RAM[LCD_RAMRegister_4]  &= (uint8_t)0xE3;
  LCD->RAM[LCD_RAMRegister_7]  &= (uint8_t)0x3F;
  LCD->RAM[LCD_RAMRegister_8]  &= (uint8_t)0xFE;
  LCD->RAM[LCD_RAMRegister_11] &= (uint8_t)0xE3;

}
/**
  * @brief  This function Clears the LCD display memory.
  * @param  None
  * @retval None
  */
void LCD_GLASS_Clear(void)
{
  uint8_t counter = 0x00;

  /* Enable the write access on the LCD RAM First banck */
  LCD->CR4 &= (uint8_t)(~LCD_CR4_PAGECOM);

  for (counter = 0x0; counter < 0x16; counter++)
  {
    LCD->RAM[counter] =  LCD_RAM_RESET_VALUE;
  }

  /* Enable the write access on the LCD RAM second banck */
  LCD->CR4 |= LCD_CR4_PAGECOM;

  for (counter = 0x0; counter < 0x16; counter++)
  {
    LCD->RAM[counter] =  LCD_RAM_RESET_VALUE;
  }
}

/**
  * @brief  Writes a String on the LCD Glass.
  * @param  ptr: Pointer to the string to display on the LCD Glass.
  * @retval None
  */
void LCD_GLASS_DisplayString(uint8_t* ptr)
{
  uint8_t i = 0x00;

  /* Send the string character by character on lCD */
  while ((*ptr != 0) & (i < 8))
  {
    /* Display one character on LCD */
    LCD_GLASS_WriteChar(ptr, (Point_Typedef)0, (DoublePoint_Typedef)0, i);

    /* Point to the next character */
    ptr++;

    /* Increment the character counter */
    i++;
  }
}
/**
  * @brief  Display a string in scrolling mode
  * @param  ptr: Pointer to the string to display on the LCD Glass.
  * @param  nScroll: Specifies how many time the message will be scrolled
  * @param  ScrollSpeed : Specifies the speed of the scroll, lower value gives
  *         higher speed
  * @retval None
  */
void LCD_GLASS_ScrollString(uint8_t* ptr, uint32_t nScroll, uint32_t ScrollSpeed)
{
  uint8_t Position   = 0;
  uint8_t Repetition = 0;
  uint8_t* ptr1;

  ptr1 = ptr;

  LCD_GLASS_DisplayString(ptr1);

  delay(ScrollSpeed);

  for (Repetition = 0; Repetition < nScroll; Repetition++)
  {
    *(str + 1) = *ptr1;
    *(str + 2) = *(ptr1 + 1);
    *(str + 3) = *(ptr1 + 2);
    *(str + 4) = *(ptr1 + 3);
    *(str + 5) = *(ptr1 + 4);
    *(str + 6) = *(ptr1 + 5);
    *(str) = *(ptr1 + 6);
    for (Position = 1 ; Position <= 7;Position++)
    {
      LCD_GLASS_ClearChar( Position);
    }
    LCD_GLASS_DisplayString((uint8_t*)str);
    delay(ScrollSpeed);

    *(str + 1) = *(ptr1 + 6);
    *(str + 2) = *ptr1;
    *(str + 3) = *(ptr1 + 1);
    *(str + 4) = *(ptr1 + 2);
    *(str + 5) = *(ptr1 + 3);
    *(str + 6) = *(ptr1 + 4);
    *(str) = *(ptr1 + 5);
    for (Position = 1 ; Position <= 7;Position++)
    {
      LCD_GLASS_ClearChar( Position);
    }
    LCD_GLASS_DisplayString((uint8_t*)str);
    delay(ScrollSpeed);

    *(str + 1) = *(ptr1 + 5);
    *(str + 2) = *(ptr1 + 6);
    *(str + 3) = *ptr1;
    *(str + 4) = *(ptr1 + 1);
    *(str + 5) = *(ptr1 + 2);
    *(str + 6) = *(ptr1 + 3);
    *(str) = *(ptr1 + 4);
    for (Position = 1 ; Position <= 7;Position++)
    {
      LCD_GLASS_ClearChar( Position);
    }
    LCD_GLASS_DisplayString((uint8_t*)str);
    delay(ScrollSpeed);

    *(str + 1) = *(ptr1 + 4);
    *(str + 2) = *(ptr1 + 5);
    *(str + 3) = *(ptr1 + 6);
    *(str + 4) = *ptr1;
    *(str + 5) = *(ptr1 + 1);
    *(str + 6) = *(ptr1 + 2);
    *(str) = *(ptr1 + 3);
    for (Position = 1 ; Position <= 7;Position++)
    {
      LCD_GLASS_ClearChar( Position);
    }
    LCD_GLASS_DisplayString((uint8_t*)str);
    delay(ScrollSpeed);

    *(str + 1) = *(ptr1 + 3);
    *(str + 2) = *(ptr1 + 4);
    *(str + 3) = *(ptr1 + 5);
    *(str + 4) = *(ptr1 + 6);
    *(str + 5) = *ptr1;
    *(str + 6) = *(ptr1 + 1);
    *(str) = *(ptr1 + 2);
    for (Position = 1 ; Position <= 7;Position++)
    {
      LCD_GLASS_ClearChar( Position);
    }
    LCD_GLASS_DisplayString((uint8_t*)str);
    delay(ScrollSpeed);

    *(str + 1) = *(ptr1 + 2);
    *(str + 2) = *(ptr1 + 3);
    *(str + 3) = *(ptr1 + 4);
    *(str + 4) = *(ptr1 + 5);
    *(str + 5) = *(ptr1 + 6);
    *(str + 6) = *ptr1;
    *(str) = *(ptr1 + 1);
    for (Position = 1 ; Position <= 7;Position++)
    {
      LCD_GLASS_ClearChar( Position);
    }
    LCD_GLASS_DisplayString((uint8_t*)str);
    delay(ScrollSpeed);

    *(str + 1) = *(ptr1 + 1);
    *(str + 2) = *(ptr1 + 2);
    *(str + 3) = *(ptr1 + 3);
    *(str + 4) = *(ptr1 + 4);
    *(str + 5) = *(ptr1 + 5);
    *(str + 6) = *(ptr1 + 6);
    *(str) = *ptr1;
    for (Position = 1 ; Position <= 7;Position++)
    {
      LCD_GLASS_ClearChar( Position);
    }
    LCD_GLASS_DisplayString((uint8_t*)str);
    delay(ScrollSpeed);
  }
}

/**
  ** @brief Configure ST Logo display.
  * @param NewState: enable or disable the logo display .
  * @retval None
  */
void LCD_GLASS_DisplayLogo(FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
    /* Enable the write access on the LCD RAM First banck */
    LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;

    /* Set logo on  */
    LCD->RAM[LCD_RAMRegister_0] |= (uint8_t)0x40;
  }
  else
  {
    /* Enable the write access on the LCD RAM First banck */
    LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
    /* Set logo of  */
    LCD->RAM[LCD_RAMRegister_0] &= (uint8_t)0xBF;
  }
}

/**
  ** @brief Configure the LCD Battery Level.
  * @param BatteryLevel: Specify the Battery Level to set.
  * @retval None
  */
void LCD_GLASS_BatteryLevelConfig(BatteryLevel_TypeDef BatteryLevel)
{

  switch (BatteryLevel)
  {
      /* BATTERYLEVEL 1/4 */
    case BATTERYLEVEL_1_4:
      /* Enable the write access on the LCD RAM First banck */
      LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
      /* Set BATTERYLEVEL_1_4  on  */
      LCD->RAM[LCD_RAMRegister_0] |= (uint8_t)0x01;
      break;

      /* BATTERYLEVEL 1/2 )*/
    case BATTERYLEVEL_1_2:
      /* Enable the write access on the LCD RAM First banck */
      LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
      /* Set BatteryLevel_1_4 on  */
      LCD->RAM[LCD_RAMRegister_0] |= (uint8_t)0x01;
      /* Set BatteryLevel_1_2 on  */
      LCD->RAM[LCD_RAMRegister_3] |= (uint8_t)0x10;
      break;

      /* Battery Level 3/4*/
    case BATTERYLEVEL_3_4:
      /* Enable the write access on the LCD RAM First banck */
      LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
      /* Set BATTERYLEVEL_1_4  on  */
      LCD->RAM[LCD_RAMRegister_0] |= (uint8_t)0x01;
      /* Set BATTERYLEVEL_1_2  on  */
      LCD->RAM[LCD_RAMRegister_3] |= (uint8_t)0x10;
      /* Set BATTERYLEVEL_3_4  on  */
      LCD->RAM[LCD_RAMRegister_3] |= (uint8_t)0x80;
      break;

      /* BATTERYLEVEL_FULL*/
    case BATTERYLEVEL_FULL:
      /* Enable the write access on the LCD RAM First banck */
      LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
      /* Set BATTERYLEVEL_1_4 on  */
      LCD->RAM[LCD_RAMRegister_0] |= (uint8_t)0x01;
      /* Set BATTERYLEVEL_1_2 on  */
      LCD->RAM[LCD_RAMRegister_3] |= (uint8_t)0x10;
      /* Set BATTERYLEVEL_3_4 on  */
      LCD->RAM[LCD_RAMRegister_3] |= (uint8_t)0x80;
      /* Set BATTERYLEVEL_FULL on  */
      LCD->RAM[LCD_RAMRegister_0] |= (uint8_t)0x08;
      break;

    case BATTERYLEVEL_OFF:
      /* Enable the write access on the LCD RAM First banck */
      LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
      /* Set BATTERYLEVEL_1_4 off  */
      LCD->RAM[LCD_RAMRegister_0] &= (uint8_t)0xFE;
      /* Set BATTERYLEVEL_1_2 off  */
      LCD->RAM[LCD_RAMRegister_3] &= (uint8_t)0xEF;
      /* Set BATTERYLEVEL_3_4 off  */
      LCD->RAM[LCD_RAMRegister_3] &= (uint8_t)0x7F;
      /* Set BATTERYLEVEL_FULL off  */
      LCD->RAM[LCD_RAMRegister_0] &= (uint8_t)0xF7;
      break;

    default:
      break;
  }


}

/**
  ** @brief Configure the LCD ArrowDirection.
  * @param Arrow: Specify the ArrowDirection to set.
  * @retval None
  */
void LCD_GLASS_ArrowConfig(ArrowDirection_TypeDef ArrowDirection)
{

  switch (ArrowDirection)
  {
      /* ARROWDIRECTION_UP*/
    case ARROWDIRECTION_UP:
      /* Enable the write access on the LCD RAM First banck */
      LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
      /* Set ARROWDIRECTION_UP on  */
      LCD->RAM[LCD_RAMRegister_1] |= (uint8_t)0x10;
      break;

      /* ARROWDIRECTION_LEFT*/
    case ARROWDIRECTION_LEFT :
      /* Enable the write access on the LCD RAM First banck */
      LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
      /* Set ARROWDIRECTION_LEFT on  */
      LCD->RAM[LCD_RAMRegister_5] |= (uint8_t)0x01;
      break;

      /* ARROWDIRECTION_DOWN*/
    case ARROWDIRECTION_DOWN:
      /* Enable the write access on the LCD RAM First banck */
      LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
      /* Set ARROWDIRECTION_DOWN on  */
      LCD->RAM[LCD_RAMRegister_5] |= (uint8_t)0x08;
      break;

      /* ARROWDIRECTION_RIGHT*/
    case ARROWDIRECTION_RIGHT:
      /* Enable the write access on the LCD RAM First banck */
      LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
      /* Set ARROWDIRECTION_RIGHT on  */
      LCD->RAM[LCD_RAMRegister_1] |= (uint8_t)0x80;
      break;

    case ARROWDIRECTION_OFF:
      /* Enable the write access on the LCD RAM second banck */
      LCD->CR4 |= LCD_CR4_PAGECOM;
      /* Set ARROWDIRECTION_UP of  */
      LCD->RAM[LCD_RAMRegister_1] &= (uint8_t)0xEF;
      /* Set ARROWDIRECTION_LEFT of  */
      LCD->RAM[LCD_RAMRegister_5] &= (uint8_t)0xFE;
      /* Set ARROWDIRECTION_DOWN of  */
      LCD->RAM[LCD_RAMRegister_5] &= (uint8_t)0xF7;
      /* Set ARROWDIRECTION_RIGHT of  */
      LCD->RAM[LCD_RAMRegister_1] &= (uint8_t)0x7F;
      break;

    default:
      break;
  }


}

/**
  ** @brief Configure the LCD Temperature level display .
  * @param Temperature: indicate the Temperature level to set.
  * @param NewState: enable or desable the digit.
  * @retval None
  */
void LCD_GLASS_TemperatureConfig(TemperatureLevel_TypeDef Temperature)
{
  switch (Temperature)
  {
      /* Temp_6*/
    case TEMPERATURELEVEL_6 :
      /* Enable the write access on the LCD RAM First banck */
      LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
      /* Set Temperature_6 on  */
      LCD->RAM[LCD_RAMRegister_4] |= (uint8_t)0x20;
      break;

      /* Temperature_5)*/
    case TEMPERATURELEVEL_5:
      /* Enable the write access on the LCD RAM First banck */
      LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
      /* Set Temperature_6 on  */
      LCD->RAM[LCD_RAMRegister_4] |= (uint8_t)0x20;
      /* Set Temperature_5 on  */
      LCD->RAM[LCD_RAMRegister_1] |= (uint8_t)0x02;
      break;

      /* Temperature_4*/
    case TEMPERATURELEVEL_4:
      /* Enable the write access on the LCD RAM First banck */
      LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
      /* Set Temperature_6 on  */
      LCD->RAM[LCD_RAMRegister_4] |= (uint8_t)0x20;
      /* Set Temperature_5 on  */
      LCD->RAM[LCD_RAMRegister_1] |= (uint8_t)0x02;
      /* Set Temperature_4 on  */
      LCD->RAM[LCD_RAMRegister_8] |= (uint8_t)0x04;
      break;
      /* Temperature_3*/
    case TEMPERATURELEVEL_3:
      /* Enable the write access on the LCD RAM First banck */
      LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
      /* Set Temperature_6 on  */
      LCD->RAM[LCD_RAMRegister_4] |= (uint8_t)0x20;
      /* Set Temperature_5 on  */
      LCD->RAM[LCD_RAMRegister_1] |= (uint8_t)0x02;
      /* Set Temperature_4 on  */
      LCD->RAM[LCD_RAMRegister_8] |= (uint8_t)0x04;
      /* Set Temperature_3 on  */
      LCD->RAM[LCD_RAMRegister_11] |= (uint8_t)0x40;
      break;

      /* Temperature_2*/
    case TEMPERATURELEVEL_2:
      /* Enable the write access on the LCD RAM First banck */
      LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
      /* Set Temperature_6 on  */
      LCD->RAM[LCD_RAMRegister_4]  |= (uint8_t)0x20;
      /* Set Temperature_5 on  */
      LCD->RAM[LCD_RAMRegister_1]  |= (uint8_t)0x02;
      /* Set Temperature_4 on  */
      LCD->RAM[LCD_RAMRegister_11] |= (uint8_t)0x40;
      /* Set Temperature_3 on  */
      LCD->RAM[LCD_RAMRegister_8]  |= (uint8_t)0x04;
      /* Set Temperature_1 on  */
      LCD->RAM[LCD_RAMRegister_8]  |= (uint8_t)0x08;
      break;

      /* Temperature_1*/
    case  TEMPERATURELEVEL_1:
      /* Enable the write access on the LCD RAM First banck */
      LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
      /* Set Temperature_6 on  */
      LCD->RAM[LCD_RAMRegister_4] |= (uint8_t)0x20;
      /* Set Temperature_5 on  */
      LCD->RAM[LCD_RAMRegister_1] |= (uint8_t)0x02;
      /* Set Temperature_4 on  */
      LCD->RAM[LCD_RAMRegister_11] |= (uint8_t)0x40;
      /* Set Temperature_3 on  */
      LCD->RAM[LCD_RAMRegister_8] |= (uint8_t)0x04;
      /* Set Temperature_2 on  */
      LCD->RAM[LCD_RAMRegister_8] |= (uint8_t)0x08;
      /* Set Temperature_3 on  */
      LCD->RAM[LCD_RAMRegister_11] |= (uint8_t)0x80;
      break;


    case TEMPERATURELEVEL_OFF:
      /* Enable the write access on the LCD RAM First banck */
      LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
      /* Set Temperature_6 off  */
      LCD->RAM[LCD_RAMRegister_4] &= (uint8_t)0xDF;
      /* Set Temperature_5 off  */
      LCD->RAM[LCD_RAMRegister_1] &= (uint8_t)0xFD;
      /* Set Temperature_4 off  */
      LCD->RAM[LCD_RAMRegister_11] &= (uint8_t)0xBF;
      /* Set Temperature_3 off */
      LCD->RAM[LCD_RAMRegister_8] &= (uint8_t)0xFB;
      /* Set Temperature_2 off  */
      LCD->RAM[LCD_RAMRegister_11] &= (uint8_t)0x7F;
      /* Set Temperature_1 off  */
      LCD->RAM[LCD_RAMRegister_8] &= (uint8_t)0xF7;
      break;

    default:
      break;
  }


}

/**
  ** @brief Configure the LCD ValueUnit.
  * @param ValueUnite: indicate the ValueUnit to set.
  * @param NewState: enable or desable the digit.
  * @retval None
  */
void LCD_GLASS_ValueUnitConfig(ValueUnit_TypeDef ValueUnit)
{

  switch (ValueUnit)
  {
      /* VALUEUNIT MILLIAMPERE*/
    case VALUEUNIT_MILLIAMPERE:
      /* Enable the write access on the LCD RAM First banck */
      LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
      /* Set VALUEUNIT_MILLIAMPERE on  */
      LCD->RAM[LCD_RAMRegister_5] |= (uint8_t)0x40;
      break;

      /* VALUEUNIT MICROAMPERE)*/
    case VALUEUNIT_MICROAMPERE:
      /* Enable the write access on the LCD RAM First banck */
      LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
      /* Set VALUEUNIT_MICROAMPERE  */
      LCD->RAM[LCD_RAMRegister_2] |= (uint8_t)0x04;
      break;

      /* VALUEUNIT NANOAMPERE*/
    case  VALUEUNIT_NANOAMPERE:
      /* Enable the write access on the LCD RAM First banck */
      LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
      /* Set VALUEUNIT_NANOAMPERE on  */
      LCD->RAM[LCD_RAMRegister_4] |= (uint8_t)0x04;
      break;

    case  VALUEUNIT_OFF:
      /* Enable the write access on the LCD RAM First banck */
      LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
      /* Set VALUEUNIT_MILLIAMPERE off  */
      LCD->RAM[LCD_RAMRegister_5] &= (uint8_t)0xBF;
      /* Set VALUEUNIT_MICROAMPERE off  */
      LCD->RAM[LCD_RAMRegister_2] &= (uint8_t)0xFB;
      /* Set VALUEUNIT_NANOAMPERE off  */
      LCD->RAM[LCD_RAMRegister_4] &= (uint8_t)0xFB;
      break;

    default:
      break;
  }


}

/**
  * @brief Configure the LCD sign.
  * @param Signe: indicate the sign to set.
  * @param NewState: enable or disable the digit.
  * @retval None
  */
void LCD_GLASS_SignCmd(Sign_TypeDef Sign, FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
    switch (Sign)
    {
        /* Signe positive */
      case SIGN_POSITIVE:
        /* Enable the write access on the LCD RAM second banck */
        LCD->CR4 |= LCD_CR4_PAGECOM;
        /* Set SIGN_POSITIVE on  */
        LCD->RAM[LCD_RAMRegister_8] |= (uint8_t)0x08;
        break;

        /* Signe negative*/
      case SIGN_NEGATIVE:
        /* Enable the write access on the LCD RAM First banck */
        LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
        /* Set SIGN_NEGATIVE on  */
        LCD->RAM[LCD_RAMRegister_1] |= (uint8_t)0x08;
        break;

      default:
        break;
    }
  }
  else
  {
    /* Enable the write access on the LCD RAM second banck */
    LCD->CR4 |= LCD_CR4_PAGECOM;
    /* Set SIGN_POSITIVE off  */
    LCD->RAM[LCD_RAMRegister_8] &= (uint8_t)0xF7;
    /* Enable the write access on the LCD RAM second banck */
    LCD->CR4 |= LCD_CR4_PAGECOM;
    /* Set SICN_NEGATIVE off  */
    LCD->RAM[LCD_RAMRegister_1] &= (uint8_t)0xF7;
  }
}
/**
  * @brief Set Matrix Pixel on.
  * @param PixelRow: Specify Matrix Row.
  * @param PixelColumn: Specify Matrix Column.
  * @retval None
  */
void LCD_GLASS_WriteMatrixPixel(PixelRow_TypeDef PixelRow, PixelColumn_TypeDef PixelColumn)
{
  switch (PixelRow)
  {
    case PIXELROW_1:

      switch (PixelColumn)
      {
        case PIXELCOLUMN_1:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 1 , Column = 1 */
          LCD->RAM[LCD_RAMRegister_11] |= (uint8_t)0x20;
          break;

        case PIXELCOLUMN_2:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 1 , Column = 2 */
          LCD->RAM[LCD_RAMRegister_12] |= (uint8_t)0x04;
          break;

        case PIXELCOLUMN_3:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 1 , Column = 3 */
          LCD->RAM[LCD_RAMRegister_12] |= (uint8_t)0x02;
          break;

        case PIXELCOLUMN_4:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 1 , Column = 4 */
          LCD->RAM[LCD_RAMRegister_12] |= (uint8_t)0x01;
          break;

        case PIXELCOLUMN_5:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 1 , Column = 5 */
          LCD->RAM[LCD_RAMRegister_12] |= (uint8_t)0x20;
          break;

        case PIXELCOLUMN_6:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 1 , Column = 6 */
          LCD->RAM[LCD_RAMRegister_12] |= (uint8_t)0x10;
          break;

        case PIXELCOLUMN_7:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 1 , Column = 7 */
          LCD->RAM[LCD_RAMRegister_12] |= (uint8_t)0x08;
          break;

        case PIXELCOLUMN_8:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 1 , Column = 8 */
          LCD->RAM[LCD_RAMRegister_13] |= (uint8_t)0x01;
          break;

        case PIXELCOLUMN_9:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 1 , Column = 9 */
          LCD->RAM[LCD_RAMRegister_12] |= (uint8_t)0x80;
          break;

        case PIXELCOLUMN_10:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 1 , Column = 10 */
          LCD->RAM[LCD_RAMRegister_12] |= (uint8_t)0x40;
          break;

        case PIXELCOLUMN_11:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 1 , Column = 11 */
          LCD->RAM[LCD_RAMRegister_10] |= (uint8_t)0x40;
          break;

        case PIXELCOLUMN_12:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 1 , Column = 12 */
          LCD->RAM[LCD_RAMRegister_10] |= (uint8_t)0x20;
          break;

        case PIXELCOLUMN_13:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 1 , Column = 13 */
          LCD->RAM[LCD_RAMRegister_10] |= (uint8_t)0x10;
          break;

        case PIXELCOLUMN_14:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 1 , Column = 14 */
          LCD->RAM[LCD_RAMRegister_11] |= (uint8_t)0x02;
          break;

        case PIXELCOLUMN_15:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 1 , Column = 15 */
          LCD->RAM[LCD_RAMRegister_11] |= (uint8_t)0x01;
          break;

        case PIXELCOLUMN_16:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 1 , Column = 16 */
          LCD->RAM[LCD_RAMRegister_10] |= (uint8_t)0x80;
          break;

        case PIXELCOLUMN_17:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 1 , Column = 17 */
          LCD->RAM[LCD_RAMRegister_11] |= (uint8_t)0x10;
          break;

        case PIXELCOLUMN_18:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 1 , Column = 18 */
          LCD->RAM[LCD_RAMRegister_11] |= (uint8_t)0x08;
          break;

        case PIXELCOLUMN_19:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 1 , Column = 19 */
          LCD->RAM[LCD_RAMRegister_11] |= (uint8_t)0x04;
          break;

        default:
          break;

      }
      break;
    case PIXELROW_2:

      switch ( PixelColumn)
      {
        case  PIXELCOLUMN_1:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 2 , Column = 1 */
          LCD->RAM[LCD_RAMRegister_8] |= (uint8_t)0x02;
          break;

        case  PIXELCOLUMN_2:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 2 , Column = 2 */
          LCD->RAM[LCD_RAMRegister_8] |= (uint8_t)0x40;
          break;

        case  PIXELCOLUMN_3:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 2 , Column = 3 */
          LCD->RAM[LCD_RAMRegister_8] |= (uint8_t)0x20;
          break;

        case  PIXELCOLUMN_4:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row =  2 , Column = 4 */
          LCD->RAM[LCD_RAMRegister_8] |= (uint8_t)0x10;
          break;

        case  PIXELCOLUMN_5:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 2 , Column = 5 */
          LCD->RAM[LCD_RAMRegister_9] |= (uint8_t)0x02;
          break;

        case  PIXELCOLUMN_6:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 2 , Column = 6 */
          LCD->RAM[LCD_RAMRegister_9] |= (uint8_t)0x01;
          break;

        case  PIXELCOLUMN_7:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 2 , Column = 7 */
          LCD->RAM[LCD_RAMRegister_8] |= (uint8_t)0x80;
          break;

        case  PIXELCOLUMN_8:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 2 , Column = 8 */
          LCD->RAM[LCD_RAMRegister_9] |= (uint8_t)0x10;
          break;

        case  PIXELCOLUMN_9:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 2 , Column = 9*/
          LCD->RAM[LCD_RAMRegister_9] |= (uint8_t)0x08;
          break;

        case  PIXELCOLUMN_10:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 2 , Column = 10*/
          LCD->RAM[LCD_RAMRegister_9] |= (uint8_t)0x04;
          break;

        case  PIXELCOLUMN_11:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 2 , Column = 11 */
          LCD->RAM[LCD_RAMRegister_7] |= (uint8_t)0x04;
          break;

        case PIXELCOLUMN_12:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 2 , Column = 12 */
          LCD->RAM[LCD_RAMRegister_7] |= (uint8_t)0x02;
          break;

        case  PIXELCOLUMN_13:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 2 , Column = 13*/
          LCD->RAM[LCD_RAMRegister_7] |= (uint8_t)0x01;
          break;

        case  PIXELCOLUMN_14:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 2 , Column = 14*/
          LCD->RAM[LCD_RAMRegister_7] |= (uint8_t)0x20;
          break;

        case  PIXELCOLUMN_15:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 2 , Column = 15 */
          LCD->RAM[LCD_RAMRegister_7] |= (uint8_t)0x10;
          break;

        case  PIXELCOLUMN_16:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 2 , Column = 16*/
          LCD->RAM[LCD_RAMRegister_7] |= (uint8_t)0x08;
          break;

        case  PIXELCOLUMN_17:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 2 , Column = 17*/
          LCD->RAM[LCD_RAMRegister_8] |= (uint8_t)0x01;
          break;

        case  PIXELCOLUMN_18:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 2 , Column = 18 */
          LCD->RAM[LCD_RAMRegister_7] |= (uint8_t)0x80;
          break;

        case  PIXELCOLUMN_19:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 2 , Column = 19 */
          LCD->RAM[LCD_RAMRegister_7] |= (uint8_t)0x40;
          break;

        default:
          break;
      }
      break;

    case PIXELROW_3:
      switch ( PixelColumn)
      {
        case PIXELCOLUMN_1:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 3 , Column = 1 */
          LCD->RAM[LCD_RAMRegister_9] |= (uint8_t)0x20;
          break;

        case PIXELCOLUMN_2:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 3 , Column = 2 */
          LCD->RAM[LCD_RAMRegister_9] |= (uint8_t)0x40;
          break;

        case PIXELCOLUMN_3:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 3 , Column = 3 */
          LCD->RAM[LCD_RAMRegister_9] |= (uint8_t)0x80;
          break;

        case PIXELCOLUMN_4:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 3 , Column = 4 */
          LCD->RAM[LCD_RAMRegister_10] |= (uint8_t)0x01;
          break;

        case PIXELCOLUMN_5:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 3 , Column = 5 */
          LCD->RAM[LCD_RAMRegister_10] |= (uint8_t)0x02;
          break;

        case PIXELCOLUMN_6:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 3 , Column = 6 */
          LCD->RAM[LCD_RAMRegister_10] |= (uint8_t)0x04;
          break;

        case PIXELCOLUMN_7:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 3 , Column = 7 */
          LCD->RAM[LCD_RAMRegister_10] |= (uint8_t)0x08;
          break;

        case PIXELCOLUMN_8:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 3 , Column = 8 */
          LCD->RAM[LCD_RAMRegister_18] |= (uint8_t)0x01;
          break;

        case PIXELCOLUMN_9:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 3 , Column = 9*/
          LCD->RAM[LCD_RAMRegister_18] |= (uint8_t)0x02;
          break;

        case PIXELCOLUMN_10:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 3 , Column = 10*/
          LCD->RAM[LCD_RAMRegister_18] |= (uint8_t)0x04;
          break;

        case PIXELCOLUMN_11:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 3 , Column = 11*/
          LCD->RAM[LCD_RAMRegister_18] |= (uint8_t)0x08;
          break;

        case PIXELCOLUMN_12:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 3 , Column = 12*/
          LCD->RAM[LCD_RAMRegister_18] |= (uint8_t)0x10;
          break;

        case PIXELCOLUMN_13:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 3 , Column = 13*/
          LCD->RAM[LCD_RAMRegister_18] |= (uint8_t)0x20;
          break;

        case PIXELCOLUMN_14:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 3 , Column = 14*/
          LCD->RAM[LCD_RAMRegister_18] |= (uint8_t)0x40;
          break;

        case PIXELCOLUMN_15:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 3 , Column = 15*/
          LCD->RAM[LCD_RAMRegister_18] |= (uint8_t)0x80;
          break;

        case PIXELCOLUMN_16:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 3 , Column = 16*/
          LCD->RAM[LCD_RAMRegister_19] |= (uint8_t)0x01;
          break;

        case PIXELCOLUMN_17:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 3 , Column = 17*/
          LCD->RAM[LCD_RAMRegister_19] |= (uint8_t)0x02;
          break;

        case PIXELCOLUMN_18:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 3 , Column = 18*/
          LCD->RAM[LCD_RAMRegister_19] |= (uint8_t)0x04;
          break;

        case PIXELCOLUMN_19:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 3 , Column = 19 */
          LCD->RAM[LCD_RAMRegister_19] |= (uint8_t)0x08;
          break;

        default:
          break;
      }
      break;
    case PIXELROW_4:
      switch ( PixelColumn)
      {
        case PIXELCOLUMN_1:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 1 , Column = 1 */
          LCD->RAM[LCD_RAMRegister_13] |= (uint8_t)0x02;
          break;

        case PIXELCOLUMN_2:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 1 , Column = 1 */
          LCD->RAM[LCD_RAMRegister_13] |= (uint8_t)0x04;
          break;

        case PIXELCOLUMN_3:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 1 , Column = 1 */
          LCD->RAM[LCD_RAMRegister_13] |= (uint8_t)0x08;
          break;

        case PIXELCOLUMN_4:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 1 , Column = 1 */
          LCD->RAM[LCD_RAMRegister_13] |= (uint8_t)0x10;
          break;

        case PIXELCOLUMN_5:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 1 , Column = 1 */
          LCD->RAM[LCD_RAMRegister_13] |= (uint8_t)0x20;
          break;

        case PIXELCOLUMN_6:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 1 , Column = 1 */
          LCD->RAM[LCD_RAMRegister_13] |= (uint8_t)0x40;
          break;

        case PIXELCOLUMN_7:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 1 , Column = 1 */
          LCD->RAM[LCD_RAMRegister_13] |= (uint8_t)0x80;
          break;

        case PIXELCOLUMN_8:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 1 , Column = 1 */
          LCD->RAM[LCD_RAMRegister_20] |= (uint8_t)0x01;
          break;

        case PIXELCOLUMN_9:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 1 , Column = 1 */
          LCD->RAM[LCD_RAMRegister_20] |= (uint8_t)0x02;
          break;

        case PIXELCOLUMN_10:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 1 , Column = 1 */
          LCD->RAM[LCD_RAMRegister_20] |= (uint8_t)0x04;
          break;

        case PIXELCOLUMN_11:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 1 , Column = 1 */
          LCD->RAM[LCD_RAMRegister_20] |= (uint8_t)0x08;
          break;

        case PIXELCOLUMN_12:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 1 , Column = 1 */
          LCD->RAM[LCD_RAMRegister_20] |= (uint8_t)0x10;
          break;

        case PIXELCOLUMN_13:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 1 , Column = 1 */
          LCD->RAM[LCD_RAMRegister_20] |= (uint8_t)0x20;
          break;

        case PIXELCOLUMN_14:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 1 , Column = 1 */
          LCD->RAM[LCD_RAMRegister_20] |= (uint8_t)0x40;
          break;

        case PIXELCOLUMN_15:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 1 , Column = 1 */
          LCD->RAM[LCD_RAMRegister_20] |= (uint8_t)0x80;
          break;

        case PIXELCOLUMN_16:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 1 , Column = 1 */
          LCD->RAM[LCD_RAMRegister_21] |= (uint8_t)0x01;
          break;

        case PIXELCOLUMN_17:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 1 , Column = 1 */
          LCD->RAM[LCD_RAMRegister_21] |= (uint8_t)0x02;
          break;

        case PIXELCOLUMN_18:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 1 , Column = 1 */
          LCD->RAM[LCD_RAMRegister_21] |= (uint8_t)0x04;
          break;

        case PIXELCOLUMN_19:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 1 , Column = 1 */
          LCD->RAM[LCD_RAMRegister_21] |= (uint8_t)0x08;
          break;

        default:
          break;

      }
      break;
    case PIXELROW_5:
      switch ( PixelColumn)
      {
        case PIXELCOLUMN_1:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 5 , Column = 1 */
          LCD->RAM[LCD_RAMRegister_2] |= (uint8_t)0x20;
          break;

        case PIXELCOLUMN_2:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 5 , Column = 2 */
          LCD->RAM[LCD_RAMRegister_2] |= (uint8_t)0x40;
          break;

        case PIXELCOLUMN_3:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 5 , Column = 3 */
          LCD->RAM[LCD_RAMRegister_2] |= (uint8_t)0x80;
          break;

        case PIXELCOLUMN_4:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 5 , Column = 4 */
          LCD->RAM[LCD_RAMRegister_3] |= (uint8_t)0x01;
          break;

        case PIXELCOLUMN_5:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 5 , Column = 5 */
          LCD->RAM[LCD_RAMRegister_3] |= (uint8_t)0x02;
          break;

        case PIXELCOLUMN_6:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 5 , Column = 6 */
          LCD->RAM[LCD_RAMRegister_3] |= (uint8_t)0x04;
          break;

        case PIXELCOLUMN_7:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 5 , Column = 7 */
          LCD->RAM[LCD_RAMRegister_3] |= (uint8_t)0x08;
          break;

        case PIXELCOLUMN_8:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 5 , Column = 8 */
          LCD->RAM[LCD_RAMRegister_14] |= (uint8_t)0x01;
          break;

        case PIXELCOLUMN_9:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 5 , Column = 9*/
          LCD->RAM[LCD_RAMRegister_14] |= (uint8_t)0x02;
          break;

        case PIXELCOLUMN_10:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 5 , Column = 10*/
          LCD->RAM[LCD_RAMRegister_14] |= (uint8_t)0x04;
          break;

        case PIXELCOLUMN_11:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 5 , Column = 11*/
          LCD->RAM[LCD_RAMRegister_14] |= (uint8_t)0x08;
          break;

        case PIXELCOLUMN_12:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 5 , Column = 12*/
          LCD->RAM[LCD_RAMRegister_14] |= (uint8_t)0x10;
          break;

        case PIXELCOLUMN_13:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 5 , Column = 13*/
          LCD->RAM[LCD_RAMRegister_14] |= (uint8_t)0x20;
          break;

        case PIXELCOLUMN_14:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 5 , Column = 14*/
          LCD->RAM[LCD_RAMRegister_14] |= (uint8_t)0x40;
          break;

        case PIXELCOLUMN_15:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 5 , Column = 15*/
          LCD->RAM[LCD_RAMRegister_14] |= (uint8_t)0x80;
          break;

        case PIXELCOLUMN_16:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 5 , Column = 16*/
          LCD->RAM[LCD_RAMRegister_15] |= (uint8_t)0x01;
          break;

        case PIXELCOLUMN_17:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 5 , Column = 17*/
          LCD->RAM[LCD_RAMRegister_15] |= (uint8_t)0x02;
          break;

        case PIXELCOLUMN_18:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 5 , Column = 18*/
          LCD->RAM[LCD_RAMRegister_15] |= (uint8_t)0x04;
          break;

        case PIXELCOLUMN_19:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 5 , Column = 19 */
          LCD->RAM[LCD_RAMRegister_15] |= (uint8_t)0x08;
          break;

        default:
          break;
      }
      break;
    case PIXELROW_6:
      switch ( PixelColumn)
      {
        case PIXELCOLUMN_1:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 6 , Column = 1 */
          LCD->RAM[LCD_RAMRegister_6] |= (uint8_t)0x02;
          break;

        case PIXELCOLUMN_2:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 6 , Column = 2 */
          LCD->RAM[LCD_RAMRegister_6] |= (uint8_t)0x04;
          break;

        case PIXELCOLUMN_3:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 6 , Column = 3 */
          LCD->RAM[LCD_RAMRegister_6] |= (uint8_t)0x08;
          break;

        case PIXELCOLUMN_4:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 6 , Column = 4 */
          LCD->RAM[LCD_RAMRegister_6] |= (uint8_t)0x10;
          break;

        case PIXELCOLUMN_5:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 6 , Column = 5 */
          LCD->RAM[LCD_RAMRegister_6] |= (uint8_t)0x20;
          break;

        case PIXELCOLUMN_6:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 6 , Column = 6*/
          LCD->RAM[LCD_RAMRegister_6] |= (uint8_t)0x40;
          break;

        case PIXELCOLUMN_7:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 6 , Column = 7 */
          LCD->RAM[LCD_RAMRegister_6] |= (uint8_t)0x80;
          break;

        case PIXELCOLUMN_8:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 6 , Column = 8 */
          LCD->RAM[LCD_RAMRegister_16] |= (uint8_t)0x01;
          break;

        case PIXELCOLUMN_9:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 6 , Column = 9*/
          LCD->RAM[LCD_RAMRegister_16] |= (uint8_t)0x02;
          break;

        case PIXELCOLUMN_10:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 6 , Column = 10*/
          LCD->RAM[LCD_RAMRegister_16] |= (uint8_t)0x04;
          break;

        case PIXELCOLUMN_11:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 6 , Column = 11*/
          LCD->RAM[LCD_RAMRegister_16] |= (uint8_t)0x08;
          break;

        case PIXELCOLUMN_12:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 6 , Column = 12*/
          LCD->RAM[LCD_RAMRegister_16] |= (uint8_t)0x10;
          break;

        case PIXELCOLUMN_13:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 6 , Column = 13*/
          LCD->RAM[LCD_RAMRegister_16] |= (uint8_t)0x20;
          break;

        case PIXELCOLUMN_14:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 6 , Column = 14*/
          LCD->RAM[LCD_RAMRegister_16] |= (uint8_t)0x40;
          break;

        case PIXELCOLUMN_15:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 6 , Column = 15*/
          LCD->RAM[LCD_RAMRegister_16] |= (uint8_t)0x80;
          break;

        case PIXELCOLUMN_16:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 6 , Column = 16*/
          LCD->RAM[LCD_RAMRegister_17] |= (uint8_t)0x01;
          break;

        case PIXELCOLUMN_17:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 6 , Column = 17*/
          LCD->RAM[LCD_RAMRegister_17] |= (uint8_t)0x02;
          break;


        case PIXELCOLUMN_18:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 6 , Column = 18*/
          LCD->RAM[LCD_RAMRegister_17] |= (uint8_t)0x04;
          break;

        case PIXELCOLUMN_19:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 1 , Column = 19*/
          LCD->RAM[LCD_RAMRegister_17] |= (uint8_t)0x08;
          break;

        default:
          break;
      }
      break;
    case PIXELROW_7:
      switch ( PixelColumn)
      {
        case PIXELCOLUMN_1:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 7 , Column = 1 */
          LCD->RAM[LCD_RAMRegister_13] |= (uint8_t)0x02;
          break;

        case PIXELCOLUMN_2:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 7 , Column = 2 */
          LCD->RAM[LCD_RAMRegister_13] |= (uint8_t)0x04;
          break;

        case PIXELCOLUMN_3:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 7 , Column = 3 */
          LCD->RAM[LCD_RAMRegister_13] |= (uint8_t)0x08;
          break;

        case PIXELCOLUMN_4:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 7 , Column = 4 */
          LCD->RAM[LCD_RAMRegister_13] |= (uint8_t)0x10;
          break;

        case PIXELCOLUMN_5:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 7 , Column = 5 */
          LCD->RAM[LCD_RAMRegister_13] |= (uint8_t)0x20;
          break;

        case PIXELCOLUMN_6:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 7 , Column = 6 */
          LCD->RAM[LCD_RAMRegister_13] |= (uint8_t)0x40;
          break;

        case PIXELCOLUMN_7:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 7 , Column = 7 */
          LCD->RAM[LCD_RAMRegister_13] |= (uint8_t)0x80;
          break;

        case PIXELCOLUMN_8:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 7 , Column = 8 */
          LCD->RAM[LCD_RAMRegister_20] |= (uint8_t)0x01;
          break;

        case PIXELCOLUMN_9:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 7 , Column = 9*/
          LCD->RAM[LCD_RAMRegister_20] |= (uint8_t)0x02;
          break;

        case PIXELCOLUMN_10:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 7 , Column = 10*/
          LCD->RAM[LCD_RAMRegister_20] |= (uint8_t)0x04;
          break;

        case PIXELCOLUMN_11:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 7 , Column = 11*/
          LCD->RAM[LCD_RAMRegister_20] |= (uint8_t)0x08;
          break;

        case PIXELCOLUMN_12:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 7 , Column = 12*/
          LCD->RAM[LCD_RAMRegister_20] |= (uint8_t)0x10;
          break;

        case PIXELCOLUMN_13:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 7 , Column = 13*/
          LCD->RAM[LCD_RAMRegister_20] |= (uint8_t)0x20;
          break;

        case PIXELCOLUMN_14:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 7 , Column = 14*/
          LCD->RAM[LCD_RAMRegister_20] |= (uint8_t)0x40;
          break;

        case PIXELCOLUMN_15:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 7 , Column = 15*/
          LCD->RAM[LCD_RAMRegister_20] |= (uint8_t)0x80;
          break;

        case PIXELCOLUMN_16:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 7 , Column = 16*/
          LCD->RAM[LCD_RAMRegister_21] |= (uint8_t)0x01;
          break;

        case PIXELCOLUMN_17:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 7 , Column = 17*/
          LCD->RAM[LCD_RAMRegister_21] |= (uint8_t)0x02;
          break;

        case PIXELCOLUMN_18:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 7 , Column = 18*/
          LCD->RAM[LCD_RAMRegister_21] |= (uint8_t)0x04;
          break;

        case PIXELCOLUMN_19:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 7 , Column = 19 */
          LCD->RAM[LCD_RAMRegister_21] |= (uint8_t)0x08;
          break;

        default:
          break;
      }
      break;
    case PIXELROW_8:
      switch ( PixelColumn)
      {
        case PIXELCOLUMN_1:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 8 , Column = 1 */
          LCD->RAM[LCD_RAMRegister_9] |= (uint8_t)0x20;
          break;

        case PIXELCOLUMN_2:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 8 , Column = 2 */
          LCD->RAM[LCD_RAMRegister_9] |= (uint8_t)0x40;
          break;

        case PIXELCOLUMN_3:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 8 , Column = 3 */
          LCD->RAM[LCD_RAMRegister_9] |= (uint8_t)0x80;
          break;

        case PIXELCOLUMN_4:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 8 , Column = 4 */
          LCD->RAM[LCD_RAMRegister_10] |= (uint8_t)0x01;
          break;

        case PIXELCOLUMN_5:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 8 , Column = 5 */
          LCD->RAM[LCD_RAMRegister_10] |= (uint8_t)0x02;
          break;

        case PIXELCOLUMN_6:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 8 , Column = 6 */
          LCD->RAM[LCD_RAMRegister_10] |= (uint8_t)0x04;
          break;

        case PIXELCOLUMN_7:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 8 , Column = 7 */
          LCD->RAM[LCD_RAMRegister_10] |= (uint8_t)0x08;
          break;

        case PIXELCOLUMN_8:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 8 , Column = 8 */
          LCD->RAM[LCD_RAMRegister_18] |= (uint8_t)0x01;
          break;

        case PIXELCOLUMN_9:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 8 , Column = 9*/
          LCD->RAM[LCD_RAMRegister_18] |= (uint8_t)0x02;
          break;

        case PIXELCOLUMN_10:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 8 , Column = 10*/
          LCD->RAM[LCD_RAMRegister_18] |= (uint8_t)0x04;
          break;

        case PIXELCOLUMN_11:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 8 , Column = 11*/
          LCD->RAM[LCD_RAMRegister_18] |= (uint8_t)0x08;
          break;

        case PIXELCOLUMN_12:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 8 , Column = 12*/
          LCD->RAM[LCD_RAMRegister_18] |= (uint8_t)0x10;
          break;

        case PIXELCOLUMN_13:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 8 , Column = 13*/
          LCD->RAM[LCD_RAMRegister_18] |= (uint8_t)0x20;
          break;

        case PIXELCOLUMN_14:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 8 , Column = 14*/
          LCD->RAM[LCD_RAMRegister_18] |= (uint8_t)0x40;
          break;

        case PIXELCOLUMN_15:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 8 , Column = 15*/
          LCD->RAM[LCD_RAMRegister_18] |= (uint8_t)0x80;
          break;

        case PIXELCOLUMN_16:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 8 , Column = 16 */
          LCD->RAM[LCD_RAMRegister_19] |= (uint8_t)0x01;
          break;

        case PIXELCOLUMN_17:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 8 , Column = 17 */
          LCD->RAM[LCD_RAMRegister_19] |= (uint8_t)0x02;
          break;

        case PIXELCOLUMN_18:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 8 , Column = 18*/
          LCD->RAM[LCD_RAMRegister_19] |= (uint8_t)0x04;
          break;

        case PIXELCOLUMN_19:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 8 , Column = 19 */
          LCD->RAM[LCD_RAMRegister_19] |= (uint8_t)0x08;
          break;

        default:
          break;
      }
      break;
    case PIXELROW_9:
      switch ( PixelColumn)
      {
        case PIXELCOLUMN_1:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 9 , Column = 1 */
          LCD->RAM[LCD_RAMRegister_6] |= (uint8_t)0x02;
          break;

        case PIXELCOLUMN_2:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 9 , Column = 2 */
          LCD->RAM[LCD_RAMRegister_6] |= (uint8_t)0x04;
          break;

        case PIXELCOLUMN_3:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 9 , Column = 3 */
          LCD->RAM[LCD_RAMRegister_6] |= (uint8_t)0x08;
          break;

        case PIXELCOLUMN_4:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 9 , Column = 4 */
          LCD->RAM[LCD_RAMRegister_6] |= (uint8_t)0x10;
          break;

        case PIXELCOLUMN_5:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 9 , Column = 5 */
          LCD->RAM[LCD_RAMRegister_6] |= (uint8_t)0x20;
          break;

        case PIXELCOLUMN_6:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 9 , Column = 6 */
          LCD->RAM[LCD_RAMRegister_6] |= (uint8_t)0x40;
          break;

        case PIXELCOLUMN_7:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 9 , Column = 7 */
          LCD->RAM[LCD_RAMRegister_6] |= (uint8_t)0x80;
          break;

        case PIXELCOLUMN_8:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 9 , Column = 8 */
          LCD->RAM[LCD_RAMRegister_16] |= (uint8_t)0x01;
          break;

        case PIXELCOLUMN_9:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 9 , Column = 9*/
          LCD->RAM[LCD_RAMRegister_16] |= (uint8_t)0x02;
          break;

        case PIXELCOLUMN_10:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 9 , Column = 10*/
          LCD->RAM[LCD_RAMRegister_16] |= (uint8_t)0x04;
          break;

        case PIXELCOLUMN_11:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 9 , Column = 11*/
          LCD->RAM[LCD_RAMRegister_16] |= (uint8_t)0x08;
          break;

        case PIXELCOLUMN_12:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 9 , Column = 12*/
          LCD->RAM[LCD_RAMRegister_16] |= (uint8_t)0x10;
          break;

        case PIXELCOLUMN_13:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 9 , Column = 13*/
          LCD->RAM[LCD_RAMRegister_16] |= (uint8_t)0x20;
          break;

        case PIXELCOLUMN_14:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 9 , Column = 14*/
          LCD->RAM[LCD_RAMRegister_16] |= (uint8_t)0x40;
          break;

        case PIXELCOLUMN_15:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 9 , Column = 15*/
          LCD->RAM[LCD_RAMRegister_16] |= (uint8_t)0x80;
          break;

        case PIXELCOLUMN_16:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 9 , Column = 16*/
          LCD->RAM[LCD_RAMRegister_17] |= (uint8_t)0x01;
          break;

        case PIXELCOLUMN_17:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 9 , Column = 17*/
          LCD->RAM[LCD_RAMRegister_17] |= (uint8_t)0x02;
          break;

        case PIXELCOLUMN_18:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 9 , Column = 18*/
          LCD->RAM[LCD_RAMRegister_17] |= (uint8_t)0x04;
          break;

        case PIXELCOLUMN_19:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 9 , Column = 19 */
          LCD->RAM[LCD_RAMRegister_17] |= (uint8_t)0x08;
          break;

        default:
          break;
      }
      break;
    case PIXELROW_10:
      switch ( PixelColumn)
      {
        case PIXELCOLUMN_1:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 10 , Column = 1 */
          LCD->RAM[LCD_RAMRegister_2] |= (uint8_t)0x20;
          break;

        case PIXELCOLUMN_2:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 10, Column = 2 */
          LCD->RAM[LCD_RAMRegister_2] |= (uint8_t)0x40;
          break;

        case PIXELCOLUMN_3:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 10, Column = 3 */
          LCD->RAM[LCD_RAMRegister_2] |= (uint8_t)0x80;
          break;

        case PIXELCOLUMN_4:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 10, Column = 4 */
          LCD->RAM[LCD_RAMRegister_3] |= (uint8_t)0x01;
          break;

        case PIXELCOLUMN_5:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 10, Column = 5 */
          LCD->RAM[LCD_RAMRegister_3] |= (uint8_t)0x02;
          break;

        case PIXELCOLUMN_6:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 10, Column = 6 */
          LCD->RAM[LCD_RAMRegister_3] |= (uint8_t)0x04;
          break;

        case PIXELCOLUMN_7:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 10, Column = 7 */
          LCD->RAM[LCD_RAMRegister_3] |= (uint8_t)0x08;
          break;

        case PIXELCOLUMN_8:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 10, Column = 8 */
          LCD->RAM[LCD_RAMRegister_14] |= (uint8_t)0x01;
          break;

        case PIXELCOLUMN_9:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 10, Column = 9*/
          LCD->RAM[LCD_RAMRegister_14] |= (uint8_t)0x02;
          break;

        case PIXELCOLUMN_10:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 10, Column = 10*/
          LCD->RAM[LCD_RAMRegister_14] |= (uint8_t)0x04;
          break;

        case PIXELCOLUMN_11:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 10, Column = 11*/
          LCD->RAM[LCD_RAMRegister_14] |= (uint8_t)0x08;
          break;

        case PIXELCOLUMN_12:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 10, Column = 12*/
          LCD->RAM[LCD_RAMRegister_14] |= (uint8_t)0x10;
          break;

        case PIXELCOLUMN_13:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 10, Column = 13*/
          LCD->RAM[LCD_RAMRegister_14] |= (uint8_t)0x20;
          break;

        case PIXELCOLUMN_14:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 10, Column = 14*/
          LCD->RAM[LCD_RAMRegister_14] |= (uint8_t)0x40;
          break;

        case PIXELCOLUMN_15:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 10, Column = 15*/
          LCD->RAM[LCD_RAMRegister_14] |= (uint8_t)0x80;
          break;

        case PIXELCOLUMN_16:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 10, Column = 16 */
          LCD->RAM[LCD_RAMRegister_15] |= (uint8_t)0x01;
          break;

        case PIXELCOLUMN_17:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 10, Column = 17*/
          LCD->RAM[LCD_RAMRegister_15] |= (uint8_t)0x02;
          break;

        case PIXELCOLUMN_18:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 10, Column = 18*/
          LCD->RAM[LCD_RAMRegister_15] |= (uint8_t)0x04;
          break;

        case PIXELCOLUMN_19:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 10, Column = 19 */
          LCD->RAM[LCD_RAMRegister_15] |= (uint8_t)0x08;
          break;

        default:
          break;
      }
      break;
  }
}

/**
  * @brief Set Matrix Pixel off.
  * @param PixelRow: Specify Matrix Row.
  * @param PixelColumn: Specify Matrix Column.
  * @retval None
  */
void LCD_GLASS_ClearMatrixPixel(PixelRow_TypeDef PIXELROW, PixelColumn_TypeDef PIXELCOLUMN)
{
  switch (PIXELROW)
  {
    case PIXELROW_1:
      switch (PIXELCOLUMN)
      {
        case PIXELCOLUMN_1:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 1, Column = 1 */
          LCD->RAM[LCD_RAMRegister_11] &= (uint8_t)0xDF;
          break;

        case PIXELCOLUMN_2:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 1, Column = 2 */
          LCD->RAM[LCD_RAMRegister_12] &= (uint8_t)0xFB;
          break;

        case PIXELCOLUMN_3:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 1, Column = 3 */
          LCD->RAM[LCD_RAMRegister_12] &= (uint8_t)0xFD;
          break;

        case PIXELCOLUMN_4:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 1, Column = 4 */
          LCD->RAM[LCD_RAMRegister_12] &= (uint8_t)0xFE;
          break;

        case PIXELCOLUMN_5:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 1, Column = 5 */
          LCD->RAM[LCD_RAMRegister_12] &= (uint8_t)0xDF;
          break;

        case PIXELCOLUMN_6:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 1, Column = 6 */
          LCD->RAM[LCD_RAMRegister_12] &= (uint8_t)0xEF;
          break;

        case PIXELCOLUMN_7:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 1, Column = 7 */
          LCD->RAM[LCD_RAMRegister_12] &= (uint8_t)0xF7;
          break;

        case PIXELCOLUMN_8:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 1, Column = 8 */
          LCD->RAM[LCD_RAMRegister_13] &= (uint8_t)0xFE;
          break;

        case PIXELCOLUMN_9:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 1, Column = 9 */
          LCD->RAM[LCD_RAMRegister_12] &= (uint8_t)0x7F;
          break;

        case PIXELCOLUMN_10:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 1, Column = 10 */
          LCD->RAM[LCD_RAMRegister_12] &= (uint8_t)0xBF;
          break;

        case PIXELCOLUMN_11:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 1, Column = 11 */
          LCD->RAM[LCD_RAMRegister_10] &= (uint8_t)0xBF;
          break;

        case PIXELCOLUMN_12:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 1, Column = 12 */
          LCD->RAM[LCD_RAMRegister_10] &= (uint8_t)0xDF;
          break;

        case PIXELCOLUMN_13:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 1, Column = 13 */
          LCD->RAM[LCD_RAMRegister_10] &= (uint8_t)0xEF;
          break;

        case PIXELCOLUMN_14:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 1, Column = 14 */
          LCD->RAM[LCD_RAMRegister_11] &= (uint8_t)0xFD;
          break;

        case PIXELCOLUMN_15:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 1, Column = 15*/
          LCD->RAM[LCD_RAMRegister_11] &= (uint8_t)0xFE;
          break;

        case PIXELCOLUMN_16:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 1, Column = 16 */
          LCD->RAM[LCD_RAMRegister_10] &= (uint8_t)0x7F;
          break;

        case PIXELCOLUMN_17:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 1, Column = 17 */
          LCD->RAM[LCD_RAMRegister_11] &= (uint8_t)0xEF;
          break;

        case PIXELCOLUMN_18:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 1, Column = 18 */
          LCD->RAM[LCD_RAMRegister_11] &= (uint8_t)0xF7;
          break;

        case PIXELCOLUMN_19:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 1, Column = 19 */
          LCD->RAM[LCD_RAMRegister_11] &= (uint8_t)0xFB;
          break;

        default:
          break;

      }
      break;
    case PIXELROW_2:
      switch ( PIXELCOLUMN)
      {
        case  PIXELCOLUMN_1:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 2, Column = 1 */
          LCD->RAM[LCD_RAMRegister_8] &= (uint8_t)0xFD;
          break;

        case  PIXELCOLUMN_2:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 2, Column = 2 */
          LCD->RAM[LCD_RAMRegister_8] &= (uint8_t)0xBF;
          break;

        case  PIXELCOLUMN_3:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 2, Column = 3 */
          LCD->RAM[LCD_RAMRegister_8] &= (uint8_t)0xDF;
          break;

        case  PIXELCOLUMN_4:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 2, Column = 4 */
          LCD->RAM[LCD_RAMRegister_8] &= (uint8_t)0xEF;
          break;

        case  PIXELCOLUMN_5:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 2, Column = 5 */
          LCD->RAM[LCD_RAMRegister_9] &= (uint8_t)0xFD;
          break;

        case  PIXELCOLUMN_6:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 2, Column = 6 */
          LCD->RAM[LCD_RAMRegister_9] &= (uint8_t)0xFE;
          break;

        case  PIXELCOLUMN_7:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 2, Column = 7 */
          LCD->RAM[LCD_RAMRegister_8] &= (uint8_t)0x7F;
          break;

        case  PIXELCOLUMN_8:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 2, Column = 8 */
          LCD->RAM[LCD_RAMRegister_9] &= (uint8_t)0xEF;
          break;

        case  PIXELCOLUMN_9:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 2, Column = 9 */
          LCD->RAM[LCD_RAMRegister_9] &= (uint8_t)0xF7;
          break;

        case  PIXELCOLUMN_10:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 2, Column = 10 */
          LCD->RAM[LCD_RAMRegister_9] &= (uint8_t)0xFB;
          break;

        case  PIXELCOLUMN_11:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 2, Column = 11 */
          LCD->RAM[LCD_RAMRegister_7] &= (uint8_t)0xFB;
          break;

        case PIXELCOLUMN_12:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 2, Column = 12 */
          LCD->RAM[LCD_RAMRegister_7] &= (uint8_t)0xFD;
          break;

        case  PIXELCOLUMN_13:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 2, Column = 13 */
          LCD->RAM[LCD_RAMRegister_7] &= (uint8_t)0xFE;
          break;

        case  PIXELCOLUMN_14:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 2, Column = 14 */
          LCD->RAM[LCD_RAMRegister_7] &= (uint8_t)0xDF;
          break;

        case  PIXELCOLUMN_15:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 2, Column = 15 */
          LCD->RAM[LCD_RAMRegister_7] &= (uint8_t)0xEF;
          break;

        case  PIXELCOLUMN_16:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 2, Column = 16 */
          LCD->RAM[LCD_RAMRegister_7] &= (uint8_t)0xF7;
          break;

        case  PIXELCOLUMN_17:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 2, Column = 17 */
          LCD->RAM[LCD_RAMRegister_8] &= (uint8_t)0xFE;
          break;

        case  PIXELCOLUMN_18:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 2, Column = 18 */
          LCD->RAM[LCD_RAMRegister_7] &= (uint8_t)0x7F;
          break;

        case  PIXELCOLUMN_19:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 2, Column = 19 */
          LCD->RAM[LCD_RAMRegister_7] &= (uint8_t)0xBF;
          break;

        default:
          break;
      }
      break;

    case PIXELROW_3:
      switch ( PIXELCOLUMN)
      {
        case PIXELCOLUMN_1:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 3, Column = 1 */
          LCD->RAM[LCD_RAMRegister_9] &= (uint8_t)0xDF;
          break;

        case PIXELCOLUMN_2:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 3, Column = 2 */
          LCD->RAM[LCD_RAMRegister_9] &= (uint8_t)0xBF;
          break;

        case PIXELCOLUMN_3:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 3, Column = 3 */
          LCD->RAM[LCD_RAMRegister_9] &= (uint8_t)0x7F;
          break;

        case PIXELCOLUMN_4:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 3, Column = 4 */
          LCD->RAM[LCD_RAMRegister_10] &= (uint8_t)0xFE;
          break;

        case PIXELCOLUMN_5:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 3, Column = 5 */
          LCD->RAM[LCD_RAMRegister_10] &= (uint8_t)0xFD;
          break;

        case PIXELCOLUMN_6:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 3, Column = 6 */
          LCD->RAM[LCD_RAMRegister_10] &= (uint8_t)0xFB;
          break;

        case PIXELCOLUMN_7:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 3, Column = 7 */
          LCD->RAM[LCD_RAMRegister_10] &= (uint8_t)0xF7;
          break;

        case PIXELCOLUMN_8:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 3, Column = 8 */
          LCD->RAM[LCD_RAMRegister_18] &= (uint8_t)0xFE;
          break;

        case PIXELCOLUMN_9:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 3, Column = 9 */
          LCD->RAM[LCD_RAMRegister_18] &= (uint8_t)0xFD;
          break;

        case PIXELCOLUMN_10:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 3, Column = 10 */
          LCD->RAM[LCD_RAMRegister_18] &= (uint8_t)0xFB;
          break;

        case PIXELCOLUMN_11:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 3, Column = 11 */
          LCD->RAM[LCD_RAMRegister_18] &= (uint8_t)0xF7;
          break;

        case PIXELCOLUMN_12:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 3, Column = 12 */
          LCD->RAM[LCD_RAMRegister_18] &= (uint8_t)0xEF;
          break;

        case PIXELCOLUMN_13:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 3, Column = 13 */
          LCD->RAM[LCD_RAMRegister_18] &= (uint8_t)0xDF;
          break;

        case PIXELCOLUMN_14:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 3, Column = 14 */
          LCD->RAM[LCD_RAMRegister_18] &= (uint8_t)0xBF;
          break;

        case PIXELCOLUMN_15:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 3, Column = 15 */
          LCD->RAM[LCD_RAMRegister_18] &= (uint8_t)0x7F;
          break;

        case PIXELCOLUMN_16:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 3, Column = 16 */
          LCD->RAM[LCD_RAMRegister_19] &= (uint8_t)0xFE;
          break;

        case PIXELCOLUMN_17:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 3, Column = 17 */
          LCD->RAM[LCD_RAMRegister_19] &= (uint8_t)0xFD;
          break;

        case PIXELCOLUMN_18:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 3, Column = 18 */
          LCD->RAM[LCD_RAMRegister_19] &= (uint8_t)0xFB;
          break;

        case PIXELCOLUMN_19:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 3, Column = 19 */
          LCD->RAM[LCD_RAMRegister_19] &= (uint8_t)0xF7;
          break;

        default:
          break;
      }
      break;
    case PIXELROW_4:
      switch ( PIXELCOLUMN)
      {
        case PIXELCOLUMN_1:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 4, Column = 1 */
          LCD->RAM[LCD_RAMRegister_13] &= (uint8_t)0xFD;
          break;

        case PIXELCOLUMN_2:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 4, Column = 2 */
          LCD->RAM[LCD_RAMRegister_13] &= (uint8_t)0xFB;
          break;

        case PIXELCOLUMN_3:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 4, Column = 3 */
          LCD->RAM[LCD_RAMRegister_13] &= (uint8_t)0xF7;
          break;

        case PIXELCOLUMN_4:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 4, Column = 4 */
          LCD->RAM[LCD_RAMRegister_13] &= (uint8_t)0xEF;
          break;

        case PIXELCOLUMN_5:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 4, Column = 5 */
          LCD->RAM[LCD_RAMRegister_13] &= (uint8_t)0xDF;
          break;

        case PIXELCOLUMN_6:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 4, Column = 6 */
          LCD->RAM[LCD_RAMRegister_13] &= (uint8_t)0xBF;
          break;

        case PIXELCOLUMN_7:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 4, Column = 7 */
          LCD->RAM[LCD_RAMRegister_13] &= (uint8_t)0x7F;
          break;

        case PIXELCOLUMN_8:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 4, Column = 8 */
          LCD->RAM[LCD_RAMRegister_20] &= (uint8_t)0xFE;
          break;

        case PIXELCOLUMN_9:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 4, Column = 9 */
          LCD->RAM[LCD_RAMRegister_20] &= (uint8_t)0xFD;
          break;

        case PIXELCOLUMN_10:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 4, Column = 10 */
          LCD->RAM[LCD_RAMRegister_20] &= (uint8_t)0xFB;
          break;

        case PIXELCOLUMN_11:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 4, Column = 11 */
          LCD->RAM[LCD_RAMRegister_20] &= (uint8_t)0xF7;
          break;

        case PIXELCOLUMN_12:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 4, Column = 12 */
          LCD->RAM[LCD_RAMRegister_20] &= (uint8_t)0xEF;
          break;

        case PIXELCOLUMN_13:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 4, Column = 13 */
          LCD->RAM[LCD_RAMRegister_20] &= (uint8_t)0xDF;
          break;

        case PIXELCOLUMN_14:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 4, Column = 14 */
          LCD->RAM[LCD_RAMRegister_20] &= (uint8_t)0xBF;
          break;

        case PIXELCOLUMN_15:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 4, Column = 15 */
          LCD->RAM[LCD_RAMRegister_20] &= (uint8_t)0x7F;
          break;

        case PIXELCOLUMN_16:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 4, Column = 16 */
          LCD->RAM[LCD_RAMRegister_21] &= (uint8_t)0xFE;
          break;

        case PIXELCOLUMN_17:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 4, Column = 17 */
          LCD->RAM[LCD_RAMRegister_21] &= (uint8_t)0xFD;
          break;

        case PIXELCOLUMN_18:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 4, Column = 18 */
          LCD->RAM[LCD_RAMRegister_21] &= (uint8_t)0xFB;
          break;

        case PIXELCOLUMN_19:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 4, Column = 19 */
          LCD->RAM[LCD_RAMRegister_21] &= (uint8_t)0xF7;
          break;

        default:
          break;

      }
      break;
    case PIXELROW_5:
      switch ( PIXELCOLUMN)
      {
        case PIXELCOLUMN_1:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 5, Column = 1 */
          LCD->RAM[LCD_RAMRegister_2] &= (uint8_t)0xDF;
          break;

        case PIXELCOLUMN_2:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 5, Column = 2 */
          LCD->RAM[LCD_RAMRegister_2] &= (uint8_t)0xBF;
          break;

        case PIXELCOLUMN_3:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 5, Column = 3 */
          LCD->RAM[LCD_RAMRegister_2] &= (uint8_t)0x7F;
          break;

        case PIXELCOLUMN_4:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 5, Column = 4 */
          LCD->RAM[LCD_RAMRegister_3] &= (uint8_t)0xFE;
          break;

        case PIXELCOLUMN_5:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 5, Column = 5 */
          LCD->RAM[LCD_RAMRegister_3] &= (uint8_t)0xFD;
          break;

        case PIXELCOLUMN_6:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 5, Column = 6 */
          LCD->RAM[LCD_RAMRegister_3] &= (uint8_t)0xFB;
          break;

        case PIXELCOLUMN_7:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 5, Column = 7 */
          LCD->RAM[LCD_RAMRegister_3] &= (uint8_t)0xF7;
          break;

        case PIXELCOLUMN_8:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 5, Column = 8 */
          LCD->RAM[LCD_RAMRegister_14] &= (uint8_t)0xFE;
          break;

        case PIXELCOLUMN_9:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 5, Column = 9 */
          LCD->RAM[LCD_RAMRegister_14] &= (uint8_t)0xFD;
          break;

        case PIXELCOLUMN_10:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 5, Column = 10 */
          LCD->RAM[LCD_RAMRegister_14] &= (uint8_t)0xFB;
          break;

        case PIXELCOLUMN_11:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 5, Column = 12 */
          LCD->RAM[LCD_RAMRegister_14] &= (uint8_t)0xF7;
          break;

        case PIXELCOLUMN_12:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 5, Column = 12 */
          LCD->RAM[LCD_RAMRegister_14] &= (uint8_t)0xEF;
          break;

        case PIXELCOLUMN_13:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 5, Column = 13 */
          LCD->RAM[LCD_RAMRegister_14] &= (uint8_t)0xDF;
          break;

        case PIXELCOLUMN_14:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 5, Column = 14 */
          LCD->RAM[LCD_RAMRegister_14] &= (uint8_t)0xBF;
          break;

        case PIXELCOLUMN_15:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 5, Column = 15 */
          LCD->RAM[LCD_RAMRegister_14] &= (uint8_t)0x7F;
          break;

        case PIXELCOLUMN_16:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 5, Column = 16 */
          LCD->RAM[LCD_RAMRegister_15] &= (uint8_t)0xFE;
          break;

        case PIXELCOLUMN_17:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 5, Column = 17 */
          LCD->RAM[LCD_RAMRegister_15] &= (uint8_t)0xFD;
          break;

        case PIXELCOLUMN_18:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 5, Column = 18 */
          LCD->RAM[LCD_RAMRegister_15] &= (uint8_t)0xFB;
          break;

        case PIXELCOLUMN_19:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 5, Column = 19 */
          LCD->RAM[LCD_RAMRegister_15] &= (uint8_t)0xF7;
          break;

        default:
          break;
      }
      break;
    case PIXELROW_6:
      switch ( PIXELCOLUMN)
      {
        case PIXELCOLUMN_1:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 6, Column = 1 */
          LCD->RAM[LCD_RAMRegister_6] &= (uint8_t)0xFD;
          break;

        case PIXELCOLUMN_2:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row =6, Column = 2*/
          LCD->RAM[LCD_RAMRegister_6] &= (uint8_t)0xFB;
          break;

        case PIXELCOLUMN_3:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 6, Column = 3 */
          LCD->RAM[LCD_RAMRegister_6] &= (uint8_t)0xF7;
          break;

        case PIXELCOLUMN_4:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 6, Column = 4 */
          LCD->RAM[LCD_RAMRegister_6] &= (uint8_t)0xEF;
          break;

        case PIXELCOLUMN_5:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 6, Column = 5 */
          LCD->RAM[LCD_RAMRegister_6] &= (uint8_t)0xDF;
          break;

        case PIXELCOLUMN_6:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 6, Column = 6 */
          LCD->RAM[LCD_RAMRegister_6] &= (uint8_t)0xBF;
          break;

        case PIXELCOLUMN_7:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 6, Column = 7 */
          LCD->RAM[LCD_RAMRegister_6] &= (uint8_t)0x7F;
          break;

        case PIXELCOLUMN_8:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 6, Column = 8 */
          LCD->RAM[LCD_RAMRegister_16] &= (uint8_t)0xFE;
          break;

        case PIXELCOLUMN_9:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 6, Column = 9 */
          LCD->RAM[LCD_RAMRegister_16] &= (uint8_t)0xFD;
          break;

        case PIXELCOLUMN_10:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 6, Column = 10 */
          LCD->RAM[LCD_RAMRegister_16] &= (uint8_t)0xFB;
          break;

        case PIXELCOLUMN_11:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 6, Column = 11 */
          LCD->RAM[LCD_RAMRegister_16] &= (uint8_t)0xF7;
          break;

        case PIXELCOLUMN_12:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 6, Column = 12 */
          LCD->RAM[LCD_RAMRegister_16] &= (uint8_t)0xEF;
          break;

        case PIXELCOLUMN_13:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 6, Column = 13 */
          LCD->RAM[LCD_RAMRegister_16] &= (uint8_t)0xDF;
          break;

        case PIXELCOLUMN_14:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 6, Column = 14 */
          LCD->RAM[LCD_RAMRegister_16] &= (uint8_t)0xBF;
          break;

        case PIXELCOLUMN_15:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 6, Column = 15 */
          LCD->RAM[LCD_RAMRegister_16] &= (uint8_t)0x7F;
          break;

        case PIXELCOLUMN_16:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 6, Column = 16 */
          LCD->RAM[LCD_RAMRegister_17] &= (uint8_t)0xFE;
          break;


        case PIXELCOLUMN_17:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 6, Column = 17 */
          LCD->RAM[LCD_RAMRegister_17] &= (uint8_t)0xFD;
          break;


        case PIXELCOLUMN_18:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 6, Column = 18 */
          LCD->RAM[LCD_RAMRegister_17] &= (uint8_t)0xFB;
          break;

        case PIXELCOLUMN_19:
          /* Enable the write access on the LCD RAM First banck */
          LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
          /* Position : Row = 6, Column = 19 */
          LCD->RAM[LCD_RAMRegister_17] &= (uint8_t)0xF7;
          break;

        default:
          break;
      }
      break;
    case PIXELROW_7:
      switch ( PIXELCOLUMN)
      {
        case PIXELCOLUMN_1:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 7, Column = 1 */
          LCD->RAM[LCD_RAMRegister_13] &= (uint8_t)0xFD;
          break;

        case PIXELCOLUMN_2:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 7, Column = 2 */
          LCD->RAM[LCD_RAMRegister_13] &= (uint8_t)0xFB;
          break;

        case PIXELCOLUMN_3:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 7, Column = 3 */
          LCD->RAM[LCD_RAMRegister_13] &= (uint8_t)0xF7;
          break;

        case PIXELCOLUMN_4:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 7, Column = 4 */
          LCD->RAM[LCD_RAMRegister_13] &= (uint8_t)0xEF;
          break;

        case PIXELCOLUMN_5:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 7, Column = 5 */
          LCD->RAM[LCD_RAMRegister_13] &= (uint8_t)0xDF;
          break;

        case PIXELCOLUMN_6:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 7, Column = 6 */
          LCD->RAM[LCD_RAMRegister_13] &= (uint8_t)0xBF;
          break;

        case PIXELCOLUMN_7:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 7, Column = 7 */
          LCD->RAM[LCD_RAMRegister_13] &= (uint8_t)0x7F;
          break;

        case PIXELCOLUMN_8:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 7, Column = 8 */
          LCD->RAM[LCD_RAMRegister_20] &= (uint8_t)0xFE;
          break;

        case PIXELCOLUMN_9:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 7, Column = 9 */
          LCD->RAM[LCD_RAMRegister_20] &= (uint8_t)0xFD;
          break;

        case PIXELCOLUMN_10:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 7, Column = 10 */
          LCD->RAM[LCD_RAMRegister_20] &= (uint8_t)0xFB;
          break;

        case PIXELCOLUMN_11:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 7, Column = 11 */
          LCD->RAM[LCD_RAMRegister_20] &= (uint8_t)0xF7;
          break;

        case PIXELCOLUMN_12:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 7, Column = 12 */
          LCD->RAM[LCD_RAMRegister_20] &= (uint8_t)0xEF;
          break;

        case PIXELCOLUMN_13:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 7, Column = 13 */
          LCD->RAM[LCD_RAMRegister_20] &= (uint8_t)0xDF;
          break;

        case PIXELCOLUMN_14:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 7, Column = 14 */
          LCD->RAM[LCD_RAMRegister_20] &= (uint8_t)0xBF;
          break;

        case PIXELCOLUMN_15:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 7, Column = 15 */
          LCD->RAM[LCD_RAMRegister_20] &= (uint8_t)0x7F;
          break;

        case PIXELCOLUMN_16:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 7, Column = 16 */
          LCD->RAM[LCD_RAMRegister_21] &= (uint8_t)0xFE;
          break;

        case PIXELCOLUMN_17:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 7, Column = 17 */
          LCD->RAM[LCD_RAMRegister_21] &= (uint8_t)0xFD;
          break;

        case PIXELCOLUMN_18:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 7, Column = 18 */
          LCD->RAM[LCD_RAMRegister_21] &= (uint8_t)0xFB;
          break;

        case PIXELCOLUMN_19:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 7, Column = 19 */
          LCD->RAM[LCD_RAMRegister_21] &= (uint8_t)0xF7;
          break;

        default:
          break;
      }
      break;
    case PIXELROW_8:
      switch ( PIXELCOLUMN)
      {
        case PIXELCOLUMN_1:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 8, Column = 1 */
          LCD->RAM[LCD_RAMRegister_9] &= (uint8_t)0xDF;
          break;

        case PIXELCOLUMN_2:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 8, Column = 2 */
          LCD->RAM[LCD_RAMRegister_9] &= (uint8_t)0xBF;
          break;

        case PIXELCOLUMN_3:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 8, Column = 3 */
          LCD->RAM[LCD_RAMRegister_9] &= (uint8_t)0x7F;
          break;

        case PIXELCOLUMN_4:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 8, Column = 4 */
          LCD->RAM[LCD_RAMRegister_10] &= (uint8_t)0xFE;
          break;

        case PIXELCOLUMN_5:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 8, Column = 5 */
          LCD->RAM[LCD_RAMRegister_10] &= (uint8_t)0xFD;
          break;

        case PIXELCOLUMN_6:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 8, Column = 6 */
          LCD->RAM[LCD_RAMRegister_10] &= (uint8_t)0xFB;
          break;

        case PIXELCOLUMN_7:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 8, Column = 7 */
          LCD->RAM[LCD_RAMRegister_10] &= (uint8_t)0xF7;
          break;

        case PIXELCOLUMN_8:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 8, Column = 8 */
          LCD->RAM[LCD_RAMRegister_18] &= (uint8_t)0xFE;
          break;

        case PIXELCOLUMN_9:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 8, Column = 9 */
          LCD->RAM[LCD_RAMRegister_18] &= (uint8_t)0xFD;
          break;

        case PIXELCOLUMN_10:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 8, Column = 10 */
          LCD->RAM[LCD_RAMRegister_18] &= (uint8_t)0xFB;
          break;

        case PIXELCOLUMN_11:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 8, Column = 11 */
          LCD->RAM[LCD_RAMRegister_18] &= (uint8_t)0xF7;
          break;

        case PIXELCOLUMN_12:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 8, Column = 12 */
          LCD->RAM[LCD_RAMRegister_18] &= (uint8_t)0xEF;
          break;

        case PIXELCOLUMN_13:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 8, Column = 13 */
          LCD->RAM[LCD_RAMRegister_18] &= (uint8_t)0xDF;
          break;

        case PIXELCOLUMN_14:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 8, Column = 14 */
          LCD->RAM[LCD_RAMRegister_18] &= (uint8_t)0xBF;
          break;

        case PIXELCOLUMN_15:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 8, Column = 15 */
          LCD->RAM[LCD_RAMRegister_18] &= (uint8_t)0x7F;
          break;

        case PIXELCOLUMN_16:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 8, Column = 16 */
          LCD->RAM[LCD_RAMRegister_19] &= (uint8_t)0xFE;
          break;

        case PIXELCOLUMN_17:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 8, Column = 17 */
          LCD->RAM[LCD_RAMRegister_19] &= (uint8_t)0xFD;
          break;

        case PIXELCOLUMN_18:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 8, Column = 18 */
          LCD->RAM[LCD_RAMRegister_19] &= (uint8_t)0xFB;
          break;

        case PIXELCOLUMN_19:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 8, Column = 19 */
          LCD->RAM[LCD_RAMRegister_19] &= (uint8_t)0xF7;
          break;

        default:
          break;
      }
      break;
    case PIXELROW_9:
      switch ( PIXELCOLUMN)
      {
        case PIXELCOLUMN_1:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 9, Column = 1 */
          LCD->RAM[LCD_RAMRegister_6] &= (uint8_t)0xFD;
          break;

        case PIXELCOLUMN_2:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 9, Column = 2 */
          LCD->RAM[LCD_RAMRegister_6] &= (uint8_t)0xFB;
          break;

        case PIXELCOLUMN_3:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 9, Column = 3 */
          LCD->RAM[LCD_RAMRegister_6] &= (uint8_t)0xF7;
          break;

        case PIXELCOLUMN_4:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 9, Column = 4 */
          LCD->RAM[LCD_RAMRegister_6] &= (uint8_t)0xEF;
          break;

        case PIXELCOLUMN_5:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 9, Column = 5 */
          LCD->RAM[LCD_RAMRegister_6] &= (uint8_t)0xDF;
          break;

        case PIXELCOLUMN_6:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 9, Column = 6 */
          LCD->RAM[LCD_RAMRegister_6] &= (uint8_t)0xBF;
          break;

        case PIXELCOLUMN_7:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 9, Column = 7 */
          LCD->RAM[LCD_RAMRegister_6] &= (uint8_t)0x7F;
          break;

        case PIXELCOLUMN_8:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 9, Column = 8 */
          LCD->RAM[LCD_RAMRegister_16] &= (uint8_t)0xFE;
          break;

        case PIXELCOLUMN_9:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 9, Column = 9 */
          LCD->RAM[LCD_RAMRegister_16] &= (uint8_t)0xFD;
          break;

        case PIXELCOLUMN_10:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 9, Column = 10 */
          LCD->RAM[LCD_RAMRegister_16] &= (uint8_t)0xFB;
          break;

        case PIXELCOLUMN_11:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 9, Column = 11 */
          LCD->RAM[LCD_RAMRegister_16] &= (uint8_t)0xF7;
          break;

        case PIXELCOLUMN_12:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 9, Column = 12 */
          LCD->RAM[LCD_RAMRegister_16] &= (uint8_t)0xEF;
          break;

        case PIXELCOLUMN_13:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 9, Column = 13 */
          LCD->RAM[LCD_RAMRegister_16] &= (uint8_t)0xDF;
          break;

        case PIXELCOLUMN_14:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 9, Column = 14 */
          LCD->RAM[LCD_RAMRegister_16] &= (uint8_t)0xBF;
          break;

        case PIXELCOLUMN_15:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 9, Column = 15 */
          LCD->RAM[LCD_RAMRegister_16] &= (uint8_t)0x7F;
          break;

        case PIXELCOLUMN_16:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 9, Column = 16 */
          LCD->RAM[LCD_RAMRegister_17] &= (uint8_t)0xFE;
          break;

        case PIXELCOLUMN_17:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 9, Column = 17 */
          LCD->RAM[LCD_RAMRegister_17] &= (uint8_t)0xFD;
          break;

        case PIXELCOLUMN_18:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 9, Column = 18 */
          LCD->RAM[LCD_RAMRegister_17] &= (uint8_t)0xFB;
          break;

        case PIXELCOLUMN_19:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 9, Column = 19 */
          LCD->RAM[LCD_RAMRegister_17] &= (uint8_t)0xF7;
          break;

        default:
          break;
      }
      break;
    case PIXELROW_10:
      switch ( PIXELCOLUMN)
      {
        case PIXELCOLUMN_1:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 10, Column = 1 */
          LCD->RAM[LCD_RAMRegister_2] &= (uint8_t)0xDF;
          break;

        case PIXELCOLUMN_2:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 10, Column = 2 */
          LCD->RAM[LCD_RAMRegister_2] &= (uint8_t)0xBF;
          break;

        case PIXELCOLUMN_3:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 10, Column = 3 */
          LCD->RAM[LCD_RAMRegister_2] &= (uint8_t)0x7F;
          break;

        case PIXELCOLUMN_4:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 10, Column = 4 */
          LCD->RAM[LCD_RAMRegister_3] &= (uint8_t)0xFE;
          break;

        case PIXELCOLUMN_5:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 10, Column = 5 */
          LCD->RAM[LCD_RAMRegister_3] &= (uint8_t)0xFD;
          break;

        case PIXELCOLUMN_6:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 10, Column = 6 */
          LCD->RAM[LCD_RAMRegister_3] &= (uint8_t)0xFB;
          break;

        case PIXELCOLUMN_7:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 10, Column = 7 */
          LCD->RAM[LCD_RAMRegister_3] &= (uint8_t)0xF7;
          break;

        case PIXELCOLUMN_8:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 10, Column = 8 */
          LCD->RAM[LCD_RAMRegister_14] &= (uint8_t)0xFE;
          break;

        case PIXELCOLUMN_9:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 10, Column = 9 */
          LCD->RAM[LCD_RAMRegister_14] &= (uint8_t)0xFD;
          break;

        case PIXELCOLUMN_10:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 10, Column = 10 */
          LCD->RAM[LCD_RAMRegister_14] &= (uint8_t)0xFB;
          break;

        case PIXELCOLUMN_11:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 10, Column = 11 */
          LCD->RAM[LCD_RAMRegister_14] &= (uint8_t)0xF7;
          break;

        case PIXELCOLUMN_12:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 10, Column = 12 */
          LCD->RAM[LCD_RAMRegister_14] &= (uint8_t)0xEF;
          break;

        case PIXELCOLUMN_13:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 10, Column = 13 */
          LCD->RAM[LCD_RAMRegister_14] &= (uint8_t)0xDF;
          break;

        case PIXELCOLUMN_14:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 10, Column = 14 */
          LCD->RAM[LCD_RAMRegister_14] &= (uint8_t)0xBF;
          break;

        case PIXELCOLUMN_15:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 10, Column = 15 */
          LCD->RAM[LCD_RAMRegister_14] &= (uint8_t)0x7F;
          break;

        case PIXELCOLUMN_16:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 10, Column = 16 */
          LCD->RAM[LCD_RAMRegister_15] &= (uint8_t)0xFE;
          break;

        case PIXELCOLUMN_17:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 10, Column = 17 */
          LCD->RAM[LCD_RAMRegister_15] &= (uint8_t)0xFD;
          break;

        case PIXELCOLUMN_18:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 10, Column = 18 */
          LCD->RAM[LCD_RAMRegister_15] &= (uint8_t)0xFB;
          break;

        case PIXELCOLUMN_19:
          /* Enable the write access on the LCD RAM second banck */
          LCD->CR4 |= LCD_CR4_PAGECOM;
          /* Position : Row = 10, Column = 19 */
          LCD->RAM[LCD_RAMRegister_15] &= (uint8_t)0xF7;
          break;

        default:
          break;
      }
      break;
  }

}
/**
  * @brief  Converts an ascii char to the a LCD digit (previous coding).
  * @param  c: a char to display.
  * @param  Point: a point to add in front of a char
  *         This parameter can be: POINT_OFF or POINT_ON
  * @param  DoublePoint: flag indicating if Double Point has to be added in front
  *         of the displayed character.
  *         This parameter can be: DOUBLEPOINT_ON or DOUBLEPOINT_OFF.
  * @retval None
  */
static void Convert(uint8_t* c, Point_Typedef Point, DoublePoint_Typedef DoublePoint)
{
  uint32_t ch = 0 , tmp = 0;
  uint16_t i;

  /* The character c is a letter in upper case*/
  if ((*c < (uint8_t)0x5B)&(*c > (uint8_t)0x40))
  {
    ch = LetterMap[*c-(uint8_t)0x41];
  }
  /* The character c is a number*/
  if ((*c < (uint8_t)0x3A)&(*c > (uint8_t)0x2F))
  {
    ch = NumberMap[*c-(uint8_t)0x30];
  }
  /* The character c is a space character */
  if (*c == (uint8_t)0x20)
  {
    ch = (uint8_t)0x00;
  }
  /* Set the Q pixel in the character that can be displayed if the point is on */
  if (Point == POINT_ON)
  {
    ch |= (uint8_t)0x400000;
  }

  /* Set the P pixel in the character that can be displayed if the double point is on */
  if (DoublePoint == DOUBLEPOINT_ON)
  {
    ch |= (uint8_t)0x000040;
  }

  for (i = 0;i < 6; i++)
  {
    tmp = ch & mask[i];
    digit[i] = (uint8_t)(tmp >> (uint8_t)shift[i]);
  }
}
/**
  * @brief  Inserts a delay time.
  * @param  nCount: specifies the delay time length.
  * @retval None
  */
static void delay(__IO uint32_t nCount)
{
  __IO uint32_t index = 0;
  for (index = (0x60 * nCount); index != 0; index--)
  {}
}

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
