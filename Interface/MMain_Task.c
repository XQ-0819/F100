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
#include "w25qxx.h"
#include "trial_ver.h"

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
#define GUI_ID_TEXT22   (GUI_ID_USER+13)
#define GUI_ID_TEXT23   (GUI_ID_USER+14)
#define GUI_ID_TEXT24   (GUI_ID_USER+15)
#define GUI_ID_TEXT25   (GUI_ID_USER+16)
#define GUI_ID_TEXT26   (GUI_ID_USER+17)
#define GUI_ID_TEXT27   (GUI_ID_USER+18)
#define GUI_ID_TEXT28   (GUI_ID_USER+19)
#define GUI_ID_TEXT29   (GUI_ID_USER+20)
#define GUI_ID_TEXT30   (GUI_ID_USER+21)
#define GUI_ID_TEXT31   (GUI_ID_USER+22)
#define GUI_ID_TEXT32   (GUI_ID_USER+23)
#define GUI_ID_TEXT33   (GUI_ID_USER+24)
#define GUI_ID_TEXT34   (GUI_ID_USER+25)
#define GUI_ID_TEXT35   (GUI_ID_USER+26)
#define GUI_ID_TEXT36   (GUI_ID_USER+27)
#define GUI_ID_TEXT37   (GUI_ID_USER+28)
#define GUI_ID_TEXT38   (GUI_ID_USER+29)
#define GUI_ID_TEXT39   (GUI_ID_USER+30)
#define GUI_ID_TEXT40   (GUI_ID_USER+31)
#define GUI_ID_TEXT41   (GUI_ID_USER+32)
#define GUI_ID_TEXT42   (GUI_ID_USER+33)
#define GUI_ID_TEXT43   (GUI_ID_USER+34)
#define GUI_ID_TEXT44   (GUI_ID_USER+35)
#define GUI_ID_TEXT45   (GUI_ID_USER+36)
#define GUI_ID_TEXT46   (GUI_ID_USER+37)
#define GUI_ID_TEXT47   (GUI_ID_USER+38)


//EventsFunctionList
//EndofEventsFunctionList


#define GUI_DEF_RED			0x00553BDB
#define GUI_DEF_BLUE		0x00917927
#define GUI_DEF_GREEN		0x0020C38F
#define GUI_DEF_BLACK 		0x00717171


GUI_CONST_STORAGE GUI_BITMAP hMainpic = {
    800, 	// xSize
    480, 	// ySize
    1600,	// BytesPerLine
    16, 	// BitsPerPixel
    (const unsigned char *)(SDRAM_PIC_ADDR + (WMAIN_ID - 1) * PIC_SIZE),
    NULL, // Pointer to palette
    GUI_DRAW_BMPM565
};

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
extern GUI_CONST_STORAGE GUI_FONT GUI_GUI_FontYH_36;
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect,    "",               0,                       0,  0,  800, 480, 0, 0},
    { TEXT_CreateIndirect,      "",      	  GUI_ID_TEXT0,            582, 5, 240, 42, 0, 0},

    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON1_0,          384, 398, 90, 82, 0, 0},
    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON1_1,          25, 400, 105, 80, 0, 0},
    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON1_2,          155, 400, 105, 80, 0, 0},
    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON1_3,          275, 400, 105, 80, 0, 0},
    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON1_4,          435, 400, 105, 80, 0, 0},
    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON1_5,          555, 400, 105, 80, 0, 0},
    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON1_6,          675, 400, 125, 80, 0, 0},

    { TEXT_CreateIndirect,      "",           GUI_ID_TEXT1,            110, 45, 150, 32, 0, 0},    //room
    { TEXT_CreateIndirect,      "",     	  GUI_ID_TEXT2,            280, 45, 150, 32, 0, 0},//day
    { TEXT_CreateIndirect,      "",     	  GUI_ID_TEXT3,            455, 45, 150, 32, 0, 0},//ttemp
    { TEXT_CreateIndirect,      "",           GUI_ID_TEXT4,            670, 45, 150, 32, 0, 0},//lv

    { TEXT_CreateIndirect,      "",     	  GUI_ID_TEXT5,        82, 140, 64, 20, 0, 0},//temp
    { TEXT_CreateIndirect,      "",           GUI_ID_TEXT6,        81, 225, 62, 62, 0, 0},//hum

    { TEXT_CreateIndirect,      "",     	  GUI_ID_TEXT7,      185, 90, 300, 62, 0, 0},//out_temp
    { TEXT_CreateIndirect,      "",           GUI_ID_TEXT8,      340, 112, 180, 20, 0, 0},//pm2.5

    { TEXT_CreateIndirect,      "",           GUI_ID_TEXT9,      50, 5, 275, 42, 0, 0},//倒计时

    { TEXT_CreateIndirect,      "",        GUI_ID_TEXT38,        185, 155, 180, 20, 0, 0},//ai1
    { TEXT_CreateIndirect,      "",        GUI_ID_TEXT40,        185, 200, 180, 20, 0, 0},//ai3
    { TEXT_CreateIndirect,      "",        GUI_ID_TEXT42,        185, 245, 180, 20, 0, 0},//ai5

    { TEXT_CreateIndirect,      "",        GUI_ID_TEXT39,        340, 155, 180, 20, 0, 0},//ai2
    { TEXT_CreateIndirect,      "",        GUI_ID_TEXT41,        340, 200, 180, 20, 0, 0},//ai4
    { TEXT_CreateIndirect,      "",        GUI_ID_TEXT43,        340, 245, 180, 20, 0, 0},//ai6

    { TEXT_CreateIndirect,      "",        GUI_ID_TEXT12,        600, 112, 150, 20, 0, 0},//temp1
    { TEXT_CreateIndirect,      "",        GUI_ID_TEXT13,        600, 137, 150, 20, 0, 0},//temp2
    { TEXT_CreateIndirect,      "",        GUI_ID_TEXT14,        600, 175, 100, 20, 0, 0},//temp3
    { TEXT_CreateIndirect,      "",        GUI_ID_TEXT15,        600, 199, 100, 20, 0, 0},//temp4
    { TEXT_CreateIndirect,      "",        GUI_ID_TEXT16,        600, 237, 100, 20, 0, 0},//temp5
    { TEXT_CreateIndirect,      "",        GUI_ID_TEXT17,        600, 262, 100, 20, 0, 0},//temp6

    { TEXT_CreateIndirect,      "",     	  GUI_ID_TEXT18,        540, 372, 100, 20, 0, 0},//twin
    { TEXT_CreateIndirect,      "",     	  GUI_ID_TEXT19,        665, 372, 100, 20, 0, 0},//twin

    { TEXT_CreateIndirect,      "",     	  GUI_ID_TEXT20,        30, 404, 130, 20, 0, 0},//pwm1
    { TEXT_CreateIndirect,      "",     	  GUI_ID_TEXT21,        155, 404, 130, 20, 0, 0},//pwm2
    { TEXT_CreateIndirect,      "",     	  GUI_ID_TEXT22,        280, 404, 130, 20, 0, 0},//pwm3
    { TEXT_CreateIndirect,      "",     	  GUI_ID_TEXT23,        405, 404, 130, 20, 0, 0},//pwm4
    { TEXT_CreateIndirect,      "",     	  GUI_ID_TEXT24,        530, 404, 130, 20, 0, 0},//tswin
    { TEXT_CreateIndirect,      "",     	  GUI_ID_TEXT25,        655, 404, 130, 20, 0, 0},//tlight

    { TEXT_CreateIndirect,      "",     	  GUI_ID_TEXT26,        665, 120, 80, 24, 0, 0},//heatA
    { TEXT_CreateIndirect,      "",     	  GUI_ID_TEXT27,        665, 180, 80, 24, 0, 0},//heatB
    { TEXT_CreateIndirect,      "",     	  GUI_ID_TEXT28,        665, 242, 80, 24, 0, 0},//heatC

