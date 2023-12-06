// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) 2023 Iota Hydrae <writeforever@foxmail.com>
 */

/*
 * Support for the lg4572b
 * Note: This is a GPIO based driver, the most obvious
 * problem is the lag of refresh
 */

#define pr_fmt(fmt) "lg4572b: " fmt

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>

#include "pico/stdio.h"
#include "pico/stdio_uart.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"

/*
 * lg4572b Command Table
 */

#define DRV_NAME "lg4572b"
#define WIDTH  480
#define HEIGHT 800

#define pr_debug printf

struct lg4572b_priv;

typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

struct lg4572b_operations {
    int (*init_display)(struct lg4572b_priv *priv);
    int (*reset)(struct lg4572b_priv *priv);
    int (*clear)(struct lg4572b_priv *priv, u16 clear);
    int (*blank)(struct lg4572b_priv *priv, bool on);
    int (*sleep)(struct lg4572b_priv *priv, bool on);
    int (*set_var)(struct lg4572b_priv *priv);
    int (*set_addr_win)(struct lg4572b_priv *priv, int xs, int ys, int xe, int ye);
    int (*set_cursor)(struct lg4572b_priv *priv, int x, int y);
};

struct lg4572b_display {
    u32                     xres;
    u32                     yres;
    u32                     bpp;
    u32                     rotate;
};

struct lg4572b_priv {
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
    const struct lg4572b_operations  *tftops;
    struct lg4572b_display           *display;
} g_priv;

static inline void dm_gpio_set_value(int *pin, int val)
{
    gpio_put(*pin, val);
}

static inline void mdelay(int val)
{
    sleep_ms(val);
}

extern void i80_set_rs_cs(bool rs, bool cs);
extern int i80_write_buf_rs(void *buf, size_t len, bool rs);
/* rs=0 means writing register, rs=1 means writing data */
static int write_buf_rs(struct lg4572b_priv *priv, void *buf, size_t len, int rs)
{
    i80_set_rs_cs(rs, 0);
    i80_write_buf_rs(buf, len, rs);
    return 0;
}

static int lg4572b_write_reg(struct lg4572b_priv *priv, int len, ...)
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
    lg4572b_write_reg(priv, NUMARGS(__VA_ARGS__), __VA_ARGS__)

static int lg4572b_reset(struct lg4572b_priv *priv)
{
    dm_gpio_set_value(&priv->gpio.reset, 1);
    mdelay(40);
    dm_gpio_set_value(&priv->gpio.reset, 0);
    mdelay(120);
    dm_gpio_set_value(&priv->gpio.reset, 1);
    mdelay(40);
    return 0;
}


static int lg4572b_set_var(struct lg4572b_priv *priv)
{
    pr_debug("%s\n", __func__);
    return 0;
}

static int lg4572b_init_display(struct lg4572b_priv *priv)
{
    pr_debug("%s, writing initial sequence...\n", __func__);
    lg4572b_reset(priv);
    dm_gpio_set_value(&priv->gpio.rd, 1);
    mdelay(150);
    
    write_reg(priv, 0x11);
    mdelay(40);
    write_reg(priv, 0x20);
    write_reg(priv, 0x29);
    write_reg(priv, 0x3A, 0x55);
    
    write_reg(priv, 0xB2, 0x20, 0xC8);
    write_reg(priv, 0xB3, 0x00);
    write_reg(priv, 0xB4, 0x04);
    write_reg(priv, 0xB5, 0x10);
    write_reg(priv, 0xB6, 0x01, 0x18, 0x02, 0x40, 0x10, 0x00);
    write_reg(priv, 0xB7, 0x46, 0x06, 0x0C, 0x00, 0x00);
    
    write_reg(priv, 0xC0, 0x01, 0x11);
    write_reg(priv, 0xC3, 0x07, 0x03, 0x04, 0x05, 0x04);
    write_reg(priv, 0xC4, 0x32, 0x24, 0x10, 0x10, 0x01, 0x0A);
    write_reg(priv, 0xC5, 0x6A);
    write_reg(priv, 0xC6, 0x24, 0x50);
    
    write_reg(priv, 0xD0, 0x02, 0x76, 0x54, 0x15, 0x12, 0x03, 0x42, 0x43, 0x03);
    
    write_reg(priv, 0xD2, 0x02, 0x76, 0x54, 0x15, 0x12, 0x03, 0x42, 0x43, 0x03);
    
    write_reg(priv, 0xD4, 0x02, 0x76, 0x54, 0x15, 0x12, 0x03, 0x42, 0x43, 0x03);
    
    write_reg(priv, 0xD1, 0x02, 0x76, 0x54, 0x15, 0x12, 0x03, 0x42, 0x43, 0x03);
    
    write_reg(priv, 0xD3, 0x02, 0x76, 0x54, 0x15, 0x12, 0x03, 0x42, 0x43, 0x03);
    
    write_reg(priv, 0xD5, 0x02, 0x76, 0x54, 0x15, 0x12, 0x03, 0x42, 0x43, 0x03);
    mdelay(60);
    return 0;
}

