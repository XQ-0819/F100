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

static U8 select_id = 0;

/*********************************************************************
*
*       static data
*
**********************************************************************
*/

static void Set_Value(WM_HWIN hWin, U8 sid)
{

    sprintf(show_buf, "%d", ctrl_inf.sencon[sid].delay_min);
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT1), (const char *) show_buf);
    sprintf(show_buf, "%.1f%s", ctrl_inf.sencon[sid].temp_drop * 1.0f / TEMP_RATE,GUI_LANG_GetTextEx(Lang_oC, Language_Bit));
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT3), (const char *) show_buf);
    sprintf(show_buf, "%d", ctrl_inf.sencon[sid].continue_sec);
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT5), (const char *) show_buf);

    switch (sid) {
    default:
    case SENCON_HUM:
        sprintf(show_buf, "%d%%", ctrl_inf.sencon[sid].target);
        EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT0), (const char *) show_buf);
        sprintf(show_buf, "%d%%", ctrl_inf.sencon[sid].alaH);
        EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT2), (const char *) show_buf);
        sprintf(show_buf, "%d%%", ctrl_inf.sencon[sid].dead_zone);
        EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT4), (const char *) show_buf);
        sprintf(show_buf, "%d%%", ctrl_inf.sencon[sid].alaL);
        EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT6), (const char *) show_buf);

        break;

    case SENCON_CO2:
    case SENCON_NH3:
        sprintf(show_buf, "%dppm", ctrl_inf.sencon[sid].target);
        EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT0), (const char *) show_buf);
        sprintf(show_buf, "%dppm", ctrl_inf.sencon[sid].alaH);
        EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT2), (const char *) show_buf);
        sprintf(show_buf, "%dppm", ctrl_inf.sencon[sid].dead_zone);
        EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT4), (const char *) show_buf);

        break;

    }
}


static void Set_OptValue(WM_HWIN hWin, U8 sid)
{
    //增加是否启用
    CHECKBOX_SetState(WM_GetDialogItem(hWin, GUI_ID_CHECK0 ), ctrl_inf.sencon[sid].enable);
    CHECKBOX_SetState(WM_GetDialogItem(hWin, GUI_ID_CHECK1 ), ctrl_inf.sencon[sid].heat_flag);
    Set_Value(hWin, sid);

    switch (sid) {
    default:
    case SENCON_HUM:
        sprintf(show_buf, "%s-%s", GUI_LANG_GetTextEx(Lang_Control, Language_Bit), GUI_LANG_GetTextEx(Lang_HumCtrl, Language_Bit));
        TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT0), show_buf);
        CHECKBOX_SetText(WM_GetDialogItem(hWin, GUI_ID_CHECK0), GUI_LANG_GetTextEx(Lang_HumCtrl, Language_Bit));

        break;
    case SENCON_CO2:

        sprintf(show_buf, "%s-%s", GUI_LANG_GetTextEx(Lang_Control, Language_Bit), GUI_LANG_GetTextEx(Lang_CO2Ctrl, Language_Bit));
        TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT0), show_buf);
        CHECKBOX_SetText(WM_GetDialogItem(hWin, GUI_ID_CHECK0), GUI_LANG_GetTextEx(Lang_CO2Ctrl, Language_Bit));
        TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT1), GUI_LANG_GetTextEx(Lang_TargetPPM, Language_Bit));
        TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT3), GUI_LANG_GetTextEx(Lang_Halarm, Language_Bit));

        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT7 ));
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT6 ));
        break;

    case SENCON_NH3:

        sprintf(show_buf, "%s-%s", GUI_LANG_GetTextEx(Lang_Control, Language_Bit), GUI_LANG_GetTextEx(Lang_NH3Ctrl, Language_Bit));
        TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT0), show_buf);
        TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT0), show_buf);
        CHECKBOX_SetText(WM_GetDialogItem(hWin, GUI_ID_CHECK0), GUI_LANG_GetTextEx(Lang_NH3Ctrl, Language_Bit));
        TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT1), GUI_LANG_GetTextEx(Lang_TargetPPM, Language_Bit));
        TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT3), GUI_LANG_GetTextEx(Lang_Halarm, Language_Bit));

        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT7 ));
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT6 ));
        break;
    }
}



