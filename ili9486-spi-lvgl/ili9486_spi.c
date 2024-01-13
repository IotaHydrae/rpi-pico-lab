// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) 2023 Iota Hydrae <writeforever@foxmail.com>
 */

/*
 * Support for the ili9486
 * Note: This is a GPIO based driver, the most obvious
 * problem is the lag of refresh
 */

#include "pico/platform.h"
#include "pico/time.h"
#define pr_fmt(fmt) "ili9486: " fmt

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>

#include "pico/stdio.h"
#include "pico/stdlib.h"
#include "pico/stdio_uart.h"
#include "pico/binary_info.h"
#include "hardware/gpio.h"
#include "hardware/spi.h"

/*
 * ili9486 Command Table
 */

#define DRV_NAME "ili9486"
#define WIDTH  320
#define HEIGHT 480

#define pr_debug printf

struct ili9486_priv;

typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

struct ili9486_operations {
    int (*init_display)(struct ili9486_priv *priv);
    int (*reset)(struct ili9486_priv *priv);
    int (*clear)(struct ili9486_priv *priv, u16 clear);
    int (*blank)(struct ili9486_priv *priv, bool on);
    int (*sleep)(struct ili9486_priv *priv, bool on);
    int (*set_var)(struct ili9486_priv *priv);
    int (*set_addr_win)(struct ili9486_priv *priv, int xs, int ys, int xe, int ye);
    int (*set_cursor)(struct ili9486_priv *priv, int x, int y);
};

struct ili9486_display {
    u32                     xres;
    u32                     yres;
    u32                     bpp;
    u32                     rotate;
};

struct ili9486_priv {
    u8                      *buf;

    struct {
        int reset;
        int cs;   /* chip select */
        int rs;   /* register/data select */
        int wr;   /* write signal */
        int rd;   /* read signal */
        int bl;   /* backlight */
        int db[16];
    } gpio;
    
    /* device specific */
    const struct ili9486_operations  *tftops;
    struct ili9486_display           *display;
} g_priv;

static inline void dm_gpio_set_value(int *pin, int val)
{
    gpio_put(*pin, val);
}

static inline void mdelay(int val)
{
    sleep_ms(val);
}

// extern void pio_spi_tx_set_rs_cs(bool rs, bool cs);
/* rs=0 means writing register, rs=1 means writing data */
static int write_buf_rs(struct ili9486_priv *priv, uint8_t *buf, size_t len, int rs)
{
    dm_gpio_set_value(&priv->gpio.cs, 0);
    dm_gpio_set_value(&priv->gpio.rs, rs);
    
    spi_write_blocking(spi_default, buf, len);

    dm_gpio_set_value(&priv->gpio.cs, 1);
    return 0;
}

static int ili9486_write_reg(struct ili9486_priv *priv, int len, ...)
{
    u8 *buf = (u8 *)priv->buf;
    va_list args;
    int i;
    
    va_start(args, len);
    *buf = (u8)va_arg(args, unsigned int);
    write_buf_rs(priv, buf, sizeof(u8), 0);
    len--;
    
    /* if there no privams */
    if (len == 0)
        return 0;
    
    for (i = 0; i < len; i++) {
        *buf = (u8)va_arg(args, unsigned int);
        buf++;
    }

    write_buf_rs(priv, priv->buf, len, 1);
    va_end(args);
    
    return 0;
}
#define NUMARGS(...)  (sizeof((int[]){__VA_ARGS__}) / sizeof(int))
#define write_reg(priv, ...) \
    ili9486_write_reg(priv, NUMARGS(__VA_ARGS__), __VA_ARGS__)

static int ili9486_reset(struct ili9486_priv *priv)
{
    dm_gpio_set_value(&priv->gpio.reset, 1);
    mdelay(10);
    dm_gpio_set_value(&priv->gpio.reset, 0);
    mdelay(50);
    dm_gpio_set_value(&priv->gpio.reset, 1);
    mdelay(10);
    return 0;
}

static int ili9486_set_var(struct ili9486_priv *priv)
{
    pr_debug("%s\n", __func__);
    return 0;
}

static int ili9486_init_display(struct ili9486_priv *priv)
{
    pr_debug("%s, writing initial sequence...\n", __func__);
    ili9486_reset(priv);
    write_reg(priv, 0xf1, 0x36, 0x04, 0x00, 0x3c, 0x0f, 0x8f);
    write_reg(priv, 0xf2, 0x18, 0xa3, 0x12, 0x02, 0xb2, 0x12, 0xff, 0x10, 0x00);
    write_reg(priv, 0xf8, 0x21, 0x04);
    write_reg(priv, 0xf9, 0x00, 0x08);
    // write_reg(priv, 0x36, 0x08);
    write_reg(priv, 0xb4, 0x00);
    write_reg(priv, 0xc1, 0x47);
    write_reg(priv, 0xc5, 0x00, 0xaf, 0x80, 0x00);
    write_reg(priv, 0xe0, 0x0f, 0x1f, 0x1c, 0x0c, 0x0f, 0x08, 0x48, 0x98, 0x37, 0x0a, 0x13, 0x04, 0x11, 0x0d, 0x00);
    write_reg(priv, 0xe1, 0x0f, 0x32, 0x2e, 0x0b, 0x0d, 0x05, 0x47, 0x75, 0x37, 0x06, 0x10, 0x03, 0x24, 0x20, 0x00);
    write_reg(priv, 0x3a, 0x55);
    write_reg(priv, 0x11);
    // write_reg(priv, 0x36, 0);
    mdelay(120);
    write_reg(priv, 0x29);
    return 0;
}

