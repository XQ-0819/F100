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
#include "ucos_ii.h"
#include "mytask.h"
#include "jdq.h"
#endif

static WM_HTIMER hTimer;

/*********************************************************************
*
*       static data
*
**********************************************************************
*/
#define GUI_ID_BUTTON10 (GUI_ID_USER + 1)
#define GUI_ID_BUTTON11 (GUI_ID_USER + 2)
#define GUI_ID_BUTTON12 (GUI_ID_USER + 3)
#define GUI_ID_BUTTON13 (GUI_ID_USER + 4)
#define GUI_ID_BUTTON14 (GUI_ID_USER + 5)
#define GUI_ID_BUTTON15 (GUI_ID_USER + 6)
#define GUI_ID_BUTTON16 (GUI_ID_USER + 7)
#define GUI_ID_BUTTON17 (GUI_ID_USER + 8)
#define GUI_ID_BUTTON18 (GUI_ID_USER + 9)
#define GUI_ID_BUTTON19 (GUI_ID_USER + 10)
#define GUI_ID_BUTTON20 (GUI_ID_USER + 11)
#define GUI_ID_BUTTON21 (GUI_ID_USER + 12)
#define GUI_ID_BUTTON22 (GUI_ID_USER + 13)
#define GUI_ID_BUTTON23 (GUI_ID_USER + 14)
#define GUI_ID_BUTTON24 (GUI_ID_USER + 15)
#define GUI_ID_BUTTON25 (GUI_ID_USER + 16)
#define GUI_ID_BUTTON26 (GUI_ID_USER + 17)
#define GUI_ID_BUTTON27 (GUI_ID_USER + 18)
#define GUI_ID_BUTTON28 (GUI_ID_USER + 19)
#define GUI_ID_BUTTON29 (GUI_ID_USER + 20)


/*********************************************************************
*
*       Dialog resource
*
* This table conatins the info required to create the dialog.
* It has been created by ucGUIbuilder.
*/
#define ButtonM_Xos  	(40)
#define ButtonM_Yos  	(75)
#define X_Space   		(145)
#define Y_Space   		(85)
#define X_SIZE			(120)


