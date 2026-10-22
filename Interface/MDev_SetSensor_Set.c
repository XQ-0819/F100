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
#include <string.h>
#include <time.h>
#include <stdlib.h>


#include "HEADER_Private.h"

//EndofEventsFunctionList


//static U8 selectX_id = 0;
//static U8 selectY_id = 0;

//static U8 real_lv_id = 0;
//static U8 real_list_id = 0;
//static U8 curY_id = 0;

/*********************************************************************
*
*       static data
*
**********************************************************************
*/

extern GUI_CONST_STORAGE GUI_BITMAP bmMEnable;
extern GUI_CONST_STORAGE GUI_BITMAP bmMDisable;


#define LIST_SIZE	(80)


/*********************************************************************
*
*       Dialog resource
*
* This table conatins the info required to create the dialog.
* It has been created by ucGUIbuilder.
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect,    "",               0,                       0,  0,  800, 480, 0, 0},
    { TEXT_CreateIndirect,      "设备管理-温度分区设置", GUI_ID_TEXT0,            280, 4, 240, 42, 0, 0},
    { LISTVIEW_CreateIndirect,   NULL,               GUI_ID_LISTVIEW0,        120, 100, 560, 200, LISTVIEW_CF_CELL_SELECT, 0},


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
}

static void listview_lv_sta(LISTVIEW_Handle listview0_hWin, U8 id, U8 add_flag)
{
    U8 i = 0, column = 0;
    GUI_COLOR Color = 0;

    if (id >= HEAT_NUM_MAX) {
        return;
    }

    if (id % 2 == 0) {
        Color = GUI_DEF_LGREEN;
    } else {
        Color = GUI_DEF_GRAY;
    }

    if (add_flag == STA_ON) {
        LISTVIEW_AddRow(listview0_hWin, NULL);
    }
    sprintf(show_buf, GUI_LANG_GetTextEx(Lang_Heat_Area, Language_Bit), id + 1);
    LISTVIEW_SetItemText(listview0_hWin, column, id, show_buf);

    LISTVIEW_SetItemBkColor(listview0_hWin, column++, id, LISTVIEW_CI_UNSEL, Color);

    for (i = 0; i < TEMP_NUM_MAX - 1; i++) {
        if (dev_inf.TEMP_Define[id][i] == STA_ON) {
            LISTVIEW_SetItemBitmap(listview0_hWin, column, id, 28, 13, &bmMEnable);
        } else {
            LISTVIEW_SetItemBitmap(listview0_hWin, column, id, 28, 13, &bmMDisable);
        }

        LISTVIEW_SetItemBkColor(listview0_hWin, column++, id, LISTVIEW_CI_UNSEL, Color);
    }
}



static void InitDialog(WM_MESSAGE * pMsg)
{
    WM_HWIN hWin = pMsg->hWin;
    int MDev_ID = 0;
    WM_HWIN hItem;
    WM_HWIN listview0_hWin;
    U8 i = 0;

    for (MDev_ID = GUI_ID_BUTTON2; MDev_ID <= GUI_ID_BUTTON2; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, _cbButtonBack);
    }

    for (MDev_ID = GUI_ID_TEXT0; MDev_ID <= GUI_ID_TEXT0; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24L(hItem);

        if (MDev_ID == GUI_ID_TEXT0) {
            TEXT_SetTextAlign(hItem,  GUI_TA_HCENTER | GUI_TA_VCENTER);
        }
    }

    sprintf(show_buf, "%s-%s", GUI_LANG_GetTextEx(Lang_Device, Language_Bit), GUI_LANG_GetTextEx(Lang_Temper_Def, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT0), show_buf);

    listview0_hWin = WM_GetDialogItem(hWin, GUI_ID_LISTVIEW0);
    HEADER_SetFont( LISTVIEW_GetHeader(listview0_hWin), &GUI_USER_HZ16);
    WM_DisableWindow(LISTVIEW_GetHeader(listview0_hWin));
    LISTVIEW_SetFont(listview0_hWin, &GUI_USER_HZ16);
    LISTVIEW_EnableCellSelect(listview0_hWin, 1);
    LISTVIEW_SetGridVis(listview0_hWin, 1);
    LISTVIEW_SetHeaderHeight(listview0_hWin, 46);
    LISTVIEW_SetRowHeight(listview0_hWin, 50);
    if (Language_Bit_original >= LANGUAGE_RUSSIAN) {
        HEADER_SetFont(LISTVIEW_GetHeader(listview0_hWin), &GUI_FontBC_16);
        LISTVIEW_SetFont(listview0_hWin, &GUI_FontBC_16);
    }

    LISTVIEW_AddColumn(listview0_hWin, LIST_SIZE, "", GUI_TA_VCENTER | GUI_TA_CENTER);
    for (i = 0; i < 6; i++) {

        sprintf(show_buf, "%s%d", GUI_LANG_GetTextEx(Lang_Temp, Language_Bit), i + 1);
        LISTVIEW_AddColumn(listview0_hWin, LIST_SIZE, show_buf, GUI_TA_VCENTER | GUI_TA_CENTER);
    }

    for (i = 0; i < HEAT_NUM_MAX; i++) {
        listview_lv_sta(listview0_hWin, i , 1);
    }

}


/*********************************************************************
*
*       Dialog callback routine
*/
static void _cbCallback(WM_MESSAGE * pMsg)
{
    int NCode, Id;
    unsigned char Slect_Row;
    unsigned char Slect_Column;
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

        case GUI_ID_BUTTON2:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                //整体存储
                if (Config_Save(STEMP_DEF_ID) < 0) {
                    MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_SAVE_ER));
                }

                Current_HWIN = WM_GetParent(hWin);
                GUI_EndDialog(hWin, 0);
            }

            break;

        case GUI_ID_LISTVIEW0:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                if (sys_sta.login_type == LOGIN_GUEST) {
                    MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_NO_LOGIN));
                    break;
                }

                Slect_Row = LISTVIEW_GetSel(WM_GetDialogItem(hWin, GUI_ID_LISTVIEW0));
                Slect_Column = LISTVIEW_GetSelCol(WM_GetDialogItem(hWin, GUI_ID_LISTVIEW0));

                if (Slect_Column == 0 || Slect_Column >= TEMP_NUM_MAX 
					|| Slect_Row >= HEAT_NUM_MAX) {
                    break;
                }

                dev_inf.TEMP_Define[Slect_Row][Slect_Column - 1] = !dev_inf.TEMP_Define[Slect_Row][Slect_Column - 1];
                listview_lv_sta(WM_GetDialogItem(hWin, GUI_ID_LISTVIEW0), Slect_Row , 0);
            }

            break;

        }

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
WM_HWIN MDev_SetSensor_Set;
void MDev_SetSensor_Set_Task(WM_HWIN hParent)
{
    MDev_SetSensor_Set = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    Current_HWIN = MDev_SetSensor_Set;
}
