/**
*    文件名：   MyTask.c
*    说明：  所有的任务都在这个文件里
*    创建时间： 2013-04-08
*/
#include "mytask.h"
#include "userconfig.h"
#include "tslib_calibration.h"
#include "menuinc.h"
#include "bsp.h"
#include "def.h"
#include "protocol.h"
#include "recode.h"
#include "energy.h"
#include "tslib_calibration.h"
#include "ads7843.h"
#include "trial_ver.h"



void UCGUI_Task(void * pdtat)
{
    u8 i = 0,time = 0;

    for (i = 0; i < 6; i++) {
    	if (get_touch_irq() == 0) {
			time++;
			if(time>3)break;
    	}
        OSTimeDlyHMSM(0, 0, 0, 500);
    }

    if (time > 3) {
        GUI_SetBkColor(GUI_RED);
        GUI_Clear();
        OSTimeDlyHMSM(0, 0, 2, 0);
        Calibration();
    }

    BK_Init();

    while (sys_sta.copy_home_pic == STA_OFF)
        OSTimeDly(100);

	if(basic_inf.home_style == HOME_CIRCLE && Language_Bit_original != LANGUAGE_ENGLISH && s_tCodeInf.wDay != 0){
	    Creat_Basic_Main2();
	}else{
	    Creat_Basic_Main();
	}

	MMain_SS_Task(0);
//	Main_Task(0);

    for (;;) {
		task_time[ucgui_id] = 0;
//        MYGUI_TOUCH_Exec();
        GUI_Exec();             //GUI事件更新
        OSTimeDly(10);
    }
}

extern void Cal_Day_Proc(void);
void Time_Polling_Task(void *pdata)
{

    for (;;) {
		task_time[time_polling_id] = 0;
        Time_Proc();
        DIN_Proc();
		Food_ReadIn();
        Pulse_ReadIn();
        USBH_Process(&USB_OTG_Core, &USB_Host);
        OSTimeDly(10);
    }
}

static u8 sensor_flag = 0;
void Sensor_Read_Task(void *pdata)
{
    u8 time = 4;
    u16 min_1 = 0;
    u16	min_10 = 0;

    for (;;) {
		task_time[sensor_read_id] = 0;
		AD7327_WIN_Proc();
        DIN_ReadIn();
        time++;
        min_1++;

        if (min_1 >= 250) {// 1min
            min_1 = 0;
            Cal_Day_Proc();
            min_10++;

            if (min_10 > 5) { // 5min
                min_10 = 0;
                Config_Save(SINDEX_ID);//记录运行时间
            }
        }
		
        Win_ReadIn();
        WIN_Porc();
		
        if (time >= 3) {
	        AD7327_Proc();
            time = 0;
            Sensor_ReadIn();
            Update_Aim();
			HumTemp_Cal();
            Update_ALA();
            sensor_flag = 1;
        }
		
        OSTimeDly(50);//不准，可能100ms
    }
}

void Control_Proc_Task(void *pdata)
{
    for (;;) {
		task_time[control_proc_id] = 0;
		
        if (sensor_flag == 1) {
            Update_Work();
            Work_Proc();
        }
		
        OSTimeDly(300);
    }
}


/*
    任务堆栈的创建

*/
__align(8) static OS_STK UCGUI_STK[TASK_STK_SIZE_B];
__align(8) static OS_STK Time_Polling_STK[TASK_STK_SIZE_L];
__align(8) static OS_STK Sensor_Read_STK[TASK_STK_SIZE_M];
__align(8) static OS_STK Control_Proc_STK[TASK_STK_SIZE_M];

__align(8) static OS_STK Comm_Rec_STK[TASK_STK_SIZE_M];
__align(8) static OS_STK Update_STK[TASK_STK_SIZE_M];
__align(8) static OS_STK Uart2_Rec_STK[TASK_STK_SIZE_M];

__align(8) static OS_STK Sensors_STK[TASK_STK_SIZE_M];
__align(8) static OS_STK Touch_STK[TASK_STK_SIZE_S];

void Event_Create(void)
{


}


