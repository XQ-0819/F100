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

#define GUI_ID_TEXT10  (GUI_ID_USER+42)
#define GUI_ID_TEXT11  (GUI_ID_USER+43)
#define GUI_ID_TEXT12  (GUI_ID_USER+44)

static U8 select_id = 0;
static TCHG_MENU Mchg_Menu[] = {

    {MSHOW, TYPE_TEXT, GUI_ID_TEXT3, "%s", 1, SNUM_FLAG},
    {MSHOW, TYPE_TEXT, GUI_ID_TEXT4, "%s", 1, SNUM_FLAG},
    {MSHOW, TYPE_TEXT, GUI_ID_TEXT5, "%s", 1, SNUM_FLAG},
    {MSHOW, TYPE_TEXT, GUI_ID_TEXT6, "%s", 1, SNUM_FLAG},
    {MSHOW, TYPE_TEXT, GUI_ID_TEXT7, "%s", 1, SNUM_FLAG},


    {MSHOW, TYPE_EDIT, GUI_ID_EDIT0, "%d", 0, 1},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT1, "%d", 0, 1},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT2, "%d", 0, 1},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT3, "%d", 0, 1},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT4, "%d", 0, 1},

    {MSHOW, TYPE_EDIT, GUI_ID_EDIT5, "%.2f", 0, WT_RATE},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT6, "%.2f", 0, WT_RATE},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT7, "%.2f", 0, WT_RATE},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT8, "%.2f", 0, WT_RATE},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT9, "%.2f", 0, WT_RATE},

    {MSHOW, TYPE_EDIT, GUI_ID_EDIT10, "%d", 0, 1},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT11, "%d", 0, 1},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT12, "%d", 0, 1},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT13, "%d", 0, 1},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT14, "%d", 0, 1},

    {MSHOW, TYPE_EDIT, GUI_ID_EDIT15, "%.2f", 0, BREATH_RATE},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT16, "%.2f", 0, BREATH_RATE},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT17, "%.2f", 0, BREATH_RATE},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT18, "%.2f", 0, BREATH_RATE},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT19, "%.2f", 0, BREATH_RATE},

//    {MSHOW, TYPE_EDIT, GUI_ID_EDIT20, "%.1f", 0, FAN_RATE},
//    {MSHOW, TYPE_EDIT, GUI_ID_EDIT21, "%.1f", 0, FAN_RATE},
//    {MSHOW, TYPE_EDIT, GUI_ID_EDIT22, "%.1f", 0, FAN_RATE},
//    {MSHOW, TYPE_EDIT, GUI_ID_EDIT23, "%.1f", 0, FAN_RATE},
//    {MSHOW, TYPE_EDIT, GUI_ID_EDIT24, "%.1f", 0, FAN_RATE},
    {MEXIT, 0, 0, "", 0, 0},
};

