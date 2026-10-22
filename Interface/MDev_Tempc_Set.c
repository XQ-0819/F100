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

static U8 select_mode = 0;
/*********************************************************************
*
*       Dialog resource
*
* This table conatins the info required to create the dialog.
* It has been created by ucGUIbuilder.
*/

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect,    "",               0,                       0,  0,  800, 480, 0, 0},
    { TEXT_CreateIndirect,      "设备管理-温控开关",          GUI_ID_TEXT0,           250, 4, 300, 42, 0, 0},

    { TEXT_CreateIndirect,      "低温报警温度/℃:",      GUI_ID_TEXT1,            X + X_Space * 0, 	Y + Y_Space * 0, 180, 36, 0, 0},
    { TEXT_CreateIndirect,      "强制散热水温/℃:",      GUI_ID_TEXT3,            X + X_Space * 0 , 	Y + Y_Space * 1, 200, 36, 0, 0},
//    { TEXT_CreateIndirect,      "小窗联动(时控):",      GUI_ID_TEXT3,            X + X_Space * 0, 	Y + Y_Space * 2, 190, 36, 0, 0},
//    { TEXT_CreateIndirect,      "风机开启延时/S:",      GUI_ID_TEXT4,            X + X_Space * 0, 	Y + Y_Space * 3, 190, 36, 0, 0},
//    { TEXT_CreateIndirect,      "报警延时时间/S:",      GUI_ID_TEXT5,            X + X_Space * 0, 	Y + Y_Space * 4, 190, 36, 0, 0},


    { TEXT_CreateIndirect,      "高温报警温度/℃:",     GUI_ID_TEXT2,             X + X_Space * 1, 	Y + Y_Space * 0, 210, 36, 0, 0},
    { TEXT_CreateIndirect,      "暖风机启动水温/℃:",    GUI_ID_TEXT4,            X + X_Space * 1, 	Y + Y_Space * 1, 210, 36, 0, 0},
//    { TEXT_CreateIndirect,      "静压报警最小级别:",    GUI_ID_TEXT7,            X + X_Space * 1, 	Y + Y_Space * 2, 190, 36, 0, 0},
//    { TEXT_CreateIndirect,      "体感低温报警偏差:",      GUI_ID_TEXT10,           X + X_Space * 1, 	Y + Y_Space * 3, 190, 36, 0, 0},
//    { TEXT_CreateIndirect,      "体感高温报警偏差:",      GUI_ID_TEXT11,            X + X_Space * 1, 	Y + Y_Space * 4, 190, 36, 0, 0},

    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT0,            X + X_Space * 0 + 190, 	Y + Y_Space * 0, 72, 36, 0, 0},
    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT2,            X + X_Space * 0 + 190, 	Y + Y_Space * 1, 72, 36, 0, 0},
//    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT2,            X + X_Space * 0 + 180, 	Y + Y_Space * 3, 72, 36, 0, 0},
//    { CHECKBOX_CreateIndirect,  "启用",             GUI_ID_CHECK0,            X + X_Space * 0 + 180,    Y + Y_Space * 2, 90, CHECK_SIZE, 0, 0},
//    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT3,            X + X_Space * 0 + 180, 	Y + Y_Space * 4, 72, 36, 0, 0},

    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT1,            X + X_Space * 1 + 210, 	Y + Y_Space * 0, 72, 36, 0, 0},
    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT3,            X + X_Space * 1 + 210, 	Y + Y_Space * 1, 72, 36, 0, 0},
//    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT5,            X + X_Space * 1 + 200, 	Y + Y_Space * 2, 72, 36, 0, 0},
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

    for (MDev_ID = GUI_ID_EDIT0; MDev_ID <= GUI_ID_EDIT3; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Edit_SetHZ24(hItem);
    }

    for (MDev_ID = GUI_ID_TEXT0; MDev_ID <= GUI_ID_TEXT4; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24L(hItem);

        if (MDev_ID == GUI_ID_TEXT0) {
            TEXT_SetTextAlign(hItem,  GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetTextColor(hItem, GUI_WHITE);
        }
    }

    sprintf(show_buf, "%s-%s", GUI_LANG_GetTextEx(Lang_Device, Language_Bit), GUI_LANG_GetTextEx(Lang_Tswitch, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT0), show_buf);                                              // 设备管理-温控开关
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT1), GUI_LANG_GetTextEx(Lang_T_Low_Ala, Language_Bit));      // 低温报警温度/℃
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT2), GUI_LANG_GetTextEx(Lang_T_High_Ala, Language_Bit));     // 高温报警温度/℃:
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT3), GUI_LANG_GetTextEx(Lang_Forced_cooling, Language_Bit)); // 强制散热水温/℃
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT4), GUI_LANG_GetTextEx(Lang_Heat_WorkT, Language_Bit));     // 暖风机启动水温/℃

    if (select_mode == 0) {
        sprintf(show_buf, "%.1f", basic_inf.tempc_pwm_en.min_ala_temp * 1.0f / TEMP_RATE);
        EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT0), show_buf);

        sprintf(show_buf, "%.1f", basic_inf.tempc_pwm_en.max_ala_temp * 1.0f / TEMP_RATE);
        EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT1), show_buf);
		
        sprintf(show_buf, "%.1f", basic_inf.tempc_pwm_en.protect_temp * 1.0f / TEMP_RATE);
        EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT2), show_buf);

        sprintf(show_buf, "%.1f", basic_inf.tempc_pwm_en.start_temp * 1.0f / TEMP_RATE);
        EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT3), show_buf);

		if(basic_inf.tempc_pwm_en.mode == TEMPC_MODE_HIGH){
			WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT3));
			WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT4));
			WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT2));
			WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT3));
		}
	
    } else {
        sprintf(show_buf, "%.1f", basic_inf.tempc_en.min_ala_temp * 1.0f / TEMP_RATE);
        EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT0), show_buf);

        sprintf(show_buf, "%.1f", basic_inf.tempc_en.max_ala_temp * 1.0f / TEMP_RATE);
        EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT1), show_buf);
	
        sprintf(show_buf, "%.1f", basic_inf.tempc_en.protect_temp * 1.0f / TEMP_RATE);
        EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT2), show_buf);

        sprintf(show_buf, "%.1f", basic_inf.tempc_en.start_temp * 1.0f / TEMP_RATE);
        EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT3), show_buf);
		
		if(basic_inf.tempc_en.mode == TEMPC_MODE_HIGH){
			WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT3));
			WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT4));
			WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT2));
			WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT3));
		}		
    }

}

