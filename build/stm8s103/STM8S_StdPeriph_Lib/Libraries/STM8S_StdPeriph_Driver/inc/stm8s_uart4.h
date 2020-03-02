/**
  ********************************************************************************
  * @file    stm8s_uart4.h
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   This file contains all functions prototypes and macros for the UART4 peripheral.
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
#ifndef __STM8S_UART4_H
#define __STM8S_UART4_H

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"

/** @addtogroup STM8S_StdPeriph_Driver
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/** @addtogroup UART4_Exported_Types
  * @{
  */

/**
  * @brief  UART4 Irda Modes
  */

typedef enum { UART4_IRDAMODE_NORMAL     = (uint8_t)0x00, /**< 0x00 Irda Normal Mode   */
               UART4_IRDAMODE_LOWPOWER   = (uint8_t)0x01  /**< 0x01 Irda Low Power Mode */

             } UART4_IrDAMode_TypeDef;

/**
  * @brief  UART4 WakeUP Modes
  */

typedef enum { UART4_WAKEUP_IDLELINE       = (uint8_t)0x00, /**< 0x01 Idle Line wake up */
               UART4_WAKEUP_ADDRESSMARK    = (uint8_t)0x08  /**< 0x02 Address Mark wake up */
             } UART4_WakeUp_TypeDef;


/**
  * @brief  UART4 LIN Break detection length possible values
  */
typedef enum { UART4_LINBREAKDETECTIONLENGTH_10BITS = (uint8_t)0x00, /**< 0x01 10 bits Lin Break detection */
               UART4_LINBREAKDETECTIONLENGTH_11BITS = (uint8_t)0x01  /**< 0x02 11 bits Lin Break detection */
             } UART4_LINBreakDetectionLength_TypeDef;

/**
  * @brief  UART4 stop bits possible values
  */

typedef enum { UART4_STOPBITS_1   = (uint8_t)0x00,    /**< One stop bit is  transmitted at the end of frame*/
               UART4_STOPBITS_0_5 = (uint8_t)0x10,    /**< Half stop bits is transmitted at the end of frame*/
               UART4_STOPBITS_2   = (uint8_t)0x20,    /**< Two stop bits are  transmitted at the end of frame*/
               UART4_STOPBITS_1_5 = (uint8_t)0x30     /**< One and half stop bits*/
             } UART4_StopBits_TypeDef;


/**
  * @brief  UART4 parity possible values
  */
typedef enum { UART4_PARITY_NO     = (uint8_t)0x00,      /**< No Parity*/
               UART4_PARITY_EVEN   = (uint8_t)0x04,      /**< Even Parity*/
               UART4_PARITY_ODD    = (uint8_t)0x06       /**< Odd Parity*/
             } UART4_Parity_TypeDef;
/**
  * @brief  UART4 Mode possible values
  */
typedef enum { UART4_LIN_MODE_MASTER  = (uint8_t)0x00, /**<  LIN Master Mode*/
               UART4_LIN_MODE_SLAVE   = (uint8_t)0x01  /**<  LIN Slave Mode*/
             } UART4_LinMode_TypeDef;
/**
  * @brief  UART4 automatic resynchronisation possible values
  */
typedef enum { UART4_LIN_AUTOSYNC_DISABLE   = (uint8_t)0x00,  /**< LIN Autosynchronization Disable*/
               UART4_LIN_AUTOSYNC_ENABLE    = (uint8_t)0x01   /**< LIN Autosynchronization Enable*/
             } UART4_LinAutosync_TypeDef;
/**
  * @brief  UART4 Divider Update Method possible values
  */
typedef enum { UART4_LIN_DIVUP_LBRR1    = (uint8_t)0x00, /**<  LIN LDIV is updated as soon as LBRR1 is written*/
               UART4_LIN_DIVUP_NEXTRXNE = (uint8_t)0x01  /**<  LIN LDIV is updated at the next received character*/
             } UART4_LinDivUp_TypeDef;

/**
  * @brief  UART4 Synchrone modes
  */
