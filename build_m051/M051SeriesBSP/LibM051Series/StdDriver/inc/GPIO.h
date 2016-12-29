/**************************************************************************//**
 * @file     GPIO.h
 * @version  V2.1
 * $Revision: 8 $
 * $Date: 12/08/10 3:12p $
 * @brief    M051 Series General Purpose I/O Driver Header File
 *
 * @note
 * Copyright (C) 2011 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#ifndef _GPIO_H_
#define _GPIO_H_

#include "M051Series.h"


/** @addtogroup M051_FUNC M051 Function Interface
  * @{
  */

/** @addtogroup GPIO_FUNC GPIO Device Function Interface
  * @{
  */



/* Define GPIO Pin Data Input/Output. It could be used to control each I/O pin by pin address mapping.
   Example 1:
   
       P00 = 1; 
   
   It is used to set P0.0 to high;
   
   Example 2:
   
       if (P00)
           P00 = 0;
   
   If P0.0 pin status is high, then set P0.0 data output to low.
 */
#define GPIO_PIN_DATA(port, pin)    (*((volatile uint32_t *)((GPIO_PIN_DATA_BASE+(0x20*(port))) + ((pin)<<2))))   
#define P00             GPIO_PIN_DATA(0, 0) /*!< Specify P00 Pin Data Input/Output */
#define P01             GPIO_PIN_DATA(0, 1) /*!< Specify P01 Pin Data Input/Output */
#define P02             GPIO_PIN_DATA(0, 2) /*!< Specify P02 Pin Data Input/Output */
#define P03             GPIO_PIN_DATA(0, 3) /*!< Specify P03 Pin Data Input/Output */
#define P04             GPIO_PIN_DATA(0, 4) /*!< Specify P04 Pin Data Input/Output */
#define P05             GPIO_PIN_DATA(0, 5) /*!< Specify P05 Pin Data Input/Output */
#define P06             GPIO_PIN_DATA(0, 6) /*!< Specify P06 Pin Data Input/Output */
#define P07             GPIO_PIN_DATA(0, 7) /*!< Specify P07 Pin Data Input/Output */
#define P10             GPIO_PIN_DATA(1, 0) /*!< Specify P10 Pin Data Input/Output */
#define P11             GPIO_PIN_DATA(1, 1) /*!< Specify P11 Pin Data Input/Output */
#define P12             GPIO_PIN_DATA(1, 2) /*!< Specify P12 Pin Data Input/Output */
#define P13             GPIO_PIN_DATA(1, 3) /*!< Specify P13 Pin Data Input/Output */
#define P14             GPIO_PIN_DATA(1, 4) /*!< Specify P14 Pin Data Input/Output */
#define P15             GPIO_PIN_DATA(1, 5) /*!< Specify P15 Pin Data Input/Output */
#define P16             GPIO_PIN_DATA(1, 6) /*!< Specify P16 Pin Data Input/Output */
#define P17             GPIO_PIN_DATA(1, 7) /*!< Specify P17 Pin Data Input/Output */
#define P20             GPIO_PIN_DATA(2, 0) /*!< Specify P20 Pin Data Input/Output */
#define P21             GPIO_PIN_DATA(2, 1) /*!< Specify P21 Pin Data Input/Output */
#define P22             GPIO_PIN_DATA(2, 2) /*!< Specify P22 Pin Data Input/Output */
#define P23             GPIO_PIN_DATA(2, 3) /*!< Specify P23 Pin Data Input/Output */
#define P24             GPIO_PIN_DATA(2, 4) /*!< Specify P24 Pin Data Input/Output */
#define P25             GPIO_PIN_DATA(2, 5) /*!< Specify P25 Pin Data Input/Output */
#define P26             GPIO_PIN_DATA(2, 6) /*!< Specify P26 Pin Data Input/Output */
#define P27             GPIO_PIN_DATA(2, 7) /*!< Specify P27 Pin Data Input/Output */
#define P30             GPIO_PIN_DATA(3, 0) /*!< Specify P30 Pin Data Input/Output */
#define P31             GPIO_PIN_DATA(3, 1) /*!< Specify P31 Pin Data Input/Output */
#define P32             GPIO_PIN_DATA(3, 2) /*!< Specify P32 Pin Data Input/Output */
#define P33             GPIO_PIN_DATA(3, 3) /*!< Specify P33 Pin Data Input/Output */
#define P34             GPIO_PIN_DATA(3, 4) /*!< Specify P34 Pin Data Input/Output */
#define P35             GPIO_PIN_DATA(3, 5) /*!< Specify P35 Pin Data Input/Output */
#define P36             GPIO_PIN_DATA(3, 6) /*!< Specify P36 Pin Data Input/Output */
#define P37             GPIO_PIN_DATA(3, 7) /*!< Specify P37 Pin Data Input/Output */
#define P40             GPIO_PIN_DATA(4, 0) /*!< Specify P40 Pin Data Input/Output */
#define P41             GPIO_PIN_DATA(4, 1) /*!< Specify P41 Pin Data Input/Output */
#define P42             GPIO_PIN_DATA(4, 2) /*!< Specify P42 Pin Data Input/Output */
#define P43             GPIO_PIN_DATA(4, 3) /*!< Specify P43 Pin Data Input/Output */
#define P44             GPIO_PIN_DATA(4, 4) /*!< Specify P44 Pin Data Input/Output */
#define P45             GPIO_PIN_DATA(4, 5) /*!< Specify P45 Pin Data Input/Output */
#define P46             GPIO_PIN_DATA(4, 6) /*!< Specify P46 Pin Data Input/Output */
#define P47             GPIO_PIN_DATA(4, 7) /*!< Specify P47 Pin Data Input/Output */


