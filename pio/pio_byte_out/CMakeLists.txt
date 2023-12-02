cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(pio_test)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project
add_executable(pio_test main.c)

target_link_libraries(pio_test pico_stdlib hardware_pio)

pico_add_extra_outputs(pio_test)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/generated)
# generate .hex file and .pio.h file for the RP2040 datasheet (to make sure
# the datasheet always shows the output of the latest pioasm version)
add_custom_target(pio_test_datasheet DEPENDS
		${CMAKE_CURRENT_LIST_DIR}/generated/test.hex
		${CMAKE_CURRENT_LIST_DIR}/generated/test.pio.h
		)

add_custom_command(OUTPUT ${CMAKE_CURRENT_LIST_DIR}/generated/test.hex
        DEPENDS ${CMAKE_CURRENT_LIST_DIR}/test.pio
        COMMAND pioasm -o hex ${CMAKE_CURRENT_LIST_DIR}/test.pio ${CMAKE_CURRENT_LIST_DIR}/generated/test.hex
        VERBATIM)
add_custom_command(OUTPUT ${CMAKE_CURRENT_LIST_DIR}/generated/test.pio.h
        DEPENDS ${CMAKE_CURRENT_LIST_DIR}/test.pio
        COMMAND pioasm ${CMAKE_CURRENT_LIST_DIR}/test.pio ${CMAKE_CURRENT_LIST_DIR}/generated/test.pio.h
        VERBATIM)

add_dependencies(pio_test pio_test_datasheet)
