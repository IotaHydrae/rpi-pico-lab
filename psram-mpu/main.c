#include <stdio.h>
#include "pico/stdlib.h"

void __attribute__((used)) fault_handler_with_exec_frame(uint32_t *push, uint32_t *regs, uint32_t ret_lr)
{

}

void isr_hardfault(void)
{
    asm volatile(
        ".syntax unified    \n\t"
        ".global HardFault_Handler \n\t"
        ".func HardFault_Handler \n\t"
        ".type HardFault_Handler function \n\t"
        "HardFault_Handler: \n\t"
        "   mov r0, lr \n\t"
        "   lsrs r0, #3 \n\t"
        "   bcs 1f  \n\t"
        :
        :
        : "cc", "memory", "r0", "r1", "r2", "r3", "r12"
    );
    return;
}

int __attribute__((used)) foo()
{
    volatile int *p = (volatile int *)0x2f000020;
    return *p;
}

int main()
{
    stdio_init_all();
    const uint led_pin = PICO_DEFAULT_LED_PIN;

    #define MPU_TYPE (volatile unsigned int *)(PPB_BASE + 0xed90)
    #define MPU_CTRL (volatile unsigned int *)(PPB_BASE + 0xed94)
    #define MPU_RNR  (volatile unsigned int *)(PPB_BASE + 0xed98)
    #define MPU_RBAR (volatile unsigned int *)(PPB_BASE + 0xed9c)
    #define MPU_RASR (volatile unsigned int *)(PPB_BASE + 0xeda0)

    volatile unsigned int *p;
    // p = MPU_TYPE;
    // printf("MPU_TYPE: 0x%08x\n", *p);

    /* Setup and enable MPU */
    /* 1. Setup the MPU region */
    // p = MPU_RNR;
    // *p = 0;

    /* 2. set the base address of the region */
    // p = MPU_RBAR;
    // *p = 0x2F000000;

    /* 3. set the attribute of the region */
    // p = MPU_RASR;
    // *p = (0x7 << 24) | (0x16 << 1);
    
    /* 4. enable the MPU */
    // p = MPU_CTRL;
    // *p = 0;
    // printf("MPU_CTRL: 0x%02x\n", *p);

    /* we write into a flash address to test if MPU RO setting is okay. */
    // printf("attempting write flash...\n");
    // p = (unsigned int *)(0x10000000 + 0x500);
    // *p = 0x12345678;

    /* read data from flash via SSI */
    foo();
    // printf("0x%x\n", *p);
    // int dump_len = 8;
    // while (dump_len--) {
    //     printf("%02x ", *p++);
    // }
    // printf("\n");

    gpio_init(led_pin);
    gpio_set_dir(led_pin, GPIO_OUT);

    printf("going to loop...\n");
    while (true) {
        gpio_put(led_pin, 1);
        sleep_ms(200);
        gpio_put(led_pin, 0);
        sleep_ms(200);
    }

    return 0;
}
