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
#include "recode.h"

/*********************************************************************
*
*       static data
*
**********************************************************************
*/


#define X  50
#define Y  50
#define X_Space     180
#define Y_Space     45
#define X_SIZE     170


#define GUI_ID_CHECK10 (GUI_ID_USER + 1)
#define GUI_ID_CHECK11 (GUI_ID_USER + 2)
#define GUI_ID_CHECK12 (GUI_ID_USER + 3)
#define GUI_ID_CHECK13 (GUI_ID_USER + 4)
#define GUI_ID_CHECK14 (GUI_ID_USER + 5)
#define GUI_ID_CHECK15 (GUI_ID_USER + 6)
#define GUI_ID_CHECK16 (GUI_ID_USER + 7)
#define GUI_ID_CHECK17 (GUI_ID_USER + 8)
#define GUI_ID_CHECK18 (GUI_ID_USER + 9)
#define GUI_ID_CHECK19 (GUI_ID_USER + 10)
#define GUI_ID_CHECK20 (GUI_ID_USER + 11)
#define GUI_ID_CHECK21 (GUI_ID_USER + 12)
#define GUI_ID_CHECK22 (GUI_ID_USER + 13)
#define GUI_ID_CHECK23 (GUI_ID_USER + 14)
#define GUI_ID_CHECK24 (GUI_ID_USER + 15)
#define GUI_ID_CHECK25 (GUI_ID_USER + 16)
#define GUI_ID_CHECK26 (GUI_ID_USER + 17)
#define GUI_ID_CHECK27 (GUI_ID_USER + 18)
#define GUI_ID_CHECK28 (GUI_ID_USER + 19)
#define GUI_ID_CHECK29 (GUI_ID_USER + 20)
#define GUI_ID_CHECK30 (GUI_ID_USER + 21)
#define GUI_ID_CHECK31 (GUI_ID_USER + 22)






/*********************************************************************
*
*       Dialog resource
*
* This table conatins the info required to create the dialog.
* It has been created by ucGUIbuilder.
*/


