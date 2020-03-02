/****************************************************************************
 * @file     IrDA.c
 * @version  V1.00
 * $Revision: 1 $
 * $Date: 12/04/05 7:07p $ 
 * @brief    M051 Series UART Interface Controller Driver Sample Code
 *
 * @note
 * Copyright (C) 2011 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#include <stdio.h>
#include "M051Series.h"

/*---------------------------------------------------------------------------------------------------------*/
/* Global variables                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
#define RXBUFSIZE 1024

/*---------------------------------------------------------------------------------------------------------*/
/* Define functions prototype                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
void IrDA_FunctionTxTest(void);
void IrDA_FunctionRxTest(void);
void IrDA_FunctionTest(void);

extern char GetChar(void);

/*---------------------------------------------------------------------------------------------------------*/
/*  IrDA Function Test                                                                                     */
/*---------------------------------------------------------------------------------------------------------*/
void IrDA_FunctionTest()
{
    uint8_t u8item;

    printf("+-----------------------------------------------------------+\n");
    printf("|     Pin Configure                                         |\n");
    printf("+-----------------------------------------------------------+\n");
    printf("|  ______                                      _______      |\n");
    printf("| |      |                                    |       |     |\n");  
    printf("| |Master|---TXD1(pin46) <====> RXD1(pin45)---|Slave  |     |\n");  
    printf("| |      |                                    |       |     |\n");  
    printf("| |______|                                    |_______|     |\n");  
    printf("|                                                           |\n");
    printf("+-----------------------------------------------------------+\n");  

    printf("\n\n");
    printf("+-----------------------------------------------------------+\n");
    printf("|     IrDA Function Test                                    |\n");
    printf("+-----------------------------------------------------------+\n");
    printf("|  Description :                                            |\n");
    printf("|    The sample code needs two boards. One is Master and    |\n");
    printf("|    the other is slave.  Master will send data based on    |\n");
    printf("|    terminal input and Slave will printf recevied data on  |\n");
    printf("|    terminal screen.                                       |\n");
    printf("|  Please select Master or Slave test                       |\n");
    printf("|  [0] Master    [1] Slave                                  |\n");
    printf("+-----------------------------------------------------------+\n\n");
    u8item = GetChar();
    
    /*
        UART0 is set to debug port and connect with PC firstly.
        The IrDA sample code needs two module board to execute.
        Set the master board is IrDA tx Mode and the other is IrDA rx mode.
        Inputing char on terminal will be sent to the UART0 of master.
        After the master receving, the inputing char will send to UART0 again.
        At the same time, it also sends to UART1 tx pin by IrDA mode.
        Slave will print received char adter UART1 send out.
        Note that IrDA mode is ONLY used when baud rate equation is selected mode 0. 
    
    */



    if(u8item=='0')
        IrDA_FunctionTxTest();
    else
        IrDA_FunctionRxTest();
    
    printf("\nIrDA Sample Code End.\n");
        
}

/*---------------------------------------------------------------------------------------------------------*/
/*  IrDA Function Transmit Test                                                                            */
/*---------------------------------------------------------------------------------------------------------*/
void IrDA_FunctionTxTest()
{
    uint8_t u8OutChar;

    printf("\n\n");
    printf("+-----------------------------------------------------------+\n");
    printf("|     IrDA Function Tx Mode Test                            |\n");
    printf("+-----------------------------------------------------------+\n");
    printf("| 1). Input char by UART0 terminal.                         |\n");
    printf("| 2). UART1 will send a char according to step 1.           |\n");
    printf("| 3). Return step 1. (Press '0' to exit)                    |\n");
    printf("+-----------------------------------------------------------+\n\n");

    printf("\nIRDA Sample Code Start. \n");

    /* In IrDA Mode, Baud Rate configuration must be used MODE0*/
    UART1->BAUD = UART_BAUD_MODE0 | UART_BAUD_DIV_MODE0(48000000, 115200);
    
    /* IrDA Function Enable */
    _UART_SEL_FUNC(UART1,UART_FUNC_SEL_IrDA);

    /* Set IrDA Tx mode */
    _UART_SET_IrDA_TXMODE(UART1,FALSE);
    
    /* Wait Terminal input to send data to UART1 TX pin */
    do
    {
        u8OutChar = GetChar();
        printf("   Input: %c , Send %c out\n",u8OutChar,u8OutChar);
        _UART_SENDBYTE(UART1,u8OutChar);
    }while(u8OutChar !='0');
    
}

/*---------------------------------------------------------------------------------------------------------*/
/*  IrDA Function Receive Test                                                                             */
/*---------------------------------------------------------------------------------------------------------*/
void IrDA_FunctionRxTest()
{
    uint8_t u8InChar=0xFF;

    printf("\n\n");
    printf("+-----------------------------------------------------------+\n");
    printf("|     IrDA Function Rx Mode Test                            |\n");
    printf("+-----------------------------------------------------------+\n");
    printf("| 1). Polling RDA_Flag to check data input though UART1     |\n");
    printf("| 2). If received data is '0', the program will exit.       |\n");
    printf("|     Otherwise, print received data on terminal            |\n");
    printf("+-----------------------------------------------------------+\n\n");

    /* In IrDA Mode, Baud Rate configuration must be used MODE0*/
    UART1->BAUD = UART_BAUD_MODE0 | UART_BAUD_DIV_MODE0(48000000, 115200);

    /* IrDA Function Enable */
    _UART_SEL_FUNC(UART1,UART_FUNC_SEL_IrDA);

    /* Set IrDA Rx mode */
    _UART_SET_IrDA_RXMODE(UART1,TRUE);

    printf("Waiting...\n");

    /* Use polling method to wait master data */
    do
    {
        if( _UART_IS_RX_READY(UART1))
        {
            _UART_RECEIVEBYTE(UART1,u8InChar);
            printf("   Input: %c \n",u8InChar);
        }
    }while(u8InChar !='0');

}
