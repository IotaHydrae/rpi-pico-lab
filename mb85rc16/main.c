/*
 * Copyright (c) 2024 embeddedboys
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
 * SOFTWARE.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include "mb85rc16.h"

/*
 * MB85RC16 Pin assignments:
 * I2C: i2c_default (i2c0)
 * SDA: PICO_DEFAULT_I2C_SDA_PIN (GP4)
 * SCL: PICO_DEFAULT_I2C_SCL_PIN (GP5)
 */

#define PAGE_BUF_SIZE 8
#if PAGE_BUF_SIZE > MB85RC16_PAGE_SIZE
    #error "PAGE_BUF_SIZE should be less than MB85RC16_PAGE_SIZE"
#endif

#define DEBUG 1
#if DEBUG
    #define pr_debug(...) printf(__VA_ARGS__)
#else
    #define pr_debug(...)
#endif

static uint32_t lv_rand(uint32_t min, uint32_t max);
static void make_random_data(uint8_t *p, size_t len);
static void compare_data(uint8_t *p1, uint8_t *p2, size_t len);

static uint8_t data_buf[PAGE_BUF_SIZE] = {0};
static uint8_t rxbuf[PAGE_BUF_SIZE] = {0};

int main()
{
    int ret;
    uint time_start, time_end;
    // stdio_init_all();
    stdio_uart_init_full(uart1, 115200, 24, 25);

    printf("\n\n\n\n\tMB85RC16 FRAM Chip tests.\n\n");
    printf("  Page size  : %d bytes\n", MB85RC16_PAGE_SIZE);
    printf("  Page count : %d\n", MB85RC16_PAGE_COUNT);
    printf("  Total size : %d bytes\n", MB85RC16_TOTAL_SIZE);

    i2c_init(i2c_default, MB85RC16_SPEED_KHZ * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

    printf("\n##### Test random write and read. size : 1 byte. #####\n");
    mb85rc16_byte_write(i2c_default, 0x01, 0xaa);
    mb85rc16_random_read(i2c_default, 0x01, rxbuf);
    printf("result (0x01): 0x%02x\n", rxbuf[0]);

    printf("\n##### Test page write and read. size : %d bytes. #####\n", sizeof(data_buf));
    make_random_data(data_buf, sizeof(data_buf));
    mb85rc16_page_write(i2c_default, 0x00, data_buf, sizeof(data_buf));
    mb85rc16_seq_read(i2c_default, 0x00, rxbuf, sizeof(rxbuf));
    compare_data(data_buf, rxbuf, sizeof(data_buf));

    printf("\n##### Test current address read. size : %d bytes. #####\n", sizeof(data_buf));
    make_random_data(data_buf, sizeof(data_buf));
    mb85rc16_page_write(i2c_default, 0x00, data_buf, sizeof(data_buf));

    /* This will reset the memory address to 0x00 */
    mb85rc16_seq_read(i2c_default, 0x00, rxbuf, sizeof(rxbuf) / 2);

    /* use currrent address read command to read the left half of data */
    for (int i = 0; i < sizeof(rxbuf) / 2; i++)
        mb85rc16_current_addr_read(i2c_default, &rxbuf[sizeof(rxbuf) / 2 + i]);

    compare_data(data_buf, rxbuf, sizeof(rxbuf));

    for(;;) {
        tight_loop_contents();
        sleep_ms(2000);
    }

    return 0;
}

static uint32_t lv_rand(uint32_t min, uint32_t max)
{
    static uint32_t a = 0x1234ABCD; /*Seed*/

    /*Algorithm "xor" from p. 4 of Marsaglia, "Xorshift RNGs"*/
    uint32_t x = a;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    a = x;

    return (a % (max - min + 1)) + min;
}

static void make_random_data(uint8_t *p, size_t len)
{
    for (int i = 0; i < len; i++) {
        p[i] = lv_rand(0, 255);

        pr_debug("0x%02x ", p[i]);
        if (i % 8 == 7)
            pr_debug("\n");
    }
    pr_debug("\n");
}

static void compare_data(uint8_t *p1, uint8_t *p2, size_t len)
{
    for (int i = 0; i < len; i++) {
        if (p1[i] != p2[i]) {
            pr_debug("[ERROR] data mismatch at 0x%02x: [0x%02x] = [0x%02x]\n", i, p1[i], p2[i]);
        } else {
            pr_debug("0x%02x: [0x%02x] = [0x%02x]\n", i, p1[i], p2[i]);
        }
    }
    pr_debug("[PASSED] data matched\n\n");
}