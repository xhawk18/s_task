cmake_minimum_required(VERSION 2.8)
project(s_task C ASM)
enable_language(C ASM)

message("CMAKE_SYSTEM_NAME = ${CMAKE_SYSTEM_NAME}")
message("CMAKE_SYSTEM_PROCESSOR = ${CMAKE_SYSTEM_PROCESSOR}")
message("CMAKE_C_COMPILER = ${CMAKE_C_COMPILER}")
message("CMAKE_ASM_COMPILER = ${CMAKE_ASM_COMPILER}")

macro(st_add_link_flags target_name)
  set(st_link_flags )
  if(MSVC)
    if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.13.0")
      target_link_options(${target_name} PRIVATE /SAFESEH:NO)
    endif()
  elseif(WIN32 OR MINGW)
    list(APPEND st_link_flags -static -s)
  else()
    #list(APPEND st_link_flags -static -s)
  endif()
endmacro()

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
    ../src/s_task.c
    ../src/s_mutex.c
    ../src/s_event.c
    ../src/s_event_embed.c
    ../src/s_chan.c
    ../src/s_chan_embed.c
    ../src/s_timer_fast.c
    ../src/s_timer_small.c
    ../src/s_list.c
    ../src/s_rbtree.c)

if(MSVC)
  if(CMAKE_CL_64)
    list(APPEND st_sources
      ../asm/jump_x86_64_ms_pe_masm.asm
      ../asm/make_x86_64_ms_pe_masm.asm)
  else()
    list(APPEND st_sources
      ../asm/jump_i386_ms_pe_masm.asm
      ../asm/make_i386_ms_pe_masm.asm)
  endif()
else()
  list(APPEND st_sources
    ../asm/make_gas.S
    ../asm/jump_gas.S)
endif()

set(uv_sources
    ../src/s_uv.c
    ../libuv/src/fs-poll.c
    ../libuv/src/idna.c
    ../libuv/src/inet.c
    ../libuv/src/strscpy.c
    ../libuv/src/threadpool.c
    ../libuv/src/timer.c
    ../libuv/src/uv-common.c
    ../libuv/src/uv-data-getter-setters.c
    ../libuv/src/version.c)

if(WIN32 OR MINGW)
  list(APPEND st_defines WIN32_LEAN_AND_MEAN _WIN32_WINNT=0x0600)
  list(APPEND st_libraries
       advapi32
       iphlpapi
       psapi
       shell32
       user32
       userenv
       ws2_32)
  list(APPEND uv_sources
       ../libuv/src/win/async.c
       ../libuv/src/win/core.c
       ../libuv/src/win/detect-wakeup.c
       ../libuv/src/win/dl.c
       ../libuv/src/win/error.c
       ../libuv/src/win/fs.c
       ../libuv/src/win/fs-event.c
       ../libuv/src/win/getaddrinfo.c
       ../libuv/src/win/getnameinfo.c
       ../libuv/src/win/handle.c
       ../libuv/src/win/loop-watcher.c
       ../libuv/src/win/pipe.c
       ../libuv/src/win/thread.c
       ../libuv/src/win/poll.c
       ../libuv/src/win/process.c
       ../libuv/src/win/process-stdio.c
       ../libuv/src/win/signal.c
       ../libuv/src/win/snprintf.c
       ../libuv/src/win/stream.c
       ../libuv/src/win/tcp.c
       ../libuv/src/win/tty.c
       ../libuv/src/win/udp.c
       ../libuv/src/win/util.c
       ../libuv/src/win/winapi.c
       ../libuv/src/win/winsock.c)
else()
  list(APPEND st_defines _FILE_OFFSET_BITS=64 _LARGEFILE_SOURCE)
  if(NOT CMAKE_SYSTEM_NAME STREQUAL "Android")
    # Android has pthread as part of its c library, not as a separate
    # libpthread.so.
    list(APPEND st_libraries pthread)
  endif()
  list(APPEND uv_sources
       ../libuv/src/unix/async.c
       ../libuv/src/unix/core.c
       ../libuv/src/unix/dl.c
       ../libuv/src/unix/fs.c
       ../libuv/src/unix/getaddrinfo.c
       ../libuv/src/unix/getnameinfo.c
       ../libuv/src/unix/loop-watcher.c
       ../libuv/src/unix/loop.c
       ../libuv/src/unix/pipe.c
       ../libuv/src/unix/poll.c
       ../libuv/src/unix/process.c
       ../libuv/src/unix/signal.c
       ../libuv/src/unix/stream.c
       ../libuv/src/unix/tcp.c
       ../libuv/src/unix/thread.c
       ../libuv/src/unix/tty.c
       ../libuv/src/unix/udp.c)
