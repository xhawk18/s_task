/****************************************************************************
 * @file     LIN_Master.c
 * @version  V1.00
 * $Revision: 2 $
 * $Date: 12/04/09 2:21p $ 
 * @brief    M051 Series UART Interface Controller Driver Sample Code
 *
 * @note
 * Copyright (C) 2011 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#include <stdio.h>
#include "M051Series.h"

/* CheckSum Method */
#define MODE_CLASSIC    2
#define MODE_ENHANCED   1

#define RXBUFSIZE 1024
/*---------------------------------------------------------------------------------------------------------*/
/* Global variables                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
extern volatile int32_t g_i32pointer;
extern volatile int32_t g_bWait;
extern uint8_t g_u8SendData[12] ;
extern uint8_t g_u8RecData[RXBUFSIZE];

/*---------------------------------------------------------------------------------------------------------*/
/* Define functions prototype                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
extern char GetChar(void);
void LIN_MasterTest(uint32_t u32id,uint32_t u32ModeSel);
void LIN_SendHeader(uint32_t u32id);
void LIN_SendResponse(int32_t checkSumOption,uint32_t *pu32TxBuf);


void LIN_TestItem()
{
    printf("\n\n");
    printf("+-----------------------------------------------------------+\n");
    printf("|     LIN Master Function Test                              |\n");
    printf("+-----------------------------------------------------------+\n");
    printf("| [1] Master send header with ID = 0x30                     |\n");    
    printf("| [2] Master send header and response with classic checksum |\n");
    printf("| [3] Master send header and response with enhanced checksum|\n");
    printf("|                                                           |\n");
    printf("| To measure UART1_TXD(pin46) to check waveform ...         |\n");
    printf("+-----------------------------------------------------------+\n");
    printf("| Quit                                              - [ESC] |\n");
    printf("+-----------------------------------------------------------+\n\n");
}


/*---------------------------------------------------------------------------------------------------------*/
/*  LIN Function Test                                                                                      */
/*---------------------------------------------------------------------------------------------------------*/
void LIN_FunctionTest()
{
    uint32_t u32Item;
    /* LIN Max Speeed is 20K */
    UART1->BAUD = UART_BAUD_MODE2 | UART_BAUD_DIV_MODE2(48000000,20000);

    /* Switch back to LIN Function */
    _UART_SEL_FUNC(UART1,UART_FUNC_SEL_LIN);

    /* === CASE 1====
        The sample code will send a LIN header with a 12-bit break field,
        0x55 sync field and ID field is 0x30. Measurement the UART1 tx pin to check it.
    */

    /* === CASE 2====
        The sample code will send a LIN header with ID is 0x35 and response field.
        The response field with 8 data bytes and checksum without including ID.
        Measurement the UART1 tx pin to check it.
    */

    /* === CASE 3====
        The sample code will send a LIN header with ID is 0x12 and response field.
        The response field with 8 data bytes and checksum with including ID.
        Measurement the UART1 tx pin to check it.
    */

    do{
        LIN_TestItem();
        u32Item = getchar();
        printf("%c\n",u32Item);
        switch(u32Item)
        {
            case '1':   LIN_SendHeader(0x30);               break;
            case '2':   LIN_MasterTest(0x35,MODE_CLASSIC);  break;
            case '3':   LIN_MasterTest(0x12,MODE_ENHANCED); break;
            default:    break;
        }
    }while(u32Item != 27); 
    
    _UART_SEL_FUNC(UART1,UART_FUNC_SEL_UART);
    
    printf("\nLINSample Code End.\n");
}


/*---------------------------------------------------------------------------------------------------------*/
/*  Master send header and response                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
void LIN_MasterTest(uint32_t u32id,uint32_t u32ModeSel)
{
    uint32_t testPattern[8] ={0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8};

	/*Send ID=0x35 Header and Reponse TestPatten*/
	LIN_SendHeader(u32id);  
	LIN_SendResponse(u32ModeSel,&testPattern[0]);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Compute Parity Value                                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
uint8_t GetParityValue(uint32_t u32id)
{
    uint32_t u32Res = 0,ID[6],p_Bit[2],mask =0;
   
    for(mask=0;mask<7;mask++)	   
   	    ID[mask] = (u32id & (1<<mask))>>mask;

    p_Bit[0] = (ID[0] + ID[1] + ID[2] + ID[4])%2;
    p_Bit[1] = (!((ID[1] + ID[3] + ID[4] + ID[5])%2));

    u32Res = u32id + (p_Bit[0] <<6) + (p_Bit[1]<<7);
    return u32Res;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Compute CheckSum Value , MODE_CLASSIC:(Not Include ID)    MODE_ENHANCED:(Include ID)                    */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t GetCheckSumValue(uint8_t *pu8Buf, uint32_t u32ModeSel)   
{
 	uint32_t i,CheckSum =0;
		
    for(i=u32ModeSel;i<=9;i++)
	{
		CheckSum+=pu8Buf[i];
	  	if (CheckSum>=256)
	  		CheckSum-=255;
	}
	return (255-CheckSum);	
}

/*---------------------------------------------------------------------------------------------------------*/
/*  Send LIN Header Field                                                                                  */
/*---------------------------------------------------------------------------------------------------------*/
/* @image      html         LIN_SendHeader0x30.JPG */

void LIN_SendHeader(uint32_t u32id)
{
	g_i32pointer =0 ;

    /* Switch back to LIN Function */
    _UART_SEL_FUNC(UART1,UART_FUNC_SEL_LIN);

    _UART_SET_LIN_TXMODE(UART1,11);
	g_u8SendData[g_i32pointer++] = 0x55 ;                  // SYNC Field
	g_u8SendData[g_i32pointer++] = GetParityValue(u32id);   // ID+Parity Field
    UART_Write(UART1,g_u8SendData,2);	
}

/*---------------------------------------------------------------------------------------------------------*/
/*  Send LIN Response Field                                                                                */
/*---------------------------------------------------------------------------------------------------------*/
void LIN_SendResponse(int32_t checkSumOption,uint32_t *pu32TxBuf)
{
	int32_t i32;

	for(i32=0;i32<8;i32++)
		g_u8SendData[g_i32pointer++] = pu32TxBuf[i32] ;

	g_u8SendData[g_i32pointer++] = GetCheckSumValue(g_u8SendData,checkSumOption) ;  //CheckSum Field

	UART_Write(UART1,g_u8SendData+2,9);	
}

