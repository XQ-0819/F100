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

static U8 select_type = 0;
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
#define GUI_ID_BUTTON15   (GUI_ID_USER+6)
#define GUI_ID_BUTTON16   (GUI_ID_USER+7)
#define GUI_ID_BUTTON17   (GUI_ID_USER+8)
#define GUI_ID_BUTTON18   (GUI_ID_USER+9)
#define GUI_ID_BUTTON19   (GUI_ID_USER+10)

#define GUI_ID_BUTTON20   (GUI_ID_USER+11)
#define GUI_ID_BUTTON21   (GUI_ID_USER+12)
#define GUI_ID_BUTTON22   (GUI_ID_USER+13)
#define GUI_ID_BUTTON23   (GUI_ID_USER+14)
#define GUI_ID_BUTTON24   (GUI_ID_USER+15)
#define GUI_ID_BUTTON25   (GUI_ID_USER+16)
#define GUI_ID_BUTTON26   (GUI_ID_USER+17)
#define GUI_ID_BUTTON27   (GUI_ID_USER+18)
#define GUI_ID_BUTTON28   (GUI_ID_USER+19)
#define GUI_ID_BUTTON29   (GUI_ID_USER+20)
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
#define X_SIZE			(-54)

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect,    "",               0,                        0,  0,  800, 480, 0, 0},
    { TEXT_CreateIndirect,      "控制管理",          GUI_ID_TEXT0,             300, 4, 200, 42, 0, 0},

    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON10,           ButtonM_Xos + X_Space * 0, ButtonM_Yos + Y_Space * 0, 180, 65, 0, 0, USER_BUF},
    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON11,           ButtonM_Xos + X_Space * 0, ButtonM_Yos + Y_Space * 1, 180, 65, 0, 0, USER_BUF},
    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON12,           ButtonM_Xos + X_Space * 0, ButtonM_Yos + Y_Space * 2, 180, 65, 0, 0, USER_BUF},
    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON13,           ButtonM_Xos + X_Space * 0, ButtonM_Yos + Y_Space * 3, 180, 65, 0, 0, USER_BUF},

    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON14,           ButtonM_Xos + X_Space * 1, ButtonM_Yos + Y_Space * 0, 180, 65, 0, 0, USER_BUF},
    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON15,           ButtonM_Xos + X_Space * 1, ButtonM_Yos + Y_Space * 1, 180, 65, 0, 0, USER_BUF},
    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON16,           ButtonM_Xos + X_Space * 1, ButtonM_Yos + Y_Space * 2, 180, 65, 0, 0, USER_BUF},
    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON17,           ButtonM_Xos + X_Space * 1, ButtonM_Yos + Y_Space * 3, 180, 65, 0, 0, USER_BUF},

    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON18,           ButtonM_Xos + X_Space * 2, ButtonM_Yos + Y_Space * 0, 180, 65, 0, 0, USER_BUF},
    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON19,           ButtonM_Xos + X_Space * 2, ButtonM_Yos + Y_Space * 1, 180, 65, 0, 0, USER_BUF},



    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON20,           X_SIZE + ButtonM_Xos + X_Space * 0, ButtonM_Yos + Y_Space * 0, 64, 64, 0, 0, USER_BUF},
    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON21,           X_SIZE + ButtonM_Xos + X_Space * 0, ButtonM_Yos + Y_Space * 1, 64, 64, 0, 0, USER_BUF},
    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON22,           X_SIZE + ButtonM_Xos + X_Space * 0, ButtonM_Yos + Y_Space * 2, 64, 64, 0, 0, USER_BUF},
    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON23,           X_SIZE + ButtonM_Xos + X_Space * 0, ButtonM_Yos + Y_Space * 3, 64, 64, 0, 0, USER_BUF},

    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON24,           X_SIZE + ButtonM_Xos + X_Space * 1, ButtonM_Yos + Y_Space * 0, 64, 64, 0, 0, USER_BUF},
    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON25,           X_SIZE + ButtonM_Xos + X_Space * 1, ButtonM_Yos + Y_Space * 1, 64, 64, 0, 0, USER_BUF},
    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON26,           X_SIZE + ButtonM_Xos + X_Space * 1, ButtonM_Yos + Y_Space * 2, 64, 64, 0, 0, USER_BUF},
    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON27,           X_SIZE + ButtonM_Xos + X_Space * 1, ButtonM_Yos + Y_Space * 3, 64, 64, 0, 0, USER_BUF},

    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON28,           X_SIZE + ButtonM_Xos + X_Space * 2, ButtonM_Yos + Y_Space * 0, 64, 64, 0, 0, USER_BUF},
    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON29,           X_SIZE + ButtonM_Xos + X_Space * 2, ButtonM_Yos + Y_Space * 1, 64, 64, 0, 0, USER_BUF},


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
}

extern void _cbDevKey(WM_MESSAGE * pMsg);
extern void _cbCtrlKey(WM_MESSAGE * pMsg);

