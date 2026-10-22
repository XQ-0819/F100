/*
******************************************************************
**                      uCGUIBuilder                            **
**                  Version:   4.0.0.0                          **
**                     2012 / 04                               **
**                   CpoyRight to: wyl                          **
**              Email:ucguibuilder@163.com                        **
**          This text was Created by uCGUIBuilder               **
******************************************************************/

#include "menuinc.h"
#if USE_VS == 0
#include "datasave.h"
#include "ucos_ii.h"
#include "mytask.h"
#include "din.h"
#include <string.h>
#include "ad7327.h"
#include "jdq.h"
#include "pwm_out.h"
#include "usart.h"
#include "protocol.h"
#include "PCF8563.h"
#include "Common.h"
#endif


static WM_HTIMER hTSTimer;

//EventsFunctionList
//EndofEventsFunctionList
/*********************************************************************
*
*       static data
*
**********************************************************************
*/
/*********************************************************************
*
*       Dialog resource
*
* This table conatins the info required to create the dialog.
* It has been created by ucGUIbuilder.
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect,    "",               0,                       0,  0,  800, 480, 0, 0},
    { TEXT_CreateIndirect,      "硬件测试",          GUI_ID_TEXT0,            250, 4, 300, 42, 0, 0},

    { TEXT_CreateIndirect,      "EEPROM:正在测试",          GUI_ID_TEXT1,            150, 60, 200, 24, 0, 0},
    { TEXT_CreateIndirect,      "",               			GUI_ID_TEXT2,            150, 90, 560, 50, 0, 0},
    { TEXT_CreateIndirect,      "",               			GUI_ID_TEXT3,            150, 145, 560, 130, 0, 0},
    { TEXT_CreateIndirect,      "",               			GUI_ID_TEXT4,            150, 280, 560, 24, 0, 0},
    { TEXT_CreateIndirect,      "",               			GUI_ID_TEXT5,            150, 310, 560, 50, 0, 0},
    { TEXT_CreateIndirect,      "RS485:正在测试",           GUI_ID_TEXT6,            150, 370, 500, 24, 0, 0},
    { TEXT_CreateIndirect,      "",          GUI_ID_TEXT7,            400, 60, 260, 24, 0, 0},


//    { BUTTON_CreateIndirect,    "上一页",          GUI_ID_BUTTON0,          MENU_BACK_X1, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
//    { BUTTON_CreateIndirect,    "下一页",          GUI_ID_BUTTON1,          MENU_BACK_X2, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
    { BUTTON_CreateIndirect,    "返回",             GUI_ID_BUTTON2,         MENU_BACK_X3, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
};
/*****************************************************************
**      FunctionName:void PaintDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**
**      call this function in _cbCallback --> WM_PAINT
*****************************************************************/

