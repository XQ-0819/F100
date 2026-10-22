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
#include "stdio.h"
#include <string.h>
#include <time.h>
#include "recode.h"
#include "DataSave.h"
#include "def.h"
#include "bsp.h"


static U8 select_id = 0;
static U8 Select;

typedef struct {
    unsigned char   all_num;
    unsigned char   sta;
    unsigned char   type[3][36];
} EQU_TYPE;

EQU_TYPE JDQ_PAPM_GBK[ALLJDQ_NUM_MAX] = {
    {1, 0, " 定速风机1", "", ""},
    {1, 0, " 定速风机2", "", ""},
    {1, 0, " 定速风机3", "", ""},
    {1, 0, " 定速风机4", "", ""},

    {2, 0, " 定速风机5;", "风窗1开", ""},
    {2, 0, " 定速风机6;", "风窗1关", ""},
    {2, 0, " 定速风机7;", "风窗2开", ""},
    {2, 0, " 定速风机8;", "风窗2关", ""},

    {2, 0, " 定速风机9;", "湿帘1", ""},
    {3, 0, " 定速风机10;", "湿帘2;", "喷淋"},
    {3, 0, " 定速风机11;", "环流风机;", "定时器1"},
    {3, 0, " 定速风机12;", "温控开关;", "定时器2"},

    {3, 0, " 定速风机13;", "加热1;", "定时器3"},
    {3, 0, " 定速风机14;", "加热2;", "定时器4"},
    {2, 0, " 定速风机15;", "加热3", ""},
    {2, 0, " 定速风机16;", "风窗3开", ""},

    {2, 0, " 定速风机17;", "风窗3关", ""},
    {2, 0, " 定速风机18;", "风窗4开", ""},
    {2, 0, " 定速风机19;", "风窗4关", ""},
    {1, 0, " 报警", "", ""},

};

EQU_TYPE PWM_PAPM_GBK[ALLPWM_NUM_MAX] = {
    {3, 0, " 调速风机1;", "A区调速;", "小窗1"},
    {3, 0, " 调速风机2;", "B区调速;", "小窗2"},
    {3, 0, " 调速风机3;", "C区调速;", "小窗3"},
    {3, 0, " 调速风机4;", "加热;", "小窗4"},
    {2, 0, " 温控开关;", "小窗5",  ""},
    {2, 0, " 光照;", "小窗6", ""},
};

