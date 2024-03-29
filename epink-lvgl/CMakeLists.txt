cmake_minimum_required(VERSION 3.13)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(epink)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# add include and src files
file(GLOB_RECURSE INCLUDES "include/*.h" "port/*.h")
file(GLOB_RECURSE SOURCES "lvgl/src/*.c" "lvgl/demos/*.c" "font/*.c" "port/*.c")

# rest of your project
add_executable(epink main.c logo.c ${INCLUDES} ${SOURCES})

target_include_directories(epink PUBLIC . "lvgl")
target_link_libraries(epink pico_stdlib hardware_spi)

pico_enable_stdio_usb(epink 0)
pico_enable_stdio_uart(epink 1)

pico_add_extra_outputs(epink)
