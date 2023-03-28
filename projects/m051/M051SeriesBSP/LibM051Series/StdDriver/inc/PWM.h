/**************************************************************************//**
 * @file     PWM.h
 * @version  V2.1
 * $Revision: 7 $
 * $Date: 12/08/10 3:12p $
 * @brief    M051 Series PWM Generator and Capture Timer Driver Header File
 *
 * @note
 * Copyright (C) 2011 Nuvoton Technology Corp. All rights reserved.
 *
 *
 ******************************************************************************/
#ifndef __PWM_H__
#define __PWM_H__    

/*---------------------------------------------------------------------------------------------------------*/
/* Includes of system headers                                                                              */
/*---------------------------------------------------------------------------------------------------------*/

#include "M051Series.h"


/** @addtogroup M051_FUNC M051 Function Interface
  * @{
  */

/** @addtogroup PWM_FUNC PWM Device Function Interface
  * @{
  */



/*---------------------------------------------------------------------------------------------------------*/
/* Macro, type and constant definitions                                                                    */
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/*  PPR constants definitions                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
#define PWM_PPR_CP01(x)     (((x)-1) << PWM_PPR_CP01_Pos) /*!< PPR setting for PWM01 clock prescale. x = 2 ~ 256 */
#define PWM_PPR_CP23(x)     (((x)-1) << PWM_PPR_CP23_Pos) /*!< PPR setting for PWM23 clock prescale. x = 2 ~ 256 */
#define PWM_PPR_DZI01(x)    ((x)<< PWM_PPR_DZI01_Pos)  /*!< PPR setting for PWM01 dead zone. x = 0 ~ 255 */
#define PWM_PPR_DZI23(x)    ((x)<< PWM_PPR_DZI23_Pos)  /*!< PPR setting for PWM01 dead zone. x = 0 ~ 255 */

/*---------------------------------------------------------------------------------------------------------*/
/*  CSR constants definitions                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
#define PWM_CSR_DIV1    4         /*!< CSR setting for PWM Timer clock source divided by 1 */
#define PWM_CSR_DIV2    0         /*!< CSR setting for PWM Timer clock source divided by 2 */
#define PWM_CSR_DIV4    1         /*!< CSR setting for PWM Timer clock source divided by 4 */
#define PWM_CSR_DIV8    2         /*!< CSR setting for PWM Timer clock source divided by 8 */
#define PWM_CSR_DIV16   3         /*!< CSR setting for PWM Timer clock source divided by 16 */

#define PWM_CSR_CSR0(x)     ((x) << PWM_CSR_CSR0_Pos) /*!< CSR setting for CSR0 */
#define PWM_CSR_CSR1(x)     ((x) << PWM_CSR_CSR1_Pos) /*!< CSR setting for CSR1 */
#define PWM_CSR_CSR2(x)     ((x) << PWM_CSR_CSR2_Pos) /*!< CSR setting for CSR2 */
#define PWM_CSR_CSR3(x)     ((x) << PWM_CSR_CSR3_Pos) /*!< CSR setting for CSR3 */

/*---------------------------------------------------------------------------------------------------------*/
/*  PCR constants definitions                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
#define PWM_PCR_CH0EN_ENABLE    (1UL << PWM_PCR_CH0EN_Pos) /*!< PCR setting for channel 0 Timer Enable */
#define PWM_PCR_CH1EN_ENABLE    (1UL << PWM_PCR_CH1EN_Pos) /*!< PCR setting for channel 1 Timer Enable */
#define PWM_PCR_CH2EN_ENABLE    (1UL << PWM_PCR_CH2EN_Pos) /*!< PCR setting for channel 2 Timer Enable */
#define PWM_PCR_CH3EN_ENABLE    (1UL << PWM_PCR_CH3EN_Pos) /*!< PCR setting for channel 3 Timer Enable */
#define PWM_PCR_CH0INV_ENABLE  (1UL << PWM_PCR_CH0INV_Pos) /*!< PCR setting for channel 0 PWM-Timer Output Inverter Enable */
#define PWM_PCR_CH1INV_ENABLE  (1UL << PWM_PCR_CH1INV_Pos) /*!< PCR setting for channel 1 PWM-Timer Output Inverter Enable */
#define PWM_PCR_CH2INV_ENABLE  (1UL << PWM_PCR_CH2INV_Pos) /*!< PCR setting for channel 2 PWM-Timer Output Inverter Enable */
#define PWM_PCR_CH3INV_ENABLE  (1UL << PWM_PCR_CH3INV_Pos) /*!< PCR setting for channel 3 PWM-Timer Output Inverter Enable */
#define PWM_PCR_CH0MOD_AUTO_RELOAD (1UL << PWM_PCR_CH0MOD_Pos) /*!< PCR setting for channel 0 Timer auto-reload mode */
#define PWM_PCR_CH1MOD_AUTO_RELOAD (1UL << PWM_PCR_CH1MOD_Pos) /*!< PCR setting for channel 1 Timer auto-reload mode */
#define PWM_PCR_CH2MOD_AUTO_RELOAD (1UL << PWM_PCR_CH2MOD_Pos) /*!< PCR setting for channel 2 Timer auto-reload mode */
#define PWM_PCR_CH3MOD_AUTO_RELOAD (1UL << PWM_PCR_CH3MOD_Pos) /*!< PCR setting for channel 3 Timer auto-reload mode */
#define PWM_PCR_CH0MOD_ONE_SHOT    0                           /*!< PCR setting for channel 0 Timer one-shot mode */
#define PWM_PCR_CH1MOD_ONE_SHOT    0                           /*!< PCR setting for channel 1 Timer one-shot mode */
#define PWM_PCR_CH2MOD_ONE_SHOT    0                           /*!< PCR setting for channel 2 Timer one-shot mode */
#define PWM_PCR_CH3MOD_ONE_SHOT    0                           /*!< PCR setting for channel 3 Timer one-shot mode */
#define PWM_PCR_DZEN01_ENABLE (1UL << PWM_PCR_DZEN01_Pos) /*!< PCR setting for Dead-Zone 0 Generator Enable/ */
#define PWM_PCR_DZEN23_ENABLE (1UL << PWM_PCR_DZEN23_Pos) /*!< PCR setting for Dead-Zone 2 Generator Enable/ */

/*---------------------------------------------------------------------------------------------------------*/
/*  PIER constants definitions                                                                             */
/*---------------------------------------------------------------------------------------------------------*/
#define PWM_PIER_PWMIE0_ENABLE  (1UL << PWM_PIER_PWMIE0_Pos) /*!< PIER setting for channel 0 Timer period Interrupt Enable */
#define PWM_PIER_PWMIE1_ENABLE  (1UL << PWM_PIER_PWMIE1_Pos) /*!< PIER setting for channel 1 Timer period Interrupt Enable */
#define PWM_PIER_PWMIE2_ENABLE  (1UL << PWM_PIER_PWMIE2_Pos) /*!< PIER setting for channel 2 Timer period Interrupt Enable */
#define PWM_PIER_PWMIE3_ENABLE  (1UL << PWM_PIER_PWMIE3_Pos) /*!< PIER setting for channel 3 Timer period Interrupt Enable */

