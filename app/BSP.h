
/*****************************************************************************/
#ifndef __BSP_H__
#define __BSP_H__
#include "stm32f4xx.h"


// 采集错误值

#define TEMP_ER_VALUE		(999)
#define SENSOR_ER_VALUE		(0xFFFF)
#define VAC_ER_VALUE		(999)
#define LIGHT_ER_VALUE		(-1)
#define DATA_ER_VALUE		(-1)

#define TEMP_MAX_ERR		(450)
#define TEMP_MIN_ERR		(0)
#define TEMP_OUTMAX_ERR		(990)
#define TEMP_OUTMIN_ERR		(-400)

//继电器功能
#define  JDQ_FAN1           (0)
#define  JDQ_FAN2           (1)
#define  JDQ_FAN3           (2)
#define  JDQ_FAN4           (3)
#define  JDQ_FAN5           (4)
#define  JDQ_FAN6           (5)
#define  JDQ_FAN7           (6)
#define  JDQ_FAN8           (7)
#define  JDQ_FAN9           (8)//共用
#define  JDQ_FAN10          (9)//共用
#define  JDQ_FAN11          (10)//共用
#define  JDQ_FAN12          (11)//共用
#define  JDQ_FAN13          (12)//共用
#define  JDQ_FAN14          (13)//共用
#define  JDQ_FAN15          (14)//共用
#define  JDQ_FAN16          (15)//共用
#define  JDQ_FAN17          (16)//共用
#define  JDQ_FAN18          (17)//共用
#define  JDQ_FAN19          (18)//共用
#define  JDQ_WET            (8)
#define  JDQ_WET2           (9)//共用
#define  JDQ_SPRAY          (9)
#define  JDQ_CIR_FAN        (10)
#define  JDQ_TIMER1         (10)
#define  JDQ_TIMER2         (11)
#define  JDQ_TEMPC          (11)//共用
#define  JDQ_TIMER3         (12)
#define  JDQ_TIMER4         (13)
#define  JDQ_HEAT1          (12)
#define  JDQ_HEAT2          (13)
#define  JDQ_HEAT3          (14)
#define  JDQ_WIN1_UP        (4) //共用
#define  JDQ_WIN1_DOWN      (5) //共用
#define  JDQ_WIN2_UP        (6) //共用
#define  JDQ_WIN2_DOWN      (7) //共用
#define  JDQ_WIN3_UP        (15) //共用
#define  JDQ_WIN3_DOWN      (16) //共用
#define  JDQ_WIN4_UP        (17) //共用
#define  JDQ_WIN4_DOWN      (18) //共用
#define  JDQ_ALA         	(19)

//数字量输入功能

#define  DIN_WATER          (0)
#define  DIN_FOOD           (1)
#define  DIN_ALA          	(2)
#define  DIN_WATER2         (3)
#define  DIN_WIN1_UP        (3)
#define  DIN_WIN1_DOWN      (4)
#define  DIN_WIN2_UP        (5)
#define  DIN_WIN2_DOWN      (6)

//模拟量输出功能
#define  PWM_FAN1           (0)
#define  PWM_FAN2           (1)
#define  PWM_FAN3           (2)
#define  APWM_FAN1          (0)//复用
#define  APWM_FAN2          (1)//复用
#define  APWM_FAN3          (2)//复用
#define  PWM_HEAT           (3)//复用
#define  PWM_FAN4           (3)
#define  PWM_TEMPC          (4)
#define  PWM_LIGHT          (5)
#define  PWM_SWIN1          (0)//复用
#define  PWM_SWIN2          (1)//复用
#define  PWM_SWIN3          (2)//复用
#define  PWM_SWIN4          (3)//复用
#define  PWM_SWIN5          (4)//复用
#define  PWM_SWIN6          (5)//复用


//模拟量功能
#define  AIN_TEMP1           (0)
#define  AIN_TEMP2           (1)
#define  AIN_TEMP3           (2)
#define  AIN_TEMP4           (3)
#define  AIN_TEMP5           (4)
#define  AIN_TEMP6           (5)
#define  AIN_TEMP7           (6)
#define  AIN_AI1          	 (11)
#define  AIN_AI2           	 (10)
#define  AIN_AI3           	 (8)
#define  AIN_AI4          	 (9)
#define  AIN_AI5             (7)
#define  AIN_AI6	         (12)
//#define  AIN_VAC          	 (7)
//#define  AIN_NH3           	 (8)
//#define  AIN_CO2           	 (9)
//#define  AIN_HUM          	 (10)
//#define  AIN_TEMP8           (11)
//#define  AIN_H2S	         (12)
#define  AIN_WIN1          	 (14)
#define  AIN_WIN2            (13)
#define  AIN_WIN3          	 (AIN_AI5)//复用
#define  AIN_WIN4            (AIN_AI6)//复用



#define MAX_TASK_NUM		(6)
#define time_polling_id 	(0)
#define ucgui_id 			(1)
#define control_proc_id 	(2)
#define sensor_read_id 		(3)
#define Update_id 			(4)
#define Sensors_id  		(5)

extern unsigned short task_time[MAX_TASK_NUM];
extern unsigned short task_time_max[MAX_TASK_NUM];
void Task_Time_Chk(void);
	
void SystemReset(void);
void PIC_Init(void);
void BSP_Init(void);
void turn_off_terminal(void);
void turn_on_terminal(void);
void Win_ReadIn(void);
void Sensor_ReadIn(void);
void DIN_ReadIn(void);
void Pulse_ReadIn(void);
void Food_ReadIn(void);
void Update_Work(void);
void Run_Time_Proc(void);
void Work_Proc(void);
void Update_Aim(void);
void ADJ_DISFAN(u8 sid, u8 dir);
void ADJ_ENFAN(u8 sid);
int WIN_UP_CON(u8 id , u8 status);
int WIN_DOWN_CON(u8 id , u8 status);
void Second_Proc(void);
void HumTemp_Cal(void);
void WIN_Porc(void);
int get_swin_vac_control(u8 id, u8 lv_id);
int get_win_vac_control(u8 id, u8 lv_id);
int get_max_win_run_time(u8 lv_id);
void check_lv_save(void);
int WIN_Get_Pos(u8 i);
int get_sensor_sta(u8 i);
void Win_Time_Proc(void);


#endif /* __BSP_H__ */