//更新各设备现在状态
static void Updata_Equ_Sta(void)
{
    unsigned char i, id, win_id;
    char name_ABC[2] = "A";
    for (i = 0; i < ALLJDQ_NUM_MAX; i++) {
        JDQ_PAPM_GBK[i].sta = STA_OFF;
    }

    for (i = 0; i < ALLPWM_NUM_MAX; i++) {
        PWM_PAPM_GBK[i].sta = STA_OFF;
    }

    if (select_id == 0) {
        for (i = 0; i < PWM_NUM_MAX; i++) {
            /*调速风机*/
            id = 0;
            /*调速风机*/
			sprintf((char *)PWM_PAPM_GBK[i].type[id], "%s%s", GUI_LANG_GetTextEx(Lang_Exh, Language_Bit),GUI_LANG_GetTextEx(Lang_Fan, Language_Bit));
            /*%s区调速*/
            name_ABC[0] = name_ABC[0] + i;
            sprintf((char *)PWM_PAPM_GBK[i].type[id + 1], GUI_LANG_GetTextEx(Lang_Exh_Area, Language_Bit), name_ABC);
            strcat((char *)PWM_PAPM_GBK[i].type[id + 1], ";");
            if (dev_inf.PWM[i].enable != STA_ON) {
                sprintf((char *)PWM_PAPM_GBK[i].type[id], "%s-;", PWM_PAPM_GBK[i].type[id]);			
                continue;
            }

            PWM_PAPM_GBK[i].sta = 1;

            if (ctrl_inf.lv_more.fan_auto_sort == STA_ON) {
                sprintf((char *)PWM_PAPM_GBK[i].type[id], "%s%d;", PWM_PAPM_GBK[i].type[id], cal_inf.pwm_fan_showid[i] + 1);		
            } else {
                sprintf((char *)PWM_PAPM_GBK[i].type[id], "%s%d;", PWM_PAPM_GBK[i].type[id], i + 1);				
            }
        }

        for (i = 0; i < SWINDOW_NUM_MAX; i++) {
            /*小窗*/
            if (i < PWM_NUM_MAX) {
                id = 2;
            } else {
                id = 1;
            }

            strcpy((char *)PWM_PAPM_GBK[i].type[id], GUI_LANG_GetTextEx(Lang_Inlet, Language_Bit));

            if (dev_inf.SWIN.detail[i].enable != STA_ON) {
                sprintf((char *)PWM_PAPM_GBK[i].type[id], "%s-;", PWM_PAPM_GBK[i].type[id]);			
                continue;
            }

            if (ctrl_inf.lv_more.fan_auto_sort == STA_ON) {
                sprintf((char *)PWM_PAPM_GBK[i].type[id], "%s%d;", PWM_PAPM_GBK[i].type[id], cal_inf.swin_showid[i] + 1);				
            } else {
                sprintf((char *)PWM_PAPM_GBK[i].type[id], "%s%d;", PWM_PAPM_GBK[i].type[id], i + 1);			
            }
        }
        /*加热*/
        sprintf((char *)PWM_PAPM_GBK[3].type[1], GUI_LANG_GetTextEx(Lang_Heat, Language_Bit));
        strcat((char *)PWM_PAPM_GBK[3].type[1], ";");
        /*温控开关*/
        sprintf((char *)PWM_PAPM_GBK[4].type[0], GUI_LANG_GetTextEx(Lang_Tswitch, Language_Bit));
        strcat((char *)PWM_PAPM_GBK[4].type[0], ";");
        /*光照*/
        sprintf((char *)PWM_PAPM_GBK[5].type[0], GUI_LANG_GetTextEx(Lang_illumination, Language_Bit));
        strcat((char *)PWM_PAPM_GBK[5].type[0], ";");

        if (basic_inf.tempc_pwm_en.enable == STA_ON) {
            PWM_PAPM_GBK[PWM_TEMPC].sta = 1;
        }

        if (dev_inf.SWIN.detail[SWIN5_ID].enable == STA_ON) {
            PWM_PAPM_GBK[PWM_SWIN5].sta = 2;
        }

        if (dev_inf.light.enable == STA_ON) {
            PWM_PAPM_GBK[PWM_LIGHT].sta = 1;
        }

        if (dev_inf.SWIN.detail[SWIN6_ID].enable == STA_ON) {
            PWM_PAPM_GBK[PWM_SWIN6].sta = 2;
        }

        for (i = 0; i < APWM_NUM_MAX; i++) {
            if (basic_inf.area_pwm[i].enable != STA_ON)
                continue;

            PWM_PAPM_GBK[APWM_FAN1 + i].sta = 2;
        }

        if (basic_inf.pheat_en == STA_ON) {
            PWM_PAPM_GBK[PWM_HEAT].sta = 2;
        }

        for (i = 0; i < PWM_NUM_MAX; i++) {
            if (dev_inf.SWIN.detail[i].enable != STA_ON)
                continue;

            PWM_PAPM_GBK[PWM_SWIN1 + i].sta = 3;
        }

        return;
    }


    for (i = 0; i < JDQ_NUM_MAX; i++) {
        /*定速风机*/
        id = 0;
        sprintf((char *)JDQ_PAPM_GBK[i].type[id], "%s%s", GUI_LANG_GetTextEx(Lang_StaFAN, Language_Bit), GUI_LANG_GetTextEx(Lang_Fan, Language_Bit));
        if (dev_inf.JDQ[i].enable != STA_ON) {
            sprintf((char *)JDQ_PAPM_GBK[i].type[id], "%s-;", JDQ_PAPM_GBK[i].type[id]);			
            continue;
        }

        JDQ_PAPM_GBK[i].sta = 1;

        if (ctrl_inf.lv_more.fan_auto_sort == STA_ON) {
            sprintf((char *)JDQ_PAPM_GBK[i].type[id], "%s%d;", JDQ_PAPM_GBK[i].type[id], cal_inf.jdq_fan_showid[i] + 1);		
        } else {
            sprintf((char *)JDQ_PAPM_GBK[i].type[id], "%s%d;", JDQ_PAPM_GBK[i].type[id], i + 1);		
        }

    }

    for (i = WIN1_ID; i <= WIN4_ID; i++) {
        /*风窗*/
        if (i < WIN3_ID) {
            win_id = JDQ_WIN1_UP + 2 * i;
        } else {
            win_id = JDQ_WIN3_UP + 2 * (i - WIN3_ID);
        }

        id = 1;
        strcpy((char *)JDQ_PAPM_GBK[win_id].type[id], GUI_LANG_GetTextEx(Lang_CurtWinDow, Language_Bit));		
        strcpy((char *)JDQ_PAPM_GBK[win_id + 1].type[id], GUI_LANG_GetTextEx(Lang_CurtWinDow, Language_Bit));

        if (dev_inf.WIN.detail[i].enable != STA_ON) {
            sprintf((char *)JDQ_PAPM_GBK[win_id].type[id], "%s-%s;", JDQ_PAPM_GBK[win_id].type[id],GUI_LANG_GetTextEx(Lang_On, Language_Bit));	
            sprintf((char *)JDQ_PAPM_GBK[win_id + 1].type[id], "%s-%s;", JDQ_PAPM_GBK[win_id + 1].type[id],GUI_LANG_GetTextEx(Lang_Off, Language_Bit));		
            continue;
        }

        JDQ_PAPM_GBK[win_id].sta = 2;
        JDQ_PAPM_GBK[win_id + 1].sta = 2;

        if (ctrl_inf.lv_more.fan_auto_sort == STA_ON) {
            sprintf((char *)JDQ_PAPM_GBK[win_id].type[id], "%s%d.%s;", JDQ_PAPM_GBK[win_id].type[id], cal_inf.win_showid[i] + 1,GUI_LANG_GetTextEx(Lang_On, Language_Bit));			
            sprintf((char *)JDQ_PAPM_GBK[win_id + 1].type[id], "%s%d.%s;", JDQ_PAPM_GBK[win_id + 1].type[id], cal_inf.win_showid[i] + 1,GUI_LANG_GetTextEx(Lang_Off, Language_Bit));		
        } else {
            sprintf((char *)JDQ_PAPM_GBK[win_id].type[id], "%s%d.%s;", JDQ_PAPM_GBK[win_id].type[id], i + 1,GUI_LANG_GetTextEx(Lang_On, Language_Bit));		
            sprintf((char *)JDQ_PAPM_GBK[win_id + 1].type[id], "%s%d.%s;", JDQ_PAPM_GBK[win_id + 1].type[id], i + 1,GUI_LANG_GetTextEx(Lang_Off, Language_Bit));		
        }
    }

    for (i = 0; i < WET_NUM_MAX; i++) {
        /*湿帘*/
        id = 1;
        sprintf((char *)JDQ_PAPM_GBK[JDQ_WET + i].type[id], "%s%d;", GUI_LANG_GetTextEx(Lang_Wet_Curtain, Language_Bit), i + 1);
        if (dev_inf.wet_en[i] != STA_ON)
            continue;

        JDQ_PAPM_GBK[JDQ_WET + i].sta = 2;
    }

    /*环流风机*/
    id = 1;
    sprintf((char *)JDQ_PAPM_GBK[JDQ_CIR_FAN].type[id], "%s;", GUI_LANG_GetTextEx(Lang_Circulating_Fan, Language_Bit));
    if (dev_inf.circle.enable == STA_ON) {
        JDQ_PAPM_GBK[JDQ_CIR_FAN].sta = 2;
    }
    /*温控开关*/
    id = 1;
    sprintf((char *)JDQ_PAPM_GBK[JDQ_TEMPC].type[id], "%s;", GUI_LANG_GetTextEx(Lang_Tswitch, Language_Bit));
    if (basic_inf.tempc_en.enable == STA_ON) {
        JDQ_PAPM_GBK[JDQ_TEMPC].sta = 2;
    }

    for (i = 0; i < HEAT_NUM_MAX; i++) {
        /*加热*/
        id = 1;
        sprintf((char *)JDQ_PAPM_GBK[JDQ_HEAT1 + i].type[id], "%s%d;", GUI_LANG_GetTextEx(Lang_Heat, Language_Bit), i + 1);
        if (basic_inf.heat_en[i] != STA_ON)
            continue;

        JDQ_PAPM_GBK[JDQ_HEAT1 + i].sta = 2;
    }
    /*喷淋*/
    id = 2;
    sprintf((char *)JDQ_PAPM_GBK[JDQ_SPRAY].type[id], "%s;", GUI_LANG_GetTextEx(Lang_Spray, Language_Bit));
    if (dev_inf.spray_en != SPRAY_MODE_OFF) {
        JDQ_PAPM_GBK[JDQ_SPRAY].sta = 3;
    }
    for (i = 0; i < TR_TYPE_NUM; i++) {
        /*定时器类型更新*/
        sprintf((char *)&JDQ_PAPM_GBK[JDQ_TIMER1 + i].type[2][0], "%s%d;", GUI_LANG_GetTextEx(Lang_timer_type[ext_inf.timer[TR_TYPE_TIMER1 + i].type], Language_Bit), (i + 1));
        if (ext_inf.timer[i].enable != STA_ON)
            continue;

        JDQ_PAPM_GBK[JDQ_TIMER1 + i].sta = 3;
    }

    if (basic_inf.ala_en == STA_ON) {
        sprintf((char *)JDQ_PAPM_GBK[JDQ_ALA].type[0], "%s;", GUI_LANG_GetTextEx(Lang_ALA, Language_Bit));
        JDQ_PAPM_GBK[JDQ_ALA].sta = 1;
    }
}

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
    { TEXT_CreateIndirect,      "设备管理-继电器功能总览",          GUI_ID_TEXT0,            250, 4, 300, 42, 0, 0},
    { LISTVIEW_CreateIndirect,   NULL,               GUI_ID_LISTVIEW0,        40, 52, 725, 356, 0, 0},

    { TEXT_CreateIndirect,      "红色字体为当前设置模式!",      GUI_ID_TEXT1,            250, 425, 300, 42, 0, 0},

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
//    GUI_SetColor(GUI_LINE_COLOR);
//    GUI_DrawRoundedRect(20, 62, 780, 395, 10);
}


