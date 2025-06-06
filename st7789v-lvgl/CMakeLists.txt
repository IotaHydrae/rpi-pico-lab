cmake_minimum_required(VERSION 3.13)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Wl,--print-memory-usage")
SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Wl,--print-memory-usage")

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(tft_lvgl)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

set(OVERCLOCK_ENABLED 1)    # 1: enable, 0: disable

if(OVERCLOCK_ENABLED)

    message(WARNING "Overclocking is enabled. This may damage your device. Use at own risk.")

    if(${PICO_BOARD} STREQUAL "pico" OR ${PICO_PLATFORM} STREQUAL "rp2040")
        # Overclocking profiles
        #      SYS_CLK  | FLASH_CLK | Voltage
        #  1  | 240MHz  |  120MHZ   |  1.10(V) (default, stable, recommended for most devices)
        #  2  | 266MHz  |  133MHz   |  1.10(V)
        #  3  | 360MHz  |  90MHz    |  1.20(V)
        #  4  | 400MHz  |  100MHz   |  1.30(V)
        #  5  | 416MHz  |  104MHz   |  1.30(V)
        set(OVERCLOCK_PROFILE 1)

        if(OVERCLOCK_PROFILE EQUAL 1)
            set(SYS_CLK_KHZ 240000)             # CPU clock speed
            set(PERI_CLK_KHZ ${SYS_CLK_KHZ})    # Peripheral clock speed
        elseif(OVERCLOCK_PROFILE EQUAL 2)
            set(SYS_CLK_KHZ 266000)
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
    elseif(${PICO_BOARD} STREQUAL "pico2" OR ${PICO_PLATFORM} STREQUAL "rp2350")
        # Overclocking profiles
        #      SYS_CLK  | FLASH_CLK | Voltage
        #  1  | 366MHz  |  122MHz   |  1.20(V)
        #  2  | 225000  |  75MHz    |  1.20(V)
        set(OVERCLOCK_PROFILE 2)

        if(OVERCLOCK_PROFILE EQUAL 1)
            set(SYS_CLK_KHZ 366000)             # CPU clock speed
            set(PERI_CLK_KHZ ${SYS_CLK_KHZ})    # Peripheral clock speed
        elseif(OVERCLOCK_PROFILE EQUAL 2)
            set(SYS_CLK_KHZ 225000)             # CPU clock speed
            set(PERI_CLK_KHZ ${SYS_CLK_KHZ})    # Peripheral clock speed
        else()
            message(FATAL_ERROR "Invalid overclocking profile")
        endif()
    endif()

else()  # OVERCLOCK_ENABLED
    message(WARNING "Overclocking is disabled.")

    if(${PICO_BOARD} STREQUAL "pico" OR ${PICO_PLATFORM} STREQUAL "rp2040")
        set(SYS_CLK_KHZ 125000) # CPU clock speed
        set(PERI_CLK_KHZ ${SYS_CLK_KHZ})    # Peripheral clock speed
    elseif(${PICO_BOARD} STREQUAL "pico2" OR ${PICO_PLATFORM} STREQUAL "rp2350")
        set(SYS_CLK_KHZ 150000) # CPU clock speed
        set(PERI_CLK_KHZ ${SYS_CLK_KHZ})    # Peripheral clock speed
    endif()

endif() # OVERCLOCK_ENABLED

# LCD Pins for SPI and PIO interface
set(TFT_SPIX      0)    # which spi controller will be used.
set(TFT_SCL_PIN   18)
set(TFT_SDA_PIN   19)
set(TFT_RES_PIN   15)
set(TFT_DC_PIN    14)
set(TFT_CS_PIN    13)
set(TFT_BLK_PIN   12)
set(TFT_HOR_RES   240)
set(TFT_VER_RES   240)
set(TFT_BUS_CLK_KHZ 75000)
set(DISP_OVER_PIO 1)  # 1: PIO, 0: SPI

# Display buffer size configuration
if(${PICO_BOARD} STREQUAL "pico" OR ${PICO_PLATFORM} STREQUAL "rp2040")
    math(EXPR MY_DISP_BUF_SIZE "${TFT_HOR_RES} * ${TFT_VER_RES} / 2")
elseif(${PICO_BOARD} STREQUAL "pico2" OR ${PICO_PLATFORM} STREQUAL "rp2350")
    math(EXPR MY_DISP_BUF_SIZE "${TFT_HOR_RES} * ${TFT_VER_RES}")
endif()

# file(GLOB_RECURSE INCLUDES "include/*.h" "port/*.h")
# file(GLOB_RECURSE SOURCES "lvgl/src/*.c" "lvgl/demos/*.c" "port/*.c")

include_directories(. include)

add_subdirectory(lvgl)
target_link_libraries(lvgl PUBLIC pico_stdlib)

add_subdirectory(pio)
target_compile_definitions(pio_spi_tx PUBLIC TFT_DC_PIN=${TFT_DC_PIN})
target_compile_definitions(pio_spi_tx PUBLIC TFT_CS_PIN=${TFT_CS_PIN})
target_compile_definitions(pio_spi_tx PUBLIC TFT_BUS_CLK_KHZ=${TFT_BUS_CLK_KHZ})
target_compile_definitions(pio_spi_tx PUBLIC DEFAULT_PIO_CLK_KHZ=${PERI_CLK_KHZ})

