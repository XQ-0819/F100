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
#define GUI_ID_TEXT13   (GUI_ID_USER+4)
#define GUI_ID_TEXT14   (GUI_ID_USER+5)


#define GUI_ID_DROPDOWN10 (GUI_ID_USER+50)
#define GUI_ID_DROPDOWN11 (GUI_ID_USER+51)
#define GUI_ID_DROPDOWN12 (GUI_ID_USER+52)
#define GUI_ID_DROPDOWN13 (GUI_ID_USER+53)
#define GUI_ID_DROPDOWN14 (GUI_ID_USER+54)
#define GUI_ID_DROPDOWN15 (GUI_ID_USER+55)

/*********************************************************************
*
*       Dialog resource
*
* This table conatins the info required to create the dialog.
* It has been created by ucGUIbuilder.
*/
static WM_HTIMER hTimer;

#define XPos_Collec  15

static U8 select_id = 0;

#define CHG_NUM_MAX			(2)
#define CHG_MENU_MAX		(23)

char sensor_unit[SENSOR_TYPE_NUM][24] = {
    "--", "%.1f", "%d%%", "%dppm", "%dppm", "%dppm", "%dPa", "%.2fm/s", "%d%%"
};

float sensor_scale[SENSOR_TYPE_NUM] = {
    1, TEMP_RATE, HUM_RATE, CO2_RATE, NH3_RATE, H2S_RATE, VAC_RATE, SPEED_RATE, HUM_RATE
};

short sensor_max[SENSOR_TYPE_NUM] = {
    1, TEMP_ADJMAX, HUM_ADJMAX, CO2_ADJMAX, NH3_ADJMAX, NH3_ADJMAX, VAC_ADJMAX, SPEED_ADJMAX, HUM_ADJMAX
};

short sensor_min[SENSOR_TYPE_NUM] = {
    1, TEMP_ADJMIN, HUM_ADJMIN, CO2_ADJMIN, NH3_ADJMIN, NH3_ADJMIN, VAC_ADJMIN, SPEED_ADJMIN, HUM_ADJMIN
};


