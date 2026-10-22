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
#include "datasave.h"
#include "Common.h"
#include "eeprom.h"
#include "w25qxx.h"

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
    { TEXT_CreateIndirect,      "历史数据-温度曲线", GUI_ID_TEXT0,           300, 4, 200, 42, 0, 0},
    { GRAPH_CreateIndirect, 	"Graph", 		GUI_ID_GRAPH0, 				4, 50, GRAPHX_SIZE, GRAPHY_SIZE, 0, 0},

//    { BUTTON_CreateIndirect,    "上一页",          GUI_ID_BUTTON0,          MENU_BACK_X1, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
//    { BUTTON_CreateIndirect,    "下一页",          GUI_ID_BUTTON1,          MENU_BACK_X2, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
    { BUTTON_CreateIndirect,    "返回",             GUI_ID_BUTTON2,         MENU_BACK_X3, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},

};

static GRAPH_DATA_Handle ahData[3];
static int data_id = 0;
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
GUI_POINT viewPoint;
int eeprom_view_out(void)
{
    int 	ret = 1, i = 0;
    int		display_num = 0, start_id = 0;
    THDATA	cur_data;

    //判断数据
    if (index_inf.his_buf_id == 0) {
        return ret;
    }

    if (index_inf.his_buf_id > 0) {
        display_num = index_inf.his_buf_id;
        start_id = index_inf.his_buf_id - 1;
    }

    for (i = 0; i < display_num; i++) {
        if (EEPROM_Read(EE_HIS_ADDR + start_id * sizeof(THDATA), (INT8U *)&cur_data.cur_time, sizeof(THDATA)) < 1) {
            continue;
        }

        if (start_id != 0) {
            start_id --;
        }

        //crc
        if (cur_data.crc != crc16_s((INT8U *)&cur_data.cur_time, (sizeof(THDATA) - sizeof(cur_data.crc)))) {
            continue;
        }
		viewPoint.x = data_id;
		viewPoint.y = cur_data.avg_temp + GRAPHY_OFF;
        GRAPH_DATA_XY_AddPoint(ahData[0], &viewPoint);
		viewPoint.y = cur_data.target_temp + GRAPHY_OFF;
        GRAPH_DATA_XY_AddPoint(ahData[1], &viewPoint);
		viewPoint.y = cur_data.fan_temp + GRAPHY_OFF;
        GRAPH_DATA_XY_AddPoint(ahData[2], &viewPoint);
        data_id++;
    }

    return ret;
}

