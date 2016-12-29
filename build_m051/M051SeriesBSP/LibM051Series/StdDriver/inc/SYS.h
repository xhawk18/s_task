/**************************************************************************//**
 * @file     SYS.h
 * @version  V2.1
 * $Revision: 11 $
 * $Date: 12/08/10 3:12p $
 * @brief    M051 Series Global Control and Clock Control Driver Header File
 *
 * @note
 * Copyright (C) 2011 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#ifndef __SYS_H__
#define __SYS_H__

#include "M051Series.h"


/** @addtogroup M051_FUNC M051 Function Interface
  * @{
  */

/** @addtogroup SYS_FUNC SYS Device Function Interface
  * @{
  */


/*---------------------------------------------------------------------------------------------------------*/
/*  PLLCON constant definitions. PLL = FIN * NF / NR / NO                                                  */
/*---------------------------------------------------------------------------------------------------------*/
#define SYSCLK_PLLCON_FIN_IRC22M   0x00080000UL        /*!< For PLL clock source is internal RC clock. 4MHz < FIN < 24MHz */ 
#define SYSCLK_PLLCON_FIN_XTAL     0x00000000UL        /*!< For PLL clock source is external cristal.  4MHz < FIN < 24MHz */

#define SYSCLK_PLLCON_NR(x)        (((x)-2)<<9)        /*!< x must be constant and 2 <= x <= 33.  1.6MHz < FIN/NR < 15MHz */
#define SYSCLK_PLLCON_NF(x)         ((x)-2)            /*!< x must be constant and 2 <= x <= 513. 100MHz < FIN*NF/NR < 200MHz. (120MHz < FIN*NF/NR < 200MHz is preferred.) */ 
 
#define SYSCLK_PLLCON_NO_1         0x0000UL            /*!< For output divider is 1 */
#define SYSCLK_PLLCON_NO_2         0x4000UL            /*!< For output divider is 2 */
#define SYSCLK_PLLCON_NO_4         0xC000UL            /*!< For output divider is 4 */

#if (__XTAL == 12000000)
#define SYSCLK_PLLCON_50MHz_XTAL   (SYSCLK_PLLCON_FIN_XTAL | SYSCLK_PLLCON_NR(3) | SYSCLK_PLLCON_NF( 25) | SYSCLK_PLLCON_NO_2) /*!< Predefined PLLCON setting for 50MHz PLL output with 12MHz X'tal */
#define SYSCLK_PLLCON_48MHz_XTAL   (SYSCLK_PLLCON_FIN_XTAL | SYSCLK_PLLCON_NR(7) | SYSCLK_PLLCON_NF(112) | SYSCLK_PLLCON_NO_4) /*!< Predefined PLLCON setting for 48MHz PLL output with 12MHz X'tal */
#define SYSCLK_PLLCON_36MHz_XTAL   (SYSCLK_PLLCON_FIN_XTAL | SYSCLK_PLLCON_NR(7) | SYSCLK_PLLCON_NF( 84) | SYSCLK_PLLCON_NO_4) /*!< Predefined PLLCON setting for 36MHz PLL output with 12MHz X'tal */
#define SYSCLK_PLLCON_32MHz_XTAL   (SYSCLK_PLLCON_FIN_XTAL | SYSCLK_PLLCON_NR(6) | SYSCLK_PLLCON_NF( 64) | SYSCLK_PLLCON_NO_4) /*!< Predefined PLLCON setting for 32MHz PLL output with 12MHz X'tal */
#define SYSCLK_PLLCON_24MHz_XTAL   (SYSCLK_PLLCON_FIN_XTAL | SYSCLK_PLLCON_NR(2) | SYSCLK_PLLCON_NF( 16) | SYSCLK_PLLCON_NO_4) /*!< Predefined PLLCON setting for 24MHz PLL output with 12MHz X'tal */
#else
# error "The PLL pre-definitions are only valid when external crystal is 12MHz"
#endif

#define SYSCLK_PLLCON_50MHz_IRC22M (SYSCLK_PLLCON_FIN_IRC22M | SYSCLK_PLLCON_NR(13) | SYSCLK_PLLCON_NF( 59) | SYSCLK_PLLCON_NO_2) /*!< Predefined PLLCON setting for 50.1918MHz PLL output with 22.1184MHz IRC */
#define SYSCLK_PLLCON_48MHz_IRC22M (SYSCLK_PLLCON_FIN_IRC22M | SYSCLK_PLLCON_NR(13) | SYSCLK_PLLCON_NF(113) | SYSCLK_PLLCON_NO_4) /*!< Predefined PLLCON setting for 48.064985MHz PLL output with 22.1184MHz IRC*/
#define SYSCLK_PLLCON_36MHz_IRC22M (SYSCLK_PLLCON_FIN_IRC22M | SYSCLK_PLLCON_NR(12) | SYSCLK_PLLCON_NF( 78) | SYSCLK_PLLCON_NO_4) /*!< Predefined PLLCON setting for 35.9424MHz PLL output with 22.1184MHz IRC */
#define SYSCLK_PLLCON_32MHz_IRC22M (SYSCLK_PLLCON_FIN_IRC22M | SYSCLK_PLLCON_NR( 9) | SYSCLK_PLLCON_NF( 52) | SYSCLK_PLLCON_NO_4) /*!< Predefined PLLCON setting for 31.9488MHz PLL output with 22.1184MHz IRC*/
#define SYSCLK_PLLCON_24MHz_IRC22M (SYSCLK_PLLCON_FIN_IRC22M | SYSCLK_PLLCON_NR( 3) | SYSCLK_PLLCON_NF( 13) | SYSCLK_PLLCON_NO_4) /*!< Predefined PLLCON setting for 23.9616MHz PLL output with 22.1184MHz IRC*/


