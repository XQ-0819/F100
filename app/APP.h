/******************************************************************************

  文 件 名   : APP.h
  版 本 号   : 初稿
  作    者   : 邹稳
  生成日期   : 2013年4月18日 星期四
  最近修改   :
  功能描述   : APP.C 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2013年4月18日 星期四
    作    者   : 邹稳
    修改内容   : 创建文件

******************************************************************************/

/*****************************************************************************/
#ifndef __APP_H__
#define __APP_H__
#include "stm32f4xx.h"
/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/



#if USE_GD == 1
#define BEEP_PIN               		GPIO_Pin_3
#define BEEP_GPIO_PORT              GPIOE
#define BEEP_GPIO_CLK               RCC_AHB1Periph_GPIOE

#define ALARM_LED_PIN               GPIO_Pin_10
#define ALARM_LED_GPIO_PORT         GPIOA
#define ALARM_LED_GPIO_CLK          RCC_AHB1Periph_GPIOA

#define RUN_LED_PIN               	GPIO_Pin_9
#define RUN_LED_GPIO_PORT         	GPIOA
#define RUN_LED_GPIO_CLK          	RCC_AHB1Periph_GPIOA


#else


#define BEEP_PIN               		GPIO_Pin_4
#define BEEP_GPIO_PORT              GPIOI
#define BEEP_GPIO_CLK               RCC_AHB1Periph_GPIOI

#define ALARM_LED_PIN               GPIO_Pin_7
#define ALARM_LED_GPIO_PORT         GPIOB
#define ALARM_LED_GPIO_CLK          RCC_AHB1Periph_GPIOB

#define RUN_LED_PIN               	GPIO_Pin_6
#define RUN_LED_GPIO_PORT         	GPIOB
#define RUN_LED_GPIO_CLK          	RCC_AHB1Periph_GPIOB


#endif


#define ALARM_LED_ON  GPIO_ResetBits(ALARM_LED_GPIO_PORT,ALARM_LED_PIN)// 报警LED
#define ALARM_LED_OFF GPIO_SetBits(ALARM_LED_GPIO_PORT,ALARM_LED_PIN)
#define ALARM_LED_TOG GPIO_ToggleBits(ALARM_LED_GPIO_PORT,ALARM_LED_PIN)

#define RUN_LED_ON  GPIO_ResetBits(RUN_LED_GPIO_PORT,RUN_LED_PIN)// 运行LED
#define RUN_LED_OFF GPIO_SetBits(RUN_LED_GPIO_PORT,RUN_LED_PIN)
#define RUN_LED_TOG GPIO_ToggleBits(RUN_LED_GPIO_PORT,RUN_LED_PIN)





#define BEEP_ON     GPIO_SetBits(BEEP_GPIO_PORT,BEEP_PIN)
#define BEEP_OFF    GPIO_ResetBits(BEEP_GPIO_PORT,BEEP_PIN)

#define HARD_V2			(0x01)
#define HARD_V3			(0x02)
#define HARD_V4			(0x03)

#define HARD_V10		(0x10)// GD版本


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */



void LED_Init(void);
void IWDG_Init(void);
void beep_off(void);
void beep_on(void);
void Write_Dev_Type(uint8_t _flag);
void VerRead_Init(void);
u8 get_hardware_ver(void);
void AC_Check_Proc(void);











#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __APP_H__ */
