/**
  ******************************************************************************
  * @file    stm32f30x_i2c_cpal.h
  * @author  MCD Application Team
  * @version V1.2.1
  * @date    31-October-2014
  * @brief   This file contains all the functions prototypes for the I2C firmware
  *          layer.
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
/*========= CPAL library files includes =========*/
#include "stm32f30x_i2c_cpal_hal.h"
#include "stm32f30x_i2c_cpal_conf.h"

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F30X_I2C_CPAL_H
#define __STM32F30X_I2C_CPAL_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Exported types ------------------------------------------------------------*/
/*========= CPAL_Dev_TypeDef =========*/
/* CPAL devices enumeration contains the device instances */

typedef enum
{
  CPAL_I2C1       =   0x00,	    /*!< Use I2C1 device */

  CPAL_I2C2       =   0x01,         /*!< Use I2C2 device */

  CPAL_I2C3       =   0x02          /*!< Use I2C3 device */

}CPAL_DevTypeDef;


/*========= CPAL_Direction_TypeDef =========*/
/* CPAL transfer directions enumeration is used to configure DMA channels
   (TX and RX) if CPAL_PROGMODEL_DMA is selected or TX/RX interrupts
   if CPAL_PROGMODEL_INTERRUPT is selected:
       - If CPAL_DIRECTION_TX is selected only TX DMA Channel or TX Interrupt is configured
       - If CPAL_DIRECTION_RX is selected only RX DMA Channel or RX Interrupt is configured
       - If CPAL_DIRECTION_TXRX is selected both TX and Rx DMA Channels or Interrupts are configured.*/

typedef enum
{
  CPAL_DIRECTION_TX        = 0x01,         /*!<Transmitter only direction */

  CPAL_DIRECTION_RX        = 0x02,         /*!<Receiver only direction */

  CPAL_DIRECTION_TXRX      = 0x03          /*!<Transmitter and Receiver direction */

}CPAL_DirectionTypeDef;


/*========= CPAL_Mode_TypeDef =========*/
/* CPAL device Mode enumeration is used to select in which mode the
   device will proceed : in master mode or slave mode.
   When an I2C device is used in master mode, it will initiate communication
   by sending start bit. When it is used in slave mode, it will wait till
   receiving its own address to start communication.*/

typedef enum
{
  CPAL_MODE_MASTER   =   0x00,    /*!< Use device as master */

  CPAL_MODE_SLAVE    =   0x01     /*!< Use device as slave */

}CPAL_ModeTypeDef;


/*========= CPAL_ProgModel_TypeDef =========*/
/* CPAL Programming Models enumeration  is used to define the programming
   model of device communication.
          - CPAL_PROGMODEL_DMA : device is programmed to communicate using DMA.
          - CPAL_PROGMODEL_INTERRUPT : device is programmed to communicate using
            interruption ( TXE , RXNE ...) handlers.
          - CPAL_PROGMODEL_POLLING : device is programmed to communicate using
            application polling routines (interrupts and DMA will not be used).*/

typedef enum
{
  CPAL_PROGMODEL_INTERRUPT = 0x01,         /*!<Interrupt transfer programming model */

  CPAL_PROGMODEL_DMA       = 0x02          /*!<DMA transfer programming model */

}CPAL_ProgModelTypeDef;


/*========= CPAL_Transfer_TypeDef =========*/
/* CPAL Transfer structure contains all transfer parameters which are used
   in every Read or Write operation.*/

typedef struct
{
  uint8_t*        pbBuffer;        /*!<The address of the buffer from/to which the transfer should start */

  __IO uint32_t   wNumData;        /*!<Number of data to be transferred for the current transaction */

  uint32_t        wAddr1;          /*!<Contains the Target device Address (optional)*/

  uint32_t        wAddr2;          /*!<Contains the Register/Physical Address into the device (optional) */

} CPAL_TransferTypeDef;