static TCHG_MENU Mchg_Menu[CHG_NUM_MAX][CHG_MENU_MAX] = {
    {
        {MSHOW, TYPE_TEXT, GUI_ID_TEXT1, "温度1:%.1f℃", 0, TEMP_RATE},
        {MSHOW, TYPE_TEXT, GUI_ID_TEXT2, "温度2:%.1f℃", 0, TEMP_RATE},
        {MSHOW, TYPE_TEXT, GUI_ID_TEXT3, "温度3:%.1f℃", 0, TEMP_RATE},
        {MSHOW, TYPE_TEXT, GUI_ID_TEXT4, "温度4:%.1f℃", 0, TEMP_RATE},
        {MSHOW, TYPE_TEXT, GUI_ID_TEXT5, "温度5:%.1f℃", 0, TEMP_RATE},
        {MSHOW, TYPE_TEXT, GUI_ID_TEXT6, "温度6:%.1f℃", 0, TEMP_RATE},
        {MSHOW, TYPE_TEXT, GUI_ID_TEXT7, "温度7:%.1f℃", 0, TEMP_RATE},

        {MSHOW, TYPE_EDIT, GUI_ID_EDIT0, "%.1f", 0, TEMP_RATE},
        {MSHOW, TYPE_EDIT, GUI_ID_EDIT1, "%.1f", 0, TEMP_RATE},
        {MSHOW, TYPE_EDIT, GUI_ID_EDIT2, "%.1f", 0, TEMP_RATE},
        {MSHOW, TYPE_EDIT, GUI_ID_EDIT3, "%.1f", 0, TEMP_RATE},
        {MSHOW, TYPE_EDIT, GUI_ID_EDIT4, "%.1f", 0, TEMP_RATE},
        {MSHOW, TYPE_EDIT, GUI_ID_EDIT5, "%.1f", 0, TEMP_RATE},
        {MSHOW, TYPE_EDIT, GUI_ID_EDIT6, "%.1f", 0, TEMP_RATE},

//        {MSHOW, TYPE_TEXT, GUI_ID_TEXT13, "校准", 0, 1},
        {MSHOW, TYPE_TEXT, GUI_ID_TEXT14, "校准:", 0, 1},

        {MSHOW, TYPE_CHECK, GUI_ID_CHECK0, "室内", 0, 1},
        {MSHOW, TYPE_CHECK, GUI_ID_CHECK1, "室内", 0, 1},
        {MSHOW, TYPE_CHECK, GUI_ID_CHECK2, "室内", 0, 1},
        {MSHOW, TYPE_CHECK, GUI_ID_CHECK3, "室内", 0, 1},
        {MSHOW, TYPE_CHECK, GUI_ID_CHECK4, "室内", 0, 1},
        {MSHOW, TYPE_CHECK, GUI_ID_CHECK5, "室内", 0, 1},
        {MSHOW, TYPE_CHECK, GUI_ID_CHECK6, "室外", 0, 1},
    },
    {
        {MSHOW, TYPE_TEXT, GUI_ID_TEXT1, "AI1:", 0, TEMP_RATE},
        {MSHOW, TYPE_TEXT, GUI_ID_TEXT2, "AI2:", 0, HUM_RATE},
        {MSHOW, TYPE_TEXT, GUI_ID_TEXT3, "AI3:", 0, CO2_RATE},
        {MSHOW, TYPE_TEXT, GUI_ID_TEXT4, "AI4:", 0, NH3_RATE},
        {MSHOW, TYPE_TEXT, GUI_ID_TEXT5, "AI5:", 0, H2S_RATE},
        {MSHOW, TYPE_TEXT, GUI_ID_TEXT6, "AI6:", 0, VAC_RATE},
        {MHIDE, TYPE_TEXT, GUI_ID_TEXT7, "风速:", 0, SPEED_RATE},

        {MSHOW, TYPE_EDIT, GUI_ID_EDIT0, "%.1f", 0, TEMP_RATE},
        {MSHOW, TYPE_EDIT, GUI_ID_EDIT1, "%d%%", 0, HUM_RATE},
        {MSHOW, TYPE_EDIT, GUI_ID_EDIT2, "%dppm", 0, CO2_RATE},
        {MSHOW, TYPE_EDIT, GUI_ID_EDIT3, "%dppm", 0, NH3_RATE},
        {MSHOW, TYPE_EDIT, GUI_ID_EDIT4, "%dppm", 0, H2S_RATE},
        {MSHOW, TYPE_EDIT, GUI_ID_EDIT5, "%dPa", 0, VAC_RATE},
        {MHIDE, TYPE_EDIT, GUI_ID_EDIT6, "%.2fm/s", 0, SPEED_RATE},

//        {MSHOW, TYPE_TEXT, GUI_ID_TEXT13, "校准", 0, 1},
        {MHIDE, TYPE_TEXT, GUI_ID_TEXT14, "校准:", 0, 1},

        {MHIDE, TYPE_CHECK, GUI_ID_CHECK0, "室内", 0, 1},
        {MHIDE, TYPE_CHECK, GUI_ID_CHECK1, "启用", 0, 1},
        {MHIDE, TYPE_CHECK, GUI_ID_CHECK2, "启用", 0, 1},
        {MHIDE, TYPE_CHECK, GUI_ID_CHECK3, "启用", 0, 1},
        {MHIDE, TYPE_CHECK, GUI_ID_CHECK4, "启用", 0, 1},
        {MHIDE, TYPE_CHECK, GUI_ID_CHECK5, "启用", 0, 1},
        {MHIDE, TYPE_CHECK, GUI_ID_CHECK6, "启用", 0, 1},

    },
};
static void Set_OptValue(WM_HWIN hWin, U8 sid)
{
    U8 i = 0;

// 	增加是否启用
    for (i = 0; i < 7; i++) { //temp
        CHECKBOX_SetState(WM_GetDialogItem(hWin, (GUI_ID_CHECK0 + i)), dev_inf.sensor_adj.TEMP_EN[i]);
    }

    for (i = 0; i < 6; i++) { //sensor
        DROPDOWN_SetSel(WM_GetDialogItem(hWin, (GUI_ID_DROPDOWN10 + i)), dev_inf.sensor_adj.SENSOR_EN[i]);
    }

    if (sid == 0) {
        for (i = 0; i < TEMP_NUM_MAX; i++) {
            sprintf((char *)Mchg_Menu[sid][i].text, "%s%d:%%.1f%s", GUI_LANG_GetTextEx(Lang_Temp, Language_Bit), i + 1, GUI_LANG_GetTextEx(Lang_oC, Language_Bit));
        }

        for (i = TEMP_NUM_MAX * 2 + 1; i < (TEMP_NUM_MAX * 3); i++) {
                strcpy((char *) Mchg_Menu[sid][i].text, GUI_LANG_GetTextEx(Lang_Indoor, Language_Bit));
        }

        strcpy((char *)Mchg_Menu[sid][TEMP_NUM_MAX * 3].text, GUI_LANG_GetTextEx(Lang_Outdoor, Language_Bit));
        strcpy((char *)Mchg_Menu[sid][TEMP_NUM_MAX * 2].text, GUI_LANG_GetTextEx(Lang_ADJ, Language_Bit));
    } else {
        for (i = TEMP_NUM_MAX * 3; i < CHG_MENU_MAX; i++) {
            strcpy((char *)Mchg_Menu[sid][i].text, GUI_LANG_GetTextEx(Lang_ADJ, Language_Bit));
        }

        for (i = TEMP_NUM_MAX * 2; i < (TEMP_NUM_MAX * 3); i++) {
            strcpy((char *)Mchg_Menu[sid][i].text, GUI_LANG_GetTextEx(Lang_Enable, Language_Bit));
        }
    }

    Button_CurShow(hWin, sid, 2);
}
static void Menu_Value_Init(WM_HMEM hWin, U8 sid)
{
    U8 i = 0;
    if (sid == 0) {
        for (i = 0; i < 7; i++) {
            Mchg_Menu[sid][i].value = sta_in.sensor_real.TEMP[i];
        }

        for (i = 7; i < 14; i++) {
            Mchg_Menu[sid][i].value = dev_inf.sensor_adj.TEMP[i - 7];
        }
    } else {
        for (i = 0; i < 6; i++) {
            Mchg_Menu[sid][i].value = sta_in.sensor_real.SENSOR[i];
        }

        for (i = 7; i < 13; i++) {
            Mchg_Menu[sid][i].value = dev_inf.sensor_adj.SENSOR[i - 7];
        }
    }
}

