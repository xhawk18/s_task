cmake_minimum_required(VERSION 2.8.12)
project(s_task C ASM)
enable_language(C ASM)

message("CMAKE_SYSTEM_NAME = ${CMAKE_SYSTEM_NAME}")
message("CMAKE_SYSTEM_PROCESSOR = ${CMAKE_SYSTEM_PROCESSOR}")
message("CMAKE_C_COMPILER = ${CMAKE_C_COMPILER}")
message("CMAKE_ASM_COMPILER = ${CMAKE_ASM_COMPILER}")

add_subdirectory(libuv)

if(MSVC)
  enable_language(ASM_MASM)
  list(APPEND st_cflags /W4)
  list(APPEND st_defines _CRT_SECURE_NO_WARNINGS _CONSOLE FRAME BOOST_CONTEXT_EXPORT)
elseif(CMAKE_C_COMPILER_ID MATCHES "AppleClang|Clang|GNU")
  list(APPEND st_cflags -fvisibility=hidden)
  list(APPEND st_cflags -Wall -Wextra -Wstrict-prototypes)
  list(APPEND st_cflags -Wno-unused-parameter -Wno-deprecated-declarations)
  list(APPEND st_cflags -static)
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
    src/s_rbtree.c
    src/s_uv.c)

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

add_library(s_task_uv ${st_sources})
target_compile_definitions(s_task_uv PUBLIC ${st_defines} USE_LIBUV)
target_compile_options(s_task_uv PUBLIC ${st_cflags})
target_include_directories(s_task_uv PUBLIC include asm libuv/include)
target_link_libraries(s_task_uv PUBLIC uv_a)

add_library(s_task ${st_sources})
target_compile_definitions(s_task PUBLIC ${st_defines})
target_compile_options(s_task PUBLIC ${st_cflags})
target_include_directories(s_task PUBLIC include asm libuv/include)

add_executable(ex0_task examples/ex0_task.c)
target_link_libraries(ex0_task s_task)

add_executable(ex1_event examples/ex1_event.c)
target_link_libraries(ex1_event s_task)

add_executable(ex2_libuv examples/ex2_libuv.c)
target_link_libraries(ex2_libuv s_task_uv)

add_executable(ex3_http_client examples/ex3_http_client.c)
target_link_libraries(ex3_http_client s_task_uv)

add_executable(ex4_echo_server examples/ex4_echo_server.c)
target_link_libraries(ex4_echo_server s_task_uv)

add_executable(ex5_led_blinking examples/ex5_led_blinking.c)
target_link_libraries(ex5_led_blinking s_task)

add_executable(ex6_chan examples/ex6_chan.c)
target_link_libraries(ex6_chan s_task)
