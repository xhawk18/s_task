/**
  ******************************************************************************
  * @file    stm8_eval_i2c_ee.c
  * @author  MCD Application Team
  * @version V2.1.3
  * @date    28-June-2013
  * @brief   This file provides a set of functions needed to manage the I2C M24CXX
  *          EEPROM memory mounted on STM8xx-EVAL board (refer to stm8_eval.h
  *          to know about the boards supporting this memory).
  *          It implements a high level communication layer for read and write
  *          from/to this memory. The needed STM8 hardware resources (I2C and
  *          GPIO) are defined in stm8xx_eval.h file, and the initialization is
  *          performed in sEE_LowLevel_Init() function declared in stm8xx_eval.c
  *          file.
  *          You can easily tailor this driver to any other development board,
  *          by just adapting the defines for hardware resources and
  *          sEE_LowLevel_Init() function.
  *
  *          @note In this driver, basic read and write functions (sEE_ReadBuffer()
  *                and sEE_WritePage()) use the DMA to perform the data transfer
  *                to/from EEPROM memory (except when number of requested data is
  *                equal to 1). Thus, after calling these two functions, user
  *                application may perform other tasks while DMA is transferring
  *                data. The application should then monitor the variable holding
  *                the number of data in order to determine when the transfer is
  *                completed (variable decremented to 0). 
  *                Stopping transfer tasks are performed into DMA interrupt 
  *                handlers by the mean of sEE_I2C_DMA_TX_IRQHandler() and 
  *                sEE_I2C_DMA_RX_IRQHandler() functions. These tow functions should 
  *                be called in the stm8l15x_it.c file respectively in 
  *                DMA1_CHANNEL2_3_IRQHandler and DMA1_CHANNEL0_1_IRQHandler.
  *                For more details on the use of this driver you can refer to 
  *                the I2C_EEPROM example provided within the STM8L15x_StdPeriph_Lib 
  *                package.      
  *
  *     +-----------------------------------------------------------------+
  *     |                        Pin assignment                           |
  *     +---------------------------------------+-----------+-------------+
  *     |  STM8 I2C Pins                       |   sEE     |   Pin       |
  *     +---------------------------------------+-----------+-------------+
  *     | .                                     |   E0(GND) |    1  (0V)  |
  *     | .                                     |   E1(GND) |    2  (0V)  |
  *     | .                                     |   E2(GND) |    3  (0V)  |
  *     | .                                     |   E0(VSS) |    4  (0V)  |
  *     | sEE_I2C_SDA_PIN/ SDA                  |   SDA     |    5        |
  *     | sEE_I2C_SCL_PIN/ SCL                  |   SCL     |    6        |
  *     | .                                     |   /WC(VDD)|    7 (3.3V) |
  *     | .                                     |   VDD     |    8 (3.3V) |
  *     +---------------------------------------+-----------+-------------+
  ******************************************************************************
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
#include "stm8_eval_i2c_ee.h"

/** @addtogroup Utilities
  * @{
  */

/** @addtogroup STM8_EVAL
  * @{
  */

/** @addtogroup Common
  * @{
  */

/** @addtogroup STM8_EVAL_I2C_EE
  * @brief      This file includes the I2C EEPROM driver of STM8-EVAL boards.
  * @{
  */

/* Private types -------------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/


/** @defgroup STM8_EVAL_I2C_EE_Private_Variables
  * @{
  */
__IO uint16_t sEEAddress = 0;
__IO uint32_t sEETimeout = sEE_TIMEOUT_MAX;
__IO uint16_t* sEEDataReadPointer;
__IO uint8_t* sEEDataWritePointer;
__IO uint8_t sEEDataNum;
/**
  * @}
  */

/* Private function prototypes -----------------------------------------------*/


/** @defgroup STM8_EVAL_I2C_EE_Private_Functions
  * @{
  */

/**
  * @brief  DeInitializes peripherals used by the I2C EEPROM driver.
  * @param  None
  * @retval None
  */
void sEE_DeInit(void)
{
  sEE_LowLevel_DeInit();
}

