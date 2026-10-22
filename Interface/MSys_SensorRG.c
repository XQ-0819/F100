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

#define GUI_ID_TEXT10   (GUI_ID_USER+1)
#define GUI_ID_TEXT11   (GUI_ID_USER+2)
#define GUI_ID_TEXT12   (GUI_ID_USER+3)
#define GUI_ID_TEXT13   (GUI_ID_USER+4)
#define GUI_ID_TEXT14   (GUI_ID_USER+5)
#define GUI_ID_TEXT15   (GUI_ID_USER+6)
#define GUI_ID_TEXT16   (GUI_ID_USER+7)
#define GUI_ID_TEXT17   (GUI_ID_USER+8)
#define GUI_ID_TEXT18   (GUI_ID_USER+9)
#define GUI_ID_TEXT19   (GUI_ID_USER+10)
#define GUI_ID_TEXT20   (GUI_ID_USER+11)
#define GUI_ID_TEXT21   (GUI_ID_USER+12)

#define GUI_ID_EDIT10  (GUI_ID_USER+31)
#define GUI_ID_EDIT11  (GUI_ID_USER+32)
#define GUI_ID_EDIT12  (GUI_ID_USER+33)
#define GUI_ID_EDIT13  (GUI_ID_USER+34)
#define GUI_ID_EDIT14  (GUI_ID_USER+35)
#define GUI_ID_EDIT15  (GUI_ID_USER+36)
#define GUI_ID_EDIT16  (GUI_ID_USER+37)

#define XPos_Collec  10



