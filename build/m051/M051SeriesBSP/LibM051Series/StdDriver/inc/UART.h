/**************************************************************************//**
 * @file     UART.h
 * @version  V2.00
 * $Revision: 5 $
 * $Date: 12/08/10 3:12p $
 * @brief    M051 Series UART Interface Controller Driver Header File
 *
 * @note                                                                              
 * Copyright (C) 2011 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#ifndef __UART_H__
#define __UART_H__

#include "M051Series.h"


/** @addtogroup M051_FUNC M051 Function Interface
  * @{
  */

/** @addtogroup UART_FUNC UART Device Function Interface
  * @{
  */


/*---------------------------------------------------------------------------------------------------------*/
/* UA_FCR constants definitions                                                                            */
/*---------------------------------------------------------------------------------------------------------*/

#define UART_FCR_RFITL_1BYTE      (0x0 << UART_FCR_RFITL_Pos)   /*!< UA_FCR setting to set RX FIFO Trigger Level to 1 bit */
#define UART_FCR_RFITL_4BYTES     (0x1 << UART_FCR_RFITL_Pos)   /*!< UA_FCR setting to set RX FIFO Trigger Level to 4 bits */
#define UART_FCR_RFITL_8BYTES     (0x2 << UART_FCR_RFITL_Pos)   /*!< UA_FCR setting to set RX FIFO Trigger Level to 8 bits */
#define UART_FCR_RFITL_14BYTES    (0x3 << UART_FCR_RFITL_Pos)   /*!< UA_FCR setting to set RX FIFO Trigger Level to 14 bits */

#define UART_FCR_RTS_TRI_LEV_1BYTE      (0x0 << UART_FCR_RTS_TRI_LEV_Pos)  /*!< UA_FCR setting to set RTS Trigger Level to 1 bit */
#define UART_FCR_RTS_TRI_LEV_4BYTES     (0x1 << UART_FCR_RTS_TRI_LEV_Pos)  /*!< UA_FCR setting to set RTS Trigger Level to 4 bits */
#define UART_FCR_RTS_TRI_LEV_8BYTES     (0x2 << UART_FCR_RTS_TRI_LEV_Pos)  /*!< UA_FCR setting to set RTS Trigger Level to 8 bits */
#define UART_FCR_RTS_TRI_LEV_14BYTES    (0x3 << UART_FCR_RTS_TRI_LEV_Pos)  /*!< UA_FCR setting to set RTS Trigger Level to 14 bits */

/*---------------------------------------------------------------------------------------------------------*/
/* UA_LCR constants definitions                                                                            */
/*---------------------------------------------------------------------------------------------------------*/
#define UART_WORD_LEN_5     (0) /*!< UA_LCR setting to set UART word length to 5 bits */
#define UART_WORD_LEN_6     (1) /*!< UA_LCR setting to set UART word length to 6 bits */
#define UART_WORD_LEN_7     (2) /*!< UA_LCR setting to set UART word length to 7 bits */
#define UART_WORD_LEN_8     (3) /*!< UA_LCR setting to set UART word length to 8 bits */

#define UART_PARITY_NONE    (0x0 << UART_LCR_PBE_Pos) /*!< UA_LCR setting to set UART as no parity   */
#define UART_PARITY_ODD     (0x1 << UART_LCR_PBE_Pos) /*!< UA_LCR setting to set UART as odd parity  */
#define UART_PARITY_EVEN    (0x3 << UART_LCR_PBE_Pos) /*!< UA_LCR setting to set UART as even parity */
#define UART_PARITY_MARK    (0x5 << UART_LCR_PBE_Pos) /*!< UA_LCR setting to keep parity bit as '1'  */
#define UART_PARITY_SPACE   (0x7 << UART_LCR_PBE_Pos) /*!< UA_LCR setting to keep parity bit as '0'  */

#define UART_STOP_BIT_1     (0x0 << UART_LCR_NSB_Pos) /*!< UA_LCR setting for one stop bit  */
#define UART_STOP_BIT_1_5   (0x1 << UART_LCR_NSB_Pos) /*!< UA_LCR setting for 1.5 stop bit when 5-bit word length  */
#define UART_STOP_BIT_2     (0x1 << UART_LCR_NSB_Pos) /*!< UA_LCR setting for two stop bit when 6, 7, 8-bit word length */



/*---------------------------------------------------------------------------------------------------------*/
/* UART RTS LEVEL TRIGGER constants definitions                                                            */
/*---------------------------------------------------------------------------------------------------------*/
#define UART_RTS_IS_HIGH_LEV_TRG (0x1 << UART_MCR_LEV_RTS_Pos) /*!< Set RTS is High Level Tigger   */
#define UART_RTS_IS_LOW_LEV_TRG  (0x0 << UART_MCR_LEV_RTS_Pos) /*!< Set RTS is Low Level Tigger    */

/*---------------------------------------------------------------------------------------------------------*/
/* UA_FUNC_SEL constants definitions                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
#define UART_FUNC_SEL_UART  (0x0 << UART_FUN_SEL_FUN_SEL_Pos) /*!< UA_FUNC_SEL setting to set UART Function  (Default) */
#define UART_FUNC_SEL_LIN   (0x1 << UART_FUN_SEL_FUN_SEL_Pos) /*!< UA_FUNC_SEL setting to set LIN Funciton             */
#define UART_FUNC_SEL_IrDA  (0x2 << UART_FUN_SEL_FUN_SEL_Pos) /*!< UA_FUNC_SEL setting to set IrDA Function            */
#define UART_FUNC_SEL_RS485 (0x3 << UART_FUN_SEL_FUN_SEL_Pos) /*!< UA_FUNC_SEL setting to set RS485 Function           */

/*---------------------------------------------------------------------------------------------------------*/
/* BAUD constants definitions                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
#define UART_BAUD_MODE0                                 (0)
#define UART_BAUD_MODE2                                 (UART_BAUD_DIV_X_EN_Msk | UART_BAUD_DIV_X_ONE_Msk)


/**
 * @brief      Compute Baud Rate Divider Settings by mode 0
 *
 * @param[in]  u32SrcFreq      UART clock source(UART_S) divides by UART divider(UART_N)
 * @param[in]  u32BaudRate     Target bitrate (Hz)
 *    
 * @return     None
 *    
 * @details    The function is used to get baud-rate divider settings by mode 0  based on your settings.\n
 *    
 *             Example: UART clock source from PLL(48MHz) and UART clock divider is set 2.\n 
 *                      If desired baudrate is 115200, you can call UART_BAUD_DIV_MODE2(24000000,115200);
 *                           
 */
