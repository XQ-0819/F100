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

static unsigned short food_adj = 0;
static U8 select_id = 0;

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect,    "",               0,                       0,  0,  800, 480, 0, 0},
    { TEXT_CreateIndirect,      "设备管理-料塔标定",          GUI_ID_TEXT0,            250, 4, 300, 42, 0, 0},

    { TEXT_CreateIndirect,      "",  GUI_ID_TEXT1,      50,  80, 700, 36, 0, 0},

    { EDIT_CreateIndirect,      "0 Kg",                GUI_ID_EDIT0,            325, 250, 150, 36, 0, 0},
    { BUTTON_CreateIndirect,    "零点标定",           	  GUI_ID_BUTTON3,          325, 130, 150, 45, 0, 0},
    { BUTTON_CreateIndirect,    "量程标定",           	  GUI_ID_BUTTON4,          325, 300, 150, 45, 0, 0},
    { TEXT_CreateIndirect,      "提示:请空载稳定后点击零点标定!",  GUI_ID_TEXT2,      50,  180, 700, 36, 0, 0},
    { TEXT_CreateIndirect,      "提示:请加载重物输入重量后点击量程标定!",  GUI_ID_TEXT3,      50, 350 , 700, 36, 0, 0},




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
    GUI_DrawRoundedRect(160, 70, 650, 390, 15);
    GUI_DrawRoundedRect(157, 67, 653, 393, 15);
}

static void Food_Adj_Get(void)
{
    float ftemp = Get_Input_Value();
    short stemp = (short)ftemp;

//    if (stemp > TEMP_ADJMAX) {
//        MWarning_Task(Current_HWIN, tips_menu[TIPS_LIMIT_ER]);
//        stemp = TEMP_ADJMAX;
//    }

    if (stemp < 1) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 1;
    }

    food_adj = stemp;

    sprintf(show_buf, "%d kg", food_adj);
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, GUI_ID_EDIT0), show_buf);
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
    for (MDev_ID = GUI_ID_BUTTON2; MDev_ID <= GUI_ID_BUTTON2; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, Cut_cbButton);
    }

    for (MDev_ID = GUI_ID_BUTTON3; MDev_ID <= GUI_ID_BUTTON4; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Button_SetHZ24(hItem);
    }


    for (MDev_ID = GUI_ID_EDIT0; MDev_ID <= GUI_ID_EDIT0; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Edit_SetHZ24(hItem);
    }

    for (MDev_ID = GUI_ID_TEXT0; MDev_ID <= GUI_ID_TEXT3; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24L(hItem);

        if (MDev_ID == GUI_ID_TEXT0) {
            TEXT_SetTextAlign(hItem,  GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetTextColor(hItem, GUI_WHITE);
        } else {
            TEXT_SetTextAlign(hItem,  GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetTextColor(hItem, GUI_LIGHTGRAY);
        }
    }

    TEXT_SetTextColor(WM_GetDialogItem(hWin, GUI_ID_TEXT1), GUI_WHITE);
    sprintf(show_buf, "%s:%d,%s:%d", GUI_LANG_GetTextEx(Lang_Feed_Bin, Language_Bit), select_id + 1, GUI_LANG_GetTextEx(Lang_Addr, Language_Bit), ext_inf.din.pro_data[select_id].food_addr);
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT1), show_buf);

    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT2), GUI_LANG_GetTextEx(Lang_Feed_Bin, Language_Bit)); // 提示:请空载稳定后点击零点标定!
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT3), GUI_LANG_GetTextEx(Lang_Feed_Bin, Language_Bit)); // 提示:请加载重物输入重量后点击量程标定!
    sprintf(show_buf, "%s-%s", GUI_LANG_GetTextEx(Lang_Device, Language_Bit), GUI_LANG_GetTextEx(Lang_Feed_Bins_ADJ, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT0), show_buf); // 设备管理-料塔标定

    BUTTON_SetText(WM_GetDialogItem(hWin, GUI_ID_BUTTON3), GUI_LANG_GetTextEx(Lang_zero_calibrate, Language_Bit));   // 零点标定
    BUTTON_SetText(WM_GetDialogItem(hWin, GUI_ID_BUTTON4), GUI_LANG_GetTextEx(Lang_weight_calibrate, Language_Bit)); // 量程标定

    sprintf(show_buf, "%d kg", food_adj);
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT0), show_buf);
}


/*********************************************************************
*
*       Dialog callback routine
*/
static void _cbCallback(WM_MESSAGE * pMsg)
{
    int NCode, Id, ret;
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
		
        if (Id >= GUI_ID_BUTTON3 && Id <= GUI_ID_BUTTON4 && NCode == WM_NOTIFICATION_RELEASED
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
                sys_sta.food_adjut_state = STA_OFF;//恢复采集
                Current_HWIN = WM_GetParent(hWin);
                GUI_EndDialog(hWin, 0);
            }

            break;

        case GUI_ID_EDIT0:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(Food_Adj_Get);
                Set_Input_Len(5);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_BUTTON3:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                ret = MMessage_Task(hWin, MESSAGE_FOOD_ADJ);

                if (ret != MESSAGE_YES) {
                    break;
                }

                if (food_adjust_proc(select_id, FOOD_ADJUST_SYN, 0) < 0) {
                    MWarning_Task(Current_HWIN, Get_Tips(TIPS_CAL_ER));
                    break;
                }

                if (food_adjust_proc(select_id, FOOD_ADJUST_ZERO, 0) < 0) {
                    MWarning_Task(Current_HWIN, Get_Tips(TIPS_CAL_ER));
                    break;
                }

                MWarning_Task(Current_HWIN, Get_Tips(TIPS_CAL_OK));
            }

            break;

        case GUI_ID_BUTTON4:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                ret = MMessage_Task(hWin, MESSAGE_FOOD_ADJ);

                if (ret != MESSAGE_YES) {
                    break;
                }
				
                if (food_adjust_proc(select_id, FOOD_ADJUST_SYN, 0) < 0) {
                    MWarning_Task(Current_HWIN, Get_Tips(TIPS_CAL_ER));
                    break;
                }

                if (food_adjust_proc(select_id, FOOD_ADJUST_NUM, food_adj) < 0) {
                    MWarning_Task(Current_HWIN, Get_Tips(TIPS_CAL_ER));
                    break;
                }

                MWarning_Task(Current_HWIN, Get_Tips(TIPS_CAL_OK));
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
WM_HWIN MDev_FoodADJ;
void MDev_FoodADJ_Task(WM_HWIN hParent, U8 id)
{
    food_adj = 100;
    select_id = id;
    sys_sta.food_adjut_state = STA_ON;//暂停采集
    food_adjust_int(select_id);

    MDev_FoodADJ = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    Current_HWIN = MDev_FoodADJ;
}

