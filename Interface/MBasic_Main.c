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
#include "pcf8563.h"
#include "bsp.h"
#include "datasave.h"
#include "recode.h"
#include <string.h>
#include "def.h"
#include "stm324x9i_eval_lcd.h"
#include "w25qxx.h"
#include "trial_ver.h"

//EventsFunctionList
//EndofEventsFunctionList


GUI_CONST_STORAGE GUI_BITMAP hHomepic = {
    800, 	// xSize
    480, 	// ySize
    1600,	// BytesPerLine
    16, 	// BitsPerPixel
    (const unsigned char *)(SDRAM_PIC_ADDR + (WHOME_ID - 1) * PIC_SIZE),
    NULL, // Pointer to palette
    GUI_DRAW_BMPM565
};


#define GUI_ID_TEXT10   (GUI_ID_USER+1)
#define GUI_ID_TEXT11   (GUI_ID_USER+2)
#define GUI_ID_TEXT12   (GUI_ID_USER+3)
#define GUI_ID_TEXT13   (GUI_ID_USER+4)
#define GUI_ID_TEXT14   (GUI_ID_USER+5)
#define GUI_ID_TEXT15   (GUI_ID_USER+6)
#define GUI_ID_TEXT16   (GUI_ID_USER+7)
#define GUI_ID_TEXT17   (GUI_ID_USER+8)
#define GUI_ID_TEXT18   (GUI_ID_USER+9)
#define GUI_ID_TEXT19   (GUI_ID_USER+10)
#define GUI_ID_TEXT20   (GUI_ID_USER+11)
#define GUI_ID_TEXT21   (GUI_ID_USER+12)

#define GUI_ID_BUTTON10 (GUI_ID_USER + 21)
#define GUI_ID_BUTTON11 (GUI_ID_USER + 22)
#define GUI_ID_BUTTON12 (GUI_ID_USER + 23)
#define GUI_ID_BUTTON13 (GUI_ID_USER + 24)

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
    { WINDOW_CreateIndirect,    "",               0,                   0,  0,  800, 480, 0, 0},
    { TEXT_CreateIndirect,      "",      	  GUI_ID_TEXT0,            10, 0, 240, 40, 0, 0},//bak
    { TEXT_CreateIndirect,      "基本信息",   GUI_ID_TEXT1,            250, 4, 300, 40, 0, 0},
    { TEXT_CreateIndirect,      "",     	  GUI_ID_TEXT2,            585, 5, 240, 40, 0, 0},//time

    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON0,          10, 420, 66, 60, 0, 0},//home

    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON2,          450 + 0 * 70, 225 - 1 * 19, 60, 60, 0, 0}, //hum
    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON1,          450 + 1 * 70, 225 - 2 * 23, 60, 60, 0, 0}, //vac
    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON3,          450 + 2 * 70, 225 - 3 * 25, 60, 60, 0, 0}, //co2

    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON5,          70, 420, 66, 60, 0, 0},//login

    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON6,          320, 220, 150, 100, 0, 0},//lv

    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON7,          650, 260, 100, 100, 0, 0},//windows

    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON8,          630, 110, 123, 44, 0, 0},//win1
    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON9,          630, 200, 100, 30, 0, 0},//win2

    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON12,          490 + 0 * 50 , 298 - 0 * 27, 25, 25, 0, 0},//heat1
    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON11,          490 + 1 * 50 , 298 - 1 * 27, 25, 25, 0, 0},//heat2
    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON10,          490 + 2 * 50 , 298 - 2 * 27, 25, 25, 0, 0},//heat3

    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON13,          30, 375, 80, 45, 0, 0},//ala shield

    { TEXT_CreateIndirect,      "",     	  GUI_ID_TEXT3,            	70, 0, 200, 50, 0, 0},//room
    { TEXT_CreateIndirect,      "",     	  GUI_ID_TEXT4,            	96, 130, 72, 24, 0, 0},//temp

    { TEXT_CreateIndirect,      "",     	  GUI_ID_TEXT5,        		103, 257, 100, 24, 0, 0},//day
    { TEXT_CreateIndirect,      "",     	  GUI_ID_TEXT6,        		103, 291, 280, 24, 0, 0},//lv
    { TEXT_CreateIndirect,      "",        	  GUI_ID_TEXT7,        		103, 326, 175, 24, 0, 0},//风机数
    { TEXT_CreateIndirect,      "",     	  GUI_ID_TEXT8,        		103, 223, 150, 24, 0, 0},//体感温度
    { TEXT_CreateIndirect,      "",        	  GUI_ID_TEXT9,        		130, 373, 680, 46, 0, 0},//ala

    { TEXT_CreateIndirect,      "",     	  GUI_ID_TEXT11,            429 + 0 * 70, 215 - 1 * 20, 100, 20, 0, 0},//hum
    { TEXT_CreateIndirect,      "",     	  GUI_ID_TEXT10,        	425 + 1 * 70, 215 - 2 * 21, 100, 20, 0, 0},//vac
    { TEXT_CreateIndirect,      "",     	  GUI_ID_TEXT12,        	425 + 2 * 70, 215 - 3 * 25, 100, 20, 0, 0},//co2

    { TEXT_CreateIndirect,      "",     	  GUI_ID_TEXT21,        	220, 70, 310, 20, 0, 0}, //报警温度
    { TEXT_CreateIndirect,      "",     	  GUI_ID_TEXT13,        	220, 100, 310, 20, 0, 0},//目标温度
    { TEXT_CreateIndirect,      "",     	  GUI_ID_TEXT14,        	103, 358, 200, 20, 0, 0},//风速
    { TEXT_CreateIndirect,      "",     	  GUI_ID_TEXT15,        	220, 130, 285, 20, 0, 0},//目标静压

    { TEXT_CreateIndirect,      "",     	  GUI_ID_TEXT16,        	510, 43, 290, 20, 0, 0},//今日饮水量
    { TEXT_CreateIndirect,      "",     	  GUI_ID_TEXT17,        	510, 63, 290, 20, 0, 0},//今日采食量

    { TEXT_CreateIndirect,      "",     	  GUI_ID_TEXT18,        	710, 210, 90, 20, 0, 0},//wet1
    { TEXT_CreateIndirect,      "",     	  GUI_ID_TEXT19,        	710, 230, 90, 20, 0, 0},//wet2

    { TEXT_CreateIndirect,      "",     	  GUI_ID_TEXT20,        	570, 83, 250, 20, 0, 0},//egg

};

static WM_HTIMER hTimer;
static U8 fan_state = 0;
static U8 show_sec = 0;
static const char *MPIC[] = {"96bmMPic_Hum", "53MPic_Vac", "94bmMPic_CO2",};
extern const char *MHEAT[];
extern const char *MWET[];
#define DIS_PIC		(2)

