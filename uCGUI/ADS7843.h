/*
********************************************************************************************************
*                                          All Rights Reserved
* Version   : V1.00
* Data      : 2012-08-18/21:49
* By        : yang
* For       : ARMv7M Cortex-M4
* Toolchain : RealView Development Suite
*             RealView Microcontroller Development Kit (MDK)
*             ARM Developer Suite (ADS)
*             Keil uVision
*********************************************************************************************************
*/
#ifndef  _ADS7843_H_
#define  _ADS7843_H_

#include "stm32f4xx.h"

typedef struct
{
	uint16_t XYChange;	/* X, Y 是否交换  */

	uint16_t usMaxAdc;	/* 触摸板最大ADC值，用于有效点判断. 最小ADC = 0  */
	int16_t usAdcNowX;
	int16_t usAdcNowY;

	uint8_t Enable;		/* 触摸检测使能标志 */

}TOUCH_T;

extern TOUCH_T g_tTP;//x--y 交换
//#define ADC_CH_X	1		/* X通道，测量X位置 */
//#define ADC_CH_Y	2		/* Y通道，测量Y位置 */

#define ADC_CH_X	2		/* X通道，测量X位置 */
#define ADC_CH_Y	1		/* Y通道，测量Y位置 */



#define TERMINAL_TURNOFF_TIME_DEF 300    //休眠时间 单位是S  需要任务定时


void SPI_TouchConfig(void);
void TOUCH_SCAN(void);
u8 Get_LCD_State(void);
void LCD_State_SW(void);
u8 get_touch_irq(void);

#endif


