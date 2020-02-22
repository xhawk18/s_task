aarch64-linux-gnu-gcc -O2 -s -static -I../include \
	../main.c \
	../src/s_task.c \
	../src/s_rbtree.c \
	../src/s_list.c \
	../asm/make_arm64_aapcs_elf_gas.S \
	../asm/jump_arm64_aapcs_elf_gas.S \
	-o main_arm64

