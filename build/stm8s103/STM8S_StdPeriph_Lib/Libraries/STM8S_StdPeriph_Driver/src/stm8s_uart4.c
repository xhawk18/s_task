/**
  ********************************************************************************
  * @file    stm8s_uart4.c
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   This file contains all the functions for the UART4 peripheral.
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
#include "stm8s_uart4.h"

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
  * @addtogroup UART4_Public_Functions
  * @{
  */

/**
  * @brief  Deinitializes the UART peripheral.
  * @param  None
  * @retval None
  */

void UART4_DeInit(void)
{
  /*  Clear the Idle Line Detected bit in the status register by a read
  to the UART4_SR register followed by a Read to the UART4_DR register */
  (void)UART4->SR;
  (void)UART4->DR;
  
  UART4->BRR2 = UART4_BRR2_RESET_VALUE;  /*  Set UART4_BRR2 to reset value 0x00 */
  UART4->BRR1 = UART4_BRR1_RESET_VALUE;  /*  Set UART4_BRR1 to reset value 0x00 */
  
  UART4->CR1 = UART4_CR1_RESET_VALUE; /*  Set UART4_CR1 to reset value 0x00  */
  UART4->CR2 = UART4_CR2_RESET_VALUE; /*  Set UART4_CR2 to reset value 0x00  */
  UART4->CR3 = UART4_CR3_RESET_VALUE; /*  Set UART4_CR3 to reset value 0x00  */
  UART4->CR4 = UART4_CR4_RESET_VALUE; /*  Set UART4_CR4 to reset value 0x00  */
  UART4->CR5 = UART4_CR5_RESET_VALUE; /*  Set UART4_CR5 to reset value 0x00  */
  UART4->CR6 = UART4_CR6_RESET_VALUE; /*  Set UART4_CR6 to reset value 0x00  */
}

/**
  * @brief  Initializes the UART4 according to the specified parameters.
  * @param  BaudRate: The baudrate.
  * @param  WordLength : This parameter can be any of the 
  *         @ref UART4_WordLength_TypeDef enumeration.
  * @param  StopBits: This parameter can be any of the 
  *         @ref UART4_StopBits_TypeDef enumeration.
  * @param  Parity: This parameter can be any of the 
  *         @ref UART4_Parity_TypeDef enumeration.
  * @param  SyncMode: This parameter can be any of the 
  *         @ref UART4_SyncMode_TypeDef values.
  * @param  Mode: This parameter can be any of the @ref UART4_Mode_TypeDef values
  * @retval None
  */
