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
#include "stdio.h"
#include <string.h>


//EventsFunctionList
//EndofEventsFunctionList


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

static WM_HTIMER hTimer;
#define Y			(60)
#define Y_Space		(50)


/*********************************************************************
*
*       Dialog resource
*
* This table conatins the info required to create the dialog.
* It has been created by ucGUIbuilder.
*/

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect,    "",               0,                       0,  0,  800, 480, 0, 0},
    { TEXT_CreateIndirect,      "设备管理-风机检测",          GUI_ID_TEXT0,           250, 4, 300, 42, 0, 0},

    { CHECKBOX_CreateIndirect,  "启用",              GUI_ID_CHECK0,           100, Y + 3, 300, CHECK_SIZE, 0, 0},

    { TEXT_CreateIndirect,      "报警阈值:",            GUI_ID_TEXT1,            410, Y, 160, 36, 0, 0},
    { TEXT_CreateIndirect,      "电流最大差值:",    	GUI_ID_TEXT2,            100, Y + Y_Space * 1, 160, 36, 0, 0},
    { TEXT_CreateIndirect,      "偏差电流:",            GUI_ID_TEXT10,            410, Y + Y_Space * 3, 250, 36, 0, 0},
    { TEXT_CreateIndirect,      "电表地址:",            GUI_ID_TEXT11,            100, Y + Y_Space * 2, 250, 36, 0, 0},
    { TEXT_CreateIndirect,      "CT变比:",              GUI_ID_TEXT12,            410, Y + Y_Space * 2, 250, 36, 0, 0},
    { TEXT_CreateIndirect,      "误差电流:",            GUI_ID_TEXT13,            410, Y + Y_Space * 1, 250, 36, 0, 0},

    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT0,            560, Y, 120, 36, 0, 0},
    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT1,            270, Y + Y_Space * 1, 120, 36, 0, 0},
    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT2,            560, Y + Y_Space * 3, 120, 36, 0, 0},
    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT3,            200, Y + Y_Space * 2, 190, 36, 0, 0},
    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT4,            560, Y + Y_Space * 2, 120, 36, 0, 0},
    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT5,            560, Y + Y_Space * 1, 120, 36, 0, 0},


    { TEXT_CreateIndirect,      "",          GUI_ID_TEXT3,            100, Y + Y_Space * 3, 300, 30, 0, 0},
    { TEXT_CreateIndirect,      "",          GUI_ID_TEXT4,            100, Y + Y_Space * 4, 200, 30, 0, 0},
    { TEXT_CreateIndirect,      "",          GUI_ID_TEXT5,            100, Y + Y_Space * 5, 200, 30, 0, 0},
    { TEXT_CreateIndirect,      "",          GUI_ID_TEXT6,            100, Y + Y_Space * 6, 200, 30, 0, 0},
    { TEXT_CreateIndirect,      "",          GUI_ID_TEXT7,            410, Y + Y_Space * 4, 200, 30, 0, 0},
    { TEXT_CreateIndirect,      "",          GUI_ID_TEXT8,            410, Y + Y_Space * 5, 200, 30, 0, 0},
    { TEXT_CreateIndirect,      "",          GUI_ID_TEXT9,            410, Y + Y_Space * 6, 200, 30, 0, 0},

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
    GUI_DrawRoundedRect(70, 55, 730, 396, 15);
    GUI_DrawRoundedRect(67, 52, 733, 399, 15);
}

static void Update_Value(WM_HWIN hWin)
{
    int i = 0;

    sprintf(show_buf, "%d%%", dev_inf.fan_chk.percent);
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT0 ), show_buf);

    sprintf(show_buf, "%.1f A", dev_inf.fan_chk.phase_cur * 1.0f / CUR_RATE);
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT1 ), show_buf);

    sprintf(show_buf, "%.1f A", dev_inf.fan_chk.offset_cur * 1.0f / (float) TEMP_RATE);
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT2 ), show_buf);

    if (ext_inf.din.pro_data[0].energy_sta == PL_DL645_97 || ext_inf.din.pro_data[0].energy_sta == PL_DL645_07) {
        for (i = 0; i < sizeof(dev_inf.fan_chk.energy_addr); i++) {
            if (dev_inf.fan_chk.energy_addr[i] < 0x30 || dev_inf.fan_chk.energy_addr[i] > 0x39) {
                break;
            }
        }

        memcpy(show_buf, dev_inf.fan_chk.energy_addr, sizeof(dev_inf.fan_chk.energy_addr));
        show_buf[i] = 0;
        EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT3 ), show_buf);

    } else {
        sprintf(show_buf, "%d", dev_inf.fan_chk.energy_addr[0]);
        EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT3 ), show_buf);
    }

    sprintf(show_buf, "%d", dev_inf.fan_chk.energy_ct);
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT4 ), show_buf);

    sprintf(show_buf, "%.1f A", dev_inf.fan_chk.error_cur * 1.0f / (float) TEMP_RATE);
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT5 ), show_buf);
}

