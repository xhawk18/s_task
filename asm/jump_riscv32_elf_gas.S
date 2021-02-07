/*
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENSE_1_0.txt or copy at
          http://www.boost.org/LICENSE_1_0.txt)
*/


.file "jump_riscv64_elf_gas.S"
.text
.align  1
.global jump_fcontext
.type   jump_fcontext, %function
jump_fcontext:
#ifdef __riscv_flen
    # prepare stack for GP + FPU
    addi  sp, sp, -0x68

    # save fs0 - fs11
    fsw  f0, 0x38(sp)
    fsw  f1, 0x3c(sp)
    fsw  f2, 0x40(sp)
    fsw  f3, 0x44(sp)
    fsw  f4, 0x48(sp)
    fsw  f5, 0x4c(sp)
    fsw  f6, 0x50(sp)
    fsw  f7, 0x54(sp)
    fsw  f8, 0x58(sp)
    fsw  f9, 0x5c(sp)
    fsw  f10, 0x60(sp)
    fsw  f11, 0x64(sp)

#else
    # prepare stack for GP
    addi  sp, sp, -0x38
#endif

    # save s0-s11, ra
    sw  s0, 0x00(sp)
    sw  s1, 0x04(sp)
    sw  s2, 0x08(sp)
    sw  s3, 0x0c(sp)
    sw  s4, 0x10(sp)
    sw  s5, 0x14(sp)
    sw  s6, 0x18(sp)
    sw  s7, 0x1c(sp)
    sw  s8, 0x20(sp)
    sw  s9, 0x24(sp)
    sw  s10, 0x28(sp)
    sw  s11, 0x2c(sp)
    
    sw  ra, 0x30(sp)

    # save RA as PC
    sw  ra, 0x34(sp)

    # store SP (pointing to context-data) in A2
    mv  a2, sp

    # restore SP (pointing to context-data) from A0
    mv  sp, a0

#ifdef __riscv_flen
    # load fs0 - fs11
    flw  f0, 0x38(sp)
    flw  f1, 0x3c(sp)
    flw  f2, 0x40(sp)
    flw  f3, 0x44(sp)
    flw  f4, 0x48(sp)
    flw  f5, 0x4c(sp)
    flw  f6, 0x50(sp)
    flw  f7, 0x54(sp)
    flw  f8, 0x58(sp)
    flw  f9, 0x5c(sp)
    flw  f10, 0x60(sp)
    flw  f11, 0x64(sp)
#endif
    
    # load s0-s11,ra
    lw  s0, 0x00(sp)
    lw  s1, 0x04(sp)
    lw  s2, 0x08(sp)
    lw  s3, 0x0c(sp)
    lw  s4, 0x10(sp)
    lw  s5, 0x14(sp)
    lw  s6, 0x18(sp)
    lw  s7, 0x1c(sp)
    lw  s8, 0x20(sp)
    lw  s9, 0x24(sp)
    lw  s10, 0x28(sp)
    lw  s11, 0x2c(sp)
    lw  ra, 0x30(sp)

    # return transfer_t from jump
    # pass transfer_t as first arg in context function
    # a0 == FCTX, a1 == DATA
    mv a0, a2

    # load pc
    lw  a2, 0x34(sp)

    # restore stack from GP + FPU
#ifdef __riscv_flen
    addi  sp, sp, 0x68
#else
    addi  sp, sp, 0x38
#endif

    jr a2
.size   jump_fcontext,.-jump_fcontext
# Mark that we don't need executable stack.
.section .note.GNU-stack,"",%progbits
