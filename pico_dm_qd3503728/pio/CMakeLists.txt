cmake_minimum_required(VERSION 3.13)

if(BUILD_TEST)
    # initialize the SDK based on PICO_SDK_PATH
    # note: this must happen before project()
    include(pico_sdk_import.cmake)

    project(pio_i80)

    # initialize the Raspberry Pi Pico SDK
    pico_sdk_init()
    add_executable(pio_i80 i80.c)

    pico_generate_pio_header(pio_i80 ${CMAKE_CURRENT_LIST_DIR}/i80.pio)

    target_link_libraries(pio_i80 PRIVATE pico_stdlib hardware_pio hardware_dma)

    pico_enable_stdio_usb(pio_i80 0)
    pico_enable_stdio_uart(pio_i80 1)
    
    pico_add_extra_outputs(pio_i80)
else(BUILD_TEST)
    add_library(pio_i80)

    pico_generate_pio_header(pio_i80 ${CMAKE_CURRENT_LIST_DIR}/i80.pio)

    target_sources(pio_i80 PRIVATE i80.c)

    target_link_libraries(pio_i80 PRIVATE
            pico_stdlib
            hardware_pio
            hardware_dma
            )

endif(BUILD_TEST)