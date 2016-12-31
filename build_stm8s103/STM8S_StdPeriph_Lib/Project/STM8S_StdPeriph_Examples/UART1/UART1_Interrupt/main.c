/**
  ******************************************************************************
  * @file UART1_Interrupt\main.c
  * @brief This file contains the main function for a basic communication between UART1 and UART3 using interrupts.
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
#include "main.h"

/**
  * @addtogroup UART1_Interrupt
  * @{
  */
/* Private typedef -----------------------------------------------------------*/
typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;
/* Private variables ---------------------------------------------------------*/
uint8_t TxBuffer1[] = "UART1 Interrupt Example: UART1 -> UART3 using Interrupt";
uint8_t TxBuffer2[] = "UART1 Interrupt Example: UART3 -> UART1 using Interrupt";  
uint8_t RxBuffer1[RxBufferSize1];
uint8_t RxBuffer2[RxBufferSize2];
__IO uint8_t TxCounter1 = 0x00;
__IO uint8_t TxCounter2 = 0x00;
__IO uint8_t RxCounter1 = 0x00;
__IO uint8_t RxCounter2 = 0x00;
uint8_t NbrOfDataToTransfer1 = TxBufferSize1;
uint8_t NbrOfDataToTransfer2 = TxBufferSize2;
uint8_t NbrOfDataToRead1 = RxBufferSize1;
uint8_t NbrOfDataToRead2 = RxBufferSize2;
__IO TestStatus TransferStatus1 = FAILED;
__IO TestStatus TransferStatus2 = FAILED;

/* Private function prototypes -----------------------------------------------*/
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);
static void CLK_Config(void);
static void UART_Config(void);

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{
  /* CLK configuration -----------------------------------------*/
  CLK_Config();

  /* UART configuration -----------------------------------------*/
  UART_Config();  

    /* Wait until end of transmission from UART1 to UART3 */
    while (GetVar_RxCounter2() < GetVar_NbrOfDataToTransfer1())
    {
    }
    /* Enable UART1 Receive and UART3 Transmit interrupt */
    UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);
    UART3_ITConfig(UART3_IT_TXE, ENABLE);
    /* Wait until end of transmission from UART3 to UART1 */
    while (GetVar_RxCounter1() < GetVar_NbrOfDataToTransfer2())
    {
    }

    /* Check the received data with the send ones */
    TransferStatus1 = Buffercmp(TxBuffer2, RxBuffer1, RxBufferSize1);
    /* TransferStatus1 = PASSED, if the data transmitted from UART3 and
       received by UART1 are the same */
    /* TransferStatus1 = FAILED, if the data transmitted from UART3 and
       received by UART1 are different */
    TransferStatus2 = Buffercmp(TxBuffer1, RxBuffer2, RxBufferSize2);
    /* TransferStatus2 = PASSED, if the data transmitted from UART1 and
       received by UART3 are the same */
    /* TransferStatus2 = FAILED, if the data transmitted from UART1 and
       received by UART3 are different */

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
  * @brief  UART1 and UART3 Configuration for interrupt communication
  * @param  None
  * @retval None
  */
static void UART_Config(void)
{
  /* Deinitializes the UART1 and UART3 peripheral */
    UART1_DeInit();
    UART3_DeInit();
    /* UART1 and UART3 configuration -------------------------------------------------*/
    /* UART1 and UART3 configured as follow:
          - BaudRate = 9600 baud  
          - Word Length = 8 Bits
          - One Stop Bit
          - No parity
          - Receive and transmit enabled
          - UART1 Clock disabled
     */
    /* Configure the UART1 */
    UART1_Init((uint32_t)9600, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO,
                UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);
    
    /* Enable UART1 Transmit interrupt*/
    UART1_ITConfig(UART1_IT_TXE, ENABLE);
    
    /* Configure the UART3 */
 		UART3_Init((uint32_t)9600, UART3_WORDLENGTH_8D, UART3_STOPBITS_1, UART3_PARITY_NO,
                UART3_MODE_TXRX_ENABLE);

    /* Enable UART3 Receive interrupt */
    UART3_ITConfig(UART3_IT_RXNE_OR, ENABLE);
    
    /* Enable general interrupts */
    enableInterrupts();    
}