static void MChange_Menu_Array(WM_HMEM hWin, U8 sid)
{
    U8 i = 0;

    if (sid > CHG_NUM_MAX)
        return;

    Menu_Value_Init(hWin, sid);

    if (sid == 0) {
        for (i = 0; i < 6; i++) { //sensor
            WM_HideWindow(WM_GetDialogItem(hWin, (GUI_ID_DROPDOWN10 + i)));
            WM_EnableWindow(WM_GetDialogItem(hWin, (GUI_ID_EDIT0 + i)));
        }
    } else {
        for (i = 0; i < 6; i++) { //sensor
            WM_ShowWindow(WM_GetDialogItem(hWin, (GUI_ID_DROPDOWN10 + i)));
            strcpy(&Mchg_Menu[sid][i].text[4], sensor_unit[dev_inf.sensor_adj.SENSOR_EN[i]]);
            strcpy(&Mchg_Menu[sid][7 + i].text[0], sensor_unit[dev_inf.sensor_adj.SENSOR_EN[i]]);
            Mchg_Menu[sid][i].scale = sensor_scale[dev_inf.sensor_adj.SENSOR_EN[i]];
            Mchg_Menu[sid][7 + i].scale = sensor_scale[dev_inf.sensor_adj.SENSOR_EN[i]];

            if (dev_inf.sensor_adj.SENSOR_EN[i] == SENSOR_TYPE_DIS) {
                WM_DisableWindow(WM_GetDialogItem(hWin, (GUI_ID_EDIT0 + i)));
            } else {
                WM_EnableWindow(WM_GetDialogItem(hWin, (GUI_ID_EDIT0 + i)));
            }
        }
    }

    for (i = 0; i < CHG_MENU_MAX; i++) {
        if (Mchg_Menu[sid][i].isshow == MHIDE) {
            WM_HideWindow(WM_GetDialogItem(hWin, Mchg_Menu[sid][i].hMem));
            continue;
        }

        WM_ShowWindow(WM_GetDialogItem(hWin, Mchg_Menu[sid][i].hMem));

        if (Mchg_Menu[sid][i].scale == 1) { //原数据
            sprintf(show_buf, (const char *) Mchg_Menu[sid][i].text, Mchg_Menu[sid][i].value);
        } else {
            sprintf(show_buf, (const char *) Mchg_Menu[sid][i].text, (float)(Mchg_Menu[sid][i].value * 1.0f / Mchg_Menu[sid][i].scale));
        }

        Set_Text[Mchg_Menu[sid][i].type](WM_GetDialogItem(hWin, Mchg_Menu[sid][i].hMem), show_buf);
    }
}


