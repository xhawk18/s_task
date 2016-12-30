# setjmp_task - A multi-task library implemented by pure C.

## Usage
setjmp_task is designed for easy use, just use task_init_system() to initialize the system,
and use task_create to create tasks is OK.

After task_init_system() was called, main_task will be created, and the main function will halt
until all tasks are finished.

setjmp_task is not preemptive multitasking. A task need call task_msleep or task_yield to make
other tasks hava chance to run.

```c
void sub_task(void *arg) {
    int i;
    int n = (int)arg;
    for (i = 0; i < 5; ++i) {
        PRINTF("task %d, delay seconds = %d, i = %d\n", n, n, i);
        task_msleep(n * 1000);
        //task_yield();
    }
}

void main_task(void *arg) {
    int i;
    task_create(sub_task, (void *)1);
    task_create(sub_task, (void *)2);

    for (i = 0; i < 4; ++i) {
        PRINTF("task_main arg = %p, i = %d\n", arg, i);
        task_yield();
    }
}

int main(int argc, char *argv) {
    task_init_system(main_task, (void *)argc);
    PRINTF("all task is over\n");
    return 0;
}
```

## API
```c
/* Function type for task entrance */
typedef void(*task_fn_t)(void *arg);

/* Initialize the task system. */
void task_init_system(task_fn_t main_entry, void *arg);

/* Create a new task */
void task_create(task_fn_t entry, void *arg);

/* Sleep in milliseconds */
void task_msleep(uint32_t msec);

/* Yield current task */
void task_yield(void);
```

## How to port
To make a port of "setjmp_task" to new system is very simple.
Here's an example --
```c
//1. define a type for clock
typedef clock_t my_clock_t;

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
    usleep(max_idle_ms / 1000);
}
```
