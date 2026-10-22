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
static U8 select_num = 0;//选择哪个
static U8 select_type = 0;//选择哪个

/*********************************************************************
*
*       Dialog resource
*
* This table conatins the info required to create the dialog.
* It has been created by ucGUIbuilder.
*/
#define X  60
#define Y1  120
#define X_Space     400
#define Y_Space     50


static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect,    "",               0,                       0,  0,  800, 480, 0, 0},
    { TEXT_CreateIndirect,      "设备管理-更多设置", GUI_ID_TEXT0,            250, 4, 300, 42, 0, 0},
    { TEXT_CreateIndirect,      "设备地址2:",     	GUI_ID_TEXT1,          X, Y1, 200, 36, 0, 0},
    { EDIT_CreateIndirect,      "",           		GUI_ID_EDIT0,          X + 180, Y1, 200, 36, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",            GUI_ID_CHECK0,          X + X_Space, Y1 + 3, 100, CHECK_SIZE, 0, 0},
    { TEXT_CreateIndirect,      "设备地址3:",     	GUI_ID_TEXT2,          X, Y1 + 1 * Y_Space, 200, 36, 0, 0},
    { EDIT_CreateIndirect,      "",           		GUI_ID_EDIT1,          X + 180, Y1 + 1 * Y_Space, 200, 36, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",            GUI_ID_CHECK1,          X + X_Space, Y1 + 1 * Y_Space + 3, 100, CHECK_SIZE, 0, 0},
    { TEXT_CreateIndirect,      "设备地址4:",     	GUI_ID_TEXT3,          X, Y1 + 2 * Y_Space, 200, 36, 0, 0},
    { EDIT_CreateIndirect,      "",           		GUI_ID_EDIT2,          X + 180, Y1 + 2 * Y_Space, 200, 36, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",            GUI_ID_CHECK2,          X + X_Space, Y1 + 2 * Y_Space + 3, 100, CHECK_SIZE, 0, 0},
    { TEXT_CreateIndirect,      "设备地址5:",     	GUI_ID_TEXT4,          X, Y1 + 3 * Y_Space, 200, 36, 0, 0},
    { EDIT_CreateIndirect,      "",           		GUI_ID_EDIT3,          X + 180, Y1 + 3 * Y_Space, 200, 36, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",            GUI_ID_CHECK3,          X + X_Space, Y1 + 3 * Y_Space + 3, 100, CHECK_SIZE, 0, 0},
    { TEXT_CreateIndirect,      "设备地址6:",     	GUI_ID_TEXT5,          X, Y1 + 4 * Y_Space, 200, 36, 0, 0},
    { EDIT_CreateIndirect,      "",           		GUI_ID_EDIT4,          X + 180, Y1 + 4 * Y_Space, 200, 36, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",            GUI_ID_CHECK4,          X + X_Space, Y1 + 4 * Y_Space + 3, 100, CHECK_SIZE, 0, 0},

    { BUTTON_CreateIndirect,    "料塔标定",          GUI_ID_BUTTON3,          X + X_Space + 110, Y1 + 0 * Y_Space, 110, 36, 0, 0},
    { BUTTON_CreateIndirect,    "料塔标定",          GUI_ID_BUTTON4,          X + X_Space + 110, Y1 + 1 * Y_Space, 110, 36, 0, 0},
    { BUTTON_CreateIndirect,    "料塔标定",          GUI_ID_BUTTON5,          X + X_Space + 110, Y1 + 2 * Y_Space, 110, 36, 0, 0},
    { BUTTON_CreateIndirect,    "料塔标定",          GUI_ID_BUTTON6,          X + X_Space + 110, Y1 + 3 * Y_Space, 110, 36, 0, 0},
    { BUTTON_CreateIndirect,    "料塔标定",          GUI_ID_BUTTON7,          X + X_Space + 110, Y1 + 4 * Y_Space, 110, 36, 0, 0},

    { TEXT_CreateIndirect,      "设备1总量:",     	GUI_ID_TEXT6,          X, 70, 200, 36, 0, 0},
    { EDIT_CreateIndirect,      "",           		GUI_ID_EDIT5,          X + 180, 70, 200, 36, 0, 0},

    { TEXT_CreateIndirect,      "水脉冲单位:",     	GUI_ID_TEXT7,          X, Y1 + 4 * Y_Space, 200, 36, 0, 0},
    { EDIT_CreateIndirect,      "",           		GUI_ID_EDIT6,          X + 180, Y1 + 4 * Y_Space, 200, 36, 0, 0},

//    { BUTTON_CreateIndirect,    "上一页",          GUI_ID_BUTTON0,          MENU_BACK_X1, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
//    { BUTTON_CreateIndirect,    "下一页",          GUI_ID_BUTTON1,          MENU_BACK_X2, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
    { BUTTON_CreateIndirect,    "返回",             GUI_ID_BUTTON2,         MENU_BACK_X3, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
    { BUTTON_CreateIndirect,    "",     GUI_ID_BUTTON9,           			660, 0, 90, 60, 0, 0},

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
    GUI_DrawRoundedRect(90, 55, 690, 406, 10);
    GUI_DrawRoundedRect(86, 52, 694, 409, 10);
}

static void Update_Value(WM_HWIN hWin)
{
    int i = 0, j = 0;

    if (select_num == WATER_MODE_PULSE) {

        sprintf(show_buf, "%d L", index_inf.water[0][PRO_DATA_ALL]); //
        EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT5), show_buf);

        for (i = 1; i < 5; i++) {
            sprintf(show_buf, "%d L", index_inf.water[i][PRO_DATA_ALL]); //
            EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT0 + i - 1), show_buf);
        }

        sprintf(show_buf, "%d L", ext_inf.din.pro_data[0].water_unit); //
        EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT6), show_buf);
        return;
    }


    for (i = 1; i < 6; i++) {
        if (select_num == WATER_MODE_CJT) {
            for (j = 0; j < sizeof(ext_inf.din.pro_data[i].water_addr); j++) {
                if (ext_inf.din.pro_data[i].water_addr[j] < 0x30 || ext_inf.din.pro_data[i].water_addr[j] > 0x39) {
                    break;
                }
            }

            memcpy(show_buf, ext_inf.din.pro_data[i].water_addr, sizeof(ext_inf.din.pro_data[i].water_addr));
            show_buf[j] = 0;
            EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT0 + i - 1), show_buf);
        } else if (select_num == WATER_MODE_MB) {
            sprintf(show_buf, "%d",  ext_inf.din.pro_data[i].water_addr[0]);
            EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT0 + i - 1), show_buf);

        } else {
            sprintf(show_buf, "%d",  ext_inf.din.pro_data[i].food_addr);
            EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT0 + i - 1), show_buf);
        }
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

    for (MDev_ID = GUI_ID_BUTTON3; MDev_ID <= GUI_ID_BUTTON7; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Button_SetHZ24(hItem);
        WM_HideWindow(hItem);
        BUTTON_SetText(hItem, GUI_LANG_GetTextEx(Lang_Feed_Bins_ADJ, Language_Bit));
    }

    for (MDev_ID = GUI_ID_BUTTON9; MDev_ID <= GUI_ID_BUTTON9; MDev_ID++) {//set
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, set_cbButton);
    }
	
    if (select_num != WATER_MODE_PULSE && select_num != WATER_MODE_CJT && select_num != WATER_MODE_MB) {
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON9));
    } 
	
    for (MDev_ID = GUI_ID_CHECK0; MDev_ID <= GUI_ID_CHECK4; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);

        CHECKBOX_SetText(hItem, GUI_LANG_GetTextEx(Lang_Enable, Language_Bit));

        Check_SetHZ24(hItem);
        Login_Show(hItem, LOGIN_ADMIN);

        if (select_num == WATER_MODE_PULSE && MDev_ID == GUI_ID_CHECK4) {
            WM_HideWindow(hItem);
        }

        if (select_num == FOOD_MODE_MB) {
            if (ext_inf.din.pro_data[MDev_ID - GUI_ID_CHECK0 + 1].food_sta == STA_OFF) {
                CHECKBOX_SetState(hItem, 0);
            } else {
                CHECKBOX_SetState(hItem, 1);
            }
        } else {
            if (ext_inf.din.pro_data[MDev_ID - GUI_ID_CHECK0 + 1].water_sta == STA_OFF) {
                CHECKBOX_SetState(hItem, 0);
            } else {
                CHECKBOX_SetState(hItem, 1);
            }
        }
    }

    for (MDev_ID = GUI_ID_EDIT0; MDev_ID <= GUI_ID_EDIT6; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Edit_SetHZ24(hItem);
        EDIT_SetMaxLen(hItem, 15);

        if (select_num != WATER_MODE_PULSE && (MDev_ID == GUI_ID_EDIT5 || MDev_ID == GUI_ID_EDIT6)) {
            WM_HideWindow(hItem);
        }

        if (select_num == WATER_MODE_PULSE && MDev_ID == GUI_ID_EDIT4) {
            WM_HideWindow(hItem);
        }

        Login_Show(hItem, LOGIN_ADMIN);
    }

    for (MDev_ID = GUI_ID_TEXT0; MDev_ID <= GUI_ID_TEXT7; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24(hItem);

        if (MDev_ID == GUI_ID_TEXT0) {
            TEXT_SetTextAlign(hItem,  GUI_TA_HCENTER | GUI_TA_VCENTER);
            sprintf(show_buf, "%s-%s", GUI_LANG_GetTextEx(Lang_Device, Language_Bit), GUI_LANG_GetTextEx(Lang_More_Settings, Language_Bit));
            TEXT_SetText(hItem, show_buf);
            continue;
        }

        sprintf(show_buf, "%s%d:", GUI_LANG_GetTextEx(Lang_Addr, Language_Bit), MDev_ID - GUI_ID_TEXT1 + 2);
        TEXT_SetText(hItem, (const char *)show_buf);

        if (select_num != WATER_MODE_PULSE && (MDev_ID == GUI_ID_TEXT6 || MDev_ID == GUI_ID_TEXT7)) {
            WM_HideWindow(hItem);
        }

        if (select_num == WATER_MODE_PULSE && MDev_ID == GUI_ID_TEXT5) {
            WM_HideWindow(hItem);
        }
    }

    if (select_num == FOOD_MODE_MB && ext_inf.din.pro_data[0].food_sta == FL_MB_XK3102) {
        for (MDev_ID = GUI_ID_BUTTON3; MDev_ID <= GUI_ID_BUTTON7; MDev_ID++) {
            hItem = WM_GetDialogItem(hWin, MDev_ID);
            WM_ShowWindow(hItem);
        }
    }

    if (select_num == WATER_MODE_PULSE) {
        for (MDev_ID = GUI_ID_TEXT1; MDev_ID <= GUI_ID_TEXT4; MDev_ID++) {

            sprintf(show_buf, "%s%d:", GUI_LANG_GetTextEx(Lang_Dev, Language_Bit), (2 + MDev_ID - GUI_ID_TEXT1));
            TEXT_SetText(WM_GetDialogItem(hWin, MDev_ID), show_buf);
        }
    }

    sprintf(show_buf, "%s%d.%s:", GUI_LANG_GetTextEx(Lang_Dev, Language_Bit), 1,
            GUI_LANG_GetTextEx(Lang_Total, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT6), show_buf);
    sprintf(show_buf, "%s:", GUI_LANG_GetTextEx(Lang_Unit, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT7), show_buf);

    WM_SetFocus(WM_GetDialogItem(hWin, GUI_ID_BUTTON2));
    Update_Value(hWin);
}

