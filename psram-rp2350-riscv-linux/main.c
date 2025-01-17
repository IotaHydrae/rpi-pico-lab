#include <stdio.h>
#include <ctype.h>
#include <machine/endian.h>
#include <stdnoreturn.h>
#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include "boot/picobin.h"
#include "hardware/pll.h"
#include "hardware/vreg.h"
#include "hardware/clocks.h"
#include "hardware/watchdog.h"

#include "sfe_psram.h"

#define min(a, b) ((a) < (b) ? (a) : (b))

#if _BYTE_ORDER == _LITTLE_ENDIAN

#define htobe16(x) __bswap16(x)
#define htole16(x) (x)
#define be16toh(x) __bswap16(x)
#define le16toh(x) (x)

#define htobe32(x) __bswap32(x)
#define htole32(x) (x)
#define be32toh(x) __bswap32(x)
#define le32toh(x) (x)

#define htobe64(x) __bswap64(x)
#define htole64(x) (x)
#define be64toh(x) __bswap64(x)
#define le64toh(x) (x)

#else

#define htobe16(x) (x)
#define htole16(x) __bswap16(x)
#define be16toh(x) (x)
#define le16toh(x) __bswap16(x)

#define htobe32(x) (x)
#define htole32(x) __bswap32(x)
#define be32toh(x) (x)
#define le32toh(x) __bswap32(x)

#define htobe64(x) (x)
#define htole64(x) __bswap64(x)
#define be64toh(x) (x)
#define le64toh(x) __bswap64(x)

#endif

#define PSRAM_LOCATION _u(0x11000000)
#define PART_LOC_FIRST(x) ( ((x) & PICOBIN_PARTITION_LOCATION_FIRST_SECTOR_BITS) >> \
			   PICOBIN_PARTITION_LOCATION_FIRST_SECTOR_LSB )
#define PART_LOC_LAST(x) ( ((x) & PICOBIN_PARTITION_LOCATION_LAST_SECTOR_BITS) >> \
			   PICOBIN_PARTITION_LOCATION_LAST_SECTOR_LSB )

#define SFE_RP2350_XIP_CSI_PIN 0

static size_t get_devicetree_size(const void *data) {
	const uint32_t *data_ptr = (const uint32_t *)data;

	if (be32toh(data_ptr[0]) == 0xd00dfeed) {
		return be32toh(data_ptr[1]);
	}

	return 0;
}

static int rom_test(void **data_addr, size_t* data_size) {
	static __attribute__((aligned(4))) uint32_t workarea[1024];
	uint32_t data_end_addr, *data_start_addr = ((uint32_t *)data_addr);
	int rc;

	rc = rom_load_partition_table((uint8_t *)workarea, sizeof(workarea), false);
	if (rc) {
		printf("Partition Table Load failed %d - resetting\n", rc);
		return -1;
	}

	rc = rom_get_partition_table_info((uint32_t*)workarea, 0x8, PT_INFO_PARTITION_LOCATION_AND_FLAGS | PT_INFO_SINGLE_PARTITION);
	if (rc != 3) {
		printf("No boot partition - assuming bin at start of flash\n");
		return -1;
	}

	*data_start_addr = PART_LOC_FIRST(workarea[1]) * 0x1000;
	data_end_addr = (PART_LOC_LAST(workarea[1]) + 1) * 0x1000;
	*data_size = data_end_addr - *data_start_addr;
	*data_start_addr += XIP_BASE;
	printf("Partition Start 0x%04x, End 0x%04x, Size: 0x%04x\n",
	       *data_start_addr, data_end_addr, *data_size);

	return 0;
}


static void hexdump(const void *data, size_t size) {
        const uint8_t *data_ptr = (const uint8_t *)data;
        size_t i, b;

        for (i = 0; i < size; i++) {
                if (i % 16 == 0) {
                        printf("%08x  ", (uint32_t)data_ptr + i);
                }
                if (i % 8 == 0) {
                        printf(" ");
                }
                printf("%02x ", data_ptr[i]);
                if (i % 16 == 15) {
                        printf(" |");
                        for (b = 0; b < 16; b++){
                                if (isprint(data_ptr[i + b - 15])) {
                                        printf("%c", data_ptr[i + b - 15]);
                                } else {
                                        printf(".");
                                }
                        }
                        printf("|\n");
                }
        }
        printf("%08x\n\n", 16 + size - (size%16));
}

int main()
{
	int ret;
	uint32_t jump_ret, data;
	size_t data_size, kernel_offset;
	void *data_addr, *ram_addr = (void *)PSRAM_LOCATION;

	vreg_set_voltage(VREG_VOLTAGE_DEFAULT);
	set_sys_clock_khz(240 * 1000, true);
	clock_configure(clk_peri,
						0,
						CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLK_SYS,
						240 * MHZ,
						240 * MHZ);

    stdio_uart_init_full(uart0, 115200, 16, 17);

    size_t psram_size = sfe_setup_psram(SFE_RP2350_XIP_CSI_PIN);
    printf("PSRAM size: %d\n", psram_size);

    ret = rom_test(&data_addr, &data_size);
    if (ret) {
        printf("Failed to load boot partition\n");
        return -1;
    }

    if (data_size > psram_size) {
        printf("Data size 0x%04x is larger than PSRAM size 0x%04x\n",
			data_size, psram_size);
        return -1;
    }

	if(data_size < 8) {
		printf("Data size is 0\n");
		return -1;
	}

	printf("\nRom dump:\n");
	hexdump(data_addr, min(data_size, 0x20));

	kernel_offset = get_devicetree_size(data_addr);
	if (kernel_offset) {
		printf("Kernel offset: 0x%08x\n", kernel_offset);
	} else {
		printf("No kernel + device tree found\n");
	}

	memcpy(ram_addr, data_addr + kernel_offset, data_size - kernel_offset);
	printf("\nRam dump:\n");
	hexdump(ram_addr, 0x20);

	if (kernel_offset) {
		typedef void (*image_entry_arg_t)(unsigned long hart, void *dtb);
		image_entry_arg_t image_entry = (image_entry_arg_t)ram_addr;

		printf("\nJumping to kernel at 0x%08x and DT at 0x%08x\n", ram_addr, data_addr);
		printf("If you are using USB serial, please connect over the hardware serial port.\n");
		image_entry(0, data_addr);
	}

    // unsigned char *psram_addr = (unsigned char *)PSRAM_LOCATION;
    // hexdump(psram_addr, 32);

    // for (int i = 0; i < 16; i++) {
    //     psram_addr[i] = i;
    // }

    // hexdump(psram_addr, 32);

    // printf("printing device tree head...\n");
    // void *device_tree_head = (void *)(0x10000000 + 64 * 1024);
    // hexdump(device_tree_head, 32);

    return 0;
}
