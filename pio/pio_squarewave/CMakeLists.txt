cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(pio_squarewave)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project
add_executable(pio_squarewave squarewave.c)

target_link_libraries(pio_squarewave pico_stdlib hardware_pio)

pico_add_extra_outputs(pio_squarewave)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/generated)
# generate .hex file and .pio.h file for the RP2040 datasheet (to make sure
# the datasheet always shows the output of the latest pioasm version)
add_custom_target(pio_squarewave_datasheet DEPENDS
		${CMAKE_CURRENT_LIST_DIR}/generated/squarewave.hex
		${CMAKE_CURRENT_LIST_DIR}/generated/squarewave.pio.h
		)

add_custom_command(OUTPUT ${CMAKE_CURRENT_LIST_DIR}/generated/squarewave.hex
        DEPENDS ${CMAKE_CURRENT_LIST_DIR}/squarewave.pio
        COMMAND pioasm -o hex ${CMAKE_CURRENT_LIST_DIR}/squarewave.pio ${CMAKE_CURRENT_LIST_DIR}/generated/squarewave.hex
        VERBATIM)
add_custom_command(OUTPUT ${CMAKE_CURRENT_LIST_DIR}/generated/squarewave.pio.h
        DEPENDS ${CMAKE_CURRENT_LIST_DIR}/squarewave.pio
        COMMAND pioasm ${CMAKE_CURRENT_LIST_DIR}/squarewave.pio ${CMAKE_CURRENT_LIST_DIR}/generated/squarewave.pio.h
        VERBATIM)

add_dependencies(pio_squarewave pio_squarewave_datasheet)
