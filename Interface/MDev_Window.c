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
#include "jdq.h"
#include "bsp.h"
#include "din.h"
#include "energy.h"
#include "recode.h"
#include "ucos_ii.h"

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
#define Ypos_Window   50


static U8 select_id = 0;
static U8 adjust_sta = ADJ_READY;
static U16 adjust_time = 0;
static WM_HTIMER hTimer;
static U8 adjust_flag = STA_OFF;

U8 get_win_adjust_sta(void)
{
    return adjust_flag;
}

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect,    "",               0,                       0,  0,  800, 480, 0, 0},
    { TEXT_CreateIndirect,      "设备管理-风窗",        GUI_ID_TEXT0,            250, 0, 300, 42, 0, 0},
    { TEXT_CreateIndirect,      "控制模式:",         GUI_ID_TEXT1,            	105 + Xpos_Window, 40 + Ypos_Window , 120, 36, 0, 0},
    { DROPDOWN_CreateIndirect,	 "",			 	GUI_ID_DROPDOWN0,			230 + Xpos_Window, 40 + Ypos_Window, 180, 110, 0, 0},
    { CHECKBOX_CreateIndirect,    "启用",            GUI_ID_CHECK0,             440 + Xpos_Window, 40 + Ypos_Window + 3, 100, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,    "启用",            GUI_ID_CHECK1,             550 + Xpos_Window, 40 + Ypos_Window + 3, 200, CHECK_SIZE, 0, 0},

    { TEXT_CreateIndirect,      "开启时间/秒:",       	GUI_ID_TEXT2,            95 + Xpos_Window, 100 + Ypos_Window, 185, 36, 0, 0},
    { EDIT_CreateIndirect,      " ",                	GUI_ID_EDIT0,            280 + Xpos_Window, 100 + Ypos_Window, 70, 36, 0, 0},
    { TEXT_CreateIndirect,      "关闭时间/秒:",        	GUI_ID_TEXT3,            400 + Xpos_Window, 100 + Ypos_Window, 170, 36, 0, 0},
    { EDIT_CreateIndirect,      " ",                	GUI_ID_EDIT1,            570 + Xpos_Window, 100 + Ypos_Window, 70, 36, 0, 0},

    { TEXT_CreateIndirect,      "全开电压/V:",       	GUI_ID_TEXT4,            105 + Xpos_Window, 160 + Ypos_Window, 140, 36, 0, 0},
    { EDIT_CreateIndirect,      " ",                	GUI_ID_EDIT2,            280 + Xpos_Window, 160 + Ypos_Window, 70, 36, 0, 0},
    { TEXT_CreateIndirect,      "全关电压/V:",        	GUI_ID_TEXT5,            400 + Xpos_Window, 160 + Ypos_Window, 140, 36, 0, 0},
    { EDIT_CreateIndirect,      " ",                	GUI_ID_EDIT3,            570 + Xpos_Window, 160 + Ypos_Window, 70, 36, 0, 0},


//    { TEXT_CreateIndirect,      "最小调节:",         GUI_ID_TEXT2,           150 + Xpos_Window, 100 + Ypos_Window , 120, 36, 0, 0},
//    { EDIT_CreateIndirect,      " ",                	GUI_ID_EDIT2,            270 + Xpos_Window, 100 + Ypos_Window, 70, 36, 0, 0},
//    { TEXT_CreateIndirect,      "%",        			GUI_ID_TEXT10,           350 + Xpos_Window, 100 + Ypos_Window, 50, 36, 0, 0},
//    { CHECKBOX_CreateIndirect,  "",         			GUI_ID_CHECK0,           400 + Xpos_Window, 100 + Ypos_Window + 3, 180, CHECK_SIZE, 0, 0},

//    { TEXT_CreateIndirect,      "自动校准:",        	GUI_ID_TEXT3,            150 + Xpos_Window, 160 + Ypos_Window, 120, 36, 0, 0},
//    { EDIT_CreateIndirect,      " ",                	GUI_ID_EDIT0,            270 + Xpos_Window, 160 + Ypos_Window, 70, 36, 0, 0},
//    { TEXT_CreateIndirect,      "次",        			GUI_ID_TEXT4,            350 + Xpos_Window, 160 + Ypos_Window, 50, 36, 0, 0},
//    { CHECKBOX_CreateIndirect,  "",         			GUI_ID_CHECK1,           400 + Xpos_Window, 160 + Ypos_Window + 3, 180, CHECK_SIZE, 0, 0},