#define UART_BAUD_DIV_MODE0(u32SrcFreq, u32BaudRate)    (((u32SrcFreq + (u32BaudRate*8)) / u32BaudRate >> 4)-2)

/**
 * @brief      Compute Baud Rate Divider Settings by mode 2
 *
 * @param[in]  u32SrcFreq      UART clock source(UART_S) was divided by UART divider(UART_N)
 * @param[in]  u32BaudRate     Target bitrate (Hz)
 *    
 * @return     None
 *    
 * @details    The function is used to get baud rate divider settings by mode 2 based on your settings.\n
 *    
 *             Example: UART clock source from PLL(48MHz) and UART clock divider is set 2.\n 
 *                      If desired baudrate is 115200, you can call UART_BAUD_DIV_MODE2(24000000,115200);
 *                           
 */

#define UART_BAUD_DIV_MODE2(u32SrcFreq, u32BaudRate)    (((u32SrcFreq + (u32BaudRate/2)) / u32BaudRate)-2)     


/*---------------------------------------------------------------------------------------------------------*/
/* Data Transmit/Receive                                                                                   */
/*---------------------------------------------------------------------------------------------------------*/
/**
 * @brief      Send one data byte from specify uart port
 *
 * @param[in]  UART         Structure pointer of UART Channel selected, should be:
 *                          - UART0 : UART Channel 0
 *                          - UART1 : UART Channel 1 
 * @param[in]  u8Data       Data byte to transmit 
 *                          
 * @return     None
 *
 * @details    The function is used to send UART data.\n
 *             Example: _UART_SENDBYTE(UART0,0x55)
 */
#define _UART_SENDBYTE(UART,u8Data)   ((UART)->THR = (u8Data))

/**
 * @brief      Receive one data byte from specify uart port
 *
 * @param[in]  UART         Structure pointer of UART Channel selected, should be: 
 *                          - UART0 : UART Channel 0
 *                          - UART1 : UART Channel 1 
 *
 * @return     The oldest data byte in RX FIFO 
 *
 * @details    The function is used to get one data byte from RX FIFO of specified UART port.\n
 *             Example: _UART_RECEIVEBYTE(UART0,u8Data)
 */
#define _UART_RECEIVEBYTE(UART,u8Data)  ((u8Data) = (UART)->RBR)

/*---------------------------------------------------------------------------------------------------------*/
/* Baud Rate Relative                                                                                      */
/*---------------------------------------------------------------------------------------------------------*/
/**
 * @brief      Get UART Clock Source Freq. (Hz)
 *
 * @param[in]  None  
 *
 * @return     UART Clock Source Freq.
 *
 * @details    The function will read UART_S bit field in CLKSEL1 register to determine UART clock source freq.\n
 *             Before calling the function, please use SystemCoreClockUpdate to update system clock freq.
 */

static __INLINE uint32_t UART_GetClockFreq()
{
    uint8_t u8UartClkSrcSel;
    uint32_t u32ClkTbl[4] = {__XTAL, 0, 0, __IRC22M};    
    u32ClkTbl[1] = PllClock;
    u8UartClkSrcSel = (SYSCLK->CLKSEL1 & SYSCLK_CLKSEL1_UART_S_Msk) >> SYSCLK_CLKSEL1_UART_S_Pos;
    return u32ClkTbl[u8UartClkSrcSel];
}

/**
 * @brief      Get UART Clock Divider. (Hz)
 *
 * @param[in]  None  
 *
 * @return     UART Clock Divider.
 *
 * @details    The function will read UART_N bit field in CLKDIV register to determine current UART clock divider.
 *             
 */

static __INLINE uint32_t UART_GetClockDivider()
{
    return ((SYSCLK->CLKDIV & SYSCLK_CLKDIV_UART_N_Msk) >> SYSCLK_CLKDIV_UART_N_Pos) + 1;
}


/**
 * @brief      Using Baud Rate Equation Mode 0 to get divisor.     (DIV_X_EN=0, DIV_X_ONE=0) 
 *
 * @param[in]  u32BaudRate  Baud rate setting for UART   
 *
 * @return     UART baud rate divisor.
 *
 * @details    The function will get UART baud rate divisor by equation. (UART_CLK / [16 * (A+2)])
 *             
 */
static __INLINE uint32_t UART_GetDivisor_ByMode0(uint32_t u32BaudRate)
{
    return ((((UART_GetClockFreq()/UART_GetClockDivider()) / u32BaudRate + 1) >> 4) -2) ;
}


/**
 * @brief      Using Baud Rate Equation Mode 1 to get divisor. (DIV_X_EN=1, DIV_X_ONE=0) 
 *
 * @param[in]  u32BaudRate  Baud rate setting for UART   
 * @param[in]  u8Divider_X  Divider X
 *
 * @return     UART baud rate divisor.
 *
 * @details    The function will get UART baud rate divisor by equation. (UART_CLK / [(B+1) * (A+2)] )
 *             
 */                      
static __INLINE uint32_t UART_GetDivisor_ByMode1(uint32_t u32BaudRate,uint8_t u8Divider_X)
{
    return ((((UART_GetClockFreq()/UART_GetClockDivider()) / u32BaudRate + 1) / u8Divider_X) -2) ;
}                                        

/**
 * @brief      Using Baud Rate Equation Mode 2 to get divisor.(DIV_X_EN=1, DIV_X_ONE=1) 
 *
 * @param[in]  u32BaudRate  Baud rate setting for UART   
 *
 * @return     UART baud rate divisor.
 *
 * @details    The function will get UART baud rate divisor by equation. (UART_CLK / 1 * (A+2)] )
 *             
 */
static __INLINE uint32_t UART_GetDivisor_ByMode2(uint32_t u32BaudRate)
{
    return (((UART_GetClockFreq()/UART_GetClockDivider() * 2) / u32BaudRate + 1) >> 1);
}

