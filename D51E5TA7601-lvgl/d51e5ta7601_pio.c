// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) 2023 Iota Hydrae <writeforever@foxmail.com>
 */

/*
 * Support for the d51e5ta7601
 * Note: This is a GPIO based driver, the most obvious
 * problem is the lag of refresh
 */

#include "pico/platform.h"
#include "pico/time.h"
#define pr_fmt(fmt) "d51e5ta7601: " fmt

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>

#include "pico/stdio.h"
#include "pico/stdio_uart.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"

/*
 * d51e5ta7601 Command Table
 */

#define DRV_NAME "d51e5ta7601"
#define WIDTH  320
#define HEIGHT 480

#define pr_debug printf

struct d51e5ta7601_priv;

typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

struct d51e5ta7601_operations {
    int (*init_display)(struct d51e5ta7601_priv *priv);
    int (*reset)(struct d51e5ta7601_priv *priv);
    int (*clear)(struct d51e5ta7601_priv *priv, u16 clear);
    int (*blank)(struct d51e5ta7601_priv *priv, bool on);
    int (*sleep)(struct d51e5ta7601_priv *priv, bool on);
    int (*set_var)(struct d51e5ta7601_priv *priv);
    int (*set_addr_win)(struct d51e5ta7601_priv *priv, int xs, int ys, int xe, int ye);
    int (*set_cursor)(struct d51e5ta7601_priv *priv, int x, int y);
};

struct d51e5ta7601_display {
    u32                     xres;
    u32                     yres;
    u32                     bpp;
    u32                     rotate;
};

