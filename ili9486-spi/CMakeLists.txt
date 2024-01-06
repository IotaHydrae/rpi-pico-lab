cmake_minimum_required(VERSION 3.13)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(ili9486_spi)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project
add_executable(ili9486_spi ili9486_spi.c pio/pio_spi_tx.c)
pico_generate_pio_header(ili9486_spi ${CMAKE_CURRENT_LIST_DIR}/pio/pio_spi_tx.pio)
target_link_libraries(ili9486_spi pico_stdlib hardware_pwm hardware_pio hardware_dma
        pico_bootsel_via_double_reset)

target_include_directories(ili9486_spi PUBLIC .)
pico_enable_stdio_usb(ili9486_spi 0)
pico_enable_stdio_uart(ili9486_spi 1)

pico_add_extra_outputs(ili9486_spi)
