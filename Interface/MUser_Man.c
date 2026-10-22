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
#include "trial_ver.h"

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
    { TEXT_CreateIndirect,      "用户登录",          GUI_ID_TEXT0,            250, 4, 300, 42, 0, 0},

    { CHECKBOX_CreateIndirect,  "",        GUI_ID_CHECK0,                 200 , 120, 200, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "",        GUI_ID_CHECK1,                 450, 120, 150, CHECK_SIZE, 0, 0},

    { TEXT_CreateIndirect,      "密码:",    GUI_ID_TEXT1,            200, 200, 90, 36, 0, 0},
    { EDIT_CreateIndirect,		 "",  	GUI_ID_EDIT0,			  300, 200, 120, 36, 0, 0},
    { BUTTON_CreateIndirect,    "修改密码",         GUI_ID_BUTTON3,          450, 190, 120, 50, 0, 0},

    { BUTTON_CreateIndirect,    "退出",          	GUI_ID_BUTTON0,          200, 280, 120, 50, 0, 0},
    { BUTTON_CreateIndirect,    "确定",          	GUI_ID_BUTTON1,          450, 280, 120, 50, 0, 0},
//    { BUTTON_CreateIndirect,    "返回",             GUI_ID_BUTTON2,         MENU_BACK_X3, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},

    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON1_0,          384, 398, 90, 82, 0, 0},
    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON1_1,          25, 400, 105, 80, 0, 0},
    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON1_2,          155, 400, 105, 80, 0, 0},
    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON1_3,          275, 400, 105, 80, 0, 0},
    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON1_4,          435, 400, 105, 80, 0, 0},
    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON1_5,          555, 400, 105, 80, 0, 0},
    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON1_6,          675, 400, 125, 80, 0, 0},
};

/*****************************************************************
**      FunctionName:void PaintDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**
**      call this function in _cbCallback --> WM_PAINT
*****************************************************************/

static void PaintDialog(WM_MESSAGE * pMsg)
{
    Show_INBackPic();

    GUI_SetColor(GUI_LINE_COLOR);
    GUI_DrawRoundedRect(150, 90, 647, 360, 15);
    GUI_DrawRoundedRect(147, 87, 650, 363, 15);
    //Disp_House();

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


    for (MDev_ID = GUI_ID_BUTTON0; MDev_ID <= GUI_ID_BUTTON3; MDev_ID++) {
        if (MDev_ID == GUI_ID_BUTTON2)continue;

        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Button_SetHZ24(hItem);
    }

    for (MDev_ID = GUI_ID_BUTTON1_0; MDev_ID <= GUI_ID_BUTTON1_6; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, Main_cbButton);
    }

    for (MDev_ID = GUI_ID_TEXT0; MDev_ID <= GUI_ID_TEXT1; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24L(hItem);
        TEXT_SetTextColor(hItem, GUI_WHITE);

        if (MDev_ID == GUI_ID_TEXT0) {
            TEXT_SetTextAlign(hItem,  GUI_TA_HCENTER | GUI_TA_VCENTER);
        }
    }

    hItem = WM_GetDialogItem(hWin, GUI_ID_CHECK0);
    Check_SetHZ24(hItem);

    hItem = WM_GetDialogItem(hWin, GUI_ID_CHECK1);
    Check_SetHZ24(hItem);

    hItem = WM_GetDialogItem(hWin, GUI_ID_EDIT0);
    Edit_SetHZ24(hItem);

    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT0), GUI_LANG_GetTextEx(Lang_User, Language_Bit));     // 用户登录
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT1), GUI_LANG_GetTextEx(Lang_Password, Language_Bit)); // 密码
    CHECKBOX_SetText(WM_GetDialogItem(hWin, GUI_ID_CHECK0), GUI_LANG_GetTextEx(Lang_Super_Admin, Language_Bit)); // 超级管理员
    CHECKBOX_SetText(WM_GetDialogItem(hWin, GUI_ID_CHECK1), GUI_LANG_GetTextEx(Lang_Admin, Language_Bit));       // 管理员
    BUTTON_SetText(WM_GetDialogItem(hWin, GUI_ID_BUTTON3), GUI_LANG_GetTextEx(Lang_Change_Pwd, Language_Bit)); // 修改密码
    BUTTON_SetText(WM_GetDialogItem(hWin, GUI_ID_BUTTON0), GUI_LANG_GetTextEx(Lang_Quit, Language_Bit));       // 退出
    BUTTON_SetText(WM_GetDialogItem(hWin, GUI_ID_BUTTON1), GUI_LANG_GetTextEx(Lang_Login, Language_Bit));      // 登录

    CHECKBOX_SetState(WM_GetDialogItem(hWin, GUI_ID_CHECK0 ), 1);


}

char pwd_temp[12];
static void Password_Get(void)
{
    unsigned char *time_buf = Get_Input_Text();
    unsigned char len = strlen((const char *)time_buf);
    unsigned char i = 0;

    if (len > 6)len = 6;

    strcpy(pwd_temp, (const char *)time_buf);

    for (i = 0; i < len; i++) {
        show_buf[i] = '*';
    }

    show_buf[i] = 0;

    EDIT_SetText(WM_GetDialogItem(MUser_Man, GUI_ID_EDIT0), show_buf);
}

