/**************************************************************************//**
 * @file     SPI.h
 * @version  V2.1
 * $Revision: 4 $
 * $Date: 12/08/10 3:12p $
 * @brief    M051 Series SPI Driver Header File
 *
 * @note
 * Copyright (C) 2011 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#ifndef __SPI_H__
#define __SPI_H__

/*---------------------------------------------------------------------------------------------------------*/
/* Include related headers                                                                                 */
/*---------------------------------------------------------------------------------------------------------*/
#include "M051Series.h"


/** @addtogroup M051_FUNC M051 Function Interface
  * @{
  */

/** @addtogroup SPI_FUNC SPI Device Function Interface
  * @{
  */



/*---------------------------------------------------------------------------------------------------------*/
/* SPI_CNTRL constant definitions                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
#define SPI_CNTRL_VARCLK_EN              (1UL << 23) /*!< SPI_CNTRL setting for enable variable clock function */
#define SPI_CNTRL_REORDER_ON_SUSPEND_ON  (1UL << 19) /*!< SPI_CNTRL setting for enable both byte reorder and byte suspend */
#define SPI_CNTRL_REORDER_ON_SUSPEND_OFF (2UL << 19) /*!< SPI_CNTRL setting for enable byte reorder but disable byte suspend */
#define SPI_CNTRL_REORDER_OFF_SUSPEND_ON (3UL << 19) /*!< SPI_CNTRL setting for disable byte reorder but enable byte suspend */
#define SPI_CNTRL_MASTER_MODE            (0UL << 18) /*!< SPI_CNTRL setting for SPI master mode */
#define SPI_CNTRL_SLAVE_MODE             (1UL << 18) /*!< SPI_CNTRL setting for SPI slave mode */
#define SPI_CNTRL_IE_EN                  (1UL << 17) /*!< SPI_CNTRL setting for eanble interrupt */
#define SPI_CNTRL_IF                     (1UL << 16) /*!< SPI_CNTRL interrupt flag */
#define SPI_CNTRL_SP_CYCLE(x)            ((x) << 12) /*!< SPI_CNTRL setting for suspend interverl. it could be 0~15 */
#define SPI_CNTRL_CLK_IDLE_HIGH          (1UL << 11) /*!< SPI_CNTRL setting for clock idle high */
#define SPI_CNTRL_CLK_IDLE_LOW           (0)         /*!< SPI_CNTRL setting for clock idle low */
#define SPI_CNTRL_LSB_FIRST              (1UL << 10) /*!< SPI_CNTRL setting for data format as LSB first */
#define SPI_CNTRL_MSB_FIRST              (0)         /*!< SPI_CNTRL setting for data format as MSB first */
#define SPI_CNTRL_TX_NUM(x)              (((x) - 1) <<  8)   /*!< SPI_CNTRL setting for burst transfer count. It could be 1 or 2 */
#define SPI_CNTRL_TX_BIT_LEN(x)          ((((x)!=32)?(x):0) << 3) /*!< SPI_CNTRL setting for bit length of a transfer. it could be 8~32 */
#define SPI_CNTRL_TX_RISING              (0)         /*!< SPI_CNTRL setting for output data at SPICLK rising edge */
#define SPI_CNTRL_TX_FALLING             (4)         /*!< SPI_CNTRL setting for TX output data at SPICLK falling edge */
#define SPI_CNTRL_RX_RISING              (0)         /*!< SPI_CNTRL setting for RX latch data at SPICLK rising edge */
#define SPI_CNTRL_RX_FALLING             (2)         /*!< SPI_CNTRL setting for RX latch data at SPICLK falling edge */
#define SPI_CNTRL_GO_BUSY                (1)         /*!< SPI_CNTRL setting for trigger SPI */


/*---------------------------------------------------------------------------------------------------------*/
/* SPI_DIVIDER constant definitions                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
#define SPI_DIVIDER_DIV(x)  (x)         /*!< SPI_DIVIDER setting for SPI master clock divider. It could be 0~255 */
#define SPI_DIVIDER_DIV2(x) ((x) << 16) /*!< SPI_DIVIDER setting for SPI master clock divider2 of varialbe clock function. It could be 0~255 */


