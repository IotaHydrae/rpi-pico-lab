cmake_minimum_required(VERSION 3.13)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(d51e5ta7601_gpio)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

include_directories(PUBLIC .)
add_subdirectory(lvgl)

# rest of your project
if (DISP_OVER_PIO)
	add_executable(d51e5ta7601_gpio main.c
        d51e5ta7601_pio.c ns2009.c pcf8574.c i2c_tools.c keypad.c
        porting/lv_port_disp_template.c
		porting/lv_port_indev_template.c
		pio/i80.c)
	pico_generate_pio_header(d51e5ta7601_gpio ${CMAKE_CURRENT_LIST_DIR}/pio/i80.pio)
    target_link_libraries(d51e5ta7601_gpio lvgl lvgl_demos pico_stdlib
				hardware_pwm hardware_pio hardware_dma hardware_i2c)
else(DISP_OVER_PIO)
	add_executable(d51e5ta7601_gpio main.c
        d51e5ta7601_gpio.c ns2009.c pcf8574.c i2c_tools.c keypad.c
        porting/lv_port_disp_template.c
		porting/lv_port_indev_template.c)
	target_link_libraries(d51e5ta7601_gpio lvgl lvgl_demos pico_stdlib
				hardware_pwm hardware_i2c)
endif(DISP_OVER_PIO)

target_include_directories(d51e5ta7601_gpio PUBLIC . porting/)

pico_enable_stdio_usb(d51e5ta7601_gpio 0)
pico_enable_stdio_uart(d51e5ta7601_gpio 1)

pico_add_extra_outputs(d51e5ta7601_gpio)
