# s_task - awaitable coroutine library for C

[中文版文档 (chinese version)](readme_cn.md)

## Table of content

  - [Features](#features)
  - [Examples](#examples)
    - [Example 1 - simple task creation](#example-1---simple-task-creation)
    - [Example 2 - asynchronized http client without callback function.](#example-2---asynchronized-http-client-without-callback-function)
    - [Example 3 - control LED with multitasking on ardinuo](#example-3---control-led-with-multitasking-on-ardinuo)
  - [Compatibility](#compatibility)
  - [Build](#build)
    - [Linux / FreeBSD / MacOS / MingW(MSYS2)](#linux--freebsd--macos--mingwmsys2)
    - [Windows and other platforms](#windows-and-other-platforms)
  - [How to use in your project?](#how-to-use-in-your-project)
  - [API](#api)
    - [Task](#task)
    - [Chan](#chan)
    - [Mutex](#mutex)
    - [Event](#event)
  - [Specials on embedded platform](#specials-on-embedded-platform)
  - [How to make port?](#how-to-make-port)
  - [Contact](#contact)
  - [VS.](#vs)
  - [Thanks](#thanks)

## Features

 + "s_task" is a coroutine library written in pure C and asm (from boost library), **without** **C++** required.
 + supports various platforms, such as windows, linux, android, macos, stm32, stm8, arduino, etc.
 + supports keywords **\_\_await\_\_** and **\_\_async\_\_** . :triangular_flag_on_post: For functions that may switch to other tasks, call it with 1st parameter \_\_await\_\_, for the caller function of which, define the 1st parameter as \_\_async\_\_, which make it is clear to know about context switching.
 + works with libuv for network programming.
 + "chan", "mutex" and "event" for communication between tasks.
 + on embedded platfrom (stm32/stm8/m051/arduino,etc), "s_task" is a special RTOS --
   - no dynamical memory allocation
   - very small memory footprint ( increased by ROM<1.5K, RAM<128 bytes + task stack size)

## Examples

### [Example 1](examples/ex0_task.c) - simple task creation

```c
#include <stdio.h>
#include "s_task.h"

void* g_stack_main[64 * 1024];
void* g_stack0[64 * 1024];
void* g_stack1[64 * 1024];

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
    s_task_create(g_stack0, sizeof(g_stack0), sub_task, (void*)1);
    s_task_create(g_stack1, sizeof(g_stack1), sub_task, (void*)2);

    for (i = 0; i < 4; ++i) {
        printf("task_main arg = %p, i = %d\n", arg, i);
        s_task_yield(__await__);
    }

    s_task_join(__await__, g_stack0);
    s_task_join(__await__, g_stack1);
}

int main(int argc, char* argv) {

    s_task_init_system();
    s_task_create(g_stack_main, sizeof(g_stack_main), main_task, (void*)(size_t)argc);
    s_task_join(__await__, g_stack_main);
    printf("all task is over\n");
    return 0;
}
```

### [Example 2](examples/ex3_http_client.c) - asynchronized http client without callback function.

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

### [Example 3](build/arduino/arduino.ino) - control LED with multitasking on ardinuo

```c
#include <stdio.h>
#include "src/s_task/s_task.h"

//This program demonstrates three tasks:
// 1) main_task - 
//    Wait 10 seconds and set flag g_exit. 
//    After all tasks finished, set LED on always.
// 2) sub_task_fast_blinking -
//    Set led blinking fast
// 3) sub_task_set_low -
//    Set led off for 1 second, and then blinking for 3 seconds.


void setup() {
    // put your setup code here, to run once:
    pinMode(LED_BUILTIN, OUTPUT);
}


char g_stack0[384];
char g_stack1[384];
volatile bool g_is_low = false;
volatile bool g_exit = false;

void sub_task_fast_blinking(__async__, void* arg) {
    while(!g_exit) {
        if(!g_is_low)
            digitalWrite(LED_BUILTIN, HIGH); // turn the LED on

        s_task_msleep(__await__, 50);        // wait for 50 milliseconds
        digitalWrite(LED_BUILTIN, LOW);      // turn the LED off
        s_task_msleep(__await__, 50);        // wait for 50 milliseconds
    }    
}

void sub_task_set_low(__async__, void* arg) {
    while(!g_exit) {
        g_is_low = true;                     // stop fast blinking
        digitalWrite(LED_BUILTIN, LOW);      // turn the LED off
        s_task_sleep(__await__, 1);          // wait for 1 second
        g_is_low = false;                    // start fast blinking
        s_task_sleep(__await__, 3);          // wait for 3 seconds
    }    
}

void main_task(__async__, void* arg) {
    // create two sub tasks
    s_task_create(g_stack0, sizeof(g_stack0), sub_task_fast_blinking, NULL);
    s_task_create(g_stack1, sizeof(g_stack1), sub_task_set_low, NULL);

    // wait for 10 seconds
    s_task_sleep(__await__, 10);
    g_exit = true;

    // wait two sub tasks return
    s_task_join(__await__, g_stack0);
    s_task_join(__await__, g_stack1);
}

void loop() {
    
    s_task_init_system();
    main_task(__await__, NULL);

    // turn the LED on always
    digitalWrite(LED_BUILTIN, HIGH);
    while(1);
}
```

## Compatibility

"s_task" can run as standalone coroutine library, or work with library libuv (compiling with macro **USE_LIBUV**).

| Platform                          | coroutine          | libuv              |
|-----------------------------------|--------------------|--------------------|
| Windows                           | :heavy_check_mark: | :heavy_check_mark: |
| Linux                             | :heavy_check_mark: | :heavy_check_mark: |
| MacOS                             | :heavy_check_mark: | :heavy_check_mark: |
| FreeBSD (12.1, x64)               | :heavy_check_mark: | :heavy_check_mark: |
| Android                           | :heavy_check_mark: | :heavy_check_mark: |
| MingW (https://www.msys2.org/)    | :heavy_check_mark: | :heavy_check_mark: |
| ARMv6-M (M051, Raspberry Pi Pico) | :heavy_check_mark: | :x:                |
| ARMv7-M (STM32F103, STM32F302)    | :heavy_check_mark: | :x:                |
| STM8 (STM8S103, STM8L051F3)       | :heavy_check_mark: | :x:                |
| Arduino UNO (AVR MEGA328P)        | :heavy_check_mark: | :x:                |
| Arduino DUE (ATSAM3X8E)           | :heavy_check_mark: | :x:                |

   linux tested on 
   * i686 (ubuntu-16.04)
   * x86_64 (centos-8.1)
   * arm (raspiberry 32bit)
   * aarch64 (① raspiberry 64bit, ② ubuntu 14.04 / centos7.6 on huawei Kunpeng920)
   * mipsel (openwrt ucLinux 3.10.14 for MT7628)
   * mips64 (fedora for loongson 3A-4000)
   * ppc64 / ppc64le (centos-7.8.2003 altarch)
   * riscv64 ([jslinux](https://bellard.org/jslinux/vm.html?cpu=riscv64&url=buildroot-riscv64.cfg&mem=256))

## Build

### Linux / FreeBSD / MacOS / MingW(MSYS2)

    git clone https://github.com/xhawk18/s_task.git
    cd s_task/build/
    cmake .
    make

If need cross compiler, please set argument CMAKE_C_COMPILER when calling "cmake ." above, for example --

    cmake . -DCMAKE_C_COMPILER=aarch64-linux-gnu-gcc

### Windows and other platforms

| Platform                   | Project                               | Tool chain                                          |
|----------------------------|---------------------------------------|-----------------------------------------------------|
| Windows                    | build\windows\s_task.sln              | visual studio 2019                                  |
| Android                    | build\android\cross_build_arm*.sh     | android ndk 20, API level 21 (test in termux)       |
| STM8S103                   | build\stm8s103\Project.eww            | IAR workbench for STM8                              |
| STM8L051F3                 | build\stm8l05x\Project.eww            | IAR workbench for STM8                              |
| STM32F103                  | build\stm32f103\arcc\Project.uvproj   | Keil uVision5                                       |
| STM32F103                  | build\stm32f103\gcc\Project.uvproj    | arm-none-eabi-gcc                                   |
| STM32F302                  | build\stm32f302\Project.uvporj        | Keil uVision5                                       |
| M051                       | build\m051\Project.uvporj             | Keil uVision5                                       |
| Raspberry Pi Pico          | build\raspberrypi_pico\CMakeLists.txt | [pico-sdk](https://github.com/raspberrypi/pico-sdk) |
| ATmega328P                 | build\atmega328p\atmega328p.atsln     | Atmel Studio 7.0                                    |
| Arduino UNO<br>Arduino DUE | build\arduino\arduino.ino             | Arduino IDE                                         |

## How to use in your project?

On linux/unix like system, after cmake build, you may get the libraries for your project

* add libs_task.a to your project
* #include "[s_task.h](include/s_task.h)"
* build with predefined macro USE_LIBUV

On arduino, copy all source files (*.h, *.c) in folder "[include](include)" and "[src](src)" into your arduino project's subfolder src/s_task/. Please take a look at the folder structure of "[build/arduino/](build/arduino/)".

On windows or other system, please find the project in folder "build" as the project template.

## API

### Task

```c
/*
 * Return values -- 
 * For all functions marked by __async__ and hava an int return value, will
 *     return 0 on waiting successfully,
 *     return -1 on waiting cancalled by s_task_cancel_wait() called by other task.
 */

/* Function type for task entrance */
typedef void(*s_task_fn_t)(__async__, void *arg);

/* Create a new task */
void s_task_create(void *stack, size_t stack_size, s_task_fn_t entry, void *arg);

/* Wait a task to exit */
int s_task_join(__async__, void *stack);

/* Sleep in milliseconds */
int s_task_msleep(__async__, uint32_t msec);

/* Sleep in seconds */
int s_task_sleep(__async__, uint32_t sec);

/* Yield current task */
void s_task_yield(__async__);

/* Cancel task waiting and make it running */
void s_task_cancel_wait(void* stack);
```

### Chan

```c
/* 
 * macro: Declare the chan variable
 *    name: name of the chan
 *    TYPE: type of element in the chan
 *    count: max count of element buffer in the chan
 */
s_chan_declare(name,TYPE,count);

/* 
 * macro: Initialize the chan (parameters same as what's in s_declare_chan).
 * To make a chan, we need to use "s_chan_declare" and then call "s_chan_init".
 */
s_chan_init(name,TYPE,count);

/* 
 * Put element into chan
 *  return 0 on chan put successfully
 *  return -1 on chan cancelled
 */
int s_chan_put(__async__, s_chan_t *chan, const void *in_object);

/* 
 * Put number of elements into chan
 *  return 0 on chan put successfully
 *  return -1 on chan cancelled
 */
int s_chan_put_n(__async__, s_chan_t *chan, const void *in_object, uint16_t number);

/* 
 * Get element from chan
 *  return 0 on chan get successfully
 *  return -1 on chan cancelled
 */
int s_chan_get(__async__, s_chan_t *chan, void *out_object);

/* 
 * Get number of elements from chan
 *  return 0 on chan get successfully
 *  return -1 on chan cancelled
 */
int s_chan_get_n(__async__, s_chan_t *chan, void *out_object, uint16_t number);
```

### Mutex

```c
/* Initialize a mutex */
void s_mutex_init(s_mutex_t *mutex);

/* Lock the mutex */
int s_mutex_lock(__async__, s_mutex_t *mutex);

/* Unlock the mutex */
void s_mutex_unlock(s_mutex_t *mutex);
```

### Event

```c
/* Initialize a wait event */
void s_event_init(s_event_t *event);

/* Wait event */
int s_event_wait(__async__, s_event_t *event);

/* Set event */
void s_event_set(s_event_t *event);

/* Wait event with timeout */
int s_event_wait_msec(__async__, s_event_t *event, uint32_t msec);

/* Wait event with timeout */
int s_event_wait_sec(__async__, s_event_t *event, uint32_t sec);
```

## Specials on embedded platform
<details>
  <summary>API on embedded platform</summary>

### Chan for interrupt (for embedded only, STM8/STM32/M051/Arduino)

#### chan api called in tasks

```c
/* Task puts element into chan and waits interrupt to read the chan */
void s_chan_put__to_irq(__async__, s_chan_t *chan, const void *in_object);

/* Task puts number of elements into chan and waits interrupt to read the chan */
void s_chan_put_n__to_irq(__async__, s_chan_t *chan, const void *in_object, uint16_t number);

/* Task waits interrupt to write the chan and then gets element from chan */
void s_chan_get__from_irq(__async__, s_chan_t *chan, void *out_object);

/* Task waits interrupt to write the chan and then gets number of elements from chan */
void s_chan_get_n__from_irq(__async__, s_chan_t *chan, void *out_object, uint16_t number);
```

#### chan api called in interrupt

```c
/*
 * Interrupt writes element into the chan,
 * return number of element was written into chan
 */
uint16_t s_chan_put__in_irq(s_chan_t *chan, const void *in_object);

/*
 * Interrupt writes number of elements into the chan,
 * return number of element was written into chan
 */
uint16_t s_chan_put_n__in_irq(s_chan_t *chan, const void *in_object, uint16_t number);

/*
 * Interrupt reads element from chan,
 * return number of element was read from chan
 */
uint16_t s_chan_get__in_irq(s_chan_t *chan, void *out_object);

/*
 * Interrupt reads number of elements from chan,
 * return number of element was read from chan
 */
uint16_t s_chan_get_n__in_irq(s_chan_t *chan, void *out_object, uint16_t number);

```

### Event for interrupt (for embedded only, STM8/STM32/M051/Arduino)

#### event api called in tasks

```c
/* 
 * Wait event from irq, disable irq before call this function!
 *   S_IRQ_DISABLE()
 *   ...
 *   s_event_wait__from_irq(...)
 *   ...
 *   S_IRQ_ENABLE()
 */
int s_event_wait__from_irq(__async__, s_event_t *event);

/* 
 * Wait event from irq, disable irq before call this function!
 *   S_IRQ_DISABLE()
 *   ...
 *   s_event_wait_msec__from_irq(...)
 *   ...
 *   S_IRQ_ENABLE()
 */
int s_event_wait_msec__from_irq(__async__, s_event_t *event, uint32_t msec);

/* 
 * Wait event from irq, disable irq before call this function!
 *   S_IRQ_DISABLE()
 *   ...
 *   s_event_wait_sec__from_irq(...)
 *   ...
 *   S_IRQ_ENABLE()
 */
int s_event_wait_sec__from_irq(__async__, s_event_t *event, uint32_t sec);
```

#### event api called in interrupt

```c
/* Set event in interrupt */
void s_event_set__in_irq(s_event_t *event);
```

</details>

<details>
  <summary>Low power mode</summary>

If there's no code in function "my_on_idle", the program will run in busy wait mode, which may cause CPU 100% occupied.
But we can avoid this and support low power mode by adding correct sleeping instructions in function my_on_idle.

Now we have do that on Windows/Linux/MacOS and Android.

On embedded platform without OS, we may not fully implement low power mode.
Please check function "my_on_idle" for the corresponding platform if you want to optimize the power consumption.

```
void my_on_idle(uint64_t max_idle_ms) {
    /* Add code here to make CPU run into sleep mode,
       the maximum sleeping time is "max_idle_ms" milliseconds. */
}
```
</details>

## How to make port?

[Please find document here](porting.md)

## Contact

使用中有任何问题或建议，欢迎QQ加群 567780316 交流。

![s_task交流群](qq.png)

## VS.

+ coro: http://www.goron.de/~froese/coro/
+ coroutine(a asymmetric coroutine library for C): https://github.com/cloudwu/coroutine
+ coroutine(a asymmetric coroutine (lua like) with fixed-size stack): https://github.com/xphh/coroutine
+ coroutine(coroutine library with pthread-like interface in pure C): https://github.com/Marcus366/coroutine
+ coroutines(A lightweight coroutine library written in C and assembler): https://github.com/xya/coroutines
+ fcontext: https://github.com/reginaldl/fcontext
+ hev-task-system: https://github.com/heiher/hev-task-system
+ libaco: https://github.com/hnes/libaco
+ libconcurrency: http://code.google.com/p/libconcurrency/
+ libconcurrent: https://github.com/sharow/libconcurrent
+ libcoro: http://software.schmorp.de/pkg/libcoro.html
+ libcoroutine: https://github.com/stevedekorte/coroutine
+ libfiber: http://www.rkeene.org/projects/info/wiki/22
+ libtask: https://code.google.com/p/libtask/
+ libwire: https://github.com/baruch/libwire
+ micro: https://github.com/mikewei/micoro
+ mill: https://github.com/sustrik/mill
+ Portable Coroutine Library (PCL): http://xmailserver.org/libpcl.html

## Thanks

+ [wooley](https://github.com/wooley) cmake + vc building.