void UART4_Init(uint32_t BaudRate, UART4_WordLength_TypeDef WordLength, UART4_StopBits_TypeDef StopBits, UART4_Parity_TypeDef Parity, UART4_SyncMode_TypeDef SyncMode, UART4_Mode_TypeDef Mode)
{
  uint8_t BRR2_1 = 0, BRR2_2 = 0;
  uint32_t BaudRate_Mantissa = 0, BaudRate_Mantissa100 = 0;
  
  /* Check the parameters */
  assert_param(IS_UART4_BAUDRATE_OK(BaudRate));
  assert_param(IS_UART4_WORDLENGTH_OK(WordLength));
  assert_param(IS_UART4_STOPBITS_OK(StopBits));
  assert_param(IS_UART4_PARITY_OK(Parity));
  assert_param(IS_UART4_MODE_OK((uint8_t)Mode));
  assert_param(IS_UART4_SYNCMODE_OK((uint8_t)SyncMode));
  
  /* Clear the word length bit */
  UART4->CR1 &= (uint8_t)(~UART4_CR1_M);
  /* Set the word length bit according to UART4_WordLength value */
  UART4->CR1 |= (uint8_t)WordLength; 
  
  /* Clear the STOP bits */
  UART4->CR3 &= (uint8_t)(~UART4_CR3_STOP);
  /* Set the STOP bits number according to UART4_StopBits value  */
  UART4->CR3 |= (uint8_t)StopBits; 
  
  /* Clear the Parity Control bit */
  UART4->CR1 &= (uint8_t)(~(UART4_CR1_PCEN | UART4_CR1_PS  ));
  /* Set the Parity Control bit to UART4_Parity value */
  UART4->CR1 |= (uint8_t)Parity;
  
  /* Clear the LSB mantissa of UART4DIV  */
  UART4->BRR1 &= (uint8_t)(~UART4_BRR1_DIVM);
  /* Clear the MSB mantissa of UART4DIV  */
  UART4->BRR2 &= (uint8_t)(~UART4_BRR2_DIVM);
  /* Clear the Fraction bits of UART4DIV */
  UART4->BRR2 &= (uint8_t)(~UART4_BRR2_DIVF);
  
  /* Set the UART4 BaudRates in BRR1 and BRR2 registers according to UART4_BaudRate value */
  BaudRate_Mantissa    = ((uint32_t)CLK_GetClockFreq() / (BaudRate << 4));
  BaudRate_Mantissa100 = (((uint32_t)CLK_GetClockFreq() * 100) / (BaudRate << 4));
  
  /* The fraction and MSB mantissa should be loaded in one step in the BRR2 register*/
  /* Set the fraction of UARTDIV  */
  BRR2_1 = (uint8_t)((uint8_t)(((BaudRate_Mantissa100 - (BaudRate_Mantissa * 100))
                                << 4) / 100) & (uint8_t)0x0F); 
  BRR2_2 = (uint8_t)((BaudRate_Mantissa >> 4) & (uint8_t)0xF0);
  
  UART4->BRR2 = (uint8_t)(BRR2_1 | BRR2_2);
  /* Set the LSB mantissa of UARTDIV  */
  UART4->BRR1 = (uint8_t)BaudRate_Mantissa;           
  
  /* Disable the Transmitter and Receiver before setting the LBCL, CPOL and CPHA bits */
  UART4->CR2 &= (uint8_t)~(UART4_CR2_TEN | UART4_CR2_REN);
  /* Clear the Clock Polarity, lock Phase, Last Bit Clock pulse */
  UART4->CR3 &= (uint8_t)~(UART4_CR3_CPOL | UART4_CR3_CPHA | UART4_CR3_LBCL);
  /* Set the Clock Polarity, lock Phase, Last Bit Clock pulse */
  UART4->CR3 |= (uint8_t)((uint8_t)SyncMode & (uint8_t)(UART4_CR3_CPOL | \
    UART4_CR3_CPHA | UART4_CR3_LBCL));
  
  if((uint8_t)(Mode & UART4_MODE_TX_ENABLE))
  {
    /* Set the Transmitter Enable bit */
    UART4->CR2 |= (uint8_t)UART4_CR2_TEN;
  }
  else
  {
    /* Clear the Transmitter Disable bit */
    UART4->CR2 &= (uint8_t)(~UART4_CR2_TEN);
  }
  if((uint8_t)(Mode & UART4_MODE_RX_ENABLE))
  {
    /* Set the Receiver Enable bit */
    UART4->CR2 |= (uint8_t)UART4_CR2_REN;
  }
  else
  {
    /* Clear the Receiver Disable bit */
    UART4->CR2 &= (uint8_t)(~UART4_CR2_REN);
  }
  /* Set the Clock Enable bit, lock Polarity, lock Phase and Last Bit Clock 
  pulse bits according to UART4_Mode value */
  if((uint8_t)(SyncMode & UART4_SYNCMODE_CLOCK_DISABLE))
  {
    /* Clear the Clock Enable bit */
    UART4->CR3 &= (uint8_t)(~UART4_CR3_CKEN); 
  }
  else
  {
    UART4->CR3 |= (uint8_t)((uint8_t)SyncMode & UART4_CR3_CKEN);
  }
}

/**
  * @brief  Enable the UART4 peripheral.
  * @param  NewState : The new state of the UART Communication.
  *         This parameter can be any of the @ref FunctionalState enumeration.
  * @retval None
  */
