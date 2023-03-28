cmake_minimum_required(VERSION 3.12)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(my_project)

# initialize the Pico SDK
pico_sdk_init()


# rest of your project

set(st_sources
    ../src/s_task.c
    ../src/s_mutex.c
    ../src/s_event.c
    ../src/s_event_embed.c
    ../src/s_chan.c
    ../src/s_chan_embed.c
    ../src/s_timer_fast.c
    ../src/s_timer_small.c
    ../src/s_list.c
    ../src/s_rbtree.c)


add_executable(project
    ../../examples/ex5_led_blinking.c
    ${st_sources}
)

target_include_directories(project PRIVATE ../../include)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(project pico_stdlib)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(project)
