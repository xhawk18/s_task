/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright (c) Nuvoton Technology Corp. All rights reserved.                                             */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include "M051Series.h"
#include "LCD_Driver.h"
#include "SPIFLASH_Driver.h"
#include "SPI.h"

#define PLLCON_SETTING      SYSCLK_PLLCON_50MHz_XTAL
#define PLL_CLOCK           50000000


uint32_t g_au32PageBuf[256/4];

char g_strBuf[32] = {0};

volatile int8_t g_isPress = 0;

void EINT0_IRQHandler(void)
{
    P3->ISRC = 1 << 2;

    g_isPress = 1;

    /* Toggle LED */
    P20 = P20 ^ 1;

    printf("EINT0 Interrupt!\n");

    LCD_Print(3, "EINT0 Interrupt!");
}

void Delay(uint32_t ms)
{
    int32_t i;
    
    for(i=0;i<ms;i++)
        SYS_SysTickDelay(1000);    
}

void SpiTest(void)
{
    uint8_t *pu8;
    int32_t i;

    pu8 = (uint8_t *)g_au32PageBuf;

    /* Erase SPI flash */
    LCD_Print(2, "Erase Flash.....");
    LCD_Print(3, "                ");
    
    SPIFLASH_WaitReady();
    SPIFLASH_WriteEnable();
    /*All chip erase*/
    SPIFLASH_ChipErase();
    //SPIFLASH_SectorErase(0);
    //SPIFLASH_32kBlockErase(0);
    //SPIFLASH_64kBlockErase(0);
    
    /* Wait ready */
    SPIFLASH_WaitReady();

    /* Verify */
    SPIFLASH_Read(0, g_au32PageBuf, 256/8);
    for(i=0;i<256/4;i++)
    {
        for(i=0;i<256;i++)
        {
            if(pu8[i] != 0xFFUL)
            {
                LCD_Print(3, "Erase Error!");
                while(1);
            }
        }
    }


    /* Get the status of SPI flash */
    LCD_Print(3, "Erase done !!!  ");
    
    Delay(1000);

    /* Program SPI flash */
    LCD_Print(2, "Program Flash...");
    LCD_Print(3, "                ");
    /* source data */
    for(i=0;i<256;i++)
        pu8[i] = i;
    
    SPIFLASH_WaitReady();
    SPIFLASH_WriteEnable();
    SPIFLASH_PageProg(0, g_au32PageBuf);
    LCD_Print(3, "Program done !!!");
    
    Delay(1000);

    /* clear page buffer */
    for(i=0;i<256/4;i++)
        g_au32PageBuf[i] = 0;

    /* Read back and compare */
    LCD_Print(2, "Verify Flash....");
    LCD_Print(3, "                ");

    SPIFLASH_WaitReady();
    SPIFLASH_WriteDisable();
    SPIFLASH_Read(0, g_au32PageBuf, 256/8);

    
    for(i=0;i<256;i++)
    {
        if(pu8[i] != i)
        {
            LCD_Print(3, "F0 Verify Error!");
            while(1);
        }
    }

    LCD_Print(3, "Verify done !!! ");
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

    /* Switch HCLK clock source to internal RC */
    SYSCLK->CLKSEL0 = SYSCLK_CLKSEL0_HCLK_IRC22M;

    /* Set PLL to power down mode and PLL_STB bit in CLKSTATUS register will be cleared by hardware.*/
    SYSCLK->PLLCON |= SYSCLK_PLLCON_PD_Msk;

    /* Enable external 12MHz XTAL, 10kHz */
    SYSCLK->PWRCON |= SYSCLK_PWRCON_XTL12M_EN_Msk | SYSCLK_PWRCON_IRC10K_EN_Msk;

    /* Enable PLL and Set PLL frequency */        
    SYSCLK->PLLCON = PLLCON_SETTING;
    
    /* Waiting for clock ready */
    SYS_WaitingForClockReady(SYSCLK_CLKSTATUS_PLL_STB_Msk | SYSCLK_CLKSTATUS_XTL12M_STB_Msk);

    /* Switch HCLK clock source to PLL, STCLK to HCLK/2 */
    SYSCLK->CLKSEL0 = SYSCLK_CLKSEL0_STCLK_HCLK_DIV2 | SYSCLK_CLKSEL0_HCLK_PLL;

    /* Enable IP clock */        
    SYSCLK->APBCLK = SYSCLK_APBCLK_UART0_EN_Msk | SYSCLK_APBCLK_SPI0_EN_Msk |
                        SYSCLK_APBCLK_SPI1_EN_Msk;
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
    /* Set P3 multi-function pins for UART0 RXD and TXD  */
    SYS->P3_MFP = SYS_MFP_P30_RXD0 | SYS_MFP_P31_TXD0;
    /* Set P1.4, P1.5, P1.6, P1.7 for SPI0 */
    SYS->P1_MFP = SYS_MFP_P14_SPISS0 | SYS_MFP_P15_MOSI_0 | SYS_MFP_P16_MISO_0 | SYS_MFP_P17_SPICLK0;
    /* Set P0.4, P0.5, P0.6, P0.7 for SPI1 */
    SYS->P0_MFP = SYS_MFP_P04_SPISS1 | SYS_MFP_P05_MOSI_1 | SYS_MFP_P06_MISO_1 | SYS_MFP_P07_SPICLK1;

    /* Lock protected registers */
    SYS_LockReg();
}