typedef enum { UART4_SYNCMODE_CLOCK_DISABLE    = (uint8_t)0x80, /**< 0x80 Sync mode Disable, SLK pin Disable */
               UART4_SYNCMODE_CLOCK_ENABLE     = (uint8_t)0x08, /**< 0x08 Sync mode Enable, SLK pin Enable     */
               UART4_SYNCMODE_CPOL_LOW         = (uint8_t)0x40, /**< 0x40 Steady low value on SCLK pin outside transmission window */
               UART4_SYNCMODE_CPOL_HIGH        = (uint8_t)0x04, /**< 0x04 Steady high value on SCLK pin outside transmission window */
               UART4_SYNCMODE_CPHA_MIDDLE      = (uint8_t)0x20, /**< 0x20 SCLK clock line activated in middle of data bit     */
               UART4_SYNCMODE_CPHA_BEGINING    = (uint8_t)0x02, /**< 0x02 SCLK clock line activated at beginning of data bit  */
               UART4_SYNCMODE_LASTBIT_DISABLE  = (uint8_t)0x10, /**< 0x10 The clock pulse of the last data bit is not output to the SCLK pin */
               UART4_SYNCMODE_LASTBIT_ENABLE   = (uint8_t)0x01  /**< 0x01 The clock pulse of the last data bit is output to the SCLK pin */
             } UART4_SyncMode_TypeDef;

/**
  * @brief  UART4 Word length possible values
  */
typedef enum { UART4_WORDLENGTH_8D = (uint8_t)0x00,/**< 0x00 8 bits Data  */
               UART4_WORDLENGTH_9D = (uint8_t)0x10 /**< 0x10 9 bits Data  */
             } UART4_WordLength_TypeDef;

/**
  * @brief  UART4 Mode possible values
  */
typedef enum { UART4_MODE_RX_ENABLE     = (uint8_t)0x08,  /**< 0x08 Receive Enable */
               UART4_MODE_TX_ENABLE     = (uint8_t)0x04,  /**< 0x04 Transmit Enable */
               UART4_MODE_TX_DISABLE    = (uint8_t)0x80,  /**< 0x80 Transmit Disable */
               UART4_MODE_RX_DISABLE    = (uint8_t)0x40,  /**< 0x40 Single-wire Half-duplex mode */
               UART4_MODE_TXRX_ENABLE   = (uint8_t)0x0C  /**< 0x0C Transmit Enable and Receive Enable */
             } UART4_Mode_TypeDef;
/**
  * @brief  UART4 Flag possible values
  */
typedef enum
{
  UART4_FLAG_TXE          = (uint16_t)0x0080, /*!< Transmit Data Register Empty flag */
  UART4_FLAG_TC           = (uint16_t)0x0040, /*!< Transmission Complete flag */
  UART4_FLAG_RXNE         = (uint16_t)0x0020, /*!< Read Data Register Not Empty flag */
  UART4_FLAG_IDLE         = (uint16_t)0x0010, /*!< Idle line detected flag */
  UART4_FLAG_OR_LHE       = (uint16_t)0x0008, /*!< OverRun error flag */
  UART4_FLAG_NF           = (uint16_t)0x0004, /*!< Noise error flag */
  UART4_FLAG_FE           = (uint16_t)0x0002, /*!< Framing Error flag */
  UART4_FLAG_PE           = (uint16_t)0x0001, /*!< Parity Error flag */
  UART4_FLAG_SBK          = (uint16_t)0x0101,  /**< Send Break Complete interrupt flag */
  UART4_FLAG_LBDF         = (uint16_t)0x0210, /**< LIN Break Detection Flag           */
  UART4_FLAG_LHDF         = (uint16_t)0x0302, /**< LIN Header Detection Flag*/
  UART4_FLAG_LSF          = (uint16_t)0x0301  /**< LIN Sync Field Flag*/
} UART4_Flag_TypeDef;

/**
  * @brief  UART4 Interrupt definition
  * UART4_IT possible values
  * Elements values convention: 0xZYX
  * X: Position of the corresponding Interrupt
  *   - For the following values, X means the interrupt position in the CR2 register.
  *     UART4_IT_TXE
  *     UART4_IT_TC
  *     UART4_IT_RXNE
  *     UART4_IT_IDLE 
  *     UART4_IT_OR 
  *   - For the UART4_IT_PE value, X means the flag position in the CR1 register.
  *   - For the UART4_IT_LBDF value, X means the flag position in the CR4 register.
  *   - For the UART4_IT_LHDF value, X means the flag position in the CR6 register.
  * Y: Flag position
  *  - For the following values, Y means the flag (pending bit) position in the SR register.
  *     UART4_IT_TXE
  *     UART4_IT_TC
  *     UART4_IT_RXNE
  *     UART4_IT_IDLE 
  *     UART4_IT_OR
  *     UART4_IT_PE
  *  - For the UART4_IT_LBDF value, Y means the flag position in the CR4 register.
  *  - For the UART4_IT_LHDF value, Y means the flag position in the CR6 register.
  * Z: Register index: indicate in which register the dedicated interrupt source is:
  *  - 1==> CR1 register
  *  - 2==> CR2 register
  *  - 3==> CR4 register
  *  - 4==> CR6 register
  */
