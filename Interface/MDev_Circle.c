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


/*********************************************************************
*
*       Dialog resource
*
* This table conatins the info required to create the dialog.
* It has been created by ucGUIbuilder.
*/
//#define XPos_Collec  60
//#define YPos_Collec  0
//#define Y_Space     40
//#define XPos_Collec1 40

#define XPos_Inital    	0
#define XPos_Collec  	0
#define YPos_Collec  	20
#define Y_Space     	70
#define X_Space   		140
#define Text_Width   	36
#define Category_Space  10

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect,    "",               0,                       0,  0,  800, 480, 0, 0},
    { TEXT_CreateIndirect,      "设备管理-环流风机",          GUI_ID_TEXT0,            250, 4, 300, 42, 0, 0},

    { TEXT_CreateIndirect,      "环流风机:启用",   GUI_ID_TEXT1,              	90 + XPos_Collec, 60, 150, 36, 0, 0},
    { TEXT_CreateIndirect,      "启动偏差",   GUI_ID_TEXT2,              	390 + XPos_Collec, 60, 100, 36, 0, 0},
    { EDIT_CreateIndirect,      "0.3",        GUI_ID_EDIT0,                 510 + XPos_Collec, 60, 60, 36, 0, 0},
    { TEXT_CreateIndirect,      "度",         GUI_ID_TEXT3,                 580 + XPos_Collec, 60, 30, 36, 0, 0},


    { TEXT_CreateIndirect,       "开始时间",    GUI_ID_TEXT4,           XPos_Inital + X_Space * 1 + XPos_Collec, 100 + YPos_Collec + Y_Space * 0, 125, Text_Width, 0, 0},
    { TEXT_CreateIndirect,       "停止时间",    GUI_ID_TEXT5,           XPos_Inital + X_Space * 2 + XPos_Collec, 100 + YPos_Collec + Y_Space * 0, 125, Text_Width, 0, 0},
    { TEXT_CreateIndirect,       "开始级别",    GUI_ID_TEXT6,           XPos_Inital + X_Space * 3 + XPos_Collec, 100 + YPos_Collec + Y_Space * 0, 125, Text_Width, 0, 0},
    { TEXT_CreateIndirect,       "停止级别",    GUI_ID_TEXT7,           XPos_Inital + X_Space * 4 + XPos_Collec, 100 + YPos_Collec + Y_Space * 0, 125, Text_Width, 0, 0},

    { EDIT_CreateIndirect,     	 "8:00",      	GUI_ID_EDIT1,            20 + XPos_Inital + X_Space * 1 + XPos_Collec, 70 + YPos_Collec + Y_Space * 1, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,    	 "10:00",     	GUI_ID_EDIT2,            20 + XPos_Inital + X_Space * 2 + XPos_Collec, 70 + YPos_Collec + Y_Space * 1, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,       "1",        	GUI_ID_EDIT3,            20 + XPos_Inital + X_Space * 3 + XPos_Collec, 70 + YPos_Collec + Y_Space * 1, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,       "3",         	GUI_ID_EDIT4,            20 + XPos_Inital + X_Space * 4 + XPos_Collec, 70 + YPos_Collec + Y_Space * 1, 72, Text_Width, 0, 0},

    { CHECKBOX_CreateIndirect,  "温度1",        GUI_ID_CHECK1,         XPos_Inital + X_Space * 1 + XPos_Collec, 70 + YPos_Collec + Y_Space * 2, 125, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "温度2",        GUI_ID_CHECK2,         XPos_Inital + X_Space * 2 + XPos_Collec, 70 + YPos_Collec + Y_Space * 2, 125, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "温度3",        GUI_ID_CHECK3,         XPos_Inital + X_Space * 3 + XPos_Collec, 70 + YPos_Collec + Y_Space * 2, 125, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "温度4",        GUI_ID_CHECK4,         XPos_Inital + X_Space * 4 + XPos_Collec, 70 + YPos_Collec + Y_Space * 2, 125, CHECK_SIZE, 0, 0},

    { CHECKBOX_CreateIndirect,  "温度5",        GUI_ID_CHECK5,         XPos_Inital + X_Space * 1 + XPos_Collec, 70 + YPos_Collec + Y_Space * 3, 125, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "温度6",        GUI_ID_CHECK6,         XPos_Inital + X_Space * 2 + XPos_Collec, 70 + YPos_Collec + Y_Space * 3, 125, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "温度7",        GUI_ID_CHECK7,         XPos_Inital + X_Space * 3 + XPos_Collec, 70 + YPos_Collec + Y_Space * 3, 125, CHECK_SIZE, 0, 0},
