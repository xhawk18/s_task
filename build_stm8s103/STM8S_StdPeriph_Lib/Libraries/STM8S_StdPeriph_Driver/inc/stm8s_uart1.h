/**
  ********************************************************************************
  * @file    stm8s_uart1.h
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   This file contains all functions prototypes and macros for the UART1 peripheral.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM8S_UART1_H
#define __STM8S_UART1_H

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"

/** @addtogroup STM8S_StdPeriph_Driver
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/** @addtogroup UART1_Exported_Types
  * @{
  */


/**
  * @brief  UART1 Irda Modes
  */

typedef enum { UART1_IRDAMODE_NORMAL    = (uint8_t)0x00, /**< 0x00 Irda Normal Mode   */
               UART1_IRDAMODE_LOWPOWER  = (uint8_t)0x01  /**< 0x01 Irda Low Power Mode */
             } UART1_IrDAMode_TypeDef;

/**
  * @brief  UART1 WakeUP Modes
  */
typedef enum { UART1_WAKEUP_IDLELINE       = (uint8_t)0x00, /**< 0x01 Idle Line wake up                */
               UART1_WAKEUP_ADDRESSMARK    = (uint8_t)0x08  /**< 0x02 Address Mark wake up          */
             } UART1_WakeUp_TypeDef;

/**
  * @brief  UART1 LIN Break detection length possible values
  */
typedef enum { UART1_LINBREAKDETECTIONLENGTH_10BITS = (uint8_t)0x00, /**< 0x01 10 bits Lin Break detection            */
               UART1_LINBREAKDETECTIONLENGTH_11BITS = (uint8_t)0x01  /**< 0x02 11 bits Lin Break detection          */
             } UART1_LINBreakDetectionLength_TypeDef;

/**
  * @brief  UART1 stop bits possible values
  */

typedef enum { UART1_STOPBITS_1   = (uint8_t)0x00,    /**< One stop bit is  transmitted at the end of frame*/
               UART1_STOPBITS_0_5 = (uint8_t)0x10,    /**< Half stop bits is transmitted at the end of frame*/
               UART1_STOPBITS_2   = (uint8_t)0x20,    /**< Two stop bits are  transmitted at the end of frame*/
               UART1_STOPBITS_1_5 = (uint8_t)0x30     /**< One and half stop bits*/
             } UART1_StopBits_TypeDef;

/**
  * @brief  UART1 parity possible values
  */
typedef enum { UART1_PARITY_NO     = (uint8_t)0x00,      /**< No Parity*/
               UART1_PARITY_EVEN   = (uint8_t)0x04,      /**< Even Parity*/
               UART1_PARITY_ODD    = (uint8_t)0x06       /**< Odd Parity*/
             } UART1_Parity_TypeDef;

/**
  * @brief  UART1 Synchrone modes
  */
typedef enum { UART1_SYNCMODE_CLOCK_DISABLE    = (uint8_t)0x80, /**< 0x80 Sync mode Disable, SLK pin Disable */
               UART1_SYNCMODE_CLOCK_ENABLE     = (uint8_t)0x08, /**< 0x08 Sync mode Enable, SLK pin Enable     */
               UART1_SYNCMODE_CPOL_LOW         = (uint8_t)0x40, /**< 0x40 Steady low value on SCLK pin outside transmission window */
               UART1_SYNCMODE_CPOL_HIGH        = (uint8_t)0x04, /**< 0x04 Steady high value on SCLK pin outside transmission window */
               UART1_SYNCMODE_CPHA_MIDDLE      = (uint8_t)0x20, /**< 0x20 SCLK clock line activated in middle of data bit     */
               UART1_SYNCMODE_CPHA_BEGINING    = (uint8_t)0x02, /**< 0x02 SCLK clock line activated at beginning of data bit  */
               UART1_SYNCMODE_LASTBIT_DISABLE  = (uint8_t)0x10, /**< 0x10 The clock pulse of the last data bit is not output to the SCLK pin */
               UART1_SYNCMODE_LASTBIT_ENABLE   = (uint8_t)0x01  /**< 0x01 The clock pulse of the last data bit is output to the SCLK pin */
             } UART1_SyncMode_TypeDef;

