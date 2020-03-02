/**
  ******************************************************************************
  * @file    stm8s_can.h
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   This file contains all the functions for the CAN peripheral.
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
#ifndef __STM8S_CAN_H
#define __STM8S_CAN_H

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"

/* Exported constants --------------------------------------------------------*/
#define CAN_STDID_SIZE ((uint16_t)0x07FF)
#define CAN_EXTID_SIZE ((uint32_t)0x1FFFFFFF)
#define CAN_DLC_MAX ((uint8_t)0x08)


/** @addtogroup CAN_Exported_Types
  * @{
  */


/**
  * @brief  CAN Page Mapping
 */
typedef enum
{
 CAN_Page_TxMailBox0  = ((uint8_t) 0), /*!< CAN TX mailbox 0 reg page */
 CAN_Page_TxMailBox1  = ((uint8_t) 1), /*!< CAN TX mailbox 1 reg page */
 CAN_Page_TxMailBox2  = ((uint8_t) 5), /*!< CAN TX mailbox 2 reg page */
 CAN_Page_Filter01    = ((uint8_t) 2), /*!< CAN Filters 0 & 1 reg page*/
 CAN_Page_Filter23    = ((uint8_t) 3), /*!< CAN Filters 2 & 3 reg page*/
 CAN_Page_Filter45    = ((uint8_t) 4), /*!< CAN Filters 4 & 5 reg page*/
 CAN_Page_Config      = ((uint8_t) 6), /*!< CAN Configuration control/status reg page*/
 CAN_Page_RxFifo      = ((uint8_t) 7) /*!< CAN RX FIFO registers page */
}CAN_Page_TypeDef;



/**
  * @brief   CAN sleep constants 
 */
typedef enum {
  CAN_InitStatus_Failed =0,                        /*!< CAN initialization failed */
  CAN_InitStatus_Success =! CAN_InitStatus_Failed  /*!< CAN initialization OK*/
} CAN_InitStatus_TypeDef;

 
 /**
  * @brief  CAN operating mode */
 typedef enum
{
  CAN_OperatingMode_Initialization    =((uint8_t)0x00),    /*!< Initialization mode */
  CAN_OperatingMode_Normal            =((uint8_t)0x01),    /*!< Normal mode */
  CAN_OperatingMode_Sleep             =((uint8_t)0x02)    /*!< sleep mode */
}CAN_OperatingMode_TypeDef;

 /**
  * @brief  CAN operating mode status */
 typedef enum
{
  CAN_ModeStatus_Failed    = ((uint8_t)0x00),          /*!< CAN entering the specific mode failed */
  CAN_ModeStatus_Success    =! CAN_ModeStatus_Failed   /*!< CAN entering the specific mode Succeed */
}CAN_ModeStatus_TypeDef;

 /**
  * @brief   CAN Time Triggered Communication mode 
  */
typedef enum
{
 CAN_MasterCtrl_AllDisabled          =((uint8_t)0x00),  /*!<  CAN  ALL Master Control Option are DISABLED */
 CAN_MasterCtrl_AllEnabled           =((uint8_t)0xFC),  /*!<  CAN  ALL Master Control Option are DISABLED */
 CAN_MasterCtrl_TimeTriggerCOMMode   =((uint8_t)0x80),  /*!<  CAN Time Triggered Communication mode ENABLED */
 CAN_MasterCtrl_AutoBusOffManagement =((uint8_t)0x40),  /*!<  CAN Auto Bus Off Management ENABLED */
 CAN_MasterCtrl_AutoWakeUpMode       =((uint8_t)0x20),  /*!<  CAN Automatic WakeUp Mode ENABLED , sleep mode is left automatically by hardware  */
 CAN_MasterCtrl_NoAutoReTx           =((uint8_t)0x10),  /*!<  CAN Non Automatic Retransmission ENABLED, MSG  will be transmitted only once */
 CAN_MasterCtrl_RxFifoLockedMode     =((uint8_t)0x08),  /*!<  CAN Receive FIFO Locked  against overrun ENABLED */
 CAN_MasterCtrl_TxFifoPriority       =((uint8_t)0x04)   /*!<  CAN Transmit FIFO Priority  driven by the request order (not by the identifier of the MSG) */
 }CAN_MasterCtrl_TypeDef;

