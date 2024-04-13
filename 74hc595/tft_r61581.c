#include <stdio.h>
#include <stdarg.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#define R61581_HSPL         0       /*HSYNC signal polarity*/
#define R61581_HSL          10      /*HSYNC length (Not Implemented)*/
#define R61581_HFP          10      /*Horitontal Front poarch (Not Implemented)*/
#define R61581_HBP          10      /*Horitontal Back poarch (Not Implemented */
#define R61581_VSPL         0       /*VSYNC signal polarity*/
#define R61581_VSL          10      /*VSYNC length (Not Implemented)*/
#define R61581_VFP          8       /*Vertical Front poarch*/
#define R61581_VBP          8       /*Vertical Back poarch */
#define R61581_DPL          0       /*DCLK signal polarity*/
#define R61581_EPL          1       /*ENABLE signal polarity*/
#define R61581_ORI          0       /*0, 180*/
#define R61581_LV_COLOR_DEPTH 16    /*Fix 16 bit*/

// #define TFT_PIN_CS  29
#define TFT_PIN_WR  19
#define TFT_PIN_DC  20
// #define TFT_PIN_RD  29
#define TFT_PIN_RST 18
#define TFT_PIN_BLK 28

// struct tft_priv {
//     uint8_t                      *buf;

//     struct {
//         int reset;
//         int cs;   /* chip select */
//         int rs;   /* register/data select */
//         int wr;   /* write signal */
//         int rd;   /* read signal */
//         int bl;   /* backlight */
//         int db[16];
//     } gpio;
    
//     /* device specific */
//     struct tft_display    *display;
//     struct tft_ops        *tftops;
// } __attribute__((__aligned__(4)));

// void fbtft_write_gpio16_wr_rs(struct tft_priv *priv, void *buf, size_t len, bool rs)
// {
//     gpio_put(TFT)
//     fbtft_write_gpio16_wr(priv, buf, len);
// }

// #define define_tft_write_reg(func, reg_type) \
// void func(struct tft_priv *priv, int len, ...)  \
// {   \
//     reg_type *buf = (reg_type *)priv->buf; \
//     va_list args;   \
//     int i;  \
//     \
//     va_start(args, len);    \
//     *buf = (reg_type)va_arg(args, unsigned int); \
//     write_buf_rs(priv, buf, sizeof(reg_type), 0); \
//     len--;  \
//     \
//     /* if there no privams */  \
//     if (len == 0)  \
//         goto exit_no_param; \
//     \
//     for (i = 0; i < len; i++) { \
//         *buf++ = (reg_type)va_arg(args, unsigned int); \
//     }   \
//     \
//     len *= sizeof(reg_type);    \
//     write_buf_rs(priv, priv->buf, len, 1);  \
// exit_no_param:  \
//     va_end(args);   \
// }

// define_tft_write_reg(tft_write_reg16, uint16_t)

extern void hc595_out8(uint8_t val);

static void gpiod_set_value(int pin, bool val)
{
    asm volatile("nop\n nop\n nop\n nop\n nop\n nop\n");
    gpio_put(pin, val);
    asm volatile("nop\n nop\n nop\n nop\n nop\n nop\n");
}

#define gpio_put gpiod_set_value

static void write_cmd(uint16_t val)
{
    gpio_put(TFT_PIN_DC, 0);

    gpio_put(TFT_PIN_WR, 0);
    hc595_out8(val);
    gpio_put(TFT_PIN_WR, 1);
    gpio_put(TFT_PIN_WR, 0);
}

static void write_data(uint16_t val)
{
    gpio_put(TFT_PIN_DC, 1);

    gpio_put(TFT_PIN_WR, 0);
    hc595_out8(val);
    gpio_put(TFT_PIN_WR, 1);
    gpio_put(TFT_PIN_WR, 0);
}

static void tft_reset(void)
{
    gpio_put(TFT_PIN_RST, 1);
    sleep_ms(10);
    gpio_put(TFT_PIN_RST, 0);
    sleep_ms(10);
    gpio_put(TFT_PIN_RST, 1);
    sleep_ms(10);
}

