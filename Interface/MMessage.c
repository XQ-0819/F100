#include "menuinc.h"
#if USE_VS == 0
#include "tslib_calibration.h"
#include "stm32f4xx.h"
#include "ucos_ii.h"
#include "def.h"
#include "encrypt.h"
#include "bsp.h"
#include "app.h"

#endif
static U8 message_type;

/******************************************************************************
 *                                STRUCT                                      *
 ******************************************************************************/
struct MESSAGE_INFO_TYPE_struct {
    u16 pTitle;
    u16 pContent;
};
typedef struct MESSAGE_INFO_TYPE_struct MESSAGE_INFO_TYPE;

static const MESSAGE_INFO_TYPE message_info[] = {
    {Lang_WARNING, Lang_WARNING0},
    {Lang_WARNING, Lang_WARNING1},
    {Lang_WARNING, Lang_WARNING2},
    {Lang_WARNING, Lang_WARNING3},
    {Lang_WARNING, Lang_WARNING4},
    {Lang_REMIND, 0},
    {Lang_REMIND, Lang_REMIND1},
    {Lang_REMIND, Lang_REMIND2},
    {Lang_REMIND, Lang_REMIND3},
    {Lang_REMIND, Lang_REMIND4},
    {Lang_REMIND, Lang_REMIND6},
    {Lang_REMIND, Lang_REMIND5},
    {Lang_REMIND, Lang_REMIND7},
    {Lang_REMIND, Lang_REMIND8},
    {Lang_WARNING, Lang_WARNING5},
    {Lang_REMIND, 0},
		
};



static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect, NULL,   0,                175, 100, 450, 280, FRAMEWIN_CF_ACTIVE, 0},
    { TEXT_CreateIndirect,      "TEXT",                 GUI_ID_TEXT1,           0, 0, 450, 30, 0, 0, 0},
    { BUTTON_CreateIndirect,    "取消",                GUI_ID_BUTTON0,          265, 210,  100, 40, 0, 0, 0},
    { BUTTON_CreateIndirect,    "确定",                GUI_ID_BUTTON1,          90, 210,  100, 40, 0, 0, 0},
    { TEXT_CreateIndirect,      "TEXT",                 GUI_ID_TEXT0,           0, 50, 450, 120, 0, 0, 0},


};

/*****************************************************************
**      FunctionName:void InitDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**
**      call this function in _cbCallback --> WM_INIT_DIALOG
*****************************************************************/

static void Init(WM_MESSAGE *pMsg)
{
    U8 show_cancel_flag = 0, i = 0;
    WM_HWIN hWin = pMsg->hWin;
    //
    // FRAMEWIN
    //
    WINDOW_SetBkColor(hWin, GUI_GRAY_E7);

    TEXT_SetFont(WM_GetDialogItem(hWin, GUI_ID_TEXT1), &GUI_USER_HZ24);
    if (Language_Bit_original >= LANGUAGE_RUSSIAN) {
        TEXT_SetFont(WM_GetDialogItem(hWin, GUI_ID_TEXT1), &GUI_FontBC_16);
    }
    TEXT_SetTextAlign(WM_GetDialogItem(hWin, GUI_ID_TEXT1), GUI_TA_VCENTER | GUI_TA_CENTER);

    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT1), GUI_LANG_GetTextEx(message_info[message_type].pTitle, Language_Bit));

    TEXT_SetTextColor(WM_GetDialogItem(hWin, GUI_ID_TEXT1), GUI_WHITE);

    TEXT_SetFont(WM_GetDialogItem(hWin, GUI_ID_TEXT0), &GUI_USER_HZ24);
    if (Language_Bit_original >= LANGUAGE_RUSSIAN) {
        TEXT_SetFont(WM_GetDialogItem(hWin, GUI_ID_TEXT0), &GUI_FontBC_16);
    }
    TEXT_SetTextAlign(WM_GetDialogItem(hWin, GUI_ID_TEXT0), GUI_TA_VCENTER | GUI_TA_CENTER);
    TEXT_SetTextColor(WM_GetDialogItem(hWin, GUI_ID_TEXT0), GUI_WHITE);

    if (message_type == MESSAGE_SOFT_VER) {
        sprintf(show_buf, "SN:");

        for (i = 0; i < Get_McuID_Len(); i += 2) {
            sprintf(show_buf, "%s-%02X%02X", show_buf, Get_McuID8(i), Get_McuID8(i + 1));
        }
        if (soft_inf.update_pro == PRO_HL) {
	        sprintf(show_buf, "%s\nHW Ver:r%02d SW Ver:V%.2fr%02dHL\nStart Time: %d\nReset Time: %d", show_buf, get_hardware_ver(), (float)SYS_VER / 100.0, SYS_VER2, index_inf.start_time, index_inf.restart_time);
        }else{
	        sprintf(show_buf, "%s\nHW Ver:r%02d SW Ver:V%.2fr%02d\nStart Time: %d\nReset Time: %d", show_buf, get_hardware_ver(), (float)SYS_VER / 100.0, SYS_VER2, index_inf.start_time, index_inf.restart_time);
        }
        TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT0), show_buf);
    } 
	
    else if (message_type == MESSAGE_ALARM_SHILED) {
	    sprintf(show_buf, "%s%s:%d Min", GUI_LANG_GetTextEx(Lang_ALA_Shield, Language_Bit), GUI_LANG_GetTextEx(Lang_Time_point, Language_Bit),ctrl_inf.var.shield_ala_time);
        TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT0), show_buf);
	}

	else {

        TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT0), GUI_LANG_GetTextEx(message_info[message_type].pContent, Language_Bit));
    }

    switch (message_type) {
    case MESSAGE_NEW_GROUP://有取消按键
    case MESSAGE_MEM_GROUP_CLC:
    case MESSAGE_SET_DEF:
    case MESSAGE_HARD_TEST:
    case MESSAGE_ADJ_TIPS:
    case MESSAGE_LVCOPY_TIPS:
    case MESSAGE_HOME_STYLE:
    case CONFIRM_NEW_GROUP:
    case MESSAGE_RESTART:
    case MESSAGE_FOOD_ADJ:
    case MESSAGE_SAVE_PARA:
    case MESSAGE_IMPORT_PARA:
    case MESSAGE_ALARM_SHILED:
        show_cancel_flag = 0;
        break;

//    case MESSAGE_MEM_ERR://无取消按键
//    case MESSAGE_MEM_GROUP_NEW:
//    case MESSAGE_USB_OUT_OK:
//    case MESSAGE_USB_IN_OK:
    case MESSAGE_SOFT_VER:
    case MESSAGE_DEATH_TIPS:
    case MESSAGE_ADD_TIPS:
        show_cancel_flag = 1;
        break;

    default:
        FRAMEWIN_SetText(hWin, " ");
        break;
    }

