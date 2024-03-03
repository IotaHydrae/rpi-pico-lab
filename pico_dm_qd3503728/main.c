// Copyright (c) 2024 embeddedboys developers

// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:

// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

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
    if(CPU_SPEED_MHZ > 266 && CPU_SPEED_MHZ <= 396)
        vreg_set_voltage(VREG_VOLTAGE_1_20);
    else if (CPU_SPEED_MHZ > 396)
        vreg_set_voltage(VREG_VOLTAGE_1_25);
    else
        vreg_set_voltage(VREG_VOLTAGE_DEFAULT);

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

    /* This is a factory test app */
    // factory_test();

    struct repeating_timer timer;
    add_repeating_timer_ms(1, lv_tick_timer_callback, NULL, &timer);

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