static int LISTVIEW_cbButton(WIDGET_ITEM_DRAW_INFO * pDrawItemInfo)
{
    WM_HWIN  listview0_hWin = WM_GetDialogItem(pDrawItemInfo->hWin, GUI_ID_LISTVIEW0);
    int      i, ret, x0 = pDrawItemInfo->x0, x1 = pDrawItemInfo->x1;
    GUI_RECT Site;

    switch (pDrawItemInfo->Cmd) {
    case WIDGET_ITEM_DRAW:

        Site.x0 = (I16)x0;
        Site.x1 = (I16)x1;
        Site.y0 = (I16)pDrawItemInfo->y0;
        Site.y1 = (I16)pDrawItemInfo->y1;

        if (select_id == 0) {
            ret = pDrawItemInfo->ItemIndex;

            if (pDrawItemInfo->Col == 0) {
                GUI_SetColor(GUI_BLACK);
                sprintf(show_buf, "%d", ret + 1);
                GUI_DispStringInRect(show_buf, &Site, GUI_TA_HCENTER | GUI_TA_HCENTER);
            } else if (pDrawItemInfo->Col == 1) {
                for (i = 0; i < PWM_PAPM_GBK[ret].all_num; i++) {
                    if (PWM_PAPM_GBK[ret].sta == i + 1) {
                        GUI_SetColor(GUI_RED);
                        GUI_DispStringInRect((const char *)PWM_PAPM_GBK[ret].type[i], &Site, GUI_TA_VCENTER | GUI_TA_LEFT);
                        if (Language_Bit_original >= LANGUAGE_RUSSIAN)
                            Site.x0 += (Get_UTF8TEXT_LEN((const char *)PWM_PAPM_GBK[ret].type[i]) * 8 + 25);
                        else if (basic_inf.language == LANGUAGE_ENGLISH)
                            Site.x0 += (strlen((const char *)PWM_PAPM_GBK[ret].type[i]) * 11 + 3);
                        else
                            Site.x0 += (strlen((const char *)PWM_PAPM_GBK[ret].type[i]) * 8 + 3);
                        continue;
                    }

                    GUI_SetColor(GUI_BLACK);
                    GUI_DispStringInRect((const char *)PWM_PAPM_GBK[ret].type[i], &Site, GUI_TA_VCENTER | GUI_TA_LEFT);
                    if (Language_Bit_original >= LANGUAGE_RUSSIAN)
                        Site.x0 += (Get_UTF8TEXT_LEN((const char *)PWM_PAPM_GBK[ret].type[i]) * 8 + 25);
                    else if (basic_inf.language == LANGUAGE_ENGLISH)
                        Site.x0 += (strlen((const char *)PWM_PAPM_GBK[ret].type[i]) * 11 + 3);
                    else
                        Site.x0 += (strlen((const char *)PWM_PAPM_GBK[ret].type[i]) * 8 + 3);
                }
            }
        } else {
            ret = pDrawItemInfo->ItemIndex + (pDrawItemInfo->Col - 1) * 5;

            if (pDrawItemInfo->Col == 0 || pDrawItemInfo->Col == 2) {
                GUI_SetColor(GUI_BLACK);
                sprintf(show_buf, "%d", (pDrawItemInfo->ItemIndex + 1 + pDrawItemInfo->Col * 5));
                GUI_DispStringInRect(show_buf, &Site, GUI_TA_HCENTER | GUI_TA_HCENTER);
            } else if (pDrawItemInfo->Col == 1 || pDrawItemInfo->Col == 3) {
                for (i = 0; i < JDQ_PAPM_GBK[ret].all_num; i++) {
                    if (JDQ_PAPM_GBK[ret].sta == i + 1) {
                        GUI_SetColor(GUI_RED);
                        GUI_DispStringInRect((const char *)JDQ_PAPM_GBK[ret].type[i], &Site, GUI_TA_VCENTER | GUI_TA_LEFT);
                        if (Language_Bit_original >= LANGUAGE_RUSSIAN)
                            Site.x0 += (Get_UTF8TEXT_LEN((const char *)JDQ_PAPM_GBK[ret].type[i]) * 9 + 8);
                        else if (basic_inf.language == LANGUAGE_ENGLISH)
                            Site.x0 += (strlen((const char *)JDQ_PAPM_GBK[ret].type[i]) * 11 + 3);
                        else
                            Site.x0 += (strlen((const char *)JDQ_PAPM_GBK[ret].type[i]) * 8 + 3);
                        continue;
                    }

                    GUI_SetColor(GUI_BLACK);
                    GUI_DispStringInRect((const char *)JDQ_PAPM_GBK[ret].type[i], &Site, GUI_TA_VCENTER | GUI_TA_LEFT);
                    if (Language_Bit_original >= LANGUAGE_RUSSIAN)
                        Site.x0 += (Get_UTF8TEXT_LEN((const char *)JDQ_PAPM_GBK[ret].type[i]) * 9 + 8);
                    else if (basic_inf.language == LANGUAGE_ENGLISH)
                        Site.x0 += (strlen((const char *)JDQ_PAPM_GBK[ret].type[i]) * 11 + 3);
                    else
                        Site.x0 += (strlen((const char *)JDQ_PAPM_GBK[ret].type[i]) * 8 + 3);
                }
            }
        }

        break;

    default:
        return LISTVIEW_OwnerDraw(pDrawItemInfo);
    }

    return LISTVIEW_OwnerDraw(pDrawItemInfo);
}

