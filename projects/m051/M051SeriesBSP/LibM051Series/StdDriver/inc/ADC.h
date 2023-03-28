/**************************************************************************//**
 * @file     ADC.h
 * @version  V2.1
 * $Revision: 7 $
 * $Date: 12/08/10 3:12p $
 * @brief    M051 Series ADC Driver Header File
 *
 * @note
 * Copyright (C) 2011 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#ifndef __ADC_H__
#define __ADC_H__


/*---------------------------------------------------------------------------------------------------------*/
/* Include related headers                                                                                 */
/*---------------------------------------------------------------------------------------------------------*/
#include "M051Series.h"

/** @addtogroup M051_FUNC M051 Function Interface
  * @{
  */

/** @addtogroup ADC_FUNC ADC Device Function Interface
  * @{
  */



/*---------------------------------------------------------------------------------------------------------*/
/*  ADCR Constant Definitions                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
#define ADC_ADCR_ADEN_CONVERTER_DISABLE   (0UL<<ADC_ADCR_ADEN_Pos)     /*!< ADC Converter Disable          */
#define ADC_ADCR_ADEN_CONVERTER_ENABLE    (1UL<<ADC_ADCR_ADEN_Pos)     /*!< ADC Converter Enable           */

#define ADC_ADCR_ADMD_SINGLE            (0UL<<ADC_ADCR_ADMD_Pos)     /*!< Single mode                */
#define ADC_ADCR_ADMD_BURST             (1UL<<ADC_ADCR_ADMD_Pos)     /*!< Burst mode                 */
#define ADC_ADCR_ADMD_SINGLE_CYCLE      (2UL<<ADC_ADCR_ADMD_Pos)     /*!< Single cycle scan mode     */
#define ADC_ADCR_ADMD_CONTINUOUS        (3UL<<ADC_ADCR_ADMD_Pos)     /*!< Continuous scan mode       */

#define ADC_ADCR_DIFFEN_SINGLE_END      (0UL<<ADC_ADCR_DIFFEN_Pos)   /*!< Single end input mode      */
#define ADC_ADCR_DIFFEN_DIFFERENTIAL    (1UL<<ADC_ADCR_DIFFEN_Pos)   /*!< Differential input type    */

#define ADC_ADCR_DMOF_UNSIGNED_OUTPUT   (0UL<<ADC_ADCR_DMOF_Pos)     /*!< Select the straight binary format as the output format of the conversion result   */
#define ADC_ADCR_DMOF_TWOS_COMPLEMENT   (1UL<<ADC_ADCR_DMOF_Pos)     /*!< Select the 2's complement format as the output format of the conversion result    */

#define ADC_ADCR_TRGEN_DISABLE          (0UL<<ADC_ADCR_TRGEN_Pos)    /*!< Enable or disable triggering of A/D conversion by external STADC pin.   */
#define ADC_ADCR_TRGEN_ENABLE           (1UL<<ADC_ADCR_TRGEN_Pos)    /*!< Disable or disable triggering of A/D conversion by external STADC pin.  */

#define ADC_ADCR_TRGS_STADC             (0UL<<ADC_ADCR_TRGS_Pos)     /*!< A/D conversion is started by external STADC pin.*/

#define ADC_ADCR_TRGCOND_LOW_LEVEL      (0UL<<ADC_ADCR_TRGCOND_Pos)  /*!< STADC Low level active     */
#define ADC_ADCR_TRGCOND_HIGH_LEVEL     (1UL<<ADC_ADCR_TRGCOND_Pos)  /*!< STADC High level active    */
#define ADC_ADCR_TRGCOND_FALLING_EDGE   (2UL<<ADC_ADCR_TRGCOND_Pos)  /*!< STADC Falling edge active  */
#define ADC_ADCR_TRGCOND_RISING_EDGE    (3UL<<ADC_ADCR_TRGCOND_Pos)  /*!< STADC Rising edge active   */

