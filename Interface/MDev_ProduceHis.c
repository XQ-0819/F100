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

static U8 select_type = 0;
static int group_num = 0;
static SCROLLBAR_Handle hScrollV;
static SCROLLBAR_Handle hScrollH;

/*********************************************************************
*
*       Dialog resource
*
* This table conatins the info required to create the dialog.
* It has been created by ucGUIbuilder.
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect,    "",               0,                       0,  0,  800, 480, 0, 0},
    { TEXT_CreateIndirect,      "设备管理-历史数据",          GUI_ID_TEXT0,            280, 4, 240, 42, 0, 0},
    { LISTVIEW_CreateIndirect,   NULL,               GUI_ID_LISTVIEW0,        40, 50, 720, 356, 0, 0},

//    { BUTTON_CreateIndirect,    "上一页",          GUI_ID_BUTTON0,          MENU_BACK_X1, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
//    { BUTTON_CreateIndirect,    "下一页",          GUI_ID_BUTTON1,          MENU_BACK_X2, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
    { BUTTON_CreateIndirect,    "返回",             GUI_ID_BUTTON2,         MENU_BACK_X3, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
    { BUTTON_CreateIndirect,    "",     GUI_ID_BUTTON4,           			660, 0, 90, 60, 0, 0},

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
//    GUI_SetColor(GUI_LINE_COLOR);
//    GUI_DrawRoundedRect(20, 62, 780, 395, 10);
}


int get_group_num(int num)
{
    if (num != 0) {
        return num;
    } else {
        return 1;
    }
}


static int _Drawscrollbar(const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo)
{

    switch (pDrawItemInfo->Cmd) {

//    case WIDGET_ITEM_DRAW_BUTTON:
//		break;

//    case WIDGET_ITEM_DRAW_BUTTON_L:
//
//		   GUI_DrawBitmap(&bmup, pDrawItemInfo->y0, pDrawItemInfo->x0);
//
//		   break;

//    case WIDGET_ITEM_DRAW_BUTTON_R:
//        GUI_DrawBitmap(&bmdowm, pDrawItemInfo->y0, pDrawItemInfo->x0);

//		break;

//    case WIDGET_ITEM_DRAW_OVERLAP:
//        return 0;

//	case WIDGET_ITEM_DRAW_SHAFT:
//		 return 0;

//    case WIDGET_ITEM_DRAW_THUMB:
//        GUI_SetColor(GUI_DARKGRAY);
//        GUI_FillRect(pDrawItemInfo->y0, pDrawItemInfo->x0, pDrawItemInfo->y1,
//                     pDrawItemInfo->x1);
//        return 0;

    case WIDGET_ITEM_GET_BUTTONSIZE:
        return 0;

    default:
        return SCROLLBAR_DrawSkinFlex(pDrawItemInfo);
    }
}

static void InitDialog(WM_MESSAGE * pMsg)
{
    WM_HWIN hWin = pMsg->hWin;
    int MDev_ID = 0 , i = 0 ,j = 0,id = 0;
    WM_HWIN hItem;
    WM_HWIN listview0_hWin;
    unsigned short display_num = 0, start_id = 0, use_num = 0;
	TUSE_DATA use_cur_data;
    struct tm *t;
	
    for (MDev_ID = GUI_ID_BUTTON2; MDev_ID <= GUI_ID_BUTTON2; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, _cbButtonBack);
    }

    for (MDev_ID = GUI_ID_BUTTON4; MDev_ID <= GUI_ID_BUTTON4; MDev_ID++) {
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

            if (select_type == 1) {
                sprintf(show_buf, "%s-%s", GUI_LANG_GetTextEx(Lang_Device, Language_Bit), GUI_LANG_GetTextEx(Lang_Each_Animal, Language_Bit));
                TEXT_SetText(hItem, show_buf);
            } else {
                sprintf(show_buf, "%s-%s", GUI_LANG_GetTextEx(Lang_Device, Language_Bit), GUI_LANG_GetTextEx(Lang_History, Language_Bit));
                TEXT_SetText(hItem, show_buf);
            }
        }
    }

    WM_CreateTimer(WM_GetClientWindow(hWin), 0, 2000, 0);

    listview0_hWin = WM_GetDialogItem(hWin, GUI_ID_LISTVIEW0);
    HEADER_SetFont( LISTVIEW_GetHeader(listview0_hWin), &GUI_USER_HZ16);
    if (Language_Bit_original >= LANGUAGE_RUSSIAN) {
        HEADER_SetFont(LISTVIEW_GetHeader(listview0_hWin), &GUI_FontBC_16);
        LISTVIEW_SetFont(listview0_hWin, &GUI_FontBC_16);
    }
    LISTVIEW_SetFont(listview0_hWin, &GUI_USER_HZ16);
    LISTVIEW_SetGridVis(listview0_hWin, 1);
    LISTVIEW_SetRowHeight(listview0_hWin, 32);
    WM_DisableWindow(LISTVIEW_GetHeader(listview0_hWin));
    HEADER_SetTextColor(LISTVIEW_GetHeader(listview0_hWin), GUI_DEF_BUTTON);
    LISTVIEW_SetBkColor(listview0_hWin, LISTVIEW_CI_UNSEL, GUI_DEF_LGREEN);

    LISTVIEW_AddColumn(listview0_hWin, 70, GUI_LANG_GetTextEx(Lang_Sec, Language_Bit), GUI_TA_VCENTER | GUI_TA_CENTER);
    LISTVIEW_AddColumn(listview0_hWin, 64, GUI_LANG_GetTextEx(Lang_Day, Language_Bit), GUI_TA_VCENTER | GUI_TA_CENTER);

    for (i = 0; i < PRO_DATA_MAX; i++) {
        if (i < ENERGY_MAX && energy_enable[i] == 1) {
            sprintf(show_buf, "%s%d/kwh", GUI_LANG_GetTextEx(Lang_Electro, Language_Bit), i + 1);
            LISTVIEW_AddColumn(listview0_hWin, 140, show_buf, GUI_TA_VCENTER | GUI_TA_CENTER);
            use_num++;
        }
    }

    for (i = 0; i < PRO_DATA_MAX; i++) {
        if (ext_inf.din.pro_data[i].water_sta != STA_OFF) {
            sprintf(show_buf, "%s%d/L", GUI_LANG_GetTextEx(Lang_Drink_Num, Language_Bit), i + 1);
            LISTVIEW_AddColumn(listview0_hWin, 140, show_buf, GUI_TA_VCENTER | GUI_TA_CENTER);
            use_num++;
        }
    }

    for (i = 0; i < PRO_DATA_MAX; i++) {
        if (ext_inf.din.pro_data[i].food_sta != STA_OFF) {
            sprintf(show_buf, "%s%d/Kg", GUI_LANG_GetTextEx(Lang_Food_Num, Language_Bit), i + 1);
            LISTVIEW_AddColumn(listview0_hWin, 140, show_buf, GUI_TA_VCENTER | GUI_TA_CENTER);
            use_num++;
        }
    }

    LISTVIEW_SetHeaderHeight(listview0_hWin, 32);
    hScrollH = SCROLLBAR_CreateAttached(listview0_hWin, SCROLLBAR_CF_VERTICAL); //创建1个垂直滚动条
    SCROLLBAR_SetSkin(hScrollH, _Drawscrollbar);
    SCROLLBAR_SetWidth(hScrollH, 31); //设置滚动条宽
    SCROLLBAR_SetThumbSizeMin(30);
    WM_DisableWindow(listview0_hWin);

    hScrollV = SCROLLBAR_CreateAttached(listview0_hWin, SCROLLBAR_CF_FOCUSSABLE); //创建1个水平滚动条
    SCROLLBAR_SetSkin(hScrollV, _Drawscrollbar);
    SCROLLBAR_SetWidth(hScrollV, 32); //设置滚动条宽
    SCROLLBAR_SetThumbSizeMin(10);

    LISTVIEW_SetFixed(listview0_hWin, 1);
    HEADER_SetFixed(LISTVIEW_GetHeader(listview0_hWin), 0);


    if (use_num == 0) {
        return;
    }

    //当前已有数据条数
    if (index_inf.data_pro_state == 1) {
        display_num = DATA_PRO_RECORD_MAX;
    } else {
        display_num = index_inf.data_pro_id;
    }

    if (display_num == 0) {
        return;
    }

    if (index_inf.data_pro_id > 0) {
        start_id = index_inf.data_pro_id - 1;
    } else {
        start_id = DATA_PRO_RECORD_MAX - 1;
    }

    for (i = 0; i < display_num; i++) {
        if (Read_Recode_Data((U8*)&use_cur_data, RECODE_USE_DATA, start_id) < 1) {
            continue;
        }	
		
        LISTVIEW_AddRow(listview0_hWin, NULL);

        if (i % 2 == 0) {
            for (id = 0; id < use_num+2; id++) {
                LISTVIEW_SetItemBkColor(listview0_hWin, id, i, LISTVIEW_CI_UNSEL, GUI_DEF_GRAY);
            }

        }

        id = 0;
        t = localtime((const time_t *)&use_cur_data.time);
        sprintf(show_buf, "%02d/%02d", t->tm_mon + 1, t->tm_mday);
        LISTVIEW_SetItemText(listview0_hWin, id++, i, show_buf);
		
        sprintf(show_buf, "%d", use_cur_data.grow_day);
        LISTVIEW_SetItemText(listview0_hWin, id++, i, show_buf);
		
        group_num = get_group_num(use_cur_data.survival);

        for (j = 0; j < PRO_DATA_MAX; j++) {
            if (j < ENERGY_MAX && energy_enable[j] == 1) {
			    if (select_type == 1) {
		            sprintf(show_buf, "%.3f", (float)(use_cur_data.energy[j] * 1.0f * 1.0f / ENERG_RATE / group_num));
			    }else{
				    sprintf(show_buf, "%.2f", (float)(use_cur_data.energy[j] * 1.0f / ENERG_RATE));
			    }
		        LISTVIEW_SetItemText(listview0_hWin, id++, i, show_buf);
            }
        }
		
        for (j = 0; j < PRO_DATA_MAX; j++) {
            if (ext_inf.din.pro_data[j].water_sta != STA_OFF) {
			    if (select_type == 1) {
                    sprintf(show_buf, "%.3f", use_cur_data.water[j] * 1.0f / group_num);
			    }else{
	                sprintf(show_buf, "%d", use_cur_data.water[j]);
			    }
		        LISTVIEW_SetItemText(listview0_hWin, id++, i, show_buf);
            }
        }
		
        for (j = 0; j < PRO_DATA_MAX; j++) {
            if (ext_inf.din.pro_data[j].food_sta != STA_OFF) {
			    if (select_type == 1) {
	                sprintf(show_buf, "%.3f", (use_cur_data.food[j] * 1.0f / FOOD_RATE / group_num));
			    }else{
	                sprintf(show_buf, "%d", (use_cur_data.food[j] / FOOD_RATE));
			    }
		        LISTVIEW_SetItemText(listview0_hWin, id++, i, show_buf);
            }
        }


        if (start_id == 0) {
            start_id = DATA_PRO_RECORD_MAX - 1;
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

        switch (Id) {
        case GUI_ID_LISTVIEW0:
            if (NCode == WM_NOTIFICATION_SCROLL_CHANGED && listview_delay_flag == STA_ON) {
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
                Current_HWIN = WM_GetParent(hWin);
                MDev_ProduceHis_Task(Current_HWIN, (select_type + 1) % 2);
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
WM_HWIN MDev_ProduceHis;
void MDev_ProduceHis_Task(WM_HWIN hParent, U8 id)
{
    select_type = id;
    listview_delay_flag = STA_OFF;
    MDev_ProduceHis = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    Current_HWIN = MDev_ProduceHis;
}