/*========= CPAL_State_TypeDef =========*/
/* CPAL global State enumeration contains the current state of CPAL communication.
   Before starting each operation this state is tested. After each operation
   CPAL_State is updated with the new value resulting from the relative operation.*/

typedef enum
{
  CPAL_STATE_DISABLED = 0x00,        /*!<The Disabled state indicates that the device
                                         is not configured.*/

  CPAL_STATE_READY    = 0x01,        /*!<The Ready state indicates that the device is configured
                                         correctly and is ready for read or write operation and/or
                                         the last transaction has been successfully completed */

  CPAL_STATE_READY_TX = 0x03,        /*!<The Ready_TX state indicates that the device is ready for
                                         transmission operation */

  CPAL_STATE_READY_RX = 0x05,        /*!<The Ready_RX state indicates that the device is ready for
                                         reception operation */

  CPAL_STATE_BUSY     = 0x02,        /*!<The Busy state indicates that a Write or Read
                                         operation started */

  CPAL_STATE_BUSY_TX  = 0x06,        /*!<The Busy_TX state indicates that a transmission
                                         operation is on going */

  CPAL_STATE_BUSY_RX  = 0x0A,        /*!<The Busy_RX state indicates that a reception
                                         operation is on going */

  CPAL_STATE_ERROR    = 0x10,        /*!<The Error state indicates that the last operation failed.
                                         To determine which error caused the failure, read the
                                         device status structure.*/
}CPAL_StateTypeDef;

/*========= CPAL_I2CError_TypeDef =========*/
/* I2C Errors TypeDef */

typedef enum
{
  CPAL_I2C_ERR_NONE      = 0x00000000, /*!<No Error: This is the default state for an Idle peripheral */

  CPAL_I2C_ERR_TIMEOUT   = 0x000000FF, /*!<Timeout error: The specified timeout has been elapsed without
                                         any response (expected flag or data didn't happen at expected time).*/

  CPAL_I2C_ERR_BERR      = 0x00000100, /*!<A bus error is detected when a START or a STOP condition is detected
                                           and is not located after a multiple of 9 SCL clock pulses.
                                           The bus error flag is set only in case the I2C is involved in the transfer
                                           as master or addressed slave.
                                           In case of a misplaced START or ReSTART detection, the I2C enters address
                                           recognition state as for a correct START condition.
                                           When a bus error is detected, BERR flag is set in I2C_ISR register, and an
                                           interrupt is generated if ERRIE is set in I2C_CR1 register.*/

  CPAL_I2C_ERR_ARLO      = 0x00000200, /*!<An arbitration loss is detected when a high level is sent on SDA, but a low
                                           level is sampled on the SCL rising edge.
                                            - In master mode, the arbitration loss is detected during address phase, data
                                              phase and data acknowledge phase. In that case, SDA and SCL lines are released,
                                              START control bit is cleared by hardware and the master switches automatically
                                              to slave mode.
                                            - In slave mode, arbitration loss is detected during data phase and data acknowledge
                                              phase. In that case, the transmission is stopped, and SCL and SDA lines are released.
                                           When an arbitration loss is detected, ARLO flag is set in I2C_ISR register, and an
                                           interrupt is generated if ERRIE is set in I2C_CR1 register.*/

  CPAL_I2C_ERR_AF        = 0x00000010, /*!<Acknowledge Failure : NACK is considered as error only when this event is set in master mode.
                                            - If Master: a Stop or repeated Start condition must be generated by software.
                                            When a NACK is detected, NACKF flag is set in I2C_ISR register, and an
                                           interrupt is generated if NACKIE is set in I2C_CR1 register.*/

  CPAL_I2C_ERR_OVR       = 0x00000400, /*!<An overrun or underrun error is detected in slave mode when NOSTRETCH=1 and:
                                            - In reception when a new byte is received and the RXDR register has not been read yet
                                            New received byte is lost, and a NACK is automatically sent as a response to the new byte.
                                            - In transmission :
                                              – when STOPF=1 and the first data should be sent. The content of TXDATA is sent.
                                              – when a new byte should be sent and the TXDR register has not been written yet.
                                                The content of TXDATA is sent (same byte is sent twice)
                                           When an overrun or underrun error is detected, OVR flag is set in I2C_ISR register,
                                           and an interrupt is generated if ERRIE is set in I2C_CR1 register.*/

 }CPAL_I2CErrorTypeDef;


