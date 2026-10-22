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
#include "bsp.h"


/*********************************************************************
*
*       static data
*
**********************************************************************
*/


static U8 JDQ_Select = 0;

static TCHG_MENU Mchg_Menu[] = {

    {MSHOW, TYPE_TEXT, GUI_ID_TEXT1, "继电器%d:", 1, 1},
    {MSHOW, TYPE_TEXT, GUI_ID_TEXT6, "继电器%d:", 2, 1},

    {MSHOW, TYPE_EDIT, GUI_ID_EDIT0, "%.1f", 0, FAN_RATE},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT1, "%d", 0, 1},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT2, "%.1f", 0, CUR_RATE},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT3, "%.1f", 0, FAN_RATE},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT4, "%d", 0, 1},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT5, "%.1f", 0, CUR_RATE},
    {MEXIT, 0, 0, "", 0, 0},

};


static void Menu_Show(WM_HWIN hWin, U8 type1, U8 type2)
{
    U8 i = 0;

    if (type1 == STA_OFF) {
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON3));
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN0));

        for (i = 0; i < 3; i++) {
            WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT0 + i));
        }
    } else if (type1 == STA_ON) {
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON3));
        WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN0));

        for (i = 0; i < 3; i++) {
            WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT0 + i));
        }
    } else {
        WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON3));
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN0));

        for (i = 0; i < 3; i++) {
            WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT0 + i));
        }
    }

    if (type2 == STA_OFF) {
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON4));
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN1));

        for (i = 0; i < 3; i++) {
            WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT3 + i));
        }
    } else if (type2 == STA_ON) {
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON4));
        WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN1));

        for (i = 0; i < 3; i++) {
            WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT3 + i));
        }
    } else {
        WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON4));
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN1));

        for (i = 0; i < 3; i++) {
            WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT3 + i));
        }
    }

    if (type1 != STA_ON && type2 != STA_ON) {
        for (i = 0; i < 4; i++) {
            WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT2 + i));
        }
    } else {
        for (i = 0; i < 4; i++) {
            WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT2 + i));
        }
    }
}
static void Set_OptValue(WM_HWIN hWin, U8 sid)
{
    U8 type1 = STA_ON, type2 = STA_ON, i = 0;
    DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN0 ), dev_inf.JDQ[2 * sid].type);

    if (2 * sid + 1 < JDQ_NUM_MAX) {
        DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN1 ), dev_inf.JDQ[2 * sid + 1].type);
    }

    Button_CurShow(hWin, sid, 10);

    for (i = 0; i < 3; i++) {
        DROPDOWN_DeleteItem(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2), 1);
        DROPDOWN_DeleteItem(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3), 1);
    }

    sprintf(show_buf, "%s%s", GUI_LANG_GetTextEx(Lang_StaFAN, Language_Bit), GUI_LANG_GetTextEx(Lang_Fan, Language_Bit));
    DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2), show_buf);

    if (2 * sid + 1 < JDQ_NUM_MAX) {
        sprintf(show_buf, "%s%s", GUI_LANG_GetTextEx(Lang_StaFAN, Language_Bit), GUI_LANG_GetTextEx(Lang_Fan, Language_Bit));
        DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3), show_buf);
    }

    switch (sid) {
    case 0:
    case 1:
        DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2 ), dev_inf.JDQ[2 * sid].enable);
        DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3 ), dev_inf.JDQ[2 * sid + 1].enable);
        Menu_Show(hWin, dev_inf.JDQ[2 * sid].enable, dev_inf.JDQ[2 * sid + 1].enable);
        break;

    case 2:

        sprintf(show_buf, "%s%s", GUI_LANG_GetTextEx(Lang_CurtWinDow, Language_Bit), GUI_LANG_GetTextEx(Lang_On, Language_Bit));
        DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2), show_buf);
        sprintf(show_buf, "%s%s", GUI_LANG_GetTextEx(Lang_CurtWinDow, Language_Bit), GUI_LANG_GetTextEx(Lang_Off, Language_Bit));
        DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3), show_buf);

        if (dev_inf.WIN.detail[WIN1_ID].enable == STA_ON) {
            type1 = STA_ON + 1;
            DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2 ), 2);
        } else {
            type1 = dev_inf.JDQ[2 * sid].enable;
            DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2 ), dev_inf.JDQ[2 * sid].enable);
        }

        if (dev_inf.WIN.detail[WIN1_ID].enable == STA_ON) {
            type2 = STA_ON + 1;
            DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3 ), 2);
        } else {
            type2 = dev_inf.JDQ[2 * sid + 1].enable;
            DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3 ), dev_inf.JDQ[2 * sid + 1].enable);
        }

        Menu_Show(hWin, type1, type2);
        break;

    case 3:

        sprintf(show_buf, "%s%s", GUI_LANG_GetTextEx(Lang_CurtWinDow, Language_Bit), GUI_LANG_GetTextEx(Lang_On, Language_Bit));
        DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2), show_buf);
        sprintf(show_buf, "%s%s", GUI_LANG_GetTextEx(Lang_CurtWinDow, Language_Bit), GUI_LANG_GetTextEx(Lang_Off, Language_Bit));
        DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3), show_buf);

        if (dev_inf.WIN.detail[WIN2_ID].enable == STA_ON) {
            type1 = STA_ON + 1;
            DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2 ), 2);
        } else {
            type1 = dev_inf.JDQ[2 * sid].enable;
            DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2 ), dev_inf.JDQ[2 * sid].enable);
        }

        if (dev_inf.WIN.detail[WIN2_ID].enable == STA_ON) {
            type2 = STA_ON + 1;
            DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3 ), 2);
        } else {
            type2 = dev_inf.JDQ[2 * sid + 1].enable;
            DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3 ), dev_inf.JDQ[2 * sid + 1].enable);
        }

        Menu_Show(hWin, type1, type2);
        break;

    case 4:

        sprintf(show_buf, "%s1", GUI_LANG_GetTextEx(Lang_Wet_Curtain, Language_Bit));
        DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2), show_buf);
        sprintf(show_buf, "%s2", GUI_LANG_GetTextEx(Lang_Wet_Curtain, Language_Bit));
        DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3), show_buf);
        DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3), GUI_LANG_GetTextEx(Lang_Spray, Language_Bit));

        if (dev_inf.wet_en[0] == STA_ON) {
            type1 = STA_ON + 1;
            DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2 ), 2);
        } else {
            type1 = dev_inf.JDQ[2 * sid].enable;
            DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2 ), dev_inf.JDQ[2 * sid].enable);
        }

        if (dev_inf.wet_en[1] == STA_ON) {
            type2 = STA_ON + 1;
            DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3 ), 2);
        } else if (dev_inf.spray_en != SPRAY_MODE_OFF) {
            type2 = STA_ON + 1;
            DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3 ), 3);
        } else {
            type2 = dev_inf.JDQ[2 * sid + 1].enable;
            DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3 ), dev_inf.JDQ[2 * sid + 1].enable);
        }

        Menu_Show(hWin, type1, type2);
        break;

    case 5:

        DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2), GUI_LANG_GetTextEx(Lang_Circulating_Fan, Language_Bit)); // 环流风机
        if (ext_inf.timer[TR_TYPE_TIMER1].enable == STA_ON) {
            sprintf(show_buf, "%s%d", GUI_LANG_GetTextEx(Lang_timer_type[ext_inf.timer[TR_TYPE_TIMER1].type], Language_Bit), 1);
            DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2), show_buf);
        } else {
            sprintf(show_buf, "%s1", GUI_LANG_GetTextEx(Lang_Timer, Language_Bit)); // 定时器1
            DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2), show_buf);
        }

        DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3), GUI_LANG_GetTextEx(Lang_Tswitch, Language_Bit)); // 温控开关
        if (ext_inf.timer[TR_TYPE_TIMER2].enable == STA_ON) {
            sprintf(show_buf, "%s%d", GUI_LANG_GetTextEx(Lang_timer_type[ext_inf.timer[TR_TYPE_TIMER2].type], Language_Bit), 2);
            DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3), show_buf);
        } else {
            sprintf(show_buf, "%s2", GUI_LANG_GetTextEx(Lang_Timer, Language_Bit)); // 定时器2
            DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3), show_buf);
        }

        if (dev_inf.circle.enable == STA_ON) {
            type1 = STA_ON + 1;
            DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2 ), 2);
        } else if (ext_inf.timer[TR_TYPE_TIMER1].enable == STA_ON) {
            type1 = STA_ON + 1;
            DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2 ), 3);
        } else {
            type1 = dev_inf.JDQ[2 * sid].enable;
            DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2 ), dev_inf.JDQ[2 * sid].enable);
        }

        if (basic_inf.tempc_en.enable == STA_ON) {
            type2 = STA_ON + 1;
            DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3 ), 2);
        } else if (ext_inf.timer[TR_TYPE_TIMER2].enable == STA_ON) {
            type2 = STA_ON + 1;
            DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3 ), 3);
        } else {
            type2 = dev_inf.JDQ[2 * sid + 1].enable;
            DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3 ), dev_inf.JDQ[2 * sid + 1].enable);
        }

        Menu_Show(hWin, type1, type2);
        break;

    case 6:

        sprintf(show_buf, "%s1", GUI_LANG_GetTextEx(Lang_Heat, Language_Bit));
        DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2), show_buf);

        if (ext_inf.timer[TR_TYPE_TIMER3].enable == STA_ON) {
            sprintf(show_buf, "%s%d", GUI_LANG_GetTextEx(Lang_timer_type[ext_inf.timer[TR_TYPE_TIMER3].type], Language_Bit), 3);
            DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2), show_buf);
        } else {
            sprintf(show_buf, "%s3", GUI_LANG_GetTextEx(Lang_Timer, Language_Bit));
            DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2), show_buf);
        }
        sprintf(show_buf, "%s2", GUI_LANG_GetTextEx(Lang_Heat, Language_Bit));
        DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3), show_buf);

        if (ext_inf.timer[TR_TYPE_TIMER4].enable == STA_ON) {
            sprintf(show_buf, "%s%d", GUI_LANG_GetTextEx(Lang_timer_type[ext_inf.timer[TR_TYPE_TIMER4].type], Language_Bit), 4);
            DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3), show_buf);
        } else {
            sprintf(show_buf, "%s4", GUI_LANG_GetTextEx(Lang_Timer, Language_Bit));
            DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3), show_buf);
        }

        if (basic_inf.heat_en[0] == STA_ON) {
            type1 = STA_ON + 1;
            DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2 ), 2);
        } else if (ext_inf.timer[TR_TYPE_TIMER3].enable == STA_ON) {
            type1 = STA_ON + 1;
            DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2 ), 3);
        } else {
            type1 = dev_inf.JDQ[2 * sid].enable;
            DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2 ), dev_inf.JDQ[2 * sid].enable);
        }

        if (basic_inf.heat_en[1] == STA_ON) {
            type2 = STA_ON + 1;
            DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3 ), 2);
        } else if (ext_inf.timer[TR_TYPE_TIMER4].enable == STA_ON) {
            type2 = STA_ON + 1;
            DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3 ), 3);
        } else {
            type2 = dev_inf.JDQ[2 * sid + 1].enable;
            DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3 ), dev_inf.JDQ[2 * sid + 1].enable);
        }

        Menu_Show(hWin, type1, type2);
        break;

    case 7:

        sprintf(show_buf, "%s3", GUI_LANG_GetTextEx(Lang_Heat, Language_Bit));
        DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2), show_buf);
        sprintf(show_buf, "%s%s", GUI_LANG_GetTextEx(Lang_CurtWinDow, Language_Bit), GUI_LANG_GetTextEx(Lang_On, Language_Bit));
        DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3), show_buf);

        if (basic_inf.heat_en[2] == STA_ON) {
            type1 = STA_ON + 1;
            DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2 ), 2);
        } else {
            type1 = dev_inf.JDQ[2 * sid].enable;
            DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2 ), dev_inf.JDQ[2 * sid].enable);
        }

        if (dev_inf.WIN.detail[WIN3_ID].enable == STA_ON) {
            type2 = STA_ON + 1;
            DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3 ), 2);
        } else {
            type2 = dev_inf.JDQ[2 * sid + 1].enable;
            DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3 ), dev_inf.JDQ[2 * sid + 1].enable);
        }

        Menu_Show(hWin, type1, type2);
        break;

    case 8:

        sprintf(show_buf, "%s%s", GUI_LANG_GetTextEx(Lang_CurtWinDow, Language_Bit), GUI_LANG_GetTextEx(Lang_Off, Language_Bit));
        DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2), show_buf);
        sprintf(show_buf, "%s%s", GUI_LANG_GetTextEx(Lang_CurtWinDow, Language_Bit), GUI_LANG_GetTextEx(Lang_On, Language_Bit));
        DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3), show_buf);

        if (dev_inf.WIN.detail[WIN3_ID].enable == STA_ON) {
            type1 = STA_ON + 1;
            DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2 ), 2);
        } else {
            type1 = dev_inf.JDQ[2 * sid].enable;
            DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2 ), dev_inf.JDQ[2 * sid].enable);
        }

        if (dev_inf.WIN.detail[WIN4_ID].enable == STA_ON) {
            type2 = STA_ON + 1;
            DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3 ), 2);
        } else {
            type2 = dev_inf.JDQ[2 * sid + 1].enable;
            DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3 ), dev_inf.JDQ[2 * sid + 1].enable);
        }

        Menu_Show(hWin, type1, type2);
        break;

    case 9:

        sprintf(show_buf, "%s%s", GUI_LANG_GetTextEx(Lang_CurtWinDow, Language_Bit), GUI_LANG_GetTextEx(Lang_Off, Language_Bit));
        DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2), show_buf);
        DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3), GUI_LANG_GetTextEx(Lang_ALA, Language_Bit));

        if (dev_inf.WIN.detail[WIN4_ID].enable == STA_ON) {
            type1 = STA_ON + 1;
            DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2 ), 2);
        } else {
            type1 = dev_inf.JDQ[2 * sid].enable;
            DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2 ), dev_inf.JDQ[2 * sid].enable);
        }

        DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3 ), basic_inf.ala_en);
        type2 = basic_inf.ala_en;

        if (type2 == STA_ON)type2 = STA_ON + 1;

        Menu_Show(hWin, type1, type2);
        break;

    }
}

