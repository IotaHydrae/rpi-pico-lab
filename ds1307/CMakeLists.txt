cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(ds1307)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project
add_executable(ds1307 main.c)

target_link_libraries(ds1307 pico_stdlib hardware_i2c)

pico_enable_stdio_usb(ds1307 0)
pico_enable_stdio_uart(ds1307 1)

pico_add_extra_outputs(ds1307)
