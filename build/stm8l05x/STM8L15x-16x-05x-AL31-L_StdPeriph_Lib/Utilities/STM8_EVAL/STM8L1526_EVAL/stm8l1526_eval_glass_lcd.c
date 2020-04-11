/**
  ******************************************************************************
  * @file    stm8l1526_eval_glass_lcd.c
  * @author  MCD Application Team
  * @version V2.1.3
  * @date    28-June-2013
  * @brief   This file includes driver for the glass LCD Module mounted on
  *          STM8L1526-EVAL board.
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
#include "stm8l1526_eval_glass_lcd.h"

/** @addtogroup Utilities
  * @{
  */

/** @defgroup STM8L1526_EVAL_GLASS_LCD
  * @brief This file includes the LCD Glass driver for Pacific Display
  *        (LCD_PD878, PD878-DP-FH-W-LV-6-RH) Module of STM8L1526-EVAL board.
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
      --  ----------
    X \/  |\   |I  /|
         F| H  |  J |B
          |  \ | /  |
          --G-- --K--
          |   /| \  |
        E |  L |  N |C
          | /  |M  \|   _
          -----------  | |DP
              D         -

A LCD character coding is based on the following matrix:

    { X , F , E , D  }
    { I , J , K , N  }
    { A , B , C , DP }
    { H , G , L , M  }

The character A for example is:
-------------------------------
    { 0 , 1 , 1 , 0 }
    { 0 , 0 , 1 , 0 }
    { 1 , 1 , 1 , 0 }
    { 0 , 1 , 0 , 0 }
   -------------------
   =  4   D   7   0   hex

   => 'A' = 0x4D70

  @endverbatim
  */

/**
  * @brief LETTERS AND NUMBERS MAPPING DEFINITION
  */

CONST uint16_t mask[4] =
  {
    0xF000, 0x0F00, 0x00F0, 0x000F
  };
CONST uint8_t shift[4] =
  {
    12, 8, 4, 0
  };

uint8_t digit[4];     /* Digit frame buffer */

/* Letters and number map of PD_878 LCD */
CONST uint16_t LetterMap[26] =
  {
    /* A      B      C      D      E      F      G      H      I  */
    0x4D70, 0x6469, 0x4111, 0x6449, 0x4911, 0x4910, 0x4171, 0x0D70, 0x6009,
    /* J      K      L      M      N      O      P      Q      R  */
    0x0451, 0x0B12, 0x0111, 0x8750, 0x8552, 0x4551, 0x4D30, 0x4553, 0x4D32,
    /* S      T      U      V      W      X      Y      Z  */
    0x4961, 0x6008, 0x0551, 0x0390, 0x05D2, 0x8282, 0x8208, 0x4281
  };

CONST uint16_t NumberMap[10] =
  {
    /* 0      1      2      3      4      5      6      7      8      9  */
    0x47D1, 0x0640, 0x4C31, 0x4C61, 0x0D60, 0x4961, 0x4971, 0x4440, 0x4D71, 0x4D61
  };

__IO uint8_t str[7] = "";
/* Private function prototypes -----------------------------------------------*/
static void Convert(uint8_t* c, Point_Typedef point, Apostrophe_Typedef apostrophe);
static void delay(__IO uint32_t nCount);
/* Private functions ---------------------------------------------------------*/


/** @defgroup STM8L1526_EVAL_GLASS_LCD_Private_Functions
  * @{
  */


