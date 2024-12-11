#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"
#include "hardware/regs/pads_bank0.h"

#define PSRAM_SPI_IFCE  spi0
#define PSRAM_CLK_KHZ   62500
#define PSRAM_SCK_PIN   2
#define PSRAM_TX_PIN    3
#define PSRAM_RX_PIN    4
#define PSRAM_CS_PIN    5

#define PSRAM_ID_LEN    8

struct psram_priv {

};

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;


static size_t spi_write_then_read(struct spi_inst *spi,
                                    const void *txbuf, u32 n_tx,
                                    void *rxbuf, u32 n_rx,
                                    unsigned delay)
{
    int      i;
    size_t   len = 0;
    u8       *txbuf8 = (u8 *)txbuf;
    u8       *rxbuf8 = (u8 *)rxbuf;

    gpio_put(PSRAM_CS_PIN, 0);

    spi_write_blocking(spi, txbuf, n_tx);

    if (n_rx <= 0)
        goto finish;

    spi_read_blocking(spi, 0xFF, rxbuf, n_rx);

finish:
    gpio_put(PSRAM_CS_PIN, 1);
    return (len == n_rx) ? -1 : len;
}

static inline size_t spi_w8r8(struct spi_inst *spi, u8 cmd)
{
    size_t status;
    u8      result;

    status = spi_write_then_read(spi, &cmd, 1, &result, 1, 0);

    return (status < 0) ? status : result;
}

static inline size_t spi_w8r16(struct spi_inst *spi, u8 cmd)
{
    size_t   status;
    u16      result;

    status = spi_write_then_read(spi, &cmd, 1, &result, 2, 0);

    return (status < 0) ? status : result;
}

static inline u32 psram_reset(struct spi_inst *spi)
{
    u8 cmds[] = {0x66, 0x99};

    return spi_write_then_read(spi, cmds, sizeof(cmds), NULL, 0, 0);
}

static inline u32 psram_read_id(struct spi_inst *spi, uint8_t *rxbuf, size_t len)
{
    u8 cmds[] = {0x9f, 0xff, 0xff, 0xff}; // 9Fh then three byte dummy clock

    if (len != PSRAM_ID_LEN)
        return -1;

    return spi_write_then_read(spi, cmds, sizeof(cmds), rxbuf, len, 0);
}

static int psram_init(struct spi_inst *spi)
{
    u8 rxbuf[PSRAM_ID_LEN];

    for (;;)  {
        psram_read_id(spi, rxbuf, sizeof(rxbuf)/sizeof(rxbuf[0]));

        for (int i = 0; i < PSRAM_ID_LEN; i++)
            printf("0x%02x ", rxbuf[i]);
        printf("\n");

        sleep_ms(100);
    }

    return 0;
};

int main(void) {
    stdio_uart_init_full(uart0, 115200, 0, 1);

    printf("\n\n\n\n\tPSRAM test.\n\n");

    spi_init(PSRAM_SPI_IFCE, PSRAM_CLK_KHZ * 1000);
    spi_set_format(PSRAM_SPI_IFCE, 8, SPI_CPOL_0, SPI_CPHA_1, SPI_MSB_FIRST);
    gpio_set_function(PSRAM_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PSRAM_TX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PSRAM_RX_PIN, GPIO_FUNC_SPI);

    gpio_init(PSRAM_CS_PIN);
    gpio_pull_up(PSRAM_CS_PIN);
    gpio_set_dir(PSRAM_CS_PIN, GPIO_OUT);
    gpio_put(PSRAM_CS_PIN, 1);

    // bi_decl(bi_4pins_with_func(PSRAM_SCK_PIN, PSRAM_TX_PIN, PSRAM_RX_PIN, PSRAM_CS_PIN, GPIO_FUNC_SPI));
    printf("spi%d initialized at %d kHz\n", spi_get_index(PSRAM_SPI_IFCE), spi_get_baudrate(PSRAM_SPI_IFCE) / 1000 );

    psram_init(PSRAM_SPI_IFCE);

    return 0;
}