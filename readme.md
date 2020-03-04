# s_task - a co-routine library for C

## Usage
"s_task" is a co-routine library written in C and asm (from boost library), **without** **C++** required.

"s_task" has been ported to various platforms, such as win32, linux, stm32.

"s_task" uses keywords \_\_await\_\_ and \_\_async\_\_. For functions that may switch to other tasks, call it with 1st parameter \_\_await\_\_, for the caller function of which, define the 1st parameter as \_\_async\_\_, which make it is clear to know about context switching.

[Example 1](examples/ex0_task.c) - simple task creation

```c
#include <stdio.h>
#include "s_task.h"

void* stack_main[64 * 1024];
void* stack0[64 * 1024];
void* stack1[64 * 1024];

void sub_task(__async__, void* arg) {
    int i;
    int n = (int)(size_t)arg;
    for (i = 0; i < 5; ++i) {
        printf("task %d, delay seconds = %d, i = %d\n", n, n, i);
        s_task_msleep(__await__, n * 1000);
        //s_task_yield(__await__);
    }
}

void main_task(__async__, void* arg) {
    int i;
    s_task_create(stack0, sizeof(stack0), sub_task, (void*)1);
    s_task_create(stack1, sizeof(stack1), sub_task, (void*)2);

    for (i = 0; i < 4; ++i) {
        printf("task_main arg = %p, i = %d\n", arg, i);
        s_task_yield(__await__);
    }

    s_task_join(__await__, stack0);
    s_task_join(__await__, stack1);
}

int main(int argc, char* argv) {
    __init_async__;

    s_task_init_system();
    s_task_create(stack_main, sizeof(stack_main), main_task, (void*)(size_t)argc);
    s_task_join(__await__, stack_main);
    printf("all task is over\n");
    return 0;
}
```

[Example 1](examples/ex3_http_client.c) - asynchronized http client without callback function.
```c
void main_task(__async__, void *arg) {
    uv_loop_t* loop = (uv_loop_t*)arg;

    const char *HOST = "baidu.com";
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
```

## API

### Task

```c
/* Function type for task entrance */
typedef void(*s_task_fn_t)(__async__, void *arg);

/* Create a new task */
void s_task_create(void *stack, size_t stack_size, s_task_fn_t entry, void *arg);

/* Wait a task to exit */
void s_task_join(__async__, void *stack);

/* Kill a task */
void s_task_kill(void *stack);

/* Sleep in milliseconds */
void s_task_msleep(__async__, uint32_t msec);

/* Sleep in seconds */
void s_task_sleep(__async__, uint32_t sec);

/* Yield current task */
void s_task_yield(__async__);
```

### Mutex
```c
/* Initialize a mutex */
void s_mutex_init(s_mutex_t *mutex);

/* Lock the mutex */
void s_mutex_lock(__async__, s_mutex_t *mutex);

/* Unlock the mutex */
void s_mutex_unlock(s_mutex_t *mutex);
```

### Event
```c
/* Initialize a wait event */
void s_event_init(s_event_t *event);

/* Wait event */
void s_event_wait(__async__, s_event_t *event);

/* Set event */
void s_event_set(s_event_t *event);

/* Wait event with timeout */
void s_event_wait_msec(__async__, s_event_t *event, uint32_t msec);

/* Wait event with timeout */
void s_event_wait_sec(__async__, s_event_t *event, uint32_t msec);
```

## How to make port?
To make a port of "s_task" to new system is very simple.
Here's an example for linux porting, s_port_posix.h --
```c
//1. define a type for clock
typedef uint32_t my_clock_t;
typedef int32_t my_clock_diff_t;

//2. define the clock ticks count for one second
#define MY_CLOCKS_PER_SEC CLOCKS_PER_SEC

//3. Implement the initilization function for clock. Leave it blank if not required.
void my_clock_init(){
}

//4. Implement the function of getting current clock ticks.
my_clock_t my_clock() {
    return clock();
}

//5. Implement the idle delay function.
void my_on_idle(uint64_t max_idle_ms) {
    usleep(max_idle_ms * 1000);
}

//6. The platform must implement one of the groups of context functions, 
    make_fcontext / jump_fcontext   (fast, suggest to use)
    or 
    makecontext / swapcontext       (slow!!)
```

## Compatibility

"s_task" can run as a standalone co-routine library, or work with libuv library.

   | Platform                    | co-routine | libuv |
   |-----------------------------|------------|-------|
   | Windows                     | yes        | yes   |
   | Linux                       | yes        | yes   |
   | MacOS                       | yes        | yes   |
   | MingW                       | yes        | no    |
   | ARMv6-M(M051)               | yes        | no    |
   | ARMv7-M(stm32f103,stm32f302)| yes        | no    |
   | stm8s103                    | no (todo)  | no    |

## Issues on embedded system
The library allocates tasks' stack from the main stack,
so please take care of the size of main stack and make sure it's big enough.

For example, for stm32f103, we adjust the main stack size to 8K,
in file build_stm32f103\Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x\startup\arm\startup_stm32f10x_md.s
```asm
    Stack_Size      EQU     0x00002000
```

and define the stack size of each task to 1K in s_port_stm32f10x.h
```c
#define STACK_SIZE 1024
```
