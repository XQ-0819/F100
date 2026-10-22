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
    { TEXT_CreateIndirect,      "模拟测试",          GUI_ID_TEXT0,            250, 4, 300, 42, 0, 0},

    { TEXT_CreateIndirect,      "当前级别:",    GUI_ID_TEXT1,              220 , 120, 150, 36, 0, 0},
    { TEXT_CreateIndirect,      "级别模拟:",    GUI_ID_TEXT2,              220 , 180, 140, 36, 0, 0},
    { TEXT_CreateIndirect,      "温度模拟:",    GUI_ID_TEXT3,              220 , 240, 140, 36, 0, 0},

    { CHECKBOX_CreateIndirect,  "启用",          GUI_ID_CHECK0,             350, 180 + 3, 100, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",          GUI_ID_CHECK1,             350, 240 + 3, 100, CHECK_SIZE, 0, 0},

    { EDIT_CreateIndirect,       "",            GUI_ID_EDIT0,          		480, 180, 70, 36, 0, 0},
    { EDIT_CreateIndirect,       "",            GUI_ID_EDIT1,            	480, 240, 70, 36, 0, 0},

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
    GUI_DrawRoundedRect(150, 80, 650, 330, 10);
    GUI_DrawRoundedRect(146, 77, 654, 333, 10);

    /*		  GUI_DrawLine(80,260,712,260);
    		  GUI_DrawLine(80,264,712,264);*/
}


static WM_HTIMER hTimer;

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

    for (MDev_ID = GUI_ID_BUTTON0; MDev_ID <= GUI_ID_BUTTON1; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Button_SetHZ24(hItem);
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
        CHECKBOX_SetText(hItem, GUI_LANG_GetTextEx(Lang_Enable, Language_Bit));
        Check_SetHZ24(hItem);
    }

    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT0), GUI_LANG_GetTextEx(Lang_Analogy_Test, Language_Bit)); // 模拟测试
    sprintf(show_buf, "%s:", GUI_LANG_GetTextEx(Lang_Cur_Lv, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT2), show_buf); // 当前级别:
    sprintf(show_buf, "%s:", GUI_LANG_GetTextEx(Lang_Temp_Simulate, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT3), show_buf); // 温度模拟:

    CHECKBOX_SetState(WM_GetDialogItem(hWin, GUI_ID_CHECK0 ), sys_sta.sl_set.lv_en);
    CHECKBOX_SetState(WM_GetDialogItem(hWin, GUI_ID_CHECK1 ), sys_sta.sl_set.temp_en);

    sprintf(show_buf, "%d", sys_sta.sl_set.lv + 1);
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT0), show_buf);

    sprintf(show_buf, "%.1f", (float)(sys_sta.sl_set.temp * 1.0f / (float)TEMP_RATE));
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT1), show_buf);

    sprintf(show_buf, "%s: %d", GUI_LANG_GetTextEx(Lang_Cur_Lv, Language_Bit), cal_inf.lv + 1);

    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT1), (const char *)show_buf);

    hTimer = WM_CreateTimer(WM_GetClientWindow(hWin), 0, 1000, 0);
}

static void LV_Get(void)
{
    float ftemp = Get_Input_Value() ;
    short stemp = (short)ftemp;

    if (stemp > CTRL_LV_NUM) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = CTRL_LV_NUM;
    }

    if (stemp < 1) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 1;
    }

    sys_sta.sl_set.lv = (stemp - 1);

    sprintf(show_buf, "%d", sys_sta.sl_set.lv + 1);
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, GUI_ID_EDIT0), show_buf);
}

static void Temp_Get(void)
{
    float ftemp = Get_Input_Value() * TEMP_RATE;
    short stemp = (short)ftemp;

    if (stemp > TEMP_MAX) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = TEMP_MAX;
    }

    if (stemp < TEMP_MIN) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = TEMP_MIN;
    }

    sys_sta.sl_set.temp = stemp;

    sprintf(show_buf, "%.1f", (float)(sys_sta.sl_set.temp * 1.0f / (float)TEMP_RATE));
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, GUI_ID_EDIT1), show_buf);
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

    case WM_TIMER:
        sprintf(show_buf, "%s: %d", GUI_LANG_GetTextEx(Lang_Cur_Lv, Language_Bit), cal_inf.lv + 1);
        TEXT_SetText(WM_GetDialogItem(Current_HWIN, GUI_ID_TEXT1), show_buf);
        WM_RestartTimer(hTimer, 1000);
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

        case GUI_ID_CHECK0:
            if (NCode == WM_NOTIFICATION_VALUE_CHANGED) {
                sys_sta.sl_set.lv_en = CHECKBOX_GetState(WM_GetDialogItem(hWin, GUI_ID_CHECK0 ));
            }

            break;

        case GUI_ID_CHECK1:
            if (NCode == WM_NOTIFICATION_VALUE_CHANGED) {
                sys_sta.sl_set.temp_en = CHECKBOX_GetState(WM_GetDialogItem(hWin, GUI_ID_CHECK1 ));
            }

            break;

        case GUI_ID_BUTTON2:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                sys_sta.sl_set.lv_en = CHECKBOX_GetState(WM_GetDialogItem(hWin, GUI_ID_CHECK0));
                sys_sta.sl_set.temp_en = CHECKBOX_GetState(WM_GetDialogItem(hWin, GUI_ID_CHECK1));
                Current_HWIN = WM_GetParent(hWin);
                GUI_EndDialog(hWin, 0);
            }

            break;

        case GUI_ID_EDIT0:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(LV_Get);
                Set_Input_Len(2);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT1:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(Temp_Get);
                Set_Input_Len(4);
                Input_Task(hWin);
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
WM_HWIN MSys_SLTS;
void MSys_SLTS_Task(WM_HWIN hParent)
{
    MSys_SLTS = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    Current_HWIN = MSys_SLTS;
}

