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
#include "def.h"
#include "datasave.h"
#include "usbh_usr.h"

/*********************************************************************
*
*       static data
*
**********************************************************************
*/
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
#define ButtonM_Xos  	(98)
#define ButtonM_Yos  	(80+Y)
#define X_Space   		(226)
#define Y_Space   		(80)

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect,    "",               0,                       0,  0,  800, 480, 0, 0},
    { TEXT_CreateIndirect,      "系统管理",          GUI_ID_TEXT0,           250, 4, 300, 42, 0, 0},

    { BUTTON_CreateIndirect,    "房间设置",           GUI_ID_BUTTON3,             ButtonM_Xos + X_Space * 0, ButtonM_Yos + Y_Space * 0, 180, 65, 0, 0},
    { BUTTON_CreateIndirect,    "时间设置",           GUI_ID_BUTTON6,           ButtonM_Xos + X_Space * 0, ButtonM_Yos + Y_Space * 1, 180, 65, 0, 0},
    { BUTTON_CreateIndirect,    "动物日龄",           GUI_ID_BUTTON4,             ButtonM_Xos + X_Space * 0, ButtonM_Yos + Y_Space * 2, 180, 65, 0, 0},
//    { BUTTON_CreateIndirect,    "系统配置",              GUI_ID_BUTTON6,          ButtonM_Xos + X_Space * 3, ButtonM_Yos + Y_Space * 0, 130, 50, 0, 0},
    { BUTTON_CreateIndirect,    "动物数量",              GUI_ID_BUTTON5,          ButtonM_Xos + X_Space * 0, ButtonM_Yos + Y_Space * 3, 180, 65, 0, 0},

    { BUTTON_CreateIndirect,    "数据导出",           GUI_ID_BUTTON7,          ButtonM_Xos + X_Space * 1, ButtonM_Yos + Y_Space * 0, 180, 65, 0, 0},
    { BUTTON_CreateIndirect,    "配置导出",           GUI_ID_BUTTON8,               ButtonM_Xos + X_Space * 1, ButtonM_Yos + Y_Space * 1, 180, 65, 0, 0},
    { BUTTON_CreateIndirect,    "配置导入",           GUI_ID_BUTTON9,           ButtonM_Xos + X_Space * 1, ButtonM_Yos + Y_Space * 2, 180, 65, 0, 0},
    { BUTTON_CreateIndirect,    "系统设置",           GUI_ID_BUTTON10,           ButtonM_Xos + X_Space * 1, ButtonM_Yos + Y_Space * 3, 180, 65, 0, 0},

    { BUTTON_CreateIndirect,    "模拟测试",           GUI_ID_BUTTON11,           ButtonM_Xos + X_Space * 2, ButtonM_Yos + Y_Space * 0, 180, 65, 0, 0},
    { BUTTON_CreateIndirect,    "手动测试",           GUI_ID_BUTTON12,               ButtonM_Xos + X_Space * 2, ButtonM_Yos + Y_Space * 1, 180, 65, 0, 0},
    { BUTTON_CreateIndirect,    "硬件测试",          GUI_ID_BUTTON13,           ButtonM_Xos + X_Space * 2, ButtonM_Yos + Y_Space * 2, 180, 65, 0, 0},
    { BUTTON_CreateIndirect,    "恢复出厂",          GUI_ID_BUTTON14,             ButtonM_Xos + X_Space * 2, ButtonM_Yos + Y_Space * 3, 180, 65, 0, 0},

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

static const u16 Lang_KEY_NAME[] = {Lang_Farmhouse_Info, Lang_DayAge_Setting, Lang_Animal_quantity, Lang_Time_Set, Lang_Data_Export, Lang_Cfg_E, Lang_Cfg_I, Lang_System_Info,
                                     Lang_Analogy_Test, Lang_Mock_Test, Lang_Hardware_Test, Lang_Restore_Factory};

static const char * KEY_PIC[] = {"75iconSys_Home", "77iconSys_Age", "78iconSys_Number", "76iconSys_Time", "79iconSys_DataEx", "80iconSys_SetEx",
                                 "81iconSys_SetIm", "82iconSys_SysSet", "83iconSys_Test", "83iconSys_Test", "83iconSys_Test", "84iconSys_Reset",
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
}

