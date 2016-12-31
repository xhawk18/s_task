/**
  ******************************************************************************
  * @file    stm8s_i2c.c
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   This file contains all the functions for the I2C peripheral.
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
#include "stm8s_i2c.h"

/** @addtogroup STM8S_StdPeriph_Driver
  * @{
  */
/* Private typedef -----------------------------------------------------------*/

/** @defgroup I2C_Private_Defines
  * @{
  */
/* I2C register mask */
#define REGISTER_Mask               ((uint16_t)0x3000)
#define REGISTER_SR1_Index          ((uint16_t)0x0100)
#define REGISTER_SR2_Index          ((uint16_t)0x0200)
/* I2C Interrupt Enable mask */
#define ITEN_Mask                   ((uint16_t)0x0700)
/* I2C FLAG mask */
#define FLAG_Mask                   ((uint16_t)0x00FF)
/**
  * @}
  */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/**
  * @addtogroup I2C_Public_Functions
  * @{
  */

/**
  * @brief  Deinitializes the I2C peripheral registers to their default reset values.
  * @param  None
  * @retval None
  */
void I2C_DeInit(void)
{
  I2C->CR1 = I2C_CR1_RESET_VALUE;
  I2C->CR2 = I2C_CR2_RESET_VALUE;
  I2C->FREQR = I2C_FREQR_RESET_VALUE;
  I2C->OARL = I2C_OARL_RESET_VALUE;
  I2C->OARH = I2C_OARH_RESET_VALUE;
  I2C->ITR = I2C_ITR_RESET_VALUE;
  I2C->CCRL = I2C_CCRL_RESET_VALUE;
  I2C->CCRH = I2C_CCRH_RESET_VALUE;
  I2C->TRISER = I2C_TRISER_RESET_VALUE;
}

/**
  * @brief  Initializes the I2C according to the specified parameters in standard
  *         or fast mode.
  * @param  OutputClockFrequencyHz : Specifies the output clock frequency in Hz.
  * @param  OwnAddress : Specifies the own address.
  * @param  I2C_DutyCycle : Specifies the duty cycle to apply in fast mode.
  *         This parameter can be any of the  @ref I2C_DutyCycle_TypeDef enumeration.
  * @note   This parameter don't have impact when the OutputClockFrequency lower
  *         than 100KHz.
  * @param  Ack : Specifies the acknowledge mode to apply.
  *         This parameter can be any of the  @ref I2C_Ack_TypeDef enumeration.
  * @param  AddMode : Specifies the acknowledge address to apply.
  *         This parameter can be any of the  @ref I2C_AddMode_TypeDef enumeration.
  * @param  InputClockFrequencyMHz : Specifies the input clock frequency in MHz.
  * @retval None
  */
