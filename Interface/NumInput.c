#include "menuinc.h"
#include <stdio.h>
#include <string.h>


#define GUI_ID_BUTTON10 (GUI_ID_USER + 1)
#define GUI_ID_BUTTON11 (GUI_ID_USER + 2)
#define GUI_ID_BUTTON12 (GUI_ID_USER + 3)
#define GUI_ID_BUTTON13 (GUI_ID_USER + 4)
#define GUI_ID_BUTTON14 (GUI_ID_USER + 5)
#define GUI_ID_BUTTON15 (GUI_ID_USER + 6)
#define GUI_ID_BUTTON16 (GUI_ID_USER + 7)
#define GUI_ID_BUTTON17 (GUI_ID_USER + 8)
#define GUI_ID_BUTTON18 (GUI_ID_USER + 9)
#define GUI_ID_BUTTON19 (GUI_ID_USER + 10)
#define GUI_ID_BUTTON20 (GUI_ID_USER + 11)


static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect,  NULL,           0,           200,  0,  400, 480, FRAMEWIN_CF_ACTIVE, 0},
    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT0,            30, 50, 336, 50, 0, 0},

    { BUTTON_CreateIndirect,    "7",                 GUI_ID_BUTTON17,          17, 120, 65, 65, 0, 0},
    { BUTTON_CreateIndirect,    "8",                 GUI_ID_BUTTON18,          116, 120, 65, 65, 0, 0},
    { BUTTON_CreateIndirect,    "9",                 GUI_ID_BUTTON19,          215, 120, 65, 65, 0, 0},
    { BUTTON_CreateIndirect,    "C",                GUI_ID_BUTTON1,          314, 120, 65, 65, 0, 0},

    { BUTTON_CreateIndirect,    "4",                 GUI_ID_BUTTON14,          18, 210, 65, 65, 0, 0},
    { BUTTON_CreateIndirect,    "5",                 GUI_ID_BUTTON15,          117, 210, 65, 65, 0, 0},
    { BUTTON_CreateIndirect,    "6",                 GUI_ID_BUTTON16,          216, 210, 65, 65, 0, 0},
    { BUTTON_CreateIndirect,    "取消",            GUI_ID_BUTTON0,          315, 210, 65, 65, 0, 0},

    { BUTTON_CreateIndirect,    "1",                 GUI_ID_BUTTON11,          18, 300, 65, 65, 0, 0},
    { BUTTON_CreateIndirect,    "2",                 GUI_ID_BUTTON12,          117, 300, 65, 65, 0, 0},
    { BUTTON_CreateIndirect,    "3",                 GUI_ID_BUTTON13,          216, 300, 65, 65, 0, 0},
    { BUTTON_CreateIndirect,    "确\n\n定",              GUI_ID_BUTTON2,          315, 300, 65, 155, 0, 0},

    { BUTTON_CreateIndirect,    "-",                 GUI_ID_BUTTON3,          17, 390, 65, 65, 0, 0},
    { BUTTON_CreateIndirect,    "0",                 GUI_ID_BUTTON10,          116, 390, 65, 65, 0, 0},
    { BUTTON_CreateIndirect,    ".",                 GUI_ID_BUTTON20,          215, 390, 65, 65, 0, 0},

    { TEXT_CreateIndirect,      "",        		GUI_ID_TEXT0,         48, 5, 304, 42, 0, 0},
};


unsigned char input_len = 4;
float input_num = 0;
unsigned char input_buf[24];
const char *Display_Buff = NULL;
const char NO_char_buff[12] = {"Number Pad"};
static void PaintDialog(WM_MESSAGE * pMsg)
{
//	WM_HWIN hWin = pMsg->hWin;
    GUI_SetColor(GUI_DEF_GREEN);
    GUI_DrawRect(0, 0, 399, 479);
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
    //
    //FRAMEWIN
    //
//    FRAMEWIN_SetTitleVis(hWin,24);
    //
    //GUI_ID_EDIT0
    //
    WINDOW_SetBkColor(hWin, GUI_GRAY_E7);

    EDIT_SetFont(WM_GetDialogItem(hWin, GUI_ID_EDIT0), &GUI_FontD32);

    EDIT_SetTextAlign(WM_GetDialogItem(hWin, GUI_ID_EDIT0), GUI_TA_VCENTER | GUI_TA_RIGHT);
    WM_DisableWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT0));
    EDIT_SetTextColor(WM_GetDialogItem(hWin, GUI_ID_EDIT0), EDIT_CI_DISABLED, GUI_RED);

    EDIT_SetMaxLen(WM_GetDialogItem(hWin, GUI_ID_EDIT0), input_len);