/*========= CPAL_Dev_TypeDef =========*/
/* CPAL Device structure definition */

typedef struct
{
  CPAL_DevTypeDef         CPAL_Dev;          /*!<Instance of the device. This parameter can be one
                                                 of the following values: CPAL_Dev_TypeDef */

  CPAL_DirectionTypeDef   CPAL_Direction;    /*!<Specifies the direction for the device transfers.
                                                 It can be one of the following values: CPAL_Direction_TypeDef */

  CPAL_ModeTypeDef        CPAL_Mode;         /*!<Specifies the maser/slave mode of device. It can be one of the
                                                 following values: CPAL_Mode_TypeDef */

  CPAL_ProgModelTypeDef   CPAL_ProgModel;    /*!<Specifies the programming model for the device transfers.
                                                 It can be one of the following values:  CPAL_ProgModel_Enum */

  CPAL_TransferTypeDef*   pCPAL_TransferTx;  /*!<Pointer on a structure specifying the parameters of the current
                                                 transmission operations. The structure fields are specified as
                                                 follows: CPAL_Transfer_TypeDef. Use pNULL value if this direction
                                                 is not needed.*/

  CPAL_TransferTypeDef*   pCPAL_TransferRx;  /*!<Pointer on a structure specifying the parameters of the current
                                                 reception operations. The structure fields are specified as
                                                 follows: CPAL_Transfer_TypeDef. Use pNULL value if this direction
                                                 is not needed.*/

  __IO CPAL_StateTypeDef  CPAL_State;        /*!<Holds the current State of the CPAL driver relative to the device
                                                 instantiated by CPAL_Dev field. The state parameter can be one of
                                                 the following values: CPAL_State_Enum */

  __IO uint32_t           wCPAL_DevError;    /*!<Specifies the error code for the current operation.The error codes
                                                 are specified for each device type as follows:
                                                 CPAL_I2CError_Enum for I2C devices */

  uint32_t                wCPAL_Options;     /*!<Bit-field value specifying additional options for the configuration
                                                 of the device: The bit-field value can be any combination of following
                                                 values: CPAL_Options_Enum. When a value is not selected the relative
                                                 feature is disabled */

  __IO uint32_t           wCPAL_Timeout;     /*!<This field is with timeout procedure. its used to detect timeout */

  I2C_InitTypeDef*        pCPAL_I2C_Struct;  /*!<Pointer to a device Initialization structure as described
                                                 in the standard device library driver.
                                                 A NULL pointer can be provided for this field and, in this case,
                                                 the default values will be used for all the device initialization.
                                                 If only some fields need to be modified for the initialization,
                                                 one can use the CPAL_PPP_StructInit() function just before setting
                                                 the needed fields.
                                                   Example:
                                                     CPAL_InitTypeDef   I2C1_DevStructure;
                                                     CPAL_I2C_StructInit(&I2C1_DevStructure);
                                                     I2C1_DevStructure->pI2C_Struct->I2C_Timing= 0x4010000A;
                                                     CPAL_I2C_Init(&I2C1_DevStructure); */
}CPAL_InitTypeDef;


/*========= Table containing all I2C device structures =========*/
extern CPAL_InitTypeDef* I2C_DevStructures[];


/*========= CPAL_Global_Device_Structures =========*/
/* CPAL Global Device Structures are the Global default structures which
   are used to handle devices configuration and status.*/

#ifdef CPAL_USE_I2C1
extern CPAL_InitTypeDef I2C1_DevStructure;
#endif /* CPAL_USE_I2C1 */

#ifdef CPAL_USE_I2C2
extern CPAL_InitTypeDef I2C2_DevStructure;
#endif /* CPAL_USE_I2C2 */

