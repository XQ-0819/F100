#include "menuinc.h"
#include "string.h"


#define ID_WINDOW_0   				(GUI_ID_USER + 0x00)
#define ID_BUTTON_0 				(GUI_ID_USER + 0x01)
#define hWin_Alpha                  (0x60)               /* 窗口透明值 */
#define hWin_CloseTime              (0x02)               /* 窗口关闭时间*/

WM_HWIN hWinWarning;
static char TextBuf[128];
static U8 hWin_TimeCount;

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect, "Warning", ID_WINDOW_0, 0, 0, 240, 60, 0, 0x0, 0 },
    { BUTTON_CreateIndirect, "DlgBack", ID_BUTTON_0, 0, 0, 240, 60, 0, 0x0, 0 },
};


static void _cbButtonReturn(WM_MESSAGE * pMsg)
{
    GUI_RECT Rect;

    switch (pMsg->MsgId) {
    case WM_PAINT:
        WM_GetClientRect(&Rect);
        GUI_SetTextMode(GUI_TM_TRANS);
        GUI_SetColor(GUI_WHITE);
        GUI_SetFont(&GUI_USER_HZ24);
		if(Language_Bit_original >= LANGUAGE_RUSSIAN){
	        GUI_SetFont(&GUI_FontBC_16);
		}GUI_DispStringInRectWrap(TextBuf, &Rect, GUI_TA_VCENTER | GUI_TA_HCENTER, GUI_WRAPMODE_WORD);
        break;

    default:
        BUTTON_Callback(pMsg);
    }
}

static void _cbDialog(WM_MESSAGE * pMsg)
{
    GUI_RECT  Rect;
    int       NCode;
    int       Id;
    WM_HWIN hWin = pMsg->hWin;

    switch (pMsg->MsgId) {
    case WM_TIMER:
        hWin_TimeCount++;

        if (hWin_TimeCount >= hWin_CloseTime) {
            hWin_TimeCount = 0;
            GUI_EndDialog(hWin, 1);
        }

        WM_RestartTimer(pMsg->Data.v, 1000);	//重置定时器时间1s
        break;

    case WM_INIT_DIALOG:
        WM_SetHasTrans(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0));
        WM_SetCallback(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0), _cbButtonReturn);
        break;

    case WM_PAINT:
        WM_GetClientRect(&Rect);
        GUI_SetAlpha(hWin_Alpha);
        GUI_SetBkColor(GUI_BLACK);
        GUI_Clear();
        GUI_SetAlpha(0);
        break;

    case WM_NOTIFY_PARENT:
        Id    = WM_GetId(pMsg->hWinSrc);
        NCode = pMsg->Data.v;
		
        if (NCode == WM_NOTIFICATION_CLICKED) {
			beep_on();
		}
		
        switch (Id) {
            /* 关闭Warning对话框 */
        case ID_BUTTON_0:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                GUI_EndDialog(hWin, 1);
            }

            break;

        default:
            break;
        }

        /*要删除窗口前发送，告诉窗口释放其数据结构（如果有）*/
    case WM_DELETE:
//      printf("\r\n 关闭提示窗口!!!\r\n");
        break;

    default:
        WM_DefaultProc(pMsg);
        break;
    }
}

/**********************************************************************************************************
*	函 数 名: u8 Get_WenJianGuanLi_TueBiao(void)
*	功能说明: 定时关闭提示框
*	形    参: Fu_hWin父窗口 //x y相对于父窗口的坐标\\ Text 显示的文本 TimeOut 显示的时间（S） Alpha 透明度
*	返 回 值:
*********************************************************************************************************
*/
void MWarning_Task(WM_HWIN hWin, const char *pDisText)
{
    int x = 0, y = 0;
    hWin_TimeCount = 0;

    if (hWin < 0)return;

//    if (WM_IsWindow(hWinWarning)) {
//        strcpy(TextBuf, pDisText);
//        WM_InvalidateWindow(hWinWarning);      // 无效化窗口
//    } else {
    x = (WM_GetWindowSizeX(hWin) - 240) / 2;
    y = (WM_GetWindowSizeY(hWin) - 36) / 2 + 50;
    strcpy(TextBuf, pDisText);

    hWinWarning = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, hWin, 0, 0);
    WM_SetHasTrans(hWinWarning);            // 设置透明度 Set transparency
    WM_SetCallback(hWinWarning, _cbDialog); // 覆盖回调 Overwrite callback
    WM_CreateTimer(hWinWarning, 0, 1000, 0);

    WM_MoveChildTo(hWinWarning, x, y);
    WM_SetStayOnTop(hWinWarning, 1);
    WM_BringToTop(hWinWarning);
//    }
}

/*************************** End of file ****************************/
