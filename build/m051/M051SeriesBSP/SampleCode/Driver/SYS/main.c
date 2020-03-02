/**************************************************************************//**
 * @file     Smpl_DrvSYS.c
 * @version  V2.00
 * $Revision: 5 $
 * $Date: 12/08/09 7:12p $ 
 * @brief    M051 Series Global Control and Clock Control Driver Sample Code
 *
 * @note
 * Copyright (C) 2011 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#include <stdio.h>
#include "M051Series.h"

#define PLLCON_SETTING      SYSCLK_PLLCON_50MHz_XTAL
#define PLL_CLOCK           50000000

#define SIGNATURE       0x125ab234
#define FLAG_ADDR       0x20000FFC

/*---------------------------------------------------------------------------------------------------------*/
/*  Brown Out Detector IRQ Handler                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void BOD_IRQHandler(void)
{
    /* Clear Interrupt Flag */
    SYS->BODCR |= SYS_BODCR_BOD_INTF_Msk;

    printf("Brown Out is Detected\n");    
}

/*---------------------------------------------------------------------------------------------------------*/
/*  Power Down Wake Up IRQ Handler                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void PWRWU_IRQHandler(void)
{
    /* Clear Power Down Wake Up interrupt flag */
    SYSCLK->PWRCON |= SYSCLK_PWRCON_PD_WU_STS_Msk;    

    printf("Wake Up Interrupt is asserted\n");
}


/*---------------------------------------------------------------------------------------------------------*/
/*  Simple calculation test function                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
#define PI_NUM  256
int32_t f[PI_NUM+1];
uint32_t piTbl[19] = {
3141,
5926,
5358,
9793,
2384,
6264,
3383,
2795,
 288,
4197,
1693,
9937,
5105,
8209,
7494,
4592,
3078,
1640,
6284
};

int32_t piResult[19];

int32_t pi(void) 
{
    int32_t i,i32Err;
    int32_t a=10000, b=0, c=PI_NUM, d=0, e=0, g=0;     

    for(;b-c;)
        f[b++]=a/5;
    
    i = 0;
    for(;d=0,g=c*2;c -=14,/*printf("%.4d\n",e+d/a),*/ piResult[i++] = e+d/a,e=d%a)
    {
        if(i==19)
            break;

        for(b=c; d+=f[b]*a,f[b]=d%--g,d/=g--,--b; d*=b);
    }
    i32Err = 0;
    for(i=0;i<19;i++)
    {
        if(piTbl[i] != piResult[i])
            i32Err = -1;    
    }

    return i32Err;
}

void Delay(uint32_t x)
{
    int32_t i;
    
    for(i=0;i<x;i++)
    {
        __NOP();
        __NOP();
    }
}

uint32_t g_au32PllSetting[] = 
{
     SYSCLK_PLLCON_FIN_XTAL | SYSCLK_PLLCON_NR(3) | SYSCLK_PLLCON_NF(25) | SYSCLK_PLLCON_NO_4,  /* PLL = 25MHz */
     SYSCLK_PLLCON_FIN_XTAL | SYSCLK_PLLCON_NR(3) | SYSCLK_PLLCON_NF(29) | SYSCLK_PLLCON_NO_4,  /* PLL = 29MHz */
     SYSCLK_PLLCON_FIN_XTAL | SYSCLK_PLLCON_NR(3) | SYSCLK_PLLCON_NF(33) | SYSCLK_PLLCON_NO_4,  /* PLL = 33MHz */
     SYSCLK_PLLCON_FIN_XTAL | SYSCLK_PLLCON_NR(3) | SYSCLK_PLLCON_NF(37) | SYSCLK_PLLCON_NO_4,  /* PLL = 37MHz */
     SYSCLK_PLLCON_FIN_XTAL | SYSCLK_PLLCON_NR(3) | SYSCLK_PLLCON_NF(41) | SYSCLK_PLLCON_NO_4,  /* PLL = 41MHz */
     SYSCLK_PLLCON_FIN_XTAL | SYSCLK_PLLCON_NR(3) | SYSCLK_PLLCON_NF(45) | SYSCLK_PLLCON_NO_4,  /* PLL = 45MHz */
     SYSCLK_PLLCON_FIN_XTAL | SYSCLK_PLLCON_NR(3) | SYSCLK_PLLCON_NF(49) | SYSCLK_PLLCON_NO_4   /* PLL = 49MHz */
};