static void InitDialog(WM_MESSAGE * pMsg)
{
    WM_HWIN hWin = pMsg->hWin;
    int MDev_ID = 0, i = 0;
    WM_HWIN hItem;
    WM_HWIN listview0_hWin;

    for (MDev_ID = GUI_ID_BUTTON2; MDev_ID <= GUI_ID_BUTTON2; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, _cbButtonBack);
    }

    for (MDev_ID = GUI_ID_TEXT0; MDev_ID <= GUI_ID_TEXT1; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24(hItem);
        TEXT_SetTextAlign(hItem,  GUI_TA_HCENTER | GUI_TA_VCENTER);

        if (MDev_ID == GUI_ID_TEXT0) {
            TEXT_SetTextColor(hItem, GUI_WHITE);
        } else {
            TEXT_SetTextColor(hItem, GUI_DEF_GRAY);
        }
    }

    listview0_hWin = WM_GetDialogItem(hWin, GUI_ID_LISTVIEW0);
    HEADER_SetFont( LISTVIEW_GetHeader(listview0_hWin), &GUI_USER_HZ24);
    LISTVIEW_SetFont(listview0_hWin, &GUI_USER_HZ24);
	if(Language_Bit_original >= LANGUAGE_RUSSIAN){
	    HEADER_SetFont( LISTVIEW_GetHeader(listview0_hWin), &GUI_FontBC_16);
	    LISTVIEW_SetFont(listview0_hWin, &GUI_FontBC_16);
	}	
    LISTVIEW_SetGridVis(listview0_hWin, 1);
    LISTVIEW_SetRowHeight(listview0_hWin, 32);
    WM_DisableWindow(LISTVIEW_GetHeader(listview0_hWin));

    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT0), GUI_LANG_GetTextEx(Lang_JDQ_Overview, Language_Bit));                  // 设备管理-继电器功能总览
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT1), GUI_LANG_GetTextEx(Lang_JDQ_Overview_Tips, Language_Bit));             // 红色字体为当前设置模式!
    LISTVIEW_AddColumn(listview0_hWin, 64, GUI_LANG_GetTextEx(Lang_Number_No, Language_Bit), GUI_TA_VCENTER | GUI_TA_CENTER); // 序号
    LISTVIEW_AddColumn(listview0_hWin, 275, GUI_LANG_GetTextEx(Lang_Function, Language_Bit), GUI_TA_VCENTER | GUI_TA_CENTER); // 功能
    LISTVIEW_AddColumn(listview0_hWin, 64, GUI_LANG_GetTextEx(Lang_Number_No, Language_Bit), GUI_TA_VCENTER | GUI_TA_CENTER); // 序号
    LISTVIEW_AddColumn(listview0_hWin, 320, GUI_LANG_GetTextEx(Lang_Function, Language_Bit), GUI_TA_VCENTER | GUI_TA_CENTER); // 功能

    LISTVIEW_EnableCellSelect(listview0_hWin, 1);
    LISTVIEW_SetHeaderHeight(listview0_hWin, 32);
