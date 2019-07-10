
#include "s3c2440_soc.h"
#include "uart.h"
#include "init.h"

char g_char = 'A';
char g_char3 = 'a';
const char g_char2 = 'B';
int g_A = 0;
int g_B;

int main(void)
{
	uart0_init();

	puts("g_A = ");
	printHex(g_A);
	puts("\n\r");
	
	while (1)
	{
		putchar(g_char);
		g_char++;
		
		putchar(g_char3);
		g_char3++;
		delay(100000);
	}
	sdram_init();

	if (sdram_test() == 0)
		led_test();
	
	return 0;
}



