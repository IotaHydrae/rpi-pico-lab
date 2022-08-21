/**
 * @file port.h
 * @author IotaHydrae (writeforever@foxmail.com)
 * @brief file of the epink-1.54.
 * @version 0.2
 * @date 2022-08-21
 *
 * Hi, guys!
 *
 * This is a simple ALL-IN-ONE driver for the LuatOS epink-1.54 screen module.
 *
 * the device init function in this file was based on :
 * https://gitee.com/openLuat/LuatOS/blob/master/components/epaper/EPD_1in54.c
 *
 * More info about the epaper module can be found in :
 * https://wiki.luatos.com/peripherals/eink_1.54/index.html
 *
 * Special thanks to :
 *   LuatOS (https://gitee.com/openLuat/LuatOS)
 *
 * MIT License
 *
 * Copyright (c) 2022 LuatOS
 * Copyright (c) 2022 IotaHydrae(writeforever@foxmail.com)
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

#ifndef __PORT_H
#define __PORT_H

#include <stdint.h>
#include "epink.h"

struct native_config {
    /* hardware */
    uint8_t pin_res;
    uint8_t pin_dc;
    uint8_t pin_cs;
    uint8_t pin_busy;
    uint8_t spi_iface;  /* which SPI interface actual in use */
    uint32_t spi_speed; /* Baud rate of SPI */
};

struct native_interface {
    void (*hal_init)(struct native_config *cfg);

    void (*cs_select)(struct native_config *cfg);
    void (*cs_deselect)(struct native_config *cfg);

    void (*dc_set)(struct native_config *cfg);
    void (*dc_clr)(struct native_config *cfg);
    
    void (*res_set)(struct native_config *cfg);
    void (*res_clr)(struct native_config *cfg);

    void (*wait_busy)(struct native_config *cfg);

    /* write a byte though SPI interface */
    void (*write_byte)(struct native_config *cfg, uint8_t byte);
    
    void (*write_command)(struct native_config *cfg, uint8_t command);
    void (*write_data)(struct native_config *cfg, uint8_t data);
};

struct native_driver {
    uint8_t id;
    uint8_t *name;
    uint8_t matched;

    struct native_config *config;
    struct native_interface *iface;

    /* register into driver chain */
    struct native_driver *p_next;
};

#endif /* __PORT_H */