//    { TEXT_CreateIndirect,      "",     	  GUI_ID_TEXT29,        290, 340, 80, 24, 0, 0},//
//    { TEXT_CreateIndirect,      "",     	  GUI_ID_TEXT30,        415, 340, 80, 24, 0, 0},//
//    { TEXT_CreateIndirect,      "",     	  GUI_ID_TEXT31,        540, 340, 80, 24, 0, 0},//
//    { TEXT_CreateIndirect,      "",     	  GUI_ID_TEXT32,        665, 340, 80, 24, 0, 0},//

//    { TEXT_CreateIndirect,      "",     	  GUI_ID_TEXT33,        40, 372, 80, 24, 0, 0},//
//    { TEXT_CreateIndirect,      "",     	  GUI_ID_TEXT34,        165, 372, 80, 24, 0, 0},//
//    { TEXT_CreateIndirect,      "",     	  GUI_ID_TEXT35,        290, 372, 80, 24, 0, 0},//
//    { TEXT_CreateIndirect,      "",     	  GUI_ID_TEXT36,        415, 372, 80, 24, 0, 0},//


//    { TEXT_CreateIndirect,      "",      GUI_ID_TEXT44,        540, 310, 100, 16, 0, 0},//
//    { TEXT_CreateIndirect,      "",      GUI_ID_TEXT45,        665, 310, 80, 16, 0, 0},//
//    { TEXT_CreateIndirect,      "",      GUI_ID_TEXT46,        40,  343, 100, 16, 0, 0},//
//    { TEXT_CreateIndirect,      "",      GUI_ID_TEXT47,        165, 343, 80, 16, 0, 0},//
};

const u16 Lang_timer_type[4] = {Lang_Timer, Lang_Light, Lang_Water, Lang_Feed};

static WM_HTIMER hTimer;
static const char *MTIMER[] = { "00bmMPic_Timer1", "01bmMPic_Timer2", "02bmMPic_Timer_Dis"};
const char *MWET[] = { "29bmMPic_Wet1", "30bmMPic_Wet2", "31bmMPic_Wet_Dis"};
const char *MHEAT[] = { "26bmMPic_Heat1", "27bmMPic_Heat2", "28bmMPic_Heat_Dis"};
static const char *MFAN[] = { "03bmMPic_Fan1", "04bmMPic_Fan2", "05bmMPic_Fan_Dis"};
static U8 fan_state = 0;
#define DIS_PIC		(2)
/*****************************************************************
**      FunctionName:void PaintDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**
**      call this function in _cbCallback --> WM_PAINT
*****************************************************************/
static void Show_Dis_Pic(int x0, int y0)
{
    GUI_SetFont(&GUI_USER_HZ16);
	
	if(Language_Bit_original >= LANGUAGE_RUSSIAN) {
		GUI_SetFont(&GUI_FontBC_16);	
	}
	
    GUI_SetColor(GUI_GRAY_D0);
    GUI_SetTextMode(GUI_TEXTMODE_TRANS);
    GUI_DispStringAt("--", x0, y0 + 4);
}

#define FAN_SHOW_NUM		(19)
#define FAN_SHOW_X			(100)
#define FAN_SHOW_XSIZE		(125)
#define FAN_SHOW_Y			(302)
#define FAN_SHOW_YSIZE		(33)
#define FAN_SHOW_XDIFF		(-70)
#define FAN_SHOW_YDIFF		(3)


