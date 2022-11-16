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

#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/spi.h"

#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"

#include "port/lv_port_disp.h"

#include "src/widgets/lv_label.h"
#include "ui/ui.h"

/* Pins Define of rpi-pico
 *
 * The SPI interface using default spi0
 *
 * RES  <->  GP14
 * DC   <->  GP15
 * BUSY <->  GP20
 */
#define EPINK_RES_PIN   15
#define EPINK_DC_PIN    14
#define EPINK_CS_PIN    13
#define EPINK_BUSY_PIN  12

/* ========== epink panel info ========== */
#define EPINK_WIDTH         200
#define EPINK_HEIGHT        200
#define EPINK_PAGE_SIZE     8
#define EPINK_LINE_WIDTH_IN_PAGE (EPINK_WIDTH/EPINK_PAGE_SIZE)
#define EPINK_BPP 1
#define EPINK_COLOR_WHITE (0xFF)
#define EPINK_COLOR_BLACK (0x00)

#define EPINK_UPDATE_MODE_FULL 1
#define EPINK_UPDATE_MODE_PART 2

#define EPINK_DISP_BUFFER_SIZE (EPINK_WIDTH*EPINK_HEIGHT/8)
#define EPINK_DISP_BUFFER_OFFSET(p,x)(p*EPINK_WIDTH + x)

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

// #define EPINK_DEBUG_MODE
#define EPINK_COORD_CHECK
#ifdef EPINK_DEBUG_MODE
    #define EPINK_DEBUG(...) printf(__VA_ARGS__)
#else
    #define EPINK_DEBUG
#endif

#define TEST_DOC "This document describes how to write an ALSA \
(Advanced Linux Sound Architecture) driver. The document focuses \
mainly on PCI soundcards. In the case of other device types, the \
API might be different, too. However, at least the ALSA kernel \
API is consistent, and therefore it would be still a bit help \
for writing them."

#define TEST_DOC2 "This variable provides a means of enabling or \
disabling features of a recipe on a per-recipe basis. PACKAGECONFIG \
blocks are defined in recipes when you specify features and then \
arguments that define feature behaviors."

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

uint8_t epink_disp_buffer[EPINK_DISP_BUFFER_SIZE];

extern unsigned char fontdata_mini_4x6[1536];
extern unsigned char fontdata_8x16[4096];

/* ========== epink pin controls ========== */
#ifdef EPINK_CS_PIN
static inline void cs_select()
{
    asm volatile( "nop \n nop \n nop" );
    gpio_put( EPINK_CS_PIN, 0 ); // Active low
    asm volatile( "nop \n nop \n nop" );
}

static inline void cs_deselect()
{
    asm volatile( "nop \n nop \n nop" );
    gpio_put( EPINK_CS_PIN, 1 );
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
    uint8_t buf[1] = {val};
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

/**
 * @brief Read the "busy" state pin to know if controller was busy,
 * but gives a timeout
 * 
 * @param timeout 
 */
static void epink_wait_busy_timeout( uint32_t timeout )
{
    while( gpio_get( EPINK_BUSY_PIN ) ) {
        if( timeout-- == 0 ) {
            EPINK_DEBUG( "epink_wait_busy timeout\n" );
            break;
        }
        else {
            sleep_ms( 100 );
        }
    }
    
    EPINK_DEBUG( "epink_wait_busy_timeout ok\n" );
}

/**
 * @brief Read the "busy" state pin to know if controller was busy
 * 
 */
static void epink_wait_busy()
{
    uint32_t timeout = 100;
    
    while( gpio_get( EPINK_BUSY_PIN ) ) {
        if( timeout-- == 0 ) {
            EPINK_DEBUG( "epink_wait_busy timeout\n" );
            break;
        }
        else {
            sleep_ms( 100 );
        }
    }
    
    EPINK_DEBUG( "epink_wait_busy ok\n" );
}

/* ========== epink operations ========== */
/**
 * @brief Set a limited drawing area of controller
 * 
 * @param x1 X start of area
 * @param y1 Y start of area
 * @param x2 X end of area
 * @param y2 Y end of area
 */
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

/**
 * @brief Set the drawing position of controller
 * 
 * @param x 
 * @param y 
 */
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

/**
 * @brief Turn on display, and sync the data in GDDRAM to screen
 * 
 */
static void epink_turn_on_display()
{
    epink_write_command( 0x22 ); // display update control 2
    epink_write_data( 0xC4 );
    epink_write_command( 0x20 ); // master activation
    epink_write_command( 0xFF ); // terminate frame read write
    
    epink_wait_busy();
}

static void epink_device_init( uint8_t mode )
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
    epink_write_data( 0xA8 );  // VCOM 7C
    
    epink_write_command( 0x3A ); // set dummy line period
    epink_write_data( 0x1A );  // 4 dummy lines per gate
    
    epink_write_command( 0x3B ); // set gate line width
    epink_write_data( 0x08 );  // 2us per line
    
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
        EPINK_DEBUG( "epink_init: unknown update mode\n" );
    }
}

