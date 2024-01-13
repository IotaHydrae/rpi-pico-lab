#include <stdio.h>
#include "pico/stdlib.h"


void __attribute__((used)) faultHandlerWithExcFrame(uint32_t* push, uint32_t *regs, uint32_t ret_lr)
{
	uint32_t *sp = push;
	unsigned i;
	
	printf("============ HARD FAULT ============\n");
	printf("R0  = 0x%08X    R8  = 0x%08X\n", (unsigned)push[0], (unsigned)regs[0]);
	printf("R1  = 0x%08X    R9  = 0x%08X\n", (unsigned)push[1], (unsigned)regs[1]);
	printf("R2  = 0x%08X    R10 = 0x%08X\n", (unsigned)push[2], (unsigned)regs[2]);
	printf("R3  = 0x%08X    R11 = 0x%08X\n", (unsigned)push[3], (unsigned)regs[3]);
	printf("R4  = 0x%08X    R12 = 0x%08X\n", (unsigned)regs[4], (unsigned)push[4]);
	printf("R5  = 0x%08X    SP  = 0x%08X\n", (unsigned)regs[5], (unsigned)sp);
	printf("R6  = 0x%08X    LR  = 0x%08X\n", (unsigned)regs[6], (unsigned)push[5]);
	printf("R7  = 0x%08X    PC  = 0x%08X\n", (unsigned)regs[7], (unsigned)push[6]);
	printf("RA  = 0x%08X    SR  = 0x%08X\n", (unsigned)ret_lr,  (unsigned)push[7]);
	// printf("SHCSR = 0x%08X\n", SCB->SHCSR);
    
	printf("WORDS @ SP: \n");
	
	for (i = 0; i < 8; i++)
		printf("[sp, #0x%03X = 0x%08X] = 0x%08x\n", i * 4, (unsigned)&sp[i], (unsigned)sp[i]);
	
	printf("\n\n");
	while(1);
}

void __attribute__((__section__(".data"))) isr_hardfault(void)
{
    asm volatile(
        ".syntax unified    \n\t"
        ".global HardFault_Handler \n\t"
        ".func HardFault_Handler \n\t"
        ".type HardFault_Handler function \n\t"
        "HardFault_Handler: \n\t"
        // "   mov r0, lr \n\t"
        // "   lsrs r0, #3 \n\t"
        // "   bcs 1f  \n\t"
		"	mov   r0, sp									\n\t"
		// "	b     2f										\n\t"
		// "1:													\n\t"
		// "	mrs   r0, psp									\n\t"
		// "2:													\n\t"
		
		//to emulate-for-write fast, we must assume that PC points somewhere valid
		//otherwise we'd have to take the penalty of switching to out safe mode, and then wrangling the MPU
		//whereas now we can use "hard fault uses default map" mode
		
		// "	ldr		r2, [r0, #4 * 6]						\n\t"
		// "	ldrh	r1, [r2]								\n\t"
		// "	lsrs	r3, r1, #8								\n\t"
		// "	add		pc, r3									\n\t"
		"	nop												\n\t"
		".rept 35											\n\t"
		"	b		report_some_fault						\n\t"
        ".endr                                              \n\t"
        "report_some_fault:									\n\t"
		"	mov		r12, r0									\n\t"
		"	mov		r0, r8									\n\t"
		"	mov		r1, r9									\n\t"
		"	mov		r2, r10									\n\t"
		"	mov		r3, r11									\n\t"
		"	push	{r0-r7}									\n\t"
		"	mov		r0, r12									\n\t"
		"	mov		r1, sp									\n\t"
		"	mov		r2, lr									\n\t"
		"	ldr		r3, =faultHandlerWithExcFrame			\n\t"
		"	bx		r3										\n\t"
		".ltorg												\n\t"
        :
        :
        : "cc", "memory", "r0", "r1", "r2", "r3", "r12"
    );
    return;
}

int __attribute__((used)) foo(unsigned addr, unsigned char val)
{
    volatile int *p = (volatile int *)0x2f000020;

    printf("writing 0x%02x to addr : 0x%08x\n", val, addr);
    *p = val;
    /* This will trigger Hard Fault */
    return *p;
}

int main()
{
    stdio_init_all();

    printf("\n\n\n\n\tPSRAM hardfault test.\n\n");
    // const uint led_pin = PICO_DEFAULT_LED_PIN;

    // #define MPU_TYPE (volatile unsigned int *)(PPB_BASE + 0xed90)
    // #define MPU_CTRL (volatile unsigned int *)(PPB_BASE + 0xed94)
    // #define MPU_RNR  (volatile unsigned int *)(PPB_BASE + 0xed98)
    // #define MPU_RBAR (volatile unsigned int *)(PPB_BASE + 0xed9c)
    // #define MPU_RASR (volatile unsigned int *)(PPB_BASE + 0xeda0)

    // volatile unsigned int *p;
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
    foo(0x2f000020, 0x77);
    // printf("0x%x\n", *p);
    // int dump_len = 8;
    // while (dump_len--) {
    //     printf("%02x ", *p++);
    // }
    // printf("\n");

    // gpio_init(led_pin);
    // gpio_set_dir(led_pin, GPIO_OUT);

    // printf("going to loop...\n");
    // while (true) {
    //     gpio_put(led_pin, 1);
    //     sleep_ms(200);
    //     gpio_put(led_pin, 0);
    //     sleep_ms(200);
    // }
    for(;;);
    return 0;
}
