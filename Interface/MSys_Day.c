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
#include "pcf8563.h"
#include "datasave.h"
#endif
#include <string.h>

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
    { TEXT_CreateIndirect,      "动物日龄",          GUI_ID_TEXT0,            250, 4, 300, 42, 0, 0},

    { TEXT_CreateIndirect,      "设置日龄:",          GUI_ID_TEXT1,            240, 80, 160, 36, 0, 0},
    { TEXT_CreateIndirect,      "当前群号:",          GUI_ID_TEXT2,            240, 130, 200, 36, 0, 0},
    { EDIT_CreateIndirect,       NULL,                GUI_ID_EDIT0,            420, 80, 72, 36, 0, 0},
    { BUTTON_CreateIndirect,    "重置",           	  GUI_ID_BUTTON3,          200, 330, 130, 50, 0, 0},
    { BUTTON_CreateIndirect,    "新群",           	  GUI_ID_BUTTON4,          450, 330, 130, 50, 0, 0},


    { TEXT_CreateIndirect,      "日龄更新时刻:",        GUI_ID_TEXT3,            240, 180, 210, 36, 0, 0},
    { EDIT_CreateIndirect,       NULL,               	GUI_ID_EDIT1,            420, 180, 120, 36, 0, 0},

    { TEXT_CreateIndirect,      "数据更新时刻:",        GUI_ID_TEXT4,            240, 230, 210, 36, 0, 0},
    { EDIT_CreateIndirect,       NULL,               	GUI_ID_EDIT2,            420, 230, 120, 36, 0, 0},

    { CHECKBOX_CreateIndirect,  "启用",              GUI_ID_CHECK0,              240, 280, 200, CHECK_SIZE, 0, 0},


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
    Show_MBackPic();

    GUI_SetColor(GUI_LINE_COLOR);
    GUI_DrawRoundedRect(160, 70, 650, 390, 15);
    GUI_DrawRoundedRect(157, 67, 653, 393, 15);
}

static void Grow_Day_Get(void)
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

    basic_inf.grow_day = stemp;
    sys_sta.update_hour = MAX_1BYTE;

    sprintf(show_buf, "%d", basic_inf.grow_day);
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, GUI_ID_EDIT0), show_buf);

#if USE_VS == 0

#endif
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

    //
    for (MDev_ID = GUI_ID_BUTTON2; MDev_ID <= GUI_ID_BUTTON2; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, Cut_cbButton);
    }

    for (MDev_ID = GUI_ID_BUTTON3; MDev_ID <= GUI_ID_BUTTON4; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Button_SetHZ24(hItem);
    }


    for (MDev_ID = GUI_ID_EDIT0; MDev_ID <= GUI_ID_EDIT2; MDev_ID++) {
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

    for (MDev_ID = GUI_ID_CHECK0; MDev_ID <= GUI_ID_CHECK0; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        CHECKBOX_SetText(hItem, GUI_LANG_GetTextEx(Lang_Clear_Record, Language_Bit));
        Check_SetHZ24(hItem);
        Login_Show(hItem, LOGIN_ADMIN);
    }

    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT0), GUI_LANG_GetTextEx(Lang_DayAge_Setting, Language_Bit)); // 动物日龄
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT1), GUI_LANG_GetTextEx(Lang_Day_Age, Language_Bit));        // 设置日龄:

    sprintf(show_buf, "%s:          %d", GUI_LANG_GetTextEx(Lang_GroupId, Language_Bit), basic_inf.group);
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT2), (const char *)show_buf); // 当前群号

    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT3), GUI_LANG_GetTextEx(Lang_Age_update_time, Language_Bit));  // 日龄更新时刻:
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT4), GUI_LANG_GetTextEx(Lang_Data_update_time, Language_Bit)); // 数据更新时刻:

    BUTTON_SetText(WM_GetDialogItem(hWin, GUI_ID_BUTTON3), GUI_LANG_GetTextEx(Lang_Reset, Language_Bit)); // 重置
    BUTTON_SetText(WM_GetDialogItem(hWin, GUI_ID_BUTTON4), GUI_LANG_GetTextEx(Lang_New, Language_Bit));   // 新群

    sprintf(show_buf, "%d", basic_inf.grow_day);
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT0), show_buf);

    sprintf(show_buf, "%02d:%02d", basic_inf.dayupdate_time / 60, basic_inf.dayupdate_time % 60);
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT1), (const char *) show_buf);
	
    sprintf(show_buf, "%02d:%02d", basic_inf.dataupdate_time / 60, basic_inf.dataupdate_time % 60);
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT2), (const char *) show_buf);

}