void SYS_Init(void)
{
    /* Unlock protected registers */
    SYS_UnlockReg();
    
/*---------------------------------------------------------------------------------------------------------*/
/* Init System Clock                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/

    /* Enable Internal RC clock */
    SYSCLK->PWRCON |= SYSCLK_PWRCON_IRC22M_EN_Msk;

    /* Waiting for IRC22M clock ready */
    SYS_WaitingForClockReady(SYSCLK_CLKSTATUS_IRC22M_STB_Msk);

    /* Switch HCLK clock source to Internal RC */
    SYSCLK->CLKSEL0 = SYSCLK_CLKSEL0_HCLK_IRC22M;

    /* Set PLL to power down mode and PLL_STB bit in CLKSTATUS register will be cleared by hardware.*/
    SYSCLK->PLLCON |= SYSCLK_PLLCON_PD_Msk;

    /* Enable external 12MHz XTAL, internal 22.1184MHz */
    SYSCLK->PWRCON |= SYSCLK_PWRCON_XTL12M_EN_Msk | SYSCLK_PWRCON_IRC22M_EN_Msk;

    /* Enable PLL and Set PLL frequency */        
    SYSCLK->PLLCON = PLLCON_SETTING;
    
    /* Waiting for clock ready */
    SYS_WaitingForClockReady(SYSCLK_CLKSTATUS_PLL_STB_Msk | SYSCLK_CLKSTATUS_XTL12M_STB_Msk | SYSCLK_CLKSTATUS_IRC22M_STB_Msk);

    /* Switch HCLK clock source to PLL, STCLK to HCLK/2 */
    SYSCLK->CLKSEL0 = SYSCLK_CLKSEL0_STCLK_HCLK_DIV2 | SYSCLK_CLKSEL0_HCLK_PLL;

    /* Enable IP clock */        
    SYSCLK->APBCLK = SYSCLK_APBCLK_UART0_EN_Msk;
    
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
    /* Set P3 multi-function pins for UART0 RXD and TXD, and Clock Output */
    SYS->P3_MFP = SYS_MFP_P30_RXD0 | SYS_MFP_P31_TXD0 | SYS_MFP_P36_CKO;

    /* Lock protected registers */
    SYS_LockReg();
}

void SYS_PLL_Test(void)
{
    int32_t  i;

/*---------------------------------------------------------------------------------------------------------*/
/* PLL clock confiruation test                                                                             */
/*---------------------------------------------------------------------------------------------------------*/
    
    printf("\n-------------------------[ Test PLL ]-----------------------------\n");
    
    for (i = 0; i < sizeof(g_au32PllSetting)/sizeof(g_au32PllSetting[0]) ; i++)
    {
        /* Switch HCLK clock source to XTAL */
        SYSCLK->CLKSEL0 = SYSCLK_CLKSEL0_HCLK_XTAL;
        
        /* Set PLL to power down mode and PLL_STB bit in CLKSTATUS register will be cleared by hardware.*/
        SYSCLK->PLLCON|= SYSCLK_PLLCON_PD_Msk;
        
        /* Set PLL frequency */
        SYSCLK->PLLCON = g_au32PllSetting[i];
    
        /* Waiting for PLL clock ready */
        SYS_WaitingForClockReady(SYSCLK_CLKSTATUS_PLL_STB_Msk);
    
        /* Switch HCLK clock source to PLL */
        SYSCLK->CLKSEL0 = SYSCLK_CLKSEL0_HCLK_PLL;       
        /* Update System Core Clock */
        SystemCoreClockUpdate();
         
        printf("  Change system clock to %d Hz ......................... ", SystemCoreClock);
        
        /* Ouput selected clock to CKO, CKO Clock = HCLK / 2^(1 + 1) */
        SYS_EnableCKO(SYSCLK_CLKSEL2_FRQDIV_HCLK, 1);
        
        /* The delay loop is used to check if the CPU speed is increasing */
        Delay(0x400000);
        
        if (pi())
        {
            printf("[FAIL]\n");
        }
        else
        {
            printf("[OK]\n");
        }

        /* Disable CKO clock */
        SYS_DisableCKO();
    }
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
    UART0->BAUD = UART_BAUD_MODE2 | UART_BAUD_DIV_MODE2(__XTAL, 115200);
    _UART_SET_DATA_FORMAT(UART0, UART_WORD_LEN_8 | UART_PARITY_NONE | UART_STOP_BIT_1);
}

