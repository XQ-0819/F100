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
    { TEXT_CreateIndirect,      "设备管理-信号输入", GUI_ID_TEXT0,            250, 4, 300, 42, 0, 0},
    { TEXT_CreateIndirect,      "外部报警:",     	GUI_ID_TEXT1,            	190, 100, 150, 36, 0, 0},
    { CHECKBOX_CreateIndirect,    "启用",            GUI_ID_CHECK4,                360 , 100 + 3, 100, CHECK_SIZE, 0, 0},

    {  CHECKBOX_CreateIndirect,       "1.环控电箱",         GUI_ID_CHECK5,              210, 200, 190, CHECK_SIZE, 0, 1},
    {  CHECKBOX_CreateIndirect,       "2.喂料系统",         GUI_ID_CHECK6,              210, 250, 190, CHECK_SIZE, 0, 1},
    { CHECKBOX_CreateIndirect,         "3.清粪系统",          GUI_ID_CHECK7,           445 , 200, 190, CHECK_SIZE, 0, 1},
    { CHECKBOX_CreateIndirect,         "4.其它报警",          GUI_ID_CHECK8,           445, 250, 190, CHECK_SIZE, 0, 1},

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
    GUI_DrawRoundedRect(180, 70, 635, 330, 10);
    GUI_DrawRoundedRect(176, 66, 639, 334, 10);

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



    for (MDev_ID = GUI_ID_CHECK4; MDev_ID <= GUI_ID_CHECK8; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Check_SetHZ24(hItem);
        Login_Show(hItem, LOGIN_ADMIN);
    }

    CHECKBOX_SetText(WM_GetDialogItem(hWin, GUI_ID_CHECK4), GUI_LANG_GetTextEx(Lang_Enable, Language_Bit));
    sprintf(show_buf, "1.%s", GUI_LANG_GetTextEx(Lang_Electric_Box, Language_Bit));
    CHECKBOX_SetText(WM_GetDialogItem(hWin, GUI_ID_CHECK5), show_buf);
    sprintf(show_buf, "2.%s", GUI_LANG_GetTextEx(Lang_Feeding, Language_Bit));
    CHECKBOX_SetText(WM_GetDialogItem(hWin, GUI_ID_CHECK6), show_buf);
    sprintf(show_buf, "3.%s", GUI_LANG_GetTextEx(Lang_Nightsoil, Language_Bit));
    CHECKBOX_SetText(WM_GetDialogItem(hWin, GUI_ID_CHECK7), show_buf);
    sprintf(show_buf, "4.%s", GUI_LANG_GetTextEx(Lang_Other_Ala, Language_Bit));
    CHECKBOX_SetText(WM_GetDialogItem(hWin, GUI_ID_CHECK8), show_buf);

    sprintf(show_buf, "%s-%s", GUI_LANG_GetTextEx(Lang_Device, Language_Bit), GUI_LANG_GetTextEx(Lang_Signal, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT0), show_buf); // 设备管理-信号输入

    sprintf(show_buf, "%s:", GUI_LANG_GetTextEx(Lang_AlaIn, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT1), show_buf); // 外部报警

    for (MDev_ID = GUI_ID_TEXT0; MDev_ID <= GUI_ID_TEXT1; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24(hItem);

        if (MDev_ID == GUI_ID_TEXT0) {
            TEXT_SetTextAlign(hItem,  GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetTextColor(hItem, GUI_WHITE);
        }
    }

    CHECKBOX_SetState(WM_GetDialogItem(hWin, GUI_ID_CHECK4 ), getbit(ext_inf.din.alarm, 7));

    CHECKBOX_SetState(WM_GetDialogItem(hWin, GUI_ID_CHECK5 ), getbit(ext_inf.din.alarm, 0));
    CHECKBOX_SetState(WM_GetDialogItem(hWin, GUI_ID_CHECK6 ), getbit(ext_inf.din.alarm, 1));
    CHECKBOX_SetState(WM_GetDialogItem(hWin, GUI_ID_CHECK7 ), getbit(ext_inf.din.alarm, 2));
    CHECKBOX_SetState(WM_GetDialogItem(hWin, GUI_ID_CHECK8 ), getbit(ext_inf.din.alarm, 3));

}

static void Get_OptValue(WM_HWIN hWin)
{
    int MDev_ID = 0, id = 0;
    WM_HWIN hItem;

    if (CHECKBOX_GetState(WM_GetDialogItem(hWin, GUI_ID_CHECK4 )) == 1) {
        setbit(ext_inf.din.alarm, 7);
    } else {
        clrbit(ext_inf.din.alarm, 7);
    }

    for (id = 0, MDev_ID = GUI_ID_CHECK5; MDev_ID <= GUI_ID_CHECK8; MDev_ID++, id++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);

        if (CHECKBOX_GetState(hItem) == 1) {
            setbit(ext_inf.din.alarm, id);
        } else {
            clrbit(ext_inf.din.alarm, id);
        }
    }
}
void alarm_chk_pluse_water(void)
{
    int i = 0, num = 0;

    for (i = 0, num = 0; i < ALARM_NUM_MAX; i++) {
        if (getbit(ext_inf.din.alarm, i) == STA_ON) {
            num++;
        }
    }

    if (num > 1 && getbit(ext_inf.din.alarm, 7) == STA_ON) {
        if (ext_inf.din.pro_data[0].water_sta == WL_PULSE) {
            for (i = 1; i < PRO_DATA_MAX; i++) {
                ext_inf.din.pro_data[i].water_sta = WL_DIS;
            }
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

        case GUI_ID_BUTTON2:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Get_OptValue(hWin);
                //如果有多个报警就把多个脉冲水表后面的关闭
				alarm_chk_pluse_water();

                if (Config_Save(SDIN_ID) < 0) {
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
WM_HWIN MDev_SignalInput;
void MDev_SignalInput_Task(WM_HWIN hParent)
{
    MDev_SignalInput = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    Current_HWIN = MDev_SignalInput;
}

