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

#include "hardware/clocks.h"
#include "hardware/pio.h"
#include "pico/stdio.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include <stdlib.h>

// Our assembled program:
#include "ws2812b.pio.h"

/* Shape of Leds martix */
#define LED_COLUMS 8
#define LED_ROWS   8
#define LED_COUNT  (LED_ROWS * LED_COLUMS)

#define WS2812B_DATA_PIN 0

#define WS2812B_RGB(r, g, b) (((g << 16) | (r << 8) | (b)) << 8)
#define WS2812B_RED	     WS2812B_RGB(0xFF, 0x00, 0x00)
#define WS2812B_GREEN	     WS2812B_RGB(0x00, 0xFF, 0x00)
#define WS2812B_BLUE	     WS2812B_RGB(0x00, 0x00, 0xFF)
#define WS2812B_BLACK	     WS2812B_RGB(0x00, 0x00, 0x00)
#define WS2812B_WHITE	     WS2812B_RGB(0xFF, 0xFF, 0xFF)
#define WS2812B_YELLOW	     WS2812B_RGB(0xFF, 0xFF, 0x00)
#define WS2812B_PURPLE	     WS2812B_RGB(0xFF, 0x00, 0xFF)
#define WS2812B_CYAN	     WS2812B_RGB(0x00, 0xFF, 0xFF)
#define WS2812B_ORANGE	     WS2812B_RGB(0xFF, 0x78, 0x00)
#define WS2812B_PINK	     WS2812B_RGB(0xFF, 0x00, 0x78)
#define WS2812B_SKYBLUE	     WS2812B_RGB(0x00, 0x78, 0xFF)
#define WS2812B_SOFT_WHITE   WS2812B_RGB(0x50, 0x50, 0x50)
#define WS2812B_SOFT_RED     WS2812B_RGB(0x3C, 0x00, 0x00)
#define WS2812B_SOFT_GREEN   WS2812B_RGB(0x00, 0x3C, 0x00)
#define WS2812B_SOFT_BLUE    WS2812B_RGB(0x00, 0x00, 0x3C)
#define WS2812B_SOFT_PINK    WS2812B_RGB(0x3C, 0x00, 0x1E)

PIO pio;
uint sm;
uint32_t framebuffer[LED_COUNT];

static inline uint32_t to_ws2812b_grb(uint8_t r, uint8_t g, uint8_t b)
{
	return ((g << 16) | (r << 8) | (b)) << 8;
}

static inline void ws2812b_clear(uint32_t color)
{
	for (int i = 0; i < LED_COUNT; i++)
		framebuffer[i] = color;
}

static inline void ws2812b_set_pixel(int x, int y, uint32_t color)
{
	if (x < 0 || x >= LED_COLUMS || y < 0 || y >= LED_ROWS)
		return;

	framebuffer[LED_ROWS * y + x] = color;
}

static inline void ws2812b_flush(void)
{
	for (int i = 0; i < LED_COUNT; i++)
		pio_sm_put_blocking(pio, sm, framebuffer[i]);

	pio_ws2812b_wait_idle(pio, sm);
}

int main()
{
	stdio_init_all();

	// set_sys_clock_48mhz();
	printf("%d\n", clock_get_hz(clk_sys));

	printf("pico pio ws2812b example\n");
	// Pick one PIO instance arbitrarily. We're also arbitrarily picking state
	// machine 0 on this PIO instance (the state machines are numbered 0 to 3
	// inclusive).
	pio = pio0;
	sm = pio_claim_unused_sm(pio, true);

	uint offset = pio_add_program(pio, &test_program);

	/* PIO operates at a fixed frequency, approximately 166ns per cycle */
	pio_ws2812b_program_init(pio, sm, offset, WS2812B_DATA_PIN,
				 clock_get_hz(clk_sys));

	uint32_t colors[] = { WS2812B_RED,	 WS2812B_GREEN,
			      WS2812B_BLUE,	 WS2812B_BLACK,
			      WS2812B_WHITE,	 WS2812B_YELLOW,
			      WS2812B_PURPLE,	 WS2812B_CYAN,
			      WS2812B_ORANGE,	 WS2812B_PINK,
			      WS2812B_SKYBLUE,	 WS2812B_SOFT_WHITE,
			      WS2812B_SOFT_RED,	 WS2812B_SOFT_GREEN,
			      WS2812B_SOFT_BLUE, WS2812B_SOFT_PINK };
#define COLOR_COUNT (sizeof(colors) / sizeof(colors[0]))

	for (;;) {
		static int last_x, last_y;

		for (int y = 0; y < LED_ROWS; y++) {
			for (int x = 0; x < LED_COLUMS; x++) {
				ws2812b_set_pixel(last_x, last_y,
						  WS2812B_BLACK);
				ws2812b_set_pixel(x, y,
						  colors[rand() % COLOR_COUNT]);
				ws2812b_flush();

				last_x = x;
				last_y = y;

				sleep_ms(100);
			}
		}
	}

	return 0;
}