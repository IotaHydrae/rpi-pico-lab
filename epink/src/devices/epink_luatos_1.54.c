#include "epink.h"

#include "pico/stdlib.h"

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

static uint8_t epink_disp_buffer[EPINK_DISP_BUFFER_SIZE];

extern unsigned char fontdata_mini_4x6[1536];
extern unsigned char fontdata_8x16[4096];

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
 * @brief really initialize function of epink, it
 * initialize the controller, display buffer etc.
 * 
 */
static void epink_init( struct epink_handler *handler )
{
    epink_device_init( handler->dev->cfg->update_mode );
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
static void epink_flush()
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
    
    epink_turn_on_display();
}

/**
 * @brief Simply clear the display buffer to 0xFF 
 */
static void epink_buffer_clear()
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
static void epink_draw_pixel( uint8_t x, uint8_t y, uint8_t color )
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

static struct epink_config config_epink_luatos_1_54 = {
    .width = 200,
    .height = 200,
    .bpp = 1,

    .update_mode = EPINK_UPDATE_MODE_PART,
    .font = EPINK_FONT_8x16,
};

static struct epink_operations opr_epink_luatos_1_54 = {

};

register_platform_device(epink_luatos_1_54);