#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/vreg.h"
#include "hardware/clocks.h"

// #define HC595_PIN_DS  2
// #define HC595_PIN_RCK 3
// #define HC595_PIN_SCK 4

#define HC595_PIN_SCK 6
#define HC595_PIN_DS  7
#define HC595_PIN_RCK 9

#define HC595_COUNT 1

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
    int i;
    // val = (val << 8) | (val >> 8);
    
    gpio_put(HC595_PIN_RCK, 0);
    
    for (i = 0; i < 8; i++) {
        gpio_put(HC595_PIN_SCK, 0);
        gpio_put(HC595_PIN_DS, val & 0x80);
        gpio_put(HC595_PIN_SCK, 1);
        val <<= 1;
    }

    gpio_put(HC595_PIN_RCK, 1);
    gpio_put(HC595_PIN_SCK, 0);
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
}

int main()
{
    /* NOTE: DO NOT MODIFY THIS BLOCK */
#define CPU_SPEED_MHZ (DEFAULT_SYS_CLK_KHZ / 1000)
    if(CPU_SPEED_MHZ > 266 && CPU_SPEED_MHZ <= 360)
        vreg_set_voltage(VREG_VOLTAGE_1_20);
    else if (CPU_SPEED_MHZ > 360 && CPU_SPEED_MHZ <= 396)
        vreg_set_voltage(VREG_VOLTAGE_1_25);
    else if (CPU_SPEED_MHZ > 396)
        vreg_set_voltage(VREG_VOLTAGE_MAX);
    else
        vreg_set_voltage(VREG_VOLTAGE_DEFAULT);

    set_sys_clock_khz(CPU_SPEED_MHZ * 1000, true);
    clock_configure(clk_peri,
                    0,
                    CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLK_SYS,
                    CPU_SPEED_MHZ * MHZ,
                    CPU_SPEED_MHZ * MHZ);
    stdio_uart_init_full(uart0, 115200, 16, 17);

    printf("\n\n74hc595 tft testing...\n");

    // set_sys_clock_48mhz();
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
