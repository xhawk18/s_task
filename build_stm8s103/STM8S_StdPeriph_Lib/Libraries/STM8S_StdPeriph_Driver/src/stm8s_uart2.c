/**
  ********************************************************************************
  * @file    stm8s_uart2.c
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   This file contains all the functions for the UART2 peripheral.
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
#include "stm8s_uart2.h"

/** @addtogroup STM8S_StdPeriph_Driver
  * @{
  */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

/** @}
  * @addtogroup UART2_Public_Functions
  * @{
  */

/**
  * @brief  Deinitializes the UART peripheral.
  * @param  None
  * @retval None
  */

void UART2_DeInit(void)
{
  /*  Clear the Idle Line Detected bit in the status register by a read
  to the UART2_SR register followed by a Read to the UART2_DR register */
  (void) UART2->SR;
  (void)UART2->DR;
  
  UART2->BRR2 = UART2_BRR2_RESET_VALUE;  /*  Set UART2_BRR2 to reset value 0x00 */
  UART2->BRR1 = UART2_BRR1_RESET_VALUE;  /*  Set UART2_BRR1 to reset value 0x00 */
  
  UART2->CR1 = UART2_CR1_RESET_VALUE; /*  Set UART2_CR1 to reset value 0x00  */
  UART2->CR2 = UART2_CR2_RESET_VALUE; /*  Set UART2_CR2 to reset value 0x00  */
  UART2->CR3 = UART2_CR3_RESET_VALUE; /*  Set UART2_CR3 to reset value 0x00  */
  UART2->CR4 = UART2_CR4_RESET_VALUE; /*  Set UART2_CR4 to reset value 0x00  */
  UART2->CR5 = UART2_CR5_RESET_VALUE; /*  Set UART2_CR5 to reset value 0x00  */
  UART2->CR6 = UART2_CR6_RESET_VALUE; /*  Set UART2_CR6 to reset value 0x00  */
}

/**
  * @brief  Initializes the UART2 according to the specified parameters.
  * @param  BaudRate: The baudrate.
  * @param  WordLength : This parameter can be any of the 
  *         @ref UART2_WordLength_TypeDef enumeration.
  * @param  StopBits: This parameter can be any of the 
  *         @ref UART2_StopBits_TypeDef enumeration.
  * @param  Parity: This parameter can be any of the 
  *         @ref UART2_Parity_TypeDef enumeration.
  * @param  SyncMode: This parameter can be any of the 
  *         @ref UART2_SyncMode_TypeDef values.
  * @param  Mode: This parameter can be any of the @ref UART2_Mode_TypeDef values
  * @retval None
  */
