/**
 * MIT License
 *
 * Copyright 2025 embeddedboys developers
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

#define ST77916_SCL_PIN   18
#define ST77916_SDA_PIN   19
#define ST77916_RES_PIN   15
#define ST77916_DC_PIN    14
#define ST77916_CS_PIN    13
#define ST77916_BLK_PIN   12

/* ========== st77916 pin controls ========== */
static inline void cs_select()
{
	asm volatile("nop \n nop \n nop");
	gpio_put(ST77916_CS_PIN, 0);   // Active low
	asm volatile("nop \n nop \n nop");
}

static inline void cs_deselect()
{
	asm volatile("nop \n nop \n nop");
	gpio_put(ST77916_CS_PIN, 1);
	asm volatile("nop \n nop \n nop");
}

static inline void st77916_dc_set()
{
	asm volatile("nop \n nop \n nop");
	gpio_put(ST77916_DC_PIN, 1);
	asm volatile("nop \n nop \n nop");
}

static inline void st77916_dc_clr()
{
	asm volatile("nop \n nop \n nop");
	gpio_put(ST77916_DC_PIN, 0);
	asm volatile("nop \n nop \n nop");
}

static inline void st77916_res_set()
{
	asm volatile("nop \n nop \n nop");
	gpio_put(ST77916_RES_PIN, 1);
	asm volatile("nop \n nop \n nop");
}

static inline void st77916_res_clr()
{
	asm volatile("nop \n nop \n nop");
	gpio_put(ST77916_RES_PIN, 0);
	asm volatile("nop \n nop \n nop");
}

static void st77916_reset()
{
	st77916_res_set();
	sleep_ms(10);

	st77916_res_clr();
	sleep_ms(10);

	st77916_res_set();
	sleep_ms(10);
}

/* ========== st77916 I/O ========== */

static inline void write_byte(uint8_t val)
{
	for (int i = 0; i < 8; i++) {
		gpio_put(ST77916_SCL_PIN, 0);
		gpio_put(ST77916_SDA_PIN, (val & 0x80));
		sleep_us(1);
		val <<= 1;
		gpio_put(ST77916_SCL_PIN, 1);
		sleep_us(1);
		gpio_put(ST77916_SCL_PIN, 0);
		sleep_us(1);
	}
}

static void write_cmd(uint8_t command, uint8_t data[], int len)
{
	cs_select();

	write_byte(0x02);
	write_byte(0x00);
	write_byte(command);
	write_byte(0x00);

	if (!data)
		goto skip_data;

	for (int i = 0; i < len; i++) {
		write_byte(data[i]);
	}

skip_data:
	cs_deselect();
}

static void __write_reg(int len, ...)
{
	va_list args;
	uint8_t reg;

	va_start(args, len);

	reg = (uint8_t)va_arg(args, unsigned int);

	cs_select();

	write_byte(0x02);	// PP

	write_byte(0x00);
	write_byte(reg);
	write_byte(0x00);
	len--;

	if (len == 0)
		goto exit_no_params;

	for (int i = 0; i < len; i++)
		write_byte((uint8_t)va_arg(args, unsigned int));

exit_no_params:
	cs_deselect();
	va_end(args);
}
#define NUMARGS(...)  (sizeof((int[]){__VA_ARGS__}) / sizeof(int))
#define write_reg(...) \
    __write_reg(NUMARGS(__VA_ARGS__), __VA_ARGS__)

