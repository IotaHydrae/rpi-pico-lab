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
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>

#include "pico/time.h"
#include "pico/stdio.h"
#include "pico/stdlib.h"
#include "pico/platform.h"
#include "pico/stdio_uart.h"
#include "pico/binary_info.h"

#include "hardware/pwm.h"
#include "hardware/pll.h"
#include "hardware/spi.h"
#include "hardware/vreg.h"
#include "hardware/clocks.h"
#include "hardware/timer.h"

#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"
#include "lvgl/examples/lv_examples.h"
#include "port/lv_port_disp.h"

#define TAG "st7789v: "

#define pr_debug(...) printf(TAG __VA_ARGS__)

#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))
#define dm_gpio_set_value(p,v) gpio_put(p, v)
#define mdelay(v) busy_wait_ms(v)

#if ST7789V_SPIX == 0
    #define spi_ifce spi0
#elif ST7789V_SPIX == 1
    #define spi_ifce spi1
#else
    #define spi_ifce spi_default
#endif

static void st7789v_reset()
{
    pr_debug("%s\n", __func__);
    dm_gpio_set_value(ST7789V_RES_PIN, 1);
    mdelay(10);
    dm_gpio_set_value(ST7789V_RES_PIN, 0);
    mdelay(10);
    dm_gpio_set_value(ST7789V_RES_PIN, 1);
    mdelay(10);
}

static void st7789v_spi_write_buf_dc(void *buf, size_t len, bool dc)
{
    gpio_put(ST7789V_DC_PIN, dc);

    dm_gpio_set_value(ST7789V_CS_PIN, 0);
    spi_write_blocking(spi_ifce, buf, len);
    dm_gpio_set_value(ST7789V_CS_PIN, 1);
}

#if DISP_OVER_PIO
extern int pio_spi_tx_init(uint data_pin, uint clk_pin);
extern void pio_spi_tx_write_buf_dc(void *buf, size_t len, bool dc);
#define write_buf_dc pio_spi_tx_write_buf_dc
#else
#define write_buf_dc st7789v_spi_write_buf_dc
#endif

static uint8_t reg_buf[64];
static void st7789v_write_reg(int len, ...)
{
    uint8_t *buf = reg_buf;
    va_list args;
    int i;

    va_start(args, len);
    *buf = (uint8_t)va_arg(args, unsigned int);
    write_buf_dc(buf, sizeof(uint8_t), 0);
    len--;

    /* if there no privams */
    if (len == 0) {
        goto exit_no_param;
    }

    for (i = 0; i < len; i++) {
        *buf++ = (uint8_t)va_arg(args, unsigned int);
    }

    write_buf_dc(reg_buf, len, 1);

exit_no_param:
    va_end(args);
}
#define NUMARGS(...)  (sizeof((int[]){__VA_ARGS__}) / sizeof(int))
#define write_reg(...) \
    st7789v_write_reg(NUMARGS(__VA_ARGS__), __VA_ARGS__)

static void st7789v_init_display(void)
{
    st7789v_reset();

    write_reg(0x11);
    sleep_ms(120);

    write_reg(0x36, 0x00);

    write_reg(0x3A, 0x05);

    write_reg(0xB2, 0x0C, 0x0C, 0x00, 0x33, 0x33);

    write_reg(0xB7, 0x35);

    write_reg(0xBB, 0x32);

    write_reg(0xC0, 0x2C);

    write_reg(0xC2, 0x01);

    write_reg(0xC3, 0x15);

    //VDV, 0x20:0v
    write_reg(0xC4, 0x20);

    //0x0F:60Hz
    write_reg(0xC6, 0x0F);

    write_reg(0xD0, 0xA4, 0xA1);

    //sleep in后，gate输出为GND
    write_reg(0xD6, 0xA1);

    write_reg(0xE0, 0xD0, 0x08, 0x0E, 0x09, 0x09, 0x05, 0x31, 0x33, 0x48, 0x17, 0x14, 0x15, 0x31, 0x34);

    write_reg(0xE1, 0xD0, 0x08, 0x0E, 0x09, 0x09, 0x15, 0x31, 0x33, 0x48, 0x17, 0x14, 0x15, 0x31, 0x34);

    //使用240根gate  (N+1)*8
    //设定gate起点位置
    //当gate没有用完时，bit4(TMG)设为0
    write_reg(0xE4, 0x25, 0x00, 0x00);

    write_reg(0x21);
    write_reg(0x29);
}

static void st7789v_set_addr_win(int xs, int ys, int xe, int ye)
{
    /* set column adddress */
    write_reg(0x2A, xs >> 8, xs & 0xFF, xe >> 8, xe & 0xFF);

    /* set row address */
    write_reg(0x2B, ys >> 8, ys & 0xFF, ye >> 8, ye & 0xFF);

    /* write start */
    write_reg(0x2C);
}