void I2C_Init(uint32_t OutputClockFrequencyHz, uint16_t OwnAddress, 
              I2C_DutyCycle_TypeDef I2C_DutyCycle, I2C_Ack_TypeDef Ack, 
              I2C_AddMode_TypeDef AddMode, uint8_t InputClockFrequencyMHz )
{
  uint16_t result = 0x0004;
  uint16_t tmpval = 0;
  uint8_t tmpccrh = 0;

  /* Check the parameters */
  assert_param(IS_I2C_ACK_OK(Ack));
  assert_param(IS_I2C_ADDMODE_OK(AddMode));
  assert_param(IS_I2C_OWN_ADDRESS_OK(OwnAddress));
  assert_param(IS_I2C_DUTYCYCLE_OK(I2C_DutyCycle));  
  assert_param(IS_I2C_INPUT_CLOCK_FREQ_OK(InputClockFrequencyMHz));
  assert_param(IS_I2C_OUTPUT_CLOCK_FREQ_OK(OutputClockFrequencyHz));


  /*------------------------- I2C FREQ Configuration ------------------------*/
  /* Clear frequency bits */
  I2C->FREQR &= (uint8_t)(~I2C_FREQR_FREQ);
  /* Write new value */
  I2C->FREQR |= InputClockFrequencyMHz;

  /*--------------------------- I2C CCR Configuration ------------------------*/
  /* Disable I2C to configure TRISER */
  I2C->CR1 &= (uint8_t)(~I2C_CR1_PE);

  /* Clear CCRH & CCRL */
  I2C->CCRH &= (uint8_t)(~(I2C_CCRH_FS | I2C_CCRH_DUTY | I2C_CCRH_CCR));
  I2C->CCRL &= (uint8_t)(~I2C_CCRL_CCR);

  /* Detect Fast or Standard mode depending on the Output clock frequency selected */
  if (OutputClockFrequencyHz > I2C_MAX_STANDARD_FREQ) /* FAST MODE */
  {
    /* Set F/S bit for fast mode */
    tmpccrh = I2C_CCRH_FS;

    if (I2C_DutyCycle == I2C_DUTYCYCLE_2)
    {
      /* Fast mode speed calculate: Tlow/Thigh = 2 */
      result = (uint16_t) ((InputClockFrequencyMHz * 1000000) / (OutputClockFrequencyHz * 3));
    }
    else /* I2C_DUTYCYCLE_16_9 */
    {
      /* Fast mode speed calculate: Tlow/Thigh = 16/9 */
      result = (uint16_t) ((InputClockFrequencyMHz * 1000000) / (OutputClockFrequencyHz * 25));
      /* Set DUTY bit */
      tmpccrh |= I2C_CCRH_DUTY;
    }

    /* Verify and correct CCR value if below minimum value */
    if (result < (uint16_t)0x01)
    {
      /* Set the minimum allowed value */
      result = (uint16_t)0x0001;
    }

    /* Set Maximum Rise Time: 300ns max in Fast Mode
    = [300ns/(1/InputClockFrequencyMHz.10e6)]+1
    = [(InputClockFrequencyMHz * 3)/10]+1 */
    tmpval = ((InputClockFrequencyMHz * 3) / 10) + 1;
    I2C->TRISER = (uint8_t)tmpval;

  }
  else /* STANDARD MODE */
  {

    /* Calculate standard mode speed */
    result = (uint16_t)((InputClockFrequencyMHz * 1000000) / (OutputClockFrequencyHz << (uint8_t)1));

    /* Verify and correct CCR value if below minimum value */
    if (result < (uint16_t)0x0004)
    {
      /* Set the minimum allowed value */
      result = (uint16_t)0x0004;
    }

    /* Set Maximum Rise Time: 1000ns max in Standard Mode
    = [1000ns/(1/InputClockFrequencyMHz.10e6)]+1
    = InputClockFrequencyMHz+1 */
    I2C->TRISER = (uint8_t)(InputClockFrequencyMHz + (uint8_t)1);

  }

  /* Write CCR with new calculated value */
  I2C->CCRL = (uint8_t)result;
  I2C->CCRH = (uint8_t)((uint8_t)((uint8_t)(result >> 8) & I2C_CCRH_CCR) | tmpccrh);

  /* Enable I2C */
  I2C->CR1 |= I2C_CR1_PE;

  /* Configure I2C acknowledgement */
  I2C_AcknowledgeConfig(Ack);

  /*--------------------------- I2C OAR Configuration ------------------------*/
  I2C->OARL = (uint8_t)(OwnAddress);
  I2C->OARH = (uint8_t)((uint8_t)(AddMode | I2C_OARH_ADDCONF) |
                   (uint8_t)((OwnAddress & (uint16_t)0x0300) >> (uint8_t)7));
}

/**
  * @brief  Enables or disables the I2C peripheral.
  * @param  NewState : Indicate the new I2C peripheral state.
  *         This parameter can be any of the @ref FunctionalState enumeration.
  * @retval None
  */
void I2C_Cmd(FunctionalState NewState)
{
  /* Check function parameters */
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));

  if (NewState != DISABLE)
  {
    /* Enable I2C peripheral */
    I2C->CR1 |= I2C_CR1_PE;
  }
  else /* NewState == DISABLE */
  {
    /* Disable I2C peripheral */
    I2C->CR1 &= (uint8_t)(~I2C_CR1_PE);
  }
}

/**
  * @brief  Enables or disables the I2C General Call feature.
  * @param  NewState : State of the General Call feature.
  *         This parameter can be any of the @ref FunctionalState enumeration.
  * @retval None
  */
void I2C_GeneralCallCmd(FunctionalState NewState)
{
  /* Check function parameters */
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));

  if (NewState != DISABLE)
  {
    /* Enable General Call */
    I2C->CR1 |= I2C_CR1_ENGC;
  }
  else /* NewState == DISABLE */
  {
    /* Disable General Call */
    I2C->CR1 &= (uint8_t)(~I2C_CR1_ENGC);
  }
}

