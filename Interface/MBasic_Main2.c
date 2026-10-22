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
#include "pcf8563.h"
#include "bsp.h"
#include "datasave.h"
#include "recode.h"
#include <string.h>
#include "def.h"
#include "stm324x9i_eval_lcd.h"
#include "w25qxx.h"

//EventsFunctionList
//EndofEventsFunctionList


extern GUI_CONST_STORAGE GUI_BITMAP hHomepic;



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
    { WINDOW_CreateIndirect,    "",               0,                   0,  0,  800, 480, 0, 0},
    { TEXT_CreateIndirect,      "",      	  GUI_ID_TEXT0,            10, 0, 240, 50, 0, 0},//room
    { TEXT_CreateIndirect,      "基本信息",   GUI_ID_TEXT1,            300, 0, 200, 50, 0, 0},
    { TEXT_CreateIndirect,      "",     	  GUI_ID_TEXT2,            590, 0, 240, 50, 0, 0},//time
    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON0,          15, 430, 46, 46, 0, 0},//home
    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON1,          740, 430, 46, 46, 0, 0},//error
    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON2,          90, 430, 46, 46, 0, 0},//login

    { TEXT_CreateIndirect,      "",     	  GUI_ID_TEXT3,            	83, 130, 126, 126, 0, 0},//temp
    { TEXT_CreateIndirect,      "",     	  GUI_ID_TEXT4,        		336, 130, 126, 126, 0, 0},//fan
    { TEXT_CreateIndirect,      "",     	  GUI_ID_TEXT5,        		593, 130, 126, 126, 0, 0},//hum
    { TEXT_CreateIndirect,      "",        	  GUI_ID_TEXT6,        		83, 367, 126, 45, 0, 0},//settemp
    { TEXT_CreateIndirect,      "",     	  GUI_ID_TEXT7,        		336, 367, 126, 45, 0, 0},//setfan
    { TEXT_CreateIndirect,      "",        	  GUI_ID_TEXT8,        		593, 367, 126, 45, 0, 0},//sethum

    { TEXT_CreateIndirect,      "",        	  GUI_ID_TEXT9,        		150, 430, 500, 46, 0, 0},//ala
};



static WM_HTIMER hTimer;
static const char *MPIC[] = {"91bmMPic_Home", "09bmMPic_HALA",};
static U8 show_sec = 0;
/*****************************************************************
**      FunctionName:void PaintDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**
**      call this function in _cbCallback --> WM_PAINT
*****************************************************************/
static void PaintDialog(WM_MESSAGE * pMsg)
{

//    WM_HWIN hWin = pMsg->hWin;
    GUI_DrawBitmap(&hHomepic, 0, 0);

    if (sys_sta.net_sta != STA_OFF) {
        GUI_defDrawBitmap("34bmMWIFI_YES", 590, 11);
    } else {
        GUI_defDrawBitmap("35bmMWIFI_NO", 590, 11);
    }

    if (soft_inf.creg_en == STA_ON) {
        if (sys_sta.net_creg == STA_ON) {
            GUI_defDrawBitmap("32bmMLogin_YES", 560, 12);
        } else {
            GUI_defDrawBitmap("33bmMLogin_NO", 560, 12);
        }
    }

}

/*****************************************************************
**      FunctionName:void InitDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**
**      call this function in _cbCallback --> WM_INIT_DIALOG
*****************************************************************/
static void _cbButton(WM_MESSAGE * pMsg)
{
    WM_HWIN  hWin;
    int 	 Id;
    hWin  = pMsg->hWin;
    Id = WM_GetId(hWin);

    switch (pMsg->MsgId) {
    case WM_PAINT:
        if (BUTTON_IsPressed(hWin)) {
            GUI_SetBkColor(GUI_WHITE);
//				GUI_Clear();
            GUI_SetAlpha(0xb0);
            GUI_defDrawBitmap(MPIC[(Id - GUI_ID_BUTTON0)], 0, 0);
            GUI_SetAlpha(0);
        } else {
            GUI_defDrawBitmap(MPIC[(Id - GUI_ID_BUTTON0)], 0, 0);
        }

        break;

    default:
        BUTTON_Callback(pMsg);
    }
}

