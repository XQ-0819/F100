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

extern const char number_id[11][8];
/*********************************************************************
*
*       static data
*
**********************************************************************
*/
static U8 select_id = 0;
static U8 select_num = 0;//选择哪个
static U8 select_page = 0;
static U8 select_chg_flag = 0;//是否能选择

static void Set_OptValue(WM_HWIN hWin, U8 sid)
{
    U8 i = 0;
    //增加是否启用

    switch (sid) {
    case TIMER_MODE_CYCLE:

        TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT8), GUI_LANG_GetTextEx(Lang_Start_time, Language_Bit));
        TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT9), GUI_LANG_GetTextEx(Lang_End_time, Language_Bit));

        for (i = 0; i < 5; i++) {
            WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_CHECK1 + i));
        }

        for (i = 0; i < 4; i++) {
            WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT4 + i));
            WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT1 + i));
            WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT6 + i));
        }

        sprintf(show_buf, "%d Min",  ext_inf.timer[select_num].start_min);
        EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT0), show_buf);
        sprintf(show_buf, "%d Min",  ext_inf.timer[select_num].stop_min);
        EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT5), show_buf);

        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON0));
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON1));
        break;

    case TIMER_MODE_ON_OFF:

        TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT8), GUI_LANG_GetTextEx(Lang_Start_time, Language_Bit));
        TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT9), GUI_LANG_GetTextEx(Lang_End_time, Language_Bit));

        for (i = 0; i < 5; i++) {
            WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_CHECK1 + i));
            CHECKBOX_SetState(WM_GetDialogItem(hWin, GUI_ID_CHECK1 + i), ext_inf.timer[select_num].num_en[5 * select_page + i]);
        }

        for (i = 0; i < 5; i++) {
            WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT4 + i));
            WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT0 + i));
            WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT5 + i));
            sprintf(show_buf, "%02d:%02d",  ext_inf.timer[select_num].start_time[5 * select_page + i] / 60, ext_inf.timer[select_num].start_time[5 * select_page + i] % 60);
            EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT0 + i), show_buf);
            sprintf(show_buf, "%02d:%02d",  ext_inf.timer[select_num].stop_time[5 * select_page + i] / 60, ext_inf.timer[select_num].stop_time[5 * select_page + i] % 60);
            EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT5 + i), show_buf);
        }

        for (i = 0; i < 5; i++) {
            TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT3 + i), number_id[5 * select_page + i + 1]);
        }

        Button_CurShow(hWin, select_page, 2);
        break;

    case TIMER_MODE_ON_DELAY:

        TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT8), GUI_LANG_GetTextEx(Lang_Start_time, Language_Bit));
        TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT9), GUI_LANG_GetTextEx(Lang_Delay_Time, Language_Bit));

        for (i = 0; i < 5; i++) {
            WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_CHECK1 + i));
            CHECKBOX_SetState(WM_GetDialogItem(hWin, GUI_ID_CHECK1 + i), ext_inf.timer[select_num].num_en[5 * select_page + i]);
        }

        for (i = 0; i < 5; i++) {
            WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT4 + i));
            WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT0 + i));
            WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT5 + i));
            sprintf(show_buf, "%02d:%02d",  ext_inf.timer[select_num].start_time[5 * select_page + i] / 60, ext_inf.timer[select_num].start_time[5 * select_page + i] % 60);
            EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT0 + i), show_buf);
            sprintf(show_buf, "%d Min",  ext_inf.timer[select_num].delay_min[5 * select_page + i]);
            EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT5 + i), show_buf);
        }

        for (i = 0; i < 5; i++) {
            TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT3 + i), number_id[5 * select_page + i + 1]);
        }

        Button_CurShow(hWin, select_page, 2);
        break;

    default:
        break;

    }

    DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN0), select_id);//必须放后面
    DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN1), ext_inf.timer[select_num].type);//必须放后面

    if (ext_inf.timer[select_num].enable == STA_ON) {
        sprintf(show_buf, ":%s", GUI_LANG_GetTextEx(Lang_Enable, Language_Bit));
        TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT2), show_buf);
    } else {
        sprintf(show_buf, ":%s", GUI_LANG_GetTextEx(Lang_Disable, Language_Bit));
        TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT2), show_buf);
    }
}


