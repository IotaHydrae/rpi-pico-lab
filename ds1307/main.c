#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "hardware/i2c.h"

void hal_init()
{
    i2c_init(i2c_default, 400 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
}
int main()
{
    int rc;
    int hour, minute, second;
    uint8_t r_buf[8], w_buf[8];

    hal_init();
    stdio_init_all();

    /* recover time and enable clock */
    w_buf[0] = 0x00;
    rc = i2c_write_blocking(i2c_default, 0x68, w_buf, 1, false);
    rc = i2c_read_blocking(i2c_default, 0x68, r_buf, 1, false);

    w_buf[0] = r_buf[0] & 0x7f;
    rc = i2c_write_blocking(i2c_default, 0x68, w_buf, 1, false);

    while (true) {
        w_buf[0] = 0x00;
        rc = i2c_write_blocking(i2c_default, 0x68, w_buf, 1, false);
        rc = i2c_read_blocking(i2c_default, 0x68, r_buf, 1, false);
        second = ((r_buf[0] & 0x7f) >> 4) * 10 + (r_buf[0] & 0x0f);
        printf("%d\n", ((r_buf[0] & 0x7f) >> 4) * 10 + (r_buf[0] & 0x0f));

        sleep_ms(1000);
    }


    return 0;
}
