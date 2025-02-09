cmake_minimum_required(VERSION 3.13)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(st7305)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project
add_executable(st7305 main.c pic_test.c)

target_include_directories(st7305 PUBLIC .)
target_link_libraries(st7305 pico_stdlib hardware_spi hardware_pwm)

pico_enable_stdio_usb(st7305 0)
pico_enable_stdio_uart(st7305 1)

pico_add_extra_outputs(st7305)