struct d51e5ta7601_priv {
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
    const struct d51e5ta7601_operations  *tftops;
    struct d51e5ta7601_display           *display;
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
static int write_buf_rs(struct d51e5ta7601_priv *priv, void *buf, size_t len, int rs)
{
    i80_write_buf_rs(buf, len, rs);
    return 0;
}

static int d51e5ta7601_write_reg(struct d51e5ta7601_priv *priv, int len, ...)
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
    d51e5ta7601_write_reg(priv, NUMARGS(__VA_ARGS__), __VA_ARGS__)

static int d51e5ta7601_reset(struct d51e5ta7601_priv *priv)
{
    dm_gpio_set_value(&priv->gpio.reset, 1);
    mdelay(10);
    dm_gpio_set_value(&priv->gpio.reset, 0);
    mdelay(50);
    dm_gpio_set_value(&priv->gpio.reset, 1);
    mdelay(10);
    return 0;
}

static int d51e5ta7601_set_var(struct d51e5ta7601_priv *priv)
{
    pr_debug("%s\n", __func__);
    return 0;
}

#if 1
static int d51e5ta7601_init_display(struct d51e5ta7601_priv *priv)
{
    pr_debug("%s, writing initial sequence...\n", __func__);
    d51e5ta7601_reset(priv);
    dm_gpio_set_value(&priv->gpio.rd, 1);
    mdelay(150);

    write_reg(priv, 0x01, 0x013c);
    write_reg(priv, 0x02, 0x0100);
    write_reg(priv, 0x03, 0x1030);
    write_reg(priv, 0x08, 0x0808);
    write_reg(priv, 0x0a, 0x0500);
    write_reg(priv, 0x0b, 0x0000);
    write_reg(priv, 0x0c, 0x0770);
    write_reg(priv, 0x0d, 0x0000);
    write_reg(priv, 0x0e, 0x0001);

    /* power control */
    write_reg(priv, 0x11, 0x0406);
    write_reg(priv, 0x12, 0x000e);
    write_reg(priv, 0x13, 0x0222);
    write_reg(priv, 0x14, 0x001c);
    write_reg(priv, 0x15, 0x3679);
    write_reg(priv, 0x16, 0x0000);

    /* Gamma Control */
    write_reg(priv, 0x30, 0x6a50);
    write_reg(priv, 0x31, 0x00c9);
    write_reg(priv, 0x32, 0xc7be);
    write_reg(priv, 0x33, 0x0003);
    write_reg(priv, 0x36, 0x3443);
    write_reg(priv, 0x3b, 0x0000);
    write_reg(priv, 0x3c, 0x0000);

    write_reg(priv, 0x2c, 0x6a50);
    write_reg(priv, 0x2d, 0x00c9);
    write_reg(priv, 0x2e, 0xc7be);
    write_reg(priv, 0x2f, 0x0003);
    write_reg(priv, 0x35, 0x3443);
    write_reg(priv, 0x39, 0x0000);
    write_reg(priv, 0x3a, 0x0000);

    write_reg(priv, 0x28, 0x6a50);
    write_reg(priv, 0x29, 0x00c9);
    write_reg(priv, 0x2a, 0xc7be);
    write_reg(priv, 0x2b, 0x0003);
    write_reg(priv, 0x34, 0x3443);
    write_reg(priv, 0x37, 0x0000);
    write_reg(priv, 0x38, 0x0000);

    mdelay(20);

    write_reg(priv, 0x12, 0x200e);

    mdelay(160);

    write_reg(priv, 0x12, 0x2003);

    mdelay(40);

    write_reg(priv, 0x44, 0x013f);
    write_reg(priv, 0x45, 0x0000);
    write_reg(priv, 0x46, 0x01df);
    write_reg(priv, 0x47, 0x0000);
    write_reg(priv, 0x20, 0x0000);
    write_reg(priv, 0x21, 0x0000);

    write_reg(priv, 0x07, 0x0012);
    mdelay(40);
    write_reg(priv, 0x07, 0x0017);

    return 0;
}
#else
static int d51e5ta7601_init_display(struct d51e5ta7601_priv *priv)
{
    pr_debug("%s, writing initial sequence...\n", __func__);
    d51e5ta7601_reset(priv);
    dm_gpio_set_value(&priv->gpio.rd, 1);
    mdelay(150);

    write_reg(priv, 0x01, 0x013c);
    write_reg(priv, 0x02, 0x0100);
    write_reg(priv, 0x03, 0x1000);
    write_reg(priv, 0x08, 0x0808);
    write_reg(priv, 0x0a, 0x0500);
    write_reg(priv, 0x0b, 0x0000);
    write_reg(priv, 0x0c, 0x0770);
    write_reg(priv, 0x0d, 0x0000);
    write_reg(priv, 0x0e, 0x0001);

    write_reg(priv, 0x11, 0x0406);
    write_reg(priv, 0x12, 0x000e);
    write_reg(priv, 0x13, 0x0222);
    write_reg(priv, 0x14, 0x0015);
    write_reg(priv, 0x15, 0x4277);
    write_reg(priv, 0x16, 0x0000);

    write_reg(priv, 0x30, 0x6a50);
    write_reg(priv, 0x31, 0x00c9);
    write_reg(priv, 0x32, 0xc7be);
    write_reg(priv, 0x33, 0x0003);
    write_reg(priv, 0x36, 0x3443);
    write_reg(priv, 0x3b, 0x0000);
    write_reg(priv, 0x3c, 0x0000);

    write_reg(priv, 0x2c, 0x6a50);
    write_reg(priv, 0x2d, 0x00c9);
    write_reg(priv, 0x2e, 0xc7be);
    write_reg(priv, 0x2f, 0x0003);
    write_reg(priv, 0x35, 0x3443);
    write_reg(priv, 0x39, 0x0000);
    write_reg(priv, 0x3a, 0x0000);

    write_reg(priv, 0x28, 0x6a50);
    write_reg(priv, 0x29, 0x00c9);
    write_reg(priv, 0x2a, 0xc7be);
    write_reg(priv, 0x2b, 0x0003);
    write_reg(priv, 0x34, 0x3443);
    write_reg(priv, 0x37, 0x0000);
    write_reg(priv, 0x38, 0x0000);

    mdelay(20);

    write_reg(priv, 0x12, 0x200e);

    mdelay(160);

    write_reg(priv, 0x12, 0x2003);

    mdelay(40);

    write_reg(priv, 0x44, 0x013f);
    write_reg(priv, 0x45, 0x0000);
    write_reg(priv, 0x46, 0x01df);
    write_reg(priv, 0x47, 0x0000);
    write_reg(priv, 0x20, 0x0000);
    write_reg(priv, 0x21, 0x0100);
    write_reg(priv, 0x07, 0x0012);
    mdelay(40);
    write_reg(priv, 0x07, 0x0017);
    write_reg(0, 0, 0);

    return 0;
}
#endif

static int d51e5ta7601_set_addr_win(struct d51e5ta7601_priv *priv, int xs, int ys, int xe,
                                int ye)
{
    write_reg(priv, 0x45, xs);
    write_reg(priv, 0x44, xe);
    write_reg(priv, 0x47, ys);
    write_reg(priv, 0x46, ye);

    return 0;
}

static int d51e5ta7601_set_cursor(struct d51e5ta7601_priv *priv, int x, int y)
{
    write_reg(priv, 0x20, x);
    write_reg(priv, 0x21, y);
    
    write_reg(priv, 0x22);
    return 0;
}

static int d51e5ta7601_clear(struct d51e5ta7601_priv *priv, u16 clear)
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

static int d51e5ta7601_blank(struct d51e5ta7601_priv *priv, bool on)
{
    pr_debug("%s\n", __func__);
    return 0;
}

static int d51e5ta7601_sleep(struct d51e5ta7601_priv *priv, bool on)
{
    pr_debug("%s\n", __func__);
    return 0;
}

static const struct d51e5ta7601_operations default_d51e5ta7601_ops = {
    .init_display    = d51e5ta7601_init_display,
    .reset           = d51e5ta7601_reset,
    .clear           = d51e5ta7601_clear,
    .blank           = d51e5ta7601_blank,
    .sleep           = d51e5ta7601_sleep,
    .set_var         = d51e5ta7601_set_var,
    .set_cursor      = d51e5ta7601_set_cursor,
    .set_addr_win    = d51e5ta7601_set_addr_win,
};

#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))
#if 1
static int d51e5ta7601_gpio_init(struct d51e5ta7601_priv *priv)
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
#else
static int d51e5ta7601_gpio_init(struct d51e5ta7601_priv *priv)
{
    printf("initializing gpios...\n");
    int *pp = (int *)&priv->gpio;

    int len = sizeof(priv->gpio)/sizeof(priv->gpio.reset);

    while(len--) {
        gpio_init(*pp);
        gpio_set_dir(*pp, GPIO_OUT);
        pp++;
    }

    return 0;
}
#endif

static int d51e5ta7601_hw_init(struct d51e5ta7601_priv *priv)
{
    int ret;

    printf("initializing hardware...\n");

    d51e5ta7601_gpio_init(priv);

    priv->tftops->init_display(priv);

    priv->tftops->clear(priv, 0x0);

    /* enable backlight after screen get cleared */
    dm_gpio_set_value(&priv->gpio.bl, 1);
    pr_debug("backlight enabled\n");

    return 0;
}

static struct d51e5ta7601_display default_d51e5ta7601_display = {
    .xres   = WIDTH,
    .yres   = HEIGHT,
    .bpp    = 16,
    .rotate = 0,
};

static int d51e5ta7601_video_sync(struct d51e5ta7601_priv *priv, int xs, int ys, int xe, int ye, void *data, size_t len)
{
    // pr_debug("video sync: xs=%d, ys=%d, xe=%d, ye=%d, len=%d\n", xs, ys, xe, ye, len);

    priv->tftops->set_addr_win(priv, xs, ys, xe, ye);

    priv->tftops->set_cursor(priv, ys, xs);
    // while (len--) {
    //     write_buf_rs(priv, data, sizeof(u16), 1);
    //     data+=2;
    // }
    write_buf_rs(priv, data, len * 2, 1);
    return 0;
}

int d51e5ta7601_video_flush(int xs, int ys, int xe, int ye, void *data, size_t len)
{
    d51e5ta7601_video_sync(&g_priv, xs, ys, xe, ye, data, len);
    return 0;
}

#define PAGE_SIZE (1 << 12)
extern int i80_pio_init(void);
static int d51e5ta7601_probe(struct d51e5ta7601_priv *priv)
{
    pr_debug("d51e5ta7601 probing ...\n");
    
    priv->buf = (u8 *)malloc(PAGE_SIZE);
    
    priv->display = &default_d51e5ta7601_display;
    priv->tftops = &default_d51e5ta7601_ops;

    priv->gpio.reset = 29;
    priv->gpio.bl    = 28;
    priv->gpio.rd    = 21;
    priv->gpio.wr    = 20;
    priv->gpio.rs    = 19;
    priv->gpio.cs    = 18;

    for (int i = 0; i < ARRAY_SIZE(priv->gpio.db); i++)
        priv->gpio.db[i] = i;

    i80_pio_init();

    d51e5ta7601_hw_init(priv);
    sleep_ms(500);

    return 0;
}

#if 0
int main()
{
    stdio_uart_init_full(uart0, 115200, 16, 17);

    pr_debug("\n\n\n\nThis is a simple test driver for d51e5ta7601\n");

    d51e5ta7601_probe(&g_priv);

    for (;;) {
        tight_loop_contents();
    }
}
#else
int d51e5ta7601_driver_init(void)
{
    d51e5ta7601_probe(&g_priv);
    return 0;
}
#endif