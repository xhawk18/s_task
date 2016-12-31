/**
  ******************************************************************************
  * @file    CAN_Networking\main.c
  * @author  MCD Application Team
  * @version  V2.2.0
  * @date     30-September-2014
  * @brief   This file contains the main function for CAN networking example.
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
/**
  * @addtogroup CAN_Networking
  * @{
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* Private defines -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t Key_Pressed_Number = 0x00;
__IO uint8_t Key_status = 0x00;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */  

void main(void)
{
  CAN_TxStatus_TypeDef status = CAN_TxStatus_Failed;
  
  /* Transmit Parameters */
  CAN_Id_TypeDef Tx_IDE = CAN_Id_Standard;
  CAN_RTR_TypeDef Tx_RTR = CAN_RTR_Data;
  uint8_t Tx_DLC = 0;
  uint8_t Tx_Data[8] = {0};
  uint32_t Tx_Id = 0;
  
  /* Clock configuration --------------------------------------*/
  CLK_Config();
  
  /* GPIO Configuration ---------------------------------------*/
  GPIO_Config();
    
  /* Configure LCD mounted on STM8-128 EVAL board -------------*/
  LCD_Config();
  
  /* CAN configuration ----------------------------------------*/
  CAN_Config();
  
  /* Enable Interrupts*/
  enableInterrupts();

  /* Infinite loop*/
  while(1)
  {
    while(Key_status != Key_NoPressed)
     {
    
      if(Key_Pressed_Number == 0x0) 
      {
          Key_Pressed_Number = 0x03;
      }
      else
      {
          Key_Pressed_Number--;
      }
      
      /* Sender Display*/
      LED_Display(Key_Pressed_Number);
      LCD_Display(Key_Pressed_Number);
      Delay(TIME);
      
      /* Transmit Parameters*/
      Tx_Id = 0x321;
      Tx_IDE = CAN_Id_Standard;
      Tx_RTR = CAN_RTR_Data;
      Tx_DLC = 1;
      Tx_Data[0] = Key_Pressed_Number;
      
      /* Sender send Frame */
      status = CAN_Transmit(Tx_Id,Tx_IDE,Tx_RTR,Tx_DLC,Tx_Data);
      
      /* while key is not pressed, loop*/
      Key_status= Key_NoPressed;
   }
  }
}
/**
  * @brief  Configure system clock to run at 16Mhz
  * @param  None
  * @retval None
  */
void CLK_Config(void)
{
  /* Initialization of the clock */
  /* Clock divider to HSI/1 */
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
}

/**
  * @brief  Configures the CAN mode and filter
  * @param  None
  * @retval None
  */
void CAN_Config(void)
{
  CAN_InitStatus_TypeDef status = CAN_InitStatus_Failed;
  
  /* Filter Parameters */
  CAN_FilterNumber_TypeDef CAN_FilterNumber;
  FunctionalState CAN_FilterActivation;
  CAN_FilterMode_TypeDef CAN_FilterMode;
  CAN_FilterScale_TypeDef CAN_FilterScale;
  uint8_t CAN_FilterID1;
  uint8_t CAN_FilterID2;
  uint8_t CAN_FilterID3;
  uint8_t CAN_FilterID4;
  uint8_t CAN_FilterIDMask1;
  uint8_t CAN_FilterIDMask2;
  uint8_t CAN_FilterIDMask3;
  uint8_t CAN_FilterIDMask4; 
  
  /* Init Parameters*/
  CAN_MasterCtrl_TypeDef CAN_MasterCtrl;
  CAN_Mode_TypeDef CAN_Mode;
  CAN_SynJumpWidth_TypeDef CAN_SynJumpWidth;
  CAN_BitSeg1_TypeDef CAN_BitSeg1;
  CAN_BitSeg2_TypeDef CAN_BitSeg2;
  uint8_t CAN_Prescaler;  
  
  /* CAN register init */
  CAN_DeInit();
    
  /* CAN  init */
  CAN_MasterCtrl=CAN_MasterCtrl_AllDisabled;
  CAN_Mode = CAN_Mode_Normal;
  CAN_SynJumpWidth = CAN_SynJumpWidth_1TimeQuantum;
  CAN_BitSeg1 = CAN_BitSeg1_8TimeQuantum;
  CAN_BitSeg2 = CAN_BitSeg2_7TimeQuantum;
  CAN_Prescaler = 1;
  status = CAN_Init(CAN_MasterCtrl, CAN_Mode, CAN_SynJumpWidth, CAN_BitSeg1, \
                    CAN_BitSeg2, CAN_Prescaler);

  /* CAN filter init */
  CAN_FilterNumber = CAN_FilterNumber_0;
  CAN_FilterActivation = ENABLE;
  CAN_FilterMode = CAN_FilterMode_IdMask;
  CAN_FilterScale = CAN_FilterScale_32Bit;
  CAN_FilterID1=0;  
  CAN_FilterID2=0;
  CAN_FilterID3=0;
  CAN_FilterID4=0;
  CAN_FilterIDMask1=0;
  CAN_FilterIDMask2=0;
  CAN_FilterIDMask3=0;
  CAN_FilterIDMask4=0;  
  CAN_FilterInit(CAN_FilterNumber, CAN_FilterActivation, CAN_FilterMode, 
                 CAN_FilterScale,CAN_FilterID1, CAN_FilterID2, CAN_FilterID3,
                 CAN_FilterID4,CAN_FilterIDMask1, CAN_FilterIDMask2, 
                 CAN_FilterIDMask3, CAN_FilterIDMask4);
        
  /* Enable Fifo message pending interrupt*/
  /* Message reception is done by CAN_RX ISR*/
  CAN_ITConfig(CAN_IT_FMP, ENABLE);
}

