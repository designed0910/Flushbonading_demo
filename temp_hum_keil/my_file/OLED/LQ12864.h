#ifndef __LQ12864_H__
#define __LQ12864_H__
 #include "stm32f10x_conf.h"

extern void LCD_Init(void);
extern void LCD_CLS(void);
extern void LCD_P6x8Str(unsigned char x,unsigned char y,unsigned char ch[]);
extern void LCD_P8x16Str(unsigned char x,unsigned char y,unsigned char ch[]);
extern void LCD_P14x16Str(unsigned char x,unsigned char y,unsigned char ch[]);
extern void LCD_Print(unsigned char x, unsigned char y, unsigned char ch[]);
extern void LCD_PutPixel(unsigned char x,unsigned char y);
extern void LCD_Rectangle(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2,unsigned char gif);
extern void Draw_LQLogo(void);
extern void Draw_LibLogo(const unsigned char *logo);
extern void Draw_BMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char * bmp); 
extern const unsigned char LIBLOGO60x58[480];


#endif