static U8 sen_sta[3] = {0};
static U8 sen_num = 0;

/*****************************************************************
**      FunctionName:void PaintDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**
**      call this function in _cbCallback --> WM_PAINT
*****************************************************************/
static void PaintDialog(WM_MESSAGE * pMsg)
{
//    WM_HWIN hWin = pMsg->hWin;

    unsigned char i, num;
    int site;

    GUI_DrawBitmap(&hHomepic, 0, 0);

    if (sys_sta.net_sta != STA_OFF) {
        GUI_defDrawBitmap("34bmMWIFI_YES", 590, 12);
    } else {
        GUI_defDrawBitmap("35bmMWIFI_NO", 590, 12);
    }

    if (soft_inf.creg_en == STA_ON) {
        if (sys_sta.net_creg == STA_ON) {
            GUI_defDrawBitmap("32bmMLogin_YES", 560, 13);
        } else {
            GUI_defDrawBitmap("33bmMLogin_NO", 560, 13);
        }
    }

    GUI_defDrawBitmap("97bmMPic_OutTemp", 195, 68);//报警
    GUI_defDrawBitmap("97bmMPic_OutTemp", 195, 98);//目标

    if (cal_inf.SEN_EN[SENSOR_TYPE_VAC] == STA_ON) {
        GUI_defDrawBitmap("99Mpic_Vac", 195, 135);//vac
    }

    if (cal_inf.SEN_EN[SENSOR_TYPE_SPEED] != STA_ON && basic_inf.humtemp_en != STA_ON) {
    } else {
        if (basic_inf.humtemp_en == STA_ON && basic_inf.wind_area == 0) {
        } else {
            GUI_defDrawBitmap("99Mpic_Speed", 65, 357);	   //风速
        }
    }

    if (cal_inf.humtemp != TEMP_ER_VALUE) {
        GUI_defDrawBitmap("97bmMPic_OutTemp", 65, 222);
    }

//    if (dev_inf.sensor_adj.TEMP_EN[TEMP_OUT_SID] == STA_OFF || sta_in.sensor_real.TEMP[TEMP_OUT_SID] == TEMP_ER_VALUE) {
//    } else {
//        GUI_defDrawBitmap("46MPic_OutTemp", 30, 140);
//    }

    if (basic_inf.type <= ROOM_TYPE_SOW) {
        GUI_defDrawBitmap("54MPic_Pig", 35, 9);
    } else if (basic_inf.type <= ROOM_CHICKEN_ADULT) {
        GUI_defDrawBitmap("45MPic_Chick", 35, 9);
    } else if (basic_inf.type <= ROOM_DUCK_SEED) {
        GUI_defDrawBitmap("55MPic_duck", 35, 9);
    } else {
        GUI_defDrawBitmap("45MPic_Chick", 35, 9);
    }


    if (ALA_Get_State(ALA_HTEMP) == STA_ON) {
        GUI_defDrawBitmap("12bmMPic_MtempHIGH", 67, 77);
    } else if (ALA_Get_State(ALA_LTEMP) == STA_ON) {
        GUI_defDrawBitmap("11bmMpic_MtempLOW", 67, 77);
    } else {
        GUI_defDrawBitmap("13bmMPic_MtempNOL", 67, 77);
    }

    GUI_SetColor(GUI_WHITE);
    GUI_SetFont(&GUI_USER_HZ16);
    GUI_SetTextMode(GUI_TM_TRANS);

    if (ALA_State() > 0) {
        if (run_inf.alarm_shield_time != 0) {
            GUI_defDrawBitmap("09bmMPic_MalaOFF", 40, 382);
            sprintf(show_buf, "%02d:%02d", run_inf.alarm_shield_time / 60, run_inf.alarm_shield_time % 60); //剩余时间
            GUI_DispStringAt((const char *) show_buf, 75, 388);

        } else {
            GUI_defDrawBitmap("09bmMPic_MalaON", 40, 382);
        }
    } else {
        GUI_defDrawBitmap("10bmMPic_Mnol", 40, 382);
    }

//    GUI_defDrawBitmap("47MPic_Day", 30, 100);
//    GUI_defDrawBitmap("48MPic_FanLv", 567, 310);
//    GUI_defDrawBitmap("49MPic_MinAir", 567, 348);
//    GUI_defDrawBitmap("50MPic_CurAir", 567, 392);

//    if (basic_inf.humtemp_en == STA_ON) {
//        GUI_defDrawBitmap("50MPic_CurAir", 30, 355);
//        GUI_defDrawBitmap("56MPic_HumTemp", 30, 390);
//    }

    if (dev_inf.wet_en[0] == STA_ON) {
        if (sta_out.ALLJDQ[JDQ_WET] == STA_ON) {
            GUI_defDrawBitmap(MWET[fan_state], 625, 220);
        } else {
            GUI_defDrawBitmap(MWET[DIS_PIC], 625, 220);
        }
    }

    if (dev_inf.wet_en[1] == STA_ON) {
        if (sta_out.ALLJDQ[JDQ_WET2] == STA_ON) {
            GUI_defDrawBitmap(MWET[fan_state], 600, 118);
        } else {
            GUI_defDrawBitmap(MWET[DIS_PIC], 600, 118);
        }
    }

    GUI_SetColor(GUI_WHITE);
    GUI_SetFont(&GUI_USER_HZ16);
    GUI_SetTextMode(GUI_TM_TRANS);

    if (Language_Bit_original >= LANGUAGE_RUSSIAN) {
        GUI_SetFont(&GUI_FontBC_16);
    }

    for (i = 0, num = 0; i < ALLPWM_NUM_MAX; i++) {
        if (dev_inf.SWIN.detail[i].enable == STA_OFF)
            continue;

        num++;
    }

    if (num == 0) {
        sprintf(show_buf, "%s: --", GUI_LANG_GetTextEx(Lang_Inlet, Language_Bit));//小窗
        GUI_DispStringAt((const char *) show_buf, 600, 320);
    } else {
        site = num >= 4 ? 260 : 320 - (num - 1) * 20;

        for (i = 0, num = 0; i < ALLPWM_NUM_MAX; i++) {
            if (dev_inf.SWIN.detail[i].enable == STA_OFF)
                continue;

            num++;
            sprintf(show_buf, "%s%d: %d%%", GUI_LANG_GetTextEx(Lang_Inlet, Language_Bit), i + 1, sta_out.ALLPWM[i]); //小窗
            GUI_DispStringAt((const char *) show_buf, 600, (site + (num - 1) * 20));
        }
    }

//    if (Language_Bit_original == LANGUAGE_GBK) {
//        sprintf(show_buf, "1%s:%.1f%s", GUI_LANG_GetTextEx(Lang_AreaSpl, Language_Bit), (float)(cal_inf.tempArea[0] * 1.0f / TEMP_RATE), GUI_LANG_GetTextEx(Lang_oC, Language_Bit));
//        GUI_DispStringAt((const char *)show_buf, 200, 100);
//        show_buf[0]++;
//        GUI_DispStringAt((const char *)show_buf, 200, 130);
//        show_buf[0]++;
//        GUI_DispStringAt((const char *)show_buf, 200, 160);
//    } else {
//        sprintf(show_buf, "1:%.1f%s", (float)(cal_inf.tempArea[0] * 1.0f / TEMP_RATE), GUI_LANG_GetTextEx(Lang_oC, Language_Bit));
//        GUI_DispStringAt((const char *)show_buf, 200, 100);
//        show_buf[0]++;
//        GUI_DispStringAt((const char *)show_buf, 200, 130);
//        show_buf[0]++;
//        GUI_DispStringAt((const char *)show_buf, 200, 160);
//    }

//    for (num = 0, i = 0; i < TEMP_NUM_MAX; i++) {
//        if (num >= 3) {
//            break;
//        }

//        if (dev_inf.sensor_adj.TEMP_EN[i] == STA_OFF) {
//            continue;
//        }

//        sprintf(show_buf, "T%d:%.1f%s", i + 1, (float)(sta_in.sensor_adj.TEMP[i] * 1.0f / TEMP_RATE), GUI_LANG_GetTextEx(Lang_oC, Language_Bit));
//        GUI_DispStringAt((const char *)show_buf, 200, 100 + 30 * num);
//		num++;
//    }
	
}

