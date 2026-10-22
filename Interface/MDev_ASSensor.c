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

static WM_HTIMER hTimer;

/*********************************************************************
*
*       static data
*
**********************************************************************
*/

#define GUI_ID_TEXT10   (GUI_ID_USER+1)
#define GUI_ID_TEXT11   (GUI_ID_USER+2)

static U8 select_id = 0;

/*********************************************************************
*
*       Dialog resource
*
* This table conatins the info required to create the dialog.
* It has been created by ucGUIbuilder.
*/
#define XPos_Collec  (-20)

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect,    "",               0,                       0,  0,  800, 480, 0, 0},
    { TEXT_CreateIndirect,      "设备管理-环境信息",          GUI_ID_TEXT0,            250, 4, 300, 42, 0, 0},


    { TEXT_CreateIndirect,      "PM2.5:",      GUI_ID_TEXT1,           130 + XPos_Collec, 100, 250, 36, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",          GUI_ID_CHECK0,        350 + XPos_Collec, 100 + 3, 100, CHECK_SIZE, 0, 0},
    { TEXT_CreateIndirect,      "地址:",      GUI_ID_TEXT2,            470 + XPos_Collec, 100, 70, 36, 0, 0},
    { EDIT_CreateIndirect,      "",            GUI_ID_EDIT0,           540 + XPos_Collec, 100, 100, 36, 0, 0},

    { TEXT_CreateIndirect,      "PM2.5    超限报警:",      GUI_ID_TEXT5,           130 + XPos_Collec, 140, 250, 36, 0, 0},
    { EDIT_CreateIndirect,      "",            GUI_ID_EDIT2,           540 + XPos_Collec, 140, 100, 36, 0, 0},
    { TEXT_CreateIndirect,      "PM10     超限报警:",      GUI_ID_TEXT6,           130 + XPos_Collec, 180, 250, 36, 0, 0},
    { EDIT_CreateIndirect,      "",            GUI_ID_EDIT3,           540 + XPos_Collec, 180, 100, 36, 0, 0},

    { TEXT_CreateIndirect,      "光照:",      GUI_ID_TEXT3,            130 + XPos_Collec, 250, 250, 36, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",          GUI_ID_CHECK1,        350 + XPos_Collec, 250 + 3, 100, CHECK_SIZE, 0, 0},
    { TEXT_CreateIndirect,      "地址:",      GUI_ID_TEXT4,            470 + XPos_Collec, 250, 70, 36, 0, 0},
    { EDIT_CreateIndirect,      "",            GUI_ID_EDIT1,           540 + XPos_Collec, 250, 100, 36, 0, 0},

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
    GUI_DrawRoundedRect(90, 55, 690, 386, 10);
    GUI_DrawRoundedRect(86, 52, 694, 389, 10);

//    GUI_DrawLine(80, 260, 712, 260);
//    GUI_DrawLine(80, 264, 712, 264);
}
static void Set_OptValue(WM_HWIN hWin)
{
    U8 i = 0;

    //增加是否启用
    for (i = 0; i < MB_DEV_NUM; i++) {
        CHECKBOX_SetState(WM_GetDialogItem(hWin, GUI_ID_CHECK0 + i ), dev_inf.env_dev[i].enable);
    }
}

static void Update_Value(WM_HWIN hWin)
{
    U8 i = 0;
    sprintf(show_buf, "PM2.5: %d ug/%s", sta_in.pm2_5, GUI_LANG_GetTextEx(Lang_m3, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT1), show_buf);

    sprintf(show_buf, "%s : %d lux", GUI_LANG_GetTextEx(Lang_illumination, Language_Bit), sta_in.light);

    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT3), show_buf);

    sprintf(show_buf, "%d", dev_inf.limit.pm2_5_H);
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT2), show_buf);
	
    sprintf(show_buf, "%d", dev_inf.limit.pm10_H);
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT3), show_buf);

    for (i = 0; i < MB_DEV_NUM; i++) {
        sprintf(show_buf, "%d", dev_inf.env_dev[i].addr);
        EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT0 + i), show_buf);
    }
}

