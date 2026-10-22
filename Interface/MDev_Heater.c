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
#define GUI_ID_EDIT15  (GUI_ID_USER+6)
#define GUI_ID_EDIT16  (GUI_ID_USER+7)
#define GUI_ID_EDIT17  (GUI_ID_USER+8)
#define GUI_ID_EDIT18  (GUI_ID_USER+9)
#define GUI_ID_EDIT19  (GUI_ID_USER+10)
#define GUI_ID_EDIT20  (GUI_ID_USER+11)
#define GUI_ID_EDIT21  (GUI_ID_USER+12)
#define GUI_ID_EDIT22  (GUI_ID_USER+13)
#define GUI_ID_EDIT23  (GUI_ID_USER+14)
#define GUI_ID_EDIT24  (GUI_ID_USER+15)
#define GUI_ID_EDIT25  (GUI_ID_USER+16)
#define GUI_ID_EDIT26  (GUI_ID_USER+17)
#define GUI_ID_EDIT27  (GUI_ID_USER+18)
#define GUI_ID_EDIT28  (GUI_ID_USER+19)
#define GUI_ID_EDIT29  (GUI_ID_USER+20)
#define GUI_ID_EDIT30  (GUI_ID_USER+21)
#define GUI_ID_EDIT31  (GUI_ID_USER+22)
#define GUI_ID_EDIT32  (GUI_ID_USER+23)
#define GUI_ID_EDIT33  (GUI_ID_USER+24)
#define GUI_ID_EDIT34  (GUI_ID_USER+25)


#define GUI_ID_TEXT10  (GUI_ID_USER+42)
#define GUI_ID_TEXT11  (GUI_ID_USER+43)
#define GUI_ID_TEXT12  (GUI_ID_USER+44)
#define GUI_ID_TEXT13  (GUI_ID_USER+45)
#define GUI_ID_TEXT14  (GUI_ID_USER+46)

static U8 select_id = 0;
static U8 select_num = 0;//选择哪个水泵
static U8 select_chg_flag = 0;//是否能选择

static TCHG_MENU Mchg_Menu[] = {

    {MSHOW, TYPE_TEXT, GUI_ID_TEXT2,  "%s", 1, SNUM_FLAG},
    {MSHOW, TYPE_TEXT, GUI_ID_TEXT3,  "%s", 1, SNUM_FLAG},
    {MSHOW, TYPE_TEXT, GUI_ID_TEXT4,  "%s", 1, SNUM_FLAG},
    {MSHOW, TYPE_TEXT, GUI_ID_TEXT5,  "%s", 1, SNUM_FLAG},
    {MSHOW, TYPE_TEXT, GUI_ID_TEXT6,  "%s", 1, SNUM_FLAG},

    {MSHOW, TYPE_EDIT, GUI_ID_EDIT10, "%.1f", 0, TEMP_RATE},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT11, "%.1f", 0, TEMP_RATE},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT12, "%.1f", 0, TEMP_RATE},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT13, "%.1f", 0, TEMP_RATE},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT14, "%.1f", 0, TEMP_RATE},

    {MSHOW, TYPE_EDIT, GUI_ID_EDIT15, "%.1f", 0, TEMP_RATE},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT16, "%.1f", 0, TEMP_RATE},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT17, "%.1f", 0, TEMP_RATE},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT18, "%.1f", 0, TEMP_RATE},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT19, "%.1f", 0, TEMP_RATE},

    {MSHOW, TYPE_EDIT, GUI_ID_EDIT20, "%dS", 0, 1},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT21, "%dS", 0, 1},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT22, "%dS", 0, 1},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT23, "%dS", 0, 1},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT24, "%dS", 0, 1},

    {MSHOW, TYPE_EDIT, GUI_ID_EDIT25, "%dS", 0, 1},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT26, "%dS", 0, 1},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT27, "%dS", 0, 1},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT28, "%dS", 0, 1},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT29, "%dS", 0, 1},

    {MEXIT, 0, 0, "", 0, 0},
};
static void Set_OptValue(WM_HWIN hWin, U8 sid)
{
    U8 i = 0;
    //增加是否启用

    for (i = 0; i < 5; i++) {
        CHECKBOX_SetState(WM_GetDialogItem(hWin, (GUI_ID_CHECK1 + i) ), basic_inf.heat[select_num][5 * sid + i].enable);
    }

    //增加时间
    for (i = 0; i < 5; i++) {
        sprintf(show_buf, "%02d:%02d",  basic_inf.heat[select_num][5 * sid + i].start_time / 60, basic_inf.heat[select_num][5 * sid + i].start_time % 60);
        EDIT_SetText(WM_GetDialogItem(hWin, (GUI_ID_EDIT0 + i) ), show_buf);
    }

    for (i = 0; i < 5; i++) {
        sprintf(show_buf, "%02d:%02d",  basic_inf.heat[select_num][5 * sid + i].stop_time / 60, basic_inf.heat[select_num][5 * sid + i].stop_time % 60);
        EDIT_SetText(WM_GetDialogItem(hWin, (GUI_ID_EDIT5 + i) ), show_buf);
    }

    if (basic_inf.heat_en[select_num] == STA_ON) {
        sprintf(show_buf, ":%s", GUI_LANG_GetTextEx(Lang_Enable, Language_Bit));
        TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT1), show_buf);
    } else {
        sprintf(show_buf, ":%s", GUI_LANG_GetTextEx(Lang_Disable, Language_Bit));
        TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT1), show_buf);
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
        Mchg_Menu[id++].value = basic_inf.heat[select_num][5 * sid + i].start_offtemp;
    }

    for (i = 0; i < 5; i++) {
        Mchg_Menu[id++].value = basic_inf.heat[select_num][5 * sid + i].stop_offtemp;
    }

    for (i = 0; i < 5; i++) {
        Mchg_Menu[id++].value = basic_inf.heat[select_num][5 * sid + i].start_sec;
    }

    for (i = 0; i < 5; i++) {
        Mchg_Menu[id++].value = basic_inf.heat[select_num][5 * sid + i].stop_sec;
    }

}