/**
  * @brief  UART1 Word length possible values
  */
typedef enum { UART1_WORDLENGTH_8D = (uint8_t)0x00,/**< 0x00 8 bits Data  */
               UART1_WORDLENGTH_9D = (uint8_t)0x10 /**< 0x10 9 bits Data  */
             } UART1_WordLength_TypeDef;

/**
  * @brief  UART1 Mode possible values
  */
typedef enum { UART1_MODE_RX_ENABLE     = (uint8_t)0x08,  /**< 0x08 Receive Enable */
               UART1_MODE_TX_ENABLE     = (uint8_t)0x04,  /**< 0x04 Transmit Enable */
               UART1_MODE_TX_DISABLE    = (uint8_t)0x80,  /**< 0x80 Transmit Disable */
               UART1_MODE_RX_DISABLE    = (uint8_t)0x40,  /**< 0x40 Single-wire Half-duplex mode */
               UART1_MODE_TXRX_ENABLE   = (uint8_t)0x0C  /**< 0x0C Transmit Enable and Receive Enable */
             } UART1_Mode_TypeDef;

/**
  * @brief  UART1 Flag possible values
  */
typedef enum { UART1_FLAG_TXE   = (uint16_t)0x0080, /*!< Transmit Data Register Empty flag */
               UART1_FLAG_TC    = (uint16_t)0x0040, /*!< Transmission Complete flag */
               UART1_FLAG_RXNE  = (uint16_t)0x0020, /*!< Read Data Register Not Empty flag */
               UART1_FLAG_IDLE  = (uint16_t)0x0010, /*!< Idle line detected flag */
               UART1_FLAG_OR    = (uint16_t)0x0008, /*!< OverRun error flag */
               UART1_FLAG_NF    = (uint16_t)0x0004, /*!< Noise error flag */
               UART1_FLAG_FE    = (uint16_t)0x0002, /*!< Framing Error flag */
               UART1_FLAG_PE    = (uint16_t)0x0001, /*!< Parity Error flag */
               UART1_FLAG_LBDF  = (uint16_t)0x0210, /*!< Line Break Detection Flag */
               UART1_FLAG_SBK   = (uint16_t)0x0101  /*!< Send Break characters Flag */
             } UART1_Flag_TypeDef;

/**
  * @brief  UART1 Interrupt definition
  * UART1_IT possible values
  * Elements values convention: 0xZYX
  * X: Position of the corresponding Interrupt
  *   - For the following values, X means the interrupt position in the CR2 register.
  *     UART1_IT_TXE
  *     UART1_IT_TC
  *     UART1_IT_RXNE
  *     UART1_IT_IDLE 
  *     UART1_IT_OR 
  *   - For the UART1_IT_PE value, X means the flag position in the CR1 register.
  *   - For the UART1_IT_LBDF value, X means the flag position in the CR4 register.
  * Y: Flag position
  *  - For the following values, Y means the flag (pending bit) position in the SR register.
  *     UART1_IT_TXE
  *     UART1_IT_TC
  *     UART1_IT_RXNE
  *     UART1_IT_IDLE 
  *     UART1_IT_OR
  *     UART1_IT_PE
  *  - For the UART1_IT_LBDF value, Y means the flag position in the CR4 register.
  * Z: Register index: indicate in which register the dedicated interrupt source is:
  *  - 1==> CR1 register
  *  - 2==> CR2 register
  *  - 3==> CR4 register
  */