//    BUTTON_SetSkinFlexProps(&BUTTON_Props_Set_EN, BUTTON_SKINFLEX_PI_ENABLED);
//    BUTTON_SetSkinFlexProps(&BUTTON_Props_Set_EN, BUTTON_SKINFLEX_PI_FOCUSSED);
//    BUTTON_SetSkinFlexProps(&BUTTON_Props_Set_PR, BUTTON_SKINFLEX_PI_PRESSED);
    BUTTON_SetDefaultTextColor(GUI_WHITE, BUTTON_CI_UNPRESSED);

    //
    //GUI_ID_BUTTON0
    //

    BUTTON_SetFont(WM_GetDialogItem(hWin, GUI_ID_BUTTON0), &GUI_USER_HZ24);

    BUTTON_SetBkColor(WM_GetDialogItem(hWin, GUI_ID_BUTTON0), BUTTON_CI_UNPRESSED, GUI_BUTTON_COLOR);
    //
    //GUI_ID_BUTTON1
    //
    BUTTON_SetFont(WM_GetDialogItem(hWin, GUI_ID_BUTTON1), GUI_FONT_32B_ASCII);
    BUTTON_SetBkColor(WM_GetDialogItem(hWin, GUI_ID_BUTTON1), BUTTON_CI_UNPRESSED, GUI_BUTTON_COLOR);

    BUTTON_SetFont(WM_GetDialogItem(hWin, GUI_ID_BUTTON3), &GUI_FontD32);
    BUTTON_SetBkColor(WM_GetDialogItem(hWin, GUI_ID_BUTTON3), BUTTON_CI_UNPRESSED, GUI_BUTTON_COLOR);
    //
    //GUI_ID_BUTTON2
    //

    BUTTON_SetFont(WM_GetDialogItem(hWin, GUI_ID_BUTTON2), &GUI_USER_HZ24);

    BUTTON_SetBkColor(WM_GetDialogItem(hWin, GUI_ID_BUTTON2), BUTTON_CI_UNPRESSED, GUI_BUTTON_COLOR);
    BUTTON_SetTextAlign(WM_GetDialogItem(hWin, GUI_ID_BUTTON2),  GUI_TA_HCENTER | GUI_TA_VCENTER);

    //
    //GUI_ID_BUTTON10
    //
    BUTTON_SetFont(WM_GetDialogItem(hWin, GUI_ID_BUTTON10), &GUI_FontD32);
    BUTTON_SetBkColor(WM_GetDialogItem(hWin, GUI_ID_BUTTON10), BUTTON_CI_UNPRESSED, GUI_BUTTON_COLOR);
    //
    //GUI_ID_BUTTON11
    //
    BUTTON_SetFont(WM_GetDialogItem(hWin, GUI_ID_BUTTON11), &GUI_FontD32);
    BUTTON_SetBkColor(WM_GetDialogItem(hWin, GUI_ID_BUTTON11), BUTTON_CI_UNPRESSED, GUI_BUTTON_COLOR);
    //
    //GUI_ID_BUTTON12
    //
    BUTTON_SetFont(WM_GetDialogItem(hWin, GUI_ID_BUTTON12), &GUI_FontD32);
    BUTTON_SetBkColor(WM_GetDialogItem(hWin, GUI_ID_BUTTON12), BUTTON_CI_UNPRESSED, GUI_BUTTON_COLOR);
    //
    //GUI_ID_BUTTON13
    //
    BUTTON_SetFont(WM_GetDialogItem(hWin, GUI_ID_BUTTON13), &GUI_FontD32);
    BUTTON_SetBkColor(WM_GetDialogItem(hWin, GUI_ID_BUTTON13), BUTTON_CI_UNPRESSED, GUI_BUTTON_COLOR);
    //
    //GUI_ID_BUTTON14
    //
    BUTTON_SetFont(WM_GetDialogItem(hWin, GUI_ID_BUTTON14), &GUI_FontD32);
    BUTTON_SetBkColor(WM_GetDialogItem(hWin, GUI_ID_BUTTON14), BUTTON_CI_UNPRESSED, GUI_BUTTON_COLOR);
    //
    //GUI_ID_BUTTON15
    //
    BUTTON_SetFont(WM_GetDialogItem(hWin, GUI_ID_BUTTON15), &GUI_FontD32);
    BUTTON_SetBkColor(WM_GetDialogItem(hWin, GUI_ID_BUTTON15), BUTTON_CI_UNPRESSED, GUI_BUTTON_COLOR);
    //
    //GUI_ID_BUTTON16
    //
    BUTTON_SetFont(WM_GetDialogItem(hWin, GUI_ID_BUTTON16), &GUI_FontD32);
    BUTTON_SetBkColor(WM_GetDialogItem(hWin, GUI_ID_BUTTON16), BUTTON_CI_UNPRESSED, GUI_BUTTON_COLOR);
    //
    //GUI_ID_BUTTON17
    //
    BUTTON_SetFont(WM_GetDialogItem(hWin, GUI_ID_BUTTON17), &GUI_FontD32);
    BUTTON_SetBkColor(WM_GetDialogItem(hWin, GUI_ID_BUTTON17), BUTTON_CI_UNPRESSED, GUI_BUTTON_COLOR);
    //
    //GUI_ID_BUTTON18
    //
    BUTTON_SetFont(WM_GetDialogItem(hWin, GUI_ID_BUTTON18), &GUI_FontD32);
    BUTTON_SetBkColor(WM_GetDialogItem(hWin, GUI_ID_BUTTON18), BUTTON_CI_UNPRESSED, GUI_BUTTON_COLOR);
    //
    //GUI_ID_BUTTON19
    //
    BUTTON_SetFont(WM_GetDialogItem(hWin, GUI_ID_BUTTON19), &GUI_FontD32);
    BUTTON_SetBkColor(WM_GetDialogItem(hWin, GUI_ID_BUTTON19), BUTTON_CI_UNPRESSED, GUI_BUTTON_COLOR);
    //
    //GUI_ID_BUTTON20
    //
    BUTTON_SetFont(WM_GetDialogItem(hWin, GUI_ID_BUTTON20), &GUI_FontD32);
    BUTTON_SetBkColor(WM_GetDialogItem(hWin, GUI_ID_BUTTON20), BUTTON_CI_UNPRESSED, GUI_BUTTON_COLOR);


    Text_SetHZ24(WM_GetDialogItem(hWin, GUI_ID_TEXT0));
    TEXT_SetTextAlign(WM_GetDialogItem(hWin, GUI_ID_TEXT0),  GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetTextColor(WM_GetDialogItem(hWin, GUI_ID_TEXT0), GUI_DEF_GREEN);

    TEXT_SetFont(WM_GetDialogItem(hWin, GUI_ID_TEXT0), &GUI_USER_HZ24);
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT0), NO_char_buff);
    if (Language_Bit_original == LANGUAGE_GBK) {
        BUTTON_SetText(WM_GetDialogItem(hWin, GUI_ID_BUTTON0), GUI_LANG_GetTextEx(Lang_Cancel, Language_Bit));
        BUTTON_SetText(WM_GetDialogItem(hWin, GUI_ID_BUTTON2), GUI_LANG_GetTextEx(Lang_Done, Language_Bit));
    } else {
        BUTTON_SetText(WM_GetDialogItem(hWin, GUI_ID_BUTTON0), "C L");
        BUTTON_SetText(WM_GetDialogItem(hWin, GUI_ID_BUTTON2), "Done");
    }
}

