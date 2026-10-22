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


/*********************************************************************
*
*       Dialog resource
*
* This table conatins the info required to create the dialog.
* It has been created by ucGUIbuilder.
*/

#define XPos_Inital    	0
#define XPos_Collec  	0
#define YPos_Collec  	20
#define Y_Space     	70
#define X_Space   		140
#define Text_Width   	36
#define Category_Space  10

#define X  110
#define Y  70
#define X1_Space     300
#define Y1_Space     70



static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect,    "",               0,                       0,  0,  800, 480, 0, 0},
    { TEXT_CreateIndirect,      "控制管理-小窗防冻",          GUI_ID_TEXT0,            250, 4, 300, 42, 0, 0},

		
    { TEXT_CreateIndirect,      "间隔时间/min:",     GUI_ID_TEXT1,            X + X1_Space * 0, 	Y + Y1_Space * 0, 190, 36, 0, 0},
    { TEXT_CreateIndirect,      "运动开度/%:",       GUI_ID_TEXT2,            X + X1_Space * 1, 	Y + Y1_Space * 0, 190, 36, 0, 0},

    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT0,            X + X1_Space * 0 + 180, 	Y + Y1_Space * 0, 72, 36, 0, 0},
    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT1,            X + X1_Space * 1 + 200, 	Y + Y1_Space * 0, 72, 36, 0, 0},		

    { CHECKBOX_CreateIndirect,  "",        GUI_ID_CHECK0,         XPos_Inital + X_Space * 1 + XPos_Collec, YPos_Collec + Y_Space * 2, 125, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "",        GUI_ID_CHECK1,         XPos_Inital + X_Space * 2 + XPos_Collec, YPos_Collec + Y_Space * 2, 125, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "",        GUI_ID_CHECK2,         XPos_Inital + X_Space * 3 + XPos_Collec, YPos_Collec + Y_Space * 2, 125, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "",        GUI_ID_CHECK3,         XPos_Inital + X_Space * 4 + XPos_Collec, YPos_Collec + Y_Space * 2, 125, CHECK_SIZE, 0, 0},

    { CHECKBOX_CreateIndirect,  "",        GUI_ID_CHECK4,         XPos_Inital + X_Space * 1 + XPos_Collec, YPos_Collec + Y_Space * 3, 125, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "",        GUI_ID_CHECK5,         XPos_Inital + X_Space * 2 + XPos_Collec, YPos_Collec + Y_Space * 3, 125, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "",        GUI_ID_CHECK6,         XPos_Inital + X_Space * 3 + XPos_Collec, YPos_Collec + Y_Space * 3, 125, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "",        GUI_ID_CHECK7,         XPos_Inital + X_Space * 4 + XPos_Collec, YPos_Collec + Y_Space * 3, 125, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "",        GUI_ID_CHECK8,         XPos_Inital + X_Space * 1 + XPos_Collec, YPos_Collec + Y_Space * 4, 125, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "",        GUI_ID_CHECK9,         XPos_Inital + X_Space * 2 + XPos_Collec, YPos_Collec + Y_Space * 4, 125, CHECK_SIZE, 0, 0},

//    { BUTTON_CreateIndirect,    "上一页",          GUI_ID_BUTTON0,          MENU_BACK_X1, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
//    { BUTTON_CreateIndirect,    "下一页",          GUI_ID_BUTTON1,          MENU_BACK_X2, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
    { BUTTON_CreateIndirect,    "返回",             GUI_ID_BUTTON2,         MENU_BACK_X3, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},


};


static void Set_OptValue(WM_HWIN hWin, U8 sid)
{
    U8 i = 0;
    //增加是否启用

    for (i = 0; i < 10; i++) {
        CHECKBOX_SetState(WM_GetDialogItem(hWin, (GUI_ID_CHECK0 + i) ), ctrl_inf.var.swin_defrost_en[i]);
    }
}

