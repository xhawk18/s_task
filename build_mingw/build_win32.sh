
gcc -m32 -O2 -s -static -I../include -Wall \
	../main.c \
	../src/s_task.c \
	../src/s_rbtree.c \
	../src/s_list.c \
	-x assembler ../asm/make_i386_ms_pe_gas.asm \
	-x assembler ../asm/jump_i386_ms_pe_gas.asm \
	-o main_win32

