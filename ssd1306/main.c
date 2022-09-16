#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include "ssd1306.h"

int main()
{
	int rc;
	uint8_t wbuf[8] = {0}, rbuf[8] = {0};
	stdio_init_all();

	i2c_init(i2c_default, 400 * 1000);
	gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
	gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
	gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
	gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

	ssd1306_init();

	while (true)
	{
		printf("Hello, world!\n");
		for (int x = 0; x < 128; x++)
		{
			for (int y = 0; y < 64; y++)
			{
				ssd1306_set_pixel(x, y, 1);
			}
		}

		ssd1306_flush();
		sleep_ms(500);

		for (int x = 0; x < 128; x++)
		{
			for (int y = 0; y < 64; y++)
			{
				ssd1306_set_pixel(x, y, 0);
			}
		}

		ssd1306_flush();
		sleep_ms(500);
	}

	return 0;
}
