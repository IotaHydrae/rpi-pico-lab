cmake_minimum_required(VERSION 3.13)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(r61505w_gpio)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project
# if (DISP_OVER_PIO)
#     add_executable(r61505w_gpio r61505w_pio.c pio/i80.c)
#     pico_generate_pio_header(r61505w_gpio ${CMAKE_CURRENT_LIST_DIR}/pio/i80.pio)
#     target_link_libraries(r61505w_gpio pico_stdlib hardware_pwm hardware_pio hardware_dma)
# else(DISP_OVER_PIO)
add_executable(r61505w_gpio r61505w_gpio.c)
target_link_libraries(r61505w_gpio pico_stdlib hardware_pwm)
# endif(DISP_OVER_PIO)

target_include_directories(r61505w_gpio PUBLIC .)
pico_enable_stdio_usb(r61505w_gpio 0)
pico_enable_stdio_uart(r61505w_gpio 1)

pico_add_extra_outputs(r61505w_gpio)

# add a firmware flash target
if(${PICO_BOARD} STREQUAL "pico" OR ${PICO_PLATFORM} STREQUAL "rp2040")
    add_custom_target(
            flash
            COMMAND openocd -f interface/cmsis-dap.cfg -c "adapter speed 10000"
                            -f target/rp2040.cfg -s tcl -c "program ${CMAKE_PROJECT_NAME}.elf verify reset exit"
            DEPENDS ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.elf
            COMMENT "Flashing firmware using CMSIS-DAP Debugger..."
            DEPENDS ${PROJECT_NAME}
        )
elseif(${PICO_BOARD} STREQUAL  "pico2" OR ${PICO_PLATFORM} STREQUAL  "rp2350")
    add_custom_target(
            flash
            COMMAND openocd -f interface/cmsis-dap.cfg -c "adapter speed 10000"
                            -f target/rp2350.cfg -s tcl -c "program ${CMAKE_PROJECT_NAME}.elf verify reset exit"
            DEPENDS ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.elf
            COMMENT "Flashing firmware using CMSIS-DAP Debugger..."
            DEPENDS ${PROJECT_NAME}
        )
endif()
