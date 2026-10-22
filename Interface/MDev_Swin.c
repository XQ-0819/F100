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


#define Xpos_Window   30
#define Ypos_Window   40

static U8 select_id = 0;

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect,    "",               0,                       0,  0,  800, 480, 0, 0},
    { TEXT_CreateIndirect,      "设备管理-小窗",        GUI_ID_TEXT0,            250, 0, 300, 42, 0, 0},

    { TEXT_CreateIndirect,      "控制模式:",         GUI_ID_TEXT3,            	120 + Xpos_Window, 40 + Ypos_Window , 130, 36, 0, 0},
    { DROPDOWN_CreateIndirect,	 "",			 	GUI_ID_DROPDOWN0,			260 + Xpos_Window, 40 + Ypos_Window, 200, 110, 0, 0},
    { CHECKBOX_CreateIndirect,  "",         		GUI_ID_CHECK1,           470 + Xpos_Window, 40 + Ypos_Window + 3, 200, CHECK_SIZE, 0, 0},

    { TEXT_CreateIndirect,      "开启时间/秒:",       	GUI_ID_TEXT1,            120 + Xpos_Window, 100 + Ypos_Window, 130, 36, 0, 0},
    { EDIT_CreateIndirect,      " ",                	GUI_ID_EDIT0,            280 + Xpos_Window, 100 + Ypos_Window, 70, 36, 0, 0},
    { TEXT_CreateIndirect,      "关闭时间/秒:",        	GUI_ID_TEXT2,            400 + Xpos_Window, 100 + Ypos_Window, 130, 36, 0, 0},
    { EDIT_CreateIndirect,      " ",                	GUI_ID_EDIT1,            560 + Xpos_Window, 100 + Ypos_Window, 70, 36, 0, 0},



//    { BUTTON_CreateIndirect,    "上一页",          GUI_ID_BUTTON0,          MENU_BACK_X1, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
//    { BUTTON_CreateIndirect,    "下一页",          GUI_ID_BUTTON1,          MENU_BACK_X2, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
    { BUTTON_CreateIndirect,    "返回",             GUI_ID_BUTTON2,         MENU_BACK_X3, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},

};


static void Show_Value(WM_HWIN hWin, U8 sid)
{
    if (dev_inf.SWIN.detail[sid].ctrl_mode != WIN_VAC_CTRL) {
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_CHECK1));
    } else {
        WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_CHECK1));
    }

	
    DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN0 ), dev_inf.SWIN.detail[sid].ctrl_mode);
    CHECKBOX_SetState(WM_GetDialogItem(hWin, GUI_ID_CHECK1 ), dev_inf.SWIN.detail[sid].prior_ctrl);
    sprintf(show_buf, "%d", dev_inf.SWIN.detail[sid].open_time);
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT0), (const char *) show_buf);
    sprintf(show_buf, "%d", dev_inf.SWIN.detail[sid].close_time);
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT1), (const char *) show_buf);
}

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
    GUI_DrawRoundedRect(120, 55, 680, 391, 10);
    GUI_DrawRoundedRect(116, 52, 684, 394, 10);

}


static void Open_Time_Get(void)
{
    float ftemp = Get_Input_Value();
    short stemp = (short)ftemp;

    if (stemp > 255) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 255;
    }

    if (stemp < 1) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 1;
    }

    dev_inf.SWIN.detail[select_id].open_time = stemp;

    sprintf(show_buf, "%d", dev_inf.SWIN.detail[select_id].open_time);
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, GUI_ID_EDIT0), (const char *) show_buf);
}

static void Close_Time_Get(void)
{
    float ftemp = Get_Input_Value();
    short stemp = (short)ftemp;

    if (stemp > 255) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 255;
    }

    if (stemp < 1) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 1;
    }

    dev_inf.SWIN.detail[select_id].close_time = stemp;

    sprintf(show_buf, "%d", dev_inf.SWIN.detail[select_id].close_time);
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, GUI_ID_EDIT1), (const char *) show_buf);
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

    //FRAMEWIN_Set_UserDefine(hWin);
    //hItem=WM_GetDialogItem(hWin,GUI_ID_TEXT0);
    //FRAMEWIN_TextBarSet(hItem);
    //
    //GUI_ID_BUTTON0
    //

    for (MDev_ID = GUI_ID_BUTTON2; MDev_ID <= GUI_ID_BUTTON2; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, Cut_cbButton);
    }

    for (MDev_ID = GUI_ID_EDIT0; MDev_ID <= GUI_ID_EDIT1; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Edit_SetHZ24(hItem);
    }


    for (MDev_ID = GUI_ID_TEXT0; MDev_ID <= GUI_ID_TEXT3; MDev_ID++) {
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
        Login_Show(hItem, LOGIN_ADMIN);
    }
	
    for (MDev_ID = GUI_ID_CHECK1; MDev_ID <= GUI_ID_CHECK1; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        CHECKBOX_SetText(hItem, GUI_LANG_GetTextEx(Lang_PriorityCtrl, Language_Bit));    // 优先控制
        Check_SetHZ24(hItem);
        Login_Show(hItem, LOGIN_ADMIN);
    }

    DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN0), GUI_LANG_GetTextEx(Lang_Lv_C, Language_Bit));
    DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN0), GUI_LANG_GetTextEx(Lang_Lv_NP_C, Language_Bit));

    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT1), GUI_LANG_GetTextEx(Lang_Open_T, Language_Bit));    // 开启时间
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT2), GUI_LANG_GetTextEx(Lang_Close_T, Language_Bit));   // 关闭时间
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT3), GUI_LANG_GetTextEx(Lang_CTRL_mode, Language_Bit)); // 控制模式

    sprintf(show_buf, "%s-%s%d", GUI_LANG_GetTextEx(Lang_Device, Language_Bit), GUI_LANG_GetTextEx(Lang_Inlet, Language_Bit), select_id + 1);
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT0), (const char *) show_buf);

    Show_Value(hWin, select_id);
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
			
        case GUI_ID_DROPDOWN0:
            if (NCode == WM_NOTIFICATION_SEL_CHANGED) {
                dev_inf.SWIN.detail[select_id].ctrl_mode = DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN0));
                Show_Value(hWin, select_id);
            }

            break;

        case GUI_ID_BUTTON2:
            if (NCode == WM_NOTIFICATION_RELEASED) {
			    dev_inf.SWIN.detail[select_id].ctrl_mode = DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN0));
                dev_inf.SWIN.detail[select_id].prior_ctrl = CHECKBOX_GetState(WM_GetDialogItem(hWin, GUI_ID_CHECK1 ));
				
                if (Config_Save(SSWIN_ID) < 0) {
                    MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_SAVE_ER));
                }

                Current_HWIN = WM_GetParent(hWin);
                GUI_EndDialog(hWin, 0);
            }

            break;

        case GUI_ID_EDIT0:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(Open_Time_Get);
                Set_Input_Len(3);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT1:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(Close_Time_Get);
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

WM_HWIN MDev_SetSWin;
void MDev_SetSWin_Task(WM_HWIN hParent, U8 sid)
{
    select_id = sid;
    MDev_SetSWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    Current_HWIN = MDev_SetSWin;
}