/*---------------------------------------------------------------------------------------------------------*/
/*  Main Function                                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
int32_t main (void)
{
    uint32_t u32data;
    
    /* If define INIT_SYSCLK_AT_BOOTING in system_M051Series.h, HCLK will be set to 50MHz in SystemInit(void)*/
    if(SYS->REGWRPROT == 1) // In end of main function, program issued CPU reset and write-protection will be disabled.
        SYS_LockReg();
    
    /* Init System, IP clock and multi-function I/O */
    SYS_Init(); //In the end of SYS_Init() will issue SYS_LockReg() to lock protected register. If user want to write protected register, please issue SYS_UnlockReg() to unlock protected register.    

    /* Init UART0 for printf */
    UART0_Init();
    
    printf("\n\nCPU @ %dHz\n", SystemCoreClock);

    /*
        This sample code will show some function about system manager controller and clock controller:
        1. Read PDID 
        2. Get and clear reset source
        3. Setting about BOD
        4. Change system clock depended on different PLL settings
        5. Outout system clock from CKO pin, and the output frequency = system clock / 4  
    */
    
    printf("+---------------------------------------+\n");
    printf("|    M05xx System Driver Sample Code    |\n");
    printf("+---------------------------------------+\n");

    if (M32(FLAG_ADDR) == SIGNATURE)
    {
        printf("  CPU Reset success!\n");
        M32(FLAG_ADDR) = 0;
        printf("  Press any key to continue ...\n");
        getchar();
    }

/*---------------------------------------------------------------------------------------------------------*/
/* Misc system function test                                                                               */
/*---------------------------------------------------------------------------------------------------------*/

    /* Read Part Device ID */
    printf("Product ID 0x%x\n", SYS->PDID);
        
    /* Get reset source from last operation */
    u32data = SYS->RSTSRC;
    printf("Reset Source 0x%x\n", u32data);
    
    /* Clear reset source */
    SYS->RSTSRC = u32data;

    /* Unlock protected registers for Brown-Out Detector settings */
    SYS_UnlockReg();
    
    /* Check if the write-protected registers are unlocked before BOD setting and CPU Reset */
    if (SYS->REGWRPROT != 0)
    {
        printf("Protected Address is Unlocked\n");    
    }

    /* Enable Brown-Out Detector and Low Voltage Reset function, and set Brown-Out Detector voltage 2.7V */    
    SYS->BODCR =SYS_BODCR_BOD_EN_Msk | SYS_BODCR_BOD_VL_2_7V | SYS_BODCR_LVR_EN_Msk;

    /* Enable Brown-Out Interrupt function */
    SYS->BODCR &= ~SYS_BODCR_BOD_RSTEN_Msk;
    NVIC_EnableIRQ(BOD_IRQn);
            
    /* Get system clock frequency and PLL clock frequency */
    printf("  Change system clock to %d Hz and PLL clock is %d Hz\n", SystemCoreClock, PllClock);

    /* Run PLL Test */
    SYS_PLL_Test();
        
    /* Write a signature work to SRAM to check if it is reset by software */   
    M32(FLAG_ADDR) = SIGNATURE; 
    printf("\n\n  >>> Reset CPU <<<\n");

    /* Waiting for message send out */
    _UART_WAIT_TX_EMPTY(UART0);

    /* Switch HCLK clock source to Internal 22MHz */
    SYSCLK->CLKSEL0 = SYSCLK_CLKSEL0_HCLK_IRC22M; 
    
    /* Set PLL to Power down mode and HW will also clear PLL_STB bit in CLKSTATUS register */
    SYSCLK->PLLCON |= SYSCLK_PLLCON_PD_Msk;  
      
    /* Reset CPU */
    _SYS_RESET_CPU();
}