static void _cbKey(WM_MESSAGE *pMsg)
{
    WM_HWIN hWin;
    int Id;
    hWin = pMsg->hWin;
    Id = WM_GetId(hWin);
    Id = Id <= GUI_ID_BUTTON9 ? Id : (GUI_ID_BUTTON9 + Id - GUI_ID_USER);

    switch (pMsg->MsgId) {
    case WM_PAINT:
        if (BUTTON_IsPressed(hWin) || !WM_IsEnabled(hWin)) {
            GUI_SetBkColor(GUI_WHITE);
            //				GUI_Clear();
            GUI_SetAlpha(0xa0);
            GUI_defDrawBitmap("56Mpic_KeyBack", 0, 0);
            GUI_defDrawBitmap(KEY_PIC[(Id - GUI_ID_BUTTON3)], 12, 15);

            ButtonText_Show(WM_GetDialogItem(hWin, Id), GUI_LANG_GetTextEx(Lang_KEY_NAME[(Id - GUI_ID_BUTTON3)], Language_Bit), 50, 15);

            GUI_SetAlpha(0);
        } else {
            GUI_defDrawBitmap("56Mpic_KeyBack", 0, 0);
            GUI_defDrawBitmap(KEY_PIC[(Id - GUI_ID_BUTTON3)], 12, 15);

            ButtonText_Show(WM_GetDialogItem(hWin, Id), GUI_LANG_GetTextEx(Lang_KEY_NAME[(Id - GUI_ID_BUTTON3)], Language_Bit), 50, 15);
        }

        break;

    default:
        BUTTON_Callback(pMsg);
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
    int IDMB = 0;
    WM_HWIN hItem;

    //
    //GUI_ID_TEXT0
    //
    Text_SetHZ24(WM_GetDialogItem(hWin, GUI_ID_TEXT0));
    TEXT_SetTextColor(WM_GetDialogItem(hWin, GUI_ID_TEXT0), GUI_WHITE);

    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT0), GUI_LANG_GetTextEx(Lang_System, Language_Bit));

    for (IDMB = GUI_ID_BUTTON3; IDMB <= GUI_ID_BUTTON9; IDMB++) {
        hItem = WM_GetDialogItem(hWin, IDMB);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, _cbKey);
    }

    for (IDMB = GUI_ID_BUTTON10; IDMB <= GUI_ID_BUTTON14; IDMB++) {
        hItem = WM_GetDialogItem(hWin, IDMB);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, _cbKey);
    }

    for (IDMB = GUI_ID_BUTTON6; IDMB <= GUI_ID_BUTTON9; IDMB++) {
        hItem = WM_GetDialogItem(hWin, IDMB);
    }

    for (IDMB = GUI_ID_BUTTON11; IDMB <= GUI_ID_BUTTON14; IDMB++) {
        hItem = WM_GetDialogItem(hWin, IDMB);
    }

    for (IDMB = GUI_ID_BUTTON1_0; IDMB <= GUI_ID_BUTTON1_6; IDMB++) {
        hItem = WM_GetDialogItem(hWin, IDMB);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, Main_cbButton);
    }

}



U8 setting_select[4] = {0};

/*********************************************************************
*
*       Dialog callback routine
*/
static void _cbCallback(WM_MESSAGE * pMsg)
{
    int 	NCode, Id;
    int		ret = 0;
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

        if (Id >= GUI_ID_BUTTON1_0 && Id <= GUI_ID_BUTTON1_6) {
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Button_Menu(Id, hWin, GUI_ID_BUTTON1_3);
            }

            break;
        }

        if (Id >= GUI_ID_BUTTON7 && Id <= GUI_ID_BUTTON9 && NCode == WM_NOTIFICATION_RELEASED
                && get_login_permiss(LOGIN_SUPADMIN) < 0) {
            MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_NO_LOGIN));
            break;
        }

        if (Id >= GUI_ID_BUTTON11 && Id <= GUI_ID_BUTTON14 && NCode == WM_NOTIFICATION_RELEASED
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

        case GUI_ID_BUTTON2:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Current_HWIN = WM_GetParent(hWin);
                GUI_EndDialog(hWin, 0);
            }

            break;

        case GUI_ID_BUTTON3:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                MSys_Room_Task(hWin);
            }

            break;

        case GUI_ID_BUTTON4:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                MSys_Day_Task(hWin);
            }

            break;


        case GUI_ID_BUTTON5:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                MSys_Total_Task(hWin);
            }

            break;

        case GUI_ID_BUTTON6:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                MSys_TimeSet_Task(hWin);
