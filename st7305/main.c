/**
 * @file main.c
 * @author Zheng Hua (hua.zheng@embeddedboys.com)
 * @brief
 * @version 0.1
 * @date 2025-2-9
 *
 * MIT License
 *
 * Copyright 2025 Zheng Hua (hua.zheng@embeddedboys.com)
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

#define ST7305_RES_PIN   15
#define ST7305_DC_PIN    14
#define ST7305_CS_PIN    13
#define ST7305_TE_PIN    12

#define ST7305_PANEL_1_54 1
#define ST7305_PANEL_2_13 2
#define ST7305_PANEL_2_90 3

#ifndef DEFAULT_ST7305_PANEL
    #define DEFAULT_ST7305_PANEL ST7305_PANEL_1_54
    // #define DEFAULT_ST7305_PANEL ST7305_PANEL_2_13
    // #define DEFAULT_ST7305_PANEL ST7305_PANEL_2_90
#endif

#if DEFAULT_ST7305_PANEL == ST7305_PANEL_1_54
    #define ST7305_WIDTH   200
    #define ST7305_HEIGHT  200
#elif DEFAULT_ST7305_PANEL == ST7305_PANEL_2_13
    #define ST7305_WIDTH   250
    #define ST7305_HEIGHT  122
#elif DEFAULT_ST7305_PANEL == ST7305_PANEL_2_90
    #define ST7305_WIDTH   200
    #define ST7305_HEIGHT  200
#else
    #error "Invalid ST7305 panel selected"
#endif

/* ========== st7305 pin controls ========== */
#ifdef PICO_DEFAULT_SPI_CSN_PIN
static inline void cs_select()
{
    asm volatile( "nop \n nop \n nop" );
    gpio_put( ST7305_CS_PIN, 0 ); // Active low
    asm volatile( "nop \n nop \n nop" );
}

static inline void cs_deselect()
{
    asm volatile( "nop \n nop \n nop" );
    gpio_put( ST7305_CS_PIN, 1 );
    asm volatile( "nop \n nop \n nop" );
}
#endif

static inline void st7305_dc_set()
{
    asm volatile( "nop \n nop \n nop" );
    gpio_put( ST7305_DC_PIN, 1 );
    asm volatile( "nop \n nop \n nop" );
}

static inline void st7305_dc_clr()
{
    asm volatile( "nop \n nop \n nop" );
    gpio_put( ST7305_DC_PIN, 0 );
    asm volatile( "nop \n nop \n nop" );
}

static inline void st7305_res_set()
{
    asm volatile( "nop \n nop \n nop" );
    gpio_put( ST7305_RES_PIN, 1 );
    asm volatile( "nop \n nop \n nop" );
}

static inline void st7305_res_clr()
{
    asm volatile( "nop \n nop \n nop" );
    gpio_put( ST7305_RES_PIN, 0 );
    asm volatile( "nop \n nop \n nop" );
}

static void st7305_reset()
{
    st7305_res_set();
    sleep_ms( 10 );

    st7305_res_clr();
    sleep_ms( 10 );

    st7305_res_set();
    sleep_ms( 10 );
}

/* ========== st7305 I/O ========== */

static inline void st7305_write_byte( uint8_t val )
{
    uint8_t buf[1] = {val};
    cs_select();
    spi_write_blocking( spi_default, buf, 1 );
    cs_deselect();
}

static void st7305_write_command( uint8_t command )
{
    st7305_dc_clr();
    st7305_write_byte( command );
}
#define write_cmd st7305_write_command

static void st7305_write_data( uint8_t data )
{
    st7305_dc_set();
    st7305_write_byte( data );
}
#define write_data st7305_write_data

