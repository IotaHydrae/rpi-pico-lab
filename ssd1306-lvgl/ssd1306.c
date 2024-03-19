/**
 * @file ssd1306.c
 * @author IotaHydrae (writeforever@foxmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-08-01
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

#include "ssd1306.h"

static uint8_t ssd1306_buffer[SSD1306_BUFFER_SIZE];

static void inline ssd1306_write_cmd(uint8_t val)
{
    i2c_write_blocking(SSD1306_I2C_IF, SSD1306_ADDRESS, (uint8_t []){0x00, val}, 2, false);
}

static void inline ssd1306_write_data(uint8_t val)
{
    i2c_write_blocking(SSD1306_I2C_IF, SSD1306_ADDRESS, (uint8_t []){0x40, val}, 2, false);
}

void ssd1306_init()
{
#if SSD1306_128_32
	ssd1306_write_cmd(0xAE);//--display off
    // ssd1306_write_cmd(0x00);
    // ssd1306_write_cmd(0x10);
	ssd1306_write_cmd(0x40);//---set low column address
	ssd1306_write_cmd(0xB0);//---set high column address
	ssd1306_write_cmd(0xC8);//-not offset
	ssd1306_write_cmd(0x81);// contract control
	ssd1306_write_cmd(0xFF);//--128 
	ssd1306_write_cmd(0xA1);//set segment remap 
	ssd1306_write_cmd(0xA6);//--normal / reverse	
	ssd1306_write_cmd(0xA8);//--set multiplex ratio(1 to 64)
	ssd1306_write_cmd(0x1F);	
	ssd1306_write_cmd(0xD3);//-set display offset
	ssd1306_write_cmd(0x00);
	ssd1306_write_cmd(0xD5);//set osc division
	ssd1306_write_cmd(0xF0);	
	ssd1306_write_cmd(0xD9);//Set Pre-Charge Period
	ssd1306_write_cmd(0x22);	
	ssd1306_write_cmd(0xDA);//set com pin configuartion
	ssd1306_write_cmd(0x02);	
	ssd1306_write_cmd(0xDB);//set Vcomh
	ssd1306_write_cmd(0x49);	
	ssd1306_write_cmd(0x8D);//set charge pump enable
	ssd1306_write_cmd(0x14);
	ssd1306_write_cmd(0xAF);//--turn on oled pan
#else
	ssd1306_write_cmd(0xAE); /*display off*/
	ssd1306_write_cmd(0x00); /*set lower column address*/
	ssd1306_write_cmd(0x10); /*set higher column address*/
	ssd1306_write_cmd(0x40); /*set display start line*/
	ssd1306_write_cmd(0xB0); /*set page address*/
	ssd1306_write_cmd(0x81); /*contract control*/
	ssd1306_write_cmd(0xFF); /*128*/
	ssd1306_write_cmd(0xA1); /*set segment remap*/
	ssd1306_write_cmd(0xA6); /*normal / reverse*/
	ssd1306_write_cmd(0xA8); /*multiplex ratio*/
	ssd1306_write_cmd(0x3F); /*duty = 1/64*/
	ssd1306_write_cmd(0xC8); /*Com scan direction*/
	ssd1306_write_cmd(0xD3); /*set display offset*/
	ssd1306_write_cmd(0x00);
	ssd1306_write_cmd(0xD5); /*set osc division*/
	ssd1306_write_cmd(0x80);
	ssd1306_write_cmd(0xD9); /*set pre-charge period*/
	ssd1306_write_cmd(0x1f);
	ssd1306_write_cmd(0xDA); /*set COM pins*/
	ssd1306_write_cmd(0x12);
	ssd1306_write_cmd(0xdb); /*set vcomh*/
	ssd1306_write_cmd(0x30);
	ssd1306_write_cmd(0x8d); /*set charge pump enable*/
	ssd1306_write_cmd(0x14);
	ssd1306_write_cmd(0xAF); /*display ON*/
#endif

	ssd1306_write_cmd(0x20);
	ssd1306_write_cmd(0x00);
}


void ssd1306_set_pos(uint8_t page, uint8_t col)
{
	ssd1306_write_cmd(0xb0 + page);

	ssd1306_write_cmd(0x00 | (col & 0x0f));
	ssd1306_write_cmd(0x10 | (col >> 4));
}

void ssd1306_clear()
{
	ssd1306_set_pos(0, 0);

	for (size_t i = 0; i < SSD1306_BUFFER_SIZE; i++)
		ssd1306_write_data(0x0);
}

void ssd1306_video_flush(int xs, int ys, int xe, int ye, void *vmem, size_t len)
{
	// printf("%s, xs : %d, ys : %d, xe : %d, ye : %d, len : %d\n", __func__, xs, ys, xe, ye, len);
	ssd1306_set_pos(0, 0);

	for (size_t i = 0; i < len / 8; i++)
		ssd1306_write_data(((uint8_t *)vmem)[i]);
}