//                Set_Output_Callback(Sys_Time_Get);
//                Set_Input_Len(12);
//                Input_Task(hWin);
            }

            break;

        case GUI_ID_BUTTON7:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                if (USB_disk_status() == 0) {

                    MWarning_Task(hWin, GUI_LANG_GetTextEx(Lang_NotFound, Language_Bit));

                    break;
                }

                sys_sta.auto_mode = STA_OFF;
                Set_UsbOP_Code(USB_SAVE_DATA);
                ret = Get_UsbOP_Res();
                sys_sta.auto_mode = STA_ON;

                if (ret < 1) {

                    MWarning_Task(hWin, GUI_LANG_GetTextEx(Lang_ExDataF, Language_Bit));

                    break;
                } else {

                    MWarning_Task(hWin, GUI_LANG_GetTextEx(Lang_ExDataS, Language_Bit));

                    break;
                }
            }

            break;

        case GUI_ID_BUTTON8:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                if (USB_disk_status() == 0) {
                    MWarning_Task(hWin, GUI_LANG_GetTextEx(Lang_NotFound, Language_Bit));
                    break;
                }

                Set_UsbOP_Code(USB_SAVE_CONFIG);
                ret = Get_UsbOP_Res();

                if (ret < 1) {
                    MWarning_Task(hWin, GUI_LANG_GetTextEx(Lang_ExconfigF, Language_Bit));
                    break;
                } else {
                    MWarning_Task(hWin, GUI_LANG_GetTextEx(Lang_ExconfigS, Language_Bit));
                    break;
                }
            }

            break;

        case GUI_ID_BUTTON9:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                if (USB_disk_status() == 0) {
                    MWarning_Task(hWin, GUI_LANG_GetTextEx(Lang_NotFound, Language_Bit));
                    break;
                }

                Set_UsbOP_Code(USB_READ_CONFIG);
                ret = Get_UsbOP_Res();
				
                MSys_InSel_Task(hWin);

                //电表等数据不修改
                if (setting_select[0] == 0) {
                    Config_Save(SDIN_ID);
                    Config_Save(SFAN_CHK_ID);
                }

                if (setting_select[1] == 0) {
                    Config_Save(SWIN_ID);
                }

                if (ret < 1) {
                    MWarning_Task(hWin, GUI_LANG_GetTextEx(Lang_ImconfigF, Language_Bit));
                    break;
                } else {
                    Load_Param();
                    MWarning_Task(hWin, GUI_LANG_GetTextEx(Lang_ImconfigS, Language_Bit));
                    break;
                }
            }

            break;


        case GUI_ID_BUTTON10:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                MSys_SysSet_Task(hWin);
//                sprintf(show_buf, "Ver: V%.2f", SYS_VER);
//                MWarning_Task(hWin, (const char *) show_buf);
//                MMessage_Task(hWin, MESSAGE_SOFT_VER);
            }

            break;

        case GUI_ID_BUTTON11:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                MSys_SLTS_Task(hWin);
            }

            break;

        case GUI_ID_BUTTON12:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                MSys_PWMTS_Task(hWin);
            }

            break;

        case GUI_ID_BUTTON13:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                ret = MMessage_Task(hWin, MESSAGE_HARD_TEST);

                if (ret == MESSAGE_YES) {
                    MSys_HardTS_Task(hWin);
                }
            }

            break;

        case GUI_ID_BUTTON14:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                ret = MMessage_Task(hWin, MESSAGE_SET_DEF);

                if (ret == MESSAGE_YES) {
                    SetInf_Def();
                    MWarning_Task(hWin, GUI_LANG_GetTextEx(Lang_Rest_S, Language_Bit));
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
WM_HWIN MSys_Man;
void MSys_Man_Task(WM_HWIN hParent)
{
    Main_Buffer=Main_Buffer_Sys;
    MSys_Man = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    Current_HWIN = MSys_Man;
}

