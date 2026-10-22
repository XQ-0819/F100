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


//EventsFunctionList
//EndofEventsFunctionList

#define X  110
#define Y  70
#define X_Space     300
#define Y_Space     70

/*********************************************************************
*
*       static data
*
**********************************************************************
*/
#define GUI_ID_TEXT10   (GUI_ID_USER+1)
#define GUI_ID_TEXT11   (GUI_ID_USER+2)


/*********************************************************************
*
*       Dialog resource
*
* This table conatins the info required to create the dialog.
* It has been created by ucGUIbuilder.
*/

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect,    "",               0,                       0,  0,  800, 480, 0, 0},
    { TEXT_CreateIndirect,      "控制管理-温度曲线",          GUI_ID_TEXT0,           250, 4, 300, 42, 0, 0},

    { TEXT_CreateIndirect,      "纵向温度简化:",      GUI_ID_TEXT1,            X + X_Space * 0, 	Y + Y_Space * 0, 190, 36, 0, 0},
    { TEXT_CreateIndirect,      "报警温度偏差化:",      GUI_ID_TEXT3,            X + X_Space * 0, 	Y + Y_Space * 1, 190, 36, 0, 0},
    { TEXT_CreateIndirect,      "精细降温曲线:",      GUI_ID_TEXT5,            X + X_Space * 0, 	Y + Y_Space * 2, 190, 36, 0, 0},
    { TEXT_CreateIndirect,      "温度变化间隔/h:",      GUI_ID_TEXT7,            X + X_Space * 0, 	Y + Y_Space * 3, 190, 50, 0, 0},
//    { TEXT_CreateIndirect,      "报警延时时间/S:",      GUI_ID_TEXT5,            X + X_Space * 0, 	Y + Y_Space * 4, 190, 36, 0, 0},


    { TEXT_CreateIndirect,      "体感低温报警偏差:",     GUI_ID_TEXT2,             X + X_Space * 1-10, 	Y + Y_Space * 0, 210, 36, 0, 0},
    { TEXT_CreateIndirect,      "体感高温报警偏差:",    GUI_ID_TEXT4,            X + X_Space * 1-10, 	Y + Y_Space * 1, 200, 36, 0, 0},
    { TEXT_CreateIndirect,      "最小通风(加热):",    GUI_ID_TEXT6,            X + X_Space * 1-10, 	Y + Y_Space * 2, 200, 36, 0, 0},
    { TEXT_CreateIndirect,      "每日温度变化次数:",      GUI_ID_TEXT8,           X + X_Space * 1-10, 	Y + Y_Space * 3, 200, 50, 0, 0},
//    { TEXT_CreateIndirect,      "体感高温报警偏差:",      GUI_ID_TEXT11,            X + X_Space * 1, 	Y + Y_Space * 4, 190, 36, 0, 0},

    { CHECKBOX_CreateIndirect,  "启用",             GUI_ID_CHECK0,            X + X_Space * 0 + 188,    Y + Y_Space * 0 + 3, 90, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",             GUI_ID_CHECK1,            X + X_Space * 0 + 188,    Y + Y_Space * 1 + 3, 90, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",             GUI_ID_CHECK2,            X + X_Space * 0 + 188,    Y + Y_Space * 2 + 3, 90, CHECK_SIZE, 0, 0},
    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT2,            X + X_Space * 0 + 188, 	Y + Y_Space * 3, 72, 36, 0, 0},
//    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT3,            X + X_Space * 0 + 188, 	Y + Y_Space * 4, 72, 36, 0, 0},

    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT0,            X + X_Space * 1 + 200, 	Y + Y_Space * 0, 72, 36, 0, 0},
    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT1,            X + X_Space * 1 + 200, 	Y + Y_Space * 1, 72, 36, 0, 0},
    { DROPDOWN_CreateIndirect,	 "",			 GUI_ID_DROPDOWN0,			  X + X_Space * 1 + 200, 	Y + Y_Space * 2, 100, 120, 0, 0},
    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT3,            X + X_Space * 1 + 200, 	Y + Y_Space * 3, 72, 36, 0, 0},