/**
 * @brief      Set UART Baud Rate
 *
 * @param[in]  UART         Structure pointer of UART Channel  
 *                          - UART0 : UART Channel 0
 *                          - UART1 : UART Channel 1 
 * @param[in]  u32BaudRate  Baud rate setting for UART 
 *                          - Bit per sec. : ex.115200
 * @return     None
 *
 * @details    The function uses mode 2 (DIV_X_EN=1, DIV_X_ONE=1) to set baud rate.\n
 *             Example: UART_SetBaudRate(UART0, 115200)
 */
static __INLINE void UART_SetBaudRate(UART_T *UART, uint32_t u32BaudRate)
{
    UART->BAUD = UART_BAUD_MODE2 | UART_GetDivisor_ByMode2(u32BaudRate); 
}



/*---------------------------------------------------------------------------------------------------------*/
/* FORMAT CONTROL                                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
/**
 * @brief      Set UART line control 
 *
 * @param[in]  UART         Structure pointer of UART Channel selected,should be:  
 *                          - UART0 : UART Channel 0
 *                          - UART1 : UART Channel 1 
 * @param[in]  u32PortSettings  Line control value: Use "|" to combine your settings
 *                          - Word Length Select : eg. UART_WORD_LEN_5 
 *                          - Parity Bit         : eg. UART_PARITY_NONE
 *                          - Stop Bit           : eg. UART_STOPBIT_1
 * @return     None
 *
 * @details    The function is used to set UART data format (Word Length Select /Parity Bit/Stop Bit).\n
 *             Example: _UART_SET_DATA_FORMAT(UART0, UART_WORD_LEN_5 | UART_PARITY_NONE | UART_STOPBIT_1)
 */
#define _UART_SET_DATA_FORMAT(UART, u32PortSettings)  ((UART)->LCR = (u32PortSettings))



/**
 * @brief      Set UART transmit delay time 
 *
 * @param[in]  UART         Structure pointer of UART Channel selected,should be:  
 *                          - UART0 : UART Channel 0
 *                          - UART1 : UART Channel 1 
 * @param[in]  DelayTime  N-th uart bit-time between each transmission
 *                                                                
 * @return     None
 *
 * @details    The function is used to set N-th uart bit-time between each transmission. \n
 *             Example: _UART_SET_TX_DELAYTIME(UART0, 10)
 */

#define _UART_SET_TX_DELAYTIME(UART,DelayTime)    ((UART)->TOR = ((UART)->TOR &~UART_TOR_DLY_Msk)|((DelayTime)<<UART_TOR_DLY_Pos))  



/**
 * @brief      Set time-out counter
 *
 * @param[in]  UART         Structure pointer of UART Channel  
 *                          - UART0 : UART Channel 0
 *                          - UART1 : UART Channel 1 
 * @param[in]  u32Count     TimeOutCounter:  0~255
 *
 * @return     None
 *                                                           
 * @details    The function is used to access TOR[7:0] to control timeout counter.\n
 *             When there is un-read data in rx-fifo and IER[11] is set, the counter will start up-count.\n
 *             Until the counter each the value, the time-out event will happened.\n
 *             Example: UART_SetTimeOutCounter(UART0, 128)
 *
 * @note       Per count is according to bitrate settings.
 */
                               
static __INLINE void UART_SetTimeOutCounter(UART_T* UART,uint32_t u32Count)
{
    (UART)->TOR = ((UART)->TOR & ~UART_TOR_TOIC_Msk)| (u32Count);
    (UART)->IER |= UART_IER_TIME_OUT_EN_Msk;
}

/*---------------------------------------------------------------------------------------------------------*/
/* FIFO Control                                                                                            */
/*---------------------------------------------------------------------------------------------------------*/
/**
 * @brief      Check specified uart port transmission is over. 
 *
 * @param[in]  UART         Structure pointer of UART Channel  
 *                          - UART0 : UART Channel 0
 *                          - UART1 : UART Channel 1 
 * @retval     0 = The transmission is not over. 
 * @retval     1 = The transmission is over. 
 *
 * @details    The function is used to read FSR[28] to check the TX_FIFO and TX_Shift_Reigster is empty.\n
 *             Example: _UART_IS_TX_EMPTY(UART0)
 */

#define _UART_IS_TX_EMPTY(UART)    (((UART)->FSR & UART_FSR_TE_FLAG_Msk)>>UART_FSR_TE_FLAG_Pos) 


/**
 * @brief      Wait specified uart port transmission is over 
 *
 * @param[in]  UART         Structure pointer of UART Channel  
 *                          - UART0 : UART Channel 0
 *                          - UART1 : UART Channel 1 
 *
 * @return     None 
 *
 * @details    The function is used to polling FSR[28] and waiting the TX_FIFO and 
 *             TX_Shift_Reigster is empty. Wait FSR[28] bit is set to exit the while loop.\n
 *             Example: _UART_WAIT_TX_EMPTY(UART0)
 */
#define _UART_WAIT_TX_EMPTY(UART)        while(!((((UART)->FSR) & UART_FSR_TE_FLAG_Msk) >> UART_FSR_TE_FLAG_Pos))


                                              
/**
 * @brief      Check RDA_IF is set or not
 *
 * @param[in]  UART         Structure pointer of UART Channel  
 *                          - UART0 : UART Channel 0
 *                          - UART1 : UART Channel 1 
 *
 * @retval     0 = the number of bytes in the RX FIFO is less than the RFITL  
 * @retval     1 = The number of bytes in the RX FIFO equals or larger than RFITL
 *
 * @details    The function is used to read RDA_IF to check the number of bytes in the RX FIFO.
 *             
 *             Example: _UART_IS_RX_READY(UART0)
 */

#define _UART_IS_RX_READY(UART)    (((UART)->ISR & UART_ISR_RDA_IF_Msk)>>UART_ISR_RDA_IF_Pos)


/**
 * @brief      Check there is un-read data in RX FIFO
 *
 * @param[in]  UART         Structure pointer of UART Channel  
 *                          - UART0 : UART Channel 0
 *                          - UART1 : UART Channel 1 
 *
 * @retval     The number of N data bytes in RX FIFO.
 *
 * @details    The function is used to read FSR[13:8] RX FIFO POINTER to check 
 *             there is un-read data int RX_FIFO.\n
 *             Example: _UART_GET_RX_POINTER(UART0)
 */