/*---------------------------------------------------------------------------------------------------------*/
/*  CCR0/CCR2 constants definitions                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
#define PWM_CCR0_INV0_ENABLE  (1UL << PWM_CCR0_INV0_Pos) /*!< CCR0 setting for Channel 0 Capture Inverter enable */
#define PWM_CCR0_INV1_ENABLE  (1UL << PWM_CCR0_INV1_Pos) /*!< CCR0 setting for Channel 1 Capture Inverter enable */
#define PWM_CCR2_INV2_ENABLE  (1UL << PWM_CCR2_INV2_Pos) /*!< CCR2 setting for Channel 2 Capture Inverter enable */
#define PWM_CCR2_INV3_ENABLE  (1UL << PWM_CCR2_INV3_Pos) /*!< CCR2 setting for Channel 3 Capture Inverter enable */

#define PWM_CCR0_CRL_IE0_ENABLE  (1UL << PWM_CCR0_CRL_IE0_Pos) /*!< CCR0 setting for Channel 0 Capture Rising Latch Interrupt Enable */
#define PWM_CCR0_CRL_IE1_ENABLE  (1UL << PWM_CCR0_CRL_IE1_Pos) /*!< CCR0 setting for Channel 1 Capture Rising Latch Interrupt Enable */
#define PWM_CCR2_CRL_IE2_ENABLE  (1UL << PWM_CCR2_CRL_IE2_Pos) /*!< CCR2 setting for Channel 2 Capture Rising Latch Interrupt Enable */
#define PWM_CCR2_CRL_IE3_ENABLE  (1UL << PWM_CCR2_CRL_IE3_Pos) /*!< CCR2 setting for Channel 3 Capture Rising Latch Interrupt Enable */

#define PWM_CCR0_CFL_IE0_ENABLE  (1UL << PWM_CCR0_CFL_IE0_Pos) /*!< CCR0 setting for Channel 0 Capture Falling Latch Interrupt Enable */
#define PWM_CCR0_CFL_IE1_ENABLE  (1UL << PWM_CCR0_CFL_IE1_Pos) /*!< CCR0 setting for Channel 1 Capture Falling Latch Interrupt Enable */
#define PWM_CCR2_CFL_IE2_ENABLE  (1UL << PWM_CCR2_CFL_IE2_Pos) /*!< CCR2 setting for Channel 2 Capture Falling Latch Interrupt Enable */
#define PWM_CCR2_CFL_IE3_ENABLE  (1UL << PWM_CCR2_CFL_IE3_Pos) /*!< CCR2 setting for Channel 3 Capture Falling Latch Interrupt Enable */

#define PWM_CCR0_CAPCH0EN_ENABLE  (1UL << PWM_CCR0_CAPCH0EN_Pos) /*!< CCR0 setting for Channel 0 Capture Function Enable */
#define PWM_CCR0_CAPCH1EN_ENABLE  (1UL << PWM_CCR0_CAPCH1EN_Pos) /*!< CCR0 setting for Channel 1 Capture Function Enable */
#define PWM_CCR2_CAPCH2EN_ENABLE  (1UL << PWM_CCR2_CAPCH2EN_Pos) /*!< CCR2 setting for Channel 2 Capture Function Enable */
#define PWM_CCR2_CAPCH3EN_ENABLE  (1UL << PWM_CCR2_CAPCH3EN_Pos) /*!< CCR2 setting for Channel 3 Capture Function Enable */

/*---------------------------------------------------------------------------------------------------------*/
/*  CAPENR constants definitions                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
#define PWM_CAPENR_CINEN0_ENABLE  (1UL << PWM_CAPENR_CINEN0_Pos) /*!< CAPENR setting for Channel 0 Capture Input Enable */
#define PWM_CAPENR_CINEN1_ENABLE  (1UL << PWM_CAPENR_CINEN1_Pos) /*!< CAPENR setting for Channel 1 Capture Input Enable */
#define PWM_CAPENR_CINEN2_ENABLE  (1UL << PWM_CAPENR_CINEN2_Pos) /*!< CAPENR setting for Channel 2 Capture Input Enable */
#define PWM_CAPENR_CINEN3_ENABLE  (1UL << PWM_CAPENR_CINEN3_Pos) /*!< CAPENR setting for Channel 3 Capture Input Enable */

/*---------------------------------------------------------------------------------------------------------*/
/*  POE constants definitions                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
#define PWM_POE_PWM0_ENABLE  (1UL << PWM_POE_PWM0_Pos) /*!< POE setting for Channel 0 PWM Output Enable */
#define PWM_POE_PWM1_ENABLE  (1UL << PWM_POE_PWM1_Pos) /*!< POE setting for Channel 1 PWM Output Enable */
#define PWM_POE_PWM2_ENABLE  (1UL << PWM_POE_PWM2_Pos) /*!< POE setting for Channel 2 PWM Output Enable */
#define PWM_POE_PWM3_ENABLE  (1UL << PWM_POE_PWM3_Pos) /*!< POE setting for Channel 3 PWM Output Enable */

/*---------------------------------------------------------------------------------------------------------*/
/*  PWM Group channel numebr constants definitions                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#define PWM_CH0     0x0            /*!< PWM Group A/B channel 0 */
#define PWM_CH1     0x1            /*!< PWM Group A/B channel 1 */
#define PWM_CH2     0x2            /*!< PWM Group A/B channel 2 */
#define PWM_CH3     0x3            /*!< PWM Group A/B channel 3 */

#define PWM_CCR_MASK    0x000F000F          /*!< PWM CCR0/CCR2 bit0~3 and bit16~19 mask */ 


/**
 * @brief      Set timer pre-scale for specified channel
 *
 * @param[in]  PWM            PWM group 
 *                            - PWMA : PWM Group A
 *                            - PWMB : PWM Group B 
 * @param[in]  u32Ch          PWM channel for PWM group A/B
 *                            - PWM_CH0
 *                            - PWM_CH1
 *                            - PWM_CH2
 *                            - PWM_CH3     
 * @param[in]  u32Prescaler   PWM timer prescaler value(0 ~ 0xFF)
 *
 * @return     None
 *
 * @details    This function is used to set timer pre-scale for specified channel
 *
 * @note       - If u32Prescaler = 0, corresponding PWM-timer will be stopped
 *             - If u32Prescaler = x (x not equal to 0), it means Clock input is divided by (x + 1) before it is fed to the corresponding PWM counter.
 
 */
