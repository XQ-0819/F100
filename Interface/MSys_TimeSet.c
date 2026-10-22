#include "menuinc.h"
#include <stdio.h>
#include <string.h>
#include "pcf8563.h"

static void Set_Value(WM_HWIN hWin)
{
    if (basic_inf.update_mode == 0) {
        BUTTON_SetText(WM_GetDialogItem(hWin, GUI_ID_BUTTON0), GUI_LANG_GetTextEx(Lang_Active_upload, Language_Bit));
    } else {
        BUTTON_SetText(WM_GetDialogItem(hWin, GUI_ID_BUTTON0), GUI_LANG_GetTextEx(Lang_Passive_acquisition, Language_Bit));
    }

    sprintf(show_buf, "%d", basic_inf.update_time);
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT0), (const char *) show_buf);

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
    { WINDOW_CreateIndirect,  "",           		0,                        0, 0, 800, 480, 0, 0},
    { TEXT_CreateIndirect,      "时间设置",          GUI_ID_TEXT0,           250, 4, 300, 42, 0, 0},

    { TEXT_CreateIndirect,      "数据通信模式:", 		GUI_ID_TEXT1,          	 210, 45 + 42, 210, 50, 0, 0},
    { BUTTON_CreateIndirect,    "主动上传",             GUI_ID_BUTTON0,          410, 45 + 42, 140, 50, 0, 0},

    { TEXT_CreateIndirect,      "数据上传间隔:",        GUI_ID_TEXT2,            210, 120 + 42, 210, 36, 0, 0},
    { EDIT_CreateIndirect,       NULL,               	GUI_ID_EDIT0,            410, 120 + 42, 120, 36, 0, 0},
    { TEXT_CreateIndirect,      "Min",            		GUI_ID_TEXT3,            540, 120 + 42, 150, 36, 0, 0},

//    { TEXT_CreateIndirect,      "日龄更新时刻:",        GUI_ID_TEXT4,            210, 190 + 42, 210, 36, 0, 0},
//    { EDIT_CreateIndirect,       NULL,               	GUI_ID_EDIT1,            410, 190 + 42, 120, 36, 0, 0},

    { BUTTON_CreateIndirect,    "系统时间",          GUI_ID_BUTTON1,          	210, 190 + 42, 140, 50, 0, 0},
    { TEXT_CreateIndirect,      "时间格式:年(后两位)月日时分秒",  GUI_ID_TEXT4,            210, 260 + 42, 400, 36, 0, 0},

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
    GUI_DrawRoundedRect(160, 25 + 42, 650, 340 + 42, 15);
    GUI_DrawRoundedRect(157, 22 + 42, 653, 343 + 42, 15);
}
/*****************************************************************
**      FunctionName:void InitDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**
**      call this function in _cbCallback --> WM_INIT_DIALOG
*****************************************************************/
static void InitDialog(WM_MESSAGE * pMsg)
{
//    CHECKBOX_SKINFLEX_PROPS fix;
    WM_HWIN hWin = pMsg->hWin;
    int MDev_ID = 0;
    WM_HWIN hItem;

    for (MDev_ID = GUI_ID_BUTTON2; MDev_ID <= GUI_ID_BUTTON2; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, Cut_cbButton);
    }

    for (MDev_ID = GUI_ID_BUTTON0; MDev_ID <= GUI_ID_BUTTON1; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Button_SetHZ24(hItem);
    }

    for (MDev_ID = GUI_ID_TEXT0; MDev_ID <= GUI_ID_TEXT4; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24L(hItem);

        if (MDev_ID == GUI_ID_TEXT0) {
            TEXT_SetTextAlign(hItem,  GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetTextColor(hItem, GUI_WHITE);
        }
    }

    TEXT_SetTextColor(WM_GetDialogItem(hWin, GUI_ID_TEXT4), GUI_LIGHTGRAY);

    for (MDev_ID = GUI_ID_EDIT0; MDev_ID <= GUI_ID_EDIT0; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Edit_SetHZ24(hItem);
    }

    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT0), GUI_LANG_GetTextEx(Lang_Time_Set, Language_Bit));      // 时间设置
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT1), GUI_LANG_GetTextEx(Lang_Upload_mode, Language_Bit));   // 数据通信模式:
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT2), GUI_LANG_GetTextEx(Lang_Time_interval, Language_Bit)); // 数据上传间隔:
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT4), GUI_LANG_GetTextEx(Lang_Time_style, Language_Bit));    // 时间格式:年(后两位)月日时分秒
    BUTTON_SetText(WM_GetDialogItem(hWin, GUI_ID_BUTTON1), GUI_LANG_GetTextEx(Lang_SysTime, Language_Bit));   // 系统时间

    Set_Value(hWin);
}