#define _UART_GET_RX_POINTER(UART)    (((UART)->FSR & UART_FSR_RX_POINTER_Msk) >> UART_FSR_RX_POINTER_Pos)


/**
 * @brief      Check there is un-transmited data in TX FIFO
 *
 * @param[in]  UART         Structure pointer of UART Channel  
 *                          - UART0 : UART Channel 0
 *                          - UART1 : UART Channel 1 
 *
 * @retval     The number of N data bytes in TX FIFO.
 *
 * @details    The function is used to read FSR[21:16] TX FIFO POINTER to check 
 *             there is un-transmited data int TX_FIFO.\n
 *             Example: _UART_GET_TX_POINTER(UART0)
 */
#define _UART_GET_TX_POINTER(UART)    (((UART)->FSR & UART_FSR_TX_POINTER_Msk) >> UARTFSR_TX_POINTER_Pos)


/**
 * @brief      Check TX FIFO is full or not 
 *
 * @param[in]  UART         Structure pointer of UART Channel  
 *                          - UART0 : UART Channel 0
 *                          - UART1 : UART Channel 1 
 *
 * @retval     1 = TX FIFO is full
 * @retval     0 = TX FIFO is not full
 *
 * @details    The function is used to read FSR[23] to check tx fifo is full or not.
 *             If writing data to tx fifo when it is full,buffer error will happen.
 *             Example: _UART_IS_TX_FULL(UART0)
 */
#define _UART_IS_TX_FULL(UART)   (((UART)->FSR & UART_FSR_TX_FULL_Msk)>>UART_FSR_TX_FULL_Pos)

/**
 * @brief      Check RX FIFO is full or not 
 *
 * @param[in]  UART         Structure pointer of UART Channel  
 *                          - UART0 : UART Channel 0
 *                          - UART1 : UART Channel 1 
 *
 * @retval     1 = RX FIFO is full
 * @retval     0 = RX FIFO is not full
 *
 * @details    The function is used to read FSR[15] to check rx fifo is full or not.
 *             If receving a new data to rx fifo when it is full,buffer error will happen.
 *             Example: _UART_IS_RX_FULL(UART0)
 */

#define _UART_IS_RX_FULL(UART)   (((UART)->FSR & UART_FSR_RX_FULL_Msk)>>UART_FSR_RX_FULL_Pos)


/**
 * @brief      Reset TX/RX FIFO
 *
 * @param[in]  UART         Structure pointer of UART Channel  
 *                          - UART0 : UART Channel 0
 *                          - UART1 : UART Channel 1 
 * @param[in]  uPara      Reset FIFO Select  (Uses "OR" to add your settings)
 *                          - UART_FCR_TFR_Msk : TX FIFO 
 *                          - UART_FCR_RFR_Msk : RX FIFO 
 * @return     None
 *
 * @details    The function is used to write FCR[2:1] to reset TX or RX FIFO.\n
 *             Example: _UART_FLUSH_FIFO(UART0,UART_TX_FIFO|UART_RX_FIFO)
 *
 * @note       After user call the function, data in tx or rx fifo in specified UART port will be cleared.
 */

#define _UART_FLUSH_FIFO(UART,uPara)    ((UART)->FCR |= (uPara))


/**
 * @brief      Set Rx trigger level
 *
 * @param[in]  UART         Structure pointer of UART Channel  
 *                          - UART0 : UART Channel 0
 *                          - UART1 : UART Channel 1 
 * @param[in]  eLevel       Rx trigger level
 *                          - UART_FCR_RFITL_1BYTE   : RDA Trigger Level is 1 byte   
 *                          - UART_FCR_RFITL_4BYTES  : RDA Trigger Level is 4 byte  
 *                          - UART_FCR_RFITL_8BYTES  : RDA Trigger Level is 8 byte 
 *                          - UART_FCR_RFITL_14BYTES : RDA Trigger Level is 14 byte 
 * @return     None
 *                                                           
 * @details    The function is used to access FCR[7:4] to control rx trigger level.\n
 *             Example: _UART_SET_RX_TRG_LEV(UART0, UART_RX_TRG_LEV_8)
 *
 * @note       Rx trigger level will effort RDA_IF and RDA_INT.
 */
#define _UART_SET_RX_TRG_LEV(UART,eLevel)    ((UART)->FCR = ((UART)->FCR & ~UART_FCR_RFITL_Msk)| (eLevel))




/*---------------------------------------------------------------------------------------------------------*/
/* Interrupt Control                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/

/**
 * @brief      Enable specified interrupt
 *
 * @param[in]  UART         Structure pointer of UART Channel  
 *                          - UART0 : UART Channel 0
 *                          - UART1 : UART Channel 1 
 * @param[in]  eIntTypeSel  Interrupt type select
 *                          - UART_IER_LIN_IEN_Msk     : LIN Rx break interrupt  
 *                          - UART_IER_BUF_ERR_IEN_Msk : Buffur Error interrupt   
 *                          - UART_IER_RTO_IEN_Msk     : Rx time-out interrupt       
 *                          - UART_IER_MOS_IEN_Msk     : Modem interrupt           
 *                          - UART_IER_RLS_IEN_Msk     : Rx Line status interrupt                              
 *                          - UART_IER_THRE_IEN_Msk    : Tx empty interrupt          
 *                          - UART_IER_RDA_IEN_Msk     : Rx ready interrupt           
 *
 * @return     None
 *                                                           
 * @details    The function is used to enable specified UART interrupt.\n
 *             Example: _UART_ENABLE_INT(UART0,(eDRVUART_RDA_INT | eDRVUART_RLS_INT)
 *             
 * @note       When enable eDRVUART_THRE_INT interrupt source, the interrup will happen continouly if
 *             TX_FIFO is empty. Remind you that be careful to use it.\n  
 *             Please reference TRM to get more information.
 *
 */

#define _UART_ENABLE_INT(UART,eIntTypeSel)    ((UART)->IER |= (eIntTypeSel))


