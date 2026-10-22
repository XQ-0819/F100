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
#include "bsp.h"

//EndofEventsFunctionList



/*********************************************************************
*
*       static data
*
**********************************************************************
*/
static U8 select_id = 0;
U8	water_realtime_page = STA_OFF;

static WM_HTIMER hTimer;
#define Y_Space		(45)
/*********************************************************************
*
*       Dialog resource
*
* This table conatins the info required to create the dialog.
* It has been created by ucGUIbuilder.
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect,    "",               0,                       0,  0,  800, 480, 0, 0},
    { TEXT_CreateIndirect,      "设备管理-实时数据",          GUI_ID_TEXT0,            280, 4, 240, 42, 0, 0},
    { TEXT_CreateIndirect,      "",          GUI_ID_TEXT1,            110, 55, 700, 60, 0, 0},
    { TEXT_CreateIndirect,      "",          GUI_ID_TEXT2,            110, 75 + Y_Space, 700, 60, 0, 0},
//    { TEXT_CreateIndirect,      "",          GUI_ID_TEXT10,           310, 70 + Y_Space * 1, 400, 30, 0, 0},
//    { TEXT_CreateIndirect,      "",          GUI_ID_TEXT11,           310, 70 + Y_Space * 2, 400, 30, 0, 0},
    { TEXT_CreateIndirect,      "",          GUI_ID_TEXT3,            310, 90 + Y_Space * 3, 300, 30, 0, 0},
    { TEXT_CreateIndirect,      "",          GUI_ID_TEXT4,            110, 90 + Y_Space * 4, 200, 30, 0, 0},
    { TEXT_CreateIndirect,      "",          GUI_ID_TEXT5,            110, 90 + Y_Space * 5, 200, 30, 0, 0},
    { TEXT_CreateIndirect,      "",          GUI_ID_TEXT6,            110, 90 + Y_Space * 6, 200, 30, 0, 0},
    { TEXT_CreateIndirect,      "",          GUI_ID_TEXT7,            430, 90 + Y_Space * 4, 200, 30, 0, 0},
    { TEXT_CreateIndirect,      "",          GUI_ID_TEXT8,            430, 90 + Y_Space * 5, 200, 30, 0, 0},
    { TEXT_CreateIndirect,      "",          GUI_ID_TEXT9,            430, 90 + Y_Space * 6, 200, 30, 0, 0},

    { BUTTON_CreateIndirect,    "电表1",     GUI_ID_BUTTON3,          110, 70 + Y_Space * 3, 120, 50, 0, 0},

//    { BUTTON_CreateIndirect,    "上一页",          GUI_ID_BUTTON0,          MENU_BACK_X1, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
//    { BUTTON_CreateIndirect,    "下一页",          GUI_ID_BUTTON1,          MENU_BACK_X2, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
    { BUTTON_CreateIndirect,    "返回",             GUI_ID_BUTTON2,         MENU_BACK_X3, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
};

static void Update_Value(WM_HWIN hWin, U8 sid)
{
    int id = GUI_ID_TEXT1, i = 0, num = 0;
    char temp[48];

    memset(show_buf, 0, sizeof(show_buf));
    /*用水量*/
    if (ext_inf.din.pro_data[0].water_sta == WL_PULSE) {

        sprintf(show_buf, "%s%d:%d L    ", GUI_LANG_GetTextEx(Lang_UseWater, Language_Bit), 1, index_inf.water[0][PRO_DATA_ALL]);

        num = 1;

        for (i = 1; i < PRO_DATA_MAX; i++) {
            if (ext_inf.din.pro_data[i].water_sta == 0)
                continue;

            sprintf(temp, "%d:%d L    ", i + 1, index_inf.water[i][PRO_DATA_ALL]);
            strcat(show_buf, temp);
            num++;

            if (num >= 3) {
                num = 0;
                strcat(show_buf, "\n");
            }
        }

        TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *)show_buf);
    } else if (ext_inf.din.pro_data[0].water_sta == WL_CJT_188 || ext_inf.din.pro_data[0].water_sta == WL_MB_OWN) {

        sprintf(show_buf, "%s%d:%d L    ", GUI_LANG_GetTextEx(Lang_UseWater, Language_Bit), 1, sta_in.water[0]);

        num = 1;

        for (i = 1; i < PRO_DATA_MAX; i++) {
            if (ext_inf.din.pro_data[i].water_sta == 0)
                continue;

            sprintf(temp, "%d:%d L    ", i + 1, sta_in.water[i]);
            strcat(show_buf, temp);

            num++;

            if (num >= 3) {
                num = 0;
                strcat(show_buf, "\n");
            }
        }

        TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *)show_buf);
    } else {
        sprintf(show_buf, "%s:-- L", GUI_LANG_GetTextEx(Lang_UseWater, Language_Bit));
        TEXT_SetText(WM_GetDialogItem(hWin, id++), show_buf);
    }
    memset(show_buf, 0, sizeof(show_buf));

    /*采食量*/
    if (ext_inf.din.pro_data[0].food_sta == FL_PULSE || ext_inf.din.pro_data[0].food_sta == FL_TIME) {
        sprintf(show_buf, "%s%d:%d Kg    ", GUI_LANG_GetTextEx(Lang_Food_Num, Language_Bit), 1, (index_inf.food[0][PRO_DATA_ALL] / FOOD_RATE));
        num = 1;
        for (i = 1; i < PRO_DATA_MAX; i++) {
            if (ext_inf.din.pro_data[i].food_sta == 0)
                continue;
            sprintf(temp, "%d:%d Kg    ", i + 1, (index_inf.food[i][PRO_DATA_ALL] / FOOD_RATE));
            strcat(show_buf, temp);
            num++;

            if (num >= 3) {
                num = 0;
                strcat(show_buf, "\n");
            }
        }
        TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *)show_buf);
    } else if (ext_inf.din.pro_data[0].food_sta == FL_MB_OWN || ext_inf.din.pro_data[0].food_sta == FL_MB_XK3102) {
        if (sta_in.food[0] == (unsigned int)DATA_ER_VALUE) {
            sprintf(show_buf, "%s%d:-1 Kg    ", GUI_LANG_GetTextEx(Lang_Remain_Feed, Language_Bit), 1);
        } else {
            sprintf(show_buf, "%s%d:%d Kg    ", GUI_LANG_GetTextEx(Lang_Remain_Feed, Language_Bit), 1, (sta_in.food[0] / FOOD_RATE));
        }

        num = 1;

        for (i = 1; i < PRO_DATA_MAX; i++) {
            if (ext_inf.din.pro_data[i].food_sta == 0)
                continue;

            if (sta_in.food[i] == (unsigned int)DATA_ER_VALUE) {
                sprintf(temp, "%d:-1 Kg    ", i + 1);
            } else {
                sprintf(temp, "%d:%d Kg    ", i + 1, (sta_in.food[i] / FOOD_RATE));
            }

            strcat(show_buf, temp);
            num++;

            if (num >= 3) {
                num = 0;
                strcat(show_buf, "\n");
            }
        }

        TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *)show_buf);
    } else {
        sprintf(show_buf, "%s:-- Kg", GUI_LANG_GetTextEx(Lang_Food_Num, Language_Bit));
        TEXT_SetText(WM_GetDialogItem(hWin, id++), show_buf);
    }

    /*用电量*/
    sprintf(show_buf, "%s:%.2f kwh", GUI_LANG_GetTextEx(Lang_Electro, Language_Bit), (float)(sta_in.ele[sid][MB_ENERGY] * 1.0f / ENERG_RATE));
    TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *)show_buf);

    sprintf(show_buf, "A%s:%.1f V", GUI_LANG_GetTextEx(Lang_ABCVoltage, Language_Bit), (float)(sta_in.ele[sid][MB_VA] * 1.0f / ENERG_PARA_RATE));
    TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *)show_buf);

    sprintf(show_buf, "B%s:%.1f V", GUI_LANG_GetTextEx(Lang_ABCVoltage, Language_Bit), (float)(sta_in.ele[sid][MB_VB] * 1.0f / ENERG_PARA_RATE));
    TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *)show_buf);

    sprintf(show_buf, "C%s:%.1f V", GUI_LANG_GetTextEx(Lang_ABCVoltage, Language_Bit), (float)(sta_in.ele[sid][MB_VC] * 1.0f / ENERG_PARA_RATE));
    TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *)show_buf);

    sprintf(show_buf, "A%s:%.1f A", GUI_LANG_GetTextEx(Lang_ABCCurrent, Language_Bit), (float)(sta_in.ele[sid][MB_IA] * 1.0f / ENERG_PARA_RATE));
    TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *)show_buf);

    sprintf(show_buf, "B%s:%.1f A", GUI_LANG_GetTextEx(Lang_ABCCurrent, Language_Bit), (float)(sta_in.ele[sid][MB_IB] * 1.0f / ENERG_PARA_RATE));
    TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *)show_buf);

    sprintf(show_buf, "C%s:%.1f A", GUI_LANG_GetTextEx(Lang_ABCCurrent, Language_Bit), (float)(sta_in.ele[sid][MB_IC] * 1.0f / ENERG_PARA_RATE));
    TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *)show_buf);

    sprintf(show_buf, "%s%d", GUI_LANG_GetTextEx(Lang_Ammeter, Language_Bit), sid + 1); // 电表
    BUTTON_SetText(WM_GetDialogItem(hWin, GUI_ID_BUTTON3), (const char *)show_buf);
}
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
    GUI_SetColor(GUI_LINE_COLOR);
    GUI_DrawRoundedRect(80, 50, 720, 406, 10);
    GUI_DrawRoundedRect(76, 48, 724, 409, 10);
}