static void PaintDialog(WM_MESSAGE * pMsg)
{
    Show_MBackPic();

    GUI_SetColor(GUI_LINE_COLOR);
    GUI_DrawRoundedRect(120, 50, 670, 400, 15);
    GUI_DrawRoundedRect(117, 47, 673, 403, 15);
}
static U8 eeprom_ts = 0xFF;
static U8 rs485_ts = 0;
typedef struct {
    unsigned char	mode;	//模式
    unsigned char	id;		//序号
} TEST;
static TEST jdq_test;
static TEST pwm_test;
static int jdq_test_on = 0;
static void Show_TS_Result(WM_HWIN hWin)
{
    U8 i = 0;
    char temp[24];
    time_t sys_time = GetSysTime();
    struct tm *t;

    t = localtime((const time_t *)&sys_time);
    sprintf(show_buf, "%02d/%02d/%02d %02d:%02d:%02d", (t->tm_year - 100), (t->tm_mon + 1), t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT7), (const char *) show_buf);


    if (eeprom_ts == 1) {
        TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT1), "EEPROM: OK");
    } else if (eeprom_ts == 0) {
        TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT1), "EEPROM: ERROR");
    }

    if (rs485_ts == 0) {
        TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT6), "RS485: OK");
    } else if (rs485_ts == 0xff) {
        TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT6), "RS485: Testing");
    } else {
        sprintf(show_buf, "RS485: ERROR");

        if (getbit(rs485_ts, 0) == 1 ) {
            strcat(show_buf, " 2;  ");
        }

        if (getbit(rs485_ts, 1) == 1 ) {
            strcat(show_buf, " IN;  ");
        }

        if (getbit(rs485_ts, 2) == 1 ) {
            strcat(show_buf, " 1&3;  ");
        }

        TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT6), show_buf);
    }


    JDQ_Control(jdq_test.id, jdq_test.mode);
    JDQ_Control_Hard();

    sprintf(show_buf, "%s%d:", GUI_LANG_GetTextEx(Lang_Relay, Language_Bit), (jdq_test.id + 1));

    if (jdq_test.mode == 0) {
        strcat(show_buf, GUI_LANG_GetTextEx(Lang_Close, Language_Bit));

    } else {
        strcat(show_buf, GUI_LANG_GetTextEx(Lang_Open, Language_Bit));
    }

    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT4), show_buf);


    jdq_test.id = (jdq_test.id + (jdq_test.mode % 2));

    if (jdq_test.id >= ALLJDQ_NUM_MAX) {
        jdq_test_on = 1;
    }

    jdq_test.id %= ALLJDQ_NUM_MAX;

    if (jdq_test_on == 1) {
        jdq_test.mode = 1;
    } else {
        jdq_test.mode = (jdq_test.mode + 1) % 2;
    }

    sprintf(show_buf, "%s:", GUI_LANG_GetTextEx(Lang_Analogy, Language_Bit));

    for (i = 0; i < ALLPWM_NUM_MAX; i++) {
        PWM_Out(i, (pwm_test.mode + 25 * i) % 125);
        sprintf(temp, "%d:%d%%   ", (i + 1), ((pwm_test.mode + 25 * i) % 125));
        strcat(show_buf, temp);

        if (i == 2) {
            strcat(show_buf, "\n");
        }
    }

    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT5), show_buf);

    pwm_test.mode = (pwm_test.mode + 25) % 125;



    show_buf[0] = 0;

    for (i = 0; i < 7; i++) {
        if (Get_DIN_State(i) == DIN_OFF) {
            sprintf(temp, "DIN%d:%s   ", (i + 1), GUI_LANG_GetTextEx(Lang_Close, Language_Bit));
        } else {
            sprintf(temp, "DIN%d:%s   ", (i + 1), GUI_LANG_GetTextEx(Lang_Open, Language_Bit));
        }

        strcat(show_buf, temp);

        if (i == 2) {
            strcat(show_buf, "\n");
        }
    }

    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT2), show_buf);


    show_buf[0] = 0;

    for (i = 0; i < TEMP_NUM_MAX; i++) {
        sprintf(temp, "T%d:%.1f   ", (i + 1), (float)(sta_in.sensor_real.TEMP[i] * 1.0f / TEMP_RATE));
        strcat(show_buf, temp);

        if ((i + 1) % 4 == 0) {
            strcat(show_buf, "\n");
        }
    }

    strcat(show_buf, "\n");

    for (i = 0; i < SENSOR_NUM_MAX; i++) {
        sprintf(temp, "AI%d:%d   ", (i + 1), sta_in.sensor_real.SENSOR[i]);
        strcat(show_buf, temp);

        if (i == 4) {
            strcat(show_buf, "\n");
        }
    }

    sprintf(temp, "WIN3:%.2fV        ", (sta_in.sensor_real.WIN[0] / 100.0));
    strcat(show_buf, temp);
    sprintf(temp, "WIN4:%.2fV ", (sta_in.sensor_real.WIN[1] / 100.0));
    strcat(show_buf, temp);

    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT3), show_buf);
}
/*****************************************************************
**      FunctionName:void InitDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**
**      call this function in _cbCallback --> WM_INIT_DIALOG
*****************************************************************/
static void InitDialog(WM_MESSAGE * pMsg)
{
    WM_HWIN hWin = pMsg->hWin;

    int MDev_ID = 0;
    WM_HWIN hItem;
    //
    //GUI_ID_BUTTON0
    //


    for (MDev_ID = GUI_ID_BUTTON2; MDev_ID <= GUI_ID_BUTTON2; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, _cbButtonBack);
    }

    for (MDev_ID = GUI_ID_TEXT0; MDev_ID <= GUI_ID_TEXT7; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24L(hItem);

        if (MDev_ID == GUI_ID_TEXT0) {
            TEXT_SetTextAlign(hItem,  GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetTextColor(hItem, GUI_WHITE);
        }
    }

    hTSTimer = WM_CreateTimer(WM_GetClientWindow(hWin), 0, 50, 0);

    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT0), GUI_LANG_GetTextEx(Lang_Hardware_Test, Language_Bit)); // 硬件测试
    sprintf(show_buf, "EEPROM:%s", GUI_LANG_GetTextEx(Lang_Testing, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT1), show_buf); // EEPROM:正在测试
    sprintf(show_buf, "RS485:%s", GUI_LANG_GetTextEx(Lang_Testing, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT6), show_buf); // RS485:正在测试
}



