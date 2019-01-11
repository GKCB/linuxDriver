

.text
.global _start

_start:
	/*为main函数， 设置内存：SP 栈*/
	/*sp 等于某块内存的地址*/
	ldr sp,= 4096 
	/*
	*Nand启动,0地址开始的4K空间对应的是片内内存.
	*现在将这个内存设置成栈的顶部
	*/
	//ldr sp,=0x40000000 + 4096 /*Nor启动*/

	
	/* 调用main函数 */
	bl main
	/*bl不光是跳转，还会保存返回地址*/
	

halt:
	b halt

