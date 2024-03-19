/**
 * @file ssd1306.h
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

#ifndef __SSD1306_H
#define __SSD1306_H

#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define BIT(x) (1 << x)
#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))

#define SSD1306_I2C_IF i2c1
#define SSD1306_PIN_SCL 27
#define SSD1306_PIN_SDA 26

#define SSD1306_ADDRESS 0x3c

#define SSD1306_CMD     0x00
#define SSD1306_DATA    0x40

#define SSD1306_128_32  1

#if SSD1306_128_32
    #define SSD1306_HOR_RES 128
    #define SSD1306_VER_RES 32
#else
    #define SSD1306_HOR_RES 128
    #define SSD1306_VER_RES 64
#endif

#define SSD1306_BUFFER_SIZE ((SSD1306_HOR_RES * SSD1306_VER_RES)/8)
#define SSD1306_PAGE_SIZE (SSD1306_VER_RES/8)

#define OFFSET(p, c) ((p)*128 + (c))
#define GET_PAGE_FROM_BUFFER(i) (i / 128)
#define GET_COL_FROM_BUFFER(i) (i % 128)

#define GET_PAGE(pc) (pc >> 16)
#define GET_COL(pc) (pc & 0xFFFF)
#define PAGE_COL(page, col) ((page << 16) | col)
#define PAGE_COL_GET_X(pc) (GET_COL(pc))
#define PAGE_COL_GET_Y(pc) (GET_PAGE(pc) * 8)

// #define OLED_COORD_CHECK

enum {
    SSD1306_ADDR_MODE_HORIZONTAL,
    SSD1306_ADDR_MODE_VERTICAL,
    SSD1306_ADDR_MODE_PAGE,
    SSD1306_ADDR_MODE_INVALID,
};

/**
 * Register Map of SSD1306
 * 0x00 ~ 0xFF
 */

void ssd1306_init();
void ssd1306_clear();
void ssd1306_video_flush(int xs, int ys, int xe, int ye, void *vmem, size_t len);

#endif /* __SSD1306_H */
