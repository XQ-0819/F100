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


//EventsFunctionList
//EndofEventsFunctionList


/*********************************************************************
*
*       static data
*
**********************************************************************
*/



/*********************************************************************
*
*       Dialog resource
*
* This table conatins the info required to create the dialog.
* It has been created by ucGUIbuilder.
*/


static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect,    "",               0,                       0,  0,  800, 480, 0, 0},
    { TEXT_CreateIndirect,      "房间设置",          GUI_ID_TEXT0,           250, 4, 300, 42, 0, 0},

    { TEXT_CreateIndirect,      "横截面积/m2:", 	 GUI_ID_TEXT4,          200, 80, 160, 36, 0, 0},
    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT2,            360, 80, 90, 36, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",              GUI_ID_CHECK0,           470, 80 + 3, 200, CHECK_SIZE, 0, 0},


    { TEXT_CreateIndirect,      "房间状态:",            GUI_ID_TEXT5,            200, 150, 250, 36, 0, 0},

    { TEXT_CreateIndirect,      "房间类型:",            GUI_ID_TEXT1,            200, 220, 160, 36, 0, 0},
    { TEXT_CreateIndirect,      "房间编号:",            GUI_ID_TEXT2,            200, 290, 160, 36, 0, 0},
    { TEXT_CreateIndirect,      "设备ID:",            GUI_ID_TEXT3,            200, 360, 250, 36, 0, 0},

    { DROPDOWN_CreateIndirect,	 "",			 GUI_ID_DROPDOWN0,			360, 220, 140, 160, 0, 0},
    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT0,            360, 290, 120, 36, 0, 0},
    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT1,            360, 360, 120, 36, 0, 0},

    { BUTTON_CreateIndirect,    "正常",          	 GUI_ID_BUTTON3,          360, 	150, 122, 50, 0, 0},

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
//	WM_HWIN hWin = pMsg->hWin;
    Show_MBackPic();
    GUI_SetColor(GUI_LINE_COLOR);
    GUI_DrawRoundedRect(160, 65, 670, 410, 15);
    GUI_DrawRoundedRect(157, 62, 673, 413, 15);
}


static void Room_Get(void)
{
    float ftemp = Get_Input_Value();
    unsigned short stemp = (unsigned short)ftemp;

//    if (stemp > TEMP_ADJMAX) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = TEMP_ADJMAX;
//    }

//    if (stemp < 0) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = 0;
//    }

    basic_inf.room = stemp;
    sprintf(show_buf, "%d", basic_inf.room);
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, GUI_ID_EDIT0), show_buf);
}
static void Id_Get(void)
{
    float ftemp = Get_Input_Value();
    unsigned long stemp = (unsigned long)ftemp;

    if (stemp > ID_MAX) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = ID_MAX;
    }

//    if (stemp < 1) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = 1;
//    }
    if (basic_inf.id != stemp) {
        sys_sta.net_creg = STA_OFF;
    }

    basic_inf.id = stemp;
    sprintf(show_buf, "%d", basic_inf.id);
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, GUI_ID_EDIT1), show_buf);
}

