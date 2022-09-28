/**
 * @file main.c
 * @author IotaHydrae (writeforever@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2022-09-28
 *
 * MIT License
 *
 * Copyright 2022 IotaHydrae(writeforever@foxmail.com)
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
#include "hardware/pwm.h"

#include "st7735s.h"

#define ST7735S_RES_PIN   15
#define ST7735S_DC_PIN    14
#define ST7735S_CS_PIN    13
#define ST7735S_BLK_PIN   12

/* ========== st7735s pin controls ========== */
#ifdef PICO_DEFAULT_SPI_CSN_PIN
static inline void cs_select()
{
    asm volatile( "nop \n nop \n nop" );
    gpio_put( ST7735S_CS_PIN, 0 ); // Active low
    asm volatile( "nop \n nop \n nop" );
}

static inline void cs_deselect()
{
    asm volatile( "nop \n nop \n nop" );
    gpio_put( ST7735S_CS_PIN, 1 );
    asm volatile( "nop \n nop \n nop" );
}
#endif

static inline void st7735s_dc_set()
{
    asm volatile( "nop \n nop \n nop" );
    gpio_put( ST7735S_DC_PIN, 1 );
    asm volatile( "nop \n nop \n nop" );
}

static inline void st7735s_dc_clr()
{
    asm volatile( "nop \n nop \n nop" );
    gpio_put( ST7735S_DC_PIN, 0 );
    asm volatile( "nop \n nop \n nop" );
}

static inline void st7735s_res_set()
{
    asm volatile( "nop \n nop \n nop" );
    gpio_put( ST7735S_RES_PIN, 1 );
    asm volatile( "nop \n nop \n nop" );
}

static inline void st7735s_res_clr()
{
    asm volatile( "nop \n nop \n nop" );
    gpio_put( ST7735S_RES_PIN, 0 );
    asm volatile( "nop \n nop \n nop" );
}

static void st7735s_reset()
{
    st7735s_res_set();
    sleep_ms( 10 );

    st7735s_res_clr();
    sleep_ms( 10 );

    st7735s_res_set();
    sleep_ms( 10 );
}

/* ========== st7735s I/O ========== */

static inline void st7735s_write_byte( uint8_t val )
{
    uint8_t buf[1] = {val};
    cs_select();
    spi_write_blocking( spi_default, buf, 1 );
    cs_deselect();
}

static void st7735s_write_command( uint8_t command )
{
    st7735s_dc_clr();
    st7735s_write_byte( command );
}

static void st7735s_write_data( uint8_t data )
{
    st7735s_dc_set();
    st7735s_write_byte( data );
}

static void st7735s_device_init(void)
{
	st7735s_reset();
	sleep_ms(50);

	st7735s_write_command(SLPOUT);	// Sleep out
	sleep_ms(120);

	/* Memory Data Access Control */
	st7735s_write_command(MADCTL);
	// st7735s_write_data(1 << 6); //X-Mirror
	// st7735s_write_data(2 << 6); //Y-Mirror
	// st7735s_write_data(0xA << 4);
	st7735s_write_data(0);

	/* Interface Pixel Format */
	st7735s_write_command(COLMOD);
	st7735s_write_data(0x05);   // RGB565

	/* Colume address set */
	// st7735s_write_command(CASET);
	// st7735s_write_data(0x00);
	// st7735s_write_data(0x00);
	// st7735s_write_data(0x00);
	// st7735s_write_data(0x9F);

	/* Row address set */
	// st7735s_write_command(RASET);
	// st7735s_write_data(0x00);
	// st7735s_write_data(0x00);
	// st7735s_write_data(0x00);
	// st7735s_write_data(0x9F);

	st7735s_write_command(INVON); // Display inversion on
	st7735s_write_command(DISPON); // Display on
}

static void st7735s_set_cursor(uint32_t x, uint32_t y)
{
	st7735s_write_command(CASET);
	st7735s_write_data(0x00);
	st7735s_write_data((x & 0xFF));
	st7735s_write_data(0x00);
	st7735s_write_data(0x9F);

	st7735s_write_command(RASET);
	st7735s_write_data(0x00);
	st7735s_write_data((y & 0xFF));
	st7735s_write_data(0x00);
	st7735s_write_data(0x7F);
}

