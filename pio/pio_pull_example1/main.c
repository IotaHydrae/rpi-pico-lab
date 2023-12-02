/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

// Output a 12.5 MHz square wave (if system clock frequency is 125 MHz).
//
// Note this program is accessing the PIO registers directly, for illustrative
// purposes. We pull this program into the datasheet so we can talk a little
// about PIO's hardware register interface. The `hardware_pio` SDK library
// provides simpler or better interfaces for all of these operations.
//
// _*This is not best practice! I don't want to see you copy/pasting this*_
//
// For a minimal example of loading and running a program using the SDK
// functions (which is what you generally want to do) have a look at
// `hello_pio` instead. That example is also the subject of a tutorial in the
// SDK book, which walks you through building your first PIO program.

#include <stdio.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
// Our assembled program:
#include "generated/test.pio.h"
#include "pico/time.h"

#define IS_RGBW false
#define NUM_PIXELS 64

#define SIG_OUT_PIN 22

static inline void put_pixel(uint32_t pixel_grb) {
    pio_sm_put_blocking(pio0, 0, pixel_grb);
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return
            ((uint32_t) (r) << 8) |
            ((uint32_t) (g) << 16) |
            (uint32_t) (b);
}

void pattern_snakes(uint len, uint t) {
    for (uint i = 0; i < len; ++i) {
        uint x = (i + (t >> 1)) % 64;
        if (x < 10)
            put_pixel(urgb_u32(0xff, 0, 0));
        else if (x >= 15 && x < 25)
            put_pixel(urgb_u32(0, 0xff, 0));
        else if (x >= 30 && x < 40)
            put_pixel(urgb_u32(0, 0, 0xff));
        else
            put_pixel(0);
    }
}

void pattern_random(uint len, uint t) {
    if (t % 8)
        return;
    for (int i = 0; i < len; ++i)
        put_pixel(rand());
}

void pattern_sparkle(uint len, uint t) {
    if (t % 8)
        return;
    for (int i = 0; i < len; ++i)
        put_pixel(rand() % 16 ? 0 : 0xffffffff);
}

void pattern_greys(uint len, uint t) {
    int max = 100; // let's not draw too much current!
    t %= max;
    for (int i = 0; i < len; ++i) {
        put_pixel(t * 0x10101);
        if (++t >= max) t = 0;
    }
}

typedef void (*pattern)(uint len, uint t);
const struct {
    pattern pat;
    const char *name;
} pattern_table[] = {
        // {pattern_snakes,  "Snakes!"},
        // {pattern_random,  "Random data"},
        {pattern_sparkle, "Sparkles"},
        {pattern_greys,   "Greys"},
};

int main() {
    stdio_init_all();

    printf("WS2812 Smoke Test, using pin %d", SIG_OUT_PIN);
    // Pick one PIO instance arbitrarily. We're also arbitrarily picking state
    // machine 0 on this PIO instance (the state machines are numbered 0 to 3
    // inclusive).
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &test_program);

    // uint sm = pio_claim_unused_sm(pio, true);
    
    test_program_init(pio, sm, offset, SIG_OUT_PIN, 800000, IS_RGBW);


    int t = 0;
    while (1) {
        int pat = rand() % count_of(pattern_table);
        int dir = (rand() >> 30) & 1 ? 1 : -1;
        puts(pattern_table[pat].name);
        puts(dir == 1 ? "(forward)" : "(backward)");
        for (int i = 0; i < 1000; ++i) {
            pattern_table[pat].pat(NUM_PIXELS, t);
            sleep_ms(10);
            t += dir;
        }
    }
    return 0;

}