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
#include "datasave.h"

/*********************************************************************
*
*       static data
*
**********************************************************************
*/
#define GUI_ID_TEXT10   (GUI_ID_USER+1)
#define GUI_ID_TEXT11   (GUI_ID_USER+2)
#define GUI_ID_TEXT12   (GUI_ID_USER+3)


#define GUI_ID_EDIT10  (GUI_ID_USER+201)
#define GUI_ID_EDIT11  (GUI_ID_USER+202)
#define GUI_ID_EDIT12  (GUI_ID_USER+203)
#define GUI_ID_EDIT13  (GUI_ID_USER+204)
#define GUI_ID_EDIT14  (GUI_ID_USER+205)
#define GUI_ID_EDIT15  (GUI_ID_USER+206)
#define GUI_ID_EDIT16  (GUI_ID_USER+207)
#define GUI_ID_EDIT17  (GUI_ID_USER+208)
#define GUI_ID_EDIT18  (GUI_ID_USER+209)
#define GUI_ID_EDIT19  (GUI_ID_USER+210)
#define GUI_ID_EDIT20  (GUI_ID_USER+211)
#define GUI_ID_EDIT21  (GUI_ID_USER+212)
#define GUI_ID_EDIT22  (GUI_ID_USER+213)
#define GUI_ID_EDIT23  (GUI_ID_USER+214)
#define GUI_ID_EDIT24  (GUI_ID_USER+215)

static U8 select_id = 0;
static TCHG_MENU Mchg_Menu[] = {
    {MSHOW, TYPE_TEXT, GUI_ID_TEXT2,  "%s", 1, SNUM_FLAG},
    {MSHOW, TYPE_TEXT, GUI_ID_TEXT3,  "%s", 1, SNUM_FLAG},
    {MSHOW, TYPE_TEXT, GUI_ID_TEXT4,  "%s", 1, SNUM_FLAG},
    {MSHOW, TYPE_TEXT, GUI_ID_TEXT5,  "%s", 1, SNUM_FLAG},
    {MSHOW, TYPE_TEXT, GUI_ID_TEXT6,  "%s", 1, SNUM_FLAG},

    {MSHOW, TYPE_EDIT, GUI_ID_EDIT0, "%d", 0, 1},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT1, "%d", 0, 1},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT2, "%d", 0, 1},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT3, "%d", 0, 1},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT4, "%d", 0, 1},

    {MSHOW, TYPE_EDIT, GUI_ID_EDIT15, "%.1f", 0, TEMP_RATE},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT16, "%.1f", 0, TEMP_RATE},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT17, "%.1f", 0, TEMP_RATE},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT18, "%.1f", 0, TEMP_RATE},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT19, "%.1f", 0, TEMP_RATE},

    {MSHOW, TYPE_EDIT, GUI_ID_EDIT20, "%d", 0, 1},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT21, "%d", 0, 1},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT22, "%d", 0, 1},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT23, "%d", 0, 1},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT24, "%d", 0, 1},

    {MEXIT, 0, 0, "", 0, 0},
};
static void Set_OptValue(WM_HWIN hWin, U8 sid)
{
    U8 i = 0;

    for (i = 0; i < 5; i++) {
        CHECKBOX_SetState(WM_GetDialogItem(hWin, (GUI_ID_CHECK1 + i) ), ext_inf.day_temp[5 * sid + i].enable);
    }

    //增加时间
    for (i = 0; i < 5; i++) {
        sprintf(show_buf, "%02d:%02d",  ext_inf.day_temp[5 * sid + i].start_time / 60, ext_inf.day_temp[5 * sid + i].start_time % 60);
        EDIT_SetText(WM_GetDialogItem(hWin, (GUI_ID_EDIT5 + i) ), show_buf);
    }

    for (i = 0; i < 5; i++) {
        sprintf(show_buf, "%02d:%02d",  ext_inf.day_temp[5 * sid + i].end_time / 60, ext_inf.day_temp[5 * sid + i].end_time % 60);
        EDIT_SetText(WM_GetDialogItem(hWin, (GUI_ID_EDIT10 + i) ), show_buf);
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
        Mchg_Menu[id++].value = ext_inf.day_temp[5 * sid + i].grow_day;
    }

    for (i = 0; i < 5; i++) {
        Mchg_Menu[id++].value = ext_inf.day_temp[5 * sid + i].fix_temp;
    }

    for (i = 0; i < 5; i++) {
        Mchg_Menu[id++].value = ext_inf.day_temp[5 * sid + i].fix_times;
    }
}