//更新最小通风就要更新曲线
void Update_MinAir(WM_HWIN hWin)
{
    int i = 0, id = 0;
    float temp;

    for (i = 0; i < MINAIR_NUM; i++) {
        temp = (float)(ctrl_inf.minair[i].weight * 1.0f / WT_RATE);
        temp = (float)(temp * ctrl_inf.minair[i].breath / BREATH_RATE);

        if (cal_inf.exminair_lv >= 0 && cal_inf.exminair_lv < EXMINAIR_NUM ) {//根据室外温度修正呼吸率
            temp = (float)(temp * ctrl_inf.exminair.x[cal_inf.exminair_lv].breath / EXBREATH_RATE);
        }

        temp = (float)(temp * basic_inf.survival * AIR_RATE / AIR_KM);
        ctrl_inf.minair[i].min_air = (unsigned int)temp;
    }

    if (hWin != 0) {
        for (id = GUI_ID_EDIT20, i = 0; i < 5; i++) {
            sprintf(show_buf, "%.2f", (float)(ctrl_inf.minair[5 * select_id + i].min_air * 1.0f / AIR_RATE));
            EDIT_SetText(WM_GetDialogItem(hWin, id++), (const char *) show_buf);
        }
    }
}
static void Set_OptValue(WM_HWIN hWin, U8 sid)
{
    U8 i = 0;
    //增加是否启用
    CHECKBOX_SetState(WM_GetDialogItem(hWin, GUI_ID_CHECK0 ), ctrl_inf.minair_en);

    for (i = 0; i < 5; i++) {
        CHECKBOX_SetState(WM_GetDialogItem(hWin, GUI_ID_CHECK1 + i ), ctrl_inf.minair[5 * sid + i].enable);
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
        Mchg_Menu[id++].value = ctrl_inf.minair[5 * sid + i].grow_day;
    }

    for (i = 0; i < 5; i++) {
        Mchg_Menu[id++].value = ctrl_inf.minair[5 * sid + i].weight;
    }

    for (i = 0; i < 5; i++) {
        Mchg_Menu[id++].value = basic_inf.survival;
    }

    for (i = 0; i < 5; i++) {
        Mchg_Menu[id++].value = ctrl_inf.minair[5 * sid + i].breath;
    }

//    for (i = 0; i < 5; i++) {
//        Mchg_Menu[id++].value = ctrl_inf.minair[5 * sid + i].min_air;
//    }

}
/*********************************************************************
*
*       Dialog resource
*
* This table conatins the info required to create the dialog.
* It has been created by ucGUIbuilder.
*/
#define XPos_Inital    	80
#define XPos_Collec  	10
#define YPos_Collec  	0
#define Y_Space     	40
#define X_Space   		120
#define Text_Width   	36
#define Category_Space  10
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect,    "",               0,                       0,  0,  800, 480, 0, 0},
    { TEXT_CreateIndirect,      "控制管理-最小通风", GUI_ID_TEXT0,            250, 4, 300, 42, 0, 0},

    { TEXT_CreateIndirect,      "最小通风",        GUI_ID_TEXT1,              30 + XPos_Collec, 60, 150, 30, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",        GUI_ID_CHECK0,                 190 + XPos_Collec, 60, 90, CHECK_SIZE, 0, 0},

    { TEXT_CreateIndirect,       "序号",    GUI_ID_TEXT2,          0 + XPos_Collec, 100 + YPos_Collec + Y_Space * 0, 60, Text_Width, 0, 0},
    { TEXT_CreateIndirect,       "1",    GUI_ID_TEXT3,               0 + XPos_Collec, 100 + YPos_Collec + Y_Space * 1 + Category_Space * 0, 60, Text_Width, 0, 0},
    { TEXT_CreateIndirect,      "2",      GUI_ID_TEXT4,             0 + XPos_Collec, 100 + YPos_Collec + Y_Space * 2 + Category_Space * 1, 60, Text_Width, 0, 0},
    { TEXT_CreateIndirect,     "3",       GUI_ID_TEXT5,               0 + XPos_Collec, 100 + YPos_Collec + Y_Space * 3 + Category_Space * 2, 60, Text_Width, 0, 0},
    { TEXT_CreateIndirect,     "4",     GUI_ID_TEXT6,                0 + XPos_Collec, 100 + YPos_Collec + Y_Space * 4 + Category_Space * 3, 60, Text_Width, 0, 0},
    { TEXT_CreateIndirect,     "5",      GUI_ID_TEXT7,               0 + XPos_Collec, 100 + YPos_Collec + Y_Space * 5 + Category_Space * 4, 60, Text_Width, 0, 0},

    { TEXT_CreateIndirect,       "日龄",    GUI_ID_TEXT8,                 XPos_Inital + X_Space * 0 + XPos_Collec, 100 + YPos_Collec + Y_Space * 0, 60, Text_Width, 0, 0},
    { EDIT_CreateIndirect,     "8:00",      GUI_ID_EDIT0,                 XPos_Inital + X_Space * 0 + XPos_Collec, 100 + YPos_Collec + Y_Space * 1, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,    "10:00",     GUI_ID_EDIT1,                  XPos_Inital + X_Space * 0 + XPos_Collec, 100 + YPos_Collec + Y_Space * 2 + Category_Space * 1, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,    "14:00",        GUI_ID_EDIT2,               XPos_Inital + X_Space * 0 + XPos_Collec, 100 + YPos_Collec + Y_Space * 3 + Category_Space * 2, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,    "17:00",         GUI_ID_EDIT3,              XPos_Inital + X_Space * 0 + XPos_Collec, 100 + YPos_Collec + Y_Space * 4 + Category_Space * 3, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,    "00:00",        GUI_ID_EDIT4,                 XPos_Inital + X_Space * 0 + XPos_Collec, 100 + YPos_Collec + Y_Space * 5 + Category_Space * 4, 72, Text_Width, 0, 0},




    { TEXT_CreateIndirect,        "均重/Kg",    GUI_ID_TEXT9,        XPos_Inital + X_Space * 1 + XPos_Collec - 10, 100 + YPos_Collec + Y_Space * 0, 120, Text_Width, 0, 0},
    { EDIT_CreateIndirect,      "10:00",   GUI_ID_EDIT5,               XPos_Inital + X_Space * 1 + XPos_Collec, 100 + YPos_Collec + Y_Space * 1, 92, Text_Width, 0, 0},
    { EDIT_CreateIndirect,       "14:00",  GUI_ID_EDIT6,               XPos_Inital + X_Space * 1 + XPos_Collec, 100 + YPos_Collec + Y_Space * 2 + Category_Space * 1, 92, Text_Width, 0, 0},
    { EDIT_CreateIndirect,     "17:00",    GUI_ID_EDIT7,               XPos_Inital + X_Space * 1 + XPos_Collec, 100 + YPos_Collec + Y_Space * 3 + Category_Space * 2, 92, Text_Width, 0, 0},
    { EDIT_CreateIndirect,    "24:00",      GUI_ID_EDIT8,              XPos_Inital + X_Space * 1 + XPos_Collec, 100 + YPos_Collec + Y_Space * 4 + Category_Space * 3, 92, Text_Width, 0, 0},
    { EDIT_CreateIndirect,     "8:00",       GUI_ID_EDIT9,             XPos_Inital + X_Space * 1 + XPos_Collec, 100 + YPos_Collec + Y_Space * 5 + Category_Space * 4, 92, Text_Width, 0, 0},



    { TEXT_CreateIndirect,  "存栏量", GUI_ID_TEXT10,                     XPos_Inital + X_Space * 2 + XPos_Collec, 100 + YPos_Collec + Y_Space * 0, 100, Text_Width, 0, 0},
    { EDIT_CreateIndirect,      "3%",      GUI_ID_EDIT10,                 10 + XPos_Inital + X_Space * 2 + XPos_Collec, 100 + YPos_Collec + Y_Space * 1, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,      "33%",     GUI_ID_EDIT11,                  10 + XPos_Inital + X_Space * 2 + XPos_Collec, 100 + YPos_Collec + Y_Space * 2 + Category_Space * 1, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,      "33%",      GUI_ID_EDIT12,                 10 + XPos_Inital + X_Space * 2 + XPos_Collec, 100 + YPos_Collec + Y_Space * 3 + Category_Space * 2, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,      "53%",       GUI_ID_EDIT13,                10 + XPos_Inital + X_Space * 2 + XPos_Collec, 100 + YPos_Collec + Y_Space * 4 + Category_Space * 3, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,      "53%",      GUI_ID_EDIT14,                 10 + XPos_Inital + X_Space * 2 + XPos_Collec, 100 + YPos_Collec + Y_Space * 5 + Category_Space * 4, 72, Text_Width, 0, 0},

    { TEXT_CreateIndirect,      "呼吸系数",      GUI_ID_TEXT11,             XPos_Inital + X_Space * 3 + XPos_Collec, 100 + YPos_Collec + Y_Space * 0, 96, Text_Width, 0, 0},
    { EDIT_CreateIndirect,      "13%",    GUI_ID_EDIT15,                    10 + XPos_Inital + X_Space * 3 + XPos_Collec, 100 + YPos_Collec + Y_Space * 1, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,      "13%",    GUI_ID_EDIT16,                   10 + XPos_Inital + X_Space * 3 + XPos_Collec, 100 + YPos_Collec + Y_Space * 2 + Category_Space * 1, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,      "13%",    GUI_ID_EDIT17,                   10 + XPos_Inital + X_Space * 3 + XPos_Collec, 100 + YPos_Collec + Y_Space * 3 + Category_Space * 2, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,     "13%",      GUI_ID_EDIT18,                   10 + XPos_Inital + X_Space * 3 + XPos_Collec, 100 + YPos_Collec + Y_Space * 4 + Category_Space * 3, 72, Text_Width, 0, 0},
    { EDIT_CreateIndirect,     "13%",       GUI_ID_EDIT19,                  10 + XPos_Inital + X_Space * 3 + XPos_Collec, 100 + YPos_Collec + Y_Space * 5 + Category_Space * 4, 72, Text_Width, 0, 0},

    { TEXT_CreateIndirect,      "最小风量/km3/h",      GUI_ID_TEXT12,       XPos_Inital + X_Space * 4 + XPos_Collec, 100 + YPos_Collec + Y_Space * 0, 180, Text_Width, 0, 0},
    { EDIT_CreateIndirect,      "1",    GUI_ID_EDIT20,              (-4) + XPos_Inital + X_Space * 4 + XPos_Collec, 100 + YPos_Collec + Y_Space * 1, 120, Text_Width, 0, 0},
    { EDIT_CreateIndirect,      "1",   GUI_ID_EDIT21,               (-4) + XPos_Inital + X_Space * 4 + XPos_Collec, 100 + YPos_Collec + Y_Space * 2 + Category_Space * 1, 120, Text_Width, 0, 0},
    { EDIT_CreateIndirect,      "1",    GUI_ID_EDIT22,              (-4) + XPos_Inital + X_Space * 4 + XPos_Collec, 100 + YPos_Collec + Y_Space * 3 + Category_Space * 2, 120, Text_Width, 0, 0},
    { EDIT_CreateIndirect,     "1",      GUI_ID_EDIT23,             (-4) + XPos_Inital + X_Space * 4 + XPos_Collec, 100 + YPos_Collec + Y_Space * 4 + Category_Space * 3, 120, Text_Width, 0, 0},
    { EDIT_CreateIndirect,     "1",     GUI_ID_EDIT24,             (-4) + XPos_Inital + X_Space * 4 + XPos_Collec, 100 + YPos_Collec + Y_Space * 5 + Category_Space * 4, 120, Text_Width, 0, 0},

    { CHECKBOX_CreateIndirect,  "启用",        GUI_ID_CHECK1,        695 + XPos_Collec, 100 + YPos_Collec + Y_Space * 1 + Category_Space * 0.5, 90, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",        GUI_ID_CHECK2,        695 + XPos_Collec, 100 + YPos_Collec + Y_Space * 2 + Category_Space * 1.5, 90, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",        GUI_ID_CHECK3,        695 + XPos_Collec, 100 + YPos_Collec + Y_Space * 3 + Category_Space * 2.5, 90, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",        GUI_ID_CHECK4,        695 + XPos_Collec, 100 + YPos_Collec + Y_Space * 4 + Category_Space * 3.5, 90, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",        GUI_ID_CHECK5,        695 + XPos_Collec, 100 + YPos_Collec + Y_Space * 5 + Category_Space * 4.5, 90, CHECK_SIZE, 0, 0},

    { BUTTON_CreateIndirect,    "上一页",          GUI_ID_BUTTON0,          MENU_BACK_X1, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
    { BUTTON_CreateIndirect,    "下一页",          GUI_ID_BUTTON1,          MENU_BACK_X2, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
    { BUTTON_CreateIndirect,    "返回",             GUI_ID_BUTTON2,         MENU_BACK_X3, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},


};



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
    //划竖线

    //GUI_DrawLine(30, 95, 30, 396);
    //GUI_DrawLine(217, 95, 217, 396);

    //GUI_DrawLine(398, 95, 398, 396);
    //GUI_DrawLine(588, 95, 588, 396);
    //GUI_DrawLine(615, 95, 615, 396);


}


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

    ctrl_inf.minair[5 * select_id + temp_id].grow_day = stemp;
    MChange_Menu(Menu_Value_Init, Current_HWIN, (PCHG_MENU )Mchg_Menu, select_id);
}

