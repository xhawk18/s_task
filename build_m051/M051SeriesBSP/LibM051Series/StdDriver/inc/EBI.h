/**************************************************************************//**
 * @file     EBI.h
 * @version  V2.1
 * $Revision: 4 $
 * $Date: 12/08/10 3:12p $
 * @brief    M051 Series External Bus Interface Driver Header File
 *
 * @note
 * Copyright (C) 2011 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#ifndef __EBI_H__
#define __EBI_H__

#include "M051Series.h"

/** @addtogroup M051_FUNC M051 Function Interface
  * @{
  */

/** @addtogroup EBI_FUNC EBI Device Function Interface
  * @{
  */


/*---------------------------------------------------------------------------------------------------------*/
/*  Miscellaneous Constant Definitions                                                                     */
/*---------------------------------------------------------------------------------------------------------*/
#define EBI_START_BASE_ADDR         0x60000000UL /*!< EBI start base address */
#define EBI_MAX_SIZE                0x20000UL     /*!< Max. EBI size          */


/*---------------------------------------------------------------------------------------------------------*/
/*  EBICON Constant Definitions                                                                            */
/*---------------------------------------------------------------------------------------------------------*/
#define EBI_EBICON_MCLKDIV_1        0x00000000UL /*!< EBI output clock(MCLK) is HCLK/1 */
#define EBI_EBICON_MCLKDIV_2        0x00000100UL /*!< EBI output clock(MCLK) is HCLK/2 */
#define EBI_EBICON_MCLKDIV_4        0x00000200UL /*!< EBI output clock(MCLK) is HCLK/4 */
#define EBI_EBICON_MCLKDIV_8        0x00000300UL /*!< EBI output clock(MCLK) is HCLK/8 */
#define EBI_EBICON_MCLKDIV_16       0x00000400UL /*!< EBI output clock(MCLK) is HCLK/16 */
#define EBI_EBICON_MCLKDIV_32       0x00000500UL /*!< EBI output clock(MCLK) is HCLK/32 */
#define EBI_EBICON_ExttALE(x)       (((x)-1) << EBI_EBICON_ExttALE_Pos) /*!< ALE width(tALE) is (x)*MCLK. It could be 1~8 */


/*---------------------------------------------------------------------------------------------------------*/
/*  EXTIME Constant Definitions                                                                            */
/*---------------------------------------------------------------------------------------------------------*/
#define EBI_EXTIME_ExttACC(x)       (((x)-1) << EBI_EXTIME_ExttACC_Pos) /*!< Data Access Time(tACC) is (x)*MCLK. It could be 1~16 */
#define EBI_EXTIME_ExttAHD(x)       (((x)-1) << EBI_EXTIME_ExttAHD_Pos) /*!< Data Access Hold Time(tAHD) is (x)*MCLK. It could be 1~8 */
#define EBI_EXTIME_ExtIW2X(x)       ((x) << EBI_EXTIME_ExtIW2X_Pos)     /*!< Idle State Cycle After Write(IW2X) is (x)*MCLK. It could be 0~15 */
#define EBI_EXTIME_ExtIR2R(x)       ((x) << EBI_EXTIME_ExtIR2R_Pos)     /*!< Idle State Cycle Between Read-Read(IR2R) is (x)*MCLK. It could be 0~15 */


/*---------------------------------------------------------------------------------------------------------*/
/*  EBI Macro Definitions                                                                                  */
/*---------------------------------------------------------------------------------------------------------*/
/**
 * @details     Enable EBI function
 */             
#define _EBI_ENABLE_EBI()           (EBI->EBICON |= EBI_EBICON_ExtEN_Msk)    


/**
 * @details     Disable EBI function
 */             
#define _EBI_DISABLE_EBI()          (EBI->EBICON &= ~EBI_EBICON_ExtEN_Msk)


/**
 * @details     Set EBI data width is 8-bit
 */             