static void Menu_Value_Init(WM_HWIN hWin, U8 sid)
{
    U8 id = 0;

    strcpy(Mchg_Menu[id++].text, GUI_LANG_GetTextEx(Lang_JDQ_NUM, Language_Bit));
    strcpy(Mchg_Menu[id++].text, GUI_LANG_GetTextEx(Lang_JDQ_NUM, Language_Bit));
    id = 0;

    Mchg_Menu[id++].value = 2 * sid + 1;
    Mchg_Menu[id++].value = 2 * sid + 2;
    Mchg_Menu[id++].value = dev_inf.JDQ[2 * sid].max_air;
    Mchg_Menu[id++].value = dev_inf.JDQ[2 * sid].fan_num;
    Mchg_Menu[id++].value = dev_inf.JDQ[2 * sid].current;

    if (2 * sid + 1 >= JDQ_NUM_MAX)return;

    Mchg_Menu[id++].value = dev_inf.JDQ[2 * sid + 1].max_air;
    Mchg_Menu[id++].value = dev_inf.JDQ[2 * sid + 1].fan_num;
    Mchg_Menu[id++].value = dev_inf.JDQ[2 * sid + 1].current;
}




/*********************************************************************
*
*       Dialog resource
*
* This table conatins the info required to create the dialog.
* It has been created by ucGUIbuilder.
*/
#define XPos_Collec  (20)
#define YPos_Space    (45)
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect,    "",               0,                     0,  0,  800, 480, 0, 0},
    { TEXT_CreateIndirect,      "设备管理-继电器", GUI_ID_TEXT0,         250, 4, 300, 42, 0, 0},
    { TEXT_CreateIndirect,      "继电器1:",        GUI_ID_TEXT1,         50 + XPos_Collec, 80 + YPos_Space * 0, 130, 36, 0, 0},
    { TEXT_CreateIndirect,      "风机型号/寸:",    GUI_ID_TEXT2,         50 + XPos_Collec, 80 + YPos_Space * 1, 200, 36, 0, 0},
    { TEXT_CreateIndirect,      "最大风量/km3/h:", GUI_ID_TEXT3,   		 50 + XPos_Collec, 80 + YPos_Space * 2, 200, 36, 0, 0},

    { TEXT_CreateIndirect,      "风机个数:",     GUI_ID_TEXT4,           50 + XPos_Collec, 80 + YPos_Space * 3, 200, 36, 0, 0},
    { TEXT_CreateIndirect,		"额定电流/A:",	 GUI_ID_TEXT5,			 50 + XPos_Collec, 80 + YPos_Space * 4, 200, 36, 0, 0},

    { DROPDOWN_CreateIndirect,	 "18",			 GUI_ID_DROPDOWN2,		190 + XPos_Collec, 80 + YPos_Space * 0, 160, 146, 0, 0},
    { DROPDOWN_CreateIndirect,	 "18",			 GUI_ID_DROPDOWN0,		245 + XPos_Collec, 80 + YPos_Space * 1, 100, 130, 0, 0},
    { EDIT_CreateIndirect,		 "6.3",		   	 GUI_ID_EDIT0,			245 + XPos_Collec, 80 + YPos_Space * 2, 90, 36, 0, 0},
    { EDIT_CreateIndirect,		 "30",			 GUI_ID_EDIT1,		  	245 + XPos_Collec, 80 + YPos_Space * 3, 90, 36, 0, 0},
    { EDIT_CreateIndirect,		 "60",		 	 GUI_ID_EDIT2,			245 + XPos_Collec, 80 + YPos_Space * 4, 90, 36, 0, 0},
    { BUTTON_CreateIndirect,     "详细设置",     GUI_ID_BUTTON3,        210 + XPos_Collec, 80 + YPos_Space * 1, 120, 50, 0, 0},

    { TEXT_CreateIndirect,       "继电器2:",   GUI_ID_TEXT6,          380 + XPos_Collec, 80 + YPos_Space * 0, 130, 36, 0, 0},
    { DROPDOWN_CreateIndirect,	 "18",			 GUI_ID_DROPDOWN3,		520 + XPos_Collec, 80 + YPos_Space * 0, 160, 146, 0, 0},
    { DROPDOWN_CreateIndirect,	 "18",			 GUI_ID_DROPDOWN1,		510 + XPos_Collec, 80 + YPos_Space * 1, 100, 130, 0, 0},
    { EDIT_CreateIndirect,		 "6.3",		   	 GUI_ID_EDIT3,			510 + XPos_Collec, 80 + YPos_Space * 2, 90, 36, 0, 0},
    { EDIT_CreateIndirect,	     "30",			 GUI_ID_EDIT4,		    510 + XPos_Collec, 80 + YPos_Space * 3, 90, 36, 0, 0},
    { EDIT_CreateIndirect,		 "60",		 	 GUI_ID_EDIT5,			510 + XPos_Collec, 80 + YPos_Space * 4, 90, 36, 0, 0},
    { BUTTON_CreateIndirect,     "详细设置",     GUI_ID_BUTTON4,        540 + XPos_Collec, 80 + YPos_Space * 1, 120, 50, 0, 0},

    { BUTTON_CreateIndirect,    "上一页",          GUI_ID_BUTTON0,          MENU_BACK_X1, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
    { BUTTON_CreateIndirect,    "下一页",          GUI_ID_BUTTON1,          MENU_BACK_X2, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
    { BUTTON_CreateIndirect,    "返回",            GUI_ID_BUTTON2,         MENU_BACK_X3, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},

    { BUTTON_CreateIndirect,    "",     GUI_ID_BUTTON5,           			720, 0, 80, 60, 0, 0},
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
    GUI_DrawRoundedRect(50, 55, 750, 386, 10);
    GUI_DrawRoundedRect(46, 52, 754, 389, 10);

    //GUI_DrawLine(80,260,712,260);
    //GUI_DrawLine(80,264,712,264);
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


    for (MDev_ID = GUI_ID_BUTTON0; MDev_ID <= GUI_ID_BUTTON2; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, Cut_cbButton);
    }

    for (MDev_ID = GUI_ID_BUTTON3; MDev_ID <= GUI_ID_BUTTON4; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Button_SetHZ24(hItem);
    }

    for (MDev_ID = GUI_ID_BUTTON5; MDev_ID <= GUI_ID_BUTTON5; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, Detail_cbButton);
    }

    for (MDev_ID = GUI_ID_EDIT0; MDev_ID <= GUI_ID_EDIT5; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Edit_SetHZ24(hItem);
    }

    for (MDev_ID = GUI_ID_TEXT0; MDev_ID <= GUI_ID_TEXT6; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24L(hItem);

        if (MDev_ID == GUI_ID_TEXT0) {
            TEXT_SetTextAlign(hItem,  GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetTextColor(hItem, GUI_WHITE);
        }
    }

    for (MDev_ID = GUI_ID_DROPDOWN0; MDev_ID <= GUI_ID_DROPDOWN3; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Dropdown_SetHZ24(hItem);
        Login_Show(hItem, LOGIN_ADMIN);

        if (MDev_ID >= GUI_ID_DROPDOWN2) {
            continue;
        }

        for (i = 0; i < FAN_SIZE_NUM; i++) {
            DROPDOWN_AddString(hItem, fan_size[i]);
        }

    }

    sprintf(show_buf, "%s-%s", GUI_LANG_GetTextEx(Lang_Device, Language_Bit), GUI_LANG_GetTextEx(Lang_Relay, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT0), show_buf);                                         //	设备管理-继电器
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT2), GUI_LANG_GetTextEx(Lang_Fan_Model, Language_Bit)); // 风机型号/寸:
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT3), GUI_LANG_GetTextEx(Lang_Rated_Air, Language_Bit)); // 最大风量/km3/h:
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT4), GUI_LANG_GetTextEx(Lang_Num_fans, Language_Bit));//风机个数:
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT5), GUI_LANG_GetTextEx(Lang_Rated_I, Language_Bit));//额定电流/A:
    BUTTON_SetText(WM_GetDialogItem(hWin, GUI_ID_BUTTON3),  GUI_LANG_GetTextEx(Lang_Detail_Set, Language_Bit));//详细设置
    BUTTON_SetText(WM_GetDialogItem(hWin, GUI_ID_BUTTON4),  GUI_LANG_GetTextEx(Lang_Detail_Set, Language_Bit));//详细设置
    DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2),  GUI_LANG_GetTextEx(Lang_Disable, Language_Bit));//停用
    DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3),  GUI_LANG_GetTextEx(Lang_Disable, Language_Bit));//停用


    MChange_Menu(Menu_Value_Init, hWin, (PCHG_MENU )Mchg_Menu, JDQ_Select);
    Set_OptValue(hWin, JDQ_Select);
}


