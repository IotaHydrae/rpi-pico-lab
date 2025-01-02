cmake_minimum_required(VERSION 3.13)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(soldering_test)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project
add_executable(soldering_test main.c pio/test.c)
pico_generate_pio_header(soldering_test ${CMAKE_CURRENT_LIST_DIR}/pio/test.pio)
target_link_libraries(soldering_test pico_stdlib hardware_pwm hardware_pio hardware_dma)


target_include_directories(soldering_test PUBLIC .)
pico_enable_stdio_usb(soldering_test 0)
pico_enable_stdio_uart(soldering_test 0)

pico_add_extra_outputs(soldering_test)
