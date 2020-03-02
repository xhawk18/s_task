/**
  ******************************************************************************
  * @file     FLASH/FLASH_DataProgram/main.c
  * @author   MCD Application Team
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

/**
  * @addtogroup FLASH_DataProgram
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;
/* Private define ------------------------------------------------------------*/
#define BLOCK_OPERATION    0    /* block 0 in data eeprom memory: address is 0x4000 */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t GBuffer[FLASH_BLOCK_SIZE];
__IO TestStatus OperationStatus;

#ifdef _RAISONANCE_
/* needed by memcpy for raisonance */
 #include <string.h>
 extern int __address__FLASH_EraseBlock;
 extern int __size__FLASH_EraseBlock;
 extern int __address__FLASH_ProgramBlock;
 extern int __size__FLASH_ProgramBlock;
#endif /*_RAISONANCE_*/

/* Private function prototypes -----------------------------------------------*/
/* Declare _fctcpy function prototype as it is packaged by default in the Cosmic
   machine library */
#ifdef _COSMIC_
 int _fctcpy(char name);
#endif /*_COSMIC_*/
void Delay (uint16_t nCount);
static void CLK_Config(void);
static void GPIO_Config(void);
static void FLASH_Config(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{
  uint32_t add = 0, startadd = 0, stopadd = 0;
  uint8_t newval = 0xAA;
  uint8_t i = 0;

#ifdef _COSMIC_
/* Call the _fctcpy() function with the first segment character as parameter 
   "_fctcpy('F');"  for a manual copy of the declared moveable code segment
   (FLASH_CODE) in RAM before execution*/
  _fctcpy('F');
#endif /*_COSMIC_*/

#ifdef _RAISONANCE_
/* Call the standard C library: memcpy() or fmemcpy() functions available through 
   the <string.h> to copy the inram function to the RAM destination address */
  MEMCPY(FLASH_EraseBlock,
         (void PointerAttr*)&__address__FLASH_EraseBlock,
         (int)&__size__FLASH_EraseBlock);
  MEMCPY(FLASH_ProgramBlock,
         (void PointerAttr*)&__address__FLASH_ProgramBlock,
         (int)&__size__FLASH_ProgramBlock);
#endif /*_RAISONANCE_*/


  /* Clock configuration -----------------------------------------*/
  CLK_Config();

  /* GPIO Configuration ------------------------------------------*/
  GPIO_Config();

  /* FLASH Configuration ------------------------------------------*/
  FLASH_Config();

  /* Fill the buffer in RAM */
  for (i = 0; i < FLASH_BLOCK_SIZE; i++)
  {
    GBuffer[i] = newval;
  }
  /* This function is executed from RAM */
  FLASH_ProgramBlock(BLOCK_OPERATION, FLASH_MEMTYPE_DATA, FLASH_PROGRAMMODE_STANDARD, GBuffer);
  
  /* Wait until End of high voltage flag is set*/
  while (FLASH_GetFlagStatus(FLASH_FLAG_HVOFF) == RESET)
  {}
  /* Check the programmed block */
  startadd = FLASH_DATA_START_PHYSICAL_ADDRESS + ((uint16_t)BLOCK_OPERATION * (uint16_t)FLASH_BLOCK_SIZE);
  stopadd = startadd + (uint16_t)FLASH_BLOCK_SIZE;
  for (add = startadd; add < stopadd; add++)
      {
        if (FLASH_ReadByte(add) != newval)
        {
          /* Error */
          OperationStatus = FAILED;
          /* OperationStatus = PASSED, if the data written/read to/from Flash program memory is correct */
          /* OperationStatus = FAILED, if the data written/read to/from Flash program memory is corrupted */
          while (1)
          {
            STM_EVAL_LEDToggle(LED1); /*FAIL: write error */
            Delay(0xFFFF);
          }
        }
      }
  /* Erase block 0 and verify it */
  /* This function is executed from RAM */
  FLASH_EraseBlock(BLOCK_OPERATION, FLASH_MEMTYPE_DATA);

  /* Wait until End of high voltage flag is set*/
  while (FLASH_GetFlagStatus(FLASH_FLAG_HVOFF) == RESET)
  {}

  for (add = startadd; add < stopadd; add++)
      {
        if (FLASH_ReadByte(add) != 0x00)
        {
          /* Error */
          OperationStatus = FAILED;
          /* OperationStatus = PASSED, if the data written/read to/from Flash program memory is correct */
          /* OperationStatus = FAILED, if the data written/read to/from Flash program memory is corrupted */
          while (1)
          {
            STM_EVAL_LEDToggle(LED2); /* FAIL: Erase error */
            Delay(0xFFFF);
          }
        }
      }

  /* Pass */
  OperationStatus = PASSED;
  /* OperationStatus = PASSED, if the data written/read to/from Flash program memory is correct */
  /* OperationStatus = FAILED, if the data written/read to/from Flash program memory is corrupted */
  while (1)
  {
    STM_EVAL_LEDToggle(LED3); /* PASS: without errors*/
    Delay(0xFFFF);
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
  * @brief  Configure GPIO for LEDs available on the evaluation board
  * @param  None
  * @retval None
  */
void GPIO_Config(void)
{
    /* Initialize LEDs mounted on STM8-128 EVAL board */
    STM_EVAL_LEDInit(LED2);
    STM_EVAL_LEDInit(LED3);
    STM_EVAL_LEDInit(LED1);
		
		/* All leds are Off */
    STM_EVAL_LEDOff(LED2);
    STM_EVAL_LEDOff(LED3);
    STM_EVAL_LEDOff(LED1);
}

/**
  * @brief  Configure the FLASH for block programming
  * @param  None
  * @retval None
  */
void FLASH_Config(void)
{
 /* Define flash programming Time*/
  FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);

  FLASH_Unlock(FLASH_MEMTYPE_PROG);
  /* Wait until Flash Program area unlocked flag is set*/
  while (FLASH_GetFlagStatus(FLASH_FLAG_PUL) == RESET)
  {}

  /* Unlock flash data eeprom memory */
  FLASH_Unlock(FLASH_MEMTYPE_DATA);
  /* Wait until Data EEPROM area unlocked flag is set*/
  while (FLASH_GetFlagStatus(FLASH_FLAG_DUL) == RESET)
  {}
}

/**
  * @brief  Delay.
  * @param  nCount
  * @retval None
  */
void Delay(uint16_t nCount)
{
    /* Decrement nCount value */
    while (nCount != 0)
    {
        nCount--;
    }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/