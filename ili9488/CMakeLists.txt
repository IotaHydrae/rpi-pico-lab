cmake_minimum_required(VERSION 3.13)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(ili9488_gpio)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project
if (DISP_OVER_PIO)
    add_executable(ili9488_gpio ili9488_pio.c pio/i80.c)
    pico_generate_pio_header(ili9488_gpio ${CMAKE_CURRENT_LIST_DIR}/pio/i80.pio)
    target_link_libraries(ili9488_gpio pico_stdlib hardware_pwm hardware_pio hardware_dma)
else(DISP_OVER_PIO)
    add_executable(ili9488_gpio ili9488_gpio.c)
    target_link_libraries(ili9488_gpio pico_stdlib hardware_pwm)
endif(DISP_OVER_PIO)

target_include_directories(ili9488_gpio PUBLIC .)
pico_enable_stdio_usb(ili9488_gpio 0)
pico_enable_stdio_uart(ili9488_gpio 1)

pico_add_extra_outputs(ili9488_gpio)
