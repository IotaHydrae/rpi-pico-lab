cmake_minimum_required(VERSION 3.13)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(ili9488_gpio)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Wl,--print-memory-usage")
SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Wl,--print-memory-usage")

file(GLOB_RECURSE LVGL_SOURCES "lvgl/src/*.c"
                            "lvgl/demos/widgets/*.c"
                            "lvgl/demos/stress/*.c"
                            "lvgl/demos/benchmark/*.c"
                            "lvgl/demos/music/*.c"
                            "lvgl/demos/keypad_encoder/*.c")


add_subdirectory(pio)

file(GLOB_RECURSE COMMON_SOURCES
    main.c
    ft6236.c
    porting/lv_port_disp_template.c
    porting/lv_port_indev_template.c
    i2c_tools.c
    backlight.c
)

# rest of your project
if (DISP_OVER_PIO)
    add_executable(ili9488_gpio ili9488_pio.c ${LVGL_SOURCES} ${COMMON_SOURCES})
    target_link_libraries(ili9488_gpio pico_stdlib hardware_pwm hardware_i2c pio_i80)
else(DISP_OVER_PIO)
    add_executable(ili9488_gpio ili9488_gpio.c ${LVGL_SOURCES} ${COMMON_SOURCES})
    target_link_libraries(ili9488_gpio pico_stdlib hardware_pwm hardware_i2c)
endif(DISP_OVER_PIO)

target_include_directories(ili9488_gpio PUBLIC .)

pico_enable_stdio_usb(ili9488_gpio 0)
pico_enable_stdio_uart(ili9488_gpio 1)

pico_add_extra_outputs(ili9488_gpio)

add_custom_target(
    print-memory-usage ALL
    COMMAND arm-none-eabi-size -G ${CMAKE_PROJECT_NAME}.elf
    DEPENDS ${CMAKE_PROJECT_NAME}.elf
    COMMENT "Print memory usage"
    DEPENDS ili9488_gpio
)