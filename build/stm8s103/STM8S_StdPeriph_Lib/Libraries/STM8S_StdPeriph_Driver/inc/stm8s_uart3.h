/**
  ********************************************************************************
  * @file    stm8s_uart3.h
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   This file contains all functions prototypes and macros for the UART3 peripheral.
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
#ifndef __STM8S_UART3_H
#define __STM8S_UART3_H

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"

/** @addtogroup STM8S_StdPeriph_Driver
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/** @addtogroup UART3_Exported_Types
  * @{
  */

/**
  * @brief  UART3 WakeUP Modes
  */
typedef enum { UART3_WAKEUP_IDLELINE      = (uint8_t)0x00,  /**< 0x01 Idle Line wake up*/
               UART3_WAKEUP_ADDRESSMARK   = (uint8_t)0x08   /**< 0x02 Address Mark wake up*/
             } UART3_WakeUp_TypeDef;

/**
  * @brief  UART3 LIN Break detection length possible values
  */
typedef enum { UART3_LINBREAKDETECTIONLENGTH_10BITS = (uint8_t)0x00, /**< 10 bits Lin Break detection */
               UART3_LINBREAKDETECTIONLENGTH_11BITS = (uint8_t)0x01  /**< 11 bits Lin Break detection */
             } UART3_LINBreakDetectionLength_TypeDef;


/**
  * @brief  UART3 stop bits possible values
  */
typedef enum { UART3_STOPBITS_1  = (uint8_t)0x00,  /**< One stop bit is  transmitted at the end of frame*/
               UART3_STOPBITS_2  = (uint8_t)0x20   /**< Two stop bits are  transmitted at the end of frame*/
             } UART3_StopBits_TypeDef;

/**
  * @brief  UART3 parity possible values
  */
typedef enum { UART3_PARITY_NO    = (uint8_t)0x00,    /**< No Parity*/
               UART3_PARITY_EVEN  = (uint8_t)0x04,    /**< Even Parity*/
               UART3_PARITY_ODD   = (uint8_t)0x06     /**< Odd Parity*/
             } UART3_Parity_TypeDef;

/**
  * @brief  UART3 Word length possible values
  */
typedef enum { UART3_WORDLENGTH_8D = (uint8_t)0x00,    /**< 0x00 8 bits Data*/
               UART3_WORDLENGTH_9D = (uint8_t)0x10     /**< 0x10 9 bits Data*/
             } UART3_WordLength_TypeDef;

/**
  * @brief  UART3 Mode Transmit/Receive possible values
  */
typedef enum { UART3_MODE_RX_ENABLE    = (uint8_t)0x08,  /**< 0x08 Receive Enable*/
               UART3_MODE_TX_ENABLE    = (uint8_t)0x04,  /**< 0x04 Transmit Enable*/
               UART3_MODE_TX_DISABLE   = (uint8_t)0x80,  /**< 0x80 Receive Enable*/
               UART3_MODE_RX_DISABLE   = (uint8_t)0x40,  /**< 0x40 Single-wire Half-duplex mode*/
               UART3_MODE_TXRX_ENABLE  = (uint8_t)0x0C   /**< 0x0C Receive Enable and Transmit enable*/
             } UART3_Mode_TypeDef;

/**
  * @brief  UART3 Mode possible values
  */
typedef enum { UART3_LIN_MODE_MASTER  = (uint8_t)0x00, /**<  LIN Master Mode*/
               UART3_LIN_MODE_SLAVE   = (uint8_t)0x01  /**<  LIN Slave Mode*/
             } UART3_LinMode_TypeDef;
/**
  * @brief  UART3 automatic resynchronisation possible values
  */
typedef enum { UART3_LIN_AUTOSYNC_DISABLE   = (uint8_t)0x00,  /**< LIN Autosynchronization Disable*/
               UART3_LIN_AUTOSYNC_ENABLE    = (uint8_t)0x01   /**< LIN Autosynchronization Enable*/
             } UART3_LinAutosync_TypeDef;
/**
  * @brief  UART3 Divider Update Method possible values
  */
typedef enum { UART3_LIN_DIVUP_LBRR1    = (uint8_t)0x00, /**<  LIN LDIV is updated as soon as LBRR1 is written*/
               UART3_LIN_DIVUP_NEXTRXNE = (uint8_t)0x01  /**<  LIN LDIV is updated at the next received character*/
             } UART3_LinDivUp_TypeDef;


/**
  * @brief  UART3 Flag possible values
  */
