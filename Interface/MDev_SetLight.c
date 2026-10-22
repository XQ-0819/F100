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

#define GUI_ID_TEXT10  (GUI_ID_USER+21)
#define GUI_ID_TEXT11  (GUI_ID_USER+22)
#define GUI_ID_TEXT12  (GUI_ID_USER+23)
#define GUI_ID_TEXT13  (GUI_ID_USER+24)

#define GUI_ID_EDIT10  (GUI_ID_USER+1)
#define GUI_ID_EDIT11  (GUI_ID_USER+2)
#define GUI_ID_EDIT12  (GUI_ID_USER+3)
#define GUI_ID_EDIT13  (GUI_ID_USER+4)
#define GUI_ID_EDIT14  (GUI_ID_USER+5)
#define GUI_ID_EDIT15  (GUI_ID_USER+6)
#define GUI_ID_EDIT16  (GUI_ID_USER+7)
#define GUI_ID_EDIT17  (GUI_ID_USER+8)
#define GUI_ID_EDIT18  (GUI_ID_USER+9)
#define GUI_ID_EDIT19  (GUI_ID_USER+10)
#define GUI_ID_EDIT20  (GUI_ID_USER+11)
#define GUI_ID_EDIT21  (GUI_ID_USER+12)

static U8 select_id = 0;
static TCHG_MENU Mchg_Menu[] = {

    {MSHOW, TYPE_TEXT, GUI_ID_TEXT5,  "%s", 1, SNUM_FLAG},
    {MSHOW, TYPE_TEXT, GUI_ID_TEXT6,  "%s", 1, SNUM_FLAG},
    {MSHOW, TYPE_TEXT, GUI_ID_TEXT7,  "%s", 1, SNUM_FLAG},
    {MSHOW, TYPE_TEXT, GUI_ID_TEXT8,  "%s", 1, SNUM_FLAG},
    {MSHOW, TYPE_TEXT, GUI_ID_TEXT9,  "%s", 1, SNUM_FLAG},


    {MSHOW, TYPE_EDIT, GUI_ID_EDIT0, "%d", 0, 1},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT1, "%d", 0, 1},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT2, "%d", 0, 1},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT3, "%d", 0, 1},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT4, "%d", 0, 1},

    {MSHOW, TYPE_EDIT, GUI_ID_EDIT15, "%d", 0, 1},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT16, "%d", 0, 1},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT17, "%d", 0, 1},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT18, "%d", 0, 1},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT19, "%d", 0, 1},

    {MSHOW, TYPE_EDIT, GUI_ID_EDIT20, "%dS", 0, 1},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT21, "%dS", 0, 1},

    {MEXIT, 0, 0, "", 0, 0},
};



static void Set_OptValue(WM_HWIN hWin, U8 sid)
{
    U8 i = 0;
    //增加是否启用

    for (i = 0; i < 5; i++) {
        CHECKBOX_SetState(WM_GetDialogItem(hWin, GUI_ID_CHECK1 + i ), dev_inf.light.curve[5 * sid + i].enable);
    }

    //增加时间
    for (i = 0; i < 5; i++) {
        sprintf(show_buf, "%02d:%02d", dev_inf.light.curve[5 * sid + i].start_time / 60, dev_inf.light.curve[5 * sid + i].start_time % 60);
        EDIT_SetText(WM_GetDialogItem(hWin, (GUI_ID_EDIT5 + i) ), show_buf);
    }

    for (i = 0; i < 5; i++) {
        sprintf(show_buf, "%02d:%02d",  dev_inf.light.curve[5 * sid + i].stop_time / 60, dev_inf.light.curve[5 * sid + i].stop_time % 60);
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
        Mchg_Menu[id++].value = dev_inf.light.curve[5 * sid + i].grow_day;
    }

    for (i = 0; i < 5; i++) {
        Mchg_Menu[id++].value = dev_inf.light.curve[5 * sid + i].value;
    }

    Mchg_Menu[id++].value = dev_inf.light.delay_ontime;
    Mchg_Menu[id++].value = dev_inf.light.delay_offtime;
}

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

#define XPos_Inital    	20
#define XPos_Collec  	10
#define YPos_Collec  	0
#define Y_Space     	40
#define X_Space   		120
#define Text_Width   	36
#define Category_Space  10