/**
  * @brief   CAN  mode options */
typedef enum
{
  CAN_Mode_Normal             =((uint8_t)0x00),  /*!< normal mode */
  CAN_Mode_LoopBack           =((uint8_t)0x01),  /*!< loopback mode */
  CAN_Mode_Silent             =((uint8_t)0x02),  /*!< silent mode */
  CAN_Mode_Silent_LoopBack    =((uint8_t)0x03)  /*!< loopback combined with silent mode */
}CAN_Mode_TypeDef;

/**
  * @brief  CAN synchronisation jump width (SJW)*/
typedef enum
{
  CAN_SynJumpWidth_1TimeQuantum                 =((uint8_t)0x00),  /*!< 1 time quantum */
  CAN_SynJumpWidth_2TimeQuantum                 =((uint8_t)0x40),  /*!< 2 time quantum */
  CAN_SynJumpWidth_3TimeQuantum                 =((uint8_t)0x80),  /*!< 3 time quantum */
  CAN_SynJumpWidth_4TimeQuantum                 =((uint8_t)0xC0)  /*!< 4 time quantum */
}CAN_SynJumpWidth_TypeDef;

/**
  * @brief   time quantum in bit segment 1 */
typedef enum
{
  CAN_BitSeg1_1TimeQuantum                 =((uint8_t)0x00),  /*!< 1 time quantum */
  CAN_BitSeg1_2TimeQuantum                 =((uint8_t)0x01),  /*!< 2 time quantum */
  CAN_BitSeg1_3TimeQuantum                 =((uint8_t)0x02),  /*!< 3 time quantum */
  CAN_BitSeg1_4TimeQuantum                 =((uint8_t)0x03) , /*!< 4 time quantum */
  CAN_BitSeg1_5TimeQuantum                 =((uint8_t)0x04) , /*!< 5 time quantum */
  CAN_BitSeg1_6TimeQuantum                 =((uint8_t)0x05) , /*!< 6 time quantum */
  CAN_BitSeg1_7TimeQuantum                 =((uint8_t)0x06) , /*!< 7 time quantum */
  CAN_BitSeg1_8TimeQuantum                 =((uint8_t)0x07),  /*!< 8 time quantum */
  CAN_BitSeg1_9TimeQuantum                 =((uint8_t)0x08),  /*!< 9 time quantum */
  CAN_BitSeg1_10TimeQuantum                =((uint8_t)0x09),  /*!< 10 time quantum */
  CAN_BitSeg1_11TimeQuantum                =((uint8_t)0x0A),  /*!< 11 time quantum */
  CAN_BitSeg1_12TimeQuantum                =((uint8_t)0x0B),  /*!< 12 time quantum */
  CAN_BitSeg1_13TimeQuantum                =((uint8_t)0x0C),  /*!< 13 time quantum */
  CAN_BitSeg1_14TimeQuantum                =((uint8_t)0x0D),  /*!< 14 time quantum */
  CAN_BitSeg1_15TimeQuantum                =((uint8_t)0x0E),  /*!< 15 time quantum */
  CAN_BitSeg1_16TimeQuantum                =((uint8_t)0x0F)  /*!< 16 time quantum */
}CAN_BitSeg1_TypeDef;

/**
  * @brief   time quantum in bit segment 2 */
