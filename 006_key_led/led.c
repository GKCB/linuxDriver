
#include "s3c2440_soc.h"
void delay(volatile int d)
{
	while(d--);
}

//#define GPFCON (*((volatile unsigned int *)0x56000050))
//#define GPFDAT (*((volatile unsigned int *)0x56000054))
/*main 函数不是必需的*/
int main(void)
{
	int val1,val2; /*val: 0b000-0b111*/
	int tmp;
	/*三盏灯对应GPF4/5/6*/
	/*设置GPFCON让GPF4/5/6配置为输出引脚 使用位操作*/
	GPFCON &= ~( (3<<8) | (3<<10) | (3<<12) ); /*进行清零*/
	GPFCON |=  ( (1<<8) | (1<<10) | (1<<12) );
	
	/*配置3个按键引脚为输入引脚
	* GPF0(S2),GPF2(S3),GPG3(S4)	
	*/
	GPFCON &= ~( (3<<0) | (3<<4) ); /* GPF0, GPF2*/
	GPGCON &= ~( (3<<6) );
	
	/*循环点亮*/
	while(1)
	{
		val1 = GPFDAT;
		val2 = GPGDAT;
		if(val1 & (1<<0) ) /*s2 -->gpf6*/
		{
			/*松开*/
			GPFDAT |= (1<<6);
		}
		else
		{
			/*按下*/
			GPFDAT &= ~(1<<6);
		}
		
		if(val1 & (1<<2) ) /*s3 -->gpf5*/
		{
			/*松开*/
			GPFDAT |= (1<<5);
		}
		else
		{
			/*按下*/
			GPFDAT &= ~(1<<5);
		}
		
		if(val2 & (1<<3) ) /*s3 -->gpf5*/
		{
			/*松开*/
			GPFDAT |= (1<<4);
		}
		else
		{
			/*按下*/
			GPFDAT &= ~(1<<4);
		}
		
		
	}
	
	return 0;
}