/**
 * @brief Real initialize function of epink, it
 * initialize the controller and display buffer
 * 
 * @param mode update mode of epink panel
 */
static void epink_init( uint8_t mode )
{
    epink_device_init( mode );
    memset( epink_disp_buffer, 0xFF, ARRAY_SIZE( epink_disp_buffer ) );
}

/**
 * @brief Directly clear the GDDRAM in controller
 * 
 * @param color 0xFF means white, 0x00 means black
 */
static void epink_clear( uint8_t color )
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
            epink_write_data( color );
        }
    }
    
    epink_turn_on_display();
}

/**
 * @brief Makes controller into sleep mode
 * 
 */
static void epink_sleep()
{
    epink_write_command( 0x10 );
    epink_write_data( 0x01 );
}

/* ========== epink drawing functions ========== */
static void __make_random_dram_data()
{
    for( int i = 0; i < ARRAY_SIZE( epink_disp_buffer ); i++ ) {
        epink_disp_buffer[i] = 0x49;
    }
}

/**
 * @brief Flush each byte in display buffer to screen
 * 
 */
void epink_flush()
{
    uint8_t *pen = epink_disp_buffer;
    uint8_t width, height;
    width = ( EPINK_WIDTH % 8 == 0 ) ? ( EPINK_WIDTH / 8 ) :
            ( EPINK_WIDTH / 8 + 1 );
    height = EPINK_HEIGHT;
    
    epink_set_window( 0, 0, EPINK_WIDTH, EPINK_HEIGHT );
    
    for( uint8_t i = 0; i < height; i++ ) {
        epink_set_cursor( 0, i );
        epink_write_command( 0x24 );
        
        /* flush each line in buffer */
        for( uint8_t j = 0; j < width; j++ ) {
            epink_write_data( pen[j + i * 25] );
        }
    }
    
    // sleep_ms(100);
    epink_turn_on_display();
}

/**
 * @brief Simply clear the display buffer to 0xFF 
 */
void epink_buffer_clear()
{
    for( int i = 0; i < ARRAY_SIZE( epink_disp_buffer ); i++ ) {
        epink_disp_buffer[i] = 0xFF;
    }
}

/**
 * @brief draw a given position pixel to display buffer
 * 
 * @param x draw position of X
 * @param y draw position os Y
 * @param color 1 means black, 0 means white
 */
void epink_draw_pixel( uint8_t x, uint8_t y, uint8_t color )
{
    /* If we want to do a given pixel draw, the best
     * way might be draw it in a display buffer, because
     * most display controller like this "epink", usually
     * could enter a page write mode(check maunal of ssd1306),
     * It's really makes a speed up and reduces the
     * display buffer size we need to alloced.
     *
     * But the problems also goes on, In this mode, the eight pixel
     * bit data was conbined to a byte and write to controller directly
     * so we need to clac the given (x,y) in which page at display buffer
     * and use "|=", "&=" to operate the page then flush it to screen.
     *
     * Actually, the flush operation can be executed when you already
     * drawed all the pixel data to buffer
     *
     * These pages in display buffer looks like this :
     *      Y
     *    X ******** ******** ... ******** 25 page
     *      ******** ******** ... ********
     *         ...
     *      ******** ******** ... ********
     *      200 line
     *                                  5000 bytes 
     * So we did it like blow
     */
    uint8_t page, page_left;
    uint8_t *pen = epink_disp_buffer;
#ifdef EPINK_COORD_CHECK
    
    if( ( x >= 0 && x < EPINK_WIDTH ) && ( y >= 0 && y < EPINK_HEIGHT ) ) {
#endif
        /* How to get the page in ram? */
        /* 1. Calc the "X" in which page of line */
        page = x / 8;
        /* The page_left is the bit we need to set to page, will use later */
        page_left = ( x % 8 == 0 ) ? 0 : x % 8;
        EPINK_DEBUG( "page:%d, page_left:%d\n", page, page_left );
        
        /* 2. Get which line using "Y" */
        /* The number 25 means a line contains 25 page,
         * I should use a MARCO better, but in order to be
         * more intuitive and if we use "Y * 25 + page",
         * we can got the target page in display buffer,
         * then we just set the offset bit to 0 means draw
         * it into black.
         *
         * A little bit explanation of "1 << (7 - page_left)",
         * when we draw a page to screen, the lowest bit of
         * page corresponded the highest bit of byte in buffer.
         */
        if( color )
            pen[y * 25 + page] &= ~( 1 << ( 7 - page_left ) );
        else
            pen[y * 25 + page] |= ( 1 << ( 7 - page_left ) );
        
        EPINK_DEBUG( "set:%d, clear:%d\n", ( uint8_t )~( 1 << page_left ),
                     ( 1 << page_left ) );
        EPINK_DEBUG( "which:%d, dump:%d\n", y * 25 + page, pen[y * 25 + page] );
        
#ifdef EPINK_COORD_CHECK
    }
    
#endif
}

