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
static U8 lv_id = 0;
/*********************************************************************
*
*       static data
*
**********************************************************************
*/
char equ_name[PWM_NUM_MAX + JDQ_NUM_MAX][24] = {
    "调速1", "调速2", "调速3", "调速4",
    "定速1", "定速2", "定速3", "定速4", "定速5", "定速6", "定速7", "定速8", "定速9", "定速10",
    "定速11", "定速12", "定速13", "定速14", "定速15", "定速16", "定速17", "定速18", "定速19",
};

extern void listview_lv_sta(LISTVIEW_Handle listview0_hWin, U8 lvid, U8 id, U8 add_flag);
extern unsigned char list_type[];
extern unsigned char list_id[];
extern unsigned char list_max_num;

static U8 select_id = 0;
static U8 select_Row = 0;
/*********************************************************************
*
*       Dialog resource
*
* This table conatins the info required to create the dialog.
* It has been created by ucGUIbuilder.
*/
static TCHG_MENU Mchg_Menu[] = {
    {MSHOW, TYPE_TEXT, GUI_ID_TEXT0, "级别%d  --", 0, 1},
    {MSHOW, TYPE_TEXT, GUI_ID_TEXT4, "%s", 0, 1},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT0, "%d", 0, 1},

    {MEXIT, 0, 0, "", 0, 0},
};

static void Get_CHECKBOX(WM_HWIN hWin, unsigned short id)
{
    int MDev_ID = 0;

    for (MDev_ID = GUI_ID_CHECK0; MDev_ID < GUI_ID_CHECK4 ; MDev_ID++) {
        if (id != MDev_ID) {
            CHECKBOX_SetState(WM_GetDialogItem(hWin, MDev_ID), 0);
            continue;
        }

        if (CHECKBOX_GetState(WM_GetDialogItem(hWin, MDev_ID )) == 0)
            CHECKBOX_SetState(WM_GetDialogItem(hWin, MDev_ID), 1);

        if (list_type[select_id] == PWM_FAN_TYPE) {
            ctrl_inf.lv[lv_id].PWM[list_id[select_id]].mode = MDev_ID - GUI_ID_CHECK0;
        } else {
            ctrl_inf.lv[lv_id].JDQ[list_id[select_id]].mode = MDev_ID - GUI_ID_CHECK0;
        }
    }
}

static void Set_CHECKBOX(WM_HWIN hWin, unsigned char id)
{
    unsigned char i;

    for (i = 0; i < 4; i++) { //进来先全部设置为零
        CHECKBOX_SetState(WM_GetDialogItem(hWin, i + GUI_ID_CHECK0), 0);
    }

    if (list_type[select_id] == PWM_FAN_TYPE) {
        if (ctrl_inf.lv[lv_id].PWM[list_id[select_id]].mode > MODE_ROTATE)
            ctrl_inf.lv[lv_id].PWM[list_id[select_id]].mode = MODE_OFF;

        CHECKBOX_SetState(WM_GetDialogItem(hWin, ctrl_inf.lv[lv_id].PWM[list_id[select_id]].mode + GUI_ID_CHECK0), 1);
    } else {
        if (ctrl_inf.lv[lv_id].JDQ[list_id[select_id]].mode > MODE_ROTATE)
            ctrl_inf.lv[lv_id].JDQ[list_id[select_id]].mode = MODE_OFF;

        CHECKBOX_SetState(WM_GetDialogItem(hWin, ctrl_inf.lv[lv_id].JDQ[list_id[select_id]].mode + GUI_ID_CHECK0), 1);
    }
}

