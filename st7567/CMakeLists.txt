cmake_minimum_required(VERSION 3.13)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(st7567)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project
add_executable(st7567 main.c)

target_include_directories(st7567 PUBLIC .)
target_link_libraries(st7567 pico_stdlib hardware_spi hardware_pwm)

pico_enable_stdio_usb(st7567 0)
pico_enable_stdio_uart(st7567 1)

pico_add_extra_outputs(st7567)