static void st77916_device_init(void)
{
	st77916_reset();
	sleep_ms(50);

	write_reg(0xf0, 0x28);
	write_reg(0xf2, 0x28);
	write_reg(0x73, 0xf0);
	write_reg(0x7c, 0xd1);
	write_reg(0x83, 0xe0);
	write_reg(0x84, 0x61);
	write_reg(0xf2, 0x82);
	write_reg(0xf0, 0x01);
	write_reg(0xf1, 0x01);
	write_reg(0xb0, 0x5e);
	write_reg(0xb1, 0x55);
	write_reg(0xb2, 0x24);
	write_reg(0xb3, 0x01);
	write_reg(0xb4, 0x87);
	write_reg(0xb5, 0x44);
	write_reg(0xb6, 0x8b);
	write_reg(0xb7, 0x40);
	write_reg(0xb8, 0x86);
	write_reg(0xb9, 0x15);
	write_reg(0xba, 0x00);
	write_reg(0xbb, 0x08);
	write_reg(0xbc, 0x08);
	write_reg(0xbd, 0x00);
	write_reg(0xbe, 0x00);
	write_reg(0xbf, 0x07);
	write_reg(0xc0, 0x80);
	write_reg(0xc1, 0x10);
	write_reg(0xc2, 0x37);
	write_reg(0xc3, 0x80);
	write_reg(0xc4, 0x10);
	write_reg(0xc5, 0x37);
	write_reg(0xc6, 0xa9);
	write_reg(0xc7, 0x41);
	write_reg(0xc8, 0x01);
	write_reg(0xc9, 0xa9);
	write_reg(0xca, 0x41);
	write_reg(0xcb, 0x01);
	write_reg(0xcc, 0x7f);
	write_reg(0xcd, 0x7f);
	write_reg(0xce, 0xff);
	write_reg(0xd0, 0x91);
	write_reg(0xd1, 0x68);
	write_reg(0xd2, 0x68);
	write_reg(0xf5, 0x00, 0xa5);
	write_reg(0xdd, 0x40);
	write_reg(0xde, 0x40);
	write_reg(0xf1, 0x10);
	write_reg(0xf0, 0x00);
	write_reg(0xf0, 0x02);
	write_reg(0xe0, 0xf0, 0x10, 0x18, 0x0d, 0x0c, 0x38, 0x3e,
			0x44, 0x51, 0x39, 0x15, 0x15, 0x30, 0x34);
	write_reg(0xe1, 0xf0, 0x0f, 0x17, 0x0d, 0x0b, 0x07, 0x3e,
			0x33, 0x51, 0x39, 0x15, 0x15, 0x30, 0x34);
	write_reg(0xf0, 0x10);
	write_reg(0xf3, 0x10);
	write_reg(0xe0, 0x08);
	write_reg(0xe1, 0x00);
	write_reg(0xe2, 0x00);
	write_reg(0xe3, 0x00);
	write_reg(0xe4, 0xe0);
	write_reg(0xe5, 0x06);
	write_reg(0xe6, 0x21);
	write_reg(0xe7, 0x03);
	write_reg(0xe8, 0x05);
	write_reg(0xe9, 0x02);
	write_reg(0xea, 0xe9);
	write_reg(0xeb, 0x00);
	write_reg(0xec, 0x00);
	write_reg(0xed, 0x14);
	write_reg(0xee, 0xff);
	write_reg(0xef, 0x00);
	write_reg(0xf8, 0xff);
	write_reg(0xf9, 0x00);
	write_reg(0xfa, 0x00);
	write_reg(0xfb, 0x30);
	write_reg(0xfc, 0x00);
	write_reg(0xfd, 0x00);
	write_reg(0xfe, 0x00);
	write_reg(0xff, 0x00);
	write_reg(0x60, 0x40);
	write_reg(0x61, 0x05);
	write_reg(0x62, 0x00);
	write_reg(0x63, 0x42);
	write_reg(0x64, 0xda);
	write_reg(0x65, 0x00);
	write_reg(0x66, 0x00);
	write_reg(0x67, 0x00);
	write_reg(0x68, 0x00);
	write_reg(0x69, 0x00);
	write_reg(0x6a, 0x00);
	write_reg(0x6b, 0x00);
	write_reg(0x70, 0x40);
	write_reg(0x71, 0x04);
	write_reg(0x72, 0x00);
	write_reg(0x73, 0x42);
	write_reg(0x74, 0xd9);
	write_reg(0x75, 0x00);
	write_reg(0x76, 0x00);
	write_reg(0x77, 0x00);
	write_reg(0x78, 0x00);
	write_reg(0x79, 0x00);
	write_reg(0x7a, 0x00);
	write_reg(0x7b, 0x00);
	write_reg(0x80, 0x48);
	write_reg(0x81, 0x00);
	write_reg(0x82, 0x07);
	write_reg(0x83, 0x02);
	write_reg(0x84, 0xd7);
	write_reg(0x85, 0x04);
	write_reg(0x86, 0x00);
	write_reg(0x87, 0x00);
	write_reg(0x88, 0x48);
	write_reg(0x89, 0x00);
	write_reg(0x8a, 0x09);
	write_reg(0x8b, 0x02);
	write_reg(0x8c, 0xd9);
	write_reg(0x8d, 0x04);
	write_reg(0x8e, 0x00);
	write_reg(0x8f, 0x00);
	write_reg(0x90, 0x48);
	write_reg(0x91, 0x00);
	write_reg(0x92, 0x0b);
	write_reg(0x93, 0x02);
	write_reg(0x94, 0xdb);
	write_reg(0x95, 0x00);
	write_reg(0x96, 0x00);
	write_reg(0x97, 0x00);
	write_reg(0x98, 0x48);
	write_reg(0x99, 0x00);
	write_reg(0x9a, 0x0d);
	write_reg(0x98, 0x02);
	write_reg(0x9c, 0xdd);
	write_reg(0x9d, 0x04);
	write_reg(0x9e, 0x00);
	write_reg(0x9f, 0x00);
	write_reg(0xa0, 0x48);
	write_reg(0xa1, 0x00);
	write_reg(0xa2, 0x06);
	write_reg(0xa3, 0x02);
	write_reg(0xa4, 0xd6);
	write_reg(0xa5, 0x04);
	write_reg(0xa6, 0x00);
	write_reg(0xa7, 0x00);
	write_reg(0xa8, 0x48);
	write_reg(0xa9, 0x00);
	write_reg(0xaa, 0x08);
	write_reg(0xab, 0x02);
	write_reg(0xac, 0xd8);
	write_reg(0xad, 0x04);
	write_reg(0xae, 0x00);
	write_reg(0xaf, 0x00);
	write_reg(0xb0, 0x48);
	write_reg(0xb1, 0x00);
	write_reg(0xb2, 0x0a);
	write_reg(0xb3, 0x02);
	write_reg(0xb4, 0xda);
	write_reg(0xb5, 0x04);
	write_reg(0xb6, 0x00);
	write_reg(0xb7, 0x00);
	write_reg(0xb8, 0x48);
	write_reg(0xb9, 0x00);
	write_reg(0xba, 0x0c);
	write_reg(0xbb, 0x02);
	write_reg(0xbc, 0xdc);
	write_reg(0xbd, 0x04);
	write_reg(0xbe, 0x00);
	write_reg(0xbf, 0x00);
	write_reg(0xc0, 0x10);
	write_reg(0xc1, 0x47);
	write_reg(0xc2, 0x56);
	write_reg(0xc3, 0x65);
	write_reg(0xc4, 0x74);
	write_reg(0xc5, 0x88);
	write_reg(0xc6, 0x99);
	write_reg(0xc7, 0x01);
	write_reg(0xc8, 0xbb);
	write_reg(0xc9, 0xaa);
	write_reg(0xd0, 0x10);
	write_reg(0xd1, 0x47);
	write_reg(0xd2, 0x56);
	write_reg(0xd3, 0x65);
	write_reg(0xd4, 0x74);
	write_reg(0xd5, 0x88);
	write_reg(0xd6, 0x99);
	write_reg(0xd7, 0x01);
	write_reg(0xd8, 0xbb);
	write_reg(0xd9, 0xaa);
	write_reg(0xf3, 0x01);
	write_reg(0xf0, 0x00);
	write_reg(0x3a, 0x55);

	write_reg(0x21);
	write_reg(0x11);
	sleep_ms(120);
	write_reg(0x29);

}