static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect,    "",               0,                       0,  0,  800, 480, 0, 0},
    { TEXT_CreateIndirect,      "设备管理-传感器",          GUI_ID_TEXT0,           250, 4, 300, 42, 0, 0},
    { TEXT_CreateIndirect,      "温度1:26.5度",     GUI_ID_TEXT1,            129 + XPos_Collec, 66, 170, 36, 0, 0},
    { TEXT_CreateIndirect,      "温度2:26.5度",    	GUI_ID_TEXT2,            129 + XPos_Collec, 113, 170, 36, 0, 0},
    { TEXT_CreateIndirect,    	"温度3:26.5度",    	GUI_ID_TEXT3,            129 + XPos_Collec, 160, 170, 36, 0, 0},
    { TEXT_CreateIndirect,      "温度4:26.5度",    	GUI_ID_TEXT4,            129 + XPos_Collec, 207, 170, 36, 0, 0},
    { TEXT_CreateIndirect,     	"温度5:26.5度",   	GUI_ID_TEXT5,               129 + XPos_Collec, 254, 170, 36, 0, 0},
    { TEXT_CreateIndirect,      "温度6:26.5度",  	GUI_ID_TEXT6,               129 + XPos_Collec, 301, 170, 36, 0, 0},
    { TEXT_CreateIndirect,      "温度7:26.5度",  	GUI_ID_TEXT7,              129 + XPos_Collec, 348, 170, 36, 0, 0},

    { TEXT_CreateIndirect,      "校准:",      	GUI_ID_TEXT8,            309 + XPos_Collec, 66, 60, 36, 0, 0},
    { TEXT_CreateIndirect,      "校准:",    	GUI_ID_TEXT9,            309 + XPos_Collec, 113, 60, 36, 0, 0},
    { TEXT_CreateIndirect,  	"校准:",    	GUI_ID_TEXT10,            309 + XPos_Collec, 160, 60, 36, 0, 0},
    { TEXT_CreateIndirect,      "校准:",    	GUI_ID_TEXT11,            309 + XPos_Collec, 207, 60, 36, 0, 0},
    { TEXT_CreateIndirect,    	"校准:",   		GUI_ID_TEXT12,               309 + XPos_Collec, 254, 60, 36, 0, 0},
    { TEXT_CreateIndirect,    	"校准:",  		GUI_ID_TEXT13,               309 + XPos_Collec, 301, 60, 36, 0, 0},
    { TEXT_CreateIndirect,      "校准:",      	GUI_ID_TEXT14,              309 + XPos_Collec, 348, 60, 36, 0, 0},

    { DROPDOWN_CreateIndirect,  "",          	GUI_ID_DROPDOWN10,       523 + XPos_Collec, 66 + 3, 140, 170, 0, 0},
    { CHECKBOX_CreateIndirect,  "室内",              GUI_ID_CHECK0,           523 + XPos_Collec, 66 + 3, 123, CHECK_SIZE, 0, 0},
    { EDIT_CreateIndirect,       NULL,            GUI_ID_EDIT0,            375 + XPos_Collec, 66, 127, 36, 0, 0},

    { DROPDOWN_CreateIndirect,  "",          	GUI_ID_DROPDOWN11,       523 + XPos_Collec, 113 + 3, 140, 170, 0, 0},
    { CHECKBOX_CreateIndirect,  "室内",            GUI_ID_CHECK1,              523 + XPos_Collec, 113 + 3, 123, CHECK_SIZE, 0, 0},
    { EDIT_CreateIndirect,       NULL,           GUI_ID_EDIT1,            375 + XPos_Collec, 113, 127, 36, 0, 0},

    { DROPDOWN_CreateIndirect,  "",          	GUI_ID_DROPDOWN12,       523 + XPos_Collec, 160 + 3, 140, 170, 0, 0},
    { CHECKBOX_CreateIndirect,  "室内",          GUI_ID_CHECK2,               523 + XPos_Collec, 160 + 3, 123, CHECK_SIZE, 0, 0},
    { EDIT_CreateIndirect,       NULL,           GUI_ID_EDIT2,               375 + XPos_Collec, 160, 127, 36, 0, 0},

    { DROPDOWN_CreateIndirect,  "",          	GUI_ID_DROPDOWN13,       523 + XPos_Collec, 207 + 3, 140, 170, 0, 0},
    { CHECKBOX_CreateIndirect,  "室内",              GUI_ID_CHECK3,           523 + XPos_Collec, 207 + 3, 123, CHECK_SIZE, 0, 0},
    { EDIT_CreateIndirect,       NULL,            GUI_ID_EDIT3,            375 + XPos_Collec, 207, 127, 36, 0, 0},

    { DROPDOWN_CreateIndirect,  "",          	GUI_ID_DROPDOWN14,       523 + XPos_Collec, 254 + 3, 140, 170, 0, 0},
    { CHECKBOX_CreateIndirect,  "室内",          GUI_ID_CHECK4,              523 + XPos_Collec, 254 + 3, 123, CHECK_SIZE, 0, 0},
    { EDIT_CreateIndirect,       NULL,         GUI_ID_EDIT4,            375 + XPos_Collec, 254, 127, 36, 0, 0},

    { DROPDOWN_CreateIndirect,  "",          	GUI_ID_DROPDOWN15,       523 + XPos_Collec, 301 + 3, 140, 170, 0, 0},
    { CHECKBOX_CreateIndirect,  "室内",         GUI_ID_CHECK5,                523 + XPos_Collec, 301 + 3, 123, CHECK_SIZE, 0, 0},
    { EDIT_CreateIndirect,       NULL,         GUI_ID_EDIT5,           375 + XPos_Collec, 301, 127, 36, 0, 0},

    { CHECKBOX_CreateIndirect,  "室内",          GUI_ID_CHECK6,                523 + XPos_Collec, 348 + 3, 123, CHECK_SIZE, 0, 0},
    { EDIT_CreateIndirect,       NULL,          GUI_ID_EDIT6,           375 + XPos_Collec, 348, 127, 36, 0, 0},

    { BUTTON_CreateIndirect,    "上一页",          GUI_ID_BUTTON0,          MENU_BACK_X1, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
    { BUTTON_CreateIndirect,    "下一页",          GUI_ID_BUTTON1,          MENU_BACK_X2, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
    { BUTTON_CreateIndirect,    "返回",             GUI_ID_BUTTON2,         MENU_BACK_X3, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},

    { BUTTON_CreateIndirect,    "",     GUI_ID_BUTTON4,           			660, 0, 90, 60, 0, 0},

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
    GUI_DrawRoundedRect(100, 55, 682, 396, 10);
    GUI_DrawRoundedRect(96, 52, 686, 399, 10);
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
	
    for (MDev_ID = GUI_ID_BUTTON4; MDev_ID <= GUI_ID_BUTTON4; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, set_cbButton);
    }
	
    for (MDev_ID = GUI_ID_EDIT0; MDev_ID <= GUI_ID_EDIT6; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Edit_SetHZ24(hItem);
    }

    for (MDev_ID = GUI_ID_CHECK0; MDev_ID <= GUI_ID_CHECK6; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Check_SetHZ24(hItem);
        Login_Show(hItem, LOGIN_ADMIN);
    }

    for (MDev_ID = GUI_ID_DROPDOWN10; MDev_ID <= GUI_ID_DROPDOWN15; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Dropdown_SetHZ24(hItem);

        DROPDOWN_AddString(hItem, GUI_LANG_GetTextEx(Lang_Disable, Language_Bit));
        DROPDOWN_AddString(hItem, GUI_LANG_GetTextEx(Lang_Temp, Language_Bit));
        DROPDOWN_AddString(hItem, GUI_LANG_GetTextEx(Lang_HUM, Language_Bit));
        DROPDOWN_AddString(hItem, "CO2");
        DROPDOWN_AddString(hItem, "NH3");
        DROPDOWN_AddString(hItem, "H2S");
        DROPDOWN_AddString(hItem, GUI_LANG_GetTextEx(Lang_NP, Language_Bit));
        DROPDOWN_AddString(hItem, GUI_LANG_GetTextEx(Lang_Wind, Language_Bit));
        DROPDOWN_AddString(hItem, GUI_LANG_GetTextEx(Lang_Out_HUM, Language_Bit));
        if(MDev_ID == GUI_ID_DROPDOWN15)
        {
            DROPDOWN_SetUpMode(hItem, 1);
        }

        Login_Show(hItem, LOGIN_ADMIN);
    }

    Text_SetHZ24(WM_GetDialogItem(hWin, GUI_ID_TEXT0));
    TEXT_SetTextColor(WM_GetDialogItem(hWin, GUI_ID_TEXT0), GUI_WHITE);

    sprintf(show_buf, "%s-%s", GUI_LANG_GetTextEx(Lang_Device, Language_Bit), GUI_LANG_GetTextEx(Lang_sensor, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT0), show_buf);

    for (MDev_ID = GUI_ID_TEXT1; MDev_ID <= GUI_ID_TEXT7; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24L(hItem);

    }

    for (MDev_ID = GUI_ID_TEXT8; MDev_ID <= GUI_ID_TEXT9; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24(hItem);
        TEXT_SetText(hItem, GUI_LANG_GetTextEx(Lang_ADJ, Language_Bit));
    }

    for (MDev_ID = GUI_ID_TEXT10; MDev_ID <= GUI_ID_TEXT14; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24(hItem);
        TEXT_SetText(hItem, GUI_LANG_GetTextEx(Lang_ADJ, Language_Bit));
    }

    hTimer = WM_CreateTimer(WM_GetClientWindow(hWin), 0, 500, 0);

    Set_OptValue(hWin, select_id);
    MChange_Menu_Array(hWin, select_id);
}

