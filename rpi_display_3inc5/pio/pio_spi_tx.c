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

#define PIN_RS  24
#define PIN_CS  8

static PIO g_pio = pio0;
static uint g_sm = 0;

static inline void pio_spi_tx_set_rs_cs(bool rs, bool cs) {
    gpio_put_masked((1u << PIN_RS) | (1u << PIN_CS), !!rs << PIN_RS | !!cs << PIN_CS);
}

#if USE_DMA
/* DMA version */
static uint dma_tx;
static dma_channel_config c;
static inline int __time_critical_func(pio_spi_tx_write8)(PIO pio, uint sm, void *buf, size_t len)
{
    channel_config_set_transfer_data_size(&c, DMA_SIZE_8);

    dma_channel_configure(dma_tx, &c,
                          &pio->txf[sm], /* write address */
                          (uint8_t *)buf, /* read address */
                          len, /* element count (each element is of size transfer_data_size) */
                          true /* don't start yet */
    );

    dma_channel_wait_for_finish_blocking(dma_tx);
    return 0;
}

static inline int __time_critical_func(pio_spi_tx_write16)(PIO pio, uint sm, void *buf, size_t len)
{
    channel_config_set_transfer_data_size(&c, DMA_SIZE_16);

    dma_channel_configure(dma_tx, &c,
                          &pio->txf[sm], /* write address */
                          (uint16_t *)buf, /* read address */
                          len, /* element count (each element is of size transfer_data_size) */
                          true /* don't start yet */
    );

    dma_channel_wait_for_finish_blocking(dma_tx);
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

    pio_spi_tx_set_rs_cs(rs, 0);
    pio_spi_tx_write8(g_pio, g_sm, buf, len);
    pio_spi_tx_wait_idle(g_pio, g_sm);
    pio_spi_tx_set_rs_cs(rs, 1);
    return 0;
}

int pio_spi_write_buf16_rs(void *buf, size_t len, int rs)
{
    pio_spi_tx_wait_idle(g_pio, g_sm);

    pio_spi_tx_set_rs_cs(rs, 0);
    pio_spi_tx_write16(g_pio, g_sm, buf, len);
    pio_spi_tx_wait_idle(g_pio, g_sm);
    pio_spi_tx_set_rs_cs(rs, 1);
    return 0;
}


int pio_spi_tx_init(int data_pin, int clk_pin)
{
    printf("PIO SPI TX initialzing...\n");

#if USE_DMA
    dma_tx = dma_claim_unused_channel(true);
    c = dma_channel_get_default_config(dma_tx);

    channel_config_set_dreq(&c, pio_get_dreq(g_pio, g_sm, true));
#endif

    uint offset = pio_add_program(g_pio, &pio_spi_tx_program);
    float clk_div = (125000 / 2.f / 30000);

    pio_spi_tx_program_init(g_pio, g_sm, offset, data_pin, clk_pin, clk_div);
}