/* Copyright xhawk, MIT license */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "s_task.h"

#define MAX_CLINETS 100

typedef struct {
    uv_loop_t* loop;
    uv_tcp_t tcp_server;
    bool closed;
    size_t client_counts;
    s_event_t event;
    s_list_t running_clients;
    s_list_t stopped_clients;
    void* stack[32 * 1024];

    void* stack_timer[32 * 1024];
} server_t;

typedef struct {
    s_list_t list_node;
    server_t* server;
    uv_tcp_t stream;
    void* stack[32 * 1024];
} client_t;

static server_t g_server;



static void run_client(__async__, void* arg) {
    client_t* client = (client_t*)arg;
    printf("in run_client\n");

    int ret;

    /* accept the socket */
    ret = uv_tcp_init(client->server->loop, (uv_tcp_t*)&client->stream);
    if (ret != 0) {
        fprintf(stderr, "uv_tcp_init: %s\n", uv_strerror(ret));
        goto out0;
    }

    ret = uv_accept((uv_stream_t*)&client->server->tcp_server, (uv_stream_t*)&client->stream);
    if (ret != 0) {
        fprintf(stderr, "uv_accept: %s\n", uv_strerror(ret));
        goto out1;
    }

    if (client->server->client_counts >= MAX_CLINETS) {
        fprintf(stderr, "max connection exceeds!");
        goto out1;
    }

    /* increase client accounts */
    ++client->server->client_counts;

    /* run echo service */
    while (true) {
        char buf[256];
        ssize_t nread;
        ret = s_uv_read(__await__, (uv_stream_t*)&client->stream, buf, sizeof(buf), &nread);
        if (ret != 0)
            break;

        ret = s_uv_write(__await__, (uv_stream_t*)&client->stream, buf, nread);
        if (ret != 0)
            break;
    }

    /* decrease client accounts */
    --client->server->client_counts;

out1:;
    s_uv_close(__await__, (uv_handle_t*)&client->stream);
out0:;

    /* set to be joined a release the memory */
    s_list_detach(&client->list_node);
    s_list_attach(&client->server->stopped_clients, &client->list_node);
    s_event_set(&client->server->event);
}


static void on_connection(uv_stream_t* stream, int status) {
    server_t* server = GET_PARENT_ADDR(stream, server_t, tcp_server);
    client_t* client;

    if (status != 0) {
        fprintf(stderr, "Connect error %s\n", uv_err_name(status));
        return;
    }

    client = (client_t*)malloc(sizeof(client_t));
    printf("alloc task = %p\n", client);
    if (client == NULL) {
        fprintf(stderr, "no enough memory %d", __LINE__);
        return;
    }

    client->server = server;
    /* s_event_init(&client->event); */
    s_list_init(&client->list_node);
    s_list_attach(&server->running_clients, &client->list_node);
    s_task_create(client->stack, sizeof(client->stack), run_client, (void*)client);
}

void stop_server(server_t* server) {
    server->closed = true;
    s_task_cancel_wait(server->stack);
    s_event_set(&server->event);
}

static void stop_client(client_t* client) {
    s_task_cancel_wait(client->stack);
}

static void join_clients(__async__, server_t* server) {
    while (true) {
        s_list_t* node = s_list_get_next(&server->stopped_clients);
        if (node == &server->stopped_clients)
            break;

        s_list_detach(node);
        client_t* client = GET_PARENT_ADDR(node, client_t, list_node);
        s_task_join(__await__, client->stack);
        printf("free task = %p\n", client);
        free(client);
    }
}

void run_server(__async__, void *arg) {
    server_t* server = (server_t*)arg;
    struct sockaddr_in addr;
    s_list_t* node;
    int ret;

    const char *HOST = "0.0.0.0";
    const unsigned short PORT = 3333;

    /* Initialize server data */
    server->closed = false;
    server->client_counts = 0;
    s_event_init(&server->event);
    s_list_init(&server->running_clients);
    s_list_init(&server->stopped_clients);
    
    ret = uv_ip4_addr(HOST, PORT, &addr);
    if (ret) {
        /* TODO: Error codes */
        fprintf(stderr, "uv_ip4_addr error: %s\n", uv_strerror(ret));
        goto out0;
    }

    ret = uv_tcp_init(server->loop, &server->tcp_server);
    if (ret) {
        fprintf(stderr, "Socket creation error: %s\n", uv_strerror(ret));
        goto out1;
    }

    ret = uv_tcp_bind(&server->tcp_server, (const struct sockaddr*) & addr, 0);
    if (ret) {
        fprintf(stderr, "Bind error: %s\n", uv_strerror(ret));
        goto out1;
    }

    ret = uv_listen((uv_stream_t*)&server->tcp_server, SOMAXCONN, on_connection);
    if (ret) {
        fprintf(stderr, "Listen error: %s\n", uv_strerror(ret));
        goto out1;
    }

    printf("Listening on port %s:%d\n", HOST, (int)PORT);
    while (!server->closed) {
        s_event_wait(__await__, &server->event);
        /* join the client tasks if there're any */
        join_clients(__await__, server);
    }

    /* stop and wait all clients exit */
    for (node = s_list_get_next(&server->running_clients);
        node != &server->running_clients;
        node = s_list_get_next(node)) {
        client_t* client = GET_PARENT_ADDR(node, client_t, list_node);
        stop_client(client);
    }
    while (!s_list_is_empty(&server->running_clients)) {
        s_event_wait(__await__, &server->event);
    }

    /* join the client tasks */
    join_clients(__await__, server);
    printf("Listen closed\n");

out1:;
    s_uv_close(__await__, (uv_handle_t*)&server->tcp_server);
out0:;
}




void run_timer(__async__, void* arg) {
    server_t* server = (server_t*)arg;
    uint32_t second = 60;
    s_task_sleep(__await__, second);
    printf("Gracefully close server after %d seconds.", (int)second);
    stop_server(server);
}


int main(void) {
    uv_loop_t *loop = uv_default_loop();
    s_task_init_uv_system(loop);

    g_server.loop = loop;

    /* Start the server task */
    s_task_create(g_server.stack, sizeof(g_server.stack), run_server, (void *)&g_server);

    /* Start a new task that will close the server gracefully after 60 seconds. */
    s_task_create(g_server.stack_timer, sizeof(g_server.stack_timer), run_timer, (void*)&g_server);

    uv_run(loop, UV_RUN_DEFAULT);
    printf("all task is over\n");
    return 0;
}