typedef enum
{
  CAN_BitSeg2_1TimeQuantum                = ((uint8_t)0x00),  /*!< 1 time quantum */
  CAN_BitSeg2_2TimeQuantum                = ((uint8_t)0x10),  /*!< 2 time quantum */
  CAN_BitSeg2_3TimeQuantum                = ((uint8_t)0x20),  /*!< 3 time quantum */
  CAN_BitSeg2_4TimeQuantum                = ((uint8_t)0x30),  /*!< 4 time quantum */
  CAN_BitSeg2_5TimeQuantum                = ((uint8_t)0x40),  /*!< 5 time quantum */
  CAN_BitSeg2_6TimeQuantum                = ((uint8_t)0x50),  /*!< 6 time quantum */
  CAN_BitSeg2_7TimeQuantum                = ((uint8_t)0x60),  /*!< 7 time quantum */
  CAN_BitSeg2_8TimeQuantum                = ((uint8_t)0x70)   /*!< 8 time quantum */
}CAN_BitSeg2_TypeDef;


/**
  * @brief   CAN filter number */
typedef enum
{                                                                  
  CAN_FilterNumber_0                = ((uint8_t)0x00),  /*!< Filter number 0 */ 
  CAN_FilterNumber_1                = ((uint8_t)0x01),  /*!< Filter number 1 */ 
  CAN_FilterNumber_2                = ((uint8_t)0x02),  /*!< Filter number 2 */
  CAN_FilterNumber_3                = ((uint8_t)0x03),  /*!< Filter number 3 */
  CAN_FilterNumber_4                = ((uint8_t)0x04),  /*!< Filter number 4 */ 
  CAN_FilterNumber_5                = ((uint8_t)0x05)   /*!< Filter number 5 */ 
}CAN_FilterNumber_TypeDef;

/**
  * @brief   CAN filter mode */
typedef enum
{
  CAN_FilterMode_IdMask                  =  ((uint8_t)0x00),    /*!< id/mask mode */
  CAN_FilterMode_IdMask_IdList           =  ((uint8_t)0x10),    /*!< Id/Mask mode First and IdList mode second */
  CAN_FilterMode_IdList_IdMask           =  ((uint8_t)0x11),    /*!< IdList mode First and IdMask mode second */
  CAN_FilterMode_IdList                  =  ((uint8_t)0x01)     /*!< identifier list mode */
}CAN_FilterMode_TypeDef;

/**
  * @brief   CAN filter scale */
typedef enum
{
  CAN_FilterScale_8Bit       =((uint8_t)0x00), /*!< 8-bit filter scale */
  CAN_FilterScale_16_8Bit    =((uint8_t)0x02), /*!< 16/8-bit filter scale */
  CAN_FilterScale_16Bit      =((uint8_t)0x04), /*!< 16-bit filter scale */
  CAN_FilterScale_32Bit      =((uint8_t)0x06)  /*!< 32-bit filter scale */
}CAN_FilterScale_TypeDef;


/**
  * @brief   CAN Tx mailboxes*/
typedef enum
{
 CAN_TransmitMailBox_0       = ((uint8_t) 0x00),  /*!< CAN TX mailbox 0 reg page */
 CAN_TransmitMailBox_1       = ((uint8_t) 0x01),  /*!< CAN TX mailbox 1 reg page */
 CAN_TransmitMailBox_2       = ((uint8_t) 0x05)   /*!< CAN TX mailbox 2 reg page */
}CAN_TransmitMailBox_TypeDef;

/**
  * @brief   CAN Pending Messages number*/
typedef enum
{
  CAN_NbrPendingMessage_0     =  ((uint8_t)0x00),  /*!< No Msg Pending */
  CAN_NbrPendingMessage_1     =  ((uint8_t)0x01),  /*!< 1 Msg Pending */
  CAN_NbrPendingMessage_2     =  ((uint8_t)0x02),  /*!< 2 Msg Pending */
  CAN_NbrPendingMessage_3     =  ((uint8_t)0x03)  /*!< 3 Msg Pending */
}CAN_NbrPendingMessage_TypeDef;

/**
  * @brief   CAN identifier type */
typedef enum
{
  CAN_Id_Standard                 =((uint8_t)0x00),  /*!< Standard Id */
  CAN_Id_Extended                 =((uint8_t)0x40)  /*!< Extended Id */
}CAN_Id_TypeDef;

/**
  * @brief   CAN remote transmission request */