static void Get_OptValue(WM_HWIN hWin)
{
    U8 i = 0;

    for (i = 0; i < MB_DEV_NUM; i++) {
        dev_inf.env_dev[i].enable = CHECKBOX_GetState(WM_GetDialogItem(hWin, GUI_ID_CHECK0 + i ));
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

    for (MDev_ID = GUI_ID_EDIT0; MDev_ID <= GUI_ID_EDIT3; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Edit_SetHZ24(hItem);
    }

    for (MDev_ID = GUI_ID_CHECK0; MDev_ID <= GUI_ID_CHECK1; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        CHECKBOX_SetText(hItem, GUI_LANG_GetTextEx(Lang_Enable, Language_Bit));
        Check_SetHZ24(hItem);
        Login_Show(hItem, LOGIN_ADMIN);
    }

    for (MDev_ID = GUI_ID_TEXT0; MDev_ID <= GUI_ID_TEXT6; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24L(hItem);

        if (MDev_ID == GUI_ID_TEXT0) {
            TEXT_SetTextAlign(hItem,  GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetTextColor(hItem, GUI_WHITE);
        }
    }

    sprintf(show_buf, "%s-%s", GUI_LANG_GetTextEx(Lang_Device, Language_Bit), GUI_LANG_GetTextEx(Lang_Environment, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT0), show_buf); // 设备管理-环境信息
    sprintf(show_buf, "%s:", GUI_LANG_GetTextEx(Lang_PM2_5_HA, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT5), show_buf); // PM2.5    高报警:
    sprintf(show_buf, "%s:", GUI_LANG_GetTextEx(Lang_PM10_HA, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT6), show_buf); // PM10     高报警:

    sprintf(show_buf, "%s:", GUI_LANG_GetTextEx(Lang_Addr, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT4), show_buf); // 地址:
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT2), show_buf); // 地址:

    hTimer = WM_CreateTimer(WM_GetClientWindow(hWin), 0, 1000, 0);
    Set_OptValue(hWin);
    Update_Value(hWin);
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
    dev_inf.env_dev[select_id].addr = stemp;
    Update_Value(Current_HWIN);
}

static void PM2_5_H_Get(void)
{
    float ftemp = Get_Input_Value();
    signed short stemp = (signed short)ftemp;

//    if (stemp > 255) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = 255;
//    }

//    if (stemp < 0) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = 0;
//    }

    dev_inf.limit.pm2_5_H = stemp;
    Update_Value(Current_HWIN);
}

static void PM10_H_Get(void)
{
    float ftemp = Get_Input_Value();
    signed short stemp = (signed short)ftemp;

//    if (stemp > 255) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = 255;
//    }

//    if (stemp < 0) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = 0;
//    }

    dev_inf.limit.pm10_H = stemp;
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

    case WM_TIMER:
        Update_Value(hWin);
        WM_RestartTimer(hTimer, 1000);
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

        case GUI_ID_CHECK0:
        case GUI_ID_CHECK1:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Get_OptValue(hWin);
            }

            break;

        case GUI_ID_BUTTON2:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Get_OptValue(hWin);

                if (Config_Save(SENV_ID) < 0) {
                    MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_SAVE_ER));
                }

                Current_HWIN = WM_GetParent(hWin);
                GUI_EndDialog(hWin, 0);
            }

            break;

        case GUI_ID_EDIT0:
        case GUI_ID_EDIT1:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                select_id = Id - GUI_ID_EDIT0;
                Set_Output_Callback(Modbus_Addr_Get);
                Set_Input_Len(3);
                Input_Task(hWin);
            }

            break;
			
        case GUI_ID_EDIT2:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(PM2_5_H_Get);
                Set_Input_Len(3);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT3:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(PM10_H_Get);
                Set_Input_Len(3);
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
WM_HWIN MDev_ASSensor;
void MDev_ASSensor_Task(WM_HWIN hParent)
{
    MDev_ASSensor = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    Current_HWIN = MDev_ASSensor;
}

