cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(keypad)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project
add_executable(keypad main.c keypad.c)

target_link_libraries(keypad pico_stdlib)

pico_add_extra_outputs(keypad)
