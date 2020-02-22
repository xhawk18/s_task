
gcc -m32 -O2 -s -static -I../include \
	../main.c \
	../src/s_task.c \
	../src/s_rbtree.c \
	../src/s_list.c \
	../asm/make_i386_sysv_elf_gas.S \
	../asm/jump_i386_sysv_elf_gas.S
	-o main_x32

