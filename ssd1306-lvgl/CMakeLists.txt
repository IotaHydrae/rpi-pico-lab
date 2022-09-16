cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(ssd1306)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project
add_executable(ssd1306 main.c ssd1306.c)

target_include_directories(ssd1306 PUBLIC .)
target_link_libraries(ssd1306 pico_stdlib hardware_i2c)

pico_enable_stdio_usb(ssd1306 0)
pico_enable_stdio_uart(ssd1306 1)

pico_add_extra_outputs(ssd1306)
