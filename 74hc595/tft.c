#include "pico/stdlib.h"

#define TFT_PIN_CS  18
#define TFT_PIN_WR  19
#define TFT_PIN_DC  20
#define TFT_PIN_RD  21
#define TFT_PIN_RST 22
#define TFT_PIN_BLK 28

extern void hc595_out16(uint16_t val);

static void write_cmd(uint16_t val)
{
    val <<= 8;
    gpio_put(TFT_PIN_DC, 0);
    // busy_wait_us(1);
    hc595_out16(val);
}

static void write_data(uint16_t val)
{
    val <<= 8;
    gpio_put(TFT_PIN_DC, 1);
    // busy_wait_us(1);
    hc595_out16(val);
}

static void tft_reset(void)
{
    gpio_put(TFT_PIN_RST, 1);
    busy_wait_ms(10);
    gpio_put(TFT_PIN_RST, 0);
    busy_wait_ms(10);
    gpio_put(TFT_PIN_RST, 1);
    busy_wait_ms(10);
}

static void tft_init_display(void)
{
    tft_reset();

    gpio_put(TFT_PIN_RD, 1);
    busy_wait_ms(120);

    write_cmd(0X11);
    busy_wait_ms(20);

    write_cmd(0XD0);//VCI1  VCL  VGH  VGL DDVDH VREG1OUT power amplitude setting
    write_data(0X07); 
    write_data(0X42); 
    write_data(0X1D); 
    write_cmd(0XD1);//VCOMH VCOM_AC amplitude setting
    write_data(0X00);
    write_data(0X1a);
    write_data(0X09); 
    write_cmd(0XD2);//Operational Amplifier Circuit Constant Current Adjust , charge pump frequency setting
    write_data(0X01);
    write_data(0X22);
    write_cmd(0XC0);//REV SM GS 
    write_data(0X10);
    write_data(0X3B);
    write_data(0X00);
    write_data(0X02);
    write_data(0X11);

    write_cmd(0XC5);// Frame rate setting = 72HZ  when setting 0x03
    write_data(0X03);

    write_cmd(0XC8);//Gamma setting
    write_data(0X00);
    write_data(0X25);
    write_data(0X21);
    write_data(0X05);
    write_data(0X00);
    write_data(0X0a);
    write_data(0X65);
    write_data(0X25);
    write_data(0X77);
    write_data(0X50);
    write_data(0X0f);
    write_data(0X00);	  
        
    write_cmd(0X0D);//Get_display_mode (0Dh)
    write_data(0X00);	
    write_data(0X00);	

    write_cmd(0XF8);
    write_data(0X01);	  

    write_cmd(0XFE);
    write_data(0X00);
    write_data(0X02);

    write_cmd(0X20);//Exit invert mode

    write_cmd(0X36);
    write_data((1 << 5) | (1 << 3));

    write_cmd(0X3A);
    write_data(0X55);
    write_cmd(0X2B);
    write_data(0X00);
    write_data(0X00);
    write_data(0X01);
    write_data(0X3F);

    write_cmd(0X2A);
    write_data(0X00);
    write_data(0X00);
    write_data(0X01);
    write_data(0XDF);
    busy_wait_ms(120);
    write_cmd(0X29);

    write_cmd(0x21);
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

    tft_set_addr_win(0, 0, width, height);
    for (x = 0; x < width; x++)
        for (y = 0; y < height; y++)
            write_data(color);
}

void tft_test_init(void)
{
    gpio_init(TFT_PIN_CS);
    gpio_init(TFT_PIN_WR);
    gpio_init(TFT_PIN_DC);
    gpio_init(TFT_PIN_RD);
    gpio_init(TFT_PIN_RST);
    gpio_init(TFT_PIN_BLK);

    gpio_set_dir(TFT_PIN_CS, GPIO_OUT);
    gpio_set_dir(TFT_PIN_WR, GPIO_OUT);
    gpio_set_dir(TFT_PIN_DC, GPIO_OUT);
    gpio_set_dir(TFT_PIN_RD, GPIO_OUT);
    gpio_set_dir(TFT_PIN_RST, GPIO_OUT);
    gpio_set_dir(TFT_PIN_BLK, GPIO_OUT);

    gpio_put(TFT_PIN_BLK, 1);

    tft_reset();
    gpio_put(TFT_PIN_RD, 1);
    busy_wait_ms(120);
}

void tft_init(void)
{
    gpio_init(TFT_PIN_CS);
    gpio_init(TFT_PIN_WR);
    gpio_init(TFT_PIN_DC);
    gpio_init(TFT_PIN_RD);
    gpio_init(TFT_PIN_RST);
    gpio_init(TFT_PIN_BLK);

    gpio_set_dir(TFT_PIN_CS, GPIO_OUT);
    gpio_set_dir(TFT_PIN_WR, GPIO_OUT);
    gpio_set_dir(TFT_PIN_DC, GPIO_OUT);
    gpio_set_dir(TFT_PIN_RD, GPIO_OUT);
    gpio_set_dir(TFT_PIN_RST, GPIO_OUT);
    gpio_set_dir(TFT_PIN_BLK, GPIO_OUT);

    gpio_put(TFT_PIN_BLK, 1);

    tft_init_display();
    tft_clear(0x1234);
}