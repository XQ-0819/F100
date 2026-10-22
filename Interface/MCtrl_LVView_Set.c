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
#define X_Space     310
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
    { TEXT_CreateIndirect,      "控制管理-通风级别",          GUI_ID_TEXT0,           250, 4, 300, 42, 0, 0},

    { TEXT_CreateIndirect,      "级别保持时间/S:",      GUI_ID_TEXT1,            X + X_Space * 0, 	Y + Y_Space * 0, 190, 36, 0, 0},
    { TEXT_CreateIndirect,      "风机间隔时间/S:",      GUI_ID_TEXT3,            X + X_Space * 1 - 10, 	Y + Y_Space * 0, 190, 36, 0, 0},
    { TEXT_CreateIndirect,      "小窗联动(时控):",      GUI_ID_TEXT5,            X + X_Space * 0, 	Y + Y_Space * 1, 190, 36, 0, 0},
    { TEXT_CreateIndirect,      "小窗联动停止级别:",    GUI_ID_TEXT7,            X + X_Space * 1 - 10, 	Y + Y_Space * 1, 210, 36, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",             	GUI_ID_CHECK1,            X + X_Space * 0,    Y + Y_Space * 3 + 3, 190, CHECK_SIZE, 0, 0},
    { TEXT_CreateIndirect,      "设备自动排序:",        GUI_ID_TEXT9,            X + X_Space * 0, 	Y + Y_Space * 4, 190, 36, 0, 0},
//    { TEXT_CreateIndirect,      "报警延时时间/S:",      GUI_ID_TEXT5,            X + X_Space * 0, 	Y + Y_Space * 4, 190, 36, 0, 0},


    { TEXT_CreateIndirect,      "静压辅助开始级别:",     GUI_ID_TEXT2,             X + X_Space * 1 - 10, 	Y + Y_Space * 0, 190, 36, 0, 0},
    { TEXT_CreateIndirect,      "静压报警最小级别:",    GUI_ID_TEXT4,            X + X_Space * 1 - 10, 	Y + Y_Space * 1, 190, 36, 0, 0},
    { TEXT_CreateIndirect,      "风机开启延时/S:",    	GUI_ID_TEXT6,            X + X_Space * 1 - 10, 	Y + Y_Space * 1, 190, 36, 0, 0},
    { TEXT_CreateIndirect,      "小窗提前开时间/S:",    GUI_ID_TEXT8,            X + X_Space * 0, 	Y + Y_Space * 2, 190, 36, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",             	GUI_ID_CHECK2,            X + X_Space * 1 - 10,    Y + Y_Space * 3 + 3, 190, CHECK_SIZE, 0, 0},

//    { TEXT_CreateIndirect,      "体感低温报警偏差:",      GUI_ID_TEXT10,           X + X_Space * 1, 	Y + Y_Space * 3, 190, 36, 0, 0},
//    { TEXT_CreateIndirect,      "体感高温报警偏差:",      GUI_ID_TEXT11,            X + X_Space * 1, 	Y + Y_Space * 4, 190, 36, 0, 0},

    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT0,            X + X_Space * 0 + 200, 	Y + Y_Space * 0, 72, 36, 0, 0},
    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT2,            X + X_Space * 1 + 220 , 	Y + Y_Space * 0, 72, 36, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",             GUI_ID_CHECK0,            X + X_Space * 0 + 200,    Y + Y_Space * 1 + 3, 120, CHECK_SIZE, 0, 0},
    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT5,            X + X_Space * 1 + 220, 	Y + Y_Space * 1, 72, 36, 0, 0},
    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT7,            X + X_Space * 0 + 200, 	Y + Y_Space * 3, 72, 36, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",             GUI_ID_CHECK3,            X + X_Space * 0 + 200,    Y + Y_Space * 4 + 3, 120, CHECK_SIZE, 0, 0},

    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT1,            X + X_Space * 1 + 220, 	Y + Y_Space * 0, 72, 36, 0, 0},
    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT3,            X + X_Space * 1 + 220, 	Y + Y_Space * 1, 72, 36, 0, 0},
    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT4,            X + X_Space * 1 + 220, 	Y + Y_Space * 1, 72, 36, 0, 0},
    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT6,            X + X_Space * 0 + 200, 	Y + Y_Space * 2, 72, 36, 0, 0},
    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT8,            X + X_Space * 1 + 220, 	Y + Y_Space * 3, 72, 36, 0, 0},
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

    for (MDev_ID = GUI_ID_EDIT0; MDev_ID <= GUI_ID_EDIT8; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Edit_SetHZ24(hItem);
    }

    for (MDev_ID = GUI_ID_TEXT0; MDev_ID <= GUI_ID_TEXT9; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24L(hItem);

        if (MDev_ID == GUI_ID_TEXT0) {
            TEXT_SetTextAlign(hItem,  GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetTextColor(hItem, GUI_WHITE);
        }
    }

    for (MDev_ID = GUI_ID_CHECK0; MDev_ID <= GUI_ID_CHECK3; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        CHECKBOX_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
        Check_SetHZ24(hItem);
        Login_Show(hItem, LOGIN_ADMIN);

    }

    sprintf(show_buf, "%s-%s", GUI_LANG_GetTextEx(Lang_Control, Language_Bit), GUI_LANG_GetTextEx(Lang_Vent_Lv, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT0), show_buf); // 控制管理-通风级别

    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT1), GUI_LANG_GetTextEx(Lang_Leve_Holding, Language_Bit));            // 级别保持时间/S:
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT2), GUI_LANG_GetTextEx(Lang_Level_Start_Level_NP, Language_Bit));    // 静压辅助开始级别:
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT3), GUI_LANG_GetTextEx(Lang_Fan_Interval, Language_Bit));            // 风机间隔时间/S:
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT4), GUI_LANG_GetTextEx(Lang_Min_leve_NPAla, Language_Bit));          // 静压报警最小级别:
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT5), GUI_LANG_GetTextEx(Lang_Inlet_Follow_Fan, Language_Bit));        // 小窗联动(时控):
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT6), GUI_LANG_GetTextEx(Lang_Fandelay, Language_Bit));                // 风机开启延时/S:
    TEXT_SetWrapMode(WM_GetDialogItem(hWin, GUI_ID_TEXT7), GUI_WRAPMODE_WORD);
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT7), GUI_LANG_GetTextEx(Lang_SmallWindow_F_StopLevel, Language_Bit)); // 小窗联动停止级别:
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT8), GUI_LANG_GetTextEx(Lang_Inlet_advance, Language_Bit));           // 小窗提前开时间/S:
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT9), GUI_LANG_GetTextEx(Lang_Devic_Aut_Sort, Language_Bit));          // 设备自动排序:

    CHECKBOX_SetText(WM_GetDialogItem(hWin, GUI_ID_CHECK0), GUI_LANG_GetTextEx(Lang_Enable, Language_Bit));         // 启用
    CHECKBOX_SetText(WM_GetDialogItem(hWin, GUI_ID_CHECK1), GUI_LANG_GetTextEx(Lang_FanCyc_Time, Language_Bit));    // 时控循环周期/S:
    CHECKBOX_SetText(WM_GetDialogItem(hWin, GUI_ID_CHECK2), GUI_LANG_GetTextEx(Lang_Same_Back_Temp, Language_Bit)); // 固定回差温度/℃:
    CHECKBOX_SetText(WM_GetDialogItem(hWin, GUI_ID_CHECK3), GUI_LANG_GetTextEx(Lang_Enable, Language_Bit));         // 启用

    CHECKBOX_SetState(WM_GetDialogItem(hWin, GUI_ID_CHECK0), ctrl_inf.var.win_link);
    CHECKBOX_SetState(WM_GetDialogItem(hWin, GUI_ID_CHECK1), ctrl_inf.lv_more.lv_fan_cycle_en);
    CHECKBOX_SetState(WM_GetDialogItem(hWin, GUI_ID_CHECK2), ctrl_inf.lv_more.lv_re_diff_en);
    CHECKBOX_SetState(WM_GetDialogItem(hWin, GUI_ID_CHECK3), ctrl_inf.lv_more.fan_auto_sort);


    sprintf(show_buf, "%d", ctrl_inf.var.lv_delay);
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT0), show_buf);

    sprintf(show_buf, "%d", ctrl_inf.var.fan_delay);
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT2), show_buf);