static void Get_OptValue(WM_HWIN hWin, U8 sid, int Id)
{
    switch (sid) {
    case 0:
    case 1:
        dev_inf.JDQ[2 * sid].enable = DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2));
        dev_inf.JDQ[2 * sid + 1].enable = DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3));
        break;

    case 2:
        if (Id == GUI_ID_DROPDOWN2) {
            if (DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2)) == 2 ) {
                dev_inf.WIN.detail[WIN1_ID].enable = STA_ON;
                dev_inf.JDQ[2 * sid].enable = STA_OFF;
                dev_inf.JDQ[2 * sid + 1].enable = STA_OFF;//下一个也关闭
            } else {
                dev_inf.WIN.detail[WIN1_ID].enable = STA_OFF;
                dev_inf.JDQ[2 * sid].enable = DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2));
            }
        } else if (Id == GUI_ID_DROPDOWN3) {

            if (DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3)) == 2) {
                dev_inf.WIN.detail[WIN1_ID].enable = STA_ON;
                dev_inf.JDQ[2 * sid + 1].enable = STA_OFF;
                dev_inf.JDQ[2 * sid].enable = STA_OFF;    //上一个也关闭
            } else {
                dev_inf.WIN.detail[WIN1_ID].enable = STA_OFF;
                dev_inf.JDQ[2 * sid + 1].enable = DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3));
            }
        }

        break;

    case 3:
        if (Id == GUI_ID_DROPDOWN2) {
            if (DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2)) == 2) {
                dev_inf.WIN.detail[WIN2_ID].enable = STA_ON;
                dev_inf.JDQ[2 * sid].enable = STA_OFF;
                dev_inf.JDQ[2 * sid + 1].enable = STA_OFF;//下一个也关闭
            } else {
                dev_inf.WIN.detail[WIN2_ID].enable = STA_OFF;
                dev_inf.JDQ[2 * sid].enable = DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2));
            }
        } else if (Id == GUI_ID_DROPDOWN3) {
            if (DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3)) == 2) {
                dev_inf.WIN.detail[WIN2_ID].enable = STA_ON;
                dev_inf.JDQ[2 * sid + 1].enable = STA_OFF;
                dev_inf.JDQ[2 * sid].enable = STA_OFF;    //上一个也关闭
            } else {
                dev_inf.WIN.detail[WIN2_ID].enable = STA_OFF;
                dev_inf.JDQ[2 * sid + 1].enable = DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3));
            }
        }

        break;

    case 4:
        if (DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2)) == 2) {
            dev_inf.wet_en[0] = STA_ON;
            dev_inf.JDQ[2 * sid].enable = STA_OFF;
        } else {
            dev_inf.wet_en[0] = STA_OFF;
            dev_inf.JDQ[2 * sid].enable = DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2));
        }

        if (DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3)) == 2) {
            dev_inf.wet_en[1] = STA_ON;
            dev_inf.spray_en = SPRAY_MODE_OFF;
            dev_inf.JDQ[2 * sid + 1].enable = STA_OFF;
        } else if (DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3)) == 3) {
            dev_inf.wet_en[1] = STA_OFF;

            if (dev_inf.spray_en == SPRAY_MODE_OFF) { //默认加湿
                dev_inf.spray_en = SPRAY_MODE_HUM;
            }

            dev_inf.JDQ[2 * sid + 1].enable = STA_OFF;
        } else {
            dev_inf.wet_en[1] = STA_OFF;
            dev_inf.spray_en = SPRAY_MODE_OFF;
            dev_inf.JDQ[2 * sid + 1].enable = DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3));
        }

        break;

    case 5:
        if (DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2)) == 2) {
            dev_inf.circle.enable = STA_ON;
            ext_inf.timer[TR_TYPE_TIMER1].enable = STA_OFF;
            dev_inf.JDQ[2 * sid].enable = STA_OFF;
        } else if (DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2)) == 3) {
            dev_inf.circle.enable = STA_OFF;
            ext_inf.timer[TR_TYPE_TIMER1].enable = STA_ON;
            dev_inf.JDQ[2 * sid].enable = STA_OFF;
        } else {
            dev_inf.circle.enable = STA_OFF;
            ext_inf.timer[TR_TYPE_TIMER1].enable = STA_OFF;
            dev_inf.JDQ[2 * sid].enable = DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2));
        }

        if (DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3)) == 2) {
            basic_inf.tempc_en.enable = STA_ON;
            ext_inf.timer[TR_TYPE_TIMER2].enable = STA_OFF;
            dev_inf.JDQ[2 * sid + 1].enable = STA_OFF;
        } else if (DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3)) == 3) {
            basic_inf.tempc_en.enable = STA_OFF;
            ext_inf.timer[TR_TYPE_TIMER2].enable = STA_ON;
            dev_inf.JDQ[2 * sid + 1].enable = STA_OFF;
        } else {
            basic_inf.tempc_en.enable = STA_OFF;
            ext_inf.timer[TR_TYPE_TIMER2].enable = STA_OFF;
            dev_inf.JDQ[2 * sid + 1].enable = DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3));
        }

        break;

    case 6:
        if (DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2)) == 2) {
            basic_inf.heat_en[0] = STA_ON;
            ext_inf.timer[TR_TYPE_TIMER3].enable = STA_OFF;
            dev_inf.JDQ[2 * sid].enable = STA_OFF;
        }  else if (DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2)) == 3) {
            basic_inf.heat_en[0] = STA_OFF;
            ext_inf.timer[TR_TYPE_TIMER3].enable = STA_ON;
            dev_inf.JDQ[2 * sid].enable = STA_OFF;
        } else {
            basic_inf.heat_en[0] = STA_OFF;
            ext_inf.timer[TR_TYPE_TIMER3].enable = STA_OFF;
            dev_inf.JDQ[2 * sid].enable = DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2));
        }

        if (DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3)) == 2) {
            basic_inf.heat_en[1] = STA_ON;
            ext_inf.timer[TR_TYPE_TIMER4].enable = STA_OFF;
            dev_inf.JDQ[2 * sid + 1].enable = STA_OFF;
        } else if (DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3)) == 3) {
            basic_inf.heat_en[1] = STA_OFF;
            ext_inf.timer[TR_TYPE_TIMER4].enable = STA_ON;
            dev_inf.JDQ[2 * sid + 1].enable = STA_OFF;
        } else {
            basic_inf.heat_en[1] = STA_OFF;
            ext_inf.timer[TR_TYPE_TIMER4].enable = STA_OFF;
            dev_inf.JDQ[2 * sid + 1].enable = DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3));
        }

        break;

    case 7:
        if (DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2)) == 2) {
            basic_inf.heat_en[2] = STA_ON;
            dev_inf.JDQ[2 * sid].enable = STA_OFF;
        } else {
            basic_inf.heat_en[2] = STA_OFF;
            dev_inf.JDQ[2 * sid].enable = DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2));
        }

        if (DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3)) == 2) {
            dev_inf.WIN.detail[WIN3_ID].enable = STA_ON;
            dev_inf.JDQ[2 * sid + 1].enable = STA_OFF;
            dev_inf.JDQ[2 * sid + 2].enable = STA_OFF;//下一个也关闭
        } else {
            dev_inf.WIN.detail[WIN3_ID].enable = STA_OFF;
            dev_inf.JDQ[2 * sid + 1].enable = DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3));
        }

        break;

    case 8:
        if (DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2)) == 2) {
            dev_inf.WIN.detail[WIN3_ID].enable = STA_ON;
            dev_inf.JDQ[2 * sid].enable = STA_OFF;
            dev_inf.JDQ[2 * sid - 1].enable = STA_OFF;//上一个也关闭
        } else {
            dev_inf.WIN.detail[WIN3_ID].enable = STA_OFF;
            dev_inf.JDQ[2 * sid].enable = DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2));
        }

        if (DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3)) == 2) {
            dev_inf.WIN.detail[WIN4_ID].enable = STA_ON;
            dev_inf.JDQ[2 * sid + 1].enable = STA_OFF;
            dev_inf.JDQ[2 * sid + 2].enable = STA_OFF;//下一个也关闭
        } else {
            dev_inf.WIN.detail[WIN4_ID].enable = STA_OFF;
            dev_inf.JDQ[2 * sid + 1].enable = DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3));
        }

        break;

    case 9:
        if (DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2)) == 2) {
            dev_inf.WIN.detail[WIN4_ID].enable = STA_ON;
            dev_inf.JDQ[2 * sid].enable = STA_OFF;
            dev_inf.JDQ[2 * sid - 1].enable = STA_OFF;//上一个也关闭
        } else {
            dev_inf.WIN.detail[WIN4_ID].enable = STA_OFF;
            dev_inf.JDQ[2 * sid].enable = DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2));
        }

        basic_inf.ala_en = DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3));
        break;

    }

    dev_inf.JDQ[2 * sid].type =  DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN0 ));

    if (2 * sid + 1 >= JDQ_NUM_MAX)return;

    dev_inf.JDQ[2 * sid + 1].type =  DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN1 ));
}