static int ili9486_set_addr_win(struct ili9486_priv *priv, int xs, int ys, int xe,
                                int ye)
{
    /* set column adddress */
    write_reg(priv, 0x2a,
        (xs >> 8) & 0xff, xs & 0xff,
        (xe >> 8) & 0xff, xe & 0xff);
    
    /* set row address */
    write_reg(priv, 0x2b,
        (ys >> 8) & 0xff, ys & 0xff,
        (ye >> 8) & 0xff, ye & 0xff);
    
    /* write start */
    write_reg(priv, 0x2c);
    return 0;
}

static int ili9486_clear(struct ili9486_priv *priv, u16 clear)
{
    u32 width = priv->display->xres;
    u32 height = priv->display->yres;
    u8 color_h, color_l;
    int x, y;

    pr_debug("clearing screen (%d x %d) with color 0x%x\n", width, height, clear);

    priv->tftops->set_addr_win(priv, 0, 0,
                         priv->display->xres - 1,
                         priv->display->yres - 1);

    color_h = clear >> 8;
    color_l = clear & 0xff;

    for (x = 0; x < width; x++) {
        for (y = 0; y < height; y++) {
            write_buf_rs(priv, &color_h, sizeof(u8), 1);
            write_buf_rs(priv, &color_l, sizeof(u8), 1);
        }
    }

    return 0;
}

static int ili9486_blank(struct ili9486_priv *priv, bool on)
{
    pr_debug("%s\n", __func__);
    return 0;
}

static int ili9486_sleep(struct ili9486_priv *priv, bool on)
{
    pr_debug("%s\n", __func__);
    return 0;
}

static const struct ili9486_operations default_ili9486_ops = {
    .init_display    = ili9486_init_display,
    .reset           = ili9486_reset,
    .clear           = ili9486_clear,
    .blank           = ili9486_blank,
    .sleep           = ili9486_sleep,
    .set_var         = ili9486_set_var,
    .set_addr_win    = ili9486_set_addr_win,
};

#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))
static int ili9486_gpio_init(struct ili9486_priv *priv)
{
    printf("initializing gpios...\n");

    gpio_init(priv->gpio.reset);
    gpio_init(priv->gpio.cs);
    gpio_init(priv->gpio.rs);

    gpio_set_dir(priv->gpio.reset, GPIO_OUT);
    gpio_set_dir(priv->gpio.cs, GPIO_OUT);
    gpio_set_dir(priv->gpio.rs, GPIO_OUT);

    return 0;
}

static int ili9486_hw_init(struct ili9486_priv *priv)
{
    int ret;

    printf("initializing hardware...\n");

    spi_init(spi_default, 12000000);
    gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);
    bi_decl(bi_2pins_with_func(PICO_DEFAULT_SPI_TX_PIN, PICO_DEFAULT_SPI_SCK_PIN,
                               GPIO_FUNC_SPI));
    ili9486_gpio_init(priv);

    priv->tftops->init_display(priv);
    priv->tftops->clear(priv, 0x0);

    /* enable backlight after screen get cleared */
    // dm_gpio_set_value(&priv->gpio.bl, 1);
    // pr_debug("backlight enabled\n");

    return 0;
}

static struct ili9486_display default_ili9486_display = {
    .xres   = WIDTH,
    .yres   = HEIGHT,
    .bpp    = 16,
    .rotate = 0,
};

static int ili9486_video_sync(struct ili9486_priv *priv, int xs, int ys, int xe, int ye, void *data, size_t len)
{
    // pr_debug("video sync: xs=%d, ys=%d, xe=%d, ye=%d, len=%d\n", xs, ys, xe, ye, len);
    priv->tftops->set_addr_win(priv, xs, ys, xe, ye);
    write_buf_rs(priv, (uint8_t *)data, len * 2, 1);
    return 0;
}

int ili9486_video_flush(int xs, int ys, int xe, int ye, void *data, size_t len)
{
    ili9486_video_sync(&g_priv, xs, ys, xe, ye, data, len);
    return 0;
}

#define PAGE_SIZE (1 << 12)
static int ili9486_probe(struct ili9486_priv *priv)
{
    pr_debug("ili9486 probing ...\n");
    
    priv->buf = (u8 *)malloc(PAGE_SIZE);
    
    priv->display = &default_ili9486_display;
    priv->tftops = &default_ili9486_ops;

    priv->gpio.reset = 15;
    priv->gpio.rs    = 27;
    priv->gpio.cs    = 17;

    ili9486_hw_init(priv);
    
    return 0;
}

#include "hardware/vreg.h"
#include "hardware/clocks.h"

#define PICO_FLASH_SPI_CLKDIV 2
#define CPU_SPEED_MHZ 280

extern int pio_spi_tx_init(void);

#if 0
int main()
{
    vreg_set_voltage(VREG_VOLTAGE_DEFAULT);
    set_sys_clock_khz(CPU_SPEED_MHZ * 1000, true);
    clock_configure(clk_peri,
                    0,
                    CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLK_SYS,
                    CPU_SPEED_MHZ * MHZ,
                    CPU_SPEED_MHZ * MHZ);
    stdio_uart_init_full(uart0, 115200, 16, 17);

    pr_debug("\n\n\n\nThis is a simple test driver for ili9486\n");

    pio_spi_tx_init();

    ili9486_probe(&g_priv);

    u16 color = 0x0000;

    for (;color < 0xffff; color+=64) {
        g_priv.tftops->clear(&g_priv, color);
    }
}
#else
int ili9486_driver_init(void)
{
    ili9486_probe(&g_priv);
    return 0;
}
#endif