/*---------------------------------------------------------------------------------------------------------*/
/*  CLKSEL0 constant definitions.                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
#define SYSCLK_CLKSEL0_HCLK_XTAL          0x00UL /*!< Setting clock source as external X'tal */ 
#define SYSCLK_CLKSEL0_HCLK_PLL           0x02UL /*!< Setting clock source as PLL output */
#define SYSCLK_CLKSEL0_HCLK_IRC10K        0x03UL /*!< Setting clock source as internal 10KHz RC clock */
#define SYSCLK_CLKSEL0_HCLK_IRC22M        0x07UL /*!< Setting clock source as internal 22.1184MHz RC clock */
#define SYSCLK_CLKSEL0_STCLK_XTAL         0x00UL /*!< Setting clock source as external X'tal */ 
#define SYSCLK_CLKSEL0_STCLK_XTAL_DIV2    0x10UL /*!< Setting clock source as external X'tal/2 */
#define SYSCLK_CLKSEL0_STCLK_HCLK_DIV2    0x18UL /*!< Setting clock source as HCLK/2 */
#define SYSCLK_CLKSEL0_STCLK_IRC22M_DIV2  0x38UL /*!< Setting clock source as internal 22.1184MHz RC clock/2 */


/*---------------------------------------------------------------------------------------------------------*/
/*  CLKSEL1 constant definitions.                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
#define SYSCLK_CLKSEL1_WDT_XTAL          0x00000000UL /*!< Setting WDT clock source as external X'tal */ 
#define SYSCLK_CLKSEL1_WDT_HCLK_DIV2048  0x00000002UL /*!< Setting WDT clock source as HCLK/2048 */
#define SYSCLK_CLKSEL1_WDT_IRC10K        0x00000003UL /*!< Setting WDT clock source as internal 10KHz RC clock */
#define SYSCLK_CLKSEL1_ADC_XTAL          0x00000000UL /*!< Setting ADC clock source as external X'tal */
#define SYSCLK_CLKSEL1_ADC_PLL           0x00000004UL /*!< Setting ADC clock source as PLL */ 
#define SYSCLK_CLKSEL1_ADC_HCLK          0x00000008UL /*!< Setting ADC clock source as HCLK */
#define SYSCLK_CLKSEL1_ADC_IRC22M        0x0000000CUL /*!< Setting ADC clock source as internal 22.1184MHz RC clock */
#define SYSCLK_CLKSEL1_TMR0_XTAL         0x00000000UL /*!< Setting Timer 0 clock source as external X'tal */
#define SYSCLK_CLKSEL1_TMR0_HCLK         0x00000200UL /*!< Setting Timer 0 clock source as HCLK */
#define SYSCLK_CLKSEL1_TMR0_IRC22M       0x00000700UL /*!< Setting Timer 0 clock source as internal 22.1184MHz RC clock */
#define SYSCLK_CLKSEL1_TMR1_XTAL         0x00000000UL /*!< Setting Timer 1 clock source as external X'tal */
#define SYSCLK_CLKSEL1_TMR1_HCLK         0x00002000UL /*!< Setting Timer 1 clock source as HCLK */
#define SYSCLK_CLKSEL1_TMR1_IRC22M       0x00007000UL /*!< Setting Timer 1 clock source as internal 22.1184MHz RC clock */
#define SYSCLK_CLKSEL1_TMR2_XTAL         0x00000000UL /*!< Setting Timer 2 clock source as external X'tal */
#define SYSCLK_CLKSEL1_TMR2_HCLK         0x00020000UL /*!< Setting Timer 2 clock source as HCLK */
#define SYSCLK_CLKSEL1_TMR2_IRC22M       0x00070000UL /*!< Setting Timer 2 clock source as internal 22.1184MHz RC clock */
#define SYSCLK_CLKSEL1_TMR3_XTAL         0x00000000UL /*!< Setting Timer 3 clock source as external X'tal */
#define SYSCLK_CLKSEL1_TMR3_HCLK         0x00200000UL /*!< Setting Timer 3 clock source as HCLK */
#define SYSCLK_CLKSEL1_TMR3_IRC22M       0x00700000UL /*!< Setting Timer 3 clock source as internal 22.1184MHz RC clock */
#define SYSCLK_CLKSEL1_UART_XTAL         0x00000000UL /*!< Setting UART clock source as external X'tal */
#define SYSCLK_CLKSEL1_UART_PLL          0x01000000UL /*!< Setting UART clock source as external PLL */
#define SYSCLK_CLKSEL1_UART_IRC22M       0x03000000UL /*!< Setting UART clock source as external internal 22.1184MHz RC clock */
#define SYSCLK_CLKSEL1_PWM01_XTAL        0x00000000UL /*!< Setting PWM01 clock source as external X'tal */
#define SYSCLK_CLKSEL1_PWM01_HCLK        0x20000000UL /*!< Setting PWM01 clock source as external HCLK */
#define SYSCLK_CLKSEL1_PWM01_IRC22M      0x30000000UL /*!< Setting PWM01 clock source as external internal 22.1184MHz RC clock */
#define SYSCLK_CLKSEL1_PWM23_XTAL        0x00000000UL /*!< Setting PWM23 clock source as external X'tal */
#define SYSCLK_CLKSEL1_PWM23_HCLK        0x80000000UL /*!< Setting PWM23 clock source as external HCLK */
#define SYSCLK_CLKSEL1_PWM23_IRC22M      0xC0000000UL /*!< Setting PWM23 clock source as external internal 22.1184MHz RC clock */