extern BYTE localRxBuf[BUF_SIZE_2K];
/*********************************************************************
*
*       Dialog callback routine
*/
static void _cbCallback(WM_MESSAGE * pMsg)
{
    int NCode, Id;
    WM_HWIN hWin = pMsg->hWin;

    switch (pMsg->MsgId) {
    case WM_PAINT:
        PaintDialog(pMsg);
        break;

    case WM_TIMER:
        if (eeprom_ts == 0xff) {
            eeprom_ts = EEPEOM_ALL_TEST();
        }

        Show_TS_Result(hWin);
        WM_RestartTimer(hTSTimer, 1000);
        break;

    case WM_INIT_DIALOG:
        InitDialog(pMsg);
        break;

    case WM_KEY:
        switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) {
        case GUI_KEY_ESCAPE:
            GUI_EndDialog(hWin, 1);
            break;

        case GUI_KEY_ENTER:
            GUI_EndDialog(hWin, 0);
            break;
        }

        break;

    case WM_NOTIFY_PARENT:
        Id = WM_GetId(pMsg->hWinSrc);
        NCode = pMsg->Data.v;

        if (NCode == WM_NOTIFICATION_CLICKED) {
            beep_on();
        }

        switch (Id) {
        case GUI_ID_OK:
            if (NCode == WM_NOTIFICATION_RELEASED)
                GUI_EndDialog(hWin, 0);

            break;

        case GUI_ID_CANCEL:
            if (NCode == WM_NOTIFICATION_RELEASED)
                GUI_EndDialog(hWin, 0);

            break;

        case GUI_ID_BUTTON2:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                UartInit(UART3_SENSOR, 0x03, UDATA_8B, PARITY_NONE, STOP_BIT1); //9600
                UartInit(UART2_HMI, 0x03, UDATA_8B, PARITY_NONE, STOP_BIT1); //9600
                Packet_Init();
                OSTaskResume(Sensors_prio);
                OSTaskResume(Update_prio);
                OSTaskResume(control_proc_prio);
                OSTaskResume(Uart2_rec_prio);
                OSTaskDel(Uart3_rec_prio);
                OSTaskDel(Uart2_time_prio);
                sys_sta.auto_mode = STA_ON;
                Current_HWIN = WM_GetParent(hWin);
                GUI_EndDialog(hWin, 0);
            }

            break;
        }

        break;

    default:
        WM_DefaultProc(pMsg);
    }
}

__align(8) static OS_STK Uart3_Rec_STK[TASK_STK_SIZE_M];
__align(8) static OS_STK Uart2_Time_STK[TASK_STK_SIZE_M];
static BYTE UART3Test[BUF_SIZE_256];
int UART3bytes;

void Uart3_Rec_Task(void * pdata)
{
    for (;;) {
        WaitUartIdle( UART3_SENSOR, 0 );
        UART3bytes = GetUartData( UART3_SENSOR, UART3Test, sizeof(UART3Test) );
        SendUartData( UART3_SENSOR, UART3Test, UART3bytes );
        OSTimeDlyHMSM(0, 0, 0, 10);
    }
}