/*********************************************************************
*
*       Dialog resource
*
* This table conatins the info required to create the dialog.
* It has been created by ucGUIbuilder.
*/
#define XPos_Inital    	-20
#define XPos_Collec  	30
#define YPos_Collec  	-30
#define Y_Space     	45
#define X_Space   		140
#define Text_Width   	36
#define Category_Space  10

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect,    "",               0,                       0,  0,  800, 480, 0, 0},
    { TEXT_CreateIndirect,      "控制管理-昼夜调整",          GUI_ID_TEXT0,            250, 4, 300, 42, 0, 0},

    { TEXT_CreateIndirect,      "",        GUI_ID_TEXT1,              30 + XPos_Collec, 80, 100, Text_Width, 0, 0},

    { TEXT_CreateIndirect,       "1",    GUI_ID_TEXT2,               -10 + XPos_Collec , 100 + YPos_Collec + Y_Space * 1 + Category_Space * 0, 24, Text_Width, 0, 0},
    { TEXT_CreateIndirect,      "2",      GUI_ID_TEXT3,               -10 + XPos_Collec, 100 + YPos_Collec + Y_Space * 2 + Category_Space * 1, 24, Text_Width, 0, 0},
    { TEXT_CreateIndirect,     "3",       GUI_ID_TEXT4,               -10 + XPos_Collec, 100 + YPos_Collec + Y_Space * 3 + Category_Space * 2, 24, Text_Width, 0, 0},
    { TEXT_CreateIndirect,     "4",     GUI_ID_TEXT5,                -10 + XPos_Collec, 100 + YPos_Collec + Y_Space * 4 + Category_Space * 3, 24, Text_Width, 0, 0},
    { TEXT_CreateIndirect,     "5",      GUI_ID_TEXT6,               -10 + XPos_Collec, 100 + YPos_Collec + Y_Space * 5 + Category_Space * 4, 24, Text_Width, 0, 0},

    { TEXT_CreateIndirect,    "日龄",    GUI_ID_TEXT7,               30 + XPos_Collec, 100 + YPos_Collec + Y_Space * 0, 100, Text_Width, 0, 0},
    { EDIT_CreateIndirect,     "",      GUI_ID_EDIT0,                 44 + XPos_Collec, 100 + YPos_Collec + Y_Space * 1, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,    "",     GUI_ID_EDIT1,                  44 + XPos_Collec, 100 + YPos_Collec + Y_Space * 2 + Category_Space * 1, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,    "",        GUI_ID_EDIT2,               44 + XPos_Collec, 100 + YPos_Collec + Y_Space * 3 + Category_Space * 2, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,    "",         GUI_ID_EDIT3,              44 + XPos_Collec, 100 + YPos_Collec + Y_Space * 4 + Category_Space * 3, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,    "",        GUI_ID_EDIT4,               44 + XPos_Collec, 100 + YPos_Collec + Y_Space * 5 + Category_Space * 4, 72, Text_Width, 0, 0},

    { TEXT_CreateIndirect,    "开始时间",    GUI_ID_TEXT8,            150 + XPos_Collec, 100 + YPos_Collec + Y_Space * 0, 100, Text_Width, 0, 0},
    { EDIT_CreateIndirect,      "",   GUI_ID_EDIT5,               164 + XPos_Collec, 100 + YPos_Collec + Y_Space * 1, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,       "",  GUI_ID_EDIT6,              164 + XPos_Collec, 100 + YPos_Collec + Y_Space * 2 + Category_Space * 1, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,     "",    GUI_ID_EDIT7,                164 + XPos_Collec, 100 + YPos_Collec + Y_Space * 3 + Category_Space * 2, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,    "",      GUI_ID_EDIT8,             164 + XPos_Collec, 100 + YPos_Collec + Y_Space * 4 + Category_Space * 3, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,     "",       GUI_ID_EDIT9,              164 + XPos_Collec, 100 + YPos_Collec + Y_Space * 5 + Category_Space * 4, 72, Text_Width, 0, 0},

    { TEXT_CreateIndirect,  "结束时间",   GUI_ID_TEXT9,                  265 + XPos_Collec, 100 + YPos_Collec + Y_Space * 0, 110, Text_Width, 0, 0},
    { EDIT_CreateIndirect,      "",      GUI_ID_EDIT10,             284 + XPos_Collec, 100 + YPos_Collec + Y_Space * 1, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,      "",     GUI_ID_EDIT11,                 284 + XPos_Collec, 100 + YPos_Collec + Y_Space * 2 + Category_Space * 1, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,      "",      GUI_ID_EDIT12,                284 + XPos_Collec, 100 + YPos_Collec + Y_Space * 3 + Category_Space * 2, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,      "",       GUI_ID_EDIT13,               284 + XPos_Collec, 100 + YPos_Collec + Y_Space * 4 + Category_Space * 3, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,      "",      GUI_ID_EDIT14,                284 + XPos_Collec, 100 + YPos_Collec + Y_Space * 5 + Category_Space * 4, 72, Text_Width, 0, 0},

    { TEXT_CreateIndirect,      "调整温度",      GUI_ID_TEXT10,           385 + XPos_Collec, 100 + YPos_Collec + Y_Space * 0, 110, Text_Width, 0, 0},
    { EDIT_CreateIndirect,      "",    GUI_ID_EDIT15,                   404 + XPos_Collec, 100 + YPos_Collec + Y_Space * 1, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,      "",    GUI_ID_EDIT16,                   404 + XPos_Collec, 100 + YPos_Collec + Y_Space * 2 + Category_Space * 1, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,      "",    GUI_ID_EDIT17,                   404 + XPos_Collec, 100 + YPos_Collec + Y_Space * 3 + Category_Space * 2, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,     "",      GUI_ID_EDIT18,                  404 + XPos_Collec, 100 + YPos_Collec + Y_Space * 4 + Category_Space * 3, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,     "",       GUI_ID_EDIT19,               404 + XPos_Collec, 100 + YPos_Collec + Y_Space * 5 + Category_Space * 4, 72, Text_Width, 0, 0},

    { TEXT_CreateIndirect,      "调整次数",         GUI_ID_TEXT11,   505 + XPos_Collec, 100 + YPos_Collec + Y_Space * 0, 110, Text_Width, 0, 0},

    { EDIT_CreateIndirect,      "",    GUI_ID_EDIT20,             524 + XPos_Collec, 100 + YPos_Collec + Y_Space * 1, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,      "",   GUI_ID_EDIT21,              524 + XPos_Collec, 100 + YPos_Collec + Y_Space * 2 + Category_Space * 1, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,      "",    GUI_ID_EDIT22,             524 + XPos_Collec, 100 + YPos_Collec + Y_Space * 3 + Category_Space * 2, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,     "",      GUI_ID_EDIT23,            524 + XPos_Collec, 100 + YPos_Collec + Y_Space * 4 + Category_Space * 3, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,     "",     GUI_ID_EDIT24,             524 + XPos_Collec, 100 + YPos_Collec + Y_Space * 5 + Category_Space * 4, 72, Text_Width, 0, 0},

    { CHECKBOX_CreateIndirect,  "启用",        GUI_ID_CHECK1,        620 + XPos_Collec, 100 + YPos_Collec + Y_Space * 1 + Category_Space * 0.5, 120, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",        GUI_ID_CHECK2,        620 + XPos_Collec, 100 + YPos_Collec + Y_Space * 2 + Category_Space * 1.5, 120, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",        GUI_ID_CHECK3,        620 + XPos_Collec, 100 + YPos_Collec + Y_Space * 3 + Category_Space * 2.5, 120, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",        GUI_ID_CHECK4,        620 + XPos_Collec, 100 + YPos_Collec + Y_Space * 4 + Category_Space * 3.5, 120, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",        GUI_ID_CHECK5,        620 + XPos_Collec, 100 + YPos_Collec + Y_Space * 5 + Category_Space * 4.5, 120, CHECK_SIZE, 0, 0},

    { BUTTON_CreateIndirect,    "上一页",          GUI_ID_BUTTON0,          MENU_BACK_X1, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
    { BUTTON_CreateIndirect,    "下一页",          GUI_ID_BUTTON1,          MENU_BACK_X2, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
    { BUTTON_CreateIndirect,    "返回",             GUI_ID_BUTTON2,         MENU_BACK_X3, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
};






static U8 temp_id = 0;
static void Grow_Day_Get(void)
{
    float ftemp = Get_Input_Value();
    short stemp = (short)ftemp;

    if (stemp > DAY_MAX) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = DAY_MAX;
    }

    if (stemp < DAY_MIN) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 0;
    }

    ext_inf.day_temp[5 * select_id + temp_id].grow_day = stemp;
    MChange_Menu(Menu_Value_Init, Current_HWIN, (PCHG_MENU )Mchg_Menu, select_id);
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

    ext_inf.day_temp[5 * select_id + temp_id].start_time = stemp;
    sprintf(show_buf, "%02d:%02d",  ext_inf.day_temp[5 * select_id + temp_id].start_time / 60, ext_inf.day_temp[5 * select_id + temp_id].start_time % 60);
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, (GUI_ID_EDIT5 + temp_id) ), show_buf);
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

    ext_inf.day_temp[5 * select_id + temp_id].end_time = stemp;
    sprintf(show_buf, "%02d:%02d",  ext_inf.day_temp[5 * select_id + temp_id].end_time / 60, ext_inf.day_temp[5 * select_id + temp_id].end_time % 60);
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, (GUI_ID_EDIT10 + temp_id) ), show_buf);
}

