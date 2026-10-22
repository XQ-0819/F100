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
#include "def.h"
#include "ucos_ii.h"

static int listview_delay_flag = STA_OFF;

const u16 alarm_show[ALA_TYPE_MAX] = {
    Lang_T_HA,      Lang_T_LA,      Lang_Temp_ERR,
    Lang_Temp_ERR,  Lang_Temp_ERR,  Lang_Temp_ERR,
    Lang_Temp_ERR,  Lang_Temp_ERR,  Lang_Temp_ERR,
    Lang_Temp_ERR,
    Lang_Hum_HA,    Lang_Hum_LA,    Lang_Hum_ERR,
    Lang_CO2_HA,    Lang_CO2_LA,    Lang_CO2_ERR,
    Lang_NH3_HA,    Lang_NH3_LA,    Lang_NH3_ERR,
    Lang_VAC_HA,    Lang_VAC_LA,    Lang_NP_ERR,
    Lang_Electric_Box,  Lang_Feeding, Lang_Nightsoil, Lang_Other_Ala,
    Lang_Electric_Comm, Lang_PM2_5_ERR, Lang_H2S_ERR,Lang_Weighing_Comm,
    Lang_Light_ERR,Lang_PM2_5_HA,Lang_PM10_HA,Lang_Temp_EBN,
    Lang_Temp_EBN,Lang_Temp_EBN,Lang_Temp_EBN,
    Lang_Temp_EBN,Lang_Temp_EBN,Lang_Temp_EBN,
    Lang_Temp_EBN,Lang_Water_Comm,Lang_Wind_ERR,
    Lang_Fan_Current,Lang_Current_Imbal,Lang_ST_HA,Lang_ST_LA,
    Lang_Curtain_POT,Lang_Outage_Ala,Lang_TH_SW,Lang_TL_SW,
    Lang_WaterOut,Lang_WaterL_ALA,
    Lang_Level_ERR,
};

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
    { TEXT_CreateIndirect,      "报警记录",          GUI_ID_TEXT0,            300, 4, 200, 42, 0, 0},
    { LISTVIEW_CreateIndirect,   NULL,               GUI_ID_LISTVIEW0,        40, 70, 720, 320, 0, 0},

    { BUTTON_CreateIndirect,    "",     GUI_ID_BUTTON6,           			660, 0, 90, 60, 0, 0},

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
    unsigned short display_num = 0, start_id = 0;
    unsigned short i, j, num = 0, flag = 0, id = 0;
    TALA 			cur_ala;
    struct tm *t;

    for (MDev_ID = GUI_ID_BUTTON1_0; MDev_ID <= GUI_ID_BUTTON1_6; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, Main_cbButton);
    }
	
    for (MDev_ID = GUI_ID_BUTTON6; MDev_ID <= GUI_ID_BUTTON6; MDev_ID++) {//set
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, set_cbButton);
    }
	
    for (MDev_ID = GUI_ID_TEXT0; MDev_ID <= GUI_ID_TEXT0; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24(hItem);

        if (MDev_ID == GUI_ID_TEXT0) {
            TEXT_SetTextAlign(hItem,  GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetTextColor(hItem, GUI_WHITE);
            TEXT_SetText(hItem, GUI_LANG_GetTextEx(Lang_ALA_Record, Language_Bit));//报警记录
        }
    }
	
    WM_CreateTimer(WM_GetClientWindow(hWin), 0, 2000, 0);

    listview0_hWin = WM_GetDialogItem(hWin, GUI_ID_LISTVIEW0);
	
	if (Language_Bit_original >= LANGUAGE_RUSSIAN) {
	    HEADER_SetFont( LISTVIEW_GetHeader(listview0_hWin), &GUI_FontBC_16);
	}else if (Language_Bit_original == LANGUAGE_GBK) {
        HEADER_SetFont( LISTVIEW_GetHeader(listview0_hWin), &GUI_USER_HZ24);
	} else {
	    HEADER_SetFont( LISTVIEW_GetHeader(listview0_hWin), &GUI_USER_HZ16);
	} 
	
    HEADER_SetTextColor(LISTVIEW_GetHeader(listview0_hWin), GUI_DEF_BUTTON);
    LISTVIEW_SetFont(listview0_hWin, &GUI_USER_HZ24);
	if(Language_Bit_original >= LANGUAGE_RUSSIAN) {
	    LISTVIEW_SetFont(listview0_hWin, &GUI_FontBC_16);
	}
    LISTVIEW_SetGridVis(listview0_hWin, 1);
//    LISTVIEW_SetDefaultGridColor(GUI_BLACK);
    LISTVIEW_SetRowHeight(listview0_hWin, 32);

    LISTVIEW_AddColumn(listview0_hWin, 64, GUI_LANG_GetTextEx(Lang_Number_No, Language_Bit), GUI_TA_VCENTER | GUI_TA_CENTER);  // 序号
    LISTVIEW_AddColumn(listview0_hWin, 64, GUI_LANG_GetTextEx(Lang_Day, Language_Bit), GUI_TA_VCENTER | GUI_TA_CENTER);  // 日龄
    LISTVIEW_AddColumn(listview0_hWin, 150, GUI_LANG_GetTextEx(Lang_ALA_Time, Language_Bit), GUI_TA_VCENTER | GUI_TA_CENTER); // 报警时间
    LISTVIEW_AddColumn(listview0_hWin, 408, GUI_LANG_GetTextEx(Lang_ALA_content, Language_Bit), GUI_TA_VCENTER | GUI_TA_CENTER); // 报警内容

    LISTVIEW_SetHeaderHeight(listview0_hWin, 32);
    hScroll = SCROLLBAR_CreateAttached(listview0_hWin, SCROLLBAR_CF_VERTICAL); //创建1个垂直滚动条
    SCROLLBAR_SetWidth(hScroll, 31); //设置滚动条宽
    SCROLLBAR_SetThumbSizeMin(30);
    LISTVIEW_SetBkColor(listview0_hWin, LISTVIEW_CI_UNSEL, GUI_DEF_LGREEN);
    WM_DisableWindow(listview0_hWin);
    WM_DisableWindow(LISTVIEW_GetHeader(listview0_hWin));


    //当前已有数据条数
    if (index_inf.ala_state == 1) {
        display_num = ALA_RECORD_MAX;
    } else {
        display_num = index_inf.ala_id;
    }

    if (display_num == 0) {
        return;
    }

    if (index_inf.ala_id > 0) {
        start_id = index_inf.ala_id - 1;
    } else {
        start_id = ALA_RECORD_MAX - 1;
    }

    for (i = 0, id = 0; i < display_num; i++) {
        if (Read_Recode_Data((U8*)&cur_ala, RECODE_ALA, start_id) < 1) {
            continue;
        }

        flag = 0;
        LISTVIEW_AddRow(listview0_hWin, NULL);
        sprintf(show_buf, "%d", (display_num - i));
        LISTVIEW_SetItemText(listview0_hWin, 0, id, show_buf);

        sprintf(show_buf, "%d", cur_ala.grow_day);
        LISTVIEW_SetItemText(listview0_hWin, 1, id, show_buf);

        t = localtime((const time_t *)&cur_ala.alarm_time);
        sprintf(show_buf, "%02d/%02d %02d:%02d", t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min);
        LISTVIEW_SetItemText(listview0_hWin, 2, id, show_buf);

        if (cur_ala.state == 0) {
            LISTVIEW_SetItemTextColor(listview0_hWin, 0, id, LISTVIEW_CI_UNSEL, GUI_BLACK);
            LISTVIEW_SetItemTextColor(listview0_hWin, 1, id, LISTVIEW_CI_UNSEL, GUI_BLACK);
            LISTVIEW_SetItemTextColor(listview0_hWin, 2, id, LISTVIEW_CI_UNSEL, GUI_BLACK);
            LISTVIEW_SetItemTextColor(listview0_hWin, 3, id, LISTVIEW_CI_UNSEL, GUI_BLACK);

	        LISTVIEW_SetItemText(listview0_hWin, 3, id, GUI_LANG_GetTextEx(Lang_ALA_Res, Language_Bit));//报警恢复
		    
            id++;
        } else {
            memset(show_buf, 0, sizeof(show_buf));

            for (j = 0, num = 0; j < ALA_MAX; j++) {
                if ((cur_ala.state >> j) & 0x01 == 1) {
                    if (j >= 2 && j <= 9) {
                        sprintf(show_buf, "%s%d#%s;", show_buf, (j - 1), GUI_LANG_GetTextEx(alarm_show[j], Language_Bit));
                    } else if (j >= 33 && j <= 40) {
                        sprintf(show_buf, "%s%d#%s;", show_buf, (j - 32), GUI_LANG_GetTextEx(alarm_show[j], Language_Bit));
                    } else {
                        sprintf(show_buf, "%s%s;", show_buf, GUI_LANG_GetTextEx(alarm_show[j], Language_Bit));
                    }
                    num ++;
                }

                if (num == 0) {
                    continue;
                }

                if (num >= ALA_SHOW_NUM || (j == (ALA_MAX - 1) && num < ALA_SHOW_NUM)) {
                    if (flag == 1) {
                        LISTVIEW_AddRow(listview0_hWin, NULL);
                    }

                    flag = 1;

                    LISTVIEW_SetItemTextColor(listview0_hWin, 0, id, LISTVIEW_CI_UNSEL, GUI_DEF_RED);
                    LISTVIEW_SetItemTextColor(listview0_hWin, 1, id, LISTVIEW_CI_UNSEL, GUI_DEF_RED);
                    LISTVIEW_SetItemTextColor(listview0_hWin, 2, id, LISTVIEW_CI_UNSEL, GUI_DEF_RED);
                    LISTVIEW_SetItemTextColor(listview0_hWin, 3, id, LISTVIEW_CI_UNSEL, GUI_DEF_RED);
//                    LISTVIEW_SetItemBkColor(listview0_hWin, 0, id, LISTVIEW_CI_UNSEL, GUI_DEF_RED);
//                    LISTVIEW_SetItemBkColor(listview0_hWin, 1, id, LISTVIEW_CI_UNSEL, GUI_DEF_RED);
//                    LISTVIEW_SetItemBkColor(listview0_hWin, 2, id, LISTVIEW_CI_UNSEL, GUI_DEF_RED);
//                    LISTVIEW_SetItemBkColor(listview0_hWin, 3, id, LISTVIEW_CI_UNSEL, GUI_DEF_RED);
                    LISTVIEW_SetItemText(listview0_hWin, 3, id, show_buf);
                    memset(show_buf, 0, sizeof(show_buf));
                    id++;
                    num = 0;
                    continue;
                }

            }

        }

        if (start_id == 0) {
            start_id = ALA_RECORD_MAX - 1;
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
                Button_Menu(Id, hWin, GUI_ID_BUTTON1_5);
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

        case GUI_ID_BUTTON6:
            if (NCode == WM_NOTIFICATION_RELEASED) {
				MDev_Ala_En_Task(hWin);
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
WM_HWIN MAla_Record;
void MAla_Record_Task(WM_HWIN hParent)
{
	listview_delay_flag = STA_OFF;
    Main_Buffer=Main_Buffer_Alarm;
    MAla_Record = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    Current_HWIN = MAla_Record;
}
