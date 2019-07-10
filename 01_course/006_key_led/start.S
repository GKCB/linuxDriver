

.text
.global _start

_start:
	/* 关闭看门狗 */
	ldr r0,=0x53000000
	ldr r1,=0x0
	str r1,[r0]
	
	/*为main函数， 设置内存：SP 栈*/
	/*sp 等于某块内存的地址*/
	/*分辨是nor/nand启动
	*写0 到0地址，再读出来
	*如果得到0，表示0地址上的内容被修改了，它对应ram，这就是nand启动
	*否则就是nor启动
	*/
	
	/*先把0地址里的数据读取出来*/
	mov r1,#0	
	ldr r0,[r1] /*读出原来的值备份*/
	str r1,[r1] /*0写入0地址 0->[0] r1 等于写入0地址的值*/
	ldr r2,[r1] /*r2=[0] r2 等于从0地址读取的值*/
	cmp r1,r2
	ldr sp,=0x40000000 + 4096 /*先假设从nor 启动*/
	moveq sp,#4096
	streq r0,[r1] /*恢复原来的值*/
		
	bl main
	
	/*bl不光是跳转，还会保存返回地址*/
	

halt:
	b halt