static void st7305_device_init(void)
{
	st7305_reset();
	sleep_ms(50);

#if DEFAULT_ST7305_PANEL == ST7305_PANEL_1_54
    // 7305
    write_cmd(0xD6); //NVM Load Control
	write_data(0X17);
	write_data(0X02);

	write_cmd(0xD1); //Booster Enable
	write_data(0X01);

	write_cmd(0xC0); //Gate Voltage Setting
	write_data(0X08); //VGH 00:8V  04:10V  08:12V   0E:15V
	write_data(0X02); //VGL 00:-5V   04:-7V   0A:-10V


// VLC=3.6V (12/-5)(delta Vp=0.6V)
	write_cmd(0xC1); //VSHP Setting (4.8V)
	write_data(0X19); //VSHP1
	write_data(0X19); //VSHP2
	write_data(0X19); //VSHP3
	write_data(0X19); //VSHP4

	write_cmd(0xC2); //VSLP Setting (0.98V)
	write_data(0X31); //VSLP1
	write_data(0X31); //VSLP2
	write_data(0X31); //VSLP3
	write_data(0X31); //VSLP4

	write_cmd(0xC4); //VSHN Setting (-3.6V)
	write_data(0X19); //VSHN1
	write_data(0X19); //VSHN2
	write_data(0X19); //VSHN3
	write_data(0X19); //VSHN4

	write_cmd(0xC5); //VSLN Setting (0.22V)
	write_data(0X27); //VSLN1
	write_data(0X27); //VSLN2
	write_data(0X27); //VSLN3
	write_data(0X27); //VSLN4

	write_cmd(0xD8); //HPM=32Hz
	write_data(0XA6); //~51Hz
	write_data(0XE9); //~1Hz

/*-- HPM=32hz ; LPM=> 0x15=8Hz 0x14=4Hz 0x13=2Hz 0x12=1Hz 0x11=0.5Hz 0x10=0.25Hz---*/
	write_cmd(0xB2); //Frame Rate Control
	write_data(0X12); //HPM=32hz ; LPM=1hz

	write_cmd(0xB3); //Update Period Gate EQ Control in HPM
	write_data(0XE5);
	write_data(0XF6);
	write_data(0X05); //HPM EQ Control
	write_data(0X46);
	write_data(0X77);
	write_data(0X77);
	write_data(0X77);
	write_data(0X77);
	write_data(0X76);
	write_data(0X45);

	write_cmd(0xB4); //Update Period Gate EQ Control in LPM
	write_data(0X05); //LPM EQ Control
	write_data(0X46);
	write_data(0X77);
	write_data(0X77);
	write_data(0X77);
	write_data(0X77);
	write_data(0X76);
	write_data(0X45);

	write_cmd(0x62); //Gate Timing Control
	write_data(0X32);
	write_data(0X03);
	write_data(0X1F);


	write_cmd(0xB7); //Source EQ Enable
	write_data(0X13);

	write_cmd(0xB0); //Gate Line Setting
	write_data(0X32); //200 line

	write_cmd(0x11); //Sleep out
	sleep_ms(120);

	write_cmd(0xC9); //Source Voltage Select
	write_data(0X00); //VSHP1; VSLP1 ; VSHN1 ; VSLN1

	write_cmd(0x36); //Memory Data Access Control
	write_data(0X48); //MX=1 ; DO=1 	//48

	write_cmd(0x3A); //Data Format Select
	write_data(0x10); //10:4write for 24bit ; 11: 3write for 24bit

	write_cmd(0xB9); //Gamma Mode Setting
	write_data(0X20); //20: Mono 00:4GS

	write_cmd(0xB8); //Panel Setting
	write_data(0X29); // Panel Setting Frame inversion  09:column 29:dot_1-Frame 25:dot_1-Line

	//WRITE RAM 200*200
	write_cmd(0x2A); //Column Address Setting
	write_data(0x16);//10
	write_data(0x26); //38

	write_cmd(0x2B); //Row Address Setting
	write_data(0X00);
	write_data(0X63); //63
/*
	write_cmd(0x72); //de-stress off
	write_data(0X13);
*/
	write_cmd(0x35); //TE
	write_data(0X00); //

	write_cmd(0xD0); //Auto power dowb
	write_data(0XFF); //

	write_cmd(0x39); //LPM

	write_cmd(0x29); //DISPLAY ON

#elif DEFAULT_ST7305_PANEL == ST7305_PANEL_2_13

    write_cmd(0xD6); //NVM Load Control
    write_data(0X17);
    write_data(0X02);

    write_cmd(0xD1); //Booster Enable
    write_data(0X01);

    write_cmd(0xC0); //Gate Voltage Setting
    write_data(0X0E); //VGH=15V
    write_data(0X05); //VGL=-7.5V

    write_cmd(0xC1); //VSHP Setting
    write_data(0X41); //VSHP1=5V
    write_data(0X41); //VSHP2=5V
    write_data(0X41); //VSHP3=5V
    write_data(0X41); //VSHP4=5V

    write_cmd(0xC2); //VSLP Setting
    write_data(0X32); //VSLP1=1V
    write_data(0X32); //VSLP2=1V
    write_data(0X32); //VSLP3=1V
    write_data(0X32); //VSLP4=1V

    write_cmd(0xC4); //VSHN Setting
    write_data(0X4B); //VSHN1=-4V
    write_data(0X4B); //VSHN2=-4V
    write_data(0X4B); //VSHN3=-4V
    write_data(0X4B); //VSHN4=-4V

    write_cmd(0xC5); //VSLN Setting
    write_data(0X00); //VSLN1=1V
    write_data(0X00); //VSLN2=1V
    write_data(0X00); //VSLN3=1V
    write_data(0X00); //VSLN4=1V

    write_cmd(0xD8); //HPM=32Hz
    write_data(0XA6);
    write_data(0XE9);

    write_cmd(0xB2); //Frame Rate Control
    write_data(0X11); //HPM=32hz ; LPM=0.5hz

    write_cmd(0xB3); //Update Period Gate EQ Control in HPM
    write_data(0XE5); //
    write_data(0XF6); //
    write_data(0X05); //HPM EQ Control
    write_data(0X46); //
    write_data(0X77); //
    write_data(0X77); //
    write_data(0X77); //
    write_data(0X77); //
    write_data(0X76); //
    write_data(0X45); //

    write_cmd(0xB4); //Update Period Gate EQ Control in LPM
    write_data(0X05); //LPM EQ Control
    write_data(0X46); //
    write_data(0X77); //
    write_data(0X77); //
    write_data(0X77); //
    write_data(0X77); //
    write_data(0X76); //
    write_data(0X45); //

    write_cmd(0xB7); //Source EQ Enable
    write_data(0X13); //

    write_cmd(0xB0); //Gate Line Setting
    write_data(0X3F); //252 line

    write_cmd(0x11); //Sleep out
    sleep_ms(120);

    write_cmd(0xC9); //Source Voltage Select
    write_data(0x00); //VSHP1; VSLP1 ; VSHN1 ; VSLN1

    write_cmd(0xC7); //ultra low power code
    write_data(0xC1);
    write_data(0x41);
    write_data(0x26);

    write_cmd(0x36); //Memory Data Access Control
    write_data(0X00); //MX=0 ; DO=0

    write_cmd(0x3A); //Data Format Select
    write_data(0X11); //10:4write for 24bit ; 11: 3write for 24bit

    write_cmd(0xB9); //Gamma Mode Setting
    write_data(0X20); //20: Mono 00:4GS

    write_cmd(0xB8); //Panel Setting
    write_data(0X25); //dot inversion; one line interval; dot inversion

    //write_cmd(0x21); //Inverse

    //WRITE RAM 122x250
    write_cmd(0x2A); //Column Address Setting
    write_data(0X19);
    write_data(0X23);

    write_cmd(0x2B); //Row Address Setting
    write_data(0X00);
    write_data(0X7C);

    write_cmd(0x35); //TE
    write_data(0X00); //

    write_cmd(0xD0); //Auto power down
    write_data(0XFF); //

    write_cmd(0x39); //0x39 low power 0x38 high power
    write_cmd(0x29); //DISPLAY ON

#elif DEFAULT_ST7305_PANEL == ST7305_PANEL_2_90

#else
    #error "Unknown ST7305 panel"
#endif

}