/*---------------------------------------------------------------------------------------------------------*/
/*  PMD Constant Definitions                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
#define GPIO_PMD_INPUT              0x0UL                  /*!< Input Mode */
#define GPIO_PMD_OUTPUT             0x1UL                  /*!< Output Mode */
#define GPIO_PMD_OPEN_DRAIN         0x2UL                  /*!< Open-Drain Mode */
#define GPIO_PMD_QUASI              0x3UL                  /*!< Quasi-bidirectional Mode */
#define GPIO_PMD_MODE(pin, mode)    ((mode) << ((pin)<<1)) /*!< Generate the PMD mode setting for each pin  */

#define GPIO_PMD_PMD0_INPUT         0x0000UL /*!< PMD Setting of Pin 0 for Input Mode */
#define GPIO_PMD_PMD0_OUTPUT        0x0001UL /*!< PMD Setting of Pin 0 for Output Mode */
#define GPIO_PMD_PMD0_OPEN_DRAIN    0x0002UL /*!< PMD Setting of Pin 0 for Open-Drain Mode */
#define GPIO_PMD_PMD0_QUASI         0x0003UL /*!< PMD Setting of Pin 0 for Quasi-bidirectional Mode */
#define GPIO_PMD_PMD1_INPUT         0x0000UL /*!< PMD Setting of Pin 1 for Input Mode */
#define GPIO_PMD_PMD1_OUTPUT        0x0004UL /*!< PMD Setting of Pin 1 for Output Mode */
#define GPIO_PMD_PMD1_OPEN_DRAIN    0x0008UL /*!< PMD Setting of Pin 1 for Open-Drain Mode */
#define GPIO_PMD_PMD1_QUASI         0x000CUL /*!< PMD Setting of Pin 1 for Quasi-bidirectional Mode */
#define GPIO_PMD_PMD2_INPUT         0x0000UL /*!< PMD Setting of Pin 2 for Input Mode */
#define GPIO_PMD_PMD2_OUTPUT        0x0010UL /*!< PMD Setting of Pin 2 for Output Mode */
#define GPIO_PMD_PMD2_OPEN_DRAIN    0x0020UL /*!< PMD Setting of Pin 2 for Open-Drain Mode */
#define GPIO_PMD_PMD2_QUASI         0x0030UL /*!< PMD Setting of Pin 2 for Quasi-bidirectional Mode */
#define GPIO_PMD_PMD3_INPUT         0x0000UL /*!< PMD Setting of Pin 3 for Input Mode */
#define GPIO_PMD_PMD3_OUTPUT        0x0040UL /*!< PMD Setting of Pin 3 for Output Mode */
#define GPIO_PMD_PMD3_OPEN_DRAIN    0x0080UL /*!< PMD Setting of Pin 3 for Open-Drain Mode */
#define GPIO_PMD_PMD3_QUASI         0x00C0UL /*!< PMD Setting of Pin 3 for Quasi-bidirectional Mode */
#define GPIO_PMD_PMD4_INPUT         0x0000UL /*!< PMD Setting of Pin 4 for Input Mode */
#define GPIO_PMD_PMD4_OUTPUT        0x0100UL /*!< PMD Setting of Pin 4 for Output Mode */
#define GPIO_PMD_PMD4_OPEN_DRAIN    0x0200UL /*!< PMD Setting of Pin 4 for Open-Drain Mode */
#define GPIO_PMD_PMD4_QUASI         0x0300UL /*!< PMD Setting of Pin 4 for Quasi-bidirectional Mode */
#define GPIO_PMD_PMD5_INPUT         0x0000UL /*!< PMD Setting of Pin 5 for Input Mode */
#define GPIO_PMD_PMD5_OUTPUT        0x0400UL /*!< PMD Setting of Pin 5 for Output Mode */
#define GPIO_PMD_PMD5_OPEN_DRAIN    0x0800UL /*!< PMD Setting of Pin 5 for Open-Drain Mode */
#define GPIO_PMD_PMD5_QUASI         0x0C00UL /*!< PMD Setting of Pin 5 for Quasi-bidirectional Mode */
#define GPIO_PMD_PMD6_INPUT         0x0000UL /*!< PMD Setting of Pin 6 for Input Mode */
#define GPIO_PMD_PMD6_OUTPUT        0x1000UL /*!< PMD Setting of Pin 6 for Output Mode */
#define GPIO_PMD_PMD6_OPEN_DRAIN    0x2000UL /*!< PMD Setting of Pin 6 for Open-Drain Mode */
#define GPIO_PMD_PMD6_QUASI         0x3000UL /*!< PMD Setting of Pin 6 for Quasi-bidirectional Mode */
#define GPIO_PMD_PMD7_INPUT         0x0000UL /*!< PMD Setting of Pin 7 for Input Mode */
#define GPIO_PMD_PMD7_OUTPUT        0x4000UL /*!< PMD Setting of Pin 7 for Output Mode */
#define GPIO_PMD_PMD7_OPEN_DRAIN    0x8000UL /*!< PMD Setting of Pin 7 for Open-Drain Mode */
#define GPIO_PMD_PMD7_QUASI         0xC000UL /*!< PMD Setting of Pin 7 for Quasi-bidirectional Mode */
                                            