void UART4_Cmd(FunctionalState NewState)
{
  if(NewState != DISABLE)
  {
    /* UART4 Enable */
    UART4->CR1 &= (uint8_t)(~UART4_CR1_UARTD);
  }
  else
  {
    /* UART4 Disable */
    UART4->CR1 |= UART4_CR1_UARTD; 
  }
}

/**
  * @brief  Enables or disables the specified UART4 interrupts.
  * @param  UART4_IT specifies the UART4 interrupt sources to be enabled or disabled.
  *         This parameter can be one of the following values:
  *         - UART4_IT_LBDF:  LIN Break detection interrupt
  *         - UART4_IT_LHDF:  LIN Break detection interrupt
  *         - UART4_IT_TXE:  Transmit Data Register empty interrupt
  *         - UART4_IT_TC:   Transmission complete interrupt
  *         - UART4_IT_RXNE_OR: Receive Data register not empty/Over run error interrupt
  *         - UART4_IT_IDLE: Idle line detection interrupt
  *         - UART4_IT_PE:   Parity Error interrupt
  * @param  NewState new state of the specified UART4 interrupts.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UART4_ITConfig(UART4_IT_TypeDef UART4_IT, FunctionalState NewState)
{
  uint8_t uartreg = 0, itpos = 0x00;
  
  /* Check the parameters */
  assert_param(IS_UART4_CONFIG_IT_OK(UART4_IT));
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
  
  /* Get the UART4 register index */
  uartreg = (uint8_t)((uint16_t)UART4_IT >> 0x08);
  
  /* Get the UART4 IT index */
  itpos = (uint8_t)((uint8_t)1 << (uint8_t)((uint8_t)UART4_IT & (uint8_t)0x0F));
  
  if(NewState != DISABLE)
  {
    /* Enable the Interrupt bits according to UART4_IT mask */
    if(uartreg == 0x01)
    {
      UART4->CR1 |= itpos;
    }
    else if(uartreg == 0x02)
    {
      UART4->CR2 |= itpos;
    }
    else if(uartreg == 0x03)
    {
      UART4->CR4 |= itpos;
    }
    else
    {
      UART4->CR6 |= itpos;
    }
  }
  else
  {
    /* Disable the interrupt bits according to UART4_IT mask */
    if(uartreg == 0x01)
    {
      UART4->CR1 &= (uint8_t)(~itpos);
    }
    else if(uartreg == 0x02)
    {
      UART4->CR2 &= (uint8_t)(~itpos);
    }
    else if(uartreg == 0x03)
    {
      UART4->CR4 &= (uint8_t)(~itpos);
    }
    else
    {
      UART4->CR6 &= (uint8_t)(~itpos);
    }
  }
}

/**
  * @brief  Enables or disables the UART’s Half Duplex communication.
  * @param  NewState new state of the UART Communication.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UART4_HalfDuplexCmd(FunctionalState NewState)
{
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
  
  if (NewState != DISABLE)
  {
    UART4->CR5 |= UART4_CR5_HDSEL;  /**< UART4 Half Duplex Enable  */
  }
  else
  {
    UART4->CR5 &= (uint8_t)~UART4_CR5_HDSEL; /**< UART4 Half Duplex Disable */
  }
}

/**
  * @brief  Configures the UART4’s IrDA interface.
  * @param  UART4_IrDAMode specifies the IrDA mode.
  *         This parameter can be any of the @ref UART4_IrDAMode_TypeDef values.
  * @retval None
  */
void UART4_IrDAConfig(UART4_IrDAMode_TypeDef UART4_IrDAMode)
{
  assert_param(IS_UART4_IRDAMODE_OK(UART4_IrDAMode));
  
  if(UART4_IrDAMode != UART4_IRDAMODE_NORMAL)
  {
    UART4->CR5 |= UART4_CR5_IRLP;
  }
  else
  {
    UART4->CR5 &= ((uint8_t)~UART4_CR5_IRLP);
  }
}