/*---------------------------------------------------------------------------------------------------------*/
/* SPI_SSR constant definitions                                                                            */
/*---------------------------------------------------------------------------------------------------------*/
#define SPI_SSR_SLAVE_FALLING_EDGE_TRIGGER (0x0)  /*!< SPI_SSR setting for falling edge trigger in SPI slave mode */
#define SPI_SSR_SLAVE_RISING_EDGE_TRIGGER  (0x4)  /*!< SPI_SSR setting for rising edge trigger in SPI slave mode */
#define SPI_SSR_SLAVE_LOW_LEVEL_ACTIVE     (0x10) /*!< SPI_SSR setting for low level active in SPI slave mode */
#define SPI_SSR_SLAVE_HIGH_LEVEL_ACTIVE    (0x14) /*!< SPI_SSR setting for high level active in SPI slave mode */
#define SPI_SSR_HW_AUTO_ACTIVE_LOW         (0x9)  /*!< SPI_SSR setting for SPI master SS signal is controled by hardware automatically and active low */
#define SPI_SSR_HW_AUTO_ACTIVE_HIGH        (0xD)  /*!< SPI_SSR setting for SPI master SS signal is controled by hardware automatically active high */
#define SPI_SSR_SW_SS_PIN_LOW              (0x1)  /*!< SPI_SSR setting to force SPI master SS signal to low by software */
#define SPI_SSR_SW_SS_PIN_HIGH             (0x0)  /*!< SPI_SSR setting to force SPI master SS signal to high by software */

/*---------------------------------------------------------------------------------------------------------*/
/* SPI_CNTRL2 constant definitions                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#define SPI_CNTRL2_3WIRE_START_INTSTS                    (1UL << 11) /*!< SPI_CNTRL2 3-wire mode start interrupt flag */
#define SPI_CNTRL2_3WIRE_START_INT_EN                    (1UL << 10) /*!< SPI_CNTRL2 setting for 3-wire mode start interrupt enable */
#define SPI_CNTRL2_3WIRE_ABORT                           (1UL << 9)  /*!< SPI_CNTRL2 setting for 3-wire mode transfer abort */
#define SPI_CNTRL2_3WIRE_MODE_EN                         (1UL << 8)  /*!< SPI_CNTRL2 setting for 3-wire mode enable */
#define SPI_CNTRL2_DIV_ONE_EN                            (1)         /*!< SPI_CNTRL2 setting for DIV_ONE feature enable */


/*---------------------------------------------------------------------------------------------------------*/
/*  Define Macros and functions                                                                            */
/*---------------------------------------------------------------------------------------------------------*/

/**
 * @details    Configure the user-specified SPI port as a master
 */
#define _SPI_SET_MASTER_MODE(port) ((port)->CNTRL &= (~SPI_CNTRL_SLAVE_MODE))

/**
 * @details    Configure the user-specified SPI port as a slave
 */
#define _SPI_SET_SLAVE_MODE(port) ((port)->CNTRL |= SPI_CNTRL_SLAVE_MODE)

/**
 * @details    Configure the transfer bit length of the user-specified SPI port
 */
#define _SPI_SET_TRANSFER_BIT_LENGTH(port, x) ((port)->CNTRL = ((port)->CNTRL & SPI_CNTRL_TX_BIT_LEN_Msk) | SPI_CNTRL_TX_BIT_LEN(x))

/**
 * @details    Enable the automatic slave select function of the user-specified SPI port
 */
#define _SPI_ENABLE_HW_AUTO_SLAVE_SELECT(port) ((port)->SSR |= SPI_SSR_AUTOSS_Msk)

/**
 * @details    Disable the automatic slave select function of the user-specified SPI port
 */
#define _SPI_DISABLE_HW_AUTO_SLAVE_SELECT(port) ((port)->SSR &= (~SPI_SSR_AUTOSS_Msk))

/**
 * @details    Configure the timing type of the user-specified SPI port
 */
#define _SPI_SET_TIMING_TYPE1(port) ((port)->CNTRL = ((port)->CNTRL & (~(SPI_CNTRL_CLKP_Msk|SPI_CNTRL_TX_NEG_Msk|SPI_CNTRL_RX_NEG_Msk)) | SPI_CNTRL_TX_NEG_Msk))

/**
 * @details    Configure the timing type of the user-specified SPI port
 */
