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

#ifndef __MB85RC16_H
#define __MB85RC16_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

/*
 * The MB85RC16 is an FRAM (Ferroelectric Random Access Memory) chip
 * in a configuration of 2,048 words x 8 bits
 */

/* MB85RC16 specific definitions */
#define MB85RC16_SPEED_KHZ      1000
#define MB85RC16_PAGE_SIZE      256  // Bytes
#define MB85RC16_PAGE_COUNT     8
#define MB85RC16_TOTAL_SIZE     (MB85RC16_PAGE_COUNT * MB85RC16_PAGE_SIZE) // Bytes

#define MB85RC16_ADDR_MASK      0x3FF

// TODO: Add explanation of the MB85RC16 commands
size_t mb85rc16_byte_write(struct i2c_inst *i2c, uint16_t addr, uint8_t val);
size_t mb85rc16_page_write(struct i2c_inst *i2c, uint16_t addr, uint8_t *txbuf, size_t len);
size_t mb85rc16_current_addr_read(struct i2c_inst *i2c, uint8_t *val);
size_t mb85rc16_random_read(struct i2c_inst *i2c, uint16_t addr, uint8_t *val);
size_t mb85rc16_seq_read(struct i2c_inst *i2c, uint16_t addr, uint8_t *rxbuf, size_t len);

// TODO: Supports writing more than 256 bytes

#ifdef __cplusplus
}
#endif

#endif /* __MB85RC16_H */