typedef enum
{
  UART3_FLAG_TXE         = (uint16_t)0x0080, /*!< Transmit Data Register Empty flag */
  UART3_FLAG_TC          = (uint16_t)0x0040, /*!< Transmission Complete flag */
  UART3_FLAG_RXNE        = (uint16_t)0x0020, /*!< Read Data Register Not Empty flag */
  UART3_FLAG_IDLE        = (uint16_t)0x0010, /*!< Idle line detected flag */
  UART3_FLAG_OR_LHE      = (uint16_t)0x0008, /*!< OverRun error flag */
  UART3_FLAG_NF          = (uint16_t)0x0004, /*!< Noise error flag */
  UART3_FLAG_FE          = (uint16_t)0x0002, /*!< Framing Error flag */
  UART3_FLAG_PE          = (uint16_t)0x0001, /*!< Parity Error flag */
  UART3_FLAG_SBK         = (uint16_t)0x0101,  /**< Send Break Complete interrupt flag */
  UART3_FLAG_LBDF        = (uint16_t)0x0210, /**< LIN Break Detection Flag           */
  UART3_FLAG_LHDF        = (uint16_t)0x0302, /**< LIN Header Detection Flag*/
  UART3_FLAG_LSF         = (uint16_t)0x0301  /**< LIN Sync Field Flag*/
} UART3_Flag_TypeDef;

/**
  * @brief  UART3 Interrupt definition
  * UART3_IT possible values
  * Elements values convention: 0xZYX
  * X: Position of the corresponding Interrupt
  *   - For the following values, X means the interrupt position in the CR2 register.
  *     UART3_IT_TXE
  *     UART3_IT_TC
  *     UART3_IT_RXNE
  *     UART3_IT_IDLE 
  *     UART3_IT_OR 
  *   - For the UART3_IT_PE value, X means the flag position in the CR1 register.
  *   - For the UART3_IT_LBDF value, X means the flag position in the CR4 register.
  *   - For the UART3_IT_LHDF value, X means the flag position in the CR6 register.
  * Y: Flag position
  *  - For the following values, Y means the flag (pending bit) position in the SR register.
  *     UART3_IT_TXE
  *     UART3_IT_TC
  *     UART3_IT_RXNE
  *     UART3_IT_IDLE 
  *     UART3_IT_OR
  *     UART3_IT_PE
  *  - For the UART3_IT_LBDF value, Y means the flag position in the CR4 register.
  *  - For the UART3_IT_LHDF value, Y means the flag position in the CR6 register.
  * Z: Register index: indicate in which register the dedicated interrupt source is:
  *  - 1==> CR1 register
  *  - 2==> CR2 register
  *  - 3==> CR4 register
  *  - 4==> CR6 register
  */
typedef enum { UART3_IT_TXE        = (uint16_t)0x0277, /**< Transmit interrupt */
               UART3_IT_TC         = (uint16_t)0x0266, /**< Transmission Complete interrupt */
               UART3_IT_RXNE       = (uint16_t)0x0255, /**< Data Register Not Empty interrupt */
               UART3_IT_IDLE       = (uint16_t)0x0244, /**< Idle line detected interrupt */
               UART3_IT_OR         = (uint16_t)0x0235, /**< OverRun error interrupt */
               UART3_IT_PE         = (uint16_t)0x0100, /**< Parity Error interrupt */
               UART3_IT_LBDF       = (uint16_t)0x0346, /**< LIN Break Detection interrupt */
               UART3_IT_LHDF       = (uint16_t)0x0412, /**< LIN Header Detection interrupt*/
               UART3_IT_RXNE_OR    = (uint16_t)0x0205  /*!< Receive/Overrun interrupt */
             } UART3_IT_TypeDef;


/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/* Exported macros ------------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/

/** @addtogroup UART3_Private_Macros
  * @{
  */

/**
  * @brief  Macro used by the assert_param function in order to check the 
  *         different sensitivity values for the FLAGs
  */
#define IS_UART3_FLAG_OK(Flag) \
  (((Flag) == UART3_FLAG_TXE) || \
   ((Flag) == UART3_FLAG_TC)  || \
   ((Flag) == UART3_FLAG_RXNE) || \
   ((Flag) == UART3_FLAG_IDLE) || \
   ((Flag) == UART3_FLAG_OR_LHE) || \
   ((Flag) == UART3_FLAG_NF) || \
   ((Flag) == UART3_FLAG_FE) || \
   ((Flag) == UART3_FLAG_PE) || \
   ((Flag) == UART3_FLAG_SBK) || \
   ((Flag) == UART3_FLAG_LSF) || \
   ((Flag) == UART3_FLAG_LHDF) || \
   ((Flag) == UART3_FLAG_LBDF))