static void WindArea_Get(void)
{
    float ftemp = Get_Input_Value() * WIND_RATE;
    int stemp = (int)ftemp;

    if (stemp > TIME_4BIT) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = TIME_4BIT;
    }

    if (stemp < 0) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 0;
    }

    basic_inf.wind_area = stemp;

    sprintf(show_buf, "%.1f", basic_inf.wind_area * 1.0 / WIND_RATE);
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, GUI_ID_EDIT2), show_buf);
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


    for (MDev_ID = GUI_ID_BUTTON2; MDev_ID <= GUI_ID_BUTTON2; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, Cut_cbButton);
    }

    for (MDev_ID = GUI_ID_BUTTON3; MDev_ID <= GUI_ID_BUTTON3; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Button_SetHZ24(hItem);
    }

    for (MDev_ID = GUI_ID_EDIT0; MDev_ID <= GUI_ID_EDIT2; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Edit_SetHZ24(hItem);
    }

    for (MDev_ID = GUI_ID_TEXT0; MDev_ID <= GUI_ID_TEXT5; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24L(hItem);

        if (MDev_ID == GUI_ID_TEXT0) {
            TEXT_SetTextAlign(hItem,  GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetTextColor(hItem, GUI_WHITE);
        }
    }

    for (MDev_ID = GUI_ID_DROPDOWN0; MDev_ID <= GUI_ID_DROPDOWN0; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Dropdown_SetHZ24(hItem);

        if (soft_inf.room_mode == ROOM_MODE_SOW) {
            for (i = 0; i < ROOM_TYPE_CHICKEN_ID; i++) {
                    DROPDOWN_AddString(hItem, GUI_LANG_GetTextEx(room_type[i], Language_Bit));
            }
        } else {
            for (i = ROOM_TYPE_CHICKEN_ID; i < ROOM_TYPE_NUM; i++) {
                DROPDOWN_AddString(hItem, GUI_LANG_GetTextEx(room_type[i], Language_Bit));
            }
        }

        Login_Show(hItem, LOGIN_SUPADMIN);
    }

    for (MDev_ID = GUI_ID_CHECK0; MDev_ID <= GUI_ID_CHECK0; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Check_SetHZ24(hItem);
        Login_Show(hItem, LOGIN_SUPADMIN);
    }

    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT0), GUI_LANG_GetTextEx(Lang_Farmhouse_Info, Language_Bit)); // 房间设置
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT1), GUI_LANG_GetTextEx(Lang_Farmhouse_Type, Language_Bit));   // 房间类型:
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT2), GUI_LANG_GetTextEx(Lang_Farmhouse_Number, Language_Bit));   // 房间编号:
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT3), GUI_LANG_GetTextEx(Lang_Device_ID, Language_Bit));          // 设备ID:
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT4), GUI_LANG_GetTextEx(Lang_CSA, Language_Bit));                // 横截面积/m2:
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT5), GUI_LANG_GetTextEx(Lang_Breeding_status, Language_Bit));    // 房间状态:
    CHECKBOX_SetText(WM_GetDialogItem(hWin, GUI_ID_CHECK0), GUI_LANG_GetTextEx(Lang_Apparent_Temp, Language_Bit)); // 启用体感温度

    if (soft_inf.room_mode == ROOM_MODE_SOW) {
        DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN0 ), basic_inf.type);
    } else {
        DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN0 ), basic_inf.type - ROOM_TYPE_CHICKEN_ID);
    }

    CHECKBOX_SetState(WM_GetDialogItem(hWin, GUI_ID_CHECK0), basic_inf.humtemp_en);

    sprintf(show_buf, "%d", basic_inf.room);
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT0), show_buf);

    sprintf(show_buf, "%d", basic_inf.id);
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT1), show_buf);

    sprintf(show_buf, "%.1f", basic_inf.wind_area * 1.0 / WIND_RATE);
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT2), show_buf);

    if (ctrl_inf.var.mode == STA_ON) {
        BUTTON_SetText(WM_GetDialogItem(hWin, GUI_ID_BUTTON3), GUI_LANG_GetTextEx(Lang_Culture, Language_Bit));

    } else {
        BUTTON_SetText(WM_GetDialogItem(hWin, GUI_ID_BUTTON3), GUI_LANG_GetTextEx(Lang_Empty, Language_Bit));
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
                && get_login_permiss(LOGIN_SUPADMIN) < 0) {
            MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_NO_LOGIN));
            break;
        }

        if (Id >= GUI_ID_BUTTON3 && Id <= GUI_ID_BUTTON3 && NCode == WM_NOTIFICATION_RELEASED
                && get_login_permiss(LOGIN_SUPADMIN) < 0) {
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
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(Room_Get);
                Set_Input_Len(3);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT1:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(Id_Get);
                Set_Input_Len(5);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT2:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(WindArea_Get);
                Set_Input_Len(5);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_BUTTON2:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                if (soft_inf.room_mode == ROOM_MODE_SOW) {
                    basic_inf.type =  DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN0 ));
                } else {
                    basic_inf.type =  DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN0 )) + ROOM_TYPE_CHICKEN_ID;
                }

                basic_inf.humtemp_en = CHECKBOX_GetState(WM_GetDialogItem(hWin, GUI_ID_CHECK0));

                if (Config_Save(SBASIC_ID) < 0) {
                    MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_SAVE_ER));
                }

                if (Config_Save(SVAR_ID) < 0) {
                    MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_SAVE_ER));
                }

                Current_HWIN = WM_GetParent(hWin);
                GUI_EndDialog(hWin, 0);
            }

            break;

        case GUI_ID_BUTTON3:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                ctrl_inf.var.mode = (ctrl_inf.var.mode + 1) % 2;
                if (ctrl_inf.var.mode == STA_ON) {
                    BUTTON_SetText(WM_GetDialogItem(hWin, GUI_ID_BUTTON3), GUI_LANG_GetTextEx(Lang_Culture, Language_Bit));

                } else {
                    BUTTON_SetText(WM_GetDialogItem(hWin, GUI_ID_BUTTON3), GUI_LANG_GetTextEx(Lang_Empty, Language_Bit));
                }
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
WM_HWIN MSys_Room;
void MSys_Room_Task(WM_HWIN hParent)
{
    MSys_Room = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    Current_HWIN = MSys_Room;
}