static void Update_CurValue(WM_HWIN hWin)
{
	int id = GUI_ID_TEXT3;

    sprintf(show_buf, "%s:%.2f kwh", GUI_LANG_GetTextEx(Lang_Electro, Language_Bit), (float)(sta_in.ele[ENERGY_FAN][MB_ENERGY] * 1.0f / ENERG_RATE)); // 用电量

    TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *)show_buf);

    sprintf(show_buf, "A.%s:%.1f V", GUI_LANG_GetTextEx(Lang_ABCVoltage, Language_Bit), (float)(sta_in.ele[ENERGY_FAN][MB_VA] * 1.0f / ENERG_PARA_RATE));
    TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *)show_buf);

    sprintf(show_buf, "B.%s:%.1f V", GUI_LANG_GetTextEx(Lang_ABCVoltage, Language_Bit), (float)(sta_in.ele[ENERGY_FAN][MB_VB] * 1.0f / ENERG_PARA_RATE));
    TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *)show_buf);

    sprintf(show_buf, "C.%s:%.1f V", GUI_LANG_GetTextEx(Lang_ABCVoltage, Language_Bit), (float)(sta_in.ele[ENERGY_FAN][MB_VC] * 1.0f / ENERG_PARA_RATE));
    TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *)show_buf);

    sprintf(show_buf, "A.%s:%.1f A", GUI_LANG_GetTextEx(Lang_ABCCurrent, Language_Bit), (float)(sta_in.ele[ENERGY_FAN][MB_IA] * 1.0f / ENERG_PARA_RATE));
    TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *)show_buf);

    sprintf(show_buf, "B.%s:%.1f A", GUI_LANG_GetTextEx(Lang_ABCCurrent, Language_Bit), (float)(sta_in.ele[ENERGY_FAN][MB_IB] * 1.0f / ENERG_PARA_RATE));
    TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *)show_buf);

    sprintf(show_buf, "C.%s:%.1f A", GUI_LANG_GetTextEx(Lang_ABCCurrent, Language_Bit), (float)(sta_in.ele[ENERGY_FAN][MB_IC] * 1.0f / ENERG_PARA_RATE));
    TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *)show_buf);
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

    for (MDev_ID = GUI_ID_EDIT0; MDev_ID <= GUI_ID_EDIT5; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Edit_SetHZ24(hItem);
    }
	
    EDIT_SetMaxLen(WM_GetDialogItem(hWin, GUI_ID_EDIT3), 15);

    for (MDev_ID = GUI_ID_TEXT0; MDev_ID <= GUI_ID_TEXT9; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24L(hItem);

        if (MDev_ID == GUI_ID_TEXT0) {
            TEXT_SetTextAlign(hItem,  GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetTextColor(hItem, GUI_WHITE);
        }
    }

    for (MDev_ID = GUI_ID_TEXT10; MDev_ID <= GUI_ID_TEXT13; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24L(hItem);
    }

    for (MDev_ID = GUI_ID_CHECK0; MDev_ID <= GUI_ID_CHECK0; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        CHECKBOX_SetText(hItem, GUI_LANG_GetTextEx(Lang_Enable_Fan_Current, Language_Bit));//启用风机电流检测
        Check_SetHZ24(hItem);
        Login_Show(hItem, LOGIN_SUPADMIN);
    }

    WM_MoveWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT0), 20, 0);
    WM_MoveWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT2), 20, 0);
    WM_MoveWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT4), 20, 0);
    WM_MoveWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT5), 20, 0);

    sprintf(show_buf, "%s-%s", GUI_LANG_GetTextEx(Lang_Device, Language_Bit), GUI_LANG_GetTextEx(Lang_detction_fan_curr, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT0), show_buf);                                               // 设备管理-风机检测
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT1), GUI_LANG_GetTextEx(Lang_Fan_I_Ala, Language_Bit));       // 报警阈值
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT2), GUI_LANG_GetTextEx(Lang_Max_Diff_I, Language_Bit));      // 电流最大差值
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT10), GUI_LANG_GetTextEx(Lang_Max_diff_Fan_I, Language_Bit)); // 偏差电流
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT11),  GUI_LANG_GetTextEx(Lang_Ammeter_Addr, Language_Bit));  // 电表地址
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT12), GUI_LANG_GetTextEx(Lang_CT_Ratio, Language_Bit)); // CT变比
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT13), GUI_LANG_GetTextEx(Lang_Fan_err, Language_Bit));  // 误差电流

    hTimer = WM_CreateTimer(WM_GetClientWindow(hWin), 0, 1000, 0);
    CHECKBOX_SetState(WM_GetDialogItem(hWin, GUI_ID_CHECK0), dev_inf.fan_chk.enable);
    Update_Value(hWin);
    Update_CurValue(hWin);
}


