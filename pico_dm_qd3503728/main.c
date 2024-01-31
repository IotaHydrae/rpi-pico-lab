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
#include "hardware/timer.h"

#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"
#include "lvgl/examples/lv_examples.h"
#include "pico/time.h"
#include "porting/lv_port_disp_template.h"

extern int i80_pio_init(void);

bool lv_tick_timer_callback(struct repeating_timer *t)
{
    lv_timer_handler();
    return true;
}

#define PICO_FLASH_SPI_CLKDIV 2
#define CPU_SPEED_MHZ 280

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
    printf("\n\n\nfl350hvc03v10 LVGL Porting\n");

    i80_pio_init();

    i2c_tools_init();

    lv_init();
    lv_port_disp_init();

    printf("Starting demo\n");
    // lv_demo_widgets();
    lv_demo_stress();
    // lv_demo_music();
    // lv_demo_benchmark();
    
    struct repeating_timer timer;
    add_repeating_timer_ms(5, lv_tick_timer_callback, NULL, &timer);

    printf("going to loop, %lld\n", time_us_64());
    for (;;) {
        tight_loop_contents();
    }

    return 0;
}