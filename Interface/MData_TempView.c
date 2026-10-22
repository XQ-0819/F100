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

#define GRAPHX_SIZE		(790)
#define GRAPHY_SIZE		(355)
#define GRAPHX_GRID		(50)
#define GRAPHY_GRID 	(50)
#define GRAPHX_OFF 		(0)
#define GRAPHY_OFF 		(-100)

#define GRAPHX_BORDER	(20)
#define GRAPHY_BORDER	(20)
#define GRAPHX_NUM		((GRAPHX_SIZE-GRAPHX_BORDER)/GRAPHX_GRID)
#define GRAPHY_NUM		((GRAPHY_SIZE-GRAPHY_BORDER)/GRAPHY_GRID)

// TODO:可以修改为GRAPH_DATA_XY_Create

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
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect,    "",               0,                       0,  0,  800, 480, 0, 0},
    { TEXT_CreateIndirect,      "历史数据-每日平均", GUI_ID_TEXT0,           300, 4, 200, 42, 0, 0},
    { GRAPH_CreateIndirect, 	"Graph", 		GUI_ID_GRAPH0, 				4, 50, GRAPHX_SIZE, GRAPHY_SIZE, 0, 0},

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

//int Get_Max_Day(void)
//{
//    int i = 0, grow_day = -1;

//    for (i = 0; i < TEMP_CURVE_NUM; i++) {
//        if (ctrl_inf.temp[i].enable != STA_ON) {
//            continue;
//        }

//        if (grow_day >= ctrl_inf.temp[i].grow_day) {
//            continue;
//        }

//        grow_day = ctrl_inf.temp[i].grow_day;
//    }

//    return grow_day;
//}

int min_grow_day = -1;
int grow_day = -1;
int grow_num = -1;
GUI_POINT points[4][DATA_RECORD_MAX] = {0};//4种温度曲线坐标

//温度值转换为坐标
//int Temp_TO_Points(void)
//{
//    int i = 0, grow_num = 0, j = 0;
//    signed short *temp = NULL;
//    float day_x = (float)(GRAPHX_GRID * 1.0 / ((grow_day / GRAPHX_NUM) + 1));
//    float temp_y = (float)(GRAPHY_GRID * 1.0 / ((35 / GRAPHY_NUM)));

//    for (i = 0; i < TEMP_CURVE_NUM; i++) {
//        if (ctrl_inf.temp[i].enable != STA_ON) {
//            continue;
//        }

//        temp = &ctrl_inf.temp[i].heat_temp;

//        for (j = 0; j < 5; j++) {
//            points[j][grow_num].x = ctrl_inf.temp[i].grow_day * day_x;
//            points[j][grow_num].y = -((*(temp + j) + GRAPHY_OFF)/10.0 * temp_y);
//        }

//        grow_num ++;
//    }

//    return grow_num;
//}

