/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright (c) Nuvoton Technology Corp. All rights reserved.                                             */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include "M051Series.h"
#include "GPIO.h"
#include "M05x_registers.h"

#define GPIO_PMD_PMD0_MASK GPIO_PMD_PMD0_QUASI
#define GPIO_PMD_PMD2_MASK GPIO_PMD_PMD2_QUASI
#define GPIO_PMD_PMD3_MASK GPIO_PMD_PMD3_QUASI
#define GPIO_PMD_PMD5_MASK GPIO_PMD_PMD5_QUASI


void test_c1(){
	P0->PMD = P0->PMD & ~GPIO_PMD_PMD0_MASK | GPIO_PMD_PMD0_INPUT;
	P0->PMD = P0->PMD & ~GPIO_PMD_PMD2_MASK | GPIO_PMD_PMD2_OUTPUT;
	P0->PMD = P0->PMD & ~GPIO_PMD_PMD3_MASK | GPIO_PMD_PMD3_OUTPUT;
}

void test_c2(){
	P0->PMD = P0->PMD
		& ~( GPIO_PMD_PMD0_MASK
		   | GPIO_PMD_PMD2_MASK
		   | GPIO_PMD_PMD3_MASK
		   ) 
		| ( GPIO_PMD_PMD0_INPUT
		  | GPIO_PMD_PMD2_OUTPUT
		  | GPIO_PMD_PMD3_OUTPUT
		  | GPIO_PMD_PMD5_INPUT
		  );
}

void test_cpp(){
	P0s.PMD()
		.PMD0(GPIO_PMD_INPUT)
		.PMD2(GPIO_PMD_OUTPUT)
		.PMD3(GPIO_PMD_OUTPUT);
}


int main(void)
{
	test_c1();
	test_c2();
	test_cpp();
    
	return 0;
}
