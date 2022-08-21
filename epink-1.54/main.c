/**
 * @file main.c
 * @author IotaHydrae (writeforever@foxmail.com)
 * @brief file of the epink-1.54.
 * @version 0.2
 * @date 2022-08-14
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
#include <string.h>

#include "epink.h"

/* Pins Define of rpi-pico
 *
 * The SPI interface using default spi0
 *
 * RES  <->  GP14
 * DC   <->  GP15
 * BUSY <->  GP20
 */

int main( void )
{
    
    /*  a global clear before drawing operations  */
    epink_clear( 0x00 );
    sleep_ms(500);
    epink_clear( 0xFF );
    sleep_ms(500);

    // sleep_ms(200);
    while( 1 ) {
        // epink_clear(0xFF);
        // epink_turn_on_display();
        // sleep_ms(200);
        // epink_buffer_clear();
        for( uint8_t x = 0, y = 0; x < 200; x++, y++ ) {
            // EPINK_DEBUG("x:%d, y:%d\n", x, y);
            // epink_draw_pixel( x-1, y-1, 1);
            epink_draw_pixel( x, y, 1 );
            epink_draw_pixel( x + 1, y, 1 );
            epink_draw_pixel( x + 2, y, 1 );
            epink_draw_pixel( x + 3, y, 1 );
            epink_draw_pixel( x + 4, y, 1 );
        }
    
        for( uint8_t x = 200, y = 0; x > 0; x--, y++ ) {
            // EPINK_DEBUG("x:%d, y:%d\n", x, y);
            // epink_draw_pixel( x-1, y-1, 1);
            epink_draw_pixel( x, y, 1 );
            epink_draw_pixel( x + 1, y, 1 );
            epink_draw_pixel( x + 2, y, 1 );
            epink_draw_pixel( x + 3, y, 1 );
            epink_draw_pixel( x + 4, y, 1 );
        }
        epink_flush();
        // sleep_ms( 200 );
        // epink_putascii(50,50,'A');
        // epink_buffer_clear();
    
        // for( int y = 0; y < 200; y += 16 ) {
        //     epink_putascii_string( 0, y, "Hello, world!" );
        // }
    
        // epink_flush();
        // sleep_ms( 200 );
    
        // epink_buffer_clear();
    
        // for( int y = 0; y < 200; y += 16 ) {
        //     epink_putascii_string( 50, y, "Hello, world!" );
        // }
    
        // epink_flush();
        // sleep_ms( 200 );
    
        // epink_buffer_clear();
    
        // for( int y = 0; y < 200; y += 16 ) {
        //     epink_putascii_string( 100, y, "Hello, world!" );
        // }
    
        // epink_flush();
        // sleep_ms( 200 );
        epink_putascii_string( 0, 0, TEST_DOC );
        epink_flush();
        sleep_ms( 500 );
    }
    
    return 0;
}