/*---------------------------------------------------------------------------------------------------------*/
/* ADCHER Constant Definitions                                                                             */
/*---------------------------------------------------------------------------------------------------------*/
#define ADC_ADCHER_PRESEL_EXT_INPUT_SIGNAL         (0<<ADC_ADCHER_PRESEL_Pos)    /*!< The input source of channel 7 is the external analog input                   */
#define ADC_ADCHER_PRESEL_INT_BANDGAP              (1<<ADC_ADCHER_PRESEL_Pos)    /*!< The input source of channel 7 is the internal bandgap voltage                */
#define ADC_ADCHER_PRESEL_INT_TEMPERATURE_SENSOR   (2<<ADC_ADCHER_PRESEL_Pos)    /*!< The input source of channel 7 is the output of internal temperature sensor   */

/*---------------------------------------------------------------------------------------------------------*/
/* ADCMPR Constant Definitions                                                                             */
/*---------------------------------------------------------------------------------------------------------*/
#define ADC_ADCMPR_CMPD(x)                    ((x) << ADC_ADCMPR_CMPD_Pos)          /*!< Compare value for compare function            */
#define ADC_ADCMPR_CMPMATCNT(x)               (((x)-1) << ADC_ADCMPR_CMPMATCNT_Pos) /*!< Match count for compare function              */
#define ADC_ADCMPR_CMPCH(x)                   ((x) << ADC_ADCMPR_CMPCH_Pos)  /*!< Compare channel for compare function                 */
#define ADC_ADCMPR_CMPCOND_LESS_THAN          (0<<ADC_ADCMPR_CMPCOND_Pos)    /*!< The compare condition is "less than".                */
#define ADC_ADCHER_CMPCOND_GREATER_OR_EQUAL   (1<<ADC_ADCMPR_CMPCOND_Pos)    /*!< The compare condition is "greater than or equal to". */


/*---------------------------------------------------------------------------------------------------------*/
/* ADC Operation Mode Constant Definitions                                                                 */
/*---------------------------------------------------------------------------------------------------------*/
#define ADC_SINGLE_MODE         0   /*!< ADC single mode            */
#define ADC_BURST_MODE          1   /*!< ADC burst mode             */
#define ADC_SINGLE_CYCLE_MODE   2   /*!< ADC single-cycle scan mode */
#define ADC_CONTINUOUS_MODE     3   /*!< ADC continuous scan mode   */

/*---------------------------------------------------------------------------------------------------------*/
/* ADC Trigger Condition Constant Definitions                                                              */
/*---------------------------------------------------------------------------------------------------------*/
#define ADC_LOW_LEVEL      0   /*!< ADC external trigger condition is low level trigger    */
#define ADC_HIGH_LEVEL     1   /*!< ADC external trigger condition is high level trigger   */
#define ADC_FALLING_EDGE   2   /*!< ADC external trigger condition is falling edge trigger */
#define ADC_RISING_EDGE    3   /*!< ADC external trigger condition is rising edge trigger  */

/*---------------------------------------------------------------------------------------------------------*/
/* ADC Compare Condition Constant Definitions                                                              */
/*---------------------------------------------------------------------------------------------------------*/
#define ADC_LESS_THAN          0   /*!< ADC compare condition is "less than the compare value"                */
#define ADC_GREATER_OR_EQUAL   1   /*!< ADC compare condition is "greater than or equal to the compare value" */

/*---------------------------------------------------------------------------------------------------------*/
/* Constant Definitions of ADC Channel 7 Input Source                                                      */
/*---------------------------------------------------------------------------------------------------------*/
#define ADC_CH7_EXT_INPUT_SIGNAL         0   /*!< External input signal       */
#define ADC_CH7_INT_BANDGAP              1   /*!< Internal band-gap voltage   */
#define ADC_CH7_INT_TEMPERATURE_SENSOR   2   /*!< Internal temperature sensor */


/*---------------------------------------------------------------------------------------------------------*/
/*  ADC Macro Definitions                                                                                  */
/*---------------------------------------------------------------------------------------------------------*/

/**
 * @details     ADC start convert
 */
#define _ADC_START_CONVERT()                (ADC->ADCR |= ADC_ADCR_ADST_Msk)

/**
 * @details     ADC stop convert
 */
#define _ADC_STOP_CONVERT()                 (ADC->ADCR &= ~ADC_ADCR_ADST_Msk)