void UART2_Init(uint32_t BaudRate, UART2_WordLength_TypeDef WordLength, UART2_StopBits_TypeDef StopBits, UART2_Parity_TypeDef Parity, UART2_SyncMode_TypeDef SyncMode, UART2_Mode_TypeDef Mode)
{
  uint8_t BRR2_1 = 0, BRR2_2 = 0;
  uint32_t BaudRate_Mantissa = 0, BaudRate_Mantissa100 = 0;
  
  /* Check the parameters */
  assert_param(IS_UART2_BAUDRATE_OK(BaudRate));
  assert_param(IS_UART2_WORDLENGTH_OK(WordLength));
  assert_param(IS_UART2_STOPBITS_OK(StopBits));
  assert_param(IS_UART2_PARITY_OK(Parity));
  assert_param(IS_UART2_MODE_OK((uint8_t)Mode));
  assert_param(IS_UART2_SYNCMODE_OK((uint8_t)SyncMode));
  
  /* Clear the word length bit */
  UART2->CR1 &= (uint8_t)(~UART2_CR1_M);
  /* Set the word length bit according to UART2_WordLength value */
  UART2->CR1 |= (uint8_t)WordLength; 
  
  /* Clear the STOP bits */
  UART2->CR3 &= (uint8_t)(~UART2_CR3_STOP);
  /* Set the STOP bits number according to UART2_StopBits value  */
  UART2->CR3 |= (uint8_t)StopBits; 
  
  /* Clear the Parity Control bit */
  UART2->CR1 &= (uint8_t)(~(UART2_CR1_PCEN | UART2_CR1_PS  ));
  /* Set the Parity Control bit to UART2_Parity value */
  UART2->CR1 |= (uint8_t)Parity;
  
  /* Clear the LSB mantissa of UART2DIV  */
  UART2->BRR1 &= (uint8_t)(~UART2_BRR1_DIVM);
  /* Clear the MSB mantissa of UART2DIV  */
  UART2->BRR2 &= (uint8_t)(~UART2_BRR2_DIVM);
  /* Clear the Fraction bits of UART2DIV */
  UART2->BRR2 &= (uint8_t)(~UART2_BRR2_DIVF);
  
  /* Set the UART2 BaudRates in BRR1 and BRR2 registers according to UART2_BaudRate value */
  BaudRate_Mantissa    = ((uint32_t)CLK_GetClockFreq() / (BaudRate << 4));
  BaudRate_Mantissa100 = (((uint32_t)CLK_GetClockFreq() * 100) / (BaudRate << 4));
  
  /* The fraction and MSB mantissa should be loaded in one step in the BRR2 register*/
  /* Set the fraction of UARTDIV  */
  BRR2_1 = (uint8_t)((uint8_t)(((BaudRate_Mantissa100 - (BaudRate_Mantissa * 100))
                                << 4) / 100) & (uint8_t)0x0F); 
  BRR2_2 = (uint8_t)((BaudRate_Mantissa >> 4) & (uint8_t)0xF0);
  
  UART2->BRR2 = (uint8_t)(BRR2_1 | BRR2_2);
  /* Set the LSB mantissa of UARTDIV  */
  UART2->BRR1 = (uint8_t)BaudRate_Mantissa;           
  
  /* Disable the Transmitter and Receiver before setting the LBCL, CPOL and CPHA bits */
  UART2->CR2 &= (uint8_t)~(UART2_CR2_TEN | UART2_CR2_REN);
  /* Clear the Clock Polarity, lock Phase, Last Bit Clock pulse */
  UART2->CR3 &= (uint8_t)~(UART2_CR3_CPOL | UART2_CR3_CPHA | UART2_CR3_LBCL);
  /* Set the Clock Polarity, lock Phase, Last Bit Clock pulse */
  UART2->CR3 |= (uint8_t)((uint8_t)SyncMode & (uint8_t)(UART2_CR3_CPOL | \
    UART2_CR3_CPHA | UART2_CR3_LBCL));
  
  if ((uint8_t)(Mode & UART2_MODE_TX_ENABLE))
  {
    /* Set the Transmitter Enable bit */
    UART2->CR2 |= (uint8_t)UART2_CR2_TEN;
  }
  else
  {
    /* Clear the Transmitter Disable bit */
    UART2->CR2 &= (uint8_t)(~UART2_CR2_TEN);
  }
  if ((uint8_t)(Mode & UART2_MODE_RX_ENABLE))
  {
    /* Set the Receiver Enable bit */
    UART2->CR2 |= (uint8_t)UART2_CR2_REN;
  }
  else
  {
    /* Clear the Receiver Disable bit */
    UART2->CR2 &= (uint8_t)(~UART2_CR2_REN);
  }
  /* Set the Clock Enable bit, lock Polarity, lock Phase and Last Bit Clock 
  pulse bits according to UART2_Mode value */
  if ((uint8_t)(SyncMode & UART2_SYNCMODE_CLOCK_DISABLE))
  {
    /* Clear the Clock Enable bit */
    UART2->CR3 &= (uint8_t)(~UART2_CR3_CKEN); 
  }
  else
  {
    UART2->CR3 |= (uint8_t)((uint8_t)SyncMode & UART2_CR3_CKEN);
  }
}

/**
  * @brief  Enable the UART2 peripheral.
  * @param  NewState : The new state of the UART Communication.
  *         This parameter can be any of the @ref FunctionalState enumeration.
  * @retval None
  */
