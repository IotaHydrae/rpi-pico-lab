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

#define USE_DMA 0

/* ILI9481 max clock frequency is 10MHz.
 * pll_sys = 120MHz and it divided by 6 = 10MHz */
#define I80_CLK_DIV 1.0f

#include "test.pio.h"

static PIO g_pio = pio0;
static uint g_sm = 0;

#if USE_DMA
/* DMA version */
static uint dma_tx;
static dma_channel_config c;
static inline int __time_critical_func(i80_write_pio16_wr)(PIO pio, uint sm, void *buf, size_t len)
{
    dma_channel_configure(dma_tx, &c,
                          &pio->txf[sm], /* write address */
                          buf, /* read address */
                          len, /* element count (each element is of size transfer_data_size) */
                          true /* start transfer immediately */
    );

    dma_channel_wait_for_finish_blocking(dma_tx);
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
    i80_write_pio16_wr(g_pio, g_sm, buf, len);
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
    i80_program_init(g_pio, g_sm, offset, 0, 32, I80_CLK_DIV);

    return 0;
}