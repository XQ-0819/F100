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
#include "datasave.h"

//EventsFunctionList
//EndofEventsFunctionList


/*********************************************************************
*
*       static data
*
**********************************************************************
*/
#define GUI_ID_BUTTON12 (GUI_ID_USER + 3)
#define GUI_ID_BUTTON13 (GUI_ID_USER + 4)
#define GUI_ID_BUTTON14 (GUI_ID_USER + 5)
#define GUI_ID_BUTTON15 (GUI_ID_USER + 6)


/*********************************************************************
*
*       Dialog resource
*
* This table conatins the info required to create the dialog.
* It has been created by ucGUIbuilder.
*/
static const u16 Lang_para_info[DATA_WINTER + 1] = {
    Lang_factoryparm, Lang_springparm, Lang_summerparm, Lang_autumnparm, Lang_winterparm
};
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect,    "",               0,                       0,  0,  800, 480, 0, 0},
    { TEXT_CreateIndirect,      "系统管理-参数管理",          GUI_ID_TEXT0,           250, 4, 300, 42, 0, 0},

    { TEXT_CreateIndirect,      "当前运行:",        GUI_ID_TEXT1,              90, 70, 280, 30, 0, 0},

    { TEXT_CreateIndirect,      "春季参数\n保存时间:NULL",         GUI_ID_TEXT2,       90, 115, 300, 60, 0, 0},
    { TEXT_CreateIndirect,      "夏季参数\n保存时间:NULL",         GUI_ID_TEXT3,       90, 190, 300, 60, 0, 0},
    { TEXT_CreateIndirect,      "秋季参数\n保存时间:NULL",         GUI_ID_TEXT4,       90, 265, 300, 60, 0, 0},
    { TEXT_CreateIndirect,      "冬季参数\n保存时间:NULL",         GUI_ID_TEXT5,       90, 340, 300, 60, 0, 0},

    { BUTTON_CreateIndirect,    "保存",          GUI_ID_BUTTON12,          400, 120, 100, 50, 0, 0},
    { BUTTON_CreateIndirect,    "保存",          GUI_ID_BUTTON13,          400, 195, 100, 50, 0, 0},
    { BUTTON_CreateIndirect,    "保存",          GUI_ID_BUTTON14,          400, 270, 100, 50, 0, 0},
    { BUTTON_CreateIndirect,    "保存",          GUI_ID_BUTTON15,          400, 345, 100, 50, 0, 0},

    { BUTTON_CreateIndirect,    "导入",          GUI_ID_BUTTON6,          600, 120, 100, 50, 0, 0},
    { BUTTON_CreateIndirect,    "导入",          GUI_ID_BUTTON7,          600, 195, 100, 50, 0, 0},
    { BUTTON_CreateIndirect,    "导入",          GUI_ID_BUTTON8,          600, 270, 100, 50, 0, 0},
    { BUTTON_CreateIndirect,    "导入",          GUI_ID_BUTTON9,          600, 345, 100, 50, 0, 0},

    { BUTTON_CreateIndirect,    "返回",        	 GUI_ID_BUTTON2,         MENU_BACK_X3, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
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
    GUI_DrawRoundedRect(66, 55, 740, 410, 10);
    GUI_DrawRoundedRect(62, 52, 744, 413, 10);
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
    int MDev_ID = 0, i = 0;
    WM_HWIN hItem;
    struct tm *t;

    for (MDev_ID = GUI_ID_BUTTON2; MDev_ID <= GUI_ID_BUTTON2; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, _cbButtonBack);
    }

    for (MDev_ID = GUI_ID_BUTTON6; MDev_ID <= GUI_ID_BUTTON9; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Button_SetHZ24(hItem);
    }

    for (MDev_ID = GUI_ID_BUTTON12; MDev_ID <= GUI_ID_BUTTON15; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Button_SetHZ24(hItem);
    }
	
    for (MDev_ID = GUI_ID_TEXT0; MDev_ID <= GUI_ID_TEXT5; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24L(hItem);

        if (MDev_ID == GUI_ID_TEXT0) {
            TEXT_SetTextAlign(hItem,  GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetTextColor(hItem, GUI_WHITE);
        }
    }

    sprintf(show_buf, "%s-%s", GUI_LANG_GetTextEx(Lang_System, Language_Bit), GUI_LANG_GetTextEx(Lang_Para_Manager, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT0), show_buf); // 系统管理-参数管理
    sprintf(show_buf, "%s:%s", GUI_LANG_GetTextEx(Lang_Current_Run, Language_Bit), GUI_LANG_GetTextEx(Lang_para_info[basic_inf.cur_season], Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT1), (const char *)show_buf);
    for (i = 0; i < 4; i++) {
        sprintf(show_buf, "%s\n%s:NULL", GUI_LANG_GetTextEx(Lang_springparm + i, Language_Bit), GUI_LANG_GetTextEx(Lang_save_time, Language_Bit));
        TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT2 + i), show_buf); // 春季参数\n保存时间:NULL
    }

    for (MDev_ID = GUI_ID_BUTTON12; MDev_ID <= GUI_ID_BUTTON15; MDev_ID++) {
        BUTTON_SetText(WM_GetDialogItem(hWin, MDev_ID), GUI_LANG_GetTextEx(Lang_Save, Language_Bit)); // 保存
    }

    for (MDev_ID = GUI_ID_BUTTON6; MDev_ID <= GUI_ID_BUTTON9; MDev_ID++) {
        BUTTON_SetText(WM_GetDialogItem(hWin, MDev_ID), GUI_LANG_GetTextEx(Lang_Import, Language_Bit)); // 导入
    }

    for (i = DATA_SPRING; i <= DATA_WINTER; i++) {
        if (Read_SeasonPara_Time(i - DATA_SPRING) < 0) {
            continue;
        }

        t = localtime((const time_t *)&season_inf.save_time);

        sprintf(show_buf, "%s\n%s:%02d/%02d/%02d %02d:%02d", GUI_LANG_GetTextEx(Lang_para_info[i], Language_Bit), GUI_LANG_GetTextEx(Lang_save_time, Language_Bit), 
                                                            (t->tm_year - 100), (t->tm_mon + 1), t->tm_mday, t->tm_hour, t->tm_min);

        TEXT_SetText(WM_GetDialogItem(hWin, (GUI_ID_TEXT2 + i - DATA_SPRING)), (const char *) show_buf);
    }

}