/**
  * @brief  Enables or disables the UART4’s IrDA interface.
  * @param  NewState new state of the IrDA mode.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UART4_IrDACmd(FunctionalState NewState)
{
  /* Check parameters */
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
  
  if(NewState != DISABLE)
  {
    /* Enable the IrDA mode by setting the IREN bit in the CR3 register */
    UART4->CR5 |= UART4_CR5_IREN;
  }
  else
  {
    /* Disable the IrDA mode by clearing the IREN bit in the CR3 register */
    UART4->CR5 &= ((uint8_t)~UART4_CR5_IREN);
  }
}

/**
  * @brief  Sets the UART4 LIN Break detection length.
  * @param  UART4_LINBreakDetectionLength specifies the LIN break detection length.
  *         This parameter can be any of the 
  *         @ref UART4_LINBreakDetectionLength_TypeDef values.
  * @retval None
  */
void UART4_LINBreakDetectionConfig(UART4_LINBreakDetectionLength_TypeDef UART4_LINBreakDetectionLength)
{
  /* Check parameters */    
  assert_param(IS_UART4_LINBREAKDETECTIONLENGTH_OK(UART4_LINBreakDetectionLength));
  
  if(UART4_LINBreakDetectionLength != UART4_LINBREAKDETECTIONLENGTH_10BITS)
  {
    UART4->CR4 |= UART4_CR4_LBDL;
  }
  else
  {
    UART4->CR4 &= ((uint8_t)~UART4_CR4_LBDL);
  }
}

/**
  * @brief  Configure the UART4 peripheral.
  * @param  UART4_Mode specifies the LIN mode.
  *         This parameter can be any of the @ref UART4_LinMode_TypeDef values.
  * @param  UART4_Autosync specifies the LIN automatic resynchronization mode.
  *         This parameter can be any of the @ref UART4_LinAutosync_TypeDef values.
  * @param  UART4_DivUp specifies the LIN divider update method.
  *         This parameter can be any of the @ref UART4_LinDivUp_TypeDef values.
  * @retval None
  */
void UART4_LINConfig(UART4_LinMode_TypeDef UART4_Mode, 
                     UART4_LinAutosync_TypeDef UART4_Autosync, 
                     UART4_LinDivUp_TypeDef UART4_DivUp)
{
  /* Check parameters */
  assert_param(IS_UART4_SLAVE_OK(UART4_Mode));
  assert_param(IS_UART4_AUTOSYNC_OK(UART4_Autosync));
  assert_param(IS_UART4_DIVUP_OK(UART4_DivUp));
  
  if(UART4_Mode != UART4_LIN_MODE_MASTER)
  {
    UART4->CR6 |=  UART4_CR6_LSLV;
  }
  else
  {
    UART4->CR6 &= ((uint8_t)~UART4_CR6_LSLV);
  }
  
  if(UART4_Autosync != UART4_LIN_AUTOSYNC_DISABLE)
  {
    UART4->CR6 |=  UART4_CR6_LASE ;
  }
  else
  {
    UART4->CR6 &= ((uint8_t)~ UART4_CR6_LASE );
  }
  
  if(UART4_DivUp != UART4_LIN_DIVUP_LBRR1)
  {
    UART4->CR6 |=  UART4_CR6_LDUM;
  }
  else
  {
    UART4->CR6 &= ((uint8_t)~ UART4_CR6_LDUM);
  }
}

/**
  * @brief  Enables or disables the UART4 LIN mode.
  * @param  NewState is new state of the UART4 LIN mode.
  *         This parameter can be ENABLE or DISABLE
  * @retval None
  */
void UART4_LINCmd(FunctionalState NewState)
{
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
  
  if(NewState != DISABLE)
  {
    /* Enable the LIN mode by setting the LINE bit in the CR2 register */
    UART4->CR3 |= UART4_CR3_LINEN;
  }
  else
  {
    /* Disable the LIN mode by clearing the LINE bit in the CR2 register */
    UART4->CR3 &= ((uint8_t)~UART4_CR3_LINEN);
  }
}

