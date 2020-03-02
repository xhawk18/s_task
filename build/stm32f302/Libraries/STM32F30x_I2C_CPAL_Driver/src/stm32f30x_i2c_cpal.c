/**
  ******************************************************************************
  * @file    stm32f30x_i2c_cpal.c
  * @author  MCD Application Team
  * @version V1.2.1
  * @date    31-October-2014
  * @brief   This file provides all the CPAL firmware functions for I2C peripheral.
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
#include "stm32f30x_i2c_cpal.h"

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/


/* This macro allows to test on a flag status and to start Timeout procedure if the
   waiting time exceeds the allowed timeout period.
   @note This macro has not been implemented as a function because the entered parameter
   'cmd' itself can be a macro (if it was implemented as a function, the check on the
   flag would be done only once, while the required behavior is to check the flag
   continuously).*/

#define __CPAL_I2C_TIMEOUT_DETECT                ((pDevInitStruct->wCPAL_Timeout == CPAL_I2C_TIMEOUT_MIN) ||\
                                                 (pDevInitStruct->wCPAL_Timeout == CPAL_I2C_TIMEOUT_DEFAULT))

#define __CPAL_I2C_TIMEOUT(cmd, timeout)         pDevInitStruct->wCPAL_Timeout = CPAL_I2C_TIMEOUT_MIN + (timeout);\
                                                 while (((cmd) == 0) && (!__CPAL_I2C_TIMEOUT_DETECT));\
                                                 if (__CPAL_I2C_TIMEOUT_DETECT)\
                                                 {\
                                                   return CPAL_I2C_Timeout (pDevInitStruct); \
                                                 }\
                                                 pDevInitStruct->wCPAL_Timeout = CPAL_I2C_TIMEOUT_DEFAULT


/* Private variables ---------------------------------------------------------*/

/*========= Table Exported from HAL =========*/

extern I2C_TypeDef* CPAL_I2C_DEVICE[];

#ifdef CPAL_I2C_DMA_PROGMODEL
extern DMA_TypeDef* CPAL_I2C_DMA[];

extern DMA_Channel_TypeDef* CPAL_I2C_DMA_TX_Channel[];
extern DMA_Channel_TypeDef* CPAL_I2C_DMA_RX_Channel[];

extern const uint32_t CPAL_I2C_DMA_TX_TC_FLAG[];
extern const uint32_t CPAL_I2C_DMA_RX_TC_FLAG[];

extern const uint32_t CPAL_I2C_DMA_TX_HT_FLAG[];
extern const uint32_t CPAL_I2C_DMA_RX_HT_FLAG[];

extern const uint32_t CPAL_I2C_DMA_TX_TE_FLAG[];
extern const uint32_t CPAL_I2C_DMA_RX_TE_FLAG[];
#endif /* CPAL_I2C_DMA_PROGMODEL */

/*========= Local structures used in CPAL_I2C_StructInit() function ==========*/

I2C_InitTypeDef I2C_InitStructure;

__IO uint32_t Num_Data = 0;
uint32_t CR2_tmp = 0;

/* Private function prototypes -----------------------------------------------*/

/*========= Local Master events handlers =========*/

#ifdef CPAL_I2C_MASTER_MODE
  static uint32_t I2C_MASTER_TCR_Handle(CPAL_InitTypeDef* pDevInitStruct);    /* Handle Master TCR Interrupt event */
  static uint32_t I2C_MASTER_TC_Handle(CPAL_InitTypeDef* pDevInitStruct);     /* Handle Master TC Interrupt event */
  static uint32_t I2C_MASTER_STOP_Handle(CPAL_InitTypeDef* pDevInitStruct);   /* Handle Master STOP Interrupt event */
  static uint32_t I2C_MASTER_NACK_Handle(CPAL_InitTypeDef* pDevInitStruct);   /* Handle Master NACK Interrupt event */
 #ifdef CPAL_I2C_IT_PROGMODEL
  static uint32_t I2C_MASTER_TXIS_Handle(CPAL_InitTypeDef* pDevInitStruct);   /* Handle Master TXIS Interrupt event */
  static uint32_t I2C_MASTER_RXNE_Handle(CPAL_InitTypeDef* pDevInitStruct);   /* Handle Master RXNE Interrupt event */
 #endif /* CPAL_I2C_IT_PROGMODEL */
#endif /* CPAL_I2C_MASTER_MODE */

/*========= Local Slave events handlers =========*/

#ifdef CPAL_I2C_SLAVE_MODE
  static uint32_t I2C_SLAVE_ADDR_Handle(CPAL_InitTypeDef* pDevInitStruct);   /* Handle Slave ADDR Interrupt event */
  static uint32_t I2C_SLAVE_STOP_Handle(CPAL_InitTypeDef* pDevInitStruct);   /* Handle Slave STOPF Interrupt event */
  static uint32_t I2C_SLAVE_NACK_Handle(CPAL_InitTypeDef* pDevInitStruct);   /* Handle Slave NACK Interrupt event */
 #ifdef CPAL_I2C_IT_PROGMODEL
  static uint32_t I2C_SLAVE_TXIS_Handle(CPAL_InitTypeDef* pDevInitStruct);   /* Handle Slave TXIS Interrupt event */
  static uint32_t I2C_SLAVE_RXNE_Handle(CPAL_InitTypeDef* pDevInitStruct);   /* Handle Slave RXNE Interrupt event */
 #endif /* CPAL_I2C_IT_PROGMODEL */
#endif /* CPAL_I2C_SLAVE_MODE */


#ifdef CPAL_I2C_DMA_PROGMODEL
/*========= Local DMA Manager =========*/
static uint32_t I2C_Enable_DMA (CPAL_InitTypeDef* pDevInitStruct, CPAL_DirectionTypeDef Direction);
#endif /* CPAL_I2C_DMA_PROGMODEL */

/*========= CPAL Timeout handler =========*/
static uint32_t CPAL_I2C_Timeout (CPAL_InitTypeDef* pDevInitStruct);


/* Private functions ---------------------------------------------------------*/

/*================== USER CPAL Functions ==================*/


/**
  * @brief  Initialize the peripheral and all related clocks, GPIOs, DMA and
  *         Interrupts according to the specified parameters in the
  *         CPAL_InitTypeDef structure.
  * @param  pDevInitStruct : Pointer to the peripheral configuration structure.
  * @retval CPAL_PASS or CPAL_FAIL
  */
uint32_t CPAL_I2C_Init(CPAL_InitTypeDef* pDevInitStruct)
{
  CPAL_LOG("\n\r\n\rLOG <CPAL_I2C_Init> : I2C Device Init");

  /* If CPAL_State is not BUSY */
  if ((pDevInitStruct->CPAL_State == CPAL_STATE_READY)
      || (pDevInitStruct->CPAL_State == CPAL_STATE_ERROR)
      || (pDevInitStruct->CPAL_State == CPAL_STATE_DISABLED))
  {
    /*
    - If CPAL_State is CPAL_STATE_ERROR (an Error occurred in transaction):
      Perform the initialization routines (device will be deinitialized during initialization).
    - If CPAL_State is CPAL_STATE_READY:
      Perform the initialization routines
    - If CPAL_State is CPAL_STATE_DISABLED:
      Perform the Initialization routines                                   */

#ifndef CPAL_I2C_DMA_PROGMODEL
    if (pDevInitStruct->CPAL_ProgModel == CPAL_PROGMODEL_DMA)
    {
      /* Update CPAL_State to CPAL_STATE_ERROR */
      pDevInitStruct->CPAL_State = CPAL_STATE_ERROR;

      /* Exit Init function */
      return CPAL_FAIL;
    }
#endif /* CPAL_I2C_DMA_PROGMODEL */

#ifndef CPAL_I2C_IT_PROGMODEL
    if (pDevInitStruct->CPAL_ProgModel == CPAL_PROGMODEL_INTERRUPT)
    {
      /* Update CPAL_State to CPAL_STATE_ERROR */
      pDevInitStruct->CPAL_State = CPAL_STATE_ERROR;

      /* Exit Init function */
      return CPAL_FAIL;
    }
#endif /* CPAL_I2C_IT_PROGMODEL */

    /* Disable I2Cx device */
    __CPAL_I2C_HAL_DISABLE_DEV(pDevInitStruct->CPAL_Dev);

    CPAL_LOG("\n\rLOG : I2C Device Disabled");

    /* Deinitialize I2Cx GPIO */
    CPAL_I2C_HAL_GPIODeInit(pDevInitStruct->CPAL_Dev);

    CPAL_LOG("\n\rLOG : I2C Device IOs Deinit");

    /* Deinitialize I2Cx clock */
    CPAL_I2C_HAL_CLKDeInit(pDevInitStruct->CPAL_Dev);

    CPAL_LOG("\n\rLOG : I2C Device Clock Deinit");

#ifdef CPAL_I2C_DMA_PROGMODEL
    /* Deinitialize DMA Channels */
    if (pDevInitStruct->CPAL_ProgModel == CPAL_PROGMODEL_DMA)
    {
      CPAL_I2C_HAL_DMADeInit(pDevInitStruct->CPAL_Dev, pDevInitStruct->CPAL_Direction);

      CPAL_LOG("\n\rLOG : I2C Device DMA Deinit");
    }
#endif /* CPAL_I2C_DMA_PROGMODEL */

    /*--------------------------------------------------------------------------
    GPIO pins configuration
    ---------------------------------------------------------------------------*/
    /* Initialize I2Cx GPIO */
    CPAL_I2C_HAL_GPIOInit(pDevInitStruct->CPAL_Dev);

    CPAL_LOG("\n\rLOG : I2C Device IOs Init");

    /*--------------------------------------------------------------------------
        Peripheral Clock Initialization
    ---------------------------------------------------------------------------*/
    /* Initialize I2Cx clock */
    CPAL_I2C_HAL_CLKInit(pDevInitStruct->CPAL_Dev);

    CPAL_LOG("\n\rLOG : I2C Device Clock Init");

    /*--------------------------------------------------------------------------
    Peripheral Initialization
    ---------------------------------------------------------------------------*/

    /* Initialize I2Cx device with parameters stored in pCPAL_I2C_Struct */
    I2C_Init(CPAL_I2C_DEVICE[pDevInitStruct->CPAL_Dev], pDevInitStruct->pCPAL_I2C_Struct);

    CPAL_LOG("\n\rLOG : I2C Device Config");

    /* If General Call mode option bit selected */
    if ((pDevInitStruct->wCPAL_Options & CPAL_OPT_I2C_GENCALL) != 0)
    {
      /* Enable GENERAL CALL address mode */
      __CPAL_I2C_HAL_ENABLE_GENCALL(pDevInitStruct->CPAL_Dev);

      CPAL_LOG("\n\rLOG : I2C Device GENCALL Mode Enabled");
    }

    /* If OA2 Address mode option bit selected */
    if ((pDevInitStruct->wCPAL_Options & CPAL_OPT_I2C_DUALADDR) != 0)
    {
      /* Configure OA2 */
      __CPAL_I2C_HAL_OA2_CONF(pDevInitStruct->CPAL_Dev, (uint32_t)(pDevInitStruct->wCPAL_Options & 0x000000FE));

      /* Configure OA2 masks */
      __CPAL_I2C_HAL_OA2_MASK_CONF(pDevInitStruct->CPAL_Dev, (uint32_t)((pDevInitStruct->wCPAL_Options & CPAL_OPT_I2C_OA2_MASK) >> 25));

      /* Enable OA2 address mode */
      __CPAL_I2C_HAL_ENABLE_OA2(pDevInitStruct->CPAL_Dev);

      CPAL_LOG("\n\rLOG : I2C Device OA2 ADDR Mode Enabled");
    }

    /* If WakeUp from STOP option bit selected */
    if ((pDevInitStruct->wCPAL_Options & CPAL_OPT_I2C_WAKEUP_STOP) != 0)
    {
      /* Enable WakeUp from STOP mode */
      __CPAL_I2C_HAL_ENABLE_WAKEUP(pDevInitStruct->CPAL_Dev);

      CPAL_LOG("\n\rLOG : I2C Device WakeUp from Stop Mode Enabled");
    }
    else
    {
      /* Disable WakeUp from STOP mode */
      __CPAL_I2C_HAL_DISABLE_WAKEUP(pDevInitStruct->CPAL_Dev);
    }

    /* If NACK Slave Own Address option bit selected */
    if ((pDevInitStruct->wCPAL_Options & CPAL_OPT_I2C_NACK_ADD) != 0)
    {
      /* Disable Acknowledgement of own Address */
      __CPAL_I2C_HAL_DISABLE_DEV(pDevInitStruct->CPAL_Dev);

      CPAL_LOG("\n\rLOG : I2C Device NACK Own Address Mode Enabled");
    }

#ifdef CPAL_I2C_DMA_PROGMODEL
    /*--------------------------------------------------------------------------
    DMA Initialization :
    ---------------------------------------------------------------------------*/
    /* If DMA Programming model is selected*/
    if (pDevInitStruct->CPAL_ProgModel == CPAL_PROGMODEL_DMA)
    {
      /* Initialize I2Cx DMA channels */
      CPAL_I2C_HAL_DMAInit(pDevInitStruct->CPAL_Dev, pDevInitStruct->CPAL_Direction, pDevInitStruct->wCPAL_Options);

      CPAL_LOG("\n\rLOG : I2C Device DMA Init");
    }
#endif /* CPAL_I2C_DMA_PROGMODEL */

    /*--------------------------------------------------------------------------
    Peripheral and DMA interrupts Initialization
    ---------------------------------------------------------------------------*/
    /* Initialize I2Cx interrupts */
    CPAL_I2C_HAL_ITInit(pDevInitStruct->CPAL_Dev, pDevInitStruct->wCPAL_Options, pDevInitStruct->CPAL_Direction, pDevInitStruct->CPAL_ProgModel);

    CPAL_LOG("\n\rLOG : I2C Device IT Init");

    /* Update CPAL_State to CPAL_STATE_READY */
    pDevInitStruct->CPAL_State = CPAL_STATE_READY;

    CPAL_LOG("\n\rLOG : I2C Device Ready");

    /* Initialize Timeout procedure */
    _CPAL_TIMEOUT_INIT();

    return CPAL_PASS;
  }
  /* If CPAL_State is BUSY (a transaction is still on going) exit Init function */
  else
  {
    CPAL_LOG("\n\rERROR : I2C Device Busy");

    return CPAL_FAIL;
  }
}


