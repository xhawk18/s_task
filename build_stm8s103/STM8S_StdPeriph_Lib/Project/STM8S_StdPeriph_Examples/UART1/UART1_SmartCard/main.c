/**
  ******************************************************************************
  * @file     UART1_SmartCard\main.c
  * @author   MCD Application Team
  * @version  V2.2.0
  * @date     30-September-2014
  * @brief    This file contains the main function for UART1 Smartcard example.
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

/**
  * @addtogroup UART1_SmartCard
  * @{
  */

/* Private define ------------------------------------------------------------*/
#define T0_PROTOCOL           0x00 /* T0 PROTOCOL */
#define SETUP_LENGHT          20
#define HIST_LENGHT           20
#define SC_Receive_Timeout    0x4000 /* direction to reader */

/* Private typedef -----------------------------------------------------------*/
/* ATR STRUCTURE - ANSWER TO RESET */
typedef  struct
{
    uint8_t TS; /* Bit Convention */
    uint8_t T0; /* high nibble = N. of setup byte; low nibble = N. of historical byte */
    uint8_t T[SETUP_LENGHT]; /* setup array */
    uint8_t H[HIST_LENGHT]; /* historical array */
    uint8_t Tlenght; /* setup array dimension */
    uint8_t Hlenght; /* historical array dimension */
} SC_ATR;

typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

SC_ATR  SC_A2R;
__IO uint32_t index = 0, Counter = 0;
__IO TestStatus ATRDecodeStatus = FAILED;
__IO uint32_t CardInserted = 0, CardProtocol = 1;
__IO uint8_t DST_Buffer[50]= {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                     0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void GPIO_Config(void);
void Multiplexer_EvalBoard_Config(void);
uint8_t SC_decode_Answer2reset(__IO uint8_t *card);

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{
    uint32_t i = 0;
    /* Configure the multiplexer on the evalboard to select the smartCard*/
    Multiplexer_EvalBoard_Config();

    /* Configure the GPIO ports */
    GPIO_Config();
    
    /*High speed internal clock prescaler: 1*/
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);

    /* Enable general interrupts */
    enableInterrupts();

    UART1_DeInit();
    /* UART1 configuration -------------------------------------------------------*/
    /* UART1 configured as follow:
          - Word Length = 9 Bits
          - 1.5 Stop Bit
          - Even parity
          - BaudRate = 10752 baud
          - Receive and transmit enabled
          - UART1 Clock enabled
    */
  UART1_Init((uint32_t)10752, UART1_WORDLENGTH_9D, UART1_STOPBITS_1_5, UART1_PARITY_EVEN,
              UART1_SYNCMODE_CLOCK_ENABLE, UART1_MODE_TXRX_ENABLE);

    /* UART1 Clock set to 4MHz (frequence master 16 MHZ / 4) */
    UART1_SetPrescaler(0x02);

    /* UART1 Guard Time set to  Bit */
    UART1_SetGuardTime(0x2);

    /* Enable the UART1 Parity Error Interrupt */
    UART1_ITConfig(UART1_IT_PE, ENABLE);

    /* Enable the NACK Transmission */
    UART1_SmartCardNACKCmd(ENABLE);

    /* Enable the Smart Card Interface */
    UART1_SmartCardCmd(ENABLE);

    /* Loop while no smart card is detected */
    while ((GPIO_ReadInputData(GPIOE)& 0x01) == 0x00)
    {
    }
    
    /* PG7 - SmartCard_/CMDVCC: low */
    GPIO_WriteLow(GPIOG, GPIO_PIN_7);  

    /* release SmartCard_RESET signal */
    GPIO_WriteLow(GPIOG, GPIO_PIN_5);  

    for (i = 0; i < 6000; i++)
    {
    }
    /* set SmartCard_RESET signal */
    GPIO_WriteHigh(GPIOG, GPIO_PIN_5);  


    /* Read Smart Card ATR response */
    for (index = 0; index < 40; index++)
    {
        Counter = 0;
        while ((UART1_GetFlagStatus(UART1_FLAG_RXNE) == RESET) && (Counter != SC_Receive_Timeout))
        {
            Counter++;
        }

        if (Counter != SC_Receive_Timeout)
        {
            DST_Buffer[index] = UART1_ReceiveData8();
        }
    }

    /* Decode ATR */
    CardProtocol = SC_decode_Answer2reset(DST_Buffer);

    /* Test if the inserted card is ISO7816-3 T=0 compatible */
    if (CardProtocol == 0)
    {
        /* Inserted card is ISO7816-3 T=0 compatible */
        ATRDecodeStatus = PASSED;
    }
    else
    {
        /* Inserted smart card is not ISO7816-3 T=0 compatible */
        ATRDecodeStatus = FAILED;
    }

    while (1)
  {}
}