#define _SPI_SET_TIMING_TYPE2(port) ((port)->CNTRL = ((port)->CNTRL & (~(SPI_CNTRL_CLKP_Msk|SPI_CNTRL_TX_NEG_Msk|SPI_CNTRL_RX_NEG_Msk)) | SPI_CNTRL_RX_NEG_Msk))

/**
 * @details    Configure the timing type of the user-specified SPI port
 */
#define _SPI_SET_TIMING_TYPE5(port) ((port)->CNTRL = ((port)->CNTRL & (~(SPI_CNTRL_CLKP_Msk|SPI_CNTRL_TX_NEG_Msk|SPI_CNTRL_RX_NEG_Msk)) | SPI_CNTRL_CLKP_Msk | SPI_CNTRL_TX_NEG_Msk))

/**
 * @details    Configure the timing type of the user-specified SPI port
 */
#define _SPI_SET_TIMING_TYPE6(port) ((port)->CNTRL = ((port)->CNTRL & (~(SPI_CNTRL_CLKP_Msk|SPI_CNTRL_TX_NEG_Msk|SPI_CNTRL_RX_NEG_Msk)) | SPI_CNTRL_CLKP_Msk | SPI_CNTRL_RX_NEG_Msk))

/**
 * @details    Configure the user-specified SPI port as LSB-first transfer type
 */
#define _SPI_SET_TRANSFER_LSB_FIRST(port) ((port)->CNTRL |= SPI_CNTRL_LSB_FIRST)

/**
 * @details    Configure the user-specified SPI port as MSB-first transfer type
 */
#define _SPI_SET_TRANSFER_MSB_FIRST(port) ((port)->CNTRL &= (~SPI_CNTRL_LSB_FIRST))

/**
 * @details    Enable byte-reordering function and byte-suspend function
 */
#define _SPI_SET_REORDER_ON_SUSPEND_ON(port) ((port)->CNTRL = (port)->CNTRL & (~SPI_CNTRL_REORDER_Msk) | SPI_CNTRL_REORDER_ON_SUSPEND_ON)

/**
 * @details    Enable byte-reordering function and disable byte-suspend function
 */
#define _SPI_SET_REORDER_ON_SUSPEND_OFF(port) ((port)->CNTRL = (port)->CNTRL & (~SPI_CNTRL_REORDER_Msk) | SPI_CNTRL_REORDER_ON_SUSPEND_OFF)

/**
 * @details    Disable byte-reordering function and enable byte-suspend function
 */
#define _SPI_SET_REORDER_OFF_SUSPEND_ON(port) ((port)->CNTRL = (port)->CNTRL & (~SPI_CNTRL_REORDER_Msk) | SPI_CNTRL_REORDER_OFF_SUSPEND_ON)

/**
 * @details    Disable byte-reordering function and byte-suspend function
 */
#define _SPI_SET_REORDER_OFF_SUSPEND_OFF(port) ((port)->CNTRL = (port)->CNTRL & (~SPI_CNTRL_REORDER_Msk))

/**
 * @details    Configure the suspend interval of the user-specified SPI port. The x seeting can be 0-0xF.
 */
#define _SPI_SET_SUSPEND_CYCLE(port, x) ((port)->CNTRL = ((port)->CNTRL & SPI_CNTRL_SP_CYCLE_Msk) | SPI_CNTRL_SP_CYCLE(x))

/**
 * @details    Configure the user-specified SPI port as a low-level-active device when it is in slave mode
 */
#define _SPI_SET_SLAVE_SELECT_LOW_LEVEL_ACTIVE(port) ((port)->SSR = (port)->SSR & (~(SPI_SSR_SS_LTRIG_Msk|SPI_SSR_SS_LVL_Msk)) | SPI_SSR_SS_LTRIG_Msk)

/**
 * @details    Configure the user-specified SPI port as a high-level-active device when it is in slave mode
 */
#define _SPI_SET_SLAVE_SELECT_HIGH_LEVEL_ACTIVE(port) ((port)->SSR |= (SPI_SSR_SS_LTRIG_Msk|SPI_SSR_SS_LVL_Msk))

/**
 * @details    Configure the user-specified SPI port as a falling-edge-active device when it is in slave mode
 */