static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect,    "",               0,                       0,  0,  800, 480, 0, 0},
    { TEXT_CreateIndirect,      "设备管理-光照",          GUI_ID_TEXT0,            250, 4, 300, 42, 0, 0},

    { TEXT_CreateIndirect,      "光照",        GUI_ID_TEXT1,              100 + XPos_Collec, 60, 150, 36, 0, 0},
    { TEXT_CreateIndirect,     "开启延时:",    GUI_ID_TEXT2,            250 + XPos_Collec, 60, 150, 36, 0, 0},
    { EDIT_CreateIndirect,       "300s",         GUI_ID_EDIT20,         385 + XPos_Collec, 60, 80, 36, 0, 0},
    { TEXT_CreateIndirect,       "关闭延时:",    GUI_ID_TEXT3,           480 + XPos_Collec, 60, 110, 36, 0, 0},
    { EDIT_CreateIndirect,       "300s",        GUI_ID_EDIT21,         600 + XPos_Collec, 60, 80, 36, 0, 0},


    { TEXT_CreateIndirect,       "序号",    GUI_ID_TEXT4,          40 + XPos_Inital + X_Space * 0 + XPos_Collec, 100 + YPos_Collec + Y_Space * 0, 60, Text_Width, 0, 0},
    { TEXT_CreateIndirect,       "1",    GUI_ID_TEXT5,               40 + XPos_Inital + X_Space * 0 + XPos_Collec, 100 + YPos_Collec + Y_Space * 1 + Category_Space * 0, 60, Text_Width, 0, 0},
    { TEXT_CreateIndirect,      "2",      GUI_ID_TEXT6,             40 + XPos_Inital + X_Space * 0 + XPos_Collec, 100 + YPos_Collec + Y_Space * 2 + Category_Space * 1, 60, Text_Width, 0, 0},
    { TEXT_CreateIndirect,     "3",       GUI_ID_TEXT7,              40 + XPos_Inital + X_Space * 0 + XPos_Collec, 100 + YPos_Collec + Y_Space * 3 + Category_Space * 2, 60, Text_Width, 0, 0},
    { TEXT_CreateIndirect,     "4",     GUI_ID_TEXT8,                40 + XPos_Inital + X_Space * 0 + XPos_Collec, 100 + YPos_Collec + Y_Space * 4 + Category_Space * 3, 60, Text_Width, 0, 0},
    { TEXT_CreateIndirect,     "5",      GUI_ID_TEXT9,               40 + XPos_Inital + X_Space * 0 + XPos_Collec, 100 + YPos_Collec + Y_Space * 5 + Category_Space * 4, 60, Text_Width, 0, 0},

    { TEXT_CreateIndirect,       "日龄",    GUI_ID_TEXT10,               XPos_Inital + X_Space * 1 + XPos_Collec, 100 + YPos_Collec + Y_Space * 0, 100, Text_Width, 0, 0},
    { EDIT_CreateIndirect,     "8:00",      GUI_ID_EDIT0,                10 + XPos_Inital + X_Space * 1 + XPos_Collec, 100 + YPos_Collec + Y_Space * 1, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,    "10:00",     GUI_ID_EDIT1,                 10 + XPos_Inital + X_Space * 1 + XPos_Collec, 100 + YPos_Collec + Y_Space * 2 + Category_Space * 1, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,    "14:00",        GUI_ID_EDIT2,              10 + XPos_Inital + X_Space * 1 + XPos_Collec, 100 + YPos_Collec + Y_Space * 3 + Category_Space * 2, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,    "17:00",         GUI_ID_EDIT3,             10 + XPos_Inital + X_Space * 1 + XPos_Collec, 100 + YPos_Collec + Y_Space * 4 + Category_Space * 3, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,    "00:00",        GUI_ID_EDIT4,              10 + XPos_Inital + X_Space * 1 + XPos_Collec, 100 + YPos_Collec + Y_Space * 5 + Category_Space * 4, 72, Text_Width, 0, 0},


    { TEXT_CreateIndirect,        "开启时间",    GUI_ID_TEXT11,        XPos_Inital + X_Space * 2 + XPos_Collec, 100 + YPos_Collec + Y_Space * 0, 100, Text_Width, 0, 0},
    { EDIT_CreateIndirect,      "10:00",   GUI_ID_EDIT5,              10 + XPos_Inital + X_Space * 2 + XPos_Collec, 100 + YPos_Collec + Y_Space * 1, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,       "14:00",  GUI_ID_EDIT6,              10 + XPos_Inital + X_Space * 2 + XPos_Collec, 100 + YPos_Collec + Y_Space * 2 + Category_Space * 1, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,     "17:00",    GUI_ID_EDIT7,              10 + XPos_Inital + X_Space * 2 + XPos_Collec, 100 + YPos_Collec + Y_Space * 3 + Category_Space * 2, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,    "24:00",      GUI_ID_EDIT8,            10 + XPos_Inital + X_Space * 2 + XPos_Collec, 100 + YPos_Collec + Y_Space * 4 + Category_Space * 3, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,     "8:00",       GUI_ID_EDIT9,           10 + XPos_Inital + X_Space * 2 + XPos_Collec, 100 + YPos_Collec + Y_Space * 5 + Category_Space * 4, 72, Text_Width, 0, 0},


    { TEXT_CreateIndirect,  "关闭时间", GUI_ID_TEXT12,                     XPos_Inital + X_Space * 3 + XPos_Collec, 100 + YPos_Collec + Y_Space * 0, 100, Text_Width, 0, 0},
    { EDIT_CreateIndirect,      "3%",      GUI_ID_EDIT10,                 10 + XPos_Inital + X_Space * 3 + XPos_Collec, 100 + YPos_Collec + Y_Space * 1, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,      "33%",     GUI_ID_EDIT11,                  10 + XPos_Inital + X_Space * 3 + XPos_Collec, 100 + YPos_Collec + Y_Space * 2 + Category_Space * 1, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,      "33%",      GUI_ID_EDIT12,                 10 + XPos_Inital + X_Space * 3 + XPos_Collec, 100 + YPos_Collec + Y_Space * 3 + Category_Space * 2, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,      "53%",       GUI_ID_EDIT13,                10 + XPos_Inital + X_Space * 3 + XPos_Collec, 100 + YPos_Collec + Y_Space * 4 + Category_Space * 3, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,      "53%",      GUI_ID_EDIT14,                 10 + XPos_Inital + X_Space * 3 + XPos_Collec, 100 + YPos_Collec + Y_Space * 5 + Category_Space * 4, 72, Text_Width, 0, 0},


    { TEXT_CreateIndirect,      "光照强度",      GUI_ID_TEXT13,             XPos_Inital + X_Space * 4 + XPos_Collec, 100 + YPos_Collec + Y_Space * 0, 96, Text_Width, 0, 0},
    { EDIT_CreateIndirect,      "13%",    GUI_ID_EDIT15,                    10 + XPos_Inital + X_Space * 4 + XPos_Collec, 100 + YPos_Collec + Y_Space * 1, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,      "13%",    GUI_ID_EDIT16,                   10 + XPos_Inital + X_Space * 4 + XPos_Collec, 100 + YPos_Collec + Y_Space * 2 + Category_Space * 1, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,      "13%",    GUI_ID_EDIT17,                   10 + XPos_Inital + X_Space * 4 + XPos_Collec, 100 + YPos_Collec + Y_Space * 3 + Category_Space * 2, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,     "13%",      GUI_ID_EDIT18,                   10 + XPos_Inital + X_Space * 4 + XPos_Collec, 100 + YPos_Collec + Y_Space * 4 + Category_Space * 3, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,     "13%",       GUI_ID_EDIT19,                  10 + XPos_Inital + X_Space * 4 + XPos_Collec, 100 + YPos_Collec + Y_Space * 5 + Category_Space * 4, 72, Text_Width, 0, 0},

    { CHECKBOX_CreateIndirect,  "启用",        GUI_ID_CHECK1,        615 + XPos_Collec, 100 + YPos_Collec + Y_Space * 1 + Category_Space * 0.5, 90, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",        GUI_ID_CHECK2,        615 + XPos_Collec, 100 + YPos_Collec + Y_Space * 2 + Category_Space * 1.5, 90, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",        GUI_ID_CHECK3,        615 + XPos_Collec, 100 + YPos_Collec + Y_Space * 3 + Category_Space * 2.5, 90, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",        GUI_ID_CHECK4,        615 + XPos_Collec, 100 + YPos_Collec + Y_Space * 4 + Category_Space * 3.5, 90, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",        GUI_ID_CHECK5,        615 + XPos_Collec, 100 + YPos_Collec + Y_Space * 5 + Category_Space * 4.5, 90, CHECK_SIZE, 0, 0},

    { BUTTON_CreateIndirect,    "上一页",          GUI_ID_BUTTON0,          MENU_BACK_X1, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
    { BUTTON_CreateIndirect,    "下一页",          GUI_ID_BUTTON1,          MENU_BACK_X2, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
    { BUTTON_CreateIndirect,    "返回",             GUI_ID_BUTTON2,         MENU_BACK_X3, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},


};

static int temp_id = 0;

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

    dev_inf.light.curve[5 * select_id + temp_id].grow_day = stemp;
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

    dev_inf.light.curve[5 * select_id + temp_id].start_time = stemp;
    sprintf(show_buf, "%02d:%02d",  dev_inf.light.curve[5 * select_id + temp_id].start_time / 60, dev_inf.light.curve[5 * select_id + temp_id].start_time % 60);
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

    dev_inf.light.curve[5 * select_id + temp_id].stop_time = stemp;
    sprintf(show_buf, "%02d:%02d",  dev_inf.light.curve[5 * select_id + temp_id].stop_time / 60, dev_inf.light.curve[5 * select_id + temp_id].stop_time % 60);
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, (GUI_ID_EDIT10 + temp_id) ), show_buf);
}