static signed char Read_Time_Cmd(unsigned char *Ethernet_Recv_Buf, unsigned short Ethernet_Recv_len)
{

    unsigned short Crc_Data = 0;
    Packet_Head Respon_Buffer;
    unsigned char rec_result = 0;
    TDATE_TIME	time_net;
    struct tm *t;
    time_t sys_time = GetSysTime();
    t = localtime((const time_t *)&sys_time);

    if ((Ethernet_Recv_Buf[0] != 0xA5) || (Ethernet_Recv_Buf[1] != 0X5A)
            || (Ethernet_Recv_Buf[Ethernet_Recv_len - 2] != 0XA5)
            || (Ethernet_Recv_Buf[Ethernet_Recv_len - 1] != 0X0D))
        return -1;

    Crc_Data = crc16_s(Ethernet_Recv_Buf, Ethernet_Recv_len - 4);

    if (Crc_Data != (Ethernet_Recv_Buf[Ethernet_Recv_len - 3] << 8) + Ethernet_Recv_Buf[Ethernet_Recv_len - 4] )
        return -2;

    if (basic_inf.id != (Ethernet_Recv_Buf[3] + (Ethernet_Recv_Buf[4] << 8))) {
        return -3;
    }

    if (HEART_BEAT_DATA != Ethernet_Recv_Buf[2])
        return -4;

    memcpy(&Respon_Buffer.Head1, &Ethernet_Recv_Buf[0], SRART_BUF_LEN);

    rec_result = Ethernet_Recv_Buf[Ethernet_Recv_len - 5];

    if (rec_result == 0x00) {

        time_net.year = Ethernet_Recv_Buf[SRART_BUF_LEN];
        time_net.mon = Ethernet_Recv_Buf[SRART_BUF_LEN + 1];
        time_net.day = Ethernet_Recv_Buf[SRART_BUF_LEN + 2];
        time_net.hour = (Ethernet_Recv_Buf[SRART_BUF_LEN + 3]);
        time_net.min = Ethernet_Recv_Buf[SRART_BUF_LEN + 4];
        time_net.sec = Ethernet_Recv_Buf[SRART_BUF_LEN + 5];
        //time_net.week=Ethernet_Recv_Buf[SRART_BUF_LEN+6];

        if (time_net.min != t->tm_min ||
                (time_net.hour !=  t->tm_hour ) ||
                (time_net.day !=  t->tm_mday ) ||
                (time_net.mon !=  (t->tm_mon + 1)) ||
                (time_net.year !=  (t->tm_year - 100))) {

            if ((time_net.min <= 0x60) &&
                    (time_net.hour <= 0x24) &&
                    (time_net.day <= 0x31) &&
                    (time_net.mon <= 0x12) &&
                    (time_net.sec <= 0x60)) {

                PCF8563_SetDT( &time_net );

                OSTimeDly(1000);
            }
        }
    }

    return 1;
}

static BYTE UART2Test[BUF_SIZE_256];
void Uart2_Time_Task(void * pdata)
{
    int bytes, err;

    for (;;) {

        Packet_Buffer.Result = 0x00;
        Packet_Buffer.Send_Seq = 0x00;
        Report_Data_Buffer(Current_Data_Buf, 0, HEART_BEAT_DATA);

        ClearUartIdle(UART2_HMI);
        GetUartData( UART2_HMI, UART2Test, sizeof(UART2Test));
        SendUartData(UART2_HMI, Data_Send_Buf, Data_Send_Num);

        err = WaitUartIdle(UART2_HMI, 1000);

        if (err == OS_ERR_NONE) { //????
            bytes = GetUartData( UART2_HMI, UART2Test, sizeof(UART2Test));

            if (bytes > 2) {

                if (Read_Time_Cmd(UART2Test, bytes) > 0) {
                    clrbit(rs485_ts, 0);
                } else {
                    setbit(rs485_ts, 0);
                }
            }
        } else {
            setbit(rs485_ts, 0);
        }

        OSTimeDlyHMSM(0, 0, 1, 0);


        Report_Data_Buffer(Current_Data_Buf, 0, HEART_BEAT_DATA);

#if USE_GD == 1
        ClearUartIdle(UART3_SENSOR);
        UART3bytes = 0;
        GetUartData( UART3_SENSOR, UART2Test, sizeof(UART2Test));
        SendUartData(UART3_SENSOR, Data_Send_Buf, Data_Send_Num);

        OSTimeDlyHMSM(0, 0, 1, 0);

        if (UART3bytes > 2) {

            if (Read_Time_Cmd(UART3Test, UART3bytes) > 0) {
                clrbit(rs485_ts, 1);
            } else {
                setbit(rs485_ts, 1);
            }
        } else {
            setbit(rs485_ts, 1);
        }

#else
        ClearUartIdle(UART1_CON);
        GetUartData( UART1_CON, UART2Test, sizeof(UART2Test));
        SendUartData(UART1_CON, Data_Send_Buf, Data_Send_Num);

        err = WaitUartIdle(UART1_CON, 1000);

        if (err == OS_ERR_NONE) { //????
            bytes = GetUartData( UART1_CON, UART2Test, sizeof(UART2Test));

            if (bytes > 2) {

                if (Read_Time_Cmd(UART2Test, bytes) > 0) {
                    clrbit(rs485_ts, 1);
                } else {
                    setbit(rs485_ts, 1);
                }
            }
        } else {
            setbit(rs485_ts, 1);
        }

#endif

        OSTimeDlyHMSM(0, 0, 1, 0);
        ClearUartIdle(UART6_COMM);
        memset(localRxBuf, 0, 20);
        SendUartData(UART6_COMM, "1234567890", 10);

        OSTimeDlyHMSM(0, 0, 1, 0);


        if (strcmp("1234567890", (const char *)localRxBuf) == 0) {
            clrbit(rs485_ts, 2);
        } else {
            setbit(rs485_ts, 2);
        }


        OSTimeDlyHMSM(0, 0, 1, 0);

    }
}