void UART2_Cmd(FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
    /* UART2 Enable */
    UART2->CR1 &= (uint8_t)(~UART2_CR1_UARTD);
  }
  else
  {
    /* UART2 Disable */
    UART2->CR1 |= UART2_CR1_UARTD; 
  }
}

/**
  * @brief  Enables or disables the specified UART2 interrupts.
  * @param  UART2_IT specifies the UART2 interrupt sources to be enabled or disabled.
  *         This parameter can be one of the following values:
  *         - UART2_IT_LBDF:  LIN Break detection interrupt
  *         - UART2_IT_LHDF:  LIN Break detection interrupt
  *         - UART2_IT_TXE:  Transmit Data Register empty interrupt
  *         - UART2_IT_TC:   Transmission complete interrupt
  *         - UART2_IT_RXNE_OR: Receive Data register not empty/Over run error interrupt
  *         - UART2_IT_IDLE: Idle line detection interrupt
  *         - UART2_IT_PE:   Parity Error interrupt
  * @param  NewState new state of the specified UART2 interrupts.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UART2_ITConfig(UART2_IT_TypeDef UART2_IT, FunctionalState NewState)
{
  uint8_t uartreg = 0, itpos = 0x00;
  
  /* Check the parameters */
  assert_param(IS_UART2_CONFIG_IT_OK(UART2_IT));
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
  
  /* Get the UART2 register index */
  uartreg = (uint8_t)((uint16_t)UART2_IT >> 0x08);
  
  /* Get the UART2 IT index */
  itpos = (uint8_t)((uint8_t)1 << (uint8_t)((uint8_t)UART2_IT & (uint8_t)0x0F));
  
  if (NewState != DISABLE)
  {
    /* Enable the Interrupt bits according to UART2_IT mask */
    if (uartreg == 0x01)
    {
      UART2->CR1 |= itpos;
    }
    else if (uartreg == 0x02)
    {
      UART2->CR2 |= itpos;
    }
    else if (uartreg == 0x03)
    {
      UART2->CR4 |= itpos;
    }
    else
    {
      UART2->CR6 |= itpos;
    }
  }
  else
  {
    /* Disable the interrupt bits according to UART2_IT mask */
    if (uartreg == 0x01)
    {
      UART2->CR1 &= (uint8_t)(~itpos);
    }
    else if (uartreg == 0x02)
    {
      UART2->CR2 &= (uint8_t)(~itpos);
    }
    else if (uartreg == 0x03)
    {
      UART2->CR4 &= (uint8_t)(~itpos);
    }
    else
    {
      UART2->CR6 &= (uint8_t)(~itpos);
    }
  }
}

/**
  * @brief  Configures the UART2’s IrDA interface.
  * @param  UART2_IrDAMode specifies the IrDA mode.
  *         This parameter can be any of the @ref UART2_IrDAMode_TypeDef values.
  * @retval None
  */
void UART2_IrDAConfig(UART2_IrDAMode_TypeDef UART2_IrDAMode)
{
  assert_param(IS_UART2_IRDAMODE_OK(UART2_IrDAMode));
  
  if (UART2_IrDAMode != UART2_IRDAMODE_NORMAL)
  {
    UART2->CR5 |= UART2_CR5_IRLP;
  }
  else
  {
    UART2->CR5 &= ((uint8_t)~UART2_CR5_IRLP);
  }
}

/**
  * @brief  Enables or disables the UART2’s IrDA interface.
  * @param  NewState new state of the IrDA mode.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UART2_IrDACmd(FunctionalState NewState)
{
  /* Check parameters */
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
  
  if (NewState != DISABLE)
  {
    /* Enable the IrDA mode by setting the IREN bit in the CR3 register */
    UART2->CR5 |= UART2_CR5_IREN;
  }
  else
  {
    /* Disable the IrDA mode by clearing the IREN bit in the CR3 register */
    UART2->CR5 &= ((uint8_t)~UART2_CR5_IREN);
  }
}