void st7789v_video_flush(int xs, int ys, int xe, int ye, void *vmem16, size_t len)
{
    st7789v_set_addr_win(xs, ys, xe, ye);
    write_buf_dc(vmem16, len, 1);
}

static void st7789v_set_backlight(uint16_t level)
{
    pwm_set_gpio_level(ST7789V_BLK_PIN, level * level);
}

/**
 * @brief hardware layer initialize
 * for each platform, do it's iomux and pinctl here
 */
static void hardware_init(void)
{
    /* NOTE: DO NOT MODIFY THIS BLOCK */
#define CPU_SPEED_MHZ (DEFAULT_SYS_CLK_KHZ / 1000)
    if(CPU_SPEED_MHZ > 266 && CPU_SPEED_MHZ <= 396)
        vreg_set_voltage(VREG_VOLTAGE_1_20);
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
    stdio_uart_init_full(uart0, 115200, 0, 1);

    /* Useing default SPI0 at 62500000 */
#if DISP_OVER_PIO
    pio_spi_tx_init(ST7789V_SDA_PIN, ST7789V_SCL_PIN);
    bi_decl(bi_2pins_with_func(ST7789V_SCL_PIN, ST7789V_SDA_PIN, GPIO_FUNC_PIO0));
#else
    spi_init(spi_ifce, ST7789V_BUS_CLK_KHZ * 1000);
    gpio_set_function(ST7789V_SCL_PIN, GPIO_FUNC_SPI);
    gpio_set_function(ST7789V_SDA_PIN, GPIO_FUNC_SPI);
    bi_decl(bi_2pins_with_func(ST7789V_SCL_PIN, ST7789V_SDA_PIN, GPIO_FUNC_SPI));
    pr_debug("spi%d initialized at %d kHz\n", spi_get_index(spi_ifce), spi_get_baudrate(spi_ifce) / 1000 );
#endif

    gpio_init(ST7789V_CS_PIN);
    gpio_set_dir(ST7789V_CS_PIN, GPIO_OUT);
    gpio_put(ST7789V_CS_PIN, 1);
    bi_decl(bi_1pin_with_name(ST7789V_CS_PIN, "SPI CS"));

    gpio_init(ST7789V_RES_PIN);
    gpio_set_dir(ST7789V_RES_PIN, GPIO_OUT);
    bi_decl(bi_1pin_with_name(ST7789V_RES_PIN, "TFT RES"));

    gpio_init(ST7789V_DC_PIN);
    gpio_set_dir(ST7789V_DC_PIN, GPIO_OUT);
    bi_decl(bi_1pin_with_name(ST7789V_DC_PIN, "TFT DC"));

    gpio_set_function(ST7789V_BLK_PIN, GPIO_FUNC_PWM);
    bi_decl(bi_1pin_with_name(ST7789V_BLK_PIN, "TFT BLK"));

    uint32_t slice_num = pwm_gpio_to_slice_num(ST7789V_BLK_PIN);
    // pwm_clear_irq(slice_num);
    // pwm_set_irq_enabled(slice_num, true);

    // irq_set_exclusive_handler(PWM_IRQ_WRAP, on_pwm_wrap);
    // irq_set_enabled(PWM_IRQ_WRAP, true);

    pwm_config config = pwm_get_default_config();

    pwm_config_set_clkdiv(&config, 4.f);
    pwm_init(slice_num, &config, true);

    st7789v_init_display();
    st7789v_set_backlight(128);
}

bool lvgl_timer_callback(struct repeating_timer *t)
{
    lv_timer_handler();
    return true;
}

int main(void)
{
    hardware_init();

    lv_init();
    lv_port_disp_init();

    pr_debug("Starting APP...\n");
    // lv_obj_t *btn = lv_btn_create(lv_scr_act());
    // lv_obj_set_style_bg_color(btn, lv_color_hex(0x1234), 0);
    // lv_obj_set_style_radius(btn, 10, 0);
    // lv_obj_center(btn);

    // lv_obj_t *label = lv_label_create(btn);
    // lv_label_set_text(label, "embeddedboys");

    // lv_demo_widgets();
    // lv_demo_stress();
    // lv_demo_music();
    lv_demo_benchmark();

    struct repeating_timer lvgl_timer;
    add_repeating_timer_ms(1, lvgl_timer_callback, NULL, &lvgl_timer);

    pr_debug("going to loop...\n");
    for (;;) {
        tight_loop_contents();
        sleep_ms(200);
    }
}