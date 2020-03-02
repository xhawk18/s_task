#!/bin/bash


ST_SOURCE=(
    ../../src/s_task.c
    ../../src/s_rbtree.c
    ../../src/s_list.c
)


ASM_SOURCE=(
    -x assembler ../../asm/make_x86_64_ms_pe_gas.asm
    -x assembler ../../asm/jump_x86_64_ms_pe_gas.asm \
)


UV_SOURCE=(
    ../../libuv/src/fs-poll.c
    ../../libuv/src/idna.c
    ../../libuv/src/inet.c
    ../../libuv/src/strscpy.c
    ../../libuv/src/threadpool.c
    ../../libuv/src/timer.c
    ../../libuv/src/uv-common.c
    ../../libuv/src/uv-data-getter-setters.c
    ../../libuv/src/version.c
    ../../libuv/src/win/async.c
    ../../libuv/src/win/core.c
    ../../libuv/src/win/detect-wakeup.c
    ../../libuv/src/win/dl.c
    ../../libuv/src/win/error.c
    ../../libuv/src/win/fs.c
    ../../libuv/src/win/fs-event.c
    ../../libuv/src/win/getaddrinfo.c
    ../../libuv/src/win/getnameinfo.c
    ../../libuv/src/win/handle.c
    ../../libuv/src/win/loop-watcher.c
    ../../libuv/src/win/pipe.c
    ../../libuv/src/win/thread.c
    ../../libuv/src/win/poll.c
    ../../libuv/src/win/process.c
    ../../libuv/src/win/process-stdio.c
    ../../libuv/src/win/signal.c
    ../../libuv/src/win/snprintf.c
    ../../libuv/src/win/stream.c
    ../../libuv/src/win/tcp.c
    ../../libuv/src/win/tty.c
    ../../libuv/src/win/udp.c
    ../../libuv/src/win/util.c
    ../../libuv/src/win/winapi.c
    ../../libuv/src/win/winsock.c
)


gcc -m64 -O2 -s -I../../include -Wall "${ST_SOURCE[@]}" "${ASM_SOURCE[@]}" ../../examples/ex0_task.c -o ex0_task 

gcc -m64 -O2 -s -I../../include -Wall "${ST_SOURCE[@]}" "${ASM_SOURCE[@]}" ../../examples/ex1_event.c -o ex1_event

#gcc -m64 -O2 -s -I../../include -I../../libuv/include -I../../libuv/src -Wall\
#     -DUSE_LIBUV -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -D_GNU_SOURCE -D_POSIX_C_SOURCE=200112 \
#    "${ST_SOURCE[@]}" "${UV_SOURCE[@]}" "${ASM_SOURCE[@]}" \
#    -lpthread -ldl -lrt \
#    ../../examples/ex2_libuv.c -o ex2_libuv