static U8 temp_id = 0;
static void Max_Fan_Get(void)
{
    float ftemp = Get_Input_Value() * FAN_RATE;
    short stemp = (short)ftemp;

//    if (stemp > TEMP_ADJMAX) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = TEMP_ADJMAX;
//    }

    if (stemp < 0) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 0;
    }

    dev_inf.JDQ[JDQ_Select * 2 + temp_id].max_air = stemp;
	Get_OptValue(Current_HWIN, JDQ_Select, NULL);
    MChange_Menu(Menu_Value_Init, Current_HWIN, (PCHG_MENU )Mchg_Menu, JDQ_Select);
    Set_OptValue(Current_HWIN, JDQ_Select);
}

static void Fan_Num_Get(void)
{
    float ftemp = Get_Input_Value();
    short stemp = (short)ftemp;

//    if (stemp > TEMP_ADJMAX) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = TEMP_ADJMAX;
//    }

    if (stemp <= 0) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 1;
    }

    dev_inf.JDQ[JDQ_Select * 2 + temp_id].fan_num = stemp;
	Get_OptValue(Current_HWIN, JDQ_Select, NULL);
    MChange_Menu(Menu_Value_Init, Current_HWIN, (PCHG_MENU )Mchg_Menu, JDQ_Select);
    Set_OptValue(Current_HWIN, JDQ_Select);
}