void Reset_Sync_Time(void)
{
    time_t sys_time;
	
    sys_time = GetSysTime();

    if ((sys_time % DAY_TO_SEC) > basic_inf.dayupdate_time * MIN_TO_SEC) {
        basic_inf.group_time = sys_time - (sys_time % DAY_TO_SEC) + DAY_TO_SEC;
    } else {
        basic_inf.group_time = sys_time - (sys_time % DAY_TO_SEC);
    }

    if ((sys_time % DAY_TO_SEC) > basic_inf.dataupdate_time * MIN_TO_SEC) {
        basic_inf.data_time = sys_time - (sys_time % DAY_TO_SEC) + DAY_TO_SEC;
    } else {
        basic_inf.data_time = sys_time - (sys_time % DAY_TO_SEC);
    }	
}

static void Sys_Time_Get(void)
{
    TDATE_TIME	dt;

    unsigned char *time_buf = Get_Input_Text();
    unsigned char len = strlen((const char *)time_buf);

    if (len != 12) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        return;
    }

    dt.year   = (time_buf[0] - 0x30) * 0x10;
    dt.year   += (time_buf[1] - 0x30);
    dt.mon  = (time_buf[2] - 0x30) * 0x10;
    dt.mon  += (time_buf[3] - 0x30);
    dt.day    = (time_buf[4] - 0x30) * 0x10;
    dt.day    += (time_buf[5] - 0x30);
    dt.hour   = (time_buf[6] - 0x30) * 0x10;
    dt.hour   += (time_buf[7] - 0x30);
    dt.min = (time_buf[8] - 0x30) * 0x10;
    dt.min += (time_buf[9] - 0x30);
    dt.sec = (time_buf[10] - 0x30) * 0x10;
    dt.sec += (time_buf[11] - 0x30);

    if (Bcd2Hex(dt.year) < 0) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        return ;
    }

    if (Bcd2Hex(dt.mon) < 0 || Bcd2Hex(dt.mon) > 12) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        return ;
    }

    if (Bcd2Hex(dt.day) < 0 || Bcd2Hex(dt.day) > 31) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        return ;
    }

    if (Bcd2Hex(dt.hour) < 0 || Bcd2Hex(dt.hour) > 23) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        return ;
    }

    if (Bcd2Hex(dt.min) < 0 || Bcd2Hex(dt.min) > 59) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        return ;
    }

    if (Bcd2Hex(dt.sec) < 0 || Bcd2Hex(dt.sec) > 59) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        return ;
    }

    if (PCF8563_SetDT( &dt ) < 0) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_SAVE_ER));
        return;
    }

	Reset_Sync_Time();
	
    MWarning_Task(Current_HWIN, Get_Tips(TIPS_TIME_OK));

    if (Config_Save(SBASIC_ID) < 0) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_SAVE_ER));
    }
}

static void UPTime_Get(void)
{
    float ftemp = Get_Input_Value();
    short stemp = (short)ftemp;

    if (stemp > 255) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 255;
    }

    if (stemp <= 0) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 1;
    }

    basic_inf.update_time = stemp;
    Set_Value(Current_HWIN);
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
                && get_login_permiss(LOGIN_SUPADMIN) < 0) {
            MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_NO_LOGIN));
            break;
        }

		if (Id >= GUI_ID_BUTTON0 && Id <= GUI_ID_BUTTON1 && NCode == WM_NOTIFICATION_RELEASED
                && get_login_permiss(LOGIN_SUPADMIN) < 0) {
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
                Set_Output_Callback(UPTime_Get);
                Set_Input_Len(3);
                Input_Task(hWin);
            }

            break;

//        case GUI_ID_EDIT1:
//            if (NCode == WM_NOTIFICATION_RELEASED) {
//                Set_Output_Callback(Time_Get);
//                Set_Input_Len(4);
//                Input_Task(hWin);
//            }

//            break;

        case GUI_ID_BUTTON0:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                basic_inf.update_mode = (basic_inf.update_mode + 1) % 2;

                if (basic_inf.update_mode == 0) {
                    BUTTON_SetText(WM_GetDialogItem(hWin, GUI_ID_BUTTON0), GUI_LANG_GetTextEx(Lang_Active_upload, Language_Bit));
                } else {
                    BUTTON_SetText(WM_GetDialogItem(hWin, GUI_ID_BUTTON0), GUI_LANG_GetTextEx(Lang_Passive_acquisition, Language_Bit));
                }
            }

            break;

        case GUI_ID_BUTTON1:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(Sys_Time_Get);
                Set_Input_Len(12);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_BUTTON2:
            if (NCode == WM_NOTIFICATION_RELEASED) {
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
WM_HWIN MSys_TimeSet;
void MSys_TimeSet_Task(WM_HWIN hParent)
{
    MSys_TimeSet = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    Current_HWIN = MSys_TimeSet;
}