const char * top_equ_PIC[] = {
    "00top_cancle", "00top",
};
void _cbTopKey(WM_MESSAGE * pMsg)
{
    WM_HWIN  hWin;
    hWin  = pMsg->hWin;
    int		show_id = 0;

    switch (pMsg->MsgId) {
    case WM_PAINT:
        BUTTON_GetUserData(hWin, &show_id, 1);

        if (BUTTON_IsPressed(hWin)) {
            GUI_SetBkColor(GUI_WHITE);
//				GUI_Clear();
            GUI_SetAlpha(0xb0);

            if (select_type == 0) {
                GUI_defDrawBitmap(top_equ_PIC[dev_inf.dev_equ_show[show_id]], 16, 16);
            } else {
                GUI_defDrawBitmap(top_equ_PIC[dev_inf.ctrl_equ_show[show_id]], 16, 16);
            }

            GUI_SetAlpha(0);
        } else {
            if (select_type == 0) {
                GUI_defDrawBitmap(top_equ_PIC[dev_inf.dev_equ_show[show_id]], 16, 16);
            } else {
                GUI_defDrawBitmap(top_equ_PIC[dev_inf.ctrl_equ_show[show_id]], 16, 16);
            }
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
    int MDev_ID = 0;
    WM_HWIN hItem, hItem2;
    U8 i = 0;


    //
    //GUI_ID_BUTTON0
    //

    for (MDev_ID = GUI_ID_BUTTON2; MDev_ID <= GUI_ID_BUTTON2; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, Cut_cbButton);
    }

    MDev_ID = GUI_ID_BUTTON10;

    if (select_type == 0) {
        for (i = 0; MDev_ID <= GUI_ID_BUTTON19; ) {
            hItem = WM_GetDialogItem(hWin, MDev_ID);
            hItem2 = WM_GetDialogItem(hWin, MDev_ID + 10);
            WM_SetHasTrans(hItem);
            WM_SetHasTrans(hItem2);

            for (; i < DEV_ITEM_NUM; i++) {
                if (sys_sta.dev_equ_en[i] != STA_ON) {//实际启用
                    continue;
                }

                BUTTON_SetUserData(hItem, &i, 1);
                WM_SetCallback(hItem, _cbDevKey);
                BUTTON_SetUserData(hItem2, &i, 1);
                WM_SetCallback(hItem2, _cbTopKey);


                i++;
                MDev_ID++;
                break;
            }


            if (i >= DEV_ITEM_NUM) {
                break;//没找到退出
            }
        }
    } else {
        for (i = 0; MDev_ID <= GUI_ID_BUTTON19; ) {
            hItem = WM_GetDialogItem(hWin, MDev_ID);
            hItem2 = WM_GetDialogItem(hWin, MDev_ID + 10);
            WM_SetHasTrans(hItem);
            WM_SetHasTrans(hItem2);

            for (; i < CTRL_ITEM_NUM; i++) {
                if (sys_sta.ctrl_equ_en[i] != STA_ON) {//实际启用
                    continue;
                }

                BUTTON_SetUserData(hItem, &i, 1);
                WM_SetCallback(hItem, _cbCtrlKey);
                BUTTON_SetUserData(hItem2, &i, 1);
                WM_SetCallback(hItem2, _cbTopKey);


                i++;
                MDev_ID++;
                break;
            }


            if (i >= CTRL_ITEM_NUM) {
                break;//没找到退出
            }
        }

    }

    for (; MDev_ID <= GUI_ID_BUTTON19; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        hItem2 = WM_GetDialogItem(hWin, MDev_ID + 10);
        WM_HideWindow(hItem);
        WM_HideWindow(hItem2);
    }


    for (MDev_ID = GUI_ID_TEXT0; MDev_ID <= GUI_ID_TEXT0; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24(hItem);

        if (MDev_ID == GUI_ID_TEXT0) {
            TEXT_SetTextAlign(hItem,  GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetTextColor(hItem, GUI_WHITE);
        }
    }

    TEXT_SetText(hItem, GUI_LANG_GetTextEx(Lang_Control, Language_Bit));//控制管理
}

extern void _cbDevCallback(WM_HWIN hWin, U8 id);
extern void _cbCtrlCallback(WM_HWIN hWin, U8 id);
/*********************************************************************
*
*       Dialog callback routine
*/

static void _cbCallback(WM_MESSAGE * pMsg)
{
    int NCode, Id;
    WM_HWIN hWin = pMsg->hWin;
    int		show_id = 0;

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

        if (Id >= GUI_ID_BUTTON10 && Id <= GUI_ID_BUTTON19) {
            if (NCode == WM_NOTIFICATION_RELEASED) {
                BUTTON_GetUserData(WM_GetDialogItem(hWin, Id), &show_id, 1);

                if (select_type == 0) {
                    _cbDevCallback(hWin, show_id);
                } else {
                    _cbCtrlCallback(hWin, show_id);
                }
            }

            break;
        }

        if (Id >= GUI_ID_BUTTON20 && Id <= GUI_ID_BUTTON29) {
            if (NCode == WM_NOTIFICATION_RELEASED) {
                BUTTON_GetUserData(WM_GetDialogItem(hWin, Id), &show_id, 1);

                if (select_type == 0) {
                    dev_inf.dev_equ_show[show_id] = !dev_inf.dev_equ_show[show_id];
                } else {
                    dev_inf.ctrl_equ_show[show_id] = !dev_inf.ctrl_equ_show[show_id];
                }
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


        case GUI_ID_BUTTON2:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                if (Config_Save(SDEV_SHOW_ID) < 0) {
                    MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_SAVE_ER));
                }

                Current_HWIN = WM_GetParent(hWin);
                GUI_EndDialog(hWin, 0);
                MCtrl_Man_Task(WM_GetParent(hWin));
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
WM_HWIN MCtrl_Man_More;
void MCtrl_Man_More_Task(WM_HWIN hParent, U8 type)
{
    select_type = type;
    MCtrl_Man_More = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    Current_HWIN = MCtrl_Man_More;

}

