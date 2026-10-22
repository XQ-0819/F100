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
    { TEXT_CreateIndirect,      "控制管理-湿热补偿",          GUI_ID_TEXT0,           250, 4, 300, 42, 0, 0},

    { TEXT_CreateIndirect,      "湿度补偿下限/%:",      GUI_ID_TEXT1,            X + X_Space * 0, 	Y + Y_Space * 0, 190, 50, 0, 0},
    { TEXT_CreateIndirect,      "最大补偿温度/℃:",      GUI_ID_TEXT3,            X + X_Space * 0, 	Y + Y_Space * 1, 190, 36, 0, 0},
    { TEXT_CreateIndirect,      "开始时间:",      		GUI_ID_TEXT5,            X + X_Space * 0, 	Y + Y_Space * 2, 190, 36, 0, 0},
//    { TEXT_CreateIndirect,      "风机开启延时/S:",      GUI_ID_TEXT4,            X + X_Space * 0, 	Y + Y_Space * 3, 190, 36, 0, 0},
//    { TEXT_CreateIndirect,      "报警延时时间/S:",      GUI_ID_TEXT5,            X + X_Space * 0, 	Y + Y_Space * 4, 190, 36, 0, 0},


    { TEXT_CreateIndirect,      "湿度补偿上限/%:",     GUI_ID_TEXT2,             X + X_Space * 1, 	Y + Y_Space * 0, 190, 50, 0, 0},
    { TEXT_CreateIndirect,      "最小调节湿度/%:",    GUI_ID_TEXT4,            X + X_Space * 1, 	Y + Y_Space * 1, 190, 50, 0, 0},
    { TEXT_CreateIndirect,      "结束时间:",    		GUI_ID_TEXT6,            X + X_Space * 1, 	Y + Y_Space * 2, 190, 36, 0, 0},
//    { TEXT_CreateIndirect,      "体感低温报警偏差:",      GUI_ID_TEXT10,           X + X_Space * 1, 	Y + Y_Space * 3, 190, 36, 0, 0},
//    { TEXT_CreateIndirect,      "体感高温报警偏差:",      GUI_ID_TEXT11,            X + X_Space * 1, 	Y + Y_Space * 4, 190, 36, 0, 0},

    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT0,            X + X_Space * 0 + 180, 	Y + Y_Space * 0, 72, 36, 0, 0},
    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT2,            X + X_Space * 0 + 180, 	Y + Y_Space * 1, 72, 36, 0, 0},
    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT4,            X + X_Space * 0 + 180, 	Y + Y_Space * 2, 72, 36, 0, 0},
//    { CHECKBOX_CreateIndirect,  "启用",             GUI_ID_CHECK0,            X + X_Space * 0 + 180,    Y + Y_Space * 2, 90, CHECK_SIZE, 0, 0},
//    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT3,            X + X_Space * 0 + 180, 	Y + Y_Space * 4, 72, 36, 0, 0},

    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT1,            X + X_Space * 1 + 200, 	Y + Y_Space * 0, 72, 36, 0, 0},
    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT3,            X + X_Space * 1 + 200, 	Y + Y_Space * 1, 72, 36, 0, 0},
    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT5,            X + X_Space * 1 + 200, 	Y + Y_Space * 2, 72, 36, 0, 0},
