#ifndef TRIAL_VER_H
#define TRIAL_VER_H
#include "stm32f4xx.h"

#define CODE_KEY_NUM			(3)

#define TRIAL_SHOW_DAY			(10)

typedef struct
{
	uint16_t wMagic;		
	uint16_t wDay;			//倒计时天
	int		 dUsedHour;	    //已用小时	
	
}CODE_T;


extern CODE_T s_tCodeInf;



void Trial_Clear(uint16_t _wDay);
int Get_Trial_ReHour(void);
void Trial_Ver_Init(void);
void Trial_Sec_Proc(void);
int Trial_Key_Check(unsigned char *input_key);
void Trial_Time_Save(void);


#endif