/**
 * @brief      Disable specified interrupt  
 *
 * @param[in]  UART         Structure pointer of UART Channel  
 *                          - UART0 : UART Channel 0
 *                          - UART1 : UART Channel 1 
 * @param[in]  eIntTypeSel  Interrupt type select
 *                          - UART_IER_LIN_IEN_Msk     : LIN Rx break interrupt  
 *                          - UART_IER_BUF_ERR_IEN_Msk : Buffur Error interrupt   
 *                          - UART_IER_RTO_IEN_Msk     : Rx time-out interrupt       
 *                          - UART_IER_MOS_IEN_Msk     : Modem interrupt           
 *                          - UART_IER_RLS_IEN_Msk     : Rx Line status interrupt                              
 *                          - UART_IER_THRE_IEN_Msk    : Tx empty interrupt          
 *                          - UART_IER_RDA_IEN_Msk     : Rx ready interrupt           
 * @return     None
 *                                                           
 * @details    The function is used to disable UART specified interrupt.\n
 *             Example: _UART_DISABLE_INT(UART0,(UART_RDA_INT | UART_RLS_INT)
 */
#define _UART_DISABLE_INT(UART,eIntSel)    ((UART)->IER &= ~ (eIntSel))


/**
 * @brief      Get specified interrupt flag/status
 *
 * @param[in]  UART         Structure pointer of UART Channel  
 *                          - UART0 : UART Channel 0
 *                          - UART1 : UART Channel 1 
 * @param[in]  eIntTypeFlag  Interrupt Type Flag,should be
 *                          - UART_LIN_INT_FLAG      : LIN Rx break interrupt flag    
 *                          - UART_BUFERR_INT_FLAG   : Buffur Error interrupt flag    
 *                          - UART_TOUT_INT_FLAG     : Rx time-out interrupt flag     
 *                          - UART_MOS_INT_FLAG      : Modem interrupt flag           
 *                          - UART_RLS_INT_FLAG      : Rx Line status interrupt flag 
 *                          - UART_THRE_INT_FLAG     : Tx empty interrupt flag        
 *                          - UART_RDA_INT_FLAG      : Rx ready interrupt flag     
 *
 * @retval     0 = The specified interrupt is not happened. 
 * @retval     1 = The specified interrupt is happened.
 *                                                           
 * @details    The function is used to read ISR[7:0] to get interrupt status.\n
 *             Example: _UART_GET_INT_FLAG(UART0,UART_RLS_INT)
 *
 */                                                                                      
#define _UART_GET_INT_FLAG(UART,eIntTypeFlag) (((UART)->ISR & (eIntTypeFlag))?1:0)



/**
 * @brief      Clear RLS interrupt flag
 *
 * @param[in]  UART         Structure pointer of UART Channel  
 *                          - UART0 : UART Channel 0
 *                          - UART1 : UART Channel 1 
 * @return     None
 *                                                           
 * @details    The function is used to access PEF/FEF/BIF BIT to clear RLS interrupt flag.\n
 *             Example: _UART_CLEAR_RLS_INT_FLAG(UART0)
 *
 */  
#define _UART_CLEAR_RLS_INT_FLAG(UART)  ((UART)->FSR |= (UART_FSR_PEF_Msk|UART_FSR_FEF_Msk|UART_FSR_BIF_Msk))


 /**
 * @brief      Clear Modem interrupt flag
 *
 * @param[in]  UART         Structure pointer of UART Channel  
 *                          - UART0 : UART Channel 0
 *                          - UART1 : UART Channel 1 
 * @return     None
 *                                                           
 * @details    The function is used to access DCTSF to clear Modem interrupt flag.\n
 *             Example: _UART_CLEAR_MODEM_INT_FLAG(UART0)
 *
 */  
#define _UART_CLEAR_MODEM_INT_FLAG(UART)  ((UART)->MSR |= UART_MSR_DCTSF_Msk)

 /**
 * @brief      Clear LIN Rx break interrupt flag
 *
 * @param[in]  UART         Structure pointer of UART Channel  
 *                          - UART0 : UART Channel 0
 *                          - UART1 : UART Channel 1 
 * @return     None
 *                                                           
 * @details    The function is used to access LIN Rx break interrupt flag
 *             to clear Modem interrupt flag.\n
 *             Example: _UART_CLEAR_LIN_RXBREAK_INT_FLAG(UART0)
 */ 
#define _UART_CLEAR_LIN_RXBREAK_INT_FLAG(UART)   ((UART)->ISR |= UART_ISR_LIN_RX_BREAK_IF_Msk)

 /**
 * @brief      Clear buffur error interrupt flag
 *
 * @param[in]  UART         Structure pointer of UART Channel  
 *                          - UART0 : UART Channel 0
 *                          - UART1 : UART Channel 1 
 * @return     None
 *                                                           
 * @details    The function is used to access RX_OVER_IF and TX_OVER_IF to clear buffur error interrupt flag.\n
 *             Example: _UART_CLEAR_BUFERR_INT_FLAG(UART0)
 *
 */
#define _UART_CLEAR_BUFERR_INT_FLAG(UART)  ((UART)->FSR |= UART_FSR_RX_OVER_IF_Msk | UART_FSR_TX_OVER_IF_Msk)


/*---------------------------------------------------------------------------------------------------------*/
/* Flow Control                                                                                            */
/*---------------------------------------------------------------------------------------------------------*/

/**
 * @brief      Set RTS Trigger Level
 *
 * @param[in]  UART         Structure pointer of UART Channel  
 *                          - UART0 : UART Channel 0
 *                          - UART1 : UART Channel 1 
 * @param[in]  eActLevel    RTS Active Level  
 *                          - UART_RTS_IS_HIGH_LEV_TRG : High Level trigger(Default)  
 *                          - UART_RTS_IS_LOW_LEV_TRG  : Low Level trigger 
 *
 * @param[in]  eTRGLevel    RTS Trigger Level Select
 *                          - UART_FCR_RTS_TRI_LEV_1BYTE   : RTS Trigger Level is 1 byte 
 *                          - UART_FCR_RTS_TRI_LEV_4BYTES  : RTS Trigger Level is 4 bytes 
 *                          - UART_FCR_RTS_TRI_LEV_8BYTES  : RTS Trigger Level is 8 bytes 
 *                          - UART_FCR_RTS_TRI_LEV_14BYTES : RTS Trigger Level is 14 bytes
 *
 * @return     None                                                            
 *                                                           
 * @details    The function is used to set RTS trigger level 1,4,8,14 bytes. It is used to autoflow control.\n
 *             Example: UART_SetRTS_TrgLev(UART0,UART_RTS_IS_HIGH_LEV_TRG,UART_RTS_TRG_LEV_1)
 *
 * @details    Default State: RTS is high level active. 
 */  
                         