void Set_Display_Buff(char *buf)
{
	Display_Buff = buf;
}
void Set_Input_Len(unsigned char len)
{
    input_len = len;
}
float Get_Input_Value(void)
{
    return input_num;
}

unsigned char * Get_Input_Text(void)
{
    return input_buf;
}



void (*Set_Callback)(void);
void Set_Output_Callback(void (*p)(void))
{
    if (p == NULL) {
        Set_Callback = NULL;
        return;
    }

    Set_Callback = p;
}

static void _cbCallback(WM_MESSAGE * pMsg)
{
    int NCode, Id;
    WM_HWIN hWin = pMsg->hWin;

//  WM_HWIN para;
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

    case WM_NOTIFY_PARENT:
        Id = WM_GetId(pMsg->hWinSrc);
        NCode = pMsg->Data.v;
		
        if (NCode == WM_NOTIFICATION_CLICKED) {
			beep_on();
		}
		
        switch (Id) {
        case GUI_ID_BUTTON0:
            if (NCode == WM_NOTIFICATION_RELEASED) {
//                BUTTON_SetSkinFlexProps(&BUTTON_Props_Def_EN, BUTTON_SKINFLEX_PI_ENABLED);
//                BUTTON_SetSkinFlexProps(&BUTTON_Props_Def_EN, BUTTON_SKINFLEX_PI_FOCUSSED);
//                BUTTON_SetSkinFlexProps(&BUTTON_Props_Def_PR, BUTTON_SKINFLEX_PI_PRESSED);
				Display_Buff = NULL;
				GUI_EndDialog(hWin, 0);
            }

            break;

        case GUI_ID_BUTTON1:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                EDIT_AddKey(WM_GetDialogItem(hWin,  GUI_ID_EDIT0), GUI_KEY_BACKSPACE);
            }

            break;

        case GUI_ID_BUTTON2://确定
            if (NCode == WM_NOTIFICATION_RELEASED) {
                EDIT_GetText(WM_GetDialogItem(input_hWin,  GUI_ID_EDIT0), (char *)input_buf, (input_len + 1));
                sscanf((char *)input_buf, "%f", &input_num);
//                BUTTON_SetSkinFlexProps(&BUTTON_Props_Def_EN, BUTTON_SKINFLEX_PI_ENABLED);
//                BUTTON_SetSkinFlexProps(&BUTTON_Props_Def_EN, BUTTON_SKINFLEX_PI_FOCUSSED);
//                BUTTON_SetSkinFlexProps(&BUTTON_Props_Def_PR, BUTTON_SKINFLEX_PI_PRESSED);

                if (Set_Callback != NULL && input_buf[0] != 0) {//键盘有输入
                    Set_Callback();
                }
				
				Display_Buff = NULL;
                GUI_EndDialog(hWin, 0);
            }

            break;

        case GUI_ID_BUTTON10:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                EDIT_AddKey(WM_GetDialogItem(hWin,  GUI_ID_EDIT0), '0');
            }

            break;

        case GUI_ID_BUTTON11:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                EDIT_AddKey(WM_GetDialogItem(hWin,  GUI_ID_EDIT0), '1');
            }

            break;

        case GUI_ID_BUTTON12:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                EDIT_AddKey(WM_GetDialogItem(hWin,  GUI_ID_EDIT0), '2');
            }

            break;

        case GUI_ID_BUTTON13:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                EDIT_AddKey(WM_GetDialogItem(hWin,  GUI_ID_EDIT0), '3');
            }

            break;

        case GUI_ID_BUTTON14:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                EDIT_AddKey(WM_GetDialogItem(hWin,  GUI_ID_EDIT0), '4');
            }

            break;

        case GUI_ID_BUTTON15:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                EDIT_AddKey(WM_GetDialogItem(hWin,  GUI_ID_EDIT0), '5');
            }

            break;

        case GUI_ID_BUTTON16:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                EDIT_AddKey(WM_GetDialogItem(hWin,  GUI_ID_EDIT0), '6');
            }

            break;

        case GUI_ID_BUTTON17:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                EDIT_AddKey(WM_GetDialogItem(hWin,  GUI_ID_EDIT0), '7');
            }

            break;

        case GUI_ID_BUTTON18:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                EDIT_AddKey(WM_GetDialogItem(hWin,  GUI_ID_EDIT0), '8');
            }

            break;

        case GUI_ID_BUTTON19:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                EDIT_AddKey(WM_GetDialogItem(hWin,  GUI_ID_EDIT0), '9');
            }

            break;

        case GUI_ID_BUTTON20:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                EDIT_AddKey(WM_GetDialogItem(hWin,  GUI_ID_EDIT0), '.');
            }

            break;

        case GUI_ID_BUTTON3:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                EDIT_AddKey(WM_GetDialogItem(hWin,  GUI_ID_EDIT0), '-');
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
WM_HWIN input_hWin;
void Input_Task(WM_HWIN hParent)
{
    input_num = 0;
    BUTTON_SetDefaultTextColor(GUI_WHITE, BUTTON_CI_UNPRESSED);
    input_hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    WM_MakeModal(input_hWin);//模态化这个窗口，必须自己关闭才能点击其他
}



