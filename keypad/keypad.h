/**
 * Copyright (c) 2023 embeddedboys.org
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#ifndef __KEYPAD_H
#define __KEYPAD_H

struct keypad_config {
    const int col_pins[8];
    const int row_pins[8];
    const char map[64];
    const int num_rows;
    const int num_cols;

    /* add more config here */
    int scan_delay;    /* in ms */
};

int keypad_init(const struct keypad_config *keypad);
char keypad_get_key(const struct keypad_config *keypad);

#endif