#define _SPI_SET_SLAVE_SELECT_FALLING_EDGE_ACTIVE(port) ((port)->SSR &= (~(SPI_SSR_SS_LTRIG_Msk|SPI_SSR_SS_LVL_Msk)))

/**
 * @details    Configure the user-specified SPI port as a rising-edge-active device when it is in slave mode
 */
#define _SPI_SET_SLAVE_SELECT_RISING_EDGE_ACTIVE(port) ((port)->SSR = (port)->SSR & (~(SPI_SSR_SS_LTRIG_Msk|SPI_SSR_SS_LVL_Msk)) | SPI_SSR_SS_LVL_Msk)

/**
 * @details    Get the level trigger Accomplish flag of the user-specified SPI port
 */
#define _SPI_GET_SLAVE_LEVEL_TRIG_SUCCESS_FLAG(port) (((port)->SSR & SPI_SSR_LTRIG_FLAG_Msk)>>SPI_SSR_LTRIG_FLAG_Pos)

/**
 * @details    Set the slave select control bit to 1
 */
#define _SPI_SET_SLAVE_SELECT_CNTRL_BIT(port) ((port)->SSR |= SPI_SSR_SSR_Msk)

/**
 * @details    Clear the slave select control bit to 0
 */
#define _SPI_CLR_SLAVE_SELECT_CNTRL_BIT(port) ((port)->SSR &= (~SPI_SSR_SSR_Msk))

/**
 * @details    Get the SPI busy status of the user-specified SPI port
 */
#define _SPI_GET_BUSY_STATUS(port) ((port)->CNTRL & SPI_CNTRL_GO_BUSY_Msk)

/**
 * @details    Set the SPI GO_BUSY bit of the user-specified SPI port
 */
#define _SPI_SET_GO(port) ((port)->CNTRL |= SPI_CNTRL_GO_BUSY_Msk)

/**
 * @details    Clear the SPI GO_BUSY bit of the user-specified SPI port
 */
#define _SPI_CLR_GO(port) ((port)->CNTRL &= (~SPI_CNTRL_GO_BUSY_Msk))

/**
 * @details    Enable the burst transfer mode of the user-specified SPI port
 */
#define _SPI_ENABLE_BURST_MODE(port) ((port)->CNTRL |= (1<<SPI_CNTRL_TX_NUM_Pos))

/**
 * @details    Disable the burst transfer mode of the user-specified SPI port
 */
#define _SPI_DISABLE_BURST_MODE(port) ((port)->CNTRL &= (~SPI_CNTRL_TX_NUM_Msk))

/**
 * @details    Set the SPI DIVIDER of the user-specified SPI port
 */
#define _SPI_SET_ENGINE_CLK_DIVIDER(port, x) ((port)->DIVIDER = (port)->DIVIDER & (~SPI_DIVIDER_DIVIDER_Msk) | (x))

/**
 * @details    Set the SPI DIVIDER2 of the user-specified SPI port
 */
#define _SPI_SET_DIVIDER2(port, x) ((port)->DIVIDER = (port)->DIVIDER & (~SPI_DIVIDER_DIVIDER2_Msk) | ((x)<<SPI_DIVIDER_DIVIDER2_Pos))

/**
 * @details    Enable the SPI variable clock function of the user-specified SPI port
 */
#define _SPI_ENABLE_VARIABLE_CLOCK_FUNCTION(port) ((port)->CNTRL |= SPI_CNTRL_VARCLK_EN_Msk)

/**
 * @details    Disable the SPI variable clock function of the user-specified SPI port
 */
#define _SPI_DISABLE_VARIABLE_CLOCK_FUNCTION(port) ((port)->CNTRL &= (~SPI_CNTRL_VARCLK_EN_Msk))

/**
 * @details    Enable the SPI unit transfer interrupt
 */
#define _SPI_ENABLE_UNIT_TRANSFER_INT(port) ((port)->CNTRL |= SPI_CNTRL_IE_Msk)

/**
 * @details    Disable the SPI unit transfer interrupt
 */
#define _SPI_DISABLE_UNIT_TRANSFER_INT(port) ((port)->CNTRL &= (~SPI_CNTRL_IE_Msk))

/**
 * @details    Clear the SPI unit transfer interrupt flag
 */
#define _SPI_CLR_UNIT_TRANS_INT_FLAG(port) ((port)->CNTRL |= SPI_CNTRL_IF_Msk  )

