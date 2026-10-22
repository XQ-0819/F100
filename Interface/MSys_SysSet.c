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
    { TEXT_CreateIndirect,      "系统设置",          GUI_ID_TEXT0,           250, 4, 300, 42, 0, 0},

    { TEXT_CreateIndirect,      "屏保时间/s:", 	 GUI_ID_TEXT1,          220, 90, 180, 36, 0, 0},
    { EDIT_CreateIndirect,       NULL,           GUI_ID_EDIT0,          440, 90, 120, 36, 0, 0},

    { TEXT_CreateIndirect,      "主页风格:", 	 GUI_ID_TEXT2,          220, 150, 180, 36, 0, 0},
    { DROPDOWN_CreateIndirect,	 "",			 GUI_ID_DROPDOWN0,		440, 150, 120, 120, 0, 0},

    { TEXT_CreateIndirect,      "触摸声音:", 	 GUI_ID_TEXT3,          220, 150, 180, 36, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",        GUI_ID_CHECK0,        	440, 150 + 3, 160, CHECK_SIZE, 0, 0},
//    { TEXT_CreateIndirect,      "温度简化:", 	 GUI_ID_TEXT4,          220, 260, 160, 36, 0, 0},
//    { CHECKBOX_CreateIndirect,  "启用",        GUI_ID_CHECK1,        	440, 260 + 3, 160, CHECK_SIZE, 0, 0},

	{ TEXT_CreateIndirect,      "系统语言:",       GUI_ID_TEXT4,            220, 210, 200, 36, 0, 0},
	{ DROPDOWN_CreateIndirect,	 "",			 GUI_ID_DROPDOWN1,		440, 210, 150, 120, 0, 0},

//    { TEXT_CreateIndirect,      "最小计量重量/kg:", GUI_ID_TEXT5,       220, 360, 200, 36, 0, 0},
//    { EDIT_CreateIndirect,       NULL,           GUI_ID_EDIT1,          440, 360, 120, 36, 0, 0},

    { BUTTON_CreateIndirect,    "通讯设置",      GUI_ID_BUTTON3,    	 MENU_BACK_X1, MENU_BACK_Y, 120, 50, 0, 0},
    { BUTTON_CreateIndirect,    "参数管理",      GUI_ID_BUTTON4,    	 220, 310, 120, 50, 0, 0},


//    { BUTTON_CreateIndirect,    "上一页",          GUI_ID_BUTTON0,          MENU_BACK_X1, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
    { BUTTON_CreateIndirect,    "版本信息",          GUI_ID_BUTTON1,          MENU_BACK_X2, MENU_BACK_Y, 120, 50, 0, 0},
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
    GUI_DrawRoundedRect(160, 70, 650, 400, 15);
    GUI_DrawRoundedRect(157, 67, 653, 403, 15);
}


static void SS_Time_Get(void)
{
    float ftemp = Get_Input_Value();
    unsigned short stemp = (unsigned short)ftemp;

    if (stemp > TIME_3BIT) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = TIME_3BIT;
    }

//    if (stemp < 0) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = 0;
//    }

    basic_inf.ss_time = stemp;
    sprintf(show_buf, "%d", basic_inf.ss_time);
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, GUI_ID_EDIT0), show_buf);
}

/*****************************************************************
**      FunctionName:void InitDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**
**      call this function in _cbCallback --> WM_INIT_DIALOG
*****************************************************************/