/*****************************************************************
**      FunctionName:void InitDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**
**      call this function in _cbCallback --> WM_INIT_DIALOG
*****************************************************************/
static void _cbButton(WM_MESSAGE * pMsg)
{
    WM_HWIN  hWin;
    int 	 Id, i, num;
    hWin  = pMsg->hWin;
    Id = WM_GetId(hWin);

    switch (pMsg->MsgId) {
    case WM_PAINT:
        if (BUTTON_IsPressed(hWin)) {
            GUI_SetBkColor(GUI_WHITE);

//				GUI_Clear();
            if (sen_num < Id - GUI_ID_BUTTON1 + 1)
                break;

            GUI_SetAlpha(0xb0);

            for (i = 0, num = 0; i < 3; i++) {
                if (sen_sta[i] != STA_ON)
                    continue;

                if (++num >= (Id - GUI_ID_BUTTON1 + 1))
                    break;
            }

            GUI_defDrawBitmap(MPIC[i], 10, 14);

            GUI_SetAlpha(0);
        } else {
            if (sen_num < Id - GUI_ID_BUTTON1 + 1)
                break;

            for (i = 0, num = 0; i < 3; i++) {
                if (sen_sta[i] != STA_ON)
                    continue;

                if (++num >= (Id - GUI_ID_BUTTON1 + 1))
                    break;
            }

            GUI_defDrawBitmap(MPIC[i], 10, 14);
        }

        break;

    default:
        BUTTON_Callback(pMsg);
    }
}

static void _cbButton_Login(WM_MESSAGE * pMsg)
{
    WM_HWIN  hWin;
    hWin  = pMsg->hWin;

    switch (pMsg->MsgId) {
    case WM_PAINT:
        if (BUTTON_IsPressed(hWin)) {
            GUI_SetBkColor(GUI_WHITE);
            GUI_SetAlpha(0xb0);

            if (sys_sta.login_type == LOGIN_GUEST) {
                GUI_defDrawBitmap("07bmMpic_logout", 10, 14);
            } else {
                GUI_defDrawBitmap("08bmMpic_login", 10, 14);
            }

            GUI_SetAlpha(0);
        } else {
            if (sys_sta.login_type == LOGIN_GUEST) {
                GUI_defDrawBitmap("07bmMpic_logout", 10, 14);
            } else {
                GUI_defDrawBitmap("08bmMpic_login", 10, 14);
            }
        }

        break;

    default:
        BUTTON_Callback(pMsg);
    }
}

static void _cbButton_Home(WM_MESSAGE * pMsg)
{
    WM_HWIN  hWin;
    hWin  = pMsg->hWin;

    switch (pMsg->MsgId) {
    case WM_PAINT:
        if (BUTTON_IsPressed(hWin)) {
            GUI_SetBkColor(GUI_WHITE);
            GUI_SetAlpha(0xb0);
            GUI_defDrawBitmap("06bmMPic_Home", 10, 14);
            GUI_SetAlpha(0);
        } else {
            GUI_defDrawBitmap("06bmMPic_Home", 10, 14);
        }

        break;

    default:
        BUTTON_Callback(pMsg);
    }
}

static void _cbButton_Heat(WM_MESSAGE * pMsg)
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
            GUI_defDrawBitmap(MHEAT[DIS_PIC], 0, 0);
            GUI_SetAlpha(0);
        } else {
            if (sta_out.ALLJDQ[JDQ_HEAT1 + (Id - GUI_ID_BUTTON10)] == STA_ON) {
                GUI_defDrawBitmap(MHEAT[fan_state], 0, 0);
            } else {
                GUI_defDrawBitmap(MHEAT[DIS_PIC], 0, 0);
            }
        }

        break;

    default:
        BUTTON_Callback(pMsg);
    }
}

static void _cbButton_None(WM_MESSAGE * pMsg)
{
    switch (pMsg->MsgId) {
    case WM_PAINT:
        break;

    default:
        BUTTON_Callback(pMsg);
    }
}
static char fan_temp_show[24];

