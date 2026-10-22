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
#include "usart.h"
#include "recode.h"
#include "SourceCode.h"
//EndofEventsFunctionList



/*********************************************************************
*
*       static data
*
**********************************************************************
*/

#define GUI_ID_DROPDOWN10 (GUI_ID_USER+50)
#define GUI_ID_DROPDOWN11 (GUI_ID_USER+51)
#define GUI_ID_DROPDOWN12 (GUI_ID_USER+52)
#define GUI_ID_DROPDOWN13 (GUI_ID_USER+53)
#define GUI_ID_DROPDOWN14 (GUI_ID_USER+54)

#define X  40
#define Y1  50
#define Y2  205
#define X_Space     260
#define Y_Space     50

#define X2_Space    93

static unsigned char last_port;
extern BYTE USARTCOMM;

static const u16 Lang_UartText_Buf[] = {Lang_Uart_num, Lang_Baud_rate, Lang_Data_bits, Lang_Stop_bit, Lang_CRC_bit};
/*********************************************************************
*
*       Dialog resource
*
* This table conatins the info required to create the dialog.
* It has been created by ucGUIbuilder.
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect,    "",               0,                       0,  0,  800, 480, 0, 0},
    { TEXT_CreateIndirect,      "系统管理-通信设置",  	GUI_ID_TEXT0,           300, 4, 200, 42, 0, 0},
    { TEXT_CreateIndirect,      "串口信息",  			GUI_ID_TEXT1,           X, 				Y1, 120, 36, 0, 0},
    { TEXT_CreateIndirect,      "串口号:",  			GUI_ID_TEXT2,           X - 20, 				Y1 + Y_Space, 120, 36, 0, 0},
    { TEXT_CreateIndirect,      "波特率:",  			GUI_ID_TEXT3,           X + X_Space - 20, 	Y1 + Y_Space, 120, 36, 0, 0},
    { TEXT_CreateIndirect,      "数据位:",  			GUI_ID_TEXT4,           X - 20, 				Y1 + Y_Space * 2, 120, 36, 0, 0},
    { TEXT_CreateIndirect,      "停止位:",  			GUI_ID_TEXT5,           X + X_Space - 20, 	Y1 + Y_Space * 2, 120, 36, 0, 0},
    { TEXT_CreateIndirect,      "校验位:",  			GUI_ID_TEXT6,           X + X_Space * 2 - 40, Y1 + Y_Space * 2, 120, 36, 0, 0},


    { DROPDOWN_CreateIndirect,  "",          			GUI_ID_DROPDOWN10,       X + X2_Space, 	Y1 + Y_Space, 120, 110, 0, 0},
    { DROPDOWN_CreateIndirect,  "",          			GUI_ID_DROPDOWN11,       X + X_Space + X2_Space, 	Y1 + Y_Space, 120, 145, 0, 0},
    { DROPDOWN_CreateIndirect,  "",          			GUI_ID_DROPDOWN12,       X + X2_Space, 	Y1 + Y_Space * 2, 120, 110, 0, 0},
    { DROPDOWN_CreateIndirect,  "",          			GUI_ID_DROPDOWN13,       X + X_Space + X2_Space, 	Y1 + Y_Space * 2, 120, 110, 0, 0},
    { DROPDOWN_CreateIndirect,  "",          			GUI_ID_DROPDOWN14,       X + X_Space * 2 + X2_Space-40, 	Y1 + Y_Space * 2, 160, 110, 0, 0},

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
//	WM_HWIN hWin = pMsg->hWin;
    Show_MBackPic();
    GUI_SetColor(GUI_LINE_COLOR);
    //GUI_DrawRoundedRect(20, 68, 775, 200, 10);
	My_DrawRoundedRect(20, 68, 775, 200, 10, 45, 150);
}

static void Set_OptValue(WM_HWIN hWin)
{
    DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN10), basic_inf.comm_info.ux.port);	
    DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN11), basic_inf.comm_info.baud);
    DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN12), basic_inf.comm_info.ux.dbits);
    DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN13), basic_inf.comm_info.ux.stopb);
    DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN14), basic_inf.comm_info.ux.parity);
}

static void Get_OptValue(WM_HWIN hWin)
{
    basic_inf.comm_info.ux.port = DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN10));	
    basic_inf.comm_info.baud = DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN11));
    basic_inf.comm_info.ux.dbits = DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN12));
    basic_inf.comm_info.ux.stopb = DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN13));
    basic_inf.comm_info.ux.parity = DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN14));
}


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

    for (MDev_ID = GUI_ID_TEXT0; MDev_ID <= GUI_ID_TEXT6; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24L(hItem);

        if (MDev_ID == GUI_ID_TEXT0 || MDev_ID == GUI_ID_TEXT1) {
            TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetTextColor(hItem, GUI_WHITE);
        }
        if (MDev_ID >= GUI_ID_TEXT2) {
            sprintf(show_buf, "%s:", GUI_LANG_GetTextEx(Lang_UartText_Buf[MDev_ID - GUI_ID_TEXT2], Language_Bit));
            TEXT_SetText(hItem, show_buf);
        }
    }

    for (MDev_ID = GUI_ID_DROPDOWN10; MDev_ID <= GUI_ID_DROPDOWN14; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Dropdown_SetHZ24(hItem);
        Login_Show(hItem, LOGIN_ADMIN);
    }

    sprintf(show_buf, "%s-%s", GUI_LANG_GetTextEx(Lang_System, Language_Bit), GUI_LANG_GetTextEx(Lang_Comm_Set, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT0), show_buf);                                               // 系统管理-通信设置
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT1), GUI_LANG_GetTextEx(Lang_Serial_port_inf, Language_Bit)); // 串口信息

    sprintf(show_buf, "%s%d", GUI_LANG_GetTextEx(Lang_UART, Language_Bit), 1);
    DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN10), show_buf);                                        // 串口1
    DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN10), GUI_LANG_GetTextEx(Lang_Built_in, Language_Bit)); // 内置

    DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN14), GUI_LANG_GetTextEx(Lang_NO_check, Language_Bit));   // 无校验
    DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN14), GUI_LANG_GetTextEx(Lang_Odd_check, Language_Bit));  // 奇校验
    DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN14), GUI_LANG_GetTextEx(Lang_Even_check, Language_Bit)); // 偶校验

    DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN11), GUI_LANG_GetTextEx(Lang_Disable, Language_Bit));  // 停用
    DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN11), "2400");
    DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN11), "4800");
    DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN11), "9600");
    DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN11), "19200");
    DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN11), "38400");
    DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN11), "57600");
    DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN11), "115200");

    DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN12), "7");
    DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN12), "8");

    DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN13), "1");
    DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN13), "1.5");
    DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN13), "2");

    Set_OptValue(hWin);
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

                if (Config_Save(SBASIC_ID) < 0) {
                    MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_SAVE_ER));
                } else {
	                if (last_port != basic_inf.comm_info.ux.port) {
		                MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_RESET));
	                    sys_sta.delay_rst = 5;
	                }
				
                    UartInit( USARTCOMM, basic_inf.comm_info.baud, basic_inf.comm_info.ux.dbits,
                              basic_inf.comm_info.ux.parity, basic_inf.comm_info.ux.stopb);
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
WM_HWIN MSys_CommSet;
void MSys_CommSet_Task(WM_HWIN hParent)
{
    last_port = basic_inf.comm_info.ux.port;	
    MSys_CommSet = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    Current_HWIN = MSys_CommSet;
}