/*********************************************************************
*
*       Dialog resource
*
* This table conatins the info required to create the dialog.
* It has been created by ucGUIbuilder.
*/
#define XPos_Collec  (50)
#define YPos_Space   (60)

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect,    "",               0,                       0,  0,  800, 480, 0, 0},
    { TEXT_CreateIndirect,      "控制管理-湿度控制", GUI_ID_TEXT0,           250, 4, 300, 42, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用除湿",       		GUI_ID_CHECK0,           		110 + XPos_Collec, 66 + YPos_Space * 0, 150, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "低于加热开启",          	GUI_ID_CHECK1,           370 + XPos_Collec, 66 + YPos_Space * 0, 250, CHECK_SIZE, 0, 0},

    { TEXT_CreateIndirect,      "目标湿度:",    	GUI_ID_TEXT1,            110 + XPos_Collec, 66 + YPos_Space * 1, 140, 36, 0, 0},
    { TEXT_CreateIndirect,      "延迟(分):",       	GUI_ID_TEXT2,            110 + XPos_Collec, 66 + YPos_Space * 2, 140, 36, 0, 0},
    { TEXT_CreateIndirect,      "高湿报警:",      	GUI_ID_TEXT3,            110 + XPos_Collec, 66 + YPos_Space * 3, 110, 36, 0, 0},
    { TEXT_CreateIndirect,      "降温幅度:",     	GUI_ID_TEXT4,            110 + XPos_Collec, 66 + YPos_Space * 4, 130, 50, 0, 0},

    { EDIT_CreateIndirect,		 "",		   GUI_ID_EDIT0,			 215 + XPos_Collec, 66 + YPos_Space * 1, 115, 36, 0, 0},
    { EDIT_CreateIndirect,		 "",		   GUI_ID_EDIT1,			 215 + XPos_Collec, 66 + YPos_Space * 2, 115, 36, 0, 0},
    { EDIT_CreateIndirect,		 "",		   GUI_ID_EDIT2,			215 + XPos_Collec, 66 + YPos_Space * 3, 115, 36, 0, 0},
    { EDIT_CreateIndirect,		 "",			GUI_ID_EDIT3,		  215 + XPos_Collec, 66 + YPos_Space * 4, 115, 36, 0, 0},

    { TEXT_CreateIndirect,      "回    差 :",    GUI_ID_TEXT5,                370 + XPos_Collec, 66 + YPos_Space * 1, 120, 36, 0, 0},
    { TEXT_CreateIndirect,      "持续(秒):",    GUI_ID_TEXT6,              370 + XPos_Collec, 66 + YPos_Space * 2, 120, 36, 0, 0},
    { TEXT_CreateIndirect,      "低湿报警:",      GUI_ID_TEXT7,           370 + XPos_Collec, 66 + YPos_Space * 3, 120, 36, 0, 0},


    { EDIT_CreateIndirect,		 "",		   GUI_ID_EDIT4,			 500 + XPos_Collec, 66 + YPos_Space * 1, 115, 36, 0, 0},
    { EDIT_CreateIndirect,		 "",		   GUI_ID_EDIT5,			 500 + XPos_Collec, 66 + YPos_Space * 2, 115, 36, 0, 0},
    { EDIT_CreateIndirect,		 "",		   GUI_ID_EDIT6,			500 + XPos_Collec, 66 + YPos_Space * 3, 115, 36, 0, 0},


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
//    WM_HWIN hWin = pMsg->hWin;

    Show_MBackPic();
    GUI_SetColor(GUI_LINE_COLOR);
    GUI_DrawRoundedRect(80, 55, 712, 386, 10);
    GUI_DrawRoundedRect(76, 52, 716, 389, 10);

    //GUI_DrawLine(80,260,712,260);
    //GUI_DrawLine(80,264,712,264);
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

    for (MDev_ID = GUI_ID_EDIT0; MDev_ID <= GUI_ID_EDIT6; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Edit_SetHZ24(hItem);
    }

    for (MDev_ID = GUI_ID_CHECK0; MDev_ID <= GUI_ID_CHECK1; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Check_SetHZ24(hItem);
        Login_Show(hItem, LOGIN_ADMIN);
    }

    for (MDev_ID = GUI_ID_TEXT0; MDev_ID <= GUI_ID_TEXT7; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24L(hItem);

        if (MDev_ID == GUI_ID_TEXT0) {
            TEXT_SetTextAlign(hItem,  GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetTextColor(hItem, GUI_WHITE);
        }
    }
    sprintf(show_buf, "%s:", GUI_LANG_GetTextEx(Lang_Tar_RH, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT1), show_buf); // 目标湿度
    sprintf(show_buf, "%s(Min):", GUI_LANG_GetTextEx(Lang_Delay, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT2), show_buf); // 延时(Min)
    sprintf(show_buf, "%s:", GUI_LANG_GetTextEx(Lang_Hum_HA, Language_Bit));
    
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT3), show_buf);                                             // 高湿报警
    TEXT_SetWrapMode(WM_GetDialogItem(hWin, GUI_ID_TEXT4), GUI_WRAPMODE_WORD);
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT4), GUI_LANG_GetTextEx(Lang_Cooling_Range, Language_Bit)); // 降温幅度
    sprintf(show_buf, "%s:", GUI_LANG_GetTextEx(Lang_DropOff, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT5), show_buf);                                        // 回差
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT6), GUI_LANG_GetTextEx(Lang_Duration, Language_Bit)); // 持续(秒)
    sprintf(show_buf,"%s:", GUI_LANG_GetTextEx(Lang_Hum_LA, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT7), show_buf);   // 低湿报警

    WM_MoveWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT0), 30, 0);
    WM_MoveWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT1), 30, 0);
    WM_MoveWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT2), 30, 0);
    WM_MoveWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT3), 30, 0);

    CHECKBOX_SetText(WM_GetDialogItem(hWin, GUI_ID_CHECK1), GUI_LANG_GetTextEx(Lang_Below_HeatT, Language_Bit)); // 低于加热温度开启
    Set_OptValue(hWin, select_id);

}

