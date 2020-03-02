/**
  ******************************************************************************
  * @file    CLK_ClockSelection\main.c
  * @brief   This file contains the main function for the CLK Clock Selection example.
  * @author  MCD Application Team
  * @version  V2.2.0
  * @date     30-September-2014
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

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "stm8s_eval.h"
#include "stm8s_eval_lcd.h"

/**
  * @addtogroup CLK_ClockSelection
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint8_t OscillatorStatus = 0x00;
/* Private function prototypes -----------------------------------------------*/
static void CLK_Config(void);
static void GPIO_Config(void);
static void LCD_Config(void);
/* Public functions ----------------------------------------------------------*/

/**
  * @brief Example  main entry point.
  * @param  None
  * @retval None
  */
void main(void)
{
  uint8_t state = 0x00;
  
  /* GPIO Configuration  -----------------------------------------------------*/
  GPIO_Config();  
  
  /* Configure LCD mounted on STM8-128 EVAL board ----------------------------*/
  LCD_Config();
  
  /* Clock configuration -----------------------------------------------------*/
  CLK_Config();
  
  enableInterrupts();
  
  while (1)
  {
    while(state == OscillatorStatus)
    {}
    state = OscillatorStatus; /* Update the selected master clock oscillator */
    
    /* joystick right */
    if(OscillatorStatus == 0x00)
    {
      CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSI, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);
      
      /* Print on LCD line2*/
      LCD_SetCursorPos(LCD_LINE2, 0);
      LCD_Print("   CLK = HSI    ");
    }
    
    /* joystick up */
    if(OscillatorStatus == 0x01)
    {
      CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSE, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);
      
      /* Print on LCD line2*/
      LCD_SetCursorPos(LCD_LINE2, 0);
      LCD_Print("   CLK = HSE    ");
    }
    
    /* joystick down */
    if(OscillatorStatus == 0x02)
    {
      CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_LSI, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);
      
      /* Print on LCD line2*/
      LCD_SetCursorPos(LCD_LINE2, 0);
      LCD_Print("   CLK = LSI    ");
    }
  }
}

/**
  * @brief  Configure GPIO for buttons available on the evaluation board
  * @param  None
  * @retval None
  */
static void GPIO_Config(void)
{
  /* Configure GPIO used to drive the joystick
  JOYSTICK_UP    --> PB6
  JOYSTICK_DOWN  --> PB7
  JOYSTICK_RIGHT --> PB5
  */
  STM_EVAL_PBInit(BUTTON_UP, BUTTON_MODE_EXTI);
  STM_EVAL_PBInit(BUTTON_DOWN, BUTTON_MODE_EXTI);
  STM_EVAL_PBInit(BUTTON_RIGHT, BUTTON_MODE_EXTI);
}

/**
  * @brief  Configure system clock to run at 16Mhz and output the system clock on
  *         CCO pin
  * @param  None
  * @retval None
  */
static void CLK_Config(void)
{
  CLK_DeInit();
  
  /* Clock divider to HSI/1 */
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
  
  /* Output Fcpu on CLK_CCO pin */
  CLK_CCOConfig(CLK_OUTPUT_MASTER);
}

/**
  * @brief  Configure the LCD available on the evaluation board
  * @param  None
  * @retval None
  */
static void LCD_Config(void)
{
  /* Init the Eval board LCD */
  STM8S_EVAL_LCD_Init();
  
  /* Clear LCD */
  LCD_Clear();
  
  /* Print on LCD line1*/
  LCD_SetCursorPos(LCD_LINE1, 0);
  LCD_Print("Clock Selection");
  /* Print on LCD line2*/
  LCD_SetCursorPos(LCD_LINE2, 0);
  LCD_Print("  Use joystick  ");
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/