__INLINE void UART_SetRTS_TrgLev(UART_T* UART,uint32_t eActLevel,uint32_t eTRG_Level)
{
    (UART)->MCR = (((UART)->MCR & ~UART_MCR_LEV_RTS_Msk)     | (eActLevel ));
    (UART)->FCR = (((UART)->FCR & ~UART_FCR_RTS_TRI_LEV_Msk) | (eTRG_Level));
}

/**
 * @brief      Enable RTS/CTS Auto Flow Control
 *
 * @param[in]  UART         Structure pointer of UART Channel
 *                          - UART0 : UART Channel 0
 *                          - UART1 : UART Channel 1
 * @param[in]  eFlowCtl     Flow Control Select
 *                          - UART_IER_AUTO_RTS_EN_Msk : RTS Auto Flow Control 
 *                          - UART_IER_AUTO_CTS_EN_Msk : CTS Auto Flow Control 
 * @return     None
 *                                                           
 * @details    The function is used to set AutoFlow Control RTS/CTS function.\n
 *             Example: _UART_ENABLE_AUTOFLOW(UART0,UART_AUTO_RTS)
 *
 */  
#define _UART_ENABLE_AUTOFLOW(UART,eFlowCtl)     ((UART)->IER |= (eFlowCtl))


/**
 * @brief      Disable RTS/CTS Auto Flow Control
 *
 * @param[in]  UART         Structure pointer of UART Channel
 *                          - UART0 : UART Channel 0
 *                          - UART1 : UART Channel 1
 * @param[in]  eFlowCtl     Flow Control Select
 *                          - UART_IER_AUTO_RTS_EN_Msk : RTS Auto Flow Control 
 *                          - UART_IER_AUTO_CTS_EN_Msk : CTS Auto Flow Control 
 * @return     None
 *                                                           
 * @details    The function is used to disable AutoFlow Control RTS/CTS function.\n
 *             Example: _UART_DISABLE_AUTOFLOW(UART0,UART_AUTO_RTS)
 *
 */    
#define _UART_DISABLE_AUTOFLOW(UART,eFlowCtl)   ((UART)->IER &= ~(eFlowCtl))



/**
 * @brief      Get RTS Pin value
 *
 * @param[in]  UART         Structure pointer of UART Channel
 *                          - UART0 : UART Channel 0
 *                          - UART1 : UART Channel 1
 * @retval     1 = RTS pin value is HIGH
 * @retval     0 = RTS pin value is LOW
 *                                                           
 * @details    The function is used to get RTS pin value.
 *             Example: _UART_GET_RTSPIN(UART0)
 *
 */
#define _UART_GET_RTSPIN(UART)   (((UART)->MCR & UART_MCR_RTS_ST_Msk)>> UART_MCR_RTS_ST_Pos)

/**
 * @brief      Get CTS Pin value
 *
 * @param[in]  UART         Structure pointer of UART Channel
 *                          - UART0 : UART Channel 0
 *                          - UART1 : UART Channel 1
 * @retval     1 = CTS pin value is HIGH
 * @retval     0 = CTS pin value is LOW
 *                                                           
 * @details    The function is used to get CTS pin value.
 *             Example: _UART_GET_CTSPIN(UART0)
 *
 */
#define _UART_GET_CTSPIN(UART)   (((UART)->MSR & UART_MSR_CTS_ST_Msk)>> UART_MSR_CTS_ST_Pos)


/**
 * @brief      Get LEV_RTS bit settings
 *
 * @param[in]  UART         Structure pointer of UART Channel
 *                          - UART0 : UART Channel 0
 *                          - UART1 : UART Channel 1
 * @retval     1 = LEV_RTS bit is set
 * @retval     0 = LEV_RTS bit is clear
 *                                                           
 * @details    The function is used to get LEV_RTS bit settings.
 *             Example: _UART_GET_LEVRTS(UART0)
 *
 */
#define _UART_GET_LEVRTS(UART)   (((UART)->MCR & UART_MCR_LEV_RTS_Msk)>> UART_MCR_LEV_RTS_Pos)


/*---------------------------------------------------------------------------------------------------------*/
/* Function Switch                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
/**
 * @brief      Switch Function 
 *
 * @param[in]  UART         Structure pointer of UART Channel
 *                          - UART0 : UART Channel 0
 *                          - UART1 : UART Channel 1
 * @param[in]  eFuncSel     Function Select  
 *                          - FUNC_UART : UART function 
 *                          - FUNC_IrDA : IrDA function
 *                          - FUNC_RS485: RS485 function 
 *                          - FUNC_LIN  : LIN function                                    
.* @return     None
 * @details    The function is used to uart function to Uart,IrDA,RS485 or LIN.\n
 *             Example: _UART_SEL_FUNC(UART_PORT0,FUNC_IrDA)
 *
 */  

#define _UART_SEL_FUNC(UART,eFuncSel)   ((UART)->FUNSEL = (eFuncSel))

/*---------------------------------------------------------------------------------------------------------*/
/* IrDA Function                                                                                           */
/*---------------------------------------------------------------------------------------------------------*/

/**
 * @brief      Initialize IrDA TX configuration
 *
 * @param[in]  UART         Structure pointer of UART Channel
 *                          - UART0 : UART Channel 0
 *                          - UART1 : UART Channel 1
 * @param[in]  bIsInvTx     Invert Tx signal or not   
 *                          - TRUE  : Invert tx signal     
 *                          - FALSE : No invert tx signal  
 *
 * @return     None
 *                                                           
 * @details    The function is used to intialize IrDA TX. It wll switch IrDA mode and 
 *             configure invert tx signal or nor.\n                                              
 *             Example: _UART_SET_IrDA_TXMODE(UART0,FALSE)
 *
 */  

#define _UART_SET_IrDA_TXMODE(UART,bIsInvTx)    ((UART)->IRCR = UART_IRCR_TX_SELECT_Msk|((bIsInvTx)<<UART_IRCR_INV_TX_Pos))