#define ARABIC_TEXT "\xd8\xa5\xd9\x86\xd9\x87\x20\xd9\x8a\xd8\xb9\xd9\x85\xd9\x84\x20\xd8\xa8\xd8\xa8\xd8\xb3\xd8\xa7\xd8\xb7\xd8\xa9\x21"
static void PaintDialog(WM_MESSAGE * pMsg)
{
    int i = 0, j = 0, x = 0, y = 0, show_id = 0 , yoffset = 0;
//    WM_HWIN hWin = pMsg->hWin;
    GUI_DrawBitmap(&hMainpic, 0, 0);
    //GUI_defDrawBitmap("25bmMPic_HouseR", 21, 427);

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

    if (ALA_Get_State(ALA_HTEMP) == STA_ON) {
        GUI_defDrawBitmap("48bmMPic_Htemp_HIGH", 67, 86);
    } else if (ALA_Get_State(ALA_LTEMP) == STA_ON) {
        GUI_defDrawBitmap("47bmMPic_Htemp_LOW", 67, 86);
    } else {
        GUI_defDrawBitmap("46bmMPic_Htemp_NOL", 67, 86);
    }

    if (ALA_Get_State(ALA_HHUM) == STA_ON) {
        GUI_defDrawBitmap("90MPic_HumHigh", 66, 194);
    } else if (ALA_Get_State(ALA_LHUM) == STA_ON) {
        GUI_defDrawBitmap("88MPic_HumLow", 66, 194);
    } else {
        GUI_defDrawBitmap("89MPic_HumNol", 66, 194);
    }

    GUI_SetFont(&GUI_USER_HZ16);
    GUI_SetColor(GUI_WHITE);
    GUI_SetTextMode(GUI_TEXTMODE_TRANS);

    if (Language_Bit_original >= LANGUAGE_RUSSIAN) {
        GUI_SetFont(&GUI_FontBC_16);
    //     sprintf(show_buf, "%s:", GUI_LANG_GetTextEx(Lang_Day, Language_Bit)); // 日龄
    //     GUI_DispStringAt((const char *)show_buf, 280, 46);
    //     sprintf(show_buf, "%s:", GUI_LANG_GetTextEx(Lang_TarT, Language_Bit));
    //     GUI_DispStringAt((const char *)show_buf, 455, 46);
    //     sprintf(show_buf, "%s:", GUI_LANG_GetTextEx(Lang_Level, Language_Bit));
    //     GUI_DispStringAt((const char *)show_buf, 670, 46);
        for (i = 0; i < 6; i++) {
            sprintf(show_buf, "%s%d", GUI_LANG_GetTextEx(Lang_Temp, Language_Bit), i + 1);
            GUI_DispStringAt((const char *)show_buf, 540, 105 + (25 * i) + (12 * (i / 2)));
        }
        yoffset = -5;
    }else{
    //     sprintf(show_buf, "%s:", GUI_LANG_GetTextEx(Lang_Day, Language_Bit)); // 日龄
    //     GUI_DispStringAt((const char *)show_buf, 280, 51);
    //     sprintf(show_buf, "%s:", GUI_LANG_GetTextEx(Lang_TarT, Language_Bit));
    //     GUI_DispStringAt((const char *)show_buf, 455, 51);
    //     sprintf(show_buf, "%s:", GUI_LANG_GetTextEx(Lang_Level, Language_Bit));
    //     GUI_DispStringAt((const char *)show_buf, 670, 51);
        for (i = 0; i < 6; i++) {
            sprintf(show_buf, "%s%d", GUI_LANG_GetTextEx(Lang_Temp, Language_Bit), i + 1);
            GUI_DispStringAt((const char *)show_buf, 540, 112 + (25 * i) + (12 * (i / 2)));
        }
        yoffset = 0;
    }

    i = 0;

    for (j = 0; i < FAN_SHOW_NUM;) {//风机
        if (i == FAN_SHOW_NUM - 1) {
            x = FAN_SHOW_X + FAN_SHOW_XSIZE * 5;
            y = FAN_SHOW_Y - FAN_SHOW_YSIZE;
        } else {
            x = FAN_SHOW_X + FAN_SHOW_XSIZE * (i % 6);
            y = FAN_SHOW_Y + FAN_SHOW_YSIZE * (i / 6);
        }

        for (; j < JDQ_NUM_MAX; j++) {
            if (dev_inf.JDQ[JDQ_FAN1 + j].enable != STA_ON) { //禁止
                continue;
            }

            if (sta_out.ALLJDQ[JDQ_FAN1 + j] == STA_ON) {
                GUI_defDrawBitmap(MFAN[fan_state], x, y);
            } else {
                GUI_defDrawBitmap(MFAN[DIS_PIC], x, y);
            }

            if (ctrl_inf.lv_more.fan_auto_sort == STA_ON) {
                show_id = cal_inf.jdq_fan_showid[j];
            } else {
                show_id = j;
            }

            sprintf(show_buf, "%s%2d", GUI_LANG_GetTextEx(Lang_Fan, Language_Bit), show_id + 1);
            GUI_DispStringAt(show_buf, x + FAN_SHOW_XDIFF, y + FAN_SHOW_YDIFF + yoffset);

            i++;
            j++;
            break;
        }

        if (j >= JDQ_NUM_MAX) {
            break;//没找到退出
        }
    }

    for (j = 0; i < FAN_SHOW_NUM;) {//湿帘
        if (i == FAN_SHOW_NUM - 1) {
            x = FAN_SHOW_X + FAN_SHOW_XSIZE * 5;
            y = FAN_SHOW_Y - FAN_SHOW_YSIZE;
        } else {
            x = FAN_SHOW_X + FAN_SHOW_XSIZE * (i % 6);
            y = FAN_SHOW_Y + FAN_SHOW_YSIZE * (i / 6);
        }

        for (; j < WET_NUM_MAX; j++) {
            if (dev_inf.wet_en[j] != STA_ON) { //禁止
                continue;
            }

            if (sta_out.ALLJDQ[JDQ_WET + j] == STA_ON) {
                GUI_defDrawBitmap(MWET[fan_state], x, y);
            } else {
                GUI_defDrawBitmap(MWET[DIS_PIC], x, y);
            }

            show_id = j;
            sprintf(show_buf, "%s%2d", GUI_LANG_GetTextEx(Lang_Wet_Curtain, Language_Bit), show_id + 1);
            GUI_DispStringAt(show_buf, x + FAN_SHOW_XDIFF, y + FAN_SHOW_YDIFF + yoffset);

            i++;
            j++;
            break;
        }

        if (j >= WET_NUM_MAX) {
            break;//没找到退出
        }
    }


    for (j = 0; i < FAN_SHOW_NUM;) {//喷淋
        if (i == FAN_SHOW_NUM - 1) {
            x = FAN_SHOW_X + FAN_SHOW_XSIZE * 5;
            y = FAN_SHOW_Y - FAN_SHOW_YSIZE;
        } else {
            x = FAN_SHOW_X + FAN_SHOW_XSIZE * (i % 6);
            y = FAN_SHOW_Y + FAN_SHOW_YSIZE * (i / 6);
        }

        for (; j < 1; j++) {
            if (dev_inf.spray_en == SPRAY_MODE_OFF) { //禁止
                continue;
            }

            if (sta_out.ALLJDQ[JDQ_SPRAY + j] == STA_ON) {
                GUI_defDrawBitmap(MWET[fan_state], x, y);
            } else {
                GUI_defDrawBitmap(MWET[DIS_PIC], x, y);
            }

            sprintf(show_buf, "%s", GUI_LANG_GetTextEx(Lang_Spray, Language_Bit));
            GUI_DispStringAt(show_buf, x + FAN_SHOW_XDIFF, y + FAN_SHOW_YDIFF + yoffset);

            i++;
            j++;
            break;
        }

        if (j >= 1) {
            break;//没找到退出
        }
    }

    for (j = 0; i < FAN_SHOW_NUM;) {//环流风机
        if (i == FAN_SHOW_NUM - 1) {
            x = FAN_SHOW_X + FAN_SHOW_XSIZE * 5;
            y = FAN_SHOW_Y - FAN_SHOW_YSIZE;
        } else {
            x = FAN_SHOW_X + FAN_SHOW_XSIZE * (i % 6);
            y = FAN_SHOW_Y + FAN_SHOW_YSIZE * (i / 6);
        }

        for (; j < 1; j++) {
            if (dev_inf.circle.enable != STA_ON) { //禁止
                continue;
            }

            if (sta_out.ALLJDQ[JDQ_CIR_FAN + j] == STA_ON) {
                GUI_defDrawBitmap(MFAN[fan_state], x, y);
            } else {
                GUI_defDrawBitmap(MFAN[DIS_PIC], x, y);
            }

            sprintf(show_buf, "%s", GUI_LANG_GetTextEx(Lang_Circulating_Fan, Language_Bit));
            GUI_DispStringAt(show_buf, x + FAN_SHOW_XDIFF, y + FAN_SHOW_YDIFF + yoffset);

            i++;
            j++;
            break;
        }

        if (j >= 1) {
            break;//没找到退出
        }
    }

    for (j = 0; i < FAN_SHOW_NUM;) {//定时器
        if (i == FAN_SHOW_NUM - 1) {
            x = FAN_SHOW_X + FAN_SHOW_XSIZE * 5;
            y = FAN_SHOW_Y - FAN_SHOW_YSIZE;
        } else {
            x = FAN_SHOW_X + FAN_SHOW_XSIZE * (i % 6);
            y = FAN_SHOW_Y + FAN_SHOW_YSIZE * (i / 6);
        }

        for (; j < TR_TYPE_NUM; j++) {
            if (ext_inf.timer[TR_TYPE_TIMER1 + j].enable != STA_ON) { //禁止
                continue;
            }

            if (sta_out.ALLJDQ[JDQ_TIMER1 + j] == STA_ON) {
                GUI_defDrawBitmap(MTIMER[fan_state], x, y);
            } else {
                GUI_defDrawBitmap(MTIMER[DIS_PIC], x, y);
            }

            show_id = j;

            sprintf(show_buf, "%s%d", GUI_LANG_GetTextEx(Lang_timer_type[ext_inf.timer[TR_TYPE_TIMER1 + j].type], Language_Bit), show_id + 1);
            GUI_DispStringAt(show_buf, x + FAN_SHOW_XDIFF, y + FAN_SHOW_YDIFF + yoffset);

            i++;
            j++;
            break;
        }

        if (j >= TR_TYPE_NUM) {
            break;//没找到退出
        }
    }

    for (j = 0; i < FAN_SHOW_NUM;) {//温控开关
        if (i == FAN_SHOW_NUM - 1) {
            x = FAN_SHOW_X + FAN_SHOW_XSIZE * 5;
            y = FAN_SHOW_Y - FAN_SHOW_YSIZE;
        } else {
            x = FAN_SHOW_X + FAN_SHOW_XSIZE * (i % 6);
            y = FAN_SHOW_Y + FAN_SHOW_YSIZE * (i / 6);
        }

        for (; j < 1; j++) {
            if (basic_inf.tempc_en.enable != STA_ON) { //禁止
                continue;
            }

            if (sta_out.ALLJDQ[JDQ_TEMPC + j] == STA_ON) {
                GUI_defDrawBitmap(MFAN[fan_state], x, y);
            } else {
                GUI_defDrawBitmap(MFAN[DIS_PIC], x, y);
            }

            sprintf(show_buf, "%s", GUI_LANG_GetTextEx(Lang_Tswitch, Language_Bit));
            GUI_DispStringAt(show_buf, x + FAN_SHOW_XDIFF, y + FAN_SHOW_YDIFF + yoffset);

            i++;
            j++;
            break;
        }

        if (j >= 1) {
            break;//没找到退出
        }
    }


    for (j = 0; i < FAN_SHOW_NUM - 1;) {//风窗
        x = FAN_SHOW_X + FAN_SHOW_XSIZE * (i % 6);
        y = FAN_SHOW_Y + FAN_SHOW_YSIZE * (i / 6);


        for (; j < WINDOW_NUM_MAX; j++) {
            if (dev_inf.WIN.detail[WIN1_ID + j].enable != STA_ON) { //禁止
                continue;
            }

            if (ctrl_inf.lv_more.fan_auto_sort == STA_ON) {
                show_id = cal_inf.win_showid[j];
            } else {
                show_id = j;
            }

            if (sta_out.WIN[WIN1_ID + j] == WIN_DEF_VA || dev_inf.WIN.detail[WIN1_ID + j].ctrl_mode == WIN_ONLY_VAC_CTRL) {
                sprintf(show_buf, "%s%d:--%%", GUI_LANG_GetTextEx(Lang_CurtWinDow, Language_Bit) ,show_id + 1);
            } else {
                sprintf(show_buf, "%s%d:%d%%", GUI_LANG_GetTextEx(Lang_CurtWinDow, Language_Bit) ,show_id + 1, sta_out.WIN[WIN1_ID + j]);
            }

            GUI_DispStringAt(show_buf, x + FAN_SHOW_XDIFF,   y + FAN_SHOW_YDIFF + yoffset);

            i++;
            j++;
            break;
        }

        if (j >= WINDOW_NUM_MAX) {
            break;//没找到退出
        }
    }

    for (; i < FAN_SHOW_NUM - 1; i++) { //剩余的显示风机和--
        x = FAN_SHOW_X + FAN_SHOW_XSIZE * (i % 6);
        y = FAN_SHOW_Y + FAN_SHOW_YSIZE * (i / 6);

        Show_Dis_Pic( x + 2, y);
        sprintf(show_buf, "%s", GUI_LANG_GetTextEx(Lang_Fan, Language_Bit));
        GUI_DispStringAt(show_buf, x + FAN_SHOW_XDIFF, y + FAN_SHOW_YDIFF + yoffset);
    }


    for (i = 0; i < HEAT_NUM_MAX; i++) {
        if (basic_inf.heat_en[i] != STA_ON) { //禁止
            Show_Dis_Pic( 730 , 120 + i * 60);
        } else {
            if (sta_out.ALLJDQ[JDQ_HEAT1 + i] == STA_ON) {
                GUI_defDrawBitmap(MHEAT[fan_state], 730 , 120 + i * 60);
            } else {
                GUI_defDrawBitmap(MHEAT[DIS_PIC], 730 , 120 + i * 60);
            }
        }
    }


    GUI_SetFont(&GUI_USER_HZ16);
    GUI_SetColor(GUI_WHITE);
    GUI_SetTextMode(GUI_TEXTMODE_TRANS);

    if (ctrl_inf.var.mode == STA_ON) {
        GUI_SetFont(&GUI_USER_HZ24);
        if (Language_Bit_original >= LANGUAGE_RUSSIAN) {
            GUI_SetFont(&GUI_FontBC_16);
        }

        GUI_SetColor(GUI_WHITE);
        GUI_SetTextMode(GUI_TEXTMODE_TRANS);
        if (Language_Bit_original == LANGUAGE_ENGLISH) {
            GUI_DispStringAt(GUI_LANG_GetTextEx(Lang_Culture, Language_Bit), 360, 11);
        } else if (Language_Bit_original >= LANGUAGE_RUSSIAN) {
            GUI_DispStringAt(GUI_LANG_GetTextEx(Lang_Culture, Language_Bit), 378, 13);
        } else {
            GUI_DispStringAt(GUI_LANG_GetTextEx(Lang_Culture, Language_Bit), 368, 11);
        }

    } else {
        GUI_SetFont(&GUI_USER_HZ24);

        if (Language_Bit_original >= LANGUAGE_RUSSIAN) {
            GUI_SetFont(&GUI_FontBC_16);
        }

        GUI_SetColor(GUI_WHITE);
        GUI_SetTextMode(GUI_TEXTMODE_TRANS);
        if (Language_Bit_original == LANGUAGE_ENGLISH) {
            GUI_DispStringAt(GUI_LANG_GetTextEx(Lang_Empty, Language_Bit), 360, 11);
        } else if (Language_Bit_original >= LANGUAGE_RUSSIAN) {
            GUI_DispStringAt(GUI_LANG_GetTextEx(Lang_Empty, Language_Bit), 378, 13);
        } else {
            GUI_DispStringAt(GUI_LANG_GetTextEx(Lang_Empty, Language_Bit), 368, 11);
        }
    }

    if (basic_inf.type <= ROOM_TYPE_SOW) {
        GUI_defDrawBitmap("54MPic_Pig", 75, 45);
    } else if (basic_inf.type <= ROOM_CHICKEN_ADULT) {
        GUI_defDrawBitmap("45MPic_Chick", 75, 45);
    } else if (basic_inf.type <= ROOM_DUCK_SEED){
        GUI_defDrawBitmap("55MPic_duck", 75, 45);
    } else{
        GUI_defDrawBitmap("45MPic_Chick", 75, 45);
    }
	
}

