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
#include "boards/pico.h"
#include "pico/stdio.h"
#include "pico/stdlib.h"
#include "hardware/pio.h"

// Our assembled program:
#include "generated/test.pio.h"
#include "pico/time.h"

#define SIG_OUT_PIN 22

int main() {
    stdio_init_all();

    printf("pico pio hello\n");
    // Pick one PIO instance arbitrarily. We're also arbitrarily picking state
    // machine 0 on this PIO instance (the state machines are numbered 0 to 3
    // inclusive).
    PIO pio = pio0;

    uint offset = pio_add_program(pio, &test_program);

    uint sm = pio_claim_unused_sm(pio, true);
    
    test_program_init(pio, sm, offset, SIG_OUT_PIN);

    uint8_t data = 0x55;

    while (true) {
        test_wait_idle(pio, sm);
        test_put(pio, sm, data);
        test_wait_idle(pio, sm);
    }

    return 0;

}