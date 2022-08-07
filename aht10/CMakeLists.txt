cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(aht10)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project
add_executable(aht10 main.c aht10.c)

target_include_directories(aht10 PUBLIC .)
target_link_libraries(aht10 pico_stdlib hardware_i2c)

pico_enable_stdio_usb(aht10 0)
pico_enable_stdio_uart(aht10 1)

pico_add_extra_outputs(aht10)