static void	Show_Data(WM_HWIN hWin)
{
    int id = GUI_ID_TEXT2, i, j, num, time;
    time_t sys_time = GetSysTime();
    int all_tatol = 0, cal_tatol = 0;

    struct tm *t;

    show_sec++;

    if (show_sec >= 6) {
        show_sec = 0;
    }

    t = localtime((const time_t *)&sys_time);
    sprintf(show_buf, "%02d/%02d/%02d %02d:%02d:%02d", (t->tm_year - 100), (t->tm_mon + 1), t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);

    TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *) show_buf);

    sprintf(show_buf, "%s %d", GUI_LANG_GetTextEx(room_type[basic_inf.type], Language_Bit), basic_inf.room);//房间类型房间号
    TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *)show_buf);

	if (basic_inf.language == LANGUAGE_Arabic){
		sprintf(show_buf, "%s%.1f", GUI_LANG_GetTextEx(Lang_oC, Language_Bit), (float)(cal_inf.temp * 1.0f / TEMP_RATE));
	    TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *)show_buf);
	} else {
	    sprintf(show_buf, "%.1f%s", (float)(cal_inf.temp * 1.0f / TEMP_RATE), GUI_LANG_GetTextEx(Lang_oC, Language_Bit));
	    TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *)show_buf);
	}

    sprintf(show_buf, "%s: %d", GUI_LANG_GetTextEx(Lang_Day, Language_Bit), basic_inf.grow_day);
    TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *)show_buf);

    if (ctrl_inf.exminair.enable == STA_ON) {
        sprintf(show_buf, "%s: %d", GUI_LANG_GetTextEx(Lang_Level, Language_Bit), cal_inf.lv + 1);
    } else {
        sprintf(show_buf, "%s: %d(%.1f%s)", GUI_LANG_GetTextEx(Lang_Level, Language_Bit), cal_inf.lv + 1, (ctrl_inf.lv_more.lv_temp_adj * 1.0f / TEMP_RATE), GUI_LANG_GetTextEx(Lang_oC, Language_Bit));
    }

    if (run_inf.LV_StartLast < 0 || run_inf.LV_StopLast < 0) {

    } else {

        if (run_inf.LV_StopTime >= (TIME_4BIT + 1)) {
            sprintf(fan_temp_show, ";%s:%ds", GUI_LANG_GetTextEx(Lang_On, Language_Bit), run_inf.LV_StartLast);
        } else {
            sprintf(fan_temp_show, ";%s:%ds", GUI_LANG_GetTextEx(Lang_Off, Language_Bit), run_inf.LV_StopLast);
        }

        strcat(show_buf, fan_temp_show);
    }

    TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *) show_buf);

    sprintf(show_buf, "%s %d", GUI_LANG_GetTextEx(Lang_Num_fans, Language_Bit), cal_inf.total_fan_num);//运行风机个数
    TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *) show_buf);

    if (cal_inf.humtemp == TEMP_ER_VALUE) {
        WM_HideWindow(WM_GetDialogItem(hWin, id));
    } else {
        WM_ShowWindow(WM_GetDialogItem(hWin, id));
    }

    sprintf(show_buf, "%s:%.1f%s", GUI_LANG_GetTextEx(Lang_Apparent_Temp, Language_Bit), (float)(cal_inf.humtemp * 1.0f / TEMP_RATE), GUI_LANG_GetTextEx(Lang_oC, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *)show_buf);

    id = GUI_ID_TEXT18;
 
    for (i = 0; i < WET_NUM_MAX; i++) {
        if (dev_inf.wet_en[i] == STA_OFF || cal_inf.WET_lv[i] < 0) {
            WM_HideWindow(WM_GetDialogItem(hWin, id++));
        } else {
            WM_ShowWindow(WM_GetDialogItem(hWin, id));

            if (sta_out.ALLJDQ[JDQ_WET + i] == STA_ON) {
                sprintf(show_buf, "%d%s:%ds", i + 1, GUI_LANG_GetTextEx(Lang_On, Language_Bit), run_inf.WET_Topen[i] - run_inf.WET_Time[i]);
            } else {
                sprintf(show_buf, "%d%s:%ds", i + 1, GUI_LANG_GetTextEx(Lang_Off, Language_Bit), run_inf.WET_Tclose[i] - run_inf.WET_Time[i]);
            }

            TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *) show_buf);
        }
    }

    id = GUI_ID_TEXT10;

    if (cal_inf.SEN_EN[SENSOR_TYPE_HUM] == STA_ON) {
        WM_ShowWindow(WM_GetDialogItem(hWin, id));

        if (cal_inf.SEN[SENSOR_TYPE_HUM] == (short)SENSOR_ER_VALUE) {
            TEXT_SetText(WM_GetDialogItem(hWin, id++), "--%");
        } else {
            sprintf(show_buf, "%d%%", cal_inf.SEN[SENSOR_TYPE_HUM]);
            TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *) show_buf);
        }
    }

    if (cal_inf.SEN_EN[SENSOR_TYPE_VAC] == STA_ON) {
        WM_ShowWindow(WM_GetDialogItem(hWin, id));

        if (cal_inf.SEN[SENSOR_TYPE_VAC] == (short)VAC_ER_VALUE) {
            TEXT_SetText(WM_GetDialogItem(hWin, id++), "--Pa");
        } else {
            sprintf(show_buf, "%dPa", cal_inf.SEN[SENSOR_TYPE_VAC]);
            TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *) show_buf);
        }

        sprintf(show_buf, "%s:%d Pa", GUI_LANG_GetTextEx(Lang_TargetNP, Language_Bit), cal_inf.target_vac);

        WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT15));
        TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT15), (const char *) show_buf);
    } else {
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT15));
    }

    if (cal_inf.SEN_EN[SENSOR_TYPE_CO2] == STA_ON) {
        WM_ShowWindow(WM_GetDialogItem(hWin, id));

        if (cal_inf.SEN[SENSOR_TYPE_CO2] == (short)SENSOR_ER_VALUE) {
            TEXT_SetText(WM_GetDialogItem(hWin, id++), "--ppm");
        } else {
            sprintf(show_buf, "%dppm", cal_inf.SEN[SENSOR_TYPE_CO2]);
            TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *) show_buf);
        }
    }

    for (; id < GUI_ID_TEXT13; id++) {
        WM_HideWindow(WM_GetDialogItem(hWin, id));
    }

    if (ctrl_inf.hum.enable == STA_ON) {
        sprintf(show_buf, "%s:%.1f%s(%.1f%s;%.1f%s)", GUI_LANG_GetTextEx(Lang_TarT, Language_Bit),
                (cal_inf.target_temp * 1.0f / TEMP_RATE), GUI_LANG_GetTextEx(Lang_oC, Language_Bit),
                (cal_inf.tar_hand_temp * 1.0f / TEMP_RATE), GUI_LANG_GetTextEx(Lang_oC, Language_Bit),
                (cal_inf.tar_hum_temp * 1.0f / TEMP_RATE), GUI_LANG_GetTextEx(Lang_oC, Language_Bit));
    } else {
        sprintf(show_buf, "%s:%.1f%s(%.1f%s)", GUI_LANG_GetTextEx(Lang_TarT, Language_Bit),
                (cal_inf.target_temp * 1.0f / TEMP_RATE), GUI_LANG_GetTextEx(Lang_oC, Language_Bit),
                (cal_inf.tar_hand_temp * 1.0f / TEMP_RATE), GUI_LANG_GetTextEx(Lang_oC, Language_Bit));
    }

    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT13), (const char *) show_buf);


    if (cal_inf.SEN_EN[SENSOR_TYPE_SPEED] != STA_ON && basic_inf.humtemp_en != STA_ON) {
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT14));
    } else {
        WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT14));
    }

    if (basic_inf.humtemp_en == STA_ON && basic_inf.wind_area == 0) {
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT14));
    }

    sprintf(show_buf, "%s:%.2fm/s", GUI_LANG_GetTextEx(Lang_Wind, Language_Bit), cal_inf.wind_speed);
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT14), (const char *)show_buf);

    memset(show_buf, 0, sizeof(show_buf));

    for (j = 0, num = 0; j < ALA_MAX; j++) {
        if ((ALA_State() >> j) & 0x01 == 1) {
            if (j >= 2 && j <= 9) {
                sprintf(show_buf, "%s%d#%s;", show_buf, (j - 1), GUI_LANG_GetTextEx(alarm_show[j], Language_Bit));
            } else if (j >= 33 && j <= 40) {
                sprintf(show_buf, "%s%d#%s;", show_buf, (j - 32), GUI_LANG_GetTextEx(alarm_show[j], Language_Bit));
            } else {
                sprintf(show_buf, "%s%s;", show_buf, GUI_LANG_GetTextEx(alarm_show[j], Language_Bit));
            }

            num++;
        }

        if (num >= ALA_SHOW_NUM || (j == (ALA_MAX - 1) && num < ALA_SHOW_NUM)) {
            TEXT_SetTextColor(WM_GetDialogItem(hWin, GUI_ID_TEXT9), GUI_RED);
            TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT9), (const char *)show_buf);
            break;
        }
    }

    if (num == 0) {
        TEXT_SetTextColor(WM_GetDialogItem(hWin, GUI_ID_TEXT9), GUI_GREEN);

        TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT9), GUI_LANG_GetTextEx(Lang_ECUnormal, Language_Bit));
    }

    time = Get_Trial_ReHour();

    if (s_tCodeInf.wDay != 0 && time < TRIAL_SHOW_DAY * DAY_TO_HOUR) {
        if (show_sec >= 3) {
            TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT1), "");
        } else {
            if (time == 0) {
                sprintf(show_buf, GUI_LANG_GetTextEx(Lang_TrialEnds, Language_Bit));
            } else {
                sprintf(show_buf, GUI_LANG_GetTextEx(Lang_DAYTrialEnds, Language_Bit), (time / DAY_TO_HOUR + 1));
            }

            TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT1), show_buf);
        }
    } else if (ctrl_inf.var.mode == STA_ON) {
        TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT1), GUI_LANG_GetTextEx(Lang_Culture, Language_Bit));
    } else {
        if (show_sec >= 3) {
            TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT1), "");
        } else {
            TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT1), GUI_LANG_GetTextEx(Lang_Empty, Language_Bit));
        }
    }

    for (i = 0; i < HEAT_NUM_MAX; i++) {
        if (basic_inf.heat_en[i] != STA_ON) {
            WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON10 + i));
        } else {
            WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON10 + i));
        }
    }

    sprintf(show_buf, "%s: %.1f%s~%.1f%s", GUI_LANG_GetTextEx(Lang_AlaT, Language_Bit),
            (cal_inf.ala_Ltemp * 1.0f / TEMP_RATE), GUI_LANG_GetTextEx(Lang_oC, Language_Bit),
            (cal_inf.ala_Htemp * 1.0f / TEMP_RATE), GUI_LANG_GetTextEx(Lang_oC, Language_Bit));

    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT21), (const char *)show_buf);

    fan_state = (fan_state + 1) % 2;
    WM_InvalidateWindow(hWin);

    num = 0;

    if (cal_inf.SEN_EN[SENSOR_TYPE_HUM] == STA_ON) {
        num ++;
        sen_sta[0] = STA_ON;
    } else {
        sen_sta[0] = STA_OFF;
    }

    if (cal_inf.SEN_EN[SENSOR_TYPE_VAC] == STA_ON) {
        num ++;
        sen_sta[1] = STA_ON;
    } else {
        sen_sta[1] = STA_OFF;
    }

    if (cal_inf.SEN_EN[SENSOR_TYPE_CO2] == STA_ON) {
        num ++;
        sen_sta[2] = STA_ON;
    } else {
        sen_sta[2] = STA_OFF;
    }

    sen_num = num;

    for (i = 0; i < 3; i++) {
        if (i < num) {
            WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON1 + i));
        } else {
            WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON1 + i));
        }
    }

    if (sys_sta.trial_end == STA_ON) {
        TEXT_SetTextColor(WM_GetDialogItem(hWin, GUI_ID_TEXT9), GUI_RED);
        TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT9), GUI_LANG_GetTextEx(Lang_Expire, Language_Bit));
    }

    if (ext_inf.din.pro_data[0].water_sta == WL_DIS) {
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT16));
    } else {
        WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT16));
        all_tatol = 0;

        for (num = 0; num < PRO_DATA_MAX; num++) {
            if (ext_inf.din.pro_data[num].water_sta == 0)
                continue;

            //当前
            if (ext_inf.din.pro_data[0].water_sta == WL_CJT_188 || ext_inf.din.pro_data[0].water_sta == WL_MB_OWN) {
                if (sta_in.water[num] != (unsigned int)DATA_ER_VALUE) {
                    cal_tatol = sta_in.water[num] - index_inf.water[num][PRO_DATA_ONE_AGO];
                } else {
                    cal_tatol = 0;
                }
            } else {
                cal_tatol = index_inf.water[num][PRO_DATA_ALL] - index_inf.water[num][PRO_DATA_ONE_AGO];
            }

            if (cal_tatol >= 0) {
                all_tatol += cal_tatol;
            }
        }

        if (basic_inf.survival != 0) {
            sprintf(show_buf, "%s:%d/%.3fL", GUI_LANG_GetTextEx(Lang_WaterDaily, Language_Bit), all_tatol, all_tatol * 1.0f / basic_inf.survival);
        } else {
            sprintf(show_buf, "%s:%dL", GUI_LANG_GetTextEx(Lang_WaterDaily, Language_Bit), all_tatol);
        }

        TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT16), show_buf);
    }


    if (ext_inf.din.pro_data[0].food_sta == FL_DIS) {
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT17));
    } else {
        WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT17));
        all_tatol = 0;

        for (num = 0; num < PRO_DATA_MAX; num++) {
            if (ext_inf.din.pro_data[num].food_sta == 0)
                continue;

            cal_tatol = index_inf.food[num][PRO_DATA_ALL] - index_inf.food[num][PRO_DATA_ONE_AGO];

            if (cal_tatol >= 0) {
                all_tatol += cal_tatol;
            }

        }

        if (basic_inf.survival != 0) {
            sprintf(show_buf, "%s:%d/%.3f Kg", GUI_LANG_GetTextEx(Lang_FeedDaily, Language_Bit), (all_tatol / FOOD_RATE), (all_tatol * 1.0f / FOOD_RATE / basic_inf.survival));
        } else {
            sprintf(show_buf, "%s:%d Kg", GUI_LANG_GetTextEx(Lang_FeedDaily, Language_Bit), (all_tatol / FOOD_RATE));
        }

        TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT17), show_buf);
    }

    if (ext_inf.din.egg_en[0] == STA_OFF) {
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT20));
    } else {
        WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT20));
        all_tatol = 0;

