cmake_minimum_required(VERSION 3.13)

SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)
SET(CMAKE_BUILD_TYPE Debug)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(psram-mpu)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project
add_executable(psram-mpu main.c)
target_link_libraries(psram-mpu pico_stdlib pico_bootsel_via_double_reset)
pico_add_extra_outputs(psram-mpu)

add_executable(psram-test tests/psram-test.c)
target_link_libraries(psram-test pico_stdlib pico_bootsel_via_double_reset hardware_spi)
pico_add_extra_outputs(psram-test)