/*---------------------------------------------------------------------------------------------------------*/
/*  CLKSEL2 constant definitions.                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
#define SYSCLK_CLKSEL2_FRQDIV_XTAL        0x00000000UL /*!< Setting FRQDIV clock source as external X'tal */ 
#define SYSCLK_CLKSEL2_FRQDIV_HCLK        0x00000008UL /*!< Setting FRQDIV clock source as HCLK */
#define SYSCLK_CLKSEL2_FRQDIV_IRC22M      0x0000000CUL /*!< Setting FRQDIV clock source as internal 22.1184MHz RC clock */
#define SYSCLK_CLKSEL2_PWM45_XTAL         0x00000000UL /*!< Setting PWM45 clock source as external X'tal */ 
#define SYSCLK_CLKSEL2_PWM45_HCLK         0x00000020UL /*!< Setting PWM45 clock source as HCLK */
#define SYSCLK_CLKSEL2_PWM45_IRC22M       0x00000030UL /*!< Setting PWM45 clock source as internal 22.1184MHz RC clock */
#define SYSCLK_CLKSEL2_PWM67_XTAL         0x00000000UL /*!< Setting PWM67 clock source as external X'tal */ 
#define SYSCLK_CLKSEL2_PWM67_HCLK         0x00000080UL /*!< Setting PWM67 clock source as HCLK */
#define SYSCLK_CLKSEL2_PWM67_IRC22M       0x000000C0UL /*!< Setting PWM67 clock source as internal 22.1184MHz RC clock */

       
/*---------------------------------------------------------------------------------------------------------*/
/*  CLKDIV constant definitions.                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
#define SYSCLK_CLKDIV_ADC(x)  (((x)-1) << 16) /*!< CLKDIV Setting for ADC clock divider. It could be 1~256 */ 
#define SYSCLK_CLKDIV_UART(x) (((x)-1) <<  8) /*!< CLKDIV Setting for UART clock divider. It could be 1~16 */ 
#define SYSCLK_CLKDIV_HCLK(x)  ((x)-1)        /*!< CLKDIV Setting for HCLK clock divider. It could be 1~16 */ 


/*---------------------------------------------------------------------------------------------------------*/
/*  Multi-Function constant definitions.                                                                   */
/*---------------------------------------------------------------------------------------------------------*/
#define SYS_MFP_TYPE_Msk(bit)       (1UL << ((bit) +16))
#define SYS_MFP_ALT_Msk(bit)        (1UL << ((bit) + 8))
#define SYS_MFP_MFP_Msk(bit)        (1UL << ((bit)    ))

#define SYS_MFP_P00_GPIO    0x00000000UL /*!< P0_MFP pin 0 setting for GPIO */      
#define SYS_MFP_P00_AD0     0x00000001UL /*!< P0_MFP pin 0 setting for AD0  */      
#define SYS_MFP_P00_CTS1    0x00000100UL /*!< P0_MFP pin 0 setting for CTS1 */      
#define SYS_MFP_P00_TXD1    0x00000101UL /*!< P0_MFP pin 0 setting for TXD1 */      
#define SYS_MFP_P00_Msk     0x01000101UL /*!< P0_MFP pin 0 mask             */      
        
#define SYS_MFP_P01_GPIO    0x00000000UL /*!< P0_MFP pin 1 setting for GPIO */      
#define SYS_MFP_P01_AD1     0x00000002UL /*!< P0_MFP pin 1 setting for AD1  */      
#define SYS_MFP_P01_RTS1    0x00000200UL /*!< P0_MFP pin 1 setting for RTS1 */      
#define SYS_MFP_P01_RXD1    0x00000202UL /*!< P0_MFP pin 1 setting for RXD1 */      
#define SYS_MFP_P01_Msk     0x02000202UL /*!< P0_MFP pin 1 mask             */      
        
#define SYS_MFP_P02_GPIO    0x00000000UL /*!< P0_MFP pin 2 setting for GPIO */
#define SYS_MFP_P02_AD2     0x00000004UL /*!< P0_MFP pin 2 setting for AD2  */
#define SYS_MFP_P02_CTS0    0x00000400UL /*!< P0_MFP pin 2 setting for CTS0 */
#define SYS_MFP_P02_TXD0    0x00000404UL /*!< P0_MFP pin 2 setting for TXD0 */
#define SYS_MFP_P02_Msk     0x00000404UL /*!< P0_MFP pin 2 mask             */      
        
#define SYS_MFP_P03_GPIO    0x00000000UL /*!< P0_MFP pin 3 setting for GPIO */      
#define SYS_MFP_P03_AD3     0x00000008UL /*!< P0_MFP pin 3 setting for AD3  */      
#define SYS_MFP_P03_RTS0    0x00000800UL /*!< P0_MFP pin 3 setting for RTS0 */      
#define SYS_MFP_P03_RXD0    0x00000808UL /*!< P0_MFP pin 3 setting for RXD0 */      
#define SYS_MFP_P03_Msk     0x00000808UL /*!< P0_MFP pin 3 mask             */      
        
#define SYS_MFP_P04_GPIO    0x00000000UL /*!< P0_MFP pin 4 setting for GPIO   */      
#define SYS_MFP_P04_AD4     0x00000010UL /*!< P0_MFP pin 4 setting for AD4    */      
#define SYS_MFP_P04_SPISS1  0x00001000UL /*!< P0_MFP pin 4 setting for SPISS1 */      
#define SYS_MFP_P04_Msk     0x00001010UL /*!< P0_MFP pin 4 mask               */      
        
#define SYS_MFP_P05_GPIO    0x00000000UL /*!< P0_MFP pin 5 setting for GPIO   */      
#define SYS_MFP_P05_AD5     0x00000020UL /*!< P0_MFP pin 5 setting for AD5    */      
#define SYS_MFP_P05_MOSI_1  0x00002000UL /*!< P0_MFP pin 5 setting for MOSI_1 */      
#define SYS_MFP_P05_Msk     0x00002020UL /*!< P0_MFP pin 5 mask               */      
        
#define SYS_MFP_P06_GPIO    0x00000000UL /*!< P0_MFP pin 6 setting for GPIO   */      
#define SYS_MFP_P06_AD6     0x00000040UL /*!< P0_MFP pin 6 setting for AD6    */      
#define SYS_MFP_P06_MISO_1  0x00004000UL /*!< P0_MFP pin 6 setting for MISO_1 */      
#define SYS_MFP_P06_Msk     0x00004040UL /*!< P0_MFP pin 6 mask               */      
        
#define SYS_MFP_P07_GPIO    0x00000000UL /*!< P0_MFP pin 7 setting for GPIO    */      
#define SYS_MFP_P07_AD7     0x00000080UL /*!< P0_MFP pin 7 setting for AD7     */      
#define SYS_MFP_P07_SPICLK1 0x00008000UL /*!< P0_MFP pin 7 setting for SPICLK1 */      
#define SYS_MFP_P07_Msk     0x00008080UL /*!< P0_MFP pin 7 mask                */      
        
