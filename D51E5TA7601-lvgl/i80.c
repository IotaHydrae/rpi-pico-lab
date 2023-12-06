// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) 2023 embeddedboys <writeforever@foxmail.com>
 */

#include "boards/pico.h"
#include "pico/stdlib.h"
#include "hardware/pio.h"

typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

// Our assembled program:
#include "generated/i80.pio.h"