typedef enum { UART4_IT_TXE        = (uint16_t)0x0277, /**< Transmit interrupt */
               UART4_IT_TC         = (uint16_t)0x0266, /**< Transmission Complete interrupt */
               UART4_IT_RXNE       = (uint16_t)0x0255, /**< Data Register Not Empty interrupt */
               UART4_IT_IDLE       = (uint16_t)0x0244, /**< Idle line detected interrupt */
               UART4_IT_OR         = (uint16_t)0x0235, /**< OverRun error interrupt */
               UART4_IT_PE         = (uint16_t)0x0100, /**< Parity Error interrupt */
               UART4_IT_LBDF       = (uint16_t)0x0346, /**< LIN Break Detection interrupt */
               UART4_IT_LHDF       = (uint16_t)0x0412, /**< LIN Header Detection interrupt*/
               UART4_IT_RXNE_OR    = (uint16_t)0x0205  /*!< Receive/Overrun interrupt */
             } UART4_IT_TypeDef;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

/** @addtogroup UART4_Private_Macros
  * @{
  */

/**
  * @brief  Macro used by the assert function to check the different functions parameters.
  */

/**
 * @brief Macro used by the assert_param function in order to check the different
 *        sensitivity values for the MODEs possible combination should be one of
 *        the following.
 */
 
#define IS_UART4_MODE_OK(Mode) \
  (((Mode) == (uint8_t)UART4_MODE_RX_ENABLE) || \
   ((Mode) == (uint8_t)UART4_MODE_RX_DISABLE) || \
   ((Mode) == (uint8_t)UART4_MODE_TX_ENABLE) || \
   ((Mode) == (uint8_t)UART4_MODE_TX_DISABLE) || \
   ((Mode) == (uint8_t)UART4_MODE_TXRX_ENABLE) || \
   ((Mode) == (uint8_t)((uint8_t)UART4_MODE_TX_ENABLE|(uint8_t)UART4_MODE_RX_ENABLE)) || \
   ((Mode) == (uint8_t)((uint8_t)UART4_MODE_TX_ENABLE|(uint8_t)UART4_MODE_RX_DISABLE)) || \
   ((Mode) == (uint8_t)((uint8_t)UART4_MODE_TX_DISABLE|(uint8_t)UART4_MODE_RX_DISABLE)) || \
   ((Mode) == (uint8_t)((uint8_t)UART4_MODE_TX_DISABLE|(uint8_t)UART4_MODE_RX_ENABLE)))

/**
 * @brief Macro used by the assert_param function in order to check the different
 *        sensitivity values for the WordLengths
 */
#define IS_UART4_WORDLENGTH_OK(WordLength) \
  (((WordLength) == UART4_WORDLENGTH_8D) || \
   ((WordLength) == UART4_WORDLENGTH_9D))

/**
  * @brief  Macro used by the assert_param function in order to check the different
  *         sensitivity values for the SyncModes; it should exclude values such 
  *         as  UART4_CLOCK_ENABLE|UART4_CLOCK_DISABLE
  */
#define IS_UART4_SYNCMODE_OK(SyncMode) \
  (!((((SyncMode)&(((uint8_t)UART4_SYNCMODE_CLOCK_ENABLE)|((uint8_t)UART4_SYNCMODE_CLOCK_DISABLE))) == (((uint8_t)UART4_SYNCMODE_CLOCK_ENABLE)|((uint8_t)UART4_SYNCMODE_CLOCK_DISABLE))) || \
     (((SyncMode)&(((uint8_t)UART4_SYNCMODE_CPOL_LOW )|((uint8_t)UART4_SYNCMODE_CPOL_HIGH))) == (((uint8_t)UART4_SYNCMODE_CPOL_LOW )|((uint8_t)UART4_SYNCMODE_CPOL_HIGH))) || \
     (((SyncMode)&(((uint8_t)UART4_SYNCMODE_CPHA_MIDDLE)|((uint8_t)UART4_SYNCMODE_CPHA_BEGINING))) == (((uint8_t)UART4_SYNCMODE_CPHA_MIDDLE)|((uint8_t)UART4_SYNCMODE_CPHA_BEGINING))) || \
     (((SyncMode)&(((uint8_t)UART4_SYNCMODE_LASTBIT_DISABLE)|((uint8_t)UART4_SYNCMODE_LASTBIT_ENABLE))) == (((uint8_t)UART4_SYNCMODE_LASTBIT_DISABLE)|((uint8_t)UART4_SYNCMODE_LASTBIT_ENABLE)))))