/*********************************************************************
*
*       Dialog resource
*
* This table conatins the info required to create the dialog.
* It has been created by ucGUIbuilder.
*/

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect,    "",               0,                       0,  0,  800, 480, 0, 0},
    { TEXT_CreateIndirect,      "传感器量程", GUI_ID_TEXT0,            250, 0, 300, 42, 0, 0},

    { TEXT_CreateIndirect,      "温度/℃:",     GUI_ID_TEXT1,            100 + XPos_Collec, 66, 170, 36, 0, 0},
    { TEXT_CreateIndirect,      "湿度/%:",    	GUI_ID_TEXT2,            100 + XPos_Collec, 113, 170, 36, 0, 0},
    { TEXT_CreateIndirect,    	"CO2/ppm:",    	GUI_ID_TEXT3,            100 + XPos_Collec, 160, 170, 36, 0, 0},
    { TEXT_CreateIndirect,      "NH3/ppm:",    	GUI_ID_TEXT4,            100 + XPos_Collec, 207, 170, 36, 0, 0},
    { TEXT_CreateIndirect,     	"H2S/ppm:",   	GUI_ID_TEXT5,            100 + XPos_Collec, 254, 170, 36, 0, 0},
    { TEXT_CreateIndirect,      "静压/pa:",  	GUI_ID_TEXT6,            100 + XPos_Collec, 301, 170, 36, 0, 0},
    { TEXT_CreateIndirect,      "风速/m/s:",  	GUI_ID_TEXT7,            100 + XPos_Collec, 348, 170, 36, 0, 0},

    { TEXT_CreateIndirect,      "最小值:",      	GUI_ID_TEXT8,            245 + XPos_Collec, 66, 80, 36, 0, 0},
    { TEXT_CreateIndirect,      "最小值:",    	GUI_ID_TEXT9,            245 + XPos_Collec, 113, 80, 36, 0, 0},
    { TEXT_CreateIndirect,  	"最小值:",    	GUI_ID_TEXT10,            245 + XPos_Collec, 160, 80, 36, 0, 0},
    { TEXT_CreateIndirect,      "最小值:",    	GUI_ID_TEXT11,            245 + XPos_Collec, 207, 80, 36, 0, 0},
    { TEXT_CreateIndirect,    	"最小值:",   		GUI_ID_TEXT12,               245 + XPos_Collec, 254, 80, 36, 0, 0},
    { TEXT_CreateIndirect,    	"最小值:",  		GUI_ID_TEXT13,               245 + XPos_Collec, 301, 80, 36, 0, 0},
    { TEXT_CreateIndirect,      "最小值:",      	GUI_ID_TEXT14,              245 + XPos_Collec, 348, 80, 36, 0, 0},

    { TEXT_CreateIndirect,      "最大值:",      	GUI_ID_TEXT15,            430 + XPos_Collec, 66, 80, 36, 0, 0},
    { TEXT_CreateIndirect,      "最大值:",    	GUI_ID_TEXT16,            430 + XPos_Collec, 113, 80, 36, 0, 0},
    { TEXT_CreateIndirect,  	"最大值:",    	GUI_ID_TEXT17,            430 + XPos_Collec, 160, 80, 36, 0, 0},
    { TEXT_CreateIndirect,      "最大值:",    	GUI_ID_TEXT18,            430 + XPos_Collec, 207, 80, 36, 0, 0},
    { TEXT_CreateIndirect,    	"最大值:",   		GUI_ID_TEXT19,               430 + XPos_Collec, 254, 80, 36, 0, 0},
    { TEXT_CreateIndirect,    	"最大值:",  		GUI_ID_TEXT20,               430 + XPos_Collec, 301, 80, 36, 0, 0},
    { TEXT_CreateIndirect,      "最大值:",      	GUI_ID_TEXT21,              430 + XPos_Collec, 348, 80, 36, 0, 0},

    { CHECKBOX_CreateIndirect,  " ",              GUI_ID_CHECK0,           600 + XPos_Collec, 66 + 3, 103, CHECK_SIZE, 0, 0},
    { EDIT_CreateIndirect,       NULL,            GUI_ID_EDIT0,            330 + XPos_Collec, 66, 70, 36, 0, 0},
    { EDIT_CreateIndirect,       NULL,            GUI_ID_EDIT10,            515 + XPos_Collec, 66, 70, 36, 0, 0},


    { CHECKBOX_CreateIndirect,  " ",            GUI_ID_CHECK1,              600 + XPos_Collec, 113 + 3, 103, CHECK_SIZE, 0, 0},
    { EDIT_CreateIndirect,       NULL,           GUI_ID_EDIT1,            330 + XPos_Collec, 113, 70, 36, 0, 0},
    { EDIT_CreateIndirect,       NULL,           GUI_ID_EDIT11,            515 + XPos_Collec, 113, 70, 36, 0, 0},


    { CHECKBOX_CreateIndirect,  " ",          GUI_ID_CHECK2,               600 + XPos_Collec, 160 + 3, 103, CHECK_SIZE, 0, 0},
    { EDIT_CreateIndirect,       NULL,           GUI_ID_EDIT2,               330 + XPos_Collec, 160, 70, 36, 0, 0},
    { EDIT_CreateIndirect,       NULL,           GUI_ID_EDIT12,               515 + XPos_Collec, 160, 70, 36, 0, 0},

    { CHECKBOX_CreateIndirect,  " ",              GUI_ID_CHECK3,           600 + XPos_Collec, 207 + 3, 103, CHECK_SIZE, 0, 0},
    { EDIT_CreateIndirect,       NULL,            GUI_ID_EDIT3,            330 + XPos_Collec, 207, 70, 36, 0, 0},
    { EDIT_CreateIndirect,       NULL,            GUI_ID_EDIT13,            515 + XPos_Collec, 207, 70, 36, 0, 0},

    { CHECKBOX_CreateIndirect,  " ",          GUI_ID_CHECK4,              600 + XPos_Collec, 254 + 3, 103, CHECK_SIZE, 0, 0},
    { EDIT_CreateIndirect,       NULL,         GUI_ID_EDIT4,            330 + XPos_Collec, 254, 70, 36, 0, 0},
    { EDIT_CreateIndirect,       NULL,         GUI_ID_EDIT14,            515 + XPos_Collec, 254, 70, 36, 0, 0},

    { CHECKBOX_CreateIndirect,  " ",         GUI_ID_CHECK5,                600 + XPos_Collec, 301 + 3, 103, CHECK_SIZE, 0, 0},
    { EDIT_CreateIndirect,       NULL,         GUI_ID_EDIT5,           330 + XPos_Collec, 301, 70, 36, 0, 0},
    { EDIT_CreateIndirect,       NULL,         GUI_ID_EDIT15,           515 + XPos_Collec, 301, 70, 36, 0, 0},


    { CHECKBOX_CreateIndirect,  " ",          GUI_ID_CHECK6,                600 + XPos_Collec, 348 + 3, 103, CHECK_SIZE, 0, 0},
    { EDIT_CreateIndirect,       NULL,          GUI_ID_EDIT6,           330 + XPos_Collec, 348, 70, 36, 0, 0},
    { EDIT_CreateIndirect,       NULL,          GUI_ID_EDIT16,           515 + XPos_Collec, 348, 70, 36, 0, 0},


