/**
 * @file main.c
 * @author IotaHydrae (writeforever@foxmail.com)
 * @brief main file of the epink-1.54.
 * @version 0.1
 * @date 2022-08-07
 *
 * Hi, guys!
 *
 * This is a simple driver for the epink-1.54.
 *
 * This file is based on :
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
#include <string.h>

#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/spi.h"

/*
 * pins define
 *
 * use default spi0
 *
 * RES - GP14
 * DC  - GP15
 * BUSY - GP20
 */
#define EPINK_RES_PIN       14
#define EPINK_DC_PIN        15
#define EPINK_BUSY_PIN      20

/* ========== epink panel info ========== */
#define EPINK_WIDTH         200
#define EPINK_HEIGHT        200
#define EPINK_BPP           1

#define EPINK_UPDATE_MODE_FULL 1
#define EPINK_UPDATE_MODE_PART 2

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

static const unsigned char EPD_1IN54_lut_full_update[] = {
        0x02, 0x02, 0x01, 0x11, 0x12, 0x12, 0x22, 0x22,
        0x66, 0x69, 0x69, 0x59, 0x58, 0x99, 0x99, 0x88,
        0x00, 0x00, 0x00, 0x00, 0xF8, 0xB4, 0x13, 0x51,
        0x35, 0x51, 0x51, 0x19, 0x01, 0x00
};