typedef enum
{
  CAN_RTR_Data               = ((uint8_t)0x00),  /*!< Data frame */
  CAN_RTR_Remote             = ((uint8_t)0x20)  /*!< Remote frame */
}CAN_RTR_TypeDef;

/**
  * @brief   CAN transmit Status */
typedef enum
{
  CAN_TxStatus_Failed                 =((uint8_t)0xF0), /*!< CAN transmission failed */
  CAN_TxStatus_Ok                     =((uint8_t)0xF1), /*!< CAN transmission succeeded */
  CAN_TxStatus_Pending                =((uint8_t)0xF2), /*!< CAN transmission pending */
  CAN_TxStatus_NoMailBox              =((uint8_t)0xF4), /*!< CAN cell did not provide an empty mailbox */
  CAN_TxStatus_MailBoxEmpty           =((uint8_t)0xF5), /*!< CAN Tx mailbox is Empty */
  CAN_TxStatus_MailBox0Ok             =((uint8_t)0x00), /*!< CAN transmission succeeded by mail box 1*/
  CAN_TxStatus_MailBox1Ok             =((uint8_t)0x01), /*!< CAN transmission succeeded by mail box 2*/
  CAN_TxStatus_MailBox2Ok             =((uint8_t)0x05) /*!< CAN transmission succeeded by mail box 3*/
}CAN_TxStatus_TypeDef;

/**
  * @brief   CAN sleep Status */
typedef enum
{
  CAN_Sleep_Failed             = ((uint8_t)0x00), /*!< CAN did not enter the sleep mode */
  CAN_Sleep_Ok                 = ((uint8_t)0x01) /*!< CAN entered the sleep mode */
}CAN_Sleep_TypeDef;
/**
  * @brief   CAN wake up status */
typedef enum
{
 CAN_WakeUp_Failed          =   ((uint8_t)0x00), /*!< CAN did not leave the sleep mode */
 CAN_WakeUp_Ok              =   ((uint8_t)0x01) /*!< CAN leaved the sleep mode */
}CAN_WakeUp_TypeDef;

/**
  * @brief   CAN flags */
typedef enum
{
  /* if the flag is 0x3XXX, it means that it can be got (CAN_GetFlagStatus) and Cleared (CAN_ClearFlag) */
  /* if the flag is 0x1XXX, it means that it can only be got (CAN_GetFlagStatus)  */
  /*Transmit Flags*/
  CAN_FLAG_RQCP0            =((uint16_t)0x3401), /*!< Request MailBox0  Flag */
  CAN_FLAG_RQCP1            =((uint16_t)0x3402), /*!< Request MailBox1  Flag */
  CAN_FLAG_RQCP2            =((uint16_t)0x3404), /*!< Request MailBox2  Flag */
  /*Receive Flags*/
  CAN_FLAG_FMP              =((uint16_t)0x1203), /*!< FIFO Message Pending Flag */
  CAN_FLAG_FF               =((uint16_t)0x3208), /*!< FIFO Full  Flag */
  CAN_FLAG_FOV              =((uint16_t)0x3210), /*!< FIFO Overrun  Flag */
  /*Wake up Flag*/
  CAN_FLAG_WKU              =((uint16_t)0x3108), /*!< wake up   Flag */
  /*Error Flags*/
  CAN_FLAG_EWG              =((uint16_t)0x1001), /*!< Error Warning Flag */
  CAN_FLAG_EPV              =((uint16_t)0x1002), /*!< Error Passive Flag */
  CAN_FLAG_BOF              =((uint16_t)0x1004), /*!< Bus-Off Flag */
  CAN_FLAG_LEC              =((uint16_t)0x3070) /*!< Last error code Flag */
}CAN_FLAG_TypeDef;

/**
  * @brief   CAN interrupts */
