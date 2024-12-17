  /*
  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
  */


  .file "jump_riscv32_elf_gas.S"
  .text
  .align  1
  .global jump_fcontext
  .type   jump_fcontext, %function
jump_fcontext:

  #if __riscv_xlen == 64
  .set GP_REGSIZE, 8
  #else
  .set GP_REGSIZE, 4
  #endif

  # GP共14个条目: s0-s11(12), ra(1), pc(1)
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

  # 寄存器偏移定义(GP)
  .set OFF_s0,     0 * GP_REGSIZE
  .set OFF_s1,     1 * GP_REGSIZE
  .set OFF_s2,     2 * GP_REGSIZE
  .set OFF_s3,     3 * GP_REGSIZE
  .set OFF_s4,     4 * GP_REGSIZE
  .set OFF_s5,     5 * GP_REGSIZE
  .set OFF_s6,     6 * GP_REGSIZE
  .set OFF_s7,     7 * GP_REGSIZE
  .set OFF_s8,     8 * GP_REGSIZE
  .set OFF_s9,     9 * GP_REGSIZE
  .set OFF_s10,   10 * GP_REGSIZE
  .set OFF_s11,   11 * GP_REGSIZE
  .set OFF_ra,    12 * GP_REGSIZE
  .set OFF_pc,    13 * GP_REGSIZE

  #ifdef __riscv_flen
  # FPU寄存器偏移，从GP_SIZE开始
  .set OFF_fs0,   GP_SIZE + (0  * FP_REGSIZE)
  .set OFF_fs1,   GP_SIZE + (1  * FP_REGSIZE)
  .set OFF_fs2,   GP_SIZE + (2  * FP_REGSIZE)
  .set OFF_fs3,   GP_SIZE + (3  * FP_REGSIZE)
  .set OFF_fs4,   GP_SIZE + (4  * FP_REGSIZE)
  .set OFF_fs5,   GP_SIZE + (5  * FP_REGSIZE)
  .set OFF_fs6,   GP_SIZE + (6  * FP_REGSIZE)
  .set OFF_fs7,   GP_SIZE + (7  * FP_REGSIZE)
  .set OFF_fs8,   GP_SIZE + (8  * FP_REGSIZE)
  .set OFF_fs9,   GP_SIZE + (9  * FP_REGSIZE)
  .set OFF_fs10,  GP_SIZE + (10 * FP_REGSIZE)
  .set OFF_fs11,  GP_SIZE + (11 * FP_REGSIZE)
  #endif

  # 调整栈指针
  addi sp, sp, -STACK_SIZE

  # 保存GP寄存器
  #if __riscv_xlen == 64
  sd s0,  OFF_s0(sp)
  sd s1,  OFF_s1(sp)
  sd s2,  OFF_s2(sp)
  sd s3,  OFF_s3(sp)
  sd s4,  OFF_s4(sp)
  sd s5,  OFF_s5(sp)
  sd s6,  OFF_s6(sp)
  sd s7,  OFF_s7(sp)
  sd s8,  OFF_s8(sp)
  sd s9,  OFF_s9(sp)
  sd s10, OFF_s10(sp)
  sd s11, OFF_s11(sp)
  sd ra,  OFF_ra(sp)
  sd ra,  OFF_pc(sp)  # pc位置存ra以备后续跳转
  #else
  sw s0,  OFF_s0(sp)
  sw s1,  OFF_s1(sp)
  sw s2,  OFF_s2(sp)
  sw s3,  OFF_s3(sp)
  sw s4,  OFF_s4(sp)
  sw s5,  OFF_s5(sp)
  sw s6,  OFF_s6(sp)
  sw s7,  OFF_s7(sp)
  sw s8,  OFF_s8(sp)
  sw s9,  OFF_s9(sp)
  sw s10, OFF_s10(sp)
  sw s11, OFF_s11(sp)
  sw ra,  OFF_ra(sp)
  sw ra,  OFF_pc(sp)   # pc位置存ra
  #endif

  # a2存储当前上下文指针
  mv a2, sp

  # sp 指向A0给出的新上下文
  mv sp, a0

  #ifdef __riscv_flen
  # 保存FPU寄存器
  #if __riscv_flen == 64
  fsd fs0,  OFF_fs0(a2)
  fsd fs1,  OFF_fs1(a2)
  fsd fs2,  OFF_fs2(a2)
  fsd fs3,  OFF_fs3(a2)
  fsd fs4,  OFF_fs4(a2)
  fsd fs5,  OFF_fs5(a2)
  fsd fs6,  OFF_fs6(a2)
  fsd fs7,  OFF_fs7(a2)
  fsd fs8,  OFF_fs8(a2)
  fsd fs9,  OFF_fs9(a2)
  fsd fs10, OFF_fs10(a2)
  fsd fs11, OFF_fs11(a2)

  # 恢复新上下文的FPU寄存器
  fld fs0,  OFF_fs0(sp)
  fld fs1,  OFF_fs1(sp)
  fld fs2,  OFF_fs2(sp)
  fld fs3,  OFF_fs3(sp)
  fld fs4,  OFF_fs4(sp)
  fld fs5,  OFF_fs5(sp)
  fld fs6,  OFF_fs6(sp)
  fld fs7,  OFF_fs7(sp)
  fld fs8,  OFF_fs8(sp)
  fld fs9,  OFF_fs9(sp)
  fld fs10, OFF_fs10(sp)
  fld fs11, OFF_fs11(sp)
  #else
  fsw fs0,  OFF_fs0(a2)
  fsw fs1,  OFF_fs1(a2)
  fsw fs2,  OFF_fs2(a2)
  fsw fs3,  OFF_fs3(a2)
  fsw fs4,  OFF_fs4(a2)
  fsw fs5,  OFF_fs5(a2)
  fsw fs6,  OFF_fs6(a2)
  fsw fs7,  OFF_fs7(a2)
  fsw fs8,  OFF_fs8(a2)
  fsw fs9,  OFF_fs9(a2)
  fsw fs10, OFF_fs10(a2)
  fsw fs11, OFF_fs11(a2)

  # 恢复新上下文的FPU寄存器
  flw fs0,  OFF_fs0(sp)
  flw fs1,  OFF_fs1(sp)
  flw fs2,  OFF_fs2(sp)
  flw fs3,  OFF_fs3(sp)
  flw fs4,  OFF_fs4(sp)
  flw fs5,  OFF_fs5(sp)
  flw fs6,  OFF_fs6(sp)
  flw fs7,  OFF_fs7(sp)
  flw fs8,  OFF_fs8(sp)
  flw fs9,  OFF_fs9(sp)
  flw fs10, OFF_fs10(sp)
  flw fs11, OFF_fs11(sp)
  #endif
  #endif

  # 恢复GP寄存器
  #if __riscv_xlen == 64
  ld s0,  OFF_s0(sp)
  ld s1,  OFF_s1(sp)
  ld s2,  OFF_s2(sp)
  ld s3,  OFF_s3(sp)
  ld s4,  OFF_s4(sp)
  ld s5,  OFF_s5(sp)
  ld s6,  OFF_s6(sp)
  ld s7,  OFF_s7(sp)
  ld s8,  OFF_s8(sp)
  ld s9,  OFF_s9(sp)
  ld s10, OFF_s10(sp)
  ld s11, OFF_s11(sp)
  ld ra,  OFF_ra(sp)
  #else
  lw s0,  OFF_s0(sp)
  lw s1,  OFF_s1(sp)
  lw s2,  OFF_s2(sp)
  lw s3,  OFF_s3(sp)
  lw s4,  OFF_s4(sp)
  lw s5,  OFF_s5(sp)
  lw s6,  OFF_s6(sp)
  lw s7,  OFF_s7(sp)
  lw s8,  OFF_s8(sp)
  lw s9,  OFF_s9(sp)
  lw s10, OFF_s10(sp)
  lw s11, OFF_s11(sp)
  lw ra,  OFF_ra(sp)
  #endif

  # 返回参数 a0 指向旧上下文，a1保留使用者处理
  mv a0, a2

  # load pc
  #if __riscv_xlen == 64
  ld  a2, OFF_pc(sp)
  #else
  lw a2,  OFF_pc(sp)
  #endif

  # 恢复栈指针
  addi sp, sp, STACK_SIZE

  jr a2
  .size jump_fcontext, .-jump_fcontext

  .section .note.GNU-stack,"",%progbits

