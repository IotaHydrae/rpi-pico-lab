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
#include "porting/lv_port_indev_template.h"

#include "pwm-tone.h"   // Include the library
#include "melodies.h"   // Optional, but ideal location to store custom melodies
// Pin definitions
#define PIEZO_PIN       22 // The pin the buzzer or speaker is connected to.
                          // The other terminal of the buzzer is connected to ground.

extern int i80_pio_init(void);

bool lv_tick_timer_callback(struct repeating_timer *t)
{
    lv_timer_handler();
    return true;
}

#define PICO_FLASH_SPI_CLKDIV 2
#define CPU_SPEED_MHZ 280

tonegenerator_t generator;

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
    // Initialize the tone generator, assigning it the output pin
    tone_init(&generator, PIEZO_PIN);
    // Use this function to speed up or down your melodies.
    // Default tempo is 120bpm. Tempo does not affect tone().
    set_tempo(120);

    // This is an example sound effect. Each note defines a pitch (float, in Hz)
    // and a duration (expressed in subdivisions of a whole note). This means that
    // a duration of 16 is half a duration of 8. Negative values represent dotted notation,
    // so that -8 = 8 + (8/2) = 12. This data structure is inspired by the work at
    // https://github.com/robsoncouto/arduino-songs/
    note_t sfx[] = {
        {NOTE_C4, 16},
        {NOTE_C5, 32},
        {NOTE_C6, 64},
        {REST, 8}, // Pause at the end to space out repeats of the melody
        {MELODY_END, 0}, // Melody end code. Necessary to trigger repeats
    };

    // Let's play the sfx we just defined, repeating it twice
    melody(&generator, sfx, 3);
    while(generator.playing) { sleep_ms(2); }

    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();

    printf("Starting demo\n");
    // lv_demo_widgets();
    // lv_demo_stress();
    lv_demo_keypad_encoder();
    // lv_demo_music();
    // lv_demo_benchmark();
    
    struct repeating_timer timer;
    add_repeating_timer_ms(5, lv_tick_timer_callback, NULL, &timer);

    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    printf("going to loop, %lld\n", time_us_64());
    for (;;) {
        gpio_put(PICO_DEFAULT_LED_PIN, 1);
        sleep_ms(200);
        gpio_put(PICO_DEFAULT_LED_PIN, 0);
        sleep_ms(200);
    }

    return 0;
}