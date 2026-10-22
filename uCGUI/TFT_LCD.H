
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef TFT_LCD_H_
#define TFT_LCD_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

enum
{
	NORMAL = 0,		/* 字体显示有背景颜色*/
	TRANSP = 1,		/* 字体显示无背景颜色 */
};

void TFT_DrawLine(u16 x1,u16 y1,u16 x2,u16 y2,u16 color);
void TFT_draw_circle(u16 cx,u16 cy,u16 r,u16 color,u8 fill);
void TFT_draw_Rectangle(u16 x0, u16 y0, u16 x1, u16 y1,u16 color,u8 fill);
void TFT_draw_Square(u16 x0, u16 y0, u16 with, u16 color,u8 fill);
void TFT_drow_touch_point(unsigned int x,unsigned int y,u16 color);
void TFT_LCD_Test(void);
void TFT_DispString(u16 x,u16 y,unsigned char *pstr,u8 dotwidth,u16 LineColor,u16 FillColor, u8 Mod); 

#ifdef __cplusplus
}
#endif

#endif /* TFT_LCD_H_ */
