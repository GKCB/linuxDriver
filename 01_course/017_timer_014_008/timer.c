
#include "s3c2440_soc.h"

void timer_init(void)
{
	/* 设置TIMER0的时钟 */
	/*Timer clk = PCLK / {prescaler value+1} / {divider value}
				= 50000000/(99+1)/16
				= 31250
	*/
	TCFG0 = 99; /*prescaler0 = 99,用于timer0,1*/
	TCFG1 &= ~0xf;
	TCFG1 |= 3; /*MUX1: 1/16*/
	
								
	
	/* 设置TIMER0的初值 */
	
	/* 加载初值，启动timer0 */
	
	/* 设置为自动加载 */
	
	/* 设置中断 */
	
}

void timer_irq(void)
{
	/* 点灯计数 */
	
}