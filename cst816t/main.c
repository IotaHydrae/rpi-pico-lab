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
#define CST816T_RST 8
#define CST816T_INT 9

#define CST816T_ADDR 0x15

void cst816t_reset(void)
{
    gpio_put(CST816T_RST, 0);
    sleep_ms(10);

    gpio_put(CST816T_RST, 1);
    sleep_ms(10);
}

int main()
{
    int rc;
    uint8_t wbuf[8] = {0}, rbuf[8] = {0};
    stdio_init_all();

    i2c_init(i2c1, 400 * 1000);
    gpio_set_function(CST816T_SDA, GPIO_FUNC_I2C);
    gpio_set_function(CST816T_SCL, GPIO_FUNC_I2C);
    gpio_set_function(CST816T_RST, GPIO_OUT);
    gpio_set_function(CST816T_INT, GPIO_OUT);

    gpio_pull_up(CST816T_SDA);
    gpio_pull_up(CST816T_SCL);
    gpio_pull_up(CST816T_RST);

    cst816t_reset();

    wbuf[0] = 0xA7;
    rc = i2c_write_blocking(i2c1, CST816T_ADDR, wbuf, 1, false);
    if (rc == PICO_ERROR_GENERIC) {
        printf("write failed!\n");
    }

    rc = i2c_read_blocking(i2c1, CST816T_ADDR, rbuf, 1, false);
    if (rc == PICO_ERROR_GENERIC) {
        printf("read failed!\n");
    }
    printf("%s, val: %d\n", __func__, rbuf[0]);

    while (true) {

        sleep_ms(500);
    }


    return 0;
}