/**
  * @brief  Sets the UART2 LIN Break detection length.
  * @param  UART2_LINBreakDetectionLength specifies the LIN break detection length.
  *         This parameter can be any of the 
  *         @ref UART2_LINBreakDetectionLength_TypeDef values.
  * @retval None
  */
void UART2_LINBreakDetectionConfig(UART2_LINBreakDetectionLength_TypeDef UART2_LINBreakDetectionLength)
{
  /* Check parameters */    
  assert_param(IS_UART2_LINBREAKDETECTIONLENGTH_OK(UART2_LINBreakDetectionLength));
  
  if (UART2_LINBreakDetectionLength != UART2_LINBREAKDETECTIONLENGTH_10BITS)
  {
    UART2->CR4 |= UART2_CR4_LBDL;
  }
  else
  {
    UART2->CR4 &= ((uint8_t)~UART2_CR4_LBDL);
  }
}

/**
  * @brief  Configure the UART2 peripheral.
  * @param  UART2_Mode specifies the LIN mode.
  *         This parameter can be any of the @ref UART2_LinMode_TypeDef values.
  * @param  UART2_Autosync specifies the LIN automatic resynchronization mode.
  *         This parameter can be any of the @ref UART2_LinAutosync_TypeDef values.
  * @param  UART2_DivUp specifies the LIN divider update method.
  *         This parameter can be any of the @ref UART2_LinDivUp_TypeDef values.
  * @retval None
  */
void UART2_LINConfig(UART2_LinMode_TypeDef UART2_Mode, 
                     UART2_LinAutosync_TypeDef UART2_Autosync, 
                     UART2_LinDivUp_TypeDef UART2_DivUp)
{
  /* Check parameters */
  assert_param(IS_UART2_SLAVE_OK(UART2_Mode));
  assert_param(IS_UART2_AUTOSYNC_OK(UART2_Autosync));
  assert_param(IS_UART2_DIVUP_OK(UART2_DivUp));
  
  if (UART2_Mode != UART2_LIN_MODE_MASTER)
  {
    UART2->CR6 |=  UART2_CR6_LSLV;
  }
  else
  {
    UART2->CR6 &= ((uint8_t)~UART2_CR6_LSLV);
  }
  
  if (UART2_Autosync != UART2_LIN_AUTOSYNC_DISABLE)
  {
    UART2->CR6 |=  UART2_CR6_LASE ;
  }
  else
  {
    UART2->CR6 &= ((uint8_t)~ UART2_CR6_LASE );
  }
  
  if (UART2_DivUp != UART2_LIN_DIVUP_LBRR1)
  {
    UART2->CR6 |=  UART2_CR6_LDUM;
  }
  else
  {
    UART2->CR6 &= ((uint8_t)~ UART2_CR6_LDUM);
  }
}

/**
  * @brief  Enables or disables the UART2 LIN mode.
  * @param  NewState is new state of the UART2 LIN mode.
  *         This parameter can be ENABLE or DISABLE
  * @retval None
  */
void UART2_LINCmd(FunctionalState NewState)
{
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
  
  if (NewState != DISABLE)
  {
    /* Enable the LIN mode by setting the LINE bit in the CR2 register */
    UART2->CR3 |= UART2_CR3_LINEN;
  }
  else
  {
    /* Disable the LIN mode by clearing the LINE bit in the CR2 register */
    UART2->CR3 &= ((uint8_t)~UART2_CR3_LINEN);
  }
}

/**
  * @brief  Enables or disables the UART2 Smart Card mode.
  * @param  NewState: new state of the Smart Card mode.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UART2_SmartCardCmd(FunctionalState NewState)
{
  /* Check parameters */
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
  
  if (NewState != DISABLE)
  {
    /* Enable the SC mode by setting the SCEN bit in the CR5 register */
    UART2->CR5 |= UART2_CR5_SCEN;
  }
  else
  {
    /* Disable the SC mode by clearing the SCEN bit in the CR5 register */
    UART2->CR5 &= ((uint8_t)(~UART2_CR5_SCEN));
  }
}

