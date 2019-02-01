
#include "s3c2440_soc.h"
void delay(volatile int d)
{
	while(d--);
}

//#define GPFCON (*((volatile unsigned int *)0x56000050))
//#define GPFDAT (*((volatile unsigned int *)0x56000054))
/*main 函数不是必需的*/
int main(void)
/*which 表示点亮那一盏灯*/
{
	volatile unsigned int * pGPFCON = pGPFCON;
	volatile unsigned int * pGPFDAT = pGPFDAT;
	int val = 0; /*val: 0b000-0b111*/
	int tmp;
	/*三盏灯对应GPF4/5/6*/
	/*设置GPFCON让GPF4/5/6配置为输出引脚 使用位操作*/
	GPFCON &= ~( (3<<8) | (3<<10) | (3<<12) ); /*Reserved 进行清零*/
	GPFCON |=  ( (1<<8) | (1<<10) | (1<<12) );
	
	/*循环点亮*/
	while(1)
	{
		tmp = ~val; /*LED 低电平输出*/
		tmp &= 7;
		GPFDAT &= ~(7<<4);
		GPFDAT |= (tmp<<4);
		delay(100000);
		val++;
		if( val==8 )
		{
			val = 0;
		}	
	}
	
	return 0;
}