/*---------------------------------------------------------------------------------------------------------*/
/*  IMD Constant Definitions                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
#define GPIO_IMD_EDGE               0UL               /*!< IMD Setting for Edge Trigger Mode */
#define GPIO_IMD_LEVEL              1UL               /*!< IMD Setting for Edge Level Mode */
#define GPIO_IMD_MODE(pin, mode)    ((mode) << (pin)) /*!< Generate IMD Mode Setting for Pin */
                                                        
#define GPIO_IMD_IMD0_EDGE          0x00UL /*!< IMD Setting of Pin 0 for Edge Trigger Mode */ 
#define GPIO_IMD_IMD1_EDGE          0x00UL /*!< IMD Setting of Pin 1 for Edge Trigger Mode */ 
#define GPIO_IMD_IMD2_EDGE          0x00UL /*!< IMD Setting of Pin 2 for Edge Trigger Mode */ 
#define GPIO_IMD_IMD3_EDGE          0x00UL /*!< IMD Setting of Pin 3 for Edge Trigger Mode */ 
#define GPIO_IMD_IMD4_EDGE          0x00UL /*!< IMD Setting of Pin 4 for Edge Trigger Mode */ 
#define GPIO_IMD_IMD5_EDGE          0x00UL /*!< IMD Setting of Pin 5 for Edge Trigger Mode */ 
#define GPIO_IMD_IMD6_EDGE          0x00UL /*!< IMD Setting of Pin 6 for Edge Trigger Mode */ 
#define GPIO_IMD_IMD7_EDGE          0x00UL /*!< IMD Setting of Pin 7 for Edge Trigger Mode */ 
#define GPIO_IMD_IMD0_LEVEL         0x01UL /*!< IMD Setting of Pin 0 for Level Trigger Mode */ 
#define GPIO_IMD_IMD1_LEVEL         0x02UL /*!< IMD Setting of Pin 1 for Level Trigger Mode */ 
#define GPIO_IMD_IMD2_LEVEL         0x04UL /*!< IMD Setting of Pin 2 for Level Trigger Mode */ 
#define GPIO_IMD_IMD3_LEVEL         0x08UL /*!< IMD Setting of Pin 3 for Level Trigger Mode */ 
#define GPIO_IMD_IMD4_LEVEL         0x10UL /*!< IMD Setting of Pin 4 for Level Trigger Mode */ 
#define GPIO_IMD_IMD5_LEVEL         0x20UL /*!< IMD Setting of Pin 5 for Level Trigger Mode */ 
#define GPIO_IMD_IMD6_LEVEL         0x40UL /*!< IMD Setting of Pin 6 for Level Trigger Mode */ 
#define GPIO_IMD_IMD7_LEVEL         0x80UL /*!< IMD Setting of Pin 7 for Level Trigger Mode */ 

                                    
/*---------------------------------------------------------------------------------------------------------*/
/*  IEN Constant Definitions                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
#define GPIO_IEN_IF_EN(pin)         (1UL << (pin))      /*!< Generate IEN Setting of Specified Pin for Enable Falling/Low Level Interrupt */ 
#define GPIO_IEN_IR_EN(pin)         (1UL << ((pin)+16)) /*!< Generate IEN Setting of Specified Pin for Enable Rising/High Level Interrupt */ 

