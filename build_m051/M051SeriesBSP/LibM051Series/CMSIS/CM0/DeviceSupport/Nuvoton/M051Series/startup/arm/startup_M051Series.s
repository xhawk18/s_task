;/**************************************************************************//**
; * @file     startup_M051Series.s
; * @version  V2.00
; * $Revision: 2 $
; * $Date: 4/09/12 2:22p $ 
; * @brief    M051 Series Startup Source File
; *
; * @note
; * Copyright (C) 2011 Nuvoton Technology Corp. All rights reserved.
; *
; ******************************************************************************/


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;



Stack_Size      EQU     0x00000200

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x00000000

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit


                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset
                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors

__Vectors       DCD     __initial_sp              ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
                DCD     NMI_Handler               ; NMI Handler
                DCD     HardFault_Handler         ; Hard Fault Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler

                ; External Interrupts
                                                  ; maximum of 32 External Interrupts are possible
                DCD     BOD_IRQHandler  
                DCD     WDT_IRQHandler  
                DCD     EINT0_IRQHandler
                DCD     EINT1_IRQHandler
                DCD     GPIOP0P1_IRQHandler 
                DCD     GPIOP2P3P4_IRQHandler
                DCD     PWMA_IRQHandler 
                DCD     PWMB_IRQHandler 
                DCD     TMR0_IRQHandler 
                DCD     TMR1_IRQHandler 
                DCD     TMR2_IRQHandler 
                DCD     TMR3_IRQHandler 
                DCD     UART0_IRQHandler
                DCD     UART1_IRQHandler
                DCD     SPI0_IRQHandler 
                DCD     SPI1_IRQHandler 
                DCD     Default_Handler 
                DCD     Default_Handler 
                DCD     I2C0_IRQHandler 
                DCD     I2C1_IRQHandler 
                DCD     Default_Handler 
                DCD     Default_Handler
                DCD     Default_Handler 
                DCD     Default_Handler  
                DCD     Default_Handler  
                DCD     ACMP0_IRQHandler 
                DCD     ACMP1_IRQHandler
                DCD     Default_Handler 
                DCD     PWRWU_IRQHandler
                DCD     ADC_IRQHandler
                DCD     Default_Handler  
                DCD     RTC_IRQHandler  
                                
                
                AREA    |.text|, CODE, READONLY
                
                
                
; Reset Handler 
                
                ENTRY
                
Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  SystemInit
                IMPORT  __main
                
                LDR     R0, =0x50000100
                ; Unlock Register                

                LDR     R1, =0x59
                STR     R1, [R0]
                LDR     R1, =0x16
                STR     R1, [R0]
                LDR     R1, =0x88
                STR     R1, [R0]

                ; Init POR
                LDR     R2, =0x50000024
                LDR     R1, =0x00005AA5
                STR     R1, [R2]

                ; Lock register
                MOVS    R1, #0
                STR     R1, [R0]                
                
                LDR     R0, =SystemInit
                BLX     R0
                LDR     R0, =__main
                BX      R0
                ENDP

                
                
; Dummy Exception Handlers (infinite loops which can be modified)                
                