typedef enum { UART1_IT_TXE        = (uint16_t)0x0277, /*!< Transmit interrupt */
               UART1_IT_TC         = (uint16_t)0x0266, /*!< Transmission Complete interrupt */
               UART1_IT_RXNE       = (uint16_t)0x0255, /*!< Receive interrupt */
               UART1_IT_IDLE       = (uint16_t)0x0244, /*!< IDLE line interrupt */
               UART1_IT_OR         = (uint16_t)0x0235, /*!< Overrun Error interrupt */
               UART1_IT_PE         = (uint16_t)0x0100, /*!< Parity Error interrupt */
               UART1_IT_LBDF       = (uint16_t)0x0346, /**< LIN break detection interrupt */
               UART1_IT_RXNE_OR    = (uint16_t)0x0205  /*!< Receive/Overrun interrupt */
             } UART1_IT_TypeDef;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/* Exported macros ------------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/

/** @addtogroup UART1_Private_Macros
  * @{
  */

/**
  * @brief  Macro used by the assert function to check the different functions parameters.
  */

/**
 * @brief Macro used by the assert_param function in order to check the different
 *        sensitivity values for the MODEs possible combination should be one of
 *        the following
 */
#define IS_UART1_MODE_OK(Mode) \
  (((Mode) == (uint8_t)UART1_MODE_RX_ENABLE) || \
   ((Mode) == (uint8_t)UART1_MODE_RX_DISABLE) || \
   ((Mode) == (uint8_t)UART1_MODE_TX_ENABLE) || \
   ((Mode) == (uint8_t)UART1_MODE_TX_DISABLE) || \
   ((Mode) == (uint8_t)UART1_MODE_TXRX_ENABLE) || \
   ((Mode) == (uint8_t)((uint8_t)UART1_MODE_TX_ENABLE|(uint8_t)UART1_MODE_RX_ENABLE)) || \
   ((Mode) == (uint8_t)((uint8_t)UART1_MODE_TX_ENABLE|(uint8_t)UART1_MODE_RX_DISABLE)) || \
   ((Mode) == (uint8_t)((uint8_t)UART1_MODE_TX_DISABLE|(uint8_t)UART1_MODE_RX_DISABLE)) || \
   ((Mode) == (uint8_t)((uint8_t)UART1_MODE_TX_DISABLE|(uint8_t)UART1_MODE_RX_ENABLE)))

/**
 * @brief Macro used by the assert_param function in order to check the different
 *        sensitivity values for the WordLengths
 */
#define IS_UART1_WORDLENGTH_OK(WordLength) \
  (((WordLength) == UART1_WORDLENGTH_8D) || \
   ((WordLength) == UART1_WORDLENGTH_9D))

/**
  * @brief  Macro used by the assert_param function in order to check the different
  *         sensitivity values for the SyncModes; it should exclude values such 
  *         as  UART1_CLOCK_ENABLE|UART1_CLOCK_DISABLE
  */
#define IS_UART1_SYNCMODE_OK(SyncMode) \
  (!((((SyncMode)&(((uint8_t)UART1_SYNCMODE_CLOCK_ENABLE)|((uint8_t)UART1_SYNCMODE_CLOCK_DISABLE))) == (((uint8_t)UART1_SYNCMODE_CLOCK_ENABLE)|((uint8_t)UART1_SYNCMODE_CLOCK_DISABLE))) \
    || (((SyncMode)&(((uint8_t)UART1_SYNCMODE_CPOL_LOW )|((uint8_t)UART1_SYNCMODE_CPOL_HIGH))) == (((uint8_t)UART1_SYNCMODE_CPOL_LOW )|((uint8_t)UART1_SYNCMODE_CPOL_HIGH))) \
    ||(((SyncMode)&(((uint8_t)UART1_SYNCMODE_CPHA_MIDDLE)|((uint8_t)UART1_SYNCMODE_CPHA_BEGINING))) ==  (((uint8_t)UART1_SYNCMODE_CPHA_MIDDLE)|((uint8_t)UART1_SYNCMODE_CPHA_BEGINING))) \
    || (((SyncMode)&(((uint8_t)UART1_SYNCMODE_LASTBIT_DISABLE)|((uint8_t)UART1_SYNCMODE_LASTBIT_ENABLE))) == (((uint8_t)UART1_SYNCMODE_LASTBIT_DISABLE)|((uint8_t)UART1_SYNCMODE_LASTBIT_ENABLE)))))