static void st7305_set_addr_win(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
	write_cmd(0x2A); //Column Address Setting
	write_data(0x16);//10
	write_data(0x26); //38

	write_cmd(0x2B); //Row Address Setting
 	write_data(0X00);
	write_data(0X63); //63

    write_cmd(0x2C);   //write image data
}

static void st7305_clear(uint8_t color)
{
    st7305_set_addr_win(0, 0, 199, 199);
    /* TODO: clear display ram */
}

static void st7305_show_pic(const unsigned char *pic, size_t len)
{
    st7305_set_addr_win(0, 0, 199, 199);

    for (size_t i = 0; i < len; i++)
        write_data(pic[i]);
}

/**
 * @brief hardware layer initialize
 * for each platform, do it's iomux and pinctl here
 */
static void hal_init(void)
{
#if !defined(spi_default) || !defined(PICO_DEFAULT_SPI_SCK_PIN) || !defined(PICO_DEFAULT_SPI_TX_PIN) || !defined(PICO_DEFAULT_SPI_RX_PIN) || !defined(ST7305_CS_PIN)
#warning spi/bme280_spi example requires a board with SPI pins
    puts( "Default SPI pins were not defined" );
#else
    spi_init( spi_default, 400 * 1000 );
    gpio_set_function( PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI );
    gpio_set_function( PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI );
    bi_decl( bi_2pins_with_func( PICO_DEFAULT_SPI_TX_PIN, PICO_DEFAULT_SPI_SCK_PIN,
                                 GPIO_FUNC_SPI ) );

    gpio_init( ST7305_CS_PIN );
    gpio_set_dir( ST7305_CS_PIN, GPIO_OUT );
    gpio_put( ST7305_CS_PIN, 1 );
    bi_decl( bi_1pin_with_name( ST7305_CS_PIN, "SPI CS" ) );

    gpio_init( ST7305_RES_PIN );
    gpio_set_dir( ST7305_RES_PIN, GPIO_OUT );

    gpio_init( ST7305_DC_PIN );
    gpio_set_dir( ST7305_DC_PIN, GPIO_OUT );

    gpio_init( ST7305_TE_PIN );
    gpio_set_dir( ST7305_TE_PIN, GPIO_IN );

	st7305_device_init();
#endif
}

extern const unsigned char pic1[6800];
int main(void)
{
    stdio_init_all();

    hal_init();

    printf("%s\n", __func__);

	st7305_show_pic(pic1, sizeof(pic1)/sizeof(pic1[0]));

    for (;;) {
        tight_loop_contents();
    }
}