//    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT7,            X + X_Space * 1 + 200, 	Y + Y_Space * 3, 72, 36, 0, 0},
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

    for (MDev_ID = GUI_ID_EDIT0; MDev_ID <= GUI_ID_EDIT5; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Edit_SetHZ24(hItem);
    }

    for (MDev_ID = GUI_ID_TEXT0; MDev_ID <= GUI_ID_TEXT6; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24L(hItem);

        if (MDev_ID == GUI_ID_TEXT0) {
            TEXT_SetTextAlign(hItem,  GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetTextColor(hItem, GUI_WHITE);
        }
    }

    sprintf(show_buf, "%s-%s", GUI_LANG_GetTextEx(Lang_Control, Language_Bit), GUI_LANG_GetTextEx(Lang_RH_Compensation, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT0), show_buf);                                           // 控制管理-湿热补偿
    TEXT_SetWrapMode(WM_GetDialogItem(hWin, GUI_ID_TEXT1), GUI_WRAPMODE_WORD);
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT1), GUI_LANG_GetTextEx(Lang_LComp_RH, Language_Bit));    // 湿度补偿下限:
    TEXT_SetWrapMode(WM_GetDialogItem(hWin, GUI_ID_TEXT2), GUI_WRAPMODE_WORD);
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT2), GUI_LANG_GetTextEx(Lang_HComp_RH, Language_Bit));    // 湿度补偿上限:
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT3), GUI_LANG_GetTextEx(Lang_Max_CompT, Language_Bit));   // 最大补偿温度:
    TEXT_SetWrapMode(WM_GetDialogItem(hWin, GUI_ID_TEXT4), GUI_WRAPMODE_WORD);
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT4), GUI_LANG_GetTextEx(Lang_Min_Comp_RH, Language_Bit)); // 最小调节湿度:
    sprintf(show_buf, "%s:", GUI_LANG_GetTextEx(Lang_Start_time, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT5), show_buf); // 开始时间:
    sprintf(show_buf, "%s:", GUI_LANG_GetTextEx(Lang_End_time, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT6), show_buf); // 结束时间:

    sprintf(show_buf, "%d", ctrl_inf.hum.start_hum);
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT0), show_buf);

    sprintf(show_buf, "%d", ctrl_inf.hum.stop_hum);
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT1), show_buf);

    sprintf(show_buf, "%.1f", ctrl_inf.hum.max_temp * 1.0f / TEMP_RATE);
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT2), show_buf);

    sprintf(show_buf, "%d", ctrl_inf.hum.min_adj_hum);
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT3), show_buf);

    sprintf(show_buf, "%02d:%02d",  ctrl_inf.hum.start_time / 60, ctrl_inf.hum.start_time % 60);
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT4), show_buf);

    sprintf(show_buf, "%02d:%02d",  ctrl_inf.hum.end_time / 60, ctrl_inf.hum.end_time % 60);
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT5), show_buf);
}


static void Start_Hum_Get(void)
{
    float ftemp = Get_Input_Value();
    short stemp = (short)ftemp;

    if (stemp > HUM_MAX) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = HUM_MAX;
    }

    if (stemp < 0) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 0;
    }

    ctrl_inf.hum.start_hum = stemp;
    sprintf(show_buf, "%d", ctrl_inf.hum.start_hum);
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, GUI_ID_EDIT0), show_buf);
}

static void Stop_Hum_Get(void)
{
    float ftemp = Get_Input_Value();
    short stemp = (short)ftemp;

    if (stemp > HUM_MAX) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = HUM_MAX;
    }

    if (stemp < 0) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 0;
    }

    ctrl_inf.hum.stop_hum = stemp;
    sprintf(show_buf, "%d", ctrl_inf.hum.stop_hum);
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, GUI_ID_EDIT1), show_buf);
}

static void Max_Temp_Get(void)
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

    ctrl_inf.hum.max_temp = stemp;
    sprintf(show_buf, "%.1f", ctrl_inf.hum.max_temp * 1.0f / TEMP_RATE);
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, GUI_ID_EDIT2), show_buf);
}

static void Min_Hum_Get(void)
{
    float ftemp = Get_Input_Value();
    short stemp = (short)ftemp;

    if (stemp > HUM_MAX) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = HUM_MAX;
    }

    if (stemp < 0) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 0;
    }

    ctrl_inf.hum.min_adj_hum = stemp;
    sprintf(show_buf, "%d", ctrl_inf.hum.min_adj_hum);
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, GUI_ID_EDIT3), show_buf);
}

static void Start_Time_Get(void)
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


    ctrl_inf.hum.start_time = stemp;
    sprintf(show_buf, "%02d:%02d",  ctrl_inf.hum.start_time / 60, ctrl_inf.hum.start_time % 60);
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, GUI_ID_EDIT4), show_buf);


}

static void Stop_Time_Get(void)
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


    ctrl_inf.hum.end_time = stemp;
    sprintf(show_buf, "%02d:%02d",  ctrl_inf.hum.end_time / 60, ctrl_inf.hum.end_time % 60);
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, GUI_ID_EDIT5), show_buf);
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
                Set_Output_Callback(Start_Hum_Get);
                Set_Input_Len(3);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT1:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(Stop_Hum_Get);
                Set_Input_Len(3);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT2:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(Max_Temp_Get);
                Set_Input_Len(3);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT3:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(Min_Hum_Get);
                Set_Input_Len(3);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT4:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(Start_Time_Get);
                Set_Input_Len(4);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT5:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(Stop_Time_Get);
                Set_Input_Len(4);
                Input_Task(hWin);
            }

            break;			

        case GUI_ID_BUTTON2:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                if (Config_Save(SHUM_ID) < 0) {
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
WM_HWIN MCtrl_HumFix_Set;
void MCtrl_HumFix_Set_Task(WM_HWIN hParent)
{
    MCtrl_HumFix_Set = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    Current_HWIN = MCtrl_HumFix_Set;
}

