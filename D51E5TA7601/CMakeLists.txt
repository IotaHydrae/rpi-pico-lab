cmake_minimum_required(VERSION 3.13)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(d51e5ta7601_gpio)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project
add_executable(d51e5ta7601_gpio main.c d51e5ta7601_gpio.c ns2009.c)

target_include_directories(d51e5ta7601_gpio PUBLIC .)
target_link_libraries(d51e5ta7601_gpio pico_stdlib hardware_pwm hardware_i2c)

pico_enable_stdio_usb(d51e5ta7601_gpio 0)
pico_enable_stdio_uart(d51e5ta7601_gpio 1)

pico_add_extra_outputs(d51e5ta7601_gpio)
