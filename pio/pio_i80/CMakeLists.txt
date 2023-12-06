cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(pio_i80)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

if(BUILD_TEST)
    add_executable(pio_i80 i80.c)

    pico_generate_pio_header(pio_i80 ${CMAKE_CURRENT_LIST_DIR}/i80.pio)

    target_link_libraries(pio_i80 PRIVATE pico_stdlib hardware_pio)

    pico_enable_stdio_usb(pio_i80 0)
    pico_enable_stdio_uart(pio_i80 1)
    
    pico_add_extra_outputs(pio_i80)
else(BUILD_TEST)
    add_library(pio_i80)

    target_sources(pio_i80 PRIVATE i80.c)

    target_link_libraries(pio_i80 PRIVATE
            pico_stdlib
            hardware_pio
            )
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/generated)
    # generate .hex file and .pio.h file for the RP2040 datasheet (to make sure
    # the datasheet always shows the output of the latest pioasm version)
    add_custom_target(pio_i80_datasheet DEPENDS
            ${CMAKE_CURRENT_LIST_DIR}/generated/i80.hex
            ${CMAKE_CURRENT_LIST_DIR}/generated/i80.pio.h
            )
    
    add_custom_command(OUTPUT ${CMAKE_CURRENT_LIST_DIR}/generated/i80.hex
            DEPENDS ${CMAKE_CURRENT_LIST_DIR}/i80.pio
            COMMAND pioasm -o hex ${CMAKE_CURRENT_LIST_DIR}/i80.pio ${CMAKE_CURRENT_LIST_DIR}/generated/i80.hex
            VERBATIM)
    add_custom_command(OUTPUT ${CMAKE_CURRENT_LIST_DIR}/generated/i80.pio.h
            DEPENDS ${CMAKE_CURRENT_LIST_DIR}/i80.pio
            COMMAND pioasm ${CMAKE_CURRENT_LIST_DIR}/i80.pio ${CMAKE_CURRENT_LIST_DIR}/generated/i80.pio.h
            VERBATIM)
    
    add_dependencies(pio_i80 pio_i80_datasheet)
endif(BUILD_TEST)