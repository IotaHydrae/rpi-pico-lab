#include <stdio.h>

#include "boards/pico.h"
#include "hardware/gpio.h"
#include "pico/platform.h"
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/vreg.h"
#include "hardware/clocks.h"
#include "hardware/dma.h"
#include "pico/time.h"

#define USE_DMA 1

// #define I80_CLK_DIV 2.5f
#define I80_CLK_DIV 5.6f /* running at 25MHz when pll_sys = 280MHz */

#include "i80.pio.h"

#define LCD_PIN_RS 19
#define LCD_PIN_CS 18

// #define LCD_PIN_RS 20
// #define LCD_PIN_CS 18

static PIO g_pio = pio0;
static uint g_sm = 0;

void __time_critical_func(i80_set_rs_cs)(bool rs, bool cs)
{
    gpio_put_masked((1u << LCD_PIN_RS) | (1u << LCD_PIN_CS), !!rs << LCD_PIN_RS | !!cs << LCD_PIN_CS);
}

#if USE_DMA
/* DMA version */
static uint dma_tx;
static dma_channel_config c;
static inline int __time_critical_func(i80_write_pio16_wr)(PIO pio, uint sm, void *buf, size_t len)
{
    uint16_t *txbuf = (uint16_t *)buf;

    // const uint dma_tx = dma_claim_unused_channel(true);
    // dma_channel_config c = dma_channel_get_default_config(dma_tx);

    // channel_config_set_transfer_data_size(&c, DMA_SIZE_16);
    // channel_config_set_dreq(&c, pio_get_dreq(pio, sm, true));

    dma_channel_configure(dma_tx, &c,
                          &pio->txf[sm], /* write address */
                          txbuf, /* read address */
                          len / 2, /* element count (each element is of size transfer_data_size) */
                          false /* don't start yet */
    );

    dma_start_channel_mask(1u << dma_tx);
    dma_channel_wait_for_finish_blocking(dma_tx);

    // dma_channel_unclaim(dma_tx);
    return 0;
}
#else
static inline int i80_write_pio16_wr(PIO pio, uint sm, void *buf, size_t len)
{
    uint16_t data;

    i80_wait_idle(pio, sm);
    while (len) {
        data = *(uint16_t *)buf;

        i80_put(pio, sm, data);

        buf += 2;
        len -= 2;
    }
    i80_wait_idle(pio, sm);
    return 0;
}
#endif

int __time_critical_func(i80_write_buf_rs)(void *buf, size_t len, bool rs)
{
    i80_wait_idle(g_pio, g_sm);
    i80_set_rs_cs(rs, false);
    i80_write_pio16_wr(g_pio, g_sm, buf, len);
    i80_wait_idle(g_pio, g_sm);
    return 0;
}

int i80_pio_init(void)
{
    printf("i80 PIO initialzing...\n");

#if USE_DMA
    dma_tx = dma_claim_unused_channel(true);
    c = dma_channel_get_default_config(dma_tx);

    channel_config_set_transfer_data_size(&c, DMA_SIZE_16);
    channel_config_set_dreq(&c, pio_get_dreq(g_pio, g_sm, true));
#endif

    uint offset = pio_add_program(g_pio, &i80_program);
    i80_program_init(g_pio, g_sm, offset, 0, 16, 20, I80_CLK_DIV);

    return 0;
}