static void InitDialog(WM_MESSAGE *pMsg)
{
    WM_HWIN hWin = pMsg->hWin;

    int MDev_ID = 0;
    WM_HWIN hItem;
    u8 i = 0;
    for (MDev_ID = GUI_ID_BUTTON2; MDev_ID <= GUI_ID_BUTTON2; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, Cut_cbButton);
    }

    for (MDev_ID = GUI_ID_BUTTON1; MDev_ID <= GUI_ID_BUTTON4; MDev_ID++) {
        if (MDev_ID == GUI_ID_BUTTON2)
            continue;

        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Button_SetHZ24(hItem);
    }

    for (MDev_ID = GUI_ID_EDIT0; MDev_ID <= GUI_ID_EDIT0; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Edit_SetHZ24(hItem);
    }

    for (MDev_ID = GUI_ID_TEXT0; MDev_ID <= GUI_ID_TEXT4; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24L(hItem);

        if (MDev_ID == GUI_ID_TEXT0) {
            TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetTextColor(hItem, GUI_WHITE);
        }
    }

    for (MDev_ID = GUI_ID_CHECK0; MDev_ID <= GUI_ID_CHECK0; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        CHECKBOX_SetText(hItem, GUI_LANG_GetTextEx(Lang_Enable, Language_Bit));
        Check_SetHZ24(hItem);
        Login_Show(hItem, LOGIN_ADMIN);
    }

    CHECKBOX_SetState(WM_GetDialogItem(hWin, GUI_ID_CHECK0), basic_inf.touch_beep);

    for (MDev_ID = GUI_ID_DROPDOWN0; MDev_ID <= GUI_ID_DROPDOWN1; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Dropdown_SetHZ24(hItem);
        Login_Show(hItem, LOGIN_SUPADMIN);
        if (MDev_ID == GUI_ID_DROPDOWN1) {
            DROPDOWN_SetFont(hItem, &GUI_FontBC_16);
            DROPDOWN_SetItemSpacing(hItem, 8);
            for (i = 0; i < LANGUAGE_NUM; i++) {
                DROPDOWN_AddString(hItem, GUI_LANG_GetTextEx(Lang_English, i + 1));
            }
            DROPDOWN_SetUpMode(hItem, 1);
            DROPDOWN_SetSel(hItem, Language_Bit_original);
        }
    }

    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT0), GUI_LANG_GetTextEx(Lang_System, Language_Bit));          // 系统设置
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT1), GUI_LANG_GetTextEx(Lang_Pause_time, Language_Bit));      // 屏保时间/s:
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT2), GUI_LANG_GetTextEx(Lang_Home_Style, Language_Bit));      // 主页风格:
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT3), GUI_LANG_GetTextEx(Lang_BEEP, Language_Bit));            // 触摸声音:
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT4), GUI_LANG_GetTextEx(Lang_System_language, Language_Bit)); // 系统语言:

    BUTTON_SetText(WM_GetDialogItem(hWin, GUI_ID_BUTTON3), GUI_LANG_GetTextEx(Lang_Comm_Set, Language_Bit));         // 通讯设置
    BUTTON_SetText(WM_GetDialogItem(hWin, GUI_ID_BUTTON4), GUI_LANG_GetTextEx(Lang_Para_Manager, Language_Bit));     // 参数管理
    BUTTON_SetText(WM_GetDialogItem(hWin, GUI_ID_BUTTON1), GUI_LANG_GetTextEx(Lang_Software_Version, Language_Bit)); // 版本信息
    sprintf(show_buf, "%s%d", GUI_LANG_GetTextEx(Lang_Style, Language_Bit), 1);
    DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN0), show_buf); // 风格1
    sprintf(show_buf, "%s%d", GUI_LANG_GetTextEx(Lang_Style, Language_Bit), 2);
    DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN0), show_buf); // 风格2

    DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN0), basic_inf.home_style);

    sprintf(show_buf, "%d", basic_inf.ss_time);
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT0), show_buf);

	WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT2));
	WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN0));
	
}

/*********************************************************************
*
*       Dialog callback routine
*/
static void _cbCallback(WM_MESSAGE * pMsg)
{
    int NCode, Id, style;
    U8 ret = 0;

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

        case GUI_ID_EDIT0:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(SS_Time_Get);
                Set_Input_Len(3);
                Input_Task(hWin);
            }

            break;
			
        case GUI_ID_BUTTON1:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                MMessage_Task(hWin, MESSAGE_SOFT_VER);
            }

            break;

        case GUI_ID_BUTTON2:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                style = DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN0));
                basic_inf.touch_beep = CHECKBOX_GetState(WM_GetDialogItem(hWin, GUI_ID_CHECK0));

                if (basic_inf.home_style != style) {
                    ret = MMessage_Task(hWin, MESSAGE_HOME_STYLE);

                    if (ret == MESSAGE_YES) {
                        basic_inf.home_style = style;
                        sys_sta.delay_rst = 3;
                    }
                }

                if (Config_Save(SBASIC_ID) < 0) {
                    MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_SAVE_ER));
                }
				
                Current_HWIN = WM_GetParent(hWin);
                GUI_EndDialog(hWin, 0);
            }

            break;

        case GUI_ID_BUTTON3:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                MSys_CommSet_Task(hWin);
            }

			break;
			
        case GUI_ID_BUTTON4:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                MSys_ParMana_Task(hWin);
            }

			break;
			
		case GUI_ID_DROPDOWN1:
            if (NCode == WM_NOTIFICATION_SEL_CHANGED) {
                ret = MMessage_Task(hWin, MESSAGE_SAVE_PARA);

                if (ret == MESSAGE_YES) {
                    Language_Bit_original = DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN1));
                    Config_Save(SBASIC_ID);
                    GUI_EndDialog(HBasic_Main, 0);
                    Creat_Basic_Main();
                } else {
                    DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN1), Language_Bit_original);
                }
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
WM_HWIN MSys_SysSet;
void MSys_SysSet_Task(WM_HWIN hParent)
{
    MSys_SysSet = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    Current_HWIN = MSys_SysSet;
}

