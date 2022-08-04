cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(bh1750)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project
add_executable(bh1750 main.c bh1750.c)

target_link_libraries(bh1750 pico_stdlib hardware_i2c)

pico_enable_stdio_usb(bh1750 0)
pico_enable_stdio_uart(bh1750 1)

pico_add_extra_outputs(bh1750)