static U8 temp_id = 0;
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

    dev_inf.sensor_adj.TEMP[temp_id] = stemp;

    MChange_Menu_Array(Current_HWIN, select_id);

}
static void Sensor_Get(void)
{
    int id = DROPDOWN_GetSel(WM_GetDialogItem(Current_HWIN, (GUI_ID_DROPDOWN10 + temp_id)));
    float ftemp = Get_Input_Value() * sensor_scale[id];
    short stemp = (short)ftemp;

    if (id == SENSOR_TYPE_DIS)
        return;

    if (stemp > sensor_max[id]) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = sensor_max[id];
    }

    if (stemp < sensor_min[id]) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = sensor_min[id];
    }

    dev_inf.sensor_adj.SENSOR[temp_id] = stemp;
    MChange_Menu_Array(Current_HWIN, select_id);

}

static void Sys_Circle_EN(WM_HWIN hWin)
{
    U8 i = 0;

    for (i = 0; i < TEMP_NUM_MAX; i++) { //temp
        if (dev_inf.sensor_adj.TEMP_EN[i] == STA_OFF)
            dev_inf.circle.temp_sta[i] = STA_OFF;
    }

    if (Config_Save(SCIRCLE_ID) < 0) {
        MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_SAVE_ER));
    }

}

void Arabic_Text_Change(void){
	u8 i = 0;
	for (i = 7; i < 14; i++) {
		if (basic_inf.language == LANGUAGE_Arabic){
			sprintf(Mchg_Menu[0][i].text, "%s%%.1f", GUI_LANG_GetTextEx(Lang_oC, Language_Bit));
		} else {
        	sprintf(Mchg_Menu[0][i].text, "%%.1f%s", GUI_LANG_GetTextEx(Lang_oC, Language_Bit));
		}
    }
	
	if (basic_inf.language == LANGUAGE_Arabic){
		sprintf(Mchg_Menu[1][7].text, "%s%%.1f", GUI_LANG_GetTextEx(Lang_oC, Language_Bit));
	} else {
    	sprintf(Mchg_Menu[1][7].text, "%%.1f%s", GUI_LANG_GetTextEx(Lang_oC, Language_Bit));
	}
}

