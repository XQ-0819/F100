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
    { TEXT_CreateIndirect,      "打卡记录",          GUI_ID_TEXT0,            300, 4, 200, 42, 0, 0},
    { LISTVIEW_CreateIndirect,   NULL,               GUI_ID_LISTVIEW0,        40, 70, 720, 324, 0, 0},

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

    for (MDev_ID = GUI_ID_BUTTON2; MDev_ID <= GUI_ID_BUTTON2; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, _cbButtonBack);
    }

    for (MDev_ID = GUI_ID_TEXT0; MDev_ID <= GUI_ID_TEXT0; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24(hItem);

        if (MDev_ID == GUI_ID_TEXT0) {
            TEXT_SetTextAlign(hItem,  GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetTextColor(hItem, GUI_WHITE);

            TEXT_SetText(hItem, GUI_LANG_GetTextEx(Lang_Clock_inD, Language_Bit));
        }
    }
	
    WM_CreateTimer(WM_GetClientWindow(hWin), 0, 2000, 0);

    listview0_hWin = WM_GetDialogItem(hWin, GUI_ID_LISTVIEW0);

    LISTVIEW_SetFont(listview0_hWin, &GUI_USER_HZ24);
//    LISTVIEW_SetDefaultGridColor(GUI_BLACK);
    LISTVIEW_SetGridVis(listview0_hWin, 1);
    HEADER_SetFont( LISTVIEW_GetHeader(listview0_hWin), &GUI_USER_HZ24);
    HEADER_SetTextColor(LISTVIEW_GetHeader(listview0_hWin), GUI_DEF_BUTTON);
    LISTVIEW_SetBkColor(listview0_hWin, LISTVIEW_CI_UNSEL, GUI_DEF_LGREEN);
    WM_DisableWindow(LISTVIEW_GetHeader(listview0_hWin));

	if(Language_Bit_original >= LANGUAGE_RUSSIAN){
	    LISTVIEW_SetFont(listview0_hWin, &GUI_FontBC_16);
	    HEADER_SetFont( LISTVIEW_GetHeader(listview0_hWin), &GUI_FontBC_16);
	}

    LISTVIEW_SetRowHeight(listview0_hWin, 32);

    LISTVIEW_AddColumn(listview0_hWin, 110, GUI_LANG_GetTextEx(Lang_Clock_inN, Language_Bit), GUI_TA_VCENTER | GUI_TA_CENTER);//打卡次数
    LISTVIEW_AddColumn(listview0_hWin, 220, GUI_LANG_GetTextEx(Lang_Clock_inT, Language_Bit), GUI_TA_VCENTER | GUI_TA_CENTER);//打卡时间
    LISTVIEW_AddColumn(listview0_hWin, 25, "", GUI_TA_VCENTER | GUI_TA_CENTER);//空
    LISTVIEW_AddColumn(listview0_hWin, 110, GUI_LANG_GetTextEx(Lang_Clock_inN, Language_Bit), GUI_TA_VCENTER | GUI_TA_CENTER);//打卡次数
    LISTVIEW_AddColumn(listview0_hWin, 220, GUI_LANG_GetTextEx(Lang_Clock_inT, Language_Bit), GUI_TA_VCENTER | GUI_TA_CENTER);//打卡时间

    LISTVIEW_SetHeaderHeight(listview0_hWin, 32);

    hScroll = SCROLLBAR_CreateAttached(listview0_hWin, SCROLLBAR_CF_VERTICAL); //创建1个垂直滚动条
    SCROLLBAR_SetWidth(hScroll, 31); //设置滚动条宽
    SCROLLBAR_SetThumbSizeMin(30);
    WM_DisableWindow(listview0_hWin);

//    LISTVIEW_SetBkColor(listview0_hWin, LISTVIEW_CI_UNSEL, 	GUI_WHITE);
//    GUI_SetAlpha(0x20);
//	LISTVIEW_SetBkColor(listview0_hWin, LISTVIEW_CI_UNSEL, 0x00532b19);
//	GUI_SetAlpha(0);


    //当前已有数据条数
    if (clock_inf.clock_in_state == 1) {
        display_num = CLOCK_RECORD_MAX;
    } else {
        display_num = clock_inf.clock_in_id;
    }

    if (display_num == 0) {
        return;
    }

    if (clock_inf.clock_in_id > 0) {
        ret = clock_inf.clock_in_id - 1;
    } else {
        ret = CLOCK_RECORD_MAX - 1;
    }
	

    start_time = clock_inf.clock_in_time;

    for (id = 0; display_num > 0; display_num --) {

        if ((id % 2) < 1) {
            s = 0;
            LISTVIEW_AddRow(listview0_hWin, NULL);
        } else {
            s = 1;
        }

        sprintf(show_buf, "%d", start_time--);
        LISTVIEW_SetItemText(listview0_hWin, (id % 2) * 2 + s, id / 2, show_buf);


        t = localtime((const time_t *)&clock_inf.clock_in_t[ret]);
        sprintf(show_buf, "%02d/%02d/%02d %02d:%02d:%02d", (t->tm_year - 100), (t->tm_mon + 1), t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
        LISTVIEW_SetItemText(listview0_hWin, (id % 2) * 2 + 1 + s, id / 2, show_buf);
		
        if (ret == 0) {
            ret = CLOCK_RECORD_MAX - 1;
        } else {
            ret --;
        }
		
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

        switch (Id) {
        case GUI_ID_LISTVIEW0:
			if(NCode == WM_NOTIFICATION_SCROLL_CHANGED && listview_delay_flag == STA_ON) {
				OSTimeDly(30);
			}
			break;            
            
        case GUI_ID_BUTTON2:
            if (NCode == WM_NOTIFICATION_RELEASED) {
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
WM_HWIN MClock_Record;
void MClock_Record_Task(WM_HWIN hParent)
{
	listview_delay_flag = STA_OFF;
    MClock_Record = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    Current_HWIN = MClock_Record;
}