/**
  * @brief  Generates I2C communication START condition.
  * @note   CCR must be programmed, i.e. I2C_Init function must have been called
  *         with a valid I2C_ClockSpeed
  * @param  NewState : Enable or disable the start condition.
  *         This parameter can be any of the @ref FunctionalState enumeration.
  * @retval None
  */
void I2C_GenerateSTART(FunctionalState NewState)
{
  /* Check function parameters */
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));

  if (NewState != DISABLE)
  {
    /* Generate a START condition */
    I2C->CR2 |= I2C_CR2_START;
  }
  else /* NewState == DISABLE */
  {
    /* Disable the START condition generation */
    I2C->CR2 &= (uint8_t)(~I2C_CR2_START);
  }
}

/**
  * @brief  Generates I2C communication STOP condition.
  * @param   NewState : Enable or disable the stop condition.
  *          This parameter can be any of the @ref FunctionalState enumeration.
  * @retval None
  */
void I2C_GenerateSTOP(FunctionalState NewState)
{
  /* Check function parameters */
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));

  if (NewState != DISABLE)
  {
    /* Generate a STOP condition */
    I2C->CR2 |= I2C_CR2_STOP;
  }
  else /* NewState == DISABLE */
  {
    /* Disable the STOP condition generation */
    I2C->CR2 &= (uint8_t)(~I2C_CR2_STOP);
  }
}

/**
  * @brief  Enables or disables I2C software reset.
  * @param  NewState : Specifies the new state of the I2C software reset.
  *         This parameter can be any of the @ref FunctionalState enumeration.
  * @retval None
  */
void I2C_SoftwareResetCmd(FunctionalState NewState)
{
  /* Check function parameters */
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));

  if (NewState != DISABLE)
  {
    /* Peripheral under reset */
    I2C->CR2 |= I2C_CR2_SWRST;
  }
  else /* NewState == DISABLE */
  {
    /* Peripheral not under reset */
    I2C->CR2 &= (uint8_t)(~I2C_CR2_SWRST);
  }
}

/**
  * @brief  Enables or disables the I2C clock stretching.
  * @param  NewState : Specifies the new state of the I2C Clock stretching.
  *         This parameter can be any of the @ref FunctionalState enumeration.
  * @retval None
  */

void I2C_StretchClockCmd(FunctionalState NewState)
{
  /* Check function parameters */
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));

  if (NewState != DISABLE)
  {
    /* Clock Stretching Enable */
    I2C->CR1 &= (uint8_t)(~I2C_CR1_NOSTRETCH);

  }
  else /* NewState == DISABLE */
  {
    /* Clock Stretching Disable (Slave mode) */
    I2C->CR1 |= I2C_CR1_NOSTRETCH;
  }
}

/**
  * @brief  Enable or Disable the I2C acknowledge and position acknowledge feature.
  * @note   This function must be called before data reception start
  * @param  Ack : Specifies the acknowledge mode to apply.
  *         This parameter can be any of the  @ref I2C_Ack_TypeDef enumeration.
  * @retval None
  */
void I2C_AcknowledgeConfig(I2C_Ack_TypeDef Ack)
{
  /* Check function parameters */
  assert_param(IS_I2C_ACK_OK(Ack));

  if (Ack == I2C_ACK_NONE)
  {
    /* Disable the acknowledgement */
    I2C->CR2 &= (uint8_t)(~I2C_CR2_ACK);
  }
  else
  {
    /* Enable the acknowledgement */
    I2C->CR2 |= I2C_CR2_ACK;

    if (Ack == I2C_ACK_CURR)
    {
      /* Configure (N)ACK on current byte */
      I2C->CR2 &= (uint8_t)(~I2C_CR2_POS);
    }
    else
    {
      /* Configure (N)ACK on next byte */
      I2C->CR2 |= I2C_CR2_POS;
    }
  }
}

/**
  * @brief  Enables or disables the specified I2C interrupt.
  * @param  ITName : Name of the interrupt to enable or disable.
  *         This parameter can be any of the  @ref I2C_IT_TypeDef enumeration.
  * @param  NewState : State of the interrupt to apply.
  *         This parameter can be any of the @ref FunctionalState enumeration.
  * @retval None
  */
void I2C_ITConfig(I2C_IT_TypeDef I2C_IT, FunctionalState NewState)
{
  /* Check functions parameters */
  assert_param(IS_I2C_INTERRUPT_OK(I2C_IT));
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected I2C interrupts */
    I2C->ITR |= (uint8_t)I2C_IT;
  }
  else /* NewState == DISABLE */
  {
    /* Disable the selected I2C interrupts */
    I2C->ITR &= (uint8_t)(~(uint8_t)I2C_IT);
  }
}