/*********************************************************************
*
*       Dialog callback routine
*/
static void _cbCallback(WM_MESSAGE * pMsg)
{
    int NCode, Id, ret;
    struct tm *t;
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
        if (Id >= GUI_ID_BUTTON6 && Id <= GUI_ID_BUTTON9 && NCode == WM_NOTIFICATION_RELEASED
                && get_login_permiss(LOGIN_ADMIN) < 0) {
            MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_NO_LOGIN));
            break;
        }

        if (Id >= GUI_ID_BUTTON12 && Id <= GUI_ID_BUTTON15 && NCode == WM_NOTIFICATION_RELEASED
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


        case GUI_ID_BUTTON12:
        case GUI_ID_BUTTON13:
        case GUI_ID_BUTTON14:
        case GUI_ID_BUTTON15:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                ret = MMessage_Task(hWin, MESSAGE_SAVE_PARA);

                if (ret != MESSAGE_YES) {
                    break;
                }

                if (Write_SeasonPara(Id - GUI_ID_BUTTON12) < 0) {

                    MWarning_Task(hWin, GUI_LANG_GetTextEx(Lang_Tips0, Language_Bit));

                    break;
                }

                if (Read_SeasonPara_Time(Id - GUI_ID_BUTTON12) < 0) {

                    MWarning_Task(hWin, GUI_LANG_GetTextEx(Lang_Tips0, Language_Bit));

                    break;
                }

                t = localtime((const time_t *)&season_inf.save_time);

                sprintf(show_buf, "%s\n%s:%02d/%02d/%02d %02d:%02d", GUI_LANG_GetTextEx(Lang_para_info[Id - GUI_ID_BUTTON12 + DATA_SPRING], Language_Bit), GUI_LANG_GetTextEx(Lang_save_time, Language_Bit),
                                                                     (t->tm_year - 100), (t->tm_mon + 1), t->tm_mday, t->tm_hour, t->tm_min);
                TEXT_SetText(WM_GetDialogItem(hWin, (GUI_ID_TEXT2 + Id - GUI_ID_BUTTON12)), (const char *)show_buf);

                MWarning_Task(hWin, GUI_LANG_GetTextEx(Lang_Tips11, Language_Bit));
            }

            break;


        case GUI_ID_BUTTON6:
        case GUI_ID_BUTTON7:
        case GUI_ID_BUTTON8:
        case GUI_ID_BUTTON9:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                ret = MMessage_Task(hWin, MESSAGE_IMPORT_PARA);

                if (ret != MESSAGE_YES) {
                    break;
                }

                if (Read_SeasonPara(Id - GUI_ID_BUTTON6) < 0) {
                    MWarning_Task(hWin, GUI_LANG_GetTextEx(Lang_ImconfigF, Language_Bit));
                    break;
                }

                basic_inf.cur_season = DATA_SPRING + Id - GUI_ID_BUTTON6;
                if (Config_Save(SBASIC_ID) < 0) {
                    MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_SAVE_ER));
                }

                sprintf(show_buf, "%s:%s", GUI_LANG_GetTextEx(Lang_Current_Run, Language_Bit),GUI_LANG_GetTextEx(Lang_para_info[basic_inf.cur_season], Language_Bit));

                TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT1), (const char *)show_buf);
                Load_Param();

                MWarning_Task(hWin, GUI_LANG_GetTextEx(Lang_ImconfigS, Language_Bit));
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
WM_HWIN MSys_ParMana;
void MSys_ParMana_Task(WM_HWIN hParent)
{
    MSys_ParMana = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    Current_HWIN = MSys_ParMana;
}

