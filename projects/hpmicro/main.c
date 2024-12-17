
#include <stdbool.h>
#include <stdint.h>
#include "board.h"
#include "hpm_common.h"
#include "s_task.h"

volatile bool g_is_low = false;
volatile bool g_exit   = false;

void sub_task_fast_blinking(__async__, void *arg)
{
    (void)arg;
    while (!g_exit)
    {
        s_task_msleep(__await__, 50); /* wait for 50 milliseconds */
        s_task_msleep(__await__, 50); /* wait for 50 milliseconds */
    }
}
void sub_task_fast_blinking1(__async__, void *arg)
{
    (void)arg;
    while (!g_exit)
    {
        s_task_msleep(__await__, 150); /* wait for 50 milliseconds */
        s_task_msleep(__await__, 150); /* wait for 50 milliseconds */
    }
}

ATTR_PLACE_AT_FAST_RAM_INIT uint32_t g_stack0[1024] = {0};
ATTR_PLACE_AT_FAST_RAM_INIT uint32_t g_stack1[1024] = {0};

void main_task(__async__, void *arg)
{
    (void)arg;


    /* create two sub tasks */
    s_task_create(g_stack0, sizeof(g_stack0), sub_task_fast_blinking, NULL);
    s_task_create(g_stack1, sizeof(g_stack1), sub_task_fast_blinking1, NULL);

    /* wait for 10 seconds */
    s_task_sleep(__await__, 10);

    g_exit = true;

    /* wait two sub tasks return */
    s_task_join(__await__, g_stack0);
    s_task_join(__await__, g_stack1);
}

ATTR_PLACE_AT_FAST_RAM_INIT void* g_stack_main[1024];

uint64_t get_timer_value(void)
{
    return HPM_MCHTMR->MTIME;
}

int main(void)
{
    board_init();

    board_delay_ms(50);

    s_task_init_system();

    s_task_create(g_stack_main, sizeof(g_stack_main), main_task, NULL);
    s_task_join(__await__, g_stack_main);

    while (1)
    {
    };
}
