cmake_minimum_required(VERSION 3.13)

if(BUILD_TEST)
    # initialize the SDK based on PICO_SDK_PATH
    # note: this must happen before project()
    include(pico_sdk_import.cmake)

    project(pio_test)

    # initialize the Raspberry Pi Pico SDK
    pico_sdk_init()
    add_executable(pio_test i80.c)

    pico_generate_pio_header(pio_test ${CMAKE_CURRENT_LIST_DIR}/i80.pio)

    target_link_libraries(pio_test PRIVATE pico_stdlib hardware_pio)

    pico_enable_stdio_usb(pio_test 0)
    pico_enable_stdio_uart(pio_test 1)
    
    pico_add_extra_outputs(pio_test)
else(BUILD_TEST)
    add_library(pio_test)

    pico_generate_pio_header(pio_test ${CMAKE_CURRENT_LIST_DIR}/i80.pio)

    target_sources(pio_test PRIVATE i80.c)

    target_link_libraries(pio_test PRIVATE
            pico_stdlib
            hardware_pio
            )

endif(BUILD_TEST)