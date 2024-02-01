#include <stdio.h>
#include "hardware/gpio.h"
#include "hardware/pwm.h"

#include "backlight.h"

static uint8_t g_bl_lvl = 100;

void backlight_set_level(uint8_t level)
{
    /* we shouldn't set backlight percent to 0%, otherwise we can't see nothing */
    uint8_t percent = (level + BL_LVL_OFFSET) > 100 ? 100 : (level + BL_LVL_OFFSET);

    /* To pwm level */
    uint16_t pwm_lvl = (percent * 65535 / 100);
    pwm_set_gpio_level(BACKLIGHT_PIN, pwm_lvl);

    g_bl_lvl = percent;
}

uint8_t backlight_get_level(void)
{
    return g_bl_lvl;
}

void backlight_init(void)
{
    gpio_init(BACKLIGHT_PIN);
    gpio_set_function(BACKLIGHT_PIN, GPIO_FUNC_PWM);

    uint slice_num = pwm_gpio_to_slice_num(BACKLIGHT_PIN);

    pwm_config config = pwm_get_default_config();
    // Set divider, reduces counter clock to sysclock/this value
    pwm_config_set_clkdiv(&config, 1.f);
    pwm_init(slice_num, &config, true);

    pwm_set_gpio_level(BACKLIGHT_PIN, 0);
}