  /*
  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
  */

  .file "make_riscv32_elf_gas.S"
  .text
  .align  1
  .global make_fcontext
  .type   make_fcontext, %function
make_fcontext:

  #if __riscv_xlen == 64
  .set GP_REGSIZE, 8
  #else
  .set GP_REGSIZE, 4
  #endif

  # GP寄存器数量：s0-s11(12个) + ra(1个) + pc(1个) = 14个
  .set GP_COUNT, 14
  .set GP_SIZE, GP_COUNT * GP_REGSIZE

  #ifdef __riscv_flen
  #if __riscv_flen == 64
  .set FP_REGSIZE, 8
  #else
  .set FP_REGSIZE, 4
  #endif
  .set FP_COUNT, 12
  .set FP_SIZE, FP_COUNT * FP_REGSIZE
  .set STACK_SIZE, GP_SIZE + FP_SIZE
  #else
  .set STACK_SIZE, GP_SIZE
  #endif

  # 定义GP寄存器偏移
  .set OFF_s0,    0 * GP_REGSIZE
  .set OFF_s1,    1 * GP_REGSIZE
  .set OFF_s2,    2 * GP_REGSIZE
  .set OFF_s3,    3 * GP_REGSIZE
  .set OFF_s4,    4 * GP_REGSIZE
  .set OFF_s5,    5 * GP_REGSIZE
  .set OFF_s6,    6 * GP_REGSIZE
  .set OFF_s7,    7 * GP_REGSIZE
  .set OFF_s8,    8 * GP_REGSIZE
  .set OFF_s9,    9 * GP_REGSIZE
  .set OFF_s10,  10 * GP_REGSIZE
  .set OFF_s11,  11 * GP_REGSIZE
  .set OFF_ra,   12 * GP_REGSIZE
  .set OFF_pc,   13 * GP_REGSIZE

  # 对齐a0到4字节边界（与原代码保持一致）
  andi a0, a0, ~0x3

  # 为上下文数据保留空间（STACK_SIZE根据上面条件计算）
  addi a0, a0, -STACK_SIZE

  # a2是第三个参数：context-function的地址
  # 将其存为要跳转的PC
  #if __riscv_xlen == 64
  sd a2, OFF_pc(a0)
  #else
  sw a2, OFF_pc(a0)
  #endif

  # 保存finish作为返回地址（RA寄存器初始值）
  lla a4, finish
  #if __riscv_xlen == 64
  sd a4, OFF_ra(a0)
  #else
  sw a4, OFF_ra(a0)
  #endif

  # 返回a0作为context-data指针
  ret

finish:
  # 结束时返回码0
  li  a0, 0
  # 调用_exit结束程序
  tail _exit@plt

  .size   make_fcontext,.-make_fcontext
  .section .note.GNU-stack,"",%progbits