#define _PWM_SET_TIMER_PRESCALE(PWM,u32Ch,u32Prescaler)\
{\
    (PWM)->PPR = (PWM)->PPR&~(PWM_PPR_CP01_Msk<<(((u32Ch)/2)*8))\
                                 |((u32Prescaler)<<(PWM_PPR_CP01_Pos+(((u32Ch)/2)*8)));\
}




/**
 * @brief      Get timer pre-scale for specified channel
 *
 * @param[in]  PWM            PWM group 
 *                            - PWMA : PWM Group A
 *                            - PWMB : PWM Group B 
 * @param[in]  u32Ch          PWM channel for PWM group A/B
 *                            - PWM_CH0
 *                            - PWM_CH1
 *                            - PWM_CH2
 *                            - PWM_CH3     
 * @return     Pre-scaler value (0~0xFF)
 *
 * @details    This function is used to get timer pre-scale for specified channel
 *
 * @note       - If retuened Pre-scaler value = 0, it means corresponding PWM-timer stop
 *             - If retuened Pre-scaler value = x (x not equal to 0), it means Clock input is divided by (x + 1) before it is fed to the corresponding PWM counter.
 */
#define _PWM_GET_TIMER_PRESCALE(PWM,u32Ch) (((PWM)->PPR&(PWM_PPR_CP01_Msk<<(((u32Ch)/2)*8)))>>(PWM_PPR_CP01_Pos+(((u32Ch)/2)*8)))




/**
 * @brief      Set PWM dead-zone length for specified channel pair
 *
 * @param[in]  PWM            PWM group 
 *                            - PWMA : PWM Group A
 *                            - PWMB : PWM Group B 
 * @param[in]  u32Ch          PWM channel for PWM group A/B
 *                            - PWM_CH0
 *                            - PWM_CH2   
 * @param[in]  u32DeadZoneLen PWM dead-zone length (0 - 0xFF)
 *
 * @return     None
 *
 * @details    This function is used to set PWM dead-zone length for specified channel.\n
 *             The unit time of dead zone length = [(prescale+1)*(clock source divider)]/PWMxy_CLK ,\n
 *             where xy, could be 01, 23 depends on selected PWM channel for PWM group A/B.
 *
 * @note       - Channel 0 and channel 1 are complementary pair and share the same dead-zone length.
 *             - Channel 2 and channel 3 are complementary pair and share the same dead-zone length. 
 *
 */
#define _PWM_SET_PWM_DEADZONE_LENGTH(PWM,u32Ch,u32DeadZoneLen)\
{\
    (PWM)->PPR = (PWM)->PPR&~(PWM_PPR_DZI01_Msk<<(((u32Ch)/2)*8))\
                                 |((u32DeadZoneLen)<<(PWM_PPR_DZI01_Pos+(((u32Ch)/2)*8)));\
}



/**
 * @brief       Get PWM dead-zone length for specified channel pair
 *
 * @param[in]  PWM            PWM group 
 *                            - PWMA : PWM Group A
 *                            - PWMB : PWM Group B 
 * @param[in]  u32Ch          PWM channel for PWM group A/B
 *                            - PWM_CH0
 *                            - PWM_CH2   
 *
 * @return     Specified channel PWM dead-zone length (0 - 0xFF)
 *
 * @details    This function is used to get PWM dead-zone length for specified channel.
 *
 *             The unit time of dead zone length = [(prescale+1)*(clock source divider)]/PWMxy_CLK , \n
 *             where xy, could be 01, 23, depends on selected PWM channel for PWM group A/B..
 *
 * @note       - Channel 0 and channel 1 are complementary pair and share the same dead-zone length.
 *             - Channel 2 and channel 3 are complementary pair and share the same dead-zone length.  
 */
#define _PWM_GET_PWM_DEADZONE_LENGTH(PWM,u32Ch) (((PWM)->PPR&(PWM_PPR_DZI01_Msk<<(((u32Ch)/2)*8)))>>(PWM_PPR_DZI01_Pos+(((u32Ch)/2)*8)))




/**
 * @brief       Set Timer clock source divider selection for specified channel
 *
 * @param[in]  PWM            PWM group 
 *                            - PWMA : PWM Group A
 *                            - PWMB : PWM Group B 
 * @param[in]  u32Ch          PWM channel for PWM group A/B
 *                            - PWM_CH0
 *                            - PWM_CH1
 *                            - PWM_CH2
 *                            - PWM_CH3     
 * @param[in]  u32ClockDivSel Timer clock source divider selection
 *                            - PWM_CSR_DIV1 (4)
 *                            - PWM_CSR_DIV2 (0) 
 *                            - PWM_CSR_DIV4 (1) 
 *                            - PWM_CSR_DIV8 (2) 
 *                            - PWM_CSR_DIV16(3)   
 *
 * @return     None
 *
 * @details    This function is used to set Timer clock source divider selection for specified channel
 *
 */
#define _PWM_SET_TIMER_CLOCK_DIV(PWM,u32Ch,u32ClockDivSel)\
{\
    (PWM)->CSR = (PWM)->CSR&~(PWM_CSR_CSR0_Msk<<((u32Ch)*4))\
                                 |((u32ClockDivSel)<<(PWM_CSR_CSR0_Pos+((u32Ch)*4)));\
}


/**
 * @brief      Get timer clock source divider selection for specified channel
 *
 * @param[in]  PWM            PWM group 
 *                            - PWMA : PWM Group A
 *                            - PWMB : PWM Group B 
 * @param[in]  u32Ch          PWM channel for PWM group A/B
 *                            - PWM_CH0
 *                            - PWM_CH1
 *                            - PWM_CH2
 *                            - PWM_CH3     
 * @retval     4              Input clock divided by 1
 * @retval     0              Input clock divided by 2
 * @retval     1              Input clock divided by 4
 * @retval     2              Input clock divided by 8
 * @retval     3              Input clock divided by 16    
 *
 * @details    This function is used to get timer clock source divider selection for specified channel
 *
 */
#define _PWM_GET_TIMER_CLOCK_DIV(PWM,u32Ch) (((PWM)->CSR&(PWM_CSR_CSR0_Msk<<((u32Ch)*4)))>>(PWM_CSR_CSR0_Pos+((u32Ch)*4)))




/**
 * @brief      Enable timer for specified channel
 *
 * @param[in]  PWM            PWM group 
 *                            - PWMA : PWM Group A
 *                            - PWMB : PWM Group B 
 * @param[in]  u32Ch          PWM channel for PWM group A/B
 *                            - PWM_CH0
 *                            - PWM_CH1
 *                            - PWM_CH2
 *                            - PWM_CH3      
 * @return     None
 *
 * @details    This function is used to enable timer for specified channel
 *
 */
#define _PWM_ENABLE_TIMER(PWM,u32Ch) (PWM)->PCR|= (PWM_PCR_CH0EN_Msk<<((u32Ch)*8))




