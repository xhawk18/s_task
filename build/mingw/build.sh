#!/bin/bash

GCC=gcc

ST_SOURCE=(
    ../../src/s_task.c
    ../../src/s_mutex.c
    ../../src/s_event.c
    ../../src/s_chan.c
    ../../src/s_timer_fast.c
    ../../src/s_timer_small.c
    ../../src/s_rbtree.c
    ../../src/s_list.c
    ../../asm/make_gas.S
    ../../asm/jump_gas.S
)


UV_SOURCE=(
    ../../src/s_uv.c
    ../../libuv/src/fs-poll.c
    ../../libuv/src/idna.c
    ../../libuv/src/inet.c
    ../../libuv/src/strscpy.c
    ../../libuv/src/threadpool.c
    ../../libuv/src/timer.c
    ../../libuv/src/uv-common.c
    ../../libuv/src/uv-data-getter-setters.c
    ../../libuv/src/version.c
)

UV_SOURCE_MINGW=(
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

UV_SOURCE_CYGWIN=(
    ../../libuv/src/unix/async.c
    ../../libuv/src/unix/core.c
    ../../libuv/src/unix/dl.c
    ../../libuv/src/unix/fs.c
    ../../libuv/src/unix/getaddrinfo.c
    ../../libuv/src/unix/getnameinfo.c
    ../../libuv/src/unix/loop-watcher.c
    ../../libuv/src/unix/loop.c
    ../../libuv/src/unix/pipe.c
    ../../libuv/src/unix/poll.c
    ../../libuv/src/unix/process.c
    ../../libuv/src/unix/signal.c
    ../../libuv/src/unix/stream.c
    ../../libuv/src/unix/tcp.c
    ../../libuv/src/unix/thread.c
    ../../libuv/src/unix/tty.c
    ../../libuv/src/unix/udp.c
    ../../libuv/src/unix/posix-hrtime.c
    ../../libuv/src/unix/posix-poll.c
    ../../libuv/src/unix/no-fsevents.c
)

if gcc -dM -E - < /dev/null | grep __CYGWIN__; then
    # for cygwin / msys
    UV_SOURCE=("${UV_SOURCE[@]}" "${UV_SOURCE_CYGWIN[@]}")
    ST_LIBS=( -lpthread -ldl -lrt )
elif gcc -dM -E - < /dev/null | grep "__MINGW.*__"; then
    # for mingw
    UV_SOURCE=("${UV_SOURCE[@]}" "${UV_SOURCE_MINGW[@]}")
    ST_LIBS=( -ladvapi32 -liphlpapi -lpsapi -lshell32 -luser32 -luserenv -lws2_32 )
else
    echo "neither cygwin nor mingw!" 1>&2
    exit 1
fi

"$GCC" -O2 -s -static -I../../include -Wall "${ST_SOURCE[@]}" ../../examples/ex0_task.c -o ex0_task

"$GCC" -O2 -s -static -I../../include -Wall "${ST_SOURCE[@]}" ../../examples/ex1_event.c -o ex1_event

"$GCC" -O2 -s -static -I../../include -I../../libuv/include -I../../libuv/src -Wall\
     -DUSE_LIBUV -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -D_GNU_SOURCE \
    "${ST_SOURCE[@]}" "${UV_SOURCE[@]}" \
    "${ST_LIBS[@]}" \
    ../../examples/ex2_libuv.c -o ex2_libuv

"$GCC" -O2 -s -static -I../../include -I../../libuv/include -I../../libuv/src -Wall\
     -DUSE_LIBUV -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -D_GNU_SOURCE \
    "${ST_SOURCE[@]}" "${UV_SOURCE[@]}" \
    "${ST_LIBS[@]}" \
    ../../examples/ex3_http_client.c -o ex3_http_client

"$GCC" -O2 -s -static -I../../include -I../../libuv/include -I../../libuv/src -Wall\
     -DUSE_LIBUV -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -D_GNU_SOURCE \
    "${ST_SOURCE[@]}" "${UV_SOURCE[@]}" \
    "${ST_LIBS[@]}" \
    ../../examples/ex4_echo_server.c -o ex4_echo_server

"$GCC" -O2 -s -static -I../../include -Wall "${ST_SOURCE[@]}" ../../examples/ex6_chan.c -o ex6_chan 