void UART0_Init(void)
{
/*---------------------------------------------------------------------------------------------------------*/
/* Init UART                                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
    UART0->BAUD = UART_BAUD_MODE2 | UART_BAUD_DIV_MODE2(PLL_CLOCK, 115200);
    _UART_SET_DATA_FORMAT(UART0, UART_WORD_LEN_8 | UART_PARITY_NONE | UART_STOP_BIT_1);
}

void GPIO_Init(void)
{
    /* Debounce function control */
    GPIO->DBNCECON = GPIO_DBNCECON_ICLK_ON | GPIO_DBNCECON_DBCLKSRC_HCLK | GPIO_DBNCECON_DBCLKSEL_32768;
    P3->DBEN = GPIO_DBEN_ENABLE(2);

    /* Configure external interrupt */
    GPIO_EnableInt(P3, 2, GPIO_INT_BOTH_EDGE);
    NVIC_EnableIRQ(EINT0_IRQn);

}

#define SPIFLASH_CNTRL(x)  (uint32_t)(SPI_CNTRL_CLK_IDLE_LOW | SPI_CNTRL_TX_FALLING | SPI_CNTRL_RX_RISING | SPI_CNTRL_TX_BIT_LEN(x))
//#define SPIFLASH_CNTRL(x)  (uint32_t)(SPI_CNTRL_CLK_IDLE_LOW | SPI_CNTRL_TX_RISING | SPI_CNTRL_RX_FALLING | SPI_CNTRL_TX_BIT_LEN(x))

uint32_t SPI_Read(uint32_t addr)
{
	uint32_t value;
    SPI1->SSR = SPI_SSR_SW_SS_PIN_LOW;

    SPI1->TX[0] = 0x3;
	SPI1->CNTRL = SPIFLASH_CNTRL(8) | SPI_CNTRL_GO_BUSY;
	while(SPI1->CNTRL & SPI_CNTRL_GO_BUSY);

    SPI1->TX[0] = addr;
	SPI1->CNTRL = SPIFLASH_CNTRL(8) | SPI_CNTRL_GO_BUSY;    
	while(SPI1->CNTRL & SPI_CNTRL_GO_BUSY);

    SPI1->TX[0] = 0xFF;
	SPI1->CNTRL = SPIFLASH_CNTRL(8) | SPI_CNTRL_GO_BUSY;    
	while(SPI1->CNTRL & SPI_CNTRL_GO_BUSY);
    
    value = SPI1->RX[0];
    SPI1->SSR = SPI_SSR_SW_SS_PIN_HIGH;
	{volatile int i; for(i = 0; i < 100000; ++i);}

	printf("%08x\n", value);
}