/*********************************************************************
*
*       Dialog resource
*
* This table conatins the info required to create the dialog.
* It has been created by ucGUIbuilder.
*/
#define XPos_Collec  50
#define YPos_Collec  0
#define Y_Space     40
//#define XPos_Collec1 40
#define Text_Width   36
#define Category_Space  10


static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect,    "",               0,                       0,  0,  800, 480, 0, 0},
    { TEXT_CreateIndirect,      "设备管理-加热器",          GUI_ID_TEXT0,           250, 4, 300, 42, 0, 0},

    { TEXT_CreateIndirect,      " ",        		GUI_ID_TEXT1,              170 + XPos_Collec, 60, 200, Text_Width, 0, 0},
    { DROPDOWN_CreateIndirect,	 "",			 	GUI_ID_DROPDOWN0,		30 + XPos_Collec, 60, 130, 120, 0, 0},


    { TEXT_CreateIndirect,       "1",    GUI_ID_TEXT2,               -10 + XPos_Collec , 100 + YPos_Collec + Y_Space * 1 + Category_Space * 0, 24, Text_Width, 0, 0},
    { TEXT_CreateIndirect,      "2",      GUI_ID_TEXT3,               -10 + XPos_Collec, 100 + YPos_Collec + Y_Space * 2 + Category_Space * 1, 24, Text_Width, 0, 0},
    { TEXT_CreateIndirect,     "3",       GUI_ID_TEXT4,               -10 + XPos_Collec, 100 + YPos_Collec + Y_Space * 3 + Category_Space * 2, 24, Text_Width, 0, 0},
    { TEXT_CreateIndirect,     "4",     GUI_ID_TEXT5,                -10 + XPos_Collec, 100 + YPos_Collec + Y_Space * 4 + Category_Space * 3, 24, Text_Width, 0, 0},
    { TEXT_CreateIndirect,     "5",      GUI_ID_TEXT6,               -10 + XPos_Collec, 100 + YPos_Collec + Y_Space * 5 + Category_Space * 4, 24, Text_Width, 0, 0},

    { TEXT_CreateIndirect,       "开始",    GUI_ID_TEXT7,               30 + XPos_Collec, 100 + YPos_Collec + Y_Space * 0, 60, Text_Width, 0, 0},

    { EDIT_CreateIndirect,     "8:00",      GUI_ID_EDIT0,                 30 + XPos_Collec, 100 + YPos_Collec + Y_Space * 1, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,    "10:00",     GUI_ID_EDIT1,                  30 + XPos_Collec, 100 + YPos_Collec + Y_Space * 2 + Category_Space * 1, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,    "14:00",        GUI_ID_EDIT2,               30 + XPos_Collec, 100 + YPos_Collec + Y_Space * 3 + Category_Space * 2, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,    "17:00",         GUI_ID_EDIT3,              30 + XPos_Collec, 100 + YPos_Collec + Y_Space * 4 + Category_Space * 3, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,    "00:00",        GUI_ID_EDIT4,                 30 + XPos_Collec, 100 + YPos_Collec + Y_Space * 5 + Category_Space * 4, 72, Text_Width, 0, 0},

    { TEXT_CreateIndirect,        "结束时间",    GUI_ID_TEXT8,            105 + XPos_Collec, 100 + YPos_Collec + Y_Space * 0, 100, Text_Width, 0, 0},

    { EDIT_CreateIndirect,      "10:00",   GUI_ID_EDIT5,               112 + XPos_Collec, 100 + YPos_Collec + Y_Space * 1, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,       "14:00",  GUI_ID_EDIT6,              112 + XPos_Collec, 100 + YPos_Collec + Y_Space * 2 + Category_Space * 1, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,     "17:00",    GUI_ID_EDIT7,                112 + XPos_Collec, 100 + YPos_Collec + Y_Space * 3 + Category_Space * 2, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,    "24:00",      GUI_ID_EDIT8,             112 + XPos_Collec, 100 + YPos_Collec + Y_Space * 4 + Category_Space * 3, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,     "8:00",       GUI_ID_EDIT9,              112 + XPos_Collec, 100 + YPos_Collec + Y_Space * 5 + Category_Space * 4, 72, Text_Width, 0, 0},
    { TEXT_CreateIndirect,  "启动", GUI_ID_TEXT9,                  220 + XPos_Collec, 100 + YPos_Collec + Y_Space * 0, 60, Text_Width, 0, 0},

    { EDIT_CreateIndirect,      "3%",      GUI_ID_EDIT10,             220 + XPos_Collec, 100 + YPos_Collec + Y_Space * 1, 60, Text_Width, 0, 0},
    { EDIT_CreateIndirect,      "33%",     GUI_ID_EDIT11,                 220 + XPos_Collec, 100 + YPos_Collec + Y_Space * 2 + Category_Space * 1, 60, Text_Width, 0, 0},
    { EDIT_CreateIndirect,      "33%",      GUI_ID_EDIT12,                220 + XPos_Collec, 100 + YPos_Collec + Y_Space * 3 + Category_Space * 2, 60, Text_Width, 0, 0},
    { EDIT_CreateIndirect,      "53%",       GUI_ID_EDIT13,               220 + XPos_Collec, 100 + YPos_Collec + Y_Space * 4 + Category_Space * 3, 60, Text_Width, 0, 0},
    { EDIT_CreateIndirect,      "53%",      GUI_ID_EDIT14,                220 + XPos_Collec, 100 + YPos_Collec + Y_Space * 5 + Category_Space * 4, 60, Text_Width, 0, 0},

    { TEXT_CreateIndirect,      "停止温差",      GUI_ID_TEXT10,           290 + XPos_Collec, 100 + YPos_Collec + Y_Space * 0, 106, Text_Width, 0, 0},

    { EDIT_CreateIndirect,      "13%",    GUI_ID_EDIT15,                   292 + XPos_Collec, 100 + YPos_Collec + Y_Space * 1, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,      "13%",    GUI_ID_EDIT16,                   292 + XPos_Collec, 100 + YPos_Collec + Y_Space * 2 + Category_Space * 1, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,      "13%",    GUI_ID_EDIT17,                   292 + XPos_Collec, 100 + YPos_Collec + Y_Space * 3 + Category_Space * 2, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,     "13%",      GUI_ID_EDIT18,                  292 + XPos_Collec, 100 + YPos_Collec + Y_Space * 4 + Category_Space * 3, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,     "13%",       GUI_ID_EDIT19,               292 + XPos_Collec, 100 + YPos_Collec + Y_Space * 5 + Category_Space * 4, 72, Text_Width, 0, 0},

    { TEXT_CreateIndirect,      "启动",         GUI_ID_TEXT11,                  400 + XPos_Collec, 100 + YPos_Collec + Y_Space * 0, 60, Text_Width, 0, 0},

    { EDIT_CreateIndirect,      "300",    GUI_ID_EDIT20,             400 + XPos_Collec, 100 + YPos_Collec + Y_Space * 1, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,      "300",   GUI_ID_EDIT21,              400 + XPos_Collec, 100 + YPos_Collec + Y_Space * 2 + Category_Space * 1, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,      "300",    GUI_ID_EDIT22,             400 + XPos_Collec, 100 + YPos_Collec + Y_Space * 3 + Category_Space * 2, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,     "300",      GUI_ID_EDIT23,            400 + XPos_Collec, 100 + YPos_Collec + Y_Space * 4 + Category_Space * 3, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,     "300",     GUI_ID_EDIT24,             400 + XPos_Collec, 100 + YPos_Collec + Y_Space * 5 + Category_Space * 4, 72, Text_Width, 0, 0},

    { TEXT_CreateIndirect,      "关闭时间",      GUI_ID_TEXT12,         480 + XPos_Collec, 100 + YPos_Collec + Y_Space * 0, 120, Text_Width, 0, 0},
    { EDIT_CreateIndirect,      "300",        GUI_ID_EDIT25,             492 + XPos_Collec, 100 + YPos_Collec + Y_Space * 1, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,      "300",        GUI_ID_EDIT26,              492 + XPos_Collec, 100 + YPos_Collec + Y_Space * 2 + Category_Space * 1, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,      "300",      GUI_ID_EDIT27,             492 + XPos_Collec, 100 + YPos_Collec + Y_Space * 3 + Category_Space * 2, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,     "300",        GUI_ID_EDIT28,            492 + XPos_Collec, 100 + YPos_Collec + Y_Space * 4 + Category_Space * 3, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,     "300",       GUI_ID_EDIT29,             492 + XPos_Collec, 100 + YPos_Collec + Y_Space * 5 + Category_Space * 4, 72, Text_Width, 0, 0},

    { TEXT_CreateIndirect,      "",      		GUI_ID_TEXT14,         550 + XPos_Collec, 60, 200, 30, 0, 0},

    { CHECKBOX_CreateIndirect,  "启用",        GUI_ID_CHECK1,        620 + XPos_Collec, 100 + YPos_Collec + Y_Space * 1 + Category_Space * 0.5, 90, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",        GUI_ID_CHECK2,        620 + XPos_Collec, 100 + YPos_Collec + Y_Space * 2 + Category_Space * 1.5, 90, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",        GUI_ID_CHECK3,        620 + XPos_Collec, 100 + YPos_Collec + Y_Space * 3 + Category_Space * 2.5, 90, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",        GUI_ID_CHECK4,        620 + XPos_Collec, 100 + YPos_Collec + Y_Space * 4 + Category_Space * 3.5, 90, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",        GUI_ID_CHECK5,        620 + XPos_Collec, 100 + YPos_Collec + Y_Space * 5 + Category_Space * 4.5, 90, CHECK_SIZE, 0, 0},

    { BUTTON_CreateIndirect,    "上一页",          GUI_ID_BUTTON0,          MENU_BACK_X1, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
    { BUTTON_CreateIndirect,    "下一页",          GUI_ID_BUTTON1,          MENU_BACK_X2, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
    { BUTTON_CreateIndirect,    "返回",             GUI_ID_BUTTON2,         MENU_BACK_X3, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
};



static U8 temp_id = 0;
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

    basic_inf.heat[select_num][5 * select_id + temp_id].start_time = stemp;
    sprintf(show_buf, "%02d:%02d",  basic_inf.heat[select_num][5 * select_id + temp_id].start_time / 60, basic_inf.heat[select_num][5 * select_id + temp_id].start_time % 60);
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

    basic_inf.heat[select_num][5 * select_id + temp_id].stop_time = stemp;
    sprintf(show_buf, "%02d:%02d",  basic_inf.heat[select_num][5 * select_id + temp_id].stop_time / 60, basic_inf.heat[select_num][5 * select_id + temp_id].stop_time % 60);
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, (GUI_ID_EDIT5 + temp_id) ), show_buf);
}