#define GPIO_IEN_IF_EN0             0x00000001UL /*!< IEN Setting of Pin 0 for Eanble Falling/Low Level Interrupt */ 
#define GPIO_IEN_IF_EN1             0x00000002UL /*!< IEN Setting of Pin 1 for Eanble Falling/Low Level Interrupt */ 
#define GPIO_IEN_IF_EN2             0x00000004UL /*!< IEN Setting of Pin 2 for Eanble Falling/Low Level Interrupt */ 
#define GPIO_IEN_IF_EN3             0x00000008UL /*!< IEN Setting of Pin 3 for Eanble Falling/Low Level Interrupt */ 
#define GPIO_IEN_IF_EN4             0x00000010UL /*!< IEN Setting of Pin 4 for Eanble Falling/Low Level Interrupt */ 
#define GPIO_IEN_IF_EN5             0x00000020UL /*!< IEN Setting of Pin 5 for Eanble Falling/Low Level Interrupt */ 
#define GPIO_IEN_IF_EN6             0x00000040UL /*!< IEN Setting of Pin 6 for Eanble Falling/Low Level Interrupt */ 
#define GPIO_IEN_IF_EN7             0x00000080UL /*!< IEN Setting of Pin 7 for Eanble Falling/Low Level Interrupt */ 
                                    