/**
  * @brief  Macro used by the assert_param function in order to check the 
  *         different sensitivity values for the FLAGs that can be cleared by writing 0
  */
#define IS_UART3_CLEAR_FLAG_OK(Flag) \
  (((Flag) == UART3_FLAG_RXNE) || \
   ((Flag) == UART3_FLAG_LHDF) || \
   ((Flag) == UART3_FLAG_LSF) || \
   ((Flag) == UART3_FLAG_LBDF))

/**
  * @brief  Macro used by the assert_param function in order to check the 
  *         different sensitivity values for the Interrupts
  */

#define IS_UART3_CONFIG_IT_OK(Interrupt) \
  (((Interrupt) == UART3_IT_PE) || \
   ((Interrupt) == UART3_IT_TXE) || \
   ((Interrupt) == UART3_IT_TC) || \
   ((Interrupt) == UART3_IT_RXNE_OR ) || \
   ((Interrupt) == UART3_IT_IDLE) || \
   ((Interrupt) == UART3_IT_LHDF) || \
   ((Interrupt) == UART3_IT_LBDF))

/**
  * @brief  Macro used by the assert function in order to check the different 
  *         sensitivity values for the pending bit
  */
#define IS_UART3_GET_IT_OK(ITPendingBit) \
  (((ITPendingBit) == UART3_IT_TXE)  || \
   ((ITPendingBit) == UART3_IT_TC)   || \
   ((ITPendingBit) == UART3_IT_RXNE) || \
   ((ITPendingBit) == UART3_IT_IDLE) || \
   ((ITPendingBit) == UART3_IT_OR)  || \
   ((ITPendingBit) == UART3_IT_LBDF)  || \
   ((ITPendingBit) == UART3_IT_LHDF)  || \
   ((ITPendingBit) == UART3_IT_PE))

/**
  * @brief  Macro used by the assert function in order to check the different 
  *         sensitivity values for the pending bit that can be cleared by writing 0
  */
#define IS_UART3_CLEAR_IT_OK(ITPendingBit) \
  (((ITPendingBit) == UART3_IT_RXNE) || \
   ((ITPendingBit) == UART3_IT_LHDF) || \
   ((ITPendingBit) == UART3_IT_LBDF))

/**
 * @brief Macro used by the assert_param function in order to check the different
 *        sensitivity values for the MODEs
 */
#define IS_UART3_MODE_OK(Mode) \
  (((Mode) == (uint8_t)UART3_MODE_RX_ENABLE) || \
   ((Mode) == (uint8_t)UART3_MODE_RX_DISABLE) || \
   ((Mode) == (uint8_t)UART3_MODE_TX_ENABLE) || \
   ((Mode) == (uint8_t)UART3_MODE_TX_DISABLE) || \
   ((Mode) == (uint8_t)UART3_MODE_TXRX_ENABLE) || \
   ((Mode) == (uint8_t)((uint8_t)UART3_MODE_TX_ENABLE|(uint8_t)UART3_MODE_RX_ENABLE)) || \
   ((Mode) == (uint8_t)((uint8_t)UART3_MODE_TX_ENABLE|(uint8_t)UART3_MODE_RX_DISABLE)) || \
   ((Mode) == (uint8_t)((uint8_t)UART3_MODE_TX_DISABLE|(uint8_t)UART3_MODE_RX_DISABLE)) || \
   ((Mode) == (uint8_t)((uint8_t)UART3_MODE_TX_DISABLE|(uint8_t)UART3_MODE_RX_ENABLE)))

/**
 * @brief Macro used by the assert_param function in order to check the different
 *        sensitivity values for the WordLengths
 */
#define IS_UART3_WORDLENGTH_OK(WordLength) \
  (((WordLength) == UART3_WORDLENGTH_8D) || \
   ((WordLength) == UART3_WORDLENGTH_9D))


/**
  * @brief  Macro used by the assert_param function in order to check the different
  *         sensitivity values for the WakeUps
  */
#define IS_UART3_WAKEUP_OK(WakeUpMode) \
  (((WakeUpMode) == UART3_WAKEUP_IDLELINE) || \
   ((WakeUpMode) == UART3_WAKEUP_ADDRESSMARK))

/**
  * @brief  Macro used by the assert_param function in order to check the different
  *         sensitivity values for the LINBreakDetectionLengths
  */
