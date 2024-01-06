/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <math.h>

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/dma.h"
#include "hardware/gpio.h"

#include "pio_spi_tx.pio.h"

#define USE_DMA 1

#define PIN_RS  27
#define PIN_CS  17

#define PIO_CLK_DIV 3.f

static PIO g_pio = pio0;
static uint g_sm = 0;

static inline void pio_spi_tx_set_rs_cs(bool rs, bool cs) {
    sleep_us(1);
    gpio_put_masked((1u << PIN_RS) | (1u << PIN_CS), !!rs << PIN_RS | !!cs << PIN_CS);
    sleep_us(1);
}
#if USE_DMA
/* DMA version */
static uint dma_tx;
static dma_channel_config c;
static inline int __time_critical_func(pio_spi_tx_write8)(PIO pio, uint sm, void *buf, size_t len)
{
    uint8_t *txbuf = (uint8_t *)buf;

    // const uint dma_tx = dma_claim_unused_channel(true);
    // dma_channel_config c = dma_channel_get_default_config(dma_tx);

    // channel_config_set_transfer_data_size(&c, DMA_SIZE_16);
    // channel_config_set_dreq(&c, pio_get_dreq(pio, sm, true));

    dma_channel_configure(dma_tx, &c,
                          &pio->txf[sm], /* write address */
                          txbuf, /* read address */
                          len, /* element count (each element is of size transfer_data_size) */
                          false /* don't start yet */
    );

    dma_start_channel_mask(1u << dma_tx);
    dma_channel_wait_for_finish_blocking(dma_tx);

    // dma_channel_unclaim(dma_tx);
    return 0;
}
#else
static inline void __time_critical_func(pio_spi_tx_write8)(PIO pio, uint sm, void *buf, size_t len)
{
    uint8_t data;

    while (len) {
        data = *(uint8_t *)buf;

        pio_spi_tx_put(pio, sm, data);

        buf++;
        len--;
    }
}
#endif

int pio_spi_write_buf_rs(void *buf, size_t len, int rs)
{
    pio_spi_tx_wait_idle(g_pio, g_sm);
    pio_spi_tx_set_rs_cs(rs, false);
    pio_spi_tx_write8(g_pio, g_sm, buf, len);
    pio_spi_tx_set_rs_cs(rs, true);
    pio_spi_tx_wait_idle(g_pio, g_sm);
    return 0;
}

int pio_spi_tx_init(void)
{
    printf("PIO SPI TX initialzing...\n");

#if USE_DMA
    dma_tx = dma_claim_unused_channel(true);
    c = dma_channel_get_default_config(dma_tx);

    channel_config_set_transfer_data_size(&c, DMA_SIZE_8);
    channel_config_set_dreq(&c, pio_get_dreq(g_pio, g_sm, true));
#endif

    uint offset = pio_add_program(g_pio, &pio_spi_tx_program);
    pio_spi_tx_program_init(g_pio, g_sm, offset, 19, 18, PIO_CLK_DIV);
}