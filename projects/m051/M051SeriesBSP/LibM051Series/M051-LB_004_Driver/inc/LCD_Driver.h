/**************************************************************************//**
 * @file     LCD_Driver.h
 * @version  V2.1
 * $Revision: 2 $
 * $Date: 12/04/24 1:56p $
 * @brief    LCD Driver Header File
 *
 * @note
 * Copyright (C) 2011 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#ifndef __LCD_DRIVER_H__
#define __LCD_DRIVER_H__

extern void LCD_ClearScreen(void);
extern void LCD_Print(unsigned char line, char *str);
extern void LCD_EnableBackLight(void);
extern void LCD_DisableBackLight(void);
extern void LCD_Init(void);
#endif