/**
  * @brief  Initializes peripherals used by the I2C EEPROM driver.
  * @param  None
  * @retval None
  */
void sEE_Init(void)
{
  sEE_LowLevel_Init();

  /*!< I2C configuration */
  /* sEE_I2C Peripheral Enable */
  I2C_Cmd(sEE_I2C, ENABLE);
  /* sEE_I2C configuration after enabling it */
  I2C_Init(sEE_I2C, I2C_SPEED, I2C_SLAVE_ADDRESS7, I2C_Mode_I2C, I2C_DutyCycle_2,
           I2C_Ack_Enable, I2C_AcknowledgedAddress_7bit);

  /* Enable the sEE_I2C peripheral DMA requests */
  I2C_DMACmd(sEE_I2C, ENABLE);

#if defined (sEE_M24C64_32)
  /*!< Select the EEPROM address according to the state of E0, E1, E2 pins */
  sEEAddress = sEE_HW_ADDRESS;
#endif /*!< sEE_M24C64_32 */
}

/**
  * @brief  Writes one byte to the I2C EEPROM.
  * @param  pBuffer : pointer to the buffer  containing the data to be written
  *         to the EEPROM.
  * @param  WriteAddr : EEPROM's internal address to write to.
  * @retval None
  */
void sEE_WriteByte(uint8_t* pBuffer, uint16_t WriteAddr)
{
  /*!< Send STRAT condition */
  I2C_GenerateSTART(sEE_I2C, ENABLE);

  /*!< Test on EV5 and clear it */
  while (!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_MODE_SELECT))
  {}

  /*!< Send EEPROM address for write */
  I2C_Send7bitAddress(sEE_I2C, (uint8_t)sEEAddress, I2C_Direction_Transmitter);

  /*!< Test on EV6 and clear it */
  while (!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {}

#ifdef sEE_M24C64_32

  /*!< Send the EEPROM's internal address to write to : MSB of the address first */
  I2C_SendData(sEE_I2C, (uint8_t)((WriteAddr & 0xFF00) >> 8));

  /*!< Test on EV8 and clear it */
  while (!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {}

  /*!< Send the EEPROM's internal address to write to : LSB of the address */
  I2C_SendData(sEE_I2C, (uint8_t)(WriteAddr & 0x00FF));

#endif /*!< sEE_M24C64_32 */

  /*!< Test on EV8 and clear it */
  while (!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {}

  /*!< Send the byte to be written */
  I2C_SendData(sEE_I2C, *pBuffer);

  /*!< Test on EV8 and clear it */
  while (!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {}

  /*!< Send STOP condition */
  I2C_GenerateSTOP(sEE_I2C, ENABLE);
}

/**
  * @brief  Reads a block of data from the EEPROM.
  * @param  pBuffer : pointer to the buffer that receives the data read from
  *         the EEPROM.
  * @param  ReadAddr : EEPROM's internal address to read from.
  * @param  NumByteToRead : pointer to the variable holding number of bytes to
  *         read from the EEPROM.
  *
  *        @note The variable pointed by NumByteToRead is reset to 0 when all the
  *              data are read from the EEPROM. Application should monitor this
  *              variable in order know when the transfer is complete.
  *
  * @note When number of data to be read is higher than 1, this function just
  *       configure the communication and enable the DMA channel to transfer data.
  *       Meanwhile, the user application may perform other tasks.
  *       When number of data to be read is 1, then the DMA is not used.
  *
  * @retval None
  */
void sEE_ReadBuffer(uint8_t* pBuffer, uint16_t ReadAddr, uint16_t* NumByteToRead)
{
  __IO uint32_t timeout = 0xFFFF;

  /*!< Wait the end of last communication */
  for (;timeout > 0; timeout--);

  /* Set the pointer to the Number of data to be read. This pointer will be used
      by the DMA Transfer Complete interrupt Handler in order to reset the 
      variable to 0. User should check on this variable in order to know if the 
      DMA transfer has been completed or not. */
  sEEDataReadPointer = NumByteToRead;

  /*!< While the bus is busy */
  while (I2C_GetFlagStatus(sEE_I2C, I2C_FLAG_BUSY))
  {}

  /*!< Send START condition */
  I2C_GenerateSTART(sEE_I2C, ENABLE);

  /*!< Test on EV5 and clear it */
  while (!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_MODE_SELECT))
  {}

  /*!< Send EEPROM address for write */
  I2C_Send7bitAddress(sEE_I2C, (uint8_t)sEEAddress, I2C_Direction_Transmitter);

  /*!< Test on EV6 and clear it */
  while (!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {}

#ifdef sEE_M24C64_32

  /*!< Send the EEPROM's internal address to read from: MSB of the address first */
  I2C_SendData(sEE_I2C, (uint8_t)((ReadAddr & 0xFF00) >> 8));

  /*!< Test on EV8 and clear it */
  while (!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {}

  /*!< Send the EEPROM's internal address to read from: LSB of the address */
  I2C_SendData(sEE_I2C, (uint8_t)(ReadAddr & 0x00FF));

#endif /*!< sEE_M24C64_32 */

  /*!< Test on EV8 and clear it */
  while (!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {}

  /*!< Send STRAT condition a second time */
  I2C_GenerateSTART(sEE_I2C, ENABLE);

  /*!< Test on EV5 and clear it */
  while (!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_MODE_SELECT))
  {}

  /*!< Send EEPROM address for read */
  I2C_Send7bitAddress(sEE_I2C, (uint8_t)sEEAddress, I2C_Direction_Receiver);

  /*!< Test on EV6 and clear it */
  while (!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
  {}

  /* If number of data to be read is 1, then DMA couldn't be used */
  if ((uint16_t)(*NumByteToRead) < 2)
  {
    /*!< Disable Acknowledgement */
    I2C_AcknowledgeConfig(sEE_I2C, DISABLE);

    /*!< Send STOP Condition */
    I2C_GenerateSTOP(sEE_I2C, ENABLE);

    /*!< Test on EV7 and clear it */
    while (!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED))
    {}

    /*!< Read a byte from the EEPROM */
    *pBuffer = I2C_ReceiveData(sEE_I2C);

    /*!< Decrement the read bytes counter */
    (uint16_t)(*NumByteToRead)--;

    /*!< Enable Acknowledgement to be ready for another reception */
    I2C_AcknowledgeConfig(sEE_I2C, ENABLE);
  }
  /* DMA could be used for number of data higher than 1 */
  else
  {
    /* Configure the DMA Rx Channel with the buffer address and the buffer size */
    sEE_LowLevel_DMAConfig((uint16_t)pBuffer, (uint8_t)(*NumByteToRead), sEE_DIRECTION_RX);

    /* Inform the DMA that the next End Of Transfer Signal will be the last one */
    I2C_DMALastTransferCmd(sEE_I2C, ENABLE);

    /* Enable the DMA Rx Channel */
    DMA_Cmd(sEE_I2C_DMA_CHANNEL_RX, ENABLE);

    /* Global DMA Enable */
    DMA_GlobalCmd(ENABLE);
  }
}

/**
  * @brief  Writes buffer of data to the I2C EEPROM.
  * @param  pBuffer : pointer to the buffer  containing the data to be written
  *         to the EEPROM.
  * @param  WriteAddr : EEPROM's internal address to write to.
  * @param  NumByteToWrite : number of bytes to write to the EEPROM.
  * @retval None
  */
void sEE_WriteBuffer(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite)
{
  uint8_t NumOfPage = 0, NumOfSingle = 0, count = 0;
  uint16_t Addr = 0;

  Addr = WriteAddr % sEE_PAGESIZE;
  count = (uint8_t)(sEE_PAGESIZE - (uint16_t)Addr);
  NumOfPage =  (uint8_t)(NumByteToWrite / sEE_PAGESIZE);
  NumOfSingle = (uint8_t)(NumByteToWrite % sEE_PAGESIZE);

  /*!< If WriteAddr is sEE_PAGESIZE aligned  */
  if (Addr == 0)
  {
    /*!< If NumByteToWrite < sEE_PAGESIZE */
    if (NumOfPage == 0)
    {
      /* Store the number of data to be written */
      sEEDataNum = NumOfSingle;
      /* Start writing data */
      sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));
      /* Wait transfer through DMA to be complete */
      sEETimeout = sEE_TIMEOUT_MAX;
      while ((sEEDataNum > 0) && (sEETimeout-- > 0))
      {}
      sEE_WaitEepromStandbyState();
    }
    /*!< If NumByteToWrite > sEE_PAGESIZE */
    else
    {
      while (NumOfPage--)
      {
        /* Store the number of data to be written */
        sEEDataNum = sEE_PAGESIZE;
        sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));
        /* Wait transfer through DMA to be complete */
        sEETimeout = sEE_TIMEOUT_MAX;
        while ((sEEDataNum > 0) && (sEETimeout-- > 0))
        {}
        sEE_WaitEepromStandbyState();
        WriteAddr +=  sEE_PAGESIZE;
        pBuffer += sEE_PAGESIZE;
      }

      if (NumOfSingle != 0)
      {
        /* Store the number of data to be written */
        sEEDataNum = NumOfSingle;
        sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));
        /* Wait transfer through DMA to be complete */
        sEETimeout = sEE_TIMEOUT_MAX;
        while ((sEEDataNum > 0) && (sEETimeout-- > 0))
        {}
        sEE_WaitEepromStandbyState();
      }
    }
  }
  /*!< If WriteAddr is not sEE_PAGESIZE aligned  */
  else
  {
    /*!< If NumByteToWrite < sEE_PAGESIZE */
    if (NumOfPage == 0)
    {
      /*!< If the number of data to be written is more than the remaining space
      in the current page: */
      if (NumByteToWrite > count)
      {
        /* Store the number of data to be written */
        sEEDataNum = count;
        /*!< Write the data conained in same page */
        sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));
        /* Wait transfer through DMA to be complete */
        sEETimeout = sEE_TIMEOUT_MAX;
        while ((sEEDataNum > 0) && (sEETimeout-- > 0))
        {}
        sEE_WaitEepromStandbyState();

        /* Store the number of data to be written */
        sEEDataNum = (uint8_t)(NumByteToWrite - count);
        /*!< Write the remaining data in the following page */
        sEE_WritePage((uint8_t*)(pBuffer + count), (WriteAddr + count), (uint8_t*)(&sEEDataNum));
        /* Wait transfer through DMA to be complete */
        sEETimeout = sEE_TIMEOUT_MAX;
        while ((sEEDataNum > 0) && (sEETimeout-- > 0))
        {}
        sEE_WaitEepromStandbyState();
      }
      else
      {
        /* Store the number of data to be written */
        sEEDataNum = NumOfSingle;
        sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));
        /* Wait transfer through DMA to be complete */
        sEETimeout = sEE_TIMEOUT_MAX;
        while ((sEEDataNum > 0) && (sEETimeout-- > 0))
        {}
        sEE_WaitEepromStandbyState();
      }
    }
    /*!< If NumByteToWrite > sEE_PAGESIZE */
    else
    {
      NumByteToWrite -= count;
      NumOfPage = (uint8_t)(NumByteToWrite / sEE_PAGESIZE);
      NumOfSingle = (uint8_t)(NumByteToWrite % sEE_PAGESIZE);

      if (count != 0)
      {
        /* Store the number of data to be written */
        sEEDataNum = count;
        sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));
        /* Wait transfer through DMA to be complete */
        sEETimeout = sEE_TIMEOUT_MAX;
        while ((sEEDataNum > 0) && (sEETimeout-- > 0))
        {}
        sEE_WaitEepromStandbyState();
        WriteAddr += count;
        pBuffer += count;
      }

      while (NumOfPage--)
      {
        /* Store the number of data to be written */
        sEEDataNum = sEE_PAGESIZE;
        sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));
        /* Wait transfer through DMA to be complete */
        sEETimeout = sEE_TIMEOUT_MAX;
        while ((sEEDataNum > 0) && (sEETimeout-- > 0))
        {}
        sEE_WaitEepromStandbyState();
        WriteAddr +=  sEE_PAGESIZE;
        pBuffer += sEE_PAGESIZE;
      }
      if (NumOfSingle != 0)
      {
        /* Store the number of data to be written */
        sEEDataNum = NumOfSingle;
        sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));
        /* Wait transfer through DMA to be complete */
        sEETimeout = sEE_TIMEOUT_MAX;
        while ((sEEDataNum > 0) && (sEETimeout-- > 0))
        {}
        sEE_WaitEepromStandbyState();
      }
    }
  }
}

