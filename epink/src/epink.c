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
#include <string.h>
#include <errno.h>

#include "common.h"
#include "epink.h"

static struct epink_device *g_pt_epink_device = NULL;
/* For driver register */
static struct native_driver *g_pt_native_driver = NULL; /* must allways point to the first node */
static uint32_t g_driver_id = 0;

static struct epink_handler *g_pt_epink_handler = NULL;

int driver_to_handler(struct native_driver *drv)
{
    if(!drv)
        return 0;

    if(!drv->matched)
        return 0;

    return container_of(drv, struct epink_handler, drv);
}

int device_to_handler(struct epink_device *dev)
{
    if (!dev)
        return 0;
    
    if (!dev->matched)
        return 0;

    return container_of(dev, struct epink_handler, dev);
}

int register_handler(struct epink_device *dev, struct native_driver *drv)
{
    struct epink_handler *handler;
    struct epink_handler *p_tmp;

    /* malloc a epink handler */
    handler = (struct epink_handler *)malloc(sizeof(struct epink_handler));

    handler->name = dev->name;
    handler->dev = dev;
    handler->drv = drv;

    /* register it into handler chain */
    if (!g_pt_epink_handler)
        g_pt_epink_handler = handler;

    else {
        p_tmp = g_pt_epink_handler;

        while(p_tmp->p_next)
            p_tmp = p_tmp->p_next;

        p_tmp = handler;
    }

    handler->p_next = NULL;
}

/**
 * @brief for each driver or device register action, call this
 * 
 * @param dev 
 * @param drv 
 * @return int 
 */
int driver_match_device(struct epink_device *dev, struct native_driver *drv)
{
    struct epink_device *p_tmp_dev;
    struct native_driver *p_tmp_drv;
    /* if non of them exists */
    if (!dev && !drv)
        return 0;
    
    /* If a device was given */
    if (dev && !drv) {
        if (!dev->matched) {
            p_tmp_drv = g_pt_native_driver;

            while (p_tmp_drv->p_next) {
                if(0 == strcmp(dev->name, p_tmp_drv->name)) {
                    /* drv and dev matched */
                    dev->matched = 1;
                    p_tmp_drv->matched = 1;
                    register_handler(dev, drv);
                } else {
                    p_tmp_drv = p_tmp_drv->p_next;
                }

            }
        }
    } else if (!dev && drv) { /* If a driver was given */
        if (!drv->matched) {
            p_tmp_dev = g_pt_epink_device;

            while(p_tmp_dev->p_next) {
                if(0 == strcmp(drv->name, p_tmp_dev->name)){
                    /* drv and dev matched */
                    drv->matched = 1;
                    p_tmp_dev->matched = 1;
                    register_handler(dev, drv);
                } else {
                    p_tmp_dev = p_tmp_dev->p_next;
                }
            }
        }
    }
}

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
    struct epink_device *p_tmp;

    if(!dev)
        return 0;
    
    if(!g_pt_epink_device)
        g_pt_epink_device = dev;

    else {
        p_tmp = g_pt_epink_device;
        while(p_tmp->p_next)
            p_tmp = p_tmp->p_next;
        
        p_tmp->p_next = dev;
    }

    dev->p_next = NULL;
}