//    hScroll = SCROLLBAR_CreateAttached(listview0_hWin, SCROLLBAR_CF_VERTICAL); //创建1个垂直滚动条
//    SCROLLBAR_SetWidth(hScroll, 31); //设置滚动条宽
//    SCROLLBAR_SetThumbSizeMin(30);
    LISTVIEW_SetBkColor(listview0_hWin, LISTVIEW_CI_UNSEL, GUI_DEF_GRAY);

    LISTVIEW_SetOwnerDraw(listview0_hWin, (WIDGET_DRAW_ITEM_FUNC *) &LISTVIEW_cbButton);

    if (select_id == 0) {

        TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT0), GUI_LANG_GetTextEx(Lang_DAC_overview, Language_Bit));

        for (i = 0; i < ALLPWM_NUM_MAX; i++) {
            LISTVIEW_AddRow(listview0_hWin, NULL);
        }

        return;
    }

    for (i = 0; i < ALLJDQ_NUM_MAX / 2; i++) {
        LISTVIEW_AddRow(listview0_hWin, NULL);
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
    unsigned char Slect_Row;
    unsigned char Slect_Column;

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
                Current_HWIN = WM_GetParent(hWin);
                GUI_EndDialog(hWin, 0);

                if (select_id == 0) {
                    MDev_Regulatingfan_Task(WM_GetParent(hWin), 0);
                } else {
                    MDev_FixedSpeedfan_Task(WM_GetParent(hWin), 0);
                }
            }

            break;

        case GUI_ID_LISTVIEW0:
            if (NCode == WM_NOTIFICATION_RELEASED) {

                Slect_Row = LISTVIEW_GetSel(WM_GetDialogItem(hWin, GUI_ID_LISTVIEW0));
                Slect_Column = LISTVIEW_GetSelCol(WM_GetDialogItem(hWin, GUI_ID_LISTVIEW0));

                if (select_id == 0) {
                    if (Slect_Row <= 5 && Slect_Column <= 1) {
                        Select = (Slect_Row / 2);
                        GUI_EndDialog(hWin, 0);
                        MDev_Regulatingfan_Task(WM_GetParent(hWin), Select);
                    }

                    break;
                }

                if (Slect_Row <= 9 && Slect_Column <= 3) {
                    Select = (Slect_Column / 2) * 5 + (Slect_Row / 2);
                    GUI_EndDialog(hWin, 0);
                    MDev_FixedSpeedfan_Task(WM_GetParent(hWin), Select);
                }
            }
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
WM_HWIN MDev_FuncView;
void MDev_FuncView_Task(WM_HWIN hParent, U8 id)
{
    select_id = id;
    MDev_FuncView = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    Current_HWIN = MDev_FuncView;
    Updata_Equ_Sta();
}

