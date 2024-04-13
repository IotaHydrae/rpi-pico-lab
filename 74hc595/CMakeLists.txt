cmake_minimum_required(VERSION 3.13)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(OVERCLOCK_ENABLED 0)    # 1: enable, 0: disable

if(OVERCLOCK_ENABLED)
    message(WARNING "Overclocking is enabled. This may damage your device. Use at own risk.")
    # Overclocking profiles
    #      SYS_CLK  | FLASH_CLK | Voltage
    #  1  | 266MHz  |  133MHz   |  1.10(V) (default, stable, recommended for most devices)
    #  2  | 240MHz  |  120MHZ   |  1.10(V) (more stable)
    #  3  | 360MHz  |  90MHz    |  1.20(V)
    #  4  | 400MHz  |  100MHz   |  1.30(V)
    #  5  | 416MHz  |  104MHz   |  1.30(V)
    set(OVERCLOCK_PROFILE 1)

    if(OVERCLOCK_PROFILE EQUAL 1)
        set(SYS_CLK_KHZ 266000)             # CPU clock speed
        set(PERI_CLK_KHZ ${SYS_CLK_KHZ})    # Peripheral clock speed
    elseif(OVERCLOCK_PROFILE EQUAL 2)
        set(SYS_CLK_KHZ 240000)
        set(PERI_CLK_KHZ ${SYS_CLK_KHZ})
    elseif(OVERCLOCK_PROFILE EQUAL 3)
        set(SYS_CLK_KHZ 360000)
        set(PERI_CLK_KHZ ${SYS_CLK_KHZ})
    elseif(OVERCLOCK_PROFILE EQUAL 4)
        set(SYS_CLK_KHZ 400000)
        set(PERI_CLK_KHZ ${SYS_CLK_KHZ})
    elseif(OVERCLOCK_PROFILE EQUAL 5)
        set(SYS_CLK_KHZ 416000)
        set(PERI_CLK_KHZ ${SYS_CLK_KHZ})
    else()
        message(FATAL_ERROR "Invalid overclocking profile")
    endif()
else()
    message(WARNING "Overclocking is disabled.")
    set(SYS_CLK_KHZ 140000) # CPU clock speed
    set(PERI_CLK_KHZ ${SYS_CLK_KHZ})    # Peripheral clock speed
endif()

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(blink)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project
add_executable(blink main.c tft_r61581.c)

target_link_libraries(blink pico_stdlib)

target_compile_definitions(${PROJECT_NAME} PUBLIC DEFAULT_SYS_CLK_KHZ=${SYS_CLK_KHZ})
target_compile_definitions(${PROJECT_NAME} PUBLIC DEFAULT_PERI_CLK_KHZ=${PERI_CLK_KHZ})

if(${SYS_CLK_KHZ} GREATER 266000)
    set(PICO_FLASH_SPI_CLKDIV 4)
else()
    set(PICO_FLASH_SPI_CLKDIV 2)
endif()
math(EXPR FLASH_CLK_KHZ "${SYS_CLK_KHZ} / ${PICO_FLASH_SPI_CLKDIV}")
target_compile_definitions(bs2_default PRIVATE PICO_FLASH_SPI_CLKDIV=${PICO_FLASH_SPI_CLKDIV})
target_compile_definitions(${PROJECT_NAME} PRIVATE FLASH_CLK_KHZ=${FLASH_CLK_KHZ})


pico_add_extra_outputs(blink)