/*********************************************************************
*
*       Dialog resource
*
* This table conatins the info required to create the dialog.
* It has been created by ucGUIbuilder.
*/

#define XPos_Inital    	20
#define XPos_Collec  	10
#define YPos_Collec  	0
#define Y_Space     	40
#define X_Space   		180
#define Text_Width   	36
#define Category_Space  10



static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect,    "",               0,                       0,  0,  800, 480, 0, 0},
    { TEXT_CreateIndirect,      "设备管理-定时器",          GUI_ID_TEXT0,            250, 4, 300, 42, 0, 0},

    { TEXT_CreateIndirect,      "类型:",        GUI_ID_TEXT1,              300 + XPos_Inital + X_Space * 0 + XPos_Collec, 68, 120, 36, 0, 0},
    { DROPDOWN_CreateIndirect,     "",      GUI_ID_DROPDOWN1,        360 + XPos_Inital + X_Space * 0 + XPos_Collec, 68, 120, 146, 0, 0},
    { DROPDOWN_CreateIndirect,     "",      GUI_ID_DROPDOWN0,        490 + XPos_Inital + X_Space * 0 + XPos_Collec, 68, 210, 110, 0, 0},

    { TEXT_CreateIndirect,      " ",        GUI_ID_TEXT2,              240 + XPos_Collec, 68, 200, Text_Width, 0, 0},
    { DROPDOWN_CreateIndirect,	 "",		GUI_ID_DROPDOWN2,		100 + XPos_Collec, 68, 130, 150, 0, 0},

    { TEXT_CreateIndirect,       "1.",    GUI_ID_TEXT3,              100 + XPos_Inital + X_Space * 0 + XPos_Collec, 100 + YPos_Collec + Y_Space * 1 + Category_Space * 0, 60, Text_Width, 0, 0},
    { TEXT_CreateIndirect,      "2.",      GUI_ID_TEXT4,             100 + XPos_Inital + X_Space * 0 + XPos_Collec, 100 + YPos_Collec + Y_Space * 2 + Category_Space * 1, 60, Text_Width, 0, 0},
    { TEXT_CreateIndirect,     "3.",       GUI_ID_TEXT5,              100 + XPos_Inital + X_Space * 0 + XPos_Collec, 100 + YPos_Collec + Y_Space * 3 + Category_Space * 2, 60, Text_Width, 0, 0},
    { TEXT_CreateIndirect,     "4.",     GUI_ID_TEXT6,                100 + XPos_Inital + X_Space * 0 + XPos_Collec, 100 + YPos_Collec + Y_Space * 4 + Category_Space * 3, 60, Text_Width, 0, 0},
    { TEXT_CreateIndirect,     "5.",      GUI_ID_TEXT7,               100 + XPos_Inital + X_Space * 0 + XPos_Collec, 100 + YPos_Collec + Y_Space * 5 + Category_Space * 4, 60, Text_Width, 0, 0},


    { TEXT_CreateIndirect,        "开启时间",    GUI_ID_TEXT8,        2 + XPos_Inital + X_Space * 1 + XPos_Collec, 100 + YPos_Collec + Y_Space * 0, 100, Text_Width, 0, 0},
    { EDIT_CreateIndirect,      "10:00",   GUI_ID_EDIT0,               XPos_Inital + X_Space * 1 + XPos_Collec, 100 + YPos_Collec + Y_Space * 1, 100, Text_Width, 0, 0},
    { EDIT_CreateIndirect,       "14:00",  GUI_ID_EDIT1,              XPos_Inital + X_Space * 1 + XPos_Collec, 100 + YPos_Collec + Y_Space * 2 + Category_Space * 1, 100, Text_Width, 0, 0},
    { EDIT_CreateIndirect,     "17:00",    GUI_ID_EDIT2,              XPos_Inital + X_Space * 1 + XPos_Collec, 100 + YPos_Collec + Y_Space * 3 + Category_Space * 2, 100, Text_Width, 0, 0},
    { EDIT_CreateIndirect,    "24:00",      GUI_ID_EDIT3,            XPos_Inital + X_Space * 1 + XPos_Collec, 100 + YPos_Collec + Y_Space * 4 + Category_Space * 3, 100, Text_Width, 0, 0},
    { EDIT_CreateIndirect,     "8:00",       GUI_ID_EDIT4,           XPos_Inital + X_Space * 1 + XPos_Collec, 100 + YPos_Collec + Y_Space * 5 + Category_Space * 4, 100, Text_Width, 0, 0},


    { TEXT_CreateIndirect,  "关闭时间", GUI_ID_TEXT9,                    2 + XPos_Inital + X_Space * 2 + XPos_Collec, 100 + YPos_Collec + Y_Space * 0, 100, Text_Width, 0, 0},
    { EDIT_CreateIndirect,      "3%",      GUI_ID_EDIT5,                  XPos_Inital + X_Space * 2 + XPos_Collec, 100 + YPos_Collec + Y_Space * 1, 100, Text_Width, 0, 0},
    { EDIT_CreateIndirect,      "33%",     GUI_ID_EDIT6,                   XPos_Inital + X_Space * 2 + XPos_Collec, 100 + YPos_Collec + Y_Space * 2 + Category_Space * 1, 100, Text_Width, 0, 0},
    { EDIT_CreateIndirect,      "33%",      GUI_ID_EDIT7,                  XPos_Inital + X_Space * 2 + XPos_Collec, 100 + YPos_Collec + Y_Space * 3 + Category_Space * 2, 100, Text_Width, 0, 0},
    { EDIT_CreateIndirect,      "53%",       GUI_ID_EDIT8,                 XPos_Inital + X_Space * 2 + XPos_Collec, 100 + YPos_Collec + Y_Space * 4 + Category_Space * 3, 100, Text_Width, 0, 0},
    { EDIT_CreateIndirect,      "53%",      GUI_ID_EDIT9,                  XPos_Inital + X_Space * 2 + XPos_Collec, 100 + YPos_Collec + Y_Space * 5 + Category_Space * 4, 100, Text_Width, 0, 0},


    { CHECKBOX_CreateIndirect,  "启用",        GUI_ID_CHECK1,        550 + XPos_Collec, 100 + YPos_Collec + Y_Space * 1 + Category_Space * 0.5, 90, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",        GUI_ID_CHECK2,        550 + XPos_Collec, 100 + YPos_Collec + Y_Space * 2 + Category_Space * 1.5, 90, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",        GUI_ID_CHECK3,        550 + XPos_Collec, 100 + YPos_Collec + Y_Space * 3 + Category_Space * 2.5, 90, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",        GUI_ID_CHECK4,        550 + XPos_Collec, 100 + YPos_Collec + Y_Space * 4 + Category_Space * 3.5, 90, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",        GUI_ID_CHECK5,        550 + XPos_Collec, 100 + YPos_Collec + Y_Space * 5 + Category_Space * 4.5, 90, CHECK_SIZE, 0, 0},

    { BUTTON_CreateIndirect,    "上一页",          GUI_ID_BUTTON0,          MENU_BACK_X1, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
    { BUTTON_CreateIndirect,    "下一页",          GUI_ID_BUTTON1,          MENU_BACK_X2, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
    { BUTTON_CreateIndirect,    "返回",             GUI_ID_BUTTON2,         MENU_BACK_X3, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},


};

static int temp_id = 0;

static void Get_OptValue(WM_HWIN hWin, U8 sid)
{
    U8 i = 0;

    if (sid != TIMER_MODE_CYCLE) {
        for (i = 0; i < 5; i++) {
            ext_inf.timer[select_num].num_en[5 * select_page + i] = CHECKBOX_GetState(WM_GetDialogItem(hWin, GUI_ID_CHECK1 + i ));
        }
    }

    ext_inf.timer[select_num].mode = DROPDOWN_GetSel(WM_GetDialogItem(hWin, (GUI_ID_DROPDOWN0)));
    ext_inf.timer[select_num].type = DROPDOWN_GetSel(WM_GetDialogItem(hWin, (GUI_ID_DROPDOWN1)));
    select_id = ext_inf.timer[select_num].mode;
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
    GUI_DrawRoundedRect(70, 55, 730, 406, 10);
    GUI_DrawRoundedRect(66, 52, 734, 409, 10);
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
    u8 i = 0;

    for (MDev_ID = GUI_ID_BUTTON0; MDev_ID <= GUI_ID_BUTTON2; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, Cut_cbButton);
    }


    for (MDev_ID = GUI_ID_EDIT0; MDev_ID <= GUI_ID_EDIT9; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Edit_SetHZ24(hItem);
    }

    for (MDev_ID = GUI_ID_CHECK1; MDev_ID <= GUI_ID_CHECK5; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        CHECKBOX_SetText(hItem, GUI_LANG_GetTextEx(Lang_Enable, Language_Bit));
        Check_SetHZ24(hItem);
        Login_Show(hItem, LOGIN_ADMIN);
    }

    for (MDev_ID = GUI_ID_TEXT0; MDev_ID <= GUI_ID_TEXT9; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24(hItem);

        if (MDev_ID == GUI_ID_TEXT0) {
            TEXT_SetTextAlign(hItem,  GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetTextColor(hItem, GUI_WHITE);
        }
    }

    TEXT_SetTextAlign(WM_GetDialogItem(hWin, GUI_ID_TEXT1),  GUI_TA_LEFT | GUI_TA_VCENTER);
    TEXT_SetTextAlign(WM_GetDialogItem(hWin, GUI_ID_TEXT2),  GUI_TA_LEFT | GUI_TA_VCENTER);

    sprintf(show_buf, "%s-%s", GUI_LANG_GetTextEx(Lang_Device, Language_Bit), GUI_LANG_GetTextEx(Lang_Timer, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT0), show_buf); // 设备管理-定时器

    sprintf(show_buf, "%s:", GUI_LANG_GetTextEx(Lang_Type, Language_Bit)); // 类型:
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT1), show_buf);

    hItem = WM_GetDialogItem(hWin, GUI_ID_DROPDOWN0);
    Dropdown_SetHZ24(hItem);

    DROPDOWN_AddString(hItem, GUI_LANG_GetTextEx(Lang_Cycle_Switch, Language_Bit));
    DROPDOWN_AddString(hItem, GUI_LANG_GetTextEx(Lang_On_Time_Switch, Language_Bit));
    DROPDOWN_AddString(hItem, GUI_LANG_GetTextEx(Lang_On_Time_Delay, Language_Bit));

    Login_Show(hItem, LOGIN_ADMIN);

    hItem = WM_GetDialogItem(hWin, GUI_ID_DROPDOWN1);
    Dropdown_SetHZ24(hItem);

    DROPDOWN_AddString(hItem, GUI_LANG_GetTextEx(Lang_None, Language_Bit));
    DROPDOWN_AddString(hItem, GUI_LANG_GetTextEx(Lang_Light, Language_Bit));
    DROPDOWN_AddString(hItem, GUI_LANG_GetTextEx(Lang_Water, Language_Bit));
    DROPDOWN_AddString(hItem, GUI_LANG_GetTextEx(Lang_Feed, Language_Bit));

    Login_Show(hItem, LOGIN_ADMIN);


    hItem = WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2);
    Dropdown_SetHZ24(hItem);

    for (i = 0; i < 4; i++) {
        sprintf(show_buf, "%s%d", GUI_LANG_GetTextEx(Lang_Timer, Language_Bit), i + 1);
        DROPDOWN_AddString(hItem, show_buf);
    }

    if (select_chg_flag == STA_OFF) {
        WM_DisableWindow(hItem);
    }

    DROPDOWN_SetSel(hItem, select_num);


    Set_OptValue(hWin, select_id);

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

    ext_inf.timer[select_num].start_time[5 * select_page + temp_id] = stemp;
    sprintf(show_buf, "%02d:%02d",  ext_inf.timer[select_num].start_time[5 * select_page + temp_id] / 60, ext_inf.timer[select_num].start_time[5 * select_page + temp_id] % 60);
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, (GUI_ID_EDIT0 + temp_id) ), show_buf);
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

    ext_inf.timer[select_num].stop_time[5 * select_page + temp_id] = stemp;
    sprintf(show_buf, "%02d:%02d",  ext_inf.timer[select_num].stop_time[5 * select_page + temp_id] / 60, ext_inf.timer[select_num].stop_time[5 * select_page + temp_id] % 60);
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, (GUI_ID_EDIT5 + temp_id) ), show_buf);
}