static void CJT_Addr_Get(void)
{
    unsigned char *time_buf = Get_Input_Text();
    unsigned char len = strlen((const char *)time_buf);
    unsigned char i = 0;

    if (len == 0 || len > 14) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        return;
    }

//    if (stemp > 255) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = 255;
//    }

//    if (stemp < 0) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = 0;
//    }

    memcpy(ext_inf.din.pro_data[select_type].water_addr, time_buf, len);

    if (len != 14)ext_inf.din.pro_data[select_type].water_addr[len] = 0;

    for (i = 0; i < sizeof(ext_inf.din.pro_data[select_type].water_addr); i++) {
        if (ext_inf.din.pro_data[select_type].water_addr[i] < 0x30 || ext_inf.din.pro_data[select_type].water_addr[i] > 0x39) {
            break;
        }
    }

    memcpy(show_buf, ext_inf.din.pro_data[select_type].water_addr, sizeof(ext_inf.din.pro_data[select_type].water_addr));
    show_buf[i] = 0;
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, GUI_ID_EDIT0 + select_type - 1), show_buf);
}

static void Water_Addr_Get(void)
{
    float ftemp = Get_Input_Value();
    unsigned short stemp = (unsigned short)ftemp;

    if (stemp > 255) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 255;
    }

//    if (stemp < 0) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = 0;
//    }
    ext_inf.din.pro_data[select_type].water_addr[0] = stemp;
    sprintf(show_buf, "%d",  ext_inf.din.pro_data[select_type].water_addr[0]);
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, GUI_ID_EDIT0 + select_type - 1), show_buf);
}

