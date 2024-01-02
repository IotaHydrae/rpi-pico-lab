// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) 2023 Iota Hydrae <writeforever@foxmail.com>
 */

/*
 * Support for the fl350hvc03v10
 * Note: This is a GPIO based driver, the most obvious
 * problem is the lag of refresh
 */

#include "pico/platform.h"
#include "pico/time.h"
#define pr_fmt(fmt) "fl350hvc03v10: " fmt

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>

#include "pico/stdio.h"
#include "pico/stdio_uart.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"

/*
 * fl350hvc03v10 Command Table
 */

#define DRV_NAME "fl350hvc03v10"
#define WIDTH  320
#define HEIGHT 480

#define pr_debug printf

struct fl350hvc03v10_priv;

typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

struct fl350hvc03v10_operations {
    int (*init_display)(struct fl350hvc03v10_priv *priv);
    int (*reset)(struct fl350hvc03v10_priv *priv);
    int (*clear)(struct fl350hvc03v10_priv *priv, u16 clear);
    int (*blank)(struct fl350hvc03v10_priv *priv, bool on);
    int (*sleep)(struct fl350hvc03v10_priv *priv, bool on);
    int (*set_var)(struct fl350hvc03v10_priv *priv);
    int (*set_addr_win)(struct fl350hvc03v10_priv *priv, int xs, int ys, int xe, int ye);
    int (*set_cursor)(struct fl350hvc03v10_priv *priv, int x, int y);
};

struct fl350hvc03v10_display {
    u32                     xres;
    u32                     yres;
    u32                     bpp;
    u32                     rotate;
};

struct fl350hvc03v10_priv {
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
    const struct fl350hvc03v10_operations  *tftops;
    struct fl350hvc03v10_display           *display;
} g_priv;

static u8 g_brounce_buf[64];

static inline void dm_gpio_set_value(int *pin, int val)
{
    gpio_put(*pin, val);
}

#if 0
static inline void mdelay(int val)
{
    sleep_ms(val);
}
#else
#define mdelay(v) sleep_ms(v)
#endif

extern int i80_write_buf_rs(void *buf, size_t len, bool rs);
/* rs=0 means writing register, rs=1 means writing data */
static inline int write_buf_rs(struct fl350hvc03v10_priv *priv, void *buf, size_t len, int rs)
{
    i80_write_buf_rs(buf, len, rs);
    return 0;
}

static int fl350hvc03v10_write_reg(struct fl350hvc03v10_priv *priv, int len, ...)
{
    u16 *buf = (u16 *)priv->buf;
    va_list args;
    int i;
    
    va_start(args, len);
    *buf = (u16)va_arg(args, unsigned int);
    write_buf_rs(priv, buf, sizeof(u16), 0);
    len--;
    
    /* if there no privams */
    if (len == 0)
        return 0;
    
    for (i = 0; i < len; i++) {
        *buf = (u16)va_arg(args, unsigned int);
        buf++;
    }
    
    len *= 2;
    write_buf_rs(priv, priv->buf, len, 1);
    va_end(args);
    
    return 0;
}
#define NUMARGS(...)  (sizeof((int[]){__VA_ARGS__}) / sizeof(int))
#define write_reg(priv, ...) \
    fl350hvc03v10_write_reg(priv, NUMARGS(__VA_ARGS__), __VA_ARGS__)

static int fl350hvc03v10_reset(struct fl350hvc03v10_priv *priv)
{
    dm_gpio_set_value(&priv->gpio.reset, 1);
    mdelay(10);
    dm_gpio_set_value(&priv->gpio.reset, 0);
    mdelay(50);
    dm_gpio_set_value(&priv->gpio.reset, 1);
    mdelay(10);
    return 0;
}

static int fl350hvc03v10_set_var(struct fl350hvc03v10_priv *priv)
{
    pr_debug("%s\n", __func__);
    return 0;
}

static int fl350hvc03v10_init_display(struct fl350hvc03v10_priv *priv)
{
    pr_debug("%s, writing initial sequence...\n", __func__);
    fl350hvc03v10_reset(priv);
    dm_gpio_set_value(&priv->gpio.rd, 1);
    mdelay(150);

    mdelay(50);
    write_reg(priv, 0x01);
    mdelay(50);

    write_reg(priv, 0x11);
    mdelay(20);
    write_reg(priv, 0x13);

    write_reg(priv, 0x00D0, 0x0007,0x0040, 0x001c);

    write_reg(priv, 0xD1, 0x00, 0x09, 0x08);

    write_reg(priv, 0x00, 0x01, 0x11);

    write_reg(priv, 0xC0, 0x10, 0x3B, 0x00, 0x02, 0x11);

    write_reg(priv, 0xC1, 0x10, 0x10, 0x88);

    write_reg(priv, 0xC5, 0x01);

    write_reg(priv, 0xC8, 0x00, 0x30, 0x36, 0x45, 0x04, 0x16, 0x37, 0x75, 0x77, 0x54, 0x0F, 0x00);

    write_reg(priv, 0xE4, 0xA0);
    write_reg(priv, 0xF0, 0x01);

    write_reg(priv, 0xF3, 0x40, 0x0A);

    write_reg(priv, 0xF7, 0x80);

    // write_reg(priv, 0x36, (1 << 5) | (1 << 7) | (1 << 3) | (1 << 1));
    write_reg(priv, 0x36, 0x28 | (1 << 0) | (1 << 1));
    write_reg(priv, 0x3A, 0x55);

    write_reg(priv, 0x2A, 0x00, 0x00, 0x01, 0x3F);
    write_reg(priv, 0x2B, 0x00, 0x00, 0x01, 0xDF);

    mdelay(50);

    write_reg(priv, 0x21);
    write_reg(priv, 0x29);
    write_reg(priv, 0x2C);

    return 0;
}

static int fl350hvc03v10_set_addr_win(struct fl350hvc03v10_priv *priv, int xs, int ys, int xe,
                                int ye)
{
    /* set column address */
    write_reg(priv, 0x2A, xs >> 8, xs & 0xFF, xe >> 8, xe & 0xFF);
    
    /* set row address */
    write_reg(priv, 0x2B, ys >> 8, ys & 0xFF, ye >> 8, ye & 0xFF);
    
    /* write start */
    write_reg(priv, 0x2C);
    return 0;
}

static int fl350hvc03v10_clear(struct fl350hvc03v10_priv *priv, u16 clear)
{
    u32 width = priv->display->xres;
    u32 height = priv->display->yres;
    int x, y;

    pr_debug("clearing screen (%d x %d) with color 0x%x\n", width, height, clear);

    priv->tftops->set_addr_win(priv, 0, 0,
                         priv->display->xres - 1,
                         priv->display->yres - 1);
    
    for (x = 0; x < width; x++) {
        for (y = 0; y < height; y++) {
            write_buf_rs(priv, &clear, sizeof(u16), 1);
        }
    }

    return 0;
}

static int fl350hvc03v10_blank(struct fl350hvc03v10_priv *priv, bool on)
{
    pr_debug("%s\n", __func__);
    return 0;
}

static int fl350hvc03v10_sleep(struct fl350hvc03v10_priv *priv, bool on)
{
    pr_debug("%s\n", __func__);
    return 0;
}

static const struct fl350hvc03v10_operations default_fl350hvc03v10_ops = {
    .init_display    = fl350hvc03v10_init_display,
    .reset           = fl350hvc03v10_reset,
    .clear           = fl350hvc03v10_clear,
    .blank           = fl350hvc03v10_blank,
    .sleep           = fl350hvc03v10_sleep,
    .set_var         = fl350hvc03v10_set_var,
    .set_addr_win    = fl350hvc03v10_set_addr_win,
};

#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))
static int fl350hvc03v10_gpio_init(struct fl350hvc03v10_priv *priv)
{
    printf("initializing gpios...\n");

    gpio_init(priv->gpio.reset);
    gpio_init(priv->gpio.bl);
    gpio_init(priv->gpio.cs);
    gpio_init(priv->gpio.rs);
    gpio_init(priv->gpio.rd);

    gpio_set_dir(priv->gpio.reset, GPIO_OUT);
    gpio_set_dir(priv->gpio.bl, GPIO_OUT);
    gpio_set_dir(priv->gpio.cs, GPIO_OUT);
    gpio_set_dir(priv->gpio.rs, GPIO_OUT);
    gpio_set_dir(priv->gpio.rd, GPIO_OUT);

    return 0;
}

static int fl350hvc03v10_hw_init(struct fl350hvc03v10_priv *priv)
{
    int ret;

    printf("initializing hardware...\n");

    fl350hvc03v10_gpio_init(priv);

    priv->tftops->init_display(priv);

    priv->tftops->clear(priv, 0x0);

    /* enable backlight after screen get cleared */
    dm_gpio_set_value(&priv->gpio.bl, 1);
    pr_debug("backlight enabled\n");

    return 0;
}

static struct fl350hvc03v10_display default_fl350hvc03v10_display = {
    .xres   = WIDTH,
    .yres   = HEIGHT,
    .bpp    = 16,
    .rotate = 0,
};

static int fl350hvc03v10_video_sync(struct fl350hvc03v10_priv *priv, int xs, int ys, int xe, int ye, void *vmem16, size_t len)
{
    // pr_debug("video sync: xs=%d, ys=%d, xe=%d, ye=%d, len=%d\n", xs, ys, xe, ye, len);

    priv->tftops->set_addr_win(priv, xs, ys, xe, ye);
    write_buf_rs(priv, vmem16, len * 2, 1);
    return 0;
}

int fl350hvc03v10_video_flush(int xs, int ys, int xe, int ye, void *data, size_t len)
{
    // pr_debug("%s\n", __func__);
    fl350hvc03v10_video_sync(&g_priv, xs, ys, xe, ye, data, len);
    return 0;
}

static int fl350hvc03v10_probe(struct fl350hvc03v10_priv *priv)
{
    pr_debug("fl350hvc03v10 probing ...\n");
    
    priv->buf = g_brounce_buf;
    
    priv->display = &default_fl350hvc03v10_display;
    priv->tftops = &default_fl350hvc03v10_ops;

    priv->gpio.bl    = 28;
    priv->gpio.reset = 22;
    priv->gpio.rd    = 21;
    priv->gpio.wr    = 20;
    priv->gpio.rs    = 19;
    priv->gpio.cs    = 18;

    for (int i = 0; i < ARRAY_SIZE(priv->gpio.db); i++)
        priv->gpio.db[i] = i;

    fl350hvc03v10_hw_init(priv);
    
    return 0;
}


#if 0
int main()
{
    stdio_uart_init_full(uart0, 115200, 16, 17);

    pr_debug("\n\n\n\nThis is a simple test driver for fl350hvc03v10\n");

    fl350hvc03v10_probe(&g_priv);

    u16 color = 0x0000;

    for (;color < 0xffff; color+=64) {
        g_priv.tftops->clear(&g_priv, color);
    }
}
#else
int fl350hvc03v10_driver_init(void)
{
    fl350hvc03v10_probe(&g_priv);
    return 0;
}
#endif