/**
  * @brief  Enables or disables the UART4 Smart Card mode.
  * @param  NewState: new state of the Smart Card mode.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UART4_SmartCardCmd(FunctionalState NewState)
{
  /* Check parameters */
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
  
  if(NewState != DISABLE)
  {
    /* Enable the SC mode by setting the SCEN bit in the CR5 register */
    UART4->CR5 |= UART4_CR5_SCEN;
  }
  else
  {
    /* Disable the SC mode by clearing the SCEN bit in the CR5 register */
    UART4->CR5 &= ((uint8_t)(~UART4_CR5_SCEN));
  }
}

/**
  * @brief  Enables or disables NACK transmission.
  * @param  NewState: new state of the Smart Card mode.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UART4_SmartCardNACKCmd(FunctionalState NewState)
{
  /* Check parameters */
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
  
  if(NewState != DISABLE)
  {
    /* Enable the NACK transmission by setting the NACK bit in the CR5 register */
    UART4->CR5 |= UART4_CR5_NACK;
  }
  else
  {
    /* Disable the NACK transmission by clearing the NACK bit in the CR5 register */
    UART4->CR5 &= ((uint8_t)~(UART4_CR5_NACK));
  }
}

/**
  * @brief  Selects the UART4 WakeUp method.
  * @param  UART4_WakeUp: specifies the UART4 wakeup method.
  *         This parameter can be any of the @ref UART4_WakeUp_TypeDef values.
  * @retval None
  */
void UART4_WakeUpConfig(UART4_WakeUp_TypeDef UART4_WakeUp)
{
  assert_param(IS_UART4_WAKEUP_OK(UART4_WakeUp));
  
  UART4->CR1 &= ((uint8_t)~UART4_CR1_WAKE);
  UART4->CR1 |= (uint8_t)UART4_WakeUp;
}

/**
  * @brief  Determines if the UART4 is in mute mode or not.
  * @param  NewState: new state of the UART4 mode.
  *         This parameter can be ENABLE or DISABLE
  * @retval None
  */
void UART4_ReceiverWakeUpCmd(FunctionalState NewState)
{
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
  
  if(NewState != DISABLE)
  {
    /* Enable the mute mode UART4 by setting the RWU bit in the CR2 register */
    UART4->CR2 |= UART4_CR2_RWU;
  }
  else
  {
    /* Disable the mute mode UART4 by clearing the RWU bit in the CR1 register */
    UART4->CR2 &= ((uint8_t)~UART4_CR2_RWU);
  }
}

/**
  * @brief  Returns the most recent received data by the UART4 peripheral.
  * @param  None
  * @retval Received Data
  */
uint8_t UART4_ReceiveData8(void)
{
  return ((uint8_t)UART4->DR);
}

/**
  * @brief  Returns the most recent received data by the UART4 peripheral.
  * @param  None  
  * @retval Received Data
  */
uint16_t UART4_ReceiveData9(void)
{
  uint16_t temp = 0;
  
  temp = ((uint16_t)(((uint16_t)((uint16_t)UART4->CR1 & (uint16_t)UART4_CR1_R8)) << 1));
  
  return (uint16_t)((((uint16_t)UART4->DR) | temp) & ((uint16_t)0x01FF));
}

/**
  * @brief  Transmits 8 bit data through the UART4 peripheral.
  * @param  Data: the data to transmit.
  * @retval None
  */
void UART4_SendData8(uint8_t Data)
{
  /* Transmit Data */
  UART4->DR = Data;
}

/**
  * @brief  Transmits 9 bit data through the UART4 peripheral.
  * @param  Data: the data to transmit.
  * @retval None
  */
void UART4_SendData9(uint16_t Data)
{
  /* Clear the transmit data bit 8 */
  UART4->CR1 &= ((uint8_t)~UART4_CR1_T8);                  
  
  /* Write the transmit data bit [8] */
  UART4->CR1 |= (uint8_t)(((uint8_t)(Data >> 2)) & UART4_CR1_T8); 
  
  /* Write the transmit data bit [0:7] */
  UART4->DR   = (uint8_t)(Data);                    
}