/**
 * @brief      Disable timer for specified channel
 *
 * @param[in]  PWM            PWM group 
 *                            - PWMA : PWM Group A
 *                            - PWMB : PWM Group B 
 * @param[in]  u32Ch          PWM channel for PWM group A/B
 *                            - PWM_CH0
 *                            - PWM_CH1
 *                            - PWM_CH2
 *                            - PWM_CH3      
 * @return     None
 *
 * @details    This function is used to disable timer for specified channel
 *
 */
#define _PWM_DISABLE_TIMER(PWM,u32Ch) (PWM)->PCR&= ~(PWM_PCR_CH0EN_Msk<<((u32Ch)*8))




/**
 * @brief      Enable PWM output inveter for specified channel
 *
 * @param[in]  PWM            PWM group 
 *                            - PWMA : PWM Group A
 *                            - PWMB : PWM Group B 
 * @param[in]  u32Ch          PWM channel for PWM group A/B
 *                            - PWM_CH0
 *                            - PWM_CH1
 *                            - PWM_CH2
 *                            - PWM_CH3      
 * @return     None
 *
 * @details    This function is used to enable PWM output inveter for specified channel
 *
 */
#define _PWM_ENABLE_PWM_INV(PWM,u32Ch) (PWM)->PCR|= (PWM_PCR_CH0INV_Msk<<((u32Ch)*8))



/**
 * @brief      Disable PWM output inveter for specified channel
 *
 * @param[in]  PWM            PWM group 
 *                            - PWMA : PWM Group A
 *                            - PWMB : PWM Group B 
 * @param[in]  u32Ch          PWM channel for PWM group A/B
 *                            - PWM_CH0
 *                            - PWM_CH1
 *                            - PWM_CH2
 *                            - PWM_CH3      
 * @return     None
 *
 * @details    This function is used to disable PWM output inveter for specified channel
 *
 */
#define _PWM_DISABLE_PWM_INV(PWM,u32Ch) (PWM)->PCR&= ~(PWM_PCR_CH0INV_Msk<<((u32Ch)*8))




/**
 * @brief      Set timer auto-reolad mode for specified channel
 *
 * @param[in]  PWM            PWM group 
 *                            - PWMA : PWM Group A
 *                            - PWMB : PWM Group B 
 * @param[in]  u32Ch          PWM channel for PWM group A/B
 *                            - PWM_CH0
 *                            - PWM_CH1
 *                            - PWM_CH2
 *                            - PWM_CH3      
 * @return     None
 *
 * @details    This function is used to set timer auto-reolad mode for specified channel
 *
 * @note       If user changes timer mode from auto-reload to one-shot or from one-shot to auto-reload mode,\n
 *             it will cause CNR0 and CMR0 be clear.
 *
 */
#define _PWM_SET_TIMER_AUTO_RELOAD_MODE(PWM,u32Ch) (PWM)->PCR |= (PWM_PCR_CH0MOD_Msk<<((u32Ch)*8))




/**
 * @brief      Set timer One-shot mode for specified channel
 *
 * @param[in]  PWM            PWM group 
 *                            - PWMA : PWM Group A
 *                            - PWMB : PWM Group B 
 * @param[in]  u32Ch          PWM channel for PWM group A/B
 *                            - PWM_CH0
 *                            - PWM_CH1
 *                            - PWM_CH2
 *                            - PWM_CH3      
 * @return     None
 *
 * @details    This function is used to set timer One-shot mode for specified channel
 *
 * @note       - If user changes timer mode from auto-reload to one-shot or from one-shot to auto-reload mode,\n
 *             it will cause CNR0 and CMR0 be clear.
 *             - After PWM waveform generated once in PWM one-shot mode, CNR and CMR settings will be cleared and\n
 *               PWM-Timer will stop automatically.
 *               The following procedure is recommended for re-starting PWM single-shot waveform.
 *               Step 1: Setup comparator register (CMR) for setting PWM duty.
 *               Step 2: Setup PWM down-counter register (CNR) for setting PWM period. After setup CNR, PWM wave will be generated once again.
 *
 */
#define _PWM_SET_TIMER_ONE_SHOT_MODE(PWM,u32Ch) (PWM)->PCR &= ~(PWM_PCR_CH0MOD_Msk<<((u32Ch)*8))




/**
 * @brief      Enable PWM Dead-Zone generator for specified channel pair
 *
 * @param[in]  PWM            PWM group 
 *                            - PWMA : PWM Group A
 *                            - PWMB : PWM Group B 
 * @param[in]  u32Ch          PWM channel for PWM group A/B
 *                            - PWM_CH0
 *                            - PWM_CH2   
 * @return     None
 *
 * @details    This function is used to enable PWM Dead-Zone generator for specified channel pair
 *
 * @note       - When Channel 0 Dead-Zone Generator is enabled, channel 0 and channel 1 becomes a complementary pair.  
 *             - When Channel 2 Dead-Zone Generator is enabled, channel 2 and channel 3 becomes a complementary pair.  
 *
 */
#define _PWM_ENABLE_PWM_DEADZONE(PWM,u32Ch) (PWM)->PCR|=(PWM_PCR_DZEN01_Msk<<((u32Ch)/2))




/**
 * @brief      Disable PWM Dead-Zone generator for specified channel pair
 *
 * @param[in]  PWM            PWM group 
 *                            - PWMA : PWM Group A
 *                            - PWMB : PWM Group B 
 * @param[in]  u32Ch          PWM channel for PWM group A/B
 *                            - PWM_CH0
 *                            - PWM_CH1
 *                            - PWM_CH2
 *                            - PWM_CH3      
 * @return     None
 *
 * @details    This function is used to disable PWM Dead-Zone generator for specified channel pair
 *
 */
#define _PWM_DISABLE_PWM_DEADZONE(PWM,u32Ch) (PWM)->PCR&=~(PWM_PCR_DZEN01_Msk<<((u32Ch)/2))




/**
 * @brief      Set timer loaded value for specified channel
 *
 * @param[in]  PWM            PWM group 
 *                            - PWMA : PWM Group A
 *                            - PWMB : PWM Group B 
 * @param[in]  u32Ch          PWM channel for PWM group A/B
 *                            - PWM_CH0
 *                            - PWM_CH1
 *                            - PWM_CH2
 *                            - PWM_CH3  
 * @param[in]  u32LoadedVal   Timer loaded value(0~0xFFFF)
 *     
 * @return     None
 *
 * @details    This function is used to set timer loaded value(CNR) for specified channel.\n
 *             Loaded value determines the PWM period.
 *
 * @note       - If u32LoadedVal=0, PWM timer counter will stop.   
 *
 */
#define _PWM_SET_TIMER_LOADED_VALUE(PWM,u32Ch,u32LoadedVal)\
{\
     (*((__IO uint32_t *) (((uint32_t)&((PWM)->CNR0) + (u32Ch)*12))) = (u32LoadedVal));\
}



