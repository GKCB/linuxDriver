
void delay(volatile int d)
{
	while(d--);
}

/*main 函数不是必需的*/
int led_on(int which)
/*which 表示点亮那一盏灯*/
{
	unsigned int *pGPFCON = (unsigned int *)0x56000050;
	unsigned int *pGPFDAT = (unsigned int *)0x56000054;
	if(which == 4)
	{
		/*配置GPF4为输出引脚*/
		*pGPFCON = 0x100;
	}
	else if(which == 5)
	{
		/*配置GPF5为输出引脚*/
		*pGPFCON = 0x400;
	}
	/*配置GPF4/5为输出0*/
	*pGPFDAT = 0;
	return 0;
}




