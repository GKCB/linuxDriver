
#include "s3c2440_soc.h"

void timer_init(void)
{
	/* ����TIMER0��ʱ�� */
	/*Timer clk = PCLK / {prescaler value+1} / {divider value}
				= 50000000/(99+1)/16
				= 31250
	*/
	TCFG0 = 99; /*prescaler0 = 99,����timer0,1*/
	TCFG1 &= ~0xf;
	TCFG1 |= 3; /*MUX1: 1/16*/
	
								
	
	/* ����TIMER0�ĳ�ֵ */
	
	/* ���س�ֵ������timer0 */
	
	/* ����Ϊ�Զ����� */
	
	/* �����ж� */
	
}

void timer_irq(void)
{
	/* ��Ƽ��� */
	
}