/**
 * @brief      Get timer loaded value for specified channel
 *
 * @param[in]  PWM            PWM group 
 *                            - PWMA : PWM Group A
 *                            - PWMB : PWM Group B 
 * @param[in]  u32Ch          PWM channel for PWM group A/B
 *                            - PWM_CH0
 *                            - PWM_CH1
 *                            - PWM_CH2
 *                            - PWM_CH3      
 * @return     Timer loaded value (0 - 0xFFFF)
 *
 * @details    This function is used to get timer loaded value for specified channel
 *
 */
#define _PWM_GET_TIMER_LOADED_VALUE(PWM,u32Ch) (*((__IO uint32_t *) (((uint32_t)&((PWM)->CNR0) + (u32Ch)*12))))




/**
 * @brief      Set PWM Comparator value for specified channel
 *
 * @param[in]  PWM            PWM group 
 *                            - PWMA : PWM Group A
 *                            - PWMB : PWM Group B 
 * @param[in]  u32Ch          PWM channel for PWM group A/B
 *                            - PWM_CH0
 *                            - PWM_CH1
 *                            - PWM_CH2
 *                            - PWM_CH3  
 * @param[in]  u32Compatator  PWM Comparator value (0 - 0xFFFF)
 *  
 * @return     None
 *
 * @details    This function is used to set PWM Comparator value for specified channel
 *
 */
#define _PWM_SET_PWM_COMP_VALUE(PWM,u32Ch,u32Compatator)\
{\
     (*((__IO uint32_t *) (((uint32_t)&((PWM)->CMR0) + (u32Ch)*12))) = (u32Compatator));\
}



/**
 * @brief      Get PWM Comparator value for specified channel
 *
 * @param[in]  PWM            PWM group 
 *                            - PWMA : PWM Group A
 *                            - PWMB : PWM Group B 
 * @param[in]  u32Ch          PWM channel for PWM group A/B
 *                            - PWM_CH0
 *                            - PWM_CH1
 *                            - PWM_CH2
 *                            - PWM_CH3      
 * @return     PWM Comparator value (0 - 0xFFFF)
 *
 * @details    This function is used to get PWM Comparator value for specified channel
 *
 */
#define _PWM_GET_PWM_COMP_VALUE(PWM,u32Ch) (*((__IO uint32_t *) (((uint32_t)&((PWM)->CMR0) + (u32Ch)*12))))




/**
 * @brief      Get timer current counter value for specified channel
 *
 * @param[in]  PWM            PWM group 
 *                            - PWMA : PWM Group A
 *                            - PWMB : PWM Group B 
 * @param[in]  u32Ch          PWM channel for PWM group A/B
 *                            - PWM_CH0
 *                            - PWM_CH1
 *                            - PWM_CH2
 *                            - PWM_CH3 
 * @return     Timer current counter value (0 - 0xFFFF)      
 *
 * @details    This function is used to get timer current counter value(PDR) for specified channel.\n
 *             User can monitor PDR to know the current value in 16-bit counter.
 *
 */
#define _PWM_GET_TIMER_CURRENT_COUNTER_VALUE(PWM,u32Ch) (*((__IO uint32_t *) (((uint32_t)&((PWM)->PDR0)) + (u32Ch)*12)))




/**
 * @brief      Enable timer period interrupt for specified channel
 *
 * @param[in]  PWM            PWM group 
 *                            - PWMA : PWM Group A
 *                            - PWMB : PWM Group B 
 * @param[in]  u32Ch          PWM channel for PWM group A/B
 *                            - PWM_CH0
 *                            - PWM_CH1
 *                            - PWM_CH2
 *                            - PWM_CH3      
 * @return     None
 *
 * @details    This function is used to enable timer period interrupt for specified channel
 *
 */
#define _PWM_ENABLE_TIMER_PERIOD_INT(PWM,u32Ch) ((PWM)->PIER|= (PWM_PIER_PWMIE0_Msk<<(u32Ch)))



/**
 * @brief       Disable timer period interrupt for specified channel
 *
 * @param[in]  PWM            PWM group 
 *                            - PWMA : PWM Group A
 *                            - PWMB : PWM Group B 
 * @param[in]  u32Ch          PWM channel for PWM group A/B
 *                            - PWM_CH0
 *                            - PWM_CH1
 *                            - PWM_CH2
 *                            - PWM_CH3      
 * @return     None
 *
 * @details    This function is used to disable timer period interrupt for specified channel
 *
 */
#define _PWM_DISABLE_TIMER_PERIOD_INT(PWM,u32Ch) ((PWM)->PIER&= ~(PWM_PIER_PWMIE0_Msk<<(u32Ch)))



/**
 * @brief       Get timer period interrupt flag for specified channel
 *
 * @param[in]  PWM            PWM group 
 *                            - PWMA : PWM Group A
 *                            - PWMB : PWM Group B 
 * @param[in]  u32Ch          PWM channel for PWM group A/B
 *                            - PWM_CH0
 *                            - PWM_CH1
 *                            - PWM_CH2
 *                            - PWM_CH3      
 * @retval      0             The specified channel timer interrupt flag is not set 
 * @retval      1             The specified channel timer interrupt flag is set
 *
 * @details    This function is used to get timer period interrupt flag for specified channel
 *
 */
#define _PWM_GET_TIMER_PERIOD_INT_FLAG(PWM,u32Ch) (((PWM)->PIIR&(PWM_PIIR_PWMIF0_Msk<<(u32Ch)))?1:0)


/**
 * @brief      Clear timer period interrupt flag for specified channel
 *
 * @param[in]  PWM            PWM group 
 *                            - PWMA : PWM Group A
 *                            - PWMB : PWM Group B 
 * @param[in]  u32Ch          PWM channel for PWM group A/B
 *                            - PWM_CH0
 *                            - PWM_CH1
 *                            - PWM_CH2
 *                            - PWM_CH3      
 * @return     None
 *
 * @details    This function is used to clear timer period interrupt flag for specified channel
 *
 */
#define _PWM_CLEAR_TIMER_PERIOD_INT_FLAG(PWM,u32Ch) ((PWM)->PIIR = (PWM_PIIR_PWMIF0_Msk<<(u32Ch)))




/**
 * @brief      Enable capture input inverter for specified channel
 *
 * @param[in]  PWM            PWM group 
 *                            - PWMA : PWM Group A
 *                            - PWMB : PWM Group B 
 * @param[in]  u32Ch          PWM channel for PWM group A/B
 *                            - PWM_CH0
 *                            - PWM_CH1
 *                            - PWM_CH2
 *                            - PWM_CH3      
 * @return     None
 *
 * @details    This function is used to enable capture input inverter for specified channel
 *
 *
 */
#define _PWM_ENABLE_CAP_INV(PWM,u32Ch)\
{\
    ((u32Ch)<2)?\
    ((PWM)->CCR0 = (PWM)->CCR0&PWM_CCR_MASK|(PWM_CCR0_INV0_Msk<<((u32Ch)*16))):\
    ((PWM)->CCR2 = (PWM)->CCR2&PWM_CCR_MASK|(PWM_CCR2_INV2_Msk<<(((u32Ch)%2)*16)));\
}