/**
 * @brief Put a single ascii char to panel
 *
 * @param x start position of X
 * @param y start position of Y
 * @param c char that will outputting to panel
 */
static void epink_putascii( uint8_t x, uint8_t y, char c )
{
    /* Get the char in ascii array, each char use 16 byte to store */
    const unsigned char *dots = ( uint8_t * )&fontdata_8x16[c * 16];
    uint8_t *pen = epink_disp_buffer;
    uint8_t row, col, byte;
    
    /* In this case, we use a 8x16 size font, so
     * we need to draw 16 byte totally, for each
     * byte, draw it's each bit from higher to low
     */
    for( row = 0; row < 16; row++ ) {
        byte = dots[row];
        
        for( col = 0; col < 8; col++ ) {
            epink_draw_pixel( x + col, y + row, ( byte << col ) & 0x80 );
        }
    }
}

/**
 * @brief Put ascii string to panel
 *
 * @param x start position of X
 * @param y start position of Y
 * @param str string that will outputting to the panel
 */
static void epink_putascii_string( uint8_t x, uint8_t y, char *str )
{
    while( *str != '\0' ) {
        epink_putascii( x, y, *str++ );
        x += 8; /* move x to the next pos */
        
        /* start a new line if reach the end of line */
        if( x >= EPINK_WIDTH ) {
            x = 0;
            y += 16; /* line hight min:16 */
        }
    }
}

/**
 * @brief hardware layer initialize 
 * for each platform, do it's iomux and pinctl here
 */
static void hal_init(void)
{
    stdio_init_all();

#if !defined(spi_default) || !defined(PICO_DEFAULT_SPI_SCK_PIN) || !defined(PICO_DEFAULT_SPI_TX_PIN) || !defined(PICO_DEFAULT_SPI_RX_PIN) || !defined(EPINK_CS_PIN)
#warning spi/bme280_spi example requires a board with SPI pins
    puts( "Default SPI pins were not defined" );
#else
    /* Useing default SPI0 at 50MHz */
    spi_init( spi_default, 50 * 1000 * 1000 );
    gpio_set_function( PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI );
    gpio_set_function( PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI );
    bi_decl( bi_2pins_with_func( PICO_DEFAULT_SPI_TX_PIN, PICO_DEFAULT_SPI_SCK_PIN,
                                 GPIO_FUNC_SPI ) );
    
    gpio_init( EPINK_CS_PIN );
    gpio_set_dir( EPINK_CS_PIN, GPIO_OUT );
    gpio_put( EPINK_CS_PIN, 1 );
    bi_decl( bi_1pin_with_name( EPINK_CS_PIN, "SPI CS" ) );
    
    gpio_init( EPINK_RES_PIN );
    gpio_set_dir( EPINK_RES_PIN, GPIO_OUT );
    
    gpio_init( EPINK_DC_PIN );
    gpio_set_dir( EPINK_DC_PIN, GPIO_OUT );
    
    gpio_init( EPINK_BUSY_PIN );
    gpio_set_dir( EPINK_BUSY_PIN, GPIO_IN );
#endif
}

static void lv_epink_update_cb(lv_timer_t * timer)
{
    LV_LOG_WARN("%s called", __func__);
    epink_flush();
}

