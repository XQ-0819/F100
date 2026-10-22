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
#include "ucos_ii.h"

static int listview_delay_flag = STA_OFF;


//EndofEventsFunctionList



/*********************************************************************
*
*       static data
*
**********************************************************************
*/

extern int min_grow_day;
extern int grow_day;
extern int grow_num;

extern GUI_POINT points[4][DATA_RECORD_MAX];
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
    { LISTVIEW_CreateIndirect,   NULL,               GUI_ID_LISTVIEW0,        30, 70, 745, 320, 0, 0},

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

    { BUTTON_CreateIndirect,    "",     GUI_ID_BUTTON4,           			660, 0, 90, 60, 0, 0},
    { BUTTON_CreateIndirect,    "",     GUI_ID_BUTTON5,           			720, 0, 90, 60, 0, 0},
	{ BUTTON_CreateIndirect,    "",     GUI_ID_BUTTON6,           			590, 0, 90, 60, 0, 0},
	{ BUTTON_CreateIndirect,    "",     GUI_ID_BUTTON7,           			530, 0, 90, 60, 0, 0},
		
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

static void Temp_TO_Text(char *buf, signed short temp)
{
    if (temp == TEMP_ER_VALUE) {
        sprintf(show_buf, "--.-%s", GUI_LANG_GetTextEx(Lang_oC, Language_Bit));
    } else {
        sprintf(show_buf, "%.1f%s", (float)(temp * 1.0f / TEMP_RATE),GUI_LANG_GetTextEx(Lang_oC, Language_Bit));
    }
}

static void Clock_cbButton(WM_MESSAGE * pMsg)
{
    WM_HWIN  hWin;
    hWin  = pMsg->hWin;

    switch (pMsg->MsgId) {
    case WM_PAINT:
        if (BUTTON_IsPressed(hWin)) {
            GUI_SetBkColor(GUI_WHITE);
            GUI_SetAlpha(0xb0);
            GUI_defDrawBitmap("00clock_in", 25, 8);
            GUI_SetAlpha(0);
        } else {
            GUI_defDrawBitmap("00clock_in", 25, 8);
        }

        break;

    default:
        BUTTON_Callback(pMsg);
    }
}

static void InitDialog(WM_MESSAGE * pMsg)
{
    WM_HWIN hWin = pMsg->hWin;
    int MDev_ID = 0;
    WM_HWIN hItem;
    SCROLLBAR_Handle hScroll;
    WM_HWIN listview0_hWin;
    float day_x =  1;
    int temp_y = 1;

    unsigned short display_num = 0, start_id = 0;
    unsigned short i, id = 0,pid = 0;
    TDATA	cur_data;
    struct tm *t;

//    for (MDev_ID = GUI_ID_BUTTON2; MDev_ID <= GUI_ID_BUTTON2; MDev_ID++) {
//        hItem = WM_GetDialogItem(hWin, MDev_ID);
//        Button_SetHZ24(hItem);
//    }

    for (MDev_ID = GUI_ID_BUTTON1_0; MDev_ID <= GUI_ID_BUTTON1_6; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, Main_cbButton);
    }

    for (MDev_ID = GUI_ID_BUTTON4; MDev_ID <= GUI_ID_BUTTON4; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, Curve_cbButton);
    }
	
    for (MDev_ID = GUI_ID_BUTTON5; MDev_ID <= GUI_ID_BUTTON5; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, Detail_cbButton);
    }

    for (MDev_ID = GUI_ID_BUTTON6; MDev_ID <= GUI_ID_BUTTON6; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, next_cbButton);
    }

    for (MDev_ID = GUI_ID_BUTTON7; MDev_ID <= GUI_ID_BUTTON7; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, Clock_cbButton);
    }

    for (MDev_ID = GUI_ID_TEXT0; MDev_ID <= GUI_ID_TEXT0; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24(hItem);

        if (MDev_ID == GUI_ID_TEXT0) {
            TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetTextColor(hItem, GUI_WHITE);
            TEXT_SetText(hItem, GUI_LANG_GetTextEx(Lang_Daily_Average, Language_Bit)); // 历史数据-每日平均
        }
    }
    WM_CreateTimer(WM_GetClientWindow(hWin), 0, 2000, 0);

    listview0_hWin = WM_GetDialogItem(hWin, GUI_ID_LISTVIEW0);


    LISTVIEW_SetFont(listview0_hWin, &GUI_USER_HZ24);
