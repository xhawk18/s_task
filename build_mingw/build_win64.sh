
gcc -m64 -o main -O2 -s -static -I.. ../main.c ../s_task.c ../s_rbtree.c ../s_list.c -x assembler ../asm/make_x86_64_ms_pe_gas.asm -x assembler ../asm/jump_x86_64_ms_pe_gas.asm