/**
  * @brief  Writes more than one byte to the EEPROM with a single WRITE cycle.
  * @note   The number of byte can't exceed the EEPROM page size.
  * @param  pBuffer : pointer to the buffer containing the data to be written to
  *         the EEPROM.
  * @param  WriteAddr : EEPROM's internal address to write to.
  * @param  NumByteToWrite : pointer to the variable holding number of bytes to
  *         written to the EEPROM.
  *
  *        @note The variable pointed by NumByteToWrite is reset to 0 when all the
  *              data are read from the EEPROM. Application should monitor this
  *              variable in order know when the transfer is complete.
  *
  * @note When number of data to be written is higher than 1, this function just
  *       configure the communication and enable the DMA channel to transfer data.
  *       Meanwhile, the user application may perform other tasks.
  *       When number of data to be written is 1, then the DMA is not used.
  *
  * @retval None
  */
void sEE_WritePage(uint8_t* pBuffer, uint16_t WriteAddr, uint8_t* NumByteToWrite)
{
  __IO uint32_t timeout = 0xFFFF;

  /*!< Wait the end of last communication */
  for (;timeout > 0; timeout--);

  /* Set the pointer to the Number of data to be written. This pointer will be used
      by the DMA Transfer Completer interrupt Handler in order to reset the 
      variable to 0. User should check on this variable in order to know if the 
      DMA transfer has been complete or not. */
  sEEDataWritePointer = NumByteToWrite;

  /*!< While the bus is busy */
  while (I2C_GetFlagStatus(sEE_I2C, I2C_FLAG_BUSY))
  {}

  /*!< Send START condition */
  I2C_GenerateSTART(sEE_I2C, ENABLE);

  /*!< Test on EV5 and clear it */
  while (!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_MODE_SELECT))
  {}

  /*!< Send EEPROM address for write */
  I2C_Send7bitAddress(sEE_I2C, (uint8_t)sEEAddress, I2C_Direction_Transmitter);

  /*!< Test on EV6 and clear it */
  while (!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {}

#ifdef sEE_M24C64_32

  /*!< Send the EEPROM's internal address to write to : MSB of the address first */
  I2C_SendData(sEE_I2C, (uint8_t)((WriteAddr & 0xFF00) >> 8));

  /*!< Test on EV8 and clear it */
  while (!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {}

  /*!< Send the EEPROM's internal address to write to : LSB of the address */
  I2C_SendData(sEE_I2C, (uint8_t)(WriteAddr & 0x00FF));

#endif /*!< sEE_M24C64_32 */

  /*!< Test on EV8 and clear it */
  while (! I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {}

  /* If number of data to be written is 1, then DMA couldn't be used */
  if ((uint16_t)(*NumByteToWrite) < 2)
  {
    /*!< Send the byte to be written */
    I2C_SendData(sEE_I2C, *pBuffer);

    /*!< Test on EV8 and clear it */
    while (!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {}

    /*!< Send STOP condition */
    I2C_GenerateSTOP(sEE_I2C, ENABLE);

    (uint8_t)(*NumByteToWrite)--;
  }
  /* DMA could be used for number of data higher than 1 */
  else
  {
    /* Configure the DMA Tx Channel with the buffer address and the buffer size */
    sEE_LowLevel_DMAConfig((uint16_t)pBuffer, (uint8_t)(*NumByteToWrite), sEE_DIRECTION_TX);

    /* Enable the DMA Tx Channel */
    DMA_Cmd(sEE_I2C_DMA_CHANNEL_TX, ENABLE);

    /* Global DMA Enable */
    DMA_GlobalCmd(ENABLE);
  }
}

/**
  * @brief  Wait for EEPROM Standby state
  * @param  None
  * @retval None
  */
void sEE_WaitEepromStandbyState(void)
{
  __IO uint8_t tempreg = 0;
  __IO uint32_t timeout = 0xFFFF;

  do
  {
    /*!< Send START condition */
    I2C_GenerateSTART(sEE_I2C, ENABLE);

    /* Test on EEPROM_I2C EV5 and clear it */
    while (!I2C_GetFlagStatus(sEE_I2C, I2C_FLAG_SB))  /* EV5 */
    {
    }

    /*!< Send EEPROM address for write */
    I2C_Send7bitAddress(sEE_I2C, (uint8_t)sEEAddress, I2C_Direction_Transmitter);

    /*!< Wait for address aknowledgement */
    for (;timeout > 0; timeout--);

    /*!< Read sEE SR1 register to clear pending flags */
    tempreg = I2C_ReadRegister(sEE_I2C, I2C_Register_SR1);

  }
  while (!(tempreg & 0x02));

  /*!< Clear AF flag */
  I2C_ClearFlag(sEE_I2C, I2C_FLAG_AF);

  /*!< STOP condition */
  I2C_GenerateSTOP(sEE_I2C, ENABLE);
}

/**
  * @}
  */


/**
  * @brief  This function handles the DMA Tx Channel interrupt Handler.
  *     @note This function should be called in the
  *       DMA1_CHANNEL2_3_IRQHandler in the stm8l15x_it.c file.
  *
  *       // INTERRUPT_HANDLER(DMA1_CHANNEL2_3_IRQHandler, 3)
  *       // {
  *           // sEE_I2C_DMA_TX_IRQHandler();
  *       // }
  * @param  None
  * @retval None
  */
void sEE_I2C_DMA_TX_IRQHandler(void)
{
  /* Check if the DMA transfer is complete */
  if (DMA_GetFlagStatus(sEE_I2C_DMA_FLAG_TX_TC) != RESET)
  {
    /* Disable the DMA Tx Channel and Clear all its Flags */
    DMA_Cmd(sEE_I2C_DMA_CHANNEL_TX, DISABLE);
    DMA_ClearFlag(sEE_I2C_DMA_FLAG_TX_TC);

    /*!< Wait till all data have been physically transferred on the bus */
    while (!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {}

    /*!< Send STOP condition */
    I2C_GenerateSTOP(sEE_I2C, ENABLE);

    /* Reset the variable holding the number of data to be written */
    *sEEDataWritePointer = 0;
  }
}

/**
  * @brief  This function handles the DMA Rx Channel interrupt Handler.
  *     @note This function should be called in the
  *       DMA1_CHANNEL0_1_IRQHandler in the stm8l15x_it.c file
  *       just as follow.
  *
  *       // INTERRUPT_HANDLER(DMA1_CHANNEL0_1_IRQHandler, 2)
  *       // {
  *           // sEE_I2C_DMA_RX_IRQHandler();
  *       // }
  * @param  None
  * @retval None
  */
void sEE_I2C_DMA_RX_IRQHandler(void)
{
  /* Check if the DMA transfer is complete */
  if (DMA_GetFlagStatus(sEE_I2C_DMA_FLAG_RX_TC) != RESET)
  {
    /*!< Send STOP Condition */
    I2C_GenerateSTOP(sEE_I2C, ENABLE);

    /* Disable the DMA Rx Channel and Clear all its Flags */
    DMA_Cmd(sEE_I2C_DMA_CHANNEL_RX, DISABLE);
    DMA_ClearFlag(sEE_I2C_DMA_FLAG_RX_TC);

    /* Reset the variable holding the number of data to be read */
    *sEEDataReadPointer = 0;
  }
}

/**
  * @}
  */


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
