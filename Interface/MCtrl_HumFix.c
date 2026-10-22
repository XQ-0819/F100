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

#define GUI_ID_EDIT10  (GUI_ID_USER+1)
#define GUI_ID_EDIT11  (GUI_ID_USER+2)
#define GUI_ID_EDIT12  (GUI_ID_USER+3)
#define GUI_ID_EDIT13  (GUI_ID_USER+4)
#define GUI_ID_EDIT14  (GUI_ID_USER+5)


static U8 select_id = 0;
static U8 temp_id = 0;
static TCHG_MENU Mchg_Menu[] = {
    {MSHOW, TYPE_TEXT, GUI_ID_TEXT2, "%s", 1, SNUM_FLAG},
    {MSHOW, TYPE_TEXT, GUI_ID_TEXT3, "%s", 1, SNUM_FLAG},
    {MSHOW, TYPE_TEXT, GUI_ID_TEXT4, "%s", 1, SNUM_FLAG},
    {MSHOW, TYPE_TEXT, GUI_ID_TEXT5, "%s", 1, SNUM_FLAG},
    {MSHOW, TYPE_TEXT, GUI_ID_TEXT6, "%s", 1, SNUM_FLAG},

    {MSHOW, TYPE_EDIT, GUI_ID_EDIT0, "%d", 0, 1},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT1, "%d", 0, 1},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT2, "%d", 0, 1},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT3, "%d", 0, 1},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT4, "%d", 0, 1},

    {MSHOW, TYPE_EDIT, GUI_ID_EDIT5, "%d", 0, 1},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT6, "%d", 0, 1},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT7, "%d", 0, 1},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT8, "%d", 0, 1},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT9, "%d", 0, 1},

    {MSHOW, TYPE_EDIT, GUI_ID_EDIT10, "%.2f", 0, HUM_SCALE},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT11, "%.2f", 0, HUM_SCALE},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT12, "%.2f", 0, HUM_SCALE},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT13, "%.2f", 0, HUM_SCALE},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT14, "%.2f", 0, HUM_SCALE},

    {MEXIT, 0, 0, "", 0, 0},
};

static void Set_OptValue(WM_HWIN hWin, U8 sid)
{
    U8 i = 0;
    //增加是否启用
    CHECKBOX_SetState(WM_GetDialogItem(hWin, GUI_ID_CHECK0 ), ctrl_inf.hum.enable);

    for (i = 0; i < 5; i++) {
        CHECKBOX_SetState(WM_GetDialogItem(hWin, (GUI_ID_CHECK1 + i) ), ctrl_inf.hum.x[5 * sid + i].enable);
    }

    Button_CurShow(hWin, sid, 2);
}