/**
  * @brief  Macro used by the assert_param function in order to check the different
  *         sensitivity values for the FLAGs
  */
#define IS_UART1_FLAG_OK(Flag) \
  (((Flag) == UART1_FLAG_TXE) || \
   ((Flag) == UART1_FLAG_TC)  || \
   ((Flag) == UART1_FLAG_RXNE) || \
   ((Flag) == UART1_FLAG_IDLE) || \
   ((Flag) == UART1_FLAG_OR) || \
   ((Flag) == UART1_FLAG_NF) || \
   ((Flag) == UART1_FLAG_FE) || \
   ((Flag) == UART1_FLAG_PE) || \
   ((Flag) == UART1_FLAG_SBK) || \
   ((Flag) == UART1_FLAG_LBDF))
/**
  * @brief  Macro used by the assert_param function in order to check the different
  *         sensitivity values for the FLAGs that can be cleared by writing 0
  */
#define IS_UART1_CLEAR_FLAG_OK(Flag) \
  (((Flag) == UART1_FLAG_RXNE) || \
   ((Flag) == UART1_FLAG_LBDF))



/**
  * @brief  Macro used by the assert_param function in order to check the different 
  *         sensitivity values for the Interrupts
  */

#define IS_UART1_CONFIG_IT_OK(Interrupt) \
  (((Interrupt) == UART1_IT_PE) || \
   ((Interrupt) == UART1_IT_TXE) || \
   ((Interrupt) == UART1_IT_TC) || \
   ((Interrupt) == UART1_IT_RXNE_OR ) || \
   ((Interrupt) == UART1_IT_IDLE) || \
   ((Interrupt) == UART1_IT_LBDF))

/**
  * @brief  Macro used by the assert function in order to check the different 
  *         sensitivity values for the pending bit
  */
#define IS_UART1_GET_IT_OK(ITPendingBit) \
  (((ITPendingBit) == UART1_IT_TXE)  || \
   ((ITPendingBit) == UART1_IT_TC)   || \
   ((ITPendingBit) == UART1_IT_RXNE) || \
   ((ITPendingBit) == UART1_IT_IDLE) || \
   ((ITPendingBit) == UART1_IT_OR)  || \
   ((ITPendingBit) == UART1_IT_LBDF)  || \
   ((ITPendingBit) == UART1_IT_PE))

/**
  * @brief  Macro used by the assert function in order to check the different 
  *         sensitivity values for the pending bit that can be cleared by writing 0
  */
#define IS_UART1_CLEAR_IT_OK(ITPendingBit) \
  (((ITPendingBit) == UART1_IT_RXNE) || \
   ((ITPendingBit) == UART1_IT_LBDF))


/**
 * @brief Macro used by the assert_param function in order to check the different
 *        sensitivity values for the IrDAModes
 */
#define IS_UART1_IRDAMODE_OK(IrDAMode) \
  (((IrDAMode) == UART1_IRDAMODE_LOWPOWER) || \
   ((IrDAMode) == UART1_IRDAMODE_NORMAL))

/**
  * @brief  Macro used by the assert_param function in order to check the different
  *         sensitivity values for the WakeUps
  */
#define IS_UART1_WAKEUP_OK(WakeUp) \
  (((WakeUp) == UART1_WAKEUP_IDLELINE) || \
   ((WakeUp) == UART1_WAKEUP_ADDRESSMARK))

/**
  * @brief  Macro used by the assert_param function in order to check the different 
  *        sensitivity values for the LINBreakDetectionLengths
  */
