// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) 2023 Iota Hydrae <writeforever@foxmail.com>
 */

/*
 * Support for the r61505w
 * Note: This is a GPIO based driver, the most obvious
 * problem is the lag of refresh
 */

#define pr_fmt(fmt) "r61505w: " fmt

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>

#include "pico/stdio.h"
#include "pico/stdio_uart.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"

/*
 * r61505w Command Table
 */

#define R61505W_REG_ID  0x00
#define R61505W_REG_DRV_OUTPUT_CTRL  0x01
#define R61505W_REG_ENTRY_MODE  0x03
#define R61505W_REG_DISP_CTRL1  0x07
#define R61505W_REG_POW_CTRL1  0x10
#define R61505W_REG_POW_CTRL2  0x11
#define R61505W_REG_POW_CTRL3  0x12
#define R61505W_REG_POW_CTRL4  0x13
#define R61505W_REG_X_ADDR  0x20
#define R61505W_REG_Y_ADDR  0x21
#define R61505W_REG_MEM_WRITE  0x22
#define R61505W_REG_NVM_CTRL4  0xA4

#define DRV_NAME "r61505w"
#define WIDTH  240
#define HEIGHT 320

#define pr_debug printf

struct r61505w_priv;

typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

struct r61505w_operations {
    int (*init_display)(struct r61505w_priv *priv);
    int (*reset)(struct r61505w_priv *priv);
    int (*clear)(struct r61505w_priv *priv, u16 clear);
    int (*blank)(struct r61505w_priv *priv, bool on);
    int (*sleep)(struct r61505w_priv *priv, bool on);
    int (*set_var)(struct r61505w_priv *priv);
    int (*set_addr_win)(struct r61505w_priv *priv, int xs, int ys, int xe, int ye);
    int (*set_cursor)(struct r61505w_priv *priv, int x, int y);
};

struct r61505w_display {
    u32                     xres;
    u32                     yres;
    u32                     bpp;
    u32                     rotate;
};

struct r61505w_priv {
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
    const struct r61505w_operations  *tftops;
    struct r61505w_display           *display;
} g_priv;

static inline void dm_gpio_set_value(int *pin, int val)
{
    gpio_put(*pin, val);
}

static inline uint dm_gpio_get_value(int *pin)
{
    return gpio_get(*pin);
}

static inline void mdelay(int val)
{
    sleep_ms(val);
}

static void fbtft_set_db_gpios_dir(struct r61505w_priv *priv, u8 output)
{
    int len = sizeof(priv->gpio.db)/sizeof(priv->gpio.db[0]);
    int *pp = (int *)&priv->gpio.db;

    while(len--)
        gpio_set_dir(*pp++, output ? GPIO_OUT : GPIO_IN);
}

// #define DO_NOT_OPTIMIZE_FBTFT_WRITE_GPIO
int fbtft_write_gpio16_wr(struct r61505w_priv *priv, void *buf, size_t len)
{
    u16 data;
    int i;
#ifndef DO_NOT_OPTIMIZE_FBTFT_WRITE_GPIO
    static u16 prev_data;
#endif

    /* claim bus */
    dm_gpio_set_value(&priv->gpio.cs, 0);

    /* Start writing by pulling down /WR */
    dm_gpio_set_value(&priv->gpio.wr, 1);

    while (len) {
        data = *(u16 *)buf;

        /* Start writing by pulling down /WR */
        dm_gpio_set_value(&priv->gpio.wr, 0);

        // printf("data : 0x%x\n", data);

        /* Set data */
#ifndef DO_NOT_OPTIMIZE_FBTFT_WRITE_GPIO
        if (data == prev_data) {
            dm_gpio_set_value(&priv->gpio.wr, 1); /* used as delay */
        } else {
            for (i = 0; i < 16; i++) {
                if ((data & 1) != (prev_data & 1))
                    dm_gpio_set_value(&priv->gpio.db[i],
                                      data & 1);
                data >>= 1;
                prev_data >>= 1;
            }
        }
#else
        for (i = 0; i < 16; i++) {
            dm_gpio_set_value(&priv->gpio.db[i], data & 1);
            data >>= 1;
        }
#endif

        /* Pullup /WR */
        dm_gpio_set_value(&priv->gpio.wr, 1);

#ifndef DO_NOT_OPTIMIZE_FBTFT_WRITE_GPIO
        prev_data = *(u16 *)buf;
#endif
        buf += 2;
        len -= 2;
    }

    /* release bus */
    dm_gpio_set_value(&priv->gpio.cs, 1);

    return 0;
}

