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

static void Set_Value(WM_HWIN hWin)
{
    sprintf(show_buf, "%dPa", ctrl_inf.vac.targetL);
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT0), (const char *) show_buf);
    sprintf(show_buf, "%dPa", ctrl_inf.vac.targetH);
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT1), (const char *) show_buf);
    sprintf(show_buf, "%dPa", ctrl_inf.vac.alaL);
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT2), (const char *) show_buf);
    sprintf(show_buf, "%dPa", ctrl_inf.vac.alaH);
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT3), (const char *) show_buf);
    sprintf(show_buf, "%dPa", ctrl_inf.vac.dead_zone);
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT4), (const char *) show_buf);
}


static void Set_OptValue(WM_HWIN hWin)
{
    //增加是否启用
    CHECKBOX_SetState(WM_GetDialogItem(hWin, GUI_ID_CHECK0 ), ctrl_inf.vac.type);
    Set_Value(hWin);
}



/*********************************************************************
*
*       Dialog resource
*
* This table conatins the info required to create the dialog.
* It has been created by ucGUIbuilder.
*/
#define XPos_Collec  (50)
#define YPos_Space   (60)

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect,    "",               0,                       0,  0,  800, 480, 0, 0},
    { TEXT_CreateIndirect,      "控制管理-静压控制", GUI_ID_TEXT0,           250, 4, 300, 42, 0, 0},
    { TEXT_CreateIndirect,      "静压控制:",    	GUI_ID_TEXT1,            70 + XPos_Collec, 66 + YPos_Space * 0, 140, 36, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",            GUI_ID_CHECK0,            210 + XPos_Collec, 66 + YPos_Space * 0 + 3, 100, CHECK_SIZE, 0, 0},

    { TEXT_CreateIndirect,      "低温静压:",    	GUI_ID_TEXT2,            70 + XPos_Collec, 66 + YPos_Space * 1, 150, 36, 0, 0},
    { TEXT_CreateIndirect,      "低压报警:",       	GUI_ID_TEXT4,            70 + XPos_Collec, 66 + YPos_Space * 2, 150, 36, 0, 0},
    { TEXT_CreateIndirect,      "过渡区:",      	GUI_ID_TEXT6,            70 + XPos_Collec, 66 + YPos_Space * 3, 150, 36, 0, 0},


    { EDIT_CreateIndirect,		 "",		   GUI_ID_EDIT0,			 210 + XPos_Collec, 66 + YPos_Space * 1, 100, 36, 0, 0},
    { EDIT_CreateIndirect,		 "",		   GUI_ID_EDIT2,			 210 + XPos_Collec, 66 + YPos_Space * 2, 100, 36, 0, 0},
    { EDIT_CreateIndirect,		 "",		   GUI_ID_EDIT4,			210 + XPos_Collec, 66 + YPos_Space * 3, 100, 36, 0, 0},

    { TEXT_CreateIndirect,      "高温静压:",    GUI_ID_TEXT3,                370 + XPos_Collec, 66 + YPos_Space * 1, 160, 36, 0, 0},
    { TEXT_CreateIndirect,      "高压报警:",    GUI_ID_TEXT5,              370 + XPos_Collec, 66 + YPos_Space * 2, 160, 36, 0, 0},


    { EDIT_CreateIndirect,		 "",		   GUI_ID_EDIT1,			 500 + XPos_Collec, 66 + YPos_Space * 1, 100, 36, 0, 0},
    { EDIT_CreateIndirect,		 "",		   GUI_ID_EDIT3,			 500 + XPos_Collec, 66 + YPos_Space * 2, 100, 36, 0, 0},


//    { BUTTON_CreateIndirect,    "上一页",          GUI_ID_BUTTON0,          MENU_BACK_X1, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
//    { BUTTON_CreateIndirect,    "下一页",          GUI_ID_BUTTON1,          MENU_BACK_X2, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
    { BUTTON_CreateIndirect,    "返回",             GUI_ID_BUTTON2,         MENU_BACK_X3, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
		
    { BUTTON_CreateIndirect,    "",     GUI_ID_BUTTON6,           			660, 0, 90, 60, 0, 0},

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
    GUI_DrawRoundedRect(80, 55, 712, 386, 10);
    GUI_DrawRoundedRect(76, 52, 716, 389, 10);

    //GUI_DrawLine(80,260,712,260);
    //GUI_DrawLine(80,264,712,264);
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
	
    for (MDev_ID = GUI_ID_BUTTON6; MDev_ID <= GUI_ID_BUTTON6; MDev_ID++) {//set
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, set_cbButton);
    }
	
    for (MDev_ID = GUI_ID_EDIT0; MDev_ID <= GUI_ID_EDIT4; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Edit_SetHZ24(hItem);
        WM_MoveWindow(hItem, 30, 0);
    }

    for (MDev_ID = GUI_ID_TEXT0; MDev_ID <= GUI_ID_TEXT6; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24L(hItem);

        if (MDev_ID == GUI_ID_TEXT0) {
            TEXT_SetTextAlign(hItem,  GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetTextColor(hItem, GUI_WHITE);
        }
    }
	
    for (MDev_ID = GUI_ID_CHECK0; MDev_ID <= GUI_ID_CHECK0; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        CHECKBOX_SetText(hItem, GUI_LANG_GetTextEx(Lang_Enable, Language_Bit));
        Check_SetHZ24(hItem);
        Login_Show(hItem, LOGIN_ADMIN);
    }

    sprintf(show_buf, "%s-%s", GUI_LANG_GetTextEx(Lang_Control, Language_Bit), GUI_LANG_GetTextEx(Lang_NP_Control, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT0), show_buf);                                          // 控制管理-静压控制
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT1), GUI_LANG_GetTextEx(Lang_NP_Control, Language_Bit)); // 静压控制:
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT2), GUI_LANG_GetTextEx(Lang_LowTemp_Target, Language_Bit));  // 低温静压:
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT3), GUI_LANG_GetTextEx(Lang_HighTemp_Target, Language_Bit)); // 高温静压:
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT4), GUI_LANG_GetTextEx(Lang_LA_NP, Language_Bit));           // 低压报警:
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT5), GUI_LANG_GetTextEx(Lang_HA_NP, Language_Bit));           // 高压报警:
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT6), GUI_LANG_GetTextEx(Lang_Transition, Language_Bit));      // 过渡区:

    Set_OptValue(hWin);
}