static void WT_Get(void)
{
    float ftemp = Get_Input_Value() * WT_RATE;
    unsigned short stemp = (unsigned short)ftemp;

//    if (stemp > TEMP_ADJMAX) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = TEMP_ADJMAX;
//    }

//    if (stemp < 0) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = 0;
//    }

    ctrl_inf.minair[5 * select_id + temp_id].weight = stemp;
    Update_MinAir(Current_HWIN);
    MChange_Menu(Menu_Value_Init, Current_HWIN, (PCHG_MENU )Mchg_Menu, select_id);
}

static void BREATH_Get(void)
{
    float ftemp = Get_Input_Value() * BREATH_RATE;
    unsigned short stemp = (unsigned short)ftemp;

//    if (stemp > TEMP_ADJMAX) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = TEMP_ADJMAX;
//    }

    if (stemp < 1) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 1;
    }

    ctrl_inf.minair[5 * select_id + temp_id].breath = stemp;
    Update_MinAir(Current_HWIN);
    MChange_Menu(Menu_Value_Init, Current_HWIN, (PCHG_MENU )Mchg_Menu, select_id);
}

/*
static void MINAIR_Get(void)
{
    float ftemp = Get_Input_Value() * FAN_RATE;
    short stemp = (short)ftemp;

//    if (stemp > TEMP_ADJMAX) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = TEMP_ADJMAX;
//    }

//    if (stemp < TEMP_ADJMIN) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = TEMP_ADJMIN;
//    }
    ctrl_inf.minair[5 * select_id + temp_id].min_air = stemp;
    MChange_Menu(Menu_Value_Init, Current_HWIN, (PCHG_MENU )Mchg_Menu, select_id);
}
*/


