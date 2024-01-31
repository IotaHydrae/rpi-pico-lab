#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"

// #include "i2c-gpio.h"
#include "ft6236.h"

#define FT6236_ADDR 0x38
#define FT6236_DEFAULT_SPEED 400000

struct ft6236_data {
    struct {
        uint8_t addr;
        i2c_inst_t *master;
        uint8_t scl_pin;
        uint8_t sda_pin;
    } i2c;

    uint8_t irq_pin;
    uint8_t rst_pin;
} g_ft6236_data;

extern int i2c_bus_scan(i2c_inst_t *i2c);

static int ft6236_write_reg(struct ft6236_data *priv, uint8_t reg, uint8_t val)
{
    uint16_t buf = val << 8 | reg;
    i2c_write_blocking(priv->i2c.master, priv->i2c.addr, (uint8_t *)&buf, sizeof(buf), false);
}
#define write_reg ft6236_write_reg

static uint8_t ft6236_read_reg(struct ft6236_data *priv, uint8_t reg)
{
    uint8_t val;
    i2c_write_blocking(priv->i2c.master, priv->i2c.addr, &reg, 1, true);
    i2c_read_blocking(priv->i2c.master, priv->i2c.addr, &val, 1, false);
    return val;
}
#define read_reg ft6236_read_reg

static void ft6236_hw_init(struct ft6236_data *priv)
{
    i2c_init(priv->i2c.master, FT6236_DEFAULT_SPEED);

    gpio_set_function(priv->i2c.scl_pin, GPIO_FUNC_I2C);
    gpio_set_function(priv->i2c.sda_pin, GPIO_FUNC_I2C);

    gpio_pull_up(priv->i2c.scl_pin);
    gpio_pull_up(priv->i2c.sda_pin);
}

static uint16_t __ft6236_read_x(struct ft6236_data *priv)
{
    uint8_t val_h = read_reg(priv, FT_REG_TOUCH1_XH) & 0x1f;
    uint8_t val_l = read_reg(priv, FT_REG_TOUCH1_XL);
    // printf("ft6236_read_x: %02x %02x\n", val_h, val_l);
    return (val_h << 8) | val_l;
}

uint16_t ft6236_read_x(void)
{
    return __ft6236_read_x(&g_ft6236_data);
}

static uint16_t __ft6236_read_y(struct ft6236_data *priv)
{
    uint8_t val_h = read_reg(priv, FT_REG_TOUCH1_YH);
    uint8_t val_l = read_reg(priv, FT_REG_TOUCH1_YL);
    return (val_h << 8) | val_l;
}

uint16_t ft6236_read_y(void)
{
    return __ft6236_read_y(&g_ft6236_data);
}

static bool __ft6236_is_pressed(struct ft6236_data *priv)
{
    uint8_t val = read_reg(priv, FT_REG_TD_STATUS);
    // printf("ft6236_is_pressed: %02x\n", val);
    return val;
}

bool ft6236_is_pressed(void)
{
    return __ft6236_is_pressed(&g_ft6236_data);
}

static int ft6236_probe(struct ft6236_data *priv)
{
    priv->i2c.master  = i2c1;
    priv->i2c.addr    = FT6236_ADDR;
    priv->i2c.scl_pin = 27;
    priv->i2c.sda_pin = 26;

    priv->irq_pin = 21;
    priv->rst_pin = 18;

    ft6236_hw_init(priv);

    return 0;
}

int ft6236_driver_init(void)
{
    printf("ft6236_driver_init\n");
    ft6236_probe(&g_ft6236_data);
    return 0;
}