/**
 * @brief      Initialize IrDA RX configuration
 *
 * @param[in]  UART         Structure pointer of UART Channel
 *                          - UART0 : UART Channel 0
 *                          - UART1 : UART Channel 1
 * @param[in]  bIsInvRx     Invert Rx signal or not
 *                          - TRUE  : Invert rx signal     
 *                          - FALSE : No invert rx signal  
 * @return     None
 *                                                           
 * @details    The function is used to intialize IrDA RX. It wll switch IrDA mode and 
 *             configure invert rx signal or nor.\n                                              
 *             Example: _UART_SET_IrDA_RXMODE(UART_PORT0,FALSE)
 *
 */  

#define _UART_SET_IrDA_RXMODE(UART,bIsInvRx)    ((UART)->IRCR = (bIsInvRx)<<UART_IRCR_INV_RX_Pos)


// /*---------------------------------------------------------------------------------------------------------*/
// /* RS485 Function                                                                                          */
// /*---------------------------------------------------------------------------------------------------------*/
// /**
//  * @brief      Initialize RS485 configuration
//  *
//  * @param[in]  UART         Structure pointer of UART Channel
//  *                          - UART0 : UART Channel 0
//  *                          - UART1 : UART Channel 1
//  * @param[in]  u8Mode       Operation Mode : RS485 operation. 
//  *                          - UART_RS485_AUD : Set Auto Direction Mode
//  *                          - UART_RS485_AAD : Set RS485 Auto Address Detection Operation Mode
//  *                          - UART_RS485_NMM : Set RS485 Normal Multi-drop Operation Mode
//  *
//  * @param[in]  u8Para       A parameter is valid if operation mode is UART_RS485_AAD, u8Para will be accress settings.
//  *
//  * @return     None
//  *                                                           
//  * @details    The function is used to intialize RS485. It wll set operation mode and access\n
//  *             Example: UART_RS485Init(UART0,UART_RS485_AAD|UART_RS485_AUD,0xC0)
//  *
//  */                                                                                      

// __INLINE void UART_RS485Init(UART_T* UART,uint32_t u32Mode,uint32_t u32Para)
// {
//     (UART)->MCR &= ~UART_MCR_LEV_RTS_Msk;

//     (UART)->ALT_CSR  =  (u32Mode)| (((u32Mode)&UART_ALT_CSR_RS485_AAD_Msk)?(((u32Para)<<UART_ALT_CSR_ADDR_MATCH_Pos)|UART_ALT_CSR_RS485_ADD_EN_Msk):0);
// }

// // New add @ 2012.05.31
// ////////////////////////////////////////////////////////////////
// __INLINE void UART_RS485_AADInit(UART_T* UART,uint32_t u32AddrMatch)
// {
//     //RX_DIS do not care 
//     
//     (UART)->ALT_CSR  =  UART_ALT_CSR_RS485_AAD_Msk | ((u32AddrMatch)<< UART_ALT_CSR_ADDR_MATCH_Pos)  ;
// }

// __INLINE void UART_RS485_NMMMInit(UART_T* UART,uint32_t u32Para)
// {
//     (UART)->FCR |= UART_FCR_RX_DIS_Msk;

//     (UART)->ALT_CSR  =  UART_ALT_CSR_RS485_NMM_Msk   ;
// }


/**
 * @brief      Enable Rx receive function in RS485 mode
 *
 * @param[in]  UART         Structure pointer of UART Channel
 *                          - UART0 : UART Channel 0
 *                          - UART1 : UART Channel 1
 * @return     None
 *                                                           
 * @details    The function is used to set Rx disable function.\n
 *             Example: _UART_RS485_SET_RXDISABLE(UART0)
 *
 */                                                                                      
#define _UART_RS485_SET_RXDISABLE(UART)    ((UART)->FCR|= UART_FCR_RX_DIS_Msk)


/**
 * @brief      Disable Rx receive function in RS485 mode
 *
 * @param[in]  UART         Structure pointer of UART Channel
 *                          - UART0 : UART Channel 0
 *                          - UART1 : UART Channel 1
 * @return     None
 *                                                           
 * @details    The function is used to Clear Rx disable function.\n
 *             Example: _UART_RS485_CLEAR_RXDISABLE(UART0)
 *
 */                                                                                      
#define _UART_RS485_CLEAR_RXDISABLE(UART)   ((UART)->FCR&= ~UART_FCR_RX_DIS_Msk) 


/**
 * @brief      Enable or Disable Rx receive function in RS485 mode
 *
 * @param[in]  UART         Structure pointer of UART Channel
 *                          - UART0 : UART Channel 0
 *                          - UART1 : UART Channel 1
 * @param[in]  bIsEnable    Control RX_DISABLE function 
 *                          - TRUE : Enable Rx disable receive function    
 *                          - FALSE : Disable Rx disable receive function 
 * @return     None
 *                                                           
 * @details    The function is used to control Rx disable function.\n
 *             Example: _UART_RS485_RXDISABLE(UART0,TRUE)
 *
 */                                                                                      

#define _UART_RS485_RXDISABLE(UART,bIsEnable)  (bIsEnable)?_UART_RS485_SET_RXDISABLE((UART)):_UART_RS485_CLEAR_RXDISABLE((UART))


/**
 * @brief      Clear RS-485 Address Byte Detection Flag
 *
 * @param[in]  UART         Structure pointer of UART Channel
 *                          - UART0 : UART Channel 0
 *                          - UART1 : UART Channel 1
 * @return     None
 *                                                           
 * @details    The function is used to write RS485_ADD_DETF_Msk bit is '1' to this bit
 *             Example: _UART_RS485_CLEAR_ADD_DETF(UART0)
 *
 */                                                                                      

#define _UART_RS485_CLEAR_ADD_DETF(UART)   ((UART)->FSR  |= UART_FSR_RS485_ADD_DETF_Msk)