static void Percent_Get(void)
{
    float ftemp = Get_Input_Value();
    short stemp = (short)ftemp;

    if (stemp > MAX_PERCENT) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = MAX_PERCENT;
    }

    if (stemp < 0) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 0;
    }

    dev_inf.fan_chk.percent = stemp;
    Update_Value(Current_HWIN);
}

static void Phase_Cur_Get(void)
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

    dev_inf.fan_chk.phase_cur = stemp;
    Update_Value(Current_HWIN);
}

static void Offset_Cur_Get(void)
{
    float ftemp = Get_Input_Value() * CUR_RATE;
    short stemp = (short)ftemp;

    if (stemp > 127) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 127;
    }

    if (stemp < -127) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 0;
    }

    dev_inf.fan_chk.offset_cur = stemp;
    Update_Value(Current_HWIN);
}

static void Error_Cur_Get(void)
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

    dev_inf.fan_chk.error_cur = stemp;
    Update_Value(Current_HWIN);
}

static void DL645_Addr_Get(void)
{
    unsigned char *time_buf = Get_Input_Text();
    unsigned char len = strlen((const char *)time_buf);

    if (len == 0 || len > 12) {
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

    memcpy(dev_inf.fan_chk.energy_addr, time_buf, len);

    if (len != 12)dev_inf.fan_chk.energy_addr[len] = 0;

    Update_Value(Current_HWIN);
}

static void Modbus_Addr_Get(void)
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
    dev_inf.fan_chk.energy_addr[0] = stemp;
    Update_Value(Current_HWIN);
}

static void CT_Get(void)
{
    float ftemp = Get_Input_Value();
    unsigned short stemp = (unsigned short)ftemp;

    if (stemp > 255) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 255;
    }

    if (stemp <= 0) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 1;
    }

    dev_inf.fan_chk.energy_ct = stemp;
    Update_Value(Current_HWIN);
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

    case WM_TIMER:
        Update_CurValue(hWin);
        WM_RestartTimer(hTimer, 1000);
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

        case GUI_ID_EDIT0:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(Percent_Get);
                Set_Input_Len(3);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT1:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(Phase_Cur_Get);
                Set_Input_Len(4);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT2:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(Offset_Cur_Get);
                Set_Input_Len(4);
                Input_Task(hWin);
            }

            break;


        case GUI_ID_EDIT3:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                if (ext_inf.din.pro_data[0].energy_sta == PL_DL645_97 || ext_inf.din.pro_data[0].energy_sta == PL_DL645_07) {
                    Set_Output_Callback(DL645_Addr_Get);
                    Set_Input_Len(12);
                } else {
                    Set_Output_Callback(Modbus_Addr_Get);
                    Set_Input_Len(3);
                }

                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT4:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(CT_Get);
                Set_Input_Len(3);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT5:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(Error_Cur_Get);
                Set_Input_Len(3);
                Input_Task(hWin);
            }

            break;
			
        case GUI_ID_BUTTON2:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                dev_inf.fan_chk.enable = CHECKBOX_GetState(WM_GetDialogItem(hWin, GUI_ID_CHECK0));

                if (Config_Save(SFAN_CHK_ID) < 0) {
                    MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_SAVE_ER));
                }

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
WM_HWIN MDev_FanChk;
void MDev_FanChk_Task(WM_HWIN hParent)
{
    MDev_FanChk = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    Current_HWIN = MDev_FanChk;
}

