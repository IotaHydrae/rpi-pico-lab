cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(buzzer)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

add_subdirectory(tone)

# rest of your project
add_executable(buzzer main.c)

target_link_libraries(buzzer pico_stdlib hardware_pwm pwm_tone)

pico_add_extra_outputs(buzzer)