void Task_Create(void)
{
    INT8U os_err;
    OSTaskCreateExt(Time_Polling_Task,  /* 启动任务函数指针 */
                    (void *)0,      /* 传递给任务的参数 */
                    (OS_STK *)&Time_Polling_STK[TASK_STK_SIZE_L - 1], /* 指向任务栈栈顶的指针 */
                    time_polling_prio,  /* 任务的优先级，必须唯一，数字越低优先级越高 */
                    time_polling_prio,  /* 任务ID，一般和任务优先级相同 */
                    (OS_STK *)&Time_Polling_STK[0],/* 指向任务栈栈底的指针。OS_STK_GROWTH 决定堆栈增长方向 */
                    TASK_STK_SIZE_L, /* 任务栈大小 */
                    (void *)0,  /* 一块用户内存区的指针，用于任务控制块TCB的扩展功能
                       （如任务切换时保存CPU浮点寄存器的数据）。一般不用，填0即可 */
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR); /* 任务选项字 */
    /* 指定任务的名称，用于调试。这个函数是可选的 */
    OSTaskNameSet(time_polling_prio, "time_polling", &os_err);


    OSTaskCreateExt(Sensor_Read_Task,  /* 启动任务函数指针 */
                    (void *)0,      /* 传递给任务的参数 */
                    (OS_STK *)&Sensor_Read_STK[TASK_STK_SIZE_M - 1], /* 指向任务栈栈顶的指针 */
                    sensor_read_prio,  /* 任务的优先级，必须唯一，数字越低优先级越高 */
                    sensor_read_prio,  /* 任务ID，一般和任务优先级相同 */
                    (OS_STK *)&Sensor_Read_STK[0],/* 指向任务栈栈底的指针。OS_STK_GROWTH 决定堆栈增长方向 */
                    TASK_STK_SIZE_M, /* 任务栈大小 */
                    (void *)0,  /* 一块用户内存区的指针，用于任务控制块TCB的扩展功能
                       （如任务切换时保存CPU浮点寄存器的数据）。一般不用，填0即可 */
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR); /* 任务选项字 */
    /* 指定任务的名称，用于调试。这个函数是可选的 */
    OSTaskNameSet(sensor_read_prio, "sensor_read", &os_err);


    OSTaskCreateExt(Control_Proc_Task,  /* 启动任务函数指针 */
                    (void *)0,      /* 传递给任务的参数 */
                    (OS_STK *)&Control_Proc_STK[TASK_STK_SIZE_M - 1], /* 指向任务栈栈顶的指针 */
                    control_proc_prio,  /* 任务的优先级，必须唯一，数字越低优先级越高 */
                    control_proc_prio,  /* 任务ID，一般和任务优先级相同 */
                    (OS_STK *)&Control_Proc_STK[0],/* 指向任务栈栈底的指针。OS_STK_GROWTH 决定堆栈增长方向 */
                    TASK_STK_SIZE_M, /* 任务栈大小 */
                    (void *)0,  /* 一块用户内存区的指针，用于任务控制块TCB的扩展功能
                       （如任务切换时保存CPU浮点寄存器的数据）。一般不用，填0即可 */
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR); /* 任务选项字 */
    /* 指定任务的名称，用于调试。这个函数是可选的 */
    OSTaskNameSet(control_proc_prio, "control_proc", &os_err);


    OSTaskCreateExt(UCGUI_Task, /* 启动任务函数指针 */
                    (void *)0,      /* 传递给任务的参数 */
                    (OS_STK *)&UCGUI_STK[TASK_STK_SIZE_B - 1], /* 指向任务栈栈顶的指针 */
                    ucgui_prio, /* 任务的优先级，必须唯一，数字越低优先级越高 */
                    ucgui_prio, /* 任务ID，一般和任务优先级相同 */
                    (OS_STK *)&UCGUI_STK[0],/* 指向任务栈栈底的指针。OS_STK_GROWTH 决定堆栈增长方向 */
                    TASK_STK_SIZE_B, /* 任务栈大小 */
                    (void *)0,  /* 一块用户内存区的指针，用于任务控制块TCB的扩展功能
                       （如任务切换时保存CPU浮点寄存器的数据）。一般不用，填0即可 */
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR); /* 任务选项字 */
    /* 指定任务的名称，用于调试。这个函数是可选的 */
    OSTaskNameSet(ucgui_prio, "ucgui_task", &os_err);

    OSTaskCreateExt(Comm_Rec_Task,  /* 启动任务函数指针 */
                    (void *)0,      /* 传递给任务的参数 */
                    (OS_STK *)&Comm_Rec_STK[TASK_STK_SIZE_M - 1], /* 指向任务栈栈顶的指针 */
                    Comm_rec_prio,    /* 任务的优先级，必须唯一，数字越低优先级越高 */
                    Comm_rec_prio,    /* 任务ID，一般和任务优先级相同 */
                    (OS_STK *)&Comm_Rec_STK[0],/* 指向任务栈栈底的指针。OS_STK_GROWTH 决定堆栈增长方向 */
                    TASK_STK_SIZE_M, /* 任务栈大小 */
                    (void *)0,  /* 一块用户内存区的指针，用于任务控制块TCB的扩展功能
                       （如任务切换时保存CPU浮点寄存器的数据）。一般不用，填0即可 */
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR); /* 任务选项字 */
    /* 指定任务的名称，用于调试。这个函数是可选的 */
    OSTaskNameSet(Comm_rec_prio, "Comm Rec", &os_err);


    OSTaskCreateExt(Update_Task,    /* 启动任务函数指针 */
                    (void *)0,        /* 传递给任务的参数 */
                    (OS_STK *)&Update_STK[TASK_STK_SIZE_M - 1], /* 指向任务栈栈顶的指针 */
                    Update_prio, /* 任务的优先级，必须唯一，数字越低优先级越高 */
                    Update_prio, /* 任务ID，一般和任务优先级相同 */
                    (OS_STK *)&Update_STK[0],/* 指向任务栈栈底的指针。OS_STK_GROWTH 决定堆栈增长方向 */
                    TASK_STK_SIZE_M, /* 任务栈大小 */
                    (void *)0,    /* 一块用户内存区的指针，用于任务控制块TCB的扩展功能
                       （如任务切换时保存CPU浮点寄存器的数据）。一般不用，填0即可 */
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR); /* 任务选项字 */

    /*  定义如下：
      OS_TASK_OPT_STK_CHK      使能检测任务栈，统计任务栈已用的和未用的
      OS_TASK_OPT_STK_CLR      在创建任务时，清零任务栈
      OS_TASK_OPT_SAVE_FP      如果CPU有浮点寄存器，则在任务切换时保存浮点寄存器的内容
    */

    /* 指定任务的名称，用于调试。这个函数是可选的 */
    OSTaskNameSet(Update_prio, "Update proc", &os_err);




    OSTaskCreateExt(Uart2_Rec_Task,  /* 启动任务函数指针 */
                    (void *)0,      /* 传递给任务的参数 */
                    (OS_STK *)&Uart2_Rec_STK[TASK_STK_SIZE_M - 1], /* 指向任务栈栈顶的指针 */
                    Uart2_rec_prio,    /* 任务的优先级，必须唯一，数字越低优先级越高 */
                    Uart2_rec_prio,    /* 任务ID，一般和任务优先级相同 */
                    (OS_STK *)&Uart2_Rec_STK[0],/* 指向任务栈栈底的指针。OS_STK_GROWTH 决定堆栈增长方向 */
                    TASK_STK_SIZE_M, /* 任务栈大小 */
                    (void *)0,  /* 一块用户内存区的指针，用于任务控制块TCB的扩展功能
                       （如任务切换时保存CPU浮点寄存器的数据）。一般不用，填0即可 */
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR); /* 任务选项字 */
    /* 指定任务的名称，用于调试。这个函数是可选的 */
    OSTaskNameSet(Uart2_rec_prio, "Uart2 Rec", &os_err);


    OSTaskCreateExt(Sensors_Task,  /* 启动任务函数指针 */
                    (void *)0,      /* 传递给任务的参数 */
                    (OS_STK *)&Sensors_STK[TASK_STK_SIZE_M - 1], /* 指向任务栈栈顶的指针 */
                    Sensors_prio,    /* 任务的优先级，必须唯一，数字越低优先级越高 */
                    Sensors_prio,    /* 任务ID，一般和任务优先级相同 */
                    (OS_STK *)&Sensors_STK[0],/* 指向任务栈栈底的指针。OS_STK_GROWTH 决定堆栈增长方向 */
                    TASK_STK_SIZE_M, /* 任务栈大小 */
                    (void *)0,  /* 一块用户内存区的指针，用于任务控制块TCB的扩展功能
                       （如任务切换时保存CPU浮点寄存器的数据）。一般不用，填0即可 */
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR); /* 任务选项字 */
    /* 指定任务的名称，用于调试。这个函数是可选的 */
    OSTaskNameSet(Sensors_prio, "Sensors", &os_err);

    OSTaskCreateExt(Touch_Task,  /* 启动任务函数指针 */
                    (void *)0,      /* 传递给任务的参数 */
                    (OS_STK *)&Touch_STK[TASK_STK_SIZE_S- 1], /* 指向任务栈栈顶的指针 */
                    touch_prio,    /* 任务的优先级，必须唯一，数字越低优先级越高 */
                    touch_prio,    /* 任务ID，一般和任务优先级相同 */
                    (OS_STK *)&Touch_STK[0],/* 指向任务栈栈底的指针。OS_STK_GROWTH 决定堆栈增长方向 */
                    TASK_STK_SIZE_S, /* 任务栈大小 */
                    (void *)0,  /* 一块用户内存区的指针，用于任务控制块TCB的扩展功能
                       （如任务切换时保存CPU浮点寄存器的数据）。一般不用，填0即可 */
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR); /* 任务选项字 */
    /* 指定任务的名称，用于调试。这个函数是可选的 */
    OSTaskNameSet(touch_prio, "Touch", &os_err);		
}