static U8 temp_id = 0;
static void VACTarget_Get(void)
{
    float ftemp = Get_Input_Value();
    short stemp = (short)ftemp;

    if (stemp > VAC_MAX) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = VAC_MAX;
    }

    if (stemp < VAC_MIN) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = VAC_MIN;
    }

    if (temp_id == 0) {
        ctrl_inf.vac.targetL= stemp;
    } else {
        ctrl_inf.vac.targetH = stemp;
    }
	
    Set_Value(Current_HWIN);
}

static void VACDead_Zone_Get(void)
{
    float ftemp = Get_Input_Value();
    short stemp = (short)ftemp;

    if (stemp > VAC_MAX) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = VAC_MAX;
    }


    if (stemp < 2) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 2;
    }

    ctrl_inf.vac.dead_zone = stemp;
    Set_Value(Current_HWIN);
}

static void VACAla_Get(void)
{
    float ftemp = Get_Input_Value();
    short stemp = (short)ftemp;

    if (stemp > VAC_MAX) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = VAC_MAX;
    }

    if (stemp < 0) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 0;
    }

    if (temp_id == 0) {
        ctrl_inf.vac.alaL= stemp;
    } else {
        ctrl_inf.vac.alaH = stemp; 
    }

    Set_Value(Current_HWIN);
}

static void Get_OptValue(WM_HWIN hWin)
{
    ctrl_inf.vac.type = CHECKBOX_GetState(WM_GetDialogItem(hWin, GUI_ID_CHECK0 ));
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
                Get_OptValue(hWin);

                if (Config_Save(SVAC_ID) < 0) {
                    MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_SAVE_ER));
                }

                Current_HWIN = WM_GetParent(hWin);
                GUI_EndDialog(hWin, 0);
            }

            break;

        case GUI_ID_EDIT0:
        case GUI_ID_EDIT1:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                temp_id = (Id - GUI_ID_EDIT0);
                Set_Output_Callback(VACTarget_Get);
                Set_Input_Len(3);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT2:
        case GUI_ID_EDIT3:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                temp_id = (Id - GUI_ID_EDIT2);
                Set_Output_Callback(VACAla_Get);
                Set_Input_Len(3);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT4:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(VACDead_Zone_Get);
                Set_Input_Len(2);
                Input_Task(hWin);
            }

            break;
			
        case GUI_ID_BUTTON6:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                MCtrl_VAC_Set_Task(hWin);
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
WM_HWIN MCtrl_VAC;
void MCtrl_VAC_Task(WM_HWIN hParent)
{
    MCtrl_VAC = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    Current_HWIN = MCtrl_VAC;
}

