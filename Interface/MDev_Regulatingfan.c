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
#include "DataSave.h"
#include "bsp.h"

/*********************************************************************
*
*       static data
*
**********************************************************************
*/

#define MAX_ID			(PWM_NUM_MAX/2 - 1)
static U8 select_id = 0;
static TCHG_MENU Mchg_Menu[] = {
    {MSHOW, TYPE_TEXT, GUI_ID_TEXT1, "模拟量%d:", 1, 1},
    {MSHOW, TYPE_TEXT, GUI_ID_TEXT7, "模拟量%d:", 2, 1},

    {MSHOW, TYPE_EDIT, GUI_ID_EDIT0, "%.1f", 0, FAN_RATE},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT1, "%d", 0, 1},
//    {MSHOW, TYPE_EDIT, GUI_ID_EDIT2, "%d", 0, 1},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT3, "%d", 0, 1},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT4, "%.1f", 0, FAN_RATE},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT5, "%d", 0, 1},
//    {MSHOW, TYPE_EDIT, GUI_ID_EDIT6, "%d", 0, 1},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT7, "%d", 0, 1},
    {MEXIT, 0, 0, "", 0, 0},
};


static void Menu_Show(WM_HWIN hWin, U8 sid)
{
    U8 i = 0;

    switch (sid) {
    case 0:
        if (DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2)) == 2
                || DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2)) == 3) {
            WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON3));
        } else {
            WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON3));
        }

        if (DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3)) == 2
                || DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3)) == 3) {
            WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON4));
        } else {
            WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON4));
        }

        if (DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2)) != 1) {
            WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN0));
            WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT0));
            WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT1));
            WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT3));
        } else {
            WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN0));
            WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT0));
            WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT1));
            WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT3));
        }

        if (DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3)) != 1) {
            WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN1));
            WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT4));
            WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT5));
            WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT7));
        } else {
            WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN1));
            WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT4));
            WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT5));
            WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT7));
        }

        break;

    case 1:
        if (DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2)) == 2
                || DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2)) == 3) {
            WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON3));
        } else {
            WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON3));
        }

        if (DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3)) == 2

                || DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3)) == 3) {
            WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON4));
        } else {
            WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON4));
        }

        if (DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2)) != 1) {
            WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN0));
            WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT0));
            WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT1));
            WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT3));
        } else {
            WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN0));
            WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT0));
            WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT1));
            WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT3));
        }

        if (DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3)) != 1) {
            WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN1));
            WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT4));
            WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT5));
            WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT7));
        } else {
            WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN1));
            WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT4));
            WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT5));
            WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT7));
        }

        break;

    case 2:
        if (DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2)) == 1 
                || DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2)) == 2) {
            WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON3));
        } else {
            WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON3));
        }

        if (DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3)) == 1
                || DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3)) == 2) {
            WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON4));
        } else {
            WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON4));
        }

        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN0));
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT0));
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT1));
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT3));

        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN1));
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT4));
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT5));
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT7));
        break;
    }

    if ((DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2)) != 1 &&
            DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3)) != 1)
            || sid == 2) {
        for (i = 0; i < 5; i++) {
            WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT2 + i));
        }
    } else {
        for (i = 0; i < 5; i++) {
            WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT2 + i));
        }
    }
}
static void Set_OptValue(WM_HWIN hWin, U8 sid)
{
    U8 i = 0;

    if (sid <= MAX_ID) {
        //增加是否启用
        DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN0 ), dev_inf.PWM[2 * sid].type);
        DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN1 ), dev_inf.PWM[2 * sid + 1].type);
    }

    Button_CurShow(hWin, sid, 3);

    for (i = 0; i < 3; i++) {
        DROPDOWN_DeleteItem(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2), 1);
        DROPDOWN_DeleteItem(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3), 1);
    }

    switch (sid) {
    case 0:
        sprintf(show_buf, "%s%s", GUI_LANG_GetTextEx(Lang_Exh, Language_Bit), GUI_LANG_GetTextEx(Lang_Fan, Language_Bit));
        DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2), show_buf);
        sprintf(show_buf, GUI_LANG_GetTextEx(Lang_Exh_Area, Language_Bit), "A");
        DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2), show_buf);
        DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2), GUI_LANG_GetTextEx(Lang_Inlet, Language_Bit));

        sprintf(show_buf, "%s%s", GUI_LANG_GetTextEx(Lang_Exh, Language_Bit), GUI_LANG_GetTextEx(Lang_Fan, Language_Bit));
        DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3), show_buf);
        sprintf(show_buf, GUI_LANG_GetTextEx(Lang_Exh_Area, Language_Bit), "B");
        DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3), show_buf);
        DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3), GUI_LANG_GetTextEx(Lang_Inlet, Language_Bit));

        if (basic_inf.area_pwm[APWM_FAN1].enable == STA_ON) {
            DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2 ), 2);
        } else if (dev_inf.SWIN.detail[SWIN1_ID].enable == STA_ON) {
            DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2 ), 3);
        } else {
            DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2 ), dev_inf.PWM[2 * sid].enable);
        }

        if (basic_inf.area_pwm[APWM_FAN2].enable == STA_ON) {
            DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3 ), 2);
        } else if (dev_inf.SWIN.detail[SWIN2_ID].enable == STA_ON) {
            DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3 ), 3);
        } else {
            DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3 ), dev_inf.PWM[2 * sid + 1].enable);
        }

        break;

    case 1:

        sprintf(show_buf, "%s%s", GUI_LANG_GetTextEx(Lang_Exh, Language_Bit), GUI_LANG_GetTextEx(Lang_Fan, Language_Bit));
        DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2), show_buf);
        sprintf(show_buf, GUI_LANG_GetTextEx(Lang_Exh_Area, Language_Bit), "C");
        DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2), show_buf);
        DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2), GUI_LANG_GetTextEx(Lang_Inlet, Language_Bit));

        sprintf(show_buf, "%s%s", GUI_LANG_GetTextEx(Lang_Exh, Language_Bit), GUI_LANG_GetTextEx(Lang_Fan, Language_Bit));
        DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3), show_buf);
        sprintf(show_buf, GUI_LANG_GetTextEx(Lang_Heat, Language_Bit));
        DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3), show_buf);
        DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3), GUI_LANG_GetTextEx(Lang_Inlet, Language_Bit));

        if (basic_inf.area_pwm[APWM_FAN3].enable == STA_ON) {
            DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2 ), 2);
        } else if (dev_inf.SWIN.detail[SWIN3_ID].enable == STA_ON) {
            DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2 ), 3);
        } else {
            DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2 ), dev_inf.PWM[2 * sid].enable);
        }

        if (basic_inf.pheat_en == STA_ON) {
            DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3 ), 2);
        } else if (dev_inf.SWIN.detail[SWIN4_ID].enable == STA_ON) {
            DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3 ), 3);
        } else {
            DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3 ), dev_inf.PWM[2 * sid + 1].enable);
        }

        break;

    case 2:
        DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2), GUI_LANG_GetTextEx(Lang_Tswitch, Language_Bit));
        DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2), GUI_LANG_GetTextEx(Lang_Inlet, Language_Bit));
        DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3), GUI_LANG_GetTextEx(Lang_illumination, Language_Bit));
        DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3), GUI_LANG_GetTextEx(Lang_Inlet, Language_Bit));
        if (dev_inf.SWIN.detail[SWIN5_ID].enable == STA_ON) {
            DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2 ), 2);
        } else {
            DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2 ), basic_inf.tempc_pwm_en.enable);
        }


        if (dev_inf.SWIN.detail[SWIN6_ID].enable == STA_ON) {
            DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3 ), 2);
        } else {
            DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3 ), dev_inf.light.enable);
        }

        break;

    }

    Menu_Show(hWin, sid);
}