uint32_t SPI_Write(uint32_t addr, uint32_t value)
{
    SPI1->SSR = SPI_SSR_SW_SS_PIN_LOW;

    SPI1->TX[0] = 0x6;
	SPI1->CNTRL = SPIFLASH_CNTRL(8) | SPI_CNTRL_GO_BUSY;
	while(SPI1->CNTRL & SPI_CNTRL_GO_BUSY);
    SPI1->SSR = SPI_SSR_SW_SS_PIN_HIGH;	
	{volatile int i; for(i = 0; i < 100000; ++i);}

	printf("write enabled\n");
	
	
    SPI1->SSR = SPI_SSR_SW_SS_PIN_LOW;
    SPI1->TX[0] = 0x2;
	SPI1->CNTRL = SPIFLASH_CNTRL(8) | SPI_CNTRL_GO_BUSY;
	while(SPI1->CNTRL & SPI_CNTRL_GO_BUSY);

    SPI1->TX[0] = addr;
	SPI1->CNTRL = SPIFLASH_CNTRL(8) | SPI_CNTRL_GO_BUSY;    
	while(SPI1->CNTRL & SPI_CNTRL_GO_BUSY);

    SPI1->TX[0] = value;
	SPI1->CNTRL = SPIFLASH_CNTRL(8) | SPI_CNTRL_GO_BUSY;    
	while(SPI1->CNTRL & SPI_CNTRL_GO_BUSY);
    
    SPI1->SSR = SPI_SSR_SW_SS_PIN_HIGH;
	{volatile int i; for(i = 0; i < 100000; ++i);}
	
	printf("write: %08x\n", value);
}


//SYS->P0_MFP = SYS_MFP_P04_SPISS1 | SYS_MFP_P05_MOSI_1 | SYS_MFP_P06_MISO_1 | SYS_MFP_P07_SPICLK1;
void delay() {
	volatile int i;
	for(i = 0; i < 10000; ++i);
}
void ss(int value) {
	if(value) P0->DOUT |=  (1 << 4);
	else P0->DOUT &= ~(1 << 4);
}

void so(int value) {	
	//printf("%d ", value);
	if(value) P0->DOUT |=  (1 << 5);
	else P0->DOUT &= ~(1 << 5);
}

int si() {
	return (P0->PIN >> 6) & 1;
}

void clk(int value) {
	if(value) P0->DOUT |=  (1 << 7);
	else P0->DOUT &= ~(1 << 7);
}

void spi_read16(uint32_t addr) {
	uint32_t value = 0;
	
	//Read
	ss(0);
	//clk(1);
	delay();
	ss(1);
	delay();

	if(1){
		int i;
		uint32_t value = 0x6;
		for(i = 3; i-- > 0; ) {
			so(  (value >> i) & 1 );
			clk(0);
			delay();
			clk(1);
			delay();
		}
	}
		
	{
		int i;
		uint32_t value = addr;// | 0xC0;
		for(i = 7; i-- > 0; ) {
			so(  (value >> i) & 1 );
			clk(0);
			delay();
			clk(1);
			delay();
		}
	}
	
	{int n; for(n = 0; n < 6; ++n){
	//while(1){
		int i;
		uint32_t bit;
		value = 0;
		for(i = 17; i-- > 0; ) {
			clk(0);
			//so( 1 );
			//value = si();
			//printf("i=%d,%08x %08x %d, ", i,  P0->DOUT, P0->PIN, value);
			delay();
			//value = si();
			//printf("%08x %d, ", P0->PIN, value);
			clk(1);
			//printf("%08x %d, ", P0->PIN, value);
			delay();
			bit = si();
			//value = si();
			//printf("%08x %d\n", P0->PIN, value);
			value <<= 1;
			value |= bit;
		}
		printf("%04x:", value);
		
		//{static int n = 0;	if(++n % 8 == 0) printf("\n");}
		break;
	}
	}
	//clk(0);	
	//delay();
	ss(0);
	delay();
	
		
}



