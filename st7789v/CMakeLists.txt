cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(st7789v)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project
add_executable(st7789v main.c)

target_include_directories(st7789v PUBLIC .)
target_link_libraries(st7789v pico_stdlib hardware_spi)

pico_enable_stdio_usb(st7789v 0)
pico_enable_stdio_uart(st7789v 1)

pico_add_extra_outputs(st7789v)