static void Adjust_Temp_Get(void)
{
    float ftemp = Get_Input_Value() * TEMP_RATE;
    short stemp = (short)ftemp;

    if (stemp > DAYTEMP_FIX_TEMP_MAX) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = DAYTEMP_FIX_TEMP_MAX;
    }

    if (stemp < DAYTEMP_FIX_TEMP_MIN) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = DAYTEMP_FIX_TEMP_MIN;
    }

    ext_inf.day_temp[5 * select_id + temp_id].fix_temp = stemp;
    MChange_Menu(Menu_Value_Init, Current_HWIN, (PCHG_MENU )Mchg_Menu, select_id);
}

static void Adjust_Time_Get(void)
{
    float ftemp = Get_Input_Value();
    short stemp = (short)ftemp;

    if (stemp > DAYTEMP_FIX_TIMES_MAX) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = DAYTEMP_FIX_TIMES_MAX;
    }

    if (stemp < DAYTEMP_FIX_TIMES_MIN) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = DAYTEMP_FIX_TIMES_MIN;
    }

    ext_inf.day_temp[5 * select_id + temp_id].fix_times = stemp;
    MChange_Menu(Menu_Value_Init, Current_HWIN, (PCHG_MENU )Mchg_Menu, select_id);
}

static void Get_OptValue(WM_HWIN hWin, U8 sid)
{
    U8 i = 0;

    for (i = 0; i < 5; i++) {
        ext_inf.day_temp[5 * sid + i].enable = CHECKBOX_GetState(WM_GetDialogItem(hWin, (GUI_ID_CHECK1 + i) ));
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
    GUI_DrawRoundedRect(6, 55, 794, 396, 10);
    GUI_DrawRoundedRect(2, 52, 798, 399, 10);
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

    //
    //GUI_ID_BUTTON0
    //
    for (MDev_ID = GUI_ID_BUTTON0; MDev_ID <= GUI_ID_BUTTON2; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, Cut_cbButton);
    }

    for (MDev_ID = GUI_ID_EDIT0; MDev_ID <= GUI_ID_EDIT9; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Edit_SetHZ24(hItem);
    }

    for (MDev_ID = GUI_ID_EDIT10; MDev_ID <= GUI_ID_EDIT24; MDev_ID++) {
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
        }
    }

    for (MDev_ID = GUI_ID_TEXT10; MDev_ID <= GUI_ID_TEXT12; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24(hItem);
    }


    sprintf(show_buf, "%s-%s", GUI_LANG_GetTextEx(Lang_Control, Language_Bit), GUI_LANG_GetTextEx(Lang_Diurnal_Adj, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT0), show_buf);                                           // 控制管理-昼夜调整
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT7), GUI_LANG_GetTextEx(Lang_Day, Language_Bit));         // 日龄
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT8), GUI_LANG_GetTextEx(Lang_Start_time, Language_Bit));       // 开始时间
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT9), GUI_LANG_GetTextEx(Lang_End_time, Language_Bit));       // 结束时间
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT10), GUI_LANG_GetTextEx(Lang_Fix_Temp, Language_Bit));       // 补偿温度
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT11), GUI_LANG_GetTextEx(Lang_Number_of_Adj, Language_Bit));       // 调节次数

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

        if (Id >= GUI_ID_EDIT10 && Id <= GUI_ID_EDIT24 && NCode == WM_NOTIFICATION_RELEASED
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

                if (Config_Save(SDAYTEMP_ID) < 0) {
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

        case GUI_ID_EDIT0:
        case GUI_ID_EDIT1:
        case GUI_ID_EDIT2:
        case GUI_ID_EDIT3:
        case GUI_ID_EDIT4:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                temp_id = Id - GUI_ID_EDIT0;
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
                temp_id = Id - GUI_ID_EDIT5;
                Set_Output_Callback(Start_Time_Get);
                Set_Input_Len(4);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT10:
        case GUI_ID_EDIT11:
        case GUI_ID_EDIT12:
        case GUI_ID_EDIT13:
        case GUI_ID_EDIT14:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                temp_id = Id - GUI_ID_EDIT10;
                Set_Output_Callback(Stop_Time_Get);
                Set_Input_Len(4);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT15:
        case GUI_ID_EDIT16:
        case GUI_ID_EDIT17:
        case GUI_ID_EDIT18:
        case GUI_ID_EDIT19:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                temp_id = Id - GUI_ID_EDIT15;
                Set_Output_Callback(Adjust_Temp_Get);
                Set_Input_Len(4);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT20:
        case GUI_ID_EDIT21:
        case GUI_ID_EDIT22:
        case GUI_ID_EDIT23:
        case GUI_ID_EDIT24:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                temp_id = Id - GUI_ID_EDIT20;
                Set_Output_Callback(Adjust_Time_Get);
                Set_Input_Len(1);
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
WM_HWIN MCtrl_DayTemp;
void MCtrl_DayTemp_Task(WM_HWIN hParent)
{
    select_id = 0;
    MCtrl_DayTemp = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    Current_HWIN = MCtrl_DayTemp;
}