#ifdef CPAL_USE_I2C3
extern CPAL_InitTypeDef I2C3_DevStructure;
#endif /* CPAL_USE_I2C3 */

/* Exported constants --------------------------------------------------------*/

/*========= CPAL_Options_TypeDef =========*/
/* CPAL Options defines contains configuration options which can be affected
   to wCPAL_Options which is a bit-field argument so any combination of these
   parameters can be selected. If one option is not selected then the relative
   feature is disabled.
   There are common options and device specific options.*/

#define CPAL_OPT_I2C_DUALADDR           ((uint32_t)0x00000001)  /*!<Use Dual Address Mode (available in Slave Mode only).
                                                                    To use this option enable it by affecting this define
                                                                    and own address2 to wCPAL_Options */

#define CPAL_OPT_DMATX_HTIT             ((uint32_t)0x00000200)  /*!<Enable the Transmitter DMA Half Transfer Complete interrupt */

#define CPAL_OPT_DMARX_HTIT             ((uint32_t)0x00001000)  /*!<Enable the Receiver DMA Half Transfer Complete interrupt */

#define CPAL_OPT_DMATX_CIRCULAR         ((uint32_t)0x00004000)  /*!<Enable the Circular Mode for DMA Transmitter */

#define CPAL_OPT_DMARX_CIRCULAR         ((uint32_t)0x00008000)  /*!<Enable the Circular Mode for DMA Receiver */

#define CPAL_OPT_NO_MEM_ADDR            ((uint32_t)0x00010000)  /*!<Enable No Memory addressing mode: only slave device address sent
                                                                    No Register/Physical address to be sent after slave address */

#define CPAL_OPT_16BIT_REG              ((uint32_t)0x00020000)  /*!<Enable 16-Bit Register/Physical addressing mode (two bytes,
                                                                   MSB first). This option is supported only when CPAL_OPT_NO_MEM_ADDR
                                                                   option is not set */

#define CPAL_OPT_I2C_GENCALL            ((uint32_t)0x00100000)  /*!<Use General Call Address Mode (available in Slave Mode only)
                                                                    (General Call Address = 0x00) */

#define CPAL_OPT_I2C_AUTOMATIC_END      ((uint32_t)0x00200000)  /*!<Enable Automatic end mode of STOP generation for master */

#define CPAL_OPT_I2C_ERRIT_DISABLE      ((uint32_t)0x00400000)  /*!<Disable I2C Errors interrupt (Bus Error, Arbitration Loss,
                                                                    Acknowledge Failure and Overrun/Underrun Errors).
                                                                    By default, errors interrupt is enabled to manage errors efficiently */

#define CPAL_OPT_I2C_NOSTOP             ((uint32_t)0x00800000)  /*!<Use communication mode with no STOP generation at the end
                                                                    of data transfer (for multi-read/write operations) */

#define CPAL_OPT_I2C_NOSTOP_MODE        ((uint32_t)0x01000000)  /*!<Start communication in No STOP generation mode and close
                                                                    communication by Generating stop */

#define CPAL_OPT_I2C_OA2_MASK           ((uint32_t)0x0E000000)  /*!<Contain Own Address 2 Mask (OA2MSK is coded on 3 bit)*/

#define CPAL_OPT_I2C_10BIT_HEADR        ((uint32_t)0x10000000)  /*!<Enable the send of slave address-10bit-header only when switching from
                                                                    master transmitter to master receiver mode with No stop generation option enabled */

#define CPAL_OPT_I2C_WAKEUP_STOP        ((uint32_t)0x20000000)  /*!<Enable the WakeUp from stop capability for the I2C slave device */

#define CPAL_OPT_I2C_NACK_ADD           ((uint32_t)0x40000000)  /*!<Initialize the I2C Slave device without enabling the acknowledgement of its
                                                                    own address. This option must not be used with No Stop generation mode */

/*========= CPAL_Interne_Defines =========*/