static void Menu_Value_Init(WM_HWIN hWin, U8 sid)
{
    U8 id = 0, show_id = 0;
    Mchg_Menu[id++].value = lv_id + 1;

    if (list_type[select_id] == PWM_FAN_TYPE) {
        if (ctrl_inf.lv_more.fan_auto_sort == STA_ON) {
            show_id = cal_inf.pwm_fan_showid[list_id[select_id]];
        } else {
            show_id = list_id[select_id];
        }

        strcpy(Mchg_Menu[0].text, GUI_LANG_GetTextEx(Lang_Level, Language_Bit));
        strcat(Mchg_Menu[0].text, "%d  --");
        Mchg_Menu[id++].value = (int)&equ_name[show_id][0];

        Set_CHECKBOX(hWin, select_id);
        Mchg_Menu[id].value = ctrl_inf.lv[lv_id].PWM[list_id[select_id]].degree;
        Mchg_Menu[id++].isshow = MSHOW;
        WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT2));
        WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT3));
    } else {
        if (ctrl_inf.lv_more.fan_auto_sort == STA_ON) {
            show_id = cal_inf.jdq_fan_showid[list_id[select_id]];
        } else {
            show_id = list_id[select_id];
        }

        strcpy(Mchg_Menu[0].text, GUI_LANG_GetTextEx(Lang_Level, Language_Bit));
        strcat(Mchg_Menu[0].text, "%d  --");
        Mchg_Menu[id++].value = (int)&equ_name[show_id + PWM_NUM_MAX][0];

        Set_CHECKBOX(hWin, select_id);
        Mchg_Menu[id].value = 0;
        Mchg_Menu[id++].isshow = MHIDE;                   //定速隐藏开度设置
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT2));
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT3));
    }

}

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect,    "",               0,                       150,  80,  500, 312, 0, 0},
    { TEXT_CreateIndirect,      "",          GUI_ID_TEXT0,            170, 0, 110, 42, 0, 0},
    { TEXT_CreateIndirect,      "",          GUI_ID_TEXT4,            270, 0, 100, 42, 0, 0},

    { TEXT_CreateIndirect,      "运行模式:",        GUI_ID_TEXT1,                30, 70, 120, 30, 0, 0},
    { CHECKBOX_CreateIndirect,  "停用",            GUI_ID_CHECK0,               155, 70, 130, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "持续运行",        GUI_ID_CHECK1,               300, 70, 130, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "时控启停",        GUI_ID_CHECK2,               155, 130, 130, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "时控轮转",        GUI_ID_CHECK3,               300, 130, 130, CHECK_SIZE, 0, 0},

    { TEXT_CreateIndirect,      "运行开度:",   GUI_ID_TEXT2,              30, 200, 120, 30, 0, 0},

    { EDIT_CreateIndirect,      "",        GUI_ID_EDIT0,               155, 200, 80, 30, 0, 0},
    { TEXT_CreateIndirect,      "%",   GUI_ID_TEXT3,              240, 200, 50, 30, 0, 0},



    { BUTTON_CreateIndirect,    "返回",             GUI_ID_BUTTON2,         300, 240, MENU_BACK_W, MENU_BACK_H, 0, 0},
    { BUTTON_CreateIndirect,    "",          GUI_ID_BUTTON3,          10, 140, 58, 58, 0, 0},
    { BUTTON_CreateIndirect,    "",          GUI_ID_BUTTON4,          432, 140, 58, 58, 0, 0},
//    { BUTTON_CreateIndirect,    "",     GUI_ID_BUTTON5,           			720, 0, 80, 40, 0, 0},

};


static void Degree_Get(void)
{
    float ftemp = Get_Input_Value();
    short stemp = (short)ftemp;

    if (stemp > MAX_DEGREE) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = MAX_DEGREE;
    }

    if (stemp < 0) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 0;
    }

    if (list_type[select_id] == PWM_FAN_TYPE) {
        ctrl_inf.lv[lv_id].PWM[list_id[select_id]].degree = stemp;
    }

    WM_MakeModal(Current_HWIN);  //模态化这个窗口
    MChange_Menu(Menu_Value_Init, Current_HWIN, (PCHG_MENU )Mchg_Menu, 0);
}