/**
  * @brief  Configures the different GPIO ports.
  * @param  None
  * @retval None
  */
void GPIO_Config(void)
{
    /* Set PA5 as Output open-drain high-impedance level - SmartCard_IO(UART11_Tx)*/
    GPIO_Init(GPIOA, GPIO_PIN_5, GPIO_MODE_OUT_OD_HIZ_FAST);
    /* Set PG5 as Output push-pull low level - SmartCard_RESET*/
    GPIO_Init(GPIOG, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_FAST);
    /* Set PA6 as Output push-pull low level - SmartCard_CLK*/
    GPIO_Init(GPIOA, GPIO_PIN_6, GPIO_MODE_OUT_PP_LOW_FAST);
    /* Set PG4 as Output push-pull low level - SmartCard_5V/3V*/
    GPIO_Init(GPIOG, GPIO_PIN_4, GPIO_MODE_OUT_PP_LOW_FAST);
    /* Set PG7 as Output push-pull high level (inactive state) - SmartCard_/CMDVCC*/
    GPIO_Init(GPIOG, GPIO_PIN_7, GPIO_MODE_OUT_PP_HIGH_FAST);
    /* Set PE0 as Input pull-up, no external interrupt - SmartCard_OFF*/
    GPIO_Init(GPIOE, GPIO_PIN_0, GPIO_MODE_IN_PU_NO_IT);
    /* Set RSTIN HIGH */
    GPIO_WriteHigh(GPIOG, GPIO_PIN_5);

    /* Select Smart Card CMDVCC */
    GPIO_WriteHigh(GPIOG, GPIO_PIN_7);

    /* Select 5 V */
    GPIO_WriteHigh(GPIOG, GPIO_PIN_4);
}

/**
  * @brief Decode the Card ATR Response.
  * @param  None
  * @retval None
  */
uint8_t SC_decode_Answer2reset(__IO uint8_t *card)
{
    uint32_t i = 0, flag = 0, buf = 0, protocol = 0;

    SC_A2R.TS = 0;
    SC_A2R.T0 = 0;
    for (i = 0; i < SETUP_LENGHT; i++)
    {
        SC_A2R.T[i] = 0;
    }
    for (i = 0;i < HIST_LENGHT; i++)
    {
        SC_A2R.H[i] = 0;
    }
    SC_A2R.Tlenght = 0;
    SC_A2R.Hlenght = 0;

    SC_A2R.TS = card[0]; /* INITIAL CHARACTER */
    SC_A2R.T0 = card[1]; /* FORMAT CHARACTER */

    SC_A2R.Hlenght = (uint8_t)(SC_A2R.T0 & 0x0F);

    if ((SC_A2R.T0 & 0x80) == 0x80) flag = 1;

    for (i = 0; i < 4; i++)
    {
        SC_A2R.Tlenght = (uint8_t)(SC_A2R.Tlenght + (((SC_A2R.T0 & 0xF0) >> (4 + i)) & 0x1));
    }

    for (i = 0; i < SC_A2R.Tlenght; i++)
    {
        SC_A2R.T[i] = card[i + 2];
    }

    protocol = SC_A2R.T[SC_A2R.Tlenght - 1] & 0x0F;

    while (flag)
    {
        if ((SC_A2R.T[SC_A2R.Tlenght-1] & 0x80)== 0x80)
        {
            flag = 1;
        }
        else
        {
            flag = 0;
        }
        buf = SC_A2R.Tlenght;
        SC_A2R.Tlenght = 0;

        for (i = 0; i < 4; i++)
        {
            SC_A2R.Tlenght = (uint8_t)(SC_A2R.Tlenght + (((SC_A2R.T[buf - 1] & 0xF0) >> (4 + i)) & 0x1));
        }
        for (i = 0; i < SC_A2R.Tlenght; i++)
        {
            SC_A2R.T[buf + i] = card[i + 2 + buf];
        }
        SC_A2R.Tlenght += (uint8_t)buf;
    }

    for (i = 0;i < SC_A2R.Hlenght; i++)
    {
        SC_A2R.H[i] = card[i + 2 + SC_A2R.Tlenght];
    }

    return ((uint8_t)protocol);
}

/**
  * @brief  Configures the Multiplexer on the evalboard to select the smard card.
  * @param  None
  * @retval None
  */
void Multiplexer_EvalBoard_Config(void)
{
    /* Initialize I/Os in Output PP Mode */
    GPIO_Init(GPIOF, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_FAST);
    GPIO_Init(GPIOF, GPIO_PIN_6, GPIO_MODE_OUT_PP_HIGH_FAST);
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
