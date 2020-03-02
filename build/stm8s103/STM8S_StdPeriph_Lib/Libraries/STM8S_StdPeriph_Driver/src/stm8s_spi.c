/**
  ******************************************************************************
  * @file    stm8s_spi.c
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   This file contains all the functions for the SPI peripheral.
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
#include "stm8s_spi.h"

/** @addtogroup STM8S_StdPeriph_Driver
  * @{
  */
  
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @addtogroup SPI_Public_Functions
  * @{
  */

/**
  * @brief  Deinitializes the SPI peripheral registers to their default reset values.
  * @param  None
  * @retval None
  */
void SPI_DeInit(void)
{
  SPI->CR1    = SPI_CR1_RESET_VALUE;
  SPI->CR2    = SPI_CR2_RESET_VALUE;
  SPI->ICR    = SPI_ICR_RESET_VALUE;
  SPI->SR     = SPI_SR_RESET_VALUE;
  SPI->CRCPR  = SPI_CRCPR_RESET_VALUE;
}

/**
  * @brief  Initializes the SPI according to the specified parameters.
  * @param  FirstBit : This parameter can be any of the 
  *         @ref SPI_FirstBit_TypeDef enumeration.
  * @param  BaudRatePrescaler : This parameter can be any of the 
  *         @ref SPI_BaudRatePrescaler_TypeDef enumeration.
  * @param  Mode : This parameter can be any of the  
  *         @ref SPI_Mode_TypeDef enumeration.
  * @param  ClockPolarity : This parameter can be any of the 
  *         @ref SPI_ClockPolarity_TypeDef enumeration.
  * @param  ClockPhase : This parameter can be any of the 
  *         @ref SPI_ClockPhase_TypeDef enumeration.
  * @param  Data_Direction : This parameter can be any of the 
  *         @ref SPI_DataDirection_TypeDef enumeration.
  * @param  Slave_Management : This parameter can be any of the 
  *         @ref SPI_NSS_TypeDef enumeration.
  * @param  CRCPolynomial : Configures the CRC polynomial.
  * @retval None
  */
void SPI_Init(SPI_FirstBit_TypeDef FirstBit, SPI_BaudRatePrescaler_TypeDef BaudRatePrescaler, SPI_Mode_TypeDef Mode, SPI_ClockPolarity_TypeDef ClockPolarity, SPI_ClockPhase_TypeDef ClockPhase, SPI_DataDirection_TypeDef Data_Direction, SPI_NSS_TypeDef Slave_Management, uint8_t CRCPolynomial)
{
  /* Check structure elements */
  assert_param(IS_SPI_FIRSTBIT_OK(FirstBit));
  assert_param(IS_SPI_BAUDRATE_PRESCALER_OK(BaudRatePrescaler));
  assert_param(IS_SPI_MODE_OK(Mode));
  assert_param(IS_SPI_POLARITY_OK(ClockPolarity));
  assert_param(IS_SPI_PHASE_OK(ClockPhase));
  assert_param(IS_SPI_DATA_DIRECTION_OK(Data_Direction));
  assert_param(IS_SPI_SLAVEMANAGEMENT_OK(Slave_Management));
  assert_param(IS_SPI_CRC_POLYNOMIAL_OK(CRCPolynomial));
  
  /* Frame Format, BaudRate, Clock Polarity and Phase configuration */
  SPI->CR1 = (uint8_t)((uint8_t)((uint8_t)FirstBit | BaudRatePrescaler) |
                       (uint8_t)((uint8_t)ClockPolarity | ClockPhase));
  
  /* Data direction configuration: BDM, BDOE and RXONLY bits */
  SPI->CR2 = (uint8_t)((uint8_t)(Data_Direction) | (uint8_t)(Slave_Management));
  
  if (Mode == SPI_MODE_MASTER)
  {
    SPI->CR2 |= (uint8_t)SPI_CR2_SSI;
  }
  else
  {
    SPI->CR2 &= (uint8_t)~(SPI_CR2_SSI);
  }
  
  /* Master/Slave mode configuration */
  SPI->CR1 |= (uint8_t)(Mode);
  
  /* CRC configuration */
  SPI->CRCPR = (uint8_t)CRCPolynomial;
}

/**
  * @brief  Enables or disables the SPI peripheral.
  * @param  NewState New state of the SPI peripheral.
  *         This parameter can be: ENABLE or DISABLE
  * @retval None
  */
void SPI_Cmd(FunctionalState NewState)
{
  /* Check function parameters */
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
  
  if (NewState != DISABLE)
  {
    SPI->CR1 |= SPI_CR1_SPE; /* Enable the SPI peripheral*/
  }
  else
  {
    SPI->CR1 &= (uint8_t)(~SPI_CR1_SPE); /* Disable the SPI peripheral*/
  }
}

/**
  * @brief  Enables or disables the specified interrupts.
  * @param  SPI_IT Specifies the SPI interrupts sources to be enabled or disabled.
  * @param  NewState: The new state of the specified SPI interrupts.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SPI_ITConfig(SPI_IT_TypeDef SPI_IT, FunctionalState NewState)
{
  uint8_t itpos = 0;
  /* Check function parameters */
  assert_param(IS_SPI_CONFIG_IT_OK(SPI_IT));
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
  
  /* Get the SPI IT index */
  itpos = (uint8_t)((uint8_t)1 << (uint8_t)((uint8_t)SPI_IT & (uint8_t)0x0F));
  
  if (NewState != DISABLE)
  {
    SPI->ICR |= itpos; /* Enable interrupt*/
  }
  else
  {
    SPI->ICR &= (uint8_t)(~itpos); /* Disable interrupt*/
  }
}