/*****************************************************************
**      FunctionName:void PaintDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**
**      call this function in _cbCallback --> WM_PAINT
*****************************************************************/
extern GUI_CONST_STORAGE GUI_BITMAP bmPage_Down;
extern GUI_CONST_STORAGE GUI_BITMAP bmPage_up;
static void PaintDialog(WM_MESSAGE * pMsg)
{
//    WM_HWIN hWin = pMsg->hWin;
    Show_MBackPic();
    GUI_SetColor(GUI_DEF_LGRAY);
    GUI_SetPenSize(5);
    GUI_DrawLine(0, 42, 0, 312);
    GUI_DrawLine(0, 312, 500, 312);
    GUI_DrawLine(500, 42, 500, 312);
    //GUI_FillRect(0,40,500,340);
//    GUI_DrawRoundedRect(6, 55, 794, 400, 10);
//    GUI_DrawRoundedRect(2, 52, 798, 403, 10);

}


/*****************************************************************
**      FunctionName:void InitDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**
**      call this function in _cbCallback --> WM_INIT_DIALOG
*****************************************************************/
static void Copy_cbButton(WM_MESSAGE * pMsg)
{
    WM_HWIN  hWin;
    hWin  = pMsg->hWin;

    switch (pMsg->MsgId) {
    case WM_PAINT:
        if (BUTTON_IsPressed(hWin)) {
            GUI_SetBkColor(GUI_WHITE);
            GUI_SetAlpha(0xb0);
            GUI_defDrawBitmap("43bmMPic_Up", 5, 5);
            GUI_SetAlpha(0);
        } else {
            GUI_defDrawBitmap("43bmMPic_Up", 5, 5);
        }

        break;

    default:
        BUTTON_Callback(pMsg);
    }
}

static void Copy_cbButton1(WM_MESSAGE * pMsg)
{
    WM_HWIN  hWin;
    hWin  = pMsg->hWin;

    switch (pMsg->MsgId) {
    case WM_PAINT:
        if (BUTTON_IsPressed(hWin)) {
            GUI_SetBkColor(GUI_WHITE);
            GUI_SetAlpha(0xb0);
            GUI_defDrawBitmap("44bmMPic_Down", 5, 5);
            GUI_SetAlpha(0);
        } else {
            GUI_defDrawBitmap("44bmMPic_Down", 5, 5);
        }

        break;

    default:
        BUTTON_Callback(pMsg);
    }
}


static void InitDialog(WM_MESSAGE * pMsg)
{
    WM_HWIN hWin = pMsg->hWin;
    int MDev_ID = 0;
    WM_HWIN hItem;

    //
    //GUI_ID_BUTTON0
    //
    for (MDev_ID = GUI_ID_BUTTON2; MDev_ID <= GUI_ID_BUTTON2; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, Cut_cbButton);
    }

    hItem = WM_GetDialogItem(hWin, GUI_ID_BUTTON3);
    WM_SetHasTrans(hItem);
    WM_SetCallback(hItem, Copy_cbButton);

    hItem = WM_GetDialogItem(hWin, GUI_ID_BUTTON4);
    WM_SetHasTrans(hItem);
    WM_SetCallback(hItem, Copy_cbButton1);

    for (MDev_ID = GUI_ID_EDIT0; MDev_ID <= GUI_ID_EDIT0; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Edit_SetHZ24(hItem);
    }

    for (MDev_ID = GUI_ID_CHECK0; MDev_ID <= GUI_ID_CHECK3; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Check_SetHZ24(hItem);
        Login_Show(hItem, LOGIN_ADMIN);

        switch (MDev_ID) {
        case GUI_ID_CHECK0:
            CHECKBOX_SetText(hItem, GUI_LANG_GetTextEx(Lang_Disable, Language_Bit)); // 停用
            break;
        case GUI_ID_CHECK1:
            CHECKBOX_SetText(hItem, GUI_LANG_GetTextEx(Lang_Continuous, Language_Bit)); // 持续运行
            break;
        case GUI_ID_CHECK2:
            CHECKBOX_SetText(hItem, GUI_LANG_GetTextEx(Lang_TimeCycle, Language_Bit)); // 时控启停
            break;
        case GUI_ID_CHECK3:
            CHECKBOX_SetText(hItem, GUI_LANG_GetTextEx(Lang_TimeRotate, Language_Bit)); // 时控轮转
            break;
        }
    }

    for (MDev_ID = GUI_ID_TEXT0; MDev_ID <= GUI_ID_TEXT4; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24(hItem);

        if (MDev_ID == GUI_ID_TEXT0 || MDev_ID == GUI_ID_TEXT4) {
            TEXT_SetTextAlign(hItem,  GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetTextColor(hItem, GUI_WHITE);
        }
    }

    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT1), GUI_LANG_GetTextEx(Lang_RunMode, Language_Bit));    // 运行模式:
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT2), GUI_LANG_GetTextEx(Lang_Percent, Language_Bit));    // 运行开度:
    BUTTON_SetText(WM_GetDialogItem(hWin, GUI_ID_BUTTON2), GUI_LANG_GetTextEx(Lang_Return, Language_Bit)); // 返回

    MChange_Menu(Menu_Value_Init, hWin, (PCHG_MENU )Mchg_Menu, 0);

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

    case WM_TIMER:
//        LVInf_Show_Data(pMsg);
//        WM_RestartTimer(hTimer, 300);
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
        case GUI_ID_CHECK0:
        case GUI_ID_CHECK1:
        case GUI_ID_CHECK2:
        case GUI_ID_CHECK3:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Get_CHECKBOX(hWin, Id);
                MChange_Menu(Menu_Value_Init, hWin, (PCHG_MENU )Mchg_Menu, 0);
            }

            break;

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
                listview_lv_sta(WM_GetDialogItem(Current_HWIN , GUI_ID_LISTVIEW0), lv_id, select_Row, 0);
                GUI_EndDialog(hWin, 0);
            }

            break;


        case GUI_ID_BUTTON3:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                if (select_id > 0
                        && (list_type[select_id - 1] == PWM_FAN_TYPE || list_type[select_id - 1] == JDQ_FAN_TYPE)) {
                    select_id --;
                    MChange_Menu(Menu_Value_Init, hWin, (PCHG_MENU )Mchg_Menu, 0);
                }
            }

            break;

        case GUI_ID_BUTTON4:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                if (select_id < list_max_num - 1) {
                    select_id ++;
                    MChange_Menu(Menu_Value_Init, hWin, (PCHG_MENU )Mchg_Menu, 0);
                }
            }

            break;

        case GUI_ID_EDIT0:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(Degree_Get);
                Set_Input_Len(3);
                Input_Task(WM_GetParent(hWin));
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
WM_HWIN MCtrl_LVPFAN_SET;
void MCtrl_LVFAN_SET_Task(WM_HWIN hParent, unsigned char lv, unsigned char Row, unsigned char id)
{
    u8 i=0;
    if (id >= list_max_num || lv >= CTRL_LV_NUM) {
        return;
    }

    lv_id = lv;
    select_id = id;
    select_Row = Row;
    for (i = 0; i < PWM_NUM_MAX; i++) {
        sprintf(equ_name[i], "%s%d", GUI_LANG_GetTextEx(Lang_Exh, Language_Bit), (i + 1));
    }
    for (; i < PWM_NUM_MAX + JDQ_NUM_MAX; i++) {
        sprintf(equ_name[i], "%s%d", GUI_LANG_GetTextEx(Lang_StaFAN, Language_Bit), (i - PWM_NUM_MAX + 1));
    }
    MCtrl_LVPFAN_SET = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    Current_HWIN = MCtrl_LVPFAN_SET;
    WM_MakeModal(Current_HWIN); // 模态化这个窗口
}

