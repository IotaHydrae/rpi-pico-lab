/**
 * Copyright (c) 2023 embeddedboys.org
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#ifndef __KEYBOARD_H
#define __KEYBOARD_H

struct keyboard_config {
    const int col_pins[8];
    const int row_pins[8];
    const char map[64];
    const int num_rows;
    const int num_cols;

    /* add more config here */
    int scan_delay;    /* in ms */
};

int keyboard_init(const struct keyboard_config *keypad);
char keyboard_get_key(const struct keyboard_config *keypad);

#endif