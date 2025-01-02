// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) 2024 Iota Hydrae <writeforever@foxmail.com>
 */

#include "pico/time.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>

#include "pico/stdio.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"

typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;
static inline void dm_gpio_set_value(int *pin, int val)
{
    gpio_put(*pin, val);
}

static inline void mdelay(int val)
{
    sleep_ms(val);
}

/* rs=0 means writing register, rs=1 means writing data */
extern int i80_write_buf_rs(void *buf, size_t len, bool rs);

#include "hardware/vreg.h"
#include "hardware/clocks.h"

#define CPU_SPEED_MHZ 240

extern int i80_pio_init(void);

bool data_flip_callback(repeating_timer_t *t)
{
    uint32_t data = 0xffffffff;
    i80_write_buf_rs(&data, sizeof(data), 1);

    data = 0x00000000;
    i80_write_buf_rs(&data, sizeof(data), 1);

    return true;
}

int main()
{
    // vreg_set_voltage(VREG_VOLTAGE_DEFAULT);
    // set_sys_clock_khz(CPU_SPEED_MHZ * 1000, true);
    // clock_configure(clk_peri,
    //                 0,
    //                 CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLK_SYS,
    //                 CPU_SPEED_MHZ * MHZ,
    //                 CPU_SPEED_MHZ * MHZ);

    // i80_pio_init();

    // struct repeating_timer timer;
    // add_repeating_timer_ms(1, data_flip_callback, NULL, &timer);
    int led_pin = 25;
    // gpio_init(led_pin);
    // gpio_set_dir(led_pin, GPIO_OUT);
    gpio_init_mask(0xffffffff);
    gpio_set_dir_masked(0xffffffff, 0xffffffff);

    for (;;) {
        // gpio_put(led_pin, 1);
        // gpio_put(led_pin, 0);
        gpio_put_all(0xffffffff);
        sleep_ms(1);
        gpio_put_all(0);
        sleep_ms(1);
    }
    return 0;
}