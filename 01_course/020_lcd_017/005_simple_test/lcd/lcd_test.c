
void lcd_test(void)
{
	unsigned int fb_base;
	int xres, yres, bpp;
	int x,y;
	unsigned short *p;

	/*初始化LCD*/
	lcd_init();

	/*使能lcd*/
	lcd_enable();

	/*获得LCD的参数: fb_base, xres, yres, bpp*/
	get_lcd_params(&fb_base, &xres, &yres, &bpp);
	
	/*往framebuffer中写数据*/
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

