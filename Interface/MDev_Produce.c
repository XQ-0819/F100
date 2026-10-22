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


/*********************************************************************
*
*       Dialog resource
*
* This table conatins the info required to create the dialog.
* It has been created by ucGUIbuilder.
*/
#define XPos_Collec  (-20)
#define YPos_Space   (80)

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect,    "",               0,                       0,  0,  800, 480, 0, 0},
    { TEXT_CreateIndirect,      "设备管理-生产数据",          GUI_ID_TEXT0,            250, 4, 300, 42, 0, 0},

    { TEXT_CreateIndirect,      "电量计:",      GUI_ID_TEXT1,           120 + XPos_Collec,  70, 110, 36, 0, 0},
    { TEXT_CreateIndirect,      "水量计:",       GUI_ID_TEXT2,           120 + XPos_Collec, 70 + YPos_Space, 110, 36, 0, 0},
    { TEXT_CreateIndirect,      "料量计:",      GUI_ID_TEXT3,            120 + XPos_Collec, 70 + 2 * YPos_Space, 110, 36, 0, 0},


    { DROPDOWN_CreateIndirect,  "",          	GUI_ID_DROPDOWN0,       230 + XPos_Collec, 70, 160, 145, 0, 0},
    { EDIT_CreateIndirect,      "",          	GUI_ID_EDIT2,           510 + XPos_Collec, 70, 170, 36, 0, 0},

    { DROPDOWN_CreateIndirect,  "",          	GUI_ID_DROPDOWN1,       230 + XPos_Collec, 70 + YPos_Space, 160, 145, 0, 0},
    { DROPDOWN_CreateIndirect,  "",          	GUI_ID_DROPDOWN2,       230 + XPos_Collec, 70 + 2 * YPos_Space, 160, 145, 0, 0},


    { TEXT_CreateIndirect,      "地址:",      GUI_ID_TEXT4,               420 + XPos_Collec, 70, 100, 36, 0, 0},
    { TEXT_CreateIndirect,      "单位:",      GUI_ID_TEXT5,               420 + XPos_Collec, 70 + YPos_Space, 100, 36, 0, 0},
    { TEXT_CreateIndirect,      "单位:",     GUI_ID_TEXT6,                420 + XPos_Collec, 70 + 2 * YPos_Space, 100, 36, 0, 0},
    {  EDIT_CreateIndirect,      "",            GUI_ID_EDIT0,         510 + XPos_Collec, 70 + YPos_Space, 170, 36, 0, 0},
    {  EDIT_CreateIndirect,      "",            GUI_ID_EDIT1,          510 + XPos_Collec, 70 + 2 * YPos_Space, 170, 36, 0, 0},


    { TEXT_CreateIndirect,      "总量:",      		GUI_ID_TEXT7,               420 + XPos_Collec, 230, 100, 36, 0, 0},
    { TEXT_CreateIndirect,      "总量:",     		GUI_ID_TEXT8,                420 + XPos_Collec, 120 + 2 * YPos_Space, 100, 36, 0, 0},
    {  EDIT_CreateIndirect,      "",            GUI_ID_EDIT3,         510 + XPos_Collec, 230, 170, 36, 0, 0},
    {  EDIT_CreateIndirect,      "",            GUI_ID_EDIT4,         510 + XPos_Collec, 120 + 2 * YPos_Space, 170, 36, 0, 0},

    { TEXT_CreateIndirect,      "CT:",     		GUI_ID_TEXT9,                420 + XPos_Collec, 120, 100, 36, 0, 0},
    {  EDIT_CreateIndirect,      "",            GUI_ID_EDIT5,         510 + XPos_Collec, 120, 170, 36, 0, 0},

    { TEXT_CreateIndirect,      "计蛋脉冲:",      GUI_ID_TEXT10,            130 + XPos_Collec, 110 + 3 * YPos_Space, 110, 36, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",           GUI_ID_CHECK0,              250 + XPos_Collec, 113 + 3 * YPos_Space, 200, CHECK_SIZE, 0, 0},
    { TEXT_CreateIndirect,      "总量:",        GUI_ID_TEXT11,                420 + XPos_Collec, 110 + 3 * YPos_Space, 100, 36, 0, 0},
    { EDIT_CreateIndirect,      "",            GUI_ID_EDIT6,         510 + XPos_Collec, 110 + 3 * YPos_Space, 170, 36, 0, 0},


    { BUTTON_CreateIndirect,    "详细设置",          GUI_ID_BUTTON7,          510 + XPos_Collec, 70, 120, 50, 0, 0},
    { BUTTON_CreateIndirect,    "详细设置",          GUI_ID_BUTTON5,          510 + XPos_Collec, 70 + YPos_Space, 120, 50, 0, 0},
    { BUTTON_CreateIndirect,    "详细设置",          GUI_ID_BUTTON6,          510 + XPos_Collec, 70 + 2 * YPos_Space, 120, 50, 0, 0},


    { BUTTON_CreateIndirect,    "实时数据",          GUI_ID_BUTTON3,          MENU_BACK_X1, MENU_BACK_Y, 120, 50, 0, 0},
    { BUTTON_CreateIndirect,    "历史数据",          GUI_ID_BUTTON4,          MENU_BACK_X2, MENU_BACK_Y, 120, 50, 0, 0},

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
    GUI_DrawRoundedRect(90, 55, 690, 406, 10);
    GUI_DrawRoundedRect(86, 52, 694, 409, 10);

//    GUI_DrawLine(80, 260, 712, 260);
//    GUI_DrawLine(80, 264, 712, 264);
}

