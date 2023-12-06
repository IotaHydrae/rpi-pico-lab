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

int main(void)
{   
    vreg_set_voltage(VREG_VOLTAGE_1_05);
    set_sys_clock_khz(240000, true);
    clock_configure(clk_peri,
                    0,
                    CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLK_SYS,
                    240 * MHZ,
                    240 * MHZ);

    stdio_uart_init_full(uart0, 115200, 16, 17);
    printf("\n\n\n\nD51E5TA7601 LVGL Porting\n");

    lv_init();
    lv_port_disp_init();

    lv_demo_widgets();

    for (;;) {
        lv_timer_handler();
        sleep_us(5000);
        lv_tick_inc(5);
    }
}