/**
  * @brief  Enables or disables NACK transmission.
  * @param  NewState: new state of the Smart Card mode.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UART2_SmartCardNACKCmd(FunctionalState NewState)
{
  /* Check parameters */
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
  
  if (NewState != DISABLE)
  {
    /* Enable the NACK transmission by setting the NACK bit in the CR5 register */
    UART2->CR5 |= UART2_CR5_NACK;
  }
  else
  {
    /* Disable the NACK transmission by clearing the NACK bit in the CR5 register */
    UART2->CR5 &= ((uint8_t)~(UART2_CR5_NACK));
  }
}

/**
  * @brief  Selects the UART2 WakeUp method.
  * @param  UART2_WakeUp: specifies the UART2 wakeup method.
  *         This parameter can be any of the @ref UART2_WakeUp_TypeDef values.
  * @retval None
  */
void UART2_WakeUpConfig(UART2_WakeUp_TypeDef UART2_WakeUp)
{
  assert_param(IS_UART2_WAKEUP_OK(UART2_WakeUp));
  
  UART2->CR1 &= ((uint8_t)~UART2_CR1_WAKE);
  UART2->CR1 |= (uint8_t)UART2_WakeUp;
}

/**
  * @brief  Determines if the UART2 is in mute mode or not.
  * @param  NewState: new state of the UART2 mode.
  *         This parameter can be ENABLE or DISABLE
  * @retval None
  */
void UART2_ReceiverWakeUpCmd(FunctionalState NewState)
{
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
  
  if (NewState != DISABLE)
  {
    /* Enable the mute mode UART2 by setting the RWU bit in the CR2 register */
    UART2->CR2 |= UART2_CR2_RWU;
  }
  else
  {
    /* Disable the mute mode UART2 by clearing the RWU bit in the CR1 register */
    UART2->CR2 &= ((uint8_t)~UART2_CR2_RWU);
  }
}

/**
  * @brief  Returns the most recent received data by the UART2 peripheral.
  * @param  None
  * @retval Received Data
  */
uint8_t UART2_ReceiveData8(void)
{
  return ((uint8_t)UART2->DR);
}

/**
  * @brief  Returns the most recent received data by the UART2 peripheral.
  * @param  None  
  * @retval Received Data
  */
uint16_t UART2_ReceiveData9(void)
{
  uint16_t temp = 0;
  
  temp = ((uint16_t)(((uint16_t)((uint16_t)UART2->CR1 & (uint16_t)UART2_CR1_R8)) << 1));
  
  return (uint16_t)((((uint16_t)UART2->DR) | temp) & ((uint16_t)0x01FF));
}

/**
  * @brief  Transmits 8 bit data through the UART2 peripheral.
  * @param  Data: the data to transmit.
  * @retval None
  */
void UART2_SendData8(uint8_t Data)
{
  /* Transmit Data */
  UART2->DR = Data;
}

/**
  * @brief  Transmits 9 bit data through the UART2 peripheral.
  * @param  Data: the data to transmit.
  * @retval None
  */
void UART2_SendData9(uint16_t Data)
{
  /* Clear the transmit data bit 8 */
  UART2->CR1 &= ((uint8_t)~UART2_CR1_T8);                  
  
  /* Write the transmit data bit [8] */
  UART2->CR1 |= (uint8_t)(((uint8_t)(Data >> 2)) & UART2_CR1_T8); 
  
  /* Write the transmit data bit [0:7] */
  UART2->DR   = (uint8_t)(Data);                    
}

/**
  * @brief  Transmits break characters.
  * @param  None
  * @retval None
  */
void UART2_SendBreak(void)
{
  UART2->CR2 |= UART2_CR2_SBK;
}

/**
  * @brief  Sets the address of the UART2 node.
  * @param  UART2_Address: Indicates the address of the UART2 node.
  * @retval None
  */
void UART2_SetAddress(uint8_t UART2_Address)
{
  /*assert_param for x UART2_Address*/
  assert_param(IS_UART2_ADDRESS_OK(UART2_Address));
  
  /* Clear the UART2 address */
  UART2->CR4 &= ((uint8_t)~UART2_CR4_ADD);
  /* Set the UART2 address node */
  UART2->CR4 |= UART2_Address;
}

