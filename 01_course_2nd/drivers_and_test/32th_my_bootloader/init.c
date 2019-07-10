

int isBootFromNorFlash(void)
{
	volatile int *p = (volatile int *)0;
	int val;

	val = *p;
	*p = 0x12345678;
	if(*p == 0x12345678)
	{
		/*д�ɹ�����Nand����*/
		*p = val;
		return 0;
	}
	else
	{	
		/*Nor�������ڴ�һ��д*/
		return 1;
	}
	
}

int copy_code_to_sdram(unsigned char *src, unsigned char *dest, unsigned int len)
{
	int i = 0;
	/*�����Nor����*/��
	if(isBootFromNorFlash())
	{
		while(i < len)
		{
			dest[i] = src[i];
			i++;
		}
	}
	else
	{
		nand_init();
		nand_read(src, dest, len);
	}
}

