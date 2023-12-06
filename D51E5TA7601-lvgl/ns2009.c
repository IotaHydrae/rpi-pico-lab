// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) 2023 embeddedboys <writeforever@foxmail.com>
 */

/*
 * Support for the ns2009
 * Note: This is a GPIO based driver, the most obvious
 * problem is the lag of refresh
 */

#include <stdint.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "pico/time.h"
#include "pico/types.h"

#define NS2009_ADDR       0x48
#define NS2009_CMD_READ_X 0xC0
#define NS2009_CMD_READ_Y 0xD0

struct ns2009_priv {
    struct {
        uint8_t addr;
        i2c_inst_t *master;
        int scl_pin;
        int sda_pin;

        uint32_t speed;
        uint32_t min_speed_hz;
        uint32_t max_speed_hz;
    }i2c;
    
    int irq_pin;
    int resolution;
    int power_mode;

    struct {
        int xres;
        int yres;
    } display;
    struct {
        int x;
        int y;
    } touch;
} g_ns2009_priv;

enum {
    NS2009_RESOLUTION_8BIT = 8,
    NS2009_RESOLUTION_12BIT = 12,
};

enum {
    NS2009_POWER_MODE_NORMAL,
    NS2009_POWER_MODE_LOW_POWER,
};

bool reserved_addr(uint8_t addr) {
    return (addr & 0x78) == 0 || (addr & 0x78) == 0x78;
}

int i2c_bus_scan(i2c_inst_t *i2c)
{
    if (!i2c)
        i2c = i2c0;

    printf("\nI2C Bus Scan\n");
    printf("   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");

    for (int addr = 0; addr < (1 << 7); ++addr) {
        if (addr % 16 == 0) {
            printf("%02x ", addr);
        }

        // Perform a 1-byte dummy read from the probe address. If a slave
        // acknowledges this address, the function returns the number of bytes
        // transferred. If the address byte is ignored, the function returns
        // -1.

        // Skip over any reserved addresses.
        int ret;
        uint8_t rxdata;
        if (reserved_addr(addr))
            ret = -1;
        else
            ret = i2c_read_blocking(i2c, addr, &rxdata, 1, false);

        printf(ret < 0 ? "." : "@");
        printf(addr % 16 == 15 ? "\n" : "  ");
    }
    printf("Done.\n");
    return 0;
}

static uint16_t ns2009_read(struct ns2009_priv *priv, uint8_t cmd)
{
    uint8_t data_out[2];
    uint16_t val = 0;

    i2c_write_blocking(priv->i2c.master, priv->i2c.addr, &cmd, 1, true);
    i2c_read_blocking(priv->i2c.master, priv->i2c.addr, data_out, 2, false);

    if (priv->resolution == NS2009_RESOLUTION_12BIT)  {
        val |= data_out[0] << 4;
        val |= data_out[1] >> 4;
    } else {
        val = data_out[0];
    }

    return val;
}

uint16_t ns2009_get_x(struct ns2009_priv *priv)
{
    uint16_t val = ns2009_read(priv, NS2009_CMD_READ_X);
    return ((val * priv->display.xres) / (1 << priv->resolution));
}

uint16_t ns2009_get_y(struct ns2009_priv *priv)
{
    uint16_t val = ns2009_read(priv, NS2009_CMD_READ_Y);
    return ((val * priv->display.yres) / (1 << priv->resolution));
}

bool ns2009_is_touched(struct ns2009_priv *priv)
{
    return !gpio_get(priv->irq_pin);
}

static void ns2009_hw_init(struct ns2009_priv *priv)
{
    i2c_init(priv->i2c.master, priv->i2c.speed);
    gpio_set_function(priv->i2c.scl_pin, GPIO_FUNC_I2C);
    gpio_set_function(priv->i2c.sda_pin, GPIO_FUNC_I2C);

    gpio_pull_up(priv->i2c.scl_pin);
    gpio_pull_up(priv->i2c.sda_pin);

    gpio_init(priv->irq_pin);
    gpio_pull_up(priv->irq_pin);

    i2c_bus_scan(priv->i2c.master);
}

static int ns2009_probe(struct ns2009_priv *priv)
{
    priv->i2c.master  = i2c1;
    priv->i2c.addr    = NS2009_ADDR;
    priv->i2c.scl_pin = 27;
    priv->i2c.sda_pin = 26;
    priv->i2c.speed   = 400000;

    priv->irq_pin = 22;

    priv->display.xres = 480;
    priv->display.yres = 320;

    priv->resolution = NS2009_RESOLUTION_12BIT;
    priv->power_mode = NS2009_POWER_MODE_LOW_POWER;

    ns2009_hw_init(priv);

    return 0;
}

int ns2009_driver_init(void)
{
    printf("ns2009_driver_init\n");
    ns2009_probe(&g_ns2009_priv);
    return 0;
}

#if 0
int main(void)
{
    stdio_uart_init_full(uart0, 115200, 16, 17);

    printf("This is a simple driver for ns2009!\n");
    ns2009_driver_init();

    for (;;) {
        if (!ns2009_is_touched(&g_ns2009_priv))
            continue;
        
        printf("x: %d, y: %d\n", ns2009_get_x(&g_ns2009_priv), ns2009_get_y(&g_ns2009_priv));
        sleep_ms(100);
    }
    return 0;
}
#endif