//    { CHECKBOX_CreateIndirect,  "温度8",        GUI_ID_CHECK8,         XPos_Inital + X_Space * 4 + XPos_Collec, 70 + YPos_Collec + Y_Space * 3, 125, CHECK_SIZE, 0, 0},

//    { BUTTON_CreateIndirect,    "上一页",          GUI_ID_BUTTON0,          MENU_BACK_X1, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
//    { BUTTON_CreateIndirect,    "下一页",          GUI_ID_BUTTON1,          MENU_BACK_X2, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
    { BUTTON_CreateIndirect,    "返回",             GUI_ID_BUTTON2,         MENU_BACK_X3, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},


};


static void Set_OptValue(WM_HWIN hWin, U8 sid)
{
    U8 i = 0;
    //增加是否启用

    for (i = 0; i < 7; i++) {
        CHECKBOX_SetState(WM_GetDialogItem(hWin, (GUI_ID_CHECK1 + i) ), dev_inf.circle.temp_sta[i]);
    }
}

static void Get_OptValue(WM_HWIN hWin, U8 sid)
{
    U8 i = 0;

    for (i = 0; i < 7; i++) {
        dev_inf.circle.temp_sta[i] = CHECKBOX_GetState(WM_GetDialogItem(hWin, GUI_ID_CHECK1 + i ));
    }
}
/*****************************************************************
**      FunctionName:void PaintDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**
**      call this function in _cbCallback --> WM_PAINT
*****************************************************************/

static void PaintDialog(WM_MESSAGE * pMsg)
{
    Show_MBackPic();

    GUI_SetColor(GUI_LINE_COLOR);
    GUI_DrawRoundedRect(70, 55, 712, 356, 10);
    GUI_DrawRoundedRect(66, 52, 716, 359, 10);
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
    int MDev_ID = 0, i = 0;
    WM_HWIN hItem;


    for (MDev_ID = GUI_ID_BUTTON2; MDev_ID <= GUI_ID_BUTTON2; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, Cut_cbButton);
    }

    for (MDev_ID = GUI_ID_EDIT0; MDev_ID <= GUI_ID_EDIT5; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Edit_SetHZ24(hItem);
    }

    for (MDev_ID = GUI_ID_CHECK1; MDev_ID <= GUI_ID_CHECK7; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Check_SetHZ24(hItem);
		Login_Show(hItem, LOGIN_ADMIN);
    }

    CHECKBOX_SetText(WM_GetDialogItem(hWin, GUI_ID_CHECK0), GUI_LANG_GetTextEx(Lang_Enable, Language_Bit));

    for (MDev_ID = GUI_ID_CHECK1; MDev_ID <= GUI_ID_CHECK7; MDev_ID++) {
        sprintf(show_buf, "%s%d", GUI_LANG_GetTextEx(Lang_Temp, Language_Bit), MDev_ID - GUI_ID_CHECK1 + 1);
        CHECKBOX_SetText(WM_GetDialogItem(hWin, MDev_ID), show_buf);
    }

    for (i = 0; i < TEMP_NUM_MAX; i++) { //temp
        if (dev_inf.sensor_adj.TEMP_EN[i] == STA_OFF)
            WM_DisableWindow(WM_GetDialogItem(hWin, GUI_ID_CHECK1 + i));
    }

    for (MDev_ID = GUI_ID_TEXT0; MDev_ID <= GUI_ID_TEXT7; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24(hItem);
        switch (MDev_ID) {
        case GUI_ID_TEXT0:
            TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetTextColor(hItem, GUI_WHITE);
            sprintf(show_buf, "%s-%s", GUI_LANG_GetTextEx(Lang_Device, Language_Bit), GUI_LANG_GetTextEx(Lang_Circulating_Fan, Language_Bit));
            TEXT_SetText(hItem, show_buf);
            break;
        case GUI_ID_TEXT1:
            sprintf(show_buf, "%s:%s", GUI_LANG_GetTextEx(Lang_Circulating_Fan, Language_Bit), GUI_LANG_GetTextEx(Lang_Enable, Language_Bit));
            TEXT_SetText(hItem, show_buf);
            break;
        case GUI_ID_TEXT2:
            TEXT_SetText(hItem, GUI_LANG_GetTextEx(Lang_Start_Tempdiff, Language_Bit));
            break;
        case GUI_ID_TEXT3:
            TEXT_SetText(hItem, GUI_LANG_GetTextEx(Lang_oC, Language_Bit));
            break;
        case GUI_ID_TEXT4:
            TEXT_SetText(hItem, GUI_LANG_GetTextEx(Lang_Start_time, Language_Bit));
            break;
        case GUI_ID_TEXT5:
            TEXT_SetText(hItem, GUI_LANG_GetTextEx(Lang_End_time, Language_Bit));
            break;
        case GUI_ID_TEXT6:
            TEXT_SetText(hItem, GUI_LANG_GetTextEx(Lang_Min_Lv, Language_Bit));
            break;
        case GUI_ID_TEXT7:
            TEXT_SetText(hItem, GUI_LANG_GetTextEx(Lang_Max_Lv, Language_Bit));
            break;
        }
    }

    sprintf(show_buf, "%.1f", (float)(dev_inf.circle.offset * 1.0f / (float) TEMP_RATE));
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT0), (const char *) show_buf);

    sprintf(show_buf, "%02d:%02d",  dev_inf.circle.start_time / 60, dev_inf.circle.start_time % 60);
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT1), show_buf);

    sprintf(show_buf, "%02d:%02d",  dev_inf.circle.stop_time / 60, dev_inf.circle.stop_time % 60);
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT2), show_buf);

    sprintf(show_buf, "%d",  dev_inf.circle.start_lv + 1);
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT3), show_buf);

    sprintf(show_buf, "%d",  dev_inf.circle.stop_lv + 1);
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT4), show_buf);
    Set_OptValue(hWin, 0);
}