/**
  * @brief  Deinitialize the peripheral and all related clocks, GPIOs, DMA and NVIC
  *         to their reset values.
  * @param  pDevInitStruct: Pointer to the peripheral configuration structure.
  * @retval CPAL_PASS or CPAL_FAIL
  * @note   The Peripheral clock is disabled but the GPIO Ports clocks remains
  *         enabled after this deinitialization.
  */
uint32_t CPAL_I2C_DeInit(CPAL_InitTypeDef* pDevInitStruct)
{
  CPAL_LOG("\n\r\n\rLOG <CPAL_I2C_DeInit> : I2C Device Deinit");

  /* If CPAL_State is not BUSY */
  if ((pDevInitStruct->CPAL_State == CPAL_STATE_READY)
      || (pDevInitStruct->CPAL_State == CPAL_STATE_ERROR)
      || (pDevInitStruct->CPAL_State == CPAL_STATE_DISABLED))
  {
    /*
    - If CPAL_State is CPAL_STATE_ERROR (an Error occurred in transaction):
      Perform the deinitialization routines
    - If CPAL_State is CPAL_STATE_READY:
      Perform the deinitialization routines
    - If CPAL_State is CPAL_STATE_DISABLED:
      Perform the deinitialization routines                                   */

    /*--------------------------------------------------------------------------
    GPIO pins Deinitialization
    Note: The GPIO clock remains enabled after this deinitialization
    ---------------------------------------------------------------------------*/
    /* Deinitialize I2Cx GPIO */
    CPAL_I2C_HAL_GPIODeInit(pDevInitStruct->CPAL_Dev);

    CPAL_LOG("\n\rLOG : I2C Device IOs Deinit");

    /*--------------------------------------------------------------------------
    Peripheral Deinitialization
    ---------------------------------------------------------------------------*/
    /* Disable I2Cx device */
    __CPAL_I2C_HAL_DISABLE_DEV(pDevInitStruct->CPAL_Dev);

    CPAL_LOG("\n\rLOG : I2C Device Disabled");

    /*--------------------------------------------------------------------------
    Peripheral Clock Deinitialization
    ---------------------------------------------------------------------------*/
    /* Deinitialize I2Cx clock */
    CPAL_I2C_HAL_CLKDeInit(pDevInitStruct->CPAL_Dev);

    CPAL_LOG("\n\rLOG : I2C Device Clock Deinit");

#ifdef CPAL_I2C_DMA_PROGMODEL
    /*--------------------------------------------------------------------------
    DMA Deinitialization : if DMA Programming model is selected
    ---------------------------------------------------------------------------*/
    if (pDevInitStruct->CPAL_ProgModel == CPAL_PROGMODEL_DMA)
    {
      CPAL_I2C_HAL_DMADeInit(pDevInitStruct->CPAL_Dev, pDevInitStruct->CPAL_Direction);

      CPAL_LOG("\n\rLOG : I2C Device DMA Deinit");
    }
#endif /* CPAL_I2C_DMA_PROGMODEL */

    /*--------------------------------------------------------------------------
    Interrupts Deinitialization
    ---------------------------------------------------------------------------*/
    CPAL_I2C_HAL_ITDeInit(pDevInitStruct->CPAL_Dev, pDevInitStruct->wCPAL_Options, pDevInitStruct->CPAL_Direction, pDevInitStruct->CPAL_ProgModel);

    CPAL_LOG("\n\rLOG : I2C Device IT Deinit");

    /*--------------------------------------------------------------------------
    Structure fields initialization
    ----------------------------------------------------------------------------*/
    /* Initialize pDevInitStruct state parameters to their default values */
    pDevInitStruct-> CPAL_State     = CPAL_STATE_DISABLED;     /* Device Disabled */
    pDevInitStruct-> wCPAL_DevError = CPAL_I2C_ERR_NONE;       /* No Device Error */
    pDevInitStruct-> wCPAL_Timeout  = ((uint32_t)CPAL_I2C_TIMEOUT_DEFAULT);  /* Set timeout value to CPAL_I2C_TIMEOUT_DEFAULT */

    CPAL_LOG("\n\rLOG :Set State fields to default");

    /*----------------------------------------------------------------------------
    Deinitialize Timeout Procedure
    -----------------------------------------------------------------------------*/
    _CPAL_TIMEOUT_DEINIT();

    return CPAL_PASS;
  }
  /* If CPAL_State is BUSY (a transaction is still on going) Exit Init function */
  else
  {
    CPAL_LOG("\n\rERROR : I2C Device Busy");

    return CPAL_FAIL;
  }
}


/**
  * @brief  Initialize the peripheral structure with default values according
  *         to the specified parameters in the CPAL_I2CDevTypeDef structure.
  * @param  pDevInitStruct: Pointer to the peripheral configuration structure.
  * @retval CPAL_PASS or CPAL_FAIL.
  */
uint32_t CPAL_I2C_StructInit(CPAL_InitTypeDef* pDevInitStruct)
{
  /* Initialize I2C_InitStructure to their default values */
  I2C_InitStructure.I2C_Timing              = 0;                             /* Initialize the I2C_Timing member */
  I2C_InitStructure.I2C_Mode                = I2C_Mode_I2C;                  /* Initialize the I2C_Mode member */
  I2C_InitStructure.I2C_AnalogFilter        = I2C_AnalogFilter_Enable;       /* Initialize the I2C_AnalogFilter member */
  I2C_InitStructure.I2C_DigitalFilter       = 0x00;                          /* Initialize the I2C_DigitalFilter member */
  I2C_InitStructure.I2C_OwnAddress1         = 0;                             /* Initialize the I2C_OwnAddress1 member */
  I2C_InitStructure.I2C_Ack                 = I2C_Ack_Enable;                /* Initialize the I2C_Ack member */
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;  /* Initialize the I2C_AcknowledgedAddress member */

  /* Initialize pDevInitStruct parameter to their default values */
  pDevInitStruct->CPAL_Direction     = CPAL_DIRECTION_TXRX;                  /* Transmitter and Receiver direction selected */
  pDevInitStruct->CPAL_Mode          = CPAL_MODE_MASTER;                     /* Mode Master selected */
  pDevInitStruct->CPAL_ProgModel     = CPAL_PROGMODEL_DMA;                   /* DMA Programming Model selected */
  pDevInitStruct->pCPAL_TransferTx   = pNULL;                                /* Point pCPAL_TransferTx to a Null pointer */
  pDevInitStruct->pCPAL_TransferRx   = pNULL;                                /* Point pCPAL_TransferRx to a Null pointer */
  pDevInitStruct->CPAL_State         = CPAL_STATE_DISABLED;                  /* Device Disabled */
  pDevInitStruct->wCPAL_DevError     = CPAL_I2C_ERR_NONE;                    /* No Device Error */
  pDevInitStruct->wCPAL_Options      = ((uint32_t)0x00000000);               /* No Options selected */
  pDevInitStruct->wCPAL_Timeout      = ((uint32_t)CPAL_I2C_TIMEOUT_DEFAULT); /* Set timeout value to CPAL_I2C_TIMEOUT_DEFAULT */
  pDevInitStruct->pCPAL_I2C_Struct   = &I2C_InitStructure;                   /* Point to I2C_InitStructure (with default values) */

  CPAL_LOG("\n\r\n\rLOG <CPAL_I2C_StructInit> : I2C Device Structure set to Default Value");

  return CPAL_PASS;
}

#if defined (CPAL_I2C_MASTER_MODE) || ! defined (CPAL_I2C_LISTEN_MODE)

/**
  * @brief  Allows to send a data or a buffer of data through the peripheral to
  *         a selected device in a selected location address.
  * @param  pDevInitStruct: Pointer to the peripheral configuration structure.
  * @retval CPAL_PASS or CPAL_FAIL.
  */