/**
  * @brief  Transmits a Data through the SPI peripheral.
  * @param  Data : Byte to be transmitted.
  * @retval None
  */
void SPI_SendData(uint8_t Data)
{
  SPI->DR = Data; /* Write in the DR register the data to be sent*/
}

/**
  * @brief  Returns the most recent received data by the SPI peripheral.
  * @param  None
  * @retval The value of the received data.
  */
uint8_t SPI_ReceiveData(void)
{
  return ((uint8_t)SPI->DR); /* Return the data in the DR register*/
}

/**
  * @brief  Configures internally by software the NSS pin.
  * @param  NewState Indicates the new state of the SPI Software slave management.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SPI_NSSInternalSoftwareCmd(FunctionalState NewState)
{
  /* Check function parameters */
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
  
  if (NewState != DISABLE)
  {
    SPI->CR2 |= SPI_CR2_SSI; /* Set NSS pin internally by software*/
  }
  else
  {
    SPI->CR2 &= (uint8_t)(~SPI_CR2_SSI); /* Reset NSS pin internally by software*/
  }
}

/**
  * @brief  Enables the transmit of the CRC value.
  * @param  None
  * @retval None
  */
void SPI_TransmitCRC(void)
{
  SPI->CR2 |= SPI_CR2_CRCNEXT; /* Enable the CRC transmission*/
}

/**
  * @brief  Enables or disables the CRC value calculation of the transferred bytes.
  * @param  NewState Indicates the new state of the SPI CRC value calculation.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SPI_CalculateCRCCmd(FunctionalState NewState)
{
  /* Check function parameters */
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
  
  if (NewState != DISABLE)
  {
    SPI->CR2 |= SPI_CR2_CRCEN; /* Enable the CRC calculation*/
  }
  else
  {
    SPI->CR2 &= (uint8_t)(~SPI_CR2_CRCEN); /* Disable the CRC calculation*/
  }
}

/**
  * @brief  Returns the transmit or the receive CRC register value.
  * @param  SPI_CRC Specifies the CRC register to be read.
  * @retval The selected CRC register value.
  */
uint8_t SPI_GetCRC(SPI_CRC_TypeDef SPI_CRC)
{
  uint8_t crcreg = 0;
  
  /* Check function parameters */
  assert_param(IS_SPI_CRC_OK(SPI_CRC));
  
  if (SPI_CRC != SPI_CRC_RX)
  {
    crcreg = SPI->TXCRCR;  /* Get the Tx CRC register*/
  }
  else
  {
    crcreg = SPI->RXCRCR; /* Get the Rx CRC register*/
  }
  
  /* Return the selected CRC register status*/
  return crcreg;
}

/**
  * @brief  Reset the Rx CRCR and Tx CRCR registers.
  * @param  None
  * @retval None
  */
void SPI_ResetCRC(void)
{
  /* Rx CRCR & Tx CRCR registers are reset when CRCEN (hardware calculation)
  bit in SPI_CR2 is written to 1 (enable) */
  SPI_CalculateCRCCmd(ENABLE);
  
  /* Previous function disable the SPI */
  SPI_Cmd(ENABLE);
}

/**
  * @brief  Returns the CRC Polynomial register value.
  * @param  None
  * @retval The CRC Polynomial register value.
  */
uint8_t SPI_GetCRCPolynomial(void)
{
  return SPI->CRCPR; /* Return the CRC polynomial register */
}

/**
  * @brief  Selects the data transfer direction in bi-directional mode.
  * @param  SPI_Direction Specifies the data transfer direction in bi-directional mode.
  * @retval None
  */
void SPI_BiDirectionalLineConfig(SPI_Direction_TypeDef SPI_Direction)
{
  /* Check function parameters */
  assert_param(IS_SPI_DIRECTION_OK(SPI_Direction));
  
  if (SPI_Direction != SPI_DIRECTION_RX)
  {
    SPI->CR2 |= SPI_CR2_BDOE; /* Set the Tx only mode*/
  }
  else
  {
    SPI->CR2 &= (uint8_t)(~SPI_CR2_BDOE); /* Set the Rx only mode*/
  }
}

/**
  * @brief  Checks whether the specified SPI flag is set or not.
  * @param  SPI_FLAG : Specifies the flag to check.
  *         This parameter can be any of the @ref SPI_FLAG_TypeDef enumeration.
  * @retval FlagStatus : Indicates the state of SPI_FLAG.
  *         This parameter can be any of the @ref FlagStatus enumeration.
  */