static void InitDialog(WM_MESSAGE * pMsg)
{
    WM_HWIN hWin = pMsg->hWin;
    int MDev_ID = 0;
    WM_HWIN hItem;

    for (MDev_ID = GUI_ID_BUTTON3; MDev_ID <= GUI_ID_BUTTON3; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Button_SetHZ24(hItem);
    }

    for (MDev_ID = GUI_ID_BUTTON2; MDev_ID <= GUI_ID_BUTTON2; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, _cbButtonBack);
    }

    for (MDev_ID = GUI_ID_TEXT0; MDev_ID <= GUI_ID_TEXT9; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24L(hItem);

        if (MDev_ID == GUI_ID_TEXT0) {
            TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetTextColor(hItem, GUI_WHITE);
            sprintf(show_buf, "%s-%s", GUI_LANG_GetTextEx(Lang_Device, Language_Bit), GUI_LANG_GetTextEx(Lang_Real_Time, Language_Bit));
            TEXT_SetText(hItem, show_buf);
        }
    }

//    for (MDev_ID = GUI_ID_TEXT10; MDev_ID <= GUI_ID_TEXT11; MDev_ID++) {
//        hItem = WM_GetDialogItem(hWin, MDev_ID);
//        Text_SetHZ24L(hItem);
//    }

    hTimer = WM_CreateTimer(WM_GetClientWindow(hWin), 0, 1000, 0);

    if (energy_enable[0] == 1 && energy_enable[1] == 1) {
        select_id = 0;
    } else {
        WM_DisableWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON3));

        if (energy_enable[0] == 1) {
            select_id = 0;
        } else {
            select_id = 1;
        }
    }

    Update_Value(hWin, select_id);
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
        Update_Value(hWin, select_id);
        WM_RestartTimer(hTimer, 1000);
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
				water_realtime_page = STA_OFF;
                Current_HWIN = WM_GetParent(hWin);
                GUI_EndDialog(hWin, 0);
            }

            break;

        case GUI_ID_BUTTON3:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                select_id = (select_id + 1) % (ENERGY_MAX - 1);//只显示两个的数据
                Update_Value(hWin, select_id);
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
WM_HWIN MDev_ProduceCur;
void MDev_ProduceCur_Task(WM_HWIN hParent)
{
	water_realtime_page = STA_ON;
    MDev_ProduceCur = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    Current_HWIN = MDev_ProduceCur;
}
