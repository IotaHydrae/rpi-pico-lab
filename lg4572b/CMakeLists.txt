cmake_minimum_required(VERSION 3.13)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(lg4572b_gpio)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project
add_executable(lg4572b_gpio lg4572b_gpio.c)

target_include_directories(lg4572b_gpio PUBLIC .)
target_link_libraries(lg4572b_gpio pico_stdlib hardware_pwm)

pico_enable_stdio_usb(lg4572b_gpio 0)
pico_enable_stdio_uart(lg4572b_gpio 1)

pico_add_extra_outputs(lg4572b_gpio)
