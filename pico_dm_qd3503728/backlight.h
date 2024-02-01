#ifndef __BACKLIGHT_H
#define __BACKLIGHT_H

#include <stdint.h>

#define BACKLIGHT_PIN 28

#define BL_LVL_MIN 0
#define BL_LVL_MAX 100

#define BL_LVL_OFFSET 5

void backlight_init(void);
void backlight_set_level(uint8_t level);
uint8_t backlight_get_level(void);

#endif