//    { BUTTON_CreateIndirect,    "上一页",          GUI_ID_BUTTON0,          MENU_BACK_X1, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
//    { BUTTON_CreateIndirect,    "下一页",          GUI_ID_BUTTON1,          MENU_BACK_X2, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
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
    GUI_DrawRoundedRect(70, 55, 712, 396, 10);
    GUI_DrawRoundedRect(66, 52, 716, 399, 10);

}

static void Set_OptValue(WM_HWIN hWin, U8 sid)
{
    int MDev_ID = 0;

    for (MDev_ID = GUI_ID_CHECK0; MDev_ID <= GUI_ID_CHECK6; MDev_ID++) {
        CHECKBOX_SetState(WM_GetDialogItem(hWin, MDev_ID), dev_inf.sensor_rg[SENSOR_TYPE_TEMP + MDev_ID - GUI_ID_CHECK0].enable);
    }
}

static void Set_Value(WM_HWIN hWin)
{
    int i = 0;

    for (i = SENSOR_TYPE_TEMP; i < SENSOR_TYPE_NUM; i++) {
        if (i == SENSOR_TYPE_TEMP) {
            sprintf(show_buf, "%.1f", dev_inf.sensor_rg[i].min_value * 1.0f / TEMP_RATE);
            EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT0 + i - SENSOR_TYPE_TEMP), (const char *) show_buf);
            sprintf(show_buf, "%.1f", dev_inf.sensor_rg[i].max_value * 1.0f / TEMP_RATE);
            EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT10 + i - SENSOR_TYPE_TEMP), (const char *) show_buf);
            continue;
        } else if (i == SENSOR_TYPE_SPEED) {
            sprintf(show_buf, "%.2f", dev_inf.sensor_rg[i].min_value * 1.0f / SPEED_RATE);
            EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT0 + i - SENSOR_TYPE_TEMP), (const char *) show_buf);
            sprintf(show_buf, "%.2f", dev_inf.sensor_rg[i].max_value * 1.0f / SPEED_RATE);
            EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT10 + i - SENSOR_TYPE_TEMP), (const char *) show_buf);
            continue;
        }

        sprintf(show_buf, "%d", dev_inf.sensor_rg[i].min_value);
        EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT0 + i - SENSOR_TYPE_TEMP), (const char *) show_buf);
        sprintf(show_buf, "%d", dev_inf.sensor_rg[i].max_value);
        EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT10 + i - SENSOR_TYPE_TEMP), (const char *) show_buf);
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

    for (MDev_ID = GUI_ID_BUTTON2; MDev_ID <= GUI_ID_BUTTON2; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, Cut_cbButton);
    }

    for (MDev_ID = GUI_ID_EDIT0; MDev_ID <= GUI_ID_EDIT6; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Edit_SetHZ24(hItem);
    }

    for (MDev_ID = GUI_ID_EDIT10; MDev_ID <= GUI_ID_EDIT16; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Edit_SetHZ24(hItem);
    }

    Text_SetHZ24(WM_GetDialogItem(hWin, GUI_ID_TEXT0));
    TEXT_SetTextColor(WM_GetDialogItem(hWin, GUI_ID_TEXT0), GUI_WHITE);

    for (MDev_ID = GUI_ID_TEXT1; MDev_ID <= GUI_ID_TEXT7; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24L(hItem);
    }

    for (MDev_ID = GUI_ID_TEXT8; MDev_ID <= GUI_ID_TEXT9; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24(hItem);
    }

    for (MDev_ID = GUI_ID_TEXT10; MDev_ID <= GUI_ID_TEXT21; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24(hItem);
    }

    for (MDev_ID = GUI_ID_CHECK0; MDev_ID <= GUI_ID_CHECK6; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Check_SetHZ24(hItem);
        CHECKBOX_SetText(hItem, GUI_LANG_GetTextEx(Lang_Enable, Language_Bit));
    }

    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT0), GUI_LANG_GetTextEx(Lang_Seneor_Range, Language_Bit)); // 传感器量程

    sprintf(show_buf, "%s(%s):", GUI_LANG_GetTextEx(Lang_Temp, Language_Bit), GUI_LANG_GetTextEx(Lang_oC, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT1), show_buf); // 温度/℃:
    sprintf(show_buf, "%s(%s):", GUI_LANG_GetTextEx(Lang_HUM, Language_Bit), GUI_LANG_GetTextEx(Lang_percent, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT2), show_buf); // 湿度/%:
    sprintf(show_buf, "%s(%s):", GUI_LANG_GetTextEx(Lang_NP, Language_Bit), GUI_LANG_GetTextEx(Lang_Pa, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT6), show_buf); // 静压/pa:
    sprintf(show_buf, "%s(%s/s):", GUI_LANG_GetTextEx(Lang_Wind, Language_Bit), GUI_LANG_GetTextEx(Lang_m3, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT7), show_buf); // 风速/m/s:

    for (MDev_ID = GUI_ID_TEXT8; MDev_ID <= GUI_ID_TEXT9; MDev_ID++) {
        TEXT_SetText(WM_GetDialogItem(hWin, MDev_ID), GUI_LANG_GetTextEx(Lang_Minimum, Language_Bit)); // 最小值
    }

    for (MDev_ID = GUI_ID_TEXT10; MDev_ID <= GUI_ID_TEXT14; MDev_ID++) {
        TEXT_SetText(WM_GetDialogItem(hWin, MDev_ID), GUI_LANG_GetTextEx(Lang_Minimum, Language_Bit)); // 最小值
    }

    for (MDev_ID = GUI_ID_TEXT15; MDev_ID <= GUI_ID_TEXT21; MDev_ID++) {
        TEXT_SetText(WM_GetDialogItem(hWin, MDev_ID), GUI_LANG_GetTextEx(Lang_Maximum, Language_Bit)); // 最大值
    }

    Set_OptValue(hWin, 0);
    Set_Value(hWin);
}