/*********************************************************************
*
*       MainTask
*
**********************************************************************
*/
WM_HWIN MSys_HardTS;
void MSys_HardTS_Task(WM_HWIN hParent)
{
    INT8U os_err;

    jdq_test_on = 0;

    while (get_jdq_ctrl_state() == STA_ON) {
        OSTimeDly(1);//等待继电器操作完成
    };

    OSTaskSuspend(control_proc_prio);

    OSTaskSuspend(Sensors_prio);

    OSTaskSuspend(Update_prio);

    OSTaskSuspend(Uart2_rec_prio);    //挂起串口2任务

    UartInit(UART3_SENSOR, 0x07, UDATA_8B, PARITY_NONE, STOP_BIT1); //115200

    UartInit(UART2_HMI, 0x07, UDATA_8B, PARITY_NONE, STOP_BIT1); //115200

    UartInit(UART6_COMM, 0x07, UDATA_8B, PARITY_NONE, STOP_BIT1); //115200

#if USE_GD == 1
#else
    UartInit(UART1_CON, 0x07, UDATA_8B, PARITY_NONE, STOP_BIT1); //115200

#endif

    sys_sta.auto_mode = STA_OFF;

    OSTaskCreateExt(Uart3_Rec_Task,  /* 启动任务函数指针 */
                    (void *)0,      /* 传递给任务的参数 */
                    (OS_STK *)&Uart3_Rec_STK[TASK_STK_SIZE_M - 1], /* 指向任务栈栈顶的指针 */
                    Uart3_rec_prio,    /* 任务的优先级，必须唯一，数字越低优先级越高 */
                    Uart3_rec_prio,    /* 任务ID，一般和任务优先级相同 */
                    (OS_STK *)&Uart3_Rec_STK[0],/* 指向任务栈栈底的指针。OS_STK_GROWTH 决定堆栈增长方向 */
                    TASK_STK_SIZE_M, /* 任务栈大小 */
                    (void *)0,  /* 一块用户内存区的指针，用于任务控制块TCB的扩展功能
                       （如任务切换时保存CPU浮点寄存器的数据）。一般不用，填0即可 */
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR); /* 任务选项字 */

    /* 指定任务的名称，用于调试。这个函数是可选的 */
    OSTaskNameSet(Uart3_rec_prio, "Uart3 Rec", &os_err);

    OSTaskCreateExt(Uart2_Time_Task,  /* 启动任务函数指针 */
                    (void *)0,      /* 传递给任务的参数 */
                    (OS_STK *)&Uart2_Time_STK[TASK_STK_SIZE_M - 1], /* 指向任务栈栈顶的指针 */
                    Uart2_time_prio,    /* 任务的优先级，必须唯一，数字越低优先级越高 */
                    Uart2_time_prio,    /* 任务ID，一般和任务优先级相同 */
                    (OS_STK *)&Uart2_Time_STK[0],/* 指向任务栈栈底的指针。OS_STK_GROWTH 决定堆栈增长方向 */
                    TASK_STK_SIZE_M, /* 任务栈大小 */
                    (void *)0,  /* 一块用户内存区的指针，用于任务控制块TCB的扩展功能
                       （如任务切换时保存CPU浮点寄存器的数据）。一般不用，填0即可 */
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR); /* 任务选项字 */

    /* 指定任务的名称，用于调试。这个函数是可选的 */
    OSTaskNameSet(Uart2_time_prio, "Uart2 Time", &os_err);


    MSys_HardTS = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);

    Current_HWIN = MSys_HardTS;

    eeprom_ts = 0xFF;

    rs485_ts = 0;
}