#define SYS_MFP_P10_GPIO    0x00000000UL /*!< P1_MFP pin 0 setting for GPIO */      
#define SYS_MFP_P10_AIN0    0x00000001UL /*!< P1_MFP pin 0 setting for AIN0 */      
#define SYS_MFP_P10_T2      0x00000100UL /*!< P1_MFP pin 0 setting for T2   */      
#define SYS_MFP_P10_nWRL    0x00000101UL /*!< P1_MFP pin 0 setting for nWRL */      
#define SYS_MFP_P10_Msk     0x00000101UL /*!< P1_MFP pin 0 mask             */      
        
#define SYS_MFP_P11_GPIO    0x00000000UL /*!< P1_MFP pin 1 setting for GPIO */  
#define SYS_MFP_P11_AIN1    0x00000002UL /*!< P1_MFP pin 1 setting for AIN1 */      
#define SYS_MFP_P11_T3      0x00000200UL /*!< P1_MFP pin 1 setting for T3   */      
#define SYS_MFP_P11_nWRH    0x00000202UL /*!< P1_MFP pin 1 setting for nWRH */      
#define SYS_MFP_P11_Msk     0x00000202UL /*!< P1_MFP pin 1 mask             */      
        
#define SYS_MFP_P12_GPIO    0x00000000UL /*!< P1_MFP pin 2 setting for GPIO */      
#define SYS_MFP_P12_AIN2    0x00000004UL /*!< P1_MFP pin 2 setting for AIN2 */      
#define SYS_MFP_P12_RXD1    0x00000400UL /*!< P1_MFP pin 2 setting for RXD1 */      
#define SYS_MFP_P12_Msk     0x00000404UL /*!< P1_MFP pin 2 mask             */      
        
#define SYS_MFP_P13_GPIO    0x00000000UL /*!< P1_MFP pin 3 setting for GPIO */      
#define SYS_MFP_P13_AIN3    0x00000008UL /*!< P1_MFP pin 3 setting for AIN3 */      
#define SYS_MFP_P13_TXD1    0x00000800UL /*!< P1_MFP pin 3 setting for TXD1 */      
#define SYS_MFP_P13_Msk     0x00000808UL /*!< P1_MFP pin 3 mask             */      
        
#define SYS_MFP_P14_GPIO    0x00000000UL /*!< P1_MFP pin 4 setting for GPIO   */      
#define SYS_MFP_P14_AIN4    0x00000010UL /*!< P1_MFP pin 4 setting for AIN4   */      
#define SYS_MFP_P14_SPISS0  0x00001000UL /*!< P1_MFP pin 4 setting for SPISS0 */      
#define SYS_MFP_P14_CPN0    0x00001010UL /*!< P1_MFP pin 4 setting for CPN0   */      
#define SYS_MFP_P14_Msk     0x00001010UL /*!< P1_MFP pin 4 mask               */      
        
#define SYS_MFP_P15_GPIO    0x00000000UL /*!< P1_MFP pin 5 setting for GPIO   */      
#define SYS_MFP_P15_AIN5    0x00000020UL /*!< P1_MFP pin 5 setting for AIN5   */      
#define SYS_MFP_P15_MOSI_0  0x00002000UL /*!< P1_MFP pin 5 setting for MOSI_0 */      
#define SYS_MFP_P15_CPP0    0x00002020UL /*!< P1_MFP pin 5 setting for CPP0   */      
#define SYS_MFP_P15_Msk     0x00002020UL /*!< P1_MFP pin 5 mask               */      
        
#define SYS_MFP_P16_GPIO    0x00000000UL /*!< P1_MFP pin 6 setting for GPIO   */
#define SYS_MFP_P16_AIN6    0x00000040UL /*!< P1_MFP pin 6 setting for AIN6   */      
#define SYS_MFP_P16_MISO_0  0x00004000UL /*!< P1_MFP pin 6 setting for MISO_0 */      
#define SYS_MFP_P16_Msk     0x00004040UL /*!< P1_MFP pin 6 mask               */      
        
#define SYS_MFP_P17_GPIO    0x00000000UL /*!< P1_MFP pin 7 setting for GPIO    */      
#define SYS_MFP_P17_AIN7    0x00000080UL /*!< P1_MFP pin 7 setting for AIN7    */      
#define SYS_MFP_P17_SPICLK0 0x00008000UL /*!< P1_MFP pin 7 setting for SPICLK0 */      
#define SYS_MFP_P17_Msk     0x00008080UL /*!< P1_MFP pin 7 mask                */      
        
#define SYS_MFP_P20_GPIO    0x00000000UL /*!< P2_MFP pin 0 setting for GPIO */      
#define SYS_MFP_P20_AD8     0x00000001UL /*!< P2_MFP pin 0 setting for AD8  */      
#define SYS_MFP_P20_PWM0    0x00000100UL /*!< P2_MFP pin 0 setting for PWM0 */      
#define SYS_MFP_P20_Msk     0x00000101UL /*!< P2_MFP pin 0 mask             */      
        
#define SYS_MFP_P21_GPIO    0x00000000UL /*!< P2_MFP pin 1 setting for GPIO */      
#define SYS_MFP_P21_AD9     0x00000002UL /*!< P2_MFP pin 1 setting for AD9  */      
#define SYS_MFP_P21_PWM1    0x00000200UL /*!< P2_MFP pin 1 setting for PWM1 */      
#define SYS_MFP_P21_Msk     0x00000202UL /*!< P2_MFP pin 1 mask             */      
        
#define SYS_MFP_P22_GPIO    0x00000000UL /*!< P2_MFP pin 2 setting for GPIO */      
#define SYS_MFP_P22_AD10    0x00000004UL /*!< P2_MFP pin 2 setting for AD10 */      
#define SYS_MFP_P22_PWM2    0x00000400UL /*!< P2_MFP pin 2 setting for PWM2 */      
#define SYS_MFP_P22_Msk     0x00000404UL /*!< P2_MFP pin 2 mask             */      
        