/**
  * @brief  Selects the specified I2C fast mode duty cycle.
  * @param  I2C_DutyCycle : Specifies the duty cycle to apply.
  *         This parameter can be any of the @ref I2C_DutyCycle_TypeDef enumeration.
  * @retval None
  */
void I2C_FastModeDutyCycleConfig(I2C_DutyCycle_TypeDef I2C_DutyCycle)
{
  /* Check function parameters */
  assert_param(IS_I2C_DUTYCYCLE_OK(I2C_DutyCycle));

  if (I2C_DutyCycle == I2C_DUTYCYCLE_16_9)
  {
    /* I2C fast mode Tlow/Thigh = 16/9 */
    I2C->CCRH |= I2C_CCRH_DUTY;
  }
  else /* I2C_DUTYCYCLE_2 */
  {
    /* I2C fast mode Tlow/Thigh = 2 */
    I2C->CCRH &= (uint8_t)(~I2C_CCRH_DUTY);
  }
}

/**
  * @brief  Returns the most recent received data.
  * @param  None
  * @retval uint8_t : The value of the received byte data.
  */
uint8_t I2C_ReceiveData(void)
{
  /* Return the data present in the DR register */
  return ((uint8_t)I2C->DR);
}

/**
  * @brief  Transmits the 7-bit address (to select the) slave device.
  * @param   Address : Specifies the slave address which will be transmitted.
  * @param   Direction : Specifies whether the I2C device will be a Transmitter or a Receiver.
  * This parameter can be any of the @ref I2C_Direction_TypeDef enumeration.
  * @retval None
  */
void I2C_Send7bitAddress(uint8_t Address, I2C_Direction_TypeDef Direction)
{
  /* Check function parameters */
  assert_param(IS_I2C_ADDRESS_OK(Address));
  assert_param(IS_I2C_DIRECTION_OK(Direction));

  /* Clear bit0 (direction) just in case */
  Address &= (uint8_t)0xFE;

  /* Send the Address + Direction */
  I2C->DR = (uint8_t)(Address | (uint8_t)Direction);
}

/**
  * @brief  Send a byte by writing in the DR register.
  * @param   Data : Byte to be sent.
  * @retval None
  */
void I2C_SendData(uint8_t Data)
{
  /* Write in the DR register the data to be sent */
  I2C->DR = Data;
}

/**
 * @brief
 ****************************************************************************************
 *
 *                         I2C State Monitoring Functions
 *
 ****************************************************************************************
 * This I2C driver provides three different ways for I2C state monitoring
 *  depending on the application requirements and constraints:
 *
 *
 * 1) Basic state monitoring:
 *    Using I2C_CheckEvent() function:
 *    It compares the status registers (SR1, SR2 and SR3) content to a given event
 *    (can be the combination of one or more flags).
 *    It returns SUCCESS if the current status includes the given flags
 *    and returns ERROR if one or more flags are missing in the current status.
 *    - When to use:
 *      - This function is suitable for most applications as well as for startup
 *      activity since the events are fully described in the product reference manual
 *      (RM0016).
 *      - It is also suitable for users who need to define their own events.
 *    - Limitations:
 *      - If an error occurs (ie. error flags are set besides to the monitored flags),
 *        the I2C_CheckEvent() function may return SUCCESS despite the communication
 *        hold or corrupted real state.
 *        In this case, it is advised to use error interrupts to monitor the error
 *        events and handle them in the interrupt IRQ handler.
 *
 *        @note
 *        For error management, it is advised to use the following functions:
 *          - I2C_ITConfig() to configure and enable the error interrupts (I2C_IT_ERR).
 *          - I2C_IRQHandler() which is called when the I2C interrupts occur.
 *          - I2C_GetFlagStatus() or I2C_GetITStatus() to be called into the
 *           I2C_IRQHandler() function in order to determine which error occurred.
 *          - I2C_ClearFlag() or I2C_ClearITPendingBit() and/or I2C_SoftwareResetCmd()
 *            and/or I2C_GenerateStop() in order to clear the error flag and
 *            source and return to correct communication status.
 *
 *
 *  2) Advanced state monitoring:
 *     Using the function I2C_GetLastEvent() which returns the image of both SR1
 *     & SR3 status registers in a single word (uint16_t) (Status Register 3 value
 *     is shifted left by 8 bits and concatenated to Status Register 1).
 *     - When to use:
 *       - This function is suitable for the same applications above but it allows to
 *         overcome the limitations of I2C_GetFlagStatus() function (see below).
 *         The returned value could be compared to events already defined in the
 *         library (stm8s_i2c.h) or to custom values defined by user.
 *       - This function is suitable when multiple flags are monitored at the same time.
 *       - At the opposite of I2C_CheckEvent() function, this function allows user to
 *         choose when an event is accepted (when all events flags are set and no
 *         other flags are set or just when the needed flags are set like
 *         I2C_CheckEvent() function).
 *     - Limitations:
 *       - User may need to define his own events.
 *       - Same remark concerning the error management is applicable for this
 *         function if user decides to check only regular communication flags (and
 *         ignores error flags).
 *
 *
 *  3) Flag-based state monitoring:
 *     Using the function I2C_GetFlagStatus() which simply returns the status of
 *     one single flag (ie. I2C_FLAG_RXNE ...).
 *     - When to use:
 *        - This function could be used for specific applications or in debug phase.
 *        - It is suitable when only one flag checking is needed (most I2C events
 *          are monitored through multiple flags).
 *     - Limitations:
 *        - When calling this function, the Status register is accessed. Some flags are
 *          cleared when the status register is accessed. So checking the status
 *          of one Flag, may clear other ones.
 *        - Function may need to be called twice or more in order to monitor one
 *          single event.
 *
 *  For detailed description of Events, please refer to section I2C_Events in
 *  stm8s_i2c.h file.
 *
 */
