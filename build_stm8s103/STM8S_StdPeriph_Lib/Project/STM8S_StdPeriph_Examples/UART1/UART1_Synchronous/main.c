/**
  ******************************************************************************
  * @file    UART1_Synchronous\main.c
  * @brief   This file contains the main function for UART1 Synchronous mode example.
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

/**
  * @addtogroup UART1_Synchronous
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;
/* Private define ------------------------------------------------------------*/
#define TxBufferSize1   (countof(TxBuffer1) - 1)
#define TxBufferSize2   (countof(TxBuffer2) - 1)
/* Private macro -------------------------------------------------------------*/
#define countof(a)   (sizeof(a) / sizeof(*(a)))
/* Private variables ---------------------------------------------------------*/
uint8_t TxBuffer1[] = "UART1 Example: UART1 -> SPI using TXE and RXNE Flags";
uint8_t TxBuffer2[] = "UART1 Example: SPI -> UART1 using TXE and RXNE Flags";
uint8_t RxBuffer1[TxBufferSize2] = {0};
uint8_t RxBuffer2[TxBufferSize1] = {0};
uint8_t NbrOfDataToRead = TxBufferSize1;
__IO uint8_t TxCounter = 0, RxCounter = 0;
__IO TestStatus TransferStatus1 = FAILED, TransferStatus2 = FAILED;

/* Private function prototypes -----------------------------------------------*/
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);
void Delay (uint16_t nCount);
static void CLK_Config(void);
static void UART1_Config(void);
static void SPI_Config(void);
/* Private functions ----------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{
  /* Clock configuration -----------------------------------------*/
  CLK_Config();  

  /* UART1 configuration -----------------------------------------*/
  UART1_Config();
 
  /* SPI configuration -----------------------------------------*/
  SPI_Config();  
  
  /* Enable the UART1*/
  UART1_Cmd(ENABLE);
  
  Delay(0xFFF);
  
  /* Enable the SPI*/
  SPI_Cmd(ENABLE);
  
    while (NbrOfDataToRead--)
    {
        /* Wait until end of transmit */
        while (SPI_GetFlagStatus(SPI_FLAG_TXE)== RESET)
        {
        }
        /* Write one byte in the SPI Transmit Data Register */
        SPI_SendData(TxBuffer2[TxCounter]);
        /* Write one byte in the UART1 Transmit Data Register */
        UART1_SendData8(TxBuffer1[TxCounter++]);
        /* Wait until end of transmit */
        while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET)
        {
        }
        /* Wait the byte is entirely received by UART1 */
        while (UART1_GetFlagStatus(UART1_FLAG_RXNE) == RESET)
        {
        }
        /* Store the received byte in the RxBuffer1 */
        RxBuffer1[RxCounter] = UART1_ReceiveData8();
        /* Wait the byte is entirely received by SPI */
        while (SPI_GetFlagStatus(SPI_FLAG_RXNE) == RESET)
        {
        }
        /* Store the received byte in the RxBuffer2 */
        RxBuffer2[RxCounter++] = SPI_ReceiveData();
    }

    /* Check the received data with the sent ones */
    TransferStatus1 = Buffercmp(TxBuffer1, RxBuffer2, TxBufferSize1);
    /* TransferStatus = PASSED, if the data transmitted from UART1 and received by SPI are the same */
    /* TransferStatus = FAILED, if the data transmitted from UART1 and received by SPI are different */
    TransferStatus2 = Buffercmp(TxBuffer2, RxBuffer1, TxBufferSize2);
    /* TransferStatus = PASSED, if the data transmitted from SPI and received by UART1 are the same */
    /* TransferStatus = FAILED, if the data transmitted from SPI and received by UART1 are different */

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
  * @brief  Configure UART1 for the synchronous mode communication
  * @param  None
  * @retval None
  */
static void UART1_Config(void)
{
  /* UART1 configured as follow:
          - Word Length = 8 Bits
          - 1 Stop Bit
          - No parity
          - BaudRate = 9600 baud
          - UART1 Clock enabled
          - Polarity Low
          - Phase Middle
          - Last Bit enabled
          - Receive and transmit enabled
   */
  UART1_DeInit();
  UART1_Init((uint32_t)9600, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO, 
              (UART1_SyncMode_TypeDef)(UART1_SYNCMODE_CLOCK_ENABLE | UART1_SYNCMODE_CPOL_LOW |UART1_SYNCMODE_CPHA_MIDDLE |UART1_SYNCMODE_LASTBIT_ENABLE),
              UART1_MODE_TXRX_ENABLE);
  UART1_Cmd(DISABLE);
}

/**
  * @brief  Configure SPI for the full duplex communication with UART1
  * @param  None
  * @retval None
  */
static void SPI_Config(void)
{
  SPI_DeInit();
  /* Initialize SPI in Slave mode  */
  SPI_Init(SPI_FIRSTBIT_LSB, SPI_BAUDRATEPRESCALER_2, SPI_MODE_SLAVE, SPI_CLOCKPOLARITY_LOW,
           SPI_CLOCKPHASE_1EDGE, SPI_DATADIRECTION_2LINES_FULLDUPLEX, SPI_NSS_SOFT,(uint8_t)0x07);
}

/**
  * @brief   Compares two buffers.
  * @param   pBuffer1 First buffer to be compared.
  * @param   pBuffer2 Second buffer to be compared.
  * @param   BufferLength Buffer's length
  * @retval  TestStatus Status of buffer comparison
  * - PASSED: pBuffer1 identical to pBuffer2
  * - FAILED: pBuffer1 differs from pBuffer2
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
