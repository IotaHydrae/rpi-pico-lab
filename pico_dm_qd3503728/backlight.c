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

#define BL_LVL_DEF_MIN 0
#define BL_LVL_DEF_MAX 100
#define BL_LVL_DEF_OFFSET 5
#define BL_LVL_DEF_LVL 100

struct backlight_device;

struct backlight_profile {
    u8 bl_lvl_min;
    u8 bl_lvl_max;
    u8 bl_lvl_offs;
    u8 bl_lvl_default;
};

struct backlight_ops {
    void (*hw_init)(struct backlight_device *dev);

    void (*set_lvl)(struct backlight_device *dev, u8 level);
    u8 (*get_lvl)(struct backlight_device *dev);
    void (*set_offs)(struct backlight_device *dev, u8 offset);
    u8 (*get_offs)(struct backlight_device *dev);

    void (*set_min)(struct backlight_device *dev, u8 min);
    u8 (*get_min)(struct backlight_device *dev);

    void (*set_max)(struct backlight_device *dev, u8 max);
    u8 (*get_max)(struct backlight_device *dev);

    void (*bl_drv_update_cb)(struct backlight_device *dev);
};

struct backlight_device {
    u8 bl_pin;
    u8 bl_lvl;

    struct backlight_profile prof;
} g_bl_priv;


void __bl_set_lvl(struct backlight_device *dev, u8 level)
{
    /* we shouldn't set backlight percent to 0%, otherwise we can't see nothing */
    u8 percent = (level + dev->prof.bl_lvl_offs) > 100 ? 100 : (level + dev->prof.bl_lvl_offs);

    /* To pwm level */
    u16 pwm_lvl = (percent * 65535 / 100);
    pwm_set_gpio_level(dev->bl_pin, pwm_lvl);

    dev->bl_lvl = percent;
}

void backlight_set_level(u8 level)
{
    __bl_set_lvl(&g_bl_priv, level);
}

static u8 __bl_get_lvl(struct backlight_device *dev)
{
    return dev->bl_lvl;
}

u8 backlight_get_level(void)
{
    return __bl_get_lvl(&g_bl_priv);
}

static void __bl_set_offset(struct backlight_device *dev, uint8_t offset)
{
    dev->prof.bl_lvl_offs = offset;
}

void backlight_set_offset(u8 offset)
{
    __bl_set_offset(&g_bl_priv, offset);
}

static u8 __bl_get_offset(struct backlight_device *dev)
{
    return dev->prof.bl_lvl_offs;
}

u8 backlight_get_offset(void)
{
    return __bl_get_offset(&g_bl_priv);
}

static void backlight_hw_init(struct backlight_device *dev)
{
    gpio_init(dev->bl_pin);
    gpio_set_function(dev->bl_pin, GPIO_FUNC_PWM);

    uint slice_num = pwm_gpio_to_slice_num(dev->bl_pin);

    pwm_config config = pwm_get_default_config();
    // Set divider, reduces counter clock to sysclock/this value
    pwm_config_set_clkdiv(&config, 1.f);
    pwm_init(slice_num, &config, true);

    pwm_set_gpio_level(dev->bl_pin, 0);
}

struct backlight_profile avaliable_profiles[] = {
    [0] = {
        .bl_lvl_min = 0,
        .bl_lvl_max = 100,
        .bl_lvl_offs = 5,
        .bl_lvl_default = 100,
    },
};

static void backlight_load_profile(struct backlight_device *dev, struct backlight_profile *profile)
{
    dev->prof.bl_lvl_min = profile->bl_lvl_min;
    dev->prof.bl_lvl_max = profile->bl_lvl_max;
    dev->prof.bl_lvl_offs = profile->bl_lvl_offs;
    dev->prof.bl_lvl_default = profile->bl_lvl_default;
}

struct backlight_profile def_bl_profile = {
    .bl_lvl_min = BL_LVL_DEF_MIN,
    .bl_lvl_max = BL_LVL_DEF_MAX,
    .bl_lvl_offs = BL_LVL_DEF_OFFSET,
    .bl_lvl_default = BL_LVL_DEF_LVL,
};

void backlight_driver_init(void)
{
    /* make default setting */
    g_bl_priv.bl_pin = LCD_PIN_BL;

    backlight_load_profile(&g_bl_priv, &def_bl_profile);

    backlight_hw_init(&g_bl_priv);
}