/**
  * @brief  Transmits break characters.
  * @param  None
  * @retval None
  */
void UART4_SendBreak(void)
{
  UART4->CR2 |= UART4_CR2_SBK;
}

/**
  * @brief  Sets the address of the UART4 node.
  * @param  UART4_Address: Indicates the address of the UART4 node.
  * @retval None
  */
void UART4_SetAddress(uint8_t UART4_Address)
{
  /*assert_param for x UART4_Address*/
  assert_param(IS_UART4_ADDRESS_OK(UART4_Address));
  
  /* Clear the UART4 address */
  UART4->CR4 &= ((uint8_t)~UART4_CR4_ADD);
  /* Set the UART4 address node */
  UART4->CR4 |= UART4_Address;
}

/**
  * @brief  Sets the specified UART4 guard time.
  * @note   SmartCard Mode should be Enabled  
  * @param  UART4_GuardTime: specifies the guard time.
  * @retval None
  */
void UART4_SetGuardTime(uint8_t UART4_GuardTime)
{
  /* Set the UART4 guard time */
  UART4->GTR = UART4_GuardTime;
}

/**
  * @brief  Sets the system clock prescaler.
  * @note   IrDA Low Power mode or smartcard mode should be enabled
  * @note   This function is related to SmartCard and IrDa mode.
  * @param  UART4_Prescaler: specifies the prescaler clock.
  *         This parameter can be one of the following values:
  *         @par IrDA Low Power Mode
  *         The clock source is divided by the value given in the register (8 bits)
  *         - 0000 0000 Reserved
  *         - 0000 0001 divides the clock source by 1
  *         - 0000 0010 divides the clock source by 2
  *         - ...
  *        @par Smart Card Mode
  *        The clock source is divided by the value given in the register
  *        (5 significant bits) multiplied by 2
  *         - 0 0000 Reserved
  *         - 0 0001 divides the clock source by 2
  *         - 0 0010 divides the clock source by 4
  *         - 0 0011 divides the clock source by 6
  *         - ...
  * @retval None
  */
void UART4_SetPrescaler(uint8_t UART4_Prescaler)
{
  /* Load the UART4 prescaler value*/
  UART4->PSCR = UART4_Prescaler;
}

/**
  * @brief  Checks whether the specified UART4 flag is set or not.
  * @param  UART4_FLAG specifies the flag to check.
  *         This parameter can be any of the @ref UART4_Flag_TypeDef enumeration.
  * @retval FlagStatus (SET or RESET)
  */
FlagStatus UART4_GetFlagStatus(UART4_Flag_TypeDef UART4_FLAG)
{
  FlagStatus status = RESET;
  
  /* Check parameters */
  assert_param(IS_UART4_FLAG_OK(UART4_FLAG));
  
  /* Check the status of the specified UART4 flag*/
  if(UART4_FLAG == UART4_FLAG_LBDF)
  {
    if((UART4->CR4 & (uint8_t)UART4_FLAG) != (uint8_t)0x00)
    {
      /* UART4_FLAG is set*/
      status = SET;
    }
    else
    {
      /* UART4_FLAG is reset*/
      status = RESET;
    }
  }
  else if(UART4_FLAG == UART4_FLAG_SBK)
  {
    if((UART4->CR2 & (uint8_t)UART4_FLAG) != (uint8_t)0x00)
    {
      /* UART4_FLAG is set*/
      status = SET;
    }
    else
    {
      /* UART4_FLAG is reset*/
      status = RESET;
    }
  }
  else if((UART4_FLAG == UART4_FLAG_LHDF) || (UART4_FLAG == UART4_FLAG_LSF))
  {
    if((UART4->CR6 & (uint8_t)UART4_FLAG) != (uint8_t)0x00)
    {
      /* UART4_FLAG is set*/
      status = SET;
    }
    else
    {
      /* UART4_FLAG is reset*/
      status = RESET;
    }
  }
  else
  {
    if((UART4->SR & (uint8_t)UART4_FLAG) != (uint8_t)0x00)
    {
      /* UART4_FLAG is set*/
      status = SET;
    }
    else
    {
      /* UART4_FLAG is reset*/
      status = RESET;
    }
  }
  
  /* Return the UART4_FLAG status*/
  return  status;
}

