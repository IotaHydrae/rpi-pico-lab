cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(epink VERSION 0.2)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# add include and src files
file(GLOB_RECURSE SOURCES main.c "src/*.c")
include_directories(GLOB "include")

# rest of your project
# add_executable(epink main.c ${INCLUDES} src/font_8x16.c)
add_executable(epink ${SOURCES})

target_link_libraries(epink pico_stdlib hardware_spi)

pico_enable_stdio_usb(epink 0)
pico_enable_stdio_uart(epink 1)

pico_add_extra_outputs(epink)