/*---------------------------------------------------------------------------------------------------------*/
/* LIN Function                                                                                            */
/*---------------------------------------------------------------------------------------------------------*/
/**
 * @brief      Set LIN master mode
 *
 * @param[in]  UART         Structure pointer of UART Channel
 *                          - UART0 : UART Channel 0
 *                          - UART1 : UART Channel 1
 * @param[in]  BreakLength  Break Length Setting: 
 *                          - 0~15
 * @return     None
 *                                                           
 * @details    The function is used to control UART to be LIN master.\n
 *             Before sending header field,please remember to enable the function.\n
 *             After sending a header to bus,LIN_TX_EN is cleared automatically.\n
 *             Example: _UART_SET_LIN_TXMODE(UART0,12)
 *
 */                                                                                      
#define _UART_SET_LIN_TXMODE(UART,BreakLength) ((UART)->ALT_CSR = (BreakLength) | UART_ALT_CSR_LIN_TX_EN_Msk)

/**
 * @brief      Set LIN slave mode
 *
 * @param[in]  UART         Structure pointer of UART Channel
 *                          - UART0 : UART Channel 0
 *                          - UART1 : UART Channel 1
 * @param[in]  BreakLength  Break Length Setting: 
 *                          - 0~15
 * @return     None
 *                                                           
 * @details    The function is used to control UART to be LIN slave.\n 
 *             Example: _UART_SET_LIN_RXMODE(UART0,12)
 *
 */                                                                                      

#define _UART_SET_LIN_RXMODE(UART,BreakLength) ((UART)->ALT_CSR = (BreakLength) | UART_ALT_CSR_LIN_RX_EN_Msk)


/*---------------------------------------------------------------------------------------------------------*/
/* UART Clock Control                                                                                      */
/*---------------------------------------------------------------------------------------------------------*/                                                                                     
/**
 * @brief      Enable UART APB Clock 
 *
 * @param[in]  UART         Structure pointer of UART Channel
 *                          - UART0 : UART Channel 0
 *                          - UART1 : UART Channel 1
 * @return     None
 *                                                           
 * @details    The function is used to access APBCLK to enable UART APB Clock.
 *             Before using UART, please call the function firstly.\n
 *             Example: _UART_ENABLE_CLOCK(UART0)
 *
 */ 
#define _UART_ENABLE_CLOCK(UART)   (SYSCLK->APBCLK  |= ( 1<< (SYSCLK_APBCLK_UART0_EN_Pos + (((UART)==(UART0))?0:1))))
  
/**
 * @brief      Disable UART APB Clock 
 *
 * @param[in]  UART         Structure pointer of UART Channel
 *                          - UART0 : UART Channel 0
 *                          - UART1 : UART Channel 1
 * @return     None
 *                                                           
 * @details    The function is used to access APBCLK to disable UART APB Clock.\n
 *             Example: _UART_DISABLE_CLOCK(UART0)
 *
 */ 
#define _UART_DISABLE_CLOCK(UART)   (SYSCLK->APBCLK &=~((1<< (SYSCLK_APBCLK_UART0_EN_Pos + (((UART)==(UART0))?0:1)))))


/**
 * @brief      Reset UART IP
 *
 * @param[in]  UART         Structure pointer of UART Channel
 *                          - UART0 : UART Channel 0
 *                          - UART1 : UART Channel 1
 * @return     None
 *                                                           
 * @details    The function is used to access IPRSTC2 to reset UART IP.\n
 *             Example: UART_ResetIP(UART0)
 *
 */ 
__INLINE void UART_ResetIP(UART_T* UART)
{
    SYS->IPRSTC2 |= 1<< (SYS_IPRSTC2_UART0_RST_Pos+ ((UART)==UART0)?0:1) ;
    SYS->IPRSTC2 &= ~(1<< (SYS_IPRSTC2_UART0_RST_Pos+ ((UART)==UART0)?0:1)) ;
}     

/*---------------------------------------------------------------------------------------------------------*/
/* Break Control                                                                                           */
/*---------------------------------------------------------------------------------------------------------*/ 
/**
 * @brief      Set break control bit
 *
 * @param[in]  UART         Structure pointer of UART Channel
 *                          - UART0 : UART Channel 0
 *                          - UART1 : UART Channel 1
 * @return     None
 *                                                           
 * @details    The function is used to set BCB bit and force TXD pin to low state.\n
 *             Example: _UART_FORCE_BREAK(UART0)
 *
 */ 
#define _UART_FORCE_BREAK(UART)           ((UART)->LCR |= UART_LCR_BCB_Msk)

/**
 * @brief      Clear break control bit
 *
 * @param[in]  UART         Structure pointer of UART Channel
 *                          - UART0 : UART Channel 0
 *                          - UART1 : UART Channel 1
 * @return     None
 *                                                           
 * @details    The function is used to clear BCB bit and force TXD pin return normal state.\n
 *             Example: _UART_CLEAR_BREAK(UART0)
 *
 */ 
#define _UART_CLEAR_BREAK(UART)           ((UART)->LCR &= ~UART_LCR_BCB_Msk)




__INLINE int32_t UART_Write(UART_T *UART,uint8_t *pu8TxBuf,uint32_t u32WriteBytes)
{
    uint32_t  u32Count, u32delayno;

    for (u32Count=0; u32Count != u32WriteBytes; u32Count++)
    {
       u32delayno = 0;
       while (((UART)->FSR & UART_FSR_TE_FLAG_Msk) == 0)  /* Wait Tx empty and Time-out manner */
       {
            u32delayno++;
            if ( u32delayno >= 0x40000000 )             
               return FALSE;               
       }
       _UART_SENDBYTE((UART),pu8TxBuf[u32Count]);        /* Send UART Data from buffer */
    }
    return TRUE;
}

__INLINE int32_t UART_Read(UART_T *UART,uint8_t *pu8RxBuf, uint32_t u32ReadBytes)
{
    uint32_t u32Count, u32delayno;

    for (u32Count=0; u32Count != u32ReadBytes; u32Count++)
    {
         u32delayno = 0;
         while ((UART)->FSR & UART_FSR_RX_EMPTY_Msk)
         {
            u32delayno++;        
            if ( u32delayno >= 0x40000000 )        
                return FALSE;               
         }
         _UART_RECEIVEBYTE((UART),pu8RxBuf[u32Count]);          /* Get Data from UART RX  */
    }
    return TRUE;
}

/**
  * @} End of UART Device Function Interface
  */ 

/**
  * @} End of M051 Function Interface
  */ 


#endif