uint32_t CPAL_I2C_Write(CPAL_InitTypeDef* pDevInitStruct)
{
  CR2_tmp = 0;

  /* Check I2C State:
  - If busy       --> exit Write operation
  - If disabled   --> exit Write operation
  - If error      --> exit Write operation
  - If ready      -->
                      *- Update CPAL_State to CPAL_STATE_BUSY
                      *- If CPAL_OPT_I2C_NOSTOP_MODE is not selected :
                         - Check if device is busy.
                      *- Update CPAL_State to CPAL_STATE_READY_TX
                      *- If DMA Prog Model :
                         - Configure and enable DMA
                      *- If Master mode :
                         - If 10Bit Mode : Enable ADD10
                         - Configure Slave address
                      *- If Memory Address mode (master)
                         - Send target and memory address
                      *- Update CPAL_State to CPAL_STATE_BUSY_TX
                      *- If Master mode :
                         - Configure AUTOEND, RELOAD and NBYTES
                         - If Interrupt Prog Model :
                             - Generate start and enable interrupts
                         - If DMA Prog Model :
                             - Enable TX DMA request
                             - Generate start and enable interrupts
                      *- If Slave mode :
                         - If Interrupt Prog Model :
                             - Enable interrupts
                         - If DMA Prog Model :
                             - Enable TX DMA request
                             - Enable interrupts                             */

  CPAL_LOG("\n\r\n\rLOG <CPAL_I2C_Write> : I2C Device Write OP");

  /* If Device is Busy (a transaction is still on going) Exit Write function */
  if (((pDevInitStruct->CPAL_State & CPAL_STATE_BUSY) != 0)
      || (pDevInitStruct->CPAL_State == CPAL_STATE_READY_TX)
        || (pDevInitStruct->CPAL_State == CPAL_STATE_READY_RX))
  {
    CPAL_LOG("\n\rERROR : I2C Device Busy");

    return CPAL_FAIL;
  }
  /* If CPAL_State is CPAL_STATE_DISABLED (device is not initialized) Exit Write function */
  else if (pDevInitStruct->CPAL_State == CPAL_STATE_DISABLED)
  {
    CPAL_LOG("\n\rERROR : I2C Device Not Initialized");

    return CPAL_FAIL;
  }
  /* If CPAL_State is CPAL_STATE_ERROR (Error occurred ) */
  else if (pDevInitStruct->CPAL_State == CPAL_STATE_ERROR)
  {
    CPAL_LOG("\n\rERROR : I2C Device Error");

    return CPAL_FAIL;
  }
  /* If CPAL_State is CPAL_STATE_READY ( Start Communication )*/
  else
  {
    /* Update CPAL_State to CPAL_STATE_BUSY */
    pDevInitStruct->CPAL_State = CPAL_STATE_BUSY;

    /* IF CPAL_OPT_I2C_NOSTOP_MODE is not selected */
    if ((pDevInitStruct->wCPAL_Options & CPAL_OPT_I2C_NOSTOP_MODE) == 0)
    {
      /* Wait until BUSY flag is reset */
      __CPAL_I2C_TIMEOUT(!(__CPAL_I2C_HAL_GET_BUSY(pDevInitStruct->CPAL_Dev)), CPAL_I2C_TIMEOUT_BUSY);
    }

    /* Update CPAL_State to CPAL_STATE_READY_TX */
    pDevInitStruct->CPAL_State = CPAL_STATE_READY_TX;
    CPAL_LOG("\n\rLOG : I2C Device Ready TX");

#ifdef CPAL_I2C_DMA_PROGMODEL
    /* If DMA programming model */
    if (pDevInitStruct->CPAL_ProgModel == CPAL_PROGMODEL_DMA)
    {
      /* Configure and enable TX DMA channel */
      I2C_Enable_DMA(pDevInitStruct, CPAL_DIRECTION_TX);
    }
#endif /* CPAL_I2C_DMA_PROGMODEL */

#ifdef CPAL_I2C_MASTER_MODE
    /* If master mode selected */
    if (pDevInitStruct->CPAL_Mode == CPAL_MODE_MASTER)
    {
  #ifdef CPAL_I2C_10BIT_ADDR_MODE
      /* If 10 Bit addressing mode */
      if (pDevInitStruct->pCPAL_I2C_Struct->I2C_AcknowledgedAddress == I2C_AcknowledgedAddress_10bit)
      {
        /* Enable 10Bit addressing mode */
        CR2_tmp |= I2C_CR2_ADD10;
      }
  #endif /* CPAL_I2C_10BIT_ADDR_MODE */

      /* Configure slave address */
      CR2_tmp |= (uint32_t)((pDevInitStruct->pCPAL_TransferTx->wAddr1) & 0x000003FF);
    }

  #ifdef CPAL_I2C_MEM_ADDR
    /* If CPAL_OPT_NO_MEM_ADDR is not selected and master mode selected */
    if (((pDevInitStruct->wCPAL_Options & CPAL_OPT_NO_MEM_ADDR) == 0)
        && (pDevInitStruct->CPAL_Mode == CPAL_MODE_MASTER ))
    {
      CPAL_LOG("\n\rLOG : I2C Device Master Mem Addr Mode");

      /* Enable reload */
      CR2_tmp |= I2C_CR2_RELOAD;
      
      /* Disable error interrupt to manage error without interrupt */
      __CPAL_I2C_HAL_DISABLE_ERRIT(pDevInitStruct->CPAL_Dev);
      
      /* If 8 Bit register mode */
      if ((pDevInitStruct->wCPAL_Options & CPAL_OPT_16BIT_REG) == 0)
      {
        /* Configure Nbytes */
        CR2_tmp |= (uint32_t)((uint32_t)(1) << 16);

        /* Update CR2 Register */
        __CPAL_I2C_HAL_CR2_UPDATE(pDevInitStruct->CPAL_Dev, CR2_tmp);

        /* Generate start */
        __CPAL_I2C_HAL_START(pDevInitStruct->CPAL_Dev);

        /* Wait until TXIS flag is set or NACK is set */
        __CPAL_I2C_TIMEOUT((__CPAL_I2C_HAL_GET_TXIS(pDevInitStruct->CPAL_Dev) || __CPAL_I2C_HAL_GET_NACK(pDevInitStruct->CPAL_Dev)), CPAL_I2C_TIMEOUT_TXIS);
       
        /* If acknowledge failure detected generate stop and abort communication */
        if (__CPAL_I2C_HAL_GET_NACK(pDevInitStruct->CPAL_Dev))
        {
          /* Generate stop if autoend option is disabled */
          if((CPAL_I2C_DEVICE[(pDevInitStruct->CPAL_Dev)]->CR2 & I2C_CR2_AUTOEND) != I2C_CR2_AUTOEND)
          {
            /* Generate stop */
            __CPAL_I2C_HAL_STOP(pDevInitStruct->CPAL_Dev);
          }
          
          /* Clear NACK flag */
          __CPAL_I2C_HAL_CLEAR_NACK(pDevInitStruct->CPAL_Dev);
          
          /* Wait until STOP flag is set */
          __CPAL_I2C_TIMEOUT(__CPAL_I2C_HAL_GET_STOP(pDevInitStruct->CPAL_Dev), CPAL_I2C_TIMEOUT_BUSY);
          
          /* Clear STOP flag */
          __CPAL_I2C_HAL_CLEAR_STOP(pDevInitStruct->CPAL_Dev);
          
          /* Update wCPAL_DevError */
          pDevInitStruct->wCPAL_DevError = CPAL_I2C_ERR_AF;
          
          /* Set CPAL_State to CPAL_STATE_ERROR */
          pDevInitStruct->CPAL_State = CPAL_STATE_ERROR;
                
          return CPAL_FAIL;
        }

        /* Send register address */
        __CPAL_I2C_HAL_SEND(pDevInitStruct->CPAL_Dev, (uint8_t)(pDevInitStruct->pCPAL_TransferTx->wAddr2));
      }
    #ifdef CPAL_16BIT_REG_OPTION
      /* If 16 Bit register mode */
      else
      {
        /* Configure Nbytes */
        CR2_tmp |= (uint32_t)((uint32_t)(2) << 16);

        /* Update CR2 Register */
        __CPAL_I2C_HAL_CR2_UPDATE(pDevInitStruct->CPAL_Dev, CR2_tmp);

        /* Generate start */
        __CPAL_I2C_HAL_START(pDevInitStruct->CPAL_Dev);

        /* Wait until TXIS flag is set or NACK is set */
        __CPAL_I2C_TIMEOUT((__CPAL_I2C_HAL_GET_TXIS(pDevInitStruct->CPAL_Dev) || __CPAL_I2C_HAL_GET_NACK(pDevInitStruct->CPAL_Dev)), CPAL_I2C_TIMEOUT_TXIS);
       
        /* If acknowledge failure detected generate stop and abort communication */
        if (__CPAL_I2C_HAL_GET_NACK(pDevInitStruct->CPAL_Dev))
        {
          /* Generate stop if autoend option is disabled */
          if((CPAL_I2C_DEVICE[(pDevInitStruct->CPAL_Dev)]->CR2 & I2C_CR2_AUTOEND) != I2C_CR2_AUTOEND)
          {
            /* Generate stop */
            __CPAL_I2C_HAL_STOP(pDevInitStruct->CPAL_Dev);
          }
          
          /* Clear NACK flag */
          __CPAL_I2C_HAL_CLEAR_NACK(pDevInitStruct->CPAL_Dev);
          
          /* Wait until STOP flag is set */
          __CPAL_I2C_TIMEOUT(__CPAL_I2C_HAL_GET_STOP(pDevInitStruct->CPAL_Dev), CPAL_I2C_TIMEOUT_BUSY);
          
          /* Clear STOP flag */
          __CPAL_I2C_HAL_CLEAR_STOP(pDevInitStruct->CPAL_Dev);
          
          /* Update wCPAL_DevError */
          pDevInitStruct->wCPAL_DevError = CPAL_I2C_ERR_AF;
          
          /* Set CPAL_State to CPAL_STATE_ERROR */
          pDevInitStruct->CPAL_State = CPAL_STATE_ERROR;
                
          return CPAL_FAIL;
        }

        /* Send register address (MSB) */
        __CPAL_I2C_HAL_SEND(pDevInitStruct->CPAL_Dev, (uint8_t)(((pDevInitStruct->pCPAL_TransferTx->wAddr2)& 0xFF00) >> 8));
        
        /* Wait until TXIS flag is set or NACK is set */
        __CPAL_I2C_TIMEOUT((__CPAL_I2C_HAL_GET_TXIS(pDevInitStruct->CPAL_Dev) || __CPAL_I2C_HAL_GET_NACK(pDevInitStruct->CPAL_Dev)), CPAL_I2C_TIMEOUT_TXIS);
       
        /* If acknowledge failure detected generate stop and abort communication */
        if (__CPAL_I2C_HAL_GET_NACK(pDevInitStruct->CPAL_Dev))
        {
          /* Generate stop if autoend option is disabled */
          if((CPAL_I2C_DEVICE[(pDevInitStruct->CPAL_Dev)]->CR2 & I2C_CR2_AUTOEND) != I2C_CR2_AUTOEND)
          {
            /* Generate stop */
            __CPAL_I2C_HAL_STOP(pDevInitStruct->CPAL_Dev);
          }
          
          /* Clear NACK flag */
          __CPAL_I2C_HAL_CLEAR_NACK(pDevInitStruct->CPAL_Dev);
          
          /* Wait until STOP flag is set */
          __CPAL_I2C_TIMEOUT(__CPAL_I2C_HAL_GET_STOP(pDevInitStruct->CPAL_Dev), CPAL_I2C_TIMEOUT_BUSY);
          
          /* Clear STOP flag */
          __CPAL_I2C_HAL_CLEAR_STOP(pDevInitStruct->CPAL_Dev);
          
          /* Update wCPAL_DevError */
          pDevInitStruct->wCPAL_DevError = CPAL_I2C_ERR_AF;
          
          /* Set CPAL_State to CPAL_STATE_ERROR */
          pDevInitStruct->CPAL_State = CPAL_STATE_ERROR;
                
          return CPAL_FAIL;
        }

        /* Send register address (LSB) */
        __CPAL_I2C_HAL_SEND(pDevInitStruct->CPAL_Dev, (uint8_t)((pDevInitStruct->pCPAL_TransferTx->wAddr2)& 0x00FF));
      }
    #endif /* CPAL_16BIT_REG_OPTION */

      /* If I2C ERR Interrupt Option Bit not selected */
      if ((pDevInitStruct->wCPAL_Options & CPAL_OPT_I2C_ERRIT_DISABLE) == 0)
      {            
        /* Enable I2C Error Interrupts */
        __CPAL_I2C_HAL_ENABLE_ERRIT(pDevInitStruct->CPAL_Dev);
      }
      
      /* Wait until TCR flag is set */
      __CPAL_I2C_TIMEOUT(__CPAL_I2C_HAL_GET_TCR(pDevInitStruct->CPAL_Dev), CPAL_I2C_TIMEOUT_TCR);

      /* Set Nbytes to zero */
      CR2_tmp &= ~I2C_CR2_NBYTES;
    }
  #endif /* CPAL_I2C_MEM_ADDR */
#endif /* CPAL_I2C_MASTER_MODE */

    /* Update CPAL_State to CPAL_STATE_BUSY_TX */
    pDevInitStruct->CPAL_State = CPAL_STATE_BUSY_TX;
    CPAL_LOG("\n\rLOG : I2C Device Busy TX");

#ifdef CPAL_I2C_MASTER_MODE
    /* If master mode selected */
    if (pDevInitStruct->CPAL_Mode == CPAL_MODE_MASTER)
    {
      CPAL_LOG("\n\rLOG : I2C Device Master");

      /* If automatic end mode is selected */
      if ((pDevInitStruct->wCPAL_Options & CPAL_OPT_I2C_AUTOMATIC_END) != 0)
      {
        /* Enable automatic end mode */
        CR2_tmp |= I2C_CR2_AUTOEND;
      }

      /* If number of data is equal or lower than 255 bytes */
      if (pDevInitStruct->pCPAL_TransferTx->wNumData <= 0xFF )
      {
        /* Update Num_Data */
        Num_Data = pDevInitStruct->pCPAL_TransferTx->wNumData;

        /* Set Nbytes to wNumData */
        CR2_tmp |= (uint32_t)((uint32_t)(Num_Data) << 16);

        /* Disable reload */
        CR2_tmp &= ~I2C_CR2_RELOAD;
      }
      /* If number of data is greater than 255 bytes */
      else
      {
        /* Set Nbytes to 255 */
        CR2_tmp |= (uint32_t)((uint32_t)(255) << 16);

        /* Enable reload */
        CR2_tmp |= I2C_CR2_RELOAD;
      }

      /* If CPAL_OPT_NO_MEM_ADDR is selected */
      if ((pDevInitStruct->wCPAL_Options & CPAL_OPT_NO_MEM_ADDR) != 0)
      {
        /* Generate start */
        CR2_tmp |= I2C_CR2_START;
      }

      /* Update CR2 Register */
      __CPAL_I2C_HAL_CR2_UPDATE(pDevInitStruct->CPAL_Dev, CR2_tmp);

      /* If interrupt programming model */
      if (pDevInitStruct->CPAL_ProgModel == CPAL_PROGMODEL_INTERRUPT)
      {
        CPAL_LOG("\n\rLOG : I2C Device IT Enabled");

        /* Enable master interrupts */
        __CPAL_I2C_HAL_ENABLE_MASTER_TXIT(pDevInitStruct->CPAL_Dev);
      }
      /* If DMA programming model */
      else
      {
        CPAL_LOG("\n\rLOG : I2C Device DMA TX Enabled");

        /* Enable TX DMA request */
        __CPAL_I2C_HAL_ENABLE_TXDMAREQ(pDevInitStruct->CPAL_Dev);

        /* Enable master interrupts */
        __CPAL_I2C_HAL_ENABLE_MASTER_IT(pDevInitStruct->CPAL_Dev);
      }
    }
    /* If slave mode selected */
    else
#endif /* CPAL_I2C_MASTER_MODE */
    {
#ifdef CPAL_I2C_SLAVE_MODE
      CPAL_LOG("\n\rLOG : I2C Device Slave");

      /* If NACK Slave Own Address option bit selected */
      if ((pDevInitStruct->wCPAL_Options & CPAL_OPT_I2C_NACK_ADD) != 0)
      {
        /* Enable Acknowledgement of Own address */
        __CPAL_I2C_HAL_ENABLE_DEV(pDevInitStruct->CPAL_Dev);
      }

      /* If interrupt programming model */
      if (pDevInitStruct->CPAL_ProgModel == CPAL_PROGMODEL_INTERRUPT)
      {
        CPAL_LOG("\n\rLOG : I2C Device IT Enabled");

        /* Enable slave interrupts */
        __CPAL_I2C_HAL_ENABLE_SLAVE_TXIT(pDevInitStruct->CPAL_Dev);
      }
      /* If DMA programming model */
      else
      {
        CPAL_LOG("\n\rLOG : I2C Device DMA TX Enabled");

        /* Enable TX DMA request */
        __CPAL_I2C_HAL_ENABLE_TXDMAREQ(pDevInitStruct->CPAL_Dev);

        /* Enable slave interrupts */
        __CPAL_I2C_HAL_ENABLE_SLAVE_IT(pDevInitStruct->CPAL_Dev);
      }
#endif /* CPAL_I2C_SLAVE_MODE */
    }
  }
  return CPAL_PASS;
}



/**
  * @brief  Allows to receive a data or a buffer of data through the peripheral
  *         from a selected device in a selected location address.
  * @param  pDevInitStruct: Pointer to the peripheral configuration structure.
  * @retval CPAL_PASS or CPAL_FAIL.
  */
