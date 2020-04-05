/* Copyright xhawk, MIT license */

#include <stdio.h>
#include <string.h>
#include "s_task.h"

static void * g_stack_main[512*1024];


void main_task(__async__, void *arg) {
    uv_loop_t* loop = (uv_loop_t*)arg;

    const char *HOST = "www.baidu.com";
    const unsigned short PORT = 80;

    //<1> resolve host
    struct addrinfo* addr = s_uv_getaddrinfo(__await__,
        loop,
        HOST,
        NULL,
        NULL);
    if (addr == NULL) {
        fprintf(stderr, "can not resolve host %s\n", HOST);
        goto out0;
    }

    if (addr->ai_addr->sa_family == AF_INET) {
        struct sockaddr_in* sin = (struct sockaddr_in*)(addr->ai_addr);
        sin->sin_port = htons(PORT);
    }
    else if (addr->ai_addr->sa_family == AF_INET6) {
        struct sockaddr_in6* sin = (struct sockaddr_in6*)(addr->ai_addr);
        sin->sin6_port = htons(PORT);
    }

    //<2> connect
    uv_tcp_t tcp_client;
    int ret = uv_tcp_init(loop, &tcp_client);
    if (ret != 0)
        goto out1;
    ret = s_uv_tcp_connect(__await__, &tcp_client, addr->ai_addr);
    //printf("connect ret = %d\n", ret);
    if (ret != 0)
        goto out2;

    //<3> send request
    const char *request = "GET / HTTP/1.0\r\n\r\n";
    uv_stream_t* tcp_stream = (uv_stream_t*)&tcp_client;
    s_uv_write(__await__, tcp_stream, request, strlen(request));

    //<4> read response
    ssize_t nread;
    char buf[1024];
    while (true) {
        ret = s_uv_read(__await__, tcp_stream, buf, sizeof(buf), &nread);
        //printf("ret = %d, nread = %d\n", (int)ret, (int)nread);
        if (ret != 0) break;

        // output response to console
        fwrite(buf, 1, nread, stdout);
    }

    //<5> close connections
out2:;
    s_uv_close(__await__, (uv_handle_t*)&tcp_client);
out1:;
    uv_freeaddrinfo(addr);
out0:;
}


int main(int argc, char *argv[]) {
    uv_loop_t *loop = uv_default_loop();
    s_task_init_system(loop);

    s_task_create(g_stack_main, sizeof(g_stack_main), main_task, (void *)loop);
    
    uv_run(loop, UV_RUN_DEFAULT);
    printf("all task is over\n");
    return 0;
}