static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect,    "",               0,                       0,  0,  800, 480, 0, 0},
    { TEXT_CreateIndirect,      "ÊÖ¶¯²âÊÔ-¼ÌµçÆ÷²âÊÔ",          GUI_ID_TEXT0,            250, 4, 300, 42, 0, 0},

    { BUTTON_CreateIndirect,    "¼ÌµçÆ÷1",           GUI_ID_BUTTON10,          ButtonM_Xos + X_Space * 0, ButtonM_Yos + Y_Space * 0, X_SIZE, 50, 0, 0},
    { BUTTON_CreateIndirect,    "¼ÌµçÆ÷2",           GUI_ID_BUTTON11,          ButtonM_Xos + X_Space * 1, ButtonM_Yos + Y_Space * 0, X_SIZE, 50, 0, 0},
    { BUTTON_CreateIndirect,    "¼ÌµçÆ÷3",           GUI_ID_BUTTON12,          ButtonM_Xos + X_Space * 2, ButtonM_Yos + Y_Space * 0, X_SIZE, 50, 0, 0},
    { BUTTON_CreateIndirect,    "¼ÌµçÆ÷4",           GUI_ID_BUTTON13,          ButtonM_Xos + X_Space * 3, ButtonM_Yos + Y_Space * 0, X_SIZE, 50, 0, 0},
    { BUTTON_CreateIndirect,    "¼ÌµçÆ÷5",           GUI_ID_BUTTON14,          ButtonM_Xos + X_Space * 4, ButtonM_Yos + Y_Space * 0, X_SIZE, 50, 0, 0},

    { BUTTON_CreateIndirect,    "¼ÌµçÆ÷6",           GUI_ID_BUTTON15,          ButtonM_Xos + X_Space * 0, ButtonM_Yos + Y_Space * 1, X_SIZE, 50, 0, 0},
    { BUTTON_CreateIndirect,    "¼ÌµçÆ÷7",           GUI_ID_BUTTON16,          ButtonM_Xos + X_Space * 1, ButtonM_Yos + Y_Space * 1, X_SIZE, 50, 0, 0},
    { BUTTON_CreateIndirect,    "¼ÌµçÆ÷8",           GUI_ID_BUTTON17,          ButtonM_Xos + X_Space * 2, ButtonM_Yos + Y_Space * 1, X_SIZE, 50, 0, 0},
    { BUTTON_CreateIndirect,    "¼ÌµçÆ÷9",           GUI_ID_BUTTON18,          ButtonM_Xos + X_Space * 3, ButtonM_Yos + Y_Space * 1, X_SIZE, 50, 0, 0},
    { BUTTON_CreateIndirect,    "¼ÌµçÆ÷10",          GUI_ID_BUTTON19,          ButtonM_Xos + X_Space * 4, ButtonM_Yos + Y_Space * 1, X_SIZE, 50, 0, 0},


    { BUTTON_CreateIndirect,    "¼ÌµçÆ÷11",           GUI_ID_BUTTON20,         ButtonM_Xos + X_Space * 0, ButtonM_Yos + Y_Space * 2, X_SIZE, 50, 0, 0},
    { BUTTON_CreateIndirect,    "¼ÌµçÆ÷12",           GUI_ID_BUTTON21,         ButtonM_Xos + X_Space * 1, ButtonM_Yos + Y_Space * 2, X_SIZE, 50, 0, 0},
    { BUTTON_CreateIndirect,    "¼ÌµçÆ÷13",           GUI_ID_BUTTON22,         ButtonM_Xos + X_Space * 2, ButtonM_Yos + Y_Space * 2, X_SIZE, 50, 0, 0},
    { BUTTON_CreateIndirect,    "¼ÌµçÆ÷14",           GUI_ID_BUTTON23,         ButtonM_Xos + X_Space * 3, ButtonM_Yos + Y_Space * 2, X_SIZE, 50, 0, 0},
    { BUTTON_CreateIndirect,    "¼ÌµçÆ÷15",           GUI_ID_BUTTON24,         ButtonM_Xos + X_Space * 4, ButtonM_Yos + Y_Space * 2, X_SIZE, 50, 0, 0},

    { BUTTON_CreateIndirect,    "¼ÌµçÆ÷16",           GUI_ID_BUTTON25,         ButtonM_Xos + X_Space * 0, ButtonM_Yos + Y_Space * 3, X_SIZE, 50, 0, 0},
    { BUTTON_CreateIndirect,    "¼ÌµçÆ÷17",           GUI_ID_BUTTON26,         ButtonM_Xos + X_Space * 1, ButtonM_Yos + Y_Space * 3, X_SIZE, 50, 0, 0},
    { BUTTON_CreateIndirect,    "¼ÌµçÆ÷18",           GUI_ID_BUTTON27,         ButtonM_Xos + X_Space * 2, ButtonM_Yos + Y_Space * 3, X_SIZE, 50, 0, 0},
    { BUTTON_CreateIndirect,    "¼ÌµçÆ÷19",           GUI_ID_BUTTON28,         ButtonM_Xos + X_Space * 3, ButtonM_Yos + Y_Space * 3, X_SIZE, 50, 0, 0},
    { BUTTON_CreateIndirect,    "¼ÌµçÆ÷20",           GUI_ID_BUTTON29,         ButtonM_Xos + X_Space * 4, ButtonM_Yos + Y_Space * 3, X_SIZE, 50, 0, 0},

    { BUTTON_CreateIndirect,    "ÉÏÒ»Ò³",          GUI_ID_BUTTON0,          MENU_BACK_X1, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
//    { BUTTON_CreateIndirect,    "±¨¾¯²âÊÔ",          GUI_ID_BUTTON1,          MENU_BACK_X2, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
    { BUTTON_CreateIndirect,    "·µ»Ø",             GUI_ID_BUTTON2,         MENU_BACK_X3, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},

};


