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

    lv_init();
    lv_port_disp_init();

    printf("Starting demo\n");
    // lv_demo_widgets();
    // lv_demo_stress();
    // lv_demo_music();
    lv_demo_benchmark();
    
    struct repeating_timer timer;
    add_repeating_timer_ms(5, lv_tick_timer_callback, NULL, &timer);

#if 1
    volatile unsigned long *p = (unsigned long *)0x10000000;
    int dump_len = 64;
    while (dump_len--) {
        printf("%02lx ", *p++);
        if (dump_len % 16)
            printf("\n");
    }
    printf("\n");
#else

#define MPU_TYPE (volatile unsigned long *)(PPB_BASE + 0xed90)
#define MPU_CTRL (volatile unsigned long *)(PPB_BASE + 0xed94)
#define MPU_RNR  (volatile unsigned long *)(PPB_BASE + 0xed98)
#define MPU_RBAR (volatile unsigned long *)(PPB_BASE + 0xed9c)
#define MPU_RASR (volatile unsigned long *)(PPB_BASE + 0xeda0)

    volatile unsigned long *p;
    p = MPU_TYPE;
    printf("MPU_TYPE: 0x%08lx\n", *p);

    /* Setup and enable MPU */

    /* 1. Setup the base address of MPU region */
    p = MPU_RNR;
    *p = 0;

    /* read data from flash via SSI */
    p = (unsigned long *)0x10000000;
    int dump_len = 64;
    while (dump_len--) {
        printf("%02lx ", *p++);
        if (dump_len % 16)
            printf("\n");
    }
    printf("\n");

    /* we write into a flash address to test if MPU RO setting is okay. */
    printf("attempting write flash...\n");
    p = (unsigned long *)(0x10000000 + 0x500);
    *p = 0x12345678;
    printf("write flash passed! (MPU hardfault not triggered)");
#endif

    printf("going to loop, %lld\n", time_us_64());
    for (;;) {
        tight_loop_contents();
    }

    return 0;
}