//    sprintf(show_buf, "%d", ctrl_inf.var.fan_lvdelay);
//    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT4), show_buf);

    sprintf(show_buf, "%d", ctrl_inf.var.win_link_lv + 1);
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT5), show_buf);

    sprintf(show_buf, "%d", ctrl_inf.var.win_pretime);
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT6), show_buf);

    sprintf(show_buf, "%d", ctrl_inf.lv_more.lv_fan_cycle);
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT7), show_buf);

    sprintf(show_buf, "%.1f", ctrl_inf.lv_more.lv_re_diff * 1.0f / TEMP_RATE);
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT8), show_buf);


	WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT1));
	WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT3));
	WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT2));
	WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT4));
	
    WM_SetFocus(WM_GetDialogItem(hWin, GUI_ID_BUTTON2));

	WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT6));
	WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT4));
	
}


static void Time_Get(void)
{
    float ftemp = Get_Input_Value();
    short stemp = (short)ftemp;

//    if (stemp > TEMP_ADJMAX) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = TEMP_ADJMAX;
//    }

    if (stemp < 0) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 0;
    }

    ctrl_inf.var.lv_delay = stemp;
    sprintf(show_buf, "%d", ctrl_inf.var.lv_delay);
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, GUI_ID_EDIT0), show_buf);
}