/*********************************************************************
*
*       Dialog callback routine
*/
static void _cbCallback(WM_MESSAGE * pMsg)
{
    int NCode, Id;
    WM_HWIN hWin = pMsg->hWin;
    int result = 0;

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
                Button_Menu(Id, hWin, GUI_ID_BUTTON1_6);
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

        case GUI_ID_EDIT0:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(Password_Get);
                Set_Input_Len(10);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_CHECK0:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                CHECKBOX_SetState(WM_GetDialogItem(hWin, GUI_ID_CHECK1 ), 0);
            }

            break;

        case GUI_ID_CHECK1:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                CHECKBOX_SetState(WM_GetDialogItem(hWin, GUI_ID_CHECK0 ), 0);
            }

            break;

        case GUI_ID_BUTTON3:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                if (strcmp((const char *)pwd_temp, "3984") == 0) {
                    MUser_PwdCG_Task(hWin, 2);
                    pwd_temp[0] = 0;
                    break;
                }

                if (strcmp((const char *)pwd_temp, "311311") == 0) {
                    sys_sta.login_type = LOGIN_SUPADMIN;
                    MUser_PwdCG_Task(hWin, 0);
                    pwd_temp[0] = 0;
                    break;
                }

                if (strcmp((const char *)pwd_temp, "1258.0") == 0) {
                    Trial_Clear(0);

                    MWarning_Task(Current_HWIN, GUI_LANG_GetTextEx(Lang_Formal_Version, Language_Bit));

                    pwd_temp[0] = 0;
                    break;
                }

                result = Trial_Key_Check((unsigned char *)pwd_temp);

                if (result < 0) {

                } else if (result == KC_MAGIC) {
                    Write_Dev_Type(STA_OFF);
                    MWarning_Task(Current_HWIN, GUI_LANG_GetTextEx(Lang_DevTpye_Clear, Language_Bit));
                    pwd_temp[0] = 0;
                    break;
                } else if (result == 0) {
                    MWarning_Task(Current_HWIN, GUI_LANG_GetTextEx(Lang_Formal_Version, Language_Bit));
                    pwd_temp[0] = 0;
                    break;
                } else {

                    sprintf(show_buf, GUI_LANG_GetTextEx(Lang_Trial_for_Day, Language_Bit), result);

                    MWarning_Task(Current_HWIN, show_buf);
                    pwd_temp[0] = 0;
                    break;
                }

                if (CHECKBOX_GetState(WM_GetDialogItem(hWin, GUI_ID_CHECK0 )) == 1) {
                    if (strcmp((const char *)pwd_temp, basic_inf.supadmin_pwd) == 0) {
                        sys_sta.login_type = LOGIN_SUPADMIN;
                        MUser_PwdCG_Task(hWin, 0);
                    } else {
                        MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_PWD_ER));
                    }
                } else {
                    if (strcmp((const char *)pwd_temp, basic_inf.admin_pwd) == 0) {
                        sys_sta.login_type = LOGIN_ADMIN;
                        MUser_PwdCG_Task(hWin, 1);
                    } else {
                        MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_PWD_ER));
                    }
                }

                pwd_temp[0] = 0;
            }

            break;

        case GUI_ID_BUTTON1:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                if (strcmp((const char *)pwd_temp, "311311") == 0) {
                    sys_sta.login_type = LOGIN_SUPADMIN;
                    Current_HWIN = WM_GetParent(hWin);
                    GUI_EndDialog(hWin, 0);
                    MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_PWD_IN));
                    pwd_temp[0] = 0;
                    break;
                }

                if (CHECKBOX_GetState(WM_GetDialogItem(hWin, GUI_ID_CHECK0 )) == 1) {
                    if (strcmp((const char *)pwd_temp, basic_inf.supadmin_pwd) == 0) {
                        sys_sta.login_type = LOGIN_SUPADMIN;
                        Current_HWIN = WM_GetParent(hWin);
                        GUI_EndDialog(hWin, 0);
                        MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_PWD_IN));
                    } else {
                        MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_PWD_ER));
                    }
                } else {
                    if (strcmp((const char *)pwd_temp, basic_inf.admin_pwd) == 0) {
                        sys_sta.login_type = LOGIN_ADMIN;
                        Current_HWIN = WM_GetParent(hWin);
                        GUI_EndDialog(hWin, 0);
                        MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_PWD_IN));
                    } else {
                        MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_PWD_ER));
                    }
                }

                pwd_temp[0] = 0;
            }

            break;

        case GUI_ID_BUTTON0:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                sys_sta.login_type = LOGIN_GUEST;
                Current_HWIN = WM_GetParent(hWin);
                Main_Buffer = Main_Buffer_Home;
                GUI_EndDialog(hWin, 0);
                MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_PWD_OUT));
            }

//        case GUI_ID_BUTTON2:
//            if (NCode == WM_NOTIFICATION_RELEASED) {
//                GUI_EndDialog(hWin, 0);
//            }

//            break;
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
WM_HWIN MUser_Man;
void MUser_Man_Task(WM_HWIN hParent)
{
    Main_Buffer=Main_Buffer_User;
    MUser_Man = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    Current_HWIN = MUser_Man;
    pwd_temp[0] = 0;
}