/**
 * @details    Get the SPI unit transfer interrupt flag
 */
#define _SPI_GET_UNIT_TRANS_INT_FLAG(port) (((port)->CNTRL & SPI_CNTRL_IF_Msk)>>SPI_CNTRL_IF_Pos )

/**
 * @details    Get the SPI Rx0 data
 */
#define _SPI_GET_RX0_DATA(port) ((port)->RX[0])

/**
 * @details    Get the SPI Rx1 data
 */
#define _SPI_GET_RX1_DATA(port) ((port)->RX[1])

/**
 * @details    Write data to the SPI Tx buffer0
 */
#define _SPI_WRITE_TX_BUFFER0(port, x) ((port)->TX[0] = (x))

/**
 * @details    Write data to the SPI Tx buffer1
 */
#define _SPI_WRITE_TX_BUFFER1(port, x) ((port)->TX[1] = (x))

/**
 * @details    Enable the SPI DIV-ONE function
 */
#define _SPI_ENABLE_DIV_ONE(port) ((port)->CNTRL2 |= SPI_CNTRL2_DIV_ONE_Msk)

/**
 * @details    Disable the SPI DIV-ONE function
 */
#define _SPI_DISABLE_DIV_ONE(port) ((port)->CNTRL2 &= (~SPI_CNTRL2_DIV_ONE_Msk))

/**
 * @details    Enable the SPI 3-wire mode
 */
#define _SPI_ENABLE_3WIRE_MODE(port) ((port)->CNTRL2 |= SPI_CNTRL2_NOSLVSEL_Msk)

/**
 * @details    Disable the 3-wire mode
 */
#define _SPI_DISABLE_3WIRE_MODE(port) ((port)->CNTRL2 &= (~SPI_CNTRL2_NOSLVSEL_Msk))

/**
 * @details    Abort the SPI transfer when the 3-wire mode is enabled
 */
#define _SPI_ABORT_3WIRE_TRANSFER(port) ((port)->CNTRL2 |= SPI_CNTRL2_SLV_ABORT_Msk)

/**
 * @details    Enable the SPI 3-wire mode start interrupt
 */
#define _SPI_ENABLE_3WIRE_START_INT(port) ((port)->CNTRL2 |= SPI_CNTRL2_SSTA_INTEN_Msk)

/**
 * @details    Disable the SPI 3-wire mode start interrupt
 */
#define _SPI_DISABLE_3WIRE_START_INT(port) ((port)->CNTRL2 &= (~SPI_CNTRL2_SSTA_INTEN_Msk))

/**
 * @details    Get the SPI 3-wire mode start interrupt flag
 */
#define _SPI_GET_3WIRE_START_INT_FLAG(port) (((port)->CNTRL2 & SPI_CNTRL2_SLV_START_INTSTS_Msk)>>SPI_CNTRL2_SLV_START_INTSTS_Pos )

/**
 * @details    Clear the SPI 3-wire mode start interrupt flag
 */
#define _SPI_CLR_3WIRE_START_INT_FLAG(port) ((port)->CNTRL2 |= SPI_CNTRL2_SLV_START_INTSTS_Msk  )




/**
 * @brief      Get SPI clock1 frequency (Hz).
 *
 * @param[in]  pSpiPort   Specify the SPI port.
 *
 * @return     SPI clock1 frequency.
 *
 * @details    This function calculates the clock rate of clock1 (SPI engine clock).
 *
 */
static __INLINE uint32_t SPI_GetClock1Freq(SPI_T * pSpiPort)
{
    return (SystemCoreClock>>1)/((pSpiPort->DIVIDER & 0xFFFF)+1);
}

/**
 * @brief      Get SPI clock2 frequency (Hz).
 *
 * @param[in]  pSpiPort   Specify the SPI port.
 *
 * @return     SPI clock2 frequency.
 *
 * @details    This function calculates the clock rate of clock2.
 *
 */
static __INLINE uint32_t SPI_GetClock2Freq(SPI_T * pSpiPort)
{
    return (SystemCoreClock>>1)/(((pSpiPort->DIVIDER & 0xFFFF0000)>>16)+1);
}

/**
  * @} End of SPI Device Function Interface
  */ 

/**
  * @} End of M051 Function Interface
  */ 


#endif