static void Get_OptValue(WM_HWIN hWin, U8 sid)
{
    U8 i = 0;

    if (sid == 0) {
        for (i = 0; i < 7; i++) { //temp
            dev_inf.sensor_adj.TEMP_EN[i] = CHECKBOX_GetState(WM_GetDialogItem(hWin, (GUI_ID_CHECK0 + i)));
        }
    } else {
        for (i = 0; i < 6; i++) { //sensor
            dev_inf.sensor_adj.SENSOR_EN[i] = DROPDOWN_GetSel(WM_GetDialogItem(hWin, (GUI_ID_DROPDOWN10 + i)));
        }
    }
}
/*********************************************************************
*
*       Dialog callback routine
*/
static void _cbCallback(WM_MESSAGE * pMsg)
{
    int NCode, Id, i;
    WM_HWIN hWin = pMsg->hWin;

    switch (pMsg->MsgId) {
    case WM_PAINT:
        PaintDialog(pMsg);
        break;

    case WM_TIMER:
        Menu_Value_Init(hWin, select_id);

        for (i = 0; i < CHG_MENU_MAX; i++) {
            if (Mchg_Menu[select_id][i].isshow == MHIDE) {
                continue;
            }

            if (Mchg_Menu[select_id][i].scale == 1) { //原数据
                sprintf(show_buf, (const char *) Mchg_Menu[select_id][i].text, Mchg_Menu[select_id][i].value);
            } else {
                sprintf(show_buf, (const char *) Mchg_Menu[select_id][i].text, (float)(Mchg_Menu[select_id][i].value * 1.0f / Mchg_Menu[select_id][i].scale));
            }

            Set_Text[Mchg_Menu[select_id][i].type](WM_GetDialogItem(hWin, Mchg_Menu[select_id][i].hMem), show_buf);
        }

        WM_RestartTimer(hTimer, 500);
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
			
        case GUI_ID_BUTTON4:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                MDev_SetSensor_Set_Task(hWin);
            }

            break;
			
        case GUI_ID_DROPDOWN10:
        case GUI_ID_DROPDOWN11:
        case GUI_ID_DROPDOWN12:
        case GUI_ID_DROPDOWN13:
        case GUI_ID_DROPDOWN14:
        case GUI_ID_DROPDOWN15:
            if (NCode == WM_NOTIFICATION_SEL_CHANGED) {
                Get_OptValue(hWin, select_id);
                MChange_Menu_Array(hWin, select_id);
            }

            break;


        case GUI_ID_BUTTON0:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                if (select_id == 1) {
                    Get_OptValue(hWin, select_id);
                    select_id = 0;
                    Set_OptValue(hWin, select_id);
                    MChange_Menu_Array(hWin, select_id);
                }
            }

            break;

        case GUI_ID_BUTTON1:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                if (select_id == 0) {
                    Get_OptValue(hWin, select_id);
                    select_id = 1;
                    Set_OptValue(hWin, select_id);
                    MChange_Menu_Array(hWin, select_id);
                }
            }

            break;

        case GUI_ID_BUTTON2:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Get_OptValue(hWin, select_id);
                Sys_Circle_EN(WM_GetParent(hWin));

                if (Config_Save(SSENSOR_ID) < 0) {
                    MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_SAVE_ER));
                }

				AIN_WIN_CHK(0);
                Current_HWIN = WM_GetParent(hWin);
                GUI_EndDialog(hWin, 0);
            }

            break;

            //EDIT
        case GUI_ID_EDIT0:
        case GUI_ID_EDIT1:
        case GUI_ID_EDIT2:
        case GUI_ID_EDIT3:
        case GUI_ID_EDIT4:
        case GUI_ID_EDIT5:
        case GUI_ID_EDIT6:
            if (NCode == WM_NOTIFICATION_RELEASED) {

                if (select_id == 0) {
                    temp_id = (Id - GUI_ID_EDIT0);
                    Set_Output_Callback(Temp_Get);
                    Set_Input_Len(4);
                } else {
                    temp_id = (Id - GUI_ID_EDIT0);
                    Set_Output_Callback(Sensor_Get);


                    switch (dev_inf.sensor_adj.SENSOR_EN[temp_id]) {
                    case SENSOR_TYPE_TEMP: {
                        Set_Input_Len(4);
                    }
                    break;
					
					case SENSOR_TYPE_OUTHUM:
                    case SENSOR_TYPE_HUM: {
                        Set_Input_Len(2);
                    }
                    break;

                    case SENSOR_TYPE_CO2: {
                        Set_Input_Len(4);
                    }
                    break;

                    default:
                    case SENSOR_TYPE_NH3: {
                        Set_Input_Len(2);
                    }
                    break;

                    case SENSOR_TYPE_H2S: {
                        Set_Input_Len(2);
                    }
                    break;

                    case SENSOR_TYPE_VAC: {
                        Set_Input_Len(2);
                    }
                    break;

                    case SENSOR_TYPE_SPEED: {
                        Set_Input_Len(5);
                    }
                    break;
                    }
                }

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
WM_HWIN MDev_SetSensor;
void MDev_SetSensor_Task(WM_HWIN hParent)
{   
    select_id = 0;
    sprintf(sensor_unit[1], "%%.1f%s", GUI_LANG_GetTextEx(Lang_oC, Language_Bit));
	Arabic_Text_Change();
    MDev_SetSensor = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    Current_HWIN = MDev_SetSensor;
}