/**
 *
 *  1) Basic state monitoring
 *******************************************************************************
 */

/**
  * @brief  Checks whether the last I2C Event is equal to the one passed
  *   as parameter.
  * @param  I2C_EVENT: specifies the event to be checked.
  *   This parameter can be one of the following values:
  *     @arg I2C_EVENT_SLAVE_TRANSMITTER_ADDRESS_MATCHED           : EV1
  *     @arg I2C_EVENT_SLAVE_RECEIVER_ADDRESS_MATCHED              : EV1
  *     @arg I2C_EVENT_SLAVE_GENERALCALLADDRESS_MATCHED            : EV1
  *     @arg I2C_EVENT_SLAVE_BYTE_RECEIVED                         : EV2
  *     @arg (I2C_EVENT_SLAVE_BYTE_RECEIVED | I2C_FLAG_GENCALL)    : EV2
  *     @arg I2C_EVENT_SLAVE_BYTE_TRANSMITTED                      : EV3
  *     @arg (I2C_EVENT_SLAVE_BYTE_TRANSMITTED | I2C_FLAG_GENCALL) : EV3
  *     @arg I2C_EVENT_SLAVE_ACK_FAILURE                           : EV3_2
  *     @arg I2C_EVENT_SLAVE_STOP_DETECTED                         : EV4
  *     @arg I2C_EVENT_MASTER_MODE_SELECT                          : EV5
  *     @arg I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED            : EV6
  *     @arg I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED               : EV6
  *     @arg I2C_EVENT_MASTER_BYTE_RECEIVED                        : EV7
  *     @arg I2C_EVENT_MASTER_BYTE_TRANSMITTING                    : EV8
  *     @arg I2C_EVENT_MASTER_BYTE_TRANSMITTED                     : EV8_2
  *     @arg I2C_EVENT_MASTER_MODE_ADDRESS10                       : EV9
  *
  * @note: For detailed description of Events, please refer to section
  *    I2C_Events in stm8s_i2c.h file.
  *
  * @retval An ErrorStatus enumeration value:
  * - SUCCESS: Last event is equal to the I2C_EVENT
  * - ERROR: Last event is different from the I2C_EVENT
  */
