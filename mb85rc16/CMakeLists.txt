cmake_minimum_required(VERSION 3.13)

SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(mb85rc16)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project
add_executable(mb85rc16 main.c mb85rc16.c)

target_include_directories(mb85rc16 PUBLIC .)
target_link_libraries(mb85rc16 pico_stdlib hardware_i2c)

pico_enable_stdio_usb(mb85rc16 0)
pico_enable_stdio_uart(mb85rc16 1)

pico_add_extra_outputs(mb85rc16)