/**
 * @details     ADC convert enable
 */
#define _ADC_CONVERT_ENABLE()               (ADC->ADCR |= ADC_ADCR_ADEN_Msk)

/**
 * @details     ADC convert disable
 */
#define _ADC_CONVERTER_DISABLE()              (ADC->ADCR &= ~ADC_ADCR_ADEN_Msk)

/**
 * @details     Enable external trigger  
 */
#define _ADC_ENABLE_EXT_TRG()               (ADC->ADCR |= ADC_ADCR_TRGEN_Msk)

/**
 * @details     Disable external trigger  
 */
#define _ADC_DISABLE_EXT_TRG()              (ADC->ADCR &= ~ADC_ADCR_TRGEN_Msk)

/**
 * @details     Enable ADC interrupt  
 */
#define _ADC_ENABLE_ADC_INT()                   (ADC->ADCR |= ADC_ADCR_ADIE_Msk)

/**
 * @details     Disable ADC interrupt  
 */
#define _ADC_DISABLE_ADC_INT()                  (ADC->ADCR &= ~ADC_ADCR_ADIE_Msk)

/**
 * @brief       Set ADC ADMD bit field
 *
 * @param[in]   Mode        A/D Converter Operation Mode, should be:
 *                          -  ADC_SINGLE_MODE       : Single mode
 *                          -  ADC_BURST_MODE        : Burst mode
 *                          -  ADC_SINGLE_CYCLE_MODE : Single cycle scan mode
 *                          -  ADC_CONTINUOUS_MODE   : Continuous scan mode 
 * @return      None
 *
 * @details     Set ADC A/D convert opreration mode
 */
#define _ADC_SET_OPMODE(Mode)               (ADC->ADCR = ((ADC->ADCR & ~ADC_ADCR_ADMD_Msk) | ((Mode)<<ADC_ADCR_ADMD_Pos)))

/**
 * @brief       Get ADC ADMD bit field
 *
 * @param[in]   None
 * @return      Mode        A/D Converter Operation Mode, should be:
 *                          -  ADC_SINGLE_MODE        : Single mode
 *                          -  ADC_BURST_MODE         : Burst mode
 *                          -  ADC_SINGLE_CYCLE_MODE  : Single cycle scan mode
 *                          -  ADC_CONTINUOUS_MODE    : Continuous scan mode
 *
 * @details     Get ADC convert opreration mode
 */
#define _ADC_GET_OPMODE()                   ((ADC->ADCR & ADC_ADCR_ADMD_Msk)>>ADC_ADCR_ADMD_Pos)

/**
 * @brief       Set ADC DIFFEN bit field
 *
 * @param[in]   None
 * @return      None
 *
 * @details     Configure the ADC analog input as differential input
 */
#define _ADC_SET_DIFFERENTIAL_INPUT_MODE()           (ADC->ADCR |= ADC_ADCR_DIFFEN_Msk)

/**
 * @brief       Set ADC DIFFEN bit field
 *
 * @param[in]   None
 * @return      None
 *
 * @details     Configure the ADC analog input as single-end input
 */
#define _ADC_SET_SINGLE_END_INPUT_MODE()           (ADC->ADCR = ((ADC->ADCR & ~ADC_ADCR_DIFFEN_Msk)))

/**
 * @brief       Set ADC TRGCOND bit field
 *
 * @param[in]   TrgCond     External trigger condition , should be:            
 *                          -  ADC_LOW_LEVEL    : STADC Low level active
 *                          -  ADC_HIGH_LEVEL   : STADC High level active
 *                          -  ADC_FALLING_EDGE : STADC Falling edge active
 *                          -  ADC_RISING_EDGE  : STADC Rising edge active
 * @return      None
 *
 * @details     Set ADC external trigger condition
 */
#define _ADC_SET_TRG_COND(TrgCond)          (ADC->ADCR = ((ADC->ADCR &~ADC_ADCR_TRGCOND_Msk)|((TrgCond)<<ADC_ADCR_TRGCOND_Pos)))

/**
 * @brief       Get converstion data of specified channel 
 *
 * @param[in]   Ch         ADC Channel , should be:
 *                          -  0-7
 * @return      16-bit data 
 *
 * @details     Read RSLT bit field to get conversion data 
 */