ErrorStatus I2C_CheckEvent(I2C_Event_TypeDef I2C_Event)
{
  __IO uint16_t lastevent = 0x00;
  uint8_t flag1 = 0x00 ;
  uint8_t flag2 = 0x00;
  ErrorStatus status = ERROR;

  /* Check the parameters */
  assert_param(IS_I2C_EVENT_OK(I2C_Event));

  if (I2C_Event == I2C_EVENT_SLAVE_ACK_FAILURE)
  {
    lastevent = I2C->SR2 & I2C_SR2_AF;
  }
  else
  {
    flag1 = I2C->SR1;
    flag2 = I2C->SR3;
    lastevent = ((uint16_t)((uint16_t)flag2 << (uint16_t)8) | (uint16_t)flag1);
  }
  /* Check whether the last event is equal to I2C_EVENT */
  if (((uint16_t)lastevent & (uint16_t)I2C_Event) == (uint16_t)I2C_Event)
  {
    /* SUCCESS: last event is equal to I2C_EVENT */
    status = SUCCESS;
  }
  else
  {
    /* ERROR: last event is different from I2C_EVENT */
    status = ERROR;
  }

  /* Return status */
  return status;
}

/**
 *
 *  2) Advanced state monitoring
 *******************************************************************************
 */
/**
  * @brief  Returns the last I2C Event.
  *
  * @note: For detailed description of Events, please refer to section
  *    I2C_Events in stm8s_i2c.h file.
  *
  * @retval The last event
  *   This parameter can be any of the  @ref I2C_Event_TypeDef enumeration.
  */
I2C_Event_TypeDef I2C_GetLastEvent(void)
{
  __IO uint16_t lastevent = 0;
  uint16_t flag1 = 0;
  uint16_t flag2 = 0;

  if ((I2C->SR2 & I2C_SR2_AF) != 0x00)
  {
    lastevent = I2C_EVENT_SLAVE_ACK_FAILURE;
  }
  else
  {
    /* Read the I2C status register */
    flag1 = I2C->SR1;
    flag2 = I2C->SR3;

    /* Get the last event value from I2C status register */
    lastevent = ((uint16_t)((uint16_t)flag2 << 8) | (uint16_t)flag1);
  }
  /* Return status */
  return (I2C_Event_TypeDef)lastevent;
}

/**
 *
 *  3) Flag-based state monitoring
 *******************************************************************************
 */
/**
  * @brief  Checks whether the specified I2C flag is set or not.
  * @param  I2C_FLAG: specifies the flag to check.
  *   This parameter can be one of the following values:
  *     @arg I2C_FLAG_GENERALCALL: General call header flag (Slave mode)
  *     @arg I2C_FLAG_TRANSMITTERRECEIVER: Transmitter/Receiver flag
  *     @arg I2C_FLAG_BUSBUSY: Bus busy flag
  *     @arg I2C_FLAG_MASTERSLAVE: Master/Slave flag
  *     @arg I2C_FLAG_WAKEUPFROMHALT: Wake up from HALT flag  
  *     @arg I2C_FLAG_OVERRUNUNDERRUN: Overrun/Underrun flag (Slave mode)
  *     @arg I2C_FLAG_ACKNOWLEDGEFAILURE: Acknowledge failure flag
  *     @arg I2C_FLAG_ARBITRATIONLOSS: Arbitration lost flag (Master mode)
  *     @arg I2C_FLAG_BUSERROR: Bus error flag
  *     @arg I2C_FLAG_TXEMPTY: Data register empty flag (Transmitter)
  *     @arg I2C_FLAG_RXNOTEMPTY: Data register not empty (Receiver) flag
  *     @arg I2C_FLAG_STOPDETECTION: Stop detection flag (Slave mode)
  *     @arg I2C_FLAG_HEADERSENT: 10-bit header sent flag (Master mode)
  *     @arg I2C_FLAG_TRANSFERFINISHED: Byte transfer finished flag
  *     @arg I2C_FLAG_ADDRESSSENTMATCHED: Address sent flag (Master mode) “ADSL”
  *   Address matched flag (Slave mode)”ENDAD”
  *     @arg I2C_FLAG_STARTDETECTION: Start bit flag (Master mode)
  * @retval The new state of I2C_FLAG (SET or RESET).
  */
FlagStatus I2C_GetFlagStatus(I2C_Flag_TypeDef I2C_Flag)
{
  uint8_t tempreg = 0;
  uint8_t regindex = 0;
  FlagStatus bitstatus = RESET;

  /* Check the parameters */
  assert_param(IS_I2C_FLAG_OK(I2C_Flag));

  /* Read flag register index */
  regindex = (uint8_t)((uint16_t)I2C_Flag >> 8);
  /* Check SRx index */
  switch (regindex)
  {
      /* Returns whether the status register to check is SR1 */
    case 0x01:
      tempreg = (uint8_t)I2C->SR1;
      break;

      /* Returns whether the status register to check is SR2 */
    case 0x02:
      tempreg = (uint8_t)I2C->SR2;
      break;

      /* Returns whether the status register to check is SR3 */
    case 0x03:
      tempreg = (uint8_t)I2C->SR3;
      break;

    default:
      break;
  }

  /* Check the status of the specified I2C flag */
  if ((tempreg & (uint8_t)I2C_Flag ) != 0)
  {
    /* Flag is set */
    bitstatus = SET;
  }
  else
  {
    /* Flag is reset */
    bitstatus = RESET;
  }
  /* Return the flag status */
  return bitstatus;
}