static void Fan_Cur_Get(void)
{
    float ftemp = Get_Input_Value() * CUR_RATE;
    short stemp = (short)ftemp;

    if (stemp > MAX_1BYTE) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = MAX_1BYTE;
    }

    if (stemp < 0) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 0;
    }

    dev_inf.JDQ[JDQ_Select * 2 + temp_id].current = stemp;
	Get_OptValue(Current_HWIN, JDQ_Select, NULL);
    MChange_Menu(Menu_Value_Init, Current_HWIN, (PCHG_MENU )Mchg_Menu, JDQ_Select);
    Set_OptValue(Current_HWIN, JDQ_Select);
}

static void Detailed_Set(WM_HWIN hWin, U8 sid)
{
    switch (sid) {
    case JDQ_WET:
        if (dev_inf.wet_en[0] == STA_ON) {
            MDev_Wet_Task(hWin, 0, STA_OFF);
        }

        break;

    case JDQ_SPRAY:
        if (dev_inf.wet_en[1] == STA_ON) {
            MDev_Wet_Task(hWin, 1, STA_OFF);
        } else if (dev_inf.spray_en != SPRAY_MODE_OFF) {
            MDev_Spray_Task(hWin);
        }

        break;

    case JDQ_CIR_FAN:
        if (dev_inf.circle.enable == STA_ON) {
            MDev_Circle_Task(hWin);
        } else if (ext_inf.timer[TR_TYPE_TIMER1].enable == STA_ON) {
            MDev_Timer_Task(hWin, TR_TYPE_TIMER1, STA_OFF);
        }

        break;

    case JDQ_TIMER2:
        if (basic_inf.tempc_en.enable == STA_ON) {
            MDev_Tempc_Task(hWin);
        } else if (ext_inf.timer[TR_TYPE_TIMER2].enable == STA_ON) {
            MDev_Timer_Task(hWin, TR_TYPE_TIMER2, STA_OFF);
        }

        break;

    case JDQ_HEAT1:
    case JDQ_HEAT2:
        if (basic_inf.heat_en[sid - JDQ_HEAT1] == STA_ON) {
            MDev_Heater_Task(hWin, sid - JDQ_HEAT1, STA_OFF);
        } else if (ext_inf.timer[TR_TYPE_TIMER3 + sid - JDQ_HEAT1].enable == STA_ON) {
            MDev_Timer_Task(hWin, TR_TYPE_TIMER3 + sid - JDQ_HEAT1, STA_OFF);
        }

        break;

    case JDQ_HEAT3:
        if (basic_inf.heat_en[sid - JDQ_HEAT1] == STA_ON) {
            MDev_Heater_Task(hWin, sid - JDQ_HEAT1, STA_OFF);
        }

        break;

    case JDQ_WIN1_UP:
    case JDQ_WIN1_DOWN:
        if (dev_inf.WIN.detail[WIN1_ID].enable == STA_ON) {
            MDev_SetWindow_Task(hWin, WIN1_ID);
        }

        break;

    case JDQ_WIN2_UP:
    case JDQ_WIN2_DOWN:
        if (dev_inf.WIN.detail[WIN2_ID].enable == STA_ON) {
            MDev_SetWindow_Task(hWin, WIN2_ID);
        }

        break;

    case JDQ_WIN3_UP:
    case JDQ_WIN3_DOWN:
        if (dev_inf.WIN.detail[WIN3_ID].enable == STA_ON) {
            MDev_SetWindow_Task(hWin, WIN3_ID);
        }

        break;

    case JDQ_WIN4_UP:
    case JDQ_WIN4_DOWN:
        if (dev_inf.WIN.detail[WIN4_ID].enable == STA_ON) {
            MDev_SetWindow_Task(hWin, WIN4_ID);
        }

        break;

    case JDQ_ALA:
        if (basic_inf.ala_en == STA_ON) {
            MSys_AlaTS_Task(hWin);
        }

        break;
    }
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

        case GUI_ID_DROPDOWN2:
        case GUI_ID_DROPDOWN3:
            if (NCode == WM_NOTIFICATION_SEL_CHANGED) {
                Get_OptValue(hWin, JDQ_Select, Id);
                Set_OptValue(hWin, JDQ_Select);
            }

            break;

        case GUI_ID_BUTTON3:
        case GUI_ID_BUTTON4:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Detailed_Set(hWin, (JDQ_Select * 2 + Id - GUI_ID_BUTTON3));
            }

            break;

        case GUI_ID_BUTTON2:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Get_OptValue(hWin, JDQ_Select, NULL);

                if (Config_Save(SJDQ_ID) < 0) {
                    MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_SAVE_ER));
                }

                if (Config_Save(SWIN_ID) < 0) {
                    MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_SAVE_ER));
                }

                if (Config_Save(SSPRAY_ID) < 0) {
                    MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_SAVE_ER));
                }

                if (Config_Save(SWET_ID) < 0) {
                    MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_SAVE_ER));
                }

                if (Config_Save(SCIRCLE_ID) < 0) {
                    MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_SAVE_ER));
                }

                if (Config_Save(STIMER_ID) < 0) {
                    MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_SAVE_ER));
                }

                if (Config_Save(SHEAT_ID) < 0) {
                    MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_SAVE_ER));
                }

                if (Config_Save(SBASIC_ID) < 0) {
                    MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_SAVE_ER));
                }

                if (Config_Save(STEMPC_ID) < 0) {
                    MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_SAVE_ER));
                }