static void Time_Get(void)
{
    float ftemp = Get_Input_Value() ;
    short stemp = (short)ftemp;

    stemp = (stemp / 100) * 60 + (stemp % 100);

    if (stemp > (MAX_TIME_DAY-29)) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = (MAX_TIME_DAY-29);
    }

    if (stemp < 0) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 0;
    }

    basic_inf.dayupdate_time = stemp;

    sprintf(show_buf, "%02d:%02d", basic_inf.dayupdate_time / 60, basic_inf.dayupdate_time % 60);
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, GUI_ID_EDIT1), (const char *) show_buf);
}

static void DataTime_Get(void)
{
    float ftemp = Get_Input_Value() ;
    short stemp = (short)ftemp;

    stemp = (stemp / 100) * 60 + (stemp % 100);

    if (stemp > (MAX_TIME_DAY-29)) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = (MAX_TIME_DAY-29);
    }

    if (stemp < 0) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 0;
    }

    basic_inf.dataupdate_time = stemp;

    sprintf(show_buf, "%02d:%02d", basic_inf.dataupdate_time / 60, basic_inf.dataupdate_time % 60);
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, GUI_ID_EDIT2), (const char *) show_buf);
}

/*********************************************************************
*
*       Dialog callback routine
*/
static void _cbCallback(WM_MESSAGE * pMsg)
{
    int NCode, Id;
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
				
        if (Id >= GUI_ID_EDIT0 && Id <= GUI_ID_EDIT9 && NCode == WM_NOTIFICATION_RELEASED
                && get_login_permiss(LOGIN_ADMIN) < 0) {
            MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_NO_LOGIN));
            break;
        }

        if (Id >= GUI_ID_BUTTON3 && Id <= GUI_ID_BUTTON4 && NCode == WM_NOTIFICATION_RELEASED
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
                if (Config_Save(SBASIC_ID) < 0) {
                    MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_SAVE_ER));
                }

                Current_HWIN = WM_GetParent(hWin);
                GUI_EndDialog(hWin, 0);
            }

            break;

        case GUI_ID_EDIT0:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(Grow_Day_Get);
                Set_Input_Len(3);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT1:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(Time_Get);
                Set_Input_Len(4);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT2:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(DataTime_Get);
                Set_Input_Len(4);
                Input_Task(hWin);
            }

            break;
			
        case GUI_ID_BUTTON3:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                ret = MMessage_Task(hWin, MESSAGE_MEM_GROUP_CLC);

                if (ret == MESSAGE_YES) {
                    basic_inf.group = 1;
                    sprintf(show_buf, "%s:          %d", GUI_LANG_GetTextEx(Lang_GroupId, Language_Bit), basic_inf.group);
                    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT2), show_buf);
                }
            }

            break;

        case GUI_ID_BUTTON4:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                if (CHECKBOX_GetState(WM_GetDialogItem(hWin, GUI_ID_CHECK0)) == 0) {
                    ret = MMessage_Task(hWin, CONFIRM_NEW_GROUP);

                    if (ret == MESSAGE_YES) {
                        basic_inf.group++;

                        if (basic_inf.group >= MAX_GROUP_NUM) {
                            basic_inf.group = 1;
                        }

                        basic_inf.survival = 0;
                        basic_inf.total = 0;
                        sprintf(show_buf, "%s:          %d", GUI_LANG_GetTextEx(Lang_GroupId, Language_Bit), basic_inf.group);
                        TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT2), show_buf);
                        break;
                    }

                    break;
                }

//	清除历史记录
                ret = MMessage_Task(hWin, MESSAGE_NEW_GROUP);

                if (ret == MESSAGE_YES) {
                    basic_inf.group++;

                    if (basic_inf.group >= MAX_GROUP_NUM) {
                        basic_inf.group = 1;
                    }

                    basic_inf.survival = 0;
                    basic_inf.total = 0;
                    sprintf(show_buf, "%s:          %d", GUI_LANG_GetTextEx(Lang_GroupId, Language_Bit), basic_inf.group);
                    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT2), show_buf);
#if USE_VS == 0
                    Group_Def();
#endif
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
WM_HWIN MSys_Day;
void MSys_Day_Task(WM_HWIN hParent)
{
    MSys_Day = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    Current_HWIN = MSys_Day;
}

