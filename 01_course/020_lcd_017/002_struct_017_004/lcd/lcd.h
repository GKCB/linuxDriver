
#ifndef _LCD_H
#define _LCD_H

enum{
	NORMAL = 0,
	INVERT = 1,	
};
/* NORMAL : 正常极性
 * INVERT : 反转极性
 */
typedef struct pins_polarity{
	int vclk; 
	int rgb;
	int hsync;
	int vsync;
}pins_polarity, *p_pins_polarity;

typedef struct timer_sequence{
	/*垂直方向*/
	int tvp;
	int tvb;
	int tvf;	
	
	/*水平方向*/
	int thp;
	int thb;
	int thf;
	
}timer_sequence, *p_timer_sequence;

typedef struct lcd_params{
	/*引脚极性*/
	pins_polarity pins_pol;	
	/*时序*/
	timer_sequence timer_seq;
	/*分辨率, bpp*/	
	int xres;
	int yres;
	int bpp;
	/*framebuffer的地址*/
	unsigned int fb_base;
}lcd_params, *p_lcd_params;


#endif /* _LCD_H */