static void Food_Addr_Get(void)
{
    float ftemp = Get_Input_Value();
    unsigned short stemp = (unsigned short)ftemp;

    if (stemp > 255) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 255;
    }

//    if (stemp < 0) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = 0;
//    }
    ext_inf.din.pro_data[select_type].food_addr = stemp;
    sprintf(show_buf, "%d",  ext_inf.din.pro_data[select_type].food_addr);
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, GUI_ID_EDIT0 + select_type - 1), show_buf);
}

static void Water_Get(void)
{
    float ftemp = Get_Input_Value();
    U32 stemp = (U32)ftemp;

//    if (stemp > TEMP_ADJMAX) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = TEMP_ADJMAX;
//    }

//    if (stemp < 0) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = 0;
//    }

    index_inf.water[select_type][PRO_DATA_ALL] = stemp;
    Update_Value(Current_HWIN);
}

static void Water_Unit_Get(void)
{
    int i;
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

    for (i = 0; i < PRO_DATA_MAX; i++) {
        ext_inf.din.pro_data[i].water_unit = stemp;
    }

    Update_Value(Current_HWIN);
}

void pluse_water_chk_alarm(void)
{
    int i = 0, num = 0;

    //如果启用了多脉冲水表，把多外部报警关闭
    for (i = 1; i < PULSE_WATER_MAX; i++) {
        if (ext_inf.din.pro_data[i].water_sta == WL_PULSE) {
            num = 2;
            break;
        }
    }

    if (num > 1 && ext_inf.din.pro_data[0].water_sta == WL_PULSE) {
        for (i = 0, num = 0; i < ALARM_NUM_MAX; i++) {
            if (getbit(ext_inf.din.alarm, i) == STA_ON) {
                num++;
            }

            if (num > 1) {
                clrbit(ext_inf.din.alarm, i);
            }
        }
    }

    if (ext_inf.din.pro_data[4].water_sta == WL_PULSE) {
        ext_inf.din.egg_en[0] = STA_OFF;
        CHECKBOX_SetState(WM_GetDialogItem(MDev_Produce, GUI_ID_CHECK0 ), ext_inf.din.egg_en[0]);
    }
}

