// Stub file for universal binary

#if defined(__APPLE__)
    #include "jump_combined_sysv_macho_gas.S"
#elif defined(__x86_64__)
    #include "jump_x86_64_sysv_elf_gas.S"
#elif defined(__i386__)
    #include "jump_i386_sysv_elf_gas.S"
#elif defined(__ppc64__)
    #include "jump_ppc64_sysv_elf_gas.S"
#elif defined(__ppc__)
    #include "jump_ppc32_sysv_elf_gas.S"
#elif defined(__aarch64__)
    #include "jump_arm64_aapcs_elf_gas.S"
#elif defined(__arm__)
    #include "jump_arm_aapcs_elf_gas.S"
#elif defined(__mips64__)
    #include "jump_mips64_n64_elf_gas.S"
#elif defined(__mips__)
    #include "jump_mips32_o32_elf_gas.S"
#else
    #error "No arch's"
#endif