/**
  * @brief  Macro used by the assert_param function in order to check the 
  *         different sensitivity values for the FLAGs
  */
#define IS_UART4_FLAG_OK(Flag) \
  (((Flag) == UART4_FLAG_TXE) || \
   ((Flag) == UART4_FLAG_TC)  || \
   ((Flag) == UART4_FLAG_RXNE) || \
   ((Flag) == UART4_FLAG_IDLE) || \
   ((Flag) == UART4_FLAG_OR_LHE) || \
   ((Flag) == UART4_FLAG_NF) || \
   ((Flag) == UART4_FLAG_FE) || \
   ((Flag) == UART4_FLAG_PE) || \
   ((Flag) == UART4_FLAG_SBK) || \
   ((Flag) == UART4_FLAG_LSF) || \
   ((Flag) == UART4_FLAG_LHDF) || \
   ((Flag) == UART4_FLAG_LBDF))


/**
  * @brief  Macro used by the assert_param function in order to check the 
  *         different sensitivity values for the FLAGs that can be cleared by writing 0
  */
#define IS_UART4_CLEAR_FLAG_OK(Flag) \
  (((Flag) == UART4_FLAG_RXNE) || \
   ((Flag) == UART4_FLAG_LHDF) || \
   ((Flag) == UART4_FLAG_LSF) || \
   ((Flag) == UART4_FLAG_LBDF))

/**
  * @brief  Macro used by the assert_param function in order to check 
  *         the different sensitivity values for the Interrupts
  */

#define IS_UART4_CONFIG_IT_OK(Interrupt) \
  (((Interrupt) == UART4_IT_PE) || \
   ((Interrupt) == UART4_IT_TXE) || \
   ((Interrupt) == UART4_IT_TC) || \
   ((Interrupt) == UART4_IT_RXNE_OR ) || \
   ((Interrupt) == UART4_IT_IDLE) || \
   ((Interrupt) == UART4_IT_LHDF) || \
   ((Interrupt) == UART4_IT_LBDF))

/**
  * @brief  Macro used by the assert function in order to check the different 
  *         sensitivity values for the pending bit
  */
#define IS_UART4_GET_IT_OK(ITPendingBit) \
  (((ITPendingBit) == UART4_IT_TXE)  || \
   ((ITPendingBit) == UART4_IT_TC)   || \
   ((ITPendingBit) == UART4_IT_RXNE) || \
   ((ITPendingBit) == UART4_IT_IDLE) || \
   ((ITPendingBit) == UART4_IT_OR)  || \
   ((ITPendingBit) == UART4_IT_LBDF)  || \
   ((ITPendingBit) == UART4_IT_LHDF)  || \
   ((ITPendingBit) == UART4_IT_PE))

/**
  * @brief  Macro used by the assert function in order to check the different 
  *         sensitivity values for the pending bit that can be cleared by writing 0
  */
#define IS_UART4_CLEAR_IT_OK(ITPendingBit) \
  (((ITPendingBit) == UART4_IT_RXNE) || \
   ((ITPendingBit) == UART4_IT_LHDF) || \
   ((ITPendingBit) == UART4_IT_LBDF))


/**
 * @brief Macro used by the assert_param function in order to check the different
 *        sensitivity values for the IrDAModes
 */
#define IS_UART4_IRDAMODE_OK(IrDAMode) \
  (((IrDAMode) == UART4_IRDAMODE_LOWPOWER) || \
   ((IrDAMode) == UART4_IRDAMODE_NORMAL))

/**
  * @brief  Macro used by the assert_param function in order to check the different
  *         sensitivity values for the WakeUps
  */
#define IS_UART4_WAKEUP_OK(WakeUp) \
  (((WakeUp) == UART4_WAKEUP_IDLELINE) || \
   ((WakeUp) == UART4_WAKEUP_ADDRESSMARK))

/**
  * @brief  Macro used by the assert_param function in order to check the different
  *         sensitivity values for the LINBreakDetectionLengths
  */
#define IS_UART4_LINBREAKDETECTIONLENGTH_OK(LINBreakDetectionLength) \
  (((LINBreakDetectionLength) == UART4_LINBREAKDETECTIONLENGTH_10BITS) || \
   ((LINBreakDetectionLength) == UART4_LINBREAKDETECTIONLENGTH_11BITS))

/**
  * @brief  Macro used by the assert_param function in order to check the different
  *         sensitivity values for the UART4_StopBits
  */
