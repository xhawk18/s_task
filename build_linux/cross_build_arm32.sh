
arm-linux-gnueabihf-gcc -O2 -s -static -I../include \
	../main.c \
	../src/s_task.c \
	../src/s_rbtree.c \
	../src/s_list.c \
	../asm/make_arm_aapcs_elf_gas.S \
	../asm/jump_arm_aapcs_elf_gas.S \
	-o main_arm32