endif()

if(MSYS OR CYGWIN)
    list(APPEND st_defines _GNU_SOURCE)
    list(APPEND uv_sources
        ../libuv/src/unix/posix-hrtime.c
        ../libuv/src/unix/posix-poll.c
        ../libuv/src/unix/no-fsevents.c
    )
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "AIX")
  list(APPEND st_defines
       _ALL_SOURCE
       _LINUX_SOURCE_COMPAT
       _THREAD_SAFE
       _XOPEN_SOURCE=500)
  list(APPEND st_libraries perfstat)
  list(APPEND uv_sources ../libuv/src/unix/aix.c)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Android")
  list(APPEND uv_libs dl)
  list(APPEND uv_sources
       ../libuv/src/unix/android-ifaddrs.c
       ../libuv/src/unix/linux-core.c
       ../libuv/src/unix/linux-inotify.c
       ../libuv/src/unix/linux-syscalls.c
       ../libuv/src/unix/procfs-exepath.c
       ../libuv/src/unix/pthread-fixes.c
       ../libuv/src/unix/sysinfo-loadavg.c
       #../libuv/src/unix/sysinfo-memory.c
       )
endif()

if(APPLE OR CMAKE_SYSTEM_NAME MATCHES "Android|Linux|OS/390")
  list(APPEND uv_sources ../libuv/src/unix/proctitle.c)
endif()

if(CMAKE_SYSTEM_NAME MATCHES "DragonFly|FreeBSD")
  list(APPEND uv_sources ../libuv/src/unix/freebsd.c)
endif()

if(CMAKE_SYSTEM_NAME MATCHES "DragonFly|FreeBSD|NetBSD|OpenBSD")
  list(APPEND uv_sources ../libuv/src/unix/posix-hrtime.c ../libuv/src/unix/bsd-proctitle.c)
  list(APPEND st_libraries kvm)
endif()

if(APPLE OR CMAKE_SYSTEM_NAME MATCHES "DragonFly|FreeBSD|NetBSD|OpenBSD")
  list(APPEND uv_sources ../libuv/src/unix/bsd-ifaddrs.c ../libuv/src/unix/kqueue.c)
endif()

if(APPLE)
  list(APPEND st_defines _DARWIN_UNLIMITED_SELECT=1 _DARWIN_USE_64_BIT_INODE=1)
  list(APPEND uv_sources
       ../libuv/src/unix/darwin-proctitle.c
       ../libuv/src/unix/darwin.c
       ../libuv/src/unix/fsevents.c)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  list(APPEND st_defines _GNU_SOURCE _POSIX_C_SOURCE=200112)
  list(APPEND st_libraries dl rt)
  list(APPEND uv_sources
       ../libuv/src/unix/linux-core.c
       ../libuv/src/unix/linux-inotify.c
       ../libuv/src/unix/linux-syscalls.c
       ../libuv/src/unix/procfs-exepath.c
       ../libuv/src/unix/sysinfo-loadavg.c)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "NetBSD")
  list(APPEND uv_sources ../libuv/src/unix/netbsd.c)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "OpenBSD")
  list(APPEND uv_sources ../libuv/src/unix/openbsd.c)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "OS/390")
  list(APPEND st_defines PATH_MAX=255)
  list(APPEND st_defines _AE_BIMODAL)
  list(APPEND st_defines _ALL_SOURCE)
  list(APPEND st_defines _LARGE_TIME_API)
  list(APPEND st_defines _OPEN_MSGQ_EXT)
  list(APPEND st_defines _OPEN_SYS_FILE_EXT)
  list(APPEND st_defines _OPEN_SYS_IF_EXT)
  list(APPEND st_defines _OPEN_SYS_SOCK_IPV6)
  list(APPEND st_defines _UNIX03_SOURCE)
  list(APPEND st_defines _UNIX03_THREADS)
  list(APPEND st_defines _UNIX03_WITHDRAWN)
  list(APPEND st_defines _XOPEN_SOURCE_EXTENDED)
  list(APPEND uv_sources
       ../libuv/src/unix/pthread-fixes.c
       ../libuv/src/unix/pthread-barrier.c
       ../libuv/src/unix/os390.c
       ../libuv/src/unix/os390-syscalls.c)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "SunOS")
  list(APPEND st_defines __EXTENSIONS__ _XOPEN_SOURCE=500)
  list(APPEND st_libraries kstat nsl sendfile socket)
  list(APPEND uv_sources ../libuv/src/unix/no-proctitle.c ../libuv/src/unix/sunos.c)