typedef enum
{
  /*Transmit Interruption*/
  CAN_IT_TME                  =((uint16_t)0x0001), /*!< Transmit mailbox empty interrupt */
  /*Receive Interruptions*/
  CAN_IT_FMP                  =((uint16_t)0x0002), /*!< FIFO  message pending interrupt   */  
  CAN_IT_FF                   =((uint16_t)0x0004), /*!< FIFO  full    interrupt                    */
  CAN_IT_FOV                  =((uint16_t)0x0008), /*!< FIFO  overrun  interrupt               */
  /*Wake Up Interruption*/
  CAN_IT_WKU                  =((uint16_t)0x0080), /*!< Wake-up interrupt                         */
  /*Error Interruptions*/
  CAN_IT_ERR                  =((uint16_t)0x4000), /*!< Genaral Error interrupt                 */
  CAN_IT_EWG                  =((uint16_t)0x0100), /*!< Error warning interrupt                */
  CAN_IT_EPV                  =((uint16_t)0x0200), /*!< Error passive  interrupt                */
  CAN_IT_BOF                  =((uint16_t)0x0400), /*!< Bus-off   interrupt                        */
  CAN_IT_LEC                  =((uint16_t)0x0800)  /*!< Last error code interrupt              */
} CAN_IT_TypeDef;

/**
  * @brief   CAN ST7 Compatibility*/
typedef enum
{
  CAN_ST7Compatibility_Enable     =  ((uint8_t)0x00), /*!< CAN is compatible with ST7 beCAN (only 2 mailboxes are available)*/
  CAN_ST7Compatibility_Disable    =  ((uint8_t)0x10)  /*!< CAN is not compatible with ST7 beCAN ( 3 mailboxes are available)*/
}CAN_ST7Compatibility_TypeDef;

/**
  * @brief   CAN Error Code description */
typedef enum
{                                                                  
  CAN_ErrorCode_NoErr                 = ((uint8_t)0x00),  /*!< No Error  */ 
  CAN_ErrorCode_StuffErr              = ((uint8_t)0x10),  /*!< Stuff Error  */ 
  CAN_ErrorCode_FormErr               = ((uint8_t)0x20),  /*!< Form Error  */ 
  CAN_ErrorCode_ACKErr                = ((uint8_t)0x30),  /*!< Acknowledgment Error  */ 
  CAN_ErrorCode_BitRecessiveErr       = ((uint8_t)0x40),  /*!< Bit Recessive Error  */ 
  CAN_ErrorCode_BitDominantErr        = ((uint8_t)0x50),  /*!< Bit Dominant Error  */ 
  CAN_ErrorCode_CRCErr                = ((uint8_t)0x60),  /*!< CRC Error  */ 
  CAN_ErrorCode_SoftwareSetErr        = ((uint8_t)0x70)  /*!< Software Set Error  */ 
}CAN_ErrorCode_TypeDef;
/**
  * @}
  */
/* Private macros ------------------------------------------------------------*/
/** @addtogroup CAN_Private_Macros
  * @{
  */
/**
  * @brief  Macro used by the assert function in order to check the CAN ST7 Compatibility parameters.
  */
#define IS_CAN_ST7_COMPATIBILITY_OK(STATE) (((STATE) == CAN_ST7Compatibility_Enable) || ((STATE) == CAN_ST7Compatibility_Disable))
/**
  * @brief  Macro used by the assert function in order to check  CAN operating mode.
  */
#define IS_CAN_OPERATINGMODE_OK(MODE) (((MODE) == CAN_OperatingMode_Initialization) ||\
                                    ((MODE) == CAN_OperatingMode_Normal)|| \
                        ((MODE) == CAN_OperatingMode_Sleep))
/**
  * @brief  Macro used by the assert function in order to check CAN Time Triggered Communication mode.
  */
#define IS_CAN_MASTERCTRL_OK(MODE) (((MODE) == CAN_MasterCtrl_AllDisabled) || \
                                    (((MODE) <= CAN_MasterCtrl_AllEnabled) && ((MODE) >= CAN_MasterCtrl_TxFifoPriority)))
/**
  * @brief  Macro used by the assert function in order to check   CAN  mode options .
  */
#define IS_CAN_MODE_OK(MODE) (((MODE) == CAN_Mode_Normal) || ((MODE) == CAN_Mode_LoopBack)|| \
                           ((MODE) == CAN_Mode_Silent) || ((MODE) == CAN_Mode_Silent_LoopBack))