static const unsigned char EPD_1IN54_lut_partial_update[] = {
        0x10, 0x18, 0x18, 0x08, 0x18, 0x18, 0x08, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x13, 0x14, 0x44, 0x12,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


/* ========== epink pin controls ========== */
#ifdef PICO_DEFAULT_SPI_CSN_PIN
static inline void cs_select()
{
        asm volatile( "nop \n nop \n nop" );
        gpio_put( PICO_DEFAULT_SPI_CSN_PIN, 0 ); // Active low
        asm volatile( "nop \n nop \n nop" );
}

static inline void cs_deselect()
{
        asm volatile( "nop \n nop \n nop" );
        gpio_put( PICO_DEFAULT_SPI_CSN_PIN, 1 );
        asm volatile( "nop \n nop \n nop" );
}
#endif

static inline void epink_dc_set()
{
        asm volatile( "nop \n nop \n nop" );
        gpio_put( EPINK_DC_PIN, 1 );
        asm volatile( "nop \n nop \n nop" );
}

static inline void epink_dc_clr()
{
        asm volatile( "nop \n nop \n nop" );
        gpio_put( EPINK_DC_PIN, 0 );
        asm volatile( "nop \n nop \n nop" );
}

static inline void epink_res_set()
{
        asm volatile( "nop \n nop \n nop" );
        gpio_put( EPINK_RES_PIN, 1 );
        asm volatile( "nop \n nop \n nop" );
}

static inline void epink_res_clr()
{
        asm volatile( "nop \n nop \n nop" );
        gpio_put( EPINK_RES_PIN, 0 );
        asm volatile( "nop \n nop \n nop" );
}

static void epink_reset()
{
        epink_res_set();
        sleep_ms( 200 );
        
        epink_res_clr();
        sleep_ms( 2 );
        
        epink_res_set();
        sleep_ms( 200 );
}

/* ========== epink I/O ========== */

static inline void epink_write_byte( uint8_t val )
{
        uint8_t buf[1] = { val };
        cs_select();
        spi_write_blocking( spi_default, buf, 1 );
        cs_deselect();
}

static void epink_write_command( uint8_t command )
{
        epink_dc_clr();
        epink_write_byte( command );
}

static void epink_write_data( uint8_t data )
{
        epink_dc_set();
        epink_write_byte( data );
}

static void epink_wait_busy_timeout( uint32_t timeout )
{
        while( gpio_get( EPINK_BUSY_PIN ) ) {
                if( timeout-- == 0 ) {
                        printf( "epink_wait_busy timeout\n" );
                        break;
                }
                else {
                        sleep_ms( 100 );
                }
        }
        
        printf( "epink_wait_busy_timeout ok\n" );
}

static void epink_wait_busy()
{
        uint32_t timeout = 100;
        
        while( gpio_get( EPINK_BUSY_PIN ) ) {
                if( timeout-- == 0 ) {
                        printf( "epink_wait_busy timeout\n" );
                        break;
                }
                else {
                        sleep_ms( 100 );
                }
        }
        
        printf( "epink_wait_busy ok\n" );
}

/* ========== epink operations ========== */
static void epink_set_window( uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2 )
{
        /* set start/end position address of x in ram */
        epink_write_command( 0x44 );
        epink_write_data( ( x1 >> 3 ) & 0xFF );
        epink_write_data( ( x2 >> 3 ) & 0xFF );
        
        /* set start/end position address of y in ram */
        epink_write_command( 0x45 );
        epink_write_data( y1 && 0xFF );
        epink_write_data( ( y1 >> 8 ) & 0xFF );
        epink_write_data( y2 && 0xFF );
        epink_write_data( ( y2 >> 8 ) & 0xFF );
}

static void epink_set_cursor( uint8_t x, uint8_t y )
{
        /* set address counter of x in ram */
        epink_write_command( 0x4E );
        epink_write_data( ( x >> 3 ) & 0xFF );
        
        
        /* set address counter of y in ram */
        epink_write_command( 0x4F );
        epink_write_data( y & 0xFF );
        epink_write_data( ( y >> 8 ) & 0xFF );
}

static void epink_turn_on_display()
{
        epink_write_command( 0x22 );    // display update control 2
        epink_write_data( 0xC4 );
        epink_write_command( 0x20 );    // master activation
        epink_write_command( 0xFF );    // terminate frame read write
        
        epink_wait_busy();
}

static void epink_init( uint8_t mode )
{
        epink_reset();
        
        epink_write_command( 0x01 ); // driver output control
        epink_write_data( ( EPINK_HEIGHT - 1 ) & 0xFF );
        epink_write_data( ( ( EPINK_HEIGHT - 1 ) >> 8 ) & 0xFF );
        epink_write_data( 0x00 ); // GD = 0; SM = 0; TB = 0;
        
        epink_write_command( 0x0C ); // booster soft start
        epink_write_data( 0xD7 );
        epink_write_data( 0xD6 );
        epink_write_data( 0x9D );
        
        epink_write_command( 0x2C ); // write vcom register
        epink_write_data( 0xA8 ); // VCOM 7C
        
        epink_write_command( 0x3A ); // set dummy line period
        epink_write_data( 0x1A ); // 4 dummy lines per gate
        
        epink_write_command( 0x3B ); // set gate line width
        epink_write_data( 0x08 ); // 2us per line
        
        epink_write_command( 0x11 );
        epink_write_data( 0x03 );
        
        /* set the look-up table register */
        epink_write_command( 0x32 );
        
        if( mode == EPINK_UPDATE_MODE_FULL )
                for( uint8_t i = 0; i < ARRAY_SIZE( EPD_1IN54_lut_full_update ); i++ ) {
                        epink_write_data( EPD_1IN54_lut_full_update[i] );
                }
        else if( mode == EPINK_UPDATE_MODE_PART )
                for( uint8_t i = 0; i < ARRAY_SIZE( EPD_1IN54_lut_partial_update ); i++ ) {
                        epink_write_data( EPD_1IN54_lut_partial_update[i] );
                }
        else {
                printf( "epink_init: unknown update mode\n" );
        }
}

static void epink_clear()
{
        uint8_t width, height;
        width = ( EPINK_WIDTH % 8 == 0 ) ? ( EPINK_WIDTH / 8 ) :
                ( EPINK_WIDTH / 8 + 1 );
        height = EPINK_HEIGHT;
        
        epink_set_window( 0, 0, EPINK_WIDTH, EPINK_HEIGHT );
        
        for( uint16_t i = 0; i < height; i++ ) {
                epink_set_cursor( 0, i );
                epink_write_command( 0x24 );
                
                for( uint16_t j = 0; j < width; j++ ) {
                        epink_write_data( 0xFF );
                }
        }
        
        epink_turn_on_display();
}

static void epink_sleep()
{
        epink_write_command( 0x10 );
        epink_write_data( 0x01 );
}

int main( void )
{
        stdio_init_all();
#if !defined(spi_default) || !defined(PICO_DEFAULT_SPI_SCK_PIN) || !defined(PICO_DEFAULT_SPI_TX_PIN) || !defined(PICO_DEFAULT_SPI_RX_PIN) || !defined(PICO_DEFAULT_SPI_CSN_PIN)
#warning spi/bme280_spi example requires a board with SPI pins
        puts( "Default SPI pins were not defined" );
#else
        
        /* useing SPI0 at 10MHz */
        spi_init( spi_default, 10 * 1000 * 1000 );
        // spi_init( spi_default, 100 * 1000 );
        gpio_set_function( PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI );
        gpio_set_function( PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI );
        bi_decl( bi_2pins_with_func( PICO_DEFAULT_SPI_TX_PIN, PICO_DEFAULT_SPI_SCK_PIN,
                                     GPIO_FUNC_SPI ) );
        
        gpio_init( PICO_DEFAULT_SPI_CSN_PIN );
        gpio_set_dir( PICO_DEFAULT_SPI_CSN_PIN, GPIO_OUT );
        gpio_put( PICO_DEFAULT_SPI_CSN_PIN, 1 );
        bi_decl( bi_1pin_with_name( PICO_DEFAULT_SPI_CSN_PIN, "SPI CS" ) );
        
        // gpio_init( EPINK_RES_PIN | EPINK_DC_PIN | EPINK_BUSY_PIN );
        // gpio_set_dir( EPINK_RES_PIN | EPINK_DC_PIN | EPINK_BUSY_PIN, GPIO_OUT );
        
        gpio_init( EPINK_RES_PIN );
        gpio_set_dir( EPINK_RES_PIN, GPIO_OUT );
        
        gpio_init( EPINK_DC_PIN );
        gpio_set_dir( EPINK_DC_PIN, GPIO_OUT );
        
        gpio_init( EPINK_BUSY_PIN );
        gpio_set_dir( EPINK_BUSY_PIN, GPIO_IN );
        
        epink_init( EPINK_UPDATE_MODE_FULL );
        
        
        while( 1 ) {
                epink_clear();
                sleep_ms( 1000 );
        }
        
        return 0;
#endif
}