static void Temp_Get(void)
{
    float ftemp = Get_Input_Value() * TEMP_RATE;
    short stemp = (short)ftemp;

    if (stemp > TEMP_ADJMAX) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = TEMP_ADJMAX;
    }

    if (stemp < TEMP_ADJMIN) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = TEMP_ADJMIN;
    }

    dev_inf.circle.offset = stemp;
    sprintf(show_buf, "%.1f", (float)(dev_inf.circle.offset * 1.0f / (float) TEMP_RATE));
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, GUI_ID_EDIT0), (const char *) show_buf);
}
static void Start_Time_Get(void)
{
    float ftemp = Get_Input_Value() ;
    short stemp = (short)ftemp;

    stemp = (stemp / 100) * 60 + (stemp % 100);

    if (stemp > MAX_TIME_DAY) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = MAX_TIME_DAY;
    }

    if (stemp < 0) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 0;
    }

    dev_inf.circle.start_time = stemp;
    sprintf(show_buf, "%02d:%02d",  dev_inf.circle.start_time / 60, dev_inf.circle.start_time % 60);
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, GUI_ID_EDIT1 ), show_buf);
}
static void Stop_Time_Get(void)
{
    float ftemp = Get_Input_Value() ;
    short stemp = (short)ftemp;

    stemp = (stemp / 100) * 60 + (stemp % 100);

    if (stemp > MAX_TIME_DAY) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = MAX_TIME_DAY;
    }

    if (stemp < 0) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 0;
    }

    dev_inf.circle.stop_time = stemp;
    sprintf(show_buf, "%02d:%02d",  dev_inf.circle.stop_time / 60, dev_inf.circle.stop_time % 60);
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, GUI_ID_EDIT2 ), show_buf);
}

static void Start_LV_Get(void)
{
    float ftemp = Get_Input_Value() ;
    short stemp = (short)ftemp;

    if (stemp > CTRL_LV_NUM) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = CTRL_LV_NUM;
    }

    if (stemp < 1) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 1;
    }

    dev_inf.circle.start_lv = (stemp - 1);
    sprintf(show_buf, "%d",  dev_inf.circle.start_lv + 1);
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, GUI_ID_EDIT3), show_buf);
}

static void Stop_LV_Get(void)
{
    float ftemp = Get_Input_Value() ;
    short stemp = (short)ftemp;

    if (stemp > CTRL_LV_NUM) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = CTRL_LV_NUM;
    }

    if (stemp < 1) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 1;
    }

    dev_inf.circle.stop_lv = (stemp - 1);
    sprintf(show_buf, "%d",  dev_inf.circle.stop_lv + 1);
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, GUI_ID_EDIT4), show_buf);
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
                Get_OptValue(hWin, 0);

                if (Config_Save(SCIRCLE_ID) < 0) {
                    MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_SAVE_ER));
                }
				
                Current_HWIN = WM_GetParent(hWin);			
                GUI_EndDialog(hWin, 0);
            }

            break;

        case GUI_ID_EDIT0:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(Temp_Get);
                Set_Input_Len(4);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT1:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(Start_Time_Get);
                Set_Input_Len(4);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT2:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(Stop_Time_Get);
                Set_Input_Len(4);
                Input_Task(hWin);
            }

            break;


        case GUI_ID_EDIT3:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(Start_LV_Get);
                Set_Input_Len(2);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT4:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(Stop_LV_Get);
                Set_Input_Len(2);
                Input_Task(hWin);
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
WM_HWIN MDev_Circle;
void MDev_Circle_Task(WM_HWIN hParent)
{
    MDev_Circle = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    Current_HWIN = MDev_Circle;
}