/*********************************************************************
*
*       Dialog callback routine
*/
static void _cbCallback(WM_MESSAGE * pMsg)
{
    int NCode, Id, i = 0;
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

        switch (Id) {
        case GUI_ID_OK:
            if (NCode == WM_NOTIFICATION_RELEASED)
                GUI_EndDialog(hWin, 0);

            break;

        case GUI_ID_CANCEL:
            if (NCode == WM_NOTIFICATION_RELEASED)
                GUI_EndDialog(hWin, 0);

            break;

        case GUI_ID_EDIT0:
        case GUI_ID_EDIT1:
        case GUI_ID_EDIT2:
        case GUI_ID_EDIT3:
        case GUI_ID_EDIT4:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                select_type = Id - GUI_ID_EDIT0 + 1;

                if (select_num == WATER_MODE_CJT) {
                    Set_Output_Callback(CJT_Addr_Get);
                    Set_Input_Len(14);
                } else if (select_num == WATER_MODE_MB) {
                    Set_Output_Callback(Water_Addr_Get);
                    Set_Input_Len(3);
                } else if (select_num == WATER_MODE_PULSE) {
                    Set_Output_Callback(Water_Get);
                    Set_Input_Len(10);
                } else {
                    Set_Output_Callback(Food_Addr_Get);
                    Set_Input_Len(3);
                }

                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT5:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                select_type = 0;
                Set_Output_Callback(Water_Get);
                Set_Input_Len(10);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT6:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(Water_Unit_Get);
                Set_Input_Len(4);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_BUTTON3:
        case GUI_ID_BUTTON4:
        case GUI_ID_BUTTON5:
        case GUI_ID_BUTTON6:
        case GUI_ID_BUTTON7:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                MDev_FoodADJ_Task(hWin, Id - GUI_ID_BUTTON3 + 1);
            }

            break;
			
        case GUI_ID_BUTTON9:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                MDev_Water_Set_Task(hWin);
            }

            break;
			
        case GUI_ID_BUTTON2:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                if (select_num == FOOD_MODE_MB) {
                    for (i = 0; i <= 4; i++) {
                        ext_inf.din.pro_data[i + 1].food_sta = CHECKBOX_GetState(WM_GetDialogItem(hWin, GUI_ID_CHECK0 + i ));
                    }

                } else {
                    for (i = 0; i <= 4; i++) {
                        ext_inf.din.pro_data[i + 1].water_sta = CHECKBOX_GetState(WM_GetDialogItem(hWin, GUI_ID_CHECK0 + i ));
                    }
                }

                pluse_water_chk_alarm();
                Current_HWIN = WM_GetParent(hWin);
                GUI_EndDialog(hWin, 0);
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
WM_HWIN MDev_ProduceMore;
void MDev_ProduceMore_Task(WM_HWIN hParent, U8 id)
{
    select_num = id;//选择哪个
    MDev_ProduceMore = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    Current_HWIN = MDev_ProduceMore;
}

