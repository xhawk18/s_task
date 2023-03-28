cmake_minimum_required(VERSION 2.8.12)
project(s_task C ASM)
enable_language(C ASM)

if(MSVC)
  enable_language(ASM_MASM)
  list(APPEND st_cflags /W4)
  list(APPEND st_defines _CRT_SECURE_NO_WARNINGS _CONSOLE FRAME BOOST_CONTEXT_EXPORT)
elseif(CMAKE_C_COMPILER_ID MATCHES "AppleClang|Clang|GNU")
  list(APPEND st_cflags -fvisibility=hidden)
  list(APPEND st_cflags -Wall -Wextra -Wstrict-prototypes)
  list(APPEND st_cflags -Wno-unused-parameter -Wno-deprecated-declarations)
  #list(APPEND st_cflags -static)
endif()

set(st_sources
    src/s_task.c
    src/s_mutex.c
    src/s_event.c
    src/s_event_embed.c
    src/s_chan.c
    src/s_chan_embed.c
    src/s_timer_fast.c
    src/s_timer_small.c
    src/s_list.c
    src/s_rbtree.c)

if(MSVC)
  if(CMAKE_CL_64)
    list(APPEND st_sources
      asm/jump_x86_64_ms_pe_masm.asm
      asm/make_x86_64_ms_pe_masm.asm)
  else()
    list(APPEND st_sources
      asm/jump_i386_ms_pe_masm.asm
      asm/make_i386_ms_pe_masm.asm)
  endif()
else()
  list(APPEND st_sources
    asm/make_gas.S
    asm/jump_gas.S)
endif()

add_library(s_task ${st_sources})
target_compile_definitions(s_task PUBLIC ${st_defines} USE_LIBUV)
target_compile_options(s_task PUBLIC ${st_cflags})
target_include_directories(s_task PUBLIC include asm)
