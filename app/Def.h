/*****************************************************************************/
#ifndef __DEF_H__
#define __DEF_H__
#include "stm32f4xx.h"




#define SYS_VER			(312)
#define SYS_VER2		(8)









#define FONT_SDRAM		(1)

//#define TEMP_DEF		(0)
//#define TEMP_BASE_DIF	(50)

#define PWD_NO_KEY		("2014")	

#define DIFF_SECOND		(180)	// 120->45 for database exception

#define WIN_RUN_FREQ	(10)//按100ms计时

#define RUN_FREQ		(1)//按秒计时
#define JDQ_IDE_TIME	(3)
#define JDQ_RUN_MIN		(3)
#define PWM_RUN_MIN		(3)
#define TEMPC_RUN_MIN	(3)
#define WET_RUN_MIN		(3)
#define HEAT_RUN_MIN	(3)
#define SPRAY_RUN_MIN	(3)
#define TIMER_RUN_MIN	(3)

#define ALA_SHOW_NUM	(3)

#define TEMP_4MA		(-400)		
#define TEMP_20MA		(800)		
#define HUM_4MA			(0)		
#define HUM_20MA		(100)		
#define CO2_4MA			(0)		
#define CO2_20MA		(5000)
#define NH3_4MA			(0)		
#define NH3_20MA		(100)
#define H2S_4MA			(0)		
#define H2S_20MA		(100)
#define VAC_4MA			(-100)		
#define VAC_20MA		(100)
#define SPEED_4MA		(0)		
#define SPEED_20MA		(500)



#endif 

