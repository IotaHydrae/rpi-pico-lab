/**
 * @file epink.h
 * @author IotaHydrae (writeforever@foxmail.com)
 * @brief file of the epink-1.54.
 * @version 0.2
 * @date 2022-08-14
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

#ifndef __EPINK_H
#define __EPINK_H

#include "port.h"

/* Pins Define of rpi-pico
 *
 * The SPI interface using default spi0
 *
 * RES  <->  GP14
 * DC   <->  GP15
 * BUSY <->  GP20
 */
#define EPINK_RES_PIN   14
#define EPINK_DC_PIN    15
#define EPINK_BUSY_PIN  20

/* ========== epink panel info ========== */
#define EPINK_WIDTH         200
#define EPINK_HEIGHT        200
#define EPINK_PAGE_SIZE     8
#define EPINK_LINE_WIDTH_IN_PAGE (EPINK_WIDTH/EPINK_PAGE_SIZE)
#define EPINK_BPP 1
#define EPINK_COLOR_WHITE 0xFF
#define EPINK_COLOR_BLACK 0x00

#define EPINK_DISP_BUFFER_SIZE (EPINK_WIDTH*EPINK_HEIGHT/8)
#define EPINK_DISP_BUFFER_OFFSET(p,x)(p*EPINK_WIDTH+x)

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

// #define EPINK_DEBUG_MODE
#define EPINK_COORD_CHECK
#ifdef EPINK_DEBUG_MODE
    #define EPINK_DEBUG(...) printf(__VA_ARGS__)
#else
    #define EPINK_DEBUG
#endif

#define TEST_DOC "This document describes how to write an ALSA \
(Advanced Linux Sound Architecture) driver. The document focuses \
mainly on PCI soundcards. In the case of other device types, the \
API might be different, too. However, at least the ALSA kernel \
API is consistent, and therefore it would be still a bit help \
for writing them."

#define TEST_DOC2 "This variable provides a means of enabling or \
disabling features of a recipe on a per-recipe basis. PACKAGECONFIG \
blocks are defined in recipes when you specify features and then \
arguments that define feature behaviors."

typedef enum {
    EPINK_UPDATE_MODE_FULL = 0x00,
    EPINK_UPDATE_MODE_PART = 0x01,
}epink_update_mode_t;

typedef enum {
    EPINK_FONT_4X16 = 0x00,
    EPINK_FONT_8x16 = 0x01,
}epink_font_t;

struct epink_config {
    /* panel config */
    uint16_t width;
    uint16_t height;
    uint8_t bpp; 

    uint8_t update_mode;
    uint8_t font;
};

struct epink_operations {
    void (*device_init)(uint8_t mode);
    void (*init)(uint8_t mode);

    void (*reset)(void);
};

struct epink_device {
    struct epink_config *cfg;
    struct epink_operations *opr;
};

struct epink_data {
    struct epink_device *dev;
    struct epink_driver *drv;
};

/* Global functions */
/* Marcos */
#define register_platform_driver(drv) \
    static struct native_driver drv##_platform_driver = { \
        .name   = #drv, \
        .config = &drv##_config, \
        .iface  = &drv##_interface, \
    }; \
    static void __attribute__((constructor)) drv##_platform_driver_register(void) \
    { \
        register_driver(&drv##_platform_driver); \
    }

void register_driver(struct native_driver *drv);


void epink_disp_port_init(void);

#endif  /* __EPINK_H */