#define _ADC_GET_CONVERSION_DATA(Ch)        ((ADC->ADDR[(Ch)] & ADC_ADDR_RSLT_Msk)>>ADC_ADDR_RSLT_Pos)

/**
 * @brief       Get data overrun flag 
 *
 * @param[in]   Ch         ADC Channel , should be:
 *                          -  0-7
 * @retval      1: The data overrun flag of The specified channel is set.
 * @retval      0: The data overrun flag of The specified channel is cleared.
 *
 * @details     Read OVERRUN bit field to get specified channel status
 */
#define _ADC_IS_DATA_OVERRUN(Ch)            ((ADC->ADSR & (1<<((Ch)+ADC_ADSR_OVERRUN_Pos)))?1:0)

/**
 * @brief       Get data valid flag 
 *
 * @param[in]   Ch         ADC Channel , should be:
 *                          -  0-7
 * @retval      1: The data valid flag of The specified channel is set.
 * @retval      0: The data valid flag of The specified channel is cleared.
 *
 * @details     Read VALID bit field to get specified channel status
 */
#define _ADC_IS_DATA_VALID(Ch)              ((ADC->ADSR & (1<<((Ch)+ADC_ADSR_VALID_Pos)))?1:0)

/**
 * @details     Check if ADC conversion done or not
 */
#define _ADC_IS_COVERSION_DONE()            ((ADC->ADSR & ADC_ADSR_ADF_Msk) >> ADC_ADSR_ADF_Pos)

/**
 * @details     A while loop to check if ADC conversion done or not. Exit the loop if ADC coversion is done
 */
#define _ADC_WAIT_COVERSION_DONE()          while(!_ADC_IS_COVERSION_DONE())

/**
 * @details     Enable specified comparator interrupt
 */
#define _ADC_ENABLE_CMP_INT(Ch)         (ADC->ADCMPR[(Ch)] |= ADC_ADCMPR_CMPIE_Msk)

/**
 * @details     Disable specified comparator interrupt
 */
#define _ADC_DISABLE_CMP_INT(Ch)        (ADC->ADCMPR[(Ch)] &= ~ADC_ADCMPR_CMPIE_Msk)

/**
 * @details     Enable compare function
 */
#define _ADC_ENABLE_CMP(Ch)             (ADC->ADCMPR[(Ch)] |= ADC_ADCMPR_CMPEN_Msk)

/**
 * @details     Disable compare function
 */
#define _ADC_DISABLE_CMP(Ch)            (ADC->ADCMPR[(Ch)] &= ~ADC_ADCMPR_CMPEN_Msk)

/**
 * @details     Set comparison data
 */
#define _ADC_SET_CMP_DATA(Ch, CmpData)   (ADC->ADCMPR[(Ch)] = ((ADC->ADCMPR[(Ch)] & ~ADC_ADCMPR_CMPD_Msk)|((CmpData)<<ADC_ADCMPR_CMPD_Pos)))

/**
 * @details     Set comparator match count
 */
#define _ADC_SET_CMP_MATCH_CNT(Ch, CmpCnt)    (ADC->ADCMPR[(Ch)] = ((ADC->ADCMPR[(Ch)] & ~ADC_ADCMPR_CMPMATCNT_Msk) | (((CmpCnt)-1)<<ADC_ADCMPR_CMPMATCNT_Pos)))

/**
 * @details     Set compare channel 
 */
#define _ADC_SET_CMP_CHANNEL(Ch, X)      (ADC->ADCMPR[(Ch)] = (ADC->ADCMPR[(Ch)] & ~ADC_ADCMPR_CMPCH_Msk) | ((X)<<ADC_ADCMPR_CMPCH_Pos))

/**
 * @brief       Set compare condition 
 *
 * @param[in]   Ch         Comparator Channel , should be:
 *                          -  0-1
 * @param[in]   Cond       Compare Condition , should be:
 *                          - ADC_LESS_THAN         : The compare condition is "less than the compare value"
 *                          - ADC_GREATER_OR_EQUAL  : The compare condition is "greater than or equal to the compare value"
 * @return      None
 *
 * @details     Set the compare condition of the specified comparator
 */
