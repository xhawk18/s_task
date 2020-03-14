/**
  ******************************************************************************
  * @file    UART1_HalfDuplex\main.c
  * @author  MCD Application Team
  * @version  V2.2.0
  * @date     30-September-2014
  * @brief   This file contains the main function for UART1 in Half-Duplex mode example.
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
  * @addtogroup UART1_HalfDuplex
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/
#define TxBufferSize1   (countof(TxBuffer1) - 1)
#define TxBufferSize2   (countof(TxBuffer2) - 1)
/* Private macro -------------------------------------------------------------*/
#define countof(a)   (sizeof(a) / sizeof(*(a)))
/* To run the transmitter connect the UART1_TX on the UART3_RX, uncomment the line 
   "#define transmitter" and comment the line "#define receiver" */
#define transmitter 

/* To run the transmitter connect the UART1_TX on the UART3_TX , uncomment the line 
  "#define receiver" and comment the line "#define transmitter" */
#define receiver 
/* Private variables ---------------------------------------------------------*/
uint8_t TxBuffer1[] = "HalfDuplex Example: UART1 -> UART3 using HalfDuplex mode";
uint8_t TxBuffer2[] = "HalfDuplex Example: UART3 -> UART1 using HalfDuplex mode";
uint8_t RxBuffer1[TxBufferSize2]={0};
uint8_t RxBuffer2[TxBufferSize1]={0};
uint8_t NbrOfDataToRead1 = TxBufferSize2;
uint8_t NbrOfDataToRead2 = TxBufferSize1;
uint8_t TxCounter1 = 0, RxCounter1 = 0;
uint8_t TxCounter2 = 0, RxCounter2 = 0;
__IO TestStatus TransferStatus1 = FAILED, TransferStatus2 = FAILED;

/* Private function prototypes -----------------------------------------------*/
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);
static void CLK_Config(void);
static void UART_Config(void);
static void GPIO_Config(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{
  /* GPIO configuration -----------------------------------------*/
  GPIO_Config();

  /* CLK configuration -----------------------------------------*/
  CLK_Config();

  /* UART configuration -----------------------------------------*/
  UART_Config();  

#ifdef transmitter

    while (NbrOfDataToRead2--)
    {
        /* Wait until end of transmit */
        while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET)
        {
        }
        /* Write one byte in the UART1 Transmit Data Register */
        UART1_SendData8(TxBuffer1[TxCounter1++]);

        /* Wait the byte is entirely received by UART3 */
        while (UART3_GetFlagStatus(UART3_FLAG_RXNE) == RESET)
        {
        }
        /* Store the received byte in the RxBuffer2 */
        RxBuffer2[RxCounter2++] = UART3_ReceiveData8();
    }
    /* Check the received data with the send ones */

    TransferStatus1 = Buffercmp(TxBuffer1, RxBuffer2, TxBufferSize1);
    /* TransferStatus = PASSED, if the data transmitted from UART1 and received 
       by UART3 are the same */
    /* TransferStatus = FAILED, if the data transmitted from UART1 and received 
       by UART3 are different */
#endif
#ifdef receiver
    /* Clear the UART1 Data Register */
    UART1_ReceiveData8();

    while (NbrOfDataToRead1--)
    {
        /* Wait until end of transmit */
        while (UART3_GetFlagStatus(UART3_FLAG_TXE)== RESET)
        {
        }
        /* Write one byte in the UART3 Transmit Data Register */
        UART3_SendData8(TxBuffer2[TxCounter2++]);

        /* Wait the byte is entirely received by UART1 */
        while (UART1_GetFlagStatus(UART1_FLAG_RXNE) == RESET)
        {
        }
        /* Store the received byte in the RxBuffer1 */
        RxBuffer1[RxCounter1++] = UART1_ReceiveData8();
    }

    /* Check the received data with the send ones */
    TransferStatus2 = Buffercmp(TxBuffer2, RxBuffer1, TxBufferSize2);
    /* TransferStatus = PASSED, if the data transmitted from UART3 and received 
       by UART1 are the same */
    /* TransferStatus = FAILED, if the data transmitted from UART3 and received 
       by UART1 are different */
#endif
    while (1)
  {}
}

/**
  * @brief  Configure system clock to run at 16Mhz
  * @param  None
  * @retval None
  */
static void CLK_Config(void)
{
    /* Initialization of the clock */
    /* Clock divider to HSI/1 */
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
}

/**
  * @brief  Configure UART1 Tx pin as Output open-drain high-impedance level 
  * @param  None
  * @retval None
  */
static void GPIO_Config(void)
{
    /* Set PA5 as Output open-drain high-impedance level (UART1_Tx)*/
    GPIO_Init(GPIOA, GPIO_PIN_5, GPIO_MODE_OUT_OD_HIZ_FAST);
}

/**
  * @brief  UART1 and UART3 Configuration for half duplex communication
  * @param  None
  * @retval None
  */
static void UART_Config(void)
{
  /* UART1 and UART3 configured as follow:
        - BaudRate = 230400 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Receive and transmit enabled
  */
  UART1_DeInit();
  UART1_Init((uint32_t)230400, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO,
              UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);

  UART3_DeInit();
  UART3_Init((uint32_t)230400, UART3_WORDLENGTH_8D, UART3_STOPBITS_1, UART3_PARITY_NO,
              UART3_MODE_TXRX_ENABLE);
   
  /* Enable UART1 Half Duplex Mode*/
  UART1_HalfDuplexCmd(ENABLE);
}

/**
  * @brief Compares two buffers.
  * @param pBuffer1 First buffer to be compared.
  * @param pBuffer2 Second buffer to be compared.
  * @param BufferLength Buffer's length
  * @retval TestStatus Status of buffer comparison
  * - PASSED: pBuffer1 identical to pBuffer2
  * - FAILED: pBuffer1 differs from pBuffer2
  * @par Required preconditions:
  * None
  */
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
    while (BufferLength--)
    {
        if (*pBuffer1 != *pBuffer2)
        {
            return FAILED;
        }

        pBuffer1++;
        pBuffer2++;
    }

    return PASSED;
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