/**
  * @brief  Sets the specified UART2 guard time.
  * @note   SmartCard Mode should be Enabled  
  * @param  UART2_GuardTime: specifies the guard time.
  * @retval None
  */
void UART2_SetGuardTime(uint8_t UART2_GuardTime)
{
  /* Set the UART2 guard time */
  UART2->GTR = UART2_GuardTime;
}

/**
  * @brief  Sets the system clock prescaler.
  * @note   IrDA Low Power mode or smartcard mode should be enabled
  * @note   This function is related to SmartCard and IrDa mode.
  * @param  UART2_Prescaler: specifies the prescaler clock.
  *         This parameter can be one of the following values:
  *         @par IrDA Low Power Mode
  *         The clock source is divided by the value given in the register (8 bits)
  *         - 0000 0000 Reserved
  *         - 0000 0001 divides the clock source by 1
  *         - 0000 0010 divides the clock source by 2
  *         - ...
  *        @par Smart Card Mode
  *        The clock source is divided by the value given in the register
  *        (5 significant bits) multiped by 2
  *         - 0 0000 Reserved
  *         - 0 0001 divides the clock source by 2
  *         - 0 0010 divides the clock source by 4
  *         - 0 0011 divides the clock source by 6
  *         - ...
  * @retval None
  */
void UART2_SetPrescaler(uint8_t UART2_Prescaler)
{
  /* Load the UART2 prescaler value*/
  UART2->PSCR = UART2_Prescaler;
}

/**
  * @brief  Checks whether the specified UART2 flag is set or not.
  * @param  UART2_FLAG specifies the flag to check.
  *         This parameter can be any of the @ref UART2_Flag_TypeDef enumeration.
  * @retval FlagStatus (SET or RESET)
  */
FlagStatus UART2_GetFlagStatus(UART2_Flag_TypeDef UART2_FLAG)
{
  FlagStatus status = RESET;
  
  /* Check parameters */
  assert_param(IS_UART2_FLAG_OK(UART2_FLAG));
  
  /* Check the status of the specified UART2 flag*/
  if (UART2_FLAG == UART2_FLAG_LBDF)
  {
    if ((UART2->CR4 & (uint8_t)UART2_FLAG) != (uint8_t)0x00)
    {
      /* UART2_FLAG is set*/
      status = SET;
    }
    else
    {
      /* UART2_FLAG is reset*/
      status = RESET;
    }
  }
  else if (UART2_FLAG == UART2_FLAG_SBK)
  {
    if ((UART2->CR2 & (uint8_t)UART2_FLAG) != (uint8_t)0x00)
    {
      /* UART2_FLAG is set*/
      status = SET;
    }
    else
    {
      /* UART2_FLAG is reset*/
      status = RESET;
    }
  }
  else if ((UART2_FLAG == UART2_FLAG_LHDF) || (UART2_FLAG == UART2_FLAG_LSF))
  {
    if ((UART2->CR6 & (uint8_t)UART2_FLAG) != (uint8_t)0x00)
    {
      /* UART2_FLAG is set*/
      status = SET;
    }
    else
    {
      /* UART2_FLAG is reset*/
      status = RESET;
    }
  }
  else
  {
    if ((UART2->SR & (uint8_t)UART2_FLAG) != (uint8_t)0x00)
    {
      /* UART2_FLAG is set*/
      status = SET;
    }
    else
    {
      /* UART2_FLAG is reset*/
      status = RESET;
    }
  }
  
  /* Return the UART2_FLAG status*/
  return  status;
}