#define _ADC_SET_CMP_COND(Ch, Cond)         (ADC->ADCMPR[(Ch)] = ((ADC->ADCMPR[(Ch)] & ~ADC_ADCMPR_CMPCOND_Msk) | ((Cond)<<ADC_ADCMPR_CMPCOND_Pos)))

/**
 * @brief       Set comparator 0 for compare configuration
 *
 * @param[in]   CmpCh      Compare ADC Channel , should be:
 *                          -  0-7
 * @param[in]   CmpCond    Compare Condition , should be:
 *                          - ADC_LESS_THAN         : The compare condition is "less than the compare value"
 *                          - ADC_GREATER_OR_EQUAL  : The compare condition is "greater than or equal to the compare value"
 * @param[in]   CmpData    Compare Data Value , should be:
 *                          -  0-0xFFF
 * @param[in]   CmpMatchCount
 *                         Compare match count, should be:
 *                          -  1-16
 * @return      None
 *
 * @details     Set comparator 0 parameters including compare channel, condition, data and match count.
 */
#define _ADC_SET_CMP0(CmpCh, CmpCond, CmpData, CmpMatchCount)  (ADC->ADCMPR[0] = (ADC->ADCMPR[0] & ~(ADC_ADCMPR_CMPD_Msk|ADC_ADCMPR_CMPMATCNT_Msk|ADC_ADCMPR_CMPCH_Msk|ADC_ADCMPR_CMPCOND_Msk)) | \
                                                               ((CmpData)<<ADC_ADCMPR_CMPD_Pos) | \
                                                               (((CmpMatchCount)-1)<<ADC_ADCMPR_CMPMATCNT_Pos) | \
                                                               ((CmpCh)<<ADC_ADCMPR_CMPCH_Pos) | \
                                                               ((CmpCond)<<ADC_ADCMPR_CMPCOND_Pos) | ADC_ADCMPR_CMPEN_Msk)

/**
 * @brief       Set comparator 1 for compare configuration
 *
 * @param[in]   CmpCh      Compare ADC Channel , should be:
 *                          -  0-7
 * @param[in]   CmpCond    Compare Condition , should be:
 *                          - ADC_LESS_THAN         : The compare condition is "less than the compare value"
 *                          - ADC_GREATER_OR_EQUAL  : The compare condition is "greater than or equal to the compare value"
 * @param[in]   CmpData    Compare Data Value , should be:
 *                          -  0-0xFFF
 * @param[in]   CmpMatchCount
 *                         Compare match count, should be:
 *                          -  1-16
 * @return      None
 *
 * @details     Set comparator 1 parameters including compare channel, condition, data and match count.
 */
#define _ADC_SET_CMP1(CmpCh, CmpCond, CmpData, CmpMatchCount)  (ADC->ADCMPR[1] = (ADC->ADCMPR[1] & ~(ADC_ADCMPR_CMPD_Msk|ADC_ADCMPR_CMPMATCNT_Msk|ADC_ADCMPR_CMPCH_Msk|ADC_ADCMPR_CMPCOND_Msk)) | \
                                                               ((CmpData)<<ADC_ADCMPR_CMPD_Pos) | \
                                                               (((CmpMatchCount)-1)<<ADC_ADCMPR_CMPMATCNT_Pos) | \
                                                               ((CmpCh)<<ADC_ADCMPR_CMPCH_Pos) | \
                                                               ((CmpCond)<<ADC_ADCMPR_CMPCOND_Pos) | ADC_ADCMPR_CMPEN_Msk)

/**
 * @brief       Use bitwise to set ADC Channel
 *
 * @param[in]   Bitwise     ADC Channel configure, should be:
 *                          -  0-0xFF
 * @return      None
 *
 * @details     The function is used to set ADC channel groups.
 *
 */
#define _ADC_SET_CHANNEL(Bitwise)          (ADC->ADCHER |= ((ADC->ADCHER&~ADC_ADCHER_CHEN_Msk)|(Bitwise)))