/*****************************************************************
**      FunctionName:void InitDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**
**      call this function in _cbCallback --> WM_INIT_DIALOG
*****************************************************************/

void Cal_Day_Proc(void)
{
    time_t sys_time = GetSysTime();

    if (sys_sta.on_time == 0) {
        sys_sta.on_time = sys_time;
        sys_sta.on_hour = 0;
    }

    if (sys_time > sys_sta.on_time + (sys_sta.on_hour + 1) * HOUR_TO_SEC) { //新的一小时
        Trial_Time_Save();
        Update_Cal_Data();
        sys_sta.on_hour = (sys_time - sys_sta.on_time) / HOUR_TO_SEC;

    }

    if (sys_time > (basic_inf.data_time + basic_inf.dataupdate_time * MIN_TO_SEC)) { //新的一天
        basic_inf.data_time = sys_time - (sys_time % DAY_TO_SEC) + DAY_TO_SEC;

        //记录数据
        Update_Pro_Data();
        SyncSysTime();//与系统时间同步一下
        sys_time = GetSysTime();//重新获得系统时间
		
        if (Config_Save(SBASIC_ID) < 0) {
            MWarning_Task(HMain, Get_Tips(TIPS_SAVE_ER));
        }		
    }

    if (sys_time > basic_inf.group_time + basic_inf.dayupdate_time * MIN_TO_SEC) { //新的一天
        //记录数据
        Updata_Data();
        //更新日龄

        basic_inf.group_time = sys_time - (sys_time % DAY_TO_SEC) + DAY_TO_SEC;
        basic_inf.grow_day++;

        sys_sta.update_hour = 1;
        sys_sta.update_time = sys_time;

        if (Config_Save(SBASIC_ID) < 0) {
            MWarning_Task(HMain, Get_Tips(TIPS_SAVE_ER));
        }
    }

    if (sys_sta.update_hour < ctrl_inf.extemp.chg_times && sys_time > sys_sta.update_time + sys_sta.update_hour * ctrl_inf.extemp.chg_hours * HOUR_TO_SEC) {
        sys_sta.update_hour ++;
    }

}


