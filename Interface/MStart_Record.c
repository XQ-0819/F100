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
#include "stdio.h"
#include <string.h>
#include <time.h>
#include "recode.h"
#include "DataSave.h"
#include "bsp.h"
#include "eeprom.h"
#include "ucos_ii.h"

static int listview_delay_flag = STA_OFF;

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
    { TEXT_CreateIndirect,      "历史数据",          GUI_ID_TEXT0,            300, 4, 200, 42, 0, 0},
    { LISTVIEW_CreateIndirect,   NULL,               GUI_ID_LISTVIEW0,        40, 70, 720, 324, 0, 0},

//    { BUTTON_CreateIndirect,    "上一页",          GUI_ID_BUTTON0,          MENU_BACK_X1, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
//    { BUTTON_CreateIndirect,    "下一页",          GUI_ID_BUTTON1,          MENU_BACK_X2, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
//    { BUTTON_CreateIndirect,    "返回",             GUI_ID_BUTTON2,         MENU_BACK_X3, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON1_0,          384, 398, 90, 82, 0, 0},
    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON1_1,          25, 400, 105, 80, 0, 0},
    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON1_2,          155, 400, 105, 80, 0, 0},
    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON1_3,          275, 400, 105, 80, 0, 0},
    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON1_4,          435, 400, 105, 80, 0, 0},
    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON1_5,          555, 400, 105, 80, 0, 0},
    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON1_6,          675, 400, 125, 80, 0, 0},

    { BUTTON_CreateIndirect,    "",            GUI_ID_BUTTON6,           			590, 0, 90, 60, 0, 0},
};

/*****************************************************************
**      FunctionName:void PaintDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**
**      call this function in _cbCallback --> WM_PAINT
*****************************************************************/

static void PaintDialog(WM_MESSAGE * pMsg)
{
    Show_INBackPic();
}