static int lg4572b_set_addr_win(struct lg4572b_priv *priv, int xs, int ys, int xe,
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

static int lg4572b_clear(struct lg4572b_priv *priv, u16 clear)
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

static int lg4572b_blank(struct lg4572b_priv *priv, bool on)
{
    pr_debug("%s\n", __func__);
    return 0;
}

static int lg4572b_sleep(struct lg4572b_priv *priv, bool on)
{
    pr_debug("%s\n", __func__);
    return 0;
}

static const struct lg4572b_operations default_lg4572b_ops = {
    .init_display    = lg4572b_init_display,
    .reset           = lg4572b_reset,
    .clear           = lg4572b_clear,
    .blank           = lg4572b_blank,
    .sleep           = lg4572b_sleep,
    .set_var         = lg4572b_set_var,
    .set_addr_win    = lg4572b_set_addr_win,
};

#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))
static int lg4572b_gpio_init(struct lg4572b_priv *priv)
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

static int lg4572b_hw_init(struct lg4572b_priv *priv)
{
    int ret;

    printf("initializing hardware...\n");

    priv->gpio.reset = 27;
    priv->gpio.bl    = 26;
    priv->gpio.cs    = 21;
    priv->gpio.rs    = 20;
    priv->gpio.wr    = 19;
    priv->gpio.rd    = 18;

    for (int i = 0; i < ARRAY_SIZE(priv->gpio.db); i++)
        priv->gpio.db[i] = i;

    lg4572b_gpio_init(priv);

    dm_gpio_set_value(&priv->gpio.cs, 0);

    priv->tftops->init_display(priv);
    priv->tftops->clear(priv, 0x0);
    
    /* enable backlight after screen cleared */
    dm_gpio_set_value(&priv->gpio.bl, 1);
    pr_debug("backlight enabled\n");
    
    return 0;
}

static struct lg4572b_display default_lg4572b_display = {
    .xres   = WIDTH,
    .yres   = HEIGHT,
    .bpp    = 16,
    .rotate = 0,
};

static int lg4572b_video_sync(struct lg4572b_priv *priv, int xs, int ys, int xe, int ye, void *vmem16, size_t len)
{

    lg4572b_set_addr_win(priv, xs, ys, xe, ye);

    write_buf_rs(priv, vmem16, len * 2, 1);
    
    return 0;
}

int lg4572b_update(int xs, int ys, int xe, int ye, void *vmem16, size_t len)
{
    // pr_debug("%s\n", __func__);
    lg4572b_video_sync(&g_priv, xs, ys, xe, ye, vmem16, len);
    return 0;
}

#define PAGE_SIZE (1 << 12)
static int lg4572b_probe(struct lg4572b_priv *priv)
{
    pr_debug("lg4572b pio probing...\n");
    
    priv->buf = (u8 *)malloc(PAGE_SIZE);
    
    priv->display = &default_lg4572b_display;
    priv->tftops = &default_lg4572b_ops;
    
    lg4572b_hw_init(priv);
    
    return 0;
}

int lg4572b_driver_init()
{
    lg4572b_probe(&g_priv);
    return 0;
}

#if 0
int main()
{
    stdio_uart_init_full(uart0, 115200, 16, 17);

    pr_debug("\n\n\n\nThis is a simple test driver for lg4572b\n");

    lg4572b_probe(&g_priv);


    for (;;) {
        tight_loop_contents();
    }
}
#endif