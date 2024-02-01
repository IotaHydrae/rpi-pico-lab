// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) 2023 Iota Hydrae <writeforever@foxmail.com>
 */

/*
 * Support for the ili9488
 * Note: This is a GPIO based driver, the most obvious
 * problem is the lag of refresh
 */

#include "pico/platform.h"
#include "pico/time.h"
#define pr_fmt(fmt) "ili9488: " fmt

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>

#include "pico/stdio.h"
#include "pico/stdio_uart.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#include "ili9488.h"

/*
 * ili9488 Command Table
 */

#define DRV_NAME "ili9488"

#define pr_debug printf

struct ili9488_priv;

typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

struct ili9488_operations {
    int (*init_display)(struct ili9488_priv *priv);
    int (*reset)(struct ili9488_priv *priv);
    int (*clear)(struct ili9488_priv *priv, u16 clear);
    int (*blank)(struct ili9488_priv *priv, bool on);
    int (*sleep)(struct ili9488_priv *priv, bool on);
    int (*set_var)(struct ili9488_priv *priv);
    int (*set_addr_win)(struct ili9488_priv *priv, int xs, int ys, int xe, int ye);
    int (*set_cursor)(struct ili9488_priv *priv, int x, int y);
};

struct ili9488_display {
    u32                     xres;
    u32                     yres;
    u32                     bpp;
    u32                     rotate;
};

struct ili9488_priv {
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
    const struct ili9488_operations  *tftops;
    struct ili9488_display           *display;
} g_priv;

static inline void dm_gpio_set_value(int *pin, int val)
{
    gpio_put(*pin, val);
}

static inline void mdelay(int val)
{
    sleep_ms(val);
}

extern int i80_write_buf_rs(void *buf, size_t len, bool rs);
/* rs=0 means writing register, rs=1 means writing data */
static int write_buf_rs(struct ili9488_priv *priv, void *buf, size_t len, int rs)
{
    i80_write_buf_rs(buf, len, rs);
    return 0;
}

static int ili9488_write_reg(struct ili9488_priv *priv, int len, ...)
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
    ili9488_write_reg(priv, NUMARGS(__VA_ARGS__), __VA_ARGS__)

static int ili9488_reset(struct ili9488_priv *priv)
{
    dm_gpio_set_value(&priv->gpio.reset, 1);
    mdelay(10);
    dm_gpio_set_value(&priv->gpio.reset, 0);
    mdelay(10);
    dm_gpio_set_value(&priv->gpio.reset, 1);
    mdelay(10);
    return 0;
}


static int ili9488_set_var(struct ili9488_priv *priv)
{
    pr_debug("%s\n", __func__);
    return 0;
}

static int ili9488_init_display(struct ili9488_priv *priv)
{
    pr_debug("%s, writing initial sequence...\n", __func__);
    ili9488_reset(priv);
    // dm_gpio_set_value(&priv->gpio.rd, 1);
    // mdelay(150);

    write_reg(priv, 0xf7, 0xa9, 0x51, 0x2c, 0x82);

    write_reg(priv, 0xc0, 0x11, 0x09);

    write_reg(priv, 0xc1, 0x41);

    write_reg(priv, 0xc5, 0x00, 0x28, 0x80);

    write_reg(priv, 0xb1, 0xb0, 0x11);

    write_reg(priv, 0xb4, 0x02);

    write_reg(priv, 0xb6, 0x02, 0x22);

    write_reg(priv, 0xb7, 0xc6);

    write_reg(priv, 0xbe, 0x00, 0x04);

    write_reg(priv, 0xe9, 0x00);

    write_reg(priv, 0x36, 0x8 | (1 << 5) | (1 << 6));

    write_reg(priv, 0x3a, 0x55);

    write_reg(priv, 0xe0, 0x00, 0x07, 0x10, 0x09, 0x17, 0x0b, 0x41, 0x89, 0x4b, 0x0a, 0x0c, 0x0e, 0x18, 0x1b, 0x0f);

    write_reg(priv, 0xe1, 0x00, 0x17, 0x1a, 0x04, 0x0e, 0x06, 0x2f, 0x45, 0x43, 0x02, 0x0a, 0x09, 0x32, 0x36, 0x0f);

    write_reg(priv, 0x11);
    mdelay(60);
    write_reg(priv, 0x29);

    return 0;
}