/**
  * @brief  Clear flags
  * @param  I2C_Flag : Specifies the flag to clear
  *   This parameter can be any combination of the following values:
  *                       - I2C_FLAG_WAKEUPFROMHALT: Wakeup from Halt
  *                       - I2C_FLAG_OVERRUNUNDERRUN: Overrun/Underrun flag (Slave mode)
  *                       - I2C_FLAG_ACKNOWLEDGEFAILURE: Acknowledge failure flag
  *                       - I2C_FLAG_ARBITRATIONLOSS: Arbitration lost flag (Master mode)
  *                       - I2C_FLAG_BUSERROR: Bus error flag.
  * @note Notes:
  *                       - STOPF (STOP detection) is cleared by software
  *                         sequence: a read operation to I2C_SR1 register
  *                         (I2C_GetFlagStatus()) followed by a write operation
  *                         to I2C_CR2 register.
  *                       - ADD10 (10-bit header sent) is cleared by software
  *                         sequence: a read operation to I2C_SR1
  *                         (I2C_GetFlagStatus()) followed by writing the
  *                         second byte of the address in DR register.
  *                       - BTF (Byte Transfer Finished) is cleared by software
  *                         sequence: a read operation to I2C_SR1 register
  *                         (I2C_GetFlagStatus()) followed by a read/write to
  *                         I2C_DR register (I2C_SendData()).
  *                       - ADDR (Address sent) is cleared by software sequence:
  *                         a read operation to I2C_SR1 register
  *                         (I2C_GetFlagStatus()) followed by a read operation to
  *                         I2C_SR3 register ((void)(I2C->SR3)).
  *                       - SB (Start Bit) is cleared software sequence: a read
  *                         operation to I2C_SR1 register (I2C_GetFlagStatus())
  *                         followed by a write operation to I2C_DR register
  *                         (I2C_SendData()).
  * @retval None
  */
void I2C_ClearFlag(I2C_Flag_TypeDef I2C_FLAG)
{
  uint16_t flagpos = 0;
  /* Check the parameters */
  assert_param(IS_I2C_CLEAR_FLAG_OK(I2C_FLAG));

  /* Get the I2C flag position */
  flagpos = (uint16_t)I2C_FLAG & FLAG_Mask;
  /* Clear the selected I2C flag */
  I2C->SR2 = (uint8_t)((uint16_t)(~flagpos));
}

/**
  * @brief  Checks whether the specified I2C interrupt has occurred or not.
  * @param  I2C_ITPendingBit: specifies the interrupt source to check.
  *            This parameter can be one of the following values:
  *               - I2C_ITPENDINGBIT_WAKEUPFROMHALT: Wakeup from Halt
  *               - I2C_ITPENDINGBIT_OVERRUNUNDERRUN: Overrun/Underrun flag (Slave mode)
  *               - I2C_ITPENDINGBIT_ACKNOWLEDGEFAILURE: Acknowledge failure flag
  *               - I2C_ITPENDINGBIT_ARBITRATIONLOSS: Arbitration lost flag (Master mode)
  *               - I2C_ITPENDINGBIT_BUSERROR: Bus error flag
  *               - I2C_ITPENDINGBIT_TXEMPTY: Data register empty flag (Transmitter)
  *               - I2C_ITPENDINGBIT_RXNOTEMPTY: Data register not empty (Receiver) flag
  *               - I2C_ITPENDINGBIT_STOPDETECTION: Stop detection flag (Slave mode)
  *               - I2C_ITPENDINGBIT_HEADERSENT: 10-bit header sent flag (Master mode)
  *               - I2C_ITPENDINGBIT_TRANSFERFINISHED: Byte transfer finished flag
  *               - I2C_ITPENDINGBIT_ADDRESSSENTMATCHED: Address sent flag (Master mode) “ADSL”
  *                              Address matched flag (Slave mode)“ENDAD”
  *               - I2C_ITPENDINGBIT_STARTDETECTION: Start bit flag (Master mode)
  * @retval The new state of I2C_ITPendingBit
  *   This parameter can be any of the @ref ITStatus enumeration.
  */
