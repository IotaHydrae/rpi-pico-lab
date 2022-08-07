/**
 * @file aht10.c
 * @author IotaHydrae (writeforever@foxmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-07-31
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

#include "aht10.h"

int aht10_reset(void)
{
    int rc;
	uint8_t wbuf[1] = {0xe1};
	rc = i2c_write_blocking(i2c_default, 0x38, wbuf, 1, false);
    if (rc == PICO_ERROR_GENERIC) {
        printf("%s: Device not responding\n", __func__);
        return rc;
    }
    
    return rc;
}

int aht10_read_raw(uint8_t *rbuf, int len)
{
    int rc;
    uint8_t wbuf[3] = {
        AHT10_CMD_MEASURE,
        AHT10_CMD_READ,
        AHT10_CMD_DUMMY
    };

    rc = i2c_write_blocking(i2c_default, AHT10_ADDRESS, wbuf, 3, false);
    if (rc == PICO_ERROR_GENERIC) {
        printf("%s: Device not responding\n", __func__);
        return rc;
    }

    rc = i2c_read_blocking(i2c_default, AHT10_ADDRESS, rbuf, len, false);
    if (rc == PICO_ERROR_GENERIC) {
        printf("%s: Device not responding\n", __func__);
        return rc;
    }

    return rc;
}

