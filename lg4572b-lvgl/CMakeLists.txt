cmake_minimum_required(VERSION 3.13)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(lg4572b_gpio)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

include_directories(PUBLIC .)
add_subdirectory(lvgl)
# add_subdirectory(pio)

# rest of your project
if (DISP_OVER_PIO)
    add_executable(lg4572b_gpio main.c lg4572b_pio.c
        porting/lv_port_disp_template.c pio/i80.c)
    pico_generate_pio_header(lg4572b_gpio ${CMAKE_CURRENT_LIST_DIR}/pio/i80.pio)
    target_link_libraries(lg4572b_gpio lvgl lvgl_demos pico_stdlib hardware_pwm hardware_pio hardware_dma)
else(DISP_OVER_PIO)
    add_executable(lg4572b_gpio main.c lg4572b_gpio.c porting/lv_port_disp_template.c)
    target_link_libraries(lg4572b_gpio lvgl lvgl_demos pico_stdlib hardware_pwm)
endif(DISP_OVER_PIO)

target_include_directories(lg4572b_gpio PUBLIC . porting/)

pico_enable_stdio_usb(lg4572b_gpio 0)
pico_enable_stdio_uart(lg4572b_gpio 1)

pico_add_extra_outputs(lg4572b_gpio)