/**
  * @brief Compares two buffers.
  * @param[in] pBuffer1 First buffer to be compared.
  * @param[in] pBuffer2 Second buffer to be compared.
  * @param[in] BufferLength Buffer's length
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
/**
  * @brief Returns TxCounter1 value.
  * @param None
  * @retval Transmit counter index1
  * - uint8_t: TxCounter1
  * @par Required preconditions:
  * None
  */
uint8_t GetVar_TxCounter1(void)
{
return TxCounter1;
}
/**
  * @brief Returns TxCounter2 value.
  * @param None
  * @retval Transmit counter index2
  * - uint8_t: TxCounter2
  * @par Required preconditions:
  * None
  */
uint8_t GetVar_TxCounter2(void)
{
return TxCounter2;
}
/**
  * @brief Returns RxCounter1 value.
  * @param None
  * @retval Receive counter index1
  * - uint8_t: RxCounter1
  * @par Required preconditions:
  * None
  */
uint8_t GetVar_RxCounter1(void)
{
return RxCounter1;
}
/**
  * @brief Returns RxCounter2 value.
  * @param None
  * @retval Receive counter index2
  * - uint8_t: RxCounter2
  * @par Required preconditions:
  * None
  */
uint8_t GetVar_RxCounter2(void)
{
return RxCounter2;
}
/**
  * @brief Increments TxCounter1 variable and return its value 
  * @param None
  * @retval Transmit counter index1++
  * - uint8_t: TxCounter1++
  * @par Required preconditions:
  * None
  */
uint8_t IncrementVar_TxCounter1(void)
{
	return TxCounter1++;
}
/**
  * @brief Increments TxCounter2 variable and return its value 
  * @param None
  * @retval Transmit counter index2++
  * - uint8_t: TxCounter2++
  * @par Required preconditions:
  * None
  */
uint8_t IncrementVar_TxCounter2(void)
{
	return TxCounter2++;
}
/**
  * @brief Increments RxCounter1 variable and return its value 
  * @param None
  * @retval Receive counter index1++
  * - uint8_t: RxCounter1++
  * @par Required preconditions:
  * None
  */
uint8_t IncrementVar_RxCounter1(void)
{
	return RxCounter1++;
}
/**
  * @brief Increments RxCounter2 variable and return its value 
  * @param None
  * @retval Receive counter index2++
  * - uint8_t: RxCounter2++
  * @par Required preconditions:
  * None
  */
uint8_t IncrementVar_RxCounter2(void)
{
	return RxCounter2++;
}
/**
  * @brief Returns NbrOfDataToTransfer1 value.
  * @param None
  * @retval Tx Buffer Size1
  * - uint8_t: NbrOfDataToTransfer1
  * @par Required preconditions:
  * None
  */
uint8_t GetVar_NbrOfDataToTransfer1(void)
{
return NbrOfDataToTransfer1;
}
/**
  * @brief Returns NbrOfDataToTransfer2 value.
  * @param None
  * @retval Tx Buffer Size2
  * - uint8_t: NbrOfDataToTransfer2
  * @par Required preconditions:
  * None
  */
uint8_t GetVar_NbrOfDataToTransfer2(void)
{
return NbrOfDataToTransfer2;
}
/**
  * @brief Returns NbrOfDataToRead1 value.
  * @param None
  * @retval Rx Buffer Size1
  * - uint8_t: NbrOfDataToRead1
  * @par Required preconditions:
  * None
  */
uint8_t GetVar_NbrOfDataToRead1(void)
{
return NbrOfDataToRead1;
}
/**
  * @brief Returns NbrOfDataToRead2 value.
  * @param None
  * @retval Rx Buffer Size2
  * - uint8_t: NbrOfDataToRead2
  * @par Required preconditions:
  * None
  */
uint8_t GetVar_NbrOfDataToRead2(void)
{
return NbrOfDataToRead2;
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
