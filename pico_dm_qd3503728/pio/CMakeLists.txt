# Copyright (c) 2024 embeddedboys developers

# Permission is hereby granted, free of charge, to any person obtaining
# a copy of this software and associated documentation files (the
# "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so, subject to
# the following conditions:

# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
# LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
# OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
# WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

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