#define _EBI_SET_DATAWIDTH_8BIT()   (EBI->EBICON &= ~EBI_EBICON_ExtBW16_Msk)


/**
 * @details     Set EBI data width is 16-bit
 */             
#define _EBI_SET_DATAWIDTH_16BIT()  (EBI->EBICON |= EBI_EBICON_ExtBW16_Msk)


/**
 * @details     Set EBI ALE time.  1 <= x <= 8
 */             
#define _EBI_SET_ExttALE(x)         (EBI->EBICON = ((EBI->EBICON & ~EBI_EBICON_ExttALE_Msk) | EBI_EBICON_ExttALE(x)))  


/**
 * @details     Set EBI MCLK divider. x = 1, 2, 4, 8, 16 or 32
 */             
#define _EBI_SET_MCLKDIV(x)         (EBI->EBICON = ((EBI->EBICON & ~EBI_EBICON_MCLKDIV_Msk) | (EBI_EBICON_MCLKDIV_##x)))


/**
 * @details     Set EBI ACC time. 1 <= x <= 16
 */             
#define _EBI_SET_ExttACC(x)         (EBI->EXTIME = ((EBI->EXTIME & ~EBI_EXTIME_ExttACC_Msk) | EBI_EXTIME_ExttACC(x)))  


/**
 * @details     Set EBI AHD time. 1 <= x <= 8
 */             
#define _EBI_SET_ExttAHD(x)         (EBI->EXTIME = ((EBI->EXTIME & ~EBI_EXTIME_ExttAHD_Msk) | EBI_EXTIME_ExttAHD(x)))  


/**
 * @details     Set Idle State Cycle After Write. 0 <= x <= 15
 */             
#define _EBI_SET_ExtIW2X(x)         (EBI->EXTIME = ((EBI->EXTIME & ~EBI_EXTIME_ExtIW2X_Msk) | EBI_EXTIME_ExtIW2X(x)))  


/**
 * @details     Set Idle State Cycle Between Read-Read. 0 <= x <= 15
 */             
#define _EBI_SET_ExtIR2R(x)         (EBI->EXTIME = ((EBI->EXTIME & ~EBI_EXTIME_ExtIR2R_Msk) | EBI_EXTIME_ExtIR2R(x))) 


/**
 * @details     Get ALE time setting
 */             
#define _EBI_GET_ExttALE()          (((EBI->EBICON & EBI_EBICON_ExttALE_Msk) >> EBI_EBICON_ExttALE_Pos) + 1)


/**
 * @details     Get MCLK divider setting of EBICON register.
 */             
#define _EBI_GET_MCLKDIV()          ((EBI->EBICON & EBI_EBICON_MCLKDIV_Msk) >> EBI_EBICON_MCLKDIV_Pos)


/**
 * @details     Get ACC time setting
 */             
#define _EBI_GET_ExttACC()          (((EBI->EXTIME & EBI_EXTIME_ExttACC_Msk) >> EBI_EXTIME_ExttACC_Pos) + 1)


/**
 * @details     Get AHD time setting
 */             
#define _EBI_GET_ExttAHD()          (((EBI->EXTIME & EBI_EXTIME_ExttAHD_Msk) >> EBI_EXTIME_ExttAHD_Pos) + 1)


/**
 * @details     Get Idle State Cycle After Write
 */             
#define _EBI_GET_ExtIW2X()          ((EBI->EXTIME & EBI_EXTIME_ExtIW2X_Msk) >> EBI_EXTIME_ExtIW2X_Pos)


/**
 * @details     Get Idle State Cycle Between Read-Read
 */             
#define _EBI_GET_ExtIR2R()          ((EBI->EXTIME & EBI_EXTIME_ExtIR2R_Msk) >> EBI_EXTIME_ExtIR2R_Pos)

/**
  * @} End of EBI Device Function Interface
  */ 

/**
  * @} End of M051 Function Interface
  */ 


#endif  // __EBI_H__