//    { TEXT_CreateIndirect,      "行程时间:",       		GUI_ID_TEXT7,            150 + Xpos_Window, 220 + Ypos_Window, 110, 36, 0, 0},
//    { EDIT_CreateIndirect,      " ",                	GUI_ID_EDIT1,            270 + Xpos_Window, 220 + Ypos_Window, 70, 36, 0, 0},
//    { TEXT_CreateIndirect,      "秒",        			GUI_ID_TEXT9,            350 + Xpos_Window, 220 + Ypos_Window, 50, 36, 0, 0},
//    { TEXT_CreateIndirect,      "",        	 			GUI_ID_TEXT5,            430 + Xpos_Window, 160 + Ypos_Window, 170, 36, 0, 0},
//    { TEXT_CreateIndirect,      "",             		GUI_ID_TEXT6,            430 + Xpos_Window, 220 + Ypos_Window, 170, 36, 0, 0},


    { BUTTON_CreateIndirect,    "打开",          		GUI_ID_BUTTON3,         250, 240 + Ypos_Window, 100, 40, 0, 0},
    { BUTTON_CreateIndirect,    "关闭",          		GUI_ID_BUTTON4,         450, 240 + Ypos_Window, 100, 40, 0, 0},
    { TEXT_CreateIndirect,      "提示:请点击打开或者关闭得到开启时间!",  GUI_ID_TEXT8,      50,  285 + Ypos_Window, 700, 36, 0, 0},
//    { BUTTON_CreateIndirect,    "上一页",          GUI_ID_BUTTON0,          MENU_BACK_X1, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
//    { BUTTON_CreateIndirect,    "下一页",          GUI_ID_BUTTON1,          MENU_BACK_X2, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
    { BUTTON_CreateIndirect,    "返回",             GUI_ID_BUTTON2,         MENU_BACK_X3, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},

    { BUTTON_CreateIndirect,    "",     GUI_ID_BUTTON6,           			660, 0, 90, 60, 0, 0},

};


static void Set_OptValue(WM_HWIN hWin, U8 sid)
{
    CHECKBOX_SetState(WM_GetDialogItem(hWin, GUI_ID_CHECK0 ), dev_inf.WIN.detail[sid].mode);
    DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN0 ), dev_inf.WIN.detail[sid].ctrl_mode);
    CHECKBOX_SetState(WM_GetDialogItem(hWin, GUI_ID_CHECK1 ), dev_inf.WIN.detail[sid].prior_ctrl);

    //增加是否启用

    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT8), GUI_LANG_GetTextEx(Lang_Tips_OpenCloseTime, Language_Bit));
    sprintf(show_buf, "%d", dev_inf.WIN.detail[sid].open_time);
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT0), (const char *) show_buf);
    sprintf(show_buf, "%d", dev_inf.WIN.detail[sid].close_time);
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT1), (const char *) show_buf);

    if (dev_inf.WIN.detail[sid].ctrl_mode == WIN_ONLY_VAC_CTRL) {

        TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT2), GUI_LANG_GetTextEx(Lang_MoveTime_Closing, Language_Bit));
        TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT3), GUI_LANG_GetTextEx(Lang_StopTime_Closing, Language_Bit));

        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT4));
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT5));
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_CHECK0));
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_CHECK1));


        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT2));
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT3));

        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT8));
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON3));
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON4));
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON6));

        return;
    } else {
    	if(dev_inf.WIN.detail[sid].ctrl_mode != WIN_VAC_CTRL){
	        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_CHECK1));
    	}else{
	        WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_CHECK1));
    	}

        TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT2), GUI_LANG_GetTextEx(Lang_OpenAll_T, Language_Bit));
        TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT3), GUI_LANG_GetTextEx(Lang_CloseAll_T, Language_Bit));

        WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT4));
        WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT5));
        WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_CHECK0));

        WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT2));
        WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT3));

        WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT8));
        WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON3));
        WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON4));
        WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON6));
    }

    if (dev_inf.WIN.detail[sid].mode == WIN_MODE_POS) {
        WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT4));
        WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT5));
        WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT2));
        WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT3));

        sprintf(show_buf, "%.2f", dev_inf.WIN.detail[sid].open_v * 1.0 / VOL_RATE);
        EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT2), (const char *) show_buf);
        sprintf(show_buf, "%.2f", dev_inf.WIN.detail[sid].close_v * 1.0 / VOL_RATE);
        EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT3), (const char *) show_buf);

    } else if (dev_inf.WIN.detail[sid].mode == WIN_MODE_TIME) {
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT4));
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT5));
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT2));
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT3));

    }

}

