/******************************************************************************

   This file defines the registers header with M-Query style for
     M05X_REGISTERS
   Created by ed_reg.hta (Sat May 14 21:24:55 UTC+0800 2011)

   Discussion and usage:
     http://www.ourdev.cn/bbs/bbs_content.jsp?bbs_sn=4711957
     http://bbs.21ic.com/icview-233164-1-1.html#pid1644710

   Copyright (C) 2011 by xhawk18_AT_GMAIL_DOT_COM

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.

*******************************************************************************/

#ifndef INC_OPT_M05X_REGISTERS_H__
#define INC_OPT_M05X_REGISTERS_H__

#if defined (__CC_ARM)
#pragma anon_unions
#endif

#ifdef __cplusplus
extern "C" {
#endif
/* Type of register integer */
typedef unsigned int           _reg32_t;
/* REG_COMMIT(reg) explicitly writes value to register whenever compiling
 * in optimized mode or not.
 */
#define REG_COMMIT(reg)                                                  \
    do{*(volatile _reg32_t *)&(reg) = *(_reg32_t *)&(reg);} while(0)

#ifdef __cplusplus
}
#endif

#ifndef __cplusplus
#define REG_CPP_OVERRIDE(reg)  /* Do not override in C code. */
#else
/* REG_MASKBIT(begin,end) returns an integer that has mask bits with 1 in the
 * range of [begin,end). It has the same value as
    #define REG_MASKBIT(begin,end) (1UL << end) - (1UL << begin)
   Use the complex definition to avoid some warning of compiler. */
#define REG_MASKBIT(begin,end)                                           \
    (((end) < 32 ? (1UL << ((end) < 32 ? (end) : 0)) : 0UL)              \
    - ((begin) >= 32 ? 0UL : (1UL << (begin))))

/* REG_CPP_OVERRIDE(reg) defines some override functions in C++ code. */
#define REG_CPP_OVERRIDE(reg)                                            \
  /* Override _reg32_t so that register can be read as an integer. */    \
  inline operator _reg32_t() const                                       \
  {                                                                      \
    return v;                                                            \
  }                                                                      \
  /* Override _reg32_t so that register can be read as an integer. */    \
  inline operator _reg32_t() const volatile                              \
  {                                                                      \
    return v;                                                            \
  }                                                                      \
  /* Override = so that integer can be assigned to the register. */      \
  inline void operator=(_reg32_t register_value)                         \
  {                                                                      \
    v = register_value;                                                  \
  }                                                                      \
  /* Override = so that integer can be assigned to the register. */      \
  inline void operator=(_reg32_t register_value) volatile                \
  {                                                                      \
    v = register_value;                                                  \
  }                                                                      \
  /* Override () for next writing as bit-field. */                       \
  inline bits_##reg<volatile union reg##_t_> operator()() volatile       \
  {                                                                      \
    return bits_##reg<volatile union reg##_t_>(this, v);                 \
  }                                                                      \
  /* Override () for next writing as bit-field. */                       \
  inline bits_##reg<union reg##_t_> operator()()                         \
  {                                                                      \
    return bits_##reg<union reg##_t_>(this, v);                          \
  }                                                                      \
  /* Override () with an initial value for next writing as bit-field. */ \
  inline bits_##reg<volatile union reg##_t_>                             \
  operator()(_reg32_t register_value) volatile                           \
  {                                                                      \
    return bits_##reg<volatile union reg##_t_>(this, register_value);    \
  }                                                                      \
  /* Override () with an initial value for next writing as bit-field. */ \
  inline bits_##reg<union reg##_t_>                                      \
  operator()(_reg32_t register_value)                                    \
  {                                                                      \
    return bits_##reg<union reg##_t_>(this, register_value);             \
  }

/* REG_BITS_COMMON(reg) defines common elements in bit-field class. */
#define REG_BITS_COMMON(reg)                                             \
  /* Write the value to register in contruction. */                      \
  inline ~bits_##reg() {w &= ~m; w |= v; o->v = w;}                      \
private:                                                                 \
  friend union reg##_t_;                                                 \
  inline bits_##reg(T *o_, _reg32_t v_): w(v_), m(0), o(o_), v(0) {}     \
  _reg32_t w;                                                            \
  _reg32_t m;                                                            \
  T *o;                                                                  \
  _reg32_t v;

