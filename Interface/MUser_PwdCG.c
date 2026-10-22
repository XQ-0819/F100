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
#include <string.h>
#include <stdio.h>
#include "trial_ver.h"
#include "pcf8563.h"

char new_pwd1[8], new_pwd2[8];
U8 pwd_type;//0 管理员 1 操作员 2 hide func修改注册使能
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
    { WINDOW_CreateIndirect,   "",           0,                        0, 0, 800, 480, 0, 0},
    { TEXT_CreateIndirect,      "密码修改",          GUI_ID_TEXT0,            250, 4, 300, 42, 0, 0},
    { CHECKBOX_CreateIndirect,  "",           			GUI_ID_CHECK0,            200, 130, 200, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "",           			GUI_ID_CHECK1,            400, 130, 200, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "",           			GUI_ID_CHECK2,            200, 300, 200, CHECK_SIZE, 0, 0},	
    { BUTTON_CreateIndirect,    "传感器量程",           GUI_ID_BUTTON3,         200, 180, 130, 50, 0, 0},
//    { BUTTON_CreateIndirect,    "版本清除",             GUI_ID_BUTTON4,         400, 180, 130, 50, 0, 0},
    { TEXT_CreateIndirect,      "试用时间/天:",           GUI_ID_TEXT3,         200, 250, 160, 40, 0, 0},
    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT2,              400, 250, 130, 40, 0, 0},
    { TEXT_CreateIndirect,      "",           			GUI_ID_TEXT4,         200, 340, 250, 51, 0, 0},


    { TEXT_CreateIndirect,      "新密码:",           GUI_ID_TEXT1,            200, 130, 160, 51, 0, 0},
    { TEXT_CreateIndirect,      "确认密码:",           GUI_ID_TEXT2,            200, 220, 160, 51, 0, 0},
    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT0,              385, 130, 160, 51, 0, 0},
    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT1,              385, 220, 160, 51, 0, 0},
//    { BUTTON_CreateIndirect,    "上一页",          GUI_ID_BUTTON0,          MENU_BACK_X1, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
    { BUTTON_CreateIndirect,    "确定",          	GUI_ID_BUTTON1,          MENU_BACK_X2, MENU_BACK_Y, 120, 50, 0, 0},
    { BUTTON_CreateIndirect,    "返回",             GUI_ID_BUTTON2,         MENU_BACK_X3, MENU_BACK_Y, 120, 50, 0, 0},
};
/*****************************************************************
**      FunctionName:void PaintDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**
**      call this function in _cbCallback --> WM_PAINT
*****************************************************************/

static void PaintDialog(WM_MESSAGE * pMsg)
{
    Show_MBackPic();

    GUI_SetColor(GUI_LINE_COLOR);
    GUI_DrawRoundedRect(160, 90, 600, 360, 15);
    GUI_DrawRoundedRect(157, 87, 603, 363, 15);
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


    for (MDev_ID = GUI_ID_BUTTON1; MDev_ID <= GUI_ID_BUTTON3; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Button_SetHZ24(hItem);
    }

    for (MDev_ID = GUI_ID_TEXT0; MDev_ID <= GUI_ID_TEXT4; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24L(hItem);

        if (MDev_ID == GUI_ID_TEXT0) {
            TEXT_SetTextAlign(hItem,  GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetTextColor(hItem, GUI_WHITE);
        }
    }

    hItem = WM_GetDialogItem(hWin, GUI_ID_EDIT0);
    Edit_SetHZ24(hItem);
    hItem = WM_GetDialogItem(hWin, GUI_ID_EDIT1);
    Edit_SetHZ24(hItem);
    hItem = WM_GetDialogItem(hWin, GUI_ID_EDIT2);
    Edit_SetHZ24(hItem);

    if (pwd_type == 2) {
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT1));
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT2));
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT0));
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT1));

        Check_SetHZ24(WM_GetDialogItem(hWin, GUI_ID_CHECK0));
        Check_SetHZ24(WM_GetDialogItem(hWin, GUI_ID_CHECK1));
        sprintf(show_buf, "%d", s_tCodeInf.wDay);
        EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT2), show_buf);

        if (s_tCodeInf.wDay != 0) {
            sprintf(show_buf, GUI_LANG_GetTextEx(Lang_UsedDays, Language_Bit), s_tCodeInf.dUsedHour * 1.0 / DAY_TO_HOUR);
            TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT4), show_buf);
        }

        if (soft_inf.creg_en == STA_ON) {
            CHECKBOX_SetState(WM_GetDialogItem(hWin, GUI_ID_CHECK0), 1);
        } else {
            CHECKBOX_SetState(WM_GetDialogItem(hWin, GUI_ID_CHECK0), 0);
        }

        if (soft_inf.room_mode == ROOM_MODE_SOW) {
            CHECKBOX_SetState(WM_GetDialogItem(hWin, GUI_ID_CHECK1), 1);
        } else {
            CHECKBOX_SetState(WM_GetDialogItem(hWin, GUI_ID_CHECK1), 0);
        }
		
        if (soft_inf.update_pro == PRO_HL) {
            CHECKBOX_SetState(WM_GetDialogItem(hWin, GUI_ID_CHECK2 ), 1);
        } else {
            CHECKBOX_SetState(WM_GetDialogItem(hWin, GUI_ID_CHECK2 ), 0);
        }

    } else {
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_CHECK0));
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_CHECK1));
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_CHECK2));
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON3));
        //        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON4));
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT3));
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT2));
    }

    if (pwd_type == 2) {
        TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT0), GUI_LANG_GetTextEx(Lang_Program_Selection, Language_Bit)); // 程序选择
        CHECKBOX_SetText(WM_GetDialogItem(hWin, GUI_ID_CHECK0), GUI_LANG_GetTextEx(Lang_Register, Language_Bit));     // 启用注册
        CHECKBOX_SetText(WM_GetDialogItem(hWin, GUI_ID_CHECK1), GUI_LANG_GetTextEx(Lang_Pig_Version, Language_Bit));  // 养猪版本
        Check_SetHZ24(WM_GetDialogItem(hWin, GUI_ID_CHECK2));
        CHECKBOX_SetText(WM_GetDialogItem(hWin, GUI_ID_CHECK2), "HL PRO");
    } else {
        TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT0), GUI_LANG_GetTextEx(Lang_Change_Pwd, Language_Bit)); // 密码修改
        sprintf(show_buf, "%s:", GUI_LANG_GetTextEx(Lang_New_Password, Language_Bit));
        TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT1), show_buf); // 新密码:
        sprintf(show_buf, "%s:", GUI_LANG_GetTextEx(Lang_Ack_Password, Language_Bit));
        TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT2), show_buf); // 确认密码:
    }

    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT3), GUI_LANG_GetTextEx(Lang_Trial_Time, Language_Bit));       // 试用时间/天:
    BUTTON_SetText(WM_GetDialogItem(hWin, GUI_ID_BUTTON1), GUI_LANG_GetTextEx(Lang_Done, Language_Bit));         // 确定
    BUTTON_SetText(WM_GetDialogItem(hWin, GUI_ID_BUTTON2), GUI_LANG_GetTextEx(Lang_Return, Language_Bit));       // 返回
    BUTTON_SetText(WM_GetDialogItem(hWin, GUI_ID_BUTTON3), GUI_LANG_GetTextEx(Lang_Seneor_Range, Language_Bit)); // 传感器量程
    //        BUTTON_SetText(WM_GetDialogItem(hWin, GUI_ID_BUTTON4), "Clear DevTpye");

    WM_DisableWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT2));
}