static void Show_Value(WM_HWIN hWin, U8 sid)
{

    sprintf(show_buf, "%s-%s%d", GUI_LANG_GetTextEx(Lang_Device, Language_Bit), GUI_LANG_GetTextEx(Lang_CurtWinDow, Language_Bit), sid + 1);

    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT0), (const char *) show_buf);
    Set_OptValue(hWin, sid);
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
    GUI_DrawRoundedRect(100, 55, 710, 391, 10);
    GUI_DrawRoundedRect(96, 52, 714, 394, 10);

}



static void Get_OptValue(WM_HWIN hWin, U8 sid)
{
    dev_inf.WIN.detail[sid].ctrl_mode = DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN0));
    dev_inf.WIN.detail[sid].mode = CHECKBOX_GetState(WM_GetDialogItem(hWin, GUI_ID_CHECK0));
    dev_inf.WIN.detail[sid].prior_ctrl = CHECKBOX_GetState(WM_GetDialogItem(hWin, GUI_ID_CHECK1));

    Set_OptValue(hWin, sid);
}


static void Open_Time_Get(void)
{
    float ftemp = Get_Input_Value();
    short stemp = (short)ftemp;

    if (stemp > 255) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 255;
    }

//    if (stemp < 1) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = 1;
//    }

    dev_inf.WIN.detail[select_id].open_time = stemp;

    sprintf(show_buf, "%d", dev_inf.WIN.detail[select_id].open_time);
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

//    if (stemp < 1) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = 1;
//    }

    dev_inf.WIN.detail[select_id].close_time = stemp;

    sprintf(show_buf, "%d", dev_inf.WIN.detail[select_id].close_time);
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, GUI_ID_EDIT1), (const char *) show_buf);
}

static void OpenV_Get(void)
{
    float ftemp = Get_Input_Value() * VOL_RATE;
    short stemp = (short)ftemp;

//    if (stemp > TEMP_ADJMAX) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = TEMP_ADJMAX;
//    }

    if (stemp < 0) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 0;
    }

    dev_inf.WIN.detail[select_id].open_v = stemp;

    sprintf(show_buf, "%.2f", dev_inf.WIN.detail[select_id].open_v * 1.0 / VOL_RATE);
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, GUI_ID_EDIT2), (const char *) show_buf);
}

