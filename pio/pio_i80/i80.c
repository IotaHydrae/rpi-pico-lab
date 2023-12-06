#include <stdio.h>

#include "boards/pico.h"
#include "hardware/gpio.h"
#include "pico/platform.h"
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/vreg.h"
#include "hardware/clocks.h"
#include "pico/time.h"

#define I80_CLK_DIV 1.f

#include "i80.pio.h"

#define LCD_PIN_RS 20
#define LCD_PIN_CS 21

static inline void i80_set_rs_cs(bool rs, bool cs)
{
    gpio_put_masked((1u << LCD_PIN_RS) | (1u << LCD_PIN_CS), !!rs << LCD_PIN_RS | !!cs << LCD_PIN_CS);
}

static inline void i80_write_16(PIO pio, uint sm, uint16_t data)
{
    i80_wait_idle(pio, sm);
    i80_put(pio, sm, data);
    i80_wait_idle(pio, sm);
}

int i80_write_pio16_wr(PIO pio, uint sm, void *buf, size_t len)
{
    uint16_t data;
    while (len) {
        data = *(uint16_t *)buf;

        i80_wait_idle(pio, sm);
        i80_put(pio, sm, data);
        i80_wait_idle(pio, sm);

        buf += 2;
        len -= 2;
    }
    return 0;
}

int write_buf_rs(PIO pio, uint sm, void *buf, size_t len, bool rs)
{
    i80_set_rs_cs(rs, false);
    int ret = i80_write_pio16_wr(pio, sm, buf, len);
    i80_set_rs_cs(rs, true);
    return ret;
}

int main(void)
{
    // vreg_set_voltage(VREG_VOLTAGE_1_05);
    // set_sys_clock_khz(240000, true);
    // clock_configure(clk_peri,
    //                 0,
    //                 CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLK_SYS,
    //                 240 * MHZ,
    //                 240 * MHZ);

    stdio_uart_init_full(uart0, 115200, 16, 17);
    printf("\n\n\ni80 PIO testing\n");

    PIO pio = pio0;
    uint sm = 0;
    uint offset = pio_add_program(pio, &i80_program);
    i80_program_init(pio, sm, offset, 0, 16, 19, I80_CLK_DIV);

    for (;;) {
        // tight_loop_contents();
        i80_write_16(pio, sm, 0xaaaa);
        i80_write_16(pio, sm, 0x5555);
    }

    return 0;
}