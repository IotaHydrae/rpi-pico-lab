// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) 2023 Iota Hydrae <writeforever@foxmail.com>
 */

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>

#include "pico/time.h"
#include "pico/stdio.h"
#include "pico/stdlib.h"
#include "pico/platform.h"
#include "pico/stdio_uart.h"

#include "hardware/pll.h"
#include "hardware/vreg.h"
#include "hardware/clocks.h"
#include "hardware/timer.h"

#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"
#include "lvgl/examples/lv_examples.h"
#include "porting/lv_port_disp_template.h"
#include "porting/lv_port_indev_template.h"

#include "backlight.h"

bool lv_tick_timer_callback(struct repeating_timer *t)
{
    lv_timer_handler();
    return true;
}

extern int factory_test(void);

int main(void)
{
    /* NOTE: DO NOT MODIFY THIS BLOCK */
#define CPU_SPEED_MHZ (DEFAULT_SYS_CLK_KHZ / 1000)
    vreg_set_voltage(VREG_VOLTAGE_1_20);
    set_sys_clock_khz(CPU_SPEED_MHZ * 1000, true);
    clock_configure(clk_peri,
                    0,
                    CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLK_SYS,
                    CPU_SPEED_MHZ * MHZ,
                    CPU_SPEED_MHZ * MHZ);
    stdio_uart_init_full(uart0, 115200, 16, 17);


    printf("\n\n\nPICO DM QD3503728 LVGL Porting\n");

    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();

    printf("Starting demo\n");
    // lv_demo_widgets();
    // lv_demo_stress();
    // lv_demo_music();

    /* measure weighted fps and opa speed */
    lv_demo_benchmark();

    // factory_test();

    struct repeating_timer timer;
    add_repeating_timer_ms(5, lv_tick_timer_callback, NULL, &timer);

    sleep_ms(10);
    backlight_init();
    backlight_set_level(100);
    printf("backlight set to 100%%\n");

    printf("going to loop, %lld\n", time_us_64());
    for (;;) {
        tight_loop_contents();
        sleep_ms(200);
    }

    return 0;
}