static void Start_Offtemp_Get(void)
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

    basic_inf.heat[select_num][5 * select_id + temp_id].start_offtemp = stemp;
    MChange_Menu(Menu_Value_Init, Current_HWIN, (PCHG_MENU )Mchg_Menu, select_id);
}
static void Stop_Offtemp_Get(void)
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

    basic_inf.heat[select_num][5 * select_id + temp_id].stop_offtemp = stemp;
    MChange_Menu(Menu_Value_Init, Current_HWIN, (PCHG_MENU )Mchg_Menu, select_id);
}

static void Start_Sec_Get(void)
{
    float ftemp = Get_Input_Value();
    short stemp = (short)ftemp;

//    if (stemp > HUM_ADJMAX) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = HUM_ADJMAX;
//    }

    if (stemp < 0) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 0;
    }

    basic_inf.heat[select_num][5 * select_id + temp_id].start_sec = stemp;
    MChange_Menu(Menu_Value_Init, Current_HWIN, (PCHG_MENU )Mchg_Menu, select_id);
}
static void Stop_Sec_Get(void)
{
    float ftemp = Get_Input_Value();
    short stemp = (short)ftemp;

//    if (stemp > HUM_ADJMAX) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = HUM_ADJMAX;
//    }

    if (stemp < 0) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 0;
    }

    basic_inf.heat[select_num][5 * select_id + temp_id].stop_sec = stemp;
    MChange_Menu(Menu_Value_Init, Current_HWIN, (PCHG_MENU )Mchg_Menu, select_id);
}