const char Order_Buf[SENSOR_NUM_MAX][8] = {"1.", "2.", "3.", "4.", "5.", "6.",};




static void	Show_Data(WM_HWIN hWin)
{
    int i, id = GUI_ID_TEXT0, show_id = 0;;
    GUI_RECT Rect;
    time_t sys_time = GetSysTime();
//    TDATE_TIME	dt;
//    PCF8563_GetDT( &dt );
//    sprintf(show_buf, "%02x/%02x/%02x %02x:%02x:%02x", dt.year, dt.mon, dt.day, dt.hour, dt.min, dt.sec);
    struct tm *t;
    t = localtime((const time_t *)&sys_time);
    sprintf(show_buf, "%02d/%02d/%02d %02d:%02d:%02d ", (t->tm_year - 100), (t->tm_mon + 1), t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);      //时间

    TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *) show_buf);

    sprintf(show_buf, "%s%d", GUI_LANG_GetTextEx(room_type[basic_inf.type], Language_Bit), basic_inf.room);         //房舍类型

    TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *) show_buf);

    sprintf(show_buf, "%s:  %d", GUI_LANG_GetTextEx(Lang_Day, Language_Bit), basic_inf.grow_day);                       
    TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *) show_buf);

    sprintf(show_buf, "%s:  %.1f%s",GUI_LANG_GetTextEx(Lang_TarT, Language_Bit), (float)(cal_inf.target_temp * 1.0f / (float)TEMP_RATE), GUI_LANG_GetTextEx(Lang_oC, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *) show_buf);

    if (sys_sta.sl_set.lv_en == STA_ON) {
        TEXT_SetTextColor(WM_GetDialogItem(hWin, id), GUI_DEF_RED);
    } else {
        TEXT_SetTextColor(WM_GetDialogItem(hWin, id), GUI_WHITE);
    }

    sprintf(show_buf, "%s:  %d", GUI_LANG_GetTextEx(Lang_Level, Language_Bit), cal_inf.lv + 1);
    TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *) show_buf);


