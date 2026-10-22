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

const u16 Lang_dev_equ_name[DEV_ITEM_NUM] = {
    Lang_Wet_Curtain, Lang_Spray, Lang_Heat, Lang_Timer, Lang_Tswitch,
    Lang_illumination, Lang_Circulating_Fan, Lang_Area_Exh, Lang_Var_Heat, Lang_Var_TSwitch};


const char * dev_equ_PIC[] = {
    "00wet", "00spray", "00heatr", "76iconSys_Time", "95bmMPic_Cut",

    "00light", "99Mpic_Speed", "79iconSys_DataEx", "00heatr", "95bmMPic_Cut"
};

const u16 Lang_ctrl_equ_name[CTRL_ITEM_NUM] = {
    Lang_NP_Control, Lang_RH_Compensation, Lang_Day_Adjunct, Lang_Time_Adjunct,
    Lang_OuAdjunct, Lang_HumCtrl, Lang_CO2Ctrl, Lang_NH3Ctrl};


const char * ctrl_equ_PIC[] = {
    "74iconCtrl_VACCtrl", "99Mpic_Hum", "68iconCtrl_Age", "69iconCtrl_Time",

    "70iconCtrl_Out", "71iconCtrl_HumCtrl", "72iconCtrl_CO2Ctrl", "73iconCtrl_NH3Ctrl",
};

#define GUI_ID_BUTTON10   (GUI_ID_USER+1)
#define GUI_ID_BUTTON11   (GUI_ID_USER+2)
#define GUI_ID_BUTTON12   (GUI_ID_USER+3)
#define GUI_ID_BUTTON13   (GUI_ID_USER+4)
#define GUI_ID_BUTTON14   (GUI_ID_USER+5)

/*********************************************************************
*
*       Dialog resource
*
* This table conatins the info required to create the dialog.
* It has been created by ucGUIbuilder.
*/

#define Y   (-15)
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect,    "",               0,                        0,  0,  800, 480, 0, 0},
    { TEXT_CreateIndirect,      "控制管理",          GUI_ID_TEXT0,             300, 4, 200, 42, 0, 0},

    { BUTTON_CreateIndirect,    "温度曲线",           GUI_ID_BUTTON0,          98, 80 + Y, 180, 65, 0, 0},
    { BUTTON_CreateIndirect,    "通风级别",           GUI_ID_BUTTON1,          98, 160 + Y, 180, 65, 0, 0},
    { BUTTON_CreateIndirect,    "控制参数",           GUI_ID_BUTTON2,          98, 240 + Y, 180, 65, 0, 0},
    { BUTTON_CreateIndirect,    "最小通风",           GUI_ID_BUTTON3,          98, 320 + Y, 180, 65, 0, 0},
//    { BUTTON_CreateIndirect,    "生产数据",           GUI_ID_BUTTON14,          133, 295, 120, 50, 0, 0},

//    { BUTTON_CreateIndirect,    "通风级别",           GUI_ID_BUTTON5,          324, 79, 120, 50, 0, 0},
    { BUTTON_CreateIndirect,    "日龄辅助",           GUI_ID_BUTTON5,          324, 80 + Y, 180, 65, 0, 0, USER_BUF},
    { BUTTON_CreateIndirect,    "时段辅助",           GUI_ID_BUTTON6,          324, 160 + Y, 180, 65, 0, 0, USER_BUF},
    { BUTTON_CreateIndirect,    "室外辅助",           GUI_ID_BUTTON7,          324, 240 + Y, 180, 65, 0, 0, USER_BUF},
    { BUTTON_CreateIndirect,    "湿热补偿",           GUI_ID_BUTTON8,          324, 320 + Y, 180, 65, 0, 0, USER_BUF},
    { BUTTON_CreateIndirect,    "...",           	  GUI_ID_BUTTON9,          324, 370, 180, 30, 0, 0},


    { BUTTON_CreateIndirect,    "湿度控制",           GUI_ID_BUTTON10,          550, 80 + Y, 180, 65, 0, 0, USER_BUF},
    { BUTTON_CreateIndirect,    "CO2控制",           GUI_ID_BUTTON11,           550, 160 + Y, 180, 65, 0, 0, USER_BUF},
    { BUTTON_CreateIndirect,    "NH3控制",           GUI_ID_BUTTON12,           550, 240 + Y, 180, 65, 0, 0, USER_BUF},
    { BUTTON_CreateIndirect,    "静压控制",           GUI_ID_BUTTON13,          550, 320 + Y, 180, 65, 0, 0, USER_BUF},
    { BUTTON_CreateIndirect,    "...",           	  GUI_ID_BUTTON14,          550, 370, 180, 40, 0, 0},


//    { BUTTON_CreateIndirect,    "上一页",          GUI_ID_BUTTON0,          MENU_BACK_X1, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
//    { BUTTON_CreateIndirect,    "下一页",          GUI_ID_BUTTON1,          MENU_BACK_X2, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
//    { BUTTON_CreateIndirect,    "返回",             GUI_ID_BUTTON2,         MENU_BACK_X3, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON1_0,          384, 398, 90, 82, 0, 0},
    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON1_1,          25, 400, 105, 80, 0, 0},
    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON1_2,          155, 400, 105, 80, 0, 0},
    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON1_3,          275, 400, 105, 80, 0, 0},
    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON1_4,          435, 400, 105, 80, 0, 0},
    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON1_5,          555, 400, 105, 80, 0, 0},
    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON1_6,          675, 400, 125, 80, 0, 0},
};
static const u16 Lang_KEY_NAME[] = {Lang_T_Curve, Lang_Vent_Lv, Lang_Control_Set, Lang_MinVent};
static const char * KEY_PIC[] = {"64iconCtrl_TempCurve", "65iconCtrl_LV", "67iconCtrl_CtrlData", "66iconCtrl_MinFan"};

/*****************************************************************
**      FunctionName:void PaintDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**
**      call this function in _cbCallback --> WM_PAINT
*****************************************************************/

static void PaintDialog(WM_MESSAGE * pMsg)
{
    Show_INBackPic();
}

static void _cbMore(WM_MESSAGE * pMsg)
{
    WM_HWIN  hWin;
    hWin  = pMsg->hWin;

    switch (pMsg->MsgId) {
    case WM_PAINT:
        if (BUTTON_IsPressed(hWin)) {
            GUI_SetBkColor(GUI_WHITE);
//				GUI_Clear();
            GUI_SetAlpha(0xb0);
            GUI_defDrawBitmap("00more", 66, 12);
            GUI_SetAlpha(0);
        } else {
            GUI_defDrawBitmap("00more", 66, 12);
        }

        break;

    default:
        BUTTON_Callback(pMsg);
    }
}


static void _cbKey(WM_MESSAGE * pMsg)
{
    WM_HWIN  hWin;
    int 	 Id;
    hWin  = pMsg->hWin;
    Id = WM_GetId(hWin);
    Id = Id <= GUI_ID_BUTTON9 ? Id : (GUI_ID_BUTTON9 + Id - GUI_ID_USER);

    switch (pMsg->MsgId) {
    case WM_PAINT:
        if (BUTTON_IsPressed(hWin)) {
            GUI_SetBkColor(GUI_WHITE);
//				GUI_Clear();
            GUI_SetAlpha(0xb0);
            GUI_defDrawBitmap("56Mpic_KeyBack", 0, 0);
            GUI_defDrawBitmap(KEY_PIC[(Id - GUI_ID_BUTTON0)], 10, 15);
            ButtonText_Show(WM_GetDialogItem(hWin, Id), GUI_LANG_GetTextEx(Lang_KEY_NAME[(Id - GUI_ID_BUTTON0)], Language_Bit), 50, 15);
            GUI_SetAlpha(0);
        } else {
            GUI_defDrawBitmap("56Mpic_KeyBack", 0, 0);
            GUI_defDrawBitmap(KEY_PIC[(Id - GUI_ID_BUTTON0)], 10, 15);
            ButtonText_Show(WM_GetDialogItem(hWin, Id), GUI_LANG_GetTextEx(Lang_KEY_NAME[(Id - GUI_ID_BUTTON0)], Language_Bit), 50, 15);
        }

        break;

    default:
        BUTTON_Callback(pMsg);
    }
}

void _cbDevKey(WM_MESSAGE * pMsg)
{
    WM_HWIN  hWin;
    int 	 Id;
    hWin  = pMsg->hWin;
    Id = WM_GetId(hWin);
    Id = Id <= GUI_ID_BUTTON9 ? Id : (GUI_ID_BUTTON9 + Id - GUI_ID_USER);
    int		show_id = 0;

    switch (pMsg->MsgId) {
    case WM_PAINT:
        BUTTON_GetUserData(hWin, &show_id, 1);

        if (BUTTON_IsPressed(hWin)) {
            GUI_SetBkColor(GUI_WHITE);
//				GUI_Clear();
            GUI_SetAlpha(0xb0);
            GUI_defDrawBitmap("56Mpic_KeyBack", 0, 0);
            GUI_defDrawBitmap(dev_equ_PIC[show_id], 10, 15);
            ButtonText_Show(WM_GetDialogItem(hWin, Id), GUI_LANG_GetTextEx(Lang_dev_equ_name[show_id], Language_Bit), 50, 16);
            GUI_SetAlpha(0);
        } else {
            GUI_defDrawBitmap("56Mpic_KeyBack", 0, 0);
            GUI_defDrawBitmap(dev_equ_PIC[show_id], 10, 15);
            ButtonText_Show(WM_GetDialogItem(hWin, Id), GUI_LANG_GetTextEx(Lang_dev_equ_name[show_id], Language_Bit), 50, 16);
        }

        break;

    default:
        BUTTON_Callback(pMsg);
    }
}

void _cbCtrlKey(WM_MESSAGE * pMsg)
{
    WM_HWIN  hWin;
    int 	 Id;
    hWin  = pMsg->hWin;
    Id = WM_GetId(hWin);
    Id = Id <= GUI_ID_BUTTON9 ? Id : (GUI_ID_BUTTON9 + Id - GUI_ID_USER);
    int		show_id = 0;

    switch (pMsg->MsgId) {
    case WM_PAINT:
        BUTTON_GetUserData(hWin, &show_id, 1);

        if (BUTTON_IsPressed(hWin)) {
            GUI_SetBkColor(GUI_WHITE);
            GUI_SetAlpha(0xb0);
            GUI_defDrawBitmap("56Mpic_KeyBack", 0, 0);
            GUI_defDrawBitmap(ctrl_equ_PIC[show_id], 10, 15);
            ButtonText_Show(WM_GetDialogItem(hWin, Id), GUI_LANG_GetTextEx(Lang_ctrl_equ_name[show_id], Language_Bit), 50, 19);
            GUI_SetAlpha(0);
        } else {
            GUI_defDrawBitmap("56Mpic_KeyBack", 0, 0);
            GUI_defDrawBitmap(ctrl_equ_PIC[show_id], 10, 15);
            ButtonText_Show(WM_GetDialogItem(hWin, Id), GUI_LANG_GetTextEx(Lang_ctrl_equ_name[show_id], Language_Bit), 50, 19);
        }

        break;

    default:
        BUTTON_Callback(pMsg);
    }
}


void cal_dev_show(WM_HWIN hWin)
{
    int id = 0, num = 0;

    if (dev_inf.wet_en[0] == STA_ON || dev_inf.wet_en[1] == STA_ON) {
        sys_sta.dev_equ_en[id++] = STA_ON;
        num++;
    } else {
        sys_sta.dev_equ_en[id++] = STA_OFF;
    }

    if (dev_inf.spray_en == STA_ON) {
        sys_sta.dev_equ_en[id++] = STA_ON;
        num++;
    } else {
        sys_sta.dev_equ_en[id++] = STA_OFF;
    }

    if (basic_inf.heat_en[HEAT_AREA_A] == STA_ON || basic_inf.heat_en[HEAT_AREA_B] == STA_ON || basic_inf.heat_en[HEAT_AREA_C] == STA_ON) {
        sys_sta.dev_equ_en[id++] = STA_ON;
        num++;
    } else {
        sys_sta.dev_equ_en[id++] = STA_OFF;
    }

    if (ext_inf.timer[TR_TYPE_TIMER1].enable == STA_ON || ext_inf.timer[TR_TYPE_TIMER2].enable == STA_ON
            || ext_inf.timer[TR_TYPE_TIMER3].enable == STA_ON || ext_inf.timer[TR_TYPE_TIMER4].enable == STA_ON ) {
        sys_sta.dev_equ_en[id++] = STA_ON;
        num++;
    } else {
        sys_sta.dev_equ_en[id++] = STA_OFF;
    }

    if (basic_inf.tempc_en.enable == STA_ON) {
        sys_sta.dev_equ_en[id++] = STA_ON;
        num++;
    } else {
        sys_sta.dev_equ_en[id++] = STA_OFF;
    }

    if (dev_inf.light.enable == STA_ON) {
        sys_sta.dev_equ_en[id++] = STA_ON;
        num++;
    } else {
        sys_sta.dev_equ_en[id++] = STA_OFF;
    }

    if (dev_inf.circle.enable == STA_ON) {
        sys_sta.dev_equ_en[id++] = STA_ON;
        num++;
    } else {
        sys_sta.dev_equ_en[id++] = STA_OFF;
    }

    if (basic_inf.area_pwm[HEAT_AREA_A].enable == STA_ON || basic_inf.area_pwm[HEAT_AREA_B].enable == STA_ON
            || basic_inf.area_pwm[HEAT_AREA_C].enable == STA_ON) {
        sys_sta.dev_equ_en[id++] = STA_ON;
        num++;
    } else {
        sys_sta.dev_equ_en[id++] = STA_OFF;
    }

    if (basic_inf.pheat_en == STA_ON) {
        sys_sta.dev_equ_en[id++] = STA_ON;
        num++;
    } else {
        sys_sta.dev_equ_en[id++] = STA_OFF;
    }

    if (basic_inf.tempc_pwm_en.enable == STA_ON) {
        sys_sta.dev_equ_en[id++] = STA_ON;
        num++;
    } else {
        sys_sta.dev_equ_en[id++] = STA_OFF;
    }

    if (num <= 4) {
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON9));
    }



    id = 0;
    num = 0;

    if (cal_inf.SEN_EN[SENSOR_TYPE_VAC] == STA_ON) {
        sys_sta.ctrl_equ_en[id++] = STA_ON;
        num++;
    } else {
        sys_sta.ctrl_equ_en[id++] = STA_OFF;
    }

    if (cal_inf.SEN_EN[SENSOR_TYPE_HUM] == STA_ON) {
        sys_sta.ctrl_equ_en[id++] = STA_ON;
        num++;
    } else {
        sys_sta.ctrl_equ_en[id++] = STA_OFF;
    }

    sys_sta.ctrl_equ_en[id++] = STA_ON;
    num++;
    sys_sta.ctrl_equ_en[id++] = STA_ON;
    num++;

    if (dev_inf.sensor_adj.TEMP_EN[TEMP_OUT_SID] == STA_ON) {
        sys_sta.ctrl_equ_en[id++] = STA_ON;
        num++;
    } else {
        sys_sta.ctrl_equ_en[id++] = STA_OFF;
    }

    if (cal_inf.SEN_EN[SENSOR_TYPE_HUM] == STA_ON) {
        sys_sta.ctrl_equ_en[id++] = STA_ON;
        num++;
    } else {
        sys_sta.ctrl_equ_en[id++] = STA_OFF;
    }

    if (cal_inf.SEN_EN[SENSOR_TYPE_CO2] == STA_ON) {
        sys_sta.ctrl_equ_en[id++] = STA_ON;
        num++;
    } else {
        sys_sta.ctrl_equ_en[id++] = STA_OFF;
    }

    if (cal_inf.SEN_EN[SENSOR_TYPE_NH3] == STA_ON) {
        sys_sta.ctrl_equ_en[id++] = STA_ON;
        num++;
    } else {
        sys_sta.ctrl_equ_en[id++] = STA_OFF;
    }

    if (num <= 4) {
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON14));
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
    U8 i = 0;


    //
    //GUI_ID_BUTTON0
    //
    for (MDev_ID = GUI_ID_BUTTON0; MDev_ID <= GUI_ID_BUTTON3; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, _cbKey);
    }

    WM_SetHasTrans(WM_GetDialogItem(hWin, GUI_ID_BUTTON9));
    WM_SetCallback(WM_GetDialogItem(hWin, GUI_ID_BUTTON9), _cbMore);
    WM_SetHasTrans(WM_GetDialogItem(hWin, GUI_ID_BUTTON14));
    WM_SetCallback(WM_GetDialogItem(hWin, GUI_ID_BUTTON14), _cbMore);

    cal_dev_show(hWin);
    MDev_ID = GUI_ID_BUTTON5;

    for (i = 0; MDev_ID <= GUI_ID_BUTTON8; ) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);

        for (; i < DEV_ITEM_NUM; i++) {
            if (dev_inf.dev_equ_show[i] != STA_ON || sys_sta.dev_equ_en[i] != STA_ON) {//首位显示且实际启用
                continue;
            }

            BUTTON_SetUserData(hItem, &i, 1);
            WM_SetCallback(hItem, _cbDevKey);

            i++;
            MDev_ID++;
            break;
        }


        if (i >= DEV_ITEM_NUM) {
            break;//没找到退出
        }
    }

    for (i = 0; MDev_ID <= GUI_ID_BUTTON8; ) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);

        for (; i < DEV_ITEM_NUM; i++) {
            if (dev_inf.dev_equ_show[i] == STA_ON || sys_sta.dev_equ_en[i] != STA_ON) {//首位不显示且实际启用
                continue;
            }

            BUTTON_SetUserData(hItem, &i, 1);
            WM_SetCallback(hItem, _cbDevKey);

            i++;
            MDev_ID++;
            break;
        }


        if (i >= DEV_ITEM_NUM) {
            break;//没找到退出
        }
    }

    for (; MDev_ID <= GUI_ID_BUTTON8; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_HideWindow(hItem);
    }


    MDev_ID = GUI_ID_BUTTON10;

    for (i = 0; MDev_ID <= GUI_ID_BUTTON13; ) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);

        for (; i < CTRL_ITEM_NUM; i++) {
            if (dev_inf.ctrl_equ_show[i] != STA_ON || sys_sta.ctrl_equ_en[i] != STA_ON) {//首位显示且实际启用
                continue;
            }

            BUTTON_SetUserData(hItem, &i, 1);
            WM_SetCallback(hItem, _cbCtrlKey);

            i++;
            MDev_ID++;
            break;
        }


        if (i >= CTRL_ITEM_NUM) {
            break;//没找到退出
        }
    }

    for (i = 0; MDev_ID <= GUI_ID_BUTTON13; ) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);

        for (; i < CTRL_ITEM_NUM; i++) {
            if (dev_inf.ctrl_equ_show[i] == STA_ON || sys_sta.ctrl_equ_en[i] != STA_ON) {//首位不显示且实际启用
                continue;
            }

            BUTTON_SetUserData(hItem, &i, 1);
            WM_SetCallback(hItem, _cbCtrlKey);

            i++;
            MDev_ID++;
            break;
        }


        if (i >= CTRL_ITEM_NUM) {
            break;//没找到退出
        }
    }

    for (; MDev_ID <= GUI_ID_BUTTON13; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);
        WM_HideWindow(hItem);
    }



    for (MDev_ID = GUI_ID_TEXT0; MDev_ID <= GUI_ID_TEXT0; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24(hItem);

        if (MDev_ID == GUI_ID_TEXT0) {
            TEXT_SetTextAlign(hItem,  GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetTextColor(hItem, GUI_WHITE);
            TEXT_SetText(hItem, GUI_LANG_GetTextEx(Lang_Control, Language_Bit));
        }
    }


    for (MDev_ID = GUI_ID_BUTTON1_0; MDev_ID <= GUI_ID_BUTTON1_6; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, Main_cbButton);
    }
}

void _cbDevCallback(WM_HWIN hWin, U8 id)
{
    int i = 0, res = 0;

    switch (id) {
    case 0:
        if (dev_inf.wet_en[0] == STA_ON && dev_inf.wet_en[1] == STA_ON) {
            MDev_Wet_Task(hWin, 0, STA_ON);
            break;
        }

        if (dev_inf.wet_en[0] == STA_ON) {
            MDev_Wet_Task(hWin, 0, STA_OFF);
            break;
        }

        MDev_Wet_Task(hWin, 1, STA_OFF);
        break;

    case 1:
        MDev_Spray_Task(hWin);
        break;

    case 2:
        for (res = 0, i = 0; i < HEAT_NUM_MAX; i++) {
            if (basic_inf.heat_en[i] == STA_ON) {
                res = res * 10 + (i + 1);
            }
        }

        if (res < 10) {//只有一个
            MDev_Heater_Task(hWin, (res - 1), STA_OFF);
            break;
        }

        if (basic_inf.heat_en[0] == STA_ON) {
            MDev_Heater_Task(hWin, 0, STA_ON);
        } else {
            MDev_Heater_Task(hWin, 1, STA_ON);
        }

        break;

    case 3:
        for (res = 0, i = 0; i < TR_TYPE_NUM; i++) {
            if (ext_inf.timer[i].enable == STA_ON) {
                res = res * 10 + (i + 1);
            }
        }

        if (res < 10) {//只有一个
            MDev_Timer_Task(hWin, (res - 1), STA_OFF);
            break;
        }

        if (ext_inf.timer[0].enable == STA_ON) {
            MDev_Timer_Task(hWin, 0, STA_ON);
        } else if (ext_inf.timer[1].enable == STA_ON) {
            MDev_Timer_Task(hWin, 1, STA_ON);
        } else {
            MDev_Timer_Task(hWin, 2, STA_ON);
        }

        break;

    case 4:
        MDev_Tempc_Task(hWin);
        break;

    case 5:
        MDev_SetLight_Task(hWin);
        break;

    case 6:
        MDev_Circle_Task(hWin);
        break;

    case 7:
        for (res = 0, i = 0; i < APWM_NUM_MAX; i++) {
            if (basic_inf.area_pwm[i].enable == STA_ON) {
                res = res * 10 + (i + 1);
            }
        }

        if (res < 10) {//只有一个
            MDev_APWM_FAN_Task(hWin, (res - 1), STA_OFF);
            break;
        }

        if (basic_inf.area_pwm[0].enable == STA_ON) {
            MDev_APWM_FAN_Task(hWin, 0, STA_ON);
        } else {
            MDev_APWM_FAN_Task(hWin, 1, STA_ON);
        }

        break;

    case 8:
        MDev_PHeater_Task(hWin, 0);
        break;

    case 9:
        MDev_Tempc_Pwm_Task(hWin);
        break;

    default:
        break;
    }
}

void _cbCtrlCallback(WM_HWIN hWin, U8 id)
{
    switch (id) {
    case 0:
        MCtrl_VAC_Task(hWin);
        break;

    case 1:
        MCtrl_HumFix_Task(hWin);
        break;

    case 2:
        Set_Assist_Type(AS_TYPE_DAY);
        MCtrl_Assist_Task(hWin);
        break;

    case 3:
        Set_Assist_Type(AS_TYPE_TIME);
        MCtrl_Assist_Task(hWin);
        break;

    case 4:
        Set_Assist_Type(AS_TYPE_OUT);
        MCtrl_Assist_Task(hWin);
        break;


    case 5:
        Set_OTSensor_Type(SENCON_HUM);
        MCtrl_OTSensor_Task(hWin);
        break;

    case 6:
        Set_OTSensor_Type(SENCON_CO2);
        MCtrl_OTSensor_Task(hWin);
        break;

    case 7:
        Set_OTSensor_Type(SENCON_NH3);
        MCtrl_OTSensor_Task(hWin);
        break;


    default:
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
    int		show_id = 0;

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

        if (Id >= GUI_ID_BUTTON1_0 && Id <= GUI_ID_BUTTON1_6) {
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Button_Menu(Id, hWin, GUI_ID_BUTTON1_2);
            }

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
            if (NCode == WM_NOTIFICATION_RELEASED)
                MCtrl_TempCurve_Task(hWin);

            break;

        case GUI_ID_BUTTON1:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                cal_list_type();
                MCtrl_LVView_Task(hWin, 0, 0);
            }

            break;

        case GUI_ID_BUTTON2:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                MCtrl_Var_Task(hWin);
            }

            break;

        case GUI_ID_BUTTON3:
            if (NCode == WM_NOTIFICATION_RELEASED)
                Mctrl_MinAir_Task(hWin);

            break;

        case GUI_ID_BUTTON5:
        case GUI_ID_BUTTON6:
        case GUI_ID_BUTTON7:
        case GUI_ID_BUTTON8:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                BUTTON_GetUserData(WM_GetDialogItem(hWin, Id), &show_id, 1);
                _cbDevCallback(hWin, show_id);
            }

            break;

        case GUI_ID_BUTTON10:
        case GUI_ID_BUTTON11:
        case GUI_ID_BUTTON12:
        case GUI_ID_BUTTON13:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                BUTTON_GetUserData(WM_GetDialogItem(hWin, Id), &show_id, 1);
                _cbCtrlCallback(hWin, show_id);
            }

            break;

        case GUI_ID_BUTTON9:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                GUI_EndDialog(hWin, 0);
                MCtrl_Man_More_Task(WM_GetParent(hWin), 0);
            }

            break;

        case GUI_ID_BUTTON14:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                GUI_EndDialog(hWin, 0);
                MCtrl_Man_More_Task(WM_GetParent(hWin), 1);
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
WM_HWIN MCtrl_Man;
void MCtrl_Man_Task(WM_HWIN hParent)
{
    Main_Buffer=Main_Buffer_Ctrl;
    MCtrl_Man = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    Current_HWIN = MCtrl_Man;

}