//        for (num = 0; num < PRO_DATA_MAX; num++) {
//            if (ext_inf.din.pro_data[num].food_sta == 0)
//                continue;
        for (num = 0; num < 1; num++) {
            cal_tatol = index_inf.egg[num][PRO_DATA_ALL] - index_inf.egg[num][PRO_DATA_ONE_AGO];

            if (cal_tatol >= 0) {
                all_tatol += cal_tatol;
            }
        }

        sprintf(show_buf, "%s:%d", GUI_LANG_GetTextEx(Lang_EggDaily, Language_Bit), all_tatol);
        TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT20), show_buf);
    }
}


static void InitDialog(WM_MESSAGE * pMsg)
{
    WM_HWIN hWin = pMsg->hWin;
    int MDev_ID = 0, i = 0;
    WM_HWIN hItem;

    for (MDev_ID = GUI_ID_TEXT0; MDev_ID <= GUI_ID_TEXT9; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        TEXT_SetTextAlign(hItem, GUI_TA_VCENTER | GUI_TA_LEFT);
        TEXT_SetFont(hItem, &GUI_USER_HZ16);

        if (Language_Bit_original >= LANGUAGE_RUSSIAN) {
            TEXT_SetFont(hItem, &GUI_FontBC_16);
        }

        TEXT_SetTextColor(hItem, GUI_WHITE);

        if (MDev_ID == GUI_ID_TEXT4) {
            TEXT_SetFont(hItem, &GUI_USER_HZ24);

            if (Language_Bit_original >= LANGUAGE_RUSSIAN) {
                TEXT_SetFont(hItem, &GUI_FontBC_16);
            }

            TEXT_SetTextAlign(hItem, GUI_TA_VCENTER | GUI_TA_CENTER);
        }

        WM_DisableWindow(hItem);
    }

    for (MDev_ID = GUI_ID_TEXT10; MDev_ID <= GUI_ID_TEXT12; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        TEXT_SetTextAlign(hItem, GUI_TA_VCENTER | GUI_TA_HCENTER);
        TEXT_SetFont(hItem, &GUI_USER_HZ16);

        if (Language_Bit_original >= LANGUAGE_RUSSIAN) {
            TEXT_SetFont(hItem, &GUI_FontBC_16);
        }

        TEXT_SetTextColor(hItem, 0x00db9612);
        WM_DisableWindow(hItem);
    }

    for (MDev_ID = GUI_ID_TEXT13; MDev_ID <= GUI_ID_TEXT21; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        TEXT_SetTextAlign(hItem, GUI_TA_VCENTER | GUI_TA_LEFT);
        TEXT_SetFont(hItem, &GUI_USER_HZ16);

        if (Language_Bit_original >= LANGUAGE_RUSSIAN) {
            TEXT_SetFont(hItem, &GUI_FontBC_16);
        }

        TEXT_SetTextColor(hItem, GUI_WHITE);
        WM_DisableWindow(hItem);
    }

//    for (MDev_ID = GUI_ID_TEXT16; MDev_ID <= GUI_ID_TEXT21; MDev_ID++) {
//        hItem = WM_GetDialogItem(hWin, MDev_ID);
//        TEXT_SetTextAlign(hItem, GUI_TA_VCENTER | GUI_TA_LEFT);
//        TEXT_SetFont(hItem, &GUI_USER_HZ16);
//        TEXT_SetTextColor(hItem, GUI_WHITE);
//    }

    WM_EnableWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT3));
    WM_EnableWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT4));
    WM_EnableWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT9));

    TEXT_SetTextAlign(WM_GetDialogItem(hWin, GUI_ID_TEXT1), GUI_TA_VCENTER | GUI_TA_HCENTER);
    TEXT_SetTextAlign(WM_GetDialogItem(hWin, GUI_ID_TEXT2), GUI_TA_VCENTER | GUI_TA_HCENTER);
    TEXT_SetFont(WM_GetDialogItem(hWin, GUI_ID_TEXT1), &GUI_USER_HZ24);
    TEXT_SetFont(WM_GetDialogItem(hWin, GUI_ID_TEXT2), &GUI_USER_HZ16);


    TEXT_SetTextAlign(WM_GetDialogItem(hWin, GUI_ID_TEXT9), GUI_TA_VCENTER | GUI_TA_LEFT);
    TEXT_SetFont(WM_GetDialogItem(hWin, GUI_ID_TEXT9), &GUI_USER_HZ24);
    TEXT_SetTextColor(WM_GetDialogItem(hWin, GUI_ID_TEXT9), GUI_RED);

    if (Language_Bit_original >= LANGUAGE_RUSSIAN) {
        TEXT_SetFont(WM_GetDialogItem(hWin, GUI_ID_TEXT1), &GUI_FontBC_16);
        TEXT_SetFont(WM_GetDialogItem(hWin, GUI_ID_TEXT2), &GUI_FontBC_16);
        TEXT_SetFont(WM_GetDialogItem(hWin, GUI_ID_TEXT9), &GUI_FontBC_16);
    }


    Show_Data(hWin);
    hTimer = WM_CreateTimer(WM_GetClientWindow(hWin), 0, 500, 0);

    WM_SetHasTrans(WM_GetDialogItem(hWin, GUI_ID_BUTTON0));
    WM_SetCallback(WM_GetDialogItem(hWin, GUI_ID_BUTTON0), _cbButton_Home);

    for (MDev_ID = GUI_ID_BUTTON1; MDev_ID <= GUI_ID_BUTTON3; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, _cbButton);
    }

    for (MDev_ID = GUI_ID_BUTTON5; MDev_ID <= GUI_ID_BUTTON5; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, _cbButton_Login);
    }

    for (MDev_ID = GUI_ID_BUTTON6; MDev_ID <= GUI_ID_BUTTON9; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, _cbButton_None);
    }

    for (MDev_ID = GUI_ID_BUTTON13; MDev_ID <= GUI_ID_BUTTON13; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, _cbButton_None);
    }

    for (MDev_ID = GUI_ID_BUTTON10; MDev_ID <= GUI_ID_BUTTON12; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, _cbButton_Heat);
    }

    for (i = 0; i < HEAT_NUM_MAX; i++) {
        if (basic_inf.heat_en[i] != STA_ON) {
            WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON10 + i));
        }
    }
}

