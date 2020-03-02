#!/bin/bash


ST_SOURCE=(
    ../../src/s_task.c
    ../../src/s_rbtree.c
    ../../src/s_list.c
)


ASM_SOURCE=(
    -x assembler ../../asm/make_i386_ms_pe_gas.asm
    -x assembler ../../asm/jump_i386_ms_pe_gas.asm \
)


gcc -m32 -O2 -s -I../../include -Wall ../../examples/ex0_task.c "${ST_SOURCE[@]}" "${ASM_SOURCE[@]}" -o ex0_task 

gcc -m32 -O2 -s -I../../include -Wall ../../examples/ex1_event.c "${ST_SOURCE[@]}" "${ASM_SOURCE[@]}" -o ex1_event

