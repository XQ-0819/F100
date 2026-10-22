#include "system_printf.h"
#include <stdio.h>
#include "ucos_ii.h"
#include "gui.h"
#include "usart.h"


/*
*********************************************************************************************************
*	函 数 名: DispTaskInfo
*	功能说明: 将uCOS-II任务信息打印到串口
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void DispTaskInfo(void)
{
	OS_TCB      *ptcb;	        /* 定义一个任务控制块指针, TCB = TASK CONTROL BLOCK */
	OS_STK_DATA stk;	        /* 用于读取任务栈使用情况 */

	ptcb    = &OSTCBTbl[0];		/* 指向第1个任务控制块(TCB) */
	
	USART_SendData(USARTx, 0x0c);//清显示

	SendUartStrX(UART1_CON,"CPU: %d%%\r\n", OSCPUUsage);
	SendUartStrX(UART1_CON,"emWin: used:%uKB/4MB    free:%uKB/4MB\r\n", (int)(GUI_ALLOC_GetNumUsedBytes()>>10),(int)(GUI_ALLOC_GetNumFreeBytes()>>10));
	
	/* 打印标题 */
	SendUartStrX(UART1_CON,"  Prio   Used  Free  Per    Taskname\r\n");

	/* 遍历任务控制块列表(TCB list)，打印所有的任务的优先级和名称 */
	while (ptcb != NULL)
	{
		/* 
			ptcb->OSTCBPrio : 任务控制块中保存的任务的优先级
			ptcb->OSTCBTaskName : 任务控制块中保存的任务名称，需要在创建任务的时候
			调用OSTaskNameSet()指定任务名称，比如：
			OSTaskNameSet(APP_TASK_USER_IF_PRIO, "User I/F", &err);
		*/
		OSTaskStkChk(ptcb->OSTCBPrio, &stk);	/* 获得任务栈已用空间 */
		SendUartStrX(UART1_CON,"   %2d  %5d  %5d  %02d%%   %s\r\n", ptcb->OSTCBPrio, 
			stk.OSUsed, stk.OSFree, (stk.OSUsed * 100) / (stk.OSUsed + stk.OSFree),
			ptcb->OSTCBTaskName);		
		ptcb = ptcb->OSTCBPrev;		            /* 指向上一个任务控制块 */
	}
}