/**
  * @brief  Clears the UART2 flags.
  * @param  UART2_FLAG specifies the flag to clear
  *         This parameter can be any combination of the following values:
  *         - UART2_FLAG_LBDF: LIN Break detection flag.
  *         - UART2_FLAG_LHDF: LIN Header detection flag.
  *         - UART2_FLAG_LSF: LIN synchrone field flag.
  *         - UART2_FLAG_RXNE: Receive data register not empty flag.
  * @note:
  *         - PE (Parity error), FE (Framing error), NE (Noise error), 
  *           OR (OverRun error) and IDLE (Idle line detected) flags are cleared
  *           by software sequence: a read operation to UART2_SR register 
  *           (UART2_GetFlagStatus())followed by a read operation to UART2_DR 
  *           register(UART2_ReceiveData8() or UART2_ReceiveData9()).
  *        
  *         - RXNE flag can be also cleared by a read to the UART2_DR register
  *           (UART2_ReceiveData8()or UART2_ReceiveData9()).
  *
  *         - TC flag can be also cleared by software sequence: a read operation
  *           to UART2_SR register (UART2_GetFlagStatus()) followed by a write 
  *           operation to UART2_DR register (UART2_SendData8() or UART2_SendData9()).
  *             
  *         - TXE flag is cleared only by a write to the UART2_DR register 
  *           (UART2_SendData8() or UART2_SendData9()).
  *             
  *         - SBK flag is cleared during the stop bit of break.
  * @retval None
  */
void UART2_ClearFlag(UART2_Flag_TypeDef UART2_FLAG)
{
  assert_param(IS_UART2_CLEAR_FLAG_OK(UART2_FLAG));
  
  /*  Clear the Receive Register Not Empty flag */
  if (UART2_FLAG == UART2_FLAG_RXNE)
  {
    UART2->SR = (uint8_t)~(UART2_SR_RXNE);
  }
  /*  Clear the LIN Break Detection flag */
  else if (UART2_FLAG == UART2_FLAG_LBDF)
  {
    UART2->CR4 &= (uint8_t)(~UART2_CR4_LBDF);
  }
  /*  Clear the LIN Header Detection Flag */
  else if (UART2_FLAG == UART2_FLAG_LHDF)
  {
    UART2->CR6 &= (uint8_t)(~UART2_CR6_LHDF);
  }
  /*  Clear the LIN Synch Field flag */
  else
  {
    UART2->CR6 &= (uint8_t)(~UART2_CR6_LSF);
  }
}

/**
  * @brief  Checks whether the specified UART2 interrupt has occurred or not.
  * @param  UART2_IT: Specifies the UART2 interrupt pending bit to check.
  *         This parameter can be one of the following values:
  *         - UART2_IT_LBDF:  LIN Break detection interrupt
  *         - UART2_IT_TXE:  Transmit Data Register empty interrupt
  *         - UART2_IT_TC:   Transmission complete interrupt
  *         - UART2_IT_RXNE: Receive Data register not empty interrupt
  *         - UART2_IT_IDLE: Idle line detection interrupt
  *         - UART2_IT_OR:  OverRun Error interrupt
  *         - UART2_IT_PE:   Parity Error interrupt
  * @retval The state of UART2_IT (SET or RESET).
  */
