cmake_minimum_required(VERSION 3.13)

SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(psram-bootloader)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project
add_executable(${PROJECT_NAME} main.c sfe_psram.c)

target_link_libraries(${PROJECT_NAME} pico_stdlib hardware_spi)

pico_enable_stdio_usb(${PROJECT_NAME} 0)
pico_enable_stdio_uart(${PROJECT_NAME} 1)

pico_embed_pt_in_binary(${PROJECT_NAME} ${CMAKE_CURRENT_LIST_DIR}/partition_table.json)

pico_set_uf2_family(${PROJECT_NAME} "absolute")
pico_package_uf2_output(${PROJECT_NAME} 0x10000000)

pico_add_extra_outputs(${PROJECT_NAME})
