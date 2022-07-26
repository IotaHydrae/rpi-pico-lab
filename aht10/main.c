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

	uint8_t init_buf[1] = {0xe1};
	i2c_write_blocking(i2c_default, 0x38, init_buf, 1, false);

	while(true) {
		// printf("Hello, world!\n");
		wbuf[0] = 0xac;
		wbuf[1] = 0x33;
		wbuf[2] = 0x00;
		rc = i2c_write_blocking(i2c_default, 0x38, wbuf, 3, false);
		if(rc == PICO_ERROR_GENERIC){
			printf("Error device not responding\n");
		}
		sleep_ms(75);
		i2c_read_blocking(i2c_default, 0x38, rbuf, 6, false);

		uint8_t status  = rbuf[0];
		uint32_t SRH = rbuf[1] << 12 | rbuf[2] << 4 | rbuf[3] >> 4;
		uint32_t ST = (rbuf[3] & 0x0f) << 16 | rbuf[4] << 8 | rbuf[5];
		// uint32_t temperture = rbuf[4] << 12 | rbuf[5] << 4 | rbuf[3] & 0x0f;

		printf("\n  Reading from AHT10:\n");
		printf("\tStatus: %02x\n", status);
		printf("\tSRH: %d\n", SRH);
		printf("\tST: %d\n", ST);

		double real_humidity = ((double)SRH / 1048576) * 100;
		double real_temperture = ((double)ST / 1048576) * 200 - 50;

		printf("\tReal Humidity: %.2f%%\n", real_humidity);
		printf("\tReal Temperature: %.2f℃\n", real_temperture);

		/* raw data */
		printf("\tRegisters: ");
		for(int i=0;i<6;i++){
			printf("%02x", rbuf[i]);
		}
		printf("\n");
		sleep_ms(2000);
	}


	return 0;
}