static void Update_Value(WM_HWIN hWin)
{
//    U8 i = 0;
//    if (ext_inf.din.energy_sta == PL_DL645_97 || ext_inf.din.energy_sta == PL_DL645_07) {
//        for (i = 0; i < sizeof(ext_inf.din.energy_addr); i++) {
//            if (ext_inf.din.energy_addr[i] < 0x30 || ext_inf.din.energy_addr[i] > 0x39) {
//                break;
//            }
//        }

//        memcpy(show_buf, ext_inf.din.energy_addr, sizeof(ext_inf.din.energy_addr));
//        show_buf[i] = 0;
//    } else {
//        sprintf(show_buf, "%d", ext_inf.din.energy_addr[0]);
//    }

//    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT2 ), show_buf);
//    sprintf(show_buf, "%d", ext_inf.din.energy_ct);
//    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT5 ), show_buf);

    if (ext_inf.din.pro_data[0].water_sta != WL_CJT_188 && ext_inf.din.pro_data[0].water_sta != WL_MB_OWN) {
        WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON5));
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT5));
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT0));
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT7));
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT3));
    } else {
        WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON5));
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT5));
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT0));
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT7));
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT3));
    }

    if (ext_inf.din.pro_data[0].food_sta != FL_MB_OWN && ext_inf.din.pro_data[0].food_sta != FL_MB_XK3102) {
        sprintf(show_buf, "%d Kg", index_inf.food[0][PRO_DATA_ALL] / FOOD_RATE); //
        EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT4), show_buf);

        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON6));
        WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT6));
        WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT1));
        WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT8));
        WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT4));

        if (ext_inf.din.pro_data[0].food_sta == FL_TIME) {
            sprintf(show_buf, "%.1f Kg/Min", ext_inf.din.pro_data[0].food_unit / 10.0);
        } else {
            sprintf(show_buf, "%.1f Kg", ext_inf.din.pro_data[0].food_unit / 10.0);
        }

        EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT1), show_buf);

    } else {
        WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON6));
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT6));
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT1));
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT8));
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT4));
    }

    sprintf(show_buf, "%d", index_inf.egg[0][PRO_DATA_ALL]); //
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT6), show_buf);

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
    }

    for (MDev_ID = GUI_ID_EDIT0; MDev_ID <= GUI_ID_EDIT6; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Edit_SetHZ24(hItem);
    }

    EDIT_SetMaxLen(WM_GetDialogItem(hWin, GUI_ID_EDIT1), 17);
    EDIT_SetMaxLen(WM_GetDialogItem(hWin, GUI_ID_EDIT2), 13);
    EDIT_SetMaxLen(WM_GetDialogItem(hWin, GUI_ID_EDIT3), 13);
    EDIT_SetMaxLen(WM_GetDialogItem(hWin, GUI_ID_EDIT4), 13);


    for (MDev_ID = GUI_ID_TEXT0; MDev_ID <= GUI_ID_TEXT9; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24(hItem);

        if (MDev_ID == GUI_ID_TEXT0) {
            TEXT_SetTextAlign(hItem,  GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetTextColor(hItem, GUI_WHITE);
        }
    }

    for (MDev_ID = GUI_ID_TEXT10; MDev_ID <= GUI_ID_TEXT11; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24(hItem);
    }

    for (MDev_ID = GUI_ID_CHECK0; MDev_ID <= GUI_ID_CHECK0; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        CHECKBOX_SetText(hItem, GUI_LANG_GetTextEx(Lang_Enable, Language_Bit));
        Check_SetHZ24(hItem);
        Login_Show(hItem, LOGIN_ADMIN);
    }

    sprintf(show_buf, "%s-%s", GUI_LANG_GetTextEx(Lang_Device, Language_Bit), GUI_LANG_GetTextEx(Lang_Production, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT0), show_buf); // 设备管理-生产数据

    sprintf(show_buf, "%s:", GUI_LANG_GetTextEx(Lang_Ammeter, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT1), show_buf); // 电量计:

    sprintf(show_buf, "%s:", GUI_LANG_GetTextEx(Lang_Wmeter, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT2), show_buf); // 水量计:

    sprintf(show_buf, "%s:", GUI_LANG_GetTextEx(Lang_Mmeter, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT3), show_buf); // 料量计:

    sprintf(show_buf, "%s:", GUI_LANG_GetTextEx(Lang_Addr, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT4), show_buf); // 地址:
    sprintf(show_buf, "%s:", GUI_LANG_GetTextEx(Lang_Unit, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT5), show_buf); // 单位:
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT6), show_buf); // 单位:

    sprintf(show_buf, "%s:", GUI_LANG_GetTextEx(Lang_Total, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT7), show_buf);  // 总量:
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT8), show_buf);  // 总量:
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT11), show_buf); // 总量:

    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT10), GUI_LANG_GetTextEx(Lang_Egg_Pulse, Language_Bit));  // 计蛋脉冲:
    CHECKBOX_SetText(WM_GetDialogItem(hWin, GUI_ID_CHECK0), GUI_LANG_GetTextEx(Lang_Enable, Language_Bit)); // 启用

    BUTTON_SetText(WM_GetDialogItem(hWin, GUI_ID_BUTTON3), GUI_LANG_GetTextEx(Lang_Real_Time, Language_Bit)); // 实时数据
    BUTTON_SetText(WM_GetDialogItem(hWin, GUI_ID_BUTTON4), GUI_LANG_GetTextEx(Lang_History, Language_Bit));   // 历史数据

    for (MDev_ID = GUI_ID_BUTTON5; MDev_ID <= GUI_ID_BUTTON7; MDev_ID++) {
        BUTTON_SetText(WM_GetDialogItem(hWin, MDev_ID), GUI_LANG_GetTextEx(Lang_Detail_Set, Language_Bit)); // 详细设置
    }

    hItem = WM_GetDialogItem(hWin, GUI_ID_DROPDOWN0);
    Dropdown_SetHZ24(hItem);

    DROPDOWN_AddString(hItem, GUI_LANG_GetTextEx(Lang_Disable, Language_Bit)); // 停用
    sprintf(show_buf, "%2d%s", 97, GUI_LANG_GetTextEx(Lang_Ammeter, Language_Bit));
    DROPDOWN_AddString(hItem, show_buf); // 97电表
    sprintf(show_buf, "07%s", GUI_LANG_GetTextEx(Lang_Ammeter, Language_Bit));
    DROPDOWN_AddString(hItem, show_buf); //                                                                                                                                                                                                                                                                                                                                             
    DROPDOWN_AddString(hItem, "DTS9866");

    Login_Show(hItem, LOGIN_ADMIN);
    hItem = WM_GetDialogItem(hWin, GUI_ID_DROPDOWN1);
    Dropdown_SetHZ24(hItem);

    DROPDOWN_AddString(hItem, GUI_LANG_GetTextEx(Lang_Disable, Language_Bit));
    DROPDOWN_AddString(hItem, GUI_LANG_GetTextEx(Lang_Pluse, Language_Bit));
    DROPDOWN_AddString(hItem, "CJ/T-188");
    DROPDOWN_AddString(hItem, "MODBUS");

    Login_Show(hItem, LOGIN_ADMIN);

    hItem = WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2);
    Dropdown_SetHZ24(hItem);

    DROPDOWN_AddString(hItem, GUI_LANG_GetTextEx(Lang_Disable, Language_Bit));
    DROPDOWN_AddString(hItem, GUI_LANG_GetTextEx(Lang_Pluse, Language_Bit));
    DROPDOWN_AddString(hItem, "MODBUS");
    DROPDOWN_AddString(hItem, GUI_LANG_GetTextEx(Lang_timing, Language_Bit));
    DROPDOWN_AddString(hItem, "XK3102");

    Login_Show(hItem, LOGIN_ADMIN);

    DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN0), ext_inf.din.pro_data[0].energy_sta);
    DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN1), ext_inf.din.pro_data[0].water_sta);
    DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2), ext_inf.din.pro_data[0].food_sta);


    WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT4));
    WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT9));
    WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT2));
    WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT5));

    CHECKBOX_SetState(WM_GetDialogItem(hWin, GUI_ID_CHECK0 ), ext_inf.din.egg_en[0]);

    if (room_type_egg_en() < 0) {
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT10));
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT11));
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_CHECK0));
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT6));
    }

    Update_Value(hWin);
}