static void _DrawPolyLine(const GUI_POINT*pPoints, int NumPoints, int x0, int y0)
{
    int x, y;

    if (pPoints == NULL || NumPoints == 0)
        return;

    x = pPoints->x + x0;
    y = pPoints->y + y0;

    while (NumPoints > 0) {
        if (pPoints->x == POINTS_ER) {
            pPoints++;
            NumPoints -- ;
            continue;
        }
		
        GUI_AA_DrawLine(x, y, pPoints->x + x0, pPoints->y + y0);
        x = pPoints->x + x0;
        y = pPoints->y + y0;
        pPoints++;
        NumPoints -- ;
    }

//    GUI_AA_DrawLine(x, y, (GRAPHX_SIZE - 10), y);
}
static void _UserDraw(WM_HWIN hWin, int Stage)
{
    switch (Stage) {
//    case GRAPH_DRAW_FIRST: // // Draw for example a user defined grid... //
//        break;
//	case GRAPH_DRAW_AFTER_BORDER:
//		break;
    case GRAPH_DRAW_LAST: // // Draw for example a user defined scale or additional text... //
        GUI_SetPenSize(3);
        GUI_SetColor(GUI_DEF_DUCKGREEN);
        _DrawPolyLine(&points[0][0], grow_num, GRAPHX_BORDER, (GRAPHY_SIZE - GRAPHY_BORDER));
        GUI_DrawLine(660, 20, 690, 20);

        GUI_SetPenSize(5);
        GUI_SetColor(GUI_DEF_GREEN);
        _DrawPolyLine(&points[1][0], grow_num, GRAPHX_BORDER, (GRAPHY_SIZE - GRAPHY_BORDER));
        GUI_DrawLine(660, 40, 690, 40);

        GUI_SetPenSize(3);
        GUI_SetColor(GUI_DEF_BLUE);
        _DrawPolyLine(&points[2][0], grow_num, GRAPHX_BORDER, (GRAPHY_SIZE - GRAPHY_BORDER));
        GUI_DrawLine(660, 60, 690, 60);

        GUI_SetPenSize(3);
        GUI_SetColor(GUI_ORANGE);
        _DrawPolyLine(&points[3][0], grow_num, GRAPHX_BORDER, (GRAPHY_SIZE - GRAPHY_BORDER));
        GUI_DrawLine(660, 80, 690, 80);

//        GUI_SetColor(GUI_RED);
//        _DrawPolyLine(&points[4][0], grow_num, GRAPHX_BORDER, (GRAPHY_SIZE - GRAPHY_BORDER));
//        GUI_DrawLine(680, 100, 710, 100);

        GUI_SetColor(GUI_BLACK);
        GUI_SetFont(&GUI_USER_HZ16);
		if(Language_Bit_original >= LANGUAGE_RUSSIAN){
	        GUI_SetFont(&GUI_FontBC_16);
		}

        sprintf(show_buf, "%s/%s", GUI_LANG_GetTextEx(Lang_oC, Language_Bit), GUI_LANG_GetTextEx(Lang_Days, Language_Bit));
        GUI_DispStringAt(show_buf, 25, 310);                                            // ℃/天
        GUI_DispStringAt(GUI_LANG_GetTextEx(Lang_Min_Temp, Language_Bit), 700, 12);     // 最小温度
        GUI_DispStringAt(GUI_LANG_GetTextEx(Lang_Average_T, Language_Bit), 700, 32);    // 平均温度
        GUI_DispStringAt(GUI_LANG_GetTextEx(Lang_Max_Temp, Language_Bit), 700, 52);     // 最大温度
        GUI_DispStringAt(GUI_LANG_GetTextEx(Lang_Outdoor_Temp, Language_Bit), 700, 72); // 室外温度

        break;
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
    WM_HWIN hItem;

    GRAPH_SCALE_Handle hScaleV, hScaleH;

    for (MDev_ID = GUI_ID_BUTTON2; MDev_ID <= GUI_ID_BUTTON2; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, _cbButtonBack);
    }
    for (MDev_ID = GUI_ID_TEXT0; MDev_ID <= GUI_ID_TEXT0; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24(hItem);

        if (MDev_ID == GUI_ID_TEXT0) {
            TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetTextColor(hItem, GUI_WHITE);
            sprintf(show_buf, "%s-%s", GUI_LANG_GetTextEx(Lang_History, Language_Bit), GUI_LANG_GetTextEx(Lang_Daily_Average, Language_Bit));
            TEXT_SetText(hItem, show_buf); // 历史数据-每日平均
        }
    }

    hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_GRAPH0);
    //设置颜色
    GRAPH_SetColor(hItem, GUI_BACK_COLOR, GRAPH_CI_BK);
    GRAPH_SetColor(hItem, GUI_BACK_COLOR, GRAPH_CI_BORDER);
    GRAPH_SetColor(hItem, GUI_BLACK, GRAPH_CI_FRAME);
//    GRAPH_SetColor(hItem, GUI_DEF_LGREEN, GRAPH_CI_GRID);


    //设置网格像素
    GRAPH_SetGridDistY(hItem, GRAPHY_GRID);
    GRAPH_SetGridDistX(hItem, GRAPHX_GRID);
    GRAPH_SetGridFixedX(hItem, 1);
    //设置网格的虚拟尺寸
//    GRAPH_SetVSizeX(hItem, 50);
//    GRAPH_SetVSizeY(hItem, 50);
    //设置网格偏移
