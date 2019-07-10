
#define FONTDATAMAX 4096
extern const unsigned char fontdata_8x16[FONTDATAMAX];

/*LCD Config params*/
static unsigned int fb_base;
static int xres,yres,bpp;

void font_init(void)
{
	get_lcd_params(&fb_base,&xres,&yres,&bpp);
}


void fb_print_char(int x,int y,char c,unsigned int color)
{
	int i,j;
	
	unsigned char *dots = &fontdata_8x16[c*16];	
	
	unsigned char data;
	int bit;
	
	for(j=y; j<y+16; j++)
	{
		data = *dots++;
		bit = 7;
		for(i=x; i<x+8; i++)
		{
			if(data & (1<<bit))
				fb_put_pixel(i,j,color);
			bit--;
		}
	}
	
}

void fb_print_string(int x,int y,char* str,unsigned int color)
{
	int i = 0,j;

	while(str[i])
	{
		if(str[i] == '\n')
		{
			y = y + 16;
		}			
		else if(str[i] == '\r')
		{
			x = 0;
		}
		else
		{
			fb_print_char(x,y,str[i],color);
			x = x+8;
			if(x>=xres)
			{
				x = 0;
				y = y+16;
			}
		}
		i++;
	}
	
}