/**
  * @brief  Clears the UART4 flags.
  * @param  UART4_FLAG specifies the flag to clear
  *         This parameter can be any combination of the following values:
  *         - UART4_FLAG_LBDF: LIN Break detection flag.
  *         - UART4_FLAG_LHDF: LIN Header detection flag.
  *         - UART4_FLAG_LSF: LIN synchrone field flag.
  *         - UART4_FLAG_RXNE: Receive data register not empty flag.
  * @note:
  *         - PE (Parity error), FE (Framing error), NE (Noise error), 
  *           OR (OverRun error) and IDLE (Idle line detected) flags are cleared
  *           by software sequence: a read operation to UART4_SR register 
  *           (UART4_GetFlagStatus())followed by a read operation to UART4_DR 
  *           register(UART4_ReceiveData8() or UART4_ReceiveData9()).
  *        
  *         - RXNE flag can be also cleared by a read to the UART4_DR register
  *           (UART4_ReceiveData8()or UART4_ReceiveData9()).
  *
  *         - TC flag can be also cleared by software sequence: a read operation
  *           to UART4_SR register (UART4_GetFlagStatus()) followed by a write 
  *           operation to UART4_DR register (UART4_SendData8() or UART4_SendData9()).
  *             
  *         - TXE flag is cleared only by a write to the UART4_DR register 
  *           (UART4_SendData8() or UART4_SendData9()).
  *             
  *         - SBK flag is cleared during the stop bit of break.
  * @retval None
  */
void UART4_ClearFlag(UART4_Flag_TypeDef UART4_FLAG)
{
  assert_param(IS_UART4_CLEAR_FLAG_OK(UART4_FLAG));
  
  /*  Clear the Receive Register Not Empty flag */
  if(UART4_FLAG == UART4_FLAG_RXNE)
  {
    UART4->SR = (uint8_t)~(UART4_SR_RXNE);
  }
  /*  Clear the LIN Break Detection flag */
  else if(UART4_FLAG == UART4_FLAG_LBDF)
  {
    UART4->CR4 &= (uint8_t)(~UART4_CR4_LBDF);
  }
  /*  Clear the LIN Header Detection Flag */
  else if(UART4_FLAG == UART4_FLAG_LHDF)
  {
    UART4->CR6 &= (uint8_t)(~UART4_CR6_LHDF);
  }
  /*  Clear the LIN Synch Field flag */
  else
  {
    UART4->CR6 &= (uint8_t)(~UART4_CR6_LSF);
  }
}

/**
  * @brief  Checks whether the specified UART4 interrupt has occurred or not.
  * @param  UART4_IT: Specifies the UART4 interrupt pending bit to check.
  *         This parameter can be one of the following values:
  *         - UART4_IT_LBDF:  LIN Break detection interrupt
  *         - UART4_IT_TXE:  Transmit Data Register empty interrupt
  *         - UART4_IT_TC:   Transmission complete interrupt
  *         - UART4_IT_RXNE: Receive Data register not empty interrupt
  *         - UART4_IT_IDLE: Idle line detection interrupt
  *         - UART4_IT_OR:  OverRun Error interrupt
  *         - UART4_IT_PE:   Parity Error interrupt
  * @retval The state of UART4_IT (SET or RESET).
  */