static void Menu_Value_Init(WM_HWIN hWin, U8 sid)
{


    U8 id = 0;

    sprintf(Mchg_Menu[id].text, "%s", GUI_LANG_GetTextEx(Lang_Analogy, Language_Bit));
    strcat(Mchg_Menu[id++].text, "%d");
    sprintf(Mchg_Menu[id].text, "%s", GUI_LANG_GetTextEx(Lang_Analogy, Language_Bit));
    strcat(Mchg_Menu[id++].text, "%d");

    id = 0;
    Mchg_Menu[id++].value = 2 * sid + 1;
    Mchg_Menu[id++].value = 2 * sid + 2;

    if (sid > MAX_ID)return;

    Mchg_Menu[id++].value = dev_inf.PWM[2 * sid].max_air;
    Mchg_Menu[id++].value = dev_inf.PWM[2 * sid].fan_num;
//    Mchg_Menu[id++].value = dev_inf.PWM[2 * sid].max_vol;
    Mchg_Menu[id++].value = dev_inf.PWM[2 * sid].min_vol;
    Mchg_Menu[id++].value = dev_inf.PWM[2 * sid + 1].max_air;
    Mchg_Menu[id++].value = dev_inf.PWM[2 * sid + 1].fan_num;
//    Mchg_Menu[id++].value = dev_inf.PWM[2 * sid + 1].max_vol;
    Mchg_Menu[id++].value = dev_inf.PWM[2 * sid + 1].min_vol;

}