/* REG_BITS_DEF(reg) defines a writing function for a bit-fields. */
#define REG_BITS_DEF(begin,end,reg,bit)                                  \
  inline bits_##reg &bit(_reg32_t bits_value_##begin##_##end)            \
  {                                                                      \
    /* Write bits value to internal varibale,                */          \
    /* and do not write to the real register for the moment. */          \
    m |= REG_MASKBIT(begin, end);                                        \
    v &= ~REG_MASKBIT(begin, end);                                       \
    v |= (bits_value_##begin##_##end << begin);                          \
    return *this;                                                        \
  }

/* class for bit field: A/D Data Register 0 */
template<class T>
struct bits_adc_addr
{
  REG_BITS_DEF(0, 12, adc_addr, RSLT)
  REG_BITS_DEF(16, 17, adc_addr, OVERRUN)
  REG_BITS_DEF(17, 18, adc_addr, VALID)
  REG_BITS_COMMON(adc_addr)
};

/* class for bit field: A/D Control Register */
template<class T>
struct bits_adc_adcr
{
  REG_BITS_DEF(0, 1, adc_adcr, ADEN)
  REG_BITS_DEF(1, 2, adc_adcr, ADIE)
  REG_BITS_DEF(2, 4, adc_adcr, ADMD)
  REG_BITS_DEF(4, 6, adc_adcr, TRGS)
  REG_BITS_DEF(6, 8, adc_adcr, TRGCOND)
  REG_BITS_DEF(8, 9, adc_adcr, TRGE)
  REG_BITS_DEF(10, 11, adc_adcr, DIFFEN)
  REG_BITS_DEF(11, 12, adc_adcr, ADST)
  REG_BITS_COMMON(adc_adcr)
};

/* class for bit field: A/D Channel Enable Register */
template<class T>
struct bits_adc_adcher
{
  REG_BITS_DEF(0, 1, adc_adcher, CHEN0)
  REG_BITS_DEF(1, 2, adc_adcher, CHEN1)
  REG_BITS_DEF(2, 3, adc_adcher, CHEN2)
  REG_BITS_DEF(3, 4, adc_adcher, CHEN3)
  REG_BITS_DEF(4, 5, adc_adcher, CHEN4)
  REG_BITS_DEF(5, 6, adc_adcher, CHEN5)
  REG_BITS_DEF(6, 7, adc_adcher, CHEN6)
  REG_BITS_DEF(7, 8, adc_adcher, CHEN7)
  REG_BITS_DEF(8, 10, adc_adcher, PRESEL)
  REG_BITS_COMMON(adc_adcher)
};

/* class for bit field: A/D Compare Register 0 */
template<class T>
struct bits_adc_adcmpr
{
  REG_BITS_DEF(0, 1, adc_adcmpr, CMPEN)
  REG_BITS_DEF(1, 2, adc_adcmpr, CMPIE)
  REG_BITS_DEF(2, 3, adc_adcmpr, CMPCOND)
  REG_BITS_DEF(3, 6, adc_adcmpr, CMPCH)
  REG_BITS_DEF(8, 12, adc_adcmpr, CMPMATCNT)
  REG_BITS_DEF(16, 28, adc_adcmpr, CMPD)
  REG_BITS_COMMON(adc_adcmpr)
};

/* class for bit field: A/D Status Register */
template<class T>
struct bits_adc_adsr
{
  REG_BITS_DEF(0, 1, adc_adsr, ADF)
  REG_BITS_DEF(1, 2, adc_adsr, CMPF0)
  REG_BITS_DEF(2, 3, adc_adsr, CMPF1)
  REG_BITS_DEF(3, 4, adc_adsr, BUSY)
  REG_BITS_DEF(4, 7, adc_adsr, CHANNEL)
  REG_BITS_DEF(8, 16, adc_adsr, VALID)
  REG_BITS_DEF(16, 24, adc_adsr, OVERRUN)
  REG_BITS_COMMON(adc_adsr)
};

/* class for bit field: A/D Calibration Register */
template<class T>
struct bits_adc_adcalr
{
  REG_BITS_DEF(0, 1, adc_adcalr, CALEN)
  REG_BITS_DEF(1, 2, adc_adcalr, CALDONE)
  REG_BITS_COMMON(adc_adcalr)
};

/* class for bit field: System Power Down Control Register */
template<class T>
struct bits_clk_pwrcon
{
  REG_BITS_DEF(0, 1, clk_pwrcon, XTL12M_EN)
  REG_BITS_DEF(2, 3, clk_pwrcon, OSC22M_EN)
  REG_BITS_DEF(3, 4, clk_pwrcon, OSC10K_EN)
  REG_BITS_DEF(4, 5, clk_pwrcon, PD_WU_DLY)
  REG_BITS_DEF(5, 6, clk_pwrcon, PD_WU_INT_EN)
  REG_BITS_DEF(6, 7, clk_pwrcon, PD_WU_STS)
  REG_BITS_DEF(7, 8, clk_pwrcon, PWR_DOWN_EN)
  REG_BITS_DEF(8, 9, clk_pwrcon, PD_WAIT_CPU)
  REG_BITS_COMMON(clk_pwrcon)
};

/* class for bit field: AHB Devices Clock Enable Control Register */
template<class T>
struct bits_clk_ahbclk
{
  REG_BITS_DEF(2, 3, clk_ahbclk, ISP_EN)
  REG_BITS_DEF(3, 4, clk_ahbclk, EBI_EN)
  REG_BITS_COMMON(clk_ahbclk)
};

/* class for bit field: APB Devices Clock Enable Control Register */
template<class T>
struct bits_clk_apbclk
{
  REG_BITS_DEF(0, 1, clk_apbclk, WDT_EN)
  REG_BITS_DEF(2, 3, clk_apbclk, TMR0_EN)
  REG_BITS_DEF(3, 4, clk_apbclk, TMR1_EN)
  REG_BITS_DEF(4, 5, clk_apbclk, TMR2_EN)
  REG_BITS_DEF(5, 6, clk_apbclk, TMR3_EN)
  REG_BITS_DEF(6, 7, clk_apbclk, FDIV_EN)
  REG_BITS_DEF(8, 9, clk_apbclk, I2C_EN)
  REG_BITS_DEF(12, 13, clk_apbclk, SPI0_EN)
  REG_BITS_DEF(13, 14, clk_apbclk, SPI1_EN)
  REG_BITS_DEF(16, 17, clk_apbclk, UART0_EN)
  REG_BITS_DEF(17, 18, clk_apbclk, UART1_EN)
  REG_BITS_DEF(20, 21, clk_apbclk, PWM01_EN)
  REG_BITS_DEF(21, 22, clk_apbclk, PWM23_EN)
  REG_BITS_DEF(22, 23, clk_apbclk, PWM45_EN)
  REG_BITS_DEF(23, 24, clk_apbclk, PWM67_EN)
  REG_BITS_DEF(28, 29, clk_apbclk, ADC_EN)
  REG_BITS_COMMON(clk_apbclk)
};

/* class for bit field: Clock status monitor Register */
template<class T>
struct bits_clk_clkstatus
{
  REG_BITS_DEF(0, 1, clk_clkstatus, XTL12M_STB)
  REG_BITS_DEF(2, 3, clk_clkstatus, PLL_STB)
  REG_BITS_DEF(3, 4, clk_clkstatus, OSC10K_STB)
  REG_BITS_DEF(4, 5, clk_clkstatus, OSC22M_STB)
  REG_BITS_DEF(7, 8, clk_clkstatus, CLK_SW_FAIL)
  REG_BITS_COMMON(clk_clkstatus)
};

/* class for bit field: Clock Source Select Control Register 0 */
template<class T>
struct bits_clk_clksel0
{
  REG_BITS_DEF(0, 3, clk_clksel0, HCLK_S)
  REG_BITS_DEF(3, 6, clk_clksel0, STCLK_S)
  REG_BITS_COMMON(clk_clksel0)
};

/* class for bit field: Clock Source Select Control Register 1 */
template<class T>
struct bits_clk_clksel1
{
  REG_BITS_DEF(0, 2, clk_clksel1, WDT_S)
  REG_BITS_DEF(2, 4, clk_clksel1, ADC_S)
  REG_BITS_DEF(8, 11, clk_clksel1, TMR0_S)
  REG_BITS_DEF(12, 15, clk_clksel1, TMR1_S)
  REG_BITS_DEF(16, 19, clk_clksel1, TMR2_S)
  REG_BITS_DEF(20, 23, clk_clksel1, TMR3_S)
  REG_BITS_DEF(24, 26, clk_clksel1, UART_S)
  REG_BITS_DEF(28, 30, clk_clksel1, PWM01_S)
  REG_BITS_DEF(30, 32, clk_clksel1, PWM23_S)
  REG_BITS_COMMON(clk_clksel1)
};

/* class for bit field: Clock Divider Number Register */
template<class T>
struct bits_clk_clkdiv
{
  REG_BITS_DEF(0, 4, clk_clkdiv, HCLK_N)
  REG_BITS_DEF(8, 12, clk_clkdiv, UART_N)
  REG_BITS_DEF(16, 24, clk_clkdiv, ADC_N)
  REG_BITS_COMMON(clk_clkdiv)
};

/* class for bit field: Clock Source Select Control Register 2 */
template<class T>
struct bits_clk_clksel2
{
  REG_BITS_DEF(2, 4, clk_clksel2, FRQDIV_S)
  REG_BITS_DEF(4, 6, clk_clksel2, PWM45_S)
  REG_BITS_DEF(6, 8, clk_clksel2, PWM67_S)
  REG_BITS_COMMON(clk_clksel2)
};

/* class for bit field: PLL Control Register */
template<class T>
struct bits_clk_pllcon
{
  REG_BITS_DEF(0, 9, clk_pllcon, FB_DV)
  REG_BITS_DEF(9, 14, clk_pllcon, IN_DV)
  REG_BITS_DEF(14, 16, clk_pllcon, OUT_DV)
  REG_BITS_DEF(16, 17, clk_pllcon, PD)
  REG_BITS_DEF(17, 18, clk_pllcon, BP)
  REG_BITS_DEF(18, 19, clk_pllcon, OE)
  REG_BITS_DEF(19, 20, clk_pllcon, PLL_SRC)
  REG_BITS_COMMON(clk_pllcon)
};

/* class for bit field: Frequency Divider Control Register */
template<class T>
struct bits_clk_frqdiv
{
  REG_BITS_DEF(0, 4, clk_frqdiv, FSEL)
  REG_BITS_DEF(4, 5, clk_frqdiv, DIVIDER_EN)
  REG_BITS_COMMON(clk_frqdiv)
};

/* class for bit field: External Bus Interface General Control Register */
template<class T>
struct bits_ebi_ctl_ebicon
{
  REG_BITS_DEF(0, 1, ebi_ctl_ebicon, ExtEN)
  REG_BITS_DEF(1, 2, ebi_ctl_ebicon, ExtBW16)
  REG_BITS_DEF(8, 11, ebi_ctl_ebicon, MCLKDIV)
  REG_BITS_DEF(16, 19, ebi_ctl_ebicon, ExttALE)
  REG_BITS_COMMON(ebi_ctl_ebicon)
};

/* class for bit field: External Bus Interface 0 Timing Control Register */
template<class T>
struct bits_ebi_ctl_extime
{
  REG_BITS_DEF(3, 8, ebi_ctl_extime, ExttACC)
  REG_BITS_DEF(8, 11, ebi_ctl_extime, ExttAHD)
  REG_BITS_DEF(12, 16, ebi_ctl_extime, ExtIW2X)
  REG_BITS_DEF(24, 28, ebi_ctl_extime, ExtIR2R)
  REG_BITS_COMMON(ebi_ctl_extime)
};

/* class for bit field: ISP Control Register */
template<class T>
struct bits_fmc_ispcon
{
  REG_BITS_DEF(0, 1, fmc_ispcon, ISPEN)
  REG_BITS_DEF(1, 2, fmc_ispcon, BS)
  REG_BITS_DEF(4, 5, fmc_ispcon, CFGUEN)
  REG_BITS_DEF(5, 6, fmc_ispcon, LDUEN)
  REG_BITS_DEF(6, 7, fmc_ispcon, ISPFF)
  REG_BITS_DEF(7, 8, fmc_ispcon, SWRST)
  REG_BITS_DEF(8, 11, fmc_ispcon, PT)
  REG_BITS_DEF(12, 15, fmc_ispcon, ET)
  REG_BITS_COMMON(fmc_ispcon)
};

/* class for bit field: ISP Command Register */
template<class T>
struct bits_fmc_ispcmd
{
  REG_BITS_DEF(0, 6, fmc_ispcmd, FOEN_FCEN_FCTRL)
  REG_BITS_COMMON(fmc_ispcmd)
};

/* class for bit field: ISP Trigger Control Register */
template<class T>
struct bits_fmc_isptrg
{
  REG_BITS_DEF(0, 1, fmc_isptrg, ISPGO)
  REG_BITS_COMMON(fmc_isptrg)
};

/* class for bit field: Flash Access Time Control Register */
template<class T>
struct bits_fmc_fatcon
{
  REG_BITS_DEF(0, 1, fmc_fatcon, FPSEN)
  REG_BITS_DEF(1, 4, fmc_fatcon, FATS)
  REG_BITS_DEF(4, 5, fmc_fatcon, L_SPEED)
  REG_BITS_COMMON(fmc_fatcon)
};

/* class for bit field: System Reset Source Register */
template<class T>
struct bits_gcr_rstsrc
{
  REG_BITS_DEF(0, 1, gcr_rstsrc, RSTS_POR)
  REG_BITS_DEF(1, 2, gcr_rstsrc, RSTS_RESET)
  REG_BITS_DEF(2, 3, gcr_rstsrc, RSTS_WDT)
  REG_BITS_DEF(3, 4, gcr_rstsrc, RSTS_LVR)
  REG_BITS_DEF(4, 5, gcr_rstsrc, RSTS_BOD)
  REG_BITS_DEF(5, 6, gcr_rstsrc, RSTS_MCU)
  REG_BITS_DEF(7, 8, gcr_rstsrc, RSTS_CPU)
  REG_BITS_COMMON(gcr_rstsrc)
};

/* class for bit field: IP Reset Control Resister1 */
template<class T>
struct bits_gcr_iprstc1
{
  REG_BITS_DEF(0, 1, gcr_iprstc1, CHIP_RST)
  REG_BITS_DEF(1, 2, gcr_iprstc1, CPU_RST)
  REG_BITS_DEF(3, 4, gcr_iprstc1, EBI_RST)
  REG_BITS_COMMON(gcr_iprstc1)
};

/* class for bit field: IP Reset Control Resister 2 */
template<class T>
struct bits_gcr_iprstc2
{
  REG_BITS_DEF(1, 2, gcr_iprstc2, GPIO_RST)
  REG_BITS_DEF(2, 3, gcr_iprstc2, TMR0_RST)
  REG_BITS_DEF(3, 4, gcr_iprstc2, TMR1_RST)
  REG_BITS_DEF(4, 5, gcr_iprstc2, TMR2_RST)
  REG_BITS_DEF(5, 6, gcr_iprstc2, TMR3_RST)
  REG_BITS_DEF(8, 9, gcr_iprstc2, I2C_RST)
  REG_BITS_DEF(12, 13, gcr_iprstc2, SPI0_RST)
  REG_BITS_DEF(13, 14, gcr_iprstc2, SPI1_RST)
  REG_BITS_DEF(16, 17, gcr_iprstc2, UART0_RST)
  REG_BITS_DEF(17, 18, gcr_iprstc2, UART1_RST)
  REG_BITS_DEF(20, 21, gcr_iprstc2, PWM03_RST)
  REG_BITS_DEF(21, 22, gcr_iprstc2, PWM47_RST)
  REG_BITS_DEF(28, 29, gcr_iprstc2, ADC_RST)
  REG_BITS_COMMON(gcr_iprstc2)
};

/* class for bit field: Brown Out Detector Control Register */
template<class T>
struct bits_gcr_bodcr
{
  REG_BITS_DEF(0, 1, gcr_bodcr, BOD_EN)
  REG_BITS_DEF(1, 3, gcr_bodcr, BOD_VL)
  REG_BITS_DEF(3, 4, gcr_bodcr, BOD_RSTEN)
  REG_BITS_DEF(4, 5, gcr_bodcr, BOD_INTF)
  REG_BITS_DEF(5, 6, gcr_bodcr, BOD_LPM)
  REG_BITS_DEF(6, 7, gcr_bodcr, BOD_OUT)
  REG_BITS_DEF(7, 8, gcr_bodcr, LVR_EN)
  REG_BITS_COMMON(gcr_bodcr)
};

/* class for bit field: Power-On-Reset Controller Register */
template<class T>
struct bits_gcr_porcr
{
  REG_BITS_DEF(0, 16, gcr_porcr, POR_DIS_CODE)
  REG_BITS_COMMON(gcr_porcr)
};

/* class for bit field: P0 multiple function and input type control register */
template<class T>
struct bits_gcr_p0_mfp
{
  REG_BITS_DEF(0, 8, gcr_p0_mfp, P0_MFP)
  REG_BITS_DEF(8, 9, gcr_p0_mfp, P0_ALT0)
  REG_BITS_DEF(9, 10, gcr_p0_mfp, P0_ALT1)
  REG_BITS_DEF(10, 11, gcr_p0_mfp, P0_ALT2)
  REG_BITS_DEF(11, 12, gcr_p0_mfp, P0_ALT3)
  REG_BITS_DEF(12, 13, gcr_p0_mfp, P0_ALT4)
  REG_BITS_DEF(13, 14, gcr_p0_mfp, P0_ALT5)
  REG_BITS_DEF(14, 15, gcr_p0_mfp, P0_ALT6)
  REG_BITS_DEF(15, 16, gcr_p0_mfp, P0_ALT7)
  REG_BITS_DEF(16, 24, gcr_p0_mfp, P0_TYPEn)
  REG_BITS_COMMON(gcr_p0_mfp)
};

/* class for bit field: P1 multiple function and input type control register */
template<class T>
struct bits_gcr_p1_mfp
{
  REG_BITS_DEF(0, 8, gcr_p1_mfp, P1_MFP)
  REG_BITS_DEF(8, 9, gcr_p1_mfp, P1_ALT0)
  REG_BITS_DEF(9, 10, gcr_p1_mfp, P1_ALT1)
  REG_BITS_DEF(10, 11, gcr_p1_mfp, P1_ALT2)
  REG_BITS_DEF(11, 12, gcr_p1_mfp, P1_ALT3)
  REG_BITS_DEF(12, 13, gcr_p1_mfp, P1_ALT4)
  REG_BITS_DEF(13, 14, gcr_p1_mfp, P1_ALT5)
  REG_BITS_DEF(14, 15, gcr_p1_mfp, P1_ALT6)
  REG_BITS_DEF(15, 16, gcr_p1_mfp, P1_ALT7)
  REG_BITS_DEF(16, 24, gcr_p1_mfp, P1_TYPEn)
  REG_BITS_COMMON(gcr_p1_mfp)
};

/* class for bit field: P2 multiple function and input type control register */
template<class T>
struct bits_gcr_p2_mfp
{
  REG_BITS_DEF(0, 8, gcr_p2_mfp, P2_MFP)
  REG_BITS_DEF(8, 9, gcr_p2_mfp, P2_ALT0)
  REG_BITS_DEF(9, 10, gcr_p2_mfp, P2_ALT1)
  REG_BITS_DEF(10, 11, gcr_p2_mfp, P2_ALT2)
  REG_BITS_DEF(11, 12, gcr_p2_mfp, P2_ALT3)
  REG_BITS_DEF(12, 13, gcr_p2_mfp, P2_ALT4)
  REG_BITS_DEF(13, 14, gcr_p2_mfp, P2_ALT5)
  REG_BITS_DEF(14, 15, gcr_p2_mfp, P2_ALT6)
  REG_BITS_DEF(15, 16, gcr_p2_mfp, P2_ALT7)
  REG_BITS_DEF(16, 24, gcr_p2_mfp, P2_TYPEn)
  REG_BITS_COMMON(gcr_p2_mfp)
};

/* class for bit field: P3 multiple function and input type control register */
template<class T>
struct bits_gcr_p3_mfp
{
  REG_BITS_DEF(0, 8, gcr_p3_mfp, P3_MFP)
  REG_BITS_DEF(8, 9, gcr_p3_mfp, P3_ALT0)
  REG_BITS_DEF(9, 10, gcr_p3_mfp, P3_ALT1)
  REG_BITS_DEF(10, 11, gcr_p3_mfp, P3_ALT2)
  REG_BITS_DEF(11, 12, gcr_p3_mfp, P3_ALT3)
  REG_BITS_DEF(12, 13, gcr_p3_mfp, P3_ALT4)
  REG_BITS_DEF(13, 14, gcr_p3_mfp, P3_ALT5)
  REG_BITS_DEF(14, 15, gcr_p3_mfp, P3_ALT6)
  REG_BITS_DEF(15, 16, gcr_p3_mfp, P3_ALT7)
  REG_BITS_DEF(16, 24, gcr_p3_mfp, P3_TYPEn)
  REG_BITS_COMMON(gcr_p3_mfp)
};

/* class for bit field: P4 input type control register */
template<class T>
struct bits_gcr_p4_mfp
{
  REG_BITS_DEF(0, 8, gcr_p4_mfp, P4_MFP)
  REG_BITS_DEF(8, 9, gcr_p4_mfp, P4_ALT0)
  REG_BITS_DEF(9, 10, gcr_p4_mfp, P4_ALT1)
  REG_BITS_DEF(10, 11, gcr_p4_mfp, P4_ALT2)
  REG_BITS_DEF(11, 12, gcr_p4_mfp, P4_ALT3)
  REG_BITS_DEF(12, 13, gcr_p4_mfp, P4_ALT4)
  REG_BITS_DEF(13, 14, gcr_p4_mfp, P4_ALT5)
  REG_BITS_DEF(14, 15, gcr_p4_mfp, P4_ALT6)
  REG_BITS_DEF(15, 16, gcr_p4_mfp, P4_ALT7)
  REG_BITS_DEF(16, 24, gcr_p4_mfp, P4_TYPEn)
  REG_BITS_COMMON(gcr_p4_mfp)
};

/* class for bit field: Register Lock Key address */
template<class T>
struct bits_gcr_regwrprot
{
  REG_BITS_DEF(0, 8, gcr_regwrprot, REGWRPROT)
  REG_BITS_DEF(0, 1, gcr_regwrprot, REGPROTDIS)
  REG_BITS_COMMON(gcr_regwrprot)
};

/* class for bit field: Bit Mode Control */
template<class T>
struct bits_gpio_pmd
{
  REG_BITS_DEF(0, 2, gpio_pmd, PMD0)
  REG_BITS_DEF(2, 4, gpio_pmd, PMD1)
  REG_BITS_DEF(4, 6, gpio_pmd, PMD2)
  REG_BITS_DEF(6, 8, gpio_pmd, PMD3)
  REG_BITS_DEF(8, 10, gpio_pmd, PMD4)
  REG_BITS_DEF(10, 12, gpio_pmd, PMD5)
  REG_BITS_DEF(12, 14, gpio_pmd, PMD6)
  REG_BITS_DEF(14, 16, gpio_pmd, PMD7)
  REG_BITS_COMMON(gpio_pmd)
};

/* class for bit field: Bit OFF Digital Enable */
template<class T>
struct bits_gpio_offd
{
  REG_BITS_DEF(16, 24, gpio_offd, OFFD)
  REG_BITS_COMMON(gpio_offd)
};

/* class for bit field: Data Output Value */
template<class T>
struct bits_gpio_dout
{
  REG_BITS_DEF(0, 1, gpio_dout, DOUT0)
  REG_BITS_DEF(1, 2, gpio_dout, DOUT1)
  REG_BITS_DEF(2, 3, gpio_dout, DOUT2)
  REG_BITS_DEF(3, 4, gpio_dout, DOUT3)
  REG_BITS_DEF(4, 5, gpio_dout, DOUT4)
  REG_BITS_DEF(5, 6, gpio_dout, DOUT5)
  REG_BITS_DEF(6, 7, gpio_dout, DOUT6)
  REG_BITS_DEF(7, 8, gpio_dout, DOUT7)
  REG_BITS_COMMON(gpio_dout)
};

/* class for bit field: Data Output Write Mask */
template<class T>
struct bits_gpio_dmask
{
  REG_BITS_DEF(0, 1, gpio_dmask, DMASK0)
  REG_BITS_DEF(1, 2, gpio_dmask, DMASK1)
  REG_BITS_DEF(2, 3, gpio_dmask, DMASK2)
  REG_BITS_DEF(3, 4, gpio_dmask, DMASK3)
  REG_BITS_DEF(4, 5, gpio_dmask, DMASK4)
  REG_BITS_DEF(5, 6, gpio_dmask, DMASK5)
  REG_BITS_DEF(6, 7, gpio_dmask, DMASK6)
  REG_BITS_DEF(7, 8, gpio_dmask, DMASK7)
  REG_BITS_COMMON(gpio_dmask)
};

/* class for bit field: Pin Value */
template<class T>
struct bits_gpio_pin
{
  REG_BITS_DEF(0, 1, gpio_pin, PIN0)
  REG_BITS_DEF(1, 2, gpio_pin, PIN1)
  REG_BITS_DEF(2, 3, gpio_pin, PIN2)
  REG_BITS_DEF(3, 4, gpio_pin, PIN3)
  REG_BITS_DEF(4, 5, gpio_pin, PIN4)
  REG_BITS_DEF(5, 6, gpio_pin, PIN5)
  REG_BITS_DEF(6, 7, gpio_pin, PIN6)
  REG_BITS_DEF(7, 8, gpio_pin, PIN7)
  REG_BITS_COMMON(gpio_pin)
};

/* class for bit field: De-bounce Enable */
template<class T>
struct bits_gpio_dben
{
  REG_BITS_DEF(0, 1, gpio_dben, DBEN0)
  REG_BITS_DEF(1, 2, gpio_dben, DBEN1)
  REG_BITS_DEF(2, 3, gpio_dben, DBEN2)
  REG_BITS_DEF(3, 4, gpio_dben, DBEN3)
  REG_BITS_DEF(4, 5, gpio_dben, DBEN4)
  REG_BITS_DEF(5, 6, gpio_dben, DBEN5)
  REG_BITS_DEF(6, 7, gpio_dben, DBEN6)
  REG_BITS_DEF(7, 8, gpio_dben, DBEN7)
  REG_BITS_COMMON(gpio_dben)
};

/* class for bit field: Interrupt Mode Control */
template<class T>
struct bits_gpio_imd
{
  REG_BITS_DEF(0, 1, gpio_imd, IMD0)
  REG_BITS_DEF(1, 2, gpio_imd, IMD1)
  REG_BITS_DEF(2, 3, gpio_imd, IMD2)
  REG_BITS_DEF(3, 4, gpio_imd, IMD3)
  REG_BITS_DEF(4, 5, gpio_imd, IMD4)
  REG_BITS_DEF(5, 6, gpio_imd, IMD5)
  REG_BITS_DEF(6, 7, gpio_imd, IMD6)
  REG_BITS_DEF(7, 8, gpio_imd, IMD7)
  REG_BITS_COMMON(gpio_imd)
};

/* class for bit field: Interrupt Enable */
template<class T>
struct bits_gpio_ien
{
  REG_BITS_DEF(0, 1, gpio_ien, IF_EN0)
  REG_BITS_DEF(1, 2, gpio_ien, IF_EN1)
  REG_BITS_DEF(2, 3, gpio_ien, IF_EN2)
  REG_BITS_DEF(3, 4, gpio_ien, IF_EN3)
  REG_BITS_DEF(4, 5, gpio_ien, IF_EN4)
  REG_BITS_DEF(5, 6, gpio_ien, IF_EN5)
  REG_BITS_DEF(6, 7, gpio_ien, IF_EN6)
  REG_BITS_DEF(7, 8, gpio_ien, IF_EN7)
  REG_BITS_DEF(16, 17, gpio_ien, IR_EN0)
  REG_BITS_DEF(17, 18, gpio_ien, IR_EN1)
  REG_BITS_DEF(18, 19, gpio_ien, IR_EN2)
  REG_BITS_DEF(19, 20, gpio_ien, IR_EN3)
  REG_BITS_DEF(20, 21, gpio_ien, IR_EN4)
  REG_BITS_DEF(21, 22, gpio_ien, IR_EN5)
  REG_BITS_DEF(22, 23, gpio_ien, IR_EN6)
  REG_BITS_DEF(23, 24, gpio_ien, IR_EN7)
  REG_BITS_COMMON(gpio_ien)
};

/* class for bit field: Interrupt Trigger Source */
template<class T>
struct bits_gpio_isrc
{
  REG_BITS_DEF(0, 1, gpio_isrc, ISRC0)
  REG_BITS_DEF(1, 2, gpio_isrc, ISRC1)
  REG_BITS_DEF(2, 3, gpio_isrc, ISRC2)
  REG_BITS_DEF(3, 4, gpio_isrc, ISRC3)
  REG_BITS_DEF(4, 5, gpio_isrc, ISRC4)
  REG_BITS_DEF(5, 6, gpio_isrc, ISRC5)
  REG_BITS_DEF(6, 7, gpio_isrc, ISRC6)
  REG_BITS_DEF(7, 8, gpio_isrc, ISRC7)
  REG_BITS_COMMON(gpio_isrc)
};

/* class for bit field: Interrupt De-bounce Cycle Control */
template<class T>
struct bits_gpio_gcr_dbncecon
{
  REG_BITS_DEF(0, 4, gpio_gcr_dbncecon, DBCLKSEL)
  REG_BITS_DEF(4, 5, gpio_gcr_dbncecon, DBCLKSRC)
  REG_BITS_DEF(5, 6, gpio_gcr_dbncecon, ICLK_ON)
  REG_BITS_COMMON(gpio_gcr_dbncecon)
};

/* class for bit field: Px.0 Data Output Value */
template<class T>
struct bits_gpio_bits_px_dout
{
  REG_BITS_DEF(0, 1, gpio_bits_px_dout, DOUT)
  REG_BITS_COMMON(gpio_bits_px_dout)
};

/* class for bit field: I2C Control Register */
template<class T>
struct bits_i2c_i2con
{
  REG_BITS_DEF(2, 3, i2c_i2con, AA)
  REG_BITS_DEF(3, 4, i2c_i2con, SI)
  REG_BITS_DEF(4, 5, i2c_i2con, STO)
  REG_BITS_DEF(5, 6, i2c_i2con, STA)
  REG_BITS_DEF(6, 7, i2c_i2con, ENSI)
  REG_BITS_DEF(7, 8, i2c_i2con, EI)
  REG_BITS_COMMON(i2c_i2con)
};

/* class for bit field: I2C slave Address Register0 */
template<class T>
struct bits_i2c_i2caddr
{
  REG_BITS_DEF(0, 1, i2c_i2caddr, GC)
  REG_BITS_DEF(1, 8, i2c_i2caddr, I2CADDR)
  REG_BITS_COMMON(i2c_i2caddr)
};

/* class for bit field: I2C DATA Register */
template<class T>
struct bits_i2c_i2cdat
{
  REG_BITS_DEF(0, 8, i2c_i2cdat, I2CDAT)
  REG_BITS_COMMON(i2c_i2cdat)
};

/* class for bit field: I2C Status Register */
template<class T>
struct bits_i2c_i2cstatus
{
  REG_BITS_DEF(0, 8, i2c_i2cstatus, I2CSTATUS)
  REG_BITS_COMMON(i2c_i2cstatus)
};

/* class for bit field: I2C clock divided Register */
template<class T>
struct bits_i2c_i2clk
{
  REG_BITS_DEF(0, 8, i2c_i2clk, I2CLK)
  REG_BITS_COMMON(i2c_i2clk)
};

/* class for bit field: I2C Time out control Register */
template<class T>
struct bits_i2c_i2ctoc
{
  REG_BITS_DEF(0, 1, i2c_i2ctoc, TIF)
  REG_BITS_DEF(1, 2, i2c_i2ctoc, DIV4)
  REG_BITS_DEF(2, 3, i2c_i2ctoc, ENTI)
  REG_BITS_COMMON(i2c_i2ctoc)
};

/* class for bit field: I2C Slave address Mask Register0 */
template<class T>
struct bits_i2c_i2cadm
{
  REG_BITS_DEF(1, 8, i2c_i2cadm, I2ADMx)
  REG_BITS_COMMON(i2c_i2cadm)
};

/* class for bit field: MCU IRQ0 (BOD) interrupt source identify */
template<class T>
struct bits_int_irq_src
{
  REG_BITS_DEF(0, 3, int_irq_src, INT_SRC)
  REG_BITS_COMMON(int_irq_src)
};


/* class for bit field: NMI source interrupt select control register */
template<class T>
struct bits_int_nmi_sel
{
  REG_BITS_DEF(0, 5, int_nmi_sel, NMI_SEL)
  REG_BITS_COMMON(int_nmi_sel)
};

/* class for bit field: PWM Pre-scalar Register */
template<class T>
struct bits_pwm_ppr
{
  REG_BITS_DEF(0, 8, pwm_ppr, CP01)
  REG_BITS_DEF(8, 16, pwm_ppr, CP23)
  REG_BITS_DEF(16, 24, pwm_ppr, DZI01)
  REG_BITS_DEF(24, 32, pwm_ppr, DZI23)
  REG_BITS_COMMON(pwm_ppr)
};

/* class for bit field: PWM Clock Select Register */
template<class T>
struct bits_pwm_csr
{
  REG_BITS_DEF(0, 3, pwm_csr, CSR0)
  REG_BITS_DEF(4, 7, pwm_csr, CSR1)
  REG_BITS_DEF(8, 11, pwm_csr, CSR2)
  REG_BITS_DEF(12, 15, pwm_csr, CSR3)
  REG_BITS_COMMON(pwm_csr)
};

/* class for bit field: PWM Control Register */
template<class T>
struct bits_pwm_pcr
{
  REG_BITS_DEF(0, 1, pwm_pcr, CH0EN)
  REG_BITS_DEF(2, 3, pwm_pcr, CH0INV)
  REG_BITS_DEF(3, 4, pwm_pcr, CH0MOD)
  REG_BITS_DEF(4, 5, pwm_pcr, DZEN01)
  REG_BITS_DEF(5, 6, pwm_pcr, DZEN23)
  REG_BITS_DEF(8, 9, pwm_pcr, CH1EN)
  REG_BITS_DEF(10, 11, pwm_pcr, CH1INV)
  REG_BITS_DEF(11, 12, pwm_pcr, CH1MOD)
  REG_BITS_DEF(16, 17, pwm_pcr, CH2EN)
  REG_BITS_DEF(18, 19, pwm_pcr, CH2INV)
  REG_BITS_DEF(19, 20, pwm_pcr, CH2MOD)
  REG_BITS_DEF(24, 25, pwm_pcr, CH3EN)
  REG_BITS_DEF(26, 27, pwm_pcr, CH3INV)
  REG_BITS_DEF(27, 28, pwm_pcr, CH3MOD)
  REG_BITS_COMMON(pwm_pcr)
};

/* class for bit field: PWM Counter Register 0 */
template<class T>
struct bits_pwm_cnr
{
  REG_BITS_DEF(0, 16, pwm_cnr, CNR)
  REG_BITS_COMMON(pwm_cnr)
};

/* class for bit field: PWM Comparator Register 0 */
template<class T>
struct bits_pwm_cmr
{
  REG_BITS_DEF(0, 16, pwm_cmr, CMR)
  REG_BITS_COMMON(pwm_cmr)
};

/* class for bit field: PWM Data Register 0 */
template<class T>
struct bits_pwm_pdr
{
  REG_BITS_DEF(0, 16, pwm_pdr, PDR)
  REG_BITS_COMMON(pwm_pdr)
};

/* class for bit field: PWM Interrupt Enable Register */
template<class T>
struct bits_pwm_pier
{
  REG_BITS_DEF(0, 1, pwm_pier, PWMIE0)
  REG_BITS_DEF(1, 2, pwm_pier, PWMIE1)
  REG_BITS_DEF(2, 3, pwm_pier, PWMIE2)
  REG_BITS_DEF(3, 4, pwm_pier, PWMIE3)
  REG_BITS_COMMON(pwm_pier)
};

/* class for bit field: PWM Interrupt Indication Register */
template<class T>
struct bits_pwm_piir
{
  REG_BITS_DEF(0, 1, pwm_piir, PWMIF0)
  REG_BITS_DEF(1, 2, pwm_piir, PWMIF1)
  REG_BITS_DEF(2, 3, pwm_piir, PWMIF2)
  REG_BITS_DEF(3, 4, pwm_piir, PWMIF3)
  REG_BITS_COMMON(pwm_piir)
};

/* class for bit field: Capture Control Register */
template<class T>
struct bits_pwm_ccr0
{
  REG_BITS_DEF(0, 1, pwm_ccr0, INV0)
  REG_BITS_DEF(1, 2, pwm_ccr0, CRL_IE0)
  REG_BITS_DEF(2, 3, pwm_ccr0, CFL_IE0)
  REG_BITS_DEF(3, 4, pwm_ccr0, CAPCH0EN)
  REG_BITS_DEF(4, 5, pwm_ccr0, CAPIF0)
  REG_BITS_DEF(6, 7, pwm_ccr0, CRLRI0)
  REG_BITS_DEF(7, 8, pwm_ccr0, CFLRI0)
  REG_BITS_DEF(16, 17, pwm_ccr0, INV1)
  REG_BITS_DEF(17, 18, pwm_ccr0, CRL_IE1)
  REG_BITS_DEF(18, 19, pwm_ccr0, CFL_IE1)
  REG_BITS_DEF(19, 20, pwm_ccr0, CAPCH1EN)
  REG_BITS_DEF(20, 21, pwm_ccr0, CAPIF1)
  REG_BITS_DEF(22, 23, pwm_ccr0, CRLRI1)
  REG_BITS_DEF(23, 24, pwm_ccr0, CFLRI1)
  REG_BITS_COMMON(pwm_ccr0)
};

/* class for bit field: Capture Control Register */
template<class T>
struct bits_pwm_ccr2
{
  REG_BITS_DEF(0, 1, pwm_ccr2, INV2)
  REG_BITS_DEF(1, 2, pwm_ccr2, CRL_IE2)
  REG_BITS_DEF(2, 3, pwm_ccr2, CFL_IE2)
  REG_BITS_DEF(3, 4, pwm_ccr2, CAPCH2EN)
  REG_BITS_DEF(4, 5, pwm_ccr2, CAPIF2)
  REG_BITS_DEF(6, 7, pwm_ccr2, CRLRI2)
  REG_BITS_DEF(7, 8, pwm_ccr2, CFLRI2)
  REG_BITS_DEF(16, 17, pwm_ccr2, INV3)
  REG_BITS_DEF(17, 18, pwm_ccr2, CRL_IE3)
  REG_BITS_DEF(18, 19, pwm_ccr2, CFL_IE3)
  REG_BITS_DEF(19, 20, pwm_ccr2, CAPCH3EN)
  REG_BITS_DEF(20, 21, pwm_ccr2, CAPIF3)
  REG_BITS_DEF(22, 23, pwm_ccr2, CRLRI3)
  REG_BITS_DEF(23, 24, pwm_ccr2, CFLRI3)
  REG_BITS_COMMON(pwm_ccr2)
};

/* class for bit field: Capture Rising Latch Register (Channel 0) */
template<class T>
struct bits_pwm_crlr
{
  REG_BITS_DEF(0, 16, pwm_crlr, CRLR)
  REG_BITS_COMMON(pwm_crlr)
};

/* class for bit field: Capture Falling Latch Register (Channel 0) */
template<class T>
struct bits_pwm_cflr
{
  REG_BITS_DEF(0, 16, pwm_cflr, CFLR)
  REG_BITS_COMMON(pwm_cflr)
};

/* class for bit field: Capture Input Enable Register */
template<class T>
struct bits_pwm_capenr
{
  REG_BITS_DEF(0, 4, pwm_capenr, CAPENR)
  REG_BITS_COMMON(pwm_capenr)
};

/* class for bit field: PWM Output Enable */
template<class T>
struct bits_pwm_poe
{
  REG_BITS_DEF(0, 1, pwm_poe, PWM0)
  REG_BITS_DEF(1, 2, pwm_poe, PWM1)
  REG_BITS_DEF(2, 3, pwm_poe, PWM2)
  REG_BITS_DEF(3, 4, pwm_poe, PWM3)
  REG_BITS_COMMON(pwm_poe)
};



/* class for bit field: SysTick Reload value Register */
template<class T>
struct bits_scs_syst_rvr
{
  REG_BITS_DEF(0, 24, scs_syst_rvr, RELOAD)
  REG_BITS_COMMON(scs_syst_rvr)
};

/* class for bit field: SysTick Current value Register */
template<class T>
struct bits_scs_syst_cvr
{
  REG_BITS_DEF(0, 24, scs_syst_cvr, CURRENT)
  REG_BITS_COMMON(scs_syst_cvr)
};

/* class for bit field: IRQ0 ~ IRQ3 Priority Control Register */
template<class T>
struct bits_scs_nvic_ipr0
{
  REG_BITS_DEF(6, 8, scs_nvic_ipr0, PRI_0)
  REG_BITS_DEF(14, 16, scs_nvic_ipr0, PRI_1)
  REG_BITS_DEF(22, 24, scs_nvic_ipr0, PRI_2)
  REG_BITS_DEF(30, 32, scs_nvic_ipr0, PRI_3)
  REG_BITS_COMMON(scs_nvic_ipr0)
};

/* class for bit field: IRQ4 ~ IRQ7 Priority Control Register */
template<class T>
struct bits_scs_nvic_ipr1
{
  REG_BITS_DEF(6, 8, scs_nvic_ipr1, PRI_4)
  REG_BITS_DEF(14, 16, scs_nvic_ipr1, PRI_5)
  REG_BITS_DEF(22, 24, scs_nvic_ipr1, PRI_6)
  REG_BITS_DEF(30, 32, scs_nvic_ipr1, PRI_7)
  REG_BITS_COMMON(scs_nvic_ipr1)
};

/* class for bit field: IRQ8 ~ IRQ11 Priority Control Register */
template<class T>
struct bits_scs_nvic_ipr2
{
  REG_BITS_DEF(6, 8, scs_nvic_ipr2, PRI_8)
  REG_BITS_DEF(14, 16, scs_nvic_ipr2, PRI_9)
  REG_BITS_DEF(22, 24, scs_nvic_ipr2, PRI_10)
  REG_BITS_DEF(30, 32, scs_nvic_ipr2, PRI_11)
  REG_BITS_COMMON(scs_nvic_ipr2)
};

/* class for bit field: IRQ12 ~ IRQ15 Priority Control Register */
template<class T>
struct bits_scs_nvic_ipr3
{
  REG_BITS_DEF(6, 8, scs_nvic_ipr3, PRI_12)
  REG_BITS_DEF(14, 16, scs_nvic_ipr3, PRI_13)
  REG_BITS_DEF(22, 24, scs_nvic_ipr3, PRI_14)
  REG_BITS_DEF(30, 32, scs_nvic_ipr3, PRI_15)
  REG_BITS_COMMON(scs_nvic_ipr3)
};

/* class for bit field: IRQ16 ~ IRQ19 Priority Control Register */
template<class T>
struct bits_scs_nvic_ipr4
{
  REG_BITS_DEF(6, 8, scs_nvic_ipr4, PRI_16)
  REG_BITS_DEF(14, 16, scs_nvic_ipr4, PRI_17)
  REG_BITS_DEF(22, 24, scs_nvic_ipr4, PRI_18)
  REG_BITS_DEF(30, 32, scs_nvic_ipr4, PRI_19)
  REG_BITS_COMMON(scs_nvic_ipr4)
};

/* class for bit field: IRQ20 ~ IRQ23 Priority Control Register */
template<class T>
struct bits_scs_nvic_ipr5
{
  REG_BITS_DEF(6, 8, scs_nvic_ipr5, PRI_20)
  REG_BITS_DEF(14, 16, scs_nvic_ipr5, PRI_21)
  REG_BITS_DEF(22, 24, scs_nvic_ipr5, PRI_22)
  REG_BITS_DEF(30, 32, scs_nvic_ipr5, PRI_23)
  REG_BITS_COMMON(scs_nvic_ipr5)
};

/* class for bit field: IRQ24 ~ IRQ27 Priority Control Register */
template<class T>
struct bits_scs_nvic_ipr6
{
  REG_BITS_DEF(6, 8, scs_nvic_ipr6, PRI_24)
  REG_BITS_DEF(14, 16, scs_nvic_ipr6, PRI_25)
  REG_BITS_DEF(22, 24, scs_nvic_ipr6, PRI_26)
  REG_BITS_DEF(30, 32, scs_nvic_ipr6, PRI_27)
  REG_BITS_COMMON(scs_nvic_ipr6)
};

/* class for bit field: IRQ28 ~ IRQ31 Priority Control Register */
template<class T>
struct bits_scs_nvic_ipr7
{
  REG_BITS_DEF(6, 8, scs_nvic_ipr7, PRI_28)
  REG_BITS_DEF(14, 16, scs_nvic_ipr7, PRI_29)
  REG_BITS_DEF(22, 24, scs_nvic_ipr7, PRI_30)
  REG_BITS_DEF(30, 32, scs_nvic_ipr7, PRI_31)
  REG_BITS_COMMON(scs_nvic_ipr7)
};

/* class for bit field: CPUID Base Register */
template<class T>
struct bits_scs_cpuid
{
  REG_BITS_DEF(0, 4, scs_cpuid, REVISION)
  REG_BITS_DEF(4, 16, scs_cpuid, PARTNO)
  REG_BITS_DEF(16, 20, scs_cpuid, PART)
  REG_BITS_DEF(24, 32, scs_cpuid, IMPLEMENTER)
  REG_BITS_COMMON(scs_cpuid)
};

/* class for bit field: Interrupt Control State Register */
template<class T>
struct bits_scs_icsr
{
  REG_BITS_DEF(0, 9, scs_icsr, VECTACTIVE)
  REG_BITS_DEF(12, 21, scs_icsr, VECTPENDING)
  REG_BITS_DEF(22, 23, scs_icsr, ISRPENDING)
  REG_BITS_DEF(23, 24, scs_icsr, ISRPREEMPT)
  REG_BITS_DEF(25, 26, scs_icsr, PENDSTCLR)
  REG_BITS_DEF(26, 27, scs_icsr, PENDSTSET)
  REG_BITS_DEF(27, 28, scs_icsr, PENDSVCLR)
  REG_BITS_DEF(28, 29, scs_icsr, PENDSVSET)
  REG_BITS_DEF(31, 32, scs_icsr, NMIPENDSET)
  REG_BITS_COMMON(scs_icsr)
};

/* class for bit field: Application Interrupt and Reset Control Register */
template<class T>
struct bits_scs_aircr
{
  REG_BITS_DEF(1, 2, scs_aircr, VECTCLRACTIVE)
  REG_BITS_DEF(2, 3, scs_aircr, SYSRESETREQ)
  REG_BITS_DEF(16, 32, scs_aircr, VECTORKEY)
  REG_BITS_COMMON(scs_aircr)
};

/* class for bit field: System Control Register */
template<class T>
struct bits_scs_scr
{
  REG_BITS_DEF(1, 2, scs_scr, SLEEPONEXIT)
  REG_BITS_DEF(2, 3, scs_scr, SLEEPDEEP)
  REG_BITS_DEF(4, 5, scs_scr, SEVONPEND)
  REG_BITS_COMMON(scs_scr)
};

/* class for bit field: System Handler Priority Register 2 */
template<class T>
struct bits_scs_shpr2
{
  REG_BITS_DEF(30, 32, scs_shpr2, PRI_11)
  REG_BITS_COMMON(scs_shpr2)
};

/* class for bit field: System Handler Priority Register 3 */
template<class T>
struct bits_scs_shpr3
{
  REG_BITS_DEF(22, 24, scs_shpr3, PRI_14)
  REG_BITS_DEF(30, 32, scs_shpr3, PRI_15)
  REG_BITS_COMMON(scs_shpr3)
};

/* class for bit field: Timer0 Control and Status Register */
template<class T>
struct bits_tmr_tcsr
{
  REG_BITS_DEF(0, 8, tmr_tcsr, PRESCALE)
  REG_BITS_DEF(16, 17, tmr_tcsr, TDR_EN)
  REG_BITS_DEF(25, 26, tmr_tcsr, CACT)
  REG_BITS_DEF(26, 27, tmr_tcsr, CRST)
  REG_BITS_DEF(27, 29, tmr_tcsr, MODE)
  REG_BITS_DEF(29, 30, tmr_tcsr, IE)
  REG_BITS_DEF(30, 31, tmr_tcsr, CEN)
  REG_BITS_COMMON(tmr_tcsr)
};

/* class for bit field: Timer0 Compare Register */
template<class T>
struct bits_tmr_tcmpr
{
  REG_BITS_DEF(0, 24, tmr_tcmpr, TCMP)
  REG_BITS_COMMON(tmr_tcmpr)
};

/* class for bit field: Timer0 Interrupt Status Register */
template<class T>
struct bits_tmr_tisr
{
  REG_BITS_DEF(0, 1, tmr_tisr, TIF)
  REG_BITS_COMMON(tmr_tisr)
};

/* class for bit field: Timer0 Data Register */
template<class T>
struct bits_tmr_tdr
{
  REG_BITS_DEF(0, 24, tmr_tdr, TDR)
  REG_BITS_COMMON(tmr_tdr)
};

/* class for bit field: Watchdog Timer Control Register */
template<class T>
struct bits_wdt_wtcr
{
  REG_BITS_DEF(0, 1, wdt_wtcr, WTR)
  REG_BITS_DEF(1, 2, wdt_wtcr, WTRE)
  REG_BITS_DEF(2, 3, wdt_wtcr, WTRF)
  REG_BITS_DEF(3, 4, wdt_wtcr, WTIF)
  REG_BITS_DEF(4, 5, wdt_wtcr, WTWKE)
  REG_BITS_DEF(5, 6, wdt_wtcr, WTWKF)
  REG_BITS_DEF(6, 7, wdt_wtcr, WTIE)
  REG_BITS_DEF(7, 8, wdt_wtcr, WTE)
  REG_BITS_DEF(8, 11, wdt_wtcr, WTIS)
  REG_BITS_COMMON(wdt_wtcr)
};

/* class for bit field: Control and Status Register */
template<class T>
struct bits_spi_spi_cntrl
{
  REG_BITS_DEF(0, 1, spi_spi_cntrl, GO_BUSY)
  REG_BITS_DEF(1, 2, spi_spi_cntrl, RX_NEG)
  REG_BITS_DEF(2, 3, spi_spi_cntrl, TX_NEG)
  REG_BITS_DEF(3, 8, spi_spi_cntrl, TX_BIT_LEN)
  REG_BITS_DEF(8, 10, spi_spi_cntrl, TX_NUM)
  REG_BITS_DEF(10, 11, spi_spi_cntrl, LSB)
  REG_BITS_DEF(11, 12, spi_spi_cntrl, CLKP)
  REG_BITS_DEF(12, 16, spi_spi_cntrl, SP_CYCLE)
  REG_BITS_DEF(16, 17, spi_spi_cntrl, IF)
  REG_BITS_DEF(17, 18, spi_spi_cntrl, IE)
  REG_BITS_DEF(18, 19, spi_spi_cntrl, SLAVE)
  REG_BITS_DEF(19, 21, spi_spi_cntrl, REORDER)
  REG_BITS_DEF(23, 24, spi_spi_cntrl, VARCLK_EN)
  REG_BITS_COMMON(spi_spi_cntrl)
};

/* class for bit field: Clock Divider Register */
template<class T>
struct bits_spi_spi_divider
{
  REG_BITS_DEF(0, 16, spi_spi_divider, DIVIDER)
  REG_BITS_DEF(16, 32, spi_spi_divider, DIVIDER2)
  REG_BITS_COMMON(spi_spi_divider)
};

/* class for bit field: Slave Select Register */
template<class T>
struct bits_spi_spi_ssr
{
  REG_BITS_DEF(0, 1, spi_spi_ssr, SSR)
  REG_BITS_DEF(2, 3, spi_spi_ssr, SS_LVL)
  REG_BITS_DEF(3, 4, spi_spi_ssr, AUTOSS)
  REG_BITS_DEF(4, 5, spi_spi_ssr, SS_LTRIG)
  REG_BITS_DEF(5, 6, spi_spi_ssr, LTRIG_FLAG)
  REG_BITS_COMMON(spi_spi_ssr)
};

/* class for bit field: UART0 Receive Buffer Register. */
template<class T>
struct bits_uart_ua_rbr
{
  REG_BITS_DEF(0, 8, uart_ua_rbr, _8_bitReceivedData)
  REG_BITS_COMMON(uart_ua_rbr)
};

/* class for bit field: UART0 Transmit Holding Register. */
template<class T>
struct bits_uart_ua_thr
{
  REG_BITS_DEF(0, 8, uart_ua_thr, _8_bitTransmittedData)
  REG_BITS_COMMON(uart_ua_thr)
};

/* class for bit field: UART0 Interrupt Enable Register. */
template<class T>
struct bits_uart_ua_ier
{
  REG_BITS_DEF(0, 1, uart_ua_ier, RDA_IEN)
  REG_BITS_DEF(1, 2, uart_ua_ier, THRE_IEN)
  REG_BITS_DEF(2, 3, uart_ua_ier, RLS_IEN)
  REG_BITS_DEF(3, 4, uart_ua_ier, Modem_IEN)
  REG_BITS_DEF(4, 5, uart_ua_ier, RTO_IEN)
  REG_BITS_DEF(5, 6, uart_ua_ier, BUF_ERR_IEN)
  REG_BITS_DEF(6, 7, uart_ua_ier, Wake_EN)
  REG_BITS_DEF(11, 12, uart_ua_ier, Time_Out_EN)
  REG_BITS_DEF(12, 13, uart_ua_ier, Auto_RTS_EN)
  REG_BITS_DEF(13, 14, uart_ua_ier, Auto_CTS_EN)
  REG_BITS_COMMON(uart_ua_ier)
};

/* class for bit field: UART0 FIFO Control Register. */
template<class T>
struct bits_uart_ua_fcr
{
  REG_BITS_DEF(1, 2, uart_ua_fcr, RFR)
  REG_BITS_DEF(2, 3, uart_ua_fcr, TFR)
  REG_BITS_DEF(4, 8, uart_ua_fcr, RFITL)
  REG_BITS_DEF(8, 9, uart_ua_fcr, RX_DIS)
  REG_BITS_DEF(16, 20, uart_ua_fcr, RTS_Tri_Lev)
  REG_BITS_COMMON(uart_ua_fcr)
};

/* class for bit field: UART0 Line Control Register. */
template<class T>
struct bits_uart_ua_lcr
{
  REG_BITS_DEF(0, 2, uart_ua_lcr, WLS)
  REG_BITS_DEF(2, 3, uart_ua_lcr, NSB)
  REG_BITS_DEF(3, 4, uart_ua_lcr, PBE)
  REG_BITS_DEF(4, 5, uart_ua_lcr, EPE)
  REG_BITS_DEF(5, 6, uart_ua_lcr, SPE)
  REG_BITS_DEF(6, 7, uart_ua_lcr, BCB)
  REG_BITS_COMMON(uart_ua_lcr)
};

/* class for bit field: UART0 Modem Control Register. */
template<class T>
struct bits_uart_ua_mcr
{
  REG_BITS_DEF(1, 2, uart_ua_mcr, RTS)
  REG_BITS_DEF(9, 10, uart_ua_mcr, Lev_RTS)
  REG_BITS_DEF(13, 14, uart_ua_mcr, RTS_St)
  REG_BITS_COMMON(uart_ua_mcr)
};

/* class for bit field: UART0 Modem Status Register. */
template<class T>
struct bits_uart_ua_msr
{
  REG_BITS_DEF(0, 1, uart_ua_msr, DCTSF)
  REG_BITS_DEF(4, 5, uart_ua_msr, CTS_St)
  REG_BITS_DEF(8, 9, uart_ua_msr, Lev_CTS)
  REG_BITS_COMMON(uart_ua_msr)
};

/* class for bit field: UART0 FIFO Status Register. */
template<class T>
struct bits_uart_ua_fsr
{
  REG_BITS_DEF(0, 1, uart_ua_fsr, Rx_Over_IF)
  REG_BITS_DEF(3, 4, uart_ua_fsr, RS_485_Add_Det)
  REG_BITS_DEF(4, 5, uart_ua_fsr, PEF)
  REG_BITS_DEF(5, 6, uart_ua_fsr, FEF)
  REG_BITS_DEF(6, 7, uart_ua_fsr, BIF)
  REG_BITS_DEF(8, 14, uart_ua_fsr, Rx_Pointer)
  REG_BITS_DEF(14, 15, uart_ua_fsr, Rx_Empty)
  REG_BITS_DEF(15, 16, uart_ua_fsr, Rx_Full)
  REG_BITS_DEF(16, 22, uart_ua_fsr, Tx_Pointer)
  REG_BITS_DEF(22, 23, uart_ua_fsr, Tx_Empty)
  REG_BITS_DEF(23, 24, uart_ua_fsr, Tx_Full)
  REG_BITS_DEF(24, 25, uart_ua_fsr, Tx_Over_IF)
  REG_BITS_DEF(28, 29, uart_ua_fsr, TE_Flag)
  REG_BITS_COMMON(uart_ua_fsr)
};

/* class for bit field: UART0 Interrupt Status Register. */
template<class T>
struct bits_uart_ua_isr
{
  REG_BITS_DEF(0, 1, uart_ua_isr, RDA_IF)
  REG_BITS_DEF(1, 2, uart_ua_isr, THRE_IF)
  REG_BITS_DEF(2, 3, uart_ua_isr, RLS_IF)
  REG_BITS_DEF(3, 4, uart_ua_isr, Modem_IF)
  REG_BITS_DEF(4, 5, uart_ua_isr, Tout_IF)
  REG_BITS_DEF(5, 6, uart_ua_isr, Buf_Err_IF)
  REG_BITS_DEF(8, 9, uart_ua_isr, RDA_INT)
  REG_BITS_DEF(9, 10, uart_ua_isr, THRE_INT)
  REG_BITS_DEF(10, 11, uart_ua_isr, RLS_INT)
  REG_BITS_DEF(11, 12, uart_ua_isr, Modem_INT)
  REG_BITS_DEF(12, 13, uart_ua_isr, Tout_INT)
  REG_BITS_DEF(13, 14, uart_ua_isr, Buf_Err_INT)
  REG_BITS_COMMON(uart_ua_isr)
};

/* class for bit field: UART0 Time Out Register */
template<class T>
struct bits_uart_ua_tor
{
  REG_BITS_DEF(0, 7, uart_ua_tor, TOIC)
  REG_BITS_DEF(8, 16, uart_ua_tor, DLY)
  REG_BITS_COMMON(uart_ua_tor)
};

/* class for bit field: UART0 Baud Rate Divisor Register */
template<class T>
struct bits_uart_ua_baud
{
  REG_BITS_DEF(0, 8, uart_ua_baud, BRD_LowByte)
  REG_BITS_DEF(8, 16, uart_ua_baud, BRD_HighByte)
  REG_BITS_DEF(24, 28, uart_ua_baud, Divider_X)
  REG_BITS_DEF(28, 29, uart_ua_baud, DIV_X_ONE)
  REG_BITS_DEF(29, 30, uart_ua_baud, DIV_X_EN)
  REG_BITS_COMMON(uart_ua_baud)
};

/* class for bit field: UART0 IrDA Control Register. */
template<class T>
struct bits_uart_ua_ircr
{
  REG_BITS_DEF(1, 2, uart_ua_ircr, Tx_SELECT)
  REG_BITS_DEF(2, 3, uart_ua_ircr, LB)
  REG_BITS_DEF(5, 6, uart_ua_ircr, INV_Tx)
  REG_BITS_DEF(6, 7, uart_ua_ircr, INV_Rx)
  REG_BITS_COMMON(uart_ua_ircr)
};

/* class for bit field: UART0 RS485 Control State Register. */
template<class T>
struct bits_uart_ua_act_csr
{
  REG_BITS_DEF(8, 9, uart_ua_act_csr, RS_485_NMM)
  REG_BITS_DEF(9, 10, uart_ua_act_csr, RS_485_AAD)
  REG_BITS_DEF(10, 11, uart_ua_act_csr, RS_485_AUD)
  REG_BITS_DEF(15, 16, uart_ua_act_csr, RS_485_Add_EN)
  REG_BITS_DEF(24, 32, uart_ua_act_csr, ADDR_MATCH)
  REG_BITS_COMMON(uart_ua_act_csr)
};

/* class for bit field: UART0 Function Select Register. */
template<class T>
struct bits_uart_ua_fun_sel
{
  REG_BITS_DEF(0, 2, uart_ua_fun_sel, FUN_SEL)
  REG_BITS_COMMON(uart_ua_fun_sel)
};

#endif

#ifdef __cplusplus
extern "C" {
#endif

/* A/D Data Register 0 */
typedef union adc_addr_t_ {
  struct {
    _reg32_t RSLT           : 12;
    _reg32_t                : 4;
    _reg32_t OVERRUN        : 1;
    _reg32_t VALID          : 1;
    _reg32_t                : 14;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(adc_addr)
} adc_addr_t;

/* A/D Control Register */
typedef union adc_adcr_t_ {
  struct {
    _reg32_t ADEN           : 1;
    _reg32_t ADIE           : 1;
    _reg32_t ADMD           : 2;
    _reg32_t TRGS           : 2;
    _reg32_t TRGCOND        : 2;
    _reg32_t TRGE           : 1;
    _reg32_t                : 1;
    _reg32_t DIFFEN         : 1;
    _reg32_t ADST           : 1;
    _reg32_t                : 20;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(adc_adcr)
} adc_adcr_t;

/* A/D Channel Enable Register */
typedef union adc_adcher_t_ {
  struct {
    _reg32_t CHEN0          : 1;
    _reg32_t CHEN1          : 1;
    _reg32_t CHEN2          : 1;
    _reg32_t CHEN3          : 1;
    _reg32_t CHEN4          : 1;
    _reg32_t CHEN5          : 1;
    _reg32_t CHEN6          : 1;
    _reg32_t CHEN7          : 1;
    _reg32_t PRESEL         : 2;
    _reg32_t                : 22;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(adc_adcher)
} adc_adcher_t;

/* A/D Compare Register 0 */
typedef union adc_adcmpr_t_ {
  struct {
    _reg32_t CMPEN          : 1;
    _reg32_t CMPIE          : 1;
    _reg32_t CMPCOND        : 1;
    _reg32_t CMPCH          : 3;
    _reg32_t                : 2;
    _reg32_t CMPMATCNT      : 4;
    _reg32_t                : 4;
    _reg32_t CMPD           : 12;
    _reg32_t                : 4;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(adc_adcmpr)
} adc_adcmpr_t;

/* A/D Status Register */
typedef union adc_adsr_t_ {
  struct {
    _reg32_t ADF            : 1;
    _reg32_t CMPF0          : 1;
    _reg32_t CMPF1          : 1;
    _reg32_t BUSY           : 1;
    _reg32_t CHANNEL        : 3;
    _reg32_t                : 1;
    _reg32_t VALID          : 8;
    _reg32_t OVERRUN        : 8;
    _reg32_t                : 8;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(adc_adsr)
} adc_adsr_t;

/* A/D Calibration Register */
typedef union adc_adcalr_t_ {
  struct {
    _reg32_t CALEN          : 1;
    _reg32_t CALDONE        : 1;
    _reg32_t                : 30;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(adc_adcalr)
} adc_adcalr_t;

/* ADC: 0x400E0000 */
typedef struct {
  union { /*0x400E0000*/
    volatile adc_addr_t             ADDR0;
             adc_addr_t             ADDR0_;
  };
  union { /*0x400E0004*/
    volatile adc_addr_t             ADDR1;
             adc_addr_t             ADDR1_;
  };
  union { /*0x400E0008*/
    volatile adc_addr_t             ADDR2;
             adc_addr_t             ADDR2_;
  };
  union { /*0x400E000C*/
    volatile adc_addr_t             ADDR3;
             adc_addr_t             ADDR3_;
  };
  union { /*0x400E0010*/
    volatile adc_addr_t             ADDR4;
             adc_addr_t             ADDR4_;
  };
  union { /*0x400E0014*/
    volatile adc_addr_t             ADDR5;
             adc_addr_t             ADDR5_;
  };
  union { /*0x400E0018*/
    volatile adc_addr_t             ADDR6;
             adc_addr_t             ADDR6_;
  };
  union { /*0x400E001C*/
    volatile adc_addr_t             ADDR7;
             adc_addr_t             ADDR7_;
  };
  union { /*0x400E0020*/
    volatile adc_adcr_t             ADCR;
             adc_adcr_t             ADCR_;
  };
  union { /*0x400E0024*/
    volatile adc_adcher_t           ADCHER;
             adc_adcher_t           ADCHER_;
  };
  union { /*0x400E0028*/
    volatile adc_adcmpr_t           ADCMPR0;
             adc_adcmpr_t           ADCMPR0_;
  };
  union { /*0x400E002C*/
    volatile adc_adcmpr_t           ADCMPR1;
             adc_adcmpr_t           ADCMPR1_;
  };
  union { /*0x400E0030*/
    volatile adc_adsr_t             ADSR;
             adc_adsr_t             ADSR_;
  };
  union { /*0x400E0034*/
    volatile adc_adcalr_t           ADCALR;
             adc_adcalr_t           ADCALR_;
  };
} adc_t;

/* System Power Down Control Register */
typedef union clk_pwrcon_t_ {
  struct {
    _reg32_t XTL12M_EN      : 1;
    _reg32_t                : 1;
    _reg32_t OSC22M_EN      : 1;
    _reg32_t OSC10K_EN      : 1;
    _reg32_t PD_WU_DLY      : 1;
    _reg32_t PD_WU_INT_EN   : 1;
    _reg32_t PD_WU_STS      : 1;
    _reg32_t PWR_DOWN_EN    : 1;
    _reg32_t PD_WAIT_CPU    : 1;
    _reg32_t                : 23;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(clk_pwrcon)
} clk_pwrcon_t;

/* AHB Devices Clock Enable Control Register */
typedef union clk_ahbclk_t_ {
  struct {
    _reg32_t                : 2;
    _reg32_t ISP_EN         : 1;
    _reg32_t EBI_EN         : 1;
    _reg32_t                : 28;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(clk_ahbclk)
} clk_ahbclk_t;

/* APB Devices Clock Enable Control Register */
typedef union clk_apbclk_t_ {
  struct {
    _reg32_t WDT_EN         : 1;
    _reg32_t                : 1;
    _reg32_t TMR0_EN        : 1;
    _reg32_t TMR1_EN        : 1;
    _reg32_t TMR2_EN        : 1;
    _reg32_t TMR3_EN        : 1;
    _reg32_t FDIV_EN        : 1;
    _reg32_t                : 1;
    _reg32_t I2C_EN         : 1;
    _reg32_t                : 1;
    _reg32_t                : 2;
    _reg32_t SPI0_EN        : 1;
    _reg32_t SPI1_EN        : 1;
    _reg32_t                : 2;
    _reg32_t UART0_EN       : 1;
    _reg32_t UART1_EN       : 1;
    _reg32_t                : 2;
    _reg32_t PWM01_EN       : 1;
    _reg32_t PWM23_EN       : 1;
    _reg32_t PWM45_EN       : 1;
    _reg32_t PWM67_EN       : 1;
    _reg32_t                : 4;
    _reg32_t ADC_EN         : 1;
    _reg32_t                : 3;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(clk_apbclk)
} clk_apbclk_t;

/* Clock status monitor Register */
typedef union clk_clkstatus_t_ {
  struct {
    _reg32_t XTL12M_STB     : 1;
    _reg32_t                : 1;
    _reg32_t PLL_STB        : 1;
    _reg32_t OSC10K_STB     : 1;
    _reg32_t OSC22M_STB     : 1;
    _reg32_t                : 2;
    _reg32_t CLK_SW_FAIL    : 1;
    _reg32_t                : 24;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(clk_clkstatus)
} clk_clkstatus_t;

/* Clock Source Select Control Register 0 */
typedef union clk_clksel0_t_ {
  struct {
    _reg32_t HCLK_S         : 3;
    _reg32_t STCLK_S        : 3;
    _reg32_t                : 26;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(clk_clksel0)
} clk_clksel0_t;

/* Clock Source Select Control Register 1 */
typedef union clk_clksel1_t_ {
  struct {
    _reg32_t WDT_S          : 2;
    _reg32_t ADC_S          : 2;
    _reg32_t                : 4;
    _reg32_t TMR0_S         : 3;
    _reg32_t                : 1;
    _reg32_t TMR1_S         : 3;
    _reg32_t                : 1;
    _reg32_t TMR2_S         : 3;
    _reg32_t                : 1;
    _reg32_t TMR3_S         : 3;
    _reg32_t                : 1;
    _reg32_t UART_S         : 2;
    _reg32_t                : 2;
    _reg32_t PWM01_S        : 2;
    _reg32_t PWM23_S        : 2;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(clk_clksel1)
} clk_clksel1_t;

/* Clock Divider Number Register */
typedef union clk_clkdiv_t_ {
  struct {
    _reg32_t HCLK_N         : 4;
    _reg32_t                : 4;
    _reg32_t UART_N         : 4;
    _reg32_t                : 4;
    _reg32_t ADC_N          : 8;
    _reg32_t                : 8;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(clk_clkdiv)
} clk_clkdiv_t;

/* Clock Source Select Control Register 2 */
typedef union clk_clksel2_t_ {
  struct {
    _reg32_t                : 2;
    _reg32_t FRQDIV_S       : 2;
    _reg32_t PWM45_S        : 2;
    _reg32_t PWM67_S        : 2;
    _reg32_t                : 24;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(clk_clksel2)
} clk_clksel2_t;

/* PLL Control Register */
typedef union clk_pllcon_t_ {
  struct {
    _reg32_t FB_DV          : 9;
    _reg32_t IN_DV          : 5;
    _reg32_t OUT_DV         : 2;
    _reg32_t PD             : 1;
    _reg32_t BP             : 1;
    _reg32_t OE             : 1;
    _reg32_t PLL_SRC        : 1;
    _reg32_t                : 12;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(clk_pllcon)
} clk_pllcon_t;

/* Frequency Divider Control Register */
typedef union clk_frqdiv_t_ {
  struct {
    _reg32_t FSEL           : 4;
    _reg32_t DIVIDER_EN     : 1;
    _reg32_t                : 27;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(clk_frqdiv)
} clk_frqdiv_t;

/* CLK: 0x50000200 */
typedef struct {
  union { /*0x50000200*/
    volatile clk_pwrcon_t           PWRCON;
             clk_pwrcon_t           PWRCON_;
  };
  union { /*0x50000204*/
    volatile clk_ahbclk_t           AHBCLK;
             clk_ahbclk_t           AHBCLK_;
  };
  union { /*0x50000208*/
    volatile clk_apbclk_t           APBCLK;
             clk_apbclk_t           APBCLK_;
  };
  union { /*0x5000020C*/
    volatile clk_clkstatus_t        CLKSTATUS;
             clk_clkstatus_t        CLKSTATUS_;
  };
  union { /*0x50000210*/
    volatile clk_clksel0_t          CLKSEL0;
             clk_clksel0_t          CLKSEL0_;
  };
  union { /*0x50000214*/
    volatile clk_clksel1_t          CLKSEL1;
             clk_clksel1_t          CLKSEL1_;
  };
  union { /*0x50000218*/
    volatile clk_clkdiv_t           CLKDIV;
             clk_clkdiv_t           CLKDIV_;
  };
  union { /*0x5000021C*/
    volatile clk_clksel2_t          CLKSEL2;
             clk_clksel2_t          CLKSEL2_;
  };
  union { /*0x50000220*/
    volatile clk_pllcon_t           PLLCON;
             clk_pllcon_t           PLLCON_;
  };
  union { /*0x50000224*/
    volatile clk_frqdiv_t           FRQDIV;
             clk_frqdiv_t           FRQDIV_;
  };
} clk_t;

/* External Bus Interface General Control Register */
typedef union ebi_ctl_ebicon_t_ {
  struct {
    _reg32_t ExtEN          : 1;
    _reg32_t ExtBW16        : 1;
    _reg32_t                : 6;
    _reg32_t MCLKDIV        : 3;
    _reg32_t                : 5;
    _reg32_t ExttALE        : 3;
    _reg32_t                : 13;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(ebi_ctl_ebicon)
} ebi_ctl_ebicon_t;

/* External Bus Interface 0 Timing Control Register */
typedef union ebi_ctl_extime_t_ {
  struct {
    _reg32_t                : 3;
    _reg32_t ExttACC        : 5;
    _reg32_t ExttAHD        : 3;
    _reg32_t                : 1;
    _reg32_t ExtIW2X        : 4;
    _reg32_t                : 8;
    _reg32_t ExtIR2R        : 4;
    _reg32_t                : 4;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(ebi_ctl_extime)
} ebi_ctl_extime_t;

/* EBI_CTL: 0x50010000 */
typedef struct {
  union { /*0x50010000*/
    volatile ebi_ctl_ebicon_t       EBICON;
             ebi_ctl_ebicon_t       EBICON_;
  };
  union { /*0x50010004*/
    volatile ebi_ctl_extime_t       EXTIME;
             ebi_ctl_extime_t       EXTIME_;
  };
} ebi_ctl_t;

/* ISP Control Register */
typedef union fmc_ispcon_t_ {
  struct {
    _reg32_t ISPEN          : 1;
    _reg32_t BS             : 1;
    _reg32_t                : 2;
    _reg32_t CFGUEN         : 1;
    _reg32_t LDUEN          : 1;
    _reg32_t ISPFF          : 1;
    _reg32_t SWRST          : 1;
    _reg32_t PT             : 3;
    _reg32_t                : 1;
    _reg32_t ET             : 3;
    _reg32_t                : 17;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(fmc_ispcon)
} fmc_ispcon_t;

/* ISP Command Register */
typedef union fmc_ispcmd_t_ {
  struct {
    _reg32_t FOEN_FCEN_FCTRL: 6;
    _reg32_t                : 26;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(fmc_ispcmd)
} fmc_ispcmd_t;

/* ISP Trigger Control Register */
typedef union fmc_isptrg_t_ {
  struct {
    _reg32_t ISPGO          : 1;
    _reg32_t                : 31;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(fmc_isptrg)
} fmc_isptrg_t;

/* Flash Access Time Control Register */
typedef union fmc_fatcon_t_ {
  struct {
    _reg32_t FPSEN          : 1;
    _reg32_t FATS           : 3;
    _reg32_t L_SPEED        : 1;
    _reg32_t                : 27;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(fmc_fatcon)
} fmc_fatcon_t;

/* FMC: 0x5000C000 */
typedef struct {
  union { /*0x5000C000*/
    volatile fmc_ispcon_t           ISPCON;
             fmc_ispcon_t           ISPCON_;
  };
  union { /*0x5000C004*/
    volatile _reg32_t               ISPADR;
             _reg32_t               ISPADR_;
  };
  union { /*0x5000C008*/
    volatile _reg32_t               ISPDAT;
             _reg32_t               ISPDAT_;
  };
  union { /*0x5000C00C*/
    volatile fmc_ispcmd_t           ISPCMD;
             fmc_ispcmd_t           ISPCMD_;
  };
  union { /*0x5000C010*/
    volatile fmc_isptrg_t           ISPTRG;
             fmc_isptrg_t           ISPTRG_;
  };
  union { /*0x5000C014*/
    volatile _reg32_t               DFBADR;
             _reg32_t               DFBADR_;
  };
  union { /*0x5000C018*/
    volatile fmc_fatcon_t           FATCON;
             fmc_fatcon_t           FATCON_;
  };
} fmc_t;

/* System Reset Source Register */
typedef union gcr_rstsrc_t_ {
  struct {
    _reg32_t RSTS_POR       : 1;
    _reg32_t RSTS_RESET     : 1;
    _reg32_t RSTS_WDT       : 1;
    _reg32_t RSTS_LVR       : 1;
    _reg32_t RSTS_BOD       : 1;
    _reg32_t RSTS_MCU       : 1;
    _reg32_t                : 1;
    _reg32_t RSTS_CPU       : 1;
    _reg32_t                : 24;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(gcr_rstsrc)
} gcr_rstsrc_t;

/* IP Reset Control Resister1 */
typedef union gcr_iprstc1_t_ {
  struct {
    _reg32_t CHIP_RST       : 1;
    _reg32_t CPU_RST        : 1;
    _reg32_t                : 1;
    _reg32_t EBI_RST        : 1;
    _reg32_t                : 28;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(gcr_iprstc1)
} gcr_iprstc1_t;

/* IP Reset Control Resister 2 */
typedef union gcr_iprstc2_t_ {
  struct {
    _reg32_t                : 1;
    _reg32_t GPIO_RST       : 1;
    _reg32_t TMR0_RST       : 1;
    _reg32_t TMR1_RST       : 1;
    _reg32_t TMR2_RST       : 1;
    _reg32_t TMR3_RST       : 1;
    _reg32_t                : 2;
    _reg32_t I2C_RST        : 1;
    _reg32_t                : 1;
    _reg32_t                : 2;
    _reg32_t SPI0_RST       : 1;
    _reg32_t SPI1_RST       : 1;
    _reg32_t                : 2;
    _reg32_t UART0_RST      : 1;
    _reg32_t UART1_RST      : 1;
    _reg32_t                : 2;
    _reg32_t PWM03_RST      : 1;
    _reg32_t PWM47_RST      : 1;
    _reg32_t                : 6;
    _reg32_t ADC_RST        : 1;
    _reg32_t                : 3;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(gcr_iprstc2)
} gcr_iprstc2_t;

/* Brown Out Detector Control Register */
typedef union gcr_bodcr_t_ {
  struct {
    _reg32_t BOD_EN         : 1;
    _reg32_t BOD_VL         : 2;
    _reg32_t BOD_RSTEN      : 1;
    _reg32_t BOD_INTF       : 1;
    _reg32_t BOD_LPM        : 1;
    _reg32_t BOD_OUT        : 1;
    _reg32_t LVR_EN         : 1;
    _reg32_t                : 24;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(gcr_bodcr)
} gcr_bodcr_t;

/* Power-On-Reset Controller Register */
typedef union gcr_porcr_t_ {
  struct {
    _reg32_t POR_DIS_CODE   : 16;
    _reg32_t                : 16;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(gcr_porcr)
} gcr_porcr_t;

/* P0 multiple function and input type control register */
typedef union gcr_p0_mfp_t_ {
  struct {
    _reg32_t P0_MFP         : 8;
    _reg32_t P0_ALT0        : 1;
    _reg32_t P0_ALT1        : 1;
    _reg32_t P0_ALT2        : 1;
    _reg32_t P0_ALT3        : 1;
    _reg32_t P0_ALT4        : 1;
    _reg32_t P0_ALT5        : 1;
    _reg32_t P0_ALT6        : 1;
    _reg32_t P0_ALT7        : 1;
    _reg32_t P0_TYPEn       : 8;
    _reg32_t                : 8;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(gcr_p0_mfp)
} gcr_p0_mfp_t;

/* P1 multiple function and input type control register */
typedef union gcr_p1_mfp_t_ {
  struct {
    _reg32_t P1_MFP         : 8;
    _reg32_t P1_ALT0        : 1;
    _reg32_t P1_ALT1        : 1;
    _reg32_t P1_ALT2        : 1;
    _reg32_t P1_ALT3        : 1;
    _reg32_t P1_ALT4        : 1;
    _reg32_t P1_ALT5        : 1;
    _reg32_t P1_ALT6        : 1;
    _reg32_t P1_ALT7        : 1;
    _reg32_t P1_TYPEn       : 8;
    _reg32_t                : 8;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(gcr_p1_mfp)
} gcr_p1_mfp_t;

/* P2 multiple function and input type control register */
typedef union gcr_p2_mfp_t_ {
  struct {
    _reg32_t P2_MFP         : 8;
    _reg32_t P2_ALT0        : 1;
    _reg32_t P2_ALT1        : 1;
    _reg32_t P2_ALT2        : 1;
    _reg32_t P2_ALT3        : 1;
    _reg32_t P2_ALT4        : 1;
    _reg32_t P2_ALT5        : 1;
    _reg32_t P2_ALT6        : 1;
    _reg32_t P2_ALT7        : 1;
    _reg32_t P2_TYPEn       : 8;
    _reg32_t                : 8;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(gcr_p2_mfp)
} gcr_p2_mfp_t;

/* P3 multiple function and input type control register */
typedef union gcr_p3_mfp_t_ {
  struct {
    _reg32_t P3_MFP         : 8;
    _reg32_t P3_ALT0        : 1;
    _reg32_t P3_ALT1        : 1;
    _reg32_t P3_ALT2        : 1;
    _reg32_t P3_ALT3        : 1;
    _reg32_t P3_ALT4        : 1;
    _reg32_t P3_ALT5        : 1;
    _reg32_t P3_ALT6        : 1;
    _reg32_t P3_ALT7        : 1;
    _reg32_t P3_TYPEn       : 8;
    _reg32_t                : 8;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(gcr_p3_mfp)
} gcr_p3_mfp_t;

/* P4 input type control register */
typedef union gcr_p4_mfp_t_ {
  struct {
    _reg32_t P4_MFP         : 8;
    _reg32_t P4_ALT0        : 1;
    _reg32_t P4_ALT1        : 1;
    _reg32_t P4_ALT2        : 1;
    _reg32_t P4_ALT3        : 1;
    _reg32_t P4_ALT4        : 1;
    _reg32_t P4_ALT5        : 1;
    _reg32_t P4_ALT6        : 1;
    _reg32_t P4_ALT7        : 1;
    _reg32_t P4_TYPEn       : 8;
    _reg32_t                : 8;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(gcr_p4_mfp)
} gcr_p4_mfp_t;

/* Register Lock Key address */
typedef union gcr_regwrprot_t_ {
  struct {
    union
    {
      _reg32_t REGWRPROT    : 8;
      _reg32_t REGPROTDIS   : 1;
    };
    _reg32_t                : 24;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(gcr_regwrprot)
} gcr_regwrprot_t;

/* GCR: 0x50000000 */
typedef struct {
  union { /*0x50000000*/
    volatile _reg32_t               PDID;
             _reg32_t               PDID_;
  };
  union { /*0x50000004*/
    volatile gcr_rstsrc_t           RSTSRC;
             gcr_rstsrc_t           RSTSRC_;
  };
  union { /*0x50000008*/
    volatile gcr_iprstc1_t          IPRSTC1;
             gcr_iprstc1_t          IPRSTC1_;
  };
  union { /*0x5000000C*/
    volatile gcr_iprstc2_t          IPRSTC2;
             gcr_iprstc2_t          IPRSTC2_;
  };
  _reg32_t                          _dummy0[0x2];
  union { /*0x50000018*/
    volatile gcr_bodcr_t            BODCR;
             gcr_bodcr_t            BODCR_;
  };
  _reg32_t                          _dummy1[0x2];
  union { /*0x50000024*/
    volatile gcr_porcr_t            PORCR;
             gcr_porcr_t            PORCR_;
  };
  _reg32_t                          _dummy2[0x2];
  union { /*0x50000030*/
    volatile gcr_p0_mfp_t           P0_MFP;
             gcr_p0_mfp_t           P0_MFP_;
  };
  union { /*0x50000034*/
    volatile gcr_p1_mfp_t           P1_MFP;
             gcr_p1_mfp_t           P1_MFP_;
  };
  union { /*0x50000038*/
    volatile gcr_p2_mfp_t           P2_MFP;
             gcr_p2_mfp_t           P2_MFP_;
  };
  union { /*0x5000003C*/
    volatile gcr_p3_mfp_t           P3_MFP;
             gcr_p3_mfp_t           P3_MFP_;
  };
  union { /*0x50000040*/
    volatile gcr_p4_mfp_t           P4_MFP;
             gcr_p4_mfp_t           P4_MFP_;
  };
  _reg32_t                          _dummy3[0x2F];
  union { /*0x50000100*/
    volatile gcr_regwrprot_t        REGWRPROT;
             gcr_regwrprot_t        REGWRPROT_;
  };
} gcr_t;

/* Bit Mode Control */
typedef union gpio_pmd_t_ {
  struct {
    _reg32_t PMD0           : 2;
    _reg32_t PMD1           : 2;
    _reg32_t PMD2           : 2;
    _reg32_t PMD3           : 2;
    _reg32_t PMD4           : 2;
    _reg32_t PMD5           : 2;
    _reg32_t PMD6           : 2;
    _reg32_t PMD7           : 2;
    _reg32_t                : 16;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(gpio_pmd)
} gpio_pmd_t;

/* Bit OFF Digital Enable */
typedef union gpio_offd_t_ {
  struct {
    _reg32_t                : 16;
    _reg32_t OFFD           : 8;
    _reg32_t                : 8;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(gpio_offd)
} gpio_offd_t;

/* Data Output Value */
typedef union gpio_dout_t_ {
  struct {
    _reg32_t DOUT0          : 1;
    _reg32_t DOUT1          : 1;
    _reg32_t DOUT2          : 1;
    _reg32_t DOUT3          : 1;
    _reg32_t DOUT4          : 1;
    _reg32_t DOUT5          : 1;
    _reg32_t DOUT6          : 1;
    _reg32_t DOUT7          : 1;
    _reg32_t                : 24;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(gpio_dout)
} gpio_dout_t;

/* Data Output Write Mask */
typedef union gpio_dmask_t_ {
  struct {
    _reg32_t DMASK0         : 1;
    _reg32_t DMASK1         : 1;
    _reg32_t DMASK2         : 1;
    _reg32_t DMASK3         : 1;
    _reg32_t DMASK4         : 1;
    _reg32_t DMASK5         : 1;
    _reg32_t DMASK6         : 1;
    _reg32_t DMASK7         : 1;
    _reg32_t                : 24;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(gpio_dmask)
} gpio_dmask_t;

/* Pin Value */
typedef union gpio_pin_t_ {
  struct {
    _reg32_t PIN0           : 1;
    _reg32_t PIN1           : 1;
    _reg32_t PIN2           : 1;
    _reg32_t PIN3           : 1;
    _reg32_t PIN4           : 1;
    _reg32_t PIN5           : 1;
    _reg32_t PIN6           : 1;
    _reg32_t PIN7           : 1;
    _reg32_t                : 24;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(gpio_pin)
} gpio_pin_t;

/* De-bounce Enable */
typedef union gpio_dben_t_ {
  struct {
    _reg32_t DBEN0          : 1;
    _reg32_t DBEN1          : 1;
    _reg32_t DBEN2          : 1;
    _reg32_t DBEN3          : 1;
    _reg32_t DBEN4          : 1;
    _reg32_t DBEN5          : 1;
    _reg32_t DBEN6          : 1;
    _reg32_t DBEN7          : 1;
    _reg32_t                : 24;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(gpio_dben)
} gpio_dben_t;

/* Interrupt Mode Control */
typedef union gpio_imd_t_ {
  struct {
    _reg32_t IMD0           : 1;
    _reg32_t IMD1           : 1;
    _reg32_t IMD2           : 1;
    _reg32_t IMD3           : 1;
    _reg32_t IMD4           : 1;
    _reg32_t IMD5           : 1;
    _reg32_t IMD6           : 1;
    _reg32_t IMD7           : 1;
    _reg32_t                : 24;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(gpio_imd)
} gpio_imd_t;

/* Interrupt Enable */
typedef union gpio_ien_t_ {
  struct {
    _reg32_t IF_EN0         : 1;
    _reg32_t IF_EN1         : 1;
    _reg32_t IF_EN2         : 1;
    _reg32_t IF_EN3         : 1;
    _reg32_t IF_EN4         : 1;
    _reg32_t IF_EN5         : 1;
    _reg32_t IF_EN6         : 1;
    _reg32_t IF_EN7         : 1;
    _reg32_t                : 8;
    _reg32_t IR_EN0         : 1;
    _reg32_t IR_EN1         : 1;
    _reg32_t IR_EN2         : 1;
    _reg32_t IR_EN3         : 1;
    _reg32_t IR_EN4         : 1;
    _reg32_t IR_EN5         : 1;
    _reg32_t IR_EN6         : 1;
    _reg32_t IR_EN7         : 1;
    _reg32_t                : 8;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(gpio_ien)
} gpio_ien_t;

/* Interrupt Trigger Source */
typedef union gpio_isrc_t_ {
  struct {
    _reg32_t ISRC0          : 1;
    _reg32_t ISRC1          : 1;
    _reg32_t ISRC2          : 1;
    _reg32_t ISRC3          : 1;
    _reg32_t ISRC4          : 1;
    _reg32_t ISRC5          : 1;
    _reg32_t ISRC6          : 1;
    _reg32_t ISRC7          : 1;
    _reg32_t                : 24;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(gpio_isrc)
} gpio_isrc_t;

/* GP0: 0x50004000 */
/* GP1: 0x50004040 */
/* GP2: 0x50004080 */
/* GP3: 0x500040C0 */
/* GP4: 0x50004100 */
typedef struct {
  union { /*base+0x00000000*/
    volatile gpio_pmd_t             PMD;
             gpio_pmd_t             PMD_;
  };
  union { /*base+0x00000004*/
    volatile gpio_offd_t            OFFD;
             gpio_offd_t            OFFD_;
  };
  union { /*base+0x00000008*/
    volatile gpio_dout_t            DOUT;
             gpio_dout_t            DOUT_;
  };
  union { /*base+0x0000000C*/
    volatile gpio_dmask_t           DMASK;
             gpio_dmask_t           DMASK_;
  };
  union { /*base+0x00000010*/
    volatile gpio_pin_t             PIN;
             gpio_pin_t             PIN_;
  };
  union { /*base+0x00000014*/
    volatile gpio_dben_t            DBEN;
             gpio_dben_t            DBEN_;
  };
  union { /*base+0x00000018*/
    volatile gpio_imd_t             IMD;
             gpio_imd_t             IMD_;
  };
  union { /*base+0x0000001C*/
    volatile gpio_ien_t             IEN;
             gpio_ien_t             IEN_;
  };
  union { /*base+0x00000020*/
    volatile gpio_isrc_t            ISRC;
             gpio_isrc_t            ISRC_;
  };
} gpio_t;

/* Interrupt De-bounce Cycle Control */
typedef union gpio_gcr_dbncecon_t_ {
  struct {
    _reg32_t DBCLKSEL       : 4;
    _reg32_t DBCLKSRC       : 1;
    _reg32_t ICLK_ON        : 1;
    _reg32_t                : 26;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(gpio_gcr_dbncecon)
} gpio_gcr_dbncecon_t;

/* GPIO: 0x50004180 */
typedef struct {
  union { /*0x50004180*/
    volatile gpio_gcr_dbncecon_t    DBNCECON;
             gpio_gcr_dbncecon_t    DBNCECON_;
  };
} gpio_gcr_t;

/* Px.0 Data Output Value */
typedef union gpio_bits_px_dout_t_ {
  struct {
    _reg32_t DOUT           : 1;
    _reg32_t                : 31;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(gpio_bits_px_dout)
} gpio_bits_px_dout_t;

/* GP0_BITS: 0x50004200 */
/* GP1_BITS: 0x50004220 */
/* GP2_BITS: 0x50004240 */
/* GP3_BITS: 0x50004260 */
/* GP4_BITS: 0x50004280 */
typedef struct {
  _reg32_t                          _dummy0[0x80];
  union { /*base+0x00000200*/
    volatile gpio_bits_px_dout_t    Px0_DOUT;
             gpio_bits_px_dout_t    Px0_DOUT_;
  };
  union { /*base+0x00000204*/
    volatile gpio_bits_px_dout_t    Px1_DOUT;
             gpio_bits_px_dout_t    Px1_DOUT_;
  };
  union { /*base+0x00000208*/
    volatile gpio_bits_px_dout_t    Px2_DOUT;
             gpio_bits_px_dout_t    Px2_DOUT_;
  };
  union { /*base+0x0000020C*/
    volatile gpio_bits_px_dout_t    Px3_DOUT;
             gpio_bits_px_dout_t    Px3_DOUT_;
  };
  union { /*base+0x00000210*/
    volatile gpio_bits_px_dout_t    Px4_DOUT;
             gpio_bits_px_dout_t    Px4_DOUT_;
  };
  union { /*base+0x00000214*/
    volatile gpio_bits_px_dout_t    Px5_DOUT;
             gpio_bits_px_dout_t    Px5_DOUT_;
  };
  union { /*base+0x00000218*/
    volatile gpio_bits_px_dout_t    Px6_DOUT;
             gpio_bits_px_dout_t    Px6_DOUT_;
  };
  union { /*base+0x0000021C*/
    volatile gpio_bits_px_dout_t    Px7_DOUT;
             gpio_bits_px_dout_t    Px7_DOUT_;
  };
} gpio_bits_t;

/* I2C Control Register */
typedef union i2c_i2con_t_ {
  struct {
    _reg32_t                : 2;
    _reg32_t AA             : 1;
    _reg32_t SI             : 1;
    _reg32_t STO            : 1;
    _reg32_t STA            : 1;
    _reg32_t ENSI           : 1;
    _reg32_t EI             : 1;
    _reg32_t                : 24;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(i2c_i2con)
} i2c_i2con_t;

/* I2C slave Address Register0 */
typedef union i2c_i2caddr_t_ {
  struct {
    _reg32_t GC             : 1;
    _reg32_t I2CADDR        : 7;
    _reg32_t                : 24;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(i2c_i2caddr)
} i2c_i2caddr_t;

/* I2C DATA Register */
typedef union i2c_i2cdat_t_ {
  struct {
    _reg32_t I2CDAT         : 8;
    _reg32_t                : 24;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(i2c_i2cdat)
} i2c_i2cdat_t;

/* I2C Status Register */
typedef union i2c_i2cstatus_t_ {
  struct {
    _reg32_t I2CSTATUS      : 8;
    _reg32_t                : 24;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(i2c_i2cstatus)
} i2c_i2cstatus_t;

/* I2C clock divided Register */
typedef union i2c_i2clk_t_ {
  struct {
    _reg32_t I2CLK          : 8;
    _reg32_t                : 24;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(i2c_i2clk)
} i2c_i2clk_t;

/* I2C Time out control Register */
typedef union i2c_i2ctoc_t_ {
  struct {
    _reg32_t TIF            : 1;
    _reg32_t DIV4           : 1;
    _reg32_t ENTI           : 1;
    _reg32_t                : 29;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(i2c_i2ctoc)
} i2c_i2ctoc_t;

/* I2C Slave address Mask Register0 */
typedef union i2c_i2cadm_t_ {
  struct {
    _reg32_t                : 1;
    _reg32_t I2ADMx         : 7;
    _reg32_t                : 24;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(i2c_i2cadm)
} i2c_i2cadm_t;

/* I2C: 0x40020000 */
typedef struct {
  union { /*0x40020000*/
    volatile i2c_i2con_t            I2CON;
             i2c_i2con_t            I2CON_;
  };
  union { /*0x40020004*/
    volatile i2c_i2caddr_t          I2CADDR0;
             i2c_i2caddr_t          I2CADDR0_;
  };
  union { /*0x40020008*/
    volatile i2c_i2cdat_t           I2CDAT;
             i2c_i2cdat_t           I2CDAT_;
  };
  union { /*0x4002000C*/
    volatile i2c_i2cstatus_t        I2CSTATUS;
             i2c_i2cstatus_t        I2CSTATUS_;
  };
  union { /*0x40020010*/
    volatile i2c_i2clk_t            I2CLK;
             i2c_i2clk_t            I2CLK_;
  };
  union { /*0x40020014*/
    volatile i2c_i2ctoc_t           I2CTOC;
             i2c_i2ctoc_t           I2CTOC_;
  };
  union { /*0x40020018*/
    volatile i2c_i2caddr_t          I2CADDR1;
             i2c_i2caddr_t          I2CADDR1_;
  };
  union { /*0x4002001C*/
    volatile i2c_i2caddr_t          I2CADDR2;
             i2c_i2caddr_t          I2CADDR2_;
  };
  union { /*0x40020020*/
    volatile i2c_i2caddr_t          I2CADDR3;
             i2c_i2caddr_t          I2CADDR3_;
  };
  union { /*0x40020024*/
    volatile i2c_i2cadm_t           I2CADM0;
             i2c_i2cadm_t           I2CADM0_;
  };
  union { /*0x40020028*/
    volatile i2c_i2cadm_t           I2CADM1;
             i2c_i2cadm_t           I2CADM1_;
  };
  union { /*0x4002002C*/
    volatile i2c_i2cadm_t           I2CADM2;
             i2c_i2cadm_t           I2CADM2_;
  };
  union { /*0x40020030*/
    volatile i2c_i2cadm_t           I2CADM3;
             i2c_i2cadm_t           I2CADM3_;
  };
} i2c_t;

/* MCU IRQ0 (BOD) interrupt source identify */
typedef union int_irq_src_t_ {
  struct {
    _reg32_t INT_SRC        : 3;
    _reg32_t                : 29;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(int_irq_src)
} int_irq_src_t;


/* NMI source interrupt select control register */
typedef union int_nmi_sel_t_ {
  struct {
    _reg32_t NMI_SEL        : 5;
    _reg32_t                : 27;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(int_nmi_sel)
} int_nmi_sel_t;

/* INT: 0x50000300 */
typedef struct {
  union { /*0x50000300*/
    volatile int_irq_src_t          IRQ0_SRC;
             int_irq_src_t          IRQ0_SRC_;
  };
  union { /*0x50000304*/
    volatile int_irq_src_t          IRQ1_SRC;
             int_irq_src_t          IRQ1_SRC_;
  };
  union { /*0x50000308*/
    volatile int_irq_src_t          IRQ2_SRC;
             int_irq_src_t          IRQ2_SRC_;
  };
  union { /*0x5000030C*/
    volatile int_irq_src_t          IRQ3_SRC;
             int_irq_src_t          IRQ3_SRC_;
  };
  union { /*0x50000310*/
    volatile int_irq_src_t          IRQ4_SRC;
             int_irq_src_t          IRQ4_SRC_;
  };
  union { /*0x50000314*/
    volatile int_irq_src_t          IRQ5_SRC;
             int_irq_src_t          IRQ5_SRC_;
  };
  union { /*0x50000318*/
    volatile int_irq_src_t          IRQ6_SRC;
             int_irq_src_t          IRQ6_SRC_;
  };
  union { /*0x5000031C*/
    volatile int_irq_src_t          IRQ7_SRC;
             int_irq_src_t          IRQ7_SRC_;
  };
  union { /*0x50000320*/
    volatile int_irq_src_t          IRQ8_SRC;
             int_irq_src_t          IRQ8_SRC_;
  };
  union { /*0x50000324*/
    volatile int_irq_src_t          IRQ9_SRC;
             int_irq_src_t          IRQ9_SRC_;
  };
  union { /*0x50000328*/
    volatile int_irq_src_t          IRQ10_SRC;
             int_irq_src_t          IRQ10_SRC_;
  };
  union { /*0x5000032C*/
    volatile int_irq_src_t          IRQ11_SRC;
             int_irq_src_t          IRQ11_SRC_;
  };
  union { /*0x50000330*/
    volatile int_irq_src_t          IRQ12_SRC;
             int_irq_src_t          IRQ12_SRC_;
  };
  union { /*0x50000334*/
    volatile int_irq_src_t          IRQ13_SRC;
             int_irq_src_t          IRQ13_SRC_;
  };
  union { /*0x50000338*/
    volatile int_irq_src_t          IRQ14_SRC;
             int_irq_src_t          IRQ14_SRC_;
  };
  union { /*0x5000033C*/
    volatile int_irq_src_t          IRQ15_SRC;
             int_irq_src_t          IRQ15_SRC_;
  };
  _reg32_t                          _dummy0[0x2];
  union { /*0x50000348*/
    volatile int_irq_src_t          IRQ18_SRC;
             int_irq_src_t          IRQ18_SRC_;
  };
  _reg32_t                          _dummy1[0x2];
  union { /*0x50000354*/
    volatile int_irq_src_t          IRQ21_SRC;
             int_irq_src_t          IRQ21_SRC_;
  };
  union { /*0x50000358*/
    volatile int_irq_src_t          IRQ22_SRC;
             int_irq_src_t          IRQ22_SRC_;
  };
  _reg32_t                          _dummy2[0x5];
  union { /*0x50000370*/
    volatile int_irq_src_t          IRQ28_SRC;
             int_irq_src_t          IRQ28_SRC_;
  };
  union { /*0x50000374*/
    volatile int_irq_src_t          IRQ29_SRC;
             int_irq_src_t          IRQ29_SRC_;
  };
  union { /*0x50000378*/
    volatile int_irq_src_t          IRQ30_SRC;
             int_irq_src_t          IRQ30_SRC_;
  };
  _reg32_t                          _dummy3[0x1];
  union { /*0x50000380*/
    volatile int_nmi_sel_t          NMI_SEL;
             int_nmi_sel_t          NMI_SEL_;
  };
  union { /*0x50000384*/
    volatile _reg32_t               MCU_IRQ;
             _reg32_t               MCU_IRQ_;
  };
} int_t;

/* PWM Pre-scalar Register */
typedef union pwm_ppr_t_ {
  struct {
    _reg32_t CP01           : 8;
    _reg32_t CP23           : 8;
    _reg32_t DZI01          : 8;
    _reg32_t DZI23          : 8;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(pwm_ppr)
} pwm_ppr_t;

/* PWM Clock Select Register */
typedef union pwm_csr_t_ {
  struct {
    _reg32_t CSR0           : 3;
    _reg32_t                : 1;
    _reg32_t CSR1           : 3;
    _reg32_t                : 1;
    _reg32_t CSR2           : 3;
    _reg32_t                : 1;
    _reg32_t CSR3           : 3;
    _reg32_t                : 17;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(pwm_csr)
} pwm_csr_t;

/* PWM Control Register */
typedef union pwm_pcr_t_ {
  struct {
    _reg32_t CH0EN          : 1;
    _reg32_t                : 1;
    _reg32_t CH0INV         : 1;
    _reg32_t CH0MOD         : 1;
    _reg32_t DZEN01         : 1;
    _reg32_t DZEN23         : 1;
    _reg32_t                : 2;
    _reg32_t CH1EN          : 1;
    _reg32_t                : 1;
    _reg32_t CH1INV         : 1;
    _reg32_t CH1MOD         : 1;
    _reg32_t                : 4;
    _reg32_t CH2EN          : 1;
    _reg32_t                : 1;
    _reg32_t CH2INV         : 1;
    _reg32_t CH2MOD         : 1;
    _reg32_t                : 4;
    _reg32_t CH3EN          : 1;
    _reg32_t                : 1;
    _reg32_t CH3INV         : 1;
    _reg32_t CH3MOD         : 1;
    _reg32_t                : 4;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(pwm_pcr)
} pwm_pcr_t;

/* PWM Counter Register 0 */
typedef union pwm_cnr_t_ {
  struct {
    _reg32_t CNR            : 16;
    _reg32_t                : 16;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(pwm_cnr)
} pwm_cnr_t;

/* PWM Comparator Register 0 */
typedef union pwm_cmr_t_ {
  struct {
    _reg32_t CMR            : 16;
    _reg32_t                : 16;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(pwm_cmr)
} pwm_cmr_t;

/* PWM Data Register 0 */
typedef union pwm_pdr_t_ {
  struct {
    _reg32_t PDR            : 16;
    _reg32_t                : 16;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(pwm_pdr)
} pwm_pdr_t;

/* PWM Interrupt Enable Register */
typedef union pwm_pier_t_ {
  struct {
    _reg32_t PWMIE0         : 1;
    _reg32_t PWMIE1         : 1;
    _reg32_t PWMIE2         : 1;
    _reg32_t PWMIE3         : 1;
    _reg32_t                : 28;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(pwm_pier)
} pwm_pier_t;

/* PWM Interrupt Indication Register */
typedef union pwm_piir_t_ {
  struct {
    _reg32_t PWMIF0         : 1;
    _reg32_t PWMIF1         : 1;
    _reg32_t PWMIF2         : 1;
    _reg32_t PWMIF3         : 1;
    _reg32_t                : 28;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(pwm_piir)
} pwm_piir_t;

/* Capture Control Register */
typedef union pwm_ccr0_t_ {
  struct {
    _reg32_t INV0           : 1;
    _reg32_t CRL_IE0        : 1;
    _reg32_t CFL_IE0        : 1;
    _reg32_t CAPCH0EN       : 1;
    _reg32_t CAPIF0         : 1;
    _reg32_t                : 1;
    _reg32_t CRLRI0         : 1;
    _reg32_t CFLRI0         : 1;
    _reg32_t                : 8;
    _reg32_t INV1           : 1;
    _reg32_t CRL_IE1        : 1;
    _reg32_t CFL_IE1        : 1;
    _reg32_t CAPCH1EN       : 1;
    _reg32_t CAPIF1         : 1;
    _reg32_t                : 1;
    _reg32_t CRLRI1         : 1;
    _reg32_t CFLRI1         : 1;
    _reg32_t                : 8;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(pwm_ccr0)
} pwm_ccr0_t;

/* Capture Control Register */
typedef union pwm_ccr2_t_ {
  struct {
    _reg32_t INV2           : 1;
    _reg32_t CRL_IE2        : 1;
    _reg32_t CFL_IE2        : 1;
    _reg32_t CAPCH2EN       : 1;
    _reg32_t CAPIF2         : 1;
    _reg32_t                : 1;
    _reg32_t CRLRI2         : 1;
    _reg32_t CFLRI2         : 1;
    _reg32_t                : 8;
    _reg32_t INV3           : 1;
    _reg32_t CRL_IE3        : 1;
    _reg32_t CFL_IE3        : 1;
    _reg32_t CAPCH3EN       : 1;
    _reg32_t CAPIF3         : 1;
    _reg32_t                : 1;
    _reg32_t CRLRI3         : 1;
    _reg32_t CFLRI3         : 1;
    _reg32_t                : 8;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(pwm_ccr2)
} pwm_ccr2_t;

/* Capture Rising Latch Register (Channel 0) */
typedef union pwm_crlr_t_ {
  struct {
    _reg32_t CRLR           : 16;
    _reg32_t                : 16;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(pwm_crlr)
} pwm_crlr_t;

/* Capture Falling Latch Register (Channel 0) */
typedef union pwm_cflr_t_ {
  struct {
    _reg32_t CFLR           : 16;
    _reg32_t                : 16;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(pwm_cflr)
} pwm_cflr_t;

/* Capture Input Enable Register */
typedef union pwm_capenr_t_ {
  struct {
    _reg32_t CAPENR         : 4;
    _reg32_t                : 28;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(pwm_capenr)
} pwm_capenr_t;

/* PWM Output Enable */
typedef union pwm_poe_t_ {
  struct {
    _reg32_t PWM0           : 1;
    _reg32_t PWM1           : 1;
    _reg32_t PWM2           : 1;
    _reg32_t PWM3           : 1;
    _reg32_t                : 28;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(pwm_poe)
} pwm_poe_t;

/* PWMA: 0x40040000 */
/* PWMB: 0x40140000 */
typedef struct {
  union { /*base+0x00000000*/
    volatile pwm_ppr_t              PPR;
             pwm_ppr_t              PPR_;
  };
  union { /*base+0x00000004*/
    volatile pwm_csr_t              CSR;
             pwm_csr_t              CSR_;
  };
  union { /*base+0x00000008*/
    volatile pwm_pcr_t              PCR;
             pwm_pcr_t              PCR_;
  };
  union { /*base+0x0000000C*/
    volatile pwm_cnr_t              CNR0;
             pwm_cnr_t              CNR0_;
  };
  union { /*base+0x00000010*/
    volatile pwm_cmr_t              CMR0;
             pwm_cmr_t              CMR0_;
  };
  union { /*base+0x00000014*/
    volatile pwm_pdr_t              PDR0;
             pwm_pdr_t              PDR0_;
  };
  union { /*base+0x00000018*/
    volatile pwm_cnr_t              CNR1;
             pwm_cnr_t              CNR1_;
  };
  union { /*base+0x0000001C*/
    volatile pwm_cmr_t              CMR1;
             pwm_cmr_t              CMR1_;
  };
  union { /*base+0x00000020*/
    volatile pwm_pdr_t              PDR1;
             pwm_pdr_t              PDR1_;
  };
  union { /*base+0x00000024*/
    volatile pwm_cnr_t              CNR2;
             pwm_cnr_t              CNR2_;
  };
  union { /*base+0x00000028*/
    volatile pwm_cmr_t              CMR2;
             pwm_cmr_t              CMR2_;
  };
  union { /*base+0x0000002C*/
    volatile pwm_pdr_t              PDR2;
             pwm_pdr_t              PDR2_;
  };
  union { /*base+0x00000030*/
    volatile pwm_cnr_t              CNR3;
             pwm_cnr_t              CNR3_;
  };
  union { /*base+0x00000034*/
    volatile pwm_cmr_t              CMR3;
             pwm_cmr_t              CMR3_;
  };
  union { /*base+0x00000038*/
    volatile pwm_pdr_t              PDR3;
             pwm_pdr_t              PDR3_;
  };
  _reg32_t                          _dummy0[0x1];
  union { /*base+0x00000040*/
    volatile pwm_pier_t             PIER;
             pwm_pier_t             PIER_;
  };
  union { /*base+0x00000044*/
    volatile pwm_piir_t             PIIR;
             pwm_piir_t             PIIR_;
  };
  _reg32_t                          _dummy1[0x2];
  union { /*base+0x00000050*/
    volatile pwm_ccr0_t             CCR0;
             pwm_ccr0_t             CCR0_;
  };
  union { /*base+0x00000054*/
    volatile pwm_ccr2_t             CCR2;
             pwm_ccr2_t             CCR2_;
  };
  union { /*base+0x00000058*/
    volatile pwm_crlr_t             CRLR0;
             pwm_crlr_t             CRLR0_;
  };
  union { /*base+0x0000005C*/
    volatile pwm_cflr_t             CFLR0;
             pwm_cflr_t             CFLR0_;
  };
  union { /*base+0x00000060*/
    volatile pwm_crlr_t             CRLR1;
             pwm_crlr_t             CRLR1_;
  };
  union { /*base+0x00000064*/
    volatile pwm_cflr_t             CFLR1;
             pwm_cflr_t             CFLR1_;
  };
  union { /*base+0x00000068*/
    volatile pwm_crlr_t             CRLR2;
             pwm_crlr_t             CRLR2_;
  };
  union { /*base+0x0000006C*/
    volatile pwm_cflr_t             CFLR2;
             pwm_cflr_t             CFLR2_;
  };
  union { /*base+0x00000070*/
    volatile pwm_crlr_t             CRLR3;
             pwm_crlr_t             CRLR3_;
  };
  union { /*base+0x00000074*/
    volatile pwm_cflr_t             CFLR3;
             pwm_cflr_t             CFLR3_;
  };
  union { /*base+0x00000078*/
    volatile pwm_capenr_t           CAPENR;
             pwm_capenr_t           CAPENR_;
  };
  union { /*base+0x0000007C*/
    volatile pwm_poe_t              POE;
             pwm_poe_t              POE_;
  };
} pwm_t;



/* SysTick Reload value Register */
typedef union scs_syst_rvr_t_ {
  struct {
    _reg32_t RELOAD         : 24;
    _reg32_t                : 8;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(scs_syst_rvr)
} scs_syst_rvr_t;

/* SysTick Current value Register */
typedef union scs_syst_cvr_t_ {
  struct {
    _reg32_t CURRENT        : 24;
    _reg32_t                : 8;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(scs_syst_cvr)
} scs_syst_cvr_t;

/* IRQ0 ~ IRQ3 Priority Control Register */
typedef union scs_nvic_ipr0_t_ {
  struct {
    _reg32_t                : 6;
    _reg32_t PRI_0          : 2;
    _reg32_t                : 6;
    _reg32_t PRI_1          : 2;
    _reg32_t                : 6;
    _reg32_t PRI_2          : 2;
    _reg32_t                : 6;
    _reg32_t PRI_3          : 2;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(scs_nvic_ipr0)
} scs_nvic_ipr0_t;

/* IRQ4 ~ IRQ7 Priority Control Register */
typedef union scs_nvic_ipr1_t_ {
  struct {
    _reg32_t                : 6;
    _reg32_t PRI_4          : 2;
    _reg32_t                : 6;
    _reg32_t PRI_5          : 2;
    _reg32_t                : 6;
    _reg32_t PRI_6          : 2;
    _reg32_t                : 6;
    _reg32_t PRI_7          : 2;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(scs_nvic_ipr1)
} scs_nvic_ipr1_t;

/* IRQ8 ~ IRQ11 Priority Control Register */
typedef union scs_nvic_ipr2_t_ {
  struct {
    _reg32_t                : 6;
    _reg32_t PRI_8          : 2;
    _reg32_t                : 6;
    _reg32_t PRI_9          : 2;
    _reg32_t                : 6;
    _reg32_t PRI_10         : 2;
    _reg32_t                : 6;
    _reg32_t PRI_11         : 2;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(scs_nvic_ipr2)
} scs_nvic_ipr2_t;

/* IRQ12 ~ IRQ15 Priority Control Register */
typedef union scs_nvic_ipr3_t_ {
  struct {
    _reg32_t                : 6;
    _reg32_t PRI_12         : 2;
    _reg32_t                : 6;
    _reg32_t PRI_13         : 2;
    _reg32_t                : 6;
    _reg32_t PRI_14         : 2;
    _reg32_t                : 6;
    _reg32_t PRI_15         : 2;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(scs_nvic_ipr3)
} scs_nvic_ipr3_t;

/* IRQ16 ~ IRQ19 Priority Control Register */
typedef union scs_nvic_ipr4_t_ {
  struct {
    _reg32_t                : 6;
    _reg32_t PRI_16         : 2;
    _reg32_t                : 6;
    _reg32_t PRI_17         : 2;
    _reg32_t                : 6;
    _reg32_t PRI_18         : 2;
    _reg32_t                : 6;
    _reg32_t PRI_19         : 2;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(scs_nvic_ipr4)
} scs_nvic_ipr4_t;

/* IRQ20 ~ IRQ23 Priority Control Register */
typedef union scs_nvic_ipr5_t_ {
  struct {
    _reg32_t                : 6;
    _reg32_t PRI_20         : 2;
    _reg32_t                : 6;
    _reg32_t PRI_21         : 2;
    _reg32_t                : 6;
    _reg32_t PRI_22         : 2;
    _reg32_t                : 6;
    _reg32_t PRI_23         : 2;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(scs_nvic_ipr5)
} scs_nvic_ipr5_t;

/* IRQ24 ~ IRQ27 Priority Control Register */
typedef union scs_nvic_ipr6_t_ {
  struct {
    _reg32_t                : 6;
    _reg32_t PRI_24         : 2;
    _reg32_t                : 6;
    _reg32_t PRI_25         : 2;
    _reg32_t                : 6;
    _reg32_t PRI_26         : 2;
    _reg32_t                : 6;
    _reg32_t PRI_27         : 2;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(scs_nvic_ipr6)
} scs_nvic_ipr6_t;

/* IRQ28 ~ IRQ31 Priority Control Register */
typedef union scs_nvic_ipr7_t_ {
  struct {
    _reg32_t                : 6;
    _reg32_t PRI_28         : 2;
    _reg32_t                : 6;
    _reg32_t PRI_29         : 2;
    _reg32_t                : 6;
    _reg32_t PRI_30         : 2;
    _reg32_t                : 6;
    _reg32_t PRI_31         : 2;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(scs_nvic_ipr7)
} scs_nvic_ipr7_t;

/* CPUID Base Register */
typedef union scs_cpuid_t_ {
  struct {
    _reg32_t REVISION       : 4;
    _reg32_t PARTNO         : 12;
    _reg32_t PART           : 4;
    _reg32_t                : 4;
    _reg32_t IMPLEMENTER    : 8;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(scs_cpuid)
} scs_cpuid_t;

/* Interrupt Control State Register */
typedef union scs_icsr_t_ {
  struct {
    _reg32_t VECTACTIVE     : 9;
    _reg32_t                : 3;
    _reg32_t VECTPENDING    : 9;
    _reg32_t                : 1;
    _reg32_t ISRPENDING     : 1;
    _reg32_t ISRPREEMPT     : 1;
    _reg32_t                : 1;
    _reg32_t PENDSTCLR      : 1;
    _reg32_t PENDSTSET      : 1;
    _reg32_t PENDSVCLR      : 1;
    _reg32_t PENDSVSET      : 1;
    _reg32_t                : 2;
    _reg32_t NMIPENDSET     : 1;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(scs_icsr)
} scs_icsr_t;

/* Application Interrupt and Reset Control Register */
typedef union scs_aircr_t_ {
  struct {
    _reg32_t                : 1;
    _reg32_t VECTCLRACTIVE  : 1;
    _reg32_t SYSRESETREQ    : 1;
    _reg32_t                : 13;
    _reg32_t VECTORKEY      : 16;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(scs_aircr)
} scs_aircr_t;

/* System Control Register */
typedef union scs_scr_t_ {
  struct {
    _reg32_t                : 1;
    _reg32_t SLEEPONEXIT    : 1;
    _reg32_t SLEEPDEEP      : 1;
    _reg32_t                : 1;
    _reg32_t SEVONPEND      : 1;
    _reg32_t                : 27;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(scs_scr)
} scs_scr_t;

/* System Handler Priority Register 2 */
typedef union scs_shpr2_t_ {
  struct {
    _reg32_t                : 30;
    _reg32_t PRI_11         : 2;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(scs_shpr2)
} scs_shpr2_t;

/* System Handler Priority Register 3 */
typedef union scs_shpr3_t_ {
  struct {
    _reg32_t                : 22;
    _reg32_t PRI_14         : 2;
    _reg32_t                : 6;
    _reg32_t PRI_15         : 2;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(scs_shpr3)
} scs_shpr3_t;

/* SCS: 0xE000E000 */
typedef struct {
  _reg32_t                          _dummy0[0x4];

  union { /*0xE000E014*/
    volatile scs_syst_rvr_t         SYST_RVR;
             scs_syst_rvr_t         SYST_RVR_;
  };
  union { /*0xE000E018*/
    volatile scs_syst_cvr_t         SYST_CVR;
             scs_syst_cvr_t         SYST_CVR_;
  };
  _reg32_t                          _dummy1[0x39];
  union { /*0xE000E100*/
    volatile _reg32_t               NVIC_ISER;
             _reg32_t               NVIC_ISER_;
  };
  _reg32_t                          _dummy2[0x1F];
  union { /*0xE000E180*/
    volatile _reg32_t               NVIC_ICER;
             _reg32_t               NVIC_ICER_;
  };
  _reg32_t                          _dummy3[0x1F];
  union { /*0xE000E200*/
    volatile _reg32_t               NVIC_ISPR;
             _reg32_t               NVIC_ISPR_;
  };
  _reg32_t                          _dummy4[0x1F];
  union { /*0xE000E280*/
    volatile _reg32_t               NVIC_ICPR;
             _reg32_t               NVIC_ICPR_;
  };
  _reg32_t                          _dummy5[0x5F];
  union { /*0xE000E400*/
    volatile scs_nvic_ipr0_t        NVIC_IPR0;
             scs_nvic_ipr0_t        NVIC_IPR0_;
  };
  union { /*0xE000E404*/
    volatile scs_nvic_ipr1_t        NVIC_IPR1;
             scs_nvic_ipr1_t        NVIC_IPR1_;
  };
  union { /*0xE000E408*/
    volatile scs_nvic_ipr2_t        NVIC_IPR2;
             scs_nvic_ipr2_t        NVIC_IPR2_;
  };
  union { /*0xE000E40C*/
    volatile scs_nvic_ipr3_t        NVIC_IPR3;
             scs_nvic_ipr3_t        NVIC_IPR3_;
  };
  union { /*0xE000E410*/
    volatile scs_nvic_ipr4_t        NVIC_IPR4;
             scs_nvic_ipr4_t        NVIC_IPR4_;
  };
  union { /*0xE000E414*/
    volatile scs_nvic_ipr5_t        NVIC_IPR5;
             scs_nvic_ipr5_t        NVIC_IPR5_;
  };
  union { /*0xE000E418*/
    volatile scs_nvic_ipr6_t        NVIC_IPR6;
             scs_nvic_ipr6_t        NVIC_IPR6_;
  };
  union { /*0xE000E41C*/
    volatile scs_nvic_ipr7_t        NVIC_IPR7;
             scs_nvic_ipr7_t        NVIC_IPR7_;
  };
  _reg32_t                          _dummy6[0x238];
  union { /*0xE000ED00*/
    volatile scs_cpuid_t            CPUID;
             scs_cpuid_t            CPUID_;
  };
  union { /*0xE000ED04*/
    volatile scs_icsr_t             ICSR;
             scs_icsr_t             ICSR_;
  };
  _reg32_t                          _dummy7[0x1];
  union { /*0xE000ED0C*/
    volatile scs_aircr_t            AIRCR;
             scs_aircr_t            AIRCR_;
  };
  union { /*0xE000ED10*/
    volatile scs_scr_t              SCR;
             scs_scr_t              SCR_;
  };
  _reg32_t                          _dummy8[0x2];
  union { /*0xE000ED1C*/
    volatile scs_shpr2_t            SHPR2;
             scs_shpr2_t            SHPR2_;
  };
  union { /*0xE000ED20*/
    volatile scs_shpr3_t            SHPR3;
             scs_shpr3_t            SHPR3_;
  };
} scs_t;

/* Timer0 Control and Status Register */
typedef union tmr_tcsr_t_ {
  struct {
    _reg32_t PRESCALE       : 8;
    _reg32_t                : 8;
    _reg32_t TDR_EN         : 1;
    _reg32_t                : 8;
    _reg32_t CACT           : 1;
    _reg32_t CRST           : 1;
    _reg32_t MODE           : 2;
    _reg32_t IE             : 1;
    _reg32_t CEN            : 1;
    _reg32_t                : 1;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(tmr_tcsr)
} tmr_tcsr_t;

/* Timer0 Compare Register */
typedef union tmr_tcmpr_t_ {
  struct {
    _reg32_t TCMP           : 24;
    _reg32_t                : 8;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(tmr_tcmpr)
} tmr_tcmpr_t;

/* Timer0 Interrupt Status Register */
typedef union tmr_tisr_t_ {
  struct {
    _reg32_t TIF            : 1;
    _reg32_t                : 31;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(tmr_tisr)
} tmr_tisr_t;

/* Timer0 Data Register */
typedef union tmr_tdr_t_ {
  struct {
    _reg32_t TDR            : 24;
    _reg32_t                : 8;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(tmr_tdr)
} tmr_tdr_t;

/* TMR0: 0x40010000 */
/* TMR1: 0x40010020 */
/* TMR2: 0x40110000 */
/* TMR3: 0x40110020 */
typedef struct {
  union { /*base+0x00000000*/
    volatile tmr_tcsr_t             TCSR;
             tmr_tcsr_t             TCSR_;
  };
  union { /*base+0x00000004*/
    volatile tmr_tcmpr_t            TCMPR;
             tmr_tcmpr_t            TCMPR_;
  };
  union { /*base+0x00000008*/
    volatile tmr_tisr_t             TISR;
             tmr_tisr_t             TISR_;
  };
  union { /*base+0x0000000C*/
    volatile tmr_tdr_t              TDR;
             tmr_tdr_t              TDR_;
  };
} tmr_t;

/* Watchdog Timer Control Register */
typedef union wdt_wtcr_t_ {
  struct {
    _reg32_t WTR            : 1;
    _reg32_t WTRE           : 1;
    _reg32_t WTRF           : 1;
    _reg32_t WTIF           : 1;
    _reg32_t WTWKE          : 1;
    _reg32_t WTWKF          : 1;
    _reg32_t WTIE           : 1;
    _reg32_t WTE            : 1;
    _reg32_t WTIS           : 3;
    _reg32_t                : 21;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(wdt_wtcr)
} wdt_wtcr_t;

/* WDT: 0x40004000 */
typedef struct {
  union { /*0x40004000*/
    volatile wdt_wtcr_t             WTCR;
             wdt_wtcr_t             WTCR_;
  };
} wdt_t;

/* Control and Status Register */
typedef union spi_spi_cntrl_t_ {
  struct {
    _reg32_t GO_BUSY        : 1;
    _reg32_t RX_NEG         : 1;
    _reg32_t TX_NEG         : 1;
    _reg32_t TX_BIT_LEN     : 5;
    _reg32_t TX_NUM         : 2;
    _reg32_t LSB            : 1;
    _reg32_t CLKP           : 1;
    _reg32_t SP_CYCLE       : 4;
    _reg32_t IF             : 1;
    _reg32_t IE             : 1;
    _reg32_t SLAVE          : 1;
    _reg32_t REORDER        : 2;
    _reg32_t                : 2;
    _reg32_t VARCLK_EN      : 1;
    _reg32_t                : 8;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(spi_spi_cntrl)
} spi_spi_cntrl_t;

/* Clock Divider Register */
typedef union spi_spi_divider_t_ {
  struct {
    _reg32_t DIVIDER        : 16;
    _reg32_t DIVIDER2       : 16;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(spi_spi_divider)
} spi_spi_divider_t;

/* Slave Select Register */
typedef union spi_spi_ssr_t_ {
  struct {
    _reg32_t SSR            : 1;
    _reg32_t                : 1;
    _reg32_t SS_LVL         : 1;
    _reg32_t AUTOSS         : 1;
    _reg32_t SS_LTRIG       : 1;
    _reg32_t LTRIG_FLAG     : 1;
    _reg32_t                : 26;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(spi_spi_ssr)
} spi_spi_ssr_t;

/* SPI0: 0x40030000 */
/* SPI1: 0x40034000 */
typedef struct {
  union { /*base+0x00000000*/
    volatile spi_spi_cntrl_t        SPI_CNTRL;
             spi_spi_cntrl_t        SPI_CNTRL_;
  };
  union { /*base+0x00000004*/
    volatile spi_spi_divider_t      SPI_DIVIDER;
             spi_spi_divider_t      SPI_DIVIDER_;
  };
  union { /*base+0x00000008*/
    volatile spi_spi_ssr_t          SPI_SSR;
             spi_spi_ssr_t          SPI_SSR_;
  };
  _reg32_t                          _dummy0[0x1];
  union { /*base+0x00000010*/
    volatile _reg32_t               SPI_RX0;
             _reg32_t               SPI_RX0_;
  };
  union { /*base+0x00000014*/
    volatile _reg32_t               SPI_RX1;
             _reg32_t               SPI_RX1_;
  };
  _reg32_t                          _dummy1[0x2];
  union { /*base+0x00000020*/
    volatile _reg32_t               SPI_TX0;
             _reg32_t               SPI_TX0_;
  };
  union { /*base+0x00000024*/
    volatile _reg32_t               SPI_TX1;
             _reg32_t               SPI_TX1_;
  };
  _reg32_t                          _dummy2[0x3];
  union { /*base+0x00000034*/
    volatile _reg32_t               SPI_VARCLK;
             _reg32_t               SPI_VARCLK_;
  };
} spi_t;

/* UART0 Receive Buffer Register. */
typedef union uart_ua_rbr_t_ {
  struct {
    _reg32_t _8_bitReceivedData: 8;
    _reg32_t                : 24;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(uart_ua_rbr)
} uart_ua_rbr_t;

/* UART0 Transmit Holding Register. */
typedef union uart_ua_thr_t_ {
  struct {
    _reg32_t _8_bitTransmittedData: 8;
    _reg32_t                : 24;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(uart_ua_thr)
} uart_ua_thr_t;

/* UART0 Interrupt Enable Register. */
typedef union uart_ua_ier_t_ {
  struct {
    _reg32_t RDA_IEN        : 1;
    _reg32_t THRE_IEN       : 1;
    _reg32_t RLS_IEN        : 1;
    _reg32_t Modem_IEN      : 1;
    _reg32_t RTO_IEN        : 1;
    _reg32_t BUF_ERR_IEN    : 1;
    _reg32_t Wake_EN        : 1;
    _reg32_t                : 1;
    _reg32_t                : 3;
    _reg32_t Time_Out_EN    : 1;
    _reg32_t Auto_RTS_EN    : 1;
    _reg32_t Auto_CTS_EN    : 1;
    _reg32_t                : 18;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(uart_ua_ier)
} uart_ua_ier_t;

/* UART0 FIFO Control Register. */
typedef union uart_ua_fcr_t_ {
  struct {
    _reg32_t                : 1;
    _reg32_t RFR            : 1;
    _reg32_t TFR            : 1;
    _reg32_t                : 1;
    _reg32_t RFITL          : 4;
    _reg32_t RX_DIS         : 1;
    _reg32_t                : 7;
    _reg32_t RTS_Tri_Lev    : 4;
    _reg32_t                : 12;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(uart_ua_fcr)
} uart_ua_fcr_t;

/* UART0 Line Control Register. */
typedef union uart_ua_lcr_t_ {
  struct {
    _reg32_t WLS            : 2;
    _reg32_t NSB            : 1;
    _reg32_t PBE            : 1;
    _reg32_t EPE            : 1;
    _reg32_t SPE            : 1;
    _reg32_t BCB            : 1;
    _reg32_t                : 25;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(uart_ua_lcr)
} uart_ua_lcr_t;

/* UART0 Modem Control Register. */
typedef union uart_ua_mcr_t_ {
  struct {
    _reg32_t                : 1;
    _reg32_t RTS            : 1;
    _reg32_t                : 7;
    _reg32_t Lev_RTS        : 1;
    _reg32_t                : 3;
    _reg32_t RTS_St         : 1;
    _reg32_t                : 18;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(uart_ua_mcr)
} uart_ua_mcr_t;

/* UART0 Modem Status Register. */
typedef union uart_ua_msr_t_ {
  struct {
    _reg32_t DCTSF          : 1;
    _reg32_t                : 3;
    _reg32_t CTS_St         : 1;
    _reg32_t                : 3;
    _reg32_t Lev_CTS        : 1;
    _reg32_t                : 23;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(uart_ua_msr)
} uart_ua_msr_t;

/* UART0 FIFO Status Register. */
typedef union uart_ua_fsr_t_ {
  struct {
    _reg32_t Rx_Over_IF     : 1;
    _reg32_t                : 2;
    _reg32_t RS_485_Add_Det : 1;
    _reg32_t PEF            : 1;
    _reg32_t FEF            : 1;
    _reg32_t BIF            : 1;
    _reg32_t                : 1;
    _reg32_t Rx_Pointer     : 6;
    _reg32_t Rx_Empty       : 1;
    _reg32_t Rx_Full        : 1;
    _reg32_t Tx_Pointer     : 6;
    _reg32_t Tx_Empty       : 1;
    _reg32_t Tx_Full        : 1;
    _reg32_t Tx_Over_IF     : 1;
    _reg32_t                : 3;
    _reg32_t TE_Flag        : 1;
    _reg32_t                : 3;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(uart_ua_fsr)
} uart_ua_fsr_t;

/* UART0 Interrupt Status Register. */
typedef union uart_ua_isr_t_ {
  struct {
    _reg32_t RDA_IF         : 1;
    _reg32_t THRE_IF        : 1;
    _reg32_t RLS_IF         : 1;
    _reg32_t Modem_IF       : 1;
    _reg32_t Tout_IF        : 1;
    _reg32_t Buf_Err_IF     : 1;
    _reg32_t                : 2;
    _reg32_t RDA_INT        : 1;
    _reg32_t THRE_INT       : 1;
    _reg32_t RLS_INT        : 1;
    _reg32_t Modem_INT      : 1;
    _reg32_t Tout_INT       : 1;
    _reg32_t Buf_Err_INT    : 1;
    _reg32_t                : 18;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(uart_ua_isr)
} uart_ua_isr_t;

/* UART0 Time Out Register */
typedef union uart_ua_tor_t_ {
  struct {
    _reg32_t TOIC           : 7;
    _reg32_t                : 1;
    _reg32_t DLY            : 8;
    _reg32_t                : 16;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(uart_ua_tor)
} uart_ua_tor_t;

/* UART0 Baud Rate Divisor Register */
typedef union uart_ua_baud_t_ {
  struct {
    _reg32_t BRD_LowByte    : 8;
    _reg32_t BRD_HighByte   : 8;
    _reg32_t                : 8;
    _reg32_t Divider_X      : 4;
    _reg32_t DIV_X_ONE      : 1;
    _reg32_t DIV_X_EN       : 1;
    _reg32_t                : 2;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(uart_ua_baud)
} uart_ua_baud_t;

/* UART0 IrDA Control Register. */
typedef union uart_ua_ircr_t_ {
  struct {
    _reg32_t                : 1;
    _reg32_t Tx_SELECT      : 1;
    _reg32_t LB             : 1;
    _reg32_t                : 2;
    _reg32_t INV_Tx         : 1;
    _reg32_t INV_Rx         : 1;
    _reg32_t                : 25;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(uart_ua_ircr)
} uart_ua_ircr_t;

/* UART0 RS485 Control State Register. */
typedef union uart_ua_act_csr_t_ {
  struct {
    _reg32_t                : 8;
    _reg32_t RS_485_NMM     : 1;
    _reg32_t RS_485_AAD     : 1;
    _reg32_t RS_485_AUD     : 1;
    _reg32_t                : 4;
    _reg32_t RS_485_Add_EN  : 1;
    _reg32_t                : 1;
    _reg32_t                : 7;
    _reg32_t ADDR_MATCH     : 8;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(uart_ua_act_csr)
} uart_ua_act_csr_t;

/* UART0 Function Select Register. */
typedef union uart_ua_fun_sel_t_ {
  struct {
    _reg32_t FUN_SEL        : 2;
    _reg32_t                : 30;
  };
  _reg32_t v;
  REG_CPP_OVERRIDE(uart_ua_fun_sel)
} uart_ua_fun_sel_t;

/* UART0: 0x40050000 */
/* UART1: 0x40150000 */
typedef struct {
  union { /*base+0x00000000*/
    volatile uart_ua_rbr_t          UA_RBR;
             uart_ua_rbr_t          UA_RBR_;
    volatile uart_ua_thr_t          UA_THR;
             uart_ua_thr_t          UA_THR_;
  };
  union { /*base+0x00000004*/
    volatile uart_ua_ier_t          UA_IER;
             uart_ua_ier_t          UA_IER_;
  };
  union { /*base+0x00000008*/
    volatile uart_ua_fcr_t          UA_FCR;
             uart_ua_fcr_t          UA_FCR_;
  };
  union { /*base+0x0000000C*/
    volatile uart_ua_lcr_t          UA_LCR;
             uart_ua_lcr_t          UA_LCR_;
  };
  union { /*base+0x00000010*/
    volatile uart_ua_mcr_t          UA_MCR;
             uart_ua_mcr_t          UA_MCR_;
  };
  union { /*base+0x00000014*/
    volatile uart_ua_msr_t          UA_MSR;
             uart_ua_msr_t          UA_MSR_;
  };
  union { /*base+0x00000018*/
    volatile uart_ua_fsr_t          UA_FSR;
             uart_ua_fsr_t          UA_FSR_;
  };
  union { /*base+0x0000001C*/
    volatile uart_ua_isr_t          UA_ISR;
             uart_ua_isr_t          UA_ISR_;
  };
  union { /*base+0x00000020*/
    volatile uart_ua_tor_t          UA_TOR;
             uart_ua_tor_t          UA_TOR_;
  };
  union { /*base+0x00000024*/
    volatile uart_ua_baud_t         UA_BAUD;
             uart_ua_baud_t         UA_BAUD_;
  };
  union { /*base+0x00000028*/
    volatile uart_ua_ircr_t         UA_IRCR;
             uart_ua_ircr_t         UA_IRCR_;
  };
  union { /*base+0x0000002C*/
    volatile uart_ua_act_csr_t      UA_ACT_CSR;
             uart_ua_act_csr_t      UA_ACT_CSR_;
  };
  union { /*base+0x00000030*/
    volatile uart_ua_fun_sel_t      UA_FUN_SEL;
             uart_ua_fun_sel_t      UA_FUN_SEL_;
  };
} uart_t;


/* register groups */
#if defined (__ICCARM__)
__no_init adc_t       ADCs        @0x400E0000;
__no_init clk_t       CLKs        @0x50000200;
__no_init ebi_ctl_t   EBI_CTLs    @0x50010000;
__no_init fmc_t       FMCs        @0x5000C000;
__no_init gcr_t       GCRs        @0x50000000;
__no_init gpio_t      GP0s        @0x50004000;
__no_init gpio_t      GP1s        @0x50004040;
__no_init gpio_t      GP2s        @0x50004080;
__no_init gpio_t      GP3s        @0x500040C0;
__no_init gpio_t      GP4s        @0x50004100;
__no_init gpio_gcr_t  GPIOs       @0x50004180;
__no_init gpio_bits_t GP0_BITSs   @0x50004200;
__no_init gpio_bits_t GP1_BITSs   @0x50004220;
__no_init gpio_bits_t GP2_BITSs   @0x50004240;
__no_init gpio_bits_t GP3_BITSs   @0x50004260;
__no_init gpio_bits_t GP4_BITSs   @0x50004280;
__no_init i2c_t       I2Cs        @0x40020000;
__no_init int_t       INTs        @0x50000300;
__no_init pwm_t       PWMAs       @0x40040000;
__no_init pwm_t       PWMBs       @0x40140000;
__no_init scs_t       SCSs        @0xE000E000;
__no_init tmr_t       TMR0s       @0x40010000;
__no_init tmr_t       TMR1s       @0x40010020;
__no_init tmr_t       TMR2s       @0x40110000;
__no_init tmr_t       TMR3s       @0x40110020;
__no_init wdt_t       WDTs        @0x40004000;
__no_init spi_t       SPI0s       @0x40030000;
__no_init spi_t       SPI1s       @0x40034000;
__no_init uart_t      UART0s      @0x40050000;
__no_init uart_t      UART1s      @0x40150000;
#elif defined (__GNUC__)
extern adc_t       ADCs;
extern clk_t       CLKs;
extern ebi_ctl_t   EBI_CTLs;
extern fmc_t       FMCs;
extern gcr_t       GCRs;
extern gpio_t      GP0s;
extern gpio_t      GP1s;
extern gpio_t      GP2s;
extern gpio_t      GP3s;
extern gpio_t      GP4s;
extern gpio_gcr_t  GPIOs;
extern gpio_bits_t GP0_BITSs;
extern gpio_bits_t GP1_BITSs;
extern gpio_bits_t GP2_BITSs;
extern gpio_bits_t GP3_BITSs;
extern gpio_bits_t GP4_BITSs;
extern i2c_t       I2Cs;
extern int_t       INTs;
extern pwm_t       PWMAs;
extern pwm_t       PWMBs;
extern scs_t       SCSs;
extern tmr_t       TMR0s;
extern tmr_t       TMR1s;
extern tmr_t       TMR2s;
extern tmr_t       TMR3s;
extern wdt_t       WDTs;
extern spi_t       SPI0s;
extern spi_t       SPI1s;
extern uart_t      UART0s;
extern uart_t      UART1s;
__asm__(".global ADCs        \n.weak ADCs        \n.set ADCs,        0x400E0000");
__asm__(".global CLKs        \n.weak CLKs        \n.set CLKs,        0x50000200");
__asm__(".global EBI_CTLs    \n.weak EBI_CTLs    \n.set EBI_CTLs,    0x50010000");
__asm__(".global FMCs        \n.weak FMCs        \n.set FMCs,        0x5000C000");
__asm__(".global GCRs        \n.weak GCRs        \n.set GCRs,        0x50000000");
__asm__(".global GP0s        \n.weak GP0s        \n.set GP0s,        0x50004000");
__asm__(".global GP1s        \n.weak GP1s        \n.set GP1s,        0x50004040");
__asm__(".global GP2s        \n.weak GP2s        \n.set GP2s,        0x50004080");
__asm__(".global GP3s        \n.weak GP3s        \n.set GP3s,        0x500040C0");
__asm__(".global GP4s        \n.weak GP4s        \n.set GP4s,        0x50004100");
__asm__(".global GPIOs       \n.weak GPIOs       \n.set GPIOs,       0x50004180");
__asm__(".global GP0_BITSs   \n.weak GP0_BITSs   \n.set GP0_BITSs,   0x50004200");
__asm__(".global GP1_BITSs   \n.weak GP1_BITSs   \n.set GP1_BITSs,   0x50004220");
__asm__(".global GP2_BITSs   \n.weak GP2_BITSs   \n.set GP2_BITSs,   0x50004240");
__asm__(".global GP3_BITSs   \n.weak GP3_BITSs   \n.set GP3_BITSs,   0x50004260");
__asm__(".global GP4_BITSs   \n.weak GP4_BITSs   \n.set GP4_BITSs,   0x50004280");
__asm__(".global I2Cs        \n.weak I2Cs        \n.set I2Cs,        0x40020000");
__asm__(".global INTs        \n.weak INTs        \n.set INTs,        0x50000300");
__asm__(".global PWMAs       \n.weak PWMAs       \n.set PWMAs,       0x40040000");
__asm__(".global PWMBs       \n.weak PWMBs       \n.set PWMBs,       0x40140000");
__asm__(".global SCSs        \n.weak SCSs        \n.set SCSs,        0xE000E000");
__asm__(".global TMR0s       \n.weak TMR0s       \n.set TMR0s,       0x40010000");
__asm__(".global TMR1s       \n.weak TMR1s       \n.set TMR1s,       0x40010020");
__asm__(".global TMR2s       \n.weak TMR2s       \n.set TMR2s,       0x40110000");
__asm__(".global TMR3s       \n.weak TMR3s       \n.set TMR3s,       0x40110020");
__asm__(".global WDTs        \n.weak WDTs        \n.set WDTs,        0x40004000");
__asm__(".global SPI0s       \n.weak SPI0s       \n.set SPI0s,       0x40030000");
__asm__(".global SPI1s       \n.weak SPI1s       \n.set SPI1s,       0x40034000");
__asm__(".global UART0s      \n.weak UART0s      \n.set UART0s,      0x40050000");
__asm__(".global UART1s      \n.weak UART1s      \n.set UART1s,      0x40150000");
#else
#define ADCs        (*((adc_t *)    0x400E0000))
#define CLKs        (*((clk_t *)    0x50000200))
#define EBI_CTLs    (*((ebi_ctl_t *)0x50010000))
#define FMCs        (*((fmc_t *)    0x5000C000))
#define GCRs        (*((gcr_t *)    0x50000000))
#define P0s        (*((gpio_t *)   0x50004000))
#define P1s        (*((gpio_t *)   0x50004040))
#define P2s        (*((gpio_t *)   0x50004080))
#define P3s        (*((gpio_t *)   0x500040C0))
#define P4s        (*((gpio_t *)   0x50004100))
#define GPIOs       (*((gpio_gcr_t *)0x50004180))
#define GP0_BITSs   (*((gpio_bits_t *)0x50004200))
#define GP1_BITSs   (*((gpio_bits_t *)0x50004220))
#define GP2_BITSs   (*((gpio_bits_t *)0x50004240))
#define GP3_BITSs   (*((gpio_bits_t *)0x50004260))
#define GP4_BITSs   (*((gpio_bits_t *)0x50004280))
#define I2Cs        (*((i2c_t *)    0x40020000))
#define INTs        (*((int_t *)    0x50000300))
#define PWMAs       (*((pwm_t *)    0x40040000))
#define PWMBs       (*((pwm_t *)    0x40140000))
#define SCSs        (*((scs_t *)    0xE000E000))
#define TMR0s       (*((tmr_t *)    0x40010000))
#define TMR1s       (*((tmr_t *)    0x40010020))
#define TMR2s       (*((tmr_t *)    0x40110000))
#define TMR3s       (*((tmr_t *)    0x40110020))
#define WDTs        (*((wdt_t *)    0x40004000))
#define SPI0s       (*((spi_t *)    0x40030000))
#define SPI1s       (*((spi_t *)    0x40034000))
#define UART0s      (*((uart_t *)   0x40050000))
#define UART1s      (*((uart_t *)   0x40150000))
#endif

#ifdef __cplusplus
}
#endif
#endif