/**
  * @brief  Configures the LCD GLASS relative GPIO port IOs and LCD peripheral.
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
     - Mode = 1/4 Duty, 1/3 Bias
     - LCD frequency = (clock source * Duty) / (Prescaler * Divider)
                     = 228 Hz ==> Frame frequency = 57 Hz */

  /* Enable LCD clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_LCD, ENABLE);
  CLK_RTCClockConfig(CLK_RTCCLKSource_LSE, CLK_RTCCLKDiv_1);

  /* Initialize the LCD */
  LCD_Init(LCD_Prescaler_2, LCD_Divider_18, LCD_Duty_1_4,
           LCD_Bias_1_3, LCD_VoltageSource_Internal);

  /* Mask register*/
  LCD_PortMaskConfig(LCD_PortMaskRegister_0, 0xFF);
  LCD_PortMaskConfig(LCD_PortMaskRegister_1, 0xFF);
  LCD_PortMaskConfig(LCD_PortMaskRegister_2, 0xFF);
  LCD_PortMaskConfig(LCD_PortMaskRegister_3, 0xFF);

  LCD_ContrastConfig(LCD_Contrast_Level_7);
  LCD_DeadTimeConfig(LCD_DeadTime_0);
  LCD_PulseOnDurationConfig(LCD_PulseOnDuration_1);

  LCD_Cmd(ENABLE); /*!< Enable LCD peripheral */
}
/**
  * @brief  This function writes a char in the LCD frame buffer.
  * @param  ch: the character to dispaly.
  * @param  point: a point to add in front of char
  *         This parameter can be: POINT_OFF or POINT_ON
  * @param  apostrophe: flag indicating if a apostrophe has to be add in front
  *         of displayed character.
  *         This parameter can be: APOSTROPHE_OFF or APOSTROPHE_ON.
  * @param  position: position in the LCD of the caracter to write [0:7]
  * @retval None
  * @par    Required preconditions: The LCD should be cleared before to start the
  *         write operation.
  */
void LCD_GLASS_WriteChar(uint8_t* ch, Point_Typedef point,
                         Apostrophe_Typedef apostrophe, uint8_t position)
{
  Convert(ch, point, apostrophe);

  switch (position)
  {
      /* Position 0 on LCD (Digit1)*/
    case 0:
      LCD->RAM[LCD_RAMRegister_3] &= LCD_CLEAR_LSB;
      LCD->RAM[LCD_RAMRegister_3] |=  digit[0];
      LCD->RAM[LCD_RAMRegister_6] &= LCD_CLEAR_MSB;
      LCD->RAM[LCD_RAMRegister_6] |=  (uint8_t)(digit[1] << 4);
      LCD->RAM[LCD_RAMRegister_10] &= LCD_CLEAR_LSB;
      LCD->RAM[LCD_RAMRegister_10] |=  digit[2];
      LCD->RAM[LCD_RAMRegister_13] &= LCD_CLEAR_MSB;
      LCD->RAM[LCD_RAMRegister_13] |=  (uint8_t)((uint8_t)digit[3] << 4);
      break;

      /* Position 1 on LCD (Digit2)*/
    case 1:
      LCD->RAM[LCD_RAMRegister_2] &= LCD_CLEAR_MSB;
      LCD->RAM[LCD_RAMRegister_2] |=  (uint8_t)(digit[0] << 4);
      LCD->RAM[LCD_RAMRegister_6] &= LCD_CLEAR_LSB;
      LCD->RAM[LCD_RAMRegister_6] |=  digit[1];
      LCD->RAM[LCD_RAMRegister_9] &= LCD_CLEAR_MSB;
      LCD->RAM[LCD_RAMRegister_9] |=   (uint8_t)(digit[2] << 4);
      LCD->RAM[LCD_RAMRegister_13] &= LCD_CLEAR_LSB;
      LCD->RAM[LCD_RAMRegister_13] |=  digit[3];
      break;

      /* Position 2 on LCD (Digit3)*/
    case 2:
      LCD->RAM[LCD_RAMRegister_2] &= LCD_CLEAR_LSB;
      LCD->RAM[LCD_RAMRegister_2] |=  digit[0];
      LCD->RAM[LCD_RAMRegister_5] &= LCD_CLEAR_MSB;
      LCD->RAM[LCD_RAMRegister_5] |=  (uint8_t)(digit[1] << 4);
      LCD->RAM[LCD_RAMRegister_9] &= LCD_CLEAR_LSB;
      LCD->RAM[LCD_RAMRegister_9] |=   digit[2];
      LCD->RAM[LCD_RAMRegister_12] &= LCD_CLEAR_MSB;
      LCD->RAM[LCD_RAMRegister_12] |=  (uint8_t)(digit[3] << 4);
      break;

      /* Position 3 on LCD (Digit4)*/
    case 3:
      LCD->RAM[LCD_RAMRegister_1] &= LCD_CLEAR_MSB;
      LCD->RAM[LCD_RAMRegister_1] |=  (uint8_t)(digit[0] << 4);
      LCD->RAM[LCD_RAMRegister_5] &= LCD_CLEAR_LSB;
      LCD->RAM[LCD_RAMRegister_5] |=  digit[1];
      LCD->RAM[LCD_RAMRegister_8] &= LCD_CLEAR_MSB;
      LCD->RAM[LCD_RAMRegister_8] |=   (uint8_t)(digit[2] << 4);
      LCD->RAM[LCD_RAMRegister_12] &= LCD_CLEAR_LSB;
      LCD->RAM[LCD_RAMRegister_12] |=  digit[3];
      break;

      /* Position 4 on LCD (Digit5)*/
    case 4:
      LCD->RAM[LCD_RAMRegister_1] &= LCD_CLEAR_LSB;
      LCD->RAM[LCD_RAMRegister_1] |=  digit[0];
      LCD->RAM[LCD_RAMRegister_4] &= LCD_CLEAR_MSB;
      LCD->RAM[LCD_RAMRegister_4] |=  (uint8_t)(digit[1] << 4);
      LCD->RAM[LCD_RAMRegister_8] &= LCD_CLEAR_LSB;
      LCD->RAM[LCD_RAMRegister_8] |=   digit[2];
      LCD->RAM[LCD_RAMRegister_11] &= LCD_CLEAR_MSB;
      LCD->RAM[LCD_RAMRegister_11] |=  (uint8_t)(digit[3] << 4);
      break;

      /* Position 5 on LCD (Digit6)*/
    case 5:
      LCD->RAM[LCD_RAMRegister_0] &= LCD_CLEAR_MSB;
      LCD->RAM[LCD_RAMRegister_0] |=  (uint8_t)(digit[0] << 4);
      LCD->RAM[LCD_RAMRegister_4] &= LCD_CLEAR_LSB;
      LCD->RAM[LCD_RAMRegister_4] |=  digit[1];
      LCD->RAM[LCD_RAMRegister_7] &= LCD_CLEAR_MSB;
      LCD->RAM[LCD_RAMRegister_7] |=   (uint8_t)(digit[2] << 4);
      LCD->RAM[LCD_RAMRegister_11] &= LCD_CLEAR_LSB;
      LCD->RAM[LCD_RAMRegister_11] |=  digit[3];
      break;

      /* Position 6 on LCD (Digit7)*/
    case 6:
      LCD->RAM[LCD_RAMRegister_0] &= LCD_CLEAR_LSB;
      LCD->RAM[LCD_RAMRegister_0] |=  digit[0];
      LCD->RAM[LCD_RAMRegister_3] &= LCD_CLEAR_MSB;
      LCD->RAM[LCD_RAMRegister_3] |=  (uint8_t)(digit[1] << 4);
      LCD->RAM[LCD_RAMRegister_7] &= LCD_CLEAR_LSB;
      LCD->RAM[LCD_RAMRegister_7] |=  digit[2];
      LCD->RAM[LCD_RAMRegister_10] &= LCD_CLEAR_MSB;
      LCD->RAM[LCD_RAMRegister_10] |=  (uint8_t)(digit[3] << 4);
      break;

    default:
      break;
  }
}

