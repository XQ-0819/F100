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

#define X  80
#define Y  70
#define X_Space     340
#define Y_Space     70

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
    { TEXT_CreateIndirect,      "控制管理-水流量报警",          GUI_ID_TEXT0,           250, 4, 300, 42, 0, 0},

    { TEXT_CreateIndirect,      "漏水报警:",      			GUI_ID_TEXT1,            X + X_Space * 0, 	Y + Y_Space * 0, 220, 36, 0, 0},
    { TEXT_CreateIndirect,      "最大流量(L/Min):",       	GUI_ID_TEXT2,            X + X_Space * 0, 	Y + Y_Space * 1, 220, 36, 0, 0},
    { TEXT_CreateIndirect,      "缺水报警:",      			GUI_ID_TEXT4,            X + X_Space * 0, 	Y + Y_Space * 2, 220, 36, 0, 0},
    { TEXT_CreateIndirect,      "最小流量(L/Min):",       	GUI_ID_TEXT5,            X + X_Space * 0, 	Y + Y_Space * 3, 220, 36, 0, 0},



    { TEXT_CreateIndirect,      "检测周期(Min):",     	GUI_ID_TEXT3,             X + X_Space * 1, 	Y + Y_Space * 1, 220, 36, 0, 0},
    { TEXT_CreateIndirect,      "检测周期(Min):",     	GUI_ID_TEXT6,             X + X_Space * 1, 	Y + Y_Space * 3, 220, 36, 0, 0},

    { CHECKBOX_CreateIndirect,  "启用",             GUI_ID_CHECK0,            X + X_Space * 0 + 225,    Y + Y_Space * 0 + 3, 120, CHECK_SIZE, 0, 0},
    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT0,            X + X_Space * 0 + 225,	Y + Y_Space * 1, 72, 36, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",             GUI_ID_CHECK1,            X + X_Space * 0 + 225,    Y + Y_Space * 2 + 3, 120, CHECK_SIZE, 0, 0},
    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT2,            X + X_Space * 0 + 225,	Y + Y_Space * 3, 72, 36, 0, 0},

    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT1,            X + X_Space * 1 + 225, 	Y + Y_Space * 1, 72, 36, 0, 0},		
    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT3,            X + X_Space * 1 + 225, 	Y + Y_Space * 3, 72, 36, 0, 0},		


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
    Show_MBackPic();
    GUI_SetColor(GUI_LINE_COLOR);
    GUI_DrawRoundedRect(6, 55, 794, 396, 10);    
	GUI_DrawRoundedRect(2, 52, 798, 399, 10);
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

    for (MDev_ID = GUI_ID_TEXT0; MDev_ID <= GUI_ID_TEXT6; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24L(hItem);

        if (MDev_ID == GUI_ID_TEXT0) {
            TEXT_SetTextAlign(hItem,  GUI_TA_HCENTER | GUI_TA_VCENTER);
        }
    }
	
    for (MDev_ID = GUI_ID_CHECK0; MDev_ID <= GUI_ID_CHECK1; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Check_SetHZ24(hItem);
		CHECKBOX_SetText(hItem, GUI_LANG_GetTextEx(Lang_Enable, Language_Bit));
        Login_Show(hItem, LOGIN_ADMIN);
    }


    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT0), GUI_LANG_GetTextEx(Lang_Water_Flow_ALA, Language_Bit));    //  水流量报警                                       // 控制管理-昼夜调整
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT1), GUI_LANG_GetTextEx(Lang_WaterOut, Language_Bit));         // 漏水报警
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT2), GUI_LANG_GetTextEx(Lang_Max_Flow, Language_Bit));       // 最大流量(L/Min):
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT3), GUI_LANG_GetTextEx(Lang_Detection_Cycle, Language_Bit));       // 检测周期(Min):
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT4), GUI_LANG_GetTextEx(Lang_WaterL_ALA, Language_Bit));       // 缺水报警
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT5), GUI_LANG_GetTextEx(Lang_Min_Flow, Language_Bit));       // 最小流量(L/Min):
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT6), GUI_LANG_GetTextEx(Lang_Detection_Cycle, Language_Bit));       // 检测周期(Min):


    CHECKBOX_SetState(WM_GetDialogItem(hWin, GUI_ID_CHECK0 ), ext_inf.din.leakage_enable);
    CHECKBOX_SetState(WM_GetDialogItem(hWin, GUI_ID_CHECK1 ), ext_inf.din.lack_enable);
	
    sprintf(show_buf, "%d", ext_inf.din.leakage_max_flow);
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT0), show_buf);

    sprintf(show_buf, "%d", ext_inf.din.leakage_delay_min);
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT1), show_buf);

    sprintf(show_buf, "%d", ext_inf.din.lack_min_flow);
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT2), show_buf);

    sprintf(show_buf, "%d", ext_inf.din.lack_delay_min);
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT3), show_buf);

}


