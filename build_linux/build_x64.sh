
gcc -m64 -O2 -s -static -I../include -Wall \
	../main.c \
	../src/s_task.c \
	../src/s_rbtree.c \
	../src/s_list.c \
	../asm/make_x86_64_sysv_elf_gas.S \
	../asm/jump_x86_64_sysv_elf_gas.S \
	-o main_x64

