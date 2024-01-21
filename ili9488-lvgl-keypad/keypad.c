/*
MIT License

Copyright (c) [year] [author]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/time.h"

#define MAX_ROWS    8
#define MAX_COLS    8

struct keyboard_config {
    const int col_pins[8];
    const int row_pins[8];
    const char map[64];
    const int num_rows;
    const int num_cols;

    /* add more config here */
    int scan_delay;    /* in ms */
};

extern int pcf8574_driver_init(void);
extern int pcf8574_gpio_put(int pin, bool val);
extern int pcf8574_gpio_put_all(uint8_t val);
extern bool pcf8574_gpio_get(int pin);
extern uint8_t pcf8574_gpio_get_all(void);

// const struct keyboard_config default_keyboard = {
//     .col_pins = {4, 5, 6, 7},
//     .row_pins = {0, 1, 2, 3},
//     .map = {
//         '7', '8', '9', 'A',
//         '4', '5', '6', 'B',
//         '1', '2', '3', 'C',
//         '-', '0', '-', 'C',
//     },
//     .num_rows = 4,
//     .num_cols = 4,
// };

static int __check_config(const struct keyboard_config *keyboard)
{
    if (keyboard->num_rows > MAX_ROWS) {
        printf("Error: number of rows exceeds maximum\n");
        return -1;
    }

    if (keyboard->num_cols > MAX_COLS) {
        printf("Error: number of columns exceeds maximum\n");
        return -1;
    }

    if (keyboard->num_rows * keyboard->num_cols > MAX_ROWS * MAX_COLS) {
        printf("Error: number of keys exceeds maximum\n");
        return -1;
    }

    return 0;
}

int keyboard_init(const struct keyboard_config *keyboard)
{
    if (__check_config(keyboard) < 0) {
        printf("Error: keyboard configuration error\n");
        return -1;
    }

    pcf8574_driver_init();

    printf("keypad init done\n");
    return 0;
}

char keyboard_get_key(const struct keyboard_config *keyboard)
{
    char key = 0;
    int scan_delay = 20;   // scan period in ms

    if (keyboard->scan_delay > 0)
        scan_delay = keyboard->scan_delay;
#if 0
    for (int i = 0; i < keyboard->num_cols; i++) {

        /* set current column to low, then check rows foreach */
        pcf8574_gpio_put(keyboard->col_pins[i], 0);
        busy_wait_us(50);   // wait for the pin be stable

        for (int j = 0; j < keyboard->num_rows; j++) {
            if (pcf8574_gpio_get(keyboard->row_pins[j]) == 0) {

                /*
                 * we need to set these pin state manually, because
                 * pcf8574t is an open-drain device, it got MOSFETs
                 * inside. when these IO ports working in read mode,
                 * the pin state won't change if we don't set it.
                 * You know, MOSFET is a voltage-controlled device.
                 */
                pcf8574_gpio_put(keyboard->col_pins[i], 1);
                pcf8574_gpio_put(keyboard->row_pins[j], 1);
                
                /* according the row and column, return the key value in map */
                key = keyboard->map[j * keyboard->num_rows + i];
                return key;
            }
        }
        pcf8574_gpio_put(keyboard->col_pins[i], 1);

        /* if you don't want the scan routine to run too fast. */
        busy_wait_ms(keyboard->scan_delay);
    }
#else
    for (int i = 0; i < keyboard->num_rows; i++) {

        /* set current row to low, then check colum foreach */
        pcf8574_gpio_put(keyboard->row_pins[i], 0);
        busy_wait_us(50);   // wait for the pin be stable

        for (int j = 0; j < keyboard->num_cols; j++) {
            if (pcf8574_gpio_get(keyboard->col_pins[j]) == 0) {

                /*
                 * we need to set these pin state manually, because
                 * pcf8574t is an open-drain device, it got MOSFETs
                 * inside. when these IO ports working in read mode,
                 * the pin state won't change if we don't set it.
                 * You know, MOSFET is a voltage-controlled device.
                 */
                pcf8574_gpio_put(keyboard->col_pins[j], 1);
                pcf8574_gpio_put(keyboard->row_pins[i], 1);
                
                /* according the row and column, return the key value in map */
                key = keyboard->map[i * keyboard->num_cols + j];
                return key;
            }
        }
        pcf8574_gpio_put(keyboard->row_pins[i], 1);

        /* if you don't want the scan routine to run too fast. */
        busy_wait_ms(keyboard->scan_delay);
    }
#endif
    return 0;
}