/**
  * @brief  This function writes a char in the LCD RAM.
  * @param  ptr: Pointer to string to display on the LCD Glass.
  * @retval None
  */
void LCD_GLASS_DisplayString(uint8_t* ptr)
{
  uint8_t i = 0x00;

  /* Send the string character by character on lCD */
  while ((*ptr != 0) & (i < 8))
  {
    /* Display one character on LCD */
    LCD_GLASS_WriteChar(ptr, POINT_OFF, APOSTROPHE_OFF, i);

    /* Point on the next character */
    ptr++;

    /* Increment the character counter */
    i++;
  }
}

/**
  * @brief  This function Clear a char in the LCD frame buffer.
  * @param  position: position in the LCD of the chacter to write [0:7]
  * @retval None
  */
void LCD_GLASS_ClearChar(uint8_t position)
{
  switch (position)
  {
      /* Position 0 on LCD (Digit1)*/
    case 0:
      LCD->RAM[LCD_RAMRegister_3]  &=  LCD_CLEAR_LSB;
      LCD->RAM[LCD_RAMRegister_6]  &=  LCD_CLEAR_MSB;
      LCD->RAM[LCD_RAMRegister_10] &=  LCD_CLEAR_LSB;
      LCD->RAM[LCD_RAMRegister_13] &=  LCD_CLEAR_MSB;
      break;

      /* Position 1 on LCD (Digit2)*/
    case 1:
      LCD->RAM[LCD_RAMRegister_2]  &=  LCD_CLEAR_MSB;
      LCD->RAM[LCD_RAMRegister_6]  &=  LCD_CLEAR_LSB;
      LCD->RAM[LCD_RAMRegister_9]  &=  LCD_CLEAR_MSB;
      LCD->RAM[LCD_RAMRegister_13] &=  LCD_CLEAR_LSB;
      break;

      /* Position 2 on LCD (Digit3)*/
    case 2:
      LCD->RAM[LCD_RAMRegister_2]  &=  LCD_CLEAR_LSB;
      LCD->RAM[LCD_RAMRegister_5]  &=  LCD_CLEAR_MSB;
      LCD->RAM[LCD_RAMRegister_9]  &=  LCD_CLEAR_LSB;
      LCD->RAM[LCD_RAMRegister_12] &=  LCD_CLEAR_MSB;
      break;

      /* Position 3 on LCD (Digit4)*/
    case 3:
      LCD->RAM[LCD_RAMRegister_1]  &=  LCD_CLEAR_MSB;
      LCD->RAM[LCD_RAMRegister_5]  &=  LCD_CLEAR_LSB;
      LCD->RAM[LCD_RAMRegister_8]  &=  LCD_CLEAR_MSB;
      LCD->RAM[LCD_RAMRegister_12] &=  LCD_CLEAR_LSB;
      break;

      /* Position 4 on LCD (Digit5)*/
    case 4:
      LCD->RAM[LCD_RAMRegister_1]  &=  LCD_CLEAR_LSB;
      LCD->RAM[LCD_RAMRegister_4]  &=  LCD_CLEAR_MSB;
      LCD->RAM[LCD_RAMRegister_8]  &=  LCD_CLEAR_LSB;
      LCD->RAM[LCD_RAMRegister_11] &=  LCD_CLEAR_MSB;
      break;

      /* Position 5 on LCD (Digit6)*/
    case 5:
      LCD->RAM[LCD_RAMRegister_0]  &=  LCD_CLEAR_MSB;
      LCD->RAM[LCD_RAMRegister_4]  &=  LCD_CLEAR_LSB;
      LCD->RAM[LCD_RAMRegister_7]  &=  LCD_CLEAR_MSB;
      LCD->RAM[LCD_RAMRegister_11] &=  LCD_CLEAR_LSB;
      break;

      /* Position 6 on LCD (Digit7)*/
    case 6:
      LCD->RAM[LCD_RAMRegister_0]  &=  LCD_CLEAR_LSB;
      LCD->RAM[LCD_RAMRegister_3]  &=  LCD_CLEAR_MSB;
      LCD->RAM[LCD_RAMRegister_7]  &=  LCD_CLEAR_LSB;
      LCD->RAM[LCD_RAMRegister_10] &=  LCD_CLEAR_MSB;
      break;

    default:
      break;
  }
}

