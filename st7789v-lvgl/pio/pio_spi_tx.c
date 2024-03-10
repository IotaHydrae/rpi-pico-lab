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

#include "pico/time.h"
#include "pico/stdlib.h"
#include "pico/platform.h"

#include "hardware/dma.h"
#include "hardware/pio.h"
#include "hardware/gpio.h"
#include "hardware/vreg.h"
#include "hardware/clocks.h"

#define USE_DMA 1

// Do not modify this header file. It is automatically generated from the pio program.
// you should modify the pio program instead.
#include "pio_spi_tx.pio.h"

static PIO g_pio = pio0;
static uint g_sm = 0;

static inline void __time_critical_func(pio_spi_tx_set_dc_cs)(bool dc, bool cs) {
    gpio_put_masked((1u << TFT_DC_PIN) | (1u << TFT_CS_PIN), !!dc << TFT_DC_PIN | !!cs << TFT_CS_PIN);
}

static inline void __time_critical_func(pio_spi_tx_set_dc)(bool dc)
{
    gpio_put(TFT_DC_PIN, dc);
}

#if USE_DMA
/* DMA version */
static uint dma_tx;
static dma_channel_config c;
static inline void pio_spi_tx_wr8(PIO pio, uint sm, void *buf, size_t len)
{
    dma_channel_configure(dma_tx, &c,
                          &pio->txf[sm], /* write address */
                          (uint8_t *)buf, /* read address */
                          len, /* element count (each element is of size transfer_data_size) */
                          true /* start right now */
    );

    dma_channel_wait_for_finish_blocking(dma_tx);
}
#else
static inline void pio_spi_tx_wr8(PIO pio, uint sm, void *buf, size_t len)
{
    uint8_t *buf8 = (uint8_t *)buf;
    while(len--)
        pio_spi_tx_put(pio, sm, *buf8++);
}
#endif

void pio_spi_tx_write_buf_dc(void *buf, size_t len, bool dc)
{
    uint8_t *buf8 = (uint8_t *)buf;
    pio_spi_tx_wait_idle(g_pio, g_sm);

    pio_spi_tx_set_dc_cs(dc, 0);
    pio_spi_tx_wr8(g_pio, g_sm, buf, len);
    pio_spi_tx_wait_idle(g_pio, g_sm);
    pio_spi_tx_set_dc_cs(dc, 1);
}

int pio_spi_tx_init(uint data_pin, uint clk_pin)
{
    printf("PIO SPI TX initializing ...\n");

#if USE_DMA
    dma_tx = dma_claim_unused_channel(true);
    c = dma_channel_get_default_config(dma_tx);

    channel_config_set_transfer_data_size(&c, DMA_SIZE_8);
    channel_config_set_dreq(&c, pio_get_dreq(g_pio, g_sm, true));
#endif

    uint offset = pio_add_program(g_pio, &pio_spi_tx_program);
    float clk_div = (DEFAULT_PIO_CLK_KHZ / 2.f / TFT_BUS_CLK_KHZ);

    pio_spi_tx_program_init(g_pio, g_sm, offset, data_pin, clk_pin, clk_div);
}