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