ITStatus I2C_GetITStatus(I2C_ITPendingBit_TypeDef I2C_ITPendingBit)
{
  ITStatus bitstatus = RESET;
  __IO uint8_t enablestatus = 0;
  uint16_t tempregister = 0;

    /* Check the parameters */
    assert_param(IS_I2C_ITPENDINGBIT_OK(I2C_ITPendingBit));

  tempregister = (uint8_t)( ((uint16_t)((uint16_t)I2C_ITPendingBit & ITEN_Mask)) >> 8);

  /* Check if the interrupt source is enabled or not */
  enablestatus = (uint8_t)(I2C->ITR & ( uint8_t)tempregister);

  if ((uint16_t)((uint16_t)I2C_ITPendingBit & REGISTER_Mask) == REGISTER_SR1_Index)
  {
    /* Check the status of the specified I2C flag */
    if (((I2C->SR1 & (uint8_t)I2C_ITPendingBit) != RESET) && enablestatus)
    {
      /* I2C_IT is set */
      bitstatus = SET;
    }
    else
    {
      /* I2C_IT is reset */
      bitstatus = RESET;
    }
  }
  else
  {
    /* Check the status of the specified I2C flag */
    if (((I2C->SR2 & (uint8_t)I2C_ITPendingBit) != RESET) && enablestatus)
    {
      /* I2C_IT is set */
      bitstatus = SET;
    }
    else
    {
      /* I2C_IT is reset */
      bitstatus = RESET;
    }
  }
  /* Return the I2C_IT status */
  return  bitstatus;
}

/**
  * @brief  Clear IT pending bit
  * @param  I2C_IT: specifies the interrupt pending bit to clear.
  *            This parameter can be any combination of the following values:
  *                 - I2C_ITPENDINGBIT_WAKEUPFROMHALT: Wakeup from Halt
  *                 - I2C_ITPENDINGBIT_OVERRUNUNDERRUN: Overrun/Underrun interrupt (Slave mode)
  *                 - I2C_ITPENDINGBIT_ACKNOWLEDGEFAILURE: Acknowledge failure interrupt
  *                 - I2C_ITPENDINGBIT_ARBITRATIONLOSS: Arbitration lost interrupt (Master mode)
  *                 - I2C_ITPENDINGBIT_BUSERROR: Bus error interrupt
  *
  *             Notes:
  *                  - STOPF (STOP detection) is cleared by software
  *                    sequence: a read operation to I2C_SR1 register
  *                    (I2C_GetITStatus()) followed by a write operation to
  *                    I2C_CR2 register (I2C_AcknowledgeConfig() to configure
  *                    the I2C peripheral Acknowledge).
  *                  - ADD10 (10-bit header sent) is cleared by software
  *                    sequence: a read operation to I2C_SR1
  *                    (I2C_GetITStatus()) followed by writing the second
  *                    byte of the address in I2C_DR register.
  *                  - BTF (Byte Transfer Finished) is cleared by software
  *                    sequence: a read operation to I2C_SR1 register
  *                    (I2C_GetITStatus()) followed by a read/write to
  *                    I2C_DR register (I2C_SendData()).
  *                  - ADDR (Address sent) is cleared by software sequence:
  *                    a read operation to I2C_SR1 register (I2C_GetITStatus())
  *                    followed by a read operation to I2C_SR3 register
  *                    ((void)(I2C->SR3)).
  *                  - SB (Start Bit) is cleared by software sequence: a
  *                    read operation to I2C_SR1 register (I2C_GetITStatus())
  *                    followed by a write operation to I2C_DR register
  *                    (I2C_SendData()).
  * @retval None
  */
void I2C_ClearITPendingBit(I2C_ITPendingBit_TypeDef I2C_ITPendingBit)
{
  uint16_t flagpos = 0;

  /* Check the parameters */
  assert_param(IS_I2C_CLEAR_ITPENDINGBIT_OK(I2C_ITPendingBit));

  /* Get the I2C flag position */
  flagpos = (uint16_t)I2C_ITPendingBit & FLAG_Mask;

  /* Clear the selected I2C flag */
  I2C->SR2 = (uint8_t)((uint16_t)~flagpos);
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
