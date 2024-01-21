/*
MIT License

Copyright (c) 2023 embeddedboys

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"

#define PCF8574_ADDR 0x20

struct pcf8474_data {
    struct {
    uint8_t addr;
    i2c_inst_t *master;
    uint8_t scl_pin;
    uint8_t sda_pin;

    } i2c;


    union {
        struct {
            uint8_t p0 : 1;
            uint8_t p1 : 1;
            uint8_t p2 : 1;
            uint8_t p3 : 1;
            uint8_t p4 : 1;
            uint8_t p5 : 1;
            uint8_t p6 : 1;
            uint8_t p7 : 1;
        }pins;
        uint8_t all;
    } gpio;

    uint8_t irq_pin;

};

static struct pcf8474_data g_pcf8474_data;

extern int i2c_bus_scan(i2c_inst_t *i2c);

static inline int pcf8574_write_byte(struct pcf8474_data *priv, uint8_t val)
{
    int rc;
    i2c_write_blocking(priv->i2c.master, priv->i2c.addr, &val, 1, false);
    return 0;
}

static inline uint8_t pcf8574_read_byte(struct pcf8474_data *priv)
{
    uint8_t rxdata;
    i2c_read_blocking(priv->i2c.master, priv->i2c.addr, &rxdata, 1, false);
    return rxdata;
}

static int __pcf8574_gpio_put(struct pcf8474_data *priv, int pin, bool val)
{
    if (pin < 0 || pin > 7) {
        printf("pcf8574_gpio_put: invalid pin %d\n", pin);
        return -1;
    }

    if (val)
        priv->gpio.all |= (1 << pin);
    else
        priv->gpio.all &= ~(1 << pin);

    // printf("gpio state: 0x%02x\n", priv->gpio.all);
    pcf8574_write_byte(priv, priv->gpio.all);

    return 0;
}

int pcf8574_gpio_put(int pin, bool val)
{
    return __pcf8574_gpio_put(&g_pcf8474_data, pin, val);
}

static int __pcf8574_gpio_put_all(struct pcf8474_data *priv, uint8_t val)
{
    priv->gpio.all = val;
    // printf("gpio state: 0x%02x\n", priv->gpio.all);
    pcf8574_write_byte(priv, priv->gpio.all);
    return 0;
}

int pcf8574_gpio_put_all(uint8_t val)
{
    return __pcf8574_gpio_put_all(&g_pcf8474_data, val);
}

static bool __pcf8574_gpio_get(struct pcf8474_data *priv, int pin)
{
    if (pin < 0 || pin > 7) {
        printf("pcf8574_gpio_get: invalid pin %d\n", pin);
        return -1;
    }
    
    priv->gpio.all = pcf8574_read_byte(priv);
    // printf("pin state: 0x%02x\n", (priv->gpio.all >> pin) & 0x01);
    return (priv->gpio.all >> pin) & 0x01;
}

bool pcf8574_gpio_get(int pin)
{
    return __pcf8574_gpio_get(&g_pcf8474_data, pin);
}

static uint8_t __pcf8574_gpio_get_all(struct pcf8474_data *priv)
{
    priv->gpio.all = pcf8574_read_byte(priv);
    // printf("gpio state: 0x%02x\n", priv->gpio.all);
    return priv->gpio.all;
}

uint8_t pcf8574_gpio_get_all(void)
{
    return __pcf8574_gpio_get_all(&g_pcf8474_data);
}

static int pcf8574_hw_init(struct pcf8474_data *priv)
{
    i2c_init(priv->i2c.master, 400000);
    gpio_set_function(priv->i2c.scl_pin, GPIO_FUNC_I2C);
    gpio_set_function(priv->i2c.sda_pin, GPIO_FUNC_I2C);

    gpio_pull_up(priv->i2c.scl_pin);
    gpio_pull_up(priv->i2c.sda_pin);

    // gpio_init(priv->irq_pin);
    // gpio_pull_up(priv->irq_pin);

    i2c_bus_scan(priv->i2c.master);

    __pcf8574_gpio_put_all(priv, priv->gpio.all);
}

static int pcf8574_probe(struct pcf8474_data *priv)
{
    priv->i2c.master  = i2c1;
    priv->i2c.addr    = PCF8574_ADDR;
    priv->i2c.scl_pin = 27;
    priv->i2c.sda_pin = 26;

    // priv->irq_pin = 22;

    priv->gpio.all = 0xff;

    pcf8574_hw_init(priv);

    return 0;
}

int pcf8574_driver_init(void)
{
    printf("pcf8574_driver_init\n");
    pcf8574_probe(&g_pcf8474_data);
    return 0;
}

int pcf8574_driver_test(void)
{
    // pcf8574_gpio_put(7, 1);
}