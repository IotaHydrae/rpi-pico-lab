/**
 * @file epink.c
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
#include <stdio.h>
#include <errno.h>

#include "epink.h"

static struct epink_device *g_pt_epink_device = NULL;
/* For driver register */
static struct native_driver *g_pt_native_driver = NULL; /* must allways point to the first node */
static uint32_t g_driver_id = 0;

static struct epink_data *g_pt_epink_data = NULL;

int register_driver(struct native_driver *drv)
{
    struct native_driver *p_tmp;

    /* Check if driver pointer is legal */
    if (!drv)
        return 0;

    /* Oh, this is the first node of driver chain */
    if (!g_pt_native_driver)
        g_pt_native_driver = drv;
    /* YEEE, we got more than one driver registered in */
    else {
        p_tmp = g_pt_native_driver;

        while(p_tmp->p_next)
            p_tmp = p_tmp->p_next;

        p_tmp->p_next = drv;    /* registered */
    }

    drv->id = g_driver_id++;
    drv->p_next = NULL;
}

int register_device(struct epink_device *dev)
{

}