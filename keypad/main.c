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

struct keypad_config default_cfg = {
    .col_pins = {11, 10, 9, 8},
    .row_pins = {7, 6, 5, 4},
    .map = {
        '1', '2', '3', 'A',
        '4', '5', '6', 'B',
        '7', '8', '9', 'C',
        '*', '0', '#', 'D',
    },
    .num_rows = 4,
    .num_cols = 4,
    .scan_delay = 100,
};
int main()
{
    stdio_init_all();
    printf("This is a simple keypad test\n");

    keypad_init(&default_cfg);
    
    char key;
    for (;;) {
        key = keypad_get_key(&default_cfg);
        if (key != 0)
            printf("key pressed: %c\n", key);
    }

    return 0;
}
