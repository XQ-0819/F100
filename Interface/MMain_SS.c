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
#include <stdlib.h>
#include "recode.h"

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
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect,    "",               0,                       0,  0,  800, 480, 0, 0},
    { TEXT_CreateIndirect,      "22.0", GUI_ID_TEXT0,            250, 200, 300, 90, 0, 0},
};



/*****************************************************************
**      FunctionName:void PaintDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**
**      call this function in _cbCallback --> WM_PAINT
*****************************************************************/

static void PaintDialog(WM_MESSAGE * pMsg)
{
    GUI_SetBkColor(GUI_BLACK);
    GUI_Clear();
}


extern GUI_CONST_STORAGE GUI_FONT GUI_FontCY_72;
static WM_HTIMER hTimer;

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

    for (MDev_ID = GUI_ID_TEXT0; MDev_ID <= GUI_ID_TEXT0; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        TEXT_SetTextAlign(hItem,  GUI_TA_LEFT | GUI_TA_TOP);
        TEXT_SetFont(hItem, &GUI_FontCY_72);
        TEXT_SetTextColor(hItem, GUI_GREEN);
        WM_DisableWindow(hItem);
    }



    hTimer = WM_CreateTimer(WM_GetClientWindow(hWin), 0, 3000, 0);
}


#define X_SITE_MAX	(500)
#define Y_SITE_MAX	(400)

/*********************************************************************
*
*       Dialog callback routine
*/
static void _cbCallback(WM_MESSAGE * pMsg)
{
    int NCode, Id, x_site, y_site;
    WM_HWIN hWin = pMsg->hWin;

    switch (pMsg->MsgId) {
    case WM_PAINT:
        PaintDialog(pMsg);
        break;

    case WM_INIT_DIALOG:
        InitDialog(pMsg);
        break;

    case WM_TIMER:
        if (sys_sta.lcd_state == 0) { //ÁÁÆÁ²»²Ù×÷	
        	if(ALA_State()!=0){
		        TEXT_SetTextColor(WM_GetDialogItem(hWin, GUI_ID_TEXT0), GUI_RED);
        	}else{
		        TEXT_SetTextColor(WM_GetDialogItem(hWin, GUI_ID_TEXT0), GUI_GREEN);
        	}
			
	        sprintf(show_buf, "%.1f%s", (float)(cal_inf.temp * 1.0f / (float)TEMP_RATE), GUI_LANG_GetTextEx(Lang_oC, Language_Bit));
	        TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT0), (const char *) show_buf);
	        x_site = rand() % X_SITE_MAX;
	        y_site = rand() % Y_SITE_MAX;
	        WM_MoveTo(WM_GetDialogItem(hWin, GUI_ID_TEXT0), x_site, y_site);
        }
		
        WM_RestartTimer(hTimer, 3000);
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
WM_HWIN MMain_SS;
void MMain_SS_Task(WM_HWIN hParent)
{
    MMain_SS = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    WM_HideWindow(MMain_SS);
}

