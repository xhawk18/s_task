# Copyright (c) 2022 HPMicro
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.13)

if("${CMAKE_BUILD_TYPE}" STREQUAL "")
    SET(CMAKE_BUILD_TYPE flash_xip)
endif()

set(CONFIG_SEGGER_RTT 1)
set(CONFIG_NDEBUG_CONSOLE 1)

set(APP_NAME "test_app")

add_definitions(-DBOARD_SHOW_CLOCK=0 -DBOARD_SHOW_BANNER=0 -DCONFIG_NDEBUG_CONSOLE -DCONSOLE_TYPE_UART=0)
add_compile_options(-g -gdwarf-2)

find_package(hpm-sdk REQUIRED HINTS $ENV{HPM_SDK_BASE})

project(app)

sdk_compile_definitions(-DUSB_HOST_MCU_CORE=HPM_CORE0)

add_definitions(-DHPMICRO)
add_subdirectory(s_task)

sdk_inc(src)
sdk_inc(s_task/include)

sdk_app_src(main.c)

target_link_libraries(app PRIVATE s_task)