ITStatus UART4_GetITStatus(UART4_IT_TypeDef UART4_IT)
{
  ITStatus pendingbitstatus = RESET;
  uint8_t itpos = 0;
  uint8_t itmask1 = 0;
  uint8_t itmask2 = 0;
  uint8_t enablestatus = 0;
  
  /* Check parameters */
  assert_param(IS_UART4_GET_IT_OK(UART4_IT));
  
  /* Get the UART4 IT index*/
  itpos = (uint8_t)((uint8_t)1 << (uint8_t)((uint8_t)UART4_IT & (uint8_t)0x0F));
  /* Get the UART4 IT index*/
  itmask1 = (uint8_t)((uint8_t)UART4_IT >> (uint8_t)4);
  /* Set the IT mask*/
  itmask2 = (uint8_t)((uint8_t)1 << itmask1);
  
  /* Check the status of the specified UART4 pending bit*/
  if(UART4_IT == UART4_IT_PE)
  {
    /* Get the UART4_ITPENDINGBIT enable bit status*/
    enablestatus = (uint8_t)((uint8_t)UART4->CR1 & itmask2);
    /* Check the status of the specified UART4 interrupt*/
    
    if(((UART4->SR & itpos) != (uint8_t)0x00) && enablestatus)
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
  else if(UART4_IT == UART4_IT_LBDF)
  {
    /* Get the UART4_IT enable bit status*/
    enablestatus = (uint8_t)((uint8_t)UART4->CR4 & itmask2);
    /* Check the status of the specified UART4 interrupt*/
    if(((UART4->CR4 & itpos) != (uint8_t)0x00) && enablestatus)
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
  else if(UART4_IT == UART4_IT_LHDF)
  {
    /* Get the UART4_IT enable bit status*/
    enablestatus = (uint8_t)((uint8_t)UART4->CR6 & itmask2);
    /* Check the status of the specified UART4 interrupt*/
    if(((UART4->CR6 & itpos) != (uint8_t)0x00) && enablestatus)
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
    /* Get the UART4_IT enable bit status*/
    enablestatus = (uint8_t)((uint8_t)UART4->CR2 & itmask2);
    /* Check the status of the specified UART4 interrupt*/
    if(((UART4->SR & itpos) != (uint8_t)0x00) && enablestatus)
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
  /* Return the UART4_IT status*/
  return  pendingbitstatus;
}

/**
  * @brief  Clears the UART4 pending flags.
  * @param  UART4_IT specifies the pending bit to clear
  *         This parameter can be one of the following values:
  *         - UART4_IT_LBDF:  LIN Break detection interrupt
  *         - UART4_IT_LHDF:  LIN Header detection interrupt
  *         - UART4_IT_RXNE: Receive Data register not empty interrupt.
  * @note
  *         - PE (Parity error), FE (Framing error), NE (Noise error), 
  *           OR (OverRun error) and IDLE (Idle line detected) pending bits are 
  *           cleared by software sequence: a read operation to UART4_SR register
  *           (UART4_GetITStatus()) followed by a read operation to UART4_DR register
  *           (UART4_ReceiveData8() or UART4_ReceiveData9()).
  *             
  *         - RXNE pending bit can be also cleared by a read to the UART4_DR 
  *           register (UART4_ReceiveData8() or UART4_ReceiveData9()).
  *              
  *         - TC (Transmit complete) pending bit can be cleared by software 
  *           sequence: a read operation to UART4_SR register 
  *           (UART4_GetITStatus()) followed by a write operation to UART4_DR 
  *           register (UART4_SendData8()or UART4_SendData9()).
  *               
  *         - TXE pending bit is cleared only by a write to the UART4_DR register
  *           (UART4_SendData8() or UART4_SendData9()).
  * @retval None
  */
void UART4_ClearITPendingBit(UART4_IT_TypeDef UART4_IT)
{
  assert_param(IS_UART4_CLEAR_IT_OK(UART4_IT));
  
  /*  Clear the Receive Register Not Empty pending bit */
  if(UART4_IT == UART4_IT_RXNE)
  {
    UART4->SR = (uint8_t)~(UART4_SR_RXNE);
  }
  /*  Clear the LIN Break Detection pending bit */
  else if(UART4_IT == UART4_IT_LBDF)
  {
    UART4->CR4 &= (uint8_t)~(UART4_CR4_LBDF);
  }
  /*  Clear the LIN Header Detection pending bit */
  else
  {
    UART4->CR6 &= (uint8_t)(~UART4_CR6_LHDF);
  }
}

/**
  * @}
  */

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