#define GPIO_IEN_IR_EN0             0x00010000UL /*!< IEN Setting of Pin 0 for Eanble Rising/High Level Interrupt */ 
#define GPIO_IEN_IR_EN1             0x00020000UL /*!< IEN Setting of Pin 1 for Eanble Rising/High Level Interrupt */ 
#define GPIO_IEN_IR_EN2             0x00040000UL /*!< IEN Setting of Pin 2 for Eanble Rising/High Level Interrupt */ 
#define GPIO_IEN_IR_EN3             0x00080000UL /*!< IEN Setting of Pin 3 for Eanble Rising/High Level Interrupt */ 
#define GPIO_IEN_IR_EN4             0x00100000UL /*!< IEN Setting of Pin 4 for Eanble Rising/High Level Interrupt */ 
#define GPIO_IEN_IR_EN5             0x00200000UL /*!< IEN Setting of Pin 5 for Eanble Rising/High Level Interrupt */ 
#define GPIO_IEN_IR_EN6             0x00400000UL /*!< IEN Setting of Pin 6 for Eanble Rising/High Level Interrupt */ 
#define GPIO_IEN_IR_EN7             0x00800000UL /*!< IEN Setting of Pin 7 for Eanble Rising/High Level Interrupt */ 


/*---------------------------------------------------------------------------------------------------------*/
/*  ISRC Constant Definitions                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
#define GPIO_ISRC_ISRC0             0x01UL /*!< ISRC Setting of Pin 0 for Indicates Pin 0 generate an interrupt */ 
#define GPIO_ISRC_ISRC1             0x02UL /*!< ISRC Setting of Pin 1 for Indicates Pin 1 generate an interrupt */ 
#define GPIO_ISRC_ISRC2             0x04UL /*!< ISRC Setting of Pin 2 for Indicates Pin 2 generate an interrupt */ 
#define GPIO_ISRC_ISRC3             0x08UL /*!< ISRC Setting of Pin 3 for Indicates Pin 3 generate an interrupt */ 
#define GPIO_ISRC_ISRC4             0x10UL /*!< ISRC Setting of Pin 4 for Indicates Pin 4 generate an interrupt */
#define GPIO_ISRC_ISRC5             0x20UL /*!< ISRC Setting of Pin 5 for Indicates Pin 5 generate an interrupt */ 
#define GPIO_ISRC_ISRC6             0x40UL /*!< ISRC Setting of Pin 6 for Indicates Pin 6 generate an interrupt */ 
#define GPIO_ISRC_ISRC7             0x80UL /*!< ISRC Setting of Pin 7 for Indicates Pin 7 generate an interrupt */ 


/*---------------------------------------------------------------------------------------------------------*/
/*  DBNCECON Constant Definitions                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
#define GPIO_DBNCECON_ICLK_ON           0x00000020UL /*!< DBNCECON setting for all IO pins edge detection circuit is always active after reset */ 
#define GPIO_DBNCECON_ICLK_OFF          0x00000000UL /*!< DBNCECON setting for edge detection circuit is active only if IO pin corresponding GPIOx_IEN bit is set to 1 */ 

#define GPIO_DBNCECON_DBCLKSRC_IRC10K   0x00000010UL /*!< DBNCECON setting for de-bounce counter clock source is the internal 10 kHz */ 
#define GPIO_DBNCECON_DBCLKSRC_HCLK     0x00000000UL /*!< DBNCECON setting for de-bounce counter clock source is the internal HCLK */ 

