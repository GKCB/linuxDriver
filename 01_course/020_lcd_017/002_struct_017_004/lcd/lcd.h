
#ifndef _LCD_H
#define _LCD_H

enum{
	NORMAL = 0,
	INVERT = 1,	
};
/* NORMAL : ��������
 * INVERT : ��ת����
 */
typedef struct pins_polarity{
	int vclk; 
	int rgb;
	int hsync;
	int vsync;
}pins_polarity, *p_pins_polarity;

typedef struct timer_sequence{
	/*��ֱ����*/
	int tvp;
	int tvb;
	int tvf;	
	
	/*ˮƽ����*/
	int thp;
	int thb;
	int thf;
	
}timer_sequence, *p_timer_sequence;

typedef struct lcd_params{
	/*���ż���*/
	pins_polarity pins_pol;	
	/*ʱ��*/
	timer_sequence timer_seq;
	/*�ֱ���, bpp*/	
	int xres;
	int yres;
	int bpp;
	/*framebuffer�ĵ�ַ*/
	unsigned int fb_base;
}lcd_params, *p_lcd_params;


#endif /* _LCD_H */