//		EDIT_SetDecMode(WM_GetDialogItem(hWin,GUI_ID_EDIT0), 0, 0, 999,0,0);
    //
    //GUI_ID_BUTTON0
    //
    BUTTON_SetFont(WM_GetDialogItem(hWin, GUI_ID_BUTTON0), &GUI_USER_HZ24);
    if (Language_Bit_original >= LANGUAGE_RUSSIAN) {
        BUTTON_SetFont(WM_GetDialogItem(hWin, GUI_ID_BUTTON0), &GUI_FontBC_16);
    }
    BUTTON_SetBkColor(WM_GetDialogItem(hWin, GUI_ID_BUTTON0), BUTTON_CI_UNPRESSED, GUI_BUTTON_COLOR);
    //
    //GUI_ID_BUTTON1
    //
    BUTTON_SetFont(WM_GetDialogItem(hWin, GUI_ID_BUTTON1), &GUI_USER_HZ24);
    if (Language_Bit_original >= LANGUAGE_RUSSIAN) {
        BUTTON_SetFont(WM_GetDialogItem(hWin, GUI_ID_BUTTON1), &GUI_FontBC_16);
    }
    BUTTON_SetBkColor(WM_GetDialogItem(hWin, GUI_ID_BUTTON1), BUTTON_CI_UNPRESSED, GUI_BUTTON_COLOR);

    BUTTON_SetText(WM_GetDialogItem(hWin, GUI_ID_BUTTON0), GUI_LANG_GetTextEx(Lang_Cancel, Language_Bit));
    BUTTON_SetText(WM_GetDialogItem(hWin, GUI_ID_BUTTON1), GUI_LANG_GetTextEx(Lang_Done, Language_Bit));

    if (show_cancel_flag == 1) {
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON1));
    }
}

static void _cbCallback(WM_MESSAGE * pMsg)
{
    int NCode, Id;
    WM_HWIN hWin = pMsg->hWin;

//	WM_HWIN para;
    switch (pMsg->MsgId) {
    case WM_PAINT:
        Show_MBackPic();
        break;

    case WM_INIT_DIALOG:
        Init(pMsg);
        break;

    case WM_KEY:
        switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) {
        case GUI_KEY_ESCAPE:
            GUI_EndDialog(hWin, 0);
            break;

        case GUI_KEY_ENTER:
            GUI_EndDialog(hWin, 1);
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
        case GUI_ID_BUTTON1:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                GUI_EndDialog(hWin, 1);
            }

            break;

        case GUI_ID_BUTTON0:
            if (NCode == WM_NOTIFICATION_RELEASED) {
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

WM_HWIN MMessage;
U8 MMessage_Task(WM_HWIN hParent, U8 type)
{
    WM_DIALOG_STATUS DialogStatus = {0};
    message_type = type;

    if (hParent == 0) {
        hParent = WM_HBKWIN ;
    }

    MMessage = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    WM_MakeModal(MMessage);
    GUI_SetDialogStatusPtr(MMessage, &DialogStatus);

    while (!DialogStatus.Done) {
        task_time[ucgui_id] = 0;
        GUI_Exec();             //GUI事件更新
        OSTimeDly(10);
        IWDG_ReloadCounter();
    }


    return DialogStatus.ReturnValue;

}