uint32_t CPAL_I2C_Read(CPAL_InitTypeDef* pDevInitStruct)
{
  CR2_tmp = 0;

  /* Check I2C State:
  - If busy       --> exit Read operation
  - If disabled   --> exit Read operation
  - If error      --> exit Read operation
  - If ready      -->
                      *- Update CPAL_State to CPAL_STATE_BUSY
                      *- If CPAL_OPT_I2C_NOSTOP_MODE is not selected :
                         - Check if device is busy
                      *- Update CPAL_State to CPAL_STATE_READY_RX
                      *- If DMA Prog Model :
                         - Configure and enable DMA
                      *- If Master mode :
                         - If 10Bit Mode : Enable ADD10
                         - Configure Slave address
                      *- If Memory Address mode (master)
                         - Send target and memory address
                      *- Update CPAL_State to CPAL_STATE_BUSY_RX
                      *- If Master mode :
                         - Configure HEADR10, AUTOEND, RELOAD and NBYTES
                         - If Interrupt Prog Model :
                             - Generate start and enable interrupts
                         - If DMA Prog Model :
                             - Enable RX DMA request
                             - Generate start and enable interrupts
                      *- If Slave mode :
                         - If Interrupt Prog Model :
                             - Enable interrupts
                         - If DMA Prog Model :
                             - Enable RX DMA request
                             - Enable interrupts                             */

  CPAL_LOG("\n\r\n\rLOG <CPAL_I2C_Read> : I2C Device Perform Read OP");

  /* If Device is Busy (a transaction is still on going) Exit Read function */
  if (((pDevInitStruct->CPAL_State & CPAL_STATE_BUSY) != 0)
      || (pDevInitStruct->CPAL_State == CPAL_STATE_READY_TX)
        || (pDevInitStruct->CPAL_State == CPAL_STATE_READY_RX))
  {
    CPAL_LOG("\n\rERROR : I2C Device Busy");

    return CPAL_FAIL;
  }
  /* If CPAL_State is CPAL_STATE_DISABLED (device is not initialized) Exit Read function */
  else if (pDevInitStruct->CPAL_State == CPAL_STATE_DISABLED)
  {
    CPAL_LOG("\n\rERROR : I2C Device Not Initialized");

    return CPAL_FAIL;
  }
  /* If CPAL_State is CPAL_STATE_ERROR (Error occurred ) */
  else if (pDevInitStruct->CPAL_State == CPAL_STATE_ERROR)
  {
    CPAL_LOG("\n\rERROR : I2C Device Error");

    return CPAL_FAIL;
  }
  /* If CPAL_State is CPAL_STATE_READY */
  else
  {
    /* Update CPAL_State to CPAL_STATE_BUSY */
    pDevInitStruct->CPAL_State = CPAL_STATE_BUSY;

    /* If CPAL_OPT_I2C_NOSTOP_MODE is not selected */
    if ((pDevInitStruct->wCPAL_Options & CPAL_OPT_I2C_NOSTOP_MODE) == 0)
    {
      /* Wait until BUSY flag is reset */
      __CPAL_I2C_TIMEOUT(!(__CPAL_I2C_HAL_GET_BUSY(pDevInitStruct->CPAL_Dev)), CPAL_I2C_TIMEOUT_BUSY);
    }

    /* Update CPAL_State to CPAL_STATE_READY_RX */
    pDevInitStruct->CPAL_State = CPAL_STATE_READY_RX;
    CPAL_LOG("\n\rLOG : I2C Device Ready RX");

#ifdef CPAL_I2C_DMA_PROGMODEL
    /* If DMA programming model */
    if (pDevInitStruct->CPAL_ProgModel == CPAL_PROGMODEL_DMA)
    {
      /* Configure and enable RX DMA channel */
      I2C_Enable_DMA(pDevInitStruct, CPAL_DIRECTION_RX);
    }
#endif /* CPAL_I2C_DMA_PROGMODEL */

#ifdef CPAL_I2C_MASTER_MODE
    /* If master mode selected */
    if (pDevInitStruct->CPAL_Mode == CPAL_MODE_MASTER)
    {
  #ifdef CPAL_I2C_10BIT_ADDR_MODE
      /* If 10 Bit addressing mode */
      if (pDevInitStruct->pCPAL_I2C_Struct->I2C_AcknowledgedAddress == I2C_AcknowledgedAddress_10bit)
      {
        /* Enable 10Bit addressing mode */
        CR2_tmp |= I2C_CR2_ADD10;
      }
  #endif /* CPAL_I2C_10BIT_ADDR_MODE */

      /* Configure slave address */
      CR2_tmp |= (uint32_t)((pDevInitStruct->pCPAL_TransferRx->wAddr1) & 0x000003FF);
    }

  #ifdef CPAL_I2C_MEM_ADDR
    /* If No Memory Address option bit is not selected and master mode selected */
    if (((pDevInitStruct->wCPAL_Options & CPAL_OPT_NO_MEM_ADDR) == 0)
        && (pDevInitStruct->CPAL_Mode == CPAL_MODE_MASTER ))
    {
      CPAL_LOG("\n\rLOG : I2C Device Master Mem Addr Mode");

      /* Disable error interrupt to manage error without interrupt */
      __CPAL_I2C_HAL_DISABLE_ERRIT(pDevInitStruct->CPAL_Dev);
      
      /* If 8 Bit register mode */
      if ((pDevInitStruct->wCPAL_Options & CPAL_OPT_16BIT_REG) == 0)
      {
        /* Configure Nbytes */
        CR2_tmp |= (uint32_t)((uint32_t)(1) << 16);

        /* Update CR2 Register */
        __CPAL_I2C_HAL_CR2_UPDATE(pDevInitStruct->CPAL_Dev, CR2_tmp);

        /* Generate start */
        __CPAL_I2C_HAL_START(pDevInitStruct->CPAL_Dev);

         /* Wait until TXIS flag is set or NACK is set */
        __CPAL_I2C_TIMEOUT((__CPAL_I2C_HAL_GET_TXIS(pDevInitStruct->CPAL_Dev) || __CPAL_I2C_HAL_GET_NACK(pDevInitStruct->CPAL_Dev)), CPAL_I2C_TIMEOUT_TXIS);
       
        /* If acknowledge failure detected generate stop and abort communication */
        if (__CPAL_I2C_HAL_GET_NACK(pDevInitStruct->CPAL_Dev))
        {
          /* Generate stop if autoend option is disabled */
          if((CPAL_I2C_DEVICE[(pDevInitStruct->CPAL_Dev)]->CR2 & I2C_CR2_AUTOEND) != I2C_CR2_AUTOEND)
          {
            /* Generate stop */
            __CPAL_I2C_HAL_STOP(pDevInitStruct->CPAL_Dev);
          }
          
          /* Clear NACK flag */
          __CPAL_I2C_HAL_CLEAR_NACK(pDevInitStruct->CPAL_Dev);
          
          /* Wait until STOP flag is set */
          __CPAL_I2C_TIMEOUT(__CPAL_I2C_HAL_GET_STOP(pDevInitStruct->CPAL_Dev), CPAL_I2C_TIMEOUT_BUSY);
          
          /* Clear STOP flag */
          __CPAL_I2C_HAL_CLEAR_STOP(pDevInitStruct->CPAL_Dev);
          
          /* Update wCPAL_DevError */
          pDevInitStruct->wCPAL_DevError = CPAL_I2C_ERR_AF;
          
          /* Set CPAL_State to CPAL_STATE_ERROR */
          pDevInitStruct->CPAL_State = CPAL_STATE_ERROR;
                
          return CPAL_FAIL;
        }
        
        /* Send register address */
        __CPAL_I2C_HAL_SEND(pDevInitStruct->CPAL_Dev, (uint8_t)(pDevInitStruct->pCPAL_TransferRx->wAddr2));
      }
    #ifdef CPAL_16BIT_REG_OPTION
      /* If 16 Bit register mode */
      else
      {
        /* Configure Nbytes */
        CR2_tmp |= (uint32_t)((uint32_t)(2) << 16);

        /* Update CR2 Register */
        __CPAL_I2C_HAL_CR2_UPDATE(pDevInitStruct->CPAL_Dev, CR2_tmp);

        /* Generate start */
        __CPAL_I2C_HAL_START(pDevInitStruct->CPAL_Dev);

   /* Wait until TXIS flag is set or NACK is set */
        __CPAL_I2C_TIMEOUT((__CPAL_I2C_HAL_GET_TXIS(pDevInitStruct->CPAL_Dev) || __CPAL_I2C_HAL_GET_NACK(pDevInitStruct->CPAL_Dev)), CPAL_I2C_TIMEOUT_TXIS);
       
        /* If acknowledge failure detected generate stop and abort communication */
        if (__CPAL_I2C_HAL_GET_NACK(pDevInitStruct->CPAL_Dev))
        {
          /* Generate stop if autoend option is disabled */
          if((CPAL_I2C_DEVICE[(pDevInitStruct->CPAL_Dev)]->CR2 & I2C_CR2_AUTOEND) != I2C_CR2_AUTOEND)
          {
            /* Generate stop */
            __CPAL_I2C_HAL_STOP(pDevInitStruct->CPAL_Dev);
          }
          
          /* Clear NACK flag */
          __CPAL_I2C_HAL_CLEAR_NACK(pDevInitStruct->CPAL_Dev);
          
          /* Wait until STOP flag is set */
          __CPAL_I2C_TIMEOUT(__CPAL_I2C_HAL_GET_STOP(pDevInitStruct->CPAL_Dev), CPAL_I2C_TIMEOUT_BUSY);
          
          /* Clear STOP flag */
          __CPAL_I2C_HAL_CLEAR_STOP(pDevInitStruct->CPAL_Dev);
          
          /* Update wCPAL_DevError */
          pDevInitStruct->wCPAL_DevError = CPAL_I2C_ERR_AF;
          
          /* Set CPAL_State to CPAL_STATE_ERROR */
          pDevInitStruct->CPAL_State = CPAL_STATE_ERROR;
                
          return CPAL_FAIL;
        }

        /* Send register address (MSB) */
        __CPAL_I2C_HAL_SEND(pDevInitStruct->CPAL_Dev, (uint8_t)(((pDevInitStruct->pCPAL_TransferRx->wAddr2)& 0xFF00) >> 8));

       /* Wait until TXIS flag is set or NACK is set */
        __CPAL_I2C_TIMEOUT((__CPAL_I2C_HAL_GET_TXIS(pDevInitStruct->CPAL_Dev) || __CPAL_I2C_HAL_GET_NACK(pDevInitStruct->CPAL_Dev)), CPAL_I2C_TIMEOUT_TXIS);
       
        /* If acknowledge failure detected generate stop and abort communication */
        if (__CPAL_I2C_HAL_GET_NACK(pDevInitStruct->CPAL_Dev))
        {
          /* Generate stop if autoend option is disabled */
          if((CPAL_I2C_DEVICE[(pDevInitStruct->CPAL_Dev)]->CR2 & I2C_CR2_AUTOEND) != I2C_CR2_AUTOEND)
          {
            /* Generate stop */
            __CPAL_I2C_HAL_STOP(pDevInitStruct->CPAL_Dev);
          }
          
          /* Clear NACK flag */
          __CPAL_I2C_HAL_CLEAR_NACK(pDevInitStruct->CPAL_Dev);
          
          /* Wait until STOP flag is set */
          __CPAL_I2C_TIMEOUT(__CPAL_I2C_HAL_GET_STOP(pDevInitStruct->CPAL_Dev), CPAL_I2C_TIMEOUT_BUSY);
          
          /* Clear STOP flag */
          __CPAL_I2C_HAL_CLEAR_STOP(pDevInitStruct->CPAL_Dev);
          
          /* Update wCPAL_DevError */
          pDevInitStruct->wCPAL_DevError = CPAL_I2C_ERR_AF;
          
          /* Set CPAL_State to CPAL_STATE_ERROR */
          pDevInitStruct->CPAL_State = CPAL_STATE_ERROR;
                
          return CPAL_FAIL;
        }
        
        /* Send register address (LSB) */
        __CPAL_I2C_HAL_SEND(pDevInitStruct->CPAL_Dev, (uint8_t)((pDevInitStruct->pCPAL_TransferRx->wAddr2)& 0x00FF));
      }
    #endif /* CPAL_16BIT_REG_OPTION */
      
      /* If I2C ERR Interrupt Option Bit not selected */
      if ((pDevInitStruct->wCPAL_Options & CPAL_OPT_I2C_ERRIT_DISABLE) == 0)
      {            
        /* Enable I2C Error Interrupts */
        __CPAL_I2C_HAL_ENABLE_ERRIT(pDevInitStruct->CPAL_Dev);
      }
      /* Wait until TC flag is set */
      __CPAL_I2C_TIMEOUT(__CPAL_I2C_HAL_GET_TC(pDevInitStruct->CPAL_Dev), CPAL_I2C_TIMEOUT_TC);

      /* Set Nbytes to zero */
      CR2_tmp &= ~I2C_CR2_NBYTES;
    }
  #endif /* CPAL_I2C_MEM_ADDR */
#endif /* CPAL_I2C_MASTER_MODE */

    /* Update CPAL_State to CPAL_STATE_BUSY_RX */
    pDevInitStruct->CPAL_State = CPAL_STATE_BUSY_RX;
    CPAL_LOG("\n\rLOG : I2C Device Busy RX");

#ifdef CPAL_I2C_MASTER_MODE
    /* If master mode selected */
    if (pDevInitStruct->CPAL_Mode == CPAL_MODE_MASTER)
    {
      CPAL_LOG("\n\rLOG : I2C Device Master");

      /* Enable transfer request */
      CR2_tmp |= I2C_CR2_RD_WRN;

  #ifdef CPAL_I2C_10BIT_ADDR_MODE
      /* If 10 Bit addressing mode */
      if (pDevInitStruct->pCPAL_I2C_Struct->I2C_AcknowledgedAddress == I2C_AcknowledgedAddress_10bit)
      {
        /* If CPAL_OPT_NO_MEM_ADDR is not selected and CPAL_OPT_I2C_10BIT_HEADR option enabled */
        if (((pDevInitStruct->wCPAL_Options & CPAL_OPT_I2C_10BIT_HEADR) != 0) || ((pDevInitStruct->wCPAL_Options & CPAL_OPT_NO_MEM_ADDR) == 0))
        {
          /* Disable 10Bit addressing complete sequence for Read */
          CR2_tmp |= I2C_CR2_HEAD10R;
        }
      }
  #endif /* CPAL_I2C_10BIT_ADDR_MODE */

      /* If automatic end mode is selected */
      if ((pDevInitStruct->wCPAL_Options & CPAL_OPT_I2C_AUTOMATIC_END) != 0)
      {
        /* Enable automatic end mode */
        CR2_tmp |= I2C_CR2_AUTOEND;
      }

      /* If number of data is equal or lower than 255 bytes */
      if (pDevInitStruct->pCPAL_TransferRx->wNumData <= 0xFF )
      {
        /* Update Num_Data */
        Num_Data = pDevInitStruct->pCPAL_TransferRx->wNumData;

        /* Set Nbytes to wNumData */
        CR2_tmp |= (uint32_t)((uint32_t)(Num_Data) << 16);

        /* Disable reload */
        CR2_tmp &= ~I2C_CR2_RELOAD;
      }
      /* If number of data is greater than 255 bytes */
      else
      {
        /* Set Nbytes to wNumData */
        CR2_tmp |= (uint32_t)((uint32_t)(255) << 16);

        /* Enable reload */
        CR2_tmp |= I2C_CR2_RELOAD;
      }

      /* Generate start */
      CR2_tmp |= I2C_CR2_START;

      /* If interrupt programming model */
      if (pDevInitStruct->CPAL_ProgModel == CPAL_PROGMODEL_INTERRUPT)
      {
        CPAL_LOG("\n\rLOG : I2C Device IT Enabled");

        /* Update CR2 Register */
        __CPAL_I2C_HAL_CR2_UPDATE(pDevInitStruct->CPAL_Dev, CR2_tmp);

        /* Enable master interrupts */
        __CPAL_I2C_HAL_ENABLE_MASTER_RXIT(pDevInitStruct->CPAL_Dev);
      }
      /* If DMA programming model */
      else
      {
        CPAL_LOG("\n\rLOG : I2C Device DMA RX Enabled");

        /* Enable RX DMA request */
        __CPAL_I2C_HAL_ENABLE_RXDMAREQ(pDevInitStruct->CPAL_Dev);

        /* Update CR2 Register */
        __CPAL_I2C_HAL_CR2_UPDATE(pDevInitStruct->CPAL_Dev, CR2_tmp);

        /* Enable master interrupt */
        __CPAL_I2C_HAL_ENABLE_MASTER_IT(pDevInitStruct->CPAL_Dev);
      }
    }
    /* If slave mode selected */
    else
#endif /* CPAL_I2C_MASTER_MODE */
    {
#ifdef CPAL_I2C_SLAVE_MODE
      CPAL_LOG("\n\rLOG : I2C Device Slave");

      /* If NACK Slave Own Address option bit selected */
      if ((pDevInitStruct->wCPAL_Options & CPAL_OPT_I2C_NACK_ADD) != 0)
      {
        /* Enable Acknowledgement of Own address */
        __CPAL_I2C_HAL_ENABLE_DEV(pDevInitStruct->CPAL_Dev);
      }

      /* If interrupt programming model */
      if (pDevInitStruct->CPAL_ProgModel == CPAL_PROGMODEL_INTERRUPT)
      {
        CPAL_LOG("\n\rLOG : I2C Device IT Enabled");

        /* Enable slave interrupts */
        __CPAL_I2C_HAL_ENABLE_SLAVE_RXIT(pDevInitStruct->CPAL_Dev);
      }
      /* If DMA programming model */
      else
      {
        CPAL_LOG("\n\rLOG : I2C Device DMA RX Enabled");

        /* Enable RX DMA request */
        __CPAL_I2C_HAL_ENABLE_RXDMAREQ(pDevInitStruct->CPAL_Dev);

        /* Enable slave interrupt */
        __CPAL_I2C_HAL_ENABLE_SLAVE_IT(pDevInitStruct->CPAL_Dev);
      }
#endif /* CPAL_I2C_SLAVE_MODE */
    }
  }
  return CPAL_PASS;
}
#endif /* CPAL_I2C_MASTER_MODE || ! CPAL_I2C_LISTEN_MODE */