static void st77916_set_cursor(uint32_t x, uint32_t y)
{

}

void st77916_draw_pixel(uint32_t x, uint32_t y, uint16_t color)
{

}

/**
 * @brief hardware layer initialize
 * for each platform, do it's iomux and pinctl here
 */
static void hal_init(void)
{

	gpio_init(ST77916_SCL_PIN);
	gpio_set_dir(ST77916_SCL_PIN, GPIO_OUT);

	gpio_init(ST77916_SDA_PIN);
	gpio_set_dir(ST77916_SDA_PIN, GPIO_OUT);

	gpio_init(ST77916_CS_PIN);
	gpio_set_dir(ST77916_CS_PIN, GPIO_OUT);
	gpio_put(ST77916_CS_PIN, 0);
	bi_decl(bi_1pin_with_name(ST77916_CS_PIN, "SPI CS"));

	gpio_init(ST77916_RES_PIN);
	gpio_set_dir(ST77916_RES_PIN, GPIO_OUT);

	gpio_init(ST77916_DC_PIN);
	gpio_set_dir(ST77916_DC_PIN, GPIO_OUT);

	gpio_init(ST77916_BLK_PIN);
	gpio_set_dir(ST77916_BLK_PIN, GPIO_OUT);

	st77916_device_init();
}

static void st77916_set_backlight(uint16_t level)
{
	if (level)
		gpio_put(ST77916_BLK_PIN, 0);
	else
		gpio_put(ST77916_BLK_PIN, 1);
}

int main(void)
{
	stdio_uart_init_full(uart1, 115200, 24, 25);

	hal_init();

	printf("%s\n", __func__);

	st77916_set_backlight(100);

	// for (int x = 0; x < 128; x++) {
	// 	for (int y = 0; y < 160; y++)
	// 		st77916_draw_pixel(x, y, 0xFFFF);
	// }

	// for (int x = 0; x < 128; x++) {
	// 	for (int y = 0; y < 160; y++) {
	// 		st77916_draw_pixel(x, y, 0);
	// 		sleep_us(200);
	// 	}
	// 	sleep_ms(20);
	// }

	for (;;) {
		tight_loop_contents();
	}

	// while(1);
}