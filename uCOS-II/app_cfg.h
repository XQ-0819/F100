/*
********************************************************************************************************
*                                          All Rights Reserved
* Version   : V1.00
* Data      : 2012-08-18
* By        : yang
* For       : ARMv7M Cortex-M4
* Toolchain : RealView Development Suite
*             RealView Microcontroller Development Kit (MDK)
*             ARM Developer Suite (ADS)
*             Keil uVision
*********************************************************************************************************
*/
#ifndef APP_CFG_H
#define APP_CFG_H
//主要任务是定义任务的优先级和堆栈大小，中断优先级等信息

/* task priority */ 
#define STARTUP_TASK_PRIO                     50


/* task stack size */ 
#define STARTUP_TASK_STK_SIZE                  512  


#define TASK_STK_SIZE_S  128
#define TASK_STK_SIZE_M  256
#define TASK_STK_SIZE_L  512
#define TASK_STK_SIZE_B  1024
#endif