# user define common source files
file(GLOB_RECURSE COMMON_SOURCES
    main.c
    port/lv_port_disp.c
)

# rest of your project
# add_executable(${PROJECT_NAME} main.c ${INCLUDES} ${SOURCES})
add_executable(${PROJECT_NAME} ${COMMON_SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC .)
target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    hardware_spi
    hardware_pwm
    hardware_clocks
    pio_spi_tx
    pico_bootsel_via_double_reset
    lvgl lvgl::demos lvgl::examples
)
target_compile_definitions(${PROJECT_NAME} PUBLIC DEFAULT_SYS_CLK_KHZ=${SYS_CLK_KHZ})
target_compile_definitions(${PROJECT_NAME} PUBLIC DEFAULT_PERI_CLK_KHZ=${PERI_CLK_KHZ})
target_compile_definitions(${PROJECT_NAME} PUBLIC TFT_SCL_PIN=${TFT_SCL_PIN})
target_compile_definitions(${PROJECT_NAME} PUBLIC TFT_SDA_PIN=${TFT_SDA_PIN})
target_compile_definitions(${PROJECT_NAME} PUBLIC TFT_RES_PIN=${TFT_RES_PIN})
target_compile_definitions(${PROJECT_NAME} PUBLIC TFT_DC_PIN=${TFT_DC_PIN})
target_compile_definitions(${PROJECT_NAME} PUBLIC TFT_CS_PIN=${TFT_CS_PIN})
target_compile_definitions(${PROJECT_NAME} PUBLIC TFT_BLK_PIN=${TFT_BLK_PIN})
target_compile_definitions(${PROJECT_NAME} PUBLIC TFT_HOR_RES=${TFT_HOR_RES})
target_compile_definitions(${PROJECT_NAME} PUBLIC TFT_VER_RES=${TFT_VER_RES})
target_compile_definitions(${PROJECT_NAME} PUBLIC TFT_SPIX=${TFT_SPIX})
target_compile_definitions(${PROJECT_NAME} PUBLIC TFT_BUS_CLK_KHZ=${TFT_BUS_CLK_KHZ})
target_compile_definitions(${PROJECT_NAME} PUBLIC DISP_OVER_PIO=${DISP_OVER_PIO})
target_compile_definitions(${PROJECT_NAME} PUBLIC MY_DISP_BUF_SIZE=${MY_DISP_BUF_SIZE})

# Note: If you are using a NOR flash like "w25q16". Just keep the following content.
# The maximum speed of "w25q16" is 133MHz, However, the clock speed of XIP QSPI is divided from "sys_clk".
# So, when your "sys_clk" is greater than 266MHz and default PICO_FLASH_SPI_CLKDIV=2, It will exceed the
# maximum speed, because PICO_FLASH_SPI_CLKDIV must be even, So 4 is good for most purpose, Otherwise,
# nothing should be done. These things will only useful when you overclocking the rp2040.
if(${PICO_BOARD} STREQUAL "pico" OR ${PICO_PLATFORM} STREQUAL "rp2040")
    if(${SYS_CLK_KHZ} GREATER 266000)
        set(PICO_FLASH_SPI_CLKDIV 4)
    else()
        set(PICO_FLASH_SPI_CLKDIV 2)
    endif()
elseif(${PICO_BOARD} STREQUAL  "pico2" OR ${PICO_PLATFORM} STREQUAL  "rp2350")
    set(PICO_FLASH_SPI_CLKDIV 3)
endif()

math(EXPR FLASH_CLK_KHZ "${SYS_CLK_KHZ} / ${PICO_FLASH_SPI_CLKDIV}")
math(EXPR FLASH_CLK_MHZ "${FLASH_CLK_KHZ} / 1000")
math(EXPR SYS_CLK_MHZ "${SYS_CLK_KHZ} / 1000")
math(EXPR DISP_BUF_SIZE "${MY_DISP_BUF_SIZE} * 2") # rgb565 cost 2 bytes
message(WARNING "
    CPU speed   : ${SYS_CLK_MHZ} MHz
    Flash speed : ${FLASH_CLK_MHZ} MHz
    LCD_X       : ${TFT_HOR_RES}
    LCD_Y       : ${TFT_VER_RES}
    LCD rotation: ${LCD_ROTATION}   //0: normal, 1: 90 degree, 2: 180 degree, 3: 270 degree
    Buffer size : ${DISP_BUF_SIZE} bytes (LVGL Draw Buffer)
")
target_compile_definitions(bs2_default PRIVATE PICO_FLASH_SPI_CLKDIV=${PICO_FLASH_SPI_CLKDIV})
target_compile_definitions(${PROJECT_NAME} PRIVATE FLASH_CLK_KHZ=${FLASH_CLK_KHZ})

pico_enable_stdio_usb(${PROJECT_NAME} 0)
pico_enable_stdio_uart(${PROJECT_NAME} 1)

pico_add_extra_outputs(${PROJECT_NAME})
