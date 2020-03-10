#!/bin/bash


ST_SOURCE=(
    ../../src/s_task.c
    ../../src/s_rbtree.c
    ../../src/s_list.c
    ../../asm/make_gas.S
    ../../asm/jump_gas.S
)


gcc -O2 -s -I../../include -Wall "${ST_SOURCE[@]}" ../../examples/ex0_task.c -o ex0_task 

gcc -O2 -s -I../../include -Wall "${ST_SOURCE[@]}" ../../examples/ex1_event.c -o ex1_event