/*********************************************************************
*
*       Dialog resource
*
* This table conatins the info required to create the dialog.
* It has been created by ucGUIbuilder.
*/
#define XPos_Collec  (50)
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect,    "",               0,                       0,  0,  800, 480, 0, 0},
    { TEXT_CreateIndirect,      "设备管理-模拟量",          GUI_ID_TEXT0,           250, 4, 300, 42, 0, 0},
    { TEXT_CreateIndirect,      "模拟量1:",      	GUI_ID_TEXT1,            50 + XPos_Collec, 70, 125, 36, 0, 0},
    { TEXT_CreateIndirect,      "风机型号/寸:",    	GUI_ID_TEXT2,            50 + XPos_Collec, 120, 200, 36, 0, 0},
    { TEXT_CreateIndirect,      "最大风量/km3/h:",  GUI_ID_TEXT3,            50 + XPos_Collec, 170, 200, 36, 0, 0},
    { TEXT_CreateIndirect,      "风机个数:",  		GUI_ID_TEXT4,            50 + XPos_Collec, 270, 200, 36, 0, 0},

//    { TEXT_CreateIndirect,      "最大电压/V:",      GUI_ID_TEXT5,            50 + XPos_Collec, 270, 200, 36, 0, 0},
    { TEXT_CreateIndirect,		"最小电压/V:",	 	GUI_ID_TEXT6,			 50 + XPos_Collec, 220, 200, 36, 0, 0},


    { DROPDOWN_CreateIndirect,	 "18",			 GUI_ID_DROPDOWN2,		160 + XPos_Collec, 70, 150, 146, 0, 0},
    { DROPDOWN_CreateIndirect,	 "18",			 GUI_ID_DROPDOWN0,		245 + XPos_Collec, 120, 100, 120, 0, 0},
    { EDIT_CreateIndirect,		 "6.3",		     GUI_ID_EDIT0,			245 + XPos_Collec, 170, 90, 36, 0, 0},
    { EDIT_CreateIndirect,		 "2",		     GUI_ID_EDIT1,			245 + XPos_Collec, 270, 90, 36, 0, 0},
