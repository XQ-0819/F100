#ifndef _MYTASK_H
#define _MYTASK_H
#include "stm32f4xx.h"
#include "ucos_ii.h"

// ******************************************************************************
// * 以下为需要的全局变量，其他工程可删除



// ******************************************************************************

//任务优先级
#define time_polling_prio 	(14)
#define ucgui_prio 			(13)
#define control_proc_prio 	(17)
#define sensor_read_prio 	(18)

#define Comm_rec_prio (19)
#define Update_prio (21)
#define Uart2_rec_prio (22)
#define Uart3_rec_prio (24)
#define Sensors_prio  (23)

#define Uart2_time_prio (30)
#define touch_prio  		4




void Event_Create(void);
void Task_Create(void);

#endif

