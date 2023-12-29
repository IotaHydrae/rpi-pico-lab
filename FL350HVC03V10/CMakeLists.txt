cmake_minimum_required(VERSION 3.13)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(fl350hvc03v10_gpio)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project
add_executable(fl350hvc03v10_gpio fl350hvc03v10_gpio.c)

target_include_directories(fl350hvc03v10_gpio PUBLIC .)
target_link_libraries(fl350hvc03v10_gpio pico_stdlib hardware_pwm hardware_i2c)

pico_enable_stdio_usb(fl350hvc03v10_gpio 0)
pico_enable_stdio_uart(fl350hvc03v10_gpio 1)

pico_add_extra_outputs(fl350hvc03v10_gpio)