/**
  * @brief  Macro used by the assert function in order to check the CAN synchronisation jump width (SJW).
  */
#define IS_CAN_SYNJUMPWIDTH_OK(SJW) (((SJW) == CAN_SynJumpWidth_1TimeQuantum) || ((SJW) == CAN_SynJumpWidth_2TimeQuantum)|| \
                         ((SJW) == CAN_SynJumpWidth_3TimeQuantum) || ((SJW) == CAN_SynJumpWidth_4TimeQuantum))
/**
  * @brief  Macro used by the assert function in order to check time quantum in bit segment 1 .
  */
#define IS_CAN_BITSEG1_OK(BS1) ((BS1) <= CAN_BitSeg1_16TimeQuantum)
/**
  * @brief  Macro used by the assert function in order to check time quantum in bit segment 2.
  */
#define IS_CAN_BITSEG2_OK(BS2) ((((BS2) >= CAN_BitSeg2_2TimeQuantum) && ((BS2) <= CAN_BitSeg2_8TimeQuantum))|| ((BS2) == CAN_BitSeg2_1TimeQuantum))
/**
  * @brief  Macro used by the assert function in order to check CAN clock prescaler.
  */
#define IS_CAN_PRESCALER_OK(PRESCALER) (((PRESCALER) >= 1) && ((PRESCALER) <= 64))
/**
  * @brief  Macro used by the assert function in order to check CAN filter number.
  */
#define IS_CAN_FILTER_NUMBER_OK(NUMBER) (((NUMBER) == CAN_FilterNumber_0) || \
                                       ((NUMBER) == CAN_FilterNumber_1) || \
                                       ((NUMBER) == CAN_FilterNumber_2) || \
                                       ((NUMBER) == CAN_FilterNumber_3) || \
                                       ((NUMBER) == CAN_FilterNumber_4) || \
                                       ((NUMBER) == CAN_FilterNumber_5))
/**
  * @brief  Macro used by the assert function in order to check  CAN filter mode.
  */
#define IS_CAN_FILTER_MODE_OK(MODE) (((MODE) == CAN_FilterMode_IdMask) || \
                                     ((MODE) == CAN_FilterMode_IdMask_IdList) || \
                                     ((MODE) == CAN_FilterMode_IdList_IdMask) || \
                                  ((MODE) == CAN_FilterMode_IdList))
/**
  * @brief  Macro used by the assert function in order to check CAN filter scale.
  */
#define IS_CAN_FILTER_SCALE_OK(SCALE) (((SCALE) == CAN_FilterScale_8Bit)|| \
                                    ((SCALE) == CAN_FilterScale_16_8Bit) ||\
                                    ((SCALE) == CAN_FilterScale_16Bit  )||\
                                    ((SCALE) == CAN_FilterScale_32Bit))
/**
  * @brief  Macro used by the assert function in order to check CAN Tx mailboxes.
  */
#define IS_CAN_TRANSMITMAILBOX_OK(TRANSMITMAILBOX) (((TRANSMITMAILBOX) == CAN_TransmitMailBox_0) || \
                                                 ((TRANSMITMAILBOX) == CAN_TransmitMailBox_1) || \
                                                 ((TRANSMITMAILBOX) == CAN_TransmitMailBox_2))
/**
  * @brief  Macro used by the assert function in order to check the Standard ID to be sent.
  */
#define IS_CAN_STDID_OK(STDID)   ((STDID) <= ((uint16_t)CAN_STDID_SIZE))
/**
  * @brief  Macro used by the assert function in order to check the Extended ID to be sent.
  */
#define IS_CAN_EXTID_OK(EXTID)   ((EXTID) <= ((uint32_t)CAN_EXTID_SIZE))
/**
  * @brief  Macro used by the assert function in order to check the DLC to be sent.
  */