//设定好的不关闭

//级别禁止
//                for (i = 0; i < JDQ_NUM_MAX; i++) {
//                    if (dev_inf.JDQ[i].enable != STA_OFF)
//                        continue;

//                    for (j = 0; j < CTRL_LV_NUM; j++) {
//                        ctrl_inf.lv[j].JDQ[i].mode = MODE_OFF;
//                    }
//                }

//                for (i = 0; i < WINDOW_NUM_MAX; i++) {
//                    if (dev_inf.WIN.detail[i].enable != STA_OFF)
//                        continue;

//                    for (j = 0; j < CTRL_LV_NUM; j++) {
//                        if (i <= WIN2_ID) {
//                            ctrl_inf.lv[j].WIN[i].degree = 0;
//                        } else {
//                            ctrl_inf.exlv[j].WIN[i - WIN3_ID].degree = 0;
//                        }
//                    }
//                }

//                if (Config_Save(SLVJDQ_ID) < 0) {
//                    MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_SAVE_ER));
//                }

                Current_HWIN = WM_GetParent(hWin);
                GUI_EndDialog(hWin, 0);
            }

            break;

        case GUI_ID_BUTTON0:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                if (JDQ_Select > 0) {
                    Get_OptValue(hWin, JDQ_Select, NULL);
                    JDQ_Select --;
                    MChange_Menu(Menu_Value_Init, hWin, (PCHG_MENU )Mchg_Menu, JDQ_Select);
                    Set_OptValue(hWin, JDQ_Select);
                }
            }

            break;

        case GUI_ID_BUTTON1:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                if (JDQ_Select < (ALLJDQ_NUM_MAX / 2 - 1)) {
                    Get_OptValue(hWin, JDQ_Select, NULL);
                    JDQ_Select ++;
                    MChange_Menu(Menu_Value_Init, hWin, (PCHG_MENU )Mchg_Menu, JDQ_Select);
                    Set_OptValue(hWin, JDQ_Select);
                }
            }

            break;

        case GUI_ID_BUTTON5:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                GUI_EndDialog(hWin, 0);
                MDev_FuncView_Task(WM_GetParent(hWin), 1);
            }

            break;

        case GUI_ID_EDIT0:
        case GUI_ID_EDIT3:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                temp_id = (Id - GUI_ID_EDIT0) / 3;
                Set_Output_Callback(Max_Fan_Get);
                Set_Input_Len(4);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT1:
        case GUI_ID_EDIT4:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                temp_id = (Id - GUI_ID_EDIT0) / 3;
                Set_Output_Callback(Fan_Num_Get);
                Set_Input_Len(1);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT2:
        case GUI_ID_EDIT5:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                temp_id = (Id - GUI_ID_EDIT0) / 3;
                Set_Output_Callback(Fan_Cur_Get);
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
WM_HWIN MDev_FixedSpeedfan;
void MDev_FixedSpeedfan_Task(WM_HWIN hParent, unsigned char Select)
{
    JDQ_Select = Select;
    MDev_FixedSpeedfan = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    Current_HWIN = MDev_FixedSpeedfan;

}