static void CloseV_Get(void)
{
    float ftemp = Get_Input_Value() * VOL_RATE;
    short stemp = (short)ftemp;

//    if (stemp > TEMP_ADJMAX) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = TEMP_ADJMAX;
//    }

    if (stemp < 0) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 0;
    }

    dev_inf.WIN.detail[select_id].close_v = stemp;

    sprintf(show_buf, "%.2f", dev_inf.WIN.detail[select_id].close_v * 1.0 / VOL_RATE);
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, GUI_ID_EDIT3), (const char *) show_buf);
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

    for (MDev_ID = GUI_ID_BUTTON6; MDev_ID <= GUI_ID_BUTTON6; MDev_ID++) {//set
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, set_cbButton);
    }

    for (MDev_ID = GUI_ID_BUTTON3; MDev_ID <= GUI_ID_BUTTON4; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Button_SetHZ24(hItem);
    }

    for (MDev_ID = GUI_ID_EDIT0; MDev_ID <= GUI_ID_EDIT3; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Edit_SetHZ24(hItem);
    }


    for (MDev_ID = GUI_ID_TEXT0; MDev_ID <= GUI_ID_TEXT5; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24L(hItem);

        if (MDev_ID == GUI_ID_TEXT0) {
            TEXT_SetTextAlign(hItem,  GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetTextColor(hItem, GUI_WHITE);
        }

    }

    for (MDev_ID = GUI_ID_TEXT8; MDev_ID <= GUI_ID_TEXT8; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24(hItem);
        TEXT_SetTextColor(hItem, GUI_LIGHTGRAY);
    }

    for (MDev_ID = GUI_ID_CHECK0; MDev_ID <= GUI_ID_CHECK0; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Check_SetHZ24(hItem);
        Login_Show(hItem, LOGIN_ADMIN);
    }
	
    for (MDev_ID = GUI_ID_CHECK1; MDev_ID <= GUI_ID_CHECK1; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        CHECKBOX_SetText(hItem, GUI_LANG_GetTextEx(Lang_PriorityCtrl, Language_Bit));    // 优先控制
        Check_SetHZ24(hItem);
        Login_Show(hItem, LOGIN_ADMIN);
    }

    for (MDev_ID = GUI_ID_DROPDOWN0; MDev_ID <= GUI_ID_DROPDOWN0; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Dropdown_SetHZ24(hItem);
        Login_Show(hItem, LOGIN_ADMIN);
    }

    DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN0), GUI_LANG_GetTextEx(Lang_Lv_C, Language_Bit));
    DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN0), GUI_LANG_GetTextEx(Lang_Lv_NP_C, Language_Bit));
    DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN0), GUI_LANG_GetTextEx(Lang_Lv_Only_NP_C, Language_Bit));

    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT1), GUI_LANG_GetTextEx(Lang_CTRL_mode, Language_Bit));    // 控制模式:
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT2), GUI_LANG_GetTextEx(Lang_Open_T, Language_Bit));       // 开启时间/秒:
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT3), GUI_LANG_GetTextEx(Lang_Close_T, Language_Bit));      // 关闭时间/秒:
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT4), GUI_LANG_GetTextEx(Lang_OpenAll_Vol, Language_Bit));  // 全开电压/V:
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT5), GUI_LANG_GetTextEx(Lang_CloseAll_Vol, Language_Bit)); // 全关电压/V:
    BUTTON_SetText(WM_GetDialogItem(hWin, GUI_ID_BUTTON3), GUI_LANG_GetTextEx(Lang_Open, Language_Bit));     // 打开
    BUTTON_SetText(WM_GetDialogItem(hWin, GUI_ID_BUTTON4), GUI_LANG_GetTextEx(Lang_Close, Language_Bit));    // 关闭
    CHECKBOX_SetText(WM_GetDialogItem(hWin, GUI_ID_CHECK0), GUI_LANG_GetTextEx(Lang_Pot, Language_Bit));     // 启用电位计

    hTimer = WM_CreateTimer(WM_GetClientWindow(hWin), 0, 250, 0);
    Show_Value(hWin, select_id);

}


