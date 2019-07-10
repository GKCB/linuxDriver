
#define S3C2440_MPLL_200MHZ	((0x5c<<12) | (0x01<<4) | (0x02))

.text
.global _start
_start:

/*1. close watch dog*/
	ldr r0,=0x53000000
	mov r1,#0
	str r1,[r0]

/*2. config clock*/
	ldr r0,=0x4c000014
	mov r1,#0x03
	str r1,[r0]

	/*CPU fast bus mode -> asynchronouse bus mode*/
	mrc p15, 0, r1, c1, c0, 0
	orr r1, r1, #0xc0000000
	mcr p15, 0, r1, c1, c0, 0	

	/*MPLLCON = S3C2440_MPLL_200MHZ*/
	ldr r0, =0x4c000004
	ldr r1, =S3C2440_MPLL_200MHZ
	str r1, [r0]
	
/*3. init SDRAM*/
	
/*4. relocate*/

/*5. jump to main*/

sdram_config:


