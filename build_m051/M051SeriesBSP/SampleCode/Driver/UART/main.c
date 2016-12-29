/****************************************************************************
 * @file     Smpl_DrvUART.c
 * @version  V1.00
 * $Revision: 5 $
 * $Date: 12/08/09 5:34p $ 
 * @brief    M051 Series UART Interface Controller Driver Sample Code
 *
 * @note
 * Copyright (C) 2011 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#include <stdio.h>
#include "M051Series.h"

#define PLLCON_SETTING      SYSCLK_PLLCON_50MHz_XTAL
#define PLL_CLOCK           50000000


#define RXBUFSIZE 1024

/*---------------------------------------------------------------------------------------------------------*/
/* Global variables                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
uint8_t g_u8SendData[12] ={0};
uint8_t g_u8RecData[RXBUFSIZE]  ={0};

volatile uint32_t g_u32comRbytes = 0;        
volatile uint32_t g_u32comRhead  = 0;
volatile uint32_t g_u32comRtail  = 0;
volatile int32_t g_bWait         = TRUE;
volatile int32_t g_i32pointer = 0;

/*---------------------------------------------------------------------------------------------------------*/
/* Define functions prototype                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
int32_t main(void);
void UART_TEST_HANDLE(void);
void UART_FunctionTest(void);
void AutoFlow_FunctionTest(void);
void TestItem (void);

extern void RS485_HANDLE(void);
extern void IrDA_FunctionTest(void);
extern void RS485_FunctionTest(void);
extern void LIN_FunctionTest(void);

/*---------------------------------------------------------------------------------------------------------*/
/*  Sample Code Menu                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
void TestItem (void)
{
    printf("+-----------------------------------------------------------+\n");
    printf("|               UART Sample Program                         |\n");
    printf("+-----------------------------------------------------------+\n");
    printf("| UART function test                                  - [1] |\n");
    printf("| IrDA funtion test                                   - [2] |\n");
    printf("| RS485 funtion test                                  - [3] |\n");
    printf("| LIN Master funtion test                             - [4] |\n");
    printf("| Auto-Flow funtion test                              - [5] |\n");
    printf("+-----------------------------------------------------------+\n");
    printf("| Quit                                              - [ESC] |\n");
    printf("+-----------------------------------------------------------+\n");
    printf("Please Select key (1~5): ");
}


void SYS_Init(void)
{
/*---------------------------------------------------------------------------------------------------------*/
/* Init System Clock                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Enable External XTAL (4~24 MHz) */
    SYSCLK->PWRCON |= SYSCLK_PWRCON_XTL12M_EN_Msk;

    /* Waiting for 12MHz clock ready */
    SYS_WaitingForClockReady( SYSCLK_CLKSTATUS_XTL12M_STB_Msk);

    /* Switch HCLK clock source to XTAL */
    SYSCLK->CLKSEL0 = SYSCLK_CLKSEL0_HCLK_XTAL;

    /* Set PLL to power down mode and PLL_STB bit in CLKSTATUS register will be cleared by hardware.*/
    SYSCLK->PLLCON|= SYSCLK_PLLCON_PD_Msk;

    /* Set PLL frequency */        
    SYSCLK->PLLCON = PLLCON_SETTING;

    /* Waiting for clock ready */
    SYS_WaitingForClockReady(SYSCLK_CLKSTATUS_PLL_STB_Msk);

    /* Switch HCLK clock source to PLL */
    SYSCLK->CLKSEL0 = SYSCLK_CLKSEL0_HCLK_PLL;

    /* Enable IP clock */        
    SYSCLK->APBCLK = SYSCLK_APBCLK_UART0_EN_Msk | SYSCLK_APBCLK_UART1_EN_Msk;
    
    /* Select IP clock source */
    SYSCLK->CLKSEL1 = SYSCLK_CLKSEL1_UART_XTAL;;
                      
    /* Update System Core Clock */
    /* User can use SystemCoreClockUpdate() to calculate PllClock, SystemCoreClock and CycylesPerUs automatically. */
    //SystemCoreClockUpdate(); 
    PllClock        = PLL_CLOCK;            // PLL
    SystemCoreClock = PLL_CLOCK / 1;        // HCLK
    CyclesPerUs     = PLL_CLOCK / 1000000;  // For SYS_SysTickDelay()

/*---------------------------------------------------------------------------------------------------------*/
/* Init I/O Multi-function                                                                                 */
/*---------------------------------------------------------------------------------------------------------*/
    /* Set P3 multi-function pins for UART0 RXD and TXD  */
    SYS->P3_MFP = SYS_MFP_P30_RXD0 | SYS_MFP_P31_TXD0;

    /* Set P1 multi-function pins for UART1 RXD and TXD  */
    SYS->P1_MFP = SYS_MFP_P12_RXD1 | SYS_MFP_P13_TXD1;

    /* Set P0 multi-function pins for UART1 RTS and CTS */
    SYS->P0_MFP = SYS_MFP_P01_RTS1 | SYS_MFP_P00_CTS1;

    /* Lock protected registers */
    SYS_LockReg();
}

