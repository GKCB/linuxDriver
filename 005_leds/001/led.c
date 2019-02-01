
void delay(volatile int d)
{
	while(d--);
}

/*main 函数不是必需的*/
int main(void)
/*which 表示点亮那一盏灯*/
{
	volatile unsigned int *pGPFCON = (volatile unsigned int *)0x56000050;
	volatile unsigned int *pGPFDAT = (volatile unsigned int *)0x56000054;
	int val = 0; /*val: 0b000-0b111*/
	int tmp;
	/*三盏灯对应GPF4/5/6*/
	/*设置GPFCON让GPF4/5/6配置为输出引脚 使用位操作*/
	*pGPFCON &= ~( (3<<8) | (3<<10) | (3<<12) ); /*Reserved 进行清零*/
	*pGPFCON |=  ( (1<<8) | (1<<10) | (1<<12) );
	
	/*循环点亮*/
	while(1)
	{
		tmp = ~val; /*LED 低电平输出*/
		tmp &= 7;
		*pGPFDAT &= ~(7<<4);
		*pGPFDAT |= (tmp<<4);
		delay(100000);
		val++;
		if( val==8 )
		{
			val = 0;
		}	
	}
	
	return 0;
}




