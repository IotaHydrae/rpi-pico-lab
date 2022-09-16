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

void ssd1306_write_cmd(uint8_t val)
{
    uint8_t wbuf[2] = {0};
    wbuf[0] = 0x00;
    wbuf[1] = val;
    i2c_write_blocking(i2c_default, 0x3c, wbuf, 2, false);
}

void ssd1306_write_data(uint8_t val)
{
    uint8_t wbuf[2] = {0};
    wbuf[0] = 0x40;
    wbuf[1] = val;
    i2c_write_blocking(i2c_default, 0x3c, wbuf, 2, false);
}
static void ssd1306_device_init()
{
#ifdef SSD1306_128_32
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
    sleep_ms(200);
	ssd1306_write_cmd(0xAF);//--turn on oled pan
#else
	ssd1306_write_cmd(0xAE); /*display off*/
	ssd1306_write_cmd(0x00); /*set lower column address*/
	ssd1306_write_cmd(0x10); /*set higher column address*/
	ssd1306_write_cmd(0x40); /*set display start line*/
	ssd1306_write_cmd(0xB0); /*set page address*/
	ssd1306_write_cmd(0x81); /*contract control*/
	ssd1306_write_cmd(0x66); /*128*/
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
}


void ssd1306_set_pos(uint8_t page, uint8_t col)
{
	ssd1306_write_cmd(0xb0 + page);

	ssd1306_write_cmd(0x00 | (col & 0x0f));
	ssd1306_write_cmd(0x10 | (col >> 4));
}

void ssd1306_init()
{
	int count = 0;
	ssd1306_device_init();

	for (; count < SSD1306_BUFFER_SIZE; count++)
	{
		ssd1306_buffer[count] = 0x00;
	}    
}

void ssd1306_flush()
{
	uint8_t page, col;

	for (page = 0; page < SSD1306_PAGE_SIZE; page++)
		for (col = 0; col < SSD1306_HOR_RES_MAX; col++)
		// if (oled_buffer[OFFSET(page, col)] != 0x00)
		{
			ssd1306_set_pos(page, col);
			ssd1306_write_data(ssd1306_buffer[OFFSET(page, col)]);
		}
}

void ssd1306_clear()
{
	uint8_t page, col;

	for (page = 0; page < SSD1306_PAGE_SIZE; page++)
		for (col = 0; col < 128; col++)
			if (ssd1306_buffer[OFFSET(page, col)] > 0x00)
			{
				ssd1306_set_pos(page, col);
				ssd1306_write_data(0x00);
			}
	// memset(oled_buffer, 0x0, 1024);
	// oled_flush();
}

void ssd1306_set_pixel(uint8_t x, uint8_t y, uint8_t color)
{
	uint8_t page, page_left;
	uint8_t *pen = ssd1306_buffer;

#ifdef OLED_COORD_CHECK
	if ((x >= 0 && x < SSD1306_HOR_RES_MAX) && (y >= 0 && y < SSD1306_VER_RES_MAX))
	{
#endif
		page = y / 8;
		page_left = y % 8 == 0 ? 0 : y % 8;
        // printf("page, page_left: %d, %d\n",page, page_left);

		if (color)
		{
			pen[OFFSET(page, x)] |= (1 << page_left);
            // printf("+++ pos : %d, dump: %d\n", OFFSET(page, x), pen[OFFSET(page, x)]);
		}
		else
		{
			pen[OFFSET(page, x)] &= ~(1 << page_left);
            // printf("--- pos : %d, dump: %d\n", OFFSET(page, x), pen[OFFSET(page, x)]);
		}

#ifdef OLED_COORD_CHECK
	}
#endif

	/*oled_set_pos(page, x);
	oled_write_dat(oled_buffer[offset]);*/
}