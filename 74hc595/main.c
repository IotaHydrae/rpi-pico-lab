#include "pico/stdlib.h"

#define HC595_PIN_DS  2
#define HC595_PIN_RCK 3
#define HC595_PIN_SCK 4

#define HC595_COUNT 2

extern void tft_init(void);
extern void tft_test_init(void);

static void gpiod_set_value(int pin, bool val)
{
    asm volatile("nop\n nop\n nop\n nop\n nop\n nop\n");
    gpio_put(pin, val);
    asm volatile("nop\n nop\n nop\n nop\n nop\n nop\n");
}

#define gpio_put gpiod_set_value

void hc595_out8(uint8_t val)
{
    gpio_put(HC595_PIN_SCK, 0);
    gpio_put(HC595_PIN_RCK, 0);
    for (int i = 0; i < 8; i++) {
        gpio_put(HC595_PIN_DS, val & 0x1);
        gpio_put(HC595_PIN_SCK, 1);
        val >>= 1;
        gpio_put(HC595_PIN_SCK, 0);
    }
    gpio_put(HC595_PIN_RCK, 1);
}

void hc595_out16(uint16_t val)
{
    int i;
    val = (val << 8) | (val >> 8);
    
    gpio_put(HC595_PIN_RCK, 0);
    
    for (i = 0; i < 16; i++) {
        gpio_put(HC595_PIN_SCK, 0);
        gpio_put(HC595_PIN_DS, val & 0x8000);
        gpio_put(HC595_PIN_SCK, 1);
        val <<= 1;
    }

    gpio_put(HC595_PIN_RCK, 1);
    gpio_put(HC595_PIN_SCK, 0);
    gpio_put(HC595_PIN_RCK, 0);
}

int main()
{
    set_sys_clock_48mhz();

    gpio_init(HC595_PIN_DS);
    gpio_init(HC595_PIN_RCK);
    gpio_init(HC595_PIN_SCK);

    gpio_set_dir(HC595_PIN_DS, GPIO_OUT);
    gpio_set_dir(HC595_PIN_RCK, GPIO_OUT);
    gpio_set_dir(HC595_PIN_SCK, GPIO_OUT);

    gpio_set_drive_strength(HC595_PIN_DS, GPIO_DRIVE_STRENGTH_2MA);
    gpio_set_drive_strength(HC595_PIN_RCK, GPIO_DRIVE_STRENGTH_2MA);
    gpio_set_drive_strength(HC595_PIN_SCK, GPIO_DRIVE_STRENGTH_2MA);
    
    gpio_set_slew_rate(HC595_PIN_DS, GPIO_SLEW_RATE_FAST);
    gpio_set_slew_rate(HC595_PIN_RCK, GPIO_SLEW_RATE_FAST);
    gpio_set_slew_rate(HC595_PIN_SCK, GPIO_SLEW_RATE_FAST);

    // tft_test_init();

    // uint16_t val = 0x11;
    // // val <<= 8;
    // for (;;) {
    //     // hc595_out8(0x11);
    //     hc595_out16(val);
    // }

    tft_init();
    return 0;
}