//    { EDIT_CreateIndirect,		 "10",			 GUI_ID_EDIT2,		  	245 + XPos_Collec, 270, 90, 36, 0, 0},
    { EDIT_CreateIndirect,		 "6",		 	 GUI_ID_EDIT3,			245 + XPos_Collec, 220, 90, 36, 0, 0},
    { BUTTON_CreateIndirect,    "详细设置",      GUI_ID_BUTTON3,        180 + XPos_Collec, 130, 120, 50, 0, 0},

    { TEXT_CreateIndirect,      "模拟量2:",    GUI_ID_TEXT7,          370 + XPos_Collec, 70, 125, 36, 0, 0},
    { DROPDOWN_CreateIndirect,	 "18",			 GUI_ID_DROPDOWN3,		470 + XPos_Collec, 70, 150, 146, 0, 0},
    { DROPDOWN_CreateIndirect,	"18",			 GUI_ID_DROPDOWN1,		450 + XPos_Collec, 120, 100, 120, 0, 0},
    { EDIT_CreateIndirect,		 "6.3",		   	 GUI_ID_EDIT4,			450 + XPos_Collec, 170, 90, 36, 0, 0},
    { EDIT_CreateIndirect,		 "2",		   	 GUI_ID_EDIT5,			450 + XPos_Collec, 270, 90, 36, 0, 0},
//    { EDIT_CreateIndirect,		 "10",			 GUI_ID_EDIT6,		    450 + XPos_Collec, 270, 90, 36, 0, 0},
    { EDIT_CreateIndirect,		 "6",		     GUI_ID_EDIT7,			450 + XPos_Collec, 220, 90, 36, 0, 0},
    { BUTTON_CreateIndirect,    "详细设置",      GUI_ID_BUTTON4,        450 + XPos_Collec, 130, 120, 50, 0, 0},


    { BUTTON_CreateIndirect,    "上一页",        GUI_ID_BUTTON0,          MENU_BACK_X1, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
    { BUTTON_CreateIndirect,    "下一页",        GUI_ID_BUTTON1,          MENU_BACK_X2, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
    { BUTTON_CreateIndirect,    "返回",          GUI_ID_BUTTON2,         MENU_BACK_X3, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},

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
    GUI_DrawRoundedRect(80, 55, 712, 386, 10);
    GUI_DrawRoundedRect(76, 52, 716, 389, 10);

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

    //FRAMEWIN_Set_UserDefine(hWin);
    //FRAMEWIN_TextBarSet(WM_GetDialogItem(hWin,GUI_ID_TEXT0));
    //
    //GUI_ID_BUTTON0
    //
    for (MDev_ID = GUI_ID_BUTTON0; MDev_ID <= GUI_ID_BUTTON2; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, Cut_cbButton);
    }

    hItem = WM_GetDialogItem(hWin, GUI_ID_BUTTON5);
    WM_SetHasTrans(hItem);
    WM_SetCallback(hItem, Detail_cbButton);

    for (MDev_ID = GUI_ID_BUTTON3; MDev_ID <= GUI_ID_BUTTON4; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Button_SetHZ24(hItem);
    }


    for (MDev_ID = GUI_ID_EDIT0; MDev_ID <= GUI_ID_EDIT7; MDev_ID++) {
        if ( MDev_ID == GUI_ID_EDIT2 || MDev_ID == GUI_ID_EDIT6) {
            continue;
        }

        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Edit_SetHZ24(hItem);
    }

    for (MDev_ID = GUI_ID_TEXT0; MDev_ID <= GUI_ID_TEXT7; MDev_ID++) {
        if (MDev_ID == GUI_ID_TEXT5) {
            continue;
        }

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

        if (MDev_ID >= GUI_ID_DROPDOWN2)
            continue;

        for (i = 0; i < FAN_SIZE_NUM; i++) {
            DROPDOWN_AddString(hItem, fan_size[i]);
        }
    }

    sprintf(show_buf, "%s-%s", GUI_LANG_GetTextEx(Lang_Device, Language_Bit), GUI_LANG_GetTextEx(Lang_Analogy, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT0), show_buf);                                                   // 设备管理-模拟量
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT2), GUI_LANG_GetTextEx(Lang_Fan_Model, Language_Bit));           // 风机型号/寸:
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT3), GUI_LANG_GetTextEx(Lang_Rated_Air, Language_Bit));           // 最大风量/km3/h:
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT4), GUI_LANG_GetTextEx(Lang_Num_fans, Language_Bit));            // 风机个数:
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT6), GUI_LANG_GetTextEx(Lang_VolMin, Language_Bit));              // 最小电压/V:
    BUTTON_SetText(WM_GetDialogItem(hWin, GUI_ID_BUTTON3), GUI_LANG_GetTextEx(Lang_Detail_Set, Language_Bit));      // 详细设置
    BUTTON_SetText(WM_GetDialogItem(hWin, GUI_ID_BUTTON4), GUI_LANG_GetTextEx(Lang_Detail_Set, Language_Bit));      // 详细设置
    DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2), GUI_LANG_GetTextEx(Lang_Disable, Language_Bit));
    DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3), GUI_LANG_GetTextEx(Lang_Disable, Language_Bit));

    MChange_Menu(Menu_Value_Init, hWin, (PCHG_MENU )Mchg_Menu, select_id);
    Set_OptValue(hWin, select_id);

}