/**
 * @brief      Disable capture input inverter for specified channel
 *
 * @param[in]  PWM            PWM group 
 *                            - PWMA : PWM Group A
 *                            - PWMB : PWM Group B 
 * @param[in]  u32Ch          PWM channel for PWM group A/B
 *                            - PWM_CH0
 *                            - PWM_CH1
 *                            - PWM_CH2
 *                            - PWM_CH3      
 * @return     None
 *
 * @details    This function is used to disable capture input inverter for specified channel
 *
 *
 */
#define _PWM_DISABLE_CAP_INV(PWM,u32Ch)\
{\
    ((u32Ch)<2)?\
    ((PWM)->CCR0 = ((PWM)->CCR0&PWM_CCR_MASK)&~(PWM_CCR0_INV0_Msk<<((u32Ch)*16))):\
    ((PWM)->CCR2 = ((PWM)->CCR2&PWM_CCR_MASK)&~(PWM_CCR2_INV2_Msk<<(((u32Ch)%2)*16)));\
}




/**
 * @brief       Enable capture rising latch interrupt for specified channel
 *
 * @param[in]   PWM            PWM group 
 *                            - PWMA : PWM Group A
 *                            - PWMB : PWM Group B 
 * @param[in]   u32Ch          PWM channel for PWM group A/B
 *                            - PWM_CH0
 *                            - PWM_CH1
 *                            - PWM_CH2
 *                            - PWM_CH3      
 * @return      None
 *
 * @details     This function is used to enable capture rising latch interrupt for specified channel
 *
 *
 */
#define _PWM_ENABLE_CAP_RISING_INT(PWM,u32Ch)\
{\
    ((u32Ch)<2)?\
    ((PWM)->CCR0 = (PWM)->CCR0&PWM_CCR_MASK|(PWM_CCR0_CRL_IE0_Msk<<((u32Ch)*16))):\
    ((PWM)->CCR2 = (PWM)->CCR2&PWM_CCR_MASK|(PWM_CCR2_CRL_IE2_Msk<<(((u32Ch)%2)*16)));\
}




/**
 * @brief       Disable capture rising latch interrupt for specified channel
 *
 * @param[in]   PWM            PWM group 
 *                            - PWMA : PWM Group A
 *                            - PWMB : PWM Group B 
 * @param[in]   u32Ch          PWM channel for PWM group A/B
 *                            - PWM_CH0
 *                            - PWM_CH1
 *                            - PWM_CH2
 *                            - PWM_CH3      
 * @return      None
 *
 * @details     This function is used to disable capture rising latch interrupt for specified channel
 *
 *
 */
#define _PWM_DISABLE_CAP_RISING_INT(PWM,u32Ch)\
{\
    ((u32Ch)<2)?\
    ((PWM)->CCR0 = ((PWM)->CCR0&PWM_CCR_MASK)&~(PWM_CCR0_CRL_IE0_Msk<<((u32Ch)*16))):\
    ((PWM)->CCR2 = ((PWM)->CCR2&PWM_CCR_MASK)&~(PWM_CCR2_CRL_IE2_Msk<<(((u32Ch)%2)*16)));\
}




/**
 * @brief       Enable capture falling latch interrupt for specified channel
 *
 * @param[in]   PWM            PWM group 
 *                            - PWMA : PWM Group A
 *                            - PWMB : PWM Group B 
 * @param[in]   u32Ch          PWM channel for PWM group A/B
 *                            - PWM_CH0
 *                            - PWM_CH1
 *                            - PWM_CH2
 *                            - PWM_CH3      
 * @return      None
 *
 * @details     This function is used to enable capture falling latch interrupt for specified channel
 *
 *
 */
#define _PWM_ENABLE_CAP_FALLING_INT(PWM,u32Ch)\
{\
    ((u32Ch)<2)?\
    ((PWM)->CCR0 = (PWM)->CCR0&PWM_CCR_MASK|(PWM_CCR0_CFL_IE0_Msk<<((u32Ch)*16))):\
    ((PWM)->CCR2 = (PWM)->CCR2&PWM_CCR_MASK|(PWM_CCR2_CFL_IE2_Msk<<(((u32Ch)%2)*16)));\
}




/**
 * @brief       Disable capture falling latch interrupt for specified channel
 *
 * @param[in]   PWM            PWM group 
 *                            - PWMA : PWM Group A
 *                            - PWMB : PWM Group B 
 * @param[in]   u32Ch          PWM channel for PWM group A/B
 *                            - PWM_CH0
 *                            - PWM_CH1
 *                            - PWM_CH2
 *                            - PWM_CH3      
 * @return      None
 *
 * @details     This function is used to disable capture falling latch interrupt for specified channel
 *
 *
 */
#define _PWM_DISABLE_CAP_FALLING_INT(PWM,u32Ch)\
{\
    ((u32Ch)<2)?\
    ((PWM)->CCR0 = ((PWM)->CCR0&PWM_CCR_MASK)&~(PWM_CCR0_CFL_IE0_Msk<<((u32Ch)*16))):\
    ((PWM)->CCR2 = ((PWM)->CCR2&PWM_CCR_MASK)&~(PWM_CCR2_CFL_IE2_Msk<<(((u32Ch)%2)*16)));\
}


/**
 * @brief       Enable capture function for specified channel
 *
 * @param[in]   PWM            PWM group 
 *                             - PWMA : PWM Group A
 *                             - PWMB : PWM Group B 
 * @param[in]   u32Ch          PWM channel for PWM group A/B
 *                             - PWM_CH0
 *                             - PWM_CH1
 *                             - PWM_CH2
 *                             - PWM_CH3      
 * @return      None
 *
 * @details     This function is used to enable capture function for specified channel
 *
 * @note        When capture function enabled, Capture latched the PWM-counter value and saved to CRLR (Rising latch) and CFLR (Falling latch). 
 *
 */
#define _PWM_ENABLE_CAP_FUNC(PWM,u32Ch)\
{\
    ((u32Ch)<2)?\
    ((PWM)->CCR0 = (PWM)->CCR0&PWM_CCR_MASK|(PWM_CCR0_CAPCH0EN_Msk<<((u32Ch)*16))):\
    ((PWM)->CCR2 = (PWM)->CCR2&PWM_CCR_MASK|(PWM_CCR2_CAPCH2EN_Msk<<(((u32Ch)%2)*16)));\
}




/**
 * @brief       Disable capture function for specified channel
 *
 * @param[in]   PWM            PWM group 
 *                             - PWMA : PWM Group A
 *                             - PWMB : PWM Group B 
 * @param[in]   u32Ch          PWM channel for PWM group A/B
 *                             - PWM_CH0
 *                             - PWM_CH1
 *                             - PWM_CH2
 *                             - PWM_CH3      
 * @return      None
 *
 * @details     This function is used to disable capture function for specified channel
 *
 * @note        When capture function disabled, Capture does not update CRLR and CFLR, and disable specified channel Interrupt.
 *
 */