extern THDATA	flash_cur_data[HIS_BUF_NUM];
int flash_view_out(void)
{
    int 	ret = 1, i = 0, id = 0, j = 0;
    u16	    start_id = 0, total_num = 0;

    //判断数据
    if (index_inf.his_state == 0 && index_inf.his_id == 0) {
        return ret;
    }

    //当前已有数据条数
    if (index_inf.his_state == 1) {
        total_num = HIS_NUM;
    } else {
        total_num = index_inf.his_id;
    }

    if (index_inf.his_id > 0) {
        start_id = index_inf.his_id - 1;
    } else {
        start_id = HIS_NUM - 1;
    }

    for (i = 0; i < total_num; id++) {
        while (1) {
            if (i >= total_num ) {
                break;
            }

            SPI_Flash_Read((INT8U *)&flash_cur_data[0].cur_time, (his_data_addr + start_id * 4096), sizeof(flash_cur_data));

            if (start_id == 0) {
                start_id = HIS_NUM - 1;
            } else {
                start_id --;
            }

            for (j = HIS_BUF_NUM; j > 0; j--) {
                //crc
                if (flash_cur_data[j - 1].crc != crc16_s((INT8U *)&flash_cur_data[j - 1].cur_time, (sizeof(THDATA) - sizeof(flash_cur_data[j - 1].crc)))) {
                    continue;
                }

				viewPoint.x = data_id;
				viewPoint.y = flash_cur_data[j - 1].avg_temp + GRAPHY_OFF;
		        GRAPH_DATA_XY_AddPoint(ahData[0], &viewPoint);
				viewPoint.y = flash_cur_data[j - 1].target_temp + GRAPHY_OFF;
		        GRAPH_DATA_XY_AddPoint(ahData[1], &viewPoint);
				viewPoint.y = flash_cur_data[j - 1].fan_temp + GRAPHY_OFF;
		        GRAPH_DATA_XY_AddPoint(ahData[2], &viewPoint);
                data_id++;

                if (data_id >= GRAPHX_SIZE)
                    return 1;
            }

            i++;

            if (i >= total_num) {
                break;
            }
        }
    }

    return ret;
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
//        _DrawPolyLine(&points[0][0], grow_num, GRAPHX_BORDER, (GRAPHY_SIZE - GRAPHY_BORDER));
        GUI_DrawLine(680 - 50, 20, 710 - 50, 20);

        GUI_SetPenSize(5);
        GUI_SetColor(GUI_DEF_RED);
//        _DrawPolyLine(&points[1][0], grow_num, GRAPHX_BORDER, (GRAPHY_SIZE - GRAPHY_BORDER));
        GUI_DrawLine(680 - 50, 40, 710 - 50, 40);

        GUI_SetPenSize(3);
        GUI_SetColor(GUI_DEF_BLUE);
//        _DrawPolyLine(&points[2][0], grow_num, GRAPHX_BORDER, (GRAPHY_SIZE - GRAPHY_BORDER));
        GUI_DrawLine(680 - 50, 60, 710 - 50, 60);

//        GUI_SetPenSize(3);
//        GUI_SetColor(GUI_ORANGE);
//        _DrawPolyLine(&points[3][0], grow_num, GRAPHX_BORDER, (GRAPHY_SIZE - GRAPHY_BORDER));
//        GUI_DrawLine(680, 80, 710, 80);

//        GUI_SetColor(GUI_RED);
//        _DrawPolyLine(&points[4][0], grow_num, GRAPHX_BORDER, (GRAPHY_SIZE - GRAPHY_BORDER));
//        GUI_DrawLine(680, 100, 710, 100);

        GUI_SetColor(GUI_BLACK);
        GUI_SetFont(&GUI_USER_HZ16);
		if(Language_Bit_original >= LANGUAGE_RUSSIAN){
	        GUI_SetFont(&GUI_FontBC_16);
		}

        sprintf(show_buf, "%s/%d%s", GUI_LANG_GetTextEx(Lang_oC, Language_Bit), 12, GUI_LANG_GetTextEx(Lang_Hour, Language_Bit));
        GUI_DispStringAt(show_buf, 25, 310);
        GUI_DispStringAt(GUI_LANG_GetTextEx(Lang_Average_T, Language_Bit), 715 - 50, 12);
        GUI_DispStringAt(GUI_LANG_GetTextEx(Lang_TarT, Language_Bit), 715 - 50, 32);
        GUI_DispStringAt(GUI_LANG_GetTextEx(Lang_TunnelT, Language_Bit), 715 - 50, 52);

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

    GRAPH_SCALE_Handle hScaleV;

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
            sprintf(show_buf, "%s-%s", GUI_LANG_GetTextEx(Lang_History, Language_Bit), GUI_LANG_GetTextEx(Lang_T_Curve, Language_Bit));
            TEXT_SetText(hItem, show_buf);
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
    GRAPH_SCALE_SetOff(hScaleV,  ((GRAPHY_GRID * GRAPHY_OFF / 10.0) / (35 / GRAPHY_NUM))); //刻度偏移
//	GRAPH_SCALE_SetPos(hScaleV, 16);//刻度文字偏移
//	GRAPH_SCALE_SetTickDist //编号距离
//    grow_day = Get_Max_Day();
//    grow_num = Temp_TO_Points();


    //添加额外的文字
    GRAPH_SetUserDraw(hItem, _UserDraw);

//    hScaleH = GRAPH_SCALE_Create((GRAPHY_SIZE - GRAPHY_BORDER) + 2, GUI_TA_RIGHT, GRAPH_SCALE_CF_HORIZONTAL, GRAPHX_GRID);
//    GRAPH_AttachScale(hItem, hScaleH);

//    GRAPH_SCALE_SetFactor(hScaleH, (float)((grow_day / GRAPHX_NUM + 1) * 1.0 / GRAPHX_GRID));
//    GRAPH_SCALE_SetFont(hScaleH, &GUI_Font16B_ASCII);
//    GRAPH_SCALE_SetTextColor(hScaleH, GUI_BLACK);
//	GRAPH_SCALE_SetNumDecs(hScaleV,1);//小数位
// 	GRAPH_SCALE_SetOff(hScaleV, -150);//刻度偏移
//	GRAPH_SCALE_SetPos(hScaleH, 16);//刻度文字偏移
//	GRAPH_SCALE_SetTickDist(hScaleH,25); //编号距离

    //添加额外的文字
//	GRAPH_SetUserDraw(hItem, _UserDraw);


    //添加数据(曲线)
    ahData[0] = GRAPH_DATA_XY_Create(GUI_DEF_DUCKGREEN, GRAPHX_SIZE, 0, 0);
    GRAPH_AttachData(hItem, ahData[0]);
    GRAPH_DATA_XY_SetPenSize(ahData[0], 3);
		
    ahData[1] = GRAPH_DATA_XY_Create(GUI_DEF_RED, GRAPHX_SIZE, 0, 0);
    GRAPH_AttachData(hItem, ahData[1]);
    GRAPH_DATA_XY_SetPenSize(ahData[1], 5);

    ahData[2] = GRAPH_DATA_XY_Create(GUI_DEF_BLUE, GRAPHX_SIZE, 0, 0);
    GRAPH_AttachData(hItem, ahData[2]);
    GRAPH_DATA_XY_SetPenSize(ahData[2], 3);
	
	data_id = 0;
    eeprom_view_out();
    flash_view_out();
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
WM_HWIN HisTemp_View;
void HisTemp_View_Task(WM_HWIN hParent)
{
    HisTemp_View = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    Current_HWIN = HisTemp_View;
}