#define CPAL_PASS             ((uint32_t)0x00000000) /*!<This value is returned if the last operation succeed */

#define CPAL_FAIL             ((uint32_t)0x00000001) /*!<This value is returned if the last operation failed */

#define pNULL                 (void*)0  /*!<This Value is used to initialise a null pointer */

/* Exported macro ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/


/*========= CPAL_I2C_Exported_Functions =========*/
/* These functions constitute the main CPAL API interface. All functions take as argument the
   CPAL_InitTypeDef structure defined in @ref CPAL_Dev_TypeDef. */

uint32_t  CPAL_I2C_Init         (CPAL_InitTypeDef* pDevInitStruct); /*<!This function Initializes the selected I2C device
                                                                        and all needed resources (GPIOs, clocks, DMA,
                                                                        interrupts …)*/

uint32_t  CPAL_I2C_DeInit       (CPAL_InitTypeDef* pDevInitStruct); /*<!This function free the resources used by the I2C
                                                                        device (GPIOs, clocks, DMA, interrupts …) and
                                                                        deinitialize the device itself */

uint32_t  CPAL_I2C_StructInit   (CPAL_InitTypeDef* pDevInitStruct); /*<!This function Initializes I2C device structure
                                                                        by filling all fields with their default values.
                                                                        Warning: Pointer fields are filled with CPAL local variables
                                                                        pointer. To avoid all risks, it is recommended to declare
                                                                        application local variables and fill these fields with their
                                                                        pointers.*/

#if defined (CPAL_I2C_MASTER_MODE) || ! defined (CPAL_I2C_LISTEN_MODE)
uint32_t  CPAL_I2C_Write        (CPAL_InitTypeDef* pDevInitStruct); /*<!This function Writes data to the specified I2C device.
                                                                        All information relative to the write transfer parameters and
                                                                        current status are extracted from pCPAL_TransferTx field defined
                                                                        in @ref CPAL_Transfer_TypeDef */

uint32_t  CPAL_I2C_Read         (CPAL_InitTypeDef* pDevInitStruct); /*<!This function Read data from the specified I2C device
                                                                        All information relative to the read transfer parameters and
                                                                        current status are extracted from pCPAL_TransferTx field defined
                                                                        in @ref CPAL_Transfer_TypeDef */

#endif /* CPAL_I2C_MASTER_MODE || ! CPAL_I2C_LISTEN_MODE */


#if defined (CPAL_I2C_LISTEN_MODE) && defined (CPAL_I2C_SLAVE_MODE)
uint32_t  CPAL_I2C_Listen       (CPAL_InitTypeDef* pDevInitStruct); /*<!This function allows the specified I2C device to enter listen mode
                                                                        All information relative to the read or write transfer parameters and
                                                                        current status are extracted from fields defined in @ref CPAL_Transfer_TypeDef */
#endif /* CPAL_I2C_LISTEN_MODE && CPAL_I2C_SLAVE_MODE */

uint32_t  CPAL_I2C_IsDeviceReady(CPAL_InitTypeDef* pDevInitStruct); /*<!This function can be used to wait until target device is ready
                                                                        for communication (ie. for memories after write operations) */


uint32_t CPAL_I2C_EV_IRQHandler(CPAL_InitTypeDef* pDevInitStruct); /*<!This function manages all I2C device events */

uint32_t CPAL_I2C_ER_IRQHandler(CPAL_InitTypeDef* pDevInitStruct); /*<!This function manages all I2C device errors */

#ifdef CPAL_I2C_DMA_PROGMODEL
uint32_t CPAL_I2C_DMA_TX_IRQHandler(CPAL_InitTypeDef* pDevInitStruct); /*<!This function Handles DMA TX Interrupts */

uint32_t CPAL_I2C_DMA_RX_IRQHandler(CPAL_InitTypeDef* pDevInitStruct); /*<!This function Handles DMA RX Interrupts */
#endif /* CPAL_I2C_DMA_PROGMODEL */

