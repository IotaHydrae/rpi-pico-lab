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

#include <string.h>
#include "mb85rc16.h"

#define MB85RC16_DEV_CODE         0x50
#define MB85RC16_ADDR(addr)       (MB85RC16_DEV_CODE | (addr >>  8) & 0xFF)

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

static inline size_t i2c_write_then_read(struct i2c_inst *i2c, u8 addr,
                                    const void *txbuf, u32 n_tx,
                                    void *rxbuf, u32 n_rx,
                                    unsigned delay)
{
    size_t   len = 0;
    u8       *txbuf8 = (u8 *)txbuf;
    u8       *rxbuf8 = (u8 *)rxbuf;

    if (!txbuf && !rxbuf)
        return -1;

    if (txbuf)
        len = i2c_write_blocking(i2c, addr, txbuf, n_tx, false);

    /* If it only needs to be written. */
    if (n_rx <= 0)
        goto skip_read;

    len = i2c_read_blocking(i2c, addr, rxbuf, n_rx, false);

skip_read:
    return (len == n_rx) ? -1 : len;
}

size_t mb85rc16_byte_write(struct i2c_inst *i2c, uint16_t addr, uint8_t val)
{
    uint8_t txbuf[] = {
        (addr & 0xff),
        (val)
    };

    return i2c_write_then_read(i2c, MB85RC16_ADDR(addr), txbuf, sizeof(txbuf),
                            NULL, 0, 0);
}

size_t mb85rc16_page_write(struct i2c_inst *i2c, uint16_t addr, uint8_t *txbuf, size_t len)
{
    uint8_t buf[MB85RC16_PAGE_SIZE + 1] = {0};

    if (len > MB85RC16_PAGE_SIZE) {
        printf("%s, invalid length\n", __func__);
        return -1;
    }

    buf[0] = (addr & 0xff);
    memcpy(buf + 1, txbuf, len);

    return i2c_write_then_read(i2c, MB85RC16_ADDR(addr), buf, sizeof(buf),
                            NULL, 0, 0);
}

size_t mb85rc16_current_addr_read(struct i2c_inst *i2c, uint8_t *val)
{
    return i2c_write_then_read(i2c, MB85RC16_DEV_CODE, NULL, 0,
                            val, 1, 0);
}

size_t mb85rc16_random_read(struct i2c_inst *i2c, uint16_t addr, uint8_t *val)
{
    uint8_t txbuf[] = {
        (addr & 0xff),
    };

    return i2c_write_then_read(i2c, MB85RC16_ADDR(addr), txbuf, sizeof(txbuf),
                            val, 1, 0);
}

size_t mb85rc16_seq_read(struct i2c_inst *i2c, uint16_t addr, uint8_t *rxbuf, size_t len)
{
    uint8_t txbuf[] = {
        (addr & 0xff),
    };

    return i2c_write_then_read(i2c, MB85RC16_ADDR(addr), txbuf, sizeof(txbuf),
                            rxbuf, len, 0);
}
