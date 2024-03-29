// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) 2023 Iota Hydrae <writeforever@foxmail.com>
 */

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>

#include "pico/platform.h"
#include "pico/stdio.h"
#include "pico/stdio_uart.h"
#include "pico/stdlib.h"
#include "hardware/pll.h"
#include "hardware/vreg.h"
#include "hardware/clocks.h"

#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"
#include "porting/lv_port_disp_template.h"
#include "porting/lv_port_indev_template.h"

bool lv_tick_timer_callback(struct repeating_timer *t)
{
    lv_tick_inc(5);
    return true;
}

#define PICO_FLASH_SPI_CLKDIV 2
#define CPU_SPEED_MHZ 240

int main(void)
{   
    vreg_set_voltage(VREG_VOLTAGE_DEFAULT);
    set_sys_clock_khz(CPU_SPEED_MHZ * 1000, true);
    clock_configure(clk_peri,
                    0,
                    CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLK_SYS,
                    CPU_SPEED_MHZ * MHZ,
                    CPU_SPEED_MHZ * MHZ);

    stdio_uart_init_full(uart0, 115200, 16, 17);
    printf("\n\n\n\nD51E5TA7601 LVGL Porting\n");

    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();


    printf("starting lvgl demo\n");
    // lv_demo_stress();
    // lv_demo_benchmark();
    lv_demo_keypad_encoder();

    struct repeating_timer timer;
    add_repeating_timer_ms(5, lv_tick_timer_callback, NULL, &timer);

    for (;;) {
        lv_timer_handler();
        sleep_ms(5);
        // tight_loop_contents();
    }

    return 0;
}