void CPAL_HAL_NVICInit(void); /*<! Initialize NVIC Priority Group */



/*========== CPAL_TIMEOUT_Callback ==========*/

#ifndef CPAL_TIMEOUT_UserCallback
 uint32_t CPAL_TIMEOUT_UserCallback(CPAL_InitTypeDef* pDevInitStruct);   /*<!This function is called when a Timeout
                                                                             occurs during communication with devices */
#endif

/*========= CPAL_I2C_User_Callbacks =========*/
/* These functions prototypes only are declared here. User can (optionally)
   implement the function body in his own application depending on the application needs.
   Each callback is called in a particular situation detailed in the callback description.*/

#ifndef CPAL_I2C_TX_UserCallback
void CPAL_I2C_TX_UserCallback   (CPAL_InitTypeDef* pDevInitStruct); /*<!This function is called when (in Interrupt mode)
                                                                        the peripheral is preparing to send data */
#endif

#ifndef CPAL_I2C_RX_UserCallback
void CPAL_I2C_RX_UserCallback   (CPAL_InitTypeDef* pDevInitStruct); /*<!This function is called when (in Interrupt mode)
                                                                        the peripheral has received data */
#endif

#ifndef CPAL_I2C_TXTC_UserCallback
void CPAL_I2C_TXTC_UserCallback(CPAL_InitTypeDef* pDevInitStruct); /*<!This function is called when (in DMA or Interrupt mode)
                                                                      TX Transfer is complete (to use in DMA mode, Transfer complete
                                                                      interrupt must be enabled) */
#endif

#ifndef CPAL_I2C_RXTC_UserCallback
void CPAL_I2C_RXTC_UserCallback(CPAL_InitTypeDef* pDevInitStruct); /*<!This function is called when (in DMA or Interrupt mode)
                                                                       RX Transfer is complete (to use in DMA mode, Transfer complete
                                                                       interrupt must be enabled) */
#endif

#ifndef CPAL_I2C_DMATXTC_UserCallback
void CPAL_I2C_DMATXTC_UserCallback(CPAL_InitTypeDef* pDevInitStruct); /*<!This function is called (in DMA mode) when
                                                                          DMA Transmission is finished (If Transfer Complete
                                                                          interrupt is enabled) */
#endif

#ifndef CPAL_I2C_DMATXHT_UserCallback
void CPAL_I2C_DMATXHT_UserCallback(CPAL_InitTypeDef* pDevInitStruct); /*<!This function is called (in DMA mode) when the
                                                                          DMA Transmission has reached the half of the
                                                                          buffer (If Half Transfer interrupt is enabled) */
#endif

#ifndef CPAL_I2C_DMATXTE_UserCallback
void CPAL_I2C_DMATXTE_UserCallback(CPAL_InitTypeDef* pDevInitStruct); /*<!This function is called when (in DMA mode) a
                                                                          DMA Transmission transfer error has occurred
                                                                          (If Transfer Error interrupt is enabled ) */
#endif

#ifndef CPAL_I2C_DMARXTC_UserCallback
void CPAL_I2C_DMARXTC_UserCallback(CPAL_InitTypeDef* pDevInitStruct); /*<!This function is called when (in DMA mode) when
                                                                          DMA Reception is finished (If Transfer Complete
                                                                          interrupt is enabled) */
#endif

#ifndef CPAL_I2C_DMARXHT_UserCallback
void CPAL_I2C_DMARXHT_UserCallback(CPAL_InitTypeDef* pDevInitStruct); /*<!This function is called when (in DMA mode) the
                                                                          DMA Reception has reached the half of the
                                                                          buffer (If Half Transfer interrupt is enabled) */
#endif

#ifndef CPAL_I2C_DMARXTE_UserCallback
void CPAL_I2C_DMARXTE_UserCallback(CPAL_InitTypeDef* pDevInitStruct); /*<!This function is called when (in DMA mode) a
                                                                          DMA Reception transfer error has occurred
                                                                          (If Transfer Error interrupt is enabled ) */