#if defined (CPAL_I2C_LISTEN_MODE) && defined (CPAL_I2C_SLAVE_MODE)
/**
  * @brief  Allows slave device to start a communication without knowing in advance
  *         the nature of the operation (read or write). Slave waits until it receive
  *         its own address.CPAL_I2C_SLAVE_READ_UserCallback is called for a read request
  *         and CPAL_I2C_SLAVE_WRITE_UserCallback for a write request in I2C_SLAVE_ADDR_Handle.
  *         User must implement inorder to configure DMA, interrupts and transfer parameters.
  * @param  pDevInitStruct: Pointer to the peripheral configuration structure.
  * @retval CPAL_PASS or CPAL_FAIL.
  */
uint32_t CPAL_I2C_Listen(CPAL_InitTypeDef* pDevInitStruct)
{
  /* Check I2C State:
  - If busy       --> exit operation
  - If disabled   --> exit operation
  - If error      --> exit operation
  - If ready      -->
          - Enable Event Interrupt                                               */

  CPAL_LOG("\n\r\n\rLOG <CPAL_I2C_Listen> : I2C Device in listen mode");

  /* If Device is Busy (a transaction is still on going) Exit function */
  if (((pDevInitStruct->CPAL_State & CPAL_STATE_BUSY) != 0)
      || (pDevInitStruct->CPAL_State == CPAL_STATE_READY_TX)
        || (pDevInitStruct->CPAL_State == CPAL_STATE_READY_RX))
  {
    CPAL_LOG("\n\rERROR : I2C Device Busy");

    return CPAL_FAIL;
  }
  /* If CPAL_State is CPAL_STATE_DISABLED (device is not initialized) Exit function */
  else if (pDevInitStruct->CPAL_State == CPAL_STATE_DISABLED)
  {
    CPAL_LOG("\n\rERROR : I2C Device Not Initialized");

    return CPAL_FAIL;
  }
  /* If CPAL_State is CPAL_STATE_ERROR (Error occurred ) */
  else if (pDevInitStruct->CPAL_State == CPAL_STATE_ERROR)
  {
    CPAL_LOG("\n\rERROR : I2C Device Error");

    return CPAL_FAIL;
  }
  /* If CPAL_State is CPAL_STATE_READY */
  else
  {

    /* If NACK Slave Own Address option bit selected */
    if ((pDevInitStruct->wCPAL_Options & CPAL_OPT_I2C_NACK_ADD) != 0)
    {
      /* Enable Acknowledgement of Own address */
      __CPAL_I2C_HAL_ENABLE_DEV(pDevInitStruct->CPAL_Dev);
    }

    /* Set device to slave mode */
    pDevInitStruct->CPAL_Mode = CPAL_MODE_SLAVE;

    /* Update CPAL_State to CPAL_STATE_BUSY */
    pDevInitStruct->CPAL_State = CPAL_STATE_BUSY;

    CPAL_LOG("\n\rLOG : I2C Device EVT IT Enabled");

    /* Enable Slave Interrupts*/
    __CPAL_I2C_HAL_ENABLE_SLAVE_IT(pDevInitStruct->CPAL_Dev);
  }

  return CPAL_PASS;
}
#endif /* CPAL_I2C_LISTEN_MODE && CPAL_I2C_SLAVE_MODE */

/**
  * @brief  Wait until target device is ready for communication (This function is
  *         used with Memory devices).
  * @param  pDevInitStruct: Pointer to the peripheral configuration structure.
  * @retval CPAL_PASS or CPAL_FAIL.
  */
uint32_t CPAL_I2C_IsDeviceReady(CPAL_InitTypeDef* pDevInitStruct)
{
  CR2_tmp = 0;

  CPAL_LOG("\n\r\n\rLOG <CPAL_I2C_IsDeviceReady> : Wait until I2C Device is Ready");

  /* Set CPAL_State to CPAL_STATE_BUSY */
  pDevInitStruct->CPAL_State = CPAL_STATE_BUSY;

  /* Disable I2Cx device */
  __CPAL_I2C_HAL_DISABLE_DEV(pDevInitStruct->CPAL_Dev);

  /* Enable I2Cx device */
  __CPAL_I2C_HAL_ENABLE_DEV(pDevInitStruct->CPAL_Dev);

  /* Disable interrupts */
  __CPAL_I2C_HAL_DISABLE_ALLIT(pDevInitStruct->CPAL_Dev);

  /* Configure slave address */
  CR2_tmp |= (uint32_t)((pDevInitStruct->pCPAL_TransferTx->wAddr1) & 0x000003FF) | I2C_CR2_AUTOEND;

  /* Update CR2 Register */
  __CPAL_I2C_HAL_CR2_UPDATE(pDevInitStruct->CPAL_Dev, CR2_tmp);

  /* Generate Start */
  __CPAL_I2C_HAL_START(pDevInitStruct->CPAL_Dev);

  /* Set 35ms timeout */
  pDevInitStruct->wCPAL_Timeout = CPAL_I2C_TIMEOUT_MIN + 35;

  /* No need to Check TC flag, with AUTOEND mode the stop is automatically generated */
  /* Wait until STOPF flag is set or a NACK flag is set*/
  while((__CPAL_I2C_HAL_GET_STOP(pDevInitStruct->CPAL_Dev) == RESET) && (__CPAL_I2C_HAL_GET_NACK(pDevInitStruct->CPAL_Dev) == RESET));

  /* Reinitialize Timeout Value to default */
  pDevInitStruct->wCPAL_Timeout = CPAL_I2C_TIMEOUT_DEFAULT;

  /* Check if the NACKF flag has not been set */
  if (__CPAL_I2C_HAL_GET_NACK(pDevInitStruct->CPAL_Dev) != RESET)
  {
    /* Clear NACK flag */
    __CPAL_I2C_HAL_CLEAR_NACK(pDevInitStruct->CPAL_Dev);

    /* Clear Stop flag */
    __CPAL_I2C_HAL_CLEAR_STOP(pDevInitStruct->CPAL_Dev);

    /* Enable error interrupt */
    __CPAL_I2C_HAL_ENABLE_ERRIT(pDevInitStruct->CPAL_Dev);

    /* Set CPAL_State to CPAL_STATE_READY */
    pDevInitStruct->CPAL_State = CPAL_STATE_READY;

    return CPAL_FAIL;
  }
  else
  {
    /* Clear Stop flag */
    __CPAL_I2C_HAL_CLEAR_STOP(pDevInitStruct->CPAL_Dev);

    /* Enable error interrupt */
    __CPAL_I2C_HAL_ENABLE_ERRIT(pDevInitStruct->CPAL_Dev);

    /* Set CPAL_State to CPAL_STATE_READY */
    pDevInitStruct->CPAL_State = CPAL_STATE_READY;

    CPAL_LOG("\n\rLOG : I2C Target device Ready");

    return CPAL_PASS;
  }
}


/*================== CPAL_I2C_Interrupt_Handler ==================*/

/**
  * @brief  This function handles I2C interrupt request for preparing communication
  *         and for transfer phase in case of using Interrupt Programming Model.
  * @param  pDevInitStruct: Pointer to the peripheral configuration structure.
  * @retval CPAL_PASS.
  */