#define IS_CAN_DLC_OK(DLC)       ((DLC) <= CAN_DLC_MAX)
/**
  * @brief  Macro used by the assert function in order to check the type of the ID to be sent.
  */
#define IS_CAN_IDTYPE_OK(IDTYPE) (((IDTYPE) == CAN_Id_Standard) || ((IDTYPE) == CAN_Id_Extended))
/**
  * @brief  Macro used by the assert function in order to check CAN  transmission  Frame Type.
  */
#define IS_CAN_RTR_OK(RTR) (((RTR) == CAN_RTR_Data) || ((RTR) == CAN_RTR_Remote))

/**
  * @brief  Macro used by the assert function in order to check  CAN flags which can be got by @ref CAN_GetFlagStatus
  */
#define IS_CAN_FLAG_STATUS_OK(FLAG) (((FLAG) == CAN_FLAG_RQCP0) || ((FLAG) == CAN_FLAG_RQCP1) ||\
                                  ((FLAG) == CAN_FLAG_RQCP2) || ((FLAG) == CAN_FLAG_FMP) ||\
                                  ((FLAG) == CAN_FLAG_FF) || ((FLAG) == CAN_FLAG_FOV) ||\
                                  ((FLAG) == CAN_FLAG_WKU) || ((FLAG) == CAN_FLAG_EWG) ||\
                                  ((FLAG) == CAN_FLAG_EPV) || ((FLAG) == CAN_FLAG_BOF) ||\
                                  ((FLAG) == CAN_FLAG_LEC))
/**
  * @brief  Macro used by the assert function in order to check  CAN flags which can be cleared by @ref CAN_ClearFlag
  */
#define IS_CAN_FLAG_CLEAR_OK(FLAG) (((FLAG) == CAN_FLAG_RQCP0) || ((FLAG) == CAN_FLAG_RQCP1) ||\
                                    ((FLAG) == CAN_FLAG_RQCP2) ||  ((FLAG) == CAN_FLAG_FF)   ||\
                                    ((FLAG) == CAN_FLAG_FOV)  ||  ((FLAG) == CAN_FLAG_WKU) ||\
                                    ((FLAG) == CAN_FLAG_LEC))
/**
  * @brief  Macro used by the assert function in order to check the  CAN Configuration interrupts.
  */
#define CAN_IT_CONFIG_MASK  ~(uint16_t)(CAN_IT_TME|CAN_IT_FMP|CAN_IT_FF|CAN_IT_FOV|CAN_IT_WKU|CAN_IT_EWG|CAN_IT_EPV|CAN_IT_BOF|CAN_IT_LEC|CAN_IT_ERR)
#define IS_CAN_IT_CONFIG_OK(IT) (((IT) != 0x0000) && ((uint16_t)((uint16_t)(IT) & (uint16_t)CAN_IT_CONFIG_MASK) == 0x0000))
/**
  * @brief  Macro used by the assert function in order to check the  CAN status interrupts.
  */
#define IS_CAN_IT_STATUS_OK(IT) (((IT) == CAN_IT_TME)   || ((IT) == CAN_IT_FMP) ||\
                             ((IT) == CAN_IT_FF)    || ((IT) == CAN_IT_FOV) || \
                             ((IT) == CAN_IT_WKU)   || ((IT) == CAN_IT_ERR)  || \
                             ((IT) == CAN_IT_EWG)   || ((IT) == CAN_IT_EPV)  || \
                             ((IT) == CAN_IT_BOF)   || ((IT) == CAN_IT_LEC)  )
/**
  * @brief  Macro used by the assert function in order to check the  CAN Pending bit interrupts.
  */
#define IS_CAN_IT_PENDING_BIT_OK(IT) (((IT) == CAN_IT_TME) || ((IT) == CAN_IT_FF)  ||\
                                      ((IT) == CAN_IT_FOV)  || ((IT) == CAN_IT_WKU) ||\
                                      ((IT) == CAN_IT_ERR)  || ((IT) == CAN_IT_EWG) ||\
                                      ((IT) == CAN_IT_EPV) || ((IT) == CAN_IT_BOF)||\
                                      ((IT) == CAN_IT_LEC))
