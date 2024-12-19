#include "pico/stdlib.h"

extern void hstx_init(void);
extern void hstx_put_word(uint32_t data);

inline void lcd_put_dc_cs_data(bool dc, bool csn, uint8_t data) {
	hstx_put_word(
		(uint32_t)data |
		(csn ? 0x0ff00000u : 0x00000000u) |
		// Note DC gets inverted inside of HSTX:
		(dc  ? 0x00000000u : 0x0003fc00u)
	);
}

inline void lcd_start_cmd(uint8_t cmd) {
	lcd_put_dc_cs_data(false, true, 0);
	lcd_put_dc_cs_data(false, false, cmd);
}

inline void lcd_put_data(uint32_t data) {
	lcd_put_dc_cs_data(true, false, data);
}

inline void lcd_start_pixels(void) {
    lcd_start_cmd(0x2C);
}

void lcd_flush(void *buf, size_t len)
{
	// lcd_start_pixels();
	for (size_t i = 0; i < len; ++i) {
		lcd_put_data(((uint8_t *)buf)[i]);
	}
}

void lcd_set_addr_win(int xs, int ys, int xe, int ye)
{
    /* set column adddress */
    lcd_start_cmd(0x2A);
	lcd_put_data(0x00);
	lcd_put_data(xs);
	lcd_put_data(xe >> 8);
	lcd_put_data(xe);

    /* set row address */
    lcd_start_cmd(0x2B);
	lcd_put_data(0x00);
	lcd_put_data(ys);
	lcd_put_data(ye >> 8);
	lcd_put_data(ye);

    /* write start */
    lcd_start_cmd(0x2C);
}

void lcd_part_flush(int xs, int ys, int xe, int ye, void *buf, size_t len) {
	lcd_set_addr_win(xs, ys, xe, ye);
	for (size_t i = 0; i < len; ++i) {
		lcd_put_data(((uint8_t *)buf)[i]);
	}
}

static void lcd_gpio_init(void)
{
    gpio_init(TFT_RES_PIN);
    gpio_init(TFT_BLK_PIN);
    gpio_set_dir(TFT_RES_PIN, GPIO_OUT);
    gpio_set_dir(TFT_BLK_PIN, GPIO_OUT);
    gpio_put(TFT_RES_PIN, 1);
    gpio_put(TFT_BLK_PIN, 1);
}

void lcd_reset(void)
{
    gpio_put(TFT_RES_PIN, 1);
    sleep_ms(10);
    gpio_put(TFT_RES_PIN, 0);
    sleep_ms(10);
    gpio_put(TFT_RES_PIN, 1);
    sleep_ms(10);
}