//	LISTVIEW_SetDefaultGridColor(GUI_BLACK);
    LISTVIEW_SetGridVis(listview0_hWin, 1);

    if (Language_Bit_original >= LANGUAGE_RUSSIAN) {
        HEADER_SetFont(LISTVIEW_GetHeader(listview0_hWin), &GUI_FontBC_16);
    } else if (Language_Bit_original == LANGUAGE_GBK) {
        HEADER_SetFont(LISTVIEW_GetHeader(listview0_hWin), &GUI_USER_HZ24);
    } else {
        HEADER_SetFont(LISTVIEW_GetHeader(listview0_hWin), &GUI_USER_HZ16);
    }

    HEADER_SetTextColor(LISTVIEW_GetHeader(listview0_hWin),GUI_DEF_BUTTON);
	LISTVIEW_SetBkColor(listview0_hWin, LISTVIEW_CI_UNSEL, GUI_DEF_LGREEN);
	
    LISTVIEW_SetRowHeight(listview0_hWin, 32);

    LISTVIEW_AddColumn(listview0_hWin, 70, GUI_LANG_GetTextEx(Lang_Time, Language_Bit), GUI_TA_VCENTER | GUI_TA_CENTER);         // 时间
    LISTVIEW_AddColumn(listview0_hWin, 64, GUI_LANG_GetTextEx(Lang_Day, Language_Bit), GUI_TA_VCENTER | GUI_TA_CENTER);          // 日龄
    LISTVIEW_AddColumn(listview0_hWin, 95, GUI_LANG_GetTextEx(Lang_Min_Temp, Language_Bit), GUI_TA_VCENTER | GUI_TA_CENTER);     // 最小温度
    LISTVIEW_AddColumn(listview0_hWin, 95, GUI_LANG_GetTextEx(Lang_Average_T, Language_Bit), GUI_TA_VCENTER | GUI_TA_CENTER);    // 平均温度
    LISTVIEW_AddColumn(listview0_hWin, 95, GUI_LANG_GetTextEx(Lang_Max_Temp, Language_Bit), GUI_TA_VCENTER | GUI_TA_CENTER);     // 最大温度
    LISTVIEW_AddColumn(listview0_hWin, 95, GUI_LANG_GetTextEx(Lang_Outdoor_Temp, Language_Bit), GUI_TA_VCENTER | GUI_TA_CENTER); // 室外温度
    LISTVIEW_AddColumn(listview0_hWin, 100, GUI_LANG_GetTextEx(Lang_Num_Deaths, Language_Bit), GUI_TA_VCENTER | GUI_TA_CENTER);  // 死亡总量
    LISTVIEW_AddColumn(listview0_hWin, 100, GUI_LANG_GetTextEx(Lang_Stock, Language_Bit), GUI_TA_VCENTER | GUI_TA_CENTER);       // 动物存量

    LISTVIEW_SetHeaderHeight(listview0_hWin, 32);
	
    hScroll = SCROLLBAR_CreateAttached(listview0_hWin, SCROLLBAR_CF_VERTICAL); //创建1个垂直滚动条
    SCROLLBAR_SetWidth(hScroll, 31); //设置滚动条宽
    SCROLLBAR_SetThumbSizeMin(30);
    WM_DisableWindow(listview0_hWin);
    WM_DisableWindow(LISTVIEW_GetHeader(listview0_hWin));
	
