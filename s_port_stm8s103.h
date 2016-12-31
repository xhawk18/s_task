#ifndef INC_S_PORT_H_
#define INC_S_PORT_H_

#include "stm8s.h"

static void TIM2_Config(void){
	//Ê±ÖÓÆµÂÊ16M
	/* Time base configuration */      
	TIM2_TimeBaseInit(TIM2_PRESCALER_1, 0xFFFF);

	/* Prescaler configuration */
	TIM2_PrescalerConfig(TIM2_PRESCALER_16384, TIM2_PSCRELOADMODE_IMMEDIATE);

	TIM2_ARRPreloadConfig(ENABLE);
  
	TIM2_ClearFlag(TIM2_FLAG_UPDATE);     
	//TIM2_ITConfig(TIM2_IT_UPDATE,ENABLE); 

	//TIM2_UpdateDisableConfig(ENABLE);

	/* TIM2 enable counter */
	TIM2_Cmd(ENABLE);
}

static void CLK_Config(void){
	CLK_DeInit();
    /* Configure the Fcpu to DIV1*/
    CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);
    /* Configure the HSI prescaler to the optimal value */
    CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);
    /* Configure the system clock to use HSI clock source and to run at 16Mhz */
    CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSI, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif


/* Copyright xhawk, MIT license */
typedef unsigned long long uint64_t;

/* Size for stack for each stask. */
#define STACK_SIZE 256

/* The original jmp_buf is too large and waste RAM, Use smaller jmb_buf instead! */
typedef struct {
    int buf[12 / sizeof(int)];
} my_jmp_buf_t;
#define MY_JMP_BUF my_jmp_buf_t

/* Timer functions need to be implemented on a new porting. */

/* === Timer functions on posix/linux === */
#include <time.h>

//1. define a type for clock
typedef uint16_t my_clock_t;

//2. define the clock ticks count for one second
#define MY_CLOCKS_PER_SEC (16*1000*1000/16384)

//3. Implement the initilization function for clock. Leave it blank if not required.
void my_clock_init(){
	CLK_Config();
	TIM2_Config();
	//enableInterrupts();
}

//4. Implement the function of getting current clock ticks.
my_clock_t my_clock() {
	uint16_t count;// = TIM2_GetCounter();	
	count =  ((uint16_t)TIM2->CNTRH << 8);
	count = (uint16_t)( count| (uint16_t)(TIM2->CNTRL));
	return count;
}

//5. Implement the idle delay function.
void my_on_idle(uint64_t max_idle_ms) {
    
}

#endif

