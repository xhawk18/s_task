/**************************************************************************//**
 * @file     LCD_Driver.c
 * @version  V2.1
 * $Revision: 4 $
 * $Date: 12/08/09 2:23p $
 * @brief    LCD Driver
 *
 * @note
 * Copyright (C) 2011 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#include "M051Series.h"
#include "LCD_Driver.h"

SPI_T *SPI;
extern  const char Ascii[];

static __INLINE void SpiWrite(uint32_t x)
{
    SPI->TX[0] = x;
    SPI->CNTRL |= SPI_CNTRL_GO_BUSY;    
    while(SPI->CNTRL & SPI_CNTRL_GO_BUSY);
}
void LCD_Init(void)
{
    /* Use SPI0 for LCD */
    SPI = SPI0;

    /* Initial SPI data format and SPI clock */    
    SPI->CNTRL   = SPI_CNTRL_CLK_IDLE_HIGH | SPI_CNTRL_TX_FALLING | 
                       SPI_CNTRL_RX_RISING | SPI_CNTRL_TX_BIT_LEN(9);
    SPI->DIVIDER = SPI_DIVIDER_DIV(4); /* SPI clock freq = system clock / 4 */
    SPI->SSR = SPI_SSR_HW_AUTO_ACTIVE_LOW;

    // Set BR
    SpiWrite(0xEB);

    // Set PM
    SpiWrite(0x81);
    SpiWrite(0xA0);

    SpiWrite(0xC0);

    // Set Display Enable
    SpiWrite(0xAF);

}

//*******************************
//this function to initial start address
//*******************************
static void SetPACA(unsigned char PA, unsigned char CA)
{
    // Set PA
    SpiWrite(0xB0 | PA);

    // Set CA MSB
    SpiWrite(0x10 | (CA >> 4) & 0xF);

    // Set CA LSB
    SpiWrite(0x00 | (CA & 0xF));
}


static void ShowChar(unsigned char x, unsigned char y, unsigned char ascii_word)
{
    int i = 0, k = 0;
    unsigned char temp;
    k = (ascii_word - 32) * 16;

    for (i = 0;i < 8;i++)
    {
        SetPACA((x*2), (129 - (y*8) - i));
        temp = Ascii[k+i];
        SpiWrite(0x100 | temp);
    }

    for (i = 0;i < 8;i++)
    {
        SetPACA((x*2) + 1, (129 - (y*8) - i));
        temp = Ascii[k+i+8];
        SpiWrite(0x100 | temp);
    }
}
void LCD_EnableBackLight(void)
{
    P11 = 0;
}
void LCD_DisableBackLight(void)
{
    P11 = 1;
}
void LCD_Print(unsigned char line, char *str)
{
    int i = 0;
    do
    {
        ShowChar(line, i, *str++);
        i++;
        if (i > 15)
            break;
    }
    while (*str != '\0');
}



void LCD_ClearScreen(void)
{
    int i = 0;
    /*CLEAR ALL PANNAL*/
    SetPACA(0x0, 0x0);

    for (i = 0; i < 132 *8; i++)
    {
        SpiWrite(0x100);
    }
    SpiWrite(0x10f);
}

