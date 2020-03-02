/**
  ******************************************************************************
  * @file     WWDG_Example\main.c
  * @author   MCD Application Team
  * @version  V2.2.0
  * @date     30-September-2014
  * @brief    This file contains the main function for wwdg Rearm Reset example.
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
  * @addtogroup WWDG_Example
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define WINDOW_VALUE        97
#define COUNTER_INIT       104

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO FunctionalState NonAlowedRefresh = DISABLE;
__IO FunctionalState AllowedRefresh = ENABLE;
uint8_t Index;
/* Private function prototypes -----------------------------------------------*/
void Delay (uint16_t nCount);
static void GPIO_Config(void);
static void WWDG_Config(void);
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{

  /* GPIO Configuration */
  GPIO_Config();

  /* Check if the MCU has resumed from WWDG reset */
  if (RST_GetFlagStatus(RST_FLAG_WWDGF) != RESET)
  {
    /* WWDGF flag set */
    /* Toggle LED1 */
    for (Index = 7; Index != 0; Index--)
    {
      STM_EVAL_LEDToggle(LED1);
      Delay(0x7FFF);
    }
    /* Clear WWDGF Flag */
    RST_ClearFlag(RST_FLAG_WWDGF);
  }

  /* WWDG Configuration */
  WWDG_Config();
  
  while (1)
  {
    /* Check if WWDG counter refresh is allowed in Allowed window */
    if (AllowedRefresh != DISABLE)
    {
      /* get WWDG counter value */
      /* wait until WWDG counter becomes lower than window value */
      while ((WWDG_GetCounter() & 0x7F) > WINDOW_VALUE);
      /* Refresh WWDG counter during allowed window so no MCU reset will occur */
      WWDG_SetCounter(COUNTER_INIT);
    }

    /* Check if WWDG counter refresh is allowed in non Allowed window */
    if (NonAlowedRefresh != DISABLE)
    {
      /* wait until WWDG counter becomes higher than window value */
      while ((WWDG_GetCounter() & 0x7F) < WINDOW_VALUE);
      /* Refresh WWDG counter during non allowed window so MCU reset will occur */
      WWDG_SetCounter(COUNTER_INIT);
    }
    /* Toggle LED2 */
    STM_EVAL_LEDToggle(LED2);
    Delay(0x6FFF);
  }
}

/**
  * @brief  Inserts a delay time.
  * @param  nCount: specifies the delay time length.
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


/**
  * @brief Configures the WWDG to generate a Reset if the WWDG is not refreshed 
  *         during the correct window.
  * @param  None
  * @retval None
  */

static void WWDG_Config(void) 
{
  /* WWDG configuration: WWDG is clocked by SYSCLK = 2MHz */
  /* WWDG timeout is equal to 251,9 ms */
  /* Watchdog Window = (COUNTER_INIT - 63) * 1 step
                     = 41 * (12288 / 2Mhz)
                     = 251,9 ms
  */
  /* Non Allowed Window = (COUNTER_INIT - WINDOW_VALUE) * 1 step
                        = (104 - 97) * 1 step
                        =  7 * 1 step 
                        =  7 * (12288 / 2Mhz) 
                        =  43.008 ms
   */
  /* So the non allowed window starts from 0.0 ms to 43.008 ms
  and the allowed window starts from 43.008 ms to 251,9 ms
  If refresh is done during non allowed window, a reset will occur.
  If refresh is done during allowed window, no reset will occur.
  If the WWDG down counter reaches 63, a reset will occur. */
  WWDG_Init(COUNTER_INIT, WINDOW_VALUE);
}

/**
  * @brief  Configures the Leds and the Buttons IO.
  * @param  None
  * @retval None
  */
static void GPIO_Config(void) 
{

  /* Initialize LED1 and LED2 mounted on the evaluation board */
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);

  STM_EVAL_LEDOff(LED1);
  STM_EVAL_LEDOff(LED2);

  /* Initialize Key and Joystick down push-buttons mounted on the evaluation board */
  STM_EVAL_PBInit(BUTTON_KEY, BUTTON_MODE_EXTI);
  STM_EVAL_PBInit(BUTTON_DOWN, BUTTON_MODE_EXTI);

  /* enable interrupts */
  enableInterrupts();
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