uint32_t CPAL_I2C_EV_IRQHandler( CPAL_InitTypeDef* pDevInitStruct)
{
  __IO uint32_t I2CFlagStatus = 0x00000000;

  /* Read I2C status registers (ISR) */
  I2CFlagStatus = __CPAL_I2C_HAL_GET_EVENT(pDevInitStruct->CPAL_Dev);

#ifdef CPAL_I2C_MASTER_MODE
  /*----------------------------------------------------------------------------------------------*/
  /*---------------------------------- If Master Mode selected ----------------------------------*/
  if (pDevInitStruct->CPAL_Mode == CPAL_MODE_MASTER)
  {
  #ifdef CPAL_I2C_IT_PROGMODEL
    /*----------------------------------------*/
    /*------------- If TXIS event ------------*/
    if (((I2CFlagStatus & CPAL_I2C_EVT_TXIS) != 0) && (pDevInitStruct->CPAL_State == CPAL_STATE_BUSY_TX))
    {
      I2C_MASTER_TXIS_Handle(pDevInitStruct);
    }

    /*----------------------------------------*/
    /*------------- If RXNE event ------------*/
    if (((I2CFlagStatus & CPAL_I2C_EVT_RXNE) != 0) && (pDevInitStruct->CPAL_State == CPAL_STATE_BUSY_RX))
    {
      I2C_MASTER_RXNE_Handle(pDevInitStruct);
    }
  #endif /* CPAL_I2C_IT_PROGMODEL */

    /*----------------------------------------*/
    /*-------------- If TCR event ------------*/
    if ((I2CFlagStatus & CPAL_I2C_EVT_TCR) != 0)
    {
      I2C_MASTER_TCR_Handle(pDevInitStruct);
    }

    /*----------------------------------------*/
    /*------------- If TC event --------------*/
    if ((I2CFlagStatus & CPAL_I2C_EVT_TC ) != 0)
    {
      I2C_MASTER_TC_Handle(pDevInitStruct);
    }

    /*----------------------------------------*/
    /*------------- If STOP event ------------*/
    if ((I2CFlagStatus & CPAL_I2C_EVT_STOP) != 0)
    {
      I2C_MASTER_STOP_Handle(pDevInitStruct);
    }

    /*----------------------------------------*/
    /*------------- If NACK event ------------*/
    if((I2CFlagStatus & CPAL_I2C_EVT_NACK ) != 0)
    {
      I2C_MASTER_NACK_Handle(pDevInitStruct);
    }
  }
#endif /* CPAL_I2C_MASTER_MODE */

#ifdef CPAL_I2C_SLAVE_MODE
  /*----------------------------------------------------------------------------------------------*/
  /*---------------------------------- If Slave Mode selected ------------------------------------*/
  if (pDevInitStruct->CPAL_Mode == CPAL_MODE_SLAVE)
  {
    /*----------------------------------------*/
    /*------------- If ADDR event ------------*/
    if ((I2CFlagStatus & CPAL_I2C_EVT_ADDR ) != 0)
    {
      I2C_SLAVE_ADDR_Handle(pDevInitStruct);
    }

 #ifdef CPAL_I2C_IT_PROGMODEL
    /*----------------------------------------*/
    /*------------- If TXIS event ------------*/
    if ((I2CFlagStatus & CPAL_I2C_EVT_TXIS) != 0)
    {
      I2C_SLAVE_TXIS_Handle(pDevInitStruct);
    }

    /*----------------------------------------*/
    /*------------- If RXNE event ------------*/
    if ((I2CFlagStatus & CPAL_I2C_EVT_RXNE) != 0)
    {
      I2C_SLAVE_RXNE_Handle(pDevInitStruct);
    }
 #endif /* CPAL_I2C_IT_PROGMODEL */

    /*----------------------------------------*/
    /*------------- If NACK event ------------*/
    if ((I2CFlagStatus & CPAL_I2C_EVT_NACK) != 0)
    {
      I2C_SLAVE_NACK_Handle(pDevInitStruct);
    }

    /*----------------------------------------*/
    /*------------- If STOP event ------------*/
    if ((I2CFlagStatus & CPAL_I2C_EVT_STOP) != 0)
    {
      I2C_SLAVE_STOP_Handle(pDevInitStruct);
    }
  }
#endif /* CPAL_I2C_SLAVE_MODE */

  return CPAL_PASS;
}


/**
  * @brief  Allows to handle errors occurred during initialization or communication
  *         in order to recover the correct communication status or call specific
  *         user functions.
  * @param  pDevInitStruct: Pointer to the peripheral configuration structure.
  * @retval CPAL_PASS.
  */
uint32_t CPAL_I2C_ER_IRQHandler(CPAL_InitTypeDef* pDevInitStruct)
{
  /* Read error register and affect to wCPAL_DevError */
  pDevInitStruct->wCPAL_DevError = __CPAL_I2C_HAL_GET_ERROR(pDevInitStruct->CPAL_Dev);

  /* Set CPAL_State to CPAL_STATE_ERROR */
  pDevInitStruct->CPAL_State = CPAL_STATE_ERROR;

  CPAL_LOG("\n\r\n\rERROR <CPAL_I2C_ErrorHandler> : I2C Device Error");

  /* If Bus error occurred ---------------------------------------------------*/
  if ((pDevInitStruct->wCPAL_DevError & CPAL_I2C_ERR_BERR) != 0)
  {
    CPAL_LOG("\n\rERROR : I2C Device BERR");

    /* Clear error flag */
    __CPAL_I2C_HAL_CLEAR_BERR(pDevInitStruct->CPAL_Dev);

#ifdef USE_MULTIPLE_ERROR_CALLBACK
    /* Call Bus Error UserCallback */
    CPAL_I2C_BERR_UserCallback(pDevInitStruct->CPAL_Dev);
#endif /* USE_MULTIPLE_ERROR_CALLBACK */
  }

  /* If Arbitration Loss error occurred --------------------------------------*/
  if ((pDevInitStruct->wCPAL_DevError & CPAL_I2C_ERR_ARLO) != 0)
  {
    CPAL_LOG("\n\rERROR : I2C Device ARLO");

    /* Clear error flag */
    __CPAL_I2C_HAL_CLEAR_ARLO(pDevInitStruct->CPAL_Dev);

#ifdef USE_MULTIPLE_ERROR_CALLBACK
    /* Call Arbitration Lost UserCallback */
    CPAL_I2C_ARLO_UserCallback(pDevInitStruct->CPAL_Dev);
#endif /* USE_MULTIPLE_ERROR_CALLBACK */
  }

  /* If Overrun error occurred -----------------------------------------------*/
  if ((pDevInitStruct->wCPAL_DevError & CPAL_I2C_ERR_OVR) != 0)
  {
    CPAL_LOG("\n\rERROR : I2C Device OVR");

    /* No I2C software reset is performed here in order to allow user to get back
    the last data received correctly */

    /* Clear error flag */
    __CPAL_I2C_HAL_CLEAR_OVR(pDevInitStruct->CPAL_Dev);

#ifdef USE_MULTIPLE_ERROR_CALLBACK
    /* Call Overrun error UserCallback */
    CPAL_I2C_OVR_UserCallback(pDevInitStruct->CPAL_Dev);
#endif /* USE_MULTIPLE_ERROR_CALLBACK */
  }

  /* USE_SINGLE_ERROR_CALLBACK is defined in stm32f30x_i2c_cpal_conf.h file */
#ifdef USE_SINGLE_ERROR_CALLBACK
  /* Call Error UserCallback */
  CPAL_I2C_ERR_UserCallback(pDevInitStruct->CPAL_Dev, pDevInitStruct->wCPAL_DevError);
#endif /* USE_SINGLE_ERROR_CALLBACK */

  return CPAL_PASS;
}


#ifdef CPAL_I2C_DMA_PROGMODEL
/**
  * @brief  Handle I2C DMA TX interrupt request when DMA programming Model is
  *         used for data transmission.
  * @param  pDevInitStruct: Pointer to the peripheral configuration structure.
  * @retval CPAL_PASS.
  */
uint32_t CPAL_I2C_DMA_TX_IRQHandler(CPAL_InitTypeDef* pDevInitStruct)
{
  /* Reinitialize timeout value to default (no timeout initiated) */
  pDevInitStruct->wCPAL_Timeout = CPAL_I2C_TIMEOUT_DEFAULT;

  CPAL_LOG("\n\r\n\rLOG <CPAL_I2C_DMA_TX_IRQHandler> : I2C Device TX DMA ");

  /*------------- If TC interrupt ------------*/
  if((__CPAL_I2C_HAL_GET_DMATX_TCIT(pDevInitStruct->CPAL_Dev)) != 0)
  {
    CPAL_LOG("\n\rLOG : I2C Device TX Complete");

    /* If DMA normal mode */
    if ((pDevInitStruct->wCPAL_Options & CPAL_OPT_DMATX_CIRCULAR) == 0)
    {
      /* Update remaining number of data */
      pDevInitStruct->pCPAL_TransferTx->wNumData = 0;

      /* Call DMA TX TC UserCallback */
      CPAL_I2C_DMATXTC_UserCallback(pDevInitStruct);

      /* Disable DMA request and channel */
      __CPAL_I2C_HAL_DISABLE_TXDMAREQ(pDevInitStruct->CPAL_Dev);
      __CPAL_I2C_HAL_DISABLE_DMATX(pDevInitStruct->CPAL_Dev);

      CPAL_LOG("\n\rLOG : I2C Device TX DMA Disabled");
    }
    /* If DMA circular mode */
    else
    {
      /* Call DMA TX TC UserCallback */
      CPAL_I2C_DMATXTC_UserCallback(pDevInitStruct);
    }

  }
  /*------------- If HT interrupt ------------*/
  else if ((__CPAL_I2C_HAL_GET_DMATX_HTIT(pDevInitStruct->CPAL_Dev)) != 0)
  {
    CPAL_LOG("\n\rLOG : I2C Device TX DMA Half Transfer ");

    /* Call DMA TX HT UserCallback */
    CPAL_I2C_DMATXHT_UserCallback(pDevInitStruct);
  }
  /*------------- If TE interrupt ------------*/
  else if ((__CPAL_I2C_HAL_GET_DMATX_TEIT(pDevInitStruct->CPAL_Dev)) != 0)
  {
    CPAL_LOG("\n\rERROR : I2C Device TX DMA Transfer Error ");

    /* Update CPAL_State to CPAL_STATE_ERROR */
    pDevInitStruct->CPAL_State = CPAL_STATE_ERROR;

    /* Update remaining number of data */
    pDevInitStruct->pCPAL_TransferTx->wNumData = __CPAL_I2C_HAL_DMATX_GET_CNDT(pDevInitStruct->CPAL_Dev);

    /* Call DMA TX TE UserCallback */
    CPAL_I2C_DMATXTE_UserCallback(pDevInitStruct);
  }

  /* Clear DMA interrupt Flag */
  __CPAL_I2C_HAL_CLEAR_DMATX_IT(pDevInitStruct->CPAL_Dev);

  return CPAL_PASS;
}


/**
  * @brief  Handle I2C DMA RX interrupt request when DMA programming Model is
  *         used for data reception.
  * @param  pDevInitStruct: Pointer to the peripheral configuration structure.
  * @retval CPAL_PASS.
  */
uint32_t CPAL_I2C_DMA_RX_IRQHandler(CPAL_InitTypeDef* pDevInitStruct)
{
  /* Reinitialize Timeout Value to default (no timeout initiated) */
  pDevInitStruct->wCPAL_Timeout = CPAL_I2C_TIMEOUT_DEFAULT;

  CPAL_LOG("\n\r\n\rLOG <CPAL_I2C_DMA_RX_IRQHandler> : I2C Device RX DMA ");

  /*------------- If TC interrupt ------------*/
  if ((__CPAL_I2C_HAL_GET_DMARX_TCIT(pDevInitStruct->CPAL_Dev)) != 0)
  {
    CPAL_LOG("\n\rLOG : I2C Device RX Complete");

    /* If DMA normal mode */
    if ((pDevInitStruct->wCPAL_Options & CPAL_OPT_DMARX_CIRCULAR) == 0)
    {
      /* Update remaining number of data */
      pDevInitStruct->pCPAL_TransferRx->wNumData = 0;

      /* Disable DMA Request and Channel */
      __CPAL_I2C_HAL_DISABLE_RXDMAREQ(pDevInitStruct->CPAL_Dev);
      __CPAL_I2C_HAL_DISABLE_DMARX(pDevInitStruct->CPAL_Dev);

      CPAL_LOG("\n\rLOG : I2C Device RX DMA Disabled");

      /* Call DMA RX TC UserCallback */
      CPAL_I2C_DMARXTC_UserCallback(pDevInitStruct);

      /* If No Stop Condition Generation option bit selected */
      if (((pDevInitStruct->wCPAL_Options & CPAL_OPT_I2C_NOSTOP) != 0) && (pDevInitStruct->CPAL_Mode == CPAL_MODE_SLAVE))
      {
        /* Disable slave interrupt */
        __CPAL_I2C_HAL_DISABLE_SLAVE_IT(pDevInitStruct->CPAL_Dev);

        /* Update CPAL_State to CPAL_STATE_READY */
        pDevInitStruct->CPAL_State = CPAL_STATE_READY;

        /* Call TX transfer complete UserCallback */
        CPAL_I2C_RXTC_UserCallback(pDevInitStruct);
      }
    }
    /* If DMA circular mode */
    else
    {
      /* Call DMA RX TC UserCallback */
      CPAL_I2C_DMARXTC_UserCallback(pDevInitStruct);
    }
  }
  /*------------- If HT interrupt ------------*/
  else if ((__CPAL_I2C_HAL_GET_DMARX_HTIT(pDevInitStruct->CPAL_Dev)) != 0)
  {
    CPAL_LOG("\n\rLOG : I2C Device RX DMA Half Transfer");

    /* Call DMA RX HT UserCallback */
    CPAL_I2C_DMARXHT_UserCallback(pDevInitStruct);
  }
  /*------------- If TE interrupt ------------*/
  else if ((__CPAL_I2C_HAL_GET_DMARX_TEIT(pDevInitStruct->CPAL_Dev)) != 0)
  {
    CPAL_LOG("\n\rERROR : I2C Device RX DMA Transfer Error ");

    /* Update CPAL_State to CPAL_STATE_ERROR */
    pDevInitStruct->CPAL_State = CPAL_STATE_ERROR;

    /* Update remaining number of data */
    pDevInitStruct->pCPAL_TransferRx->wNumData = __CPAL_I2C_HAL_DMARX_GET_CNDT(pDevInitStruct->CPAL_Dev);

    /* Call DMA RX TE UserCallback */
    CPAL_I2C_DMARXTE_UserCallback(pDevInitStruct);
  }

  /* Clear DMA interrupt Flag */
  __CPAL_I2C_HAL_CLEAR_DMARX_IT(pDevInitStruct->CPAL_Dev);

  return CPAL_PASS;
}
#endif /* CPAL_I2C_DMA_PROGMODEL */


/*================== CPAL_I2C_Timeout_Function ==================*/

/**
  * @brief  This function Manages I2C Timeouts when waiting for specific events.
  * @param  None
  * @retval CPAL_PASS or CPAL_FAIL.
  */
