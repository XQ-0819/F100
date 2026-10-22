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
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "ucos_ii.h"


#include "HEADER_Private.h"

//EndofEventsFunctionList

#define GUI_ID_BUTTON10 (GUI_ID_USER + 1)
#define GUI_ID_BUTTON11 (GUI_ID_USER + 2)
#define GUI_ID_BUTTON12 (GUI_ID_USER + 3)
#define GUI_ID_BUTTON13 (GUI_ID_USER + 4)
static U8 selectX_id = 0;
static U8 selectY_id = 0;

static U8 real_lv_id = 0;
static U8 real_list_id = 0;
static U8 curY_id = 0;

/*********************************************************************
*
*       static data
*
**********************************************************************
*/

extern GUI_CONST_STORAGE GUI_BITMAP bmMEnable;
extern GUI_CONST_STORAGE GUI_BITMAP bmMDisable;
extern GUI_CONST_STORAGE GUI_BITMAP bmMFan1;
extern GUI_CONST_STORAGE GUI_BITMAP bmMFan2;
extern GUI_CONST_STORAGE GUI_BITMAP bmMFan3;

unsigned char list_type[SWINDOW_NUM_MAX + WINDOW_NUM_MAX + PWM_NUM_MAX + JDQ_NUM_MAX] = {0};
unsigned char list_id[SWINDOW_NUM_MAX + WINDOW_NUM_MAX + PWM_NUM_MAX + JDQ_NUM_MAX] = {0};
unsigned char list_max_num = 0;
static unsigned char list_head_num = 0;



#define JDQ_LIST_SIZE	(44)
#define LIST_H_NUM	(18)
#define LIST_V_NUM	(7)


