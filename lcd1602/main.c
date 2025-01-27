#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "pico/stdlib.h"

#include "hardware/clocks.h"

#define LCD_RS_PIN  15
#define LCD_WR_PIN  14
#define LCD_EN_PIN   13

#define LCD_DATA_PINS { 7, 6, 5, 4, 3, 2, 1, 0 }
#define LCD_DATA_PIN_COUNT 8

struct lcd1602 {
    uint8_t rs_pin;
    uint8_t rw_pin;
    uint8_t en_pin;

    uint8_t data_pins[LCD_DATA_PIN_COUNT];
    uint8_t data_pin_count;
};

static struct lcd1602 g_lcd = {0};

static void lcd_wait_busy(struct lcd1602 *priv)
{
    char tmp = 0x00;
    int timeout =  20;

    while (timeout--) {
        sleep_us(50);
        gpio_put(priv->rs_pin, 0);
        gpio_put(priv->rw_pin, 1);

        gpio_put(priv->en_pin, 0);
        gpio_put(priv->en_pin, 1);
        for (uint8_t i = 7; i < priv->data_pin_count; i--) {
            // printf("%d ", gpio_get(priv->data_pins[i]));
            tmp |= (gpio_get(priv->data_pins[i]) << i);
        }
        // printf(", tmp : %d\n", tmp);
        gpio_put(priv->en_pin, 0);

        if (!(tmp & 0x80))
            break;
    }

}

static void lcd_write_byte(struct lcd1602 *priv, uint8_t val)
{
    gpio_put(priv->en_pin, 0);
    for (uint8_t i = 0; i < priv->data_pin_count; i++)
    {
        gpio_put(priv->data_pins[i], (val >> i) & 0x01);
    }
    gpio_put(priv->en_pin, 1);
    // sleep_us(1);
    gpio_put(priv->en_pin, 0);
}

static void lcd_write_cmd(struct lcd1602 *priv, uint8_t cmd)
{
    lcd_wait_busy(priv);
    gpio_put(priv->rs_pin, 0);
    gpio_put(priv->rw_pin, 0);
    lcd_write_byte(priv, cmd);
}
#define write_cmd(priv, cmd) lcd_write_cmd(priv, cmd)

static void lcd_write_data(struct lcd1602 *priv, uint8_t data)
{
    lcd_wait_busy(priv);
    gpio_put(priv->rs_pin, 1);
    gpio_put(priv->rw_pin, 0);
    lcd_write_byte(priv, data);
}
#define write_data(priv, data) lcd_write_data(priv, data)

static void lcd_send_string(struct lcd1602 *priv, uint8_t row, uint8_t col, const char *str)
{
    if (row > 0)
        lcd_write_cmd(priv, 0xC0 + col);
    else
        lcd_write_cmd(priv, 0x80 + col);

    while (*str) {
        lcd_write_data(priv, *str++);
    }
}

static void lcd_init(struct lcd1602 *priv)
{
    gpio_init(priv->rs_pin);
    gpio_init(priv->rw_pin);
    gpio_init(priv->en_pin);
    gpio_set_dir(priv->rs_pin, GPIO_OUT);
    gpio_set_dir(priv->rw_pin, GPIO_OUT);
    gpio_set_dir(priv->en_pin, GPIO_OUT);

    gpio_put(priv->rs_pin, 0);
    gpio_put(priv->rw_pin, 0);
    gpio_put(priv->en_pin, 0);

    for (uint8_t i = 0; i < priv->data_pin_count; i++)
    {
        gpio_init(priv->data_pins[i]);
        gpio_set_dir(priv->data_pins[i], GPIO_OUT);
    }

    sleep_ms(15);
    write_cmd(priv, 0x38);
    write_cmd(priv, 0x0C);
    write_cmd(priv, 0x06);
    write_cmd(priv, 0x01);
}

int main()
{
    uint8_t data_pins[] = LCD_DATA_PINS;
    struct lcd1602 *priv = &g_lcd;

    set_sys_clock_48mhz();
    stdio_init_all();

    sleep_ms(1000);

    printf("LCD1602 mode test\n");

    g_lcd.rs_pin = LCD_RS_PIN;
    g_lcd.rw_pin = LCD_WR_PIN;
    g_lcd.en_pin = LCD_EN_PIN;

    for (int i = 0; i < LCD_DATA_PIN_COUNT; i++)
        g_lcd.data_pins[i] = data_pins[i];

    g_lcd.data_pin_count = LCD_DATA_PIN_COUNT;

    lcd_init(&g_lcd);

    printf("going to loop \n");
    srand(time(NULL));
    for (int i = 0;; i++) {
        lcd_write_data(&g_lcd, rand() % 256);
        if (i == 16) {
            lcd_write_cmd(&g_lcd, 0xC0);
        } else if (i == 32) {
            lcd_write_cmd(&g_lcd, 0x80);
            i = 0;
        }
        sleep_ms(50);
    }
    return 0;
}