static void InitDialog(WM_MESSAGE * pMsg)
{
    WM_HWIN hWin = pMsg->hWin;
    int MDev_ID = 0;
    WM_HWIN hItem;
    SCROLLBAR_Handle hScroll;
    WM_HWIN listview0_hWin;
    struct tm *t;
    int id, s, ret, display_num, start_time;

//    for (MDev_ID = GUI_ID_BUTTON2; MDev_ID <= GUI_ID_BUTTON2; MDev_ID++) {
//        hItem = WM_GetDialogItem(hWin, MDev_ID);
//        Button_SetHZ24(hItem);
//    }

    for (MDev_ID = GUI_ID_BUTTON1_0; MDev_ID <= GUI_ID_BUTTON1_6; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, Main_cbButton);
    }

    for (MDev_ID = GUI_ID_BUTTON6; MDev_ID <= GUI_ID_BUTTON6; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, next_cbButton);
    }

    for (MDev_ID = GUI_ID_TEXT0; MDev_ID <= GUI_ID_TEXT0; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24(hItem);

        if (MDev_ID == GUI_ID_TEXT0) {
            TEXT_SetTextAlign(hItem,  GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetTextColor(hItem, GUI_WHITE);
            TEXT_SetText(hItem, GUI_LANG_GetTextEx(Lang_History, Language_Bit));
        }
    }
    WM_CreateTimer(WM_GetClientWindow(hWin), 0, 2000, 0);

    listview0_hWin = WM_GetDialogItem(hWin, GUI_ID_LISTVIEW0);

    LISTVIEW_SetFont(listview0_hWin, &GUI_USER_HZ24);
//    LISTVIEW_SetDefaultGridColor(GUI_BLACK);
    LISTVIEW_SetGridVis(listview0_hWin, 1);
    HEADER_SetFont( LISTVIEW_GetHeader(listview0_hWin), &GUI_USER_HZ24);
	if(Language_Bit_original >= LANGUAGE_RUSSIAN){
	    LISTVIEW_SetFont(listview0_hWin, &GUI_FontBC_16);
	    HEADER_SetFont( LISTVIEW_GetHeader(listview0_hWin), &GUI_FontBC_16);
	}
    HEADER_SetTextColor(LISTVIEW_GetHeader(listview0_hWin), GUI_DEF_BUTTON);
    LISTVIEW_SetBkColor(listview0_hWin, LISTVIEW_CI_UNSEL, GUI_DEF_LGREEN);
    WM_DisableWindow(LISTVIEW_GetHeader(listview0_hWin));

    LISTVIEW_SetRowHeight(listview0_hWin, 32);

    LISTVIEW_AddColumn(listview0_hWin, 100, GUI_LANG_GetTextEx(Lang_Start_up_Num, Language_Bit), GUI_TA_VCENTER | GUI_TA_CENTER);
    LISTVIEW_AddColumn(listview0_hWin, 220, GUI_LANG_GetTextEx(Lang_Time, Language_Bit), GUI_TA_VCENTER | GUI_TA_CENTER);
    LISTVIEW_AddColumn(listview0_hWin, 45, "", GUI_TA_VCENTER | GUI_TA_CENTER);
    LISTVIEW_AddColumn(listview0_hWin, 100, GUI_LANG_GetTextEx(Lang_Start_up_Num, Language_Bit), GUI_TA_VCENTER | GUI_TA_CENTER);
    LISTVIEW_AddColumn(listview0_hWin, 220, GUI_LANG_GetTextEx(Lang_Time, Language_Bit), GUI_TA_VCENTER | GUI_TA_CENTER);

    LISTVIEW_SetHeaderHeight(listview0_hWin, 32);

    hScroll = SCROLLBAR_CreateAttached(listview0_hWin, SCROLLBAR_CF_VERTICAL); //创建1个垂直滚动条
    SCROLLBAR_SetWidth(hScroll, 31); //设置滚动条宽
    SCROLLBAR_SetThumbSizeMin(30);
    WM_DisableWindow(listview0_hWin);

//    LISTVIEW_SetBkColor(listview0_hWin, LISTVIEW_CI_UNSEL, 	GUI_WHITE);
//    GUI_SetAlpha(0x20);
//	LISTVIEW_SetBkColor(listview0_hWin, LISTVIEW_CI_UNSEL, 0x00532b19);
//	GUI_SetAlpha(0);

    EEPROM_Read( EE_INDEX_ADDR, (INT8U*)&index_inf.wMagic, sizeof(index_inf));

    if (index_inf.start_time / DATA_RECORD_MAX < 1) {
        display_num = index_inf.start_time;
    } else {
        display_num = DATA_RECORD_MAX;
        ret = index_inf.start_time % DATA_RECORD_MAX;
    }

    start_time = index_inf.start_time;

    for (id = 0; display_num > 0; display_num --) {

        if ((id % 2) < 1) {
            s = 0;
            LISTVIEW_AddRow(listview0_hWin, NULL);
        } else {
            s = 1;
        }

        sprintf(show_buf, "%d", start_time--);
        LISTVIEW_SetItemText(listview0_hWin, (id % 2) * 2 + s, id / 2, show_buf);

        if (index_inf.start_time / DATA_RECORD_MAX < 1) {
            ret = display_num;
        } else {
            if (--ret < 0) {
                ret = DATA_RECORD_MAX - 1;
            }
        }

        t = localtime((const time_t *)&index_inf.start_t[ret]);
        sprintf(show_buf, "%02d/%02d/%02d %02d:%02d:%02d", (t->tm_year - 100), (t->tm_mon + 1), t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
        LISTVIEW_SetItemText(listview0_hWin, (id % 2) * 2 + 1 + s, id / 2, show_buf);

        id ++;
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
		
    case WM_TIMER:
		listview_delay_flag = STA_ON;
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
		
        if (Id >= GUI_ID_BUTTON1_0 && Id <= GUI_ID_BUTTON1_6) {
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Button_Menu(Id, hWin, GUI_ID_BUTTON1_4);
            }

            break;
        }

        switch (Id) {
        case GUI_ID_LISTVIEW0:
			if(NCode == WM_NOTIFICATION_SCROLL_CHANGED && listview_delay_flag == STA_ON) {
				OSTimeDly(30);
			}
			break;
			
        case GUI_ID_OK:
            if (NCode == WM_NOTIFICATION_RELEASED)
                GUI_EndDialog(hWin, 0);

            break;

        case GUI_ID_CANCEL:
            if (NCode == WM_NOTIFICATION_RELEASED)
                GUI_EndDialog(hWin, 0);

            break;

        case GUI_ID_BUTTON6:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                GUI_EndDialog(hWin, 0);
                MData_Record_Task(WM_GetParent(hWin));
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
WM_HWIN MStart_Record;
void MStart_Record_Task(WM_HWIN hParent)
{
    Main_Buffer=Main_Buffer_History;
	listview_delay_flag = STA_OFF;
    MStart_Record = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    Current_HWIN = MStart_Record;
}



