
#include "s3c2440_soc.h"
#include "uart.h"
#include "init.h"

char g_char = 'A';

int main(void)
{
	uart0_init();

	while (1)
	{
		putchar(g_char);
		g_char++;
		delay(100000);
	}
	sdram_init();

	if (sdram_test() == 0)
		led_test();
	
	return 0;
}