#define SYS_MFP_P23_GPIO    0x00000000UL /*!< P2_MFP pin 3 setting for GPIO */      
#define SYS_MFP_P23_AD11    0x00000008UL /*!< P2_MFP pin 3 setting for AD11 */      
#define SYS_MFP_P23_PWM3    0x00000800UL /*!< P2_MFP pin 3 setting for PWM3 */      
#define SYS_MFP_P23_Msk     0x00000808UL /*!< P2_MFP pin 3 mask             */      
        
#define SYS_MFP_P24_GPIO    0x00000000UL /*!< P2_MFP pin 4 setting for GPIO */      
#define SYS_MFP_P24_AD12    0x00000010UL /*!< P2_MFP pin 4 setting for AD12 */      
#define SYS_MFP_P24_PWM4    0x00001000UL /*!< P2_MFP pin 4 setting for PWM4 */      
#define SYS_MFP_P24_Msk     0x00001010UL /*!< P2_MFP pin 4 mask             */      
        
#define SYS_MFP_P25_GPIO    0x00000000UL /*!< P2_MFP pin 5 setting for GPIO */      
#define SYS_MFP_P25_AD13    0x00000020UL /*!< P2_MFP pin 5 setting for AD13 */      
#define SYS_MFP_P25_PWM5    0x00002000UL /*!< P2_MFP pin 5 setting for PWM5 */      
#define SYS_MFP_P25_Msk     0x00002020UL /*!< P2_MFP pin 5 mask             */      
        
#define SYS_MFP_P26_GPIO    0x00000000UL /*!< P2_MFP pin 6 setting for GPIO */      
#define SYS_MFP_P26_AD14    0x00000040UL /*!< P2_MFP pin 6 setting for AD14 */      
#define SYS_MFP_P26_PWM6    0x00004000UL /*!< P2_MFP pin 6 setting for PWM6 */      
#define SYS_MFP_P26_CPO1    0x00004040UL /*!< P2_MFP pin 6 setting for CPO1 */      
#define SYS_MFP_P26_Msk     0x00004040UL /*!< P2_MFP pin 6 mask             */      
        
#define SYS_MFP_P27_GPIO    0x00000000UL /*!< P2_MFP pin 7 setting for GPIO */      
#define SYS_MFP_P27_AD15    0x00000080UL /*!< P2_MFP pin 7 setting for AD15 */      
#define SYS_MFP_P27_PWM7    0x00008000UL /*!< P2_MFP pin 7 setting for PWM7 */      
#define SYS_MFP_P27_Msk     0x00008080UL /*!< P2_MFP pin 7 mask             */      

#define SYS_MFP_P30_GPIO    0x00000000UL /*!< P3_MFP pin 0 setting for GPIO */      
#define SYS_MFP_P30_RXD0    0x00000001UL /*!< P3_MFP pin 0 setting for RXD0 */      
#define SYS_MFP_P30_CPN1    0x00000100UL /*!< P3_MFP pin 0 setting for CPN1 */      
#define SYS_MFP_P30_Msk     0x00000101UL /*!< P3_MFP pin 0 mask             */      

#define SYS_MFP_P31_GPIO    0x00000000UL /*!< P3_MFP pin 1 setting for GPIO */      
#define SYS_MFP_P31_TXD0    0x00000002UL /*!< P3_MFP pin 1 setting for TXD0 */      
#define SYS_MFP_P31_CPP1    0x00000200UL /*!< P3_MFP pin 1 setting for CPP1 */      
#define SYS_MFP_P31_Msk     0x00000202UL /*!< P3_MFP pin 1 mask             */      

#define SYS_MFP_P32_GPIO    0x00000000UL /*!< P3_MFP pin 2 setting for GPIO  */
#define SYS_MFP_P32_INT0    0x00000004UL /*!< P3_MFP pin 2 setting for /INT0 */            
#define SYS_MFP_P32_T0EX    0x00000400UL /*!< P3_MFP pin 2 setting for T0EX  */      
#define SYS_MFP_P32_Msk     0x00000404UL /*!< P3_MFP pin 2 mask              */      

#define SYS_MFP_P33_GPIO    0x00000000UL /*!< P3_MFP pin 3 setting for GPIO  */
#define SYS_MFP_P33_INT1    0x00000008UL /*!< P3_MFP pin 3 setting for /INT1 */
#define SYS_MFP_P33_MCLK    0x00000800UL /*!< P3_MFP pin 3 setting for MCLK  */
#define SYS_MFP_P33_T1EX    0x00000808UL /*!< P3_MFP pin 3 setting for T1EX  */
#define SYS_MFP_P33_Msk     0x00000808UL /*!< P3_MFP pin 3 mask              */

#define SYS_MFP_P34_GPIO    0x00000000UL /*!< P3_MFP pin 4 setting for GPIO */
#define SYS_MFP_P34_T0      0x00000010UL /*!< P3_MFP pin 4 setting for T0   */
#define SYS_MFP_P34_SDA0    0x00001000UL /*!< P3_MFP pin 4 setting for SDA0 */
#define SYS_MFP_P34_Msk     0x00001010UL /*!< P3_MFP pin 4 mask             */

#define SYS_MFP_P35_GPIO    0x00000000UL /*!< P3_MFP pin 5 setting for GPIO */
#define SYS_MFP_P35_T1      0x00000020UL /*!< P3_MFP pin 5 setting for T1   */
#define SYS_MFP_P35_SCL0    0x00002000UL /*!< P3_MFP pin 5 setting for SCL0 */
#define SYS_MFP_P35_Msk     0x00002020UL /*!< P3_MFP pin 5 mask             */

#define SYS_MFP_P36_GPIO    0x00000000UL /*!< P3_MFP pin 6 setting for GPIO */
#define SYS_MFP_P36_nWR     0x00000040UL /*!< P3_MFP pin 6 setting for nWR  */
#define SYS_MFP_P36_CKO     0x00004000UL /*!< P3_MFP pin 6 setting for CKO  */
#define SYS_MFP_P36_CPO0    0x00004040UL /*!< P3_MFP pin 6 setting for CPO0 */
#define SYS_MFP_P36_Msk     0x00004040UL /*!< P3_MFP pin 6 mask             */