/**
  * @brief  Macro used by the assert function in order to check the Last Error Code.
  */
#define IS_CAN_LAST_ERROR_CODE_OK(CODE)  (((CODE) & 0x8F) == 0x00)
/**
  * @}
  */

/* Exported function prototypes --------------------------------------------- */
/** @addtogroup CAN_Exported_Functions
  * @{
  */
void CAN_DeInit(void);
CAN_InitStatus_TypeDef CAN_Init(CAN_MasterCtrl_TypeDef CAN_MasterCtrl,
                                CAN_Mode_TypeDef CAN_Mode,
                                CAN_SynJumpWidth_TypeDef CAN_SynJumpWidth,
                                CAN_BitSeg1_TypeDef CAN_BitSeg1,
                                CAN_BitSeg2_TypeDef CAN_BitSeg2,
                                uint8_t CAN_Prescaler);

void CAN_FilterInit(CAN_FilterNumber_TypeDef CAN_FilterNumber,
                    FunctionalState CAN_FilterActivation,
                    CAN_FilterMode_TypeDef CAN_FilterMode,
                    CAN_FilterScale_TypeDef CAN_FilterScale,
                    uint8_t CAN_FilterID1,  
                    uint8_t CAN_FilterID2,
                    uint8_t CAN_FilterID3,
                     uint8_t CAN_FilterID4,
                    uint8_t CAN_FilterIDMask1,
                     uint8_t CAN_FilterIDMask2,
                    uint8_t CAN_FilterIDMask3,
                    uint8_t CAN_FilterIDMask4);
void CAN_ITConfig(CAN_IT_TypeDef CAN_IT, FunctionalState NewState);
void CAN_ST7CompatibilityCmd(CAN_ST7Compatibility_TypeDef CAN_ST7Compatibility);
CAN_TxStatus_TypeDef CAN_Transmit( uint32_t CAN_Id,
                                   CAN_Id_TypeDef CAN_IDE,
                                   CAN_RTR_TypeDef CAN_RTR,
                                   uint8_t CAN_DLC,
                                   uint8_t *CAN_Data);
void CAN_TTComModeCmd(FunctionalState NewState);
CAN_TxStatus_TypeDef CAN_TransmitStatus(CAN_TransmitMailBox_TypeDef CAN_TransmitMailbox);
void CAN_CancelTransmit(CAN_TransmitMailBox_TypeDef CAN_TransmitMailbox);
void CAN_FIFORelease(void);
CAN_NbrPendingMessage_TypeDef CAN_MessagePending(void);
void CAN_Receive(void);
uint32_t CAN_GetReceivedId(void);
CAN_Id_TypeDef CAN_GetReceivedIDE(void);
CAN_RTR_TypeDef CAN_GetReceivedRTR(void);
uint8_t CAN_GetReceivedDLC(void);
uint8_t CAN_GetReceivedData(uint8_t CAN_DataIndex);
uint8_t CAN_GetReceivedFMI(void);
uint16_t CAN_GetMessageTimeStamp(void);
CAN_Sleep_TypeDef CAN_Sleep(void);
CAN_WakeUp_TypeDef CAN_WakeUp(void);
CAN_ModeStatus_TypeDef CAN_OperatingModeRequest(CAN_OperatingMode_TypeDef CAN_OperatingMode);
CAN_ErrorCode_TypeDef CAN_GetLastErrorCode(void);
CAN_Page_TypeDef CAN_GetSelectedPage(void);
void CAN_SelectPage(CAN_Page_TypeDef CAN_Page);
FlagStatus CAN_GetFlagStatus(CAN_FLAG_TypeDef CAN_Flag);
void CAN_ClearFlag(CAN_FLAG_TypeDef CAN_Flag);
ITStatus CAN_GetITStatus(CAN_IT_TypeDef CAN_IT);
void CAN_ClearITPendingBit(CAN_IT_TypeDef CAN_IT);
/**
  * @}
  */
#endif /* __STM8S_CAN_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
