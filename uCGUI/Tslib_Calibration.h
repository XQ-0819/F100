/******************************************************************************

  文 件 名   : Tslib_Calibration.h
  版 本 号   : 初稿
  作    者   : 邹稳
  生成日期   : 2014年6月17日星期二
  最近修改   :
  功能描述   : Tslib_Calibration.C 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2014年6月17日星期二
    作    者   : 邹稳
    修改内容   : 创建文件

  2.日    期   : 2014-11-30
    作    者   : 邹稳
    修改内容   : 添加MYGUI_TOUCH_Exec函数，适应emWin
******************************************************************************/


/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/



/*****************************************************************************/
#ifndef __TSLIB_CALIBRATION_H__
#define __TSLIB_CALIBRATION_H__
#include "stm32f4xx.h"
typedef struct
{
    int x[5], xfb[5];
    int y[5], yfb[5];
    int a[7];
} calibration;
extern calibration cal;
#define LCD_XSIZE 800
#define LCD_YSIZE 480





#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */
extern __IO u8 calibration_break_flag;

extern int  AD2GUI_COORDX(unsigned  int ad_x,unsigned int ad_y);
extern int  AD2GUI_COORDY(unsigned  int ad_x,unsigned int ad_y);
extern void Calibration(void);
extern int perform_calibration(calibration *cal);
void MYGUI_TOUCH_Exec(void);
extern void Touch_Task(void * pdata);	
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __TSLIB_CALIBRATION_H__ */