static void Menu_Value_Init(WM_HWIN hWin, U8 sid)
{
    U8 id = 0, i = 0;

    for (i = 0; i < 5; i++) {
        Mchg_Menu[id++].value = 5 * sid + i + 1;
    }

    for (i = 0; i < 5; i++) {
        Mchg_Menu[id++].value = ctrl_inf.hum.x[5 * sid + i].grow_day;
    }

    for (i = 0; i < 5; i++) {
        Mchg_Menu[id++].value = ctrl_inf.hum.x[5 * sid + i].target_hum;
    }

    for (i = 0; i < 5; i++) {
        Mchg_Menu[id++].value = ctrl_inf.hum.x[5 * sid + i].scale;
    }
}
/*********************************************************************
*
*       Dialog resource
*
* This table conatins the info required to create the dialog.
* It has been created by ucGUIbuilder.
*/
#define XPos_Collec  10
#define YPos_Collec  0
#define Y_Space     40
//#define XPos_Collec1 40
#define Text_X   100
#define Text_Y   36
#define Category_Space  10
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect,    "",               0,                       0,  0,  800, 480, 0, 0},
    { TEXT_CreateIndirect,      "控制管理-湿热补偿",          GUI_ID_TEXT0,           250, 4, 300, 42, 0, 0},

    { TEXT_CreateIndirect,      "湿热补偿",        GUI_ID_TEXT1,              60 + XPos_Collec, 60, 170, 30, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",        GUI_ID_CHECK0,                 190 + XPos_Collec, 60, 90, CHECK_SIZE, 0, 0},

    { TEXT_CreateIndirect,      "1",    	GUI_ID_TEXT2,               90 + XPos_Collec , 100 + YPos_Collec + Y_Space * 1 + Category_Space * 0, 24, Text_Y, 0, 0},
    { TEXT_CreateIndirect,      "2",      	GUI_ID_TEXT3,               90 + XPos_Collec , 100 + YPos_Collec + Y_Space * 2 + Category_Space * 1, 24, Text_Y, 0, 0},
    { TEXT_CreateIndirect,     	"3",       	GUI_ID_TEXT4,               90 + XPos_Collec , 100 + YPos_Collec + Y_Space * 3 + Category_Space * 2, 24, Text_Y, 0, 0},
    { TEXT_CreateIndirect,     	"4",     	GUI_ID_TEXT5,               90 + XPos_Collec , 100 + YPos_Collec + Y_Space * 4 + Category_Space * 3, 24, Text_Y, 0, 0},
    { TEXT_CreateIndirect,      "5",      	GUI_ID_TEXT6,               90 + XPos_Collec , 100 + YPos_Collec + Y_Space * 5 + Category_Space * 4, 24, Text_Y, 0, 0},


    { TEXT_CreateIndirect,      "日龄",     GUI_ID_TEXT7,               130 + XPos_Collec, 100 + YPos_Collec + Y_Space * 0, Text_X, Text_Y, 0, 0},
    { EDIT_CreateIndirect,     	"",      	GUI_ID_EDIT0,               130 + XPos_Collec, 100 + YPos_Collec + Y_Space * 1, Text_X, Text_Y, 0, 0},
    { EDIT_CreateIndirect,    	"",     	GUI_ID_EDIT1,               130 + XPos_Collec, 100 + YPos_Collec + Y_Space * 2 + Category_Space * 1, Text_X, Text_Y, 0, 0},
    { EDIT_CreateIndirect,    	"",         GUI_ID_EDIT2,               130 + XPos_Collec, 100 + YPos_Collec + Y_Space * 3 + Category_Space * 2, Text_X, Text_Y, 0, 0},
    { EDIT_CreateIndirect,    	"",         GUI_ID_EDIT3,               130 + XPos_Collec, 100 + YPos_Collec + Y_Space * 4 + Category_Space * 3, Text_X, Text_Y, 0, 0},
    { EDIT_CreateIndirect,    	"",         GUI_ID_EDIT4,               130 + XPos_Collec, 100 + YPos_Collec + Y_Space * 5 + Category_Space * 4, Text_X, Text_Y, 0, 0},

    { TEXT_CreateIndirect,     "目标湿度",    	GUI_ID_TEXT8,            	270 + XPos_Collec, 100 + YPos_Collec + Y_Space * 0, Text_X +20, Text_Y, 0, 0},
    { EDIT_CreateIndirect,     "",   		GUI_ID_EDIT5,               280 + XPos_Collec, 100 + YPos_Collec + Y_Space * 1, Text_X, Text_Y, 0, 0},
    { EDIT_CreateIndirect,     "",  		GUI_ID_EDIT6,              	280 + XPos_Collec, 100 + YPos_Collec + Y_Space * 2 + Category_Space * 1, Text_X, Text_Y, 0, 0},
    { EDIT_CreateIndirect,     "",    		GUI_ID_EDIT7,               280 + XPos_Collec, 100 + YPos_Collec + Y_Space * 3 + Category_Space * 2, Text_X, Text_Y, 0, 0},
    { EDIT_CreateIndirect,     "",      	GUI_ID_EDIT8,               280 + XPos_Collec, 100 + YPos_Collec + Y_Space * 4 + Category_Space * 3, Text_X, Text_Y, 0, 0},
    { EDIT_CreateIndirect,     "",       	GUI_ID_EDIT9,               280 + XPos_Collec, 100 + YPos_Collec + Y_Space * 5 + Category_Space * 4, Text_X, Text_Y, 0, 0},


    { TEXT_CreateIndirect,  	"补偿系数", GUI_ID_TEXT9,               430 + XPos_Collec, 100 + YPos_Collec + Y_Space * 0, Text_X, Text_Y, 0, 0},
    { EDIT_CreateIndirect,      "",      	GUI_ID_EDIT10,             	430 + XPos_Collec, 100 + YPos_Collec + Y_Space * 1, Text_X, Text_Y, 0, 0},
    { EDIT_CreateIndirect,      "",     	GUI_ID_EDIT11,              430 + XPos_Collec, 100 + YPos_Collec + Y_Space * 2 + Category_Space * 1, Text_X, Text_Y, 0, 0},
    { EDIT_CreateIndirect,      "",      	GUI_ID_EDIT12,              430 + XPos_Collec, 100 + YPos_Collec + Y_Space * 3 + Category_Space * 2, Text_X, Text_Y, 0, 0},
    { EDIT_CreateIndirect,      "",       	GUI_ID_EDIT13,              430 + XPos_Collec, 100 + YPos_Collec + Y_Space * 4 + Category_Space * 3, Text_X, Text_Y, 0, 0},
    { EDIT_CreateIndirect,      "",      	GUI_ID_EDIT14,              430 + XPos_Collec, 100 + YPos_Collec + Y_Space * 5 + Category_Space * 4, Text_X, Text_Y, 0, 0},

    { CHECKBOX_CreateIndirect,  "启用",        GUI_ID_CHECK1,        580 + XPos_Collec, 100 + YPos_Collec + Y_Space * 1 + Category_Space * 0.5, 120, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",        GUI_ID_CHECK2,        580 + XPos_Collec, 100 + YPos_Collec + Y_Space * 2 + Category_Space * 1.5, 120, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",        GUI_ID_CHECK3,        580 + XPos_Collec, 100 + YPos_Collec + Y_Space * 3 + Category_Space * 2.5, 120, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",        GUI_ID_CHECK4,        580 + XPos_Collec, 100 + YPos_Collec + Y_Space * 4 + Category_Space * 3.5, 120, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",        GUI_ID_CHECK5,        580 + XPos_Collec, 100 + YPos_Collec + Y_Space * 5 + Category_Space * 4.5, 120, CHECK_SIZE, 0, 0},

    { BUTTON_CreateIndirect,    "上一页",          GUI_ID_BUTTON0,          MENU_BACK_X1, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
    { BUTTON_CreateIndirect,    "下一页",          GUI_ID_BUTTON1,          MENU_BACK_X2, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
    { BUTTON_CreateIndirect,    "返回",            GUI_ID_BUTTON2,         MENU_BACK_X3, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},

    { BUTTON_CreateIndirect,    "",     GUI_ID_BUTTON4,           			660, 0, 90, 60, 0, 0},
};

static void Get_OptValue(WM_HWIN hWin, U8 sid)
{
    U8 i = 0;
    //增加是否启用
    ctrl_inf.hum.enable = CHECKBOX_GetState(WM_GetDialogItem(hWin, GUI_ID_CHECK0 ));

    for (i = 0; i < 5; i++) {
        ctrl_inf.hum.x[5 * sid + i].enable = CHECKBOX_GetState(WM_GetDialogItem(hWin, (GUI_ID_CHECK1 + i) ));
    }
}


static void Grow_Day_Get(void)
{
    float ftemp = Get_Input_Value();
    short stemp = (short)ftemp;

//    if (stemp > TEMP_ADJMAX) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = TEMP_ADJMAX;
//    }

    if (stemp < 0) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 0;
    }

    ctrl_inf.hum.x[5 * select_id + temp_id].grow_day = stemp;
    MChange_Menu(Menu_Value_Init, Current_HWIN, (PCHG_MENU )Mchg_Menu, select_id);
}

static void Target_Hum_Get(void)
{
    float ftemp = Get_Input_Value();
    short stemp = (short)ftemp;

    if (stemp > HUM_MAX) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = HUM_MAX;
    }

    if (stemp < 0) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 0;
    }

    ctrl_inf.hum.x[5 * select_id + temp_id].target_hum = stemp;
    MChange_Menu(Menu_Value_Init, Current_HWIN, (PCHG_MENU )Mchg_Menu, select_id);
}

