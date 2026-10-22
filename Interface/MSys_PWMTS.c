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
#include <stdio.h>
#include <string.h>
#if USE_VS == 0
#include "ucos_ii.h"
#include "mytask.h"
#include "pwm_out.h"
#include "jdq.h"
#endif

/*********************************************************************
*
*       static data
*
**********************************************************************
*/

#define GUI_ID_TEXT10   (GUI_ID_USER+1)
#define GUI_ID_TEXT11   (GUI_ID_USER+2)
#define GUI_ID_TEXT12   (GUI_ID_USER+3)


static WM_HTIMER hTimer;


/*********************************************************************
*
*       Dialog resource
*
* This table conatins the info required to create the dialog.
* It has been created by ucGUIbuilder.
*/
#define XPos_Collec  (-130)
#define YPos_Space   (90)
#define YPos_Start   (100)
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect,    "",               0,                       0,  0,  800, 480, 0, 0},
    { TEXT_CreateIndirect,      "手动测试-模拟量测试",          GUI_ID_TEXT0,            250, 4, 300, 42, 0, 0},

    { TEXT_CreateIndirect,      "模拟量1:",    GUI_ID_TEXT1,              320 + XPos_Collec, YPos_Start + YPos_Space * 0, 100, 36, 0, 0},
    { TEXT_CreateIndirect,      "模拟量2:",    GUI_ID_TEXT2,              320 + XPos_Collec, YPos_Start + YPos_Space * 1, 100, 36, 0, 0},
    { TEXT_CreateIndirect,      "模拟量3:",    GUI_ID_TEXT3,               320 + XPos_Collec, YPos_Start + YPos_Space * 2, 100, 36, 0, 0},

    { EDIT_CreateIndirect,       "",            GUI_ID_EDIT0,          420 + XPos_Collec, YPos_Start + YPos_Space * 0, 70, 36, 0, 0},
    { EDIT_CreateIndirect,       "",           GUI_ID_EDIT1,            420 + XPos_Collec, YPos_Start + YPos_Space * 1, 70, 36, 0, 0},
    { EDIT_CreateIndirect,       "",           GUI_ID_EDIT2,            420 + XPos_Collec, YPos_Start + YPos_Space * 2, 70, 36, 0, 0},

    { TEXT_CreateIndirect,   	"%",    GUI_ID_TEXT4,              500 + XPos_Collec, YPos_Start + YPos_Space * 0, 32, 36, 0, 0},
    { TEXT_CreateIndirect,      "%",    GUI_ID_TEXT5,              500 + XPos_Collec, YPos_Start + YPos_Space * 1, 32, 36, 0, 0},
    { TEXT_CreateIndirect,      "%",    GUI_ID_TEXT6,               500 + XPos_Collec, YPos_Start + YPos_Space * 2, 32, 36, 0, 0},

    { TEXT_CreateIndirect,      "模拟量4:",    GUI_ID_TEXT7,              560 + XPos_Collec, YPos_Start + YPos_Space * 0, 100, 36, 0, 0},
    { TEXT_CreateIndirect,      "模拟量5:",    GUI_ID_TEXT8,               560 + XPos_Collec, YPos_Start + YPos_Space * 1, 100, 36, 0, 0},
    { TEXT_CreateIndirect,      "模拟量6:",    GUI_ID_TEXT9,               560 + XPos_Collec, YPos_Start + YPos_Space * 2, 100, 36, 0, 0},


    { EDIT_CreateIndirect,         "",          GUI_ID_EDIT3,           660 + XPos_Collec, YPos_Start + YPos_Space * 0, 70, 36, 0, 0},
    { EDIT_CreateIndirect,         "",          GUI_ID_EDIT4,           660 + XPos_Collec, YPos_Start + YPos_Space * 1, 70, 36, 0, 0},
    { EDIT_CreateIndirect,         "",          GUI_ID_EDIT5,           660 + XPos_Collec, YPos_Start + YPos_Space * 2, 70, 36, 0, 0},

    { TEXT_CreateIndirect,      "%",    GUI_ID_TEXT10,              740 + XPos_Collec, YPos_Start + YPos_Space * 0, 32, 36, 0, 0},
    { TEXT_CreateIndirect,      "%",    GUI_ID_TEXT11,               740 + XPos_Collec, YPos_Start + YPos_Space * 1, 32, 36, 0, 0},
    { TEXT_CreateIndirect,      "%",    GUI_ID_TEXT12,               740 + XPos_Collec, YPos_Start + YPos_Space * 2, 32, 36, 0, 0},