int fbtft_read_gpio16_rd(struct r61505w_priv *priv, void *buf, size_t len)
{
    u16 data;
    int i;

    fbtft_set_db_gpios_dir(priv, GPIO_IN);

    /* claim bus */
    dm_gpio_set_value(&priv->gpio.cs, 0);

    /* Start reading by pulling up /RD */
    dm_gpio_set_value(&priv->gpio.rd, 1);

    /* read data */
    while (len) {

        /* host reading by pulling up /RD */
        dm_gpio_set_value(&priv->gpio.rd, 0);

        while (!dm_gpio_get_value(&priv->gpio.rd));

        for (i = 0; i < 16; i++) {
            data = dm_gpio_get_value(&priv->gpio.db[i]);
            *(u16 *)buf |= data << i;
        }

        buf += 2;
        len -= 2;
    }

    /* release bus */
    dm_gpio_set_value(&priv->gpio.cs, 1);

    fbtft_set_db_gpios_dir(priv, GPIO_OUT);
    return 0;
}

/* rs=0 means writing register, rs=1 means writing data */
static int write_buf_rs(struct r61505w_priv *priv, void *buf, size_t len, int rs)
{
    dm_gpio_set_value(&priv->gpio.rs, rs);
    fbtft_write_gpio16_wr(priv, buf, len);
    return 0;
}

static int read_buf_rs(struct r61505w_priv *priv, void *buf, size_t len, int rs)
{
    dm_gpio_set_value(&priv->gpio.rs, rs);
    fbtft_read_gpio16_rd(priv, buf, len);
    return 0;
}

static int r61505w_write_reg(struct r61505w_priv *priv, int len, ...)
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
static int r61505w_read_reg(struct r61505w_priv *priv, u16 reg, void *buf, int len)
{
    va_list args;
    int i;

    write_buf_rs(priv, &reg, sizeof(reg), 0);
    read_buf_rs(priv, buf, len, 1);

    return 0;
}
#define NUMARGS(...)  (sizeof((int[]){__VA_ARGS__}) / sizeof(int))
#define write_reg(priv, ...) \
    r61505w_write_reg(priv, NUMARGS(__VA_ARGS__), __VA_ARGS__)
#define read_reg(p, r, b, l) \
    r61505w_read_reg(p, r, b, l)

static int r61505w_reset(struct r61505w_priv *priv)
{
    dm_gpio_set_value(&priv->gpio.reset, 1);
    mdelay(10);
    dm_gpio_set_value(&priv->gpio.reset, 0);
    mdelay(50);
    dm_gpio_set_value(&priv->gpio.reset, 1);
    mdelay(10);
    return 0;
}

static int r61505w_set_var(struct r61505w_priv *priv)
{
    pr_debug("%s\n", __func__);
    return 0;
}

static int r61505w_init_display(struct r61505w_priv *priv)
{
    pr_debug("%s, writing initial sequence...\n", __func__);
    r61505w_reset(priv);
    dm_gpio_set_value(&priv->gpio.rd, 1);
    mdelay(150);

    // u8 sync_buf[] = {0x00, 0x00, 0x00, 0x00};
    // write_buf_rs(priv, sync_buf, sizeof(sync_buf)/sizeof(sync_buf[0]), 0);

    /*
     * The manual says that the device code is stored in the
     * `R00h` Device Code Read IR(Index Register) and should
     * be the fixed value `0xC505`.
     */
    u16 device_code = 0x0;
    read_reg(priv, 0x00, &device_code, sizeof(device_code));
    printf("Device code: 0x%04X\n", device_code);

    // write_reg(priv, R61505W_REG_NVM_CTRL4, 0x01);
    // write_reg(priv, 0x12, 0x30);
    // write_reg(priv, R61505W_REG_DISP_CTRL1, 1 << 8);

    return 0;
}