#define SYS_MFP_P37_GPIO    0x00000000UL /*!< P3_MFP pin 7 setting for GPIO */
#define SYS_MFP_P37_nRD     0x00000080UL /*!< P3_MFP pin 7 setting for nWR  */
#define SYS_MFP_P37_Msk     0x00008080UL /*!< P3_MFP pin 7 mask             */

#define SYS_MFP_P40_GPIO    0x00000000UL /*!< P4_MFP pin 0 setting for GPIO */
#define SYS_MFP_P40_PWM0    0x00000001UL /*!< P4_MFP pin 0 setting for PWM0 */
#define SYS_MFP_P40_T2EX    0x00000100UL /*!< P4_MFP pin 0 setting for T2EX */
#define SYS_MFP_P40_Msk     0x00000101UL /*!< P4_MFP pin 0 mask             */

#define SYS_MFP_P41_GPIO    0x00000000UL /*!< P4_MFP pin 1 setting for GPIO */
#define SYS_MFP_P41_PWM1    0x00000002UL /*!< P4_MFP pin 1 setting for PWM1 */
#define SYS_MFP_P41_T3EX    0x00000200UL /*!< P4_MFP pin 1 setting for T3EX */
#define SYS_MFP_P41_Msk     0x00000202UL /*!< P4_MFP pin 1 mask             */

#define SYS_MFP_P42_GPIO    0x00000000UL /*!< P4_MFP pin 2 setting for GPIO */
#define SYS_MFP_P42_PWM2    0x00000004UL /*!< P4_MFP pin 2 setting for PWM2 */
#define SYS_MFP_P42_Msk     0x00000404UL /*!< P4_MFP pin 2 mask             */

#define SYS_MFP_P43_GPIO    0x00000000UL /*!< P4_MFP pin 3 setting for GPIO */
#define SYS_MFP_P43_PWM3    0x00000008UL /*!< P4_MFP pin 3 setting for PWM3 */
#define SYS_MFP_P43_Msk     0x00000808UL /*!< P4_MFP pin 3 mask             */

#define SYS_MFP_P44_GPIO    0x00000000UL /*!< P4_MFP pin 4 setting for GPIO */
#define SYS_MFP_P44_nCS     0x00000010UL /*!< P4_MFP pin 4 setting for nCS  */
#define SYS_MFP_P44_Msk     0x00001010UL /*!< P4_MFP pin 4 mask             */

#define SYS_MFP_P45_GPIO    0x00000000UL /*!< P4_MFP pin 5 setting for GPIO */
#define SYS_MFP_P45_ALE     0x00000020UL /*!< P4_MFP pin 5 setting for ALE  */
#define SYS_MFP_P45_Msk     0x00002020UL /*!< P4_MFP pin 5 mask             */

#define SYS_MFP_P46_GPIO    0x00000000UL /*!< P4_MFP pin 6 setting for GPIO    */
#define SYS_MFP_P46_ICE_CLK 0x00000040UL /*!< P4_MFP pin 6 setting for ICE_CLK */
#define SYS_MFP_P46_Msk     0x00004040UL /*!< P4_MFP pin 6 mask                */
        
#define SYS_MFP_P47_GPIO    0x00000000UL /*!< P4_MFP pin 7 setting for GPIO    */
#define SYS_MFP_P47_ICE_DAT 0x00000080UL /*!< P4_MFP pin 7 setting for ICE_DAT */
#define SYS_MFP_P47_Msk     0x00008080UL /*!< P4_MFP pin 7 mask                */


/*---------------------------------------------------------------------------------------------------------*/
/*  Brown Out Detector Threshold Voltage Selection constant definitions.                                   */
/*---------------------------------------------------------------------------------------------------------*/
#define SYS_BODCR_BOD_VL_4_5V     0x06UL       /*!< Setting Brown Out Detector Threshold Voltage as 4.5V */
#define SYS_BODCR_BOD_VL_3_8V     0x04UL       /*!< Setting Brown Out Detector Threshold Voltage as 3.8V */ 
#define SYS_BODCR_BOD_VL_2_7V     0x02UL       /*!< Setting Brown Out Detector Threshold Voltage as 2.7V */
#define SYS_BODCR_BOD_VL_2_2V     0x00UL       /*!< Setting Brown Out Detector Threshold Voltage as 2.2V */


/*---------------------------------------------------------------------------------------------------------*/
/*  Define Macro                                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
#define _SYS_RESET_CPU()   (SYS->IPRSTC1 |= SYS_IPRSTC1_CPU_RST_Msk) /*!< Reset CPU by setting CPU_RST bit from IPRSTC1 Register         */
#define _SYS_RESET_CHIP()  (SYS->IPRSTC1 = SYS_IPRSTC1_CHIP_RST_Msk) /*!< Reset Whole Chip by setting CHIP_RST bit from IPRSTC1 Register */
#define _SYS_P00_MFP(x)    (SYS->P0_MFP = (SYS->P0_MFP & (~SYS_MFP_P00_Msk)) | (x))
#define _SYS_P01_MFP(x)    (SYS->P0_MFP = (SYS->P0_MFP & (~SYS_MFP_P01_Msk)) | (x))
#define _SYS_P02_MFP(x)    (SYS->P0_MFP = (SYS->P0_MFP & (~SYS_MFP_P02_Msk)) | (x))
#define _SYS_P03_MFP(x)    (SYS->P0_MFP = (SYS->P0_MFP & (~SYS_MFP_P03_Msk)) | (x))
#define _SYS_P04_MFP(x)    (SYS->P0_MFP = (SYS->P0_MFP & (~SYS_MFP_P04_Msk)) | (x))
#define _SYS_P05_MFP(x)    (SYS->P0_MFP = (SYS->P0_MFP & (~SYS_MFP_P05_Msk)) | (x))
#define _SYS_P06_MFP(x)    (SYS->P0_MFP = (SYS->P0_MFP & (~SYS_MFP_P06_Msk)) | (x))
#define _SYS_P07_MFP(x)    (SYS->P0_MFP = (SYS->P0_MFP & (~SYS_MFP_P07_Msk)) | (x))