static void anim_y_cb(void *var, int32_t v)
{
    lv_obj_set_y(var, v);
}

extern lv_obj_t *ui_RollerHour;
extern lv_obj_t *ui_RollerMinute;
extern lv_obj_t *ui_RollerHour;
extern lv_obj_t *ui_LabelTips;

static uint8_t hour = 0;
static uint8_t minute = 0;
static uint8_t second = 0;
static uint8_t tips_index = 0;

static void lv_timer_roller_time_cb()
{
    lv_roller_set_selected(ui_RollerSecond, ++second, LV_ANIM_OFF);
    
    if (second == 60) {
        second = 0;
        lv_roller_set_selected(ui_RollerSecond, second, LV_ANIM_OFF);
        lv_roller_set_selected(ui_RollerMinute, ++minute, LV_ANIM_OFF);
    }

    if (minute == 60) {
        minute = 0;
        lv_roller_set_selected(ui_RollerMinute, minute, LV_ANIM_OFF);
        lv_roller_set_selected(ui_RollerHour, ++hour, LV_ANIM_OFF);
    }

    if (hour == 24) {
        hour=0;
        lv_roller_set_selected(ui_RollerHour, hour, LV_ANIM_OFF);
    }

}

static const char *g_tips[] = {
    "Learn to pause.",
    "Don't Worry, Be Happy.",
    "Gah! YEEEEEEEE!",
    "Never laugh at live dragons.",
    "You are not dead yet.",
    "Cheers!",
};

static void lv_timer_label_tips_cb()
{
    lv_label_set_text(ui_LabelTips, g_tips[tips_index++]);
    if (tips_index > (sizeof(g_tips)/sizeof(g_tips[0]) - 1))
        tips_index = 0;
}

int main( void )
{
    stdio_init_all();

    hal_init();

    lv_init();
    lv_port_disp_init();

    printf("%s\n", __func__);

    epink_init( EPINK_UPDATE_MODE_FULL );

    /*  a global clear before drawing operations  */
    epink_clear( 0x00 );
    sleep_ms(200);
    epink_clear( 0xFF );
    sleep_ms(200);

    epink_init( EPINK_UPDATE_MODE_PART );

    epink_clear( 0x00 );
    sleep_ms(200);
    epink_clear( 0xFF );
    sleep_ms(200);
    
    // epink_putascii_string( 0, 0, TEST_DOC );
    // epink_flush();
    // sleep_ms( 500 );

    // epink_clear( 0x00 );
    // sleep_ms(200);
    // epink_clear( 0xFF );
    // sleep_ms(200);

    // lv_demo_widgets();
    // lv_demo_stress();
    // lv_demo_music();
    ui_init();

    lv_timer_t *timer_roller = lv_timer_create_basic();
    timer_roller->timer_cb = lv_timer_roller_time_cb;
    timer_roller->period = 1000;
    
    lv_label_set_long_mode(ui_LabelTips, LV_LABEL_LONG_WRAP);
    
    lv_timer_t *timer_tips = lv_timer_create_basic();
    timer_tips->timer_cb = lv_timer_label_tips_cb;
    timer_tips->period = 5000;

    // lv_obj_t *btn = lv_btn_create(lv_scr_act());
    // lv_obj_set_style_bg_color(btn, lv_color_hex(0x0), 0);
    // lv_obj_set_style_radius(btn, 10, 0);
    // lv_obj_set_style_border_width(btn, 3, 0);
    // lv_obj_center(btn);

    // LV_FONT_DECLARE(lv_font_montserrat_22);
    // lv_obj_t *label = lv_label_create(btn);
    // lv_label_set_text(label, "embeddedboys");
    // lv_obj_set_style_text_font(label, &lv_font_montserrat_22, 0);

    // lv_obj_align(btn, LV_ALIGN_TOP_MID, 0, 0);
    // lv_anim_t a;
    // lv_anim_init(&a);

    // lv_anim_set_var(&a, btn);
    // lv_anim_set_values(&a, lv_obj_get_y(btn), 150);
    // lv_anim_set_time(&a, 1000);
    // lv_anim_set_exec_cb(&a, anim_y_cb);
    // lv_anim_set_path_cb(&a, lv_anim_path_bounce);
    // lv_anim_start(&a);

    while( 1 ) {
        sleep_us(5*1000);
        lv_timer_handler();
        lv_tick_inc(5);
    }
    
    return 0;

}
