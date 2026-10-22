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



//EventsFunctionList
//void OnxxxClicked(WM_MESSAGE * pMsg);
//void OnButtonClicked(WM_MESSAGE * pMsg);
//EndofEventsFunctionList


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
#define ButtonM_Xos  	(60)
#define ButtonM_Yos  	(120)
#define X_Space   		(250)
#define Y_Space   		(150)


static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect,    "",               0,                       0,  0,  800, 480, 0, 0},
    { TEXT_CreateIndirect,      "设备管理",          GUI_ID_TEXT0,            250, 4, 300, 42, 0, 0},

    { BUTTON_CreateIndirect,    "传感器",           GUI_ID_BUTTON3,          ButtonM_Xos + X_Space * 0, ButtonM_Yos + Y_Space * 0, 180, 65, 0, 0},
    { BUTTON_CreateIndirect,    "信号输入",         GUI_ID_BUTTON6,          ButtonM_Xos + X_Space * 0, ButtonM_Yos + Y_Space * 1, 180, 65, 0, 0},
		
    { BUTTON_CreateIndirect,    "模拟量",           GUI_ID_BUTTON4,           ButtonM_Xos + X_Space * 1, ButtonM_Yos + Y_Space * 0, 180, 65, 0, 0},
    { BUTTON_CreateIndirect,    "继电器",           GUI_ID_BUTTON5,           ButtonM_Xos + X_Space * 1, ButtonM_Yos + Y_Space * 1, 180, 65, 0, 0},

    { BUTTON_CreateIndirect,    "环境信息",         GUI_ID_BUTTON7,          ButtonM_Xos + X_Space * 2, ButtonM_Yos + Y_Space * 0, 180, 65, 0, 0},
    { BUTTON_CreateIndirect,    "生产数据",           GUI_ID_BUTTON8,               ButtonM_Xos + X_Space * 2, ButtonM_Yos + Y_Space * 1, 180, 65, 0, 0},
//    { BUTTON_CreateIndirect,    "湿帘水泵",           GUI_ID_BUTTON9,           ButtonM_Xos + X_Space * 2, ButtonM_Yos + Y_Space * 1, 120, 50, 0, 0},
//    { BUTTON_CreateIndirect,    "环流风机",           GUI_ID_BUTTON10,           ButtonM_Xos + X_Space * 3, ButtonM_Yos + Y_Space * 1, 120, 50, 0, 0},

//    { BUTTON_CreateIndirect,    "信号输入",           GUI_ID_BUTTON11,           ButtonM_Xos + X_Space * 0, ButtonM_Yos + Y_Space * 2, 120, 50, 0, 0},
//    { BUTTON_CreateIndirect,    "  ",           GUI_ID_BUTTON12,               ButtonM_Xos + X_Space * 1, ButtonM_Yos + Y_Space * 2, 120, 50, 0, 0},
//    { BUTTON_CreateIndirect,    "喷淋喷雾",          GUI_ID_BUTTON13,           ButtonM_Xos + X_Space * 2, ButtonM_Yos + Y_Space * 2, 120, 50, 0, 0},
//    { BUTTON_CreateIndirect,    "定时器",          GUI_ID_BUTTON14,             ButtonM_Xos + X_Space * 3, ButtonM_Yos + Y_Space * 2, 120, 50, 0, 0},

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