static void Password_Get1(void)
{
    unsigned char *time_buf = Get_Input_Text();
    unsigned char len = strlen((const char *)time_buf);
    unsigned char i = 0;

    if (len > 6)len = 6;

    strcpy(new_pwd1, (const char *)time_buf);

    for (i = 0; i < len; i++) {
        show_buf[i] = '*';
    }

    show_buf[i] = 0;
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, GUI_ID_EDIT0), show_buf);
}

static void Password_Get2(void)
{
    unsigned char *time_buf = Get_Input_Text();
    unsigned char len = strlen((const char *)time_buf);
    unsigned char i = 0;

    if (len > 6)len = 6;

    strcpy(new_pwd2, (const char *)time_buf);

    for (i = 0; i < len; i++) {
        show_buf[i] = '*';
    }

    show_buf[i] = 0;
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, GUI_ID_EDIT1), show_buf);
}

static void Trial_Day_Set(void)
{
    float ftemp = Get_Input_Value();
    unsigned short stemp = (unsigned short)ftemp;

//    if (stemp > 255) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = 255;
//    }

//    if (stemp < 0) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = 0;
//    }

    Trial_Clear(stemp);
    sprintf(show_buf, "%d",  s_tCodeInf.wDay);
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, GUI_ID_EDIT2), show_buf);
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

        case GUI_ID_EDIT0:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(Password_Get1);
                Set_Input_Len(6);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT1:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(Password_Get2);
                Set_Input_Len(6);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT2:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(Trial_Day_Set);
                Set_Input_Len(3);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_BUTTON1:
            if (NCode == WM_NOTIFICATION_RELEASED) {

                if (pwd_type == 2) {
                    soft_inf.creg_en = CHECKBOX_GetState(WM_GetDialogItem(hWin, GUI_ID_CHECK0));
                    soft_inf.room_mode = CHECKBOX_GetState(WM_GetDialogItem(hWin, GUI_ID_CHECK1));
                    soft_inf.update_pro = CHECKBOX_GetState(WM_GetDialogItem(hWin, GUI_ID_CHECK2));

                    if (Config_Save(SSOFT_ID) < 0) {
                        MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_SAVE_ER));
                        break;
                    }
                    MWarning_Task(WM_GetParent(hWin), GUI_LANG_GetTextEx(Lang_SET_Successfully, Language_Bit));
                    Current_HWIN = WM_GetParent(hWin);
                    GUI_EndDialog(hWin, 0);
                    break;
                }

                if (strcmp((const char *)new_pwd1, new_pwd2) == 0) {
                    if (pwd_type == 0) {
                        strcpy(basic_inf.supadmin_pwd, (const char *)new_pwd1);
                    } else {
                        strcpy(basic_inf.admin_pwd, (const char *)new_pwd1);
                    }

                    if (Config_Save(SBASIC_ID) < 0) {
                        MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_SAVE_ER));
                        break;
                    }
                    MWarning_Task(WM_GetParent(hWin), GUI_LANG_GetTextEx(Lang_Tips12, Language_Bit));
                    Current_HWIN = WM_GetParent(hWin);
                    GUI_EndDialog(hWin, 0);
                } else {
                    MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_PWD_ER));
                }
            }

            break;

        case GUI_ID_BUTTON2:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Current_HWIN = WM_GetParent(hWin);
                GUI_EndDialog(hWin, 0);
            }

            break;

        case GUI_ID_BUTTON3:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                MSys_SensorRG_Task(hWin);
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
WM_HWIN MUser_PwdCG;
void MUser_PwdCG_Task(WM_HWIN hParent, U8 id)
{
    pwd_type = id;
    MUser_PwdCG = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    Current_HWIN = MUser_PwdCG;
    memset(new_pwd1, 0, sizeof(new_pwd1));
    memset(new_pwd2, 0, sizeof(new_pwd2));
}

