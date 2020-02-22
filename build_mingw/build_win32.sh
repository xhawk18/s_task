
gcc -m32 -o main -O2 -s -static -I.. ../main.c ../s_task.c ../s_rbtree.c ../s_list.c -x assembler ../asm/make_i386_ms_pe_gas.asm -x assembler ../asm/jump_i386_ms_pe_gas.asm