static U8 select_id = 0;
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect,    "",               0,                       0,  0,  800, 480, 0, 0},
    { TEXT_CreateIndirect,      "报警屏蔽", GUI_ID_TEXT0,            250, 0, 300, 42, 0, 0},

    { CHECKBOX_CreateIndirect,  "启用",         GUI_ID_CHECK0,            X + X_Space * 0, 	Y + Y_Space * 0, X_SIZE, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",         GUI_ID_CHECK1,            X + X_Space * 0,	Y + Y_Space * 1, X_SIZE, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",         GUI_ID_CHECK2,            X + X_Space * 0,	Y + Y_Space * 2, X_SIZE, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",         GUI_ID_CHECK3,            X + X_Space * 0,	Y + Y_Space * 3, X_SIZE, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",         GUI_ID_CHECK4,            X + X_Space * 0, 	Y + Y_Space * 4, X_SIZE, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",         GUI_ID_CHECK5,            X + X_Space * 0,	Y + Y_Space * 5, X_SIZE, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",         GUI_ID_CHECK6,            X + X_Space * 0, 	Y + Y_Space * 6, X_SIZE, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",         GUI_ID_CHECK7,            X + X_Space * 0, 	Y + Y_Space * 7, X_SIZE, CHECK_SIZE, 0, 0},

    { CHECKBOX_CreateIndirect,  "启用",         GUI_ID_CHECK8,            X + X_Space * 1, 	Y + Y_Space * 0, X_SIZE, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",         GUI_ID_CHECK9,            X + X_Space * 1,	Y + Y_Space * 1, X_SIZE, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",         GUI_ID_CHECK10,            X + X_Space * 1,	Y + Y_Space * 2, X_SIZE, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",         GUI_ID_CHECK11,            X + X_Space * 1,	Y + Y_Space * 3, X_SIZE, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",         GUI_ID_CHECK12,            X + X_Space * 1, Y + Y_Space * 4, X_SIZE, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",         GUI_ID_CHECK13,            X + X_Space * 1,	Y + Y_Space * 5, X_SIZE, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",         GUI_ID_CHECK14,            X + X_Space * 1, Y + Y_Space * 6, X_SIZE, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",         GUI_ID_CHECK15,            X + X_Space * 1, Y + Y_Space * 7, X_SIZE, CHECK_SIZE, 0, 0},

    { CHECKBOX_CreateIndirect,  "启用",         GUI_ID_CHECK16,            X + X_Space * 2, Y + Y_Space * 0, X_SIZE, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",         GUI_ID_CHECK17,            X + X_Space * 2,	Y + Y_Space * 1, X_SIZE, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",         GUI_ID_CHECK18,            X + X_Space * 2,	Y + Y_Space * 2, X_SIZE, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",         GUI_ID_CHECK19,            X + X_Space * 2,	Y + Y_Space * 3, X_SIZE, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",         GUI_ID_CHECK20,            X + X_Space * 2, Y + Y_Space * 4, X_SIZE, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",         GUI_ID_CHECK21,            X + X_Space * 2,	Y + Y_Space * 5, X_SIZE, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",         GUI_ID_CHECK22,            X + X_Space * 2, Y + Y_Space * 6, X_SIZE, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",         GUI_ID_CHECK23,            X + X_Space * 2, Y + Y_Space * 7, X_SIZE, CHECK_SIZE, 0, 0},

    { CHECKBOX_CreateIndirect,  "启用",         GUI_ID_CHECK24,            X + X_Space * 3, Y + Y_Space * 0, X_SIZE, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",         GUI_ID_CHECK25,            X + X_Space * 3,	Y + Y_Space * 1, X_SIZE, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",         GUI_ID_CHECK26,            X + X_Space * 3,	Y + Y_Space * 2, X_SIZE, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",         GUI_ID_CHECK27,            X + X_Space * 3,	Y + Y_Space * 3, X_SIZE, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",         GUI_ID_CHECK28,            X + X_Space * 3, Y + Y_Space * 4, X_SIZE, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",         GUI_ID_CHECK29,            X + X_Space * 3,	Y + Y_Space * 5, X_SIZE, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",         GUI_ID_CHECK30,            X + X_Space * 3, Y + Y_Space * 6, X_SIZE, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",         GUI_ID_CHECK31,            X + X_Space * 3, Y + Y_Space * 7, X_SIZE, CHECK_SIZE, 0, 0},


    { BUTTON_CreateIndirect,    "上一页",          GUI_ID_BUTTON0,          MENU_BACK_X1, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
    { BUTTON_CreateIndirect,    "下一页",          GUI_ID_BUTTON1,          MENU_BACK_X2, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
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
//    GUI_SetColor(GUI_LINE_COLOR);
//    GUI_DrawRoundedRect(90, 55, 710, 390, 10);
//    GUI_DrawRoundedRect(86, 51, 714, 394, 10);

}

static void Set_OptValue(WM_HWIN hWin, U8 sid)
{
    int MDev_ID = 0, id = sid * 32;
    WM_HWIN hItem;

    for (MDev_ID = GUI_ID_CHECK0; MDev_ID <= GUI_ID_CHECK9; MDev_ID++, id++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);

        if (id > ALA_MAX - 1) {
            WM_HideWindow(hItem);
            continue;
        }

        WM_ShowWindow(hItem);

        if (id >= 2 && id <= 9) {
            sprintf(show_buf, "%d#%s", (id - 1), GUI_LANG_GetTextEx(alarm_show[id], Language_Bit));
        } else if (id >= 33 && id <= 40) {
            sprintf(show_buf, "%d#%s", (id - 32), GUI_LANG_GetTextEx(alarm_show[id], Language_Bit));
        } else {
            sprintf(show_buf, "%s", GUI_LANG_GetTextEx(alarm_show[id], Language_Bit));
        }
        CHECKBOX_SetText(hItem, show_buf);
        CHECKBOX_SetState(hItem, getbit(soft_inf.ala_en, id));
    }

    for (MDev_ID = GUI_ID_CHECK10; MDev_ID <= GUI_ID_CHECK31; MDev_ID++, id++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);

        if (id > ALA_MAX - 1) {
            WM_HideWindow(hItem);
            continue;
        }

        WM_ShowWindow(hItem);

        if (id >= 2 && id <= 9) {
            sprintf(show_buf, "%d#%s", (id - 1), GUI_LANG_GetTextEx(alarm_show[id], Language_Bit));
        } else if (id >= 33 && id <= 40) {
            sprintf(show_buf, "%d#%s", (id - 32), GUI_LANG_GetTextEx(alarm_show[id], Language_Bit));
        } else {
            sprintf(show_buf, "%s", GUI_LANG_GetTextEx(alarm_show[id], Language_Bit));
        }
        CHECKBOX_SetText(hItem, show_buf);
        CHECKBOX_SetState(hItem, getbit(soft_inf.ala_en, id));
    }

    Button_CurShow(hWin, sid, 2);
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

    for (MDev_ID = GUI_ID_BUTTON0; MDev_ID <= GUI_ID_BUTTON2; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, Cut_cbButton);
    }

    for (MDev_ID = GUI_ID_TEXT0; MDev_ID <= GUI_ID_TEXT0; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24(hItem);

        if (MDev_ID == GUI_ID_TEXT0) {
            TEXT_SetTextAlign(hItem,  GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetTextColor(hItem, GUI_WHITE);
        }
    }

    for (MDev_ID = GUI_ID_CHECK0; MDev_ID <= GUI_ID_CHECK9; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Check_SetHZ24(hItem);
        Login_Show(hItem, LOGIN_ADMIN);
    }

    for (MDev_ID = GUI_ID_CHECK10; MDev_ID <= GUI_ID_CHECK31; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Check_SetHZ24(hItem);
        Login_Show(hItem, LOGIN_ADMIN);
    }

    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT0), GUI_LANG_GetTextEx(Lang_ALA_Shield, Language_Bit));
    
    Set_OptValue(hWin, select_id);
}


static void Get_OptValue(WM_HWIN hWin, U8 sid)
{
    int MDev_ID = 0, id = sid * 32;
    WM_HWIN hItem;

    for (MDev_ID = GUI_ID_CHECK0; MDev_ID <= GUI_ID_CHECK9; MDev_ID++, id++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);

        if (id > ALA_MAX - 1) {
            continue;
        }

        if (CHECKBOX_GetState(hItem) == 1) {
            setbit(soft_inf.ala_en, id);
        } else {
            clrbit(soft_inf.ala_en, id);
        }
    }

    for (MDev_ID = GUI_ID_CHECK10; MDev_ID <= GUI_ID_CHECK31; MDev_ID++, id++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);

        if (id > ALA_MAX - 1) {
            continue;
        }

        if (CHECKBOX_GetState(hItem) == 1) {
            setbit(soft_inf.ala_en, id);
        } else {
            clrbit(soft_inf.ala_en, id);
        }
    }
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

        case GUI_ID_BUTTON0:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                if (select_id == 1) {
                    Get_OptValue(hWin, select_id);
                    select_id = 0;
                    Set_OptValue(hWin, select_id);
                }
            }

            break;

        case GUI_ID_BUTTON1:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                if (select_id == 0) {
                    Get_OptValue(hWin, select_id);
                    select_id = 1;
                    Set_OptValue(hWin, select_id);
                }
            }

            break;

        case GUI_ID_BUTTON2:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Get_OptValue(hWin, select_id);
				
				if(getbit(soft_inf.ala_en, ALA_WINPOS) == 1){
					ALA_Set_State(ALA_WINPOS, ALA_STA_OFF);
				}
				
                if (Config_Save(SSOFT_ID) < 0) {
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
WM_HWIN MDev_Ala_En;
void MDev_Ala_En_Task(WM_HWIN hParent)
{
    select_id = 0;
    MDev_Ala_En = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    Current_HWIN = MDev_Ala_En;
}

