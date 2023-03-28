/**************************************************************************//**
 * @file     I2C.h   
 * @version  V2.1
 * $Revision: 4 $
 * $Date: 12/08/10 3:12p $
 * @brief    M051 Series I2C Driver Header File
 *
 * @note
 * Copyright (C) 2011 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#ifndef __I2C_H__
#define __I2C_H__

#include "M051Series.h"


/** @addtogroup M051_FUNC M051 Function Interface
  * @{
  */

/** @addtogroup I2C_FUNC I2C Device Function Interface
  * @{
  */



/*---------------------------------------------------------------------------------------------------------*/
/*  I2CON constant definitions.                                                                            */
/*---------------------------------------------------------------------------------------------------------*/
#define I2C_I2CON_STA_STO_SI        0x38UL /*!< I2CON setting for I2C control bits. It would set STA, STO and SI bits     */
#define I2C_I2CON_STA_STO_SI_AA     0x3CUL /*!< I2CON setting for I2C control bits. It would set STA, STO, SI and AA bits */
#define I2C_I2CON_STA_SI            0x28UL /*!< I2CON setting for I2C control bits. It would set STA and SI bits          */
#define I2C_I2CON_STA_SI_AA         0x2CUL /*!< I2CON setting for I2C control bits. It would set STA, SI and AA bits      */
#define I2C_I2CON_STO_SI            0x18UL /*!< I2CON setting for I2C control bits. It would set STO and SI bits          */
#define I2C_I2CON_STO_SI_AA         0x1CUL /*!< I2CON setting for I2C control bits. It would set STO, SI and AA bits      */
#define I2C_I2CON_SI                0x08UL /*!< I2CON setting for I2C control bits. It would set SI bit                   */
#define I2C_I2CON_SI_AA             0x0CUL /*!< I2CON setting for I2C control bits. It would set SI and AA bits           */
#define I2C_I2CON_STA               0x20UL /*!< I2CON setting for I2C control bits. It would set STA bit                  */
#define I2C_I2CON_AA                0x04UL /*!< I2CON setting for I2C control bits. It would set AA bit                   */


/*---------------------------------------------------------------------------------------------------------*/
/*  I2CLK constant definitions                                                                             */
/*---------------------------------------------------------------------------------------------------------*/
#define I2C_I2CLK_DIV4(x)    ((x)-1)    /*!< I2CLK setting for I2C clock divide. I2C clock = APBCLK / (x*4). 5 <= x <= 256 */


/*---------------------------------------------------------------------------------------------------------*/
/*  Define Macro                                                                                           */
/*---------------------------------------------------------------------------------------------------------*/

/**
 * @brief      Generate START signal to I2C bus
 *
 * @param[in]  port    I2C port 
 *
 * @return     None
 *
 * @details    This macro is used to generate START signal to specified I2C port 
 */
#define _I2C_START(port) _I2C_SET_CONTROL_BITS(port, I2C_I2CON_STA_SI)

/**
 * @brief      Waiting for I2C action done
 *
 * @param[in]  port    I2C port 
 *
 * @return     None
 *
 * @details    This function is used to wait I2C action done by polling SI bit of I2CON. 
 */
#define _I2C_WAIT_READY(port) while(((port)->I2CON & I2C_I2CON_SI_Msk) == 0)

/**
 * @brief      Generate STOP signal to I2C bus
 *
 * @param[in]  port    I2C port 
 *
 * @return     None
 *
 * @details    This macro is used to generate STOP signal to specified I2C port  
 */
#define _I2C_STOP(port) _I2C_SET_CONTROL_BITS(port, I2C_I2CON_STO_SI)

/**
 * @brief      Send a byte to I2C bus
 *
 * @param[in]  port    I2C port 
 * @param[in]  u8Data  The data to send to I2C bus. 
 *
 * @return     None
 *
 * @details    This macro is used to write a byte to specified I2C port  
 */
#define _I2C_WRITE(port, u8Data) ((port)->I2CDAT = (u8Data));((port)->I2CON = I2C_I2CON_ENS1_Msk | I2C_I2CON_SI_Msk)

/**
 * @brief      Read a byte from I2C bus and return ACK
 *
 * @param[in]  port    I2C port 
 *
 * @return     The byte got form I2C bus
 *
 * @details    This macro is used to read a byte from specified I2C port and return ACK
 */
#define _I2C_READ_ACK(port)         I2C_ReadAck(port)

/**
 * @brief      Read a byte from I2C bus and return NAK
 *
 * @param[in]  port    I2C port 
 *
 * @return     The byte got form I2C bus
 *
 * @details    This macro is used to read a byte from specified I2C port and return NAK
 */
#define _I2C_READ_NAK(port)     I2C_ReadNak(port)