void UART0_Init()
{
/*---------------------------------------------------------------------------------------------------------*/
/* Init UART                                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
    UART0->BAUD = UART_BAUD_MODE2 | UART_BAUD_DIV_MODE2(__XTAL,115200);
   
    _UART_SET_DATA_FORMAT(UART0, UART_WORD_LEN_8 | UART_PARITY_NONE | UART_STOP_BIT_1);
}

void UART1_Init()
{
/*---------------------------------------------------------------------------------------------------------*/
/* Init UART                                                                                               */
/*---------------------------------------------------------------------------------------------------------*/

    UART1->BAUD = UART_BAUD_MODE2 | UART_BAUD_DIV_MODE2(PLL_CLOCK, 115200);

    _UART_SET_DATA_FORMAT(UART1, UART_WORD_LEN_8 | UART_PARITY_NONE | UART_STOP_BIT_1);
}

/*---------------------------------------------------------------------------------------------------------*/
/* UART Test Sample                                                                                        */
/* Test Item                                                                                               */
/* It sends the received data to HyperTerminal.                                                            */
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/* MAIN function                                                                                           */
/*---------------------------------------------------------------------------------------------------------*/

int main(void)
{
    uint32_t u32Item;
    /* Init System, IP clock and multi-function I/O */
    SYS_Init();
    /* Init UART0 for printf */
    UART0_Init();
    /* Init UART1 for testing */
    UART1_Init();

/*---------------------------------------------------------------------------------------------------------*/
/* SAMPLE CODE                                                                                             */
/*---------------------------------------------------------------------------------------------------------*/
    
    printf("\n\nCPU @ %dHz\n", SystemCoreClock);

    do{
        TestItem();
        u32Item = getchar();
        printf("%c\n",u32Item);
        switch(u32Item)
        {
            case '1':   UART_FunctionTest();     break;
            case '2':   IrDA_FunctionTest();     break;
            case '3':   RS485_FunctionTest();    break;
            case '4':   LIN_FunctionTest();      break;
            case '5':   AutoFlow_FunctionTest(); break;
            default:    break;
        }
    }while(u32Item != 27); 

}

/*---------------------------------------------------------------------------------------------------------*/
/* ISR to handle UART Channel 0 interrupt event                                                            */
/*---------------------------------------------------------------------------------------------------------*/
void UART0_IRQHandler(void)
{
    UART_TEST_HANDLE();
}

/*---------------------------------------------------------------------------------------------------------*/
/* ISR to handle UART Channel 1 interrupt event                                                            */
/*---------------------------------------------------------------------------------------------------------*/
void UART1_IRQHandler(void)
{
    RS485_HANDLE();
}

/*---------------------------------------------------------------------------------------------------------*/
/* UART Callback function                                                                                  */
/*---------------------------------------------------------------------------------------------------------*/
void UART_TEST_HANDLE()
{
    uint8_t u8InChar=0xFF;
    uint32_t u32IntSts= UART0->ISR;

    if(u32IntSts & UART_ISR_RDA_INT_Msk)
    {
        printf("\nInput:");
        
        /* Get all the input characters */
        while(_UART_IS_RX_READY(UART0)) 
        {
            /* Get the character from UART Buffer */
            _UART_RECEIVEBYTE(UART0,u8InChar);           /* Rx trigger level is 1 byte*/    

            printf("%c ", u8InChar);
            
            if(u8InChar == '0')   
            {   
                g_bWait = FALSE;
            }
        
            /* Check if buffer full */
            if(g_u32comRbytes < RXBUFSIZE)
            {
                /* Enqueue the character */
                g_u8RecData[g_u32comRtail] = u8InChar;
                g_u32comRtail = (g_u32comRtail == (RXBUFSIZE-1)) ? 0 : (g_u32comRtail+1);
                g_u32comRbytes++;
            }           
        }
        printf("\nTransmission Test:");
    }

    if(u32IntSts & UART_ISR_THRE_INT_Msk)
    {   
        uint16_t tmp;
        tmp = g_u32comRtail;
        if(g_u32comRhead != tmp)
        {
            u8InChar = g_u8RecData[g_u32comRhead];
            _UART_SENDBYTE(UART0,u8InChar);
            g_u32comRhead = (g_u32comRhead == (RXBUFSIZE-1)) ? 0 : (g_u32comRhead+1);
            g_u32comRbytes--;
        }
    }
}


