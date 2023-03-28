/******************************************************************************
 * @file:   semihost.s
 * @author:	xhchen
 *****************************************************************************/
				.code 16
				.syntax unified
				.text
				.thumb_func
.ifdef DEBUG_ENABLE_SEMIHOST
				.type SH_DoCommand, function
				.type HardFault_Handler, function

/* int SH_DoCommand(int n32In_R0, int n32In_R1, int *pn32Out_R0);				*/
/* Input																		*/
/*	R0,n32In_R0: semihost register 0											*/
/*	R1,n32In_R1: semihost register 1											*/
/* Output																		*/
/*	R2,*pn32Out_R0: semihost register 0											*/
/* Return																		*/
/*	0: No ICE debug																*/
/*	1: ICE debug																*/
				.global	SH_DoCommand
SH_DoCommand:
                BKPT   0xAB				/* Wait ICE or HardFault				*/
										/* ICE will step over BKPT directly		*/
										/* HardFault will step BKPT and the next line	*/
                B      SH_ICE
SH_HardFault:							/* Captured by HardFault				*/
                MOVS   R0, #0			/* Set return value to 0				*/
                BX     lr				/* Return								*/
SH_ICE:									/* Captured by ICE
				/* Save return value
				CMP    R2, #0
				BEQ    SH_End
				STR    R0, [R2]			/* Save the return value to *pn32Out_R0	*/
SH_End:
				MOVS   R0, #1			/* Set return value to 1				*/
				BX     lr				/* Return								*/

/* HardFault_Handler															*/
/* Skip the semihost command in free run mode.									*/
				.global	HardFault_Handler
HardFault_Handler:
				/* Check if the interrupt comes from thread or not              */
				/* When enterring interrupt hander                              */
				/*	if CurrentMode==Mode_Handler then                           */
				/*		LR = 0xFFFFFFF1;	(previous stack: MSP)               */
				/*	else                                                        */
				/*	if CONTROL<1> == '0' then                                   */
				/*		LR = 0xFFFFFFF9;	(previous stack: MSP)               */
				/*	else                                                        */
				/*		LR = 0xFFFFFFFD;	(previous stack: PSP)               */
				MOV		R0, LR
				LSLS	R0, R0, #29
				BMI		SP_is_PSP
				MRS		R0, MSP
				B		SP_Read_Ready
SP_is_PSP:
				MRS		R0, PSP
SP_Read_Ready:
				LDR    R1, [R0, #24]				/* Get previous PC						*/
				LDRH   R3, [R1]						/* Get instruction						*/
				LDR    R2, =0xBEAB					/* The sepcial BKPT instruction			*/
				CMP    R3, R2						/* Test if the instruction at previous PC is BKPT	*/
				BNE    HardFault_Handler_Ret		/* Not BKPT								*/

				ADDS   R1, #4						/* Skip BKPT and next line				*/
				STR    R1, [R0, #24]				/* Save previous PC						*/

				BX     LR

HardFault_Handler_Ret:
				B      .
.endif

.end
