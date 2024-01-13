#include <string.h>





void __attribute__((used)) faultHandlerWithExcFrame(uint32_t* push, uint32_t *regs, uint32_t ret_lr)
{
	uint32_t *sp = push + 8;
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
	printf("SHCSR = 0x%08X\n", SCB->SHCSR);
    
	printf("WORDS @ SP: \n");
	
	for (i = 0; i < 32; i++)
		printf("[sp, #0x%03x = 0x%08x] = 0x%08x\n", i * 4, (unsigned)&sp[i], (unsigned)sp[i]);
	
	printf("\n\n");
	while(1);
}

void __attribute__((noreturn, naked, noinline, section(".ramcode"))) HardFault_Handler(void)		//we do not support stack in this memory
{
	asm volatile(
	
		".syntax unified									\n\t"
		
		//grab the appropriate SP
		".globl HardFault_Handler							\n\t"
		".func HardFault_Handler							\n\t"
		".type HardFault_Handler function					\n\t"
		"HardFault_Handler:									\n\t"
		"	mov   r0, lr									\n\t"
		"	lsrs  r0, #3									\n\t"
		"	bcs   1f										\n\t"
		"	mov   r0, sp									\n\t"
		"	b     2f										\n\t"
		"1:													\n\t"
		"	mrs   r0, psp									\n\t"
		"2:													\n\t"
		
		//to emulate-for-write fast, we must assume that PC points somewhere valid
		//otherwise we'd have to take the penalty of switching to out safe mode, and then wrangling the MPU
		//whereas now we can use "hard fault uses default map" mode
		
		"	ldr		r2, [r0, #4 * 6]						\n\t"
		"	ldrh	r1, [r2]								\n\t"
		"	lsrs	r3, r1, #8								\n\t"
		"	add		pc, r3									\n\t"
		"	nop												\n\t"
		".rept 35											\n\t"
		"	b		report_some_fault						\n\t"
		".endr												\n\t"
		"	b		report_some_fault						\n\t"
		".rept 4											\n\t"
		"	b		report_some_fault						\n\t"
		".endr												\n\t"
		"	b		str_2									\n\t"
		"	b		strh_2									\n\t"
		"	b		strb_2									\n\t"
		".rept 5											\n\t"
		"	b		report_some_fault						\n\t"
		".endr												\n\t"
		".rept 4											\n\t"
		"	b		str_1									\n\t"
		".endr												\n\t"
		".rept 4											\n\t"
		"	b		report_some_fault						\n\t"
		".endr												\n\t"
		".rept 4											\n\t"
		"	b		strb_1									\n\t"
		".endr												\n\t"
		".rept 4											\n\t"
		"	b		report_some_fault						\n\t"
		".endr												\n\t"
		".rept 4											\n\t"
		"	b		strh_1									\n\t"
		".endr												\n\t"
		".rept 28											\n\t"
		"	b		report_some_fault						\n\t"
		".endr												\n\t"
		"	b		stmia_r0r1r2r3							\n\t"
		"	b		stmia_r0r1r2r3							\n\t"
		"	b		stmia_r4r5r6r7							\n\t"
		"	b		stmia_r4r5r6r7							\n\t"
		".rept 11											\n\t"
		"	b		report_some_fault						\n\t"
		".endr												\n\t"
		"	b		report_some_fault						\n\t"
		".rept 4											\n\t"
		"	b		report_some_fault						\n\t"
		".endr												\n\t"
		".rept 4											\n\t"
		"	b		report_some_fault						\n\t"
		".endr												\n\t"
		".rept 4											\n\t"
		"	b		report_some_fault						\n\t"
		".endr												\n\t"
		".rept 4											\n\t"
		"	b		report_some_fault						\n\t"
		".endr												\n\t"
		
		"report_some_fault_pop_r4lr:						\n\t"
		"	pop		{r4}									\n\t"
		"	pop		{r3}									\n\t"
		"	mov		lr, r3									\n\t"
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
		
		"str_2:												\n\t"	//r0 is state, r1 is instr
		"	push	{r4, lr}								\n\t"
		"	bl		get_addr_2reg							\n\t"	//r3 is now addr
		"	ldr		r2, mRomRamStart						\n\t"
		"	ldr		r4, mRomRamLen							\n\t"
		"	subs	r2, r3, r2								\n\t"
		"	cmp		r2, r4									\n\t"
		"	bcs		report_some_fault_pop_r4lr				\n\t"
		"	ldr		r2, [r0, #4 * 6]						\n\t"
		"	adds	r2, #2									\n\t"
		"	str		r2, [r0, #4 * 6]						\n\t"
		"	bl		get_rd_0								\n\t"	//r1 is now val
		"	b		write_4b								\n\t"
		
		"strh_2:											\n\t"
		"	push	{r4, lr}								\n\t"
		"	bl		get_addr_2reg							\n\t"	//r3 is now addr
		"	ldr		r2, mRomRamStart						\n\t"
		"	ldr		r4, mRomRamLen							\n\t"
		"	subs	r2, r3, r2								\n\t"
		"	cmp		r2, r4									\n\t"
		"	bcs		report_some_fault_pop_r4lr				\n\t"
		"	ldr		r2, [r0, #4 * 6]						\n\t"
		"	adds	r2, #2									\n\t"
		"	str		r2, [r0, #4 * 6]						\n\t"
		"	bl		get_rd_0								\n\t"	//r1 is now val
		"	b		write_2b								\n\t"
		
		"strb_2:											\n\t"
		"	push	{r4, lr}								\n\t"
		"	bl		get_addr_2reg							\n\t"	//r3 is now addr
		"	ldr		r2, mRomRamStart						\n\t"
		"	ldr		r4, mRomRamLen							\n\t"
		"	subs	r2, r3, r2								\n\t"
		"	cmp		r2, r4									\n\t"
		"	bcs		report_some_fault_pop_r4lr				\n\t"
		"	ldr		r2, [r0, #4 * 6]						\n\t"
		"	adds	r2, #2									\n\t"
		"	str		r2, [r0, #4 * 6]						\n\t"
		"	bl		get_rd_0								\n\t"	//r1 is now val
		"	b		write_1b								\n\t"
		
		"str_1:												\n\t"
		"	push	{r4, lr}								\n\t"
		"	movs	r3, #2									\n\t"
		"	bl		get_addr_with_imm						\n\t"	//r3 is now addr
		"	ldr		r2, mRomRamStart						\n\t"
		"	ldr		r4, mRomRamLen							\n\t"
		"	subs	r2, r3, r2								\n\t"
		"	cmp		r2, r4									\n\t"
		"	bcs		report_some_fault_pop_r4lr				\n\t"
		"	ldr		r2, [r0, #4 * 6]						\n\t"
		"	adds	r2, #2									\n\t"
		"	str		r2, [r0, #4 * 6]						\n\t"
		"	bl		get_rd_0								\n\t"	//r1 is now val
		"	b		write_4b								\n\t"
		
		"strb_1:											\n\t"
		"	push	{r4, lr}								\n\t"
		"	movs	r3, #0									\n\t"
		"	bl		get_addr_with_imm						\n\t"	//r3 is now addr
		"	ldr		r2, mRomRamStart						\n\t"
		"	ldr		r4, mRomRamLen							\n\t"
		"	subs	r2, r3, r2								\n\t"
		"	cmp		r2, r4									\n\t"
		"	bcs		report_some_fault_pop_r4lr				\n\t"
		"	ldr		r2, [r0, #4 * 6]						\n\t"
		"	adds	r2, #2									\n\t"
		"	str		r2, [r0, #4 * 6]						\n\t"
		"	bl		get_rd_0								\n\t"	//r1 is now val
		"	b		write_1b								\n\t"
		
		"strh_1:											\n\t"
		"	push	{r4, lr}								\n\t"
		"	movs	r3, #1									\n\t"
		"	bl		get_addr_with_imm						\n\t"	//r3 is now addr
		"	ldr		r2, mRomRamStart						\n\t"
		"	ldr		r4, mRomRamLen							\n\t"
		"	subs	r2, r3, r2								\n\t"
		"	cmp		r2, r4									\n\t"
		"	bcs		report_some_fault_pop_r4lr				\n\t"
		"	ldr		r2, [r0, #4 * 6]						\n\t"
		"	adds	r2, #2									\n\t"
		"	str		r2, [r0, #4 * 6]						\n\t"
		"	bl		get_rd_0								\n\t"	//r1 is now val
		"	b		write_2b								\n\t"
		
		"stmia_r0r1r2r3:									\n\t"	//r0 is state, r1 is instr, r2 is pc, r3 is "instr >> 8"
		"	push	{r4-r7}									\n\t"
		"	subs	r3, #0xc0								\n\t"
		"	lsls	r3, r3, #2								\n\t"
		"	adds	r3, r0, r3								\n\t"	//r3 points to where base reg is
		"	b		do_stmia								\n\t"
		
		"stmia_r4r5r6r7:									\n\t"	//r0 is state, r1 is instr, r2 is pc, r3 is "instr >> 8"
		"	push	{r4-r7}									\n\t"
		"	subs	r3, #0xc4								\n\t"
		"	lsls	r3, r3, #2								\n\t"
		"	add		r3, sp									\n\t"	//r3 points to where base reg is
		"	b		do_stmia								\n\t"
		
		"write_4b:											\n\t"	//r1 is val, r3 is addr
		"	strb	r1, [r3]								\n\t"	//flush cache line
		"	movs	r2, #0x18								\n\t"
		"	lsls	r2, r2, #24								\n\t"
		"	strb	r2, [r2, #0x08]							\n\t"	//use lane replication to disable ssi
		"	ldr		r0, [r2, #0x00]							\n\t"
		"	mov		r12, r0									\n\t"
		"	subs	r0, #0xc0								\n\t"	//minus 0x200
		"	subs	r0, #0xc0								\n\t"
		"	subs	r0, #0x80								\n\t"
		"	str		r0, [r2, #0x00]							\n\t"
		"	movs	r0, #1									\n\t"
		"	str		r0, [r2, #0x08]							\n\t"
		"	movs	r0, #0x38								\n\t"
		"	str		r0, [r2, #0x60]							\n\t"
		"	str		r3, [r2, #0x60]							\n\t"
		"	rev     r1, r1									\n\t"
		"	str		r1, [r2, #0x60]							\n\t"
		"	mov		r3, r12									\n\t"
		"1:													\n\t"
		"	ldr		r0, [r2, #0x28]							\n\t"
		"	lsrs	r0, r0, #1								\n\t"
		"	bcs		1b										\n\t"
		"	strb	r2, [r2, #0x08]							\n\t"	//use lane replication to disable ssi
		"	str		r3, [r2, #0x00]							\n\t"
		"	movs	r0, #1									\n\t"
		"	str		r0, [r2, #0x08]							\n\t"
		"	pop		{r4, pc}								\n\t"
		
		"write_2b:											\n\t"	//r1 is val, r3 is addr
		"	strb	r1, [r3]								\n\t"	//flush cache line
		"	movs	r2, #0x18								\n\t"
		"	lsls	r2, r2, #24								\n\t"
		"	strb	r2, [r2, #0x08]							\n\t"	//use lane replication to disable ssi
		"	ldr		r0, [r2, #0x00]							\n\t"
		"	mov		r12, r0									\n\t"
		"	ldr		r4, =0x00100200							\n\t"	//16 bit write, tx only
		"	subs	r0, r0, r4								\n\t"
		"	str		r0, [r2, #0x00]							\n\t"
		"	movs	r0, #1									\n\t"
		"	str		r0, [r2, #0x08]							\n\t"
		"	movs	r0, #0x38								\n\t"
		"	str		r0, [r2, #0x60]							\n\t"
		"	str		r3, [r2, #0x60]							\n\t"
		"	rev16	r1, r1									\n\t"
		"	str		r1, [r2, #0x60]							\n\t"
		"	mov		r3, r12									\n\t"
		"1:													\n\t"
		"	ldr		r0, [r2, #0x28]							\n\t"
		"	lsrs	r0, r0, #1								\n\t"
		"	bcs		1b										\n\t"
		"	strb	r2, [r2, #0x08]							\n\t"	//use lane replication to disable ssi
		"	str		r3, [r2, #0x00]							\n\t"
		"	movs	r0, #1									\n\t"
		"	str		r0, [r2, #0x08]							\n\t"
		"	pop		{r4, pc}								\n\t"
		
		"write_1b:											\n\t"	//r1 is val, r3 is addr
		"	strb	r1, [r3]								\n\t"	//flush cache line
		"	movs	r2, #0x18								\n\t"
		"	lsls	r2, r2, #24								\n\t"
		"	strb	r2, [r2, #0x08]							\n\t"	//use lane replication to disable ssi
		"	ldr		r0, [r2, #0x00]							\n\t"
		"	mov		r12, r0									\n\t"
		"	ldr		r4, =0x00180200							\n\t"	//8 bit write, tx only
		"	subs	r0, r0, r4								\n\t"
		"	str		r0, [r2, #0x00]							\n\t"
		"	movs	r0, #1									\n\t"
		"	str		r0, [r2, #0x08]							\n\t"
		"	movs	r0, #0x38								\n\t"
		"	str		r0, [r2, #0x60]							\n\t"
		"	str		r3, [r2, #0x60]							\n\t"
		"	str		r1, [r2, #0x60]							\n\t"
		"	mov		r3, r12									\n\t"
		"1:													\n\t"
		"	ldr		r0, [r2, #0x28]							\n\t"
		"	lsrs	r0, r0, #1								\n\t"
		"	bcs		1b										\n\t"
		"	strb	r2, [r2, #0x08]							\n\t"		//use lane replication to disable ssi
		"	str		r3, [r2, #0x00]							\n\t"
		"	movs	r0, #1									\n\t"
		"	str		r0, [r2, #0x08]							\n\t"
		"	pop		{r4, pc}								\n\t"
		
		".ltorg												\n\t"
		
		"j_report_some_fault:								\n\t"
		"	pop		{r4-r7}									\n\t"
		"	b		report_some_fault						\n\t"
		
		"do_stmia:											\n\t"		//r0 is state, r1 is instr, r3 is &base_addr
		"	lsls	r2, r1, #24								\n\t"
		"	beq		j_report_some_fault						\n\t"
		
		"	ldr		r4, [r3]								\n\t"		//addr

		"	ldr		r2, mRomRamStart						\n\t"
		"	ldr		r5, mRomRamLen							\n\t"
		"	subs	r2, r4, r2								\n\t"
		"	cmp		r2, r5									\n\t"
		"	bcs		j_report_some_fault						\n\t"

		"	movs	r2, #0x18								\n\t"
		"	lsls	r2, r2, #24								\n\t"
		
		"	strb	r2, [r2, #0x08]							\n\t"		//use lane replication to disable ssi
		"	ldr		r6, [r2, #0x00]							\n\t"
		"	subs	r7, r6, #0x02							\n\t"		//minus 0x200
		"	subs	r7, #0xff								\n\t"
		"	subs	r7, #0xff								\n\t"
		"	str		r7, [r2, #0x00]							\n\t"
		"	movs	r7, #1									\n\t"
		"	str		r7, [r2, #0x08]							\n\t"
		"	movs	r7, #0x38								\n\t"
		"	str		r7, [r2, #0x60]							\n\t"		//send cmd
		"	nop												\n\t"		//(this delay is needed ... do NOT ask)
		"	str		r4, [r2, #0x60]							\n\t"		//send addr
		
		".macro wrreg,a,getter								\n\t"
		"	lsrs	r1, r1, #1								\n\t"
		"	bcc		2f										\n\t"
		"	\\getter \\a									\n\t"
		"	rev     r5, r5									\n\t"
		"1:													\n\t"
		"	ldr		r7, [r2, #0x28]							\n\t"
		"	lsrs	r7, r7, #2								\n\t"
		"	bcc		1b										\n\t"
		"	str		r5, [r2, #0x60]							\n\t"
		"	adds	r4, #4									\n\t"
		"2:													\n\t"
		".endm												\n\t"
		".macro wrreg_get_L,a								\n\t"
		"	ldr		r5, [r0, #4 * \\a]						\n\t"
		".endm												\n\t"
		".macro wrreg_get_H,a								\n\t"
		"	ldr		r5, [sp, #4*((\\a) - 4)]				\n\t"
		".endm												\n\t"
		".macro wrreg_L,a									\n\t"
		"	wrreg	\\a, wrreg_get_L						\n\t"
		".endm												\n\t"
		".macro wrreg_H,a									\n\t"
		"	wrreg	\\a, wrreg_get_H						\n\t"
		".endm												\n\t"
		
		"	wrreg_L	0										\n\t"
		"	wrreg_L	1										\n\t"
		"	wrreg_L	2										\n\t"
		"	wrreg_L	3										\n\t"
		"	wrreg_H	4										\n\t"
		"	wrreg_H	5										\n\t"
		"	wrreg_H	6										\n\t"
		"	wrreg_H	7										\n\t"
		
		"	ldr		r5, [r3]								\n\t"		//get old addr for flushing
		"	str		r4, [r3]								\n\t"		//wbak
		
		//wait for done
		"1:													\n\t"
		"	ldr		r7, [r2, #0x28]							\n\t"
		"	lsrs	r7, r7, #1								\n\t"
		"	bcs		1b										\n\t"
		"	strb	r2, [r2, #0x08]							\n\t"		//use lane replication to disable ssi
		"	str		r6, [r2, #0x00]							\n\t"
		"	movs	r7, #1									\n\t"
		"	str		r7, [r2, #0x08]							\n\t"
		
		"	ldr		r2, [r0, #4 * 6]						\n\t"
		"	adds	r2, #2									\n\t"
		"	str		r2, [r0, #4 * 6]						\n\t"
		
		//flush now (it was found that flushing DURING write was causing the SSI unit to issue spurious word writes
		"1:													\n\t"
		"	stmia	r5!, {r2}								\n\t"
		"	cmp		r5, r4									\n\t"
		"	bne		1b										\n\t"
		"	pop		{r4-r7}									\n\t"
		"	bx		lr										\n\t"
		
		
		".balign 4											\n\t"
		".globl mRomRamStart								\n\t"
		"mRomRamStart:										\n\t"
		"	.word	0x10200000								\n\t"
		".globl mRomRamLen									\n\t"
		"mRomRamLen:										\n\t"
		"	.word	0x00600000								\n\t"
		
		"get_rd_0:											\n\t"		//r1 is instr, r0 is pushed state, on return r1 is value of Rd at position 0 in instr
		"	lsls	r1, r1, #29								\n\t"		//	do rememeber that r4 is at [sp] not in "r4"
		"	lsrs	r1, r1, #27								\n\t"		//2 instrs per case
		"	add		pc, r1									\n\t"
		"	nop												\n\t"
		
		".macro get_rd_0_rL a								\n\t"
		"	ldr		r1, [r0, #4 * \\a]						\n\t"
		"	bx		lr										\n\t"
		".endm												\n\t"
		".macro get_rd_0_rH a								\n\t"
		"	mov		r1, r\\a								\n\t"
		"	bx		lr										\n\t"
		".endm												\n\t"
		
		"	get_rd_0_rL 0									\n\t"
		"	get_rd_0_rL 1									\n\t"
		"	get_rd_0_rL 2									\n\t"
		"	get_rd_0_rL 3									\n\t"
		
		//r4 is special
		"	ldr		r1, [sp]								\n\t"
		"	bx		lr										\n\t"
		
		"	get_rd_0_rH 5									\n\t"
		"	get_rd_0_rH 6									\n\t"
		"	get_rd_0_rH 7									\n\t"
		
		
		"get_addr_with_imm:									\n\t"		//r1 is instr, r0 is pushed state, r3 is shift imm by val, on return r3 is addr
		"	lsls	r2, r1, #21								\n\t"
		"	lsrs	r2, r2, #27								\n\t"
		"	lsls	r2, r3									\n\t"		//r2 is now the imm, properly shifted
		"	movs	r3, #0x38								\n\t"
		"	ands	r3, r1									\n\t"
		"	add		pc, r3									\n\t"		//4 instrs per case
		"	nop												\n\t"
		
		".macro get_addr_with_imm_rL,a						\n\t"
		"	ldr		r3, [r0, #4 * \\a]						\n\t"
		"	adds	r3, r3, r2								\n\t"
		"	bx		lr										\n\t"
		"	nop												\n\t"
		".endm												\n\t"
		".macro get_addr_with_imm_rH,a						\n\t"
		"	adds	r3, r\\a, r2							\n\t"
		"	bx		lr										\n\t"
		"	nop												\n\t"
		"	nop												\n\t"
		".endm												\n\t"
		
		"	get_addr_with_imm_rL 0							\n\t"
		"	get_addr_with_imm_rL 1							\n\t"
		"	get_addr_with_imm_rL 2							\n\t"
		"	get_addr_with_imm_rL 3							\n\t"
		"	get_addr_with_imm_rH 4							\n\t"
		"	get_addr_with_imm_rH 5							\n\t"
		"	get_addr_with_imm_rH 6							\n\t"
		"	get_addr_with_imm_rH 7							\n\t"
		
		
		"get_addr_2reg:										\n\t"		//r1 is instr, r0 is pushed state, on return r3 is addr, r2 is clobbered. [rX, rX] case could be one cycle faster, but then again, who does that?
		"	lsls	r3, r1, #23								\n\t"
		"	lsrs	r3, r3, #26								\n\t"
		"	lsls	r3, r3, #3								\n\t"		//4 instrs per case
		"	add		pc, r3									\n\t"
		"	nop												\n\t"
		
		".macro	get_addr_2reg_rL_rL,a,b						\n\t"
		"	ldr		r3, [r0, #4 * \\a]						\n\t"
		"	ldr		r2, [r0, #4 * \\b]						\n\t"
		"	adds	r3, r3, r2								\n\t"
		"	bx		lr										\n\t"
		".endm												\n\t"
		".macro	get_addr_2reg_rL_rH,a,b						\n\t"
		"	ldr		r3, [r0, #4 * \\a]						\n\t"
		"	adds	r3, r3, r\\b							\n\t"
		"	bx		lr										\n\t"
		"	nop												\n\t"
		".endm												\n\t"
		".macro	get_addr_2reg_rH_rH,a,b						\n\t"
		"	adds	r3, r\\a, r\\b							\n\t"
		"	bx		lr										\n\t"
		"	nop												\n\t"
		"	nop												\n\t"
		".endm												\n\t"
		".macro get_addr_2reg_rL,a							\n\t"
		"	get_addr_2reg_rL_rL \\a 0						\n\t"
		"	get_addr_2reg_rL_rL \\a 1						\n\t"
		"	get_addr_2reg_rL_rL \\a 2						\n\t"
		"	get_addr_2reg_rL_rL \\a 3						\n\t"
		"	get_addr_2reg_rL_rH \\a 4						\n\t"
		"	get_addr_2reg_rL_rH \\a 5						\n\t"
		"	get_addr_2reg_rL_rH \\a 6						\n\t"
		"	get_addr_2reg_rL_rH \\a 7						\n\t"
		".endm												\n\t"
		".macro get_addr_2reg_rH,a							\n\t"
		"	get_addr_2reg_rL_rH 0 \\a						\n\t"
		"	get_addr_2reg_rL_rH 1 \\a						\n\t"
		"	get_addr_2reg_rL_rH 2 \\a						\n\t"
		"	get_addr_2reg_rL_rH 3 \\a						\n\t"
		"	get_addr_2reg_rH_rH 4 \\a						\n\t"
		"	get_addr_2reg_rH_rH 5 \\a						\n\t"
		"	get_addr_2reg_rH_rH 6 \\a						\n\t"
		"	get_addr_2reg_rH_rH 7 \\a						\n\t"
		".endm												\n\t"
		"	get_addr_2reg_rL 0								\n\t"
		"	get_addr_2reg_rL 1								\n\t"
		"	get_addr_2reg_rL 2								\n\t"
		"	get_addr_2reg_rL 3								\n\t"
		"	get_addr_2reg_rH 4								\n\t"
		"	get_addr_2reg_rH 5								\n\t"
		"	get_addr_2reg_rH 6								\n\t"
		"	get_addr_2reg_rH 7								\n\t"
		:
		:
		: "cc", "memory", "r0", "r1", "r2", "r3", "r12" //yes gcc needs this list...
	);
}
