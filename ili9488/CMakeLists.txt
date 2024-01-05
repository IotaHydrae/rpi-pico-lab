cmake_minimum_required(VERSION 3.13)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(ili9488_gpio)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project
add_executable(ili9488_gpio ili9488_gpio.c)

target_include_directories(ili9488_gpio PUBLIC .)
target_link_libraries(ili9488_gpio pico_stdlib hardware_pwm hardware_i2c)

pico_enable_stdio_usb(ili9488_gpio 0)
pico_enable_stdio_uart(ili9488_gpio 1)

pico_add_extra_outputs(ili9488_gpio)