//    { BUTTON_CreateIndirect,    "上一页",          GUI_ID_BUTTON0,          MENU_BACK_X1, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
    { BUTTON_CreateIndirect,    "下一页",          GUI_ID_BUTTON1,          MENU_BACK_X2, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
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
//    WM_HWIN hWin = pMsg->hWin;
    Show_MBackPic();
    GUI_SetColor(GUI_LINE_COLOR);
    GUI_DrawRoundedRect(80, 65, 722, 366, 10);
    GUI_DrawRoundedRect(76, 62, 726, 369, 10);

    /*		  GUI_DrawLine(80,260,712,260);
    		  GUI_DrawLine(80,264,712,264);*/
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
    int MDev_ID = 0, i = 0;
    WM_HWIN hItem;


    for (MDev_ID = GUI_ID_BUTTON1; MDev_ID <= GUI_ID_BUTTON2; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, _cbButtonBack);
    }

    for (MDev_ID = GUI_ID_EDIT0; MDev_ID <= GUI_ID_EDIT5; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Edit_SetHZ24(hItem);
    }

    for (MDev_ID = GUI_ID_TEXT0; MDev_ID <= GUI_ID_TEXT9; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24L(hItem);
        if (MDev_ID == GUI_ID_TEXT0) {
            TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetTextColor(hItem, GUI_WHITE);
            TEXT_SetText(hItem, GUI_LANG_GetTextEx(Lang_Analog_Test, Language_Bit)); // 模拟量测试
        } else if (MDev_ID <= GUI_ID_TEXT3) {
            sprintf(show_buf, "%s%d:", GUI_LANG_GetTextEx(Lang_Analogy, Language_Bit), MDev_ID - GUI_ID_TEXT0);
            TEXT_SetText(hItem, show_buf); // 模拟量
        } else if (MDev_ID >= GUI_ID_TEXT7) {
            sprintf(show_buf, "%s%d:", GUI_LANG_GetTextEx(Lang_Analogy, Language_Bit), MDev_ID - GUI_ID_TEXT7 + 4);
            TEXT_SetText(hItem, show_buf); // 模拟量
        }
    }
    for (MDev_ID = GUI_ID_TEXT10; MDev_ID <= GUI_ID_TEXT12; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24L(hItem);
    }
    for (i = 0; i < 6; i++) {
        sprintf(show_buf, "%d", sta_out.ALLPWM[i]);
        EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT0 + i), show_buf);
    }

    hTimer = WM_CreateTimer(WM_GetClientWindow(hWin), 0, TEST_HOLD_TIME, 0);
}

static U8 temp_id = 0;
static void PWM_OUT_Set(void)
{
    float ftemp = Get_Input_Value();
    short stemp = (short)ftemp;

    if (stemp > MAX_DEGREE) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = MAX_DEGREE;
    }

    if (stemp < 0) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 0;
    }

    sta_out.ALLPWM[temp_id] = stemp;
    PWM_Out(temp_id, sta_out.ALLPWM[temp_id]);
    sprintf(show_buf, "%d", sta_out.ALLPWM[temp_id]);
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, GUI_ID_EDIT0 + temp_id), show_buf);

}



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

    case WM_INIT_DIALOG:
        InitDialog(pMsg);
        break;

    case WM_TIMER:
        OSTaskResume(control_proc_prio);
        sys_sta.auto_mode = STA_ON;
        Current_HWIN = WM_GetParent(hWin);
        GUI_EndDialog(hWin, 0);
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
	        WM_RestartTimer(hTimer, TEST_HOLD_TIME);
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

        case GUI_ID_BUTTON1:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                OSTaskResume(control_proc_prio);
                sys_sta.auto_mode = STA_ON;
                MSys_JDQTS_Task(WM_GetParent(hWin));
                GUI_EndDialog(hWin, 0);
            }

            break;

        case GUI_ID_BUTTON2:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                OSTaskResume(control_proc_prio);
                sys_sta.auto_mode = STA_ON;
                Current_HWIN = WM_GetParent(hWin);
                GUI_EndDialog(hWin, 0);
            }

            break;

        case GUI_ID_EDIT0:
        case GUI_ID_EDIT1:
        case GUI_ID_EDIT2:
        case GUI_ID_EDIT3:
        case GUI_ID_EDIT4:
        case GUI_ID_EDIT5:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                temp_id = Id - GUI_ID_EDIT0;
                Set_Output_Callback(PWM_OUT_Set);
                Set_Input_Len(3);
                Input_Task(hWin);
            }

            break;

        }

        break;

    default:
        WM_DefaultProc(pMsg);
    }
}


/*********************************************************************
*
*       MainTask
*
**********************************************************************
*/
WM_HWIN MSys_PWMTS;
void MSys_PWMTS_Task(WM_HWIN hParent)
{
	while(get_jdq_ctrl_state()==STA_ON){
        OSTimeDly(1);//等待继电器操作完成
	};
	
    OSTaskSuspend(control_proc_prio);
    sys_sta.auto_mode = STA_OFF;
    MSys_PWMTS = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    Current_HWIN = MSys_PWMTS;

}

