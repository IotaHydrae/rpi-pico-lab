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

#define TAG "tft: "

#define pr_debug(...) printf(TAG __VA_ARGS__)

#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))
#define dm_gpio_set_value(p,v) gpio_put(p, v)
#define mdelay(v) busy_wait_ms(v)

#if TFT_SPIX == 0
    #define spi_ifce spi0
#elif TFT_SPIX == 1
    #define spi_ifce spi1
#else
    #define spi_ifce spi_default
#endif

static void tft_reset()
{
    pr_debug("%s\n", __func__);
    dm_gpio_set_value(TFT_RES_PIN, 1);
    mdelay(10);
    dm_gpio_set_value(TFT_RES_PIN, 0);
    mdelay(10);
    dm_gpio_set_value(TFT_RES_PIN, 1);
    mdelay(10);
}

static void tft_spi_write_buf_dc(void *buf, size_t len, bool dc)
{
    gpio_put(TFT_DC_PIN, dc);

    dm_gpio_set_value(TFT_CS_PIN, 0);
    spi_write_blocking(spi_ifce, buf, len);
    dm_gpio_set_value(TFT_CS_PIN, 1);
}

#if DISP_OVER_PIO
extern int pio_spi_tx_init(uint data_pin, uint clk_pin);
extern void pio_spi_tx_write_buf_dc(void *buf, size_t len, bool dc);
#define write_buf_dc pio_spi_tx_write_buf_dc
#else
#define write_buf_dc tft_spi_write_buf_dc
#endif

static uint8_t reg_buf[64];
static void tft_write_reg(int len, ...)
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
    tft_write_reg(NUMARGS(__VA_ARGS__), __VA_ARGS__)

static void tft_wait_for_busy(void)
{
    while( gpio_get( TFT_BLK_PIN ) );
}

static void tft_init_display(void)
{
    tft_reset();
    tft_wait_for_busy();

    write_reg(0x12);
    tft_wait_for_busy();

    write_reg(0x01, 0xC7, 0x00, 0x01);

    write_reg(0x11, 0x01);

    write_reg(0x44, 0x00, 0x18);

    write_reg(0x45, 0xC7, 0x00, 0x00, 0x00);

    write_reg(0x3C, 0x05);

    write_reg(0x18, 0x80);

    write_reg(0x4E, 0x00, 0x4F, 0xC7, 0x00);
    tft_wait_for_busy();
}

static void tft_set_addr_win(int xs, int ys, int xe, int ye)
{
    /* set column adddress */
    write_reg(0x44, xs & 0xFF, xe & 0xFF);

    /* set row address */
    write_reg(0x45, ys & 0xFF, (ys >> 8) & 0xFF, ye & 0xFF, (ye >> 8) & 0xFF);
}

static void tft_set_cursor(int x, int y)
{
    write_reg(0x4E, x & 0xFF);
    write_reg(0x4F, y & 0xFF, (y >> 8) & 0xFF);
}

static void tft_part_refresh(void)
{
    write_reg(0x22, 0xFF);
    write_reg(0x20);
    tft_wait_for_busy();
}

static void tft_full_refresh(void)
{
    write_reg(0x22, 0xF7);
    write_reg(0x20);
    tft_wait_for_busy();
}

static void tft_fast_refresh(void)
{
    write_reg(0x22, 0xC7);
    write_reg(0x20);
    tft_wait_for_busy();
}

static void tft_clear(void)
{
    uint8_t width, height;
    width = ( TFT_HOR_RES % 8 == 0 ) ? ( TFT_HOR_RES / 8 ) :
            ( TFT_HOR_RES / 8 + 1 );
    height = TFT_VER_RES;
    int i, j;

    tft_reset();
    write_reg(0x3C, 0x80);

    write_reg(0x44, 0x00, 0x18);
    write_reg(0x45, 0xC7, 0x00, 0x00, 0x00);

    write_reg(0x24);
    for (i = 0; i < TFT_HOR_RES * height; i++) {
        write_buf_dc((uint8_t[]){0xFF}, 1, 1);
    }

    tft_full_refresh();
}

static uint8_t refresh_count = 0;
void tft_video_flush(int xs, int ys, int xe, int ye, void *vmem, size_t len)
{
    uint8_t *vmem8 = (uint8_t *)vmem;
    tft_reset();
    write_reg(0x3C, 0x80);

    write_reg(0x44, 0x00, 0x18);
    write_reg(0x45, 0xC7, 0x00, 0x00, 0x00);
    write_reg(0x24);
    write_buf_dc(vmem8, len, 1);
    refresh_count++;

    /* should do a full refresh after 8 times part refresh */
    if (refresh_count % 8 == 0)
        tft_full_refresh();
    else
        tft_part_refresh();
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
    pio_spi_tx_init(TFT_SDA_PIN, TFT_SCL_PIN);
    bi_decl(bi_2pins_with_func(TFT_SCL_PIN, TFT_SDA_PIN, GPIO_FUNC_PIO0));
#else
    spi_init(spi_ifce, TFT_BUS_CLK_KHZ * 1000);
    gpio_set_function(TFT_SCL_PIN, GPIO_FUNC_SPI);
    gpio_set_function(TFT_SDA_PIN, GPIO_FUNC_SPI);
    bi_decl(bi_2pins_with_func(TFT_SCL_PIN, TFT_SDA_PIN, GPIO_FUNC_SPI));
    pr_debug("spi%d initialized at %d kHz\n", spi_get_index(spi_ifce), spi_get_baudrate(spi_ifce) / 1000 );
#endif

    gpio_init(TFT_CS_PIN);
    gpio_set_dir(TFT_CS_PIN, GPIO_OUT);
    gpio_put(TFT_CS_PIN, 1);
    bi_decl(bi_1pin_with_name(TFT_CS_PIN, "SPI CS"));

    gpio_init(TFT_RES_PIN);
    gpio_set_dir(TFT_RES_PIN, GPIO_OUT);
    bi_decl(bi_1pin_with_name(TFT_RES_PIN, "TFT RES"));

    gpio_init(TFT_DC_PIN);
    gpio_set_dir(TFT_DC_PIN, GPIO_OUT);
    bi_decl(bi_1pin_with_name(TFT_DC_PIN, "TFT DC"));

    gpio_init(TFT_BLK_PIN);
    gpio_set_dir(TFT_BLK_PIN, GPIO_IN);
    bi_decl(bi_1pin_with_name(TFT_BLK_PIN, "TFT BUSY"));

    tft_init_display();
    tft_clear();
}

bool lvgl_timer_callback(struct repeating_timer *t)
{
    lv_timer_handler();
    return true;
}

LV_IMG_DECLARE(logo);

int main(void)
{
    hardware_init();

    lv_init();
    lv_port_disp_init();

    pr_debug("Starting APP...\n");
    // lv_obj_t *btn = lv_btn_create(lv_scr_act());
    // lv_obj_set_style_radius(btn, 10, 0);
    // lv_obj_center(btn);

    // lv_obj_t *label = lv_label_create(btn);
    // lv_label_set_text(label, "embeddedboys.github.io/");

    lv_demo_stress();
    // lv_demo_benchmark();

    struct repeating_timer lvgl_timer;
    add_repeating_timer_ms(5, lvgl_timer_callback, NULL, &lvgl_timer);

    pr_debug("going to loop...\n");
    for (;;) {
        tight_loop_contents();
        sleep_ms(200);
    }
}