endif()

add_executable(ex0_task ../examples/ex0_task.c ${st_sources})
target_compile_definitions(ex0_task PRIVATE ${st_defines})
target_compile_options(ex0_task PRIVATE ${st_cflags})
st_add_link_flags(ex0_task)
target_include_directories(ex0_task PRIVATE ../include ../libuv/include ../libuv/src ../asm)
target_link_libraries(ex0_task ${st_link_flags} ${st_libraries})

add_executable(ex1_event ../examples/ex1_event.c ${st_sources})
target_compile_definitions(ex1_event PRIVATE ${st_defines})
target_compile_options(ex1_event PRIVATE ${st_cflags})
st_add_link_flags(ex1_event)
target_include_directories(ex1_event PRIVATE ../include ../libuv/include ../libuv/src ../asm)
target_link_libraries(ex1_event ${st_link_flags} ${st_libraries})

add_library(s_task STATIC ${st_sources} ${uv_sources})
target_compile_definitions(s_task PRIVATE ${st_defines} USE_LIBUV)
target_compile_options(s_task PRIVATE ${st_cflags})
target_include_directories(s_task PRIVATE ../include ../libuv/include ../libuv/src ../asm)

add_executable(ex2_libuv ../examples/ex2_libuv.c)
target_compile_definitions(ex2_libuv PRIVATE ${st_defines} USE_LIBUV)
target_compile_options(ex2_libuv PRIVATE ${st_cflags})
st_add_link_flags(ex2_libuv)
target_include_directories(ex2_libuv PRIVATE ../include ../libuv/include ../libuv/src ../asm)
target_link_libraries(ex2_libuv s_task ${st_link_flags} ${st_libraries})

add_executable(ex3_http_client ../examples/ex3_http_client.c)
target_compile_definitions(ex3_http_client PRIVATE ${st_defines} USE_LIBUV)
target_compile_options(ex3_http_client PRIVATE ${st_cflags})
st_add_link_flags(ex3_http_client)
target_include_directories(ex3_http_client PRIVATE ../include ../libuv/include ../libuv/src ../asm)
target_link_libraries(ex3_http_client s_task ${st_link_flags} ${st_libraries})

add_executable(ex4_echo_server ../examples/ex4_echo_server.c)
target_compile_definitions(ex4_echo_server PRIVATE ${st_defines} USE_LIBUV)
target_compile_options(ex4_echo_server PRIVATE ${st_cflags})
st_add_link_flags(ex4_echo_server)
target_include_directories(ex4_echo_server PRIVATE ../include ../libuv/include ../libuv/src ../asm)
target_link_libraries(ex4_echo_server s_task ${st_link_flags} ${st_libraries})

add_executable(ex5_led_blinking ../examples/ex5_led_blinking.c ${st_sources})
target_compile_definitions(ex5_led_blinking PRIVATE ${st_defines})
target_compile_options(ex5_led_blinking PRIVATE ${st_cflags})
st_add_link_flags(ex5_led_blinking)
target_include_directories(ex5_led_blinking PRIVATE ../include ../libuv/include ../libuv/src ../asm)
target_link_libraries(ex5_led_blinking ${st_link_flags} ${st_libraries})

add_executable(ex6_chan ../examples/ex6_chan.c ${st_sources})
target_compile_definitions(ex6_chan PRIVATE ${st_defines})
target_compile_options(ex6_chan PRIVATE ${st_cflags})
st_add_link_flags(ex6_chan)
target_include_directories(ex6_chan PRIVATE ../include ../libuv/include ../libuv/src ../asm)
target_link_libraries(ex6_chan ${st_link_flags} ${st_libraries})