static void OnTime_Get(void)
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

    ext_inf.timer[select_num].start_min = stemp;
    sprintf(show_buf, "%d Min", ext_inf.timer[select_num].start_min);
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, (GUI_ID_EDIT0) ), show_buf);

}
static void OffTime_Get(void)
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

    ext_inf.timer[select_num].stop_min = stemp;
    sprintf(show_buf, "%d Min", ext_inf.timer[select_num].stop_min);
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, (GUI_ID_EDIT5) ), show_buf);
}

static void DelayTime_Get(void)
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

    ext_inf.timer[select_num].delay_min[5 * select_page + temp_id] = stemp;
    ext_inf.timer[select_num].stop_time[5 * select_page + temp_id] = ext_inf.timer[select_num].start_time[5 * select_page + temp_id] + ext_inf.timer[select_num].delay_min[5 * select_page + temp_id];
    sprintf(show_buf, "%d Min", ext_inf.timer[select_num].delay_min[5 * select_page + temp_id]);
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, (GUI_ID_EDIT5 + temp_id) ), show_buf);
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

        if (old_Id != Id) {
            old_Id = Id;
            break;
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

        case GUI_ID_BUTTON0:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                if (select_page == 1) {
                    Get_OptValue(hWin, select_id);
                    select_page = 0;
                    Set_OptValue(hWin, select_id);
                }
            }

            break;

        case GUI_ID_BUTTON1:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                if (select_page == 0) {
                    Get_OptValue(hWin, select_id);
                    select_page = 1;
                    Set_OptValue(hWin, select_id);
                }
            }

            break;

        case GUI_ID_BUTTON2:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Get_OptValue(hWin, select_id);

                if (Config_Save(STIMER_ID) < 0) {
                    MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_SAVE_ER));
                }

                Current_HWIN = WM_GetParent(hWin);
                GUI_EndDialog(hWin, 0);
            }

        case GUI_ID_DROPDOWN0:
            if (NCode == WM_NOTIFICATION_SEL_CHANGED) {
                Get_OptValue(hWin, select_id);
                Set_OptValue(hWin, select_id);
            }

            break;

        case GUI_ID_DROPDOWN2:
            if (NCode == WM_NOTIFICATION_SEL_CHANGED) {
                Get_OptValue(hWin, select_id);
                select_num = DROPDOWN_GetSel(WM_GetDialogItem(hWin, (GUI_ID_DROPDOWN2)));
                select_id = ext_inf.timer[select_num].mode;
                Set_OptValue(hWin, select_id);
            }

            break;

        case GUI_ID_EDIT0:
        case GUI_ID_EDIT1:
        case GUI_ID_EDIT2:
        case GUI_ID_EDIT3:
        case GUI_ID_EDIT4:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                temp_id = (Id - GUI_ID_EDIT0);

                switch (select_id) {
                case TIMER_MODE_CYCLE:
                    Set_Output_Callback(OnTime_Get);
                    Set_Input_Len(3);
                    Input_Task(hWin);
                    break;

                case TIMER_MODE_ON_OFF:
                case TIMER_MODE_ON_DELAY:
                    Set_Output_Callback(Start_Time_Get);
                    Set_Input_Len(4);
                    Input_Task(hWin);
                    break;

                default:
                    break;
                }

            }

            break;

        case GUI_ID_EDIT5:
        case GUI_ID_EDIT6:
        case GUI_ID_EDIT7:
        case GUI_ID_EDIT8:
        case GUI_ID_EDIT9:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                temp_id = Id - GUI_ID_EDIT5;

                switch (select_id) {
                case TIMER_MODE_CYCLE:
                    Set_Output_Callback(OffTime_Get);
                    Set_Input_Len(3);
                    Input_Task(hWin);
                    break;

                case TIMER_MODE_ON_OFF:
                    Set_Output_Callback(Stop_Time_Get);
                    Set_Input_Len(4);
                    Input_Task(hWin);
                    break;

                case TIMER_MODE_ON_DELAY:
                    Set_Output_Callback(DelayTime_Get);
                    Set_Input_Len(3);
                    Input_Task(hWin);
                    break;

                default:
                    break;
                }

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
WM_HWIN MDev_Timer;
void MDev_Timer_Task(WM_HWIN hParent, U8 sid, U8 chg_flag)
{
    select_num = sid;
    select_chg_flag = chg_flag;
    select_id = ext_inf.timer[select_num].mode;
    select_page = 0;
    MDev_Timer = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    Current_HWIN = MDev_Timer;
}