#define IS_UART1_LINBREAKDETECTIONLENGTH_OK(LINBreakDetectionLength) \
  (((LINBreakDetectionLength) == UART1_LINBREAKDETECTIONLENGTH_10BITS) || \
   ((LINBreakDetectionLength) == UART1_LINBREAKDETECTIONLENGTH_11BITS))

/**
  * @brief  Macro used by the assert_param function in order to check the different
  *         sensitivity values for the UART1_StopBits
  */
#define IS_UART1_STOPBITS_OK(StopBit) (((StopBit) == UART1_STOPBITS_1) || \
                                       ((StopBit) == UART1_STOPBITS_0_5) || \
                                       ((StopBit) == UART1_STOPBITS_2) || \
                                       ((StopBit) == UART1_STOPBITS_1_5 ))

/**
 * @brief Macro used by the assert_param function in order to check the different
 *        sensitivity values for the Parity
 */
#define IS_UART1_PARITY_OK(Parity) (((Parity) == UART1_PARITY_NO) || \
                                    ((Parity) == UART1_PARITY_EVEN) || \
                                    ((Parity) == UART1_PARITY_ODD ))

/**
 * @brief Macro used by the assert_param function in order to check the maximum
 *        baudrate value
 */
#define IS_UART1_BAUDRATE_OK(NUM) ((NUM) <= (uint32_t)625000)


/**
 * @brief Macro used by the assert_param function in order to check the address
 *        of the UART1 or UART node
 */
#define UART1_ADDRESS_MAX ((uint8_t)16)
#define IS_UART1_ADDRESS_OK(node) ((node) < UART1_ADDRESS_MAX )

/**
  * @}
  */

/* Exported functions ------------------------------------------------------- */

/** @addtogroup UART1_Exported_Functions
  * @{
  */

void UART1_DeInit(void);
void UART1_Init(uint32_t BaudRate, UART1_WordLength_TypeDef WordLength, 
                UART1_StopBits_TypeDef StopBits, UART1_Parity_TypeDef Parity, 
                UART1_SyncMode_TypeDef SyncMode, UART1_Mode_TypeDef Mode);
void UART1_Cmd(FunctionalState NewState);
void UART1_ITConfig(UART1_IT_TypeDef UART1_IT, FunctionalState NewState);
void UART1_HalfDuplexCmd(FunctionalState NewState);
void UART1_IrDAConfig(UART1_IrDAMode_TypeDef UART1_IrDAMode);
void UART1_IrDACmd(FunctionalState NewState);
void UART1_LINBreakDetectionConfig(UART1_LINBreakDetectionLength_TypeDef UART1_LINBreakDetectionLength);
void UART1_LINCmd(FunctionalState NewState);
void UART1_SmartCardCmd(FunctionalState NewState);
void UART1_SmartCardNACKCmd(FunctionalState NewState);
void UART1_WakeUpConfig(UART1_WakeUp_TypeDef UART1_WakeUp);
void UART1_ReceiverWakeUpCmd(FunctionalState NewState);
uint8_t UART1_ReceiveData8(void);
uint16_t UART1_ReceiveData9(void);
void UART1_SendData8(uint8_t Data);
void UART1_SendData9(uint16_t Data);
void UART1_SendBreak(void);
void UART1_SetAddress(uint8_t UART1_Address);
void UART1_SetGuardTime(uint8_t UART1_GuardTime);
void UART1_SetPrescaler(uint8_t UART1_Prescaler);
FlagStatus UART1_GetFlagStatus(UART1_Flag_TypeDef UART1_FLAG);
void UART1_ClearFlag(UART1_Flag_TypeDef UART1_FLAG);
ITStatus UART1_GetITStatus(UART1_IT_TypeDef UART1_IT);
void UART1_ClearITPendingBit(UART1_IT_TypeDef UART1_IT);

/**
  * @}
  */

#endif /* __STM8S_UART1_H */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
