/*
*LED:GPF4
*/

//.text标明这是代码段 
//2018-12-29
.text 
.global _start



/*配置 GPF5 为输出引脚*/
/*把 0x0x400 写到地址 0x56000050 */
_start:
	ldr r1,=0x56000050
	ldr r0,=0x400
	str r0,[r1]
	
/*设置 GPF5 输出高电平*/
/*把0x0写到地址0x56000054*/

	ldr r1,=0x56000054
	ldr r0,=0x0
	str r0,[r1]

/* 添加一个死循环,后继Flash的内容是不确定的 */
halt:
	b halt