/**
 * @brief       Enable specified ADC channel
 *
 * @param[in]   Ch         ADC Channel , should be:
 *                          -  0-7
 * @return      None
 *
 * @details     The function is used to enable single ADC channel.
 */
#define _ADC_ENABLE_CHANNEL(Ch)          (ADC->ADCHER |= ((ADC->ADCHER&~ADC_ADCHER_CHEN_Msk)|(1<<(Ch))))

/**
 * @brief       Disable specified ADC channel
 *
 * @param[in]   Ch         ADC Channel , should be:
 *                          -  0-7
 * @return      None
 *
 * @details     The function is used to disable single ADC channel.
 */
#define _ADC_DISABLE_CHANNEL(Ch)         (ADC->ADCHER &= ~(1<<(Ch)))

/**
 * @brief       Select the source of the analog input channel 7
 *
 * @param[in]   Option        Analog input select, should be:
 *                          -  ADC_CH7_EXT_INPUT_SIGNAL        : the external analog input 
 *                          -  ADC_CH7_INT_BANDGAP             : the internal bandgap voltage  
 *                          -  ADC_CH7_INT_TEMPERATURE_SENSOR  : the output of internal temperature sensor 
 * @return      None
 *
 * @details     Select the source of the analog input channel 7
 */
#define _ADC_CONFIG_CH7(Option)              (ADC->ADCHER = (ADC->ADCHER &~(ADC_ADCHER_PRESEL_Msk))|((Option)<<ADC_ADCHER_PRESEL_Pos))

/**
 * @details    Get current converse channel
 */
#define _ADC_GET_CURRENT_COVERSION_CHANNEL()    ((ADC->ADSR & ADC_ADSR_CHANNEL_Msk) >> ADC_ADSR_CHANNEL_Pos)

/**
 * @details    Check if ADC is busy or not
 */
#define _ADC_IS_BUSY()                      ((ADC->ADSR & ADC_ADSR_BUSY_Msk) >> ADC_ADSR_BUSY_Pos)

/**
 * @details    Get specified comparator channel compare flag
 */
#define _ADC_GET_CMP_FLAG(ADCCMP_CH)        ((ADC->ADSR & (1<<(ADC_ADSR_CMPF0_Pos+(ADCCMP_CH)))) >> (ADC_ADSR_CMPF0_Pos+(ADCCMP_CH)))

/**
 * @details     ADC IP reset 
 */
__INLINE void ADC_ResetIP()
{
    SYS->IPRSTC2 |= (1<<SYS_IPRSTC2_ADC_RST_Pos) ;
    SYS->IPRSTC2 &= ~(1<<(SYS_IPRSTC2_ADC_RST_Pos)) ;
} 

/**
 * @details     Enable ADC IP clock
 */
#define _ADC_ENABLE_CLOCK()   (SYSCLK->APBCLK |= SYSCLK_APBCLK_ADC_EN_Msk) 

/**
 * @details     Disable ADC IP clock
 */
#define _ADC_DISABLE_CLOCK()  (SYSCLK->APBCLK &= ~SYSCLK_APBCLK_ADC_EN_Msk) 

/**
 * @brief      Return the A/D conversion rate (sample/second)
 *
 * @param      None
 *
 * @return     Conversion rate
 *
 * @details    The conversion rate depends on the clock source of ADC clock.
 *             In NuMicro M05xxAx Series (Ex. M0516LAN), it needs about 27 ADC clocks to complete
 *             an A/D conversion.
 */
static __INLINE uint32_t ADC_GetConversionRate_M05xxxAN()
{
    uint32_t u32AdcClkSrcSel;
    uint32_t u32ClkTbl[4] = {__XTAL, 0, 0, __IRC22M};    
    
    u32ClkTbl[1] = PllClock;
    u32AdcClkSrcSel = ((SYSCLK->CLKSEL1 & SYSCLK_CLKSEL1_ADC_S_Msk) >> SYSCLK_CLKSEL1_ADC_S_Pos);
    return   ((u32ClkTbl[u32AdcClkSrcSel]) / (((SYSCLK->CLKDIV & SYSCLK_CLKDIV_ADC_N_Msk)>>SYSCLK_CLKDIV_ADC_N_Pos)+ 1) / 27);
}