static void Max_Flow_Get(void)
{
    float ftemp = Get_Input_Value();
    short stemp = (short)ftemp;

    if (stemp > MAX_1BYTE) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = MAX_1BYTE;
    }

    if (stemp < 0) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 0;
    }
	
    ext_inf.din.leakage_max_flow = stemp;
    sprintf(show_buf, "%d", ext_inf.din.leakage_max_flow);
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, GUI_ID_EDIT0), show_buf);
}

static void Min_Flow_Get(void)
{
    float ftemp = Get_Input_Value();
    short stemp = (short)ftemp;

    if (stemp > MAX_1BYTE) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = MAX_1BYTE;
    }
	
    if (stemp < 0) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 0;
    }
	
    ext_inf.din.lack_min_flow = stemp;
    sprintf(show_buf, "%d", ext_inf.din.lack_min_flow);
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, GUI_ID_EDIT2), show_buf);
}

static void LeakDelay_Get(void)
{
    float ftemp = Get_Input_Value();
    short stemp = (short)ftemp;

    if (stemp > WATER_ALA_DELAY) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = WATER_ALA_DELAY;
    }

    if (stemp < 1) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 1;
    }

    ext_inf.din.leakage_delay_min = stemp;
    sprintf(show_buf, "%d", ext_inf.din.leakage_delay_min);
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, GUI_ID_EDIT1), show_buf);
}

static void LackDelay_Get(void)
{
    float ftemp = Get_Input_Value();
    short stemp = (short)ftemp;

    if (stemp > WATER_ALA_DELAY) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = WATER_ALA_DELAY;
    }

    if (stemp < 1) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 1;
    }

    ext_inf.din.lack_delay_min = stemp;
    sprintf(show_buf, "%d", ext_inf.din.lack_delay_min);
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, GUI_ID_EDIT3), show_buf);
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
                Set_Output_Callback(Max_Flow_Get);
                Set_Input_Len(3);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT1:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(LeakDelay_Get);
                Set_Input_Len(2);
                Input_Task(hWin);
            }

            break;	
			
        case GUI_ID_EDIT2:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(Min_Flow_Get);
                Set_Input_Len(3);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT3:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(LackDelay_Get);
                Set_Input_Len(2);
                Input_Task(hWin);
            }

            break;	
			
        case GUI_ID_BUTTON2:
            if (NCode == WM_NOTIFICATION_RELEASED) {	
                ext_inf.din.leakage_enable = CHECKBOX_GetState(WM_GetDialogItem(hWin, GUI_ID_CHECK0 ));
                ext_inf.din.lack_enable = CHECKBOX_GetState(WM_GetDialogItem(hWin, GUI_ID_CHECK1 ));
				
                if (Config_Save(SDIN_ID) < 0) {
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
WM_HWIN MDev_Water_Set;
void MDev_Water_Set_Task(WM_HWIN hParent)
{
    MCtrl_HumFix_Set = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    Current_HWIN = MCtrl_HumFix_Set;
}

