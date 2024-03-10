cmake_minimum_required(VERSION 3.13)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Wl,--print-memory-usage")
SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Wl,--print-memory-usage")

# set global vals here
set(ST7789V_SCL_PIN   18)
set(ST7789V_SDA_PIN   19)
set(ST7789V_RES_PIN   15)
set(ST7789V_DC_PIN    14)
set(ST7789V_CS_PIN    13)
set(ST7789V_BLK_PIN   12)
set(ST7789V_HOR_RES   240)
set(ST7789V_VER_RES   240)
set(ST7789V_BUS_CLK_KHZ 62500)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(st7789v_lvgl)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

file(GLOB_RECURSE INCLUDES "include/*.h" "port/*.h")
file(GLOB_RECURSE SOURCES "lvgl/src/*.c" "lvgl/demos/*.c" "port/*.c")

# rest of your project
add_executable(${PROJECT_NAME} main.c ${INCLUDES} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC .)
target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    hardware_spi
    hardware_pwm
    hardware_clocks
    pico_bootsel_via_double_reset
)
target_compile_definitions(${PROJECT_NAME} PUBLIC ST7789V_SCL_PIN=${ST7789V_SCL_PIN})
target_compile_definitions(${PROJECT_NAME} PUBLIC ST7789V_SDA_PIN=${ST7789V_SDA_PIN})
target_compile_definitions(${PROJECT_NAME} PUBLIC ST7789V_RES_PIN=${ST7789V_RES_PIN})
target_compile_definitions(${PROJECT_NAME} PUBLIC ST7789V_DC_PIN=${ST7789V_DC_PIN})
target_compile_definitions(${PROJECT_NAME} PUBLIC ST7789V_CS_PIN=${ST7789V_CS_PIN})
target_compile_definitions(${PROJECT_NAME} PUBLIC ST7789V_BLK_PIN=${ST7789V_BLK_PIN})
target_compile_definitions(${PROJECT_NAME} PUBLIC ST7789V_BUS_CLK_KHZ=${ST7789V_BUS_CLK_KHZ})

pico_enable_stdio_usb(${PROJECT_NAME} 0)
pico_enable_stdio_uart(${PROJECT_NAME} 1)

pico_add_extra_outputs(${PROJECT_NAME})
