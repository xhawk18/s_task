/****************************************************************************
 * @file     main.c
 * @version  V2.10
 * $Revision: 5 $
 * $Date: 12/08/09 7:02p $ 
 * @brief    M051 Series External Bus Interface Driver Sample Code
 *
 * @note
 * Copyright (C) 2011 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#include <stdio.h>
#include "M051Series.h"
#include "EBI_NOR.h"

#define PLLCON_SETTING      SYSCLK_PLLCON_32MHz_XTAL
#define PLL_CLOCK           32000000



extern void SRAM_BS616LV4017(void);	


/*---------------------------------------------------------------------------------------------------------*/
/* Program Continue Data to NOR Flash                                                                      */
/*---------------------------------------------------------------------------------------------------------*/    
uint8_t ProgramContinueDataTest(void)
{
	uint8_t u8DataIn, u8DataOut;
	uint32_t u32NORAddr;

	printf("    >> Start to program flash ... \n");

    /* Write */
	for (u32NORAddr=0; u32NORAddr<EBI_MAX_SIZE; u32NORAddr++)
	{
        u8DataIn = (u32NORAddr%256)+u32NORAddr;
		if (NOR_WriteData(u32NORAddr, u8DataIn) == FALSE)
		{
			printf("Program [0x%05X]:[0x%02X] FAIL !!! \n\n", u32NORAddr, u8DataIn);
			return FALSE;
		}

	}

    /* Read */
	for (u32NORAddr=0; u32NORAddr<EBI_MAX_SIZE; u32NORAddr++)
	{
        u8DataIn = (u32NORAddr%256)+u32NORAddr;
        u8DataOut = NOR_ReadData(u32NORAddr);
		if (u8DataOut != u8DataIn)
		{
			printf("Read [0x%05X]:[0x%02X] FAIL !!! (Got [0x%02X]) \n\n", u32NORAddr, u8DataIn, u8DataOut);
			printf("Program flash FAIL !!! \n\n");
			return FALSE;
		}

	}
	printf("    >> Continue Data Program OK !!! \n\n");
		
	return TRUE;
}



void SYS_Init(void)
{
/*---------------------------------------------------------------------------------------------------------*/
/* Init System Clock                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Enable Internal RC clock */
    SYSCLK->PWRCON |= SYSCLK_PWRCON_IRC22M_EN_Msk;

    /* Waiting for IRC22M clock ready */
    SYS_WaitingForClockReady(SYSCLK_CLKSTATUS_IRC22M_STB_Msk);

    /* Switch HCLK clock source to Internal RC */
    SYSCLK->CLKSEL0 = SYSCLK_CLKSEL0_HCLK_IRC22M;

    /* Set PLL to power down mode and PLL_STB bit in CLKSTATUS register will be cleared by hardware.*/
    SYSCLK->PLLCON |= SYSCLK_PLLCON_PD_Msk;

    /* Enable external 12MHz XTAL */
    SYSCLK->PWRCON |= SYSCLK_PWRCON_XTL12M_EN_Msk;

    /* Enable PLL and Set PLL frequency */        
    SYSCLK->PLLCON = PLLCON_SETTING;

    /* Waiting for clock ready */
    SYS_WaitingForClockReady(SYSCLK_CLKSTATUS_PLL_STB_Msk | SYSCLK_CLKSTATUS_XTL12M_STB_Msk);

    /* Switch HCLK clock source to PLL, STCLK to HCLK/2 */
    SYSCLK->CLKSEL0 = SYSCLK_CLKSEL0_STCLK_HCLK_DIV2 | SYSCLK_CLKSEL0_HCLK_PLL;

    /* Enable EBI clock */        
    SYSCLK->AHBCLK |= SYSCLK_AHBCLK_EBI_EN_Msk;

    /* Enable IP clock */        
    SYSCLK->APBCLK = SYSCLK_APBCLK_UART0_EN_Msk;

    /* IP clock source */
    SYSCLK->CLKSEL1 = SYSCLK_CLKSEL1_UART_PLL;

    /* Update System Core Clock */
    /* User can use SystemCoreClockUpdate() to calculate PllClock, SystemCoreClock and CycylesPerUs automatically. */
    //SystemCoreClockUpdate(); 
    PllClock        = PLL_CLOCK;            // PLL
    SystemCoreClock = PLL_CLOCK / 1;        // HCLK
    CyclesPerUs     = PLL_CLOCK / 1000000;  // For SYS_SysTickDelay()

/*---------------------------------------------------------------------------------------------------------*/
/* Init I/O Multi-function                                                                                 */
/*---------------------------------------------------------------------------------------------------------*/
    /* Set P3 multi-function pins for UART0 RXD, TXD and EBI MCLK, nWR and nRD */
    SYS->P3_MFP = SYS_MFP_P30_RXD0 | SYS_MFP_P31_TXD0 | SYS_MFP_P33_MCLK | SYS_MFP_P36_nWR | SYS_MFP_P37_nRD;

    /* Set P0 multi-function pins for EBI AD0 ~ AD7 */
    SYS->P0_MFP = SYS_MFP_P00_AD0 | SYS_MFP_P01_AD1 | SYS_MFP_P02_AD2 | SYS_MFP_P03_AD3 |
                     SYS_MFP_P04_AD4 | SYS_MFP_P05_AD5 | SYS_MFP_P06_AD6 | SYS_MFP_P07_AD7;

    /* Set P2 multi-function pins for EBI AD8 ~ AD15 */
    SYS->P2_MFP = SYS_MFP_P20_AD8 | SYS_MFP_P21_AD9 | SYS_MFP_P22_AD10 | SYS_MFP_P23_AD11 |
                     SYS_MFP_P24_AD12 | SYS_MFP_P25_AD13 | SYS_MFP_P26_AD14 | SYS_MFP_P27_AD15;

    /* Set P1 multi-function pins for EBI nWRL and nWRH */
    SYS->P1_MFP = SYS_MFP_P10_nWRL | SYS_MFP_P11_nWRH;

    /* Set P4 multi-function pins for EBI nCS, ALE and ICE CLK and DAT */
    SYS->P4_MFP = SYS_MFP_P44_nCS | SYS_MFP_P45_ALE | SYS_MFP_P46_ICE_CLK | SYS_MFP_P47_ICE_DAT;

    /* Lock protected registers */
    SYS_LockReg();
}