void lcd_init(void)
{
	lcd_gpio_init();
    lcd_reset();

	hstx_init();

    lcd_start_cmd(0xEF);
	lcd_start_cmd(0xEB);
	lcd_put_data(0x14);

    lcd_start_cmd(0xFE);
	lcd_start_cmd(0xEF);

	lcd_start_cmd(0xEB);
	lcd_put_data(0x14);

	lcd_start_cmd(0x84);
	lcd_put_data(0x40);

	lcd_start_cmd(0x85);
	lcd_put_data(0xFF);

	lcd_start_cmd(0x86);
	lcd_put_data(0xFF);

	lcd_start_cmd(0x87);
	lcd_put_data(0xFF);

	lcd_start_cmd(0x88);
	lcd_put_data(0x0A);

	lcd_start_cmd(0x89);
	lcd_put_data(0x21);

	lcd_start_cmd(0x8A);
	lcd_put_data(0x00);

	lcd_start_cmd(0x8B);
	lcd_put_data(0x80);

	lcd_start_cmd(0x8C);
	lcd_put_data(0x01);

	lcd_start_cmd(0x8D);
	lcd_put_data(0x01);

	lcd_start_cmd(0x8E);
	lcd_put_data(0xFF);

	lcd_start_cmd(0x8F);
	lcd_put_data(0xFF);


	lcd_start_cmd(0xB6);
	lcd_put_data(0x00);
	lcd_put_data(0x20);

	lcd_start_cmd(0x36);
	lcd_put_data(0x08);//Set as vertical screen

	lcd_start_cmd(0x3A);
	lcd_put_data(0x05);


	lcd_start_cmd(0x90);
	lcd_put_data(0x08);
	lcd_put_data(0x08);
	lcd_put_data(0x08);
	lcd_put_data(0x08);

	lcd_start_cmd(0xBD);
	lcd_put_data(0x06);

	lcd_start_cmd(0xBC);
	lcd_put_data(0x00);

	lcd_start_cmd(0xFF);
	lcd_put_data(0x60);
	lcd_put_data(0x01);
	lcd_put_data(0x04);

	lcd_start_cmd(0xC3);
	lcd_put_data(0x13);
	lcd_start_cmd(0xC4);
	lcd_put_data(0x13);

	lcd_start_cmd(0xC9);
	lcd_put_data(0x22);

	lcd_start_cmd(0xBE);
	lcd_put_data(0x11);

	lcd_start_cmd(0xE1);
	lcd_put_data(0x10);
	lcd_put_data(0x0E);

	lcd_start_cmd(0xDF);
	lcd_put_data(0x21);
	lcd_put_data(0x0c);
	lcd_put_data(0x02);

	lcd_start_cmd(0xF0);
	lcd_put_data(0x45);
	lcd_put_data(0x09);
	lcd_put_data(0x08);
	lcd_put_data(0x08);
	lcd_put_data(0x26);
 	lcd_put_data(0x2A);

 	lcd_start_cmd(0xF1);
 	lcd_put_data(0x43);
 	lcd_put_data(0x70);
 	lcd_put_data(0x72);
 	lcd_put_data(0x36);
 	lcd_put_data(0x37);
 	lcd_put_data(0x6F);


 	lcd_start_cmd(0xF2);
 	lcd_put_data(0x45);
 	lcd_put_data(0x09);
 	lcd_put_data(0x08);
 	lcd_put_data(0x08);
 	lcd_put_data(0x26);
 	lcd_put_data(0x2A);

 	lcd_start_cmd(0xF3);
 	lcd_put_data(0x43);
 	lcd_put_data(0x70);
 	lcd_put_data(0x72);
 	lcd_put_data(0x36);
 	lcd_put_data(0x37);
 	lcd_put_data(0x6F);

	lcd_start_cmd(0xED);
	lcd_put_data(0x1B);
	lcd_put_data(0x0B);

	lcd_start_cmd(0xAE);
	lcd_put_data(0x77);

	lcd_start_cmd(0xCD);
	lcd_put_data(0x63);


	lcd_start_cmd(0x70);
	lcd_put_data(0x07);
	lcd_put_data(0x07);
	lcd_put_data(0x04);
	lcd_put_data(0x0E);
	lcd_put_data(0x0F);
	lcd_put_data(0x09);
	lcd_put_data(0x07);
	lcd_put_data(0x08);
	lcd_put_data(0x03);

	lcd_start_cmd(0xE8);
	lcd_put_data(0x34);

	lcd_start_cmd(0x62);
	lcd_put_data(0x18);
	lcd_put_data(0x0D);
	lcd_put_data(0x71);
	lcd_put_data(0xED);
	lcd_put_data(0x70);
	lcd_put_data(0x70);
	lcd_put_data(0x18);
	lcd_put_data(0x0F);
	lcd_put_data(0x71);
	lcd_put_data(0xEF);
	lcd_put_data(0x70);
	lcd_put_data(0x70);

	lcd_start_cmd(0x63);
	lcd_put_data(0x18);
	lcd_put_data(0x11);
	lcd_put_data(0x71);
	lcd_put_data(0xF1);
	lcd_put_data(0x70);
	lcd_put_data(0x70);
	lcd_put_data(0x18);
	lcd_put_data(0x13);
	lcd_put_data(0x71);
	lcd_put_data(0xF3);
	lcd_put_data(0x70);
	lcd_put_data(0x70);

	lcd_start_cmd(0x64);
	lcd_put_data(0x28);
	lcd_put_data(0x29);
	lcd_put_data(0xF1);
	lcd_put_data(0x01);
	lcd_put_data(0xF1);
	lcd_put_data(0x00);
	lcd_put_data(0x07);

	lcd_start_cmd(0x66);
	lcd_put_data(0x3C);
	lcd_put_data(0x00);
	lcd_put_data(0xCD);
	lcd_put_data(0x67);
	lcd_put_data(0x45);
	lcd_put_data(0x45);
	lcd_put_data(0x10);
	lcd_put_data(0x00);
	lcd_put_data(0x00);
	lcd_put_data(0x00);

	lcd_start_cmd(0x67);
	lcd_put_data(0x00);
	lcd_put_data(0x3C);
	lcd_put_data(0x00);
	lcd_put_data(0x00);
	lcd_put_data(0x00);
	lcd_put_data(0x01);
	lcd_put_data(0x54);
	lcd_put_data(0x10);
	lcd_put_data(0x32);
	lcd_put_data(0x98);

	lcd_start_cmd(0x74);
	lcd_put_data(0x10);
	lcd_put_data(0x85);
	lcd_put_data(0x80);
	lcd_put_data(0x00);
	lcd_put_data(0x00);
	lcd_put_data(0x4E);
	lcd_put_data(0x00);

    lcd_start_cmd(0x98);
	lcd_put_data(0x3e);
	lcd_put_data(0x07);

	lcd_start_cmd(0x35);
	lcd_start_cmd(0x21);

	lcd_start_cmd(0x11);
	sleep_ms(120);
	lcd_start_cmd(0x29);
	sleep_ms(20);
}
