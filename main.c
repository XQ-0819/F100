/**
 *    文件名：Main.c
 *    说明：
 *    创建时间：  2013-04-08
 */

#include "userconfig.h"
#include "mytask.h"
#include "bsp.h"
#include "system_printf.h"
#include "parameters.h"
#include "encrypt.h"

static OS_STK startup_task_stk[TASK_STK_SIZE_M];

static void SystickInit(void)
{
    RCC_ClocksTypeDef rcc_clocks;
    RCC_GetClocksFreq(&rcc_clocks);
    SysTick_Config(rcc_clocks.HCLK_Frequency / OS_TICKS_PER_SEC);

}



static void StartupTask(void *p_arg)
{
    SystickInit();
#if (OS_TASK_STAT_EN>0)
    OSStatInit();   /*Determine CPU capacity. */
#endif

    BSP_Init();

    if (NoWrite_Check_ID() < 0) { //检测ID失败
        GUI_SetBkColor(GUI_RED);
        GUI_SetFont(&GUI_Font32_ASCII);
        GUI_DispStringAt("ERROR 004!", 336, 224);

        while (1) {
            delay_ms(1000);
            IWDG_ReloadCounter();
        }
    }

	Write_Dev_Type(STA_ON);
    IWDG_ReloadCounter();	
    Load_Param();
    Param_Init();
    Event_Create();
    Task_Create();
    PIC_Init();

    for (;;) {
        IWDG_ReloadCounter();
        RUN_LED_TOG;
// 屏保
        if (Get_LCD_State() == 0 && sys_sta.lcd_state == 1
                && basic_inf.ss_time != 0) {
            turn_off_terminal();
            sys_sta.lcd_state  = 0;
        } else if (Get_LCD_State() == 1 && sys_sta.lcd_state == 0) {
            turn_on_terminal();
            sys_sta.lcd_state  = 1;
        }

        LCD_State_SW();

        if (sys_sta.delay_rst != 0) {
            sys_sta.delay_rst--;

            if (sys_sta.delay_rst == 0) {
                SystemReset();
            }
        }

        Task_Time_Chk();
        his_data_save();

        OSTimeDly(1000);//1
//        DispTaskInfo();
    }
}

int main(void)

{
    INT8U os_err;

    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x20000);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);//中断
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//外部

    OSInit();
    IWDG_ReloadCounter();

    OSTaskCreateExt(StartupTask,    /* 启动任务函数指针 */
                    (void *)0,      /* 传递给任务的参数 */
                    (OS_STK *)&startup_task_stk[TASK_STK_SIZE_M - 1], /* 指向任务栈栈顶的指针 */
                    STARTUP_TASK_PRIO,  /* 任务的优先级，必须唯一，数字越低优先级越高 */
                    STARTUP_TASK_PRIO,  /* 任务ID，一般和任务优先级相同 */
                    (OS_STK *)&startup_task_stk[0],/* 指向任务栈栈底的指针。OS_STK_GROWTH 决定堆栈增长方向 */
                    TASK_STK_SIZE_M, /* 任务栈大小 */
                    (void *)0,  /* 一块用户内存区的指针，用于任务控制块TCB的扩展功能
                       （如任务切换时保存CPU浮点寄存器的数据）。一般不用，填0即可 */
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR); /* 任务选项字 */

    /*  定义如下：
        OS_TASK_OPT_STK_CHK      使能检测任务栈，统计任务栈已用的和未用的
        OS_TASK_OPT_STK_CLR      在创建任务时，清零任务栈
        OS_TASK_OPT_SAVE_FP      如果CPU有浮点寄存器，则在任务切换时保存浮点寄存器的内容
    */

    /* 指定任务的名称，用于调试。这个函数是可选的 */
    OSTaskNameSet(STARTUP_TASK_PRIO, "start task", &os_err);
    OSStart();
    return 0;
}