static void Get_OptValue(WM_HWIN hWin, U8 sid)
{
    switch (sid) {
    case 0:
        if (DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2)) == 2) {
            basic_inf.area_pwm[APWM_FAN1].enable = STA_ON;
            dev_inf.SWIN.detail[SWIN1_ID].enable = STA_OFF;
            dev_inf.PWM[2 * sid].enable = STA_OFF;
        } else if (DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2)) == 3) {
            basic_inf.area_pwm[APWM_FAN1].enable = STA_OFF;
            dev_inf.SWIN.detail[SWIN1_ID].enable = STA_ON;
            dev_inf.PWM[2 * sid].enable = STA_OFF;
        } else {
            basic_inf.area_pwm[APWM_FAN1].enable = STA_OFF;
            dev_inf.SWIN.detail[SWIN1_ID].enable = STA_OFF;
            dev_inf.PWM[2 * sid].enable = DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2));
        }

        if (DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3)) == 2) {
            basic_inf.area_pwm[APWM_FAN2].enable = STA_ON;
            dev_inf.SWIN.detail[SWIN2_ID].enable = STA_OFF;
            dev_inf.PWM[2 * sid + 1].enable = STA_OFF;
        } else if (DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3)) == 3) {
            basic_inf.area_pwm[APWM_FAN2].enable = STA_OFF;
            dev_inf.SWIN.detail[SWIN2_ID].enable = STA_ON;
            dev_inf.PWM[2 * sid + 1].enable = STA_OFF;
        } else {
            basic_inf.area_pwm[APWM_FAN2].enable = STA_OFF;
            dev_inf.SWIN.detail[SWIN2_ID].enable = STA_OFF;
            dev_inf.PWM[2 * sid + 1].enable = DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3));
        }

        break;

    case 1:
        if (DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2)) == 2) {
            basic_inf.area_pwm[APWM_FAN3].enable = STA_ON;
            dev_inf.SWIN.detail[SWIN3_ID].enable = STA_OFF;
            dev_inf.PWM[2 * sid].enable = STA_OFF;
        } else if (DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2)) == 3) {
            basic_inf.area_pwm[APWM_FAN3].enable = STA_OFF;
            dev_inf.SWIN.detail[SWIN3_ID].enable = STA_ON;
            dev_inf.PWM[2 * sid].enable = STA_OFF;
        } else {
            basic_inf.area_pwm[APWM_FAN3].enable = STA_OFF;
            dev_inf.SWIN.detail[SWIN3_ID].enable = STA_OFF;
            dev_inf.PWM[2 * sid].enable = DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2));
        }


        if (DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3)) == 2) {
            basic_inf.pheat_en = STA_ON;
            dev_inf.SWIN.detail[SWIN4_ID].enable = STA_OFF;
            dev_inf.PWM[2 * sid + 1].enable = STA_OFF;
        } else if (DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3)) == 3) {
            basic_inf.pheat_en = STA_OFF;
            dev_inf.SWIN.detail[SWIN4_ID].enable = STA_ON;
            dev_inf.PWM[2 * sid + 1].enable = STA_OFF;
        } else {
            basic_inf.pheat_en = STA_OFF;
            dev_inf.SWIN.detail[SWIN4_ID].enable = STA_OFF;
            dev_inf.PWM[2 * sid + 1].enable = DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3));
        }

        break;

    case 2:
        if (DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2)) == 2) {
            dev_inf.SWIN.detail[SWIN5_ID].enable = STA_ON;
            basic_inf.tempc_pwm_en.enable = STA_OFF;
        } else {
            dev_inf.SWIN.detail[SWIN5_ID].enable = STA_OFF;
            basic_inf.tempc_pwm_en.enable = DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2));
        }

        if (DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3)) == 2) {
            dev_inf.SWIN.detail[SWIN6_ID].enable = STA_ON;
            dev_inf.light.enable = STA_OFF;
        } else {
            dev_inf.SWIN.detail[SWIN6_ID].enable = STA_OFF;
            dev_inf.light.enable = DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN3));
        }

        break;

    }

    if (sid > MAX_ID)return;

    dev_inf.PWM[2 * sid].type =  DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN0 ));
    dev_inf.PWM[2 * sid + 1].type =  DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN1 ));

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

    dev_inf.PWM[select_id * 2 + temp_id].max_air = stemp;
    Get_OptValue(Current_HWIN, select_id);
    MChange_Menu(Menu_Value_Init, Current_HWIN, (PCHG_MENU )Mchg_Menu, select_id);
    Set_OptValue(Current_HWIN, select_id);
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

    dev_inf.PWM[select_id * 2 + temp_id].fan_num = stemp;
    Get_OptValue(Current_HWIN, select_id);
    MChange_Menu(Menu_Value_Init, Current_HWIN, (PCHG_MENU )Mchg_Menu, select_id);
    Set_OptValue(Current_HWIN, select_id);
}
/*
static void Max_FanP_Get(void)
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

    dev_inf.PWM[select_id * 2 + temp_id].max_vol = stemp;
    MChange_Menu(Menu_Value_Init, Current_HWIN, (PCHG_MENU )Mchg_Menu, select_id);
    Set_OptValue(Current_HWIN, select_id);
}
*/
static void Min_FanP_Get(void)
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

    dev_inf.PWM[select_id * 2 + temp_id].min_vol = stemp;
    Get_OptValue(Current_HWIN, select_id);
    MChange_Menu(Menu_Value_Init, Current_HWIN, (PCHG_MENU )Mchg_Menu, select_id);
    Set_OptValue(Current_HWIN, select_id);
}