/**
  * @brief  Configure GPIO for LEDs and buttons available on the evaluation board
  * @param  None
  * @retval None
  */
void GPIO_Config(void)
{
  /* Initialize LEDs mounted on STM8-128 EVAL board */
    STM_EVAL_LEDInit(LED1);
    STM_EVAL_LEDInit(LED2);
    STM_EVAL_LEDInit(LED3);
    STM_EVAL_LEDInit(LED4);
    
    /* Initialize KEY push-button mounted on STM8-128 EVAL board */
    STM_EVAL_PBInit(BUTTON_KEY, BUTTON_MODE_EXTI);
  
  /* Switch LEDs Off */
    STM_EVAL_LEDOff(LED1);
    STM_EVAL_LEDOff(LED2);
    STM_EVAL_LEDOff(LED3);
    STM_EVAL_LEDOff(LED4);
}

/**
  * @brief  Configure the LCD available on the evaluation board
  * @param  None
  * @retval None
  */
void LCD_Config(void)
{
    /* Init the Eval board LCD */
    STM8S_EVAL_LCD_Init();

    /* Clear LCD */
    LCD_Clear();
    
    /* Display the welcome text on LCD */
    LCD_SetCursorPos(LCD_LINE1, 0);
    LCD_Print("   STM8S CAN    ");
    /* Print "Press Key" on LCD line2*/
    LCD_SetCursorPos(LCD_LINE2, 0);
    LCD_Print("   Press Key    ");
}

/**
  * @brief Display led status 
  * @param Ledstatus the led to set ON, other leds are set OFF
  * @retval None
  */
void LED_Display(uint8_t Ledstatus)
{
  /* Switch LEDs Off */
    STM_EVAL_LEDOff(LED1);
    STM_EVAL_LEDOff(LED2);
    STM_EVAL_LEDOff(LED3);
    STM_EVAL_LEDOff(LED4);

 /* Set the led ON depending on the value of Ledstatus*/
 switch((uint8_t)Ledstatus)
 {
   case(0): STM_EVAL_LEDOn(LED4);
            break;
   
   case(1): STM_EVAL_LEDOn(LED3);
            break;
 
   case(2): STM_EVAL_LEDOn(LED2);
            break;

   case(3): STM_EVAL_LEDOn(LED1);
            break;
   }
}

/**
  * @brief Display led status and communication direction on the LCD 
  * @param Ledstatus the led status
  * @retval None
  */
void LCD_Display(uint8_t Ledstatus)
{
  LCD_SetCursorPos(LCD_LINE2, 0);
    
  switch((uint8_t)Ledstatus)
 {
   case(0): LCD_Print("Sender   LD4 ON "); 
            break;
   
   case(1): LCD_Print("Sender   LD3 ON ");
            break;
 
   case(2): LCD_Print("Sender   LD2 ON ");
            break;

   case(3): LCD_Print("Sender   LD1 ON ");
            break;
            
   case(4): LCD_Print("Receiver LD4 ON "); 
            break;
   
   case(5): LCD_Print("Receiver LD3 ON ");
            break;
 
   case(6): LCD_Print("Receiver LD2 ON ");
            break;

   case(7): LCD_Print("Receiver LD1 ON ");
            break;
  }
}

/**
  * @brief Generates a delay time.
  * @param nCount the value for looping.
  * @retval None
  */
void Delay (uint16_t nCount)
{
  /* Decrement nCount value */
  for (; nCount != 0; nCount--);
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