/*********************************************************************
*
*       Dialog resource
*
* This table conatins the info required to create the dialog.
* It has been created by ucGUIbuilder.
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect,    "",               0,                       0,  0,  800, 480, 0, 0},
    { TEXT_CreateIndirect,      "控制管理-通风级别", GUI_ID_TEXT0,            300, 4, 200, 42, 0, 0},
    { LISTVIEW_CreateIndirect,   NULL,               GUI_ID_LISTVIEW0,        4, 47, 790, 370, LISTVIEW_CF_CELL_SELECT, 0},

    { TEXT_CreateIndirect,      "-持续", GUI_ID_TEXT1,            168, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
    { TEXT_CreateIndirect,      "-时控", GUI_ID_TEXT2,            283, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
    { TEXT_CreateIndirect,      "-轮转", GUI_ID_TEXT3,            398, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
    { TEXT_CreateIndirect,      "偏差修正/℃:",   GUI_ID_TEXT4,         10, 7, 140, 36, 0, 0},
    { EDIT_CreateIndirect,       NULL,         GUI_ID_EDIT0,            150, 7, 56, 36, 0, 0},



    { BUTTON_CreateIndirect,      "", GUI_ID_BUTTON3,            140, 443, 24, 24, 0, 0},
    { BUTTON_CreateIndirect,      "", GUI_ID_BUTTON4,            255, 443, 24, 24, 0, 0},
    { BUTTON_CreateIndirect,      "", GUI_ID_BUTTON5,            370, 443, 24, 24, 0, 0},

    { BUTTON_CreateIndirect,      "", GUI_ID_BUTTON10,            10, 428, 68, 48, 0, 0},
    { BUTTON_CreateIndirect,      "", GUI_ID_BUTTON11,            70, 428, 68, 48, 0, 0},
    { BUTTON_CreateIndirect,      "", GUI_ID_BUTTON12,            657, 0, 68, 48, 0, 0},
    { BUTTON_CreateIndirect,      "", GUI_ID_BUTTON13,            720, 0, 68, 48, 0, 0},

//    { BUTTON_CreateIndirect,    "上一页",          GUI_ID_BUTTON0,          MENU_BACK_X1, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
//    { BUTTON_CreateIndirect,    "下一页",          GUI_ID_BUTTON1,          MENU_BACK_X2, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
    { BUTTON_CreateIndirect,    "返回",             GUI_ID_BUTTON2,         MENU_BACK_X3, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},

    { BUTTON_CreateIndirect,    "",     GUI_ID_BUTTON6,           			580, 0, 90, 60, 0, 0},

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
}
static const GUI_BITMAP * pBitmap[] = {&bmMDisable, &bmMFan3, &bmMFan2, &bmMFan1};
static WM_HTIMER hTimer;
void listview_air_show(LISTVIEW_Handle listview0_hWin)
{
    U8 i = 0;

    for (i = 0; i < LIST_V_NUM; i++) {
        sprintf(show_buf, "%.1f", cal_inf.lv_air[selectY_id + i] * 1.0f / AIR_RATE);
        LISTVIEW_SetItemText(listview0_hWin, 2, i, show_buf);
    }
}

//级别是否有时控的设备
int get_lv_time(U8 lvid)
{
    int i = 0;

    for (i = 0; i < JDQ_NUM_MAX; i++) {
        if (dev_inf.JDQ[i].enable != STA_ON) {
            continue;
        }

        if (ctrl_inf.lv[lvid].JDQ[i].mode == MODE_ON_OFF || ctrl_inf.lv[lvid].JDQ[i].mode == MODE_ROTATE) {
            return 1;
        }

    }

    for (i = 0; i < PWM_NUM_MAX; i++) {
        if (dev_inf.PWM[i].enable != STA_ON) {
            continue;
        }

        if (ctrl_inf.lv[lvid].PWM[i].mode == MODE_ON_OFF || ctrl_inf.lv[lvid].PWM[i].mode == MODE_ROTATE) {
            return 1;
        }
    }

    return -1;
}

void listview_lv_sta(LISTVIEW_Handle listview0_hWin, U8 lvid, U8 id, U8 add_flag)
{
    U8 i = 0, column = 0;
    GUI_COLOR Color = 0;

    if (id % 2 == 0) {
        Color = GUI_DEF_LGREEN;
    } else {
        Color = GUI_DEF_GRAY;
    }

    if (add_flag == STA_ON) {
        LISTVIEW_AddRow(listview0_hWin, NULL);
    }

    sprintf(show_buf, "%d", lvid + 1);
    LISTVIEW_SetItemText(listview0_hWin, column, id, show_buf);
    LISTVIEW_SetItemBkColor(listview0_hWin, column++, id, LISTVIEW_CI_UNSEL, Color);

    if (ctrl_inf.lv[lvid].enable == STA_ON) {
        LISTVIEW_SetItemBitmap(listview0_hWin, column, id, 7, 11, &bmMEnable);
    } else {
        LISTVIEW_SetItemBitmap(listview0_hWin, column, id, 7, 11, &bmMDisable);
    }

    LISTVIEW_SetItemBkColor(listview0_hWin, column++, id, LISTVIEW_CI_UNSEL, Color);

    sprintf(show_buf, "%.1f", cal_inf.lv_air[lvid] * 1.0f / AIR_RATE);
    LISTVIEW_SetItemText(listview0_hWin, column, id, show_buf);
    LISTVIEW_SetItemBkColor(listview0_hWin, column++, id, LISTVIEW_CI_UNSEL, Color);

    sprintf(show_buf, "%.1f", ctrl_inf.lv[lvid].offset * 1.0f / TEMP_RATE);
    LISTVIEW_SetItemText(listview0_hWin, column, id, show_buf);
    LISTVIEW_SetItemBkColor(listview0_hWin, column++, id, LISTVIEW_CI_UNSEL, Color);

    if (ctrl_inf.lv_more.lv_re_diff_en != STA_ON && selectX_id == 0) {
        if (lvid != 0) {
            sprintf(show_buf, "%.1f", ctrl_inf.lv[lvid].re_diff * 1.0f / TEMP_RATE);
            LISTVIEW_SetItemText(listview0_hWin, column, id, show_buf);
        } else {
            LISTVIEW_SetItemText(listview0_hWin, column, id, "--");
        }

        LISTVIEW_SetItemBkColor(listview0_hWin, column++, id, LISTVIEW_CI_UNSEL, Color);
    }

    if (selectX_id == 0) {
        if (get_lv_time(lvid) > 0) {
            sprintf(show_buf, "%d", ctrl_inf.lv[lvid].PWM[0].on_time);
        } else {
            sprintf(show_buf, "--");
        }

        LISTVIEW_SetItemText(listview0_hWin, column, id, show_buf);
        LISTVIEW_SetItemBkColor(listview0_hWin, column++, id, LISTVIEW_CI_UNSEL, Color);
    }

    if (ctrl_inf.lv_more.lv_fan_cycle_en != STA_ON && selectX_id == 0) {
        if (get_lv_time(lvid) > 0) {
            sprintf(show_buf, "%d", ctrl_inf.lv[lvid].PWM[0].off_time);
        } else {
            sprintf(show_buf, "--");
        }

        LISTVIEW_SetItemText(listview0_hWin, column, id, show_buf);
        LISTVIEW_SetItemBkColor(listview0_hWin, column++, id, LISTVIEW_CI_UNSEL, Color);
    }

    for (i = selectX_id; i < list_max_num; i++) {
        if (column >= LIST_H_NUM) {
            break;
        }

        switch (list_type[i]) {
        case SWIN_TYPE:
            if ((ctrl_inf.vac.type == STA_ON && dev_inf.SWIN.detail[list_id[i]].ctrl_mode  == WIN_VAC_CTRL)
                    && ctrl_inf.lv[lvid].SWIN[list_id[i]].degree != 0 
                    && lvid >= ctrl_inf.vac.min_ctrllv && lvid <= ctrl_inf.vac.max_ctrllv) {
                LISTVIEW_SetItemTextColor(listview0_hWin, column, id, LISTVIEW_CI_UNSEL, GUI_RED);
                sprintf(show_buf, "%d%%\n%d%%", ext_inf.lv_save[lvid].SWIN[list_id[i]], ctrl_inf.lv[lvid].SWIN[list_id[i]].degree);
            } else {
                LISTVIEW_SetItemTextColor(listview0_hWin, column, id, LISTVIEW_CI_UNSEL, GUI_BLACK);
                sprintf(show_buf, "%d%%", ctrl_inf.lv[lvid].SWIN[list_id[i]].degree);
            }

            LISTVIEW_SetItemText(listview0_hWin, column, id, show_buf);
            LISTVIEW_SetItemBkColor(listview0_hWin, column++, id, LISTVIEW_CI_UNSEL, Color);

            break;

        case WIN_TYPE:
            if (list_id[i] < WIN3_ID) {					
                if ((ctrl_inf.vac.type == STA_ON && dev_inf.WIN.detail[list_id[i]].ctrl_mode  == WIN_VAC_CTRL)
                        && ctrl_inf.lv[lvid].WIN[list_id[i]].degree != 0
                        && lvid >= ctrl_inf.vac.min_ctrllv && lvid <= ctrl_inf.vac.max_ctrllv) {
                    LISTVIEW_SetItemTextColor(listview0_hWin, column, id, LISTVIEW_CI_UNSEL, GUI_RED);
                    sprintf(show_buf, "%d%%\n%d%%", ext_inf.lv_save[lvid].WIN[list_id[i]], ctrl_inf.lv[lvid].WIN[list_id[i]].degree);
                } else {
                    LISTVIEW_SetItemTextColor(listview0_hWin, column, id, LISTVIEW_CI_UNSEL, GUI_BLACK);
                    sprintf(show_buf, "%d%%", ctrl_inf.lv[lvid].WIN[list_id[i]].degree);
                }
            } else {
                //开度存储位置不同
                if ((ctrl_inf.vac.type == STA_ON && dev_inf.WIN.detail[list_id[i]].ctrl_mode  == WIN_VAC_CTRL)
                        && ctrl_inf.exlv[lvid].WIN[list_id[i] - WIN3_ID].degree != 0
                        && lvid >= ctrl_inf.vac.min_ctrllv && lvid <= ctrl_inf.vac.max_ctrllv) {
                    LISTVIEW_SetItemTextColor(listview0_hWin, column, id, LISTVIEW_CI_UNSEL, GUI_RED);
                    sprintf(show_buf, "%d%%\n%d%%", ext_inf.lv_save[lvid].WIN[list_id[i]], ctrl_inf.exlv[lvid].WIN[list_id[i] - WIN3_ID].degree);
                } else {
                    LISTVIEW_SetItemTextColor(listview0_hWin, column, id, LISTVIEW_CI_UNSEL, GUI_BLACK);
                    sprintf(show_buf, "%d%%", ctrl_inf.exlv[lvid].WIN[list_id[i] - WIN3_ID].degree);
                }
            }

            LISTVIEW_SetItemText(listview0_hWin, column, id, show_buf);
            LISTVIEW_SetItemBkColor(listview0_hWin, column++, id, LISTVIEW_CI_UNSEL, Color);

            break;

        case PWM_FAN_TYPE:
            if (ctrl_inf.lv[lvid].PWM[list_id[i]].mode == MODE_OFF) {
                LISTVIEW_SetItemBitmap(listview0_hWin, column, id, 10, 11, pBitmap[ctrl_inf.lv[lvid].PWM[list_id[i]].mode]);
                LISTVIEW_SetItemText(listview0_hWin, column, id, "");
                LISTVIEW_SetItemBkColor(listview0_hWin, column++, id, LISTVIEW_CI_UNSEL, Color);
                continue;
            }

            LISTVIEW_SetItemBitmap(listview0_hWin, column, id, 10, 3, pBitmap[ctrl_inf.lv[lvid].PWM[list_id[i]].mode]);
            sprintf(show_buf, "%d%%", ctrl_inf.lv[lvid].PWM[list_id[i]].degree);
            LISTVIEW_SetItemText(listview0_hWin, column, id, show_buf);
            LISTVIEW_SetItemBkColor(listview0_hWin, column++, id, LISTVIEW_CI_UNSEL, Color);

            break;

        case JDQ_FAN_TYPE:
            LISTVIEW_SetItemBitmap(listview0_hWin, column, id, 10, 11, pBitmap[ctrl_inf.lv[lvid].JDQ[list_id[i]].mode]);
            LISTVIEW_SetItemBkColor(listview0_hWin, column++, id, LISTVIEW_CI_UNSEL, Color);

            break;

        default:
            break;
        }
    }

}

static void Temp_Get(void)
{
    float ftemp = Get_Input_Value() * TEMP_RATE;
    short stemp = (short)ftemp;

    if (real_lv_id == 0) {
        return ;
    }

    if (stemp > TEMP_OFFSET_MAX) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = TEMP_OFFSET_MAX;
    }

//    if (stemp < TEMP_ADJMIN) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = TEMP_ADJMIN;
//    }

    if (stemp < ctrl_inf.lv[real_lv_id - 1].offset) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = ctrl_inf.lv[real_lv_id - 1].offset;
    }

    ctrl_inf.lv[real_lv_id].offset = stemp;
    listview_lv_sta(WM_GetDialogItem(Current_HWIN, GUI_ID_LISTVIEW0), real_lv_id, curY_id , 0);
}

static void ReDiff_Get(void)
{
    float ftemp = Get_Input_Value() * TEMP_RATE;
    short stemp = (short)ftemp;

    if (stemp > TEMP_ADJMAX) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = TEMP_ADJMAX;
    }

    if (stemp < 0) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 0;
    }

    ctrl_inf.lv[real_lv_id].re_diff = stemp;
    listview_lv_sta(WM_GetDialogItem(Current_HWIN, GUI_ID_LISTVIEW0), real_lv_id, curY_id , 0);
}

static void OnTime_Get(void)
{
    unsigned char i = 0;
    float ftemp = Get_Input_Value();
    short stemp = (short)ftemp;

//    if (stemp > TEMP_ADJMAX) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = TEMP_ADJMAX;
//    }

    if (stemp < 1) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 1;
    }

    if (ctrl_inf.lv_more.lv_fan_cycle_en == STA_ON &&  stemp > ctrl_inf.lv_more.lv_fan_cycle) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = ctrl_inf.lv_more.lv_fan_cycle;
    }


    for (i = 0; i < PWM_NUM_MAX; i++) {
        ctrl_inf.lv[real_lv_id].PWM[i].on_time = stemp;
    }

    for (i = 0; i < JDQ_NUM_MAX; i++) {
        ctrl_inf.lv[real_lv_id].JDQ[i].on_time = stemp;
    }

    listview_lv_sta(WM_GetDialogItem(Current_HWIN, GUI_ID_LISTVIEW0), real_lv_id, curY_id , 0);

}
static void OffTime_Get(void)
{
    unsigned char i = 0;
    float ftemp = Get_Input_Value();
    short stemp = (short)ftemp;

//    if (stemp > TEMP_ADJMAX) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = TEMP_ADJMAX;
//    }

    if (stemp < 0) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 0;
    }

    for (i = 0; i < PWM_NUM_MAX; i++) {
        ctrl_inf.lv[real_lv_id].PWM[i].off_time = stemp;
    }

    for (i = 0; i < JDQ_NUM_MAX; i++) {
        ctrl_inf.lv[real_lv_id].JDQ[i].off_time = stemp;
    }

    listview_lv_sta(WM_GetDialogItem(Current_HWIN, GUI_ID_LISTVIEW0), real_lv_id, curY_id , 0);
}

static void Win_Degree_Get(void)
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

    if (list_type[real_list_id] == SWIN_TYPE ) {
        ctrl_inf.lv[real_lv_id].SWIN[list_id[real_list_id]].degree = stemp;
    } else {
        if (list_id[real_list_id] < WIN3_ID) {
            ctrl_inf.lv[real_lv_id].WIN[list_id[real_list_id]].degree = stemp;
        } else {
            ctrl_inf.exlv[real_lv_id].WIN[list_id[real_list_id] - WIN3_ID].degree = stemp;
        }
    }

    listview_lv_sta(WM_GetDialogItem(Current_HWIN, GUI_ID_LISTVIEW0), real_lv_id, curY_id , 0);
}

static void _cbButton(WM_MESSAGE * pMsg)
{
    WM_HWIN  hWin;
    int 	 Id;
    hWin  = pMsg->hWin;
    Id = WM_GetId(hWin);

    switch (pMsg->MsgId) {
    case WM_PAINT:
        if (BUTTON_IsPressed(hWin)) {
            GUI_SetBkColor(GUI_WHITE);
//				GUI_Clear();
            GUI_SetAlpha(0xb0);
            GUI_DrawBitmap(pBitmap[(Id - GUI_ID_BUTTON3 + 1)], 0, 0);
            GUI_SetAlpha(0);
        } else {
            GUI_DrawBitmap(pBitmap[(Id - GUI_ID_BUTTON3 + 1)], 0, 0);
        }

        break;

    default:
        BUTTON_Callback(pMsg);
    }
}

static const char *pSelectBitmap[] = {"99Mpic_Up", "99Mpic_Down", "99Mpic_left", "99Mpic_Right"};

void select_cbButton(WM_MESSAGE * pMsg)
{
    WM_HWIN  hWin;
    int 	 Id;
    hWin  = pMsg->hWin;
    Id = WM_GetId(hWin);

    switch (pMsg->MsgId) {
    case WM_PAINT:
        if (BUTTON_IsPressed(hWin)) {
            GUI_SetBkColor(GUI_WHITE);
            GUI_SetAlpha(0xb0);
            GUI_defDrawBitmap(pSelectBitmap[(Id - GUI_ID_BUTTON10)], 10, 4);
            GUI_SetAlpha(0);
        } else {
            GUI_defDrawBitmap(pSelectBitmap[(Id - GUI_ID_BUTTON10)], 10, 4);
        }

        break;

    default:
        BUTTON_Callback(pMsg);
    }
}

void cal_list_type(void)
{
    U8 i = 0, id = 0;


//    for (i = 0 ; i < CTRL_LV_NUM; i++) {
//        //连续的 level
//        if ((ctrl_inf.lv[i].enable == true)) {
//        } else {
//			break;
//        }
//    }

//    for (; i < CTRL_LV_NUM; i++) {
//		ctrl_inf.lv[i].enable = STA_OFF;
//    }

	
    for (i = 0; i < SWINDOW_NUM_MAX; i++) {
        if (dev_inf.SWIN.detail[i].enable != STA_ON) {
            continue;
        }

        list_type[id] = SWIN_TYPE;
        list_id[id++] = i;
    }

    for (i = 0; i < WINDOW_NUM_MAX; i++) {
        if (dev_inf.WIN.detail[i].enable != STA_ON) {
            continue;
        }

        list_type[id] = WIN_TYPE;
        list_id[id++] = i;
    }

    for (i = 0; i < PWM_NUM_MAX; i++) {
        if (dev_inf.PWM[i].enable != STA_ON) {
            continue;
        }

        list_type[id] = PWM_FAN_TYPE;
        list_id[id++] = i;
    }

    for (i = 0; i < JDQ_NUM_MAX; i++) {
        if (dev_inf.JDQ[i].enable != STA_ON) {
            continue;
        }

        list_type[id] = JDQ_FAN_TYPE;
        list_id[id++] = i;
    }

    list_max_num = id;
}

static void InitDialog(WM_MESSAGE * pMsg)
{
    WM_HWIN hWin = pMsg->hWin;
    int MDev_ID = 0;
    WM_HWIN hItem;
    WM_HWIN listview0_hWin;
    U8 i = 0, j = 0, id = 0, show_id = 0;

    for (MDev_ID = GUI_ID_BUTTON2; MDev_ID <= GUI_ID_BUTTON2; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, Cut_cbButton);
    }

    for (MDev_ID = GUI_ID_BUTTON3; MDev_ID <= GUI_ID_BUTTON5; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, _cbButton);
        WM_DisableWindow(hItem);
    }

    for (MDev_ID = GUI_ID_BUTTON6; MDev_ID <= GUI_ID_BUTTON6; MDev_ID++) {//set
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, set_cbButton);
    }

    for (MDev_ID = GUI_ID_BUTTON10; MDev_ID <= GUI_ID_BUTTON13; MDev_ID++) {//select
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, select_cbButton);
    }

    for (MDev_ID = GUI_ID_TEXT0; MDev_ID <= GUI_ID_TEXT4; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24L(hItem);

        if (MDev_ID == GUI_ID_TEXT0) {
            TEXT_SetTextAlign(hItem,  GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetTextColor(hItem, GUI_WHITE);
        }
    }

    for (MDev_ID = GUI_ID_EDIT0; MDev_ID <= GUI_ID_EDIT0; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Edit_SetHZ24(hItem);
    }

    sprintf(show_buf, "%.1f", ctrl_inf.lv_more.lv_temp_adj * 1.0f / TEMP_RATE);
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT0), show_buf);

    hTimer = WM_CreateTimer(WM_GetClientWindow(hWin), 0, 2000, 0);

    sprintf(show_buf, "%s-%s", GUI_LANG_GetTextEx(Lang_Control, Language_Bit), GUI_LANG_GetTextEx(Lang_Vent_Lv, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT0), show_buf); // 控制管理-通风级别

    sprintf(show_buf, "-%s", GUI_LANG_GetTextEx(Lang_Cont, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT1), show_buf); //-持续

    sprintf(show_buf, "-%s", GUI_LANG_GetTextEx(Lang_Cycle, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT2), show_buf); //-时控

    sprintf(show_buf, "-%s", GUI_LANG_GetTextEx(Lang_Rotate, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT3), show_buf); //-轮转

    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT4), GUI_LANG_GetTextEx(Lang_TAMD, Language_Bit)); // 偏差修正/℃:

    listview0_hWin = WM_GetDialogItem(hWin, GUI_ID_LISTVIEW0);
    HEADER_SetFont( LISTVIEW_GetHeader(listview0_hWin), &GUI_USER_HZ16);
    WM_DisableWindow(LISTVIEW_GetHeader(listview0_hWin));
    LISTVIEW_SetFont(listview0_hWin, &GUI_USER_HZ16);
    if (Language_Bit_original >= LANGUAGE_RUSSIAN) {
        HEADER_SetFont(LISTVIEW_GetHeader(listview0_hWin), &GUI_FontBC_16);
    }
    LISTVIEW_EnableCellSelect(listview0_hWin, 1);
    LISTVIEW_SetGridVis(listview0_hWin, 1);
    LISTVIEW_SetHeaderHeight(listview0_hWin, 46);
    LISTVIEW_SetRowHeight(listview0_hWin, 46);

    id = 4;

    LISTVIEW_AddColumn(listview0_hWin, 38, GUI_LANG_GetTextEx(Lang_Level, Language_Bit), GUI_TA_VCENTER | GUI_TA_CENTER); // 级别
    LISTVIEW_AddColumn(listview0_hWin, 38, GUI_LANG_GetTextEx(Lang_Sta, Language_Bit), GUI_TA_VCENTER | GUI_TA_CENTER);   // 状态
    LISTVIEW_AddColumn(listview0_hWin, 58, GUI_LANG_GetTextEx(Lang_Air, Language_Bit), GUI_TA_VCENTER | GUI_TA_CENTER);   // 风量
    LISTVIEW_AddColumn(listview0_hWin, 40, GUI_LANG_GetTextEx(Lang_Diff, Language_Bit), GUI_TA_VCENTER | GUI_TA_CENTER);  // 偏差

    if (ctrl_inf.lv_more.lv_re_diff_en != STA_ON && selectX_id == 0) {
        LISTVIEW_AddColumn(listview0_hWin, JDQ_LIST_SIZE, GUI_LANG_GetTextEx(Lang_DropOff, Language_Bit), GUI_TA_VCENTER | GUI_TA_CENTER); // 回差
        id++;
    }

    if (selectX_id == 0) {
        sprintf(show_buf, "%s\n%s", GUI_LANG_GetTextEx(Lang_On, Language_Bit), GUI_LANG_GetTextEx(Lang_Sec, Language_Bit));
        LISTVIEW_AddColumn(listview0_hWin, JDQ_LIST_SIZE, show_buf, GUI_TA_VCENTER | GUI_TA_CENTER); // 开\r\n时间
        id++;
    }

    if (ctrl_inf.lv_more.lv_fan_cycle_en != STA_ON && selectX_id == 0) {
        sprintf(show_buf, "%s\n%s", GUI_LANG_GetTextEx(Lang_Off, Language_Bit), GUI_LANG_GetTextEx(Lang_Sec, Language_Bit));
        LISTVIEW_AddColumn(listview0_hWin, JDQ_LIST_SIZE, show_buf, GUI_TA_VCENTER | GUI_TA_CENTER); // 关\r\n时间
        id++;
    }

    list_head_num = id;

    if (selectX_id == 0) {
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON12));

        if (list_max_num <= (LIST_H_NUM - list_head_num)) {
            WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON13));
        }
    } else {
        if (list_max_num <= (LIST_H_NUM - list_head_num)) {
            selectX_id = 1;
        } else {
            selectX_id = (list_max_num - (LIST_H_NUM - list_head_num));
        }

        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON13));
    }

    for (i = selectX_id; i < list_max_num; i++, id++) {
        if (id >= LIST_H_NUM) {
            break;
        }

        switch (list_type[i]) {
        case SWIN_TYPE:
			if(ctrl_inf.lv_more.fan_auto_sort == STA_ON){
				show_id = cal_inf.swin_showid[list_id[i]];
			}else{
				show_id = list_id[i];
			}

            sprintf(show_buf, "%s\n%d", GUI_LANG_GetTextEx(Lang_Inlet, Language_Bit), show_id + 1);
            LISTVIEW_AddColumn(listview0_hWin, JDQ_LIST_SIZE, show_buf, GUI_TA_VCENTER | GUI_TA_CENTER);

            break;

        case WIN_TYPE:
			if(ctrl_inf.lv_more.fan_auto_sort == STA_ON){
				show_id = cal_inf.win_showid[list_id[i]];
			}else{
				show_id = list_id[i];
			}

            sprintf(show_buf, "%s\n%d", GUI_LANG_GetTextEx(Lang_CurtWinDow, Language_Bit), show_id + 1);
            LISTVIEW_AddColumn(listview0_hWin, JDQ_LIST_SIZE, show_buf, GUI_TA_VCENTER | GUI_TA_CENTER);

            break;


        case PWM_FAN_TYPE:
			if(ctrl_inf.lv_more.fan_auto_sort == STA_ON){
				show_id = cal_inf.pwm_fan_showid[list_id[i]];
			}else{
				show_id = list_id[i];
			}

            sprintf(show_buf, "%s\n%d", GUI_LANG_GetTextEx(Lang_Exh, Language_Bit), show_id + 1);
            LISTVIEW_AddColumn(listview0_hWin, JDQ_LIST_SIZE, show_buf, GUI_TA_VCENTER | GUI_TA_CENTER);

            LISTVIEW_SetTextAlign(listview0_hWin, id, GUI_TA_BOTTOM | GUI_TA_CENTER);

            break;

        case JDQ_FAN_TYPE:
			if(ctrl_inf.lv_more.fan_auto_sort == STA_ON){
				show_id = cal_inf.jdq_fan_showid[list_id[i]];
			}else{
				show_id = list_id[i];
			}

            sprintf(show_buf, "%s\n%d", GUI_LANG_GetTextEx(Lang_StaFAN, Language_Bit), show_id + 1);
            LISTVIEW_AddColumn(listview0_hWin, JDQ_LIST_SIZE, show_buf, GUI_TA_VCENTER | GUI_TA_CENTER);

            break;

        default:
            break;
        }
    }


    if (selectY_id == 0) {
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON10));
    } else if (selectY_id + LIST_V_NUM > CTRL_LV_NUM) {
        selectY_id = CTRL_LV_NUM - LIST_V_NUM;
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON11));
    }

    for (i = selectY_id, j = 0; i < selectY_id + LIST_V_NUM; i++, j++) {
        listview_lv_sta(listview0_hWin, i , j , 1);
    }

    //LISTVIEW_SetOwnerDraw(listview0_hWin, LISTVIEW_cbButton);

}

static void lv_temp_adj_Get(void)
{
    float ftemp = Get_Input_Value() * TEMP_RATE;
    short stemp = (short)ftemp;

//    if (stemp > TIME_2BIT) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = TIME_2BIT;
//    }

    if (abs(stemp) > TIME_2BIT) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 0;
    }

    ctrl_inf.lv_more.lv_temp_adj = stemp;
    sprintf(show_buf, "%.1f", ctrl_inf.lv_more.lv_temp_adj * 1.0f / TEMP_RATE);
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, GUI_ID_EDIT0), show_buf);

    Config_Save(SLVMORE_ID);
}

/*********************************************************************
*
*       Dialog callback routine
*/
static void _cbCallback(WM_MESSAGE * pMsg)
{
    int NCode, Id, show_id;
    unsigned char Slect_Row;
    unsigned char Slect_Column;
    WM_HWIN hWin = pMsg->hWin;

    switch (pMsg->MsgId) {
    case WM_PAINT:
        PaintDialog(pMsg);
        break;

    case WM_INIT_DIALOG:
        InitDialog(pMsg);
        break;

    case WM_TIMER:
        listview_air_show(WM_GetDialogItem(hWin, GUI_ID_LISTVIEW0));
        WM_RestartTimer(hTimer, 1000);
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

        if (Id >= GUI_ID_EDIT0 && Id <= GUI_ID_EDIT9 && NCode == WM_NOTIFICATION_RELEASED
                && get_login_permiss(LOGIN_ADMIN) < 0) {
            MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_NO_LOGIN));
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

        case GUI_ID_EDIT0:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(lv_temp_adj_Get);
                Set_Input_Len(4);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_BUTTON2:
            if (NCode == WM_NOTIFICATION_RELEASED) {

                //整体存储
                if (Config_Save(SLVPWM_ID) < 0) {
                    MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_SAVE_ER));
                }

                Current_HWIN = WM_GetParent(hWin);
                GUI_EndDialog(hWin, 0);
            }

            break;

        case GUI_ID_BUTTON6:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                GUI_EndDialog(hWin, 0);
                MCtrl_LVView_Set_Task(WM_GetParent(hWin));
            }

            break;

        case GUI_ID_BUTTON10:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                GUI_EndDialog(hWin, 0);

                if (selectY_id < LIST_V_NUM) {
                    MCtrl_LVView_Task(WM_GetParent(hWin), selectX_id, 0);
                } else {
                    MCtrl_LVView_Task(WM_GetParent(hWin), selectX_id, selectY_id - LIST_V_NUM);
                }
            }

            break;

        case GUI_ID_BUTTON11:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                GUI_EndDialog(hWin, 0);
                MCtrl_LVView_Task(WM_GetParent(hWin), selectX_id, selectY_id + LIST_V_NUM);//初始化的时候判断是否超出
            }

            break;

        case GUI_ID_BUTTON12:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                GUI_EndDialog(hWin, 0);
                MCtrl_LVView_Task(WM_GetParent(hWin), 0, selectY_id);
            }

            break;

        case GUI_ID_BUTTON13:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                GUI_EndDialog(hWin, 0);
                MCtrl_LVView_Task(WM_GetParent(hWin), selectX_id + (LIST_H_NUM - list_head_num), selectY_id);//初始化的时候判断是否超出
            }

            break;

        case GUI_ID_LISTVIEW0:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                if (sys_sta.login_type == LOGIN_GUEST) {
                    MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_NO_LOGIN));
                    break;
                }

                Slect_Row = LISTVIEW_GetSel(WM_GetDialogItem(hWin, GUI_ID_LISTVIEW0));
                Slect_Column = LISTVIEW_GetSelCol(WM_GetDialogItem(hWin, GUI_ID_LISTVIEW0));
                curY_id = Slect_Row;
                real_lv_id = selectY_id + curY_id;
                real_list_id = selectX_id + Slect_Column - list_head_num;

                switch (Slect_Column) {
                case 0:
                case 1:
                case 2:
                    if (real_lv_id == 0) { //第一个级别不能修改
                        MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_NOT_CHG));
                        break;
                    }

                    if ((real_lv_id < CTRL_LV_NUM - 1) && ctrl_inf.lv[real_lv_id].enable == STA_ON
                            && ctrl_inf.lv[real_lv_id + 1].enable == STA_ON ) {
                        MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_NOT_CHG));
                        break;
                    }

                    if ((real_lv_id > 0) && ctrl_inf.lv[real_lv_id].enable == STA_OFF
                            && ctrl_inf.lv[real_lv_id - 1].enable == STA_OFF) {
                        MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_NOT_CHG));
                        break;
                    }	
					
                    ctrl_inf.lv[real_lv_id].enable = !ctrl_inf.lv[real_lv_id].enable;
                    listview_lv_sta(WM_GetDialogItem(hWin, GUI_ID_LISTVIEW0), real_lv_id, curY_id, 0);
                    break;

                case 3:
                    if (real_lv_id == 0) { //第一个级别不能修改
                        MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_NOT_CHG));
                        break;
                    }
                    sprintf(show_buf, "%s%d-%s:%.1f%s", GUI_LANG_GetTextEx(Lang_Level, Language_Bit), (real_lv_id + 1),
                            GUI_LANG_GetTextEx(Lang_TDeviation, Language_Bit), ctrl_inf.lv[real_lv_id].offset / 10.0,
                            GUI_LANG_GetTextEx(Lang_oC, Language_Bit));

                    Set_Output_Callback(Temp_Get);
                    Set_Input_Len(4);
                    Set_Display_Buff(show_buf);
                    Input_Task(hWin);
                    break;

                case 4:
                case 5:
                case 6:
                case 7:
                case 8:
                case 9:
                case 10:
                case 11:
                case 12:
                case 13:
                case 14:
                case 15:
                case 16:
                    if (ctrl_inf.lv_more.lv_re_diff_en != STA_ON && selectX_id == 0 && Slect_Column == 4) {
                        if (real_lv_id == 0) { //第一个级别不能修改
                            MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_NOT_CHG));
                            break;
                        }
                        sprintf(show_buf, "%s%d-%s:%.1f%s", GUI_LANG_GetTextEx(Lang_Level, Language_Bit), (real_lv_id + 1),
                                GUI_LANG_GetTextEx(Lang_DropOff, Language_Bit), ctrl_inf.lv[real_lv_id].re_diff / 10.0,
                                GUI_LANG_GetTextEx(Lang_oC, Language_Bit));
                        Set_Output_Callback(ReDiff_Get);
                        Set_Input_Len(4);
                        Set_Display_Buff(show_buf);
                        Input_Task(hWin);
                        break;
                    } else if (selectX_id == 0 && Slect_Column == 4) {
                        sprintf(show_buf, "%s%d-%s%dS", GUI_LANG_GetTextEx(Lang_Level, Language_Bit), (real_lv_id + 1),
                                GUI_LANG_GetTextEx(Lang_Open_T, Language_Bit), ctrl_inf.lv[real_lv_id].PWM[0].on_time);
                        Set_Output_Callback(OnTime_Get);
                        Set_Input_Len(4);
                        Set_Display_Buff(show_buf);
                        Input_Task(hWin);
                        break;
                    }
                    if (ctrl_inf.lv_more.lv_re_diff_en != STA_ON && selectX_id == 0 && Slect_Column == 5) { // 开时间
                        sprintf(show_buf, "%s%d-%s%dS", GUI_LANG_GetTextEx(Lang_Level, Language_Bit), (real_lv_id + 1),
                                GUI_LANG_GetTextEx(Lang_Open_T, Language_Bit), ctrl_inf.lv[real_lv_id].PWM[0].on_time);
                        Set_Output_Callback(OnTime_Get);
                        Set_Input_Len(4);
                        Set_Display_Buff(show_buf);
                        Input_Task(hWin);
                        break;
                    }

                    if (ctrl_inf.lv_more.lv_re_diff_en == STA_ON && ctrl_inf.lv_more.lv_fan_cycle_en != STA_ON && selectX_id == 0 && Slect_Column == 5) { // 关时间
                        sprintf(show_buf, "%s%d-%s%dS", GUI_LANG_GetTextEx(Lang_Level, Language_Bit), (real_lv_id + 1),
                                GUI_LANG_GetTextEx(Lang_Close_T, Language_Bit), ctrl_inf.lv[real_lv_id].PWM[0].off_time);
                        Set_Output_Callback(OffTime_Get);
                        Set_Input_Len(4);
                        Set_Display_Buff(show_buf);
                        Input_Task(hWin);
                        break;
                    }

                    if (ctrl_inf.lv_more.lv_re_diff_en != STA_ON && ctrl_inf.lv_more.lv_fan_cycle_en != STA_ON && selectX_id == 0 && Slect_Column == 6) { // 关时间

                        sprintf(show_buf, "%s%d-%s%dS", GUI_LANG_GetTextEx(Lang_Level, Language_Bit), (real_lv_id + 1),
                                GUI_LANG_GetTextEx(Lang_Close_T, Language_Bit), ctrl_inf.lv[real_lv_id].PWM[0].off_time);

                        Set_Output_Callback(OffTime_Get);
                        Set_Input_Len(4);
                        Set_Display_Buff(show_buf);
                        Input_Task(hWin);
                        break;
                    }

                    if (list_type[real_list_id] ==  SWIN_TYPE ) {
						if(ctrl_inf.lv_more.fan_auto_sort == STA_ON){
							show_id = cal_inf.swin_showid[list_id[real_list_id]];
						}else{
							show_id = list_id[real_list_id];
						}

                        sprintf(show_buf, "%s%d-%s%d:%d%%", GUI_LANG_GetTextEx(Lang_Level, Language_Bit), (real_lv_id + 1),
                                GUI_LANG_GetTextEx(Lang_Inlet, Language_Bit), (show_id + 1),
                                ctrl_inf.lv[real_lv_id].SWIN[list_id[real_list_id]].degree);

                        Set_Output_Callback(Win_Degree_Get);
                        Set_Input_Len(3);
                        Set_Display_Buff(show_buf);
                        Input_Task(hWin);

                    } else if (list_type[real_list_id] ==  WIN_TYPE) {
						if(ctrl_inf.lv_more.fan_auto_sort == STA_ON){
							show_id = cal_inf.win_showid[list_id[real_list_id]];
						}else{
							show_id = list_id[real_list_id];
						}

                        if (list_id[real_list_id] < WIN3_ID) {
                            sprintf(show_buf, "%s%d-%s%d:%d%%", GUI_LANG_GetTextEx(Lang_Level, Language_Bit), (real_lv_id + 1),
                                    GUI_LANG_GetTextEx(Lang_Curt, Language_Bit), (show_id + 1),
                                    ctrl_inf.lv[real_lv_id].WIN[list_id[real_list_id]].degree);
                        } else {
                            sprintf(show_buf, "%s%d-%s%d:%d%%", GUI_LANG_GetTextEx(Lang_Level, Language_Bit), (real_lv_id + 1),
                                    GUI_LANG_GetTextEx(Lang_Curt, Language_Bit), (show_id + 1),
                                    ctrl_inf.exlv[real_lv_id].WIN[list_id[real_list_id] - WIN3_ID].degree);
                        }

                        Set_Output_Callback(Win_Degree_Get);
                        Set_Input_Len(3);
                        Set_Display_Buff(show_buf);
                        Input_Task(hWin);

                    } else {
                        if (real_list_id <= list_max_num) {
                            MCtrl_LVFAN_SET_Task(hWin, real_lv_id, curY_id, real_list_id);
                        }
                    }

                    break;

                default:
                    if (real_list_id <= list_max_num) {
                        MCtrl_LVFAN_SET_Task(hWin, real_lv_id, curY_id, real_list_id);
                    }

                    break;
                }


            }

            break;

        }

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

WM_HWIN MCtrl_LVView;
void MCtrl_LVView_Task(WM_HWIN hParent, U8 xid, U8 yid)
{
    selectX_id = xid;
    selectY_id = yid;
    MCtrl_LVView = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    Current_HWIN = MCtrl_LVView;
}