static void _cbButton_Login(WM_MESSAGE * pMsg)
{
    WM_HWIN  hWin;
    hWin  = pMsg->hWin;

    switch (pMsg->MsgId) {
    case WM_PAINT:
        if (BUTTON_IsPressed(hWin)) {
            GUI_SetBkColor(GUI_WHITE);
//				GUI_Clear();
            GUI_SetAlpha(0xb0);

            if (sys_sta.login_type == LOGIN_GUEST) {
                GUI_defDrawBitmap("92bmMpic_logout", 0, 0);
            } else {
                GUI_defDrawBitmap("93bmMpic_login", 0, 0);
            }

            GUI_SetAlpha(0);
        } else {
            if (sys_sta.login_type == LOGIN_GUEST) {
                GUI_defDrawBitmap("92bmMpic_logout", 0, 0);
            } else {
                GUI_defDrawBitmap("93bmMpic_login", 0, 0);
            }
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

    for (MDev_ID = GUI_ID_TEXT0; MDev_ID <= GUI_ID_TEXT9; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        TEXT_SetTextAlign(hItem, GUI_TA_VCENTER | GUI_TA_HCENTER  );
        TEXT_SetFont(hItem, &GUI_USER_HZ24);
        TEXT_SetTextColor(hItem, GUI_WHITE);
    }

    TEXT_SetTextAlign(WM_GetDialogItem(hWin, GUI_ID_TEXT0), GUI_TA_VCENTER | GUI_TA_LEFT);
    TEXT_SetFont(WM_GetDialogItem(hWin, GUI_ID_TEXT0), &GUI_USER_HZ16);
    TEXT_SetFont(WM_GetDialogItem(hWin, GUI_ID_TEXT1), &GUI_USER_HZ24);
    TEXT_SetFont(WM_GetDialogItem(hWin, GUI_ID_TEXT2), &GUI_USER_HZ16);

    TEXT_SetFont(WM_GetDialogItem(hWin, GUI_ID_TEXT3), &GUI_USER_HZ24);
    TEXT_SetFont(WM_GetDialogItem(hWin, GUI_ID_TEXT4), &GUI_USER_HZ24);
    TEXT_SetFont(WM_GetDialogItem(hWin, GUI_ID_TEXT5), &GUI_USER_HZ24);

    TEXT_SetTextColor(WM_GetDialogItem(hWin, GUI_ID_TEXT9), GUI_RED);


    hTimer = WM_CreateTimer(WM_GetClientWindow(hWin), 0, 300, 0);

    for (MDev_ID = GUI_ID_BUTTON0; MDev_ID <= GUI_ID_BUTTON1; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, _cbButton);
    }

    for (MDev_ID = GUI_ID_BUTTON2; MDev_ID <= GUI_ID_BUTTON2; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, _cbButton_Login);
    }
}