static void Food_Unit_Get(void)
{
    float ftemp = Get_Input_Value() * 10;
    short stemp = (short)ftemp;



    if (stemp < 0) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 0;
    }

    ext_inf.din.pro_data[0].food_unit = stemp;
    Update_Value(Current_HWIN);
}

static void Food_Get(void)
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

    index_inf.food[0][PRO_DATA_ALL] = stemp * FOOD_RATE;
    Update_Value(Current_HWIN);
}

static void Egg_Get(void)
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

    index_inf.egg[0][PRO_DATA_ALL] = stemp;
    Update_Value(Current_HWIN);
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

        case GUI_ID_DROPDOWN0:
            if (NCode == WM_NOTIFICATION_SEL_CHANGED) {
                ext_inf.din.pro_data[0].energy_sta = DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN0));
                Update_Value(hWin);
            }

            break;

        case GUI_ID_DROPDOWN1:
            if (NCode == WM_NOTIFICATION_SEL_CHANGED) {
                ext_inf.din.pro_data[0].water_sta = DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN1));
                Update_Value(hWin);
            }

            break;

        case GUI_ID_DROPDOWN2:
            if (NCode == WM_NOTIFICATION_SEL_CHANGED) {
                ext_inf.din.pro_data[0].food_sta = DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2));
                Update_Value(hWin);
            }

            break;

        case GUI_ID_CHECK0:
            if (NCode == WM_NOTIFICATION_VALUE_CHANGED) {
                if (room_type_egg_en() < 0) {
                    ext_inf.din.egg_en[0] = 0;
                } else {
                    ext_inf.din.egg_en[0] = CHECKBOX_GetState(WM_GetDialogItem(hWin, GUI_ID_CHECK0 ));
                }

                if (ext_inf.din.egg_en[0] == STA_ON) {
                    ext_inf.din.pro_data[4].water_sta = STA_OFF;
                }
            }

            break;

        case GUI_ID_BUTTON2:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                ext_inf.din.pro_data[0].energy_sta = DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN0));
                ext_inf.din.pro_data[0].water_sta = DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN1));
                ext_inf.din.pro_data[0].food_sta = DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN2));

                if (ext_inf.din.pro_data[0].food_sta == FL_PULSE || ext_inf.din.pro_data[0].food_sta == FL_TIME) {
                    for (i = 1; i < PRO_DATA_MAX; i++) {
                        ext_inf.din.pro_data[i].food_sta = FL_DIS;
                    }
                }

                Config_Save(SINDEX_ID);

                if (Config_Save(SDIN_ID) < 0) {
                    MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_SAVE_ER));
                }

                Current_HWIN = WM_GetParent(hWin);
                GUI_EndDialog(hWin, 0);
            }

            break;

        case GUI_ID_BUTTON3:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                MDev_ProduceCur_Task(hWin);
            }

            break;

        case GUI_ID_BUTTON4:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                MDev_ProduceHis_Task(hWin, 0);
            }

            break;

        case GUI_ID_BUTTON5:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                if (ext_inf.din.pro_data[0].water_sta == WL_CJT_188) {
                    MDev_ModbusSet_Task(hWin, WATER_MODE_CJT);
                } else if (ext_inf.din.pro_data[0].water_sta == WL_PULSE) {
                    MDev_ProduceMore_Task(hWin, WATER_MODE_PULSE);
                } else {
                    MDev_ModbusSet_Task(hWin, WATER_MODE_MB);
                }
            }

            break;

        case GUI_ID_BUTTON6:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                MDev_ModbusSet_Task(hWin, FOOD_MODE_MB);
            }

            break;

        case GUI_ID_BUTTON7:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                MDev_ModbusSet_Task(hWin, ENERGY_MODE_MORE);
            }

            break;