static void Get_OptValue(WM_HWIN hWin, U8 sid)
{
    U8 i = 0;

    for (i = 0; i < 10; i++) {
        ctrl_inf.var.swin_defrost_en[i] = CHECKBOX_GetState(WM_GetDialogItem(hWin, GUI_ID_CHECK0 + i ));
    }
}
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
    GUI_DrawRoundedRect(70, 55, 712, 356, 10);
    GUI_DrawRoundedRect(66, 52, 716, 359, 10);
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

    for (MDev_ID = GUI_ID_EDIT0; MDev_ID <= GUI_ID_EDIT1; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Edit_SetHZ24(hItem);
    }

    for (MDev_ID = GUI_ID_CHECK0; MDev_ID <= GUI_ID_CHECK9; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Check_SetHZ24(hItem);
		Login_Show(hItem, LOGIN_ADMIN);
    }

    for (MDev_ID = GUI_ID_TEXT0; MDev_ID <= GUI_ID_TEXT2; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24(hItem);

        if (MDev_ID == GUI_ID_TEXT0) {
            TEXT_SetTextAlign(hItem,  GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetTextColor(hItem, GUI_WHITE);
        }
    }

    for (MDev_ID = GUI_ID_CHECK0; MDev_ID <= GUI_ID_CHECK3; MDev_ID++) {
        sprintf(show_buf, "%s%d", GUI_LANG_GetTextEx(Lang_CurtWinDow, Language_Bit), MDev_ID - GUI_ID_CHECK0 + 1); // 风窗
        CHECKBOX_SetText(WM_GetDialogItem(hWin, MDev_ID), show_buf);
    }

    for (MDev_ID = GUI_ID_CHECK4; MDev_ID <= GUI_ID_CHECK9; MDev_ID++) {
        sprintf(show_buf, "%s%d", GUI_LANG_GetTextEx(Lang_Inlet, Language_Bit), MDev_ID - GUI_ID_CHECK4 + 1); // 小窗
        CHECKBOX_SetText(WM_GetDialogItem(hWin, MDev_ID), show_buf);
    }

    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT0), GUI_LANG_GetTextEx(Lang_Inlet_Defrost, Language_Bit)); // 小窗防冻

    sprintf(show_buf, "%s(%s):", GUI_LANG_GetTextEx(Lang_Interval_Time, Language_Bit), GUI_LANG_GetTextEx(Lang_Time_Min, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT1), show_buf); // 间隔时间/min

    sprintf(show_buf, "%s", GUI_LANG_GetTextEx(Lang_Inlet, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT2), show_buf); // 运行开度/%

    sprintf(show_buf, "%d", ctrl_inf.var.swin_hold_time);
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT0), (const char *)show_buf);

    sprintf(show_buf, "%d", ctrl_inf.var.swin_defrost_degree);
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT1), show_buf);

    Set_OptValue(hWin, 0);
}

/*********************************************************************
*
*       Dialog callback routine
*/
static void defrost_time_get(void)
{
    float ftemp = Get_Input_Value();
    unsigned short stemp = (unsigned short)ftemp;

//    if (stemp > MAX_1BYTE) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = MAX_1BYTE;
//    }

//    if (stemp < 0) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = 0;
//    }

    ctrl_inf.var.swin_hold_time = stemp;
    sprintf(show_buf, "%d", ctrl_inf.var.swin_hold_time);
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, GUI_ID_EDIT0), show_buf);
}

static void defrost_degree_get(void)
{
    float ftemp = Get_Input_Value();
    signed short stemp = (signed short)ftemp;

    if (stemp > 100) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 100;
    }

    if (stemp < -100) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = -100;
    }

    ctrl_inf.var.swin_defrost_degree = stemp;
    sprintf(show_buf, "%d", ctrl_inf.var.swin_defrost_degree);
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, GUI_ID_EDIT1), show_buf);
}

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

        case GUI_ID_BUTTON2:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Get_OptValue(hWin, 0);

                if (Config_Save(SVAR_ID) < 0) {
                    MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_SAVE_ER));
                }
				
                Current_HWIN = WM_GetParent(hWin);			
                GUI_EndDialog(hWin, 0);
            }

            break;

        case GUI_ID_EDIT0:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(defrost_time_get);
                Set_Input_Len(3);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT1:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(defrost_degree_get);
                Set_Input_Len(4);
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
WM_HWIN MCtrl_Defrost;
void MCtrl_Defrost_Task(WM_HWIN hParent)
{
    MCtrl_Defrost = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    Current_HWIN = MCtrl_Defrost;
}