static void Get_OptValue(WM_HWIN hWin, U8 sid)
{
    U8 i = 0;

    ctrl_inf.minair_en = CHECKBOX_GetState(WM_GetDialogItem(hWin, GUI_ID_CHECK0));

    for (i = 0; i < 5; i++) {
        ctrl_inf.minair[5 * sid + i].enable = CHECKBOX_GetState(WM_GetDialogItem(hWin, GUI_ID_CHECK1 + i ));
    }
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

    for (MDev_ID = GUI_ID_EDIT10; MDev_ID <= GUI_ID_EDIT24; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Edit_SetHZ24(hItem);
    }

    for (MDev_ID = GUI_ID_EDIT10; MDev_ID <= GUI_ID_EDIT14; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_DisableWindow(hItem);
    }

    for (MDev_ID = GUI_ID_EDIT20; MDev_ID <= GUI_ID_EDIT24; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_DisableWindow(hItem);
    }

    for (MDev_ID = GUI_ID_CHECK0; MDev_ID <= GUI_ID_CHECK5; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        CHECKBOX_SetText(hItem, GUI_LANG_GetTextEx(Lang_Enable, Language_Bit));//启用
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

    sprintf(show_buf, "%s-%s", GUI_LANG_GetTextEx(Lang_Control, Language_Bit), GUI_LANG_GetTextEx(Lang_MinVent, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT0), show_buf);                                         // 控制管理-最小通风
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT1), GUI_LANG_GetTextEx(Lang_MinVent, Language_Bit));   // 最小通风
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT2), GUI_LANG_GetTextEx(Lang_Number_No, Language_Bit)); // 序号
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT8), GUI_LANG_GetTextEx(Lang_Day, Language_Bit));       // 日龄
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT9), GUI_LANG_GetTextEx(Lang_AVGWeight, Language_Bit)); // 均重(kg)
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT10), GUI_LANG_GetTextEx(Lang_Stock, Language_Bit));   // 存栏量
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT11), GUI_LANG_GetTextEx(Lang_Breathe, Language_Bit)); // 呼吸系数

    WM_MoveWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT12), -25, 0);
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT12), GUI_LANG_GetTextEx(Lang_MinAIR, Language_Bit)); // 最小风量(km3/h)

    Set_OptValue(hWin, select_id);
    MChange_Menu(Menu_Value_Init, hWin, (PCHG_MENU )Mchg_Menu, select_id);
    Update_MinAir(hWin);

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
		
        if (Id >= GUI_ID_EDIT15 && Id <= GUI_ID_EDIT19 && NCode == WM_NOTIFICATION_RELEASED
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

                if (Config_Save(SMINAIRC_ID) < 0) {
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
                    select_id --;
                    Set_OptValue(hWin, select_id);
                    MChange_Menu(Menu_Value_Init, hWin, (PCHG_MENU )Mchg_Menu, select_id);
                    Update_MinAir(hWin);

                }
            }

            break;

        case GUI_ID_BUTTON1:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                if (select_id == 0) {
                    Get_OptValue(hWin, select_id);
                    select_id ++;
                    Set_OptValue(hWin, select_id);
                    MChange_Menu(Menu_Value_Init, hWin, (PCHG_MENU )Mchg_Menu, select_id);
                    Update_MinAir(hWin);

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
                temp_id = (Id - GUI_ID_EDIT5);
                Set_Output_Callback(WT_Get);
                Set_Input_Len(5);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT15:
        case GUI_ID_EDIT16:
        case GUI_ID_EDIT17:
        case GUI_ID_EDIT18:
        case GUI_ID_EDIT19:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                temp_id = (Id - GUI_ID_EDIT15);
                Set_Output_Callback(BREATH_Get);
                Set_Input_Len(4);
                Input_Task(hWin);
            }

            break;

//        case GUI_ID_EDIT20:
//        case GUI_ID_EDIT21:
//        case GUI_ID_EDIT22:
//        case GUI_ID_EDIT23:
//        case GUI_ID_EDIT24:
//            if (NCode == WM_NOTIFICATION_RELEASED) {
//                temp_id = (Id - GUI_ID_EDIT20);
//                Set_Output_Callback(MINAIR_Get);
//                Set_Input_Len(4);
//                Input_Task(hWin);
//            }

//            break;

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
WM_HWIN Mctrl_MinAir;
void Mctrl_MinAir_Task(WM_HWIN hParent)
{
    select_id = 0;
    Mctrl_MinAir = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    Current_HWIN = Mctrl_MinAir;

}