static void Min_Temp_Get(void)
{
    float ftemp = Get_Input_Value() * TEMP_RATE;
    short stemp = (short)ftemp;

    if (stemp > TIME_3BIT) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = TIME_3BIT;
    }

    if (stemp < 0) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 0;
    }

    if (select_mode == 0) {
        basic_inf.tempc_pwm_en.min_ala_temp = stemp;
        sprintf(show_buf, "%.1f", basic_inf.tempc_pwm_en.min_ala_temp * 1.0f / TEMP_RATE);
    } else {
        basic_inf.tempc_en.min_ala_temp = stemp;
        sprintf(show_buf, "%.1f", basic_inf.tempc_en.min_ala_temp * 1.0f / TEMP_RATE);
    }

    EDIT_SetText(WM_GetDialogItem(Current_HWIN, GUI_ID_EDIT0), show_buf);
}

static void Max_Temp_Get(void)
{
    float ftemp = Get_Input_Value() * TEMP_RATE;
    short stemp = (short)ftemp;

    if (stemp > TIME_3BIT) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = TIME_3BIT;
    }

    if (stemp < 0) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 0;
    }

    if (select_mode == 0) {
        basic_inf.tempc_pwm_en.max_ala_temp = stemp;
        sprintf(show_buf, "%.1f", basic_inf.tempc_pwm_en.max_ala_temp * 1.0f / TEMP_RATE);
    } else {
        basic_inf.tempc_en.max_ala_temp = stemp;
        sprintf(show_buf, "%.1f", basic_inf.tempc_en.max_ala_temp * 1.0f / TEMP_RATE);
    }

    EDIT_SetText(WM_GetDialogItem(Current_HWIN, GUI_ID_EDIT1), show_buf);
}

static void Protect_Temp_Get(void)
{
    float ftemp = Get_Input_Value() * TEMP_RATE;
    short stemp = (short)ftemp;

    if (stemp > TIME_3BIT) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = TIME_3BIT;
    }

    if (stemp < 0) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 0;
    }

    if (select_mode == 0) {
        basic_inf.tempc_pwm_en.protect_temp = stemp;
        sprintf(show_buf, "%.1f", basic_inf.tempc_pwm_en.protect_temp * 1.0f / TEMP_RATE);
    } else {
        basic_inf.tempc_en.protect_temp = stemp;
        sprintf(show_buf, "%.1f", basic_inf.tempc_en.protect_temp * 1.0f / TEMP_RATE);
    }

    EDIT_SetText(WM_GetDialogItem(Current_HWIN, GUI_ID_EDIT2), show_buf);
}

static void Start_Temp_Get(void)
{
    float ftemp = Get_Input_Value() * TEMP_RATE;
    short stemp = (short)ftemp;

    if (stemp > TIME_3BIT) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = TIME_3BIT;
    }

    if (stemp < 0) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 0;
    }

    if (select_mode == 0) {
        basic_inf.tempc_pwm_en.start_temp = stemp;
        sprintf(show_buf, "%.1f", basic_inf.tempc_pwm_en.start_temp * 1.0f / TEMP_RATE);
    } else {
        basic_inf.tempc_en.start_temp = stemp;
        sprintf(show_buf, "%.1f", basic_inf.tempc_en.start_temp * 1.0f / TEMP_RATE);
    }

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
                Set_Output_Callback(Min_Temp_Get);
                Set_Input_Len(4);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT1:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(Max_Temp_Get);
                Set_Input_Len(4);
                Input_Task(hWin);
            }

            break;
			
        case GUI_ID_EDIT2:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(Protect_Temp_Get);
                Set_Input_Len(3);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT3:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(Start_Temp_Get);
                Set_Input_Len(3);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_BUTTON2:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                if (select_mode == 0) {
                    if (Config_Save(STEMPC_PWM_ID) < 0) {
                        MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_SAVE_ER));
                    }
                } else {
                    if (Config_Save(STEMPC_ID) < 0) {
                        MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_SAVE_ER));
                    }
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
WM_HWIN MDev_Tempc_Set;
void MDev_Tempc_Set_Task(WM_HWIN hParent, U8 mode) //0-pwm ,1 - jdq
{
    select_mode = mode;
    MDev_Tempc_Set = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    Current_HWIN = MDev_Tempc_Set;
}