static U8 temp_id = 0;
static void SenTarget_Get(void)
{
    float ftemp = Get_Input_Value();
    short stemp = (short)ftemp;

    if (select_id == SENCON_CO2) {
        if (stemp > CO2_MAX) {
            MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
            stemp = CO2_MAX;
        }
    } else {
        if (stemp > HUM_MAX) {
            MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
            stemp = HUM_MAX;
        }
    }

    if (stemp < 0) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 0;
    }

    if (temp_id == 0) {
        ctrl_inf.sencon[select_id].target = stemp;
    } else {
        ctrl_inf.sencon[select_id].dead_zone = stemp;
    }

    Set_Value(Current_HWIN, select_id);
}

static void SenAla_Get(void)
{
    float ftemp = Get_Input_Value();
    short stemp = (short)ftemp;

    if (select_id == SENCON_CO2) {
        if (stemp > CO2_MAX) {
            MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
            stemp = CO2_MAX;
        }
    } else {
        if (stemp > HUM_MAX) {
            MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
            stemp = HUM_MAX;
        }
    }

    if (stemp < 0) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 0;
    }

    if (temp_id == 0) {
        ctrl_inf.sencon[select_id].alaH = stemp;
    } else {
        ctrl_inf.sencon[select_id].alaL = stemp;
    }

    Set_Value(Current_HWIN, select_id);
}

static void SenDelay_Get(void)
{
    float ftemp = Get_Input_Value();
    short stemp = (short)ftemp;

//    if (stemp > HUM_MAX) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = HUM_MAX;
//    }

    if (stemp < 0) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 0;
    }

    if (temp_id == 0) {
        ctrl_inf.sencon[select_id].delay_min = stemp;
    } else {
        ctrl_inf.sencon[select_id].continue_sec = stemp;
    }

    Set_Value(Current_HWIN, select_id);
}

static void SenTemp_Get(void)
{
    float ftemp = Get_Input_Value() * TEMP_RATE;
    short stemp = (short)ftemp;

    if (stemp > TEMP_ADJMAX) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = TEMP_ADJMAX;
    }

    if (stemp < 0) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 0;
    }


    ctrl_inf.sencon[select_id].temp_drop = stemp;
    Set_Value(Current_HWIN, select_id);
}

static void Get_OptValue(WM_HWIN hWin, U8 sid)
{
    ctrl_inf.sencon[sid].enable =  CHECKBOX_GetState(WM_GetDialogItem(hWin, GUI_ID_CHECK0 ));
    ctrl_inf.sencon[sid].heat_flag =  CHECKBOX_GetState(WM_GetDialogItem(hWin, GUI_ID_CHECK1 ));
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

        if (old_Id != Id) {
            old_Id = Id;
            break;
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

        case GUI_ID_BUTTON2:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Get_OptValue(hWin, select_id);

                if (Config_Save(SSENCON_ID) < 0) {
                    MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_SAVE_ER));
                }

                Current_HWIN = WM_GetParent(hWin);
                GUI_EndDialog(hWin, 0);
            }

            break;


        case GUI_ID_EDIT0:
        case GUI_ID_EDIT4:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                temp_id = (Id - GUI_ID_EDIT0) / 4;
                Set_Output_Callback(SenTarget_Get);
                Set_Input_Len(4);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT1:
        case GUI_ID_EDIT5:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                temp_id = (Id - GUI_ID_EDIT0) / 4;
                Set_Output_Callback(SenDelay_Get);
                Set_Input_Len(3);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT2:
        case GUI_ID_EDIT6:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                temp_id = (Id - GUI_ID_EDIT0) / 4;
                Set_Output_Callback(SenAla_Get);
                Set_Input_Len(4);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT3:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(SenTemp_Get);
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

void Set_OTSensor_Type(U8 type)
{
    if (type > SENCON_NH3) {
        select_id = SENCON_HUM;
        return;
    }

    select_id = type;
}
/*********************************************************************
*
*       MainTask
*
**********************************************************************
*/
WM_HWIN MCtrl_OTSensor;
void MCtrl_OTSensor_Task(WM_HWIN hParent)
{
    MCtrl_OTSensor = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    Current_HWIN = MCtrl_OTSensor;
}