#define _PWM_DISABLE_CAP_FUNC(PWM,u32Ch)\
{\
    ((u32Ch)<2)?\
    ((PWM)->CCR0 = ((PWM)->CCR0&PWM_CCR_MASK)&~(PWM_CCR0_CAPCH0EN_Msk<<((u32Ch)*16))):\
    ((PWM)->CCR2 = ((PWM)->CCR2&PWM_CCR_MASK)&~(PWM_CCR2_CAPCH2EN_Msk<<(((u32Ch)%2)*16)));\
}




/**
 * @brief       Get capture interrupt flag for specified channel
 *
 * @param[in]   PWM            PWM group 
 *                             - PWMA : PWM Group A
 *                             - PWMB : PWM Group B 
 * @param[in]   u32Ch          PWM channel for PWM group A/B
 *                             - PWM_CH0
 *                             - PWM_CH1
 *                             - PWM_CH2
 *                             - PWM_CH3      
 * @retval      0             The specified channel capture interrupt flag is not set 
 * @retval      1             The specified channel capture interrupt flag is set
 *
 * @details     This function is used to get capture interrupt flag for specified channel
 *
 *
 */
#define _PWM_GET_CAP_INT_FLAG(PWM,u32Ch) ((u32Ch)<2)?\
        (((PWM)->CCR0&(PWM_CCR0_CAPIF0_Msk<<((u32Ch)*16)))?1:0):\
        (((PWM)->CCR2&(PWM_CCR2_CAPIF2_Msk<<(((u32Ch)%2)*16)))?1:0)

/**
 * @brief      Clear capture interrupt flag for specified channel
 *
 * @param[in]  PWM            PWM group 
 *                            - PWMA : PWM Group A
 *                            - PWMB : PWM Group B 
 * @param[in]  u32Ch          PWM channel for PWM group A/B
 *                            - PWM_CH0
 *                            - PWM_CH1
 *                            - PWM_CH2
 *                            - PWM_CH3      
 * @return     None
 *
 * @details    This function is used to clear capture interrupt flag for specified channel
 *
 * @note       Write 1 to clear this bit
 *
 */
#define _PWM_CLR_CAP_INT_FLAG(PWM,u32Ch)\
{\
    ((u32Ch)<2)?\
    ((PWM)->CCR0 = (PWM)->CCR0&PWM_CCR_MASK|(PWM_CCR0_CAPIF0_Msk<<((u32Ch)*16))):\
    ((PWM)->CCR2 = (PWM)->CCR2&PWM_CCR_MASK|(PWM_CCR2_CAPIF2_Msk<<(((u32Ch)%2)*16)));\
}

/**
 * @brief       Get capture rising latched indicator for specified channel
 *
 * @param[in]   PWM            PWM group 
 *                            - PWMA : PWM Group A
 *                            - PWMB : PWM Group B 
 * @param[in]   u32Ch          PWM channel for PWM group A/B
 *                            - PWM_CH0
 *                            - PWM_CH1
 *                            - PWM_CH2
 *                            - PWM_CH3      
 * @retval      0             The specified channel capture falling rising indicator is not set 
 * @retval      1             The specified channel capture falling rising indicator is set
 *
 * @details     This function is used to get capture rising latched indicator for specified channel
 *
 *
 */
#define _PWM_GET_CAP_RISING_INDICATOR(PWM,u32Ch) ((u32Ch)<2)?\
    (((PWM)->CCR0&(PWM_CCR0_CRLRI0_Msk<<((u32Ch)*16)))?1:0):\
    (((PWM)->CCR2&(PWM_CCR2_CRLRI2_Msk<<(((u32Ch)%2)*16)))?1:0)\




/**
 * @brief      Clear capture rising latched indicator for specified channel
 *
 * @param[in]  PWM            PWM group 
 *                            - PWMA : PWM Group A
 *                            - PWMB : PWM Group B 
 * @param[in]  u32Ch          PWM channel for PWM group A/B
 *                            - PWM_CH0
 *                            - PWM_CH1
 *                            - PWM_CH2
 *                            - PWM_CH3      
 * @return      None
 *
 * @details     This function is used to clear capture rising latched indicator for specified channel
 *
 *
 */
#define _PWM_CLR_CAP_RISING_INDICATOR(PWM,u32Ch)\
{\
    ((u32Ch)<2)?\
    ((PWM)->CCR0 = (PWM)->CCR0&PWM_CCR_MASK|(PWM_CCR0_CRLRI0_Msk<<((u32Ch)*16))):\
    ((PWM)->CCR2 = (PWM)->CCR2&PWM_CCR_MASK|(PWM_CCR2_CRLRI2_Msk<<(((u32Ch)%2)*16)));\
}
        



/**
 * @brief      Get capture falling latched indicator for specified channel
 *
 * @param[in]  PWM            PWM group 
 *                            - PWMA : PWM Group A
 *                            - PWMB : PWM Group B 
 * @param[in]  u32Ch          PWM channel for PWM group A/B
 *                            - PWM_CH0
 *                            - PWM_CH1
 *                            - PWM_CH2
 *                            - PWM_CH3    
 * @retval      0        The specified channel capture falling latched indicator is not set 
 * @retval      1        The specified channel capture falling latched indicator is set
 *
 * @details     This function is used to get capture falling latched indicator for specified channel
 *
 */
#define _PWM_GET_CAP_FALLING_INDICATOR(PWM,u32Ch) ((u32Ch)<2)?\
    (((PWM)->CCR0&(PWM_CCR0_CFLRI0_Msk<<((u32Ch)*16)))?1:0):\
    (((PWM)->CCR2&(PWM_CCR2_CFLRI2_Msk<<(((u32Ch)%2)*16)))?1:0)




/**
 * @brief       Clear capture falling latched indicator for specified channel
 *
 * @param[in]   PWM            PWM group 
 *                             - PWMA : PWM Group A
 *                             - PWMB : PWM Group B 
 * @param[in]   u32Ch          PWM channel for PWM group A/B
 *                             - PWM_CH0
 *                             - PWM_CH1
 *                             - PWM_CH2
 *                             - PWM_CH3      
 * @return      None
 *
 * @details     This function is used to clear capture falling latched indicator for specified channel
 *
 *
 */
#define _PWM_CLR_CAP_FALLING_INDICATOR(PWM,u32Ch)\
{\
    ((u32Ch)<2)?\
    ((PWM)->CCR0 = (PWM)->CCR0&PWM_CCR_MASK|(PWM_CCR0_CFLRI0_Msk<<((u32Ch)*16))):\
    ((PWM)->CCR2 = (PWM)->CCR2&PWM_CCR_MASK|(PWM_CCR2_CFLRI2_Msk<<(((u32Ch)%2)*16)));\
}