/**
  * @brief  This function Clear the whole LCD RAM.
  * @param  None
  * @retval None
  */
void LCD_GLASS_Clear(void)
{
  uint8_t counter = 0;
  for (counter = 0;counter < 0x0E; counter++)
  {
    LCD->RAM[counter] =  LCD_RAM_RESET_VALUE;
  }
}
/**
  * @brief  Display a string in scrolling mode
  * @param  ptr: Pointer to string to display on the LCD Glass.
  * @param  nScroll: Specifies how many time the message will be scrolled
  * @param  ScrollSpeed : Speciifes the speed of the scroll, low value gives
  *         higher speed
  * @retval None
  * @par    Required preconditions: The LCD should be cleared before to start the
  *         write operation.
  */
void LCD_GLASS_ScrollString(uint8_t* ptr, uint16_t nScroll, uint16_t ScrollSpeed)
{
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
    LCD_GLASS_Clear();
    LCD_GLASS_DisplayString((uint8_t*)str);
    delay(ScrollSpeed);

    *(str + 1) = *(ptr1 + 6);
    *(str + 2) = *ptr1;
    *(str + 3) = *(ptr1 + 1);
    *(str + 4) = *(ptr1 + 2);
    *(str + 5) = *(ptr1 + 3);
    *(str + 6) = *(ptr1 + 4);
    *(str) = *(ptr1 + 5);
    LCD_GLASS_Clear();
    LCD_GLASS_DisplayString((uint8_t*)str);
    delay(ScrollSpeed);

    *(str + 1) = *(ptr1 + 5);
    *(str + 2) = *(ptr1 + 6);
    *(str + 3) = *ptr1;
    *(str + 4) = *(ptr1 + 1);
    *(str + 5) = *(ptr1 + 2);
    *(str + 6) = *(ptr1 + 3);
    *(str) = *(ptr1 + 4);
    LCD_GLASS_Clear();
    LCD_GLASS_DisplayString((uint8_t*)str);
    delay(ScrollSpeed);

    *(str + 1) = *(ptr1 + 4);
    *(str + 2) = *(ptr1 + 5);
    *(str + 3) = *(ptr1 + 6);
    *(str + 4) = *ptr1;
    *(str + 5) = *(ptr1 + 1);
    *(str + 6) = *(ptr1 + 2);
    *(str) = *(ptr1 + 3);
    LCD_GLASS_Clear();
    LCD_GLASS_DisplayString((uint8_t*)str);
    delay(ScrollSpeed);

    *(str + 1) = *(ptr1 + 3);
    *(str + 2) = *(ptr1 + 4);
    *(str + 3) = *(ptr1 + 5);
    *(str + 4) = *(ptr1 + 6);
    *(str + 5) = *ptr1;
    *(str + 6) = *(ptr1 + 1);
    *(str) = *(ptr1 + 2);
    LCD_GLASS_Clear();
    LCD_GLASS_DisplayString((uint8_t*)str);
    delay(ScrollSpeed);

    *(str + 1) = *(ptr1 + 2);
    *(str + 2) = *(ptr1 + 3);
    *(str + 3) = *(ptr1 + 4);
    *(str + 4) = *(ptr1 + 5);
    *(str + 5) = *(ptr1 + 6);
    *(str + 6) = *ptr1;
    *(str) = *(ptr1 + 1);
    LCD_GLASS_Clear();
    LCD_GLASS_DisplayString((uint8_t*)str);
    delay(ScrollSpeed);

    *(str + 1) = *(ptr1 + 1);
    *(str + 2) = *(ptr1 + 2);
    *(str + 3) = *(ptr1 + 3);
    *(str + 4) = *(ptr1 + 4);
    *(str + 5) = *(ptr1 + 5);
    *(str + 6) = *(ptr1 + 6);
    *(str) = *ptr1;
    LCD_GLASS_Clear();
    LCD_GLASS_DisplayString((uint8_t*)str);
    delay(ScrollSpeed);
  }
}

