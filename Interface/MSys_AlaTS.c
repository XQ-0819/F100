#include "menuinc.h"
#include "jdq.h"
#include "bsp.h"



static void Set_Value(WM_HWIN hWin)
{
    sprintf(show_buf, "%02d:%02d", basic_inf.ala_test.time / 60, basic_inf.ala_test.time % 60);
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT0), show_buf);
    sprintf(show_buf, "%d", basic_inf.ala_test.hold_time);
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT1), (const char *) show_buf);
}


static void Set_OptValue(WM_HWIN hWin)
{
    CHECKBOX_SetState(WM_GetDialogItem(hWin, GUI_ID_CHECK0 ), basic_inf.ala_test.enable);
    CHECKBOX_SetState(WM_GetDialogItem(hWin, GUI_ID_CHECK1 ), basic_inf.ala_open_mode);
    Set_Value(hWin);
}

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
    { TEXT_CreateIndirect,      "报警测试",          GUI_ID_TEXT0,            300, 4, 200, 42, 0, 0},

    { CHECKBOX_CreateIndirect,  "启用报警测试",         GUI_ID_CHECK0,           270, 100, 240, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用报警测试",         GUI_ID_CHECK1,           270, 160, 240, CHECK_SIZE, 0, 0},
    { TEXT_CreateIndirect,      "报警时间:",            GUI_ID_TEXT1,            270, 220, 150, 36, 0, 0},
    { EDIT_CreateIndirect,       NULL,               	GUI_ID_EDIT0,            400, 220, 90, 36, 0, 0},

    { TEXT_CreateIndirect,      "持续时间:",            GUI_ID_TEXT2,            270, 280, 150, 36, 0, 0},
    { EDIT_CreateIndirect,       NULL,               	GUI_ID_EDIT1,            400, 280, 90, 36, 0, 0},
    { TEXT_CreateIndirect,      "S",            		GUI_ID_TEXT3,            500, 280, 150, 36, 0, 0},

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
    GUI_DrawRoundedRect(200, 30 + 42, 600, 300 + 42, 15);
    GUI_DrawRoundedRect(197, 27 + 42, 603, 303 + 42, 15);
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

    for (MDev_ID = GUI_ID_BUTTON2; MDev_ID <= GUI_ID_BUTTON2; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, Cut_cbButton);
    }

    for (MDev_ID = GUI_ID_EDIT0; MDev_ID <= GUI_ID_EDIT1; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Edit_SetHZ24(hItem);
    }

    for (MDev_ID = GUI_ID_TEXT0; MDev_ID <= GUI_ID_TEXT3; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24L(hItem);

        if (MDev_ID == GUI_ID_TEXT0) {
            TEXT_SetTextAlign(hItem,  GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetTextColor(hItem, GUI_WHITE);
        }
    }

    for (MDev_ID = GUI_ID_CHECK0; MDev_ID <= GUI_ID_CHECK1; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Check_SetHZ24(hItem);
    }
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT0), GUI_LANG_GetTextEx(Lang_Alarm_Test, Language_Bit));   // 报警测试
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT1), GUI_LANG_GetTextEx(Lang_ALA_Time, Language_Bit));     // 报警时间
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT2), GUI_LANG_GetTextEx(Lang_Ala_Duration, Language_Bit)); // 持续时间

    CHECKBOX_SetText(WM_GetDialogItem(hWin, GUI_ID_CHECK0), GUI_LANG_GetTextEx(Lang_EnAlarm_Test, Language_Bit));     // 启用报警测试
    CHECKBOX_SetText(WM_GetDialogItem(hWin, GUI_ID_CHECK1), GUI_LANG_GetTextEx(Lang_Disconnect_Alarm, Language_Bit)); // 断开报警

    // TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT4), "Tip: When enabled, alarm output at a specific time!");//

    Set_OptValue(hWin);

}


static void Time_Get(void)
{
    float ftemp = Get_Input_Value() ;
    short stemp = (short)ftemp;

    stemp = (stemp / 100) * 60 + (stemp % 100);

    if (stemp > MAX_TIME_DAY) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = MAX_TIME_DAY;
    }

    if (stemp < 0) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 0;
    }

    basic_inf.ala_test.time = stemp;
    Set_Value(Current_HWIN);
}
static void HoldTime_Get(void)
{
    float ftemp = Get_Input_Value();
    short stemp = (short)ftemp;

//    if (stemp > TIME_MAX) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = TIME_MAX;
//    }

    if (stemp < 0) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 0;
    }

    basic_inf.ala_test.hold_time = stemp;
    Set_Value(Current_HWIN);
}
static void Get_OptValue(WM_HWIN hWin)
{
    basic_inf.ala_test.enable =  CHECKBOX_GetState(WM_GetDialogItem(hWin, GUI_ID_CHECK0 ));
    basic_inf.ala_open_mode =  CHECKBOX_GetState(WM_GetDialogItem(hWin, GUI_ID_CHECK1 ));
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

        case GUI_ID_EDIT0:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(Time_Get);
                Set_Input_Len(4);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT1:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(HoldTime_Get);
                Set_Input_Len(3);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_BUTTON2:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Get_OptValue(hWin);
		        JDQ_Control(JDQ_ALA, sta_out.ALLJDQ[JDQ_ALA]);//更改一下状态
				
                if (Config_Save(SBASIC_ID) < 0) {
                    MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_SAVE_ER));
                }

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
WM_HWIN MSys_AlaTS;
void MSys_AlaTS_Task(WM_HWIN hParent)
{
    MSys_AlaTS = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    Current_HWIN = MSys_AlaTS;
}