/**
 * @brief       Get capture rising latch register value for specified channel
 *
 * @param[in]   PWM            PWM group 
 *                             - PWMA : PWM Group A
 *                             - PWMB : PWM Group B 
 * @param[in]   u32Ch          PWM channel for PWM group A/B
 *                             - PWM_CH0
 *                             - PWM_CH1
 *                             - PWM_CH2
 *                             - PWM_CH3      
 * @return      Capture rising latch register value 
 *
 * @details     This function is used to get capture rising latch register value for specified channel
 *
 *
 */
#define _PWM_GET_CAP_RISING_LATCH_VALUE(PWM,u32Ch) (*((__IO uint32_t *) (((uint32_t)&((PWM)->CRLR0) + (u32Ch)*8))))




/**
 * @brief      Get capture falling latch register value for specified channel
 *
 * @param[in]  PWM            PWM group 
 *                            - PWMA : PWM Group A
 *                            - PWMB : PWM Group B 
 * @param[in]  u32Ch          PWM channel for PWM group A/B
 *                            - PWM_CH0
 *                            - PWM_CH1
 *                            - PWM_CH2
 *                            - PWM_CH3      
 * @return     Capture falling latch register value 
 *
 * @details    This function is used to get capture falling latch register value for specified channel
 *
 *
 */
#define _PWM_GET_CAP_FALLING_LATCH_VALUE(PWM,u32Ch) (*((__IO uint32_t *) (((uint32_t)&((PWM)->CFLR0) + (u32Ch)*8))))




/**
 * @brief      Enable capture input path for specified channel
 *
 * @param[in]  PWM            PWM group 
 *                            - PWMA : PWM Group A
 *                            - PWMB : PWM Group B 
 * @param[in]  u32Ch          PWM channel for PWM group A/B
 *                            - PWM_CH0
 *                            - PWM_CH1
 *                            - PWM_CH2
 *                            - PWM_CH3      
 * @return     None
 *
 * @details    This function is used to enable capture input path for specified channel
 *
 *
 */
#define _PWM_ENABLE_CAP_IN(PWM,u32Ch) ((PWM)->CAPENR |= (PWM_CAPENR_CINEN0_Msk<<(u32Ch)))



/**
 * @brief       Disable capture input path for specified channel
 *
 * @param[in]   PWM            PWM group 
 *                             - PWMA : PWM Group A
 *                             - PWMB : PWM Group B 
 * @param[in]   u32Ch          PWM channel for PWM group A/B
 *                             - PWM_CH0
 *                             - PWM_CH1
 *                             - PWM_CH2
 *                             - PWM_CH3      
 * @return      None
 *
 * @details     This function is used to disable capture input path for specified channel
 *
 *
 */
#define _PWM_DISABLE_CAP_IN(PWM,u32Ch) ((PWM)->CAPENR&= ~(PWM_CAPENR_CINEN0_Msk<<(u32Ch)))




/**
 * @brief       Enable PWM output path for specified channel
 *
 * @param[in]   PWM            PWM group 
 *                             - PWMA : PWM Group A
 *                             - PWMB : PWM Group B 
 * @param[in]   u32Ch          PWM channel for PWM group A/B
 *                             - PWM_CH0
 *                             - PWM_CH1
 *                             - PWM_CH2
 *                             - PWM_CH3      
 * @return      None
 *
 * @details     This function is used to enable PWM output path for specified channel
 *
 *
 */
#define _PWM_ENABLE_PWM_OUT(PWM,u32Ch) ((PWM)->POE |= (PWM_POE_PWM0_Msk<<(u32Ch)))




/**
 * @brief      Disable PWM output path for specified channel
 *
 * @param[in]  PWM            PWM group 
 *                            - PWMA : PWM Group A
 *                            - PWMB : PWM Group B 
 * @param[in]  u32Ch          PWM channel for PWM group A/B
 *                            - PWM_CH0
 *                            - PWM_CH1
 *                            - PWM_CH2
 *                            - PWM_CH3      
 * @return     None
 *
 * @details    This function is used to disable PWM output path for specified channel.
 *
 *
 */
#define _PWM_DISABLE_PWM_OUT(PWM,u32Ch) ((PWM)->POE&= ~(PWM_POE_PWM0_Msk<<(u32Ch)))





/**
 * @brief      Reset PWM controller for specified group
 *
 * @param[in]  PWM            PWM group 
 *                            - PWMA : PWM Group A
 *                            - PWMB : PWM Group B     
 * @return     None
 *                                                           
 * @details    The function is used to reset PWM controller for specified group
 *
 * @note       PWM channel 0~3 in each group share the same PWM reset controller.
 */ 
__INLINE void PWM_ResetPwmController(PWM_T* PWM)
{
    SYS->IPRSTC2 |= (SYS_IPRSTC2_PWM03_RST_Msk<<(((PWM)==PWMA)?0:1));
    SYS->IPRSTC2 &= ~((SYS_IPRSTC2_PWM03_RST_Msk<<(((PWM)==PWMA)?0:1)));
}  




/**
 * @brief      Get PWM Clock Source Frequency
 *
 * @param[in]  PWM            PWM group 
 *                            - PWMA : PWM Group A
 *                            - PWMB : PWM Group B 
 * @param[in]  u32Ch          PWM channel for PWM group A/B
 *                            - PWM_CH0
 *                            - PWM_CH1
 *                            - PWM_CH2
 *                            - PWM_CH3  
 * @return     PWM clock source frequency
 * @details    Get the PWM clock source frequency for specified channel.
 *
 * @note       - PWM channel 0 and channel 1 share the same clock source setting. 
 *             - PWM channel 2 and channel 3 share the same clock source setting. 
 */
__INLINE uint32_t PWM_GetTimerClockSource(PWM_T* PWM, uint32_t u32Ch)
{
    uint8_t     u8EngineClk;
    uint32_t u32ClkTbl[4] = {__XTAL, 0, 0, __IRC22M};
    
    u32ClkTbl[2] = SystemCoreClock;        
    u8EngineClk = ((PWM)==PWMA)?((SYSCLK->CLKSEL1&(SYSCLK_CLKSEL1_PWM01_S_Msk<<(((u32Ch)/2)*2)))
                               >>(SYSCLK_CLKSEL1_PWM01_S_Pos+((u32Ch)/2)*2)):
                              ((SYSCLK->CLKSEL2&(SYSCLK_CLKSEL2_PWM45_S_Msk<<(((u32Ch)/2)*2)))
                               >>(SYSCLK_CLKSEL2_PWM45_S_Pos+((u32Ch)/2)*2));                                             
    return u32ClkTbl[u8EngineClk];    
}


/**
  * @} End of PWM Device Function Interface
  */ 

/**
  * @} End of M051 Function Interface
  */ 


#endif	//__PWM_H__