static void Light_Get(void)
{
    float ftemp = Get_Input_Value();
    short stemp = (short)ftemp;

    if (stemp > MAX_PERCENT) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = MAX_PERCENT;
    }

    if (stemp < 0) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 0;
    }

    dev_inf.light.curve[5 * select_id + temp_id].value = stemp;
    MChange_Menu(Menu_Value_Init, Current_HWIN, (PCHG_MENU )Mchg_Menu, select_id);
}

static void Start_Delay_Get(void)
{
    float ftemp = Get_Input_Value();
    short stemp = (short)ftemp;

//    if (stemp > HUM_ADJMAX) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = HUM_ADJMAX;
//    }

    if (stemp <= 0) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 1;
    }

    dev_inf.light.delay_ontime = stemp;
    MChange_Menu(Menu_Value_Init, Current_HWIN, (PCHG_MENU )Mchg_Menu, select_id);
}
static void Stop_Delay_Get(void)
{
    float ftemp = Get_Input_Value();
    short stemp = (short)ftemp;

//    if (stemp > HUM_ADJMAX) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = HUM_ADJMAX;
//    }

    if (stemp <= 0) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 1;
    }

    dev_inf.light.delay_offtime = stemp;
    MChange_Menu(Menu_Value_Init, Current_HWIN, (PCHG_MENU )Mchg_Menu, select_id);
}

