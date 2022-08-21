/**
 * @file port.c
 * @author IotaHydrae (writeforever@foxmail.com)
 * @brief file of the epink-1.54.
 * @version 0.2
 * @date 2022-08-21
 *
 * Hi, guys!
 *
 * This is a simple ALL-IN-ONE driver for the LuatOS epink-1.54 screen module.
 *
 * the device init function in this file was based on :
 * https://gitee.com/openLuat/LuatOS/blob/master/components/epaper/EPD_1in54.c
 *
 * More info about the epaper module can be found in :
 * https://wiki.luatos.com/peripherals/eink_1.54/index.html
 *
 * Special thanks to :
 *   LuatOS (https://gitee.com/openLuat/LuatOS)
 *
 * MIT License
 *
 * Copyright (c) 2022 LuatOS
 * Copyright (c) 2022 IotaHydrae(writeforever@foxmail.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/spi.h"
#include "port.h"

/* ========== epink pin controls ========== */
#ifdef PICO_DEFAULT_SPI_CSN_PIN
static inline void native_cs_select(struct native_config *cfg)
{
    asm volatile( "nop \n nop \n nop" );
    gpio_put( PICO_DEFAULT_SPI_CSN_PIN, 0 ); // Active low
    asm volatile( "nop \n nop \n nop" );
}

static inline void native_cs_deselect(struct native_config *cfg)
{
    asm volatile( "nop \n nop \n nop" );
    gpio_put( PICO_DEFAULT_SPI_CSN_PIN, 1 );
    asm volatile( "nop \n nop \n nop" );
}
#endif

static inline void native_dc_set(struct native_config *cfg)
{
    asm volatile( "nop \n nop \n nop" );
    gpio_put( EPINK_DC_PIN, 1 );
    asm volatile( "nop \n nop \n nop" );
}

static inline void native_dc_clr(struct native_config *cfg)
{
    asm volatile( "nop \n nop \n nop" );
    gpio_put( EPINK_DC_PIN, 0 );
    asm volatile( "nop \n nop \n nop" );
}

static inline void native_res_set(struct native_config *cfg)
{
    asm volatile( "nop \n nop \n nop" );
    gpio_put( EPINK_RES_PIN, 1 );
    asm volatile( "nop \n nop \n nop" );
}

static inline void native_res_clr(struct native_config *cfg)
{
    asm volatile( "nop \n nop \n nop" );
    gpio_put( EPINK_RES_PIN, 0 );
    asm volatile( "nop \n nop \n nop" );
}

// static void epink_reset()
// {
//     native_res_set();
//     sleep_ms( 200 );
    
//     native_res_clr();
//     sleep_ms( 2 );
    
//     native_res_set();
//     sleep_ms( 200 );
// }

/* ========== epink I/O ========== */

static inline void native_spi_write_byte(struct native_config *cfg, uint8_t val )
{
    uint8_t buf[1] = {val};
    cs_select();
    spi_write_blocking( spi_default, buf, 1 );
    cs_deselect();
}

/**
 * @brief hardware layer initialize 
 * for each platform, do it's iomux and pinctl here
 */
static void native_hal_init(struct native_config *cfg)
{
    stdio_init_all();

#if !defined(spi_default) || !defined(PICO_DEFAULT_SPI_SCK_PIN) || !defined(PICO_DEFAULT_SPI_TX_PIN) || !defined(PICO_DEFAULT_SPI_RX_PIN) || !defined(PICO_DEFAULT_SPI_CSN_PIN)
#warning spi/bme280_spi example requires a board with SPI pins
    puts( "Default SPI pins were not defined" );
#else
    
    /* Useing default SPI0 at 50MHz */
    spi_init( spi_default, 50 * 1000 * 1000 );
    gpio_set_function( PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI );
    gpio_set_function( PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI );
    bi_decl( bi_2pins_with_func( PICO_DEFAULT_SPI_TX_PIN, PICO_DEFAULT_SPI_SCK_PIN,
                                 GPIO_FUNC_SPI ) );

    gpio_init( PICO_DEFAULT_SPI_CSN_PIN );
    gpio_set_dir( PICO_DEFAULT_SPI_CSN_PIN, GPIO_OUT );
    gpio_put( PICO_DEFAULT_SPI_CSN_PIN, 1 );
    bi_decl( bi_1pin_with_name( PICO_DEFAULT_SPI_CSN_PIN, "SPI CS" ) );
#endif

    gpio_init( EPINK_RES_PIN );
    gpio_set_dir( EPINK_RES_PIN, GPIO_OUT );
    
    gpio_init( EPINK_DC_PIN );
    gpio_set_dir( EPINK_DC_PIN, GPIO_OUT );
    
    gpio_init( EPINK_BUSY_PIN );
    gpio_set_dir( EPINK_BUSY_PIN, GPIO_IN );

}

static struct native_config config_default = {
    .pin_res    = 14,
    .pin_dc     = 15,
    .pin_cs     = 17,
    .pin_busy   = 20,
    .spi_speed  = 50000000, /* 50MHz */
};

static struct native_interface iface_default = {
    .hal_init    = native_hal_init,
    .cs_select   = native_cs_select,
    .cs_deselect = native_cs_deselect,
    .dc_set      = native_dc_set,
    .dc_clr      = native_dc_clr,
    .res_set     = native_res_set,
    .res_clr     = native_dc_set,
};

// static struct native_driver platform_driver[] = {
//     [0] = {
//         .name   = "default",
//         .config = &platform_config,
//         .iface  = &platform_interface,
//     },
// };

register_platform_driver(default);