//        case GUI_ID_EDIT0:
//            if (NCode == WM_NOTIFICATION_RELEASED) {
//                Set_Output_Callback(Water_Unit_Get);
//                Set_Input_Len(3);
//                Input_Task(hWin);
//            }

//            break;

        case GUI_ID_EDIT1:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(Food_Unit_Get);
                Set_Input_Len(4);
                Input_Task(hWin);
            }

            break;

//        case GUI_ID_EDIT2:
//            if (NCode == WM_NOTIFICATION_RELEASED) {

//                if (ext_inf.din.energy_sta == PL_DL645_97 || ext_inf.din.energy_sta == PL_DL645_07) {
//                    Set_Output_Callback(DL645_Addr_Get);
//                    Set_Input_Len(12);
//                } else {
//                    Set_Output_Callback(Modbus_Addr_Get);
//                    Set_Input_Len(3);
//                }

//                Input_Task(hWin);
//            }

//            break;

//        case GUI_ID_EDIT3:
//            if (NCode == WM_NOTIFICATION_RELEASED) {
//                Set_Output_Callback(Water_Get);
//                Set_Input_Len(8);
//                Input_Task(hWin);
//            }

//            break;

        case GUI_ID_EDIT4:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(Food_Get);
                Set_Input_Len(8);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT6:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(Egg_Get);
                Set_Input_Len(6);
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
WM_HWIN MDev_Produce;
void MDev_Produce_Task(WM_HWIN hParent)
{
    MDev_Produce = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    Current_HWIN = MDev_Produce;
}

