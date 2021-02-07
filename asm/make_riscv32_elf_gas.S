/*
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENSE_1_0.txt or copy at
          http://www.boost.org/LICENSE_1_0.txt)
*/

.file "make_riscv64_elf_gas.S"
.text
.align  1
.global make_fcontext
.type   make_fcontext, %function
make_fcontext:
    # shift address in a0 (allocated stack) to lower 4 byte boundary
    andi a0, a0, ~0x3

    # reserve space for context-data on context-stack
#ifdef __riscv_flen
    addi  a0, a0, -0x68
#else
    addi  a0, a0, -0x38
#endif

    # third arg of make_fcontext() == address of context-function
    # store address as a PC to jump in
    sw  a2, 0x34(a0)

    # save address of finish as return-address for context-function
    # will be entered after context-function returns (RA register)
    lla  a4, finish
    sw  a4, 0x30(a0)

    ret // return pointer to context-data (a0)

finish:
    # exit code is zero
    li  a0, 0
    # exit application
    tail  _exit@plt

.size   make_fcontext,.-make_fcontext
# Mark that we don't need executable stack.
.section .note.GNU-stack,"",%progbits