static void Fan_Time_Get(void)
{
    float ftemp = Get_Input_Value();
    short stemp = (short)ftemp;

//    if (stemp > TEMP_ADJMAX) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = TEMP_ADJMAX;
//    }

    if (stemp < 0) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 0;
    }

    ctrl_inf.var.fan_delay = stemp;
    sprintf(show_buf, "%d", ctrl_inf.var.fan_delay);
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, GUI_ID_EDIT2), show_buf);
}



static void Lv_fan_cycle_Get(void)
{
    float ftemp = Get_Input_Value();
    short stemp = (short)ftemp;

//    if (stemp > TEMP_ADJMAX) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = TEMP_ADJMAX;
//    }

    if (stemp < 0) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 0;
    }

    ctrl_inf.lv_more.lv_fan_cycle = stemp;
    sprintf(show_buf, "%d", ctrl_inf.lv_more.lv_fan_cycle);
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, GUI_ID_EDIT7), show_buf);

}

static void lv_re_diff_Get(void)
{
    float ftemp = Get_Input_Value() * TEMP_RATE;
    short stemp = (short)ftemp;

    if (stemp > TIME_2BIT) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = TIME_2BIT;
    }

    if (stemp < 0) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 0;
    }

    ctrl_inf.lv_more.lv_re_diff = stemp;
    sprintf(show_buf, "%.1f", ctrl_inf.lv_more.lv_re_diff * 1.0f / TEMP_RATE);
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, GUI_ID_EDIT8), show_buf);
}

static void win_link_Lv_Get(void)
{
    float ftemp = Get_Input_Value();
    short stemp = (short)ftemp;

    if (stemp > CTRL_LV_NUM) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = CTRL_LV_NUM;
    }

    if (stemp < 1) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 1;
    }

    ctrl_inf.var.win_link_lv = stemp - 1;
    sprintf(show_buf, "%d", ctrl_inf.var.win_link_lv + 1);
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, GUI_ID_EDIT5), show_buf);
}

static void win_pretime_cycle_Get(void)
{
    float ftemp = Get_Input_Value();
    short stemp = (short)ftemp;

//    if (stemp > TEMP_ADJMAX) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = TEMP_ADJMAX;
//    }

    if (stemp < 0) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 0;
    }

    ctrl_inf.var.win_pretime = stemp;
    sprintf(show_buf, "%d", ctrl_inf.var.win_pretime);
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, GUI_ID_EDIT6), show_buf);

}

extern void check_lv_re_diff(void);
extern void check_lv_fan_cycle(void);

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
                Set_Output_Callback(Time_Get);
                Set_Input_Len(3);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT2:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(Fan_Time_Get);
                Set_Input_Len(1);
                Input_Task(hWin);
            }

            break;

//        case GUI_ID_EDIT4:
//            if (NCode == WM_NOTIFICATION_RELEASED) {
//                Set_Output_Callback(Fan_DelayTime_Get);
//                Set_Input_Len(2);
//                Input_Task(hWin);
//            }

//            break;

        case GUI_ID_EDIT5:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(win_link_Lv_Get);
                Set_Input_Len(2);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT6:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(win_pretime_cycle_Get);
                Set_Input_Len(2);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT7:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(Lv_fan_cycle_Get);
                Set_Input_Len(4);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT8:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(lv_re_diff_Get);
                Set_Input_Len(3);
                Input_Task(hWin);
            }

            break;


        case GUI_ID_BUTTON2:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                ctrl_inf.var.win_link = CHECKBOX_GetState(WM_GetDialogItem(hWin, GUI_ID_CHECK0));
                ctrl_inf.lv_more.lv_fan_cycle_en = CHECKBOX_GetState(WM_GetDialogItem(hWin, GUI_ID_CHECK1));
                ctrl_inf.lv_more.lv_re_diff_en = CHECKBOX_GetState(WM_GetDialogItem(hWin, GUI_ID_CHECK2));
                ctrl_inf.lv_more.fan_auto_sort = CHECKBOX_GetState(WM_GetDialogItem(hWin, GUI_ID_CHECK3));
				
                check_lv_re_diff();
                check_lv_fan_cycle();

                if (Config_Save(SVAR_ID) < 0) {
                    MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_SAVE_ER));
                }

                if (Config_Save(SLVMORE_ID) < 0) {
                    MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_SAVE_ER));
                }


                Current_HWIN = WM_GetParent(hWin);
                GUI_EndDialog(hWin, 0);
                MCtrl_LVView_Task(WM_GetParent(hWin),0,0);
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
WM_HWIN MCtrl_LVView_Set;
void MCtrl_LVView_Set_Task(WM_HWIN hParent)
{
    MCtrl_LVView_Set = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    Current_HWIN = MCtrl_LVView_Set;
}