static void Password_Get(void)
{
    unsigned char *time_buf = Get_Input_Text();
    unsigned char len = strlen((const char *)time_buf);
    int result = 0;

    if (strcmp((const char *)time_buf, "1258.0") == 0) {
        Trial_Clear(0);

        MWarning_Task(Current_HWIN, GUI_LANG_GetTextEx(Lang_Formal_Version, Language_Bit));

        return;
    }

    result = Trial_Key_Check(time_buf);

    if (result < 0) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_PWD_ER));
    } else if (result == 0) {

        MWarning_Task(Current_HWIN, GUI_LANG_GetTextEx(Lang_Formal_Version, Language_Bit));

    } else {

        sprintf(show_buf, GUI_LANG_GetTextEx(Lang_Trial_for_Day, Language_Bit), result);

        MWarning_Task(Current_HWIN, show_buf);
    }
}

void SetAlarmShield(void)
{
    run_inf.alarm_shield_state = ALA_State();
    run_inf.alarm_shield_time = ctrl_inf.var.shield_ala_time * MIN_TO_SEC;
}

/*********************************************************************
*
*       Dialog callback routine
*/
static void _cbCallback(WM_MESSAGE * pMsg)
{
    int NCode, Id, i, num, ret;
    WM_HWIN hWin = pMsg->hWin;

    switch (pMsg->MsgId) {
    case WM_PAINT:
        PaintDialog(pMsg);
        break;

    case WM_TIMER:
        Show_Data(hWin);
        WM_RestartTimer(hTimer, 500);
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

        if (sys_sta.lcd_state == 0) { //亮屏不操作
            break;
        }

        if (NCode == WM_NOTIFICATION_CLICKED) {
            beep_on();
        }

        if (sys_sta.trial_end == STA_ON) {
            if (Id == GUI_ID_TEXT9 && NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(Password_Get);
                Set_Input_Len(8);
                Input_Task(hWin);
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

        case GUI_ID_TEXT3:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                clock_in_data_save();
                MWarning_Task(Current_HWIN, Get_Tips(TIPS_CLOCK));
            }

            break;

        case GUI_ID_TEXT4:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                while (sys_sta.copy_bg_pic == STA_OFF)
                    OSTimeDly(100);

                MCtrl_TempCurve_Task(hWin);
            }

            break;

        case GUI_ID_TEXT9:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                while (sys_sta.copy_bg_pic == STA_OFF)
                    OSTimeDly(100);

                MAla_Record_Task(hWin);
            }

            break;


        case GUI_ID_BUTTON0:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                while (sys_sta.copy_bg_pic == STA_OFF)
                    OSTimeDly(100);

                Main_Task(hWin);
            }

            break;

        case GUI_ID_BUTTON5:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                while (sys_sta.copy_bg_pic == STA_OFF)
                    OSTimeDly(100);

                MUser_Man_Task(hWin);
            }

            break;

        case GUI_ID_BUTTON1:
        case GUI_ID_BUTTON2:
        case GUI_ID_BUTTON3:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                while (sys_sta.copy_bg_pic == STA_OFF)
                    OSTimeDly(100);

                if (sen_num < Id - GUI_ID_BUTTON1 + 1)
                    break;

                for (i = 0, num = 0; i < 3; i++) {
                    if (sen_sta[i] != STA_ON)
                        continue;

                    if (++num >= (Id - GUI_ID_BUTTON1 + 1))
                        break;
                }

                if (i == 0) {
                    Set_OTSensor_Type(SENCON_HUM);
                    MCtrl_OTSensor_Task(hWin);
                } else if (i == 1) {
                    MCtrl_VAC_Task(hWin);
                } else if (i == 2) {
                    Set_OTSensor_Type(SENCON_CO2);
                    MCtrl_OTSensor_Task(hWin);
                }
            }

            break;

        case GUI_ID_BUTTON6:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                while (sys_sta.copy_bg_pic == STA_OFF)
                    OSTimeDly(100);

                cal_list_type();
                MCtrl_LVView_Task(hWin, 0, 0);
            }

            break;

        case GUI_ID_BUTTON7:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                while (sys_sta.copy_bg_pic == STA_OFF)
                    OSTimeDly(100);

                MDev_Regulatingfan_Task(hWin, 0);
            }

            break;


        case GUI_ID_BUTTON8:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                while (sys_sta.copy_bg_pic == STA_OFF)
                    OSTimeDly(100);

                MDev_Wet_Task(hWin, 1, STA_OFF);
            }

            break;

        case GUI_ID_BUTTON9:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                while (sys_sta.copy_bg_pic == STA_OFF)
                    OSTimeDly(100);

                MDev_Wet_Task(hWin, 0, STA_OFF);
            }

            break;

        case GUI_ID_BUTTON10:
        case GUI_ID_BUTTON11:
        case GUI_ID_BUTTON12:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                while (sys_sta.copy_bg_pic == STA_OFF)
                    OSTimeDly(100);

                MDev_Heater_Task(hWin, (Id - GUI_ID_BUTTON10), STA_OFF);
            }

            break;

        case GUI_ID_BUTTON13:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                if (ALA_State() == 0) {
                    break;
                }

                ret = MMessage_Task(hWin, MESSAGE_ALARM_SHILED);

                if (ret == MESSAGE_YES) {
                    SetAlarmShield();
                }
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

