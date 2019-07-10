
void lcd_test(void)
{
	unsigned int fb_base;
	int xres, yres, bpp;
	int x,y;
	unsigned short *p;

	/*��ʼ��LCD*/
	lcd_init();

	/*ʹ��lcd*/
	lcd_enable();

	/*���LCD�Ĳ���: fb_base, xres, yres, bpp*/
	get_lcd_params(&fb_base, &xres, &yres, &bpp);
	
	/*��framebuffer��д����*/
	if(bpp == 16)
	{
		/*RED Color*/
		
		/*565: 0xf800*/
		p = (unsigned short *)fb_base;
		for(x = 0; x < xres; x++)
		{
			for(y = 0; y < yres; y++)
			{
				*p++ = 0xf800;
			}
		}
		
		/**/
	}
	
	
}

