/**
 * @file bh1750.h
 * @author IotaHydrae (writeforever@foxmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-08-04
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

#ifndef __BH1750_H
#define __BH1750_H

#define BIT(x) (1 << x)
#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))

#define BH1750_ADDR_H 0x23
#define BH1750_ADDR_L

/* bh1750 instruction set */
#define BH1750_POWER_DOWN                   0x00
#define BH1750_POWER_ON                     0x01
#define BH1750_RESET                        0x07

/*
 * Measurement mode explanation
Measurement Mode    Measurement Time.   Resolurtion
H-resolution Mode2  Typ. 120ms.         0.5 lx
H-Resolution Mode   Typ. 120ms.         1 lx.
L-Resolution Mode   Typ. 16ms.          4 lx.
*/
#define BH1750_CONTINUOUS_HIGH_RES_MODE     0x10
#define BH1750_CONTINUOUS_HIGH_RES_MODE_2   0x11
#define BH1750_CONTINUOUS_LOW_RES_MODE      0x13

#define BH1750_ONE_TIME_HIGH_RES_MODE       0x20
#define BH1750_ONE_TIME_HIGH_RES_MODE_2     0x21
#define BH1750_ONE_TIME_LOW_RES_MODE        0x23

#define BH1750_CHANGE_MEASUREMENT_TIME_HIGH_BIT
#define BH1750_CHANGE_MEASUREMENT_TIME_LOW_BIT



#endif /* __BH1750_H */