/* BASIC INTERRUPT VECTORS TABLE FOR STM8 devices
 * Copyright (c) 2014 STMicroelectronics
 */

#include "stm8s_it.h"

typedef void @far (*interrupt_handler_t)(void);

struct interrupt_vector {
	u8 interrupt_instruction;
	interrupt_handler_t interrupt_handler;
};

struct interrupt_vector const _vectab[] = {
  {0x82, (interrupt_handler_t)_stext}, /* RESET */
  {0x82, (interrupt_handler_t)TRAP_IRQHandler}, /* TRAP - Software interrupt */
	{0x82, (interrupt_handler_t)TLI_IRQHandler}, /* irq0 - External Top Level interrupt (TLI) */
	{0x82, (interrupt_handler_t)AWU_IRQHandler}, /* irq1 - Auto Wake Up from Halt interrupt */
	{0x82, (interrupt_handler_t)CLK_IRQHandler}, /* irq2 - Clock Controller interrupt */
  {0x82, (interrupt_handler_t)EXTI_PORTA_IRQHandler}, /* irq3 - External interrupt 0 (GPIOA) */
	{0x82, (interrupt_handler_t)EXTI_PORTB_IRQHandler}, /* irq4 - External interrupt 1 (GPIOB) */
	{0x82, (interrupt_handler_t)EXTI_PORTC_IRQHandler}, /* irq5 - External interrupt 2 (GPIOC) */
	{0x82, (interrupt_handler_t)EXTI_PORTD_IRQHandler}, /* irq6 - External interrupt 3 (GPIOD) */
	{0x82, (interrupt_handler_t)EXTI_PORTE_IRQHandler}, /* irq7 - External interrupt 4 (GPIOE) */
	
#if defined(STM8S208) || defined(STM8AF52Ax)
	{0x82, (interrupt_handler_t)CAN_RX_IRQHandler}, /* irq8 - CAN Rx interrupt */
	{0x82, (interrupt_handler_t)CAN_TX_IRQHandler}, /* irq9 - CAN Tx/ER/SC interrupt */
#elif defined(STM8S903) || defined(STM8AF622x)
	{0x82, (interrupt_handler_t)EXTI_PORTF_IRQHandler}, /* irq8 - External interrupt 5 (GPIOF) */
	{0x82, (interrupt_handler_t)NonHandledInterrupt}, /* irq9 - Reserved */
#else /*STM8S207, STM8S105 or STM8AF62Ax or STM8AF626x*/
	{0x82, (interrupt_handler_t)NonHandledInterrupt}, /* irq8 - Reserved */
	{0x82, (interrupt_handler_t)NonHandledInterrupt}, /* irq9 - Reserved */
#endif /* STM8S208 or STM8AF52Ax */
	{0x82, (interrupt_handler_t)SPI_IRQHandler}, /* irq10 - SPI End of transfer interrupt */
	{0x82, (interrupt_handler_t)TIM1_UPD_OVF_TRG_BRK_IRQHandler}, /* irq11 - TIM1 Update/Overflow/Trigger/Break interrupt */
	{0x82, (interrupt_handler_t)TIM1_CAP_COM_IRQHandler}, /* irq12 - TIM1 Capture/Compare interrupt */
  
#if defined(STM8S903) || defined(STM8AF622x)
  {0x82, (interrupt_handler_t)TIM5_UPD_OVF_BRK_TRG_IRQHandler}, /* irq13 - TIM5 Update/Overflow/Break/Trigger interrupt  */
  {0x82, (interrupt_handler_t)TIM5_CAP_COM_IRQHandler}, /* irq14 - TIM5 Capture/Compare interrupt */
	
#else /*STM8S208, STM8S207, STM8S105 or STM8S103 or STM8AF62Ax or STM8AF52Ax or STM8AF626x*/
  {0x82, (interrupt_handler_t)TIM2_UPD_OVF_BRK_IRQHandler}, /* irq13 - TIM2 Update/Overflow/Break interrupt  */
  {0x82, (interrupt_handler_t)TIM2_CAP_COM_IRQHandler}, /* irq14 - TIM2 Capture/Compare interrupt */
#endif /*STM8S903*/
	
#if defined(STM8S208) || defined(STM8S207) || defined(STM8S007) || defined(STM8S105) || \
    defined(STM8S005) ||  defined(STM8AF52Ax) || defined(STM8AF62Ax) || defined(STM8AF626x)
  {0x82, (interrupt_handler_t)TIM3_UPD_OVF_BRK_IRQHandler}, /* irq15 - TIM3 Update/Overflow/Break interrupt */
  {0x82, (interrupt_handler_t)TIM3_CAP_COM_IRQHandler}, /* irq16 - TIM3 Capture/Compare interrupt */
#else
	{0x82, (interrupt_handler_t)NonHandledInterrupt}, /* irq15 - Reserved */
	{0x82, (interrupt_handler_t)NonHandledInterrupt}, /* irq16 - Reserved */
#endif /*STM8S208, STM8S207, STM8S105 or STM8AF62Ax or STM8AF52Ax or STM8AF626x*/
	
#if defined(STM8S105) || defined(STM8S005) || defined(STM8AF626x)
	{0x82, (interrupt_handler_t)NonHandledInterrupt}, /* irq17 - Reserved */
	{0x82, (interrupt_handler_t)NonHandledInterrupt}, /* irq18 - Reserved */
#elif defined (STM8AF622x)
	{0x82, (interrupt_handler_t)UART4_TX_IRQHandler}, /* irq17 - UART4 Tx complete interrupt */
	{0x82, (interrupt_handler_t)UART4_RX_IRQHandler}, /* irq18 - UART4 Rx interrupt */
#else	
	{0x82, (interrupt_handler_t)UART1_TX_IRQHandler}, /* irq17 - UART1 Tx complete interrupt */
	{0x82, (interrupt_handler_t)UART1_RX_IRQHandler}, /* irq18 - UART1 Rx interrupt */
#endif /*STM8S105 or STM8AF626x */
	{0x82, (interrupt_handler_t)I2C_IRQHandler}, /* irq19 - I2C interrupt */

#if defined(STM8S208) || defined(STM8S207) || defined(STM8S007) || defined(STM8AF52Ax) || defined(STM8AF62Ax)

