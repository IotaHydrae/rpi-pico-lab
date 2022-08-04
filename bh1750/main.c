#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "hardware/i2c.h"



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

	// uint8_t init_buf[1] = {0xe1};
	// i2c_write_blocking(i2c_default, 0x38, init_buf, 1, false);

	while(true) {
		wbuf[0] = 0x23;
		rc = i2c_write_blocking(i2c_default, 0x23, wbuf, 1, false);
		sleep_ms(30);

		i2c_read_blocking(i2c_default, 0x23, rbuf, 2, false);

		float lux = ((rbuf[0] << 8) | rbuf[1])/1.2;

		printf("lux: %f\n", lux);

		sleep_ms(500);
	}


	return 0;
}
