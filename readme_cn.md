# s_task - 跨平台的C语言协程多任务库

## 目录

  - [特性](#%e7%89%b9%e6%80%a7)
  - [协程 vs 多线程](#%e5%8d%8f%e7%a8%8b-vs-%e5%a4%9a%e7%ba%bf%e7%a8%8b)
  - [示例](#%e7%a4%ba%e4%be%8b)
    - [示例 1 - 创建简单任务](#%e7%a4%ba%e4%be%8b-1---%e5%88%9b%e5%bb%ba%e7%ae%80%e5%8d%95%e4%bb%bb%e5%8a%a1)
    - [示例 2 - （无需回调函数的）异步HTTP客户端程序](#%e7%a4%ba%e4%be%8b-2---%e6%97%a0%e9%9c%80%e5%9b%9e%e8%b0%83%e5%87%bd%e6%95%b0%e7%9a%84%e5%bc%82%e6%ad%a5http%e5%ae%a2%e6%88%b7%e7%ab%af%e7%a8%8b%e5%ba%8f)
    - [示例 3 - ardinuo下同时跑多个任务控制led闪烁](#%e7%a4%ba%e4%be%8b-3---ardinuo%e4%b8%8b%e5%90%8c%e6%97%b6%e8%b7%91%e5%a4%9a%e4%b8%aa%e4%bb%bb%e5%8a%a1%e6%8e%a7%e5%88%b6led%e9%97%aa%e7%83%81)
  - [兼容性](#%e5%85%bc%e5%ae%b9%e6%80%a7)
  - [编译](#%e7%bc%96%e8%af%91)
    - [Linux / MacOS / MingW(MSYS2)](#linux--macos--mingwmsys2)
    - [Windows 或其他平台 other platforms](#windows-%e6%88%96%e5%85%b6%e4%bb%96%e5%b9%b3%e5%8f%b0)
  - [如何在您的项目中使用s_task？](#%e5%a6%82%e4%bd%95%e5%9c%a8%e6%82%a8%e7%9a%84%e9%a1%b9%e7%9b%ae%e4%b8%ad%e4%bd%bf%e7%94%a8stask)
  - [API](#api)
    - [Task （任务）](#task-%e4%bb%bb%e5%8a%a1)
    - [Mutex （互斥量）](#mutex-%e4%ba%92%e6%96%a5%e9%87%8f)
    - [Event （事件）](#event-%e4%ba%8b%e4%bb%b6)
  - [希望移植到新的平台？](#%e5%b8%8c%e6%9c%9b%e7%a7%bb%e6%a4%8d%e5%88%b0%e6%96%b0%e7%9a%84%e5%b9%b3%e5%8f%b0)
  - [联系方式](#%e8%81%94%e7%b3%bb%e6%96%b9%e5%bc%8f)
  - [其他协程库对比](#%e5%85%b6%e4%bb%96%e5%8d%8f%e7%a8%8b%e5%ba%93%e5%af%b9%e6%af%94)
  
## 特性

 + 全c和汇编实现，紧凑小巧又不失实用，并且不需要c++。
 + 协程切换代码来自boost汇编，性能极好，稳定可靠，移植性好，几乎全平台支持。
 + 和libuv（稍作修改）无缝融合，完美支持跨平台网络编程。
 + 支持 __await__ , __async__ 关键词，含义和用法都其他语言的await/async相同 --
    没有调用 await 函数的地方，协程肯定不会被切换出去，可确保共享数据不会被其他协程改变。
    具备传染性，能调用 await 的函数，一定在一个 async 函数里。这个async 函数需要用 await 调用。
 + 支持协程间的event变量和mutex锁，方便不同协程间同步数据和状态。这个方式比其他协程resume函数更好用和可控。
 + 除支持windows, linux, macos这些常规环境外，更能为stm32等嵌入式小芯片环境提供多任务支持（注：小芯片环境下不支持libuv)。
 + 嵌入式小芯片版本没有动态内存分配，增加程序大小不到2k, 不增加程序空间负担。

## 协程 vs 多线程

协程和多线程编程模式对比，协程的优势极其明显 --

+ 协程不会陷入死锁的窘境，多线程编程时，您可在死锁的坑里摸爬滚打过？
+ 一般协程比多线程更少的代码量就能实现，这在资源捉襟见肘的嵌入式单片机中尤其重要。有时跑个多线程 RTOS 库，应用自己都没空间了。s_task协程只增加了不到2K的代码量，这对单片机极其友好。
+ 协程主动让出CPU，没有 “抢占式多任务” 一说。您没看错，人们已经开始反思，“抢占式多任务” 根本不是啥优势，而是**多线程最大的缺点**，更是**bug之源**。主动让出CPU的协程，减少bug的同时，更能带来更好的CPU利用率，更多的并发任务数。这也是近年来，不管C++, C#, nodejs, java, php各式语言，都开始引入协程的原因。
+ 协程比任何的所谓 “实时操作系统RTOS” 更实时。
+ 协程通常用 __await__ 标注某个函数在运行的时候，CPU可能会被切换到其他任务。如此多任务间共享变量变得无比安全，这点是线程不能比的。

s_task协程库，更是打造了全平台兼容的协程支持环境，从高端linux服务器、windows/apple桌面，到安卓ndk，到stm32、arduino等各式嵌入式无操作系统环境都能支持，有些芯片的运行内存甚至可以低至1K大小。（[参考兼容性列表](#%e5%85%bc%e5%ae%b9%e6%80%a7)）

所有这些平台，全部共享一套同样接口的多任务[API](#api)。使用s_task，您将用最小的使用成本，获得最大的收益。

现在，放弃进入历史垃圾堆的多线程编程，开始您的 s_task 协程之旅！

## 示例

### [示例 1](examples/ex0_task.c) - 创建简单任务

```c
#include <stdio.h>
#include "s_task.h"

//定义协程任务需要的栈空间
void* g_stack_main[64 * 1024];
void* g_stack0[64 * 1024];
void* g_stack1[64 * 1024];

void sub_task(__async__, void* arg) {
    int i;
    int n = (int)(size_t)arg;
    for (i = 0; i < 5; ++i) {
        printf("task %d, delay seconds = %d, i = %d\n", n, n, i);
        s_task_msleep(__await__, n * 1000);  //等待一点时间
    }
}

void main_task(__async__, void* arg) {
    int i;

    //创建两个子任务
    s_task_create(g_stack0, sizeof(g_stack0), sub_task, (void*)1);
    s_task_create(g_stack1, sizeof(g_stack1), sub_task, (void*)2);

    for (i = 0; i < 4; ++i) {
        printf("task_main arg = %p, i = %d\n", arg, i);
        s_task_yield(__await__); //主动让出cpu
    }

    //等待子任务结束
    s_task_join(__await__, g_stack0);
    s_task_join(__await__, g_stack1);
}

int main(int argc, char* argv) {
    __init_async__;

    s_task_init_system();

    //创建一个任务
    s_task_create(g_stack_main, sizeof(g_stack_main), main_task, (void*)(size_t)argc);
    s_task_join(__await__, g_stack_main);
    printf("all task is over\n");
    return 0;
}
```

### [示例 2](examples/ex3_http_client.c) - （无需回调函数的）异步HTTP客户端程序

```c
void main_task(__async__, void *arg) {
    uv_loop_t* loop = (uv_loop_t*)arg;

    const char *HOST = "baidu.com";
    const unsigned short PORT = 80;

    //<1> 异步域名解析
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

    //<2> 异步连接服务端
    uv_tcp_t tcp_client;
    int ret = uv_tcp_init(loop, &tcp_client);
    if (ret != 0)
        goto out1;
    ret = s_uv_tcp_connect(__await__, &tcp_client, addr->ai_addr);
    if (ret != 0)
        goto out2;

    //<3> 异步发送请求
    const char *request = "GET / HTTP/1.0\r\n\r\n";
    uv_stream_t* tcp_stream = (uv_stream_t*)&tcp_client;
    s_uv_write(__await__, tcp_stream, request, strlen(request));

    //<4> 异步读HTTP返回数据
    ssize_t nread;
    char buf[1024];
    while (true) {
        ret = s_uv_read(__await__, tcp_stream, buf, sizeof(buf), &nread);
        if (ret != 0) break;

        // 输出从HTTP服务器读到的数据
        fwrite(buf, 1, nread, stdout);
    }

    //<5> 关闭连接
out2:;
    s_uv_close(__await__, (uv_handle_t*)&tcp_client);
out1:;
    uv_freeaddrinfo(addr);
out0:;
}
```

### [示例 3](build/arduino/arduino.ino) - ardinuo下同时跑多个任务控制led闪烁

```c
#include <stdio.h>
#include "src/s_task/s_task.h"

//这个程序运行了三个任务
// 1) 任务 main_task - 
//    等10秒并设置退出标志 g_exit，
//    在所有其他任务退出后，将LED设为常量。
// 2) 任务 sub_task_fast_blinking -
//    使 LED 快速闪烁
// 3) 任务 sub_task_set_low -
//    使上个任务中快速闪烁的LED，没快速闪烁3秒种，熄灭1秒种。

void setup() {
    // 初始化LED
    pinMode(LED_BUILTIN, OUTPUT);
}


//定义协程任务需要的栈空间
char g_stack0[384];
char g_stack1[384];
volatile bool g_is_low = false;
volatile bool g_exit = false;

void sub_task_fast_blinking(__async__, void* arg) {
    while(!g_exit) {
        if(!g_is_low)
            digitalWrite(LED_BUILTIN, HIGH); // 点亮LED

        s_task_msleep(__await__, 50);        // 等50毫秒
        digitalWrite(LED_BUILTIN, LOW);      // 熄灭LED
        s_task_msleep(__await__, 50);        // 等50毫秒
    }    
}

void sub_task_set_low(__async__, void* arg) {
    while(!g_exit) {
        g_is_low = true;                     // 关闭LED快闪
        digitalWrite(LED_BUILTIN, LOW);      // 熄灭LED
        s_task_sleep(__await__, 1);          // 等1秒
        g_is_low = false;                    // 打开LED快闪
        s_task_sleep(__await__, 3);          // 等3秒
    }    
}

void main_task(__async__, void* arg) {
    // 创建两个任务
    s_task_create(g_stack0, sizeof(g_stack0), sub_task_fast_blinking, NULL);
    s_task_create(g_stack1, sizeof(g_stack1), sub_task_set_low, NULL);

    // 等10秒
    s_task_sleep(__await__, 10);
    g_exit = true;

    // 等待两个任务结束
    s_task_join(__await__, g_stack0);
    s_task_join(__await__, g_stack1);
}

void loop() {
    __init_async__;
    
    s_task_init_system();
    main_task(__await__, NULL);

    // 使LED常亮
    digitalWrite(LED_BUILTIN, HIGH);
    while(1);
}
```


## 兼容性

"s_task" 可以作为一个单独的库使用，也可以配合libuv实现跨平台网络编程（编译时加上宏定义**USE_LIBUV**）。

| 平台                           | coroutine协程      | libuv支持          |
|--------------------------------|--------------------|--------------------|
| Windows                        | :heavy_check_mark: | :heavy_check_mark: |
| Linux                          | :heavy_check_mark: | :heavy_check_mark: |
| MacOS                          | :heavy_check_mark: | :heavy_check_mark: |
| Android                        | :heavy_check_mark: | :heavy_check_mark: |
| MingW (https://www.msys2.org/) | :heavy_check_mark: | :heavy_check_mark: |
| ARMv6-M (M051)                 | :heavy_check_mark: | :x:                |
| ARMv7-M (STM32F103,STM32F302)  | :heavy_check_mark: | :x:                |
| STM8S103                       | :heavy_check_mark: | :x:                |
| STM8L051F3                     | :heavy_check_mark: | :x:                |
| Arduino UNO (AVR MEGA328P)     | :heavy_check_mark: | :x:                |

   linux在以下硬件环境测试通过
   * i686 (ubuntu-16.04)
   * x86_64 (centos-8.1)
   * arm (树莓派32位)
   * aarch64 (① 树莓派64位, ② ubuntu 14.04 运行于华为鲲鹏920)
   * mipsel (openwrt ucLinux 3.10.14 for MT7628)
   * mips64 (fedora for loongson 3A-4000 龙芯)

## 编译

### Linux / MacOS / MingW(MSYS2)

    cd build
    cmake .
    make

### Windows 或其他平台

| 平台       | 项目                              | 工具链                                      |
|------------|-----------------------------------|---------------------------------------------|
| Windows    | build\windows\s_task.sln          | visual studio 2019                          |
| Android    | build\android\cross_build_arm*.sh | android ndk 20, API level 21 (在termux测试) |
| STM8S103   | build\stm8s103\Project.eww        | IAR workbench for STM8                      |
| STM8L051F3 | build\stm8l05x\Project.eww        | IAR workbench for STM8                      |
| STM32F103  | build\stm32f103\Project.uvproj    | Keil uVision5                               |
| STM32F302  | build\stm32f302\Project.uvporj    | Keil uVision5                               |
| M051       | build\m051\Project.uvporj         | Keil uVision5                               |
| ATmega328P | build\atmega328p\atmega328p.atsln | Atmel Studio 7.0                            |
| Arduino    | build\arduino\arduino.ino         | Arduino IDE                                 |

## 如何在您的项目中使用s_task？

在 linux/unix 等环境里，可以先用cmake编译，编译完成后，将产生可以直接用于您的项目的链接库文件，您可以通过以下简单3步使用s_task --

* 将 libs_task.a 加入到您的项目
* #include "[s_task.h](include/s_task.h)"
* 编译时加上宏定义 USE_LIBUV

在 arduino 上使用，可以复制目录 "[include](include)" 和 "[src](src)" 下的所有*.h, *.c文件到您的项目的 src/s_task目录下。这里有个实际的目录结果可供参考："[build/arduino/](build/arduino/)"。

在 windows 或其他平台，请用 build 目录下的项目作为项目模板和参考。

## API

### Task （任务）

```c
/* Return values -- 
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

### Mutex （互斥量）

```c
/* Initialize a mutex */
void s_mutex_init(s_mutex_t *mutex);

/* Lock the mutex */
int s_mutex_lock(__async__, s_mutex_t *mutex);

/* Unlock the mutex */
void s_mutex_unlock(s_mutex_t *mutex);
```

### Event （事件）

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

#### Event for interrupt (中断里的事件，仅嵌入式平台支持，STM8/STM32/M051/Arduino)

```c
/* Set event in interrupt */
void s_event_set_irq(s_event_t *event);

/* Wait event from interrupt, need to disable interrupt before call this function!
 *   S_IRQ_DISABLE()
 *   ...
 *   s_event_wait_irq(...)
 *   ...
 *   S_IRQ_ENABLE()
 */
int s_event_wait_irq(__async__, s_event_t *event);

/* Wait event from interrupt, need to disable interrupt before call this function! */
int s_event_wait_irq_msec(__async__, s_event_t *event, uint32_t msec);

/* Wait event from interrupt, need to disable interrupt before call this function! */
int s_event_wait_irq_sec(__async__, s_event_t *event, uint32_t sec);
```



## 希望移植到新的平台？

[移植文档参考此处](porting.md)

## 联系方式

使用中有任何问题或建议，欢迎QQ加群 567780316 交流。

![s_task交流群](qq.png)

## 其他协程库对比

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