#endif

#ifndef CPAL_I2C_GENCALL_UserCallback
void CPAL_I2C_GENCALL_UserCallback(CPAL_InitTypeDef* pDevInitStruct); /*<!This function is called when General Call mode
                                                                          is set (used in General Call Mode only ) */
#endif

#ifndef CPAL_I2C_DUALF_UserCallback
void CPAL_I2C_DUALF_UserCallback(CPAL_InitTypeDef* pDevInitStruct); /*<!This function is called when Slave OA2 Address
                                                                        matched (used in Dual Address Mode only ) */
#endif

#ifndef CPAL_I2C_SLAVE_READ_UserCallback
void CPAL_I2C_SLAVE_READ_UserCallback(CPAL_InitTypeDef* pDevInitStruct); /*<!This function is called when a write operation is
                                                                             requested from master in Listen mode only */
#endif

#ifndef CPAL_I2C_SLAVE_WRITE_UserCallback
void CPAL_I2C_SLAVE_WRITE_UserCallback(CPAL_InitTypeDef* pDevInitStruct); /*<!This function is called when a read operation is
                                                                             requested from master in Listen mode only */
#endif

/*========= CPAL_User_ErrorCallback_Prototypes =========*/
/* User can use two types of Callback:
    - Single Error Callback : All error are handled by one Callback (CPAL_I2C_ERR_UserCallback()).
    - Multiple Error Callback : Each error has its own Callback ( CPAL_I2C_ERRTYPE_UserCallback () ,
        ERRTYPE : can be one of I2C Errors (BERR, ARLO, OVR and NACK)).
   To select one of this type, user should comment or uncomment adequate defines in stm32f30x_i2c_cpal_conf.h file. */

#ifdef USE_SINGLE_ERROR_CALLBACK

  #ifndef CPAL_I2C_ERR_UserCallback
  void  CPAL_I2C_ERR_UserCallback(CPAL_DevTypeDef pDevInstance, uint32_t DeviceError); /*<!This callback is called when an error
                                                                                           occurred on the peripheral while transferring
                                                                                           (If I2C Error interrupt is enabled). Device
                                                                                           instance and error type (DeviceError) are
                                                                                           passed as argument. Device_Error value can be
                                                                                           one of CPAL_I2CErrorTypeDef enumeration */
  #endif
#endif /* USE_SINGLE_ERROR_CALLBACK */

#ifdef USE_MULTIPLE_ERROR_CALLBACK

  #ifndef CPAL_I2C_BERR_UserCallback
   void  CPAL_I2C_BERR_UserCallback(CPAL_DevTypeDef pDevInstance); /*<!This callback is called when an Bus ERROR
                                                                       occurred on the peripheral while transferring
                                                                       (If I2C Error interrupt is enabled) */
  #endif

  #ifndef CPAL_I2C_ARLO_UserCallback
   void  CPAL_I2C_ARLO_UserCallback(CPAL_DevTypeDef pDevInstance); /*<!This callback is called when an Arbitration Lost
                                                                       ERROR occurred on the peripheral while transferring
                                                                       (If I2C Error interrupt is enabled) */
  #endif

  #ifndef CPAL_I2C_OVR_UserCallback
   void  CPAL_I2C_OVR_UserCallback(CPAL_DevTypeDef pDevInstance); /*<!This callback is called when an Overrun/Underrun
                                                                      ERROR occurred on the peripheral while transferring
                                                                      (If I2C Error interrupt is enabled) */
  #endif

  #ifndef CPAL_I2C_AF_UserCallback
   void  CPAL_I2C_AF_UserCallback(CPAL_DevTypeDef pDevInstance); /*<!This callback is called when an Not Acknowledge
                                                                     is received (If I2C NACK interrupt is enabled) */
  #endif

#endif /* USE_SINGLE_ERROR_CALLBACK */



#ifdef __cplusplus
}
#endif

#endif /*__STM32F30X_I2C_CPAL_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