/**
 * @brief      Set I2C Control Bits
 *
 * @param[in]  port    I2C port
 * @param[in]  u8Ctrl  I2C Control Bit. Including
 *                     -I2C_I2CON_STA_STO_SI   : Set STA, STO and SI bits
 *                     -I2C_I2CON_STA_STO_SI_AA: Set STA, STO, SI and AA bits 
 *                     -I2C_I2CON_STA_SI       : Set STA and SI bits      
 *                     -I2C_I2CON_STA_SI_AA    : Set STA, SI and AA bits
 *                     -I2C_I2CON_STO_SI       : Set STO and SI bits        
 *                     -I2C_I2CON_STO_SI_AA    : Set STO, SI and AA bits    
 *                     -I2C_I2CON_SI           : Set SI bit               
 *                     -I2C_I2CON_SI_AA        : Set SI and AA bits
 *                     -I2C_I2CON_STA          : Set STA bit               
 *                     -I2C_I2CON_AA           : Set AA bit
 *
 * @return     None
 *
 * @details    To set I2C control bits include STA, STO, SI and AA in I2C CONTROL REGISTER (I2CON)
 *
 * @note
 *             This function will enable EI bit. If user does not need to use interrupt, 
 *             user can disable corresponding NVIC bit of I2C.
 */
#define _I2C_SET_CONTROL_BITS(port, u8Ctrl)     ((port)->I2CON = I2C_I2CON_EI_Msk | I2C_I2CON_ENS1_Msk | (u8Ctrl))

/**
 * @brief      Get I2C Bus Clock 
 *
 * @param[in]  port    I2C port 
 *
 * @return     The actual I2C Bus clock in Hz
 *
 * @details    To get the actual I2C Bus Clock frequency.
 */
#define _I2C_GET_BUS_CLOCK(port)         (SystemCoreClock/((((port)->I2CLK)+1)<<2))       

/**
 * @brief      Set 7-bit Slave Address of I2CADDR0
 *
 * @param[in]  port         I2C port 
 * @param[in]  u8SlaveAddr  7-bit slave address
 *
 * @return     None
 *
 * @details    This function is used to set 7-bit slave addresses in I2C SLAVE ADDRESS REGISTER (I2CADDR0). 
 */
#define _I2C_SET_SLAVE_ADDRESS_0(port, u8SlaveAddr)    ((port)->I2CADDR0 = ((port)->I2CADDR0 & ~I2C_I2CADDR_I2CADDR_Msk) | ((u8SlaveAddr) << I2C_I2CADDR_I2CADDR_Pos))     

/**
 * @brief      Set 7-bit Slave Address of I2CADDR1
 *
 * @param[in]  port         I2C port 
 * @param[in]  u8SlaveAddr  7-bit slave address
 *
 * @return     None
 *
 * @details    This function is used to set 7-bit slave addresses in I2C SLAVE ADDRESS REGISTER (I2CADDR1). 
 */
#define _I2C_SET_SLAVE_ADDRESS_1(port, u8SlaveAddr)    ((port)->I2CADDR1 = ((port)->I2CADDR1 & ~I2C_I2CADDR_I2CADDR_Msk) | ((u8SlaveAddr) << I2C_I2CADDR_I2CADDR_Pos))     

/**
 * @brief      Set 7-bit Slave Address of I2CADDR2
 *
 * @param[in]  port         I2C port 
 * @param[in]  u8SlaveAddr  7-bit slave address 
 *
 * @return     None
 *
 * @details    This function is used to set 7-bit slave addresses in I2C SLAVE ADDRESS REGISTER (I2CADDR2). 
 */
#define _I2C_SET_SLAVE_ADDRESS_2(port, u8SlaveAddr)    ((port)->I2CADDR2 = ((port)->I2CADDR2 & ~I2C_I2CADDR_I2CADDR_Msk) | ((u8SlaveAddr) << I2C_I2CADDR_I2CADDR_Pos))     

/**
 * @brief      Set 7-bit Slave Address of I2CADDR3
 *
 * @param[in]  port         I2C port 
 * @param[in]  u8SlaveAddr  7-bit slave address
 *
 * @return     None
 *
 * @details    This function is used to set 7-bit slave addresses in I2C SLAVE ADDRESS REGISTER (I2CADDR3). 
 */
#define _I2C_SET_SLAVE_ADDRESS_3(port, u8SlaveAddr)    ((port)->I2CADDR3 = ((port)->I2CADDR3 & ~I2C_I2CADDR_I2CADDR_Msk) | ((u8SlaveAddr) << I2C_I2CADDR_I2CADDR_Pos))     


/**
 * @brief      Set 7-bit Slave Address Mask in I2CADM0
 *
 * @param[in]  port          I2C port 
 * @param[in]  u8SlaAddrMsk  7-bit data to mask the corresponding slave address bit(s) of I2CADDR0
 *
 * @return     None
 *
 * @details    This function is used to set 7-bit slave addresses mask in I2C SLAVE ADDRESS MASK REGISTER (I2CADM0). 
 */
#define _I2C_SET_SLAVE_ADDRESS_MASK_0(port, u8SlaAddrMsk)    ((port)->I2CADM0 = (u8SlaAddrMsk) << I2C_I2CADM_I2CADM_Pos)     