static void Detailed_Set(WM_HWIN hWin, U8 sid)
{
    switch (sid) {
    case APWM_FAN1:
    case APWM_FAN2:
    case APWM_FAN3:
        if (dev_inf.SWIN.detail[sid - PWM_SWIN1].enable == STA_ON) {
            MDev_SetSWin_Task(hWin, sid - PWM_SWIN1);
        } else {
            MDev_APWM_FAN_Task(hWin, (sid - APWM_FAN1), STA_OFF);
        }

        break;

    case PWM_HEAT:
        if (dev_inf.SWIN.detail[sid - PWM_SWIN1].enable == STA_ON) {
            MDev_SetSWin_Task(hWin, sid - PWM_SWIN1);
        } else {
            MDev_PHeater_Task(hWin, 0);
        }

        break;

    case PWM_SWIN5:
        if (dev_inf.SWIN.detail[sid - PWM_SWIN1].enable == STA_ON) {
            MDev_SetSWin_Task(hWin, sid - PWM_SWIN1);
        } else {
            MDev_Tempc_Pwm_Task(hWin);
        }
		
        break;

    case PWM_LIGHT:
        if (dev_inf.SWIN.detail[sid - PWM_SWIN1].enable == STA_ON) {
            MDev_SetSWin_Task(hWin, sid - PWM_SWIN1);
        } else {
            MDev_SetLight_Task(hWin);
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
                Get_OptValue(hWin, select_id);
                Menu_Show(hWin, select_id);
            }

            break;

        case GUI_ID_BUTTON3:
        case GUI_ID_BUTTON4:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Detailed_Set(hWin, (select_id * 2 + Id - GUI_ID_BUTTON3));
            }

            break;

        case GUI_ID_BUTTON5:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                GUI_EndDialog(hWin, 0);
                MDev_FuncView_Task(WM_GetParent(hWin), 0);
            }

            break;

        case GUI_ID_BUTTON2:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Get_OptValue(hWin, select_id);

                if (Config_Save(SPWM_ID) < 0) {
                    MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_SAVE_ER));
                }

                if (Config_Save(SSWIN_ID) < 0) {
                    MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_SAVE_ER));
                }

                if (Config_Save(SLIGHT_ID) < 0) {
                    MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_SAVE_ER));
                }

                if (Config_Save(SHEAT_ID) < 0) {
                    MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_SAVE_ER));
                }

                if (Config_Save(SAPWM_ID) < 0) {
                    MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_SAVE_ER));
                }

                if (Config_Save(STEMPC_PWM_ID) < 0) {
                    MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_SAVE_ER));
                }
//设定好的不关闭
//级别禁止

//                for (i = 0; i < SWINDOW_NUM_MAX; i++) {
//                    //级别禁止
//                    if (dev_inf.SWIN.detail[i].enable == STA_OFF) {
//                        for (j = 0; j < CTRL_LV_NUM; j++) {
//                            ctrl_inf.lv[j].SWIN[i].degree = 0;
//                        }
//                    }
//                }

//                for (i = 0; i < PWM_NUM_MAX; i++) {
//                    if (dev_inf.PWM[i].enable != STA_OFF)
//                        continue;

//                    for (j = 0; j < CTRL_LV_NUM; j++) {
//                        ctrl_inf.lv[j].PWM[i].mode = MODE_OFF;
//                    }
//                }

//                if (Config_Save(SLVPWM_ID) < 0) {
//                    MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_SAVE_ER));
//                }

                Current_HWIN = WM_GetParent(hWin);
                GUI_EndDialog(hWin, 0);
            }

            break;

        case GUI_ID_BUTTON0:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                if (select_id > 0) {
                    Get_OptValue(hWin, select_id);
                    select_id --;
                    MChange_Menu(Menu_Value_Init, hWin, (PCHG_MENU )Mchg_Menu, select_id);
                    Set_OptValue(hWin, select_id);
                }
            }

            break;

        case GUI_ID_BUTTON1:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                if (select_id < (ALLPWM_NUM_MAX / 2 - 1)) {
                    Get_OptValue(hWin, select_id);
                    select_id ++;
                    MChange_Menu(Menu_Value_Init, hWin, (PCHG_MENU )Mchg_Menu, select_id);
                    Set_OptValue(hWin, select_id);
                }
            }

            break;

        case GUI_ID_EDIT0:
        case GUI_ID_EDIT4:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                temp_id = (Id - GUI_ID_EDIT0) / 4;
                Set_Output_Callback(Max_Fan_Get);
                Set_Input_Len(4);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT1:
        case GUI_ID_EDIT5:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                temp_id = (Id - GUI_ID_EDIT0) / 4;
                Set_Output_Callback(Fan_Num_Get);
                Set_Input_Len(1);
                Input_Task(hWin);
            }

            break;

//        case GUI_ID_EDIT2:
//        case GUI_ID_EDIT6:
//            if (NCode == WM_NOTIFICATION_RELEASED) {
//                temp_id = (Id - GUI_ID_EDIT0) / 4;
//                Set_Output_Callback(Max_FanP_Get);
//                Set_Input_Len(2);
//                Input_Task(hWin);
//            }

//            break;

        case GUI_ID_EDIT3:
        case GUI_ID_EDIT7:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                temp_id = (Id - GUI_ID_EDIT0) / 4;
                Set_Output_Callback(Min_FanP_Get);
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
WM_HWIN MDev_Regulatingfan;
void MDev_Regulatingfan_Task(WM_HWIN hParent, unsigned char select)
{
    select_id = select;
    MDev_Regulatingfan = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    Current_HWIN = MDev_Regulatingfan;
}