//    if (cal_inf.temp == TEMP_ER_VALUE) {
//        TEXT_SetText(WM_GetDialogItem(hWin, id++), "--.-");
//    } else {
//        sprintf(show_buf, "%.1f", (float)(cal_inf.temp*1.0f / (float)TEMP_RATE));
//        TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *) show_buf);
//    }

    if (sys_sta.sl_set.temp_en == STA_ON) {
        TEXT_SetTextColor(WM_GetDialogItem(hWin, id), GUI_DEF_RED);
    } else {
        TEXT_SetTextColor(WM_GetDialogItem(hWin, id), GUI_WHITE);
    }

    sprintf(show_buf, "%.1f", (float)(cal_inf.temp * 1.0f / (float)TEMP_RATE));
    TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *) show_buf);

    if (cal_inf.SEN_EN[SENSOR_TYPE_HUM] == STA_OFF || cal_inf.SEN[SENSOR_TYPE_HUM] == (short)SENSOR_ER_VALUE) {
        TEXT_SetText(WM_GetDialogItem(hWin, id++), "--");
    } else {
        sprintf(show_buf, "%d", cal_inf.SEN[SENSOR_TYPE_HUM]);
        TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *) show_buf);
    }

    //    if (dev_inf.sensor_adj.TEMP_EN[TEMP_OUT_SID] == STA_OFF || sta_in.sensor_real.TEMP[TEMP_OUT_SID] == TEMP_ER_VALUE) {
    if (dev_inf.sensor_adj.TEMP_EN[TEMP_OUT_SID] == STA_OFF || sta_in.sensor_real.TEMP[TEMP_OUT_SID] > TEMP_OUTMAX_ERR || sta_in.sensor_real.TEMP[TEMP_OUT_SID] < TEMP_OUTMIN_ERR) {
        TEXT_SetTextColor(WM_GetDialogItem(hWin, id), GUI_GRAY_D0);

        sprintf(show_buf, "%s: --.-%s", GUI_LANG_GetTextEx(Lang_Outdoor_Temp, Language_Bit), GUI_LANG_GetTextEx(Lang_oC, Language_Bit));
        TEXT_SetText(WM_GetDialogItem(hWin, id++), show_buf);

    } else {

        sprintf(show_buf, "%s: %.1f%s", GUI_LANG_GetTextEx(Lang_Outdoor_Temp, Language_Bit),
                (float)(sta_in.sensor_adj.TEMP[TEMP_OUT_SID] * 1.0f / (float)TEMP_RATE), GUI_LANG_GetTextEx(Lang_oC, Language_Bit));

        TEXT_SetTextColor(WM_GetDialogItem(hWin, id), GUI_WHITE);
        TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *)show_buf);
    }

    if (dev_inf.env_dev[MB_DEV_PM2_5].enable == STA_OFF || ALA_Get_State(ALA_EPM2_5) == ALA_STA_ON) {
        WM_HideWindow(WM_GetDialogItem(hWin, id++));
    } else {
        WM_ShowWindow(WM_GetDialogItem(hWin, id));
        sprintf(show_buf, "PM2.5: %dug/m3", sta_in.pm2_5);
        TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *) show_buf);
    }

    if (run_inf.LV_StartLast < 0 || run_inf.LV_StopLast < 0) {
        WM_HideWindow(WM_GetDialogItem(hWin, id++));
    } else {
        WM_ShowWindow(WM_GetDialogItem(hWin, id));

        if (run_inf.LV_StopTime >= (TIME_4BIT + 1)) {
            sprintf(show_buf, "%s%s%ds", GUI_LANG_GetTextEx(Lang_Level, Language_Bit), GUI_LANG_GetTextEx(Lang_Open_T, Language_Bit), run_inf.LV_StartLast);
        } else {
            sprintf(show_buf, "%s%s%ds", GUI_LANG_GetTextEx(Lang_Level, Language_Bit), GUI_LANG_GetTextEx(Lang_Close_T, Language_Bit), run_inf.LV_StopLast);
        }

        TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *)show_buf);
    }

    id = GUI_ID_TEXT38;

    for (i = 0; i < SENSOR_NUM_MAX; i++) {
        TEXT_SetTextColor(WM_GetDialogItem(hWin, id), GUI_WHITE);

        switch (dev_inf.sensor_adj.SENSOR_EN[i]) {
        case SENSOR_TYPE_DIS:
        default:
            sprintf(show_buf, "%s %s: --", Order_Buf[i], GUI_LANG_GetTextEx(Lang_Disable, Language_Bit));
            TEXT_SetTextColor(WM_GetDialogItem(hWin, id), GUI_GRAY_D0);
            TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *)show_buf);
            break;

        case SENSOR_TYPE_TEMP:
            if (sta_in.sensor_adj.SENSOR[i] == TEMP_ER_VALUE) {
                sprintf(show_buf, "%s %s: --.-%s", Order_Buf[i], GUI_LANG_GetTextEx(Lang_Temp, Language_Bit), GUI_LANG_GetTextEx(Lang_oC, Language_Bit));
                TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *)show_buf);
                break;
            }

            sprintf(show_buf, "%s %s: %.1f%s", Order_Buf[i], GUI_LANG_GetTextEx(Lang_Temp, Language_Bit),
                    (float)(sta_in.sensor_adj.SENSOR[i] * 1.0f / (float)TEMP_RATE), GUI_LANG_GetTextEx(Lang_oC, Language_Bit));
            TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *)show_buf);
            break;

        case SENSOR_TYPE_HUM:
            if (sta_in.sensor_adj.SENSOR[i] == (short)SENSOR_ER_VALUE) {
                sprintf(show_buf, "%s %s: --.-%%", Order_Buf[i], GUI_LANG_GetTextEx(Lang_HUM, Language_Bit));
                TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *)show_buf);
                break;
            }

            sprintf(show_buf, "%s %s: %d%%", Order_Buf[i], GUI_LANG_GetTextEx(Lang_HUM, Language_Bit), sta_in.sensor_adj.SENSOR[i]);
            TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *)show_buf);
            break;

        case SENSOR_TYPE_OUTHUM:
            if (sta_in.sensor_adj.SENSOR[i] == (short)SENSOR_ER_VALUE) {
                sprintf(show_buf, "%s %s:--%%", Order_Buf[i], GUI_LANG_GetTextEx(Lang_Out_HUM, Language_Bit));
                TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *)show_buf);
                break;
            }

            sprintf(show_buf, "%s %s: %d%%", Order_Buf[i], GUI_LANG_GetTextEx(Lang_Out_HUM, Language_Bit), sta_in.sensor_adj.SENSOR[i]);
            TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *)show_buf);
            break;

        case SENSOR_TYPE_CO2:
            if (sta_in.sensor_adj.SENSOR[i] == (short)SENSOR_ER_VALUE) {
                if(Language_Bit_original == LANGUAGE_Arabic)
                {
                    sprintf(show_buf, "ppm---:CO2 .%d", i+1);
                }else{
                    sprintf(show_buf, "%s CO2:---ppm", Order_Buf[i]);
                }
            }else{
                if(Language_Bit_original == LANGUAGE_Arabic)
                {
                    sprintf(show_buf, "ppm%d :CO2 .%d", sta_in.sensor_adj.SENSOR[i], i+1);
                }else{
                    sprintf(show_buf, "%s CO2: %dppm", Order_Buf[i], sta_in.sensor_adj.SENSOR[i]);
                }
            }

            
            TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *)show_buf);
			 
			break;

        case SENSOR_TYPE_NH3:
            if (sta_in.sensor_adj.SENSOR[i] == (short)SENSOR_ER_VALUE) {
                if(Language_Bit_original == LANGUAGE_Arabic)
                {
                    sprintf(show_buf, "ppm--:NH3 .%d", i+1);
                }else{
                    sprintf(show_buf, "%s NH3:--ppm", Order_Buf[i]);
                }
            }else{
                if(Language_Bit_original == LANGUAGE_Arabic)
                {
                    sprintf(show_buf, "ppm%d :NH3 .%d", sta_in.sensor_adj.SENSOR[i], i+1);
                }else{
                    sprintf(show_buf, "%s NH3: %dppm", Order_Buf[i], sta_in.sensor_adj.SENSOR[i]);
                }
            }
            TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *)show_buf);
            break;

        case SENSOR_TYPE_H2S:
            if (sta_in.sensor_adj.SENSOR[i] == (short)SENSOR_ER_VALUE) {
                if(Language_Bit_original == LANGUAGE_Arabic)
                {
                    sprintf(show_buf, "ppm--:H2S .%d", i+1);
                }else{
                    sprintf(show_buf, "%s H2S:--ppm", Order_Buf[i]);
                }
            }else{
                if(Language_Bit_original == LANGUAGE_Arabic)
                {
                    sprintf(show_buf, "ppm%d :H2S .%d", sta_in.sensor_adj.SENSOR[i], i+1);
                }else{
                    sprintf(show_buf, "%s H2S: %dppm", Order_Buf[i], sta_in.sensor_adj.SENSOR[i]);
                }
            }
            TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *)show_buf);
            break;

        case SENSOR_TYPE_VAC:
            if (sta_in.sensor_adj.SENSOR[i] == VAC_ER_VALUE) {
                sprintf(show_buf, "%s %s: --Pa", Order_Buf[i], GUI_LANG_GetTextEx(Lang_NP, Language_Bit));
                TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *)show_buf);
                break;
            }

            sprintf(show_buf, "%s %s: %dPa", Order_Buf[i], GUI_LANG_GetTextEx(Lang_NP, Language_Bit), sta_in.sensor_adj.SENSOR[i]);
            TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *)show_buf);
            break;

        case SENSOR_TYPE_SPEED:
            if (sta_in.sensor_adj.SENSOR[i] == (short)SENSOR_ER_VALUE) {
                sprintf(show_buf, "%s %s:-.--m/s", Order_Buf[i], GUI_LANG_GetTextEx(Lang_Wind, Language_Bit));
                TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *)show_buf);
                break;
            }

            sprintf(show_buf, "%s %s: %.2fm/s", (char *)Order_Buf[i], GUI_LANG_GetTextEx(Lang_Wind, Language_Bit), (float)(sta_in.sensor_adj.SENSOR[i] * 1.0f / SPEED_RATE));
            TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *)show_buf);
            break;
			
		
        }
    }

    id = GUI_ID_TEXT12;

    for (i = 0; i < 6; i++) {
        if ((basic_inf.tempc_en.enable == STA_ON && (i + 1) == basic_inf.tempc_en.temp_id)
                || (basic_inf.tempc_pwm_en.enable == STA_ON && (i + 1) == basic_inf.tempc_pwm_en.temp_id) ) { //单独温控只判断故障
            TEXT_SetTextColor(WM_GetDialogItem(hWin, id), GUI_RED);
            sprintf(show_buf, "%.1f%s", (float)(sta_in.sensor_adj.TEMP[i] * 1.0f / (float)TEMP_RATE), GUI_LANG_GetTextEx(Lang_oC, Language_Bit));
            TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *) show_buf);
            continue;
        }

        if (dev_inf.sensor_adj.TEMP_EN[i] == STA_OFF) {
            TEXT_SetTextColor(WM_GetDialogItem(hWin, id), GUI_GRAY);
        } else {
            TEXT_SetTextColor(WM_GetDialogItem(hWin, id), GUI_WHITE);
        }

        if (sta_in.sensor_real.TEMP[i] > TEMP_MAX_ERR || sta_in.sensor_real.TEMP[i] < TEMP_MIN_ERR) { //出错不显示
            sprintf(show_buf, "--.-%s", GUI_LANG_GetTextEx(Lang_oC, Language_Bit));
            TEXT_SetText(WM_GetDialogItem(hWin, id++), show_buf);
        } else {
            sprintf(show_buf, "%.1f%s", (float)(sta_in.sensor_adj.TEMP[i] * 1.0f / (float) TEMP_RATE),GUI_LANG_GetTextEx(Lang_oC, Language_Bit));
            TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *) show_buf);
        }
    }

    id = GUI_ID_TEXT20;

    for (i = 0; i < PWM_NUM_MAX; i++) {
        TEXT_SetTextColor(WM_GetDialogItem(hWin, id), GUI_WHITE);

        if (i == IM_PWM3_PHEAT && basic_inf.pheat_en == STA_ON) {
			sprintf(show_buf, "%s: %d%%", GUI_LANG_GetTextEx(Lang_Heat, Language_Bit), sta_out.ALLPWM[PWM_HEAT]);
            TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *)show_buf);
            continue;
        }

        if (i <= APWM_FAN3 && basic_inf.area_pwm[i].enable == STA_ON) {
            sprintf(show_buf, "%s%c: %d%%", GUI_LANG_GetTextEx(Lang_Exh, Language_Bit), 'A'+i, sta_out.ALLPWM[i]);
            TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *)show_buf);
            continue;
        }

        if (dev_inf.SWIN.detail[i].enable == STA_ON) {
            if (ctrl_inf.lv_more.fan_auto_sort == STA_ON) {
                show_id = cal_inf.swin_showid[i];
            } else {
                show_id = i;
            }

            sprintf(show_buf, "%s%d: %d%%", GUI_LANG_GetTextEx(Lang_Inlet, Language_Bit), show_id + 1, sta_out.ALLPWM[i]);
            TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *)show_buf);
            continue;
        }

        if (dev_inf.PWM[i].enable == STA_OFF) {
            TEXT_SetTextColor(WM_GetDialogItem(hWin, id), GUI_GRAY_D0);
            sprintf(show_buf, "%s: --%%", GUI_LANG_GetTextEx(Lang_Exh, Language_Bit));
            TEXT_SetText(WM_GetDialogItem(hWin, id++),  (const char *) show_buf);
        } else {
            if (ctrl_inf.lv_more.fan_auto_sort == STA_ON) {
                show_id = cal_inf.pwm_fan_showid[i];
            } else {
                show_id = i;
            }

            sprintf(show_buf, "%s%d: %d%%", GUI_LANG_GetTextEx(Lang_Exh, Language_Bit), show_id + 1, sta_out.ALLPWM[PWM_FAN1 + i]);
            TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *) show_buf);
        }
    }

    for (i = PWM_NUM_MAX; i < SWINDOW_NUM_MAX; i++) {
        TEXT_SetTextColor(WM_GetDialogItem(hWin, id), GUI_WHITE);

        if (i == PWM_TEMPC && dev_inf.SWIN.detail[i].enable != STA_ON) {
            if (basic_inf.tempc_pwm_en.enable == STA_OFF) {
                TEXT_SetTextColor(WM_GetDialogItem(hWin, id), GUI_GRAY_D0);
                sprintf(show_buf, "%s: --%%", GUI_LANG_GetTextEx(Lang_Tswitch, Language_Bit));
                TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *) show_buf);
            } else {
                sprintf(show_buf, "%s: %d%%", GUI_LANG_GetTextEx(Lang_Tswitch, Language_Bit), sta_out.ALLPWM[PWM_TEMPC]);
                TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *) show_buf);
            }

            continue;
        }

        if (i == PWM_LIGHT && dev_inf.SWIN.detail[i].enable != STA_ON) {
            if (dev_inf.light.enable == STA_OFF) {
                TEXT_SetTextColor(WM_GetDialogItem(hWin, id), GUI_GRAY_D0);
                sprintf(show_buf, "%s: --%%", GUI_LANG_GetTextEx(Lang_illumination, Language_Bit));
                TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *)show_buf);
            } else {
                sprintf(show_buf, "%s: %d%%", GUI_LANG_GetTextEx(Lang_illumination, Language_Bit), sta_out.ALLPWM[PWM_LIGHT]);
                TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *)show_buf);
            }

            continue;
        }

        if (dev_inf.SWIN.detail[i].enable == STA_OFF) {
            TEXT_SetTextColor(WM_GetDialogItem(hWin, id), GUI_GRAY_D0);
            sprintf(show_buf, "%s: --%%", GUI_LANG_GetTextEx(Lang_Inlet, Language_Bit));
            TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *) show_buf);
        } else {
            if (ctrl_inf.lv_more.fan_auto_sort == STA_ON) {
                show_id = cal_inf.swin_showid[i];
            } else {
                show_id = i;
            }

            sprintf(show_buf, "%s%d: %d%%", GUI_LANG_GetTextEx(Lang_Inlet, Language_Bit), show_id + 1, sta_out.ALLPWM[i]);
            TEXT_SetText(WM_GetDialogItem(hWin, id++), (const char *) show_buf);
        }
    }

    for (i = 0; i < HEAT_NUM_MAX; i++) {
        sprintf(show_buf, "%s%d", GUI_LANG_GetTextEx(Lang_Heat, Language_Bit), i + 1);
        TEXT_SetText(WM_GetDialogItem(hWin, id++), show_buf);
    }

    Rect.x0 = 730;
    Rect.y0 = 115;
    Rect.x1 = 760;
    Rect.y1 = 260;
    WM_InvalidateRect(hWin, &Rect);

    Rect.x0 = 80;
    Rect.y0 = 285;
    Rect.x1 = 760;
    Rect.y1 = 400;
    WM_InvalidateRect(hWin, &Rect);

    Rect.x0 = 560;
    Rect.y0 = 0;
    Rect.x1 = 620;
    Rect.y1 = 42;
    WM_InvalidateRect(hWin, &Rect);

    Rect.x0 = 60;
    Rect.y0 = 80;
    Rect.x1 = 120;
    Rect.y1 = 140;
    WM_InvalidateRect(hWin, &Rect);

    fan_state = (fan_state + 1) % 2;
}

