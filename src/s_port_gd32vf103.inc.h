/* Copyright xhawk, MIT license */

/* Timer functions need to be implemented on a new porting. */

void my_clock_init(){
    // Don't start measuruing until we see an mtime tick
    uint64_t start_mtime;
    uint64_t tmp = get_timer_value();
    do {
        start_mtime = get_timer_value();
    } while (start_mtime == tmp);
}

my_clock_t my_clock() {
    return get_timer_value();
}

void my_on_idle(uint64_t max_idle_ms) {
    uint64_t start_mtime, delta_mtime;

    start_mtime = get_timer_value();

    do {
        delta_mtime = get_timer_value() - start_mtime;
    } while(
        !g_globals.irq_actived
        && delta_mtime < (MY_CLOCKS_PER_SEC * max_idle_ms / 1000));
}


#ifdef USE_JUMP_FCONTEXT
extern
transfer_t jump_fcontext( fcontext_t const to, void * vp);
extern
fcontext_t make_fcontext( void * sp, size_t size, void (* fn)( transfer_t) );

void create_fcontext(fcontext_t *fc, void *stack, size_t stack_size, void (* fn)( transfer_t)) {
    stack = (void *)((char *)stack + stack_size);
    *fc = make_fcontext(stack, stack_size, fn);
}
#endif