//    LISTVIEW_SetBkColor(listview0_hWin, LISTVIEW_CI_UNSEL, 	GUI_WHITE);
//    GUI_SetAlpha(0x20); 
//	LISTVIEW_SetBkColor(listview0_hWin, LISTVIEW_CI_UNSEL, 0x00532b19);
//	GUI_SetAlpha(0);	

    //当前已有数据条数
    if (index_inf.data_state == 1) {
        display_num = DATA_RECORD_MAX;
    } else {
        display_num = index_inf.data_id;
    }

    if (display_num == 0) {
        return;
    }

    if (index_inf.data_id > 0) {
        start_id = index_inf.data_id - 1;
    } else {
        start_id = DATA_RECORD_MAX - 1;
    }

    Read_Recode_Data((U8*)&cur_data, RECODE_DATA, start_id);
    grow_num = display_num;
    grow_day = cur_data.grow_day;
	min_grow_day = cur_data.grow_day;
    day_x =  50.0 / (grow_day / 15 + 1);

    for (i = 0; i < display_num; i++) {
        if (Read_Recode_Data((U8*)&cur_data, RECODE_DATA, start_id) < 1) {
            continue;
        }
		
		if(min_grow_day > cur_data.grow_day){
			min_grow_day = cur_data.grow_day;
		}
		
        LISTVIEW_AddRow(listview0_hWin, NULL);

        if (i % 2 == 0) {
            for (id = 0; id < 8; id++) {
                LISTVIEW_SetItemBkColor(listview0_hWin, id, i, LISTVIEW_CI_UNSEL, GUI_DEF_GRAY);
            }

        }


        id = 0;
		pid = 0;

	    t = localtime((const time_t *)&cur_data.time);
        sprintf(show_buf, "%02d/%02d", t->tm_mon + 1, t->tm_mday);
        LISTVIEW_SetItemText(listview0_hWin, id++, i, show_buf);
		
        sprintf(show_buf, "%d", cur_data.grow_day);
        LISTVIEW_SetItemText(listview0_hWin, id++, i, show_buf);

        if (cur_data.min_temp > TEMP_MAX_ERR || cur_data.min_temp < TEMP_MIN_ERR) {
            points[pid][i].x = POINTS_ER;
            points[pid++][i].y = 0;
        } else {
            points[pid][i].x = cur_data.grow_day * day_x;
            points[pid++][i].y = -((cur_data.min_temp - 100) * temp_y);
        }

        Temp_TO_Text(show_buf, cur_data.min_temp);
        LISTVIEW_SetItemText(listview0_hWin, id++, i, show_buf);

        if (cur_data.avg_temp > TEMP_MAX_ERR || cur_data.avg_temp < TEMP_MIN_ERR) {
            points[pid][i].x = POINTS_ER;
            points[pid++][i].y = 0;
        } else {
            points[pid][i].x = cur_data.grow_day * day_x;
            points[pid++][i].y = -((cur_data.avg_temp - 100) * temp_y);
        }

        Temp_TO_Text(show_buf, cur_data.avg_temp);
        LISTVIEW_SetItemText(listview0_hWin, id++, i, show_buf);

        if (cur_data.max_temp > TEMP_MAX_ERR || cur_data.max_temp < TEMP_MIN_ERR) {
            points[pid][i].x = POINTS_ER;
            points[pid++][i].y = 0;
        } else {
            points[pid][i].x = cur_data.grow_day * day_x;
            points[pid++][i].y = -((cur_data.max_temp - 100) * temp_y);
        }

        Temp_TO_Text(show_buf, cur_data.max_temp);
        LISTVIEW_SetItemText(listview0_hWin, id++, i, show_buf);

        if (cur_data.out_temp > TEMP_MAX_ERR || cur_data.out_temp < TEMP_MIN_ERR) {
            points[pid][i].x = POINTS_ER;
            points[pid++][i].y = 0;
        } else {
            points[pid][i].x = cur_data.grow_day * day_x;
            points[pid++][i].y = -((cur_data.out_temp - 100) * temp_y);
        }

        Temp_TO_Text(show_buf, cur_data.out_temp);
        LISTVIEW_SetItemText(listview0_hWin, id++, i, show_buf);

        sprintf(show_buf, "%d", cur_data.dead_total);
        LISTVIEW_SetItemText(listview0_hWin, id++, i, show_buf);
        sprintf(show_buf, "%d", cur_data.survival);
        LISTVIEW_SetItemText(listview0_hWin, id++, i, show_buf);

        if (start_id == 0) {
            start_id = DATA_RECORD_MAX - 1;
        } else {
            start_id --;
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

        case GUI_ID_BUTTON2:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Current_HWIN = WM_GetParent(hWin);
                GUI_EndDialog(hWin, 0);
            }

            break;

        case GUI_ID_BUTTON4:
            if (NCode == WM_NOTIFICATION_RELEASED) {
				HisTemp_View_Task(hWin);
            }

            break;
			
        case GUI_ID_BUTTON5:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                DataTemp_View_Task(hWin);
            }
			break;

        case GUI_ID_BUTTON6:
            if (NCode == WM_NOTIFICATION_RELEASED) {
				GUI_EndDialog(hWin, 0);
                MStart_Record_Task(WM_GetParent(hWin));
            }
            break;

        case GUI_ID_BUTTON7:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                MClock_Record_Task(hWin);
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
WM_HWIN MData_Record;
void MData_Record_Task(WM_HWIN hParent)
{
    Main_Buffer=Main_Buffer_History;
	listview_delay_flag = STA_OFF;
    MData_Record = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    Current_HWIN = MData_Record;
}