BUTTON_SKINFLEX_PROPS BUTTON_Props_Set_EN;
BUTTON_SKINFLEX_PROPS BUTTON_Props_Set_PR;
SCROLLBAR_SKINFLEX_PROPS SCROLLBAR_Props_Def;
CHECKBOX_SKINFLEX_PROPS CHECKBOX_Props_Def;
DROPDOWN_SKINFLEX_PROPS DROPDOWN_Props_Def;


WM_HWIN HBasic_Main;
void Creat_Basic_Main(void)
{
    HBasic_Main = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, 0, 0, 0);
    Current_HWIN = HBasic_Main;
}

GUI_BITMAP bmMEnable;
GUI_BITMAP bmMDisable;
GUI_BITMAP bmMFan1;
GUI_BITMAP bmMFan2;
GUI_BITMAP bmMFan3;

void BK_Init(void)
{
    U8 i = 0;
    WM_MULTIBUF_Enable(1);

    //PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
    //FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
    //PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
    //SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
    //SLIDER_SetDefaultSkin(SLIDER_SKIN_FLEX);
    //HEADER_SetDefaultSkin(HEADER_SKIN_FLEX);
    //RADIO_SetDefaultSkin(RADIO_SKIN_FLEX);

    BUTTON_GetSkinFlexProps(&BUTTON_Props_Set_EN, BUTTON_SKINFLEX_PI_ENABLED);
    BUTTON_GetSkinFlexProps(&BUTTON_Props_Set_PR, BUTTON_SKINFLEX_PI_PRESSED);

    BUTTON_Props_Set_EN.aColorFrame[0] = GUI_DEF_BUTTON;
    BUTTON_Props_Set_EN.aColorFrame[1] = GUI_DEF_BUTTON;
    BUTTON_Props_Set_EN.aColorFrame[2] = GUI_DEF_BUTTON;
    BUTTON_Props_Set_EN.aColorLower[0] = GUI_DEF_BUTTON;
    BUTTON_Props_Set_EN.aColorLower[1] = GUI_DEF_BUTTON;
    BUTTON_Props_Set_EN.aColorUpper[0] = GUI_DEF_BUTTON;
    BUTTON_Props_Set_EN.aColorUpper[1] = GUI_DEF_BUTTON;

    BUTTON_Props_Set_PR.aColorFrame[0] = GUI_LIGHTBLUE;
    BUTTON_Props_Set_PR.aColorFrame[1] = GUI_LIGHTBLUE;
    BUTTON_Props_Set_PR.aColorFrame[2] = GUI_LIGHTBLUE;
    BUTTON_Props_Set_PR.aColorLower[0] = GUI_LIGHTBLUE;
    BUTTON_Props_Set_PR.aColorLower[1] = GUI_LIGHTBLUE;
    BUTTON_Props_Set_PR.aColorUpper[0] = GUI_LIGHTBLUE;
    BUTTON_Props_Set_PR.aColorUpper[1] = GUI_LIGHTBLUE;

    BUTTON_SetSkinFlexProps(&BUTTON_Props_Set_EN, BUTTON_SKINFLEX_PI_ENABLED);
    BUTTON_SetSkinFlexProps(&BUTTON_Props_Set_EN, BUTTON_SKINFLEX_PI_FOCUSSED);
    BUTTON_SetSkinFlexProps(&BUTTON_Props_Set_PR, BUTTON_SKINFLEX_PI_PRESSED);

    BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);


    CHECKBOX_GetSkinFlexProps(&CHECKBOX_Props_Def, CHECKBOX_SKINFLEX_PI_ENABLED);

    for (i = 0; i < 3; i++) {
        CHECKBOX_Props_Def.aColorFrame[i] = GUI_DEF_BUTTON;
    }

    for (i = 0; i < 2; i++) {
        CHECKBOX_Props_Def.aColorInner[i] = GUI_WHITE;
    }

    CHECKBOX_Props_Def.ColorCheck = GUI_DEF_BUTTON;
    CHECKBOX_Props_Def.ButtonSize = 18;

    CHECKBOX_SetSkinFlexProps(&CHECKBOX_Props_Def, CHECKBOX_SKINFLEX_PI_ENABLED);
    CHECKBOX_SetDefaultSkin(CHECKBOX_SKIN_FLEX);


    DROPDOWN_GetSkinFlexProps(&DROPDOWN_Props_Def, DROPDOWN_SKINFLEX_PI_ENABLED);

    for (i = 0; i < 3; i++) {
        DROPDOWN_Props_Def.aColorFrame[i] = GUI_DEF_BUTTON;
    }

    for (i = 0; i < 2; i++) {
        DROPDOWN_Props_Def.aColorUpper[i] = GUI_WHITE;
        DROPDOWN_Props_Def.aColorLower[i] = GUI_WHITE;
    }

    DROPDOWN_Props_Def.ColorArrow = GUI_DEF_BUTTON;
    DROPDOWN_Props_Def.ColorText = GUI_BLACK;
    DROPDOWN_Props_Def.ColorSep = GUI_DEF_BUTTON;
    DROPDOWN_Props_Def.Radius = 0;

    DROPDOWN_SetSkinFlexProps(&DROPDOWN_Props_Def, DROPDOWN_SKINFLEX_PI_EXPANDED);
    DROPDOWN_SetSkinFlexProps(&DROPDOWN_Props_Def, DROPDOWN_SKINFLEX_PI_FOCUSSED);
    DROPDOWN_SetSkinFlexProps(&DROPDOWN_Props_Def, DROPDOWN_SKINFLEX_PI_ENABLED);
    DROPDOWN_SetDefaultSkin(DROPDOWN_SKIN_FLEX);


    SCROLLBAR_GetSkinFlexProps(&SCROLLBAR_Props_Def, SCROLLBAR_SKINFLEX_PI_PRESSED);

    for (i = 0; i < 3; i++) {
        SCROLLBAR_Props_Def.aColorFrame[i] = GUI_DEF_BUTTON;
    }

    for (i = 0; i < 2; i++) {
        SCROLLBAR_Props_Def.aColorUpper[i] = GUI_DEF_LGRAY;
        SCROLLBAR_Props_Def.aColorLower[i] = GUI_DEF_LGRAY;
        SCROLLBAR_Props_Def.aColorShaft[i] = GUI_DEF_LGRAY;
    }

    SCROLLBAR_Props_Def.ColorArrow = GUI_DEF_LGRAY;
    SCROLLBAR_Props_Def.ColorGrasp = GUI_DEF_BUTTON;

    SCROLLBAR_SetSkinFlexProps(&SCROLLBAR_Props_Def, SCROLLBAR_SKINFLEX_PI_PRESSED);
    SCROLLBAR_SetSkinFlexProps(&SCROLLBAR_Props_Def, SCROLLBAR_SKINFLEX_PI_UNPRESSED);
    SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);

    GUI_defCreateBitmapFromStream("38bmMPic_Dis", &bmMDisable);
    GUI_defCreateBitmapFromStream("39bmMPic_En", &bmMEnable);
    GUI_defCreateBitmapFromStream("40bmMPic_Fan1", &bmMFan1);
    GUI_defCreateBitmapFromStream("41bmMPic_Fan2", &bmMFan2);
    GUI_defCreateBitmapFromStream("42bmMPic_Fan3", &bmMFan3);
	
    GUI_UC_SetEncodeUTF8();
    GUI_UC_EnableBIDI(1); 
    GUI_LANG_LoadCSV((u8 *)(SDRAM_LANG_ADDR), MULTI_LANG_SIZE);
}

