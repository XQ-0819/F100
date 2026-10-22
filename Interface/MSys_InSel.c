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
#include "ucos_ii.h"
#include "bsp.h"

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
    { WINDOW_CreateIndirect,    "",               0,                       150,  60,  500, 300, 0, 0},
    { TEXT_CreateIndirect,      "导入选择",  GUI_ID_TEXT0,              170, 0, 160, 42, 0, 0},

    { TEXT_CreateIndirect,      "生产数据:",    GUI_ID_TEXT1,              130, 70, 170, 30, 0, 0},
    { TEXT_CreateIndirect,      "风窗参数:",    GUI_ID_TEXT2,              130, 130, 170, 30, 0, 0},



    { CHECKBOX_CreateIndirect,  "导入",          GUI_ID_CHECK0,             300, 70, 130, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "导入",          GUI_ID_CHECK1,             300, 130, 130, CHECK_SIZE, 0, 0},


    { BUTTON_CreateIndirect,    "确定",             GUI_ID_BUTTON2,        190, 240, 120, 40, 0, 0},
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
    //GUI_ID_BUTTON0
    //
    WINDOW_SetBkColor(hWin, GUI_GRAY_E7);

//    WINDOW_SetBkColor(hWin, GUI_BLUE_DEF);

    for (MDev_ID = GUI_ID_TEXT0; MDev_ID <= GUI_ID_TEXT2; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24L(hItem);

        if (MDev_ID == GUI_ID_TEXT0) {
            TEXT_SetTextAlign(hItem,  GUI_TA_HCENTER | GUI_TA_VCENTER);
        }
    }

    for (MDev_ID = GUI_ID_BUTTON2; MDev_ID <= GUI_ID_BUTTON2; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Button_SetHZ24(hItem);
    }

    for (MDev_ID = GUI_ID_CHECK0; MDev_ID <= GUI_ID_CHECK1; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        CHECKBOX_SetText(hItem, GUI_LANG_GetTextEx(Lang_Import, Language_Bit));
        Check_SetHZ24(hItem);
        Login_Show(hItem, LOGIN_ADMIN);
        CHECKBOX_SetState(hItem, 1);
    }

    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT0), GUI_LANG_GetTextEx(Lang_Import_Select, Language_Bit)); // 导入选择
    sprintf(show_buf, "%s:", GUI_LANG_GetTextEx(Lang_Production, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT1), show_buf);                                            // 生产数据:
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT2), GUI_LANG_GetTextEx(Lang_Curt_Setting, Language_Bit)); // 风窗参数:
    BUTTON_SetText(WM_GetDialogItem(hWin, GUI_ID_BUTTON2), GUI_LANG_GetTextEx(Lang_Done, Language_Bit));     // 确定
}

extern U8 setting_select[4];

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
                setting_select[0] = CHECKBOX_GetState(WM_GetDialogItem(hWin, GUI_ID_CHECK0 ));
                setting_select[1] = CHECKBOX_GetState(WM_GetDialogItem(hWin, GUI_ID_CHECK1 ));
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
WM_HWIN MSys_InSel;
U8 MSys_InSel_Task(WM_HWIN hParent)
{
    WM_DIALOG_STATUS DialogStatus = {0};

    MSys_InSel = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    WM_MakeModal(MSys_InSel);  //模态化这个窗口
    GUI_SetDialogStatusPtr(MSys_InSel, &DialogStatus);

    while (!DialogStatus.Done) {
        task_time[ucgui_id] = 0;
        GUI_Exec();
        OSTimeDly(10);
        IWDG_ReloadCounter();
    }


    return DialogStatus.ReturnValue;
}