/**
 * @brief      Return the A/D conversion rate (sample/second)
 *
 * @param      None
 *
 * @return     Conversion rate
 *
 * @details    The conversion rate depends on the clock source of ADC clock.
 *             In NuMicro M05xxBx Series (Ex. M0516LBN), it only needs 21 ADC
 *             clocks to complete an A/D conversion when the ADC clock source is HCLK.
 */
static __INLINE uint32_t ADC_GetConversionRate_M05xxxBN()
{
    uint32_t u32AdcClkSrcSel;
    uint32_t u32ClkTbl[4] = {__XTAL, 0, 0, __IRC22M};    
    
    u32ClkTbl[1] = PllClock;
    u32ClkTbl[2] = SystemCoreClock;
    u32AdcClkSrcSel = ((SYSCLK->CLKSEL1 & SYSCLK_CLKSEL1_ADC_S_Msk) >> SYSCLK_CLKSEL1_ADC_S_Pos);
    return   ((u32ClkTbl[u32AdcClkSrcSel]) / (((SYSCLK->CLKDIV & SYSCLK_CLKDIV_ADC_N_Msk)>>SYSCLK_CLKDIV_ADC_N_Pos)+ 1) / 21);
}

/**
 * @details     Set ADC clock source  
 */
#define _ADC_SET_CLK_SRC(ClockSrc)     (SYSCLK->CLKSEL1 = (SYSCLK->CLKSEL1 &~SYSCLK_CLKSEL1_ADC_S_Msk)|((ClockSrc)<<SYSCLK_CLKSEL1_ADC_S_Pos))

/**
 * @details     Set ADC clock divisor. It could be 1~256. 
 */
#define _ADC_SET_CLK_DIV(Divisor)      (SYSCLK->CLKDIV  = (SYSCLK->CLKDIV & ~SYSCLK_CLKDIV_ADC_N_Msk)|(SYSCLK_CLKDIV_ADC(Divisor)))

/**
 * @details     Get the status of the ADC interrupt flag
 */
#define _ADC_GET_ADC_INT_FLAG()    ((ADC->ADSR & ADC_ADSR_ADF_Msk)>>ADC_ADSR_ADF_Pos)

/**
 * @details     Get the status of the ADC CMP0 interrupt flag
 */
#define _ADC_GET_CMP0_INT_FLAG()    _ADC_GET_CMP_FLAG(0)

/**
 * @details     Get the status of the ADC CMP1 interrupt flag
 */
#define _ADC_GET_CMP1_INT_FLAG()    _ADC_GET_CMP_FLAG(1)

/**
 * @details     Clear the ADC interrupt flag
 */
#define _ADC_CLEAR_ADC_INT_FLAG()   (ADC->ADSR = ADC_ADSR_ADF_Msk)

/**
 * @details     Clear the ADC CMP0 interrupt flag
 */
#define _ADC_CLEAR_CMP0_INT_FLAG()  (ADC->ADSR = ADC_ADSR_CMPF0_Msk)

/**
 * @details     Clear the ADC CMP1 interrupt flag
 */
#define _ADC_CLEAR_CMP1_INT_FLAG()  (ADC->ADSR = ADC_ADSR_CMPF1_Msk)




/*---------------------------------------------------------------------------------------------------------*/
/* (ONLY in M05xxxAN)                                                                                      */
/*---------------------------------------------------------------------------------------------------------*/
/**
 * @details     Enable self-calibaration function
 */
#define _ADC_ENABLE_SELF_CALIBRATION()      (ADC->ADCALR |= ADC_ADCALR_CALEN_Msk)

/**
 * @details     Disable self-calibaration function
 */
#define _ADC_DISABLE_SELF_CALIBRATION()     (ADC->ADCALR &= ~ADC_ADCALR_CALEN_Msk)

/**
 * @details     Check if calibaration done or not.
 */
#define _ADC_IS_CALIBRATION_DONE()          ((ADC->ADCALR & ADC_ADCALR_CALDONE_Msk)>>ADC_ADCALR_CALDONE_Pos)

/**
  * @} End of ADC Device Function Interface
  */ 

/**
  * @} End of M051 Function Interface
  */ 


#endif
