/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright (c) Nuvoton Technology Corp. All rights reserved.                                             */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include "M051Series.h"
#include "LCD_Driver.h"
#include "EEPROM_24LC64.h"

#define PLLCON_SETTING      SYSCLK_PLLCON_50MHz_XTAL
#define PLL_CLOCK           50000000

#define EEPROM_READ_ADDR      0xA1 /* Address of slave for read  */
#define EEPROM_WRITE_ADDR     0xA0 /* Address of slave for write */


void _I2C_WAIT_READY_TIMEOUT(I2C_T *port) {
	extern uint32_t CyclesPerUs;
	SysTick->LOAD = 2000 * CyclesPerUs;
    SysTick->VAL  =  (0x00);
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;

	while(((port)->I2CON & I2C_I2CON_SI_Msk) == 0
		&& (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0);
	
}



void EEPROM_Init(void)
{
	SYS->IPRSTC2 |= SYS_IPRSTC2_I2C_RST_Msk;
	SYS->IPRSTC2 &= ~SYS_IPRSTC2_I2C_RST_Msk;
	
    I2C0->I2CLK = I2C_I2CLK_DIV4(120); /* 48000000Hz / 4 / 120 = 100000Hz */
	_I2C_ENABLE_TIMEOUT_COUNTER(I2C0);
}


uint8_t EEPROM_Read(uint32_t u32Addr)
{
	int try_count = 0;
    int32_t i32Err;
    uint8_t u8Data;

    u8Data = 0;
    do 
    {
        i32Err = 0;
        /* Send start */
        _I2C_START(I2C0);
        _I2C_WAIT_READY_TIMEOUT(I2C0);
        /* Send control byte */
        _I2C_WRITE(I2C0, EEPROM_WRITE_ADDR);
        _I2C_WAIT_READY_TIMEOUT(I2C0);
        if(I2C0->I2CSTATUS == 0x18)
        {
            /* ACK */
    
            /* Send high address */
            //_I2C_WRITE(I2C0, (u32Addr >> 8) & 0xFFUL); // high address
            _I2C_WRITE(I2C0, u32Addr & 0xFFUL); // low address			
            _I2C_WAIT_READY_TIMEOUT(I2C0);
			//printf("I2C0->I2CSTATUS = %x\n", I2C0->I2CSTATUS);
            if(I2C0->I2CSTATUS == 0x28)
            {
                /* ACK */
    
                /* Send low address */
                //_I2C_WRITE(I2C0, u32Addr & 0xFFUL); // low address
                //_I2C_WAIT_READY_TIMEOUT(I2C0);
				//printf("I2C0->I2CSTATUS = %x\n", I2C0->I2CSTATUS);
                if(I2C0->I2CSTATUS == 0x28)
                {
                    /* ACK */
        retry:
                    /* Send data */
                    _I2C_START(I2C0); // repeat start
                    _I2C_WAIT_READY_TIMEOUT(I2C0);
					//printf("I2C0->I2CSTATUS = %x\n", I2C0->I2CSTATUS);
                    if(I2C0->I2CSTATUS == 0x10 || I2C0->I2CSTATUS == 0x08)
                    {
                        /* ACK */

                        /* Send control byte */
                        _I2C_WRITE(I2C0, EEPROM_READ_ADDR);
                        _I2C_WAIT_READY_TIMEOUT(I2C0);
                        if(I2C0->I2CSTATUS == 0x40)
                        {

                            /* Read data */
							//while(1)
                            u8Data = _I2C_READ_NAK(I2C0);
#if 0							
							{
							static int i = 0;
							if(i % 8 == 0) printf("%d: ", i);
							printf("0x%02x, ", u8Data);
							if((++i) %8 == 0) printf("\n");
							if(i < 6){
								goto retry;
							}
							else{
								printf("\n");
								i = 0;
							}
#endif
                            if(I2C0->I2CSTATUS == 0x58)
                            {
                                /* NACK */
                                /* Send stop */
                                _I2C_STOP(I2C0);
                            }
                            else
                            {
                                /* ACK */
            
                                /* read data error */
                                i32Err = 6;
                            }
                        }
                        else
                        {
                            /* NACK */
        
                            /* Send control read error */
                            i32Err = 5;
                        }
                    }
                    else
                    {
                        /* NACK */
    
                        /* Send start error */
                        i32Err = 4;
                    }
                }
                else
                {
                    /* NACK */
                
                    /* Send low address error */
                    i32Err = 3;
                }
            }
            else
            {
                /* NACK */
            
                /* Send high address error */
                i32Err = 2;
            }
        }
        else
        {
            /* NACK */    
        
            /* Send control write error */
            i32Err = 1;

        }

        if(i32Err)
        {
            /* Send stop */
            _I2C_STOP(I2C0);

			//printf("err %d\n", i32Err);
            SYS_SysTickDelay(100);
			//printf("err\n");
        }

    }while(i32Err && ++try_count < 2);

    return (i32Err == 0 ? u8Data : 0xFF);
}


void SYS_Init(void)
{

    /* Unlock protected registers */
    SYS_UnlockReg();

/*---------------------------------------------------------------------------------------------------------*/
/* Init System Clock                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/

    /* Enable External XTAL (4~24 MHz) */
    SYSCLK->PWRCON |= SYSCLK_PWRCON_XTL12M_EN_Msk | SYSCLK_PWRCON_IRC10K_EN_Msk;

    /* Waiting for 12MHz & IRC10Khz clock ready */
    SYS_WaitingForClockReady( SYSCLK_CLKSTATUS_XTL12M_STB_Msk | SYSCLK_CLKSTATUS_IRC10K_STB_Msk);

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
    SYSCLK->APBCLK = SYSCLK_APBCLK_UART0_EN_Msk | SYSCLK_APBCLK_SPI0_EN_Msk |
                        SYSCLK_APBCLK_I2C_EN_Msk;
    
    /* Select IP clock source */
    SYSCLK->CLKSEL1 = SYSCLK_CLKSEL1_UART_XTAL;

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
    SYS->P3_MFP = SYS_MFP_P30_RXD0 | SYS_MFP_P31_TXD0
    /* Set P3.4 P3.5 for I2C */
				 | SYS_MFP_P34_SDA0 | SYS_MFP_P35_SCL0;
    /* Set P1.4, P1.5, P1.6, P1.7 for SPI0 to driver LCD */
    SYS->P1_MFP = SYS_MFP_P14_SPISS0 | SYS_MFP_P15_MOSI_0 | SYS_MFP_P16_MISO_0 | SYS_MFP_P17_SPICLK0;

    /* Lock protected registers */
    SYS_LockReg();
}


void UART0_Init(void)
{
/*---------------------------------------------------------------------------------------------------------*/
/* Init UART                                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
    UART0->BAUD = UART_BAUD_MODE2 | UART_BAUD_DIV_MODE2(__XTAL, 115200);
    _UART_SET_DATA_FORMAT(UART0, UART_WORD_LEN_8 | UART_PARITY_NONE | UART_STOP_BIT_1);
}


int main(void)
{
	int i;
	int no;
	unsigned char old[6];
	unsigned char addr[6];

    /* Init system, IP clock and multi-function I/O */
    SYS_Init();

    /* Init UART0 for printf */	
    _GPIO_SET_PIN_MODE(P3, 2, GPIO_PMD_OUTPUT);
	P3->DOUT |= (1<<2);
    UART0_Init();
	
	printf("I2C EEPROM read program, supports MAC as 00:04:A3:XX:XX:XX.\n");
	printf("Please connect EEPROM to I2C\n");

	memset(old, 0, sizeof(old));

	no = 0;
	while(1) {
#define MAC_ADDR 0xFA
#define MAC0 0x00
#define MAC1 0x1e
#define MAC2 0xc0

		EEPROM_Init();
		for(i = 0; i < 6; ++i)
			addr[i] = EEPROM_Read(MAC_ADDR + i);
	
		if(memcmp(addr, old, sizeof(old)) != 0
			&& addr[0] == MAC0
			&& addr[1] == MAC1
			&& addr[2] == MAC2
			&& !(addr[3] == 0xFF && addr[4] == 0xFF && addr[5] == 0xFF)
			&& !(addr[4] == 0xFF && addr[5] == 0xFF)
			&& !(addr[5] == 0xFF)		
			) 
		{
			
			printf("%d - ", no++);
			for(i = 0; i < 6; ++i)
				printf("%02x ", (int)addr[i]);
			printf("\n");
			memcpy(old, addr, sizeof(old));
		}
	}
    
	return 0;
}