static void Scale_Get(void)
{
    float ftemp = Get_Input_Value() * HUM_SCALE;
    short stemp = (short)ftemp;

    if (stemp > MAX_PERCENT) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = MAX_PERCENT;
    }

    if (stemp < 0) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 0;
    }

    ctrl_inf.hum.x[5 * select_id + temp_id].scale = stemp;
    MChange_Menu(Menu_Value_Init, Current_HWIN, (PCHG_MENU )Mchg_Menu, select_id);
}


/*****************************************************************
**      FunctionName:void PaintDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**
**      call this function in _cbCallback --> WM_PAINT
*****************************************************************/

static void PaintDialog(WM_MESSAGE * pMsg)
{
//    WM_HWIN hWin = pMsg->hWin;
    Show_MBackPic();
    GUI_SetColor(GUI_LINE_COLOR);
    GUI_DrawRoundedRect(66, 55, 700, 396, 10);
    GUI_DrawRoundedRect(62, 52, 704, 399, 10);
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

    for (MDev_ID = GUI_ID_BUTTON0; MDev_ID <= GUI_ID_BUTTON2; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, Cut_cbButton);
    }

    for (MDev_ID = GUI_ID_BUTTON4; MDev_ID <= GUI_ID_BUTTON4; MDev_ID++) {//set
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, set_cbButton);
    }

    for (MDev_ID = GUI_ID_EDIT0; MDev_ID <= GUI_ID_EDIT9; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Edit_SetHZ24(hItem);
    }

    for (MDev_ID = GUI_ID_EDIT10; MDev_ID <= GUI_ID_EDIT14; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Edit_SetHZ24(hItem);
    }

    for (MDev_ID = GUI_ID_CHECK0; MDev_ID <= GUI_ID_CHECK5; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        CHECKBOX_SetText(hItem, GUI_LANG_GetTextEx(Lang_Enable, Language_Bit));
        Check_SetHZ24(hItem);
        Login_Show(hItem, LOGIN_ADMIN);
    }

    for (MDev_ID = GUI_ID_TEXT0; MDev_ID <= GUI_ID_TEXT9; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24(hItem);

        if (MDev_ID >= GUI_ID_TEXT0 && MDev_ID <= GUI_ID_TEXT6) {
            Text_SetHZ24L(hItem);
        }

        if (MDev_ID == GUI_ID_TEXT0) {
            TEXT_SetTextAlign(hItem,  GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetTextColor(hItem, GUI_WHITE);
        }
    }

    sprintf(show_buf, "%s-%s", GUI_LANG_GetTextEx(Lang_Control, Language_Bit), GUI_LANG_GetTextEx(Lang_RH_Compensation, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT0), show_buf);                                               // 控制管理-湿热补偿
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT1), GUI_LANG_GetTextEx(Lang_RH_Compensation, Language_Bit)); // 湿热补偿
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT7), GUI_LANG_GetTextEx(Lang_Day, Language_Bit));             // 日龄
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT8), GUI_LANG_GetTextEx(Lang_Tar_RH, Language_Bit));          // 目标湿度
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT9), GUI_LANG_GetTextEx(Lang_Comp_Num, Language_Bit));        // 补偿系数

    Set_OptValue(hWin, select_id);
    MChange_Menu(Menu_Value_Init, hWin, (PCHG_MENU )Mchg_Menu, select_id);
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

        if (Id >= GUI_ID_EDIT10 && Id <= GUI_ID_EDIT14 && NCode == WM_NOTIFICATION_RELEASED
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
                Get_OptValue(hWin, select_id);

                if (Config_Save(SHUM_ID) < 0) {
                    MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_SAVE_ER));
                }

                Current_HWIN = WM_GetParent(hWin);
                GUI_EndDialog(hWin, 0);
            }

            break;

        case GUI_ID_BUTTON0:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                if (select_id == 1) {
                    Get_OptValue(hWin, select_id);
                    select_id = 0;
                    Set_OptValue(hWin, select_id);
                    MChange_Menu(Menu_Value_Init, hWin, (PCHG_MENU )Mchg_Menu, select_id);
                }
            }

            break;

        case GUI_ID_BUTTON1:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                if (select_id == 0) {
                    Get_OptValue(hWin, select_id);
                    select_id = 1;
                    Set_OptValue(hWin, select_id);
                    MChange_Menu(Menu_Value_Init, hWin, (PCHG_MENU )Mchg_Menu, select_id);
                }
            }

            break;

        case GUI_ID_BUTTON4:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                MCtrl_HumFix_Set_Task(hWin);
            }

            break;

        case GUI_ID_EDIT0:
        case GUI_ID_EDIT1:
        case GUI_ID_EDIT2:
        case GUI_ID_EDIT3:
        case GUI_ID_EDIT4:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                temp_id = (Id - GUI_ID_EDIT0);
                Set_Output_Callback(Grow_Day_Get);
                Set_Input_Len(3);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT5:
        case GUI_ID_EDIT6:
        case GUI_ID_EDIT7:
        case GUI_ID_EDIT8:
        case GUI_ID_EDIT9:
            if (NCode == WM_NOTIFICATION_RELEASED) {			
                temp_id = (Id - GUI_ID_EDIT5);
                Set_Output_Callback(Target_Hum_Get);
                Set_Input_Len(3);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT10:
        case GUI_ID_EDIT11:
        case GUI_ID_EDIT12:
        case GUI_ID_EDIT13:
        case GUI_ID_EDIT14:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                temp_id = (Id - GUI_ID_EDIT10);
                Set_Output_Callback(Scale_Get);
                Set_Input_Len(4);
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
WM_HWIN MCtrl_HumFix;
void MCtrl_HumFix_Task(WM_HWIN hParent)
{
    select_id = 0;
    MCtrl_HumFix = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    Current_HWIN = MCtrl_HumFix;
}

