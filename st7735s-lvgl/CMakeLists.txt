cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(st7735s)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

file(GLOB_RECURSE INCLUDES "include/*.h" "port/*.h" "ui/*.h")
file(GLOB_RECURSE SOURCES "lvgl/src/*.c" "lvgl/demos/stress/*.c" "port/*.c" "ui/*.c")

# rest of your project
add_executable(st7735s main.c ${INCLUDES} ${SOURCES})

target_include_directories(st7735s PUBLIC .)
target_link_libraries(st7735s pico_stdlib hardware_spi hardware_pwm)

pico_enable_stdio_usb(st7735s 0)
pico_enable_stdio_uart(st7735s 1)

pico_add_extra_outputs(st7735s)
