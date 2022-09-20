/**
 * @file main.c
 * @author IotaHydrae (writeforever@foxmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-09-19
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
#include <stdlib.h>

#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define CST816T_SDA 6
#define CST816T_SCL 7
#define CST816T_RST	8
#define CST816T_INT	9

int main()
{
	int rc;
	uint8_t wbuf[8] = {0}, rbuf[8] = {0};
	stdio_init_all();

	i2c_init(i2c1, 400 * 1000);
	gpio_set_function(CST816T_SDA, GPIO_FUNC_I2C);
	gpio_set_function(CST816T_SCL, GPIO_FUNC_I2C);
	gpio_pull_up(6);
	gpio_pull_up(7);

	// uint8_t init_buf[1] = {0xe1};
	// i2c_write_blocking(i2c_default, 0x38, init_buf, 1, false);



	while(true) {

		sleep_ms(500);
	}


	return 0;
}