void CPAL_I2C_TIMEOUT_Manager(void)
{
  uint32_t index = 0;

  /* Manage I2C timeouts conditions */
  for (index = 0; index < CPAL_I2C_DEV_NUM; index ++)
  {
    if (I2C_DevStructures[index] != pNULL)
    {
      /* If Timeout occurred  */
      if (I2C_DevStructures[index]->wCPAL_Timeout == CPAL_I2C_TIMEOUT_DETECTED)
      {
        /* Reinitialize timeout value */
        I2C_DevStructures[index]->wCPAL_Timeout = CPAL_I2C_TIMEOUT_DEFAULT;

        /* Update CPAL_State to CPAL_STATE_ERROR */
        I2C_DevStructures[index]->CPAL_State = CPAL_STATE_ERROR;

        /* In case of Device Error Timeout_Callback should not be called */
        if (I2C_DevStructures[index]->wCPAL_DevError == CPAL_I2C_ERR_NONE)
        {
          /* Update wCPAL_DevError to CPAL_I2C_ERR_TIMEOUT */
          I2C_DevStructures[index]->wCPAL_DevError = CPAL_I2C_ERR_TIMEOUT;

          CPAL_LOG("\n\r\n\rLOG <CPAL_I2C_TIMEOUT_Manager> : I2C Device Timeout Error");

          /* Call CPAL_TIMEOUT_UserCallback */
          CPAL_TIMEOUT_UserCallback(I2C_DevStructures[index]);
        }
      }
      /* If Timeout is triggered (wCPAL_Timeout != CPAL_I2C_TIMEOUT_DEFAULT)*/
      else if (I2C_DevStructures[index]->wCPAL_Timeout != CPAL_I2C_TIMEOUT_DEFAULT)
      {
        /* Decrement the timeout value */
        I2C_DevStructures[index]->wCPAL_Timeout--;
      }
    }
  }
}


/**
  * @brief  This function Manages I2C Timeouts when Timeout occurred.
  * @param  pDevInitStruct: Pointer to the peripheral configuration structure.
  * @retval CPAL_PASS or CPAL_FAIL.
  */
uint32_t CPAL_I2C_Timeout (CPAL_InitTypeDef* pDevInitStruct)
{
  /* Reinitialize timeout value */
  pDevInitStruct->wCPAL_Timeout = CPAL_I2C_TIMEOUT_DEFAULT;

  /* update CPAL_State to CPAL_STATE_ERROR */
  pDevInitStruct->CPAL_State = CPAL_STATE_ERROR;

  /* update wCPAL_DevError to CPAL_I2C_ERR_TIMEOUT */
  pDevInitStruct->wCPAL_DevError = CPAL_I2C_ERR_TIMEOUT;

  /* Call Timeout Callback and quit current function */
  return (CPAL_TIMEOUT_UserCallback(pDevInitStruct));
}

/*================== CPAL_I2C_Event_Handler ==================*/

#ifdef CPAL_I2C_MASTER_MODE
/**
  * @brief  Handles Master TCR interrupt event.
  * @param  pDevInitStruct: Pointer to the peripheral configuration structure.
  * @retval CPAL_PASS or CPAL_FAIL.
  */
static uint32_t I2C_MASTER_TCR_Handle(CPAL_InitTypeDef* pDevInitStruct)
{
  CR2_tmp = 0;

  /* If DMA programming model */
  if (pDevInitStruct->CPAL_ProgModel == CPAL_PROGMODEL_DMA)
  {
    /* If master transmitter */
    if (pDevInitStruct->CPAL_State == CPAL_STATE_BUSY_TX)
    {
      /* Update wNumData */
      pDevInitStruct->pCPAL_TransferTx->wNumData = pDevInitStruct->pCPAL_TransferTx->wNumData - 0xff;
    }
    /* If master receiver */
    else
    {
      /* Update wNumData */
      pDevInitStruct->pCPAL_TransferRx->wNumData = pDevInitStruct->pCPAL_TransferRx->wNumData - 0xff;
    }
  }

  /* If master transmitter */
  if (pDevInitStruct->CPAL_State == CPAL_STATE_BUSY_TX)
  {
    /* If remaining number of data is equal or lower than 255 */
    if (pDevInitStruct->pCPAL_TransferTx->wNumData <= 0xff)
    {
      /* Update Num_Data */
      Num_Data = pDevInitStruct->pCPAL_TransferTx->wNumData;

      /* Set Nbytes to wNumData */
      CR2_tmp |= (uint32_t)((uint32_t)(Num_Data) << 16);

      /* Disable reload */
      CR2_tmp &= ~I2C_CR2_RELOAD;
    }
    /* If remaining number of data is greater than 255 */
    else
    {
      /* Set Nbytes to wNumData */
      CR2_tmp |= (uint32_t)((uint32_t)(255) << 16);

      /* Enable reload */
      CR2_tmp |= I2C_CR2_RELOAD;
    }
  }
  /* If master receiver */
  else
  {
    /* If remaining number of data is equal or lower than 255 */
    if (pDevInitStruct->pCPAL_TransferRx->wNumData <= 0xff)
    {
      /* Update num data */
      Num_Data = pDevInitStruct->pCPAL_TransferRx->wNumData;

      /* Set Nbytes to wNumData */
      CR2_tmp |= (uint32_t)((uint32_t)(Num_Data) << 16);

      /* Disable reload */
      CR2_tmp &= ~I2C_CR2_RELOAD;

    }
    /* If remaining number of data is greater than 255 */
    else
    {
      /* Set Nbytes to wNumData */
      CR2_tmp |= (uint32_t)((uint32_t)(255) << 16);

      /* Enable reload */
      CR2_tmp |= I2C_CR2_RELOAD;
    }
  }

  /* Update CR2 Register */
  __CPAL_I2C_HAL_CR2_UPDATE(pDevInitStruct->CPAL_Dev, CR2_tmp);

  return CPAL_PASS;
}

/**
  * @brief  Handles Master TC interrupt event.
  * @param  pDevInitStruct: Pointer to the peripheral configuration structure.
  * @retval CPAL_PASS or CPAL_FAIL.
  */
static uint32_t I2C_MASTER_TC_Handle(CPAL_InitTypeDef* pDevInitStruct)
{
  /* No Stop Condition Generation option bit is not selected */
  if ((pDevInitStruct->wCPAL_Options & CPAL_OPT_I2C_NOSTOP) == 0)
  {
    /* Generate stop condition */
    __CPAL_I2C_HAL_STOP(pDevInitStruct->CPAL_Dev);
  }
  /* No Stop Condition Generation option bit is selected */
  else
  {
    /* Disable master interrupts */
    __CPAL_I2C_HAL_DISABLE_MASTER_IT(pDevInitStruct->CPAL_Dev);

    /* If master transmitter */
    if (pDevInitStruct->CPAL_State == CPAL_STATE_BUSY_TX)
    {
  #ifdef CPAL_I2C_IT_PROGMODEL
      /* If Interrupt Programming Model */
      if (pDevInitStruct->CPAL_ProgModel == CPAL_PROGMODEL_INTERRUPT)
      {
        /* Disable TX interrupt */
        __CPAL_I2C_HAL_DISABLE_TXIE_IT(pDevInitStruct->CPAL_Dev);
      }
  #endif /* CPAL_I2C_IT_PROGMODEL */

      /* Update CPAL_State to CPAL_STATE_READY */
      pDevInitStruct->CPAL_State = CPAL_STATE_READY;

      /* Call TX Transfer complete Callback */
      CPAL_I2C_TXTC_UserCallback(pDevInitStruct);
    }
    /* If master receiver */
    else
    {
  #ifdef CPAL_I2C_IT_PROGMODEL
      /* If Interrupt Programming Model */
      if (pDevInitStruct->CPAL_ProgModel == CPAL_PROGMODEL_INTERRUPT)
      {
        /* Disable RX interrupt */
        __CPAL_I2C_HAL_DISABLE_RXIE_IT(pDevInitStruct->CPAL_Dev);
      }
  #endif /* CPAL_I2C_IT_PROGMODEL */

      /* Update CPAL_State to CPAL_STATE_READY */
      pDevInitStruct->CPAL_State = CPAL_STATE_READY;

      /* Call RX Transfer complete Callback */
      CPAL_I2C_RXTC_UserCallback(pDevInitStruct);
    }
  }
  return CPAL_PASS;
}

/**
  * @brief  Handles Master STOP interrupt event.
  * @param  pDevInitStruct: Pointer to the peripheral configuration structure.
  * @retval CPAL_PASS or CPAL_FAIL.
  */
static uint32_t I2C_MASTER_STOP_Handle(CPAL_InitTypeDef* pDevInitStruct)
{
  /* If NACK received by master */
  if (pDevInitStruct->wCPAL_DevError == CPAL_I2C_ERR_AF)
  {
    /* Set CPAL_State to CPAL_STATE_ERROR */
    pDevInitStruct->CPAL_State = CPAL_STATE_ERROR;

    /* Clear STOP flag */
    __CPAL_I2C_HAL_CLEAR_STOP(pDevInitStruct->CPAL_Dev);
  }
  else
  {
    /* Clear STOP flag */
    __CPAL_I2C_HAL_CLEAR_STOP(pDevInitStruct->CPAL_Dev);

    CPAL_LOG("\n\r\n\rLOG <I2C_EV_IRQHandler> : I2C Device Master IT");

    CPAL_LOG("\n\rLOG : I2C Device Stop Generated");

    /* Disable master interrupt */
    __CPAL_I2C_HAL_DISABLE_MASTER_IT(pDevInitStruct->CPAL_Dev);

    /* Wait until BUSY flag is reset */
    __CPAL_I2C_TIMEOUT(!(__CPAL_I2C_HAL_GET_BUSY(pDevInitStruct->CPAL_Dev)), CPAL_I2C_TIMEOUT_BUSY);

    /* If master transmitter */
    if (pDevInitStruct->CPAL_State == CPAL_STATE_BUSY_TX)
    {
  #ifdef CPAL_I2C_IT_PROGMODEL
      /* If Interrupt Programming Model */
      if (pDevInitStruct->CPAL_ProgModel == CPAL_PROGMODEL_INTERRUPT)
      {
        /* Disable TX interrupt */
        __CPAL_I2C_HAL_DISABLE_TXIE_IT(pDevInitStruct->CPAL_Dev);
      }
  #endif /* CPAL_I2C_IT_PROGMODEL */

      /* Update CPAL_State to CPAL_STATE_READY */
      pDevInitStruct->CPAL_State = CPAL_STATE_READY;

      /* Call TX Transfer complete Callback */
      CPAL_I2C_TXTC_UserCallback(pDevInitStruct);
    }
    /* If master receiver */
    else
    {
  #ifdef CPAL_I2C_IT_PROGMODEL
      /* If Interrupt Programming Model */
      if (pDevInitStruct->CPAL_ProgModel == CPAL_PROGMODEL_INTERRUPT)
      {
        /* Disable RX interrupt */
        __CPAL_I2C_HAL_DISABLE_RXIE_IT(pDevInitStruct->CPAL_Dev);
      }
  #endif /* CPAL_I2C_IT_PROGMODEL */

      /* Update CPAL_State to CPAL_STATE_READY */
      pDevInitStruct->CPAL_State = CPAL_STATE_READY;

      /* Call RX Transfer complete Callback */
      CPAL_I2C_RXTC_UserCallback(pDevInitStruct);
    }
  }
  return CPAL_PASS;
}

/**
  * @brief  Handles Master NACK interrupt event.
  * @param  pDevInitStruct: Pointer to the peripheral configuration structure.
  * @retval CPAL_PASS or CPAL_FAIL.
  */
static uint32_t I2C_MASTER_NACK_Handle(CPAL_InitTypeDef* pDevInitStruct)
{
  /* Update wCPAL_DevError */
  pDevInitStruct->wCPAL_DevError = CPAL_I2C_ERR_AF;

  /* Clear NACK flag */
  __CPAL_I2C_HAL_CLEAR_NACK(pDevInitStruct->CPAL_Dev);

  /* USE_SINGLE_ERROR_CALLBACK is defined in stm32f30x_i2c_cpal_conf.h file */
#ifdef USE_SINGLE_ERROR_CALLBACK
  /* Call Error UserCallback */
  CPAL_I2C_ERR_UserCallback(pDevInitStruct->CPAL_Dev, pDevInitStruct->wCPAL_DevError);
#elif defined(USE_MULTIPLE_ERROR_CALLBACK)
  /* Call AF UserCallback */
  CPAL_I2C_AF_UserCallback(pDevInitStruct->CPAL_Dev);
#endif /* USE_SINGLE_ERROR_CALLBACK */

  return CPAL_PASS;
}

  #ifdef CPAL_I2C_IT_PROGMODEL
/**
  * @brief  Handles Master transmission TXIS interrupt event.
  * @param  pDevInitStruct: Pointer to the peripheral configuration structure.
  * @retval CPAL_PASS or CPAL_FAIL.
  */
static uint32_t I2C_MASTER_TXIS_Handle(CPAL_InitTypeDef* pDevInitStruct)
{
  /* Call TX UserCallback */
  CPAL_I2C_TX_UserCallback(pDevInitStruct);

  /* Write Byte */
  __CPAL_I2C_HAL_SEND((pDevInitStruct->CPAL_Dev), (*(pDevInitStruct->pCPAL_TransferTx->pbBuffer)));

  /* Decrement remaining number of data */
  pDevInitStruct->pCPAL_TransferTx->wNumData--;

  if (pDevInitStruct->pCPAL_TransferTx->wNumData != 0)
  {
    /* Point to next data */
    pDevInitStruct->pCPAL_TransferTx->pbBuffer++;
  }
  return CPAL_PASS;
}

