/**************************************************************************//**
 * @file     EEPROM_24LC64.c
 * @version  V2.1
 * $Revision: 2 $
 * $Date: 12/08/09 2:23p $
 * @brief    24LC64 EEPROM Driver
 *
 * @note
 * Copyright (C) 2011 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#include <stdio.h>
#include "M051Series.h"
#include "EEPROM_24LC64.h"

#define EEPROM_READ_ADDR      0xA1 /* Address of slave for read  */
#define EEPROM_WRITE_ADDR     0xA0 /* Address of slave for write */


void EEPROM_Init(void)
{
	SYS->IPRSTC2 |= SYS_IPRSTC2_I2C_RST_Msk;
	SYS->IPRSTC2 &= ~SYS_IPRSTC2_I2C_RST_Msk;
	
    I2C0->I2CLK = I2C_I2CLK_DIV4(120); /* 48000000Hz / 4 / 120 = 100000Hz */
	_I2C_ENABLE_TIMEOUT_COUNTER(I2C0);
}

void EEPROM_Write(uint32_t u32Addr, uint8_t u8Data)
{
    int32_t i32Err;

    do 
    {
        i32Err = 0;
        /* Send start */
        _I2C_START(I2C0);
        _I2C_WAIT_READY(I2C0);
        /* Send control byte */
        _I2C_WRITE(I2C0, EEPROM_WRITE_ADDR);
        _I2C_WAIT_READY(I2C0);
        if(I2C0->I2CSTATUS == 0x18)
        {
            /* ACK */
    
            /* Send high address */
            _I2C_WRITE(I2C0, (u32Addr >> 8) & 0xFFUL); // high address
            _I2C_WAIT_READY(I2C0);
            if(I2C0->I2CSTATUS == 0x28)
            {
                /* ACK */
    
                /* Send low address */
                //_I2C_WRITE(I2C0, u32Addr & 0xFFUL); // low address
                //_I2C_WAIT_READY(I2C0);
                if(I2C0->I2CSTATUS == 0x28)
                {
                    /* ACK */
        
                    /* Send data */
                    _I2C_WRITE(I2C0, u8Data); // data
                    _I2C_WAIT_READY(I2C0);
                    if(I2C0->I2CSTATUS == 0x28)
                    {
                        /* ACK */
            
                        /* Send stop */
                        _I2C_STOP(I2C0);
            
                    }
                    else
                    {
                        /* NACK */
    
                        /* Send data error */
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
        
            /* Send control error */
            i32Err = 1;
        }

        if(i32Err)
        {

            /* Send stop */
            _I2C_STOP(I2C0);

            SYS_SysTickDelay(100);
        }

    }while(i32Err);

}

void _I2C_WAIT_READY_TIMEOUT(I2C_T *port) {
	extern uint32_t CyclesPerUs;
	SysTick->LOAD = 2000 * CyclesPerUs;
    SysTick->VAL  =  (0x00);
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;

	while(((port)->I2CON & I2C_I2CON_SI_Msk) == 0
		&& (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0);
	
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
							}
goto retry;
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

uint8_t EEPROM_SequentialRead(uint32_t u32Addr, uint8_t *pu8Buf, uint32_t u32Size)
{
    int32_t i32Err;
    int32_t i;

    do 
    {
        i32Err = 0;
        /* Send start */
        _I2C_START(I2C0);
        _I2C_WAIT_READY(I2C0);
        /* Send control byte */
        _I2C_WRITE(I2C0, EEPROM_WRITE_ADDR);
        _I2C_WAIT_READY(I2C0);
        if(I2C0->I2CSTATUS == 0x18)
        {
            /* ACK */
    
            /* Send high address */
            _I2C_WRITE(I2C0, (u32Addr >> 8) & 0xFFUL); // high address
            _I2C_WAIT_READY(I2C0);
            if(I2C0->I2CSTATUS == 0x28)
            {
                /* ACK */
    
                /* Send low address */
                _I2C_WRITE(I2C0, u32Addr & 0xFFUL); // low address
                _I2C_WAIT_READY(I2C0);
                if(I2C0->I2CSTATUS == 0x28)
                {
                    /* ACK */
        
                    /* Send data */
                    _I2C_START(I2C0); // repeat start
                    _I2C_WAIT_READY(I2C0);
                    if(I2C0->I2CSTATUS == 0x10)
                    {
                        /* ACK */

                        /* Send control byte */
                        _I2C_WRITE(I2C0, EEPROM_READ_ADDR);
                        _I2C_WAIT_READY(I2C0);
                        if(I2C0->I2CSTATUS == 0x40)
                        {
                            for(i=0;i<u32Size-1;i++)
                            {
                                /* Read data */
                                pu8Buf[i] = _I2C_READ_ACK(I2C0);
                            }
                            
                            pu8Buf[i] = _I2C_READ_NAK(I2C0);

                            /* Send stop */
                            _I2C_STOP(I2C0);
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

            SYS_SysTickDelay(100);
        }

    }while(i32Err);

    return u32Size;
}

// EEPROM page size is fixed to be 32 bytes
void EEPROM_PageWrite(uint32_t u32Addr, uint8_t *pu8Buf)
{
    int32_t i32Err;
    int32_t i;

    do 
    {
        i32Err = 0;
        /* Send start */
        _I2C_START(I2C0);
        _I2C_WAIT_READY(I2C0);
        /* Send control byte */
        _I2C_WRITE(I2C0, EEPROM_WRITE_ADDR);
        _I2C_WAIT_READY(I2C0);
        if(I2C0->I2CSTATUS == 0x18)
        {
            /* ACK */
    
            /* Send high address */
            _I2C_WRITE(I2C0, (u32Addr >> 8) & 0xFFUL); // high address
            _I2C_WAIT_READY(I2C0);
            if(I2C0->I2CSTATUS == 0x28)
            {
                /* ACK */
    
                /* Send low address */
                _I2C_WRITE(I2C0, u32Addr & 0xFFUL); // low address
                _I2C_WAIT_READY(I2C0);
                if(I2C0->I2CSTATUS == 0x28)
                {
                    /* ACK */
        
                    for(i=0;i<32;i++)
                    {
                        /* Send data */
                        _I2C_WRITE(I2C0, pu8Buf[i]); // data
                        _I2C_WAIT_READY(I2C0);
                        if(I2C0->I2CSTATUS == 0x30)
                        {
                            /* NACK */
        
                            /* Send data error */
                            i32Err = 4;
                        }
                    }
                    
                    /* Send stop when no any error */
                    if(i32Err == 0)
                    {
                        /* Send stop */
                        _I2C_STOP(I2C0);
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
        
            /* Send control error */
            i32Err = 1;
        }

        if(i32Err)
        {
            /* Send stop */
            _I2C_STOP(I2C0);

            SYS_SysTickDelay(100);
        }

    }while(i32Err);

}