#define IS_UART3_LINBREAKDETECTIONLENGTH_OK(LINBreakDetectionLengths) \
  (((LINBreakDetectionLengths) == UART3_LINBREAKDETECTIONLENGTH_10BITS) || \
   ((LINBreakDetectionLengths) == UART3_LINBREAKDETECTIONLENGTH_11BITS))

/**
  * @brief  Macro used by the assert_param function in order to check the different
  *         sensitivity values for the UART3_StopBits
  */
#define IS_UART3_STOPBITS_OK(StopBit) \
  (((StopBit) == UART3_STOPBITS_1) || \
   ((StopBit) == UART3_STOPBITS_2))

/**
 * @brief Macro used by the assert_param function in order to check the different
 *        sensitivity values for the Parity
 */
#define IS_UART3_PARITY_OK(Parity) \
  (((Parity) == UART3_PARITY_NO) || \
   ((Parity) == UART3_PARITY_EVEN) || \
   ((Parity) == UART3_PARITY_ODD ))

/**
 * @brief Macro used by the assert_param function in order to check the maximum 
 *        baudrate value
 */
#define IS_UART3_BAUDRATE_OK(NUM) ((NUM) <= (uint32_t)625000)

/**
 * @brief Macro used by the assert_param function in order to check the address 
 *        of the UART3 or UART node
 */
#define UART3_ADDRESS_MAX ((uint8_t)16)
#define IS_UART3_ADDRESS_OK(Node) ((Node) < UART3_ADDRESS_MAX)

/**
 * @brief Macro used by the assert_param function in order to check the LIN mode
 */
#define IS_UART3_SLAVE_OK(Mode) \
  (((Mode) == UART3_LIN_MODE_MASTER) || \
   ((Mode) == UART3_LIN_MODE_SLAVE))

/**
 * @brief Macro used by the assert_param function in order to check the LIN 
 *        automatic resynchronization mode
 */
#define IS_UART3_AUTOSYNC_OK(AutosyncMode) \
  (((AutosyncMode) ==  UART3_LIN_AUTOSYNC_ENABLE) || \
   ((AutosyncMode) == UART3_LIN_AUTOSYNC_DISABLE))

/**
 * @brief Macro used by the assert_param function in order to check the LIN 
 *        divider update method
 */
#define IS_UART3_DIVUP_OK(DivupMethod) \
  (((DivupMethod) == UART3_LIN_DIVUP_LBRR1) || \
   ((DivupMethod) == UART3_LIN_DIVUP_NEXTRXNE))

/**
  * @}
  */

/* Exported functions ------------------------------------------------------- */

/** @addtogroup UART3_Exported_Functions
  * @{
  */

void UART3_DeInit(void);
void UART3_Init(uint32_t BaudRate, UART3_WordLength_TypeDef WordLength, 
                UART3_StopBits_TypeDef StopBits, UART3_Parity_TypeDef Parity, 
                UART3_Mode_TypeDef Mode);
void UART3_Cmd(FunctionalState NewState);
void UART3_ITConfig(UART3_IT_TypeDef UART3_IT, FunctionalState NewState);
void UART3_LINBreakDetectionConfig(UART3_LINBreakDetectionLength_TypeDef UART3_LINBreakDetectionLength);
void UART3_LINConfig(UART3_LinMode_TypeDef UART3_Mode, 
                     UART3_LinAutosync_TypeDef UART3_Autosync, 
                     UART3_LinDivUp_TypeDef UART3_DivUp);
void UART3_LINCmd(FunctionalState NewState);
void UART3_ReceiverWakeUpCmd(FunctionalState NewState);
void UART3_WakeUpConfig( UART3_WakeUp_TypeDef UART3_WakeUp);
uint8_t UART3_ReceiveData8(void);
uint16_t UART3_ReceiveData9(void);
void UART3_SendData8(uint8_t Data);
void UART3_SendData9(uint16_t Data);
void UART3_SendBreak(void);
void UART3_SetAddress(uint8_t UART3_Address);
FlagStatus UART3_GetFlagStatus(UART3_Flag_TypeDef UART3_FLAG);
void UART3_ClearFlag(UART3_Flag_TypeDef UART3_FLAG);
ITStatus UART3_GetITStatus(UART3_IT_TypeDef UART3_IT);
void UART3_ClearITPendingBit(UART3_IT_TypeDef UART3_IT);
/**
  * @}
  */

#endif /* __STM8S_UART3_H */

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