//    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT8,            X + X_Space * 1 + 200, 	Y + Y_Space * 4, 72, 36, 0, 0},

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
//	WM_HWIN hWin = pMsg->hWin;
    Show_MBackPic();
    GUI_SetColor(GUI_LINE_COLOR);
    GUI_DrawRoundedRect(80, 60, 720, 400, 15);
    GUI_DrawRoundedRect(77, 57, 723, 403, 15);
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

    for (MDev_ID = GUI_ID_EDIT0; MDev_ID <= GUI_ID_EDIT3; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Edit_SetHZ24(hItem);
    }

    for (MDev_ID = GUI_ID_TEXT0; MDev_ID <= GUI_ID_TEXT8; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24L(hItem);

        if (MDev_ID == GUI_ID_TEXT0) {
            TEXT_SetTextAlign(hItem,  GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetTextColor(hItem, GUI_WHITE);
        }
    }

    for (MDev_ID = GUI_ID_CHECK0; MDev_ID <= GUI_ID_CHECK2; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        CHECKBOX_SetText(hItem, GUI_LANG_GetTextEx(Lang_Enable, Language_Bit));
        Check_SetHZ24(hItem);
        Login_Show(hItem, LOGIN_ADMIN);
    }

    for (MDev_ID = GUI_ID_DROPDOWN0; MDev_ID <= GUI_ID_DROPDOWN0; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Dropdown_SetHZ24(hItem);

        DROPDOWN_AddString(hItem, GUI_LANG_GetTextEx(Lang_None, Language_Bit)); // 无
        DROPDOWN_AddString(hItem, GUI_LANG_GetTextEx(Lang_Avg, Language_Bit));  // 平均
        DROPDOWN_AddString(hItem, GUI_LANG_GetTextEx(Lang_Area, Language_Bit)); // 区域
        Login_Show(hItem, LOGIN_SUPADMIN);
    }

    sprintf(show_buf, "%s-%s", GUI_LANG_GetTextEx(Lang_Control, Language_Bit), GUI_LANG_GetTextEx(Lang_T_Curve, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT0), show_buf);                                                 // 控制管理-温度曲线
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT1), GUI_LANG_GetTextEx(Lang_TunnelT_S, Language_Bit));         // 纵向温度简化:
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT2), GUI_LANG_GetTextEx(Lang_LowST_Alarm_DEVN, Language_Bit));  // 体感低温偏差:
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT3), GUI_LANG_GetTextEx(Lang_Alarm_bias, Language_Bit));        // 报警温度偏差化:
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT4), GUI_LANG_GetTextEx(Lang_HighST_Alarm_DEVN, Language_Bit)); // 体感高温偏差:
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT5), GUI_LANG_GetTextEx(Lang_LTChange, Language_Bit));          // 精细降温曲线:
    sprintf(show_buf, "%s(%s):", GUI_LANG_GetTextEx(Lang_MinVent, Language_Bit), GUI_LANG_GetTextEx(Lang_Heat, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT6), show_buf);                                                 // 最小通风(加热):
    TEXT_SetWrapMode(WM_GetDialogItem(hWin, GUI_ID_TEXT7), GUI_WRAPMODE_WORD);
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT7), GUI_LANG_GetTextEx(Lang_T_change_interval, Language_Bit)); // 温度变化间隔(h):
    TEXT_SetWrapMode(WM_GetDialogItem(hWin, GUI_ID_TEXT8), GUI_WRAPMODE_WORD);
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT8), GUI_LANG_GetTextEx(Lang_Adjust_Times, Language_Bit));      // 每日温度变化次数

    CHECKBOX_SetState(WM_GetDialogItem(hWin, GUI_ID_CHECK0 ), ctrl_inf.extemp.temp_simple);
    CHECKBOX_SetState(WM_GetDialogItem(hWin, GUI_ID_CHECK1 ), ctrl_inf.extemp.ala_diff_en);
    CHECKBOX_SetState(WM_GetDialogItem(hWin, GUI_ID_CHECK2 ), ctrl_inf.extemp.day_temp_en);
    DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN0), ctrl_inf.extemp.minwind_temp_sta);


    sprintf(show_buf, "%.1f", ctrl_inf.extemp.hum_Ltemp * 1.0f / TEMP_RATE);
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT0), show_buf);

    sprintf(show_buf, "%.1f", ctrl_inf.extemp.hum_Htemp * 1.0f / TEMP_RATE);
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT1), show_buf);

    sprintf(show_buf, "%d", ctrl_inf.extemp.chg_hours);
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT2), show_buf);

    sprintf(show_buf, "%d", ctrl_inf.extemp.chg_times);
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT3), show_buf);
}