/*---------------------------------------------------------------------------------------------------------*/
/*  UART Function Test                                                                                     */
/*---------------------------------------------------------------------------------------------------------*/
void UART_FunctionTest()
{
    printf("+-----------------------------------------------------------+\n");
    printf("|  UART Function Test                                       |\n");
    printf("+-----------------------------------------------------------+\n");
    printf("|  Description :                                            |\n");
    printf("|    The sample code will print input char on terminal      |\n");
    printf("|    Please enter any to start     (Press '0' to exit)      |\n");
    printf("+-----------------------------------------------------------+\n");
   
    /*
        Using a RS232 cable to connect UART0 and PC.
        UART0 is set to debug port. UART0 is enable RDA and RLS interrupt.
        When inputing char to terminal screen, RDA interrupt will happen and         
        UART0 will print the received char on screen.
    */
   
    /* Enable Interrupt and install the call back function */
    _UART_ENABLE_INT(UART0, (UART_IER_RDA_IEN_Msk | UART_IER_THRE_IEN_Msk | UART_IER_RTO_IEN_Msk));
    NVIC_EnableIRQ(UART0_IRQn);
    while(g_bWait); 
        
    /* Disable Interrupt */
    _UART_DISABLE_INT(UART0, (UART_IER_RDA_IEN_Msk | UART_IER_THRE_IEN_Msk | UART_IER_RTO_IEN_Msk));
    NVIC_DisableIRQ(UART0_IRQn);
    g_bWait =TRUE;
    printf("\nUART Sample Demo End.\n");
        
}

/*---------------------------------------------------------------------------------------------------------*/
/*  AutoFlow Function Test                                                                                 */
/*---------------------------------------------------------------------------------------------------------*/
void AutoFlow_FunctionTest()
{
    uint8_t u8Item;
    uint32_t u32i;
    printf("+-----------------------------------------------------------+\n");
    printf("|     Pin Configure                                         |\n");
    printf("+-----------------------------------------------------------+\n");
    printf("|  _______                                      _______     |\n");
    printf("| |       |                                    |       |    |\n");  
    printf("| |Master |---TXD1(pin46) <====> RXD1(pin45)---| Slave |    |\n");  
    printf("| |       |---RTS1(pin39) <====> CTS1(pin40)---|       |    |\n");  
    printf("| |_______|---CTS1(pin40) <====> RTS1(pin39)---|_______|    |\n");  
    printf("|                                                           |\n");
    printf("+-----------------------------------------------------------+\n\n");  

    /* Set RTS Trigger Level */
    UART_SetRTS_TrgLev(UART1,UART_RTS_IS_HIGH_LEV_TRG,UART_FCR_RTS_TRI_LEV_14BYTES);
    
    /* Enable RTS and CTS autoflow control */
    UART1->IER |= UART_IER_AUTO_RTS_EN_Msk | UART_IER_AUTO_CTS_EN_Msk;
    
    printf("+-----------------------------------------------------------+\n");
    printf("|       AutoFlow Function Test                              |\n");
    printf("+-----------------------------------------------------------+\n");
    printf("|  Description :                                            |\n");
    printf("|    The sample code needs two boards. One is Master and    |\n");
    printf("|    the other is slave. Master will send 1k bytes data     |\n");
    printf("|    to slave.Slave will check if received data is correct  |\n");
    printf("|    after getting 1k bytes data.                           |\n");
    printf("|  Please select Master or Slave test                       |\n");
    printf("|  [0] Master    [1] Slave                                  |\n");
    printf("+-----------------------------------------------------------+\n\n");
    u8Item = getchar();
    
    
    if(u8Item=='0')
    {
        for(u32i=0;u32i<(RXBUFSIZE-1);u32i++)
        {
            _UART_SENDBYTE(UART1,((u32i+1)&0xFF));

            /* Slave will control RTS pin*/
            while(_UART_GET_RTSPIN(UART1));
        }
        printf("\n Transmit Done\n");
    }
    else
    {
        g_i32pointer = 0;
    
        /* Enable RDA\RLS\RTO Interrupt  */
        _UART_ENABLE_INT(UART1, (UART_IER_RDA_IEN_Msk | UART_IER_THRE_IEN_Msk | UART_IER_RTO_IEN_Msk));

        /* Set RX Trigger Level = 8 */
        _UART_SET_RX_TRG_LEV(UART1,UART_FCR_RFITL_8BYTES);

        /* Set Timeout time 0x3E bit-time */
        UART_SetTimeOutCounter(UART1,0x3E);
        
        NVIC_EnableIRQ(UART1_IRQn);

        printf("Starting to recevice %d bytes data...\n", RXBUFSIZE);
        
        while(g_i32pointer<(RXBUFSIZE-1))
        {
            printf("%d\r",g_i32pointer);
        }

        /* Compare Data */
        for(u32i=0;u32i!=(RXBUFSIZE-1);u32i++)
        {
            if(g_u8RecData[u32i] != ((u32i+1)&0xFF) )
            {
                printf("Compare Data Failed\n");
                while(1);
            }
        }
        printf("\n Receive OK & Check OK\n");
    }

    NVIC_DisableIRQ(UART1_IRQn);

    _UART_DISABLE_INT(UART1, (UART_IER_RDA_IEN_Msk | UART_IER_THRE_IEN_Msk | UART_IER_RTO_IEN_Msk));
}