//    GRAPH_SetGridOffY(hItem, 0);
    //设置网格线
    GRAPH_SetLineStyleH(hItem, GUI_LS_DASHDOT);
    GRAPH_SetLineStyleV(hItem, GUI_LS_DASH);
    GRAPH_SetGridVis(hItem, 1);
    //设置边框
    GRAPH_SetBorder(hItem, GRAPHX_BORDER, 0, 0, GRAPHY_BORDER);


    hScaleV = GRAPH_SCALE_Create(GRAPHX_BORDER - 4, GUI_TA_RIGHT, GRAPH_SCALE_CF_VERTICAL, GRAPHY_GRID);
    GRAPH_AttachScale(hItem, hScaleV);
    GRAPH_SCALE_SetFactor(hScaleV, (float)((35 / GRAPHY_NUM) * 1.0 / GRAPHY_GRID));
    GRAPH_SCALE_SetFont(hScaleV, &GUI_Font16B_ASCII);
    GRAPH_SCALE_SetTextColor(hScaleV, GUI_BLACK);
//	GRAPH_SCALE_SetNumDecs(hScaleV,1);//小数位
    GRAPH_SCALE_SetOff(hScaleV,  ((GRAPHY_GRID * GRAPHY_OFF/10.0) / (35 / GRAPHY_NUM)));//刻度偏移
//	GRAPH_SCALE_SetPos(hScaleV, 16);//刻度文字偏移
//	GRAPH_SCALE_SetTickDist //编号距离
//    grow_day = Get_Max_Day();
//    grow_num = Temp_TO_Points();

    if (grow_day < 0 || grow_num <= 0)return;

    //添加额外的文字
    GRAPH_SetUserDraw(hItem, _UserDraw);

    hScaleH = GRAPH_SCALE_Create((GRAPHY_SIZE - GRAPHY_BORDER) + 2, GUI_TA_RIGHT, GRAPH_SCALE_CF_HORIZONTAL, GRAPHX_GRID);
    GRAPH_AttachScale(hItem, hScaleH);

    GRAPH_SCALE_SetFactor(hScaleH, (float)((grow_day / GRAPHX_NUM + 1) * 1.0 / GRAPHX_GRID));
    GRAPH_SCALE_SetFont(hScaleH, &GUI_Font16B_ASCII);
    GRAPH_SCALE_SetTextColor(hScaleH, GUI_BLACK);
// 	GRAPH_SCALE_SetOff(hScaleH, -10);//刻度偏移
//	GRAPH_SCALE_SetNumDecs(hScaleV,1);//小数位
// 	GRAPH_SCALE_SetOff(hScaleV, -150);//刻度偏移
//	GRAPH_SCALE_SetPos(hScaleH, 16);//刻度文字偏移
//	GRAPH_SCALE_SetTickDist(hScaleH,25); //编号距离

    //添加额外的文字
//	GRAPH_SetUserDraw(hItem, _UserDraw);


    //添加数据(曲线)
//    ahData = GRAPH_DATA_YT_Create(GUI_GREEN, 780, 0, 0);
//    GRAPH_AttachData(hItem, ahData);
//    GRAPH_DATA_YT_SetAlign(ahData, GRAPH_ALIGN_LEFT); //左对齐

//	GRAPH_DATA_XY_SetPenSize(hItem,2);
//    for (i = 0; i < max_day; i++) {
//		for(j = 0;j<15;j++){
//	        GRAPH_DATA_YT_AddValue(ahData, (15 + i) % 38 * 10);
//		}
//    }

//    //添加数据(曲线)
//    ahData = GRAPH_DATA_YT_Create(GUI_GREEN, 780, 0, 0);
//    GRAPH_AttachData(hItem, ahData);
//    GRAPH_DATA_YT_SetAlign(ahData, GRAPH_ALIGN_LEFT); //左对齐

//    for (i = 0; i < max_day; i++) {
//		for(j = 0;j<15;j++){
//	        GRAPH_DATA_YT_AddValue(ahData, (20 + i) % 38 * 10);
//		}
//    }
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
        case GUI_ID_BUTTON2:
            if (NCode == WM_NOTIFICATION_RELEASED) {
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
WM_HWIN DataTemp_View;
void DataTemp_View_Task(WM_HWIN hParent)
{
    DataTemp_View = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    Current_HWIN = DataTemp_View;
}