NMI_Handler     PROC
                EXPORT  NMI_Handler               [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler         [WEAK]

    IF :DEF:DEBUG_ENABLE_SEMIHOST
                ; Check if the interrupt comes from thread or not
                ; When enterring interrupt hander
                ;    if CurrentMode==Mode_Handler then
                ;        LR = 0xFFFFFFF1;	(previous stack: MSP)
                ;    else
                ;    if CONTROL<1> == '0' then
                ;        LR = 0xFFFFFFF9;	(previous stack: MSP)
                ;    else
                ;        LR = 0xFFFFFFFD;	(previous stack: PSP)
                MOV     R0, LR
                LSLS    R0, R0, #29           ; Check bit 2
                BMI     SP_is_PSP             ; previous stack is PSP
                MRS     R0, MSP               ; previous stack is MSP, read MSP
                B       SP_Read_Ready
SP_is_PSP
                MRS     R0, PSP               ; Read PSP
SP_Read_Ready
                LDR     R1, [R0, #24]         ; Get previous PC
                LDRH    R3, [R1]              ; Get instruction
                LDR     R2, =0xBEAB           ; The sepcial BKPT instruction
                CMP     R3, R2                ; Test if the instruction at previous PC is BKPT
                BNE     HardFault_Handler_Ret ; Not BKPT
        
                ADDS    R1, #4                ; Skip BKPT and next line
                STR     R1, [R0, #24]         ; Save previous PC
        
                BX      LR
HardFault_Handler_Ret
    ENDIF

                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler               [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler            [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler           [WEAK]
                B       .
                ENDP

Default_Handler PROC

                EXPORT  BOD_IRQHandler            [WEAK]
                EXPORT  WDT_IRQHandler            [WEAK]
                EXPORT  EINT0_IRQHandler          [WEAK]
                EXPORT  EINT1_IRQHandler          [WEAK]
                EXPORT  GPIOP0P1_IRQHandler       [WEAK]
                EXPORT  GPIOP2P3P4_IRQHandler     [WEAK]
                EXPORT  PWMA_IRQHandler           [WEAK]
                EXPORT  PWMB_IRQHandler           [WEAK]
                EXPORT  TMR0_IRQHandler           [WEAK]
                EXPORT  TMR1_IRQHandler           [WEAK]
                EXPORT  TMR2_IRQHandler           [WEAK]
                EXPORT  TMR3_IRQHandler           [WEAK]
                EXPORT  UART0_IRQHandler          [WEAK]
                EXPORT  UART1_IRQHandler          [WEAK]
                EXPORT  SPI0_IRQHandler           [WEAK]
                EXPORT  SPI1_IRQHandler           [WEAK]
                EXPORT  I2C0_IRQHandler           [WEAK]
                EXPORT  I2C1_IRQHandler           [WEAK]
                EXPORT  ACMP0_IRQHandler          [WEAK]
                EXPORT  ACMP1_IRQHandler          [WEAK]
                EXPORT  PWRWU_IRQHandler          [WEAK]
                EXPORT  ADC_IRQHandler            [WEAK]
                EXPORT  RTC_IRQHandler            [WEAK]
                
BOD_IRQHandler
WDT_IRQHandler
EINT0_IRQHandler
EINT1_IRQHandler
GPIOP0P1_IRQHandler
GPIOP2P3P4_IRQHandler
PWMA_IRQHandler
PWMB_IRQHandler
TMR0_IRQHandler
TMR1_IRQHandler
TMR2_IRQHandler
TMR3_IRQHandler
UART0_IRQHandler
UART1_IRQHandler
SPI0_IRQHandler
SPI1_IRQHandler
I2C0_IRQHandler
I2C1_IRQHandler
ACMP0_IRQHandler
ACMP1_IRQHandler
PWRWU_IRQHandler
ADC_IRQHandler
RTC_IRQHandler
                B       .
                ENDP


                ALIGN


; User Initial Stack & Heap

                IF      :DEF:__MICROLIB
                
                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit
                
                ELSE
                
                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap
__user_initial_stackheap

                LDR     R0, =  Heap_Mem
                LDR     R1, = (Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR

                ALIGN

                ENDIF

    IF :DEF:DEBUG_ENABLE_SEMIHOST

                ALIGN

; int SH_DoCommand(int n32In_R0, int n32In_R1, int *pn32Out_R0);
; Input
;   R0,n32In_R0: semihost register 0
;   R1,n32In_R1: semihost register 1
; Output
;   R2,*pn32Out_R0: semihost register 0
; Return
;   0: No ICE debug
;   1: ICE debug
SH_DoCommand  PROC  
                EXPORT SH_DoCommand
                BKPT   0xAB                  ; Wait ICE or HardFault
                                     ; ICE will step over BKPT directly
                                     ; HardFault will step BKPT and the next line
                B      SH_ICE
SH_HardFault                         ; Captured by HardFault
                MOVS   R0, #0                ; Set return value to 0
                BX     lr                    ; Return
SH_ICE                               ; Captured by ICE
                ; Save return value
                CMP    R2, #0
                BEQ    SH_End
                STR    R0, [R2]              ; Save the return value to *pn32Out_R0
SH_End
                MOVS   R0, #1                ; Set return value to 1
                BX     lr                    ; Return

                ALIGN
				ENDP
    ENDIF


                END
