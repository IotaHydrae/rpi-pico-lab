/*
 * RP2040 PCF8574 Keypad example
 *
 * By embeddedboys
*/

#include <stdio.h>
#include <pico/stdlib.h>

#include "keypad.h"

const static struct keypad_config default_keypad = {
    .row_pins = {0, 1, 2, 3},
    .col_pins = {4, 5, 6, 7},
    .map = {
        '7', '8', '9', 'A',
        '4', '5', '6', 'B',
        '1', '2', '3', 'C',
        '*', '0', '#', 'C',
    },
    .num_rows = 4,
    .num_cols = 4,
};


int main(void)
{
    stdio_uart_init_full(uart0, 115200, 16, 17);
    printf("RP2040 PCF8574 Keypad example\n");

    keypad_init(&default_keypad);

    uint8_t key;

    for(;;)
    {
        key = keypad_get_key(&default_keypad);
        if(!key)
            continue;

        switch(key)
        {
            case '1':
                printf("KEY1 down\n");
                break;
            case '2':
                printf("KEY2 down\n");
                break;
            case '3':
                printf("KEY3 down\n");
                break;
            case '4':
                printf("KEY4 down\n");
                break;
            case '5':
                printf("KEY5 down\n");
                break;
            case '6':
                printf("KEY6 down\n");
                break;
            case '7':
                printf("KEY7 down\n");
                break;
            case '8':
                printf("KEY8 down\n");
                break;
            case '9':
                printf("KEY9 down\n");
                break;
            case '0':
                printf("KEY0 down\n");
                break;
            case '*':
                printf("KEY* down\n");
                break;
            case '#':
                printf("KEY# down\n");
                break;
            case 'A':
                printf("KEYA down\n");
                break;
            case 'B':
                printf("KEYB down\n");
                break;
            case 'C':
                printf("KEYC down\n");
                break;
            case 'D':
                printf("KEYD down\n");
                break;
            default:
                break;
        }

        sleep_ms(30);
    }
}