/**
 * @brief      Set 7-bit Slave Address Mask in I2CADM1
 *
 * @param[in]  port          I2C port 
 * @param[in]  u8SlaAddrMsk  7-bit data to mask the corresponding slave address bit(s) of I2CADDR1
 *
 * @return     None
 *
 * @details    This function is used to set 7-bit slave addresses mask in I2C SLAVE ADDRESS MASK REGISTER (I2CADM1). 
 */
#define _I2C_SET_SLAVE_ADDRESS_MASK_1(port, u8SlaAddrMsk)    ((port)->I2CADM1 = (u8SlaAddrMsk) << I2C_I2CADM_I2CADM_Pos)     

/**
 * @brief      Set 7-bit Slave Address Mask in I2CADM2
 *
 * @param[in]  port          I2C port 
 * @param[in]  u8SlaAddrMsk  7-bit data to mask the corresponding slave address bit(s) of I2CADDR2
 *
 * @return     None
 *
 * @details    This function is used to set 7-bit slave addresses mask in I2C SLAVE ADDRESS MASK REGISTER (I2CADM2). 
 */
#define _I2C_SET_SLAVE_ADDRESS_MASK_2(port, u8SlaAddrMsk)    ((port)->I2CADM2 = (u8SlaAddrMsk) << I2C_I2CADM_I2CADM_Pos)     

/**
 * @brief      Set 7-bit Slave Address Mask in I2CADM3
 *
 * @param[in]  port          I2C port 
 * @param[in]  u8SlaAddrMsk  7-bit data to mask the corresponding slave address bit(s) of I2CADDR3
 *
 * @return     None
 *
 * @details    This function is used to set 7-bit slave addresses mask in I2C SLAVE ADDRESS MASK REGISTER (I2CADM3). 
 */
#define _I2C_SET_SLAVE_ADDRESS_MASK_3(port, u8SlaAddrMsk)    ((port)->I2CADM3 = (u8SlaAddrMsk) << I2C_I2CADM_I2CADM_Pos)    


#define _I2C_ENABLE_TIMEOUT_COUNTER(port)  ((port)->I2CTOC |=  I2C_I2CTOC_ENTI_Msk) /*!< Enable 14-bit Time-out Counter  */
#define _I2C_DISABLE_TIMEOUT_COUNTER(port) ((port)->I2CTOC &= ~I2C_I2CTOC_ENTI_Msk) /*!< Disable 14-bit Time-out Counter */
#define _I2C_ENABLE_GENERAL_CALL(port)    ((port)->I2CADDR0 |=  I2C_I2CADDR_GC_Msk) /*!< Enable General Call Mode  */
#define _I2C_DISABLE_GENERAL_CALL(port)   ((port)->I2CADDR0 &= ~I2C_I2CADDR_GC_Msk) /*!< Disable General Call Mode */

/**
 * @brief      Read a byte from I2C bus and return ACK
 *
 * @param[in]  port    I2C port. 
 *
 * @return     The byte got form I2C bus
 *
 * @details    This function is used to read a byte from specified I2C port and return ACK
 */
static __INLINE uint8_t I2C_ReadAck(I2C_T *port)
{
    (port)->I2CDAT= 0xFF;
    (port)->I2CON = I2C_I2CON_ENS1_Msk | I2C_I2CON_SI_Msk | I2C_I2CON_AA_Msk;
    _I2C_WAIT_READY(port);
    return (port)->I2CDAT;
}

/**
 * @brief      Read a byte from I2C bus and return NAK
 *
 * @param[in]  port    I2C port. 
 *
 * @return     The byte got form I2C bus
 *
 * @details    This function is used to read a byte from specified I2C port and return NAK
 */
static __INLINE uint8_t I2C_ReadNak(I2C_T *port)
{
    (port)->I2CDAT= 0xFF;
    (port)->I2CON = I2C_I2CON_ENS1_Msk | I2C_I2CON_SI_Msk;
    _I2C_WAIT_READY(port);
    return (port)->I2CDAT;
}


/**
 * @brief      Set I2C Clock Divider
 *
 * @param[in]  tI2C         I2C port 
 * @param[in]  u32BusClock  The target I2C Bus Clock in Hz
 *
 * @return     None
 *                                
 * @details    To set proper clock divider in I2C CLOCK DIVIDED REGISTER (I2CLK) 
 *             according to the target I2C Bus Clock. I2C bus clock = PCLK / (4x(divider+1)).
 *
 * @note
 *             The maximum of I2C bus clock is 1 MHz and the minimum of the divider is 4.
 */
static __INLINE void I2C_SetBusClockFreq(I2C_T * tI2C, uint32_t u32BusClock)
{
    uint32_t u32divider;     
 
    u32divider = (uint32_t) (((SystemCoreClock * 10)/(u32BusClock * 4) + 5) / 10 - 1);
                        
    tI2C->I2CLK = (u32divider & ~0xFF)? 0xFF:((u32divider & ~0x03)? u32divider: 4);
}

/**
  * @} End of I2C Device Function Interface
  */ 

/**
  * @} End of M051 Function Interface
  */ 


#endif