static void Get_OptValue(WM_HWIN hWin, U8 sid)
{
    U8 i = 0;

    for (i = 0; i < 5; i++) {
        dev_inf.light.curve[5 * sid + i].enable = CHECKBOX_GetState(WM_GetDialogItem(hWin, GUI_ID_CHECK1 + i ));
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


    for (MDev_ID = GUI_ID_BUTTON0; MDev_ID <= GUI_ID_BUTTON2; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, Cut_cbButton);
    }

    for (MDev_ID = GUI_ID_EDIT0; MDev_ID <= GUI_ID_EDIT9; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Edit_SetHZ24(hItem);
    }

    for (MDev_ID = GUI_ID_EDIT10; MDev_ID <= GUI_ID_EDIT21; MDev_ID++) {
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

    for (MDev_ID = GUI_ID_TEXT10; MDev_ID <= GUI_ID_TEXT13; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24(hItem);
    }

    sprintf(show_buf, "%s-%s", GUI_LANG_GetTextEx(Lang_Device, Language_Bit), GUI_LANG_GetTextEx(Lang_Light, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT0), show_buf);                                           // 设备管理-光照
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT2), GUI_LANG_GetTextEx(Lang_Open_Delay, Language_Bit));  // 开启延时
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT3), GUI_LANG_GetTextEx(Lang_Close_Delay, Language_Bit)); // 关闭延时

    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT4), GUI_LANG_GetTextEx(Lang_Number_No, Language_Bit));   // 序号
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT10), GUI_LANG_GetTextEx(Lang_Day, Language_Bit));        // 日龄
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT11), GUI_LANG_GetTextEx(Lang_Start_time, Language_Bit)); // 开启时间
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT12), GUI_LANG_GetTextEx(Lang_End_time, Language_Bit));   // 关闭时间
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT13), GUI_LANG_GetTextEx(Lang_Light_PCT, Language_Bit));  // 光照强度

    if (dev_inf.light.enable == STA_ON) {
        sprintf(show_buf, "%s:%s", GUI_LANG_GetTextEx(Lang_Light, Language_Bit), GUI_LANG_GetTextEx(Lang_Enable, Language_Bit));
        TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT1), show_buf); // 光照:开启
    } else {
        sprintf(show_buf, "%s:%s", GUI_LANG_GetTextEx(Lang_Light, Language_Bit), GUI_LANG_GetTextEx(Lang_Disable, Language_Bit));
        TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT1), show_buf); // 光照:关闭
    }

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

        if (Id >= GUI_ID_EDIT10 && Id <= GUI_ID_EDIT21 && NCode == WM_NOTIFICATION_RELEASED
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

                if (Config_Save(SLIGHT_ID) < 0) {
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
                Set_Output_Callback(Light_Get);
                Set_Input_Len(3);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT20:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                temp_id = Id - GUI_ID_EDIT15;
                Set_Output_Callback(Start_Delay_Get);
                Set_Input_Len(4);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT21:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                temp_id = Id - GUI_ID_EDIT15;
                Set_Output_Callback(Stop_Delay_Get);
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
WM_HWIN MDev_SetLight;
void MDev_SetLight_Task(WM_HWIN hParent)
{
    select_id = 0;
    MDev_SetLight = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    Current_HWIN = MDev_SetLight;
}