static int r61505w_set_addr_win(struct r61505w_priv *priv, int xs, int ys, int xe,
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

static int r61505w_clear(struct r61505w_priv *priv, u16 clear)
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

static int r61505w_blank(struct r61505w_priv *priv, bool on)
{
    pr_debug("%s\n", __func__);
    return 0;
}

static int r61505w_sleep(struct r61505w_priv *priv, bool on)
{
    pr_debug("%s\n", __func__);
    return 0;
}

static const struct r61505w_operations default_r61505w_ops = {
    .init_display    = r61505w_init_display,
    .reset           = r61505w_reset,
    .clear           = r61505w_clear,
    .blank           = r61505w_blank,
    .sleep           = r61505w_sleep,
    .set_var         = r61505w_set_var,
    .set_addr_win    = r61505w_set_addr_win,
};

#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))
static int r61505w_gpio_init(struct r61505w_priv *priv)
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

static int r61505w_hw_init(struct r61505w_priv *priv)
{
    int ret;

    printf("initializing hardware...\n");

    r61505w_gpio_init(priv);

    priv->tftops->init_display(priv);

    // priv->tftops->clear(priv, 0x1234);

    /* enable backlight after screen get cleared */
    dm_gpio_set_value(&priv->gpio.bl, 1);
    pr_debug("backlight enabled\n");

    return 0;
}

static struct r61505w_display default_r61505w_display = {
    .xres   = WIDTH,
    .yres   = HEIGHT,
    .bpp    = 16,
    .rotate = 0,
};

static int r61505w_video_sync(struct r61505w_priv *priv, int xs, int ys, int xe, int ye, void *vmem16, size_t len)
{
    // pr_debug("video sync: xs=%d, ys=%d, xe=%d, ye=%d, len=%d\n", xs, ys, xe, ye, len);
    priv->tftops->set_addr_win(priv, xs, ys, xe, ye);
    write_buf_rs(priv, vmem16, len * 2, 1);
    return 0;
}

int r61505w_video_flush(int xs, int ys, int xe, int ye, void *data, size_t len)
{
    r61505w_video_sync(&g_priv, xs, ys, xe, ye, data, len);
    return 0;
}

#define PAGE_SIZE (1 << 12)
static int r61505w_probe(struct r61505w_priv *priv)
{
    pr_debug("r61505w probing ...\n");

    priv->buf = (u8 *)malloc(PAGE_SIZE);

    priv->display = &default_r61505w_display;
    priv->tftops = &default_r61505w_ops;

    priv->gpio.bl    = 28;
    priv->gpio.reset = 22;
    priv->gpio.rd    = 21;
    priv->gpio.rs    = 20;
    priv->gpio.wr    = 19;
    priv->gpio.cs    = 18;

    for (int i = 0; i < ARRAY_SIZE(priv->gpio.db); i++)
        priv->gpio.db[i] = i;

    r61505w_hw_init(priv);

    return 0;
}

#include "hardware/vreg.h"
#include "hardware/clocks.h"

#define CPU_SPEED_MHZ 240

#if 1
int main()
{
    vreg_set_voltage(VREG_VOLTAGE_DEFAULT);
    set_sys_clock_khz(CPU_SPEED_MHZ * 1000, true);
    clock_configure(clk_peri,
                    0,
                    CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLK_SYS,
                    CPU_SPEED_MHZ * MHZ,
                    CPU_SPEED_MHZ * MHZ);
    stdio_uart_init_full(uart1, 115200, 24, 25);

    pr_debug("\n\n\n\nThis is a simple test driver for r61505w\n");

    r61505w_probe(&g_priv);

    for (;;);

    u16 color = 0x0000;

    for (;color < 0xffff; color+=64) {
        g_priv.tftops->clear(&g_priv, color);
    }
}
#else
int r61505w_driver_init(void)
{
    r61505w_probe(&g_priv);
    return 0;
}
#endif