#define _SYS_P10_MFP(x)    (SYS->P1_MFP = (SYS->P1_MFP & (~SYS_MFP_P10_Msk)) | (x))
#define _SYS_P11_MFP(x)    (SYS->P1_MFP = (SYS->P1_MFP & (~SYS_MFP_P11_Msk)) | (x))
#define _SYS_P12_MFP(x)    (SYS->P1_MFP = (SYS->P1_MFP & (~SYS_MFP_P12_Msk)) | (x))
#define _SYS_P13_MFP(x)    (SYS->P1_MFP = (SYS->P1_MFP & (~SYS_MFP_P13_Msk)) | (x))
#define _SYS_P14_MFP(x)    (SYS->P1_MFP = (SYS->P1_MFP & (~SYS_MFP_P14_Msk)) | (x))
#define _SYS_P15_MFP(x)    (SYS->P1_MFP = (SYS->P1_MFP & (~SYS_MFP_P15_Msk)) | (x))
#define _SYS_P16_MFP(x)    (SYS->P1_MFP = (SYS->P1_MFP & (~SYS_MFP_P16_Msk)) | (x))
#define _SYS_P17_MFP(x)    (SYS->P1_MFP = (SYS->P1_MFP & (~SYS_MFP_P17_Msk)) | (x))

#define _SYS_P20_MFP(x)    (SYS->P2_MFP = (SYS->P2_MFP & (~SYS_MFP_P20_Msk)) | (x))
#define _SYS_P21_MFP(x)    (SYS->P2_MFP = (SYS->P2_MFP & (~SYS_MFP_P21_Msk)) | (x))
#define _SYS_P22_MFP(x)    (SYS->P2_MFP = (SYS->P2_MFP & (~SYS_MFP_P22_Msk)) | (x))
#define _SYS_P23_MFP(x)    (SYS->P2_MFP = (SYS->P2_MFP & (~SYS_MFP_P23_Msk)) | (x))
#define _SYS_P24_MFP(x)    (SYS->P2_MFP = (SYS->P2_MFP & (~SYS_MFP_P24_Msk)) | (x))
#define _SYS_P25_MFP(x)    (SYS->P2_MFP = (SYS->P2_MFP & (~SYS_MFP_P25_Msk)) | (x))
#define _SYS_P26_MFP(x)    (SYS->P2_MFP = (SYS->P2_MFP & (~SYS_MFP_P26_Msk)) | (x))
#define _SYS_P27_MFP(x)    (SYS->P2_MFP = (SYS->P2_MFP & (~SYS_MFP_P27_Msk)) | (x))

#define _SYS_P30_MFP(x)    (SYS->P3_MFP = (SYS->P3_MFP & (~SYS_MFP_P30_Msk)) | (x))
#define _SYS_P31_MFP(x)    (SYS->P3_MFP = (SYS->P3_MFP & (~SYS_MFP_P31_Msk)) | (x))
#define _SYS_P32_MFP(x)    (SYS->P3_MFP = (SYS->P3_MFP & (~SYS_MFP_P32_Msk)) | (x))
#define _SYS_P33_MFP(x)    (SYS->P3_MFP = (SYS->P3_MFP & (~SYS_MFP_P33_Msk)) | (x))
#define _SYS_P34_MFP(x)    (SYS->P3_MFP = (SYS->P3_MFP & (~SYS_MFP_P34_Msk)) | (x))
#define _SYS_P35_MFP(x)    (SYS->P3_MFP = (SYS->P3_MFP & (~SYS_MFP_P35_Msk)) | (x))
#define _SYS_P36_MFP(x)    (SYS->P3_MFP = (SYS->P3_MFP & (~SYS_MFP_P36_Msk)) | (x))
#define _SYS_P37_MFP(x)    (SYS->P3_MFP = (SYS->P3_MFP & (~SYS_MFP_P37_Msk)) | (x))

#define _SYS_P40_MFP(x)    (SYS->P4_MFP = (SYS->P4_MFP & (~SYS_MFP_P40_Msk)) | (x))
#define _SYS_P41_MFP(x)    (SYS->P4_MFP = (SYS->P4_MFP & (~SYS_MFP_P41_Msk)) | (x))
#define _SYS_P42_MFP(x)    (SYS->P4_MFP = (SYS->P4_MFP & (~SYS_MFP_P42_Msk)) | (x))
#define _SYS_P43_MFP(x)    (SYS->P4_MFP = (SYS->P4_MFP & (~SYS_MFP_P43_Msk)) | (x))
#define _SYS_P44_MFP(x)    (SYS->P4_MFP = (SYS->P4_MFP & (~SYS_MFP_P44_Msk)) | (x))
#define _SYS_P45_MFP(x)    (SYS->P4_MFP = (SYS->P4_MFP & (~SYS_MFP_P45_Msk)) | (x))
#define _SYS_P46_MFP(x)    (SYS->P4_MFP = (SYS->P4_MFP & (~SYS_MFP_P46_Msk)) | (x))
#define _SYS_P47_MFP(x)    (SYS->P4_MFP = (SYS->P4_MFP & (~SYS_MFP_P47_Msk)) | (x))


/*---------------------------------------------------------------------------------------------------------*/
/* static inline functions                                                                                 */
/*---------------------------------------------------------------------------------------------------------*/

/**
 * @brief    Get PLL Clock Output Frequency  
 *
 * @param    None
 *
 * @return   PLL clock output frequency 
 *
 * @details  To get actual PLL clock output frequency. The clock uint is in Hz.
 */