static void Get_OptValue(WM_HWIN hWin, U8 sid)
{
    U8 i = 0;
    //增加是否启用

    for (i = 0; i < 5; i++) {
        basic_inf.heat[select_num][5 * sid + i].enable = CHECKBOX_GetState(WM_GetDialogItem(hWin, (GUI_ID_CHECK1 + i) ));
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
//    WM_HWIN hWin = pMsg->hWin;
    Show_MBackPic();
    GUI_SetColor(GUI_LINE_COLOR);
    GUI_DrawRoundedRect(6, 55, 794, 396, 10);
    GUI_DrawRoundedRect(2, 52, 798, 399, 10);

    //划横线1
//    GUI_DrawLine(6, 95, 794, 95);

//    GUI_DrawLine(6, 136, 794, 136);

//    GUI_DrawLine(6, 184, 794, 184);
//    GUI_DrawLine(6, 234, 794, 234);
//    GUI_DrawLine(6, 284, 794, 284);
//    GUI_DrawLine(6, 334, 794, 334);
//    //划竖线

//    GUI_DrawLine(30, 95, 30, 396);
//    GUI_DrawLine(217, 95, 217, 396);

//    GUI_DrawLine(398, 95, 398, 396);
//    GUI_DrawLine(588, 95, 588, 396);
    //GUI_DrawLine(615, 95, 615, 396);


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

    for (MDev_ID = GUI_ID_EDIT10; MDev_ID <= GUI_ID_EDIT29; MDev_ID++) {
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

    for (MDev_ID = GUI_ID_TEXT10; MDev_ID <= GUI_ID_TEXT12; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24(hItem);
    }

    for (MDev_ID = GUI_ID_TEXT14; MDev_ID <= GUI_ID_TEXT14; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24L(hItem);
        TEXT_SetTextColor(hItem, GUI_LIGHTGRAY);
    }
	
    for (MDev_ID = GUI_ID_TEXT1; MDev_ID <= GUI_ID_TEXT1; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24L(hItem);
    }

    sprintf(show_buf, "%s-%s", GUI_LANG_GetTextEx(Lang_Device, Language_Bit), GUI_LANG_GetTextEx(Lang_Heat_Setting, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT0), show_buf);                                              // 设备管理-加热器
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT7), GUI_LANG_GetTextEx(Lang_Begin, Language_Bit));          // 开始
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT8), GUI_LANG_GetTextEx(Lang_End_time, Language_Bit));       // 结束时间
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT9), GUI_LANG_GetTextEx(Lang_Start, Language_Bit));          // 启动温差
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT10), GUI_LANG_GetTextEx(Lang_Stop_Tempdiff, Language_Bit)); // 停止温差
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT11), GUI_LANG_GetTextEx(Lang_Start, Language_Bit));         // 启动
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT12), GUI_LANG_GetTextEx(Lang_Stop_Time, Language_Bit));     // 关闭时间

    sprintf(show_buf, "%s:%.1f%s", GUI_LANG_GetTextEx(Lang_HeatT, Language_Bit), (float)(cal_inf.heat_temp * 1.0f / (float)TEMP_RATE), GUI_LANG_GetTextEx(Lang_oC, Language_Bit)); // 加热温度
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT14), (const char *)show_buf);

    hItem = WM_GetDialogItem(hWin, GUI_ID_DROPDOWN0);
    Dropdown_SetHZ24(hItem);

    sprintf(show_buf, "%s%d", GUI_LANG_GetTextEx(Lang_Heat, Language_Bit), 1);
    DROPDOWN_AddString(hItem, show_buf);
    sprintf(show_buf, "%s%d", GUI_LANG_GetTextEx(Lang_Heat, Language_Bit), 2);
    DROPDOWN_AddString(hItem, show_buf);
    sprintf(show_buf, "%s%d", GUI_LANG_GetTextEx(Lang_Heat, Language_Bit), 3);
    DROPDOWN_AddString(hItem, show_buf);

    if(select_chg_flag == STA_OFF){
		WM_DisableWindow(hItem);
	}
	
    DROPDOWN_SetSel(hItem, select_num);

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
		
        if (old_Id != Id) {
            old_Id = Id;
            break;
        }
		
        if (Id >= GUI_ID_EDIT0 && Id <= GUI_ID_EDIT9 && NCode == WM_NOTIFICATION_RELEASED
                && get_login_permiss(LOGIN_ADMIN) < 0) {
            MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_NO_LOGIN));
            break;
        }
		
        if (Id >= GUI_ID_EDIT10 && Id <= GUI_ID_EDIT29 && NCode == WM_NOTIFICATION_RELEASED
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
			
        case GUI_ID_DROPDOWN0:
            if (NCode == WM_NOTIFICATION_SEL_CHANGED) {
                Get_OptValue(hWin, select_id);
                select_num = DROPDOWN_GetSel(WM_GetDialogItem(hWin, (GUI_ID_DROPDOWN0)));
                Set_OptValue(hWin, select_id);
                MChange_Menu(Menu_Value_Init, hWin, (PCHG_MENU )Mchg_Menu, select_id);
            }

            break;
			
        case GUI_ID_BUTTON2:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Get_OptValue(hWin, select_id);

                if (Config_Save(SHEAT_ID) < 0) {
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
                Set_Output_Callback(Start_Time_Get);
                Set_Input_Len(4);
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
                Set_Output_Callback(Stop_Time_Get);
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
                Set_Output_Callback(Start_Offtemp_Get);
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
                Set_Output_Callback(Stop_Offtemp_Get);
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
                Set_Output_Callback(Start_Sec_Get);
                Set_Input_Len(3);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT25:
        case GUI_ID_EDIT26:
        case GUI_ID_EDIT27:
        case GUI_ID_EDIT28:
        case GUI_ID_EDIT29:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                temp_id = Id - GUI_ID_EDIT25;
                Set_Output_Callback(Stop_Sec_Get);
                Set_Input_Len(3);
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
WM_HWIN MDev_Heater;
void MDev_Heater_Task(WM_HWIN hParent, U8 num, U8 chg_flag)
{
    select_id = 0;
    select_num = num;
	select_chg_flag = chg_flag;
    MDev_Heater = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    Current_HWIN = MDev_Heater;
}