static const u16 LANG_KEY_NAME[] = {Lang_sensor, Lang_Analogy, Lang_Relay, Lang_Signal, Lang_Environment, Lang_Production};
static const char * KEY_PIC[] = {"58iconDev_Sensor", "59iconDev_Pwm", "62iconDev_JDQ", "61iconDev_Input", "60iconDev_Enviro", "63iconDev_Product"};

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

    switch (pMsg->MsgId) {
    case WM_PAINT:
        if (BUTTON_IsPressed(hWin)) {
            GUI_SetBkColor(GUI_WHITE);
            //				GUI_Clear();
            GUI_SetAlpha(0xb0);
            GUI_defDrawBitmap("56Mpic_KeyBack", 0, 0);
            GUI_defDrawBitmap(KEY_PIC[(Id - GUI_ID_BUTTON3)], 10, 15);
            ButtonText_Show(WM_GetDialogItem(hWin, Id), GUI_LANG_GetTextEx(LANG_KEY_NAME[(Id - GUI_ID_BUTTON3)], Language_Bit), 50, 15);

            GUI_SetAlpha(0);
        } else {
            GUI_defDrawBitmap("56Mpic_KeyBack", 0, 0);
            GUI_defDrawBitmap(KEY_PIC[(Id - GUI_ID_BUTTON3)], 10, 15);
            ButtonText_Show(WM_GetDialogItem(hWin, Id), GUI_LANG_GetTextEx(LANG_KEY_NAME[(Id - GUI_ID_BUTTON3)], Language_Bit), 50, 15);
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
    BUTTON_Handle hObj_Temp;

    //
    //GUI_ID_TEXT0
    //
    Text_SetHZ24(WM_GetDialogItem(hWin, GUI_ID_TEXT0));
    TEXT_SetTextColor(WM_GetDialogItem(hWin, GUI_ID_TEXT0), GUI_WHITE);

    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT0), GUI_LANG_GetTextEx(Lang_Device, Language_Bit)); // 设备管理

    for (IDMB = GUI_ID_BUTTON3; IDMB <= GUI_ID_BUTTON8; IDMB++) {
        hObj_Temp = WM_GetDialogItem(hWin, IDMB);
        WM_SetHasTrans(hObj_Temp);
        WM_SetCallback(hObj_Temp, _cbKey);	
		
    }

    for (IDMB = GUI_ID_BUTTON10; IDMB <= GUI_ID_BUTTON14; IDMB++) {
        hObj_Temp = WM_GetDialogItem(hWin, IDMB);
        Button_SetHZ24(hObj_Temp);
    }


    for (IDMB = GUI_ID_BUTTON1_0; IDMB <= GUI_ID_BUTTON1_6; IDMB++) {
        hObj_Temp = WM_GetDialogItem(hWin, IDMB);
        WM_SetHasTrans(hObj_Temp);
        WM_SetCallback(hObj_Temp, Main_cbButton);
    }

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
		
        if (Id >= GUI_ID_BUTTON1_0 && Id <= GUI_ID_BUTTON1_6) {
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Button_Menu(Id, hWin, GUI_ID_BUTTON1_1);
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

//        case GUI_ID_BUTTON2:
//            if (NCode == WM_NOTIFICATION_RELEASED)
//                GUI_EndDialog(hWin, 0);

//            break;

        case GUI_ID_BUTTON3:
            if (NCode == WM_NOTIFICATION_RELEASED)
                MDev_SetSensor_Task(hWin);

            break;

        case GUI_ID_BUTTON6:
            if (NCode == WM_NOTIFICATION_RELEASED)
                MDev_SignalInput_Task(hWin);

            break;

        case GUI_ID_BUTTON4:
            if (NCode == WM_NOTIFICATION_RELEASED)
				//MDev_FuncView_Task(WM_GetParent(hWin), 0);
                MDev_Regulatingfan_Task(hWin, 0);

            break;

        case GUI_ID_BUTTON5:
            if (NCode == WM_NOTIFICATION_RELEASED)
				//MDev_FuncView_Task(WM_GetParent(hWin), 1);
                MDev_FixedSpeedfan_Task(hWin, 0);

            break;

        case GUI_ID_BUTTON7:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                MDev_ASSensor_Task(hWin);
            }

            break;

        case GUI_ID_BUTTON8:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                MDev_Produce_Task(hWin);
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
WM_HWIN MDev_Man;
void MDev_Man_Task(WM_HWIN hParent)
{
    Main_Buffer=Main_Buffer_Dev;
    MDev_Man = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    Current_HWIN = MDev_Man;
}