/**
  * @brief  Converts an ascii char to the a LCD digit (previous coding).
  * @param  c: a char to display.
  * @param  point: a point to add in front of char
  *         This parameter can be: POINT_OFF or POINT_ON
  * @param  apostrophe: flag indicating if a apostrophe has to be add in front
  *         of displayed character.
  *         This parameter can be: APOSTROPHE_OFF or APOSTROPHE_ON.
  * @retval None
  */
static void Convert(uint8_t* c, Point_Typedef point, Apostrophe_Typedef apostrophe)
{
  uint16_t ch = 0 , tmp = 0;
  uint8_t i;

  /* The character c is a letter in upper case*/
  if ((*c < 0x5B)&(*c > 0x40))
  {
    ch = LetterMap[*c-0x41];
  }
  /* The character c is a number*/
  if ((*c < 0x3A)&(*c > 0x2F))
  {
    ch = NumberMap[*c-0x30];
  }
  /* The character c is a space character */
  if (*c == 0x20)
  {
    ch = 0x00;
  }
  /* Set the DP seg in the character that can be displayed if the point is on */
  if (point == POINT_ON)
  {
    ch |= 0x0004;
  }

  /* Set the X seg in the character that can be displayed if the apostrophe is on */
  if (apostrophe == APOSTROPHE_ON)
  {
    ch |= 0x1000;
  }

  for (i = 0;i < 4; i++)
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