static __INLINE uint32_t DrvSYS_GetPLLClockFreq(void)
{
    uint32_t u32PllFreq;
    uint32_t u32FIN, u32NF, u32NR, u32NO;
    uint8_t au8NoTbl[4] = {1, 2, 2, 4}; /* OUT_DV :DEF: {1, 2, 2, 4} */
    uint32_t u32Reg;

    u32PllFreq = 0;
    u32Reg = SYSCLK->PLLCON;
    if((u32Reg & (SYSCLK_PLLCON_PD_Msk | SYSCLK_PLLCON_OE_Msk)) == 0)
    {    
        /* PLL is enabled and output enabled */

        u32FIN = (u32Reg & SYSCLK_PLLCON_PLL_SRC_Msk)?__IRC22M:__XTAL;
        if(u32Reg & SYSCLK_PLLCON_BP_Msk)
        {
            /* PLL is in bypass mode */
            u32PllFreq = u32FIN;
        }
        else 
        {
            /* PLL is in normal work mode */
            u32NO = au8NoTbl[((u32Reg & SYSCLK_PLLCON_OUT_DV_Msk) >> SYSCLK_PLLCON_OUT_DV_Pos)];
            u32NF  = ((u32Reg & SYSCLK_PLLCON_FB_DV_Msk) >> SYSCLK_PLLCON_FB_DV_Pos) + 2;
            u32NR  = ((u32Reg & SYSCLK_PLLCON_IN_DV_Msk) >> SYSCLK_PLLCON_IN_DV_Pos) + 2;
            /* u32FIN is shifted 2 bits to avoid overflow */
            u32PllFreq = (((u32FIN >> 2) * u32NF) / (u32NR * u32NO) << 2); 
        }
    }

    return u32PllFreq;
}


/**
 * @brief    Waiting for clock stable
 *
 * @param    Mask bits of CLKSTATUS register. Including:
 *           SYSCLK_CLKSTATUS_CLK_SW_FAIL_Msk
 *           SYSCLK_CLKSTATUS_IRC22M_STB_Msk
 *           SYSCLK_CLKSTATUS_IRC10K_STB_Msk
 *           SYSCLK_CLKSTATUS_PLL_STB_Msk
 *           SYSCLK_CLKSTATUS_XTL12M_STB_Msk
 *
 * @return   None
 *
 * @details  To wait for clock ready by specified CLKSTATUS bit or timeout (~5ms)
 */
static __INLINE void SYS_WaitingForClockReady(uint32_t u32Mask)
{
    int32_t i32TimeOutCnt;

    i32TimeOutCnt = __HSI / 200; /* About 5ms */
    
    while((SYSCLK->CLKSTATUS & u32Mask) != u32Mask)
    {
        if(i32TimeOutCnt-- <= 0)
            break;    
    }
}

/**
 * @brief      Generate the Delay Time by Systick
 *
 * @param[in]  us  Delay time. The Max value is 2^24 / CPU Clock(MHz). Ex:
 *                             50MHz => 335544us, 48MHz => 349525us, 28MHz => 699050us ...
 *
 * @return     None
 *
 * @details    Use the SysTick to generate the delay time and the UNIT is in us. 
 *             The SysTick clock source is from HCLK, i.e the same as system core clock.
 */
static __INLINE void SYS_SysTickDelay(uint32_t us)
{
    SysTick->LOAD = us * CyclesPerUs;
    SysTick->VAL  =  (0x00);
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;

    /* Waiting for down-count to zero */
    while((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0);
}


/**
 * @brief      Enable CKO clock
 *
 * @param[in]  u32ClkSrc  Select the source clock for CKO. It could be 
 *                        SYSCLK_CLKSEL2_FRQDIV_XTAL, SYSCLK_CLKSEL2_FRQDIV_HCLK or SYSCLK_CLKSEL2_FRQDIV_IRC22M.
 * @param[in]  u32ClkDiv  Set the clock divider to CKO. 0 <= u32ClkDiv <= 15
 *
 * @return     None
 *
 * @details    Ouput selected clock to CKO. The output clock frequency is divided by u32ClkDiv. 
 *             The formula is:
 *                 CKO frequency = (Clock source frequency) / 2^(u32ClkDiv + 1)
 *             This function is just used to set CKO clock.
 *             User must enable I/O for CKO clock output pin by themselves.
 */
static __INLINE void SYS_EnableCKO(uint32_t u32ClkSrc, uint32_t u32ClkDiv)
{
    /* CKO = clock source / 2^(u32ClkDiv + 1) */
    SYSCLK->FRQDIV = SYSCLK_FRQDIV_DIVIDER_EN_Msk | u32ClkDiv;

    /* Enable CKO clock source */
    SYSCLK->APBCLK |= SYSCLK_APBCLK_FDIV_EN_Msk;

    /* Select CKO clock source */
    SYSCLK->CLKSEL2 = (SYSCLK->CLKSEL2 & (~SYSCLK_CLKSEL2_FRQDIV_S_Msk)) | u32ClkSrc;
}


/**
 * @brief      Disable CKO clock
 *
 * @return     None
 *
 * @details    Disable CKO clock 
 *
 */
static __INLINE void SYS_DisableCKO(void)
{
    /* Disable CKO clock source */
    SYSCLK->APBCLK &= (~SYSCLK_APBCLK_FDIV_EN_Msk);
}

/**
 * @brief      Initialization for power down (Deep sleep)
 *
 * @details    To inital the power down control registers
 *
 * @note   
 *             User must call SYS_UnlockReg() to unlock protected registers before calling this function 
 */
static __INLINE void SYS_PowerDownInit(void)
{   
    SCB->SCR = SCB_SCR_SLEEPDEEP_Msk;
    SYSCLK->PWRCON |= (SYSCLK_PWRCON_PD_WAIT_CPU_Msk | SYSCLK_PWRCON_PWR_DOWN_EN_Msk | SYSCLK_PWRCON_PD_WU_STS_Msk);
}


/**
 * @brief      Unlock the protected registers
 *
 * @details    To unlock the protected register to allow write access
 *
 */
static __INLINE void SYS_UnlockReg(void)
{   
    SYS->REGWRPROT = 0x59;
    SYS->REGWRPROT = 0x16;
    SYS->REGWRPROT = 0x88;
}

/**
 * @brief      Lock the protected registers
 *
 * @details    To lock the protected register to forbid write access
 *
 */
static __INLINE void SYS_LockReg(void)
{   
    SYS->REGWRPROT = 0;
}
/**
  * @} End of SYS Device Function Interface
  */ 

/**
  * @} End of M051 Function Interface
  */ 


#endif

