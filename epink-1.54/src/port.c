#include "epink.h"


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
    uint8_t buf[1] = {val};
    cs_select();
    spi_write_blocking( spi_default, buf, 1 );
    cs_deselect();
}

/**
 * @brief hardware layer initialize 
 * for each platform, do it's iomux and pinctl here
 */
static void hal_init(void)
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
    
    gpio_init( EPINK_RES_PIN );
    gpio_set_dir( EPINK_RES_PIN, GPIO_OUT );
    
    gpio_init( EPINK_DC_PIN );
    gpio_set_dir( EPINK_DC_PIN, GPIO_OUT );
    
    gpio_init( EPINK_BUSY_PIN );
    gpio_set_dir( EPINK_BUSY_PIN, GPIO_IN );
}