static void	Show_Data(WM_HWIN hWin)
{
    int id = GUI_ID_TEXT2, j, num;
    time_t sys_time = GetSysTime();

    struct tm *t;

    show_sec++;

    if (show_sec >= 6) {
        show_sec = 0;
    }

    t = localtime((const time_t *)&sys_time);
    sprintf(show_buf, "%02d/%02d/%02d %02d:%02d:%02d", (t->tm_year - 100), (t->tm_mon + 1), t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);

    TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *) show_buf);

    sprintf(show_buf, "%.1f", (float)(cal_inf.temp * 1.0f / (float)TEMP_RATE));
    TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *) show_buf);

    sprintf(show_buf, "%.1f", cal_inf.lv_air[cal_inf.lv] * 1.0f / AIR_RATE);
    TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *) show_buf);

    if (cal_inf.SEN_EN[SENSOR_TYPE_HUM] == STA_OFF || cal_inf.SEN[SENSOR_TYPE_HUM] == (short)SENSOR_ER_VALUE) {
        TEXT_SetText(WM_GetDialogItem(hWin, id++), "--%");
    } else {
        sprintf(show_buf, "%d%%", cal_inf.SEN[SENSOR_TYPE_HUM]);
        TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *) show_buf);
    }

    sprintf(show_buf, "%.1f℃", (float)(cal_inf.target_temp * 1.0f / (float)TEMP_RATE));
    TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *) show_buf);

    sprintf(show_buf, "%.1f", cal_inf.min_air * 1.0f / AIR_RATE);
    TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *) show_buf);

    if (cal_inf.target_hum != SENSOR_ER_VALUE) {
	    sprintf(show_buf, "%d%%", cal_inf.target_hum);
    } else {
	    sprintf(show_buf, "%d%%", ctrl_inf.sencon[SENCON_HUM].target);
    }
	
    TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *) show_buf);

    sprintf(show_buf, "级别: %d/%d", cal_inf.lv + 1, cal_inf.max_lv + 1);
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT0), (const char *)show_buf);

    //show_buf[0] = 0;
    memset(show_buf, 0, sizeof(show_buf));

    for (j = 0, num = 0; j < ALA_MAX; j++) {
        if ((ALA_State() >> j) & 0x01 == 1) {
            if (j >= 2 && j <= 9) {
                sprintf(show_buf, "%s%d#%s;", show_buf, (id - 1), GUI_LANG_GetTextEx(alarm_show[j], Language_Bit));
            } else if (j >= 33 && j <= 40) {
                sprintf(show_buf, "%s%d#%s;", show_buf, (id - 32), GUI_LANG_GetTextEx(alarm_show[j], Language_Bit));
            } else {
                sprintf(show_buf, "%s%s;", show_buf, GUI_LANG_GetTextEx(alarm_show[j], Language_Bit));
            }
            num++;
        }

        if (num == 0) {
            WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON1));
            continue;
        }

        WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON1));

        if (num >= ALA_SHOW_NUM || (j == (ALA_MAX - 1) && num < ALA_SHOW_NUM)) {
            TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT9), (const char *)show_buf);
            break;
        }
    }

    if (num == 0) {
        TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT9), "");
    }

    if (ctrl_inf.var.mode == STA_ON) {
        TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT1), "基本信息");
    } else {
        if (show_sec >= 3) {
            TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT1), "");
        } else {
            TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT1), "空舍中");
        }
    }

    WM_InvalidateWindow(hWin);
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

    case WM_TIMER:
        Show_Data(hWin);
        WM_RestartTimer(hTimer, 300);
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

        case GUI_ID_BUTTON0:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                while (sys_sta.copy_bg_pic == STA_OFF)
                    OSTimeDly(100);

                Main_Task(hWin);
            }

            break;

        case GUI_ID_BUTTON1:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                while (sys_sta.copy_bg_pic == STA_OFF)
                    OSTimeDly(100);

                MAla_Record_Task(hWin);
            }

            break;

        case GUI_ID_BUTTON2:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                while (sys_sta.copy_bg_pic == STA_OFF)
                    OSTimeDly(100);

                MUser_Man_Task(hWin);
            }

            break;

        case GUI_ID_TEXT3:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                while (sys_sta.copy_bg_pic == STA_OFF)
                    OSTimeDly(100);

                MCtrl_TempCurve_Task(hWin);
            }

            break;

        case GUI_ID_TEXT4:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                while (sys_sta.copy_bg_pic == STA_OFF)
                    OSTimeDly(100);

				cal_list_type();
                MCtrl_LVView_Task(hWin,0,0);
            }

            break;

        case GUI_ID_TEXT5:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                while (sys_sta.copy_bg_pic == STA_OFF)
                    OSTimeDly(100);

                Set_OTSensor_Type(SENCON_HUM);
                MCtrl_OTSensor_Task(hWin);
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
extern WM_HWIN HBasic_Main;
void Creat_Basic_Main2(void)
{
    HBasic_Main = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, 0, 0, 0);
    Current_HWIN = HBasic_Main;
}