FlagStatus SPI_GetFlagStatus(SPI_Flag_TypeDef SPI_FLAG)
{
  FlagStatus status = RESET;
  /* Check parameters */
  assert_param(IS_SPI_FLAGS_OK(SPI_FLAG));
  
  /* Check the status of the specified SPI flag */
  if ((SPI->SR & (uint8_t)SPI_FLAG) != (uint8_t)RESET)
  {
    status = SET; /* SPI_FLAG is set */
  }
  else
  {
    status = RESET; /* SPI_FLAG is reset*/
  }
  
  /* Return the SPI_FLAG status */
  return status;
}

/**
  * @brief  Clears the SPI flags.
  * @param  SPI_FLAG : Specifies the flag to clear.
  *         This parameter can be one of the following values:
  *         - SPI_FLAG_CRCERR
  *         - SPI_FLAG_WKUP
  * @note   - OVR (OverRun Error) interrupt pending bit is cleared by software
  *         sequence:
  *         a read operation to SPI_DR register (SPI_ReceiveData()) followed by
  *         a read operation to SPI_SR register (SPI_GetFlagStatus()).
  *         - MODF (Mode Fault) interrupt pending bit is cleared by software sequence:
  *         a read/write operation to SPI_SR register (SPI_GetFlagStatus()) followed by
  *         a write operation to SPI_CR1 register (SPI_Cmd() to enable the SPI).
  * @retval None
  */
void SPI_ClearFlag(SPI_Flag_TypeDef SPI_FLAG)
{
  assert_param(IS_SPI_CLEAR_FLAGS_OK(SPI_FLAG));
  /* Clear the flag bit */
  SPI->SR = (uint8_t)(~SPI_FLAG);
}

/**
  * @brief  Checks whether the specified interrupt has occurred or not.
  * @param  SPI_IT: Specifies the SPI interrupt pending bit to check.
  *         This parameter can be one of the following values:
  *         - SPI_IT_CRCERR
  *         - SPI_IT_WKUP
  *         - SPI_IT_OVR
  *         - SPI_IT_MODF
  *         - SPI_IT_RXNE
  *         - SPI_IT_TXE
  * @retval ITStatus : Indicates the state of the SPI_IT.
  *         This parameter can be any of the @ref ITStatus enumeration.
  */
ITStatus SPI_GetITStatus(SPI_IT_TypeDef SPI_IT)
{
  ITStatus pendingbitstatus = RESET;
  uint8_t itpos = 0;
  uint8_t itmask1 = 0;
  uint8_t itmask2 = 0;
  uint8_t enablestatus = 0;
  assert_param(IS_SPI_GET_IT_OK(SPI_IT));
  /* Get the SPI IT index */
  itpos = (uint8_t)((uint8_t)1 << ((uint8_t)SPI_IT & (uint8_t)0x0F));
  
  /* Get the SPI IT mask */
  itmask1 = (uint8_t)((uint8_t)SPI_IT >> (uint8_t)4);
  /* Set the IT mask */
  itmask2 = (uint8_t)((uint8_t)1 << itmask1);
  /* Get the SPI_ITPENDINGBIT enable bit status */
  enablestatus = (uint8_t)((uint8_t)SPI->SR & itmask2);
  /* Check the status of the specified SPI interrupt */
  if (((SPI->ICR & itpos) != RESET) && enablestatus)
  {
    /* SPI_ITPENDINGBIT is set */
    pendingbitstatus = SET;
  }
  else
  {
    /* SPI_ITPENDINGBIT is reset */
    pendingbitstatus = RESET;
  }
  /* Return the SPI_ITPENDINGBIT status */
  return  pendingbitstatus;
}

/**
  * @brief  Clears the interrupt pending bits.
  * @param  SPI_IT: Specifies the interrupt pending bit to clear.
  *         This parameter can be one of the following values:
  *         - SPI_IT_CRCERR
  *         - SPI_IT_WKUP
  * @note   - OVR (OverRun Error) interrupt pending bit is cleared by software sequence:
  *         a read operation to SPI_DR register (SPI_ReceiveData()) followed by
  *         a read operation to SPI_SR register (SPI_GetITStatus()).
  *         - MODF (Mode Fault) interrupt pending bit is cleared by software sequence:
  *         a read/write operation to SPI_SR register (SPI_GetITStatus()) followed by
  *         a write operation to SPI_CR1 register (SPI_Cmd() to enable the SPI).
  * @retval None
  */
void SPI_ClearITPendingBit(SPI_IT_TypeDef SPI_IT)
{
  uint8_t itpos = 0;
  assert_param(IS_SPI_CLEAR_IT_OK(SPI_IT));
  
  /* Clear  SPI_IT_CRCERR or SPI_IT_WKUP interrupt pending bits */
  
  /* Get the SPI pending bit index */
  itpos = (uint8_t)((uint8_t)1 << (uint8_t)((uint8_t)(SPI_IT & (uint8_t)0xF0) >> 4));
  /* Clear the pending bit */
  SPI->SR = (uint8_t)(~itpos);
  
}

/**
  * @}
  */
  
/**
  * @}
  */
  

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