//static void PWD_Check(void)
//{
//    unsigned char *pwd_buf = Get_Input_Text();

////这个地方必须使用HMain，因为这个界面只创建一次
//    if (strcmp((const char *)pwd_buf, basic_inf.supadmin_pwd) == 0) {
//        MSys_Man_Task(HMain, 0);
//    } else if (strcmp((const char *)pwd_buf, basic_inf.admin_pwd) == 0) {
//        MSys_Man_Task(HMain, 1);
//    } else {
//        MWarning_Task(HMain, Get_Tips(TIPS_PWD_ER));
//    }
//}

static void InitDialog(WM_MESSAGE * pMsg)
{
    WM_HWIN hWin = pMsg->hWin;
    int MDev_ID = 0;
    WM_HWIN hItem;

    hItem = WM_GetDialogItem(hWin, GUI_ID_TEXT0);
    TEXT_SetTextAlign(hItem, GUI_TA_VCENTER | GUI_TA_HCENTER  );
    TEXT_SetTextColor(hItem, GUI_WHITE);
    TEXT_SetFont(hItem, &GUI_USER_HZ16);
	
	if(Language_Bit_original >= LANGUAGE_RUSSIAN) {
 	   TEXT_SetFont(hItem, &GUI_FontBC_16);
	}	
	
    WM_DisableWindow(hItem);

    hItem = WM_GetDialogItem(hWin, GUI_ID_TEXT1);
    TEXT_SetTextAlign(hItem, GUI_TA_VCENTER | GUI_TA_LEFT );
    TEXT_SetFont(hItem, &GUI_USER_HZ16);
    TEXT_SetTextColor(hItem, GUI_WHITE);
	
	if(Language_Bit_original >= LANGUAGE_RUSSIAN) {
 	   TEXT_SetFont(hItem, &GUI_FontBC_16);
	}	
	
    WM_DisableWindow(hItem);

    for (MDev_ID = GUI_ID_BUTTON1_0; MDev_ID <= GUI_ID_BUTTON1_6; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, Main_cbButton);
    }


    for (MDev_ID = GUI_ID_TEXT2; MDev_ID <= GUI_ID_TEXT5; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        TEXT_SetTextAlign(hItem, GUI_TA_VCENTER | GUI_TA_LEFT  );
        TEXT_SetFont(hItem, &GUI_USER_HZ16);
        TEXT_SetTextColor(hItem, GUI_WHITE);
		
		if(Language_Bit_original >= LANGUAGE_RUSSIAN) {
	 	   TEXT_SetFont(hItem, &GUI_FontBC_16);
		}	
        if (MDev_ID == GUI_ID_TEXT5) {
            TEXT_SetTextAlign(hItem, GUI_TA_VCENTER | GUI_TA_HCENTER);
        }	
		
        WM_DisableWindow(hItem);
    }

    for (MDev_ID = GUI_ID_TEXT6; MDev_ID <= GUI_ID_TEXT9; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        TEXT_SetTextAlign(hItem, GUI_TA_VCENTER | GUI_TA_LEFT);
        TEXT_SetFont(hItem, &GUI_USER_HZ16);
		
		if(Language_Bit_original >= LANGUAGE_RUSSIAN) {
	 	   TEXT_SetFont(hItem, &GUI_FontBC_16);
		}
		
        if (MDev_ID == GUI_ID_TEXT6) {
            TEXT_SetTextAlign(hItem, GUI_TA_VCENTER | GUI_TA_HCENTER);
        }

        if (MDev_ID == GUI_ID_TEXT9) {
            TEXT_SetTextColor(hItem, GUI_WHITE);
        }

        TEXT_SetTextColor(hItem, GUI_WHITE);
        WM_DisableWindow(hItem);
    }

    for (MDev_ID = GUI_ID_TEXT12; MDev_ID <= GUI_ID_TEXT43; MDev_ID++) {
        if (MDev_ID >= GUI_ID_TEXT29 && MDev_ID <= GUI_ID_TEXT36) {
            continue;
        }

        hItem = WM_GetDialogItem(hWin, MDev_ID);
        TEXT_SetTextAlign(hItem, GUI_TA_VCENTER | GUI_TA_LEFT);
        TEXT_SetFont(hItem, &GUI_USER_HZ16);
		
		if(Language_Bit_original >= LANGUAGE_RUSSIAN) {
	 	   TEXT_SetFont(hItem, &GUI_FontBC_16);
		}

        TEXT_SetTextColor(hItem, GUI_WHITE);
        WM_DisableWindow(hItem);
    }

    Show_Data(hWin);
    hTimer = WM_CreateTimer(WM_GetClientWindow(hWin), 0, 700, 0);


    WM_EnableWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT20));
    WM_EnableWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT21));
    WM_EnableWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT22));
    WM_EnableWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT23));
    WM_EnableWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT24));
    WM_EnableWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT25));
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

    case WM_TIMER:
        Show_Data(hWin);
        WM_RestartTimer(hTimer, 700);
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

        case GUI_ID_BUTTON1_0:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                HMain = 0;
                Current_HWIN = WM_GetParent(hWin);
                GUI_EndDialog(hWin, 0);
            }

            break;

        case GUI_ID_BUTTON1_1:
        case GUI_ID_TEXT20:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                MDev_Man_Task(hWin);
            }

            break;

        case GUI_ID_BUTTON1_2:
        case GUI_ID_TEXT21:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                MCtrl_Man_Task(hWin);
            }

            break;

        case GUI_ID_BUTTON1_3:
        case GUI_ID_TEXT22:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                MSys_Man_Task(hWin);
            }

            break;

        case GUI_ID_BUTTON1_4:
        case GUI_ID_TEXT23:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                MData_Record_Task(hWin);
            }

            break;

        case GUI_ID_BUTTON1_5:
        case GUI_ID_TEXT24:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                MAla_Record_Task(hWin);
            }

            break;

        case GUI_ID_BUTTON1_6:
        case GUI_ID_TEXT25:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                MUser_Man_Task(hWin);
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


WM_HWIN HMain = 0;
void Main_Task(WM_HWIN hWin)
{
    Main_Buffer=Main_Buffer_Home;
    HMain = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hWin, 0, 0);
    Current_HWIN = HMain;
}

