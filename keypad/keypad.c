/**
 * Copyright (c) 2023 embeddedboys.org
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/time.h"

#include "keypad.h"

#define MAX_ROWS    8
#define MAX_COLS    8

const struct keypad_config default_keypad = {
    .col_pins = {11, 10, 9, 8},
    .row_pins = {7, 6, 5, 4},
    .map = {
        '1', '2', '3', 'A',
        '4', '5', '6', 'B',
        '7', '8', '9', 'C',
        '*', '0', '#', 'D',
    },
    .num_rows = 4,
    .num_cols = 4
};

static int __check_config(const struct keypad_config *keypad)
{
    if (keypad->num_rows > MAX_ROWS) {
        printf("Error: number of rows exceeds maximum\n");
        return -1;
    }

    if (keypad->num_cols > MAX_COLS) {
        printf("Error: number of columns exceeds maximum\n");
        return -1;
    }

    if (keypad->num_rows * keypad->num_cols > MAX_ROWS * MAX_COLS) {
        printf("Error: number of keys exceeds maximum\n");
        return -1;
    }

    return 0;
}

int keypad_init(const struct keypad_config *keypad)
{
    if (__check_config(keypad) < 0) {
        printf("Error: keypad configuration error\n");
        return -1;
    }

    for (int i = 0; i < keypad->num_rows; i++) {
        gpio_init(keypad->row_pins[i]);
        gpio_set_dir(keypad->row_pins[i], GPIO_IN);
        gpio_pull_up(keypad->row_pins[i]);
    }

    for (int i = 0; i < keypad->num_cols; i++) {
        gpio_init(keypad->col_pins[i]);
        gpio_set_dir(keypad->col_pins[i], GPIO_OUT);
        gpio_put(keypad->col_pins[i], 1);
    }

    return 0;
}

char keypad_get_key(const struct keypad_config *keypad)
{
    char key = 0;
    int scan_delay = 50;   // scan period in ms

    if (keypad->scan_delay > 0)
        scan_delay = keypad->scan_delay;

    for (int i = 0; i < keypad->num_cols; i++) {
        gpio_put(keypad->col_pins[i], 0);
        sleep_us(50);   // wait for the pin to settle

        for (int j = 0; j < keypad->num_rows; j++) {
            if (gpio_get(keypad->row_pins[j]) == 0) {
                key = keypad->map[j * keypad->num_cols + i];
                sleep_ms(scan_delay);
            }
        }
        gpio_put(keypad->col_pins[i], 1);
    }
    return key;
}