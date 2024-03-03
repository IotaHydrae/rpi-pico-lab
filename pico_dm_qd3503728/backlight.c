// Copyright (c) 2024 embeddedboys developers

// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:

// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include <stdio.h>
#include "hardware/gpio.h"
#include "hardware/pwm.h"

#include "backlight.h"

#define BL_LVL_MIN 0
#define BL_LVL_MAX 100

#define BL_LVL_OFFSET 5

struct backlight_device {
    const uint8_t bl_pin;
    uint8_t bl_lvl;

    const uint8_t bl_lvl_min;
    const uint8_t bl_lvl_max;
    uint8_t bl_lvl_offset;
} g_bl_priv;

static uint8_t g_bl_lvl = 100;

void backlight_set_level(uint8_t level)
{
    /* we shouldn't set backlight percent to 0%, otherwise we can't see nothing */
    uint8_t percent = (level + BL_LVL_OFFSET) > 100 ? 100 : (level + BL_LVL_OFFSET);

    /* To pwm level */
    uint16_t pwm_lvl = (percent * 65535 / 100);
    pwm_set_gpio_level(LCD_PIN_BL, pwm_lvl);

    g_bl_lvl = percent;
}

uint8_t backlight_get_level(void)
{
    return g_bl_lvl;
}

void backlight_init(void)
{
    gpio_init(LCD_PIN_BL);
    gpio_set_function(LCD_PIN_BL, GPIO_FUNC_PWM);

    uint slice_num = pwm_gpio_to_slice_num(LCD_PIN_BL);

    pwm_config config = pwm_get_default_config();
    // Set divider, reduces counter clock to sysclock/this value
    pwm_config_set_clkdiv(&config, 1.f);
    pwm_init(slice_num, &config, true);

    pwm_set_gpio_level(LCD_PIN_BL, 0);
}