void UART0_Init(void)
{
/*---------------------------------------------------------------------------------------------------------*/
/* Init UART                                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
    /* Reset IP */
    SYS->IPRSTC2 |=  SYS_IPRSTC2_UART0_RST_Msk;
    SYS->IPRSTC2 &= ~SYS_IPRSTC2_UART0_RST_Msk;

    /* Configure UART0 and set UART0 Baudrate */
    UART0->BAUD = UART_BAUD_MODE2 | UART_BAUD_DIV_MODE2(PLL_CLOCK, 115200);
    _UART_SET_DATA_FORMAT(UART0, UART_WORD_LEN_8 | UART_PARITY_NONE | UART_STOP_BIT_1);
}


/*---------------------------------------------------------------------------------------------------------*/
/* MAIN function                                                                                           */
/*---------------------------------------------------------------------------------------------------------*/    
int main (void)
{
	uint8_t	u8Item = 0x0;
    uint32_t u32NORIDInfo;
    uint32_t u32i;
    uint8_t u8ReadOutData;

    /* Init System, IP clock and multi-function I/O */
    SYS_Init();     

    /* Init UART0 for printf */
    UART0_Init();

    printf("\n\nCPU @ %dHz\n", SystemCoreClock);

    printf("+------------------------------------+\n");
    printf("|    M05xx EBI Driver Sample Code    |\n");
    printf("+------------------------------------+\n");
	printf("\n");
	
	do {
		printf("*****************************************************************\n");
		printf("* Please connect BS616LV4017 or W39L010 to M051 Series EBI bus  *\n");
		printf("* before EBI testing!!                                          *\n");
		printf("*                                                               *\n");								
		printf("* The testing result will be FAILED, if there is no BS616LV4017 *\n");
		printf("* or W39L010 connecting to M051 sereis EBI bus!                 *\n");
		printf("*****************************************************************\n");
		printf("\n");
		printf("|   >>> Selese item to test <<<    |\n");
		printf("| [0] SRAM  --- BS616LV4017, 16bit |\n");
		printf("| [1] NOR   --- W39L010, 8bit      |\n");
		printf("| [ESC] Exit                       |\n");
		u8Item = getchar();

		printf("\n");
		switch (u8Item)
		{
			case '0':               
                /* Enable EBI function and data width 16-bit, MCLK is HCLK/4 */
                EBI->EBICON = EBI_EBICON_ExtEN_Msk | EBI_EBICON_ExtBW16_Msk | EBI_EBICON_MCLKDIV_4; 

                /* Start SRAM test */
                SRAM_BS616LV4017();

                /* Disable EBI function */
                EBI->EBICON = 0;
				break;
				
			case '1':		
                /* Enable EBI function and data width 8-bit, MCLK is HCLK/4 */
                EBI->EBICON = EBI_EBICON_ExtEN_Msk | EBI_EBICON_MCLKDIV_4; 

                /* Initial NOR flash and check ID */
				NOR_Init();
                u32NORIDInfo = NOR_GetID();
            	if (u32NORIDInfo == 0xDA31)
            	{
            		printf("NOR W39L010 initial OK ! ManufactureID:0x%X, DeviceID:0x%X. \n", (u32NORIDInfo>>8), (u32NORIDInfo&0xFF));
            	}else
            	{
            		printf("NOR W39L010 initial fail ! (ID:0x%X) \n\n", u32NORIDInfo);
            		break;
            	}

            	/* Erase flash */
            	NOR_Erase();	        		
        		for (u32i=0; u32i<EBI_MAX_SIZE; u32i++)
        		{
                    u8ReadOutData = NOR_ReadData(u32i);
        			if (u8ReadOutData != 0xFF)
        			{
        				printf("    >> Chip Erase Fail !! Addr:0x%X, Data:0x%X. \n\n", u32i, u8ReadOutData);
        				break;
        			}
        		}
        		printf("    >> Chip Erase OK !!! \n");

                /* Start to program NOR flash test */
                ProgramContinueDataTest();		

                /* Disable EBI function */
                EBI->EBICON = 0;
				break;
		}
	} while (u8Item != 27);
	
    /* Disable EBI clock */        
    SYSCLK->AHBCLK &= ~SYSCLK_AHBCLK_EBI_EN_Msk;

	printf("Exit EBI Driver Sample Code \n\n");
	
	return 1;
}




