cmake_minimum_required(VERSION 3.13)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(fl350hvc03v10_gpio)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

include_directories(PUBLIC .)
# add_subdirectory(lvgl)

file(GLOB_RECURSE SOURCES "lvgl/src/*.c"
                            "lvgl/demos/widgets/*.c"
                            "lvgl/demos/stress/*.c"
                            "lvgl/demos/benchmark/*.c"
                            "lvgl/demos/music/*.c"
                            "lvgl/demos/keypad_encoder/*.c")

# rest of your project
if (DISP_OVER_PIO)
    add_executable(fl350hvc03v10_gpio main.c fl350hvc03v10_pio.c
        porting/lv_port_disp_template.c pio/i80.c ${SOURCES})
    pico_generate_pio_header(fl350hvc03v10_gpio ${CMAKE_CURRENT_LIST_DIR}/pio/i80.pio)
    target_link_libraries(fl350hvc03v10_gpio pico_stdlib hardware_pwm hardware_pio hardware_dma)
else(DISP_OVER_PIO)
    add_executable(fl350hvc03v10_gpio main.c fl350hvc03v10_gpio.c porting/lv_port_disp_template.c)
    target_link_libraries(fl350hvc03v10_gpio lvgl lvgl_demos pico_stdlib hardware_pwm)
endif(DISP_OVER_PIO)

target_include_directories(fl350hvc03v10_gpio PUBLIC . porting/)

pico_enable_stdio_usb(fl350hvc03v10_gpio 0)
pico_enable_stdio_uart(fl350hvc03v10_gpio 1)

pico_add_extra_outputs(fl350hvc03v10_gpio)