void spi_read(uint32_t addr) {
	uint32_t value = 0;
	
	//Read
	ss(0);
	clk(0);
	delay();
	ss(1);
	delay();

	if(1){
		int i;
		uint32_t value = 0x6;
		for(i = 3; i-- > 0; ) {
			so(  (value >> i) & 1 );
			clk(0);
			delay();
			clk(1);
			delay();
		}
	}
		
	{
		int i;
		uint32_t value = addr;// | 0xC0;
		for(i = 8; i-- > 0; ) {
			so(  (value >> i) & 1 );
			clk(0);
			delay();
			clk(1);
			delay();
		}
	}
	
	{int n; for(n = 0; n < 6; ++n){
	//while(1){
		int i;
		uint32_t bit;
		value = 0;
		for(i = 9; i-- > 0; ) {
			clk(0);
			//so( 1 );
			//value = si();
			//printf("i=%d,%08x %08x %d, ", i,  P0->DOUT, P0->PIN, value);
			delay();
			//value = si();
			//printf("%08x %d, ", P0->PIN, value);
			//bit = si();
			clk(1);
			//printf("%08x %d, ", P0->PIN, value);
			delay();
			bit = si();
			//value = si();
			//printf("%08x %d\n", P0->PIN, value);
			value <<= 1;
			value |= bit;
		}
		printf("%02x:", value);
		
		//{static int n = 0;	if(++n % 8 == 0) printf("\n");}
		break;
	}
	}
	//clk(0);	
	//delay();
	ss(0);
	delay();
	
		
}

int main(void)
{
	int i;
    char strClearLine[15] = "ID:            ";
    /* Init System, IP clock and multi-function I/O */
    SYS_Init();     

    /* Init UART0 for printf */
    UART0_Init();   

    //printf("CPU @ %dHz\n", SystemCoreClock);
	
	SYS->P0_MFP = 0;
	_GPIO_SET_PIN_MODE(P0, 3, GPIO_PMD_OUTPUT);
	P0->DOUT |=  (1 << 3);
	
	_GPIO_SET_PIN_MODE(P0, 4, GPIO_PMD_OUTPUT);
	_GPIO_SET_PIN_MODE(P0, 5, GPIO_PMD_OUTPUT);
	_GPIO_SET_PIN_MODE(P0, 6, GPIO_PMD_INPUT);
	_GPIO_SET_PIN_MODE(P0, 7, GPIO_PMD_OUTPUT);

	/* Init */
	clk(1);
	ss(0);
	
	//spi_read(0x0FA);
	//printf("\n");
	//return;
#if 1
	for(i = 0; i < 1024; ++i){
		spi_read(i);
		if((i + 1) % 8 == 0)
			printf("\r\n i=%d, ", i);
	}
	return;
#endif	
	
	spi_read(0xFA);
	spi_read(0xFB);
	spi_read(0xFC);
	spi_read(0xFD);
	spi_read(0xFE);
	spi_read(0xFF);
	printf("\n");
	
	while(1);


    /* Init SPI0 and LCD */
    //LCD_Init();
    //LCD_EnableBackLight();
    //LCD_ClearScreen();


    //LCD_Print(0, "SPI Sample Code ");
    //LCD_Print(1, "For Flash Test");
    //LCD_Print(2, "Press INT button");

    /* Init P3.2 */
    //GPIO_Init();

    SPIFLASH_Init();
	
	SPI_Write(0x00, 0x5A);
	SPI_Read(0x00);
	SPI_Read(0xFA);
	SPI_Read(0xFB);
	SPI_Read(0xFC);
	SPI_Read(0xFD);
	SPI_Read(0xFE);
	
    //sprintf(g_strBuf, "ID:%x         ", SPIFLASH_ReadId()&0xFFFFUL);
    //LCD_Print(3, strClearLine);
    //LCD_Print(3, g_strBuf);

    while (!g_isPress);

    //SpiTest();

    return 1;
}