static void Show_TS_Result(WM_HWIN hWin)
{
    int id = 0;

    for (id = GUI_ID_BUTTON10; id <= GUI_ID_BUTTON29; id++) {
        if (sta_out.ALLJDQ[id - GUI_ID_BUTTON10] == STA_ON) {
            BUTTON_SetBkColor(WM_GetDialogItem(hWin, id), BUTTON_CI_UNPRESSED, GUI_GREEN);
        } else {
            BUTTON_SetBkColor(WM_GetDialogItem(hWin, id), BUTTON_CI_UNPRESSED, GUI_RED);
        }
    }
}
static void JDQ_TS(U8 id)
{
    if (sta_out.ALLJDQ[id] == STA_ON) {
        sta_out.ALLJDQ[id] = STA_OFF;
    } else {
        sta_out.ALLJDQ[id] = STA_ON;
    }

    JDQ_Control(id, sta_out.ALLJDQ[id]);
	JDQ_Control_Hard();	
}

/*****************************************************************
**      FunctionName:void PaintDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**
**      call this function in _cbCallback --> WM_PAINT
*****************************************************************/

static void PaintDialog(WM_MESSAGE * pMsg)
{
//    WM_HWIN hWin = pMsg->hWin;
    //90, 95, 592,286,0
    Show_MBackPic();

    GUI_SetColor(GUI_LINE_COLOR);
//    GUI_DrawRoundedRect(90, 95, 692, 386, 10);
//    GUI_DrawRoundedRect(86, 92, 696, 389, 10);
    // GUI_DrawRoundedRect(53, 63, 747,321, 10);
    //GUI_DrawRoundedRect(50, 60, 750,324, 10);

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
    int IDMB = 0;
    BUTTON_Handle hObj_Temp;

    //
    //GUI_ID_TEXT0
    //
    Text_SetHZ24(WM_GetDialogItem(hWin, GUI_ID_TEXT0));
    TEXT_SetTextColor(WM_GetDialogItem(hWin, GUI_ID_TEXT0), GUI_WHITE);

//    hObj_Temp = WM_GetDialogItem(hWin, GUI_ID_BUTTON1);
//    Button_SetHZ24(hObj_Temp);

    for (IDMB = GUI_ID_BUTTON0; IDMB <= GUI_ID_BUTTON2; IDMB++) {
        if (IDMB == GUI_ID_BUTTON1)
            continue;

        hObj_Temp = WM_GetDialogItem(hWin, IDMB);
        WM_SetHasTrans(hObj_Temp);
        WM_SetCallback(hObj_Temp, _cbButtonBack);
    }

    for (IDMB = GUI_ID_BUTTON10; IDMB <= GUI_ID_BUTTON29; IDMB++) {
        hObj_Temp = WM_GetDialogItem(hWin, IDMB);
        Button_SetHZ24(hObj_Temp);
        BUTTON_SetSkinClassic(hObj_Temp);
        sprintf(show_buf, "%s%d", GUI_LANG_GetTextEx(Lang_Relay, Language_Bit), IDMB - GUI_ID_BUTTON10 + 1); // ¼ÌµçÆ÷
        BUTTON_SetText(hObj_Temp, show_buf);
    }

    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT0), GUI_LANG_GetTextEx(Lang_Relay_Test, Language_Bit)); // ¼ÌµçÆ÷²âÊÔ

    Show_TS_Result(hWin);
    hTimer = WM_CreateTimer(WM_GetClientWindow(hWin), 0, TEST_HOLD_TIME, 0);

}




/*********************************************************************
*
*       Dialog callback routine
*/
static void _cbCallback(WM_MESSAGE * pMsg)
{
    int 	NCode, Id;
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

        if (Id >= GUI_ID_BUTTON10 && Id <= GUI_ID_BUTTON29 && NCode == WM_NOTIFICATION_RELEASED) {
            JDQ_TS(Id - GUI_ID_BUTTON10);
            Show_TS_Result(hWin);
            return;
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

        case GUI_ID_BUTTON0:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                OSTaskResume(control_proc_prio);
                sys_sta.auto_mode = STA_ON;
                MSys_PWMTS_Task(WM_GetParent(hWin));
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
WM_HWIN MSys_JDQTS;
void MSys_JDQTS_Task(WM_HWIN hParent)
{
    while (get_jdq_ctrl_state() == STA_ON) {
        OSTimeDly(1);//µÈ´ý¼ÌµçÆ÷²Ù×÷Íê³É
    };

    OSTaskSuspend(control_proc_prio);

    sys_sta.auto_mode = STA_OFF;

    MSys_JDQTS = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);

    Current_HWIN = MSys_JDQTS;
}