void st7735s_draw_pixel(uint32_t x, uint32_t y, uint16_t color)
{
    int invert_x = y;
    int invert_y = x;
	// st7735s_set_cursor(invert_x, invert_y);
	st7735s_set_cursor(x,y);

	st7735s_write_command(RAMWR);
	st7735s_write_data(color >> 8);
	st7735s_write_data(color & 0xFF);
}

void on_pwm_wrap() {
    static int fade = 0;
    static bool going_up = true;
    // Clear the interrupt flag that brought us here
    pwm_clear_irq(pwm_gpio_to_slice_num(ST7735S_BLK_PIN));

    if (going_up) {
        ++fade;
        if (fade > 255) {
            fade = 255;
            going_up = false;
        }
    } else {
        --fade;
        if (fade < 0) {
            fade = 0;
            going_up = true;
        }
    }
    // Square the fade value to make the LED's brightness appear more linear
    // Note this range matches with the wrap value
    pwm_set_gpio_level(ST7735S_BLK_PIN, fade * fade);
}

/**
 * @brief hardware layer initialize
 * for each platform, do it's iomux and pinctl here
 */
static void hal_init(void)
{
#if !defined(spi_default) || !defined(PICO_DEFAULT_SPI_SCK_PIN) || !defined(PICO_DEFAULT_SPI_TX_PIN) || !defined(PICO_DEFAULT_SPI_RX_PIN) || !defined(ST7735S_CS_PIN)
#warning spi/bme280_spi example requires a board with SPI pins
    puts( "Default SPI pins were not defined" );
#else
    /* Useing default SPI0 at 50MHz */
    spi_init( spi_default, 50 * 1000 * 1000 );
    gpio_set_function( PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI );
    gpio_set_function( PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI );
    bi_decl( bi_2pins_with_func( PICO_DEFAULT_SPI_TX_PIN, PICO_DEFAULT_SPI_SCK_PIN,
                                 GPIO_FUNC_SPI ) );

    gpio_init( ST7735S_CS_PIN );
    gpio_set_dir( ST7735S_CS_PIN, GPIO_OUT );
    gpio_put( ST7735S_CS_PIN, 1 );
    bi_decl( bi_1pin_with_name( ST7735S_CS_PIN, "SPI CS" ) );

    gpio_init( ST7735S_RES_PIN );
    gpio_set_dir( ST7735S_RES_PIN, GPIO_OUT );

    gpio_init( ST7735S_DC_PIN );
    gpio_set_dir( ST7735S_DC_PIN, GPIO_OUT );

	gpio_set_function(ST7735S_BLK_PIN, GPIO_FUNC_PWM);

	uint32_t slice_num = pwm_gpio_to_slice_num(ST7735S_BLK_PIN);
	pwm_clear_irq(slice_num);
	pwm_set_irq_enabled(slice_num, true);

	irq_set_exclusive_handler(PWM_IRQ_WRAP, on_pwm_wrap);
	// irq_set_enabled(PWM_IRQ_WRAP, true);

	pwm_config config = pwm_get_default_config();

	pwm_config_set_clkdiv(&config, 2.f);
	pwm_init(slice_num, &config, true);

	st7735s_device_init();
#endif
}

static void st7735s_set_backlight(uint16_t level)
{
	pwm_set_gpio_level(ST7735S_BLK_PIN, level * level);
}

int main(void)
{
    stdio_init_all();

    hal_init();

    printf("%s\n", __func__);

	st7735s_set_backlight(255);

	for(int x=0;x<128;x++) {
		for(int y=0;y<160;y++)
			st7735s_draw_pixel(x, y, 0xFFFF);
	}

	for(int x=0;x<128;x++) {
		for(int y=0;y<160;y++){
			st7735s_draw_pixel(x, y, 0);
			sleep_us(200);
		}
		sleep_ms(20);
	}

    while( 1 ) {
        tight_loop_contents();
    }

    // while(1);
}