static void tft_init_display(void)
{
    printf("init display\n");
    tft_reset();
    // gpio_put(TFT_PIN_RD, 1);
    // busy_wait_ms(120);

    write_cmd(0xB0);
    write_data(0x00);
    
    write_cmd(0xB3);
    write_data(0x02);
    write_data(0x00);
    write_data(0x00);
    write_data(0x00);
    
    write_cmd(0xC0);
    write_data(0x13);
    write_data(0x3B);
    write_data(0x00);
    write_data(0x02);
    write_data(0x00);
    write_data(0x01);
    write_data(0x00);
    write_data(0x43);
    
    write_cmd(0xC1);
    write_data(0x08);
    write_data(0x16);
    write_data(0x08);
    write_data(0x08);
    
    write_cmd(0xC4);
    write_data(0x11);
    write_data(0x07);
    write_data(0x03);
    write_data(0x03);
    
    write_cmd(0xC6);
    write_data(0x00);
    
    write_cmd(0xC8);
    write_data(0x03);
    write_data(0x03);
    write_data(0x13);
    write_data(0x5C);
    write_data(0x03);
    write_data(0x07);
    write_data(0x14);
    write_data(0x08);
    write_data(0x00);
    write_data(0x21);
    write_data(0x08);
    write_data(0x14);
    write_data(0x07);
    write_data(0x53);
    write_data(0x0C);
    write_data(0x13);
    write_data(0x03);
    write_data(0x03);
    write_data(0x21);
    write_data(0x00);
    
    write_cmd(0x0C);
    write_data(0x55);
    
    write_cmd(0x36);
    write_data((1 << 6) | (1 << 5));
    
    write_cmd(0x38);
    
    write_cmd(0x3A);
    write_data(0x55);
    
    write_cmd(0xD0);
    write_cmd(0x07);
    write_cmd(0x07);
    write_cmd(0x1D);
    write_cmd(0x03);
    
    write_cmd(0xD1);
    write_data(0x03);
    write_data(0x30);
    write_data(0x10);

    write_cmd(0xD2);
    write_data(0x03);
    write_data(0x14);
    write_data(0x04);
    
    write_cmd(0x11);
    busy_wait_ms(10);
    write_cmd(0x29);
}

static void tft_set_addr_win(int xs, int ys, int xe, int ye)
{
    write_cmd(0x2A);
    write_data(xs >> 8);
    write_data(xs & 0xFF);
    write_data(xe >> 8);
    write_data(xe & 0xFF);

    write_cmd(0x2B);
    write_data(ys >> 8);
    write_data(ys & 0xFF);
    write_data(ye >> 8);
    write_data(ye & 0xFF);

    write_cmd(0x2C);
}

static void tft_clear(uint16_t color)
{
    uint16_t width = 480;
    uint16_t height = 320;
    int x, y;

    tft_set_addr_win(0, 0, width-1, height-1);
    for (x = 0; x < width; x++)
        for (y = 0; y < height; y++) {
            write_data(color >> 8);
            write_data(color & 0xFF);
        }
}

void tft_init(void)
{
    // gpio_init(TFT_PIN_CS);
    gpio_init(TFT_PIN_WR);
    gpio_init(TFT_PIN_DC);
    // gpio_init(TFT_PIN_RD);
    gpio_init(TFT_PIN_RST);
    gpio_init(TFT_PIN_BLK);

    // gpio_set_dir(TFT_PIN_CS, GPIO_OUT);
    gpio_set_dir(TFT_PIN_WR, GPIO_OUT);
    gpio_set_dir(TFT_PIN_DC, GPIO_OUT);
    // gpio_set_dir(TFT_PIN_RD, GPIO_OUT);
    gpio_set_dir(TFT_PIN_RST, GPIO_OUT);
    gpio_set_dir(TFT_PIN_BLK, GPIO_OUT);

    // origin: 0x11
    // 1 0 0 0  1 0 0 0
    
    // this: 0x11
    // 
    
    tft_init_display();
    gpio_put(TFT_PIN_BLK, 1);
    for (int color = 0; color < 0xffff; color += 128) {
        tft_clear(0x1111);
    }
}