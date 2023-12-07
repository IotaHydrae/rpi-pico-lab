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

#define I80_CLK_DIV 8.f

#include "i80.pio.h"

#define LCD_PIN_RS 19
#define LCD_PIN_CS 18

static PIO g_pio = pio0;
static uint g_sm = 0;

static void i80_set_rs(bool rs)
{
    asm volatile("nop \n nop \n nop");
    gpio_put(LCD_PIN_RS, rs);
    asm volatile("nop \n nop \n nop");
}

static void i80_set_cs(bool cs)
{
    asm volatile("nop \n nop \n nop");
    gpio_put(LCD_PIN_CS, cs);
    asm volatile("nop \n nop \n nop");
}

void i80_set_rs_cs(bool rs, bool cs)
{
    asm volatile("nop \n nop \n nop");
    gpio_put_masked((1u << LCD_PIN_RS) | (1u << LCD_PIN_CS), !!rs << LCD_PIN_RS | !!cs << LCD_PIN_CS);
    asm volatile("nop \n nop \n nop");
}

#if 0
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
#else
/* DMA version */
static inline int i80_write_pio16_wr(PIO pio, uint sm, void *buf, size_t len)
{
    uint16_t *txbuf = (uint16_t *)buf;

    const uint dma_tx = dma_claim_unused_channel(true);
    dma_channel_config c = dma_channel_get_default_config(dma_tx);

    channel_config_set_transfer_data_size(&c, DMA_SIZE_16);

    channel_config_set_dreq(&c, pio_get_dreq(pio, sm, true));
    dma_channel_configure(dma_tx, &c,
                          &pio->txf[sm], /* write address */
                          txbuf, /* read address */
                          len / 2, /* element count (each element is of size transfer_data_size) */
                          false /* don't start yet */
    );

    dma_start_channel_mask(1u << dma_tx);
    dma_channel_wait_for_finish_blocking(dma_tx);

    dma_channel_unclaim(dma_tx);
    return 0;
}
#endif

int i80_write_buf_rs(void *buf, size_t len, bool rs)
{
    
    i80_set_rs(rs);
    i80_set_cs(0);
    i80_write_pio16_wr(g_pio, g_sm, buf, len);
    i80_set_cs(1);
    return 0;
}

int i80_pio_init(void)
{
    printf("i80 PIO initialzing...\n");

    uint offset = pio_add_program(g_pio, &i80_program);
    i80_program_init(g_pio, g_sm, offset, 0, 16, 20, I80_CLK_DIV);

    return 0;
}