static void	Show_Data(WM_HWIN hWin, U8 sid)
{
    static U8 time_num = 0;

    if (dev_inf.WIN.detail[sid].enable == STA_OFF) {
        return;
    }

    if (adjust_sta != ADJ_START) {
        return;
    }


    if (++time_num > 3) {
        time_num = 0;
        adjust_time ++;
    }

    if (dev_inf.WIN.detail[sid].mode == WIN_MODE_POS) {
        sprintf(show_buf, "%s:%s%dS,%s:%.2fV", GUI_LANG_GetTextEx(Lang_Current_state, Language_Bit), GUI_LANG_GetTextEx(Lang_Sec, Language_Bit), adjust_time, 
                                               GUI_LANG_GetTextEx(Lang_voltage, Language_Bit), sta_in.sensor_adj.WIN[sid] * 1.0 / VOL_RATE);
        TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT8), show_buf);
    } else {
        sprintf(show_buf, "%s:%s%dS", GUI_LANG_GetTextEx(Lang_Current_state, Language_Bit), GUI_LANG_GetTextEx(Lang_Sec, Language_Bit), adjust_time);
        TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT8), show_buf);
    }
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

    case WM_TIMER:
        Show_Data(hWin, select_id);

        if (adjust_sta == ADJ_START) {
            WM_RestartTimer(hTimer, 250);
        }

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
                if (adjust_sta > ADJ_READY && adjust_sta < ADJ_FINISH) {
                    ret = MMessage_Task(hWin, MESSAGE_ADJ_TIPS);

                    if (ret != MESSAGE_YES) {
                        break;
                    }

                    WIN_UP_CON(select_id, STA_OFF);
                    WIN_DOWN_CON(select_id, STA_OFF);
                }

                ADJ_ENFAN(select_id);
                Get_OptValue(hWin, select_id);

                if (dev_inf.WIN.detail[select_id].ctrl_mode == WIN_ONLY_VAC_CTRL) {
                    dev_inf.WIN.detail[select_id].mode = WIN_MODE_TIME;
                }

                if (Config_Save(SWIN_ID) < 0) {
                    MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_SAVE_ER));
                }

				AIN_WIN_CHK(1);

                adjust_flag = STA_OFF;
                Current_HWIN = WM_GetParent(hWin);
                GUI_EndDialog(hWin, 0);
            }

            break;


        case GUI_ID_CHECK0:
            if (NCode == WM_NOTIFICATION_VALUE_CHANGED) {
                Get_OptValue(hWin, select_id);
            }

            break;

        case GUI_ID_DROPDOWN0:
            if (NCode == WM_NOTIFICATION_SEL_CHANGED) {
                Get_OptValue(hWin, select_id);
            }

            break;

        case GUI_ID_BUTTON3://open 开始计时，开始显示电压值
            if (NCode == WM_NOTIFICATION_RELEASED) {
                if (dev_inf.WIN.detail[select_id].enable != STA_ON) {
                    break;
                }

                if (adjust_sta == ADJ_READY || adjust_sta == ADJ_FINISH) {
                    run_inf.WIN_Time[select_id] = 0;
                    run_inf.WIN_CalMode[select_id] = ADJ_READY;
                    run_inf.WIN_ErTime[select_id] = 0;
                    run_inf.WIN_RunMode[select_id] = WIN_RUN_STOP;
                    WIN_UP_CON(select_id, STA_OFF);
                    WIN_DOWN_CON(select_id, STA_OFF);//关闭正在动作的风窗
                    OSTimeDly(300);
                    adjust_flag = STA_ON;
                    adjust_sta = ADJ_START;
                    WIN_UP_CON(select_id, STA_ON);
                    adjust_time = 1;
                    WM_RestartTimer(hTimer, 250);
                    WM_DisableWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON4));

                    BUTTON_SetText(WM_GetDialogItem(hWin, GUI_ID_BUTTON3), GUI_LANG_GetTextEx(Lang_Stop, Language_Bit));

                } else if (adjust_sta == ADJ_START) {
                    adjust_sta = ADJ_FINISH;
                    WIN_UP_CON(select_id, STA_OFF);
                    WIN_DOWN_CON(select_id, STA_OFF);//关闭正在动作的风窗
                    WM_EnableWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON4));

                    BUTTON_SetText(WM_GetDialogItem(hWin, GUI_ID_BUTTON3), GUI_LANG_GetTextEx(Lang_Open, Language_Bit));
                }
            }

            break;

        case GUI_ID_BUTTON4://close
            if (NCode == WM_NOTIFICATION_RELEASED) {
                if (dev_inf.WIN.detail[select_id].enable != STA_ON) {
                    break;
                }

                if (adjust_sta == ADJ_READY || adjust_sta == ADJ_FINISH) {
                    run_inf.WIN_Time[select_id] = 0;
                    run_inf.WIN_CalMode[select_id] = ADJ_READY;
                    run_inf.WIN_ErTime[select_id] = 0;
                    run_inf.WIN_RunMode[select_id] = WIN_RUN_STOP;
                    WIN_UP_CON(select_id, STA_OFF);
                    WIN_DOWN_CON(select_id, STA_OFF);//关闭正在动作的风窗
                    OSTimeDly(300);
                    adjust_flag = STA_ON;
                    adjust_sta = ADJ_START;
                    WIN_DOWN_CON(select_id, STA_ON);
                    adjust_time = 1;
                    WM_RestartTimer(hTimer, 250);
                    WM_DisableWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON3));

                    BUTTON_SetText(WM_GetDialogItem(hWin, GUI_ID_BUTTON4), GUI_LANG_GetTextEx(Lang_Stop, Language_Bit));

                } else if (adjust_sta == ADJ_START) {
                    adjust_sta = ADJ_FINISH;
                    WIN_UP_CON(select_id, STA_OFF);
                    WIN_DOWN_CON(select_id, STA_OFF);//关闭正在动作的风窗
                    WM_EnableWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON3));
                    BUTTON_SetText(WM_GetDialogItem(hWin, GUI_ID_BUTTON4), GUI_LANG_GetTextEx(Lang_Close, Language_Bit));
                }

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

        case GUI_ID_EDIT2:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(OpenV_Get);
                Set_Input_Len(4);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT3:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(CloseV_Get);
                Set_Input_Len(4);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_BUTTON6:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                MDev_Window_Set_Task(hWin);
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

WM_HWIN MDev_SetWindow;
void MDev_SetWindow_Task(WM_HWIN hParent, U8 sid)
{
    select_id = sid;
    adjust_sta = ADJ_READY;
    MDev_SetWindow = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    Current_HWIN = MDev_SetWindow;
}