#define GPIO_DBNCECON_DBCLKSEL_1        0x00000000UL /*!< DBNCECON setting for sampling cycle = 1 clocks */
#define GPIO_DBNCECON_DBCLKSEL_2        0x00000001UL /*!< DBNCECON setting for sampling cycle = 2 clocks */
#define GPIO_DBNCECON_DBCLKSEL_4        0x00000002UL /*!< DBNCECON setting for sampling cycle = 4 clocks */
#define GPIO_DBNCECON_DBCLKSEL_8        0x00000003UL /*!< DBNCECON setting for sampling cycle = 8 clocks */
#define GPIO_DBNCECON_DBCLKSEL_16       0x00000004UL /*!< DBNCECON setting for sampling cycle = 16 clocks */
#define GPIO_DBNCECON_DBCLKSEL_32       0x00000005UL /*!< DBNCECON setting for sampling cycle = 32 clocks */
#define GPIO_DBNCECON_DBCLKSEL_64       0x00000006UL /*!< DBNCECON setting for sampling cycle = 64 clocks */
#define GPIO_DBNCECON_DBCLKSEL_128      0x00000007UL /*!< DBNCECON setting for sampling cycle = 128 clocks */
#define GPIO_DBNCECON_DBCLKSEL_256      0x00000008UL /*!< DBNCECON setting for sampling cycle = 256 clocks */
#define GPIO_DBNCECON_DBCLKSEL_512      0x00000009UL /*!< DBNCECON setting for sampling cycle = 512 clocks */
#define GPIO_DBNCECON_DBCLKSEL_1024     0x0000000AUL /*!< DBNCECON setting for sampling cycle = 1024 clocks */
#define GPIO_DBNCECON_DBCLKSEL_2048     0x0000000BUL /*!< DBNCECON setting for sampling cycle = 2048 clocks */
#define GPIO_DBNCECON_DBCLKSEL_4096     0x0000000CUL /*!< DBNCECON setting for sampling cycle = 4096 clocks */
#define GPIO_DBNCECON_DBCLKSEL_8192     0x0000000DUL /*!< DBNCECON setting for sampling cycle = 8192 clocks */
#define GPIO_DBNCECON_DBCLKSEL_16384    0x0000000EUL /*!< DBNCECON setting for sampling cycle = 16384 clocks */
#define GPIO_DBNCECON_DBCLKSEL_32768    0x0000000FUL /*!< DBNCECON setting for sampling cycle = 32768 clocks */


/*---------------------------------------------------------------------------------------------------------*/
/*  DBEN Constant Definitions                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
#define GPIO_DBEN_ENABLE(pin)       (1UL << (pin)) /*!< DBEN setting for enable specified pin de-bounce function */      


/*---------------------------------------------------------------------------------------------------------*/
/*  DMASK Constant Definitions                                                                             */
/*---------------------------------------------------------------------------------------------------------*/
#define GPIO_DMASK_ENABLE(pin)      (1UL << (pin)) /*!< DMASK setting for mask specified pin DOUT function */      


/*---------------------------------------------------------------------------------------------------------*/
/*  OFFD Constant Definitions                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
#define GPIO_OFFD_ENABLE(pin)       (1UL << ((pin)+16)) /*!< OFFD setting for disable specified pin I/O digital input path */      


/*---------------------------------------------------------------------------------------------------------*/
/*  GPIO Interrupt Type Constant Definitions                                                               */
/*---------------------------------------------------------------------------------------------------------*/
#define GPIO_INT_RISING             0x00010000UL /*!< Interrupt enable by Input Rising Edge */
#define GPIO_INT_FALLING            0x00000001UL /*!< Interrupt enable by Input Falling Edge */
#define GPIO_INT_BOTH_EDGE          0x00010001UL /*!< Interrupt enable by both Rising Edge and Falling Edge */
#define GPIO_INT_HIGH               0x01010000UL /*!< Interrupt enable by Level-High */    
#define GPIO_INT_LOW                0x01000001UL /*!< Interrupt enable by Level-Level */


/*---------------------------------------------------------------------------------------------------------*/
/*  GPIO Macro Definitions                                                                                 */
/*---------------------------------------------------------------------------------------------------------*/
/**
 * @brief       Set GPIO Pin Mode
 *
 * @param[in]   PORT        GPIO port. It could be PORT0, PORT1, .. PORT4.
 * @param[in]   pin         The pin of specified GPIO port. It could be 0 ~ 7.
 * @param[in]   mode        The pin mode of specified GPIO pin. It could be GPIO_PMD_INPUT, GPIO_PMD_OUTPUT, GPIO_PMD_OPEN_DRAIN or GPIO_PMD_QUASI.
 *
 * @return      None
 *
 * @details     Set the pin mode of specified GPIO pin.
 */
