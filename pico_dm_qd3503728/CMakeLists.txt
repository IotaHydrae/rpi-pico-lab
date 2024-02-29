cmake_minimum_required(VERSION 3.13)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(pico_dm_qd3503728)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Wl,--print-memory-usage")
SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Wl,--print-memory-usage")

include_directories(./ include)

add_subdirectory(lvgl)
add_subdirectory(pio)
add_subdirectory(factory)

target_link_libraries(lvgl PRIVATE pico_stdlib)

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
    add_executable(${PROJECT_NAME} ili9488_pio.c ${COMMON_SOURCES})
    target_link_libraries(${PROJECT_NAME}
        pico_bootsel_via_double_reset 
        pico_stdlib hardware_pwm 
        hardware_i2c pio_i80 factory_test
        lvgl lvgl::demos lvgl::examples)
else(DISP_OVER_PIO)
    add_executable(${PROJECT_NAME} ili9488_gpio.c ${LVGL_SOURCES} ${COMMON_SOURCES})
    target_link_libraries(${PROJECT_NAME}
        pico_bootsel_via_double_reset
        pico_stdlib hardware_pwm
        hardware_i2c factory_test
        lvgl lvgl::demos lvgl::examples)
endif(DISP_OVER_PIO)

target_include_directories(${PROJECT_NAME} PUBLIC .)

pico_enable_stdio_usb(${PROJECT_NAME} 0)
pico_enable_stdio_uart(${PROJECT_NAME} 1)

pico_add_extra_outputs(${PROJECT_NAME})

add_custom_target(
    print-memory-usage ALL
    COMMAND arm-none-eabi-size -G ${CMAKE_PROJECT_NAME}.elf
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/build/${CMAKE_PROJECT_NAME}.elf
    COMMENT "Print memory usage"
    DEPENDS ${PROJECT_NAME}
)