static void THW_L_Get(void)
{
    float ftemp = Get_Input_Value() * TEMP_RATE;
    short stemp = (short)ftemp;

    if (stemp > 255) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 255;
    }

    if (stemp < 0) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 0;
    }

    ctrl_inf.extemp.hum_Ltemp = stemp;
    sprintf(show_buf, "%.1f", ctrl_inf.extemp.hum_Ltemp * 1.0f / TEMP_RATE);
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, GUI_ID_EDIT0), show_buf);
}

static void THW_H_Get(void)
{
    float ftemp = Get_Input_Value() * TEMP_RATE;
    short stemp = (short)ftemp;

    if (stemp > 255) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 255;
    }

    if (stemp < 0) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 0;
    }

    ctrl_inf.extemp.hum_Htemp = stemp;
    sprintf(show_buf, "%.1f", ctrl_inf.extemp.hum_Htemp * 1.0f / TEMP_RATE);
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, GUI_ID_EDIT1), show_buf);
}

static void Chg_Hours_Get(void)
{
    float ftemp = Get_Input_Value();
    short stemp = (short)ftemp;

    if (stemp > 12) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 1;
    }

    if (stemp <= 0) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 1;
    }

    ctrl_inf.extemp.chg_hours = stemp;
    sprintf(show_buf, "%d", ctrl_inf.extemp.chg_hours);
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, GUI_ID_EDIT2), show_buf);
}

static void Chg_Times_Get(void)
{
    float ftemp = Get_Input_Value();
    short stemp = (short)ftemp;

    if (stemp > 24) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 4;
    }

    if (stemp <= 0) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 1;
    }

    if (stemp * ctrl_inf.extemp.chg_hours > 24) {//时间
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 24 / ctrl_inf.extemp.chg_hours;
    }

    ctrl_inf.extemp.chg_times = stemp;
    sprintf(show_buf, "%d", ctrl_inf.extemp.chg_times);
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, GUI_ID_EDIT3), show_buf);
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
				
        if (Id >= GUI_ID_EDIT0 && Id <= GUI_ID_EDIT9 && NCode == WM_NOTIFICATION_RELEASED
                && get_login_permiss(LOGIN_ADMIN) < 0) {
            MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_NO_LOGIN));
            break;
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
                Set_Output_Callback(THW_L_Get);
                Set_Input_Len(4);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT1:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(THW_H_Get);
                Set_Input_Len(4);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT2:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(Chg_Hours_Get);
                Set_Input_Len(2);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT3:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(Chg_Times_Get);
                Set_Input_Len(2);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_BUTTON2:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                ctrl_inf.extemp.temp_simple = CHECKBOX_GetState(WM_GetDialogItem(hWin, GUI_ID_CHECK0 ));
                ctrl_inf.extemp.ala_diff_en = CHECKBOX_GetState(WM_GetDialogItem(hWin, GUI_ID_CHECK1 ));
                ctrl_inf.extemp.day_temp_en = CHECKBOX_GetState(WM_GetDialogItem(hWin, GUI_ID_CHECK2 ));
                ctrl_inf.extemp.minwind_temp_sta = DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN0));

                if (Config_Save(STEMP_ID) < 0) {
                    MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_SAVE_ER));
                }

                Current_HWIN = WM_GetParent(hWin);
                GUI_EndDialog(hWin, 0);
                MCtrl_TempCurve_Task(WM_GetParent(hWin));
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
WM_HWIN MCtrl_TempCurve_Set;
void MCtrl_TempCurve_Set_Task(WM_HWIN hParent)
{
    MCtrl_TempCurve_Set = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    Current_HWIN = MCtrl_TempCurve_Set;
}