#define _GPIO_SET_PIN_MODE(PORT, pin, mode)     ((PORT)->PMD = ((PORT)->PMD & ~GPIO_PMD_MODE(pin, GPIO_PMD_QUASI)) | GPIO_PMD_MODE(pin, mode))


/**
 * @brief       Enable I/O Digital Input Path
 *
 * @param[in]   PORT        GPIO port. It could be PORT0, PORT1, .. PORT4.
 * @param[in]   pin         The pin of specified GPIO port. It could be 0 ~ 7.
 *
 * @return      None
 *
 * @details     Enable I/O digital input path of specified GPIO pin.
 */
#define _GPIO_ENABLE_DIGITAL_PATH(PORT, pin)    ((PORT)->OFFD &= ~GPIO_OFFD_ENABLE(pin))


/**
 * @brief       Disable I/O Digital Input Path
 *
 * @param[in]   PORT        GPIO port. It could be PORT0, PORT1, .. PORT4.
 * @param[in]   pin         The pin of specified GPIO port. It could be 0 ~ 7.
 *
 * @return      None
 *
 * @details     Disable I/O digital input path of specified GPIO pin.
 */
#define _GPIO_DISABLE_DIGITAL_PATH(PORT, pin)   ((PORT)->OFFD |= GPIO_OFFD_ENABLE(pin))


/**
 * @brief       Enable Pin De-bounce Function
 *
 * @param[in]   PORT        GPIO port. It could be PORT0, PORT1, .. PORT4.
 * @param[in]   pin         The pin of specified GPIO port. It could be 0 ~ 7.
 *
 * @return      None
 *
 * @details     Enable the interrupt de-bounce function of specified GPIO pin.
 */
#define _GPIO_ENABLE_DEBOUNCE(PORT, pin)         ((PORT)->DBEN |= GPIO_DBEN_ENABLE(pin))


/**
 * @brief       Disable Pin De-bounce Function
 *
 * @param[in]   PORT        GPIO port. It could be PORT0, PORT1, .. PORT4.
 * @param[in]   pin         The pin of specified GPIO port. It could be 0 ~ 7.
 *
 * @return      None
 *
 * @details     Disable the interrupt de-bounce function of specified GPIO pin.
 */
#define _GPIO_DISABLE_DEBOUNCE(PORT, pin)       ((PORT)->DBEN &= ~GPIO_DBEN_ENABLE(pin))


/**
 * @brief       Set De-bounce Sampling Cycle Time
 *
 * @param[in]   clksrc      The de-bounce counter clock source. It could be GPIO_DBNCECON_DBCLKSRC_HCLK or GPIO_DBNCECON_DBCLKSRC_IRC10K.
 * @param[in]   clksel      The de-bounce sampling cycle selectionn. It could be \n
 *                              GPIO_DBNCECON_DBCLKSEL_1, GPIO_DBNCECON_DBCLKSEL_2, GPIO_DBNCECON_DBCLKSEL_4, GPIO_DBNCECON_DBCLKSEL_8, \n
 *                              GPIO_DBNCECON_DBCLKSEL_16, GPIO_DBNCECON_DBCLKSEL_32, GPIO_DBNCECON_DBCLKSEL_64, GPIO_DBNCECON_DBCLKSEL_128, \n
 *                              GPIO_DBNCECON_DBCLKSEL_256, GPIO_DBNCECON_DBCLKSEL_512, GPIO_DBNCECON_DBCLKSEL_1024, GPIO_DBNCECON_DBCLKSEL_2048, \n
 *                              GPIO_DBNCECON_DBCLKSEL_4096, GPIO_DBNCECON_DBCLKSEL_8192, GPIO_DBNCECON_DBCLKSEL_16384, GPIO_DBNCECON_DBCLKSEL_32768.
 *
 * @return      None
 *
 * @details     Set the interrupt de-bounce sampling cycle time based on the debounce counter clock source. \n
 *              Example: _GPIO_SET_DEBOUNCE_TIME(GPIO_DBNCECON_DBCLKSRC_IRC10K, GPIO_DBNCECON_DBCLKSEL_4). \n
 *              It's meaning the De-debounce counter clock source is internal 10 KHz and sampling cycle selection is 4. \n
 *              Then the target de-bounce sampling cycle time is (2^4)*(1/(10*1000)) s = 16*0.0001 s = 1600 us,
 *              and system will sampling interrupt input once per 1600 us.
 */
