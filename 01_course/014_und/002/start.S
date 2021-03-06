
.text
.global _start

_start:
	b reset  /*vector 0： reset*/
	ldr pc, und_addr /*vector 4： und*/ /*确保进入SDRAM*/
	//b do_und /*vector 4： und*/

und_addr:
	.word do_und
	
do_und:
	/* 执行到这里之前：
	* 1. lr_und 保存有被中断模式中的下一条即将执行的指令的地址
	* 2. SPSR_und 保存有被中断模式的CPSR
	* 3. CPSR中的M4-M0被设置为11011，进入到und模式
	* 4. 跳到0x4的地方执行程序
	*/
	
	/*1,保存现场*/
	/*2,处理und异常*/
	/*3,恢复现场*/
	
	/*sp_und未设置，需要设置*/
	/*指向一块没有使用的内存*/
	ldr sp,=0x34000000 /*指向64M SDRAM的最高地址*/
	
	/*保存现场 r0 - r12*/
	/*在und异常处理函数中，有可能会修改r0-r12需要先保存下来*/
	/*lr是异常处理完后的返回地址，也要保存*/
	stmdb sp!,{r0-r12,lr}	
	
	/*处理und异常*/	
	/*函数需要传递两个参数*/
	/* cpsr的值读入r0 */
	mrs r0, cpsr /* R表示regsiter, S表示state */
	
	/*字符串地址赋值给reg r1*/
	ldr r1,=und_string
	bl printException
	
	/*恢复现场*/
	ldmia sp!,{r0-r12,pc}^ /* ^会把spsr的值恢复到CPSR里 */
	
und_string:
	.string "undefined instruction exception"
	/*恢复现场*/
.align 4 	
/*保证后面的指令是4字节对齐*/

reset:
	/* 关闭看门狗 */
	ldr r0, =0x53000000
	ldr r1, =0
	str r1, [r0]

	/* 设置MPLL, FCLK : HCLK : PCLK = 400m : 100m : 50m */
	/* LOCKTIME(0x4C000000) = 0xFFFFFFFF */
	ldr r0, =0x4C000000
	ldr r1, =0xFFFFFFFF
	str r1, [r0]

	/* CLKDIVN(0x4C000014) = 0X5, tFCLK:tHCLK:tPCLK = 1:4:8  */
	ldr r0, =0x4C000014
	ldr r1, =0x5
	str r1, [r0]

	/* 设置CPU工作于异步模式 */
	mrc p15,0,r0,c1,c0,0
	orr r0,r0,#0xc0000000   //R1_nF:OR:R1_iA
	mcr p15,0,r0,c1,c0,0

	/* 设置MPLLCON(0x4C000004) = (92<<12)|(1<<4)|(1<<0) 
	 *  m = MDIV+8 = 92+8=100
	 *  p = PDIV+2 = 1+2 = 3
	 *  s = SDIV = 1
	 *  FCLK = 2*m*Fin/(p*2^s) = 2*100*12/(3*2^1)=400M
	 */
	ldr r0, =0x4C000004
	ldr r1, =(92<<12)|(1<<4)|(1<<0)
	str r1, [r0]

	/* 一旦设置PLL, 就会锁定lock time直到PLL输出稳定
	 * 然后CPU工作于新的频率FCLK
	 */
	
	

	/* 设置内存: sp 栈 */
	/* 分辨是nor/nand启动
	 * 写0到0地址, 再读出来
	 * 如果得到0, 表示0地址上的内容被修改了, 它对应ram, 这就是nand启动
	 * 否则就是nor启动
	 */
	mov r1, #0
	ldr r0, [r1] /* 读出原来的值备份 */
	str r1, [r1] /* 0->[0] */ 
	ldr r2, [r1] /* r2=[0] */
	cmp r1, r2   /* r1==r2? 如果相等表示是NAND启动 */
	ldr sp, =0x40000000+4096 /* 先假设是nor启动 */
	moveq sp, #4096  /* nand启动 */
	streq r0, [r1]   /* 恢复原来的值 */

	bl sdram_init
	//bl sdram_init2	 /* 用到有初始值的数组, 不是位置无关码 */

	/* 重定位text, rodata, data段整个程序 */

	bl copy2sdram 
	
	/* 清除BSS段 */
	bl clean_bss

	ldr pc,=sdram
sdram:
	bl uart0_init
	
	bl print1
	/*故意加入一条未定义指令*/
und_code:
	.word 0x03000000 /*或者用0xdeadc0de*/
	/*查阅ARM instruction set*/
	/*看看是不是一条不能识别的指令*/
	
	/*以一个数据的方式引入一条und命令*/
	bl print2
	
	//bl main  /* 使用BL命令相对跳转, 程序仍然在NOR/sram执行 */
	ldr pc, =main  /* 绝对跳转, 跳到SDRAM */

halt:
	b halt
	