static int ili9488_set_addr_win(struct ili9488_priv *priv, int xs, int ys, int xe,
                                int ye)
{
    /* set column adddress */
    write_reg(priv, 0x2A, xs >> 8, xs & 0xFF, xe >> 8, xe & 0xFF);
    
    /* set row address */
    write_reg(priv, 0x2B, ys >> 8, ys & 0xFF, ye >> 8, ye & 0xFF);
    
    /* write start */
    write_reg(priv, 0x2C);
    return 0;
}

static int ili9488_clear(struct ili9488_priv *priv, u16 clear)
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

static int ili9488_blank(struct ili9488_priv *priv, bool on)
{
    pr_debug("%s\n", __func__);
    return 0;
}

static int ili9488_sleep(struct ili9488_priv *priv, bool on)
{
    pr_debug("%s\n", __func__);
    return 0;
}

static const struct ili9488_operations default_ili9488_ops = {
    .init_display    = ili9488_init_display,
    .reset           = ili9488_reset,
    .clear           = ili9488_clear,
    .blank           = ili9488_blank,
    .sleep           = ili9488_sleep,
    .set_var         = ili9488_set_var,
    .set_addr_win    = ili9488_set_addr_win,
};

#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))
static int ili9488_gpio_init(struct ili9488_priv *priv)
{
    printf("initializing gpios...\n");

    gpio_init(priv->gpio.reset);
    // gpio_init(priv->gpio.bl);
    // gpio_init(priv->gpio.cs);
    gpio_init(priv->gpio.rs);
    // gpio_init(priv->gpio.rd);

    gpio_set_dir(priv->gpio.reset, GPIO_OUT);
    // gpio_set_dir(priv->gpio.bl, GPIO_OUT);
    // gpio_set_dir(priv->gpio.cs, GPIO_OUT);
    gpio_set_dir(priv->gpio.rs, GPIO_OUT);
    // gpio_set_dir(priv->gpio.rd, GPIO_OUT);

    return 0;
}

int i80_pio_init(uint8_t db_base, uint8_t db_count, uint8_t pin_wr);
static int ili9488_hw_init(struct ili9488_priv *priv)
{
    int ret;

    printf("initializing hardware...\n");

    i80_pio_init(priv->gpio.db[0], ARRAY_SIZE(priv->gpio.db), priv->gpio.wr);

    ili9488_gpio_init(priv);

    priv->tftops->init_display(priv);
    // priv->tftops->clear(priv, 0x0);

    return 0;
}

static struct ili9488_display default_ili9488_display = {
    .xres   = ILI9488_X_RES,
    .yres   = ILI9488_Y_RES,
    .bpp    = 16,
    .rotate = 0,
};

static int ili9488_video_sync(struct ili9488_priv *priv, int xs, int ys, int xe, int ye, void *vmem16, size_t len)
{
    // pr_debug("video sync: xs=%d, ys=%d, xe=%d, ye=%d, len=%d\n", xs, ys, xe, ye, len);
    priv->tftops->set_addr_win(priv, xs, ys, xe, ye);
    write_buf_rs(priv, vmem16, len * 2, 1);
    return 0;
}

int ili9488_video_flush(int xs, int ys, int xe, int ye, void *vmem16, uint32_t len)
{
    ili9488_video_sync(&g_priv, xs, ys, xe, ye, vmem16, len);
    return 0;
}

#define BUF_SIZE 64
static int ili9488_probe(struct ili9488_priv *priv)
{
    pr_debug("ili9488 probing ...\n");
    
    priv->buf = (u8 *)malloc(BUF_SIZE);
    
    priv->display = &default_ili9488_display;
    priv->tftops = &default_ili9488_ops;

    priv->gpio.bl    = 28;
    priv->gpio.reset = 22;
    // priv->gpio.rd    = 21;
    priv->gpio.rs    = 20;
    priv->gpio.wr    = 19;
    // priv->gpio.cs    = 18;

    /* pin0 - pin15 for I8080 data bus */
    for (int i = 0; i < ARRAY_SIZE(priv->gpio.db); i++)
        priv->gpio.db[i] = i;

    ili9488_hw_init(priv);
    
    return 0;
}

int ili9488_driver_init(void)
{
    ili9488_probe(&g_priv);
    return 0;
}