#define _GPIO_SET_DEBOUNCE_TIME(clksrc, clksel)  (GPIO->DBNCECON = (GPIO_DBNCECON_ICLK_ON_Msk | clksrc | clksel))


/**
 * @brief       Get GPIO Pin Interrupt Status
 *
 * @param[in]   PORT        GPIO port. It could be PORT0, PORT1, .. PORT4.
 * @param[in]   pin         The pin of specified GPIO port. It could be 0 ~ 7.
 *
 * @retval      0           No interrupt at specified GPIO pin
 * @retval      1           The specified GPIO pin generate an interrupt
 *
 * @details     Get the interrupt status of specified GPIO pin.
 */
#define _GPIO_GET_INT_STATUS(PORT, pin)          ((((PORT)->ISRC & (GPIO_ISRC_ISRC##pin)) == (GPIO_ISRC_ISRC##pin))? 1:0)


/**
 * @brief       Clear GPIO Pin Interrupt Status
 *
 * @param[in]   PORT        GPIO port. It could be PORT0, PORT1, .. PORT4.
 * @param[in]   pin         The pin of specified GPIO port. It could be 0 ~ 7.
 *
 * @return      None
 *
 * @details     Clear the interrupt status of specified GPIO pin.
 */
#define _GPIO_CLEAR_INT_STATUS(PORT, pin)          ((PORT)->ISRC = (GPIO_ISRC_ISRC##pin))


/*---------------------------------------------------------------------------------------------------------*/
/* static inline functions                                                                                 */
/*---------------------------------------------------------------------------------------------------------*/
/**
 * @brief       Enable GPIO interrupt
 *
 * @param[in]   PORT        GPIO port. It could be PORT0, PORT1, .. PORT4.
 * @param[in]   u32Pin      The pin of specified GPIO port. It could be 0 ~ 7.
 * @param[in]   u32Attribs  The interrupt attribute of specified GPIO pin. It could be \n
 *                          GPIO_INT_RISING, GPIO_INT_FALLING, GPIO_INT_BOTH_EDGE, GPIO_INT_HIGH, GPIO_INT_LOW.
 *
 * @return      None
 *
 * @details     This function is used to enable specified GPIO pin interrupt.
 */
static __INLINE void GPIO_EnableInt(GPIO_T *PORT, uint32_t u32Pin, uint32_t u32IntAttribs)
{
    PORT->IMD |= (_GET_BYTE3(u32IntAttribs) << u32Pin);
    PORT->IEN |= ((u32IntAttribs & 0xFFFFFFUL) << u32Pin);
}


/**
 * @brief       Disable GPIO interrupt
 *
 * @param[in]   PORT        GPIO port. It could be PORT0, PORT1, .. PORT4.
 * @param[in]   u32Pin      The pin of specified GPIO port. It could be 0 ~ 7.
 *
 * @return      None
 *
 * @details     This function is used to enable specified GPIO pin interrupt.
 */
static __INLINE void GPIO_DisableInt(GPIO_T *PORT, uint32_t u32Pin)
{
    PORT->IMD &= ~((BIT0) << u32Pin);
    PORT->IEN &= ~((0x00010001UL) << u32Pin);
}


/**
  * @} End of GPIO Device Function Interface
  */ 

/**
  * @} End of M051 Function Interface
  */ 


#endif  // __GPIO_H__