ITStatus UART2_GetITStatus(UART2_IT_TypeDef UART2_IT)
{
  ITStatus pendingbitstatus = RESET;
  uint8_t itpos = 0;
  uint8_t itmask1 = 0;
  uint8_t itmask2 = 0;
  uint8_t enablestatus = 0;
  
  /* Check parameters */
  assert_param(IS_UART2_GET_IT_OK(UART2_IT));
  
  /* Get the UART2 IT index*/
  itpos = (uint8_t)((uint8_t)1 << (uint8_t)((uint8_t)UART2_IT & (uint8_t)0x0F));
  /* Get the UART2 IT index*/
  itmask1 = (uint8_t)((uint8_t)UART2_IT >> (uint8_t)4);
  /* Set the IT mask*/
  itmask2 = (uint8_t)((uint8_t)1 << itmask1);
  
  /* Check the status of the specified UART2 pending bit*/
  if (UART2_IT == UART2_IT_PE)
  {
    /* Get the UART2_ITPENDINGBIT enable bit status*/
    enablestatus = (uint8_t)((uint8_t)UART2->CR1 & itmask2);
    /* Check the status of the specified UART2 interrupt*/
    
    if (((UART2->SR & itpos) != (uint8_t)0x00) && enablestatus)
    {
      /* Interrupt occurred*/
      pendingbitstatus = SET;
    }
    else
    {
      /* Interrupt not occurred*/
      pendingbitstatus = RESET;
    }
  }
  else if (UART2_IT == UART2_IT_LBDF)
  {
    /* Get the UART2_IT enable bit status*/
    enablestatus = (uint8_t)((uint8_t)UART2->CR4 & itmask2);
    /* Check the status of the specified UART2 interrupt*/
    if (((UART2->CR4 & itpos) != (uint8_t)0x00) && enablestatus)
    {
      /* Interrupt occurred*/
      pendingbitstatus = SET;
    }
    else
    {
      /* Interrupt not occurred*/
      pendingbitstatus = RESET;
    }
  }
  else if (UART2_IT == UART2_IT_LHDF)
  {
    /* Get the UART2_IT enable bit status*/
    enablestatus = (uint8_t)((uint8_t)UART2->CR6 & itmask2);
    /* Check the status of the specified UART2 interrupt*/
    if (((UART2->CR6 & itpos) != (uint8_t)0x00) && enablestatus)
    {
      /* Interrupt occurred*/
      pendingbitstatus = SET;
    }
    else
    {
      /* Interrupt not occurred*/
      pendingbitstatus = RESET;
    }
  }
  else
  {
    /* Get the UART2_IT enable bit status*/
    enablestatus = (uint8_t)((uint8_t)UART2->CR2 & itmask2);
    /* Check the status of the specified UART2 interrupt*/
    if (((UART2->SR & itpos) != (uint8_t)0x00) && enablestatus)
    {
      /* Interrupt occurred*/
      pendingbitstatus = SET;
    }
    else
    {
      /* Interrupt not occurred*/
      pendingbitstatus = RESET;
    }
  }
  /* Return the UART2_IT status*/
  return  pendingbitstatus;
}

/**
  * @brief  Clears the UART2 pending flags.
  * @param  UART2_IT specifies the pending bit to clear
  *         This parameter can be one of the following values:
  *         - UART2_IT_LBDF:  LIN Break detection interrupt
  *         - UART2_IT_LHDF:  LIN Header detection interrupt
  *         - UART2_IT_RXNE: Receive Data register not empty interrupt.
  * @note
  *         - PE (Parity error), FE (Framing error), NE (Noise error), 
  *           OR (OverRun error) and IDLE (Idle line detected) pending bits are 
  *           cleared by software sequence: a read operation to UART2_SR register
  *           (UART2_GetITStatus()) followed by a read operation to UART2_DR register
  *           (UART2_ReceiveData8() or UART2_ReceiveData9()).
  *             
  *         - RXNE pending bit can be also cleared by a read to the UART2_DR 
  *           register (UART2_ReceiveData8() or UART2_ReceiveData9()).
  *              
  *         - TC (Transmit complete) pending bit can be cleared by software 
  *           sequence: a read operation to UART2_SR register 
  *           (UART2_GetITStatus()) followed by a write operation to UART2_DR 
  *           register (UART2_SendData8()or UART2_SendData9()).
  *               
  *         - TXE pending bit is cleared only by a write to the UART2_DR register
  *           (UART2_SendData8() or UART2_SendData9()).
  * @retval None
  */
void UART2_ClearITPendingBit(UART2_IT_TypeDef UART2_IT)
{
  assert_param(IS_UART2_CLEAR_IT_OK(UART2_IT));
  
  /*  Clear the Receive Register Not Empty pending bit */
  if (UART2_IT == UART2_IT_RXNE)
  {
    UART2->SR = (uint8_t)~(UART2_SR_RXNE);
  }
  /*  Clear the LIN Break Detection pending bit */
  else if (UART2_IT == UART2_IT_LBDF)
  {
    UART2->CR4 &= (uint8_t)~(UART2_CR4_LBDF);
  }
  /*  Clear the LIN Header Detection pending bit */
  else
  {
    UART2->CR6 &= (uint8_t)(~UART2_CR6_LHDF);
  }
}

/**
  * @}
  */

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