#define IS_UART4_STOPBITS_OK(StopBit) (((StopBit) == UART4_STOPBITS_1) || \
                                       ((StopBit) == UART4_STOPBITS_0_5) || \
                                       ((StopBit) == UART4_STOPBITS_2) || \
                                       ((StopBit) == UART4_STOPBITS_1_5 ))

/**
 * @brief Macro used by the assert_param function in order to check the different
 *        sensitivity values for the Paritys
 */
#define IS_UART4_PARITY_OK(Parity) (((Parity) == UART4_PARITY_NO) || \
                                    ((Parity) == UART4_PARITY_EVEN) || \
                                    ((Parity) == UART4_PARITY_ODD ))

/**
 * @brief Macro used by the assert_param function in order to check the maximum
 *        baudrate value
 */
#define IS_UART4_BAUDRATE_OK(NUM) ((NUM) <= (uint32_t)625000)

/**
 * @brief Macro used by the assert_param function in order to check the address 
 *        of the UART4 or UART node
 */
#define UART4_ADDRESS_MAX ((uint8_t)16)
#define IS_UART4_ADDRESS_OK(node) ((node) < UART4_ADDRESS_MAX )

/**
 * @brief Macro used by the assert_param function in order to check the LIN mode
 */
#define IS_UART4_SLAVE_OK(Mode) \
  (((Mode) == UART4_LIN_MODE_MASTER) || \
   ((Mode) == UART4_LIN_MODE_SLAVE))

/**
 * @brief Macro used by the assert_param function in order to check the LIN 
 *        automatic resynchronization mode
 */
#define IS_UART4_AUTOSYNC_OK(AutosyncMode) \
  (((AutosyncMode) ==  UART4_LIN_AUTOSYNC_ENABLE) || \
   ((AutosyncMode) == UART4_LIN_AUTOSYNC_DISABLE))

/**
 * @brief Macro used by the assert_param function in order to check the LIN divider update method
 */
#define IS_UART4_DIVUP_OK(DivupMethod) \
  (((DivupMethod) == UART4_LIN_DIVUP_LBRR1) || \
   ((DivupMethod) == UART4_LIN_DIVUP_NEXTRXNE))


/**
  * @}
  */

/* Exported functions ------------------------------------------------------- */

/** @addtogroup UART4_Exported_Functions
  * @{
  */
void UART4_DeInit(void);
void UART4_Init(uint32_t BaudRate, UART4_WordLength_TypeDef WordLength, 
                UART4_StopBits_TypeDef StopBits, UART4_Parity_TypeDef Parity, 
                UART4_SyncMode_TypeDef SyncMode, UART4_Mode_TypeDef Mode);
void UART4_Cmd(FunctionalState NewState);
void UART4_ITConfig(UART4_IT_TypeDef UART4_IT, FunctionalState NewState);
void UART4_HalfDuplexCmd(FunctionalState NewState);
void UART4_IrDAConfig(UART4_IrDAMode_TypeDef UART4_IrDAMode);
void UART4_IrDACmd(FunctionalState NewState);
void UART4_LINBreakDetectionConfig(UART4_LINBreakDetectionLength_TypeDef UART4_LINBreakDetectionLength);
void UART4_LINConfig(UART4_LinMode_TypeDef UART4_Mode, 
                     UART4_LinAutosync_TypeDef UART4_Autosync, 
                     UART4_LinDivUp_TypeDef UART4_DivUp);
void UART4_LINCmd(FunctionalState NewState);
void UART4_SmartCardCmd(FunctionalState NewState);
void UART4_SmartCardNACKCmd(FunctionalState NewState);
void UART4_WakeUpConfig(UART4_WakeUp_TypeDef UART4_WakeUp);
void UART4_ReceiverWakeUpCmd(FunctionalState NewState);
uint8_t UART4_ReceiveData8(void);
uint16_t UART4_ReceiveData9(void);
void UART4_SendData8(uint8_t Data);
void UART4_SendData9(uint16_t Data);
void UART4_SendBreak(void);
void UART4_SetAddress(uint8_t UART4_Address);
void UART4_SetGuardTime(uint8_t UART4_GuardTime);
void UART4_SetPrescaler(uint8_t UART4_Prescaler);
FlagStatus UART4_GetFlagStatus(UART4_Flag_TypeDef UART4_FLAG);
void UART4_ClearFlag(UART4_Flag_TypeDef UART4_FLAG);
ITStatus UART4_GetITStatus(UART4_IT_TypeDef UART4_IT);
void UART4_ClearITPendingBit(UART4_IT_TypeDef UART4_IT);


/**
  * @}
  */

#endif /* __STM8S_UART4_H */

/**
  * @}
  */
  

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
