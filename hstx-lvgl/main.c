// Copyright (c) 2024 Raspberry Pi (Trading) Ltd.

// Drive a ST7789 SPI LCD using the HSTX. The SPI clock rate is fully
// independent of (and can be faster than) the system clock.

// You'll need an LCD module for this example. It was tested with: WaveShare
// 1.3 inch ST7789 module. Wire up the signals as per PIN_xxx defines below,
// and don't forget to connect GND and VCC to GND/3V3 on your board!
//
// Theory of operation: Each 32-bit HSTX record contains 3 x 8-bit fields:
//
// 27:20  CSn x8    (noninverted CSn pin)
// 17:10  !DC x 8   (inverted DC pin)
//  7: 0  data bits (DIN pin)
//
// SCK is driven by the HSTX clock generator. We do issue extra clocks whilst
// CSn is high, but this should be ignored by the display. Packing the
// control lines in the HSTX FIFO records makes it easy to drive them in sync
// with SCK without having to reach around and do manual GPIO wiggling.

#include "pico/stdlib.h"

#include "hardware/pll.h"
#include "hardware/vreg.h"
#include "hardware/clocks.h"

#include "lvgl.h"
#include "port/lv_port_disp.h"
#include "lvgl/demos/lv_demos.h"

int main() {
    /* NOTE: DO NOT MODIFY THIS BLOCK */
#define CPU_SPEED_MHZ (DEFAULT_SYS_CLK_KHZ / 1000)
    if(CPU_SPEED_MHZ > 266 && CPU_SPEED_MHZ <= 360)
        vreg_set_voltage(VREG_VOLTAGE_1_20);
    else if (CPU_SPEED_MHZ > 360 && CPU_SPEED_MHZ <= 396)
        vreg_set_voltage(VREG_VOLTAGE_1_25);
    else if (CPU_SPEED_MHZ > 396)
        vreg_set_voltage(VREG_VOLTAGE_MAX);
    else
        vreg_set_voltage(VREG_VOLTAGE_DEFAULT);

    set_sys_clock_khz(CPU_SPEED_MHZ * 1000, true);
    clock_configure(clk_peri,
                    0,
                    CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLK_SYS,
                    CPU_SPEED_MHZ * MHZ,
                    CPU_SPEED_MHZ * MHZ);
    stdio_init_all();

	lv_init();
	lv_port_disp_init();

	// lv_demo_benchmark();
	lv_demo_music();

	for (;;) {
		lv_timer_handler_run_in_period(1);
	}
}
