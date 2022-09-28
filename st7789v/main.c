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

#include "include/st7789v.h"

#define ST7789V_RES_PIN   15
#define ST7789V_DC_PIN    14
#define ST7789V_CS_PIN    13
#define ST7789V_BLK_PIN   12

/* ========== st7789v pin controls ========== */
#ifdef PICO_DEFAULT_SPI_CSN_PIN
static inline void cs_select()
{
    asm volatile( "nop \n nop \n nop" );
    gpio_put( ST7789V_CS_PIN, 0 ); // Active low
    asm volatile( "nop \n nop \n nop" );
}

static inline void cs_deselect()
{
    asm volatile( "nop \n nop \n nop" );
    gpio_put( ST7789V_CS_PIN, 1 );
    asm volatile( "nop \n nop \n nop" );
}
#endif

static inline void st7789v_dc_set()
{
    asm volatile( "nop \n nop \n nop" );
    gpio_put( ST7789V_DC_PIN, 1 );
    asm volatile( "nop \n nop \n nop" );
}

static inline void st7789v_dc_clr()
{
    asm volatile( "nop \n nop \n nop" );
    gpio_put( ST7789V_DC_PIN, 0 );
    asm volatile( "nop \n nop \n nop" );
}

static inline void st7789v_res_set()
{
    asm volatile( "nop \n nop \n nop" );
    gpio_put( ST7789V_RES_PIN, 1 );
    asm volatile( "nop \n nop \n nop" );
}

static inline void st7789v_res_clr()
{
    asm volatile( "nop \n nop \n nop" );
    gpio_put( ST7789V_RES_PIN, 0 );
    asm volatile( "nop \n nop \n nop" );
}

static void st7789v_reset()
{
    st7789v_res_set();
    sleep_ms( 200 );

    st7789v_res_clr();
    sleep_ms( 50 );

    st7789v_res_set();
    sleep_ms( 200 );
}

/* ========== st7789v I/O ========== */

static inline void st7789v_write_byte( uint8_t val )
{
    uint8_t buf[1] = {val};
    cs_select();
    spi_write_blocking( spi_default, buf, 1 );
    cs_deselect();
}

static void st7789v_write_command( uint8_t command )
{
    st7789v_dc_clr();
    st7789v_write_byte( command );
}

static void st7789v_write_data( uint8_t data )
{
    st7789v_dc_set();
    st7789v_write_byte( data );
}

static void st7789v_device_init(void)
{
    st7789v_reset();

    sleep_ms(50);

    st7789v_write_command(SWRESET);
    st7789v_write_command(0x11);

    sleep_ms(120);

    st7789v_write_command(0x36);
    st7789v_write_data(0x00);

    st7789v_write_command(0x3A);
    st7789v_write_data(0x06);

    st7789v_write_command(0xB2);
    st7789v_write_data(0x0B);
    st7789v_write_data(0x0B);
    st7789v_write_data(0x00);
    st7789v_write_data(0x33);
    st7789v_write_data(0x33);

    st7789v_write_command(0xB7);
    st7789v_write_data(0x11);

    st7789v_write_command(0xBB);
    st7789v_write_data(0x2F);

    st7789v_write_command(0xC0);
    st7789v_write_data(0x2C);

    st7789v_write_command(0xC2);
    st7789v_write_data(0x01);

    st7789v_write_command(0xC3);
    st7789v_write_data(0x0D);

    st7789v_write_command(0xC4);
    st7789v_write_data(0x20);   //VDV, 0x20:0v

    st7789v_write_command(0xC6);
    st7789v_write_data(0x13);   //0x13:60Hz

    st7789v_write_command(0xD0);
    st7789v_write_data(0xA4);
    st7789v_write_data(0xA1);

    st7789v_write_command(0xD6);
    st7789v_write_data(0xA1);   //sleep in后，gate输出为GND

    st7789v_write_command(0xE0);
    st7789v_write_data(0xF0);
    st7789v_write_data(0x04);
    st7789v_write_data(0x07);
    st7789v_write_data(0x09);
    st7789v_write_data(0x07);
    st7789v_write_data(0x13);
    st7789v_write_data(0x25);
    st7789v_write_data(0x33);
    st7789v_write_data(0x3C);
    st7789v_write_data(0x34);
    st7789v_write_data(0x10);
    st7789v_write_data(0x10);
    st7789v_write_data(0x29);
    st7789v_write_data(0x32);

    st7789v_write_command(0xE1);
    st7789v_write_data(0xF0);
    st7789v_write_data(0x05);
    st7789v_write_data(0x08);
    st7789v_write_data(0x0A);
    st7789v_write_data(0x09);
    st7789v_write_data(0x05);
    st7789v_write_data(0x25);
    st7789v_write_data(0x32);
    st7789v_write_data(0x3B);
    st7789v_write_data(0x3B);
    st7789v_write_data(0x17);
    st7789v_write_data(0x18);
    st7789v_write_data(0x2E);
    st7789v_write_data(0x37);

    st7789v_write_command(0xE4);
    st7789v_write_data(0x25);   //使用240根gate  (N+1)*8
    st7789v_write_data(0x00);   //设定gate起点位置
    st7789v_write_data(0x00);   //当gate没有用完时，bit4(TMG)设为0

    st7789v_write_command(0x21);

    st7789v_write_command(0x29);

    // st7789v_write_command(0x2A);     //Column Address Set
    // st7789v_write_data(0x00);
    // st7789v_write_data(0x00);   //0
    // st7789v_write_data(0x00);
    // st7789v_write_data(0xEF);   //239

    // st7789v_write_command(0x2B);     //Row Address Set
    // st7789v_write_data(0x00);
    // st7789v_write_data(0x14);   //0
    // st7789v_write_data(0x01);
    // st7789v_write_data(0x2B);

    // st7789v_write_command(0x2C);


}

/**
 * @brief hardware layer initialize
 * for each platform, do it's iomux and pinctl here
 */
static void hal_init(void)
{
    stdio_init_all();

#if !defined(spi_default) || !defined(PICO_DEFAULT_SPI_SCK_PIN) || !defined(PICO_DEFAULT_SPI_TX_PIN) || !defined(PICO_DEFAULT_SPI_RX_PIN) || !defined(ST7789V_CS_PIN)
#warning spi/bme280_spi example requires a board with SPI pins
    puts( "Default SPI pins were not defined" );
#else
    /* Useing default SPI0 at 50MHz */
    spi_init( spi_default, 10 * 1000 * 1000 );
    gpio_set_function( PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI );
    gpio_set_function( PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI );
    bi_decl( bi_2pins_with_func( PICO_DEFAULT_SPI_TX_PIN, PICO_DEFAULT_SPI_SCK_PIN,
                                 GPIO_FUNC_SPI ) );

    gpio_init( ST7789V_CS_PIN );
    gpio_set_dir( ST7789V_CS_PIN, GPIO_OUT );
    gpio_put( ST7789V_CS_PIN, 1 );
    bi_decl( bi_1pin_with_name( ST7789V_CS_PIN, "SPI CS" ) );

    gpio_init( ST7789V_RES_PIN );
    gpio_set_dir( ST7789V_RES_PIN, GPIO_OUT );

    gpio_init( ST7789V_DC_PIN );
    gpio_set_dir( ST7789V_DC_PIN, GPIO_OUT );

    gpio_set_function(ST7789V_BLK_PIN, GPIO_FUNC_PWM);
#endif
}

void on_pwm_wrap() {
    static int fade = 0;
    static bool going_up = true;
    // Clear the interrupt flag that brought us here
    pwm_clear_irq(pwm_gpio_to_slice_num(ST7789V_BLK_PIN));

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
    pwm_set_gpio_level(ST7789V_BLK_PIN, fade * fade);
}

int main(void)
{
    stdio_init_all();

    hal_init();

    gpio_put(ST7789V_BLK_PIN, 1);
    st7789v_device_init();

    uint32_t slice_num = pwm_gpio_to_slice_num(ST7789V_BLK_PIN);
    pwm_clear_irq(slice_num);
    pwm_set_irq_enabled(slice_num, true);

    irq_set_exclusive_handler(PWM_IRQ_WRAP, on_pwm_wrap);
    irq_set_enabled(PWM_IRQ_WRAP, true);

    pwm_config config = pwm_get_default_config();

    pwm_config_set_clkdiv(&config, 4.f);
    pwm_init(slice_num, &config, true);

    while(1){
        tight_loop_contents();
    }

    // while(1);
}