static void Get_OptValue(WM_HWIN hWin, U8 sid)
{
    int MDev_ID = 0;

    for (MDev_ID = GUI_ID_CHECK0; MDev_ID <= GUI_ID_CHECK6; MDev_ID++) {
        dev_inf.sensor_rg[SENSOR_TYPE_TEMP + MDev_ID - GUI_ID_CHECK0].enable = CHECKBOX_GetState(WM_GetDialogItem(hWin, MDev_ID));
    }
}
static U8 temp_id = 0;
static void Min_Value_Get(void)
{
    float ftemp = Get_Input_Value();

    if (temp_id == SENSOR_TYPE_TEMP) {
        ftemp = ftemp * TEMP_RATE;
    } else if (temp_id == SENSOR_TYPE_SPEED) {
        ftemp = ftemp * SPEED_RATE;
    }

    short stemp = (short)ftemp;

//    if (stemp > TIME_2BIT) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = TIME_2BIT;
//    }

//    if (stemp < 0) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = 0;
//    }

    dev_inf.sensor_rg[temp_id].min_value = stemp;
    Set_Value(Current_HWIN);
}

static void Max_Value_Get(void)
{
    float ftemp = Get_Input_Value();

    if (temp_id == SENSOR_TYPE_TEMP) {
        ftemp = ftemp * TEMP_RATE;
    } else if (temp_id == SENSOR_TYPE_SPEED) {
        ftemp = ftemp * SPEED_RATE;
    }

    short stemp = (short)ftemp;

//    if (stemp > TIME_2BIT) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = TIME_2BIT;
//    }

//    if (stemp < 0) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = 0;
//    }

    dev_inf.sensor_rg[temp_id].max_value = stemp;
    Set_Value(Current_HWIN);
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
                Get_OptValue(hWin, 0);

                if (Config_Save(SSENSOR_RG_ID) < 0) {
                    MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_SAVE_ER));
                }

                Current_HWIN = WM_GetParent(hWin);
                GUI_EndDialog(hWin, 0);
            }

            break;

        case GUI_ID_EDIT0:
        case GUI_ID_EDIT1:
        case GUI_ID_EDIT2:
        case GUI_ID_EDIT3:
        case GUI_ID_EDIT4:
        case GUI_ID_EDIT5:
        case GUI_ID_EDIT6:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                temp_id = (Id - GUI_ID_EDIT0 + 1);
                Set_Input_Len(5);
                Set_Output_Callback(Min_Value_Get);
                Input_Task(hWin);

            }

            break;

        case GUI_ID_EDIT10:
        case GUI_ID_EDIT11:
        case GUI_ID_EDIT12:
        case GUI_ID_EDIT13:
        case GUI_ID_EDIT14:
        case GUI_ID_EDIT15:
        case GUI_ID_EDIT16:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                temp_id = (Id - GUI_ID_EDIT10 + 1);
                Set_Input_Len(5);
                Set_Output_Callback(Max_Value_Get);
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
WM_HWIN MSys_SensorRG;
void MSys_SensorRG_Task(WM_HWIN hParent)
{
    MSys_SensorRG = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    Current_HWIN = MSys_SensorRG;
}