	{0x82, (interrupt_handler_t)UART3_TX_IRQHandler}, /* irq20 - UART3 Tx interrupt */
	{0x82, (interrupt_handler_t)UART3_RX_IRQHandler}, /* irq21 - UART3 Rx interrupt */
#elif defined(STM8S105) || defined(STM8S005) || defined(STM8AF626x)
	{0x82, (interrupt_handler_t)UART2_TX_IRQHandler}, /* irq20 - UART2 Tx interrupt */
	{0x82, (interrupt_handler_t)UART2_RX_IRQHandler}, /* irq21 - UART2 Rx interrupt */

#else /* STM8S103, STM8S903, STM8AF622x */
	{0x82, (interrupt_handler_t)NonHandledInterrupt}, /* irq20 - Reserved */
	{0x82, (interrupt_handler_t)NonHandledInterrupt}, /* irq21 - Reserved */
#endif /* STM8S208, STM8S207, STM8AF52Ax or STM8AF62Ax */

#if defined(STM8S208) || defined(STM8S207) || defined(STM8S007) || defined(STM8AF52Ax) || defined(STM8AF62Ax)
	{0x82, (interrupt_handler_t)ADC2_IRQHandler}, /* irq22 - ADC2 end of conversion interrupt */
#else /* STM8S105, STM8S103, STM8S903, STM8AF622x   */
	{0x82, (interrupt_handler_t)ADC1_IRQHandler}, /* irq22 - ADC1 end of conversion/Analog watchdog interrupts */

#endif /* STM8S208, STM8S207, STM8AF52Ax or STM8AF62Ax */

#if defined(STM8S903) || defined(STM8AF622x)
	{0x82, (interrupt_handler_t)TIM6_UPD_OVF_TRG_IRQHandler}, /* irq23 - TIM6 Update/Overflow/Trigger interrupt */
#else
	{0x82, (interrupt_handler_t)TIM4_UPD_OVF_IRQHandler}, /* irq23 - TIM4 Update/Overflow interrupt */
#endif /* (STM8S903) || (STM8AF622x) */
	{0x82, (interrupt_handler_t)EEPROM_EEC_IRQHandler},  /* irq24 - FLASH interrupt */
	{0x82, (interrupt_handler_t)NonHandledInterrupt}, /* irq25 - Reserved */
	{0x82, (interrupt_handler_t)NonHandledInterrupt}, /* irq26 - Reserved */
	{0x82, (interrupt_handler_t)NonHandledInterrupt}, /* irq27 - Reserved */
	{0x82, (interrupt_handler_t)NonHandledInterrupt}, /* irq28 - Reserved */
	{0x82, (interrupt_handler_t)NonHandledInterrupt}, /* irq29 - Reserved */

};

/******************* (C) COPYRIGHT 2014 STMicroelectronics *****END OF FILE****/