/**
  * @brief  Handles Master reception RXNE interrupt event.
  * @param  pDevInitStruct: Pointer to the peripheral configuration structure.
  * @retval CPAL_PASS or CPAL_FAIL.
  */
static uint32_t I2C_MASTER_RXNE_Handle(CPAL_InitTypeDef* pDevInitStruct)
{
  /* Read Byte */
  *(pDevInitStruct->pCPAL_TransferRx->pbBuffer) = __CPAL_I2C_HAL_RECEIVE(pDevInitStruct->CPAL_Dev);

  /* Call RX UserCallback */
  CPAL_I2C_RX_UserCallback(pDevInitStruct);

  /* Decrement remaining number of data */
  pDevInitStruct->pCPAL_TransferRx->wNumData--;

  /* If data remaining for reception */
  if (pDevInitStruct->pCPAL_TransferRx->wNumData != 0)
  {
    /* Point to next data */
    pDevInitStruct->pCPAL_TransferRx->pbBuffer++;
  }
  return CPAL_PASS;
}
  #endif /* CPAL_I2C_IT_PROGMODEL */
#endif /* CPAL_I2C_MASTER_MODE */


#ifdef CPAL_I2C_SLAVE_MODE
/**
  * @brief  Handles Slave ADDR interrupt event.
  * @param  pDevInitStruct: Pointer to the peripheral configuration structure.
  * @retval CPAL_PASS or CPAL_FAIL.
  */
static uint32_t I2C_SLAVE_ADDR_Handle(CPAL_InitTypeDef* pDevInitStruct)
{
#ifdef CPAL_I2C_LISTEN_MODE
  /* If slave receive request for write */
  if (__CPAL_I2C_HAL_GET_DIR(pDevInitStruct->CPAL_Dev) == 0)
  {
    pDevInitStruct->CPAL_State = CPAL_STATE_BUSY_RX;

    /* Call Slave receive UserCallback */
    CPAL_I2C_SLAVE_READ_UserCallback(pDevInitStruct);
  }
  /* If slave receive request for read */
  else
  {
    pDevInitStruct->CPAL_State = CPAL_STATE_BUSY_TX;

    /* Call Slave Transmit UserCallback */
    CPAL_I2C_SLAVE_WRITE_UserCallback(pDevInitStruct);
  }
#else
  uint32_t slaveaddr = 0;

  /* If 7 Bit Addressing Mode */
  if (pDevInitStruct->pCPAL_I2C_Struct->I2C_AcknowledgedAddress == I2C_AcknowledgedAddress_7bit)
  {
    /* Get slave matched address */
    slaveaddr = __CPAL_I2C_HAL_GET_ADDCODE(pDevInitStruct->CPAL_Dev);

    /* if matched address is not equal to OA1 */
    if (slaveaddr !=__CPAL_I2C_HAL_GET_OA1(pDevInitStruct->CPAL_Dev))
    {
      /* If General Call addressing mode selected */
      if ( slaveaddr == 0x00000000)
      {
        CPAL_LOG("\n\rLOG : I2C Device GENCALL Mode");

        /* Call GENCALL UserCallback */
        CPAL_I2C_GENCALL_UserCallback(pDevInitStruct);
      }
      /* If DUAL addressing mode selected */
      else
      {
        CPAL_LOG("\n\rLOG : I2C Device DUAL ADDR Mode Selected");

        /* Call DUALF UserCallback */
        CPAL_I2C_DUALF_UserCallback(pDevInitStruct);
      }
    }
  }
#endif /* CPAL_I2C_LISTEN_MODE */

  /* Clear ADDR flag */
  __CPAL_I2C_HAL_CLEAR_ADDR(pDevInitStruct->CPAL_Dev);

  return CPAL_PASS;
}

/**
  * @brief  Handles Slave STOP interrupt event.
  * @param  pDevInitStruct: Pointer to the peripheral configuration structure.
  * @retval CPAL_PASS or CPAL_FAIL.
  */
static uint32_t I2C_SLAVE_STOP_Handle(CPAL_InitTypeDef* pDevInitStruct)
{
  /* Clear STOP flag */
  __CPAL_I2C_HAL_CLEAR_STOP(pDevInitStruct->CPAL_Dev);

  CPAL_LOG("\n\r\n\rLOG <I2C_EV_IRQHandler> : I2C Device Slave IT");

  CPAL_LOG("\n\rLOG : I2C Device Stop Detected");

  /* Disable slave interrupt */
  __CPAL_I2C_HAL_DISABLE_SLAVE_IT(pDevInitStruct->CPAL_Dev);

  /* Wait until BUSY flag is reset */
  __CPAL_I2C_TIMEOUT(!(__CPAL_I2C_HAL_GET_BUSY(pDevInitStruct->CPAL_Dev)), CPAL_I2C_TIMEOUT_BUSY);

  /* If NACK Slave Own Address option bit selected */
  if ((pDevInitStruct->wCPAL_Options & CPAL_OPT_I2C_NACK_ADD) != 0)
  {
    /* Disable Acknowledgement of own Address */
    __CPAL_I2C_HAL_DISABLE_DEV(pDevInitStruct->CPAL_Dev);
  }

  /* If slave transmitter */
  if (pDevInitStruct->CPAL_State == CPAL_STATE_BUSY_TX)
  {
  #ifdef CPAL_I2C_IT_PROGMODEL
    /* If Interrupt Programming Model */
    if (pDevInitStruct->CPAL_ProgModel == CPAL_PROGMODEL_INTERRUPT)
    {
      /* Disable TX interrupt */
      __CPAL_I2C_HAL_DISABLE_TXIE_IT(pDevInitStruct->CPAL_Dev);
    }
  #endif /* CPAL_I2C_IT_PROGMODEL */

    /* Update CPAL_State to CPAL_STATE_READY */
    pDevInitStruct->CPAL_State = CPAL_STATE_READY;

    /* Call TX Transfer complete Callback */
    CPAL_I2C_TXTC_UserCallback(pDevInitStruct);
  }
  /* If slave receiver */
  else
  {
  #ifdef CPAL_I2C_IT_PROGMODEL
    /* If Interrupt Programming Model */
    if (pDevInitStruct->CPAL_ProgModel == CPAL_PROGMODEL_INTERRUPT)
    {
      /* Disable RX interrupt */
      __CPAL_I2C_HAL_DISABLE_RXIE_IT(pDevInitStruct->CPAL_Dev);
    }
  #endif /* CPAL_I2C_IT_PROGMODEL */

    /* Update CPAL_State to CPAL_STATE_READY */
    pDevInitStruct->CPAL_State = CPAL_STATE_READY;

    /* Call RX Transfer complete Callback */
    CPAL_I2C_RXTC_UserCallback(pDevInitStruct);
  }
  return CPAL_PASS;
}

/**
  * @brief  Handles Slave NACK interrupt event.
  * @param  pDevInitStruct: Pointer to the peripheral configuration structure.
  * @retval CPAL_PASS or CPAL_FAIL.
  */
static uint32_t I2C_SLAVE_NACK_Handle(CPAL_InitTypeDef* pDevInitStruct)
{
  /* Clear NACK flag */
  __CPAL_I2C_HAL_CLEAR_NACK(pDevInitStruct->CPAL_Dev);

  /* No Stop Condition Generation option bit selected and slave transmitter */
  if (((pDevInitStruct->wCPAL_Options & CPAL_OPT_I2C_NOSTOP) != 0) && (pDevInitStruct->CPAL_State == CPAL_STATE_BUSY_TX))
  {
    /* Disable slave interrupt */
    __CPAL_I2C_HAL_DISABLE_SLAVE_IT(pDevInitStruct->CPAL_Dev);

  #ifdef CPAL_I2C_IT_PROGMODEL
    /* If Interrupt Programming Model */
    if (pDevInitStruct->CPAL_ProgModel == CPAL_PROGMODEL_INTERRUPT)
    {
      /* Disable TX interrupt */
      __CPAL_I2C_HAL_DISABLE_TXIE_IT(pDevInitStruct->CPAL_Dev);
    }
  #endif /* CPAL_I2C_IT_PROGMODEL */

    /* Update CPAL_State to CPAL_STATE_READY */
    pDevInitStruct->CPAL_State = CPAL_STATE_READY;

    /* Call TX Transfer complete Callback */
    CPAL_I2C_TXTC_UserCallback(pDevInitStruct);
  }
  return CPAL_PASS;
}

  #ifdef CPAL_I2C_IT_PROGMODEL
/**
  * @brief  Handles Slave transmission TXIS interrupt event.
  * @param  pDevInitStruct: Pointer to the peripheral configuration structure.
  * @retval CPAL_PASS or CPAL_FAIL.
  */
static uint32_t I2C_SLAVE_TXIS_Handle(CPAL_InitTypeDef* pDevInitStruct)
{
  /* If data remaining for transmission */
  if (pDevInitStruct->pCPAL_TransferTx->wNumData != 0)
  {
    /* Call TX UserCallback */
    CPAL_I2C_TX_UserCallback(pDevInitStruct);

    /* Write Byte */
    __CPAL_I2C_HAL_SEND((pDevInitStruct->CPAL_Dev), (*(pDevInitStruct->pCPAL_TransferTx->pbBuffer)));

    /* Decrement remaining number of data */
    pDevInitStruct->pCPAL_TransferTx->wNumData--;

    if (pDevInitStruct->pCPAL_TransferTx->wNumData != 0)
    {
      /* Point to next data */
      pDevInitStruct->pCPAL_TransferTx->pbBuffer++;
    }
  }
  return CPAL_PASS;
}

/**
  * @brief  Handles Slave reception RXNE interrupt event.
  * @param  pDevInitStruct: Pointer to the peripheral configuration structure.
  * @retval CPAL_PASS or CPAL_FAIL.
  */
static uint32_t I2C_SLAVE_RXNE_Handle(CPAL_InitTypeDef* pDevInitStruct)
{
  /* If data remaining for reception */
  if (pDevInitStruct->pCPAL_TransferRx->wNumData != 0)
  {
    /* Read Byte */
    *(pDevInitStruct->pCPAL_TransferRx->pbBuffer) = __CPAL_I2C_HAL_RECEIVE(pDevInitStruct->CPAL_Dev);

    /* Call RX UserCallback */
    CPAL_I2C_RX_UserCallback(pDevInitStruct);

    /* Decrement remaining number of data */
    pDevInitStruct->pCPAL_TransferRx->wNumData--;

    /* If data remaining for reception */
    if (pDevInitStruct->pCPAL_TransferRx->wNumData != 0)
    {
      /* Point to next data */
      pDevInitStruct->pCPAL_TransferRx->pbBuffer++;
    }
    /* If all data received and No Stop Condition Generation option bit selected */
    else if ((pDevInitStruct->wCPAL_Options & CPAL_OPT_I2C_NOSTOP) != 0)
    {
      /* Disable slave interrupt */
      __CPAL_I2C_HAL_DISABLE_SLAVE_IT(pDevInitStruct->CPAL_Dev);

      /* Disable RX interrupt */
      __CPAL_I2C_HAL_DISABLE_RXIE_IT(pDevInitStruct->CPAL_Dev);

      /* Update CPAL_State to CPAL_STATE_READY */
      pDevInitStruct->CPAL_State = CPAL_STATE_READY;

      /* Call RX Transfer complete Callback */
      CPAL_I2C_RXTC_UserCallback(pDevInitStruct);
    }
  }
  return CPAL_PASS;
}
  #endif /* CPAL_I2C_IT_PROGMODEL */
#endif /* CPAL_I2C_SLAVE_MODE */



/*================== Local DMA and IT Manager ==================*/

#ifdef CPAL_I2C_DMA_PROGMODEL
/**
  * @brief  This function Configures and enables I2C DMA before starting transfer phase.
  * @param  pDevInitStruct: Pointer to the peripheral configuration structure.
  * @param  Direction : Transfer direction.
  * @retval CPAL_PASS or CPAL_FAIL.
  */
static uint32_t I2C_Enable_DMA (CPAL_InitTypeDef* pDevInitStruct, CPAL_DirectionTypeDef Direction)
{
  /* If data transmission will be performed */
  if ((pDevInitStruct->CPAL_State == CPAL_STATE_BUSY_TX) || (Direction == CPAL_DIRECTION_TX))
  {
    /* Configure TX DMA channels */
    CPAL_I2C_HAL_DMATXConfig(pDevInitStruct->CPAL_Dev, pDevInitStruct->pCPAL_TransferTx, pDevInitStruct->wCPAL_Options);

    /* Enable TX DMA channels */
    __CPAL_I2C_HAL_ENABLE_DMATX(pDevInitStruct->CPAL_Dev);

    CPAL_LOG("\n\rLOG : I2C Device DMA TX Configured and Enabled");
  }
  /* If data reception will be performed */
  else if ((pDevInitStruct->CPAL_State == CPAL_STATE_BUSY_RX) || (Direction == CPAL_DIRECTION_RX))
  {
    /* Configure RX DMA channels */
    CPAL_I2C_HAL_DMARXConfig(pDevInitStruct->CPAL_Dev, pDevInitStruct->pCPAL_TransferRx, pDevInitStruct->wCPAL_Options);

    /* Enable RX DMA channels */
    __CPAL_I2C_HAL_ENABLE_DMARX(pDevInitStruct->CPAL_Dev);

    CPAL_LOG("\n\rLOG : I2C Device DMA RX Configured and Enabled");
  }
  return CPAL_PASS;
}
#endif /* CPAL_I2C_DMA_PROGMODEL */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
