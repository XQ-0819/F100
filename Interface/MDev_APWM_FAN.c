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
#include <string.h>
#include "bsp.h"

/*********************************************************************
*
*       static data
*
**********************************************************************
*/

#define GUI_ID_EDIT10  (GUI_ID_USER+1)
#define GUI_ID_EDIT11  (GUI_ID_USER+2)


#define GUI_ID_TEXT10  (GUI_ID_USER+42)


static U8 select_id = 0;
static U8 select_num = 0;
static U8 select_chg_flag = 0;//是否能选择


static TCHG_MENU Mchg_Menu[] = {

    {MSHOW, TYPE_TEXT, GUI_ID_TEXT2, "%s", 1, SNUM_FLAG},
    {MSHOW, TYPE_TEXT, GUI_ID_TEXT3, "%s", 1, SNUM_FLAG},
    {MSHOW, TYPE_TEXT, GUI_ID_TEXT4, "%s", 1, SNUM_FLAG},
    {MSHOW, TYPE_TEXT, GUI_ID_TEXT5, "%s", 1, SNUM_FLAG},
    {MSHOW, TYPE_TEXT, GUI_ID_TEXT6, "%s", 1, SNUM_FLAG},

    {MSHOW, TYPE_EDIT, GUI_ID_EDIT0, "%.1f", 0, TEMP_RATE},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT1, "%.1f", 0, TEMP_RATE},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT2, "%.1f", 0, TEMP_RATE},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT3, "%.1f", 0, TEMP_RATE},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT4, "%.1f", 0, TEMP_RATE},

    {MSHOW, TYPE_EDIT, GUI_ID_EDIT5, "%d%%", 0, 1},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT6, "%d%%", 0, 1},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT7, "%d%%", 0, 1},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT8, "%d%%", 0, 1},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT9, "%d%%", 0, 1},

    {MSHOW, TYPE_EDIT, GUI_ID_EDIT10, "%.1f", 0, TEMP_RATE},
    {MSHOW, TYPE_EDIT, GUI_ID_EDIT11, "%d V", 0, 1},

    {MEXIT, 0, 0, "", 0, 0},
};
static void Set_OptValue(WM_HWIN hWin, U8 sid)
{
    U8 i = 0;
    //增加是否启用

    for (i = 0; i < 5; i++) {
        CHECKBOX_SetState(WM_GetDialogItem(hWin, (GUI_ID_CHECK1 + i) ), basic_inf.area_pwm[select_num].freq[5 * sid + i].enable);
    }

    if (basic_inf.area_pwm[select_num].enable == STA_ON) {
        sprintf(show_buf, ":%s", GUI_LANG_GetTextEx(Lang_Enable, Language_Bit));
        TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT10), show_buf);
    } else {
        sprintf(show_buf, ":%s", GUI_LANG_GetTextEx(Lang_Disable, Language_Bit));
        TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT10), show_buf);
    }

    Button_CurShow(hWin, sid, 2);
}

static void Menu_Value_Init(WM_HWIN hWin, U8 sid)
{
    U8 id = 0, i = 0;

    for (i = 0; i < 5; i++) {
        Mchg_Menu[id++].value = 5 * sid + i + 1;
    }

    for (i = 0; i < 5; i++) {
        Mchg_Menu[id].value = basic_inf.area_pwm[select_num].freq[5 * sid + i].offset;
        sprintf(Mchg_Menu[id].text, "%%.1f%s", GUI_LANG_GetTextEx(Lang_oC, Language_Bit));
        id++;
    }

    for (i = 0; i < 5; i++) {
        Mchg_Menu[id++].value = basic_inf.area_pwm[select_num].freq[5 * sid + i].freq;
    }

    Mchg_Menu[id].value = basic_inf.area_pwm[select_num].target_temp;
    sprintf(Mchg_Menu[id].text, "%%.1f%s", GUI_LANG_GetTextEx(Lang_oC, Language_Bit));
    id++;
    Mchg_Menu[id++].value = basic_inf.area_pwm[select_num].min_vol;
}

/*********************************************************************
*
*       Dialog resource
*
* This table conatins the info required to create the dialog.
* It has been created by ucGUIbuilder.
*/
#define XPos_Collec  50
#define YPos_Collec  0
#define Y_Space     40
//#define XPos_Collec1 40
#define Text_Width   36
#define Category_Space  10
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect,    "",               0,                       0,  0,  800, 480, 0, 0},
    { TEXT_CreateIndirect,      "设备管理-分区调速",          GUI_ID_TEXT0,           250, 4, 300, 42, 0, 0},

    { TEXT_CreateIndirect,      "目标温度:",        GUI_ID_TEXT1,              250 + XPos_Collec, 60, 115, Text_Width, 0, 0},
    { EDIT_CreateIndirect,    " ",     			GUI_ID_EDIT10,                  370 + XPos_Collec, 60, 80, Text_Width, 0, 0},

    { TEXT_CreateIndirect,      "最小电压:",        GUI_ID_TEXT9,              465 + XPos_Collec, 60, 115, Text_Width, 0, 0},
    { EDIT_CreateIndirect,    " ",     			GUI_ID_EDIT11,                  585 + XPos_Collec, 60, 72, Text_Width, 0, 0},

    { TEXT_CreateIndirect,      " ",        		GUI_ID_TEXT10,              175 + XPos_Collec, 60, 200, Text_Width, 0, 0},
    { DROPDOWN_CreateIndirect,	 "",			 	GUI_ID_DROPDOWN0,		45 + XPos_Collec, 60, 125, 120, 0, 0},

    { TEXT_CreateIndirect,     "1",    GUI_ID_TEXT2,               100 + XPos_Collec , 100 + YPos_Collec + Y_Space * 1 + Category_Space * 0, 24, Text_Width, 0, 0},
    { TEXT_CreateIndirect,     "2",      GUI_ID_TEXT3,               100 + XPos_Collec, 100 + YPos_Collec + Y_Space * 2 + Category_Space * 1, 24, Text_Width, 0, 0},
    { TEXT_CreateIndirect,     "3",       GUI_ID_TEXT4,               100 + XPos_Collec, 100 + YPos_Collec + Y_Space * 3 + Category_Space * 2, 24, Text_Width, 0, 0},
    { TEXT_CreateIndirect,     "4",     GUI_ID_TEXT5,                100 + XPos_Collec, 100 + YPos_Collec + Y_Space * 4 + Category_Space * 3, 24, Text_Width, 0, 0},
    { TEXT_CreateIndirect,     "5",      GUI_ID_TEXT6,               100 + XPos_Collec, 100 + YPos_Collec + Y_Space * 5 + Category_Space * 4, 24, Text_Width, 0, 0},

    { TEXT_CreateIndirect,    "温度偏差",    GUI_ID_TEXT7,               180 + XPos_Collec, 100 + YPos_Collec + Y_Space * 0, 100, Text_Width, 0, 0},
    { EDIT_CreateIndirect,     "8:00",      GUI_ID_EDIT0,                 180 + XPos_Collec, 100 + YPos_Collec + Y_Space * 1, 100, Text_Width, 0, 0},
    { EDIT_CreateIndirect,    "10:00",     GUI_ID_EDIT1,                  180 + XPos_Collec, 100 + YPos_Collec + Y_Space * 2 + Category_Space * 1, 100, Text_Width, 0, 0},
    { EDIT_CreateIndirect,    "14:00",        GUI_ID_EDIT2,               180 + XPos_Collec, 100 + YPos_Collec + Y_Space * 3 + Category_Space * 2, 100, Text_Width, 0, 0},
    { EDIT_CreateIndirect,    "17:00",         GUI_ID_EDIT3,              180 + XPos_Collec, 100 + YPos_Collec + Y_Space * 4 + Category_Space * 3, 100, Text_Width, 0, 0},
    { EDIT_CreateIndirect,    "00:00",        GUI_ID_EDIT4,               180 + XPos_Collec, 100 + YPos_Collec + Y_Space * 5 + Category_Space * 4, 100, Text_Width, 0, 0},

    { TEXT_CreateIndirect,    "开启频率",    GUI_ID_TEXT8,            330 + XPos_Collec, 100 + YPos_Collec + Y_Space * 0, 100, Text_Width, 0, 0},
    { EDIT_CreateIndirect,      "10:00",   GUI_ID_EDIT5,               340 + XPos_Collec, 100 + YPos_Collec + Y_Space * 1, 80, Text_Width, 0, 0},
    { EDIT_CreateIndirect,       "14:00",  GUI_ID_EDIT6,              340 + XPos_Collec, 100 + YPos_Collec + Y_Space * 2 + Category_Space * 1, 80, Text_Width, 0, 0},
    { EDIT_CreateIndirect,     "17:00",    GUI_ID_EDIT7,                340 + XPos_Collec, 100 + YPos_Collec + Y_Space * 3 + Category_Space * 2, 80, Text_Width, 0, 0},
    { EDIT_CreateIndirect,    "24:00",      GUI_ID_EDIT8,             340 + XPos_Collec, 100 + YPos_Collec + Y_Space * 4 + Category_Space * 3, 80, Text_Width, 0, 0},
    { EDIT_CreateIndirect,     "8:00",       GUI_ID_EDIT9,              340 + XPos_Collec, 100 + YPos_Collec + Y_Space * 5 + Category_Space * 4, 80, Text_Width, 0, 0},


    { CHECKBOX_CreateIndirect,  "启用",        GUI_ID_CHECK1,        520 + XPos_Collec, 100 + YPos_Collec + Y_Space * 1 + Category_Space * 0.5, 90, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",        GUI_ID_CHECK2,        520 + XPos_Collec, 100 + YPos_Collec + Y_Space * 2 + Category_Space * 1.5, 90, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",        GUI_ID_CHECK3,        520 + XPos_Collec, 100 + YPos_Collec + Y_Space * 3 + Category_Space * 2.5, 90, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",        GUI_ID_CHECK4,        520 + XPos_Collec, 100 + YPos_Collec + Y_Space * 4 + Category_Space * 3.5, 90, CHECK_SIZE, 0, 0},
    { CHECKBOX_CreateIndirect,  "启用",        GUI_ID_CHECK5,        520 + XPos_Collec, 100 + YPos_Collec + Y_Space * 5 + Category_Space * 4.5, 90, CHECK_SIZE, 0, 0},

    { BUTTON_CreateIndirect,    "上一页",          GUI_ID_BUTTON0,          MENU_BACK_X1, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
    { BUTTON_CreateIndirect,    "下一页",          GUI_ID_BUTTON1,          MENU_BACK_X2, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
    { BUTTON_CreateIndirect,    "返回",             GUI_ID_BUTTON2,         MENU_BACK_X3, MENU_BACK_Y, MENU_BACK_W, MENU_BACK_H, 0, 0},
};






static U8 temp_id = 0;
static void Start_Offtemp_Get(void)
{
    float ftemp = Get_Input_Value() * TEMP_RATE;
    short stemp = (short)ftemp;

    if (stemp > TEMP_ADJMAX) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = TEMP_ADJMAX;
    }

    if (stemp < TEMP_ADJMIN) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = TEMP_ADJMIN;
    }

    basic_inf.area_pwm[select_num].freq[5 * select_id + temp_id].offset = stemp;
    MChange_Menu(Menu_Value_Init, Current_HWIN, (PCHG_MENU )Mchg_Menu, select_id);
}

static void Start_Freq_Get(void)
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

    basic_inf.area_pwm[select_num].freq[5 * select_id + temp_id].freq = stemp;
    MChange_Menu(Menu_Value_Init, Current_HWIN, (PCHG_MENU )Mchg_Menu, select_id);
}

static void Target_Temp_Get(void)
{
    float ftemp = Get_Input_Value() * TEMP_RATE;
    short stemp = (short)ftemp;

    if (stemp > TEMP_MAX) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = TEMP_MAX;
    }

    if (stemp < TEMP_MIN) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = TEMP_MIN;
    }

    basic_inf.area_pwm[select_num].target_temp = stemp;
    MChange_Menu(Menu_Value_Init, Current_HWIN, (PCHG_MENU )Mchg_Menu, select_id);
}

static void Min_FanP_Get(void)
{
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

    basic_inf.area_pwm[select_num].min_vol = stemp;
    MChange_Menu(Menu_Value_Init, Current_HWIN, (PCHG_MENU )Mchg_Menu, select_id);
}

/*
static void Start_Sec_Get(void)
{
    float ftemp = Get_Input_Value();
    short stemp = (short)ftemp;

//    if (stemp > HUM_ADJMAX) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = HUM_ADJMAX;
//    }

    if (stemp < 0) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 0;
    }

    basic_inf.area_pwm[select_num].start_sec = stemp;
    MChange_Menu(Menu_Value_Init, Current_HWIN, (PCHG_MENU )Mchg_Menu, select_id);
}
static void Stop_Sec_Get(void)
{
    float ftemp = Get_Input_Value();
    short stemp = (short)ftemp;

//    if (stemp > HUM_ADJMAX) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = HUM_ADJMAX;
//    }

    if (stemp < 0) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 0;
    }

    basic_inf.area_pwm[select_num].stop_sec = stemp;
    MChange_Menu(Menu_Value_Init, Current_HWIN, (PCHG_MENU )Mchg_Menu, select_id);
}
*/

static void Get_OptValue(WM_HWIN hWin, U8 sid)
{
    U8 i = 0;
    //增加是否启用

    for (i = 0; i < 5; i++) {
        basic_inf.area_pwm[select_num].freq[5 * sid + i].enable = CHECKBOX_GetState(WM_GetDialogItem(hWin, (GUI_ID_CHECK1 + i) ));
    }
}

/*****************************************************************
**      FunctionName:void PaintDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**
**      call this function in _cbCallback --> WM_PAINT
*****************************************************************/

static void PaintDialog(WM_MESSAGE * pMsg)
{
//    WM_HWIN hWin = pMsg->hWin;
    Show_MBackPic();
    GUI_SetColor(GUI_LINE_COLOR);
    GUI_DrawRoundedRect(80, 55, 712, 386, 10);
    GUI_DrawRoundedRect(76, 52, 716, 389, 10);

    //划横线1
//    GUI_DrawLine(6, 95, 794, 95);

//    GUI_DrawLine(6, 136, 794, 136);

//    GUI_DrawLine(6, 184, 794, 184);
//    GUI_DrawLine(6, 234, 794, 234);
//    GUI_DrawLine(6, 284, 794, 284);
//    GUI_DrawLine(6, 334, 794, 334);
//    //划竖线

//    GUI_DrawLine(30, 95, 30, 396);
//    GUI_DrawLine(217, 95, 217, 396);

//    GUI_DrawLine(398, 95, 398, 396);
//    GUI_DrawLine(588, 95, 588, 396);
    //GUI_DrawLine(615, 95, 615, 396);


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

    for (MDev_ID = GUI_ID_BUTTON0; MDev_ID <= GUI_ID_BUTTON2; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, Cut_cbButton);
    }

    for (MDev_ID = GUI_ID_EDIT0; MDev_ID <= GUI_ID_EDIT9; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Edit_SetHZ24(hItem);
    }

    for (MDev_ID = GUI_ID_EDIT10; MDev_ID <= GUI_ID_EDIT11; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Edit_SetHZ24(hItem);
    }

    for (MDev_ID = GUI_ID_CHECK1; MDev_ID <= GUI_ID_CHECK5; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        CHECKBOX_SetText(hItem, GUI_LANG_GetTextEx(Lang_Enable, Language_Bit));
        Check_SetHZ24(hItem);
        Login_Show(hItem, LOGIN_ADMIN);
    }

    for (MDev_ID = GUI_ID_TEXT0; MDev_ID <= GUI_ID_TEXT9; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24(hItem);

        if (MDev_ID == GUI_ID_TEXT0) {
            TEXT_SetTextAlign(hItem,  GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetTextColor(hItem, GUI_WHITE);
        }
    }

    for (MDev_ID = GUI_ID_TEXT10; MDev_ID <= GUI_ID_TEXT10; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24L(hItem);

    }
	
    hItem = WM_GetDialogItem(hWin, GUI_ID_DROPDOWN0);
    Dropdown_SetHZ24(hItem);

    sprintf(show_buf, GUI_LANG_GetTextEx(Lang_Exh_Area, Language_Bit), "A");
    DROPDOWN_AddString(hItem, show_buf); // A区调速
    sprintf(show_buf, GUI_LANG_GetTextEx(Lang_Exh_Area, Language_Bit), "B");
    DROPDOWN_AddString(hItem, show_buf); // B区调速
    sprintf(show_buf, GUI_LANG_GetTextEx(Lang_Exh_Area, Language_Bit), "C");
    DROPDOWN_AddString(hItem, show_buf); // C区调速

    if(select_chg_flag == STA_OFF){
		WM_DisableWindow(hItem);
	}
	
    DROPDOWN_SetSel(hItem, select_num);


    WM_SetFocus(WM_GetDialogItem(hWin, GUI_ID_BUTTON2));	
    Set_OptValue(hWin, select_id);
    MChange_Menu(Menu_Value_Init, hWin, (PCHG_MENU )Mchg_Menu, select_id);

    sprintf(show_buf, "%s-%s", GUI_LANG_GetTextEx(Lang_Device, Language_Bit), GUI_LANG_GetTextEx(Lang_Area_Exh, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT0), show_buf); // 设备管理-分区调速
    sprintf(show_buf, "%s:", GUI_LANG_GetTextEx(Lang_TarT, Language_Bit));
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT1), show_buf);                                         // 目标温度:
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT7), GUI_LANG_GetTextEx(Lang_Temp_Diff, Language_Bit)); // 温度偏差
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT8), GUI_LANG_GetTextEx(Lang_Frequency, Language_Bit)); // 开启频率
    TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT9), GUI_LANG_GetTextEx(Lang_VolMin, Language_Bit));    // 最小电压:
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
		
        if (old_Id != Id) {
            old_Id = Id;
            break;
        }
						
        if (Id >= GUI_ID_EDIT0 && Id <= GUI_ID_EDIT9 && NCode == WM_NOTIFICATION_RELEASED
                && get_login_permiss(LOGIN_ADMIN) < 0) {
            MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_NO_LOGIN));
            break;
        }
		
        if (Id >= GUI_ID_EDIT10 && Id <= GUI_ID_EDIT11 && NCode == WM_NOTIFICATION_RELEASED
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
			
        case GUI_ID_DROPDOWN0:
            if (NCode == WM_NOTIFICATION_SEL_CHANGED) {
                Get_OptValue(hWin, select_id);
                select_num = DROPDOWN_GetSel(WM_GetDialogItem(hWin, (GUI_ID_DROPDOWN0)));
                Set_OptValue(hWin, select_id);
                MChange_Menu(Menu_Value_Init, hWin, (PCHG_MENU )Mchg_Menu, select_id);
            }

            break;
			
        case GUI_ID_BUTTON2:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Get_OptValue(hWin, select_id);

                if (Config_Save(SAPWM_ID) < 0) {
                    MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_SAVE_ER));
                }

                Current_HWIN = WM_GetParent(hWin);
                GUI_EndDialog(hWin, 0);
            }

            break;

        case GUI_ID_BUTTON0:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                if (select_id == 1) {
                    Get_OptValue(hWin, select_id);
                    select_id = 0;
                    Set_OptValue(hWin, select_id);
                    MChange_Menu(Menu_Value_Init, hWin, (PCHG_MENU )Mchg_Menu, select_id);
                }
            }

            break;

        case GUI_ID_BUTTON1:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                if (select_id == 0) {
                    Get_OptValue(hWin, select_id);
                    select_id = 1;
                    Set_OptValue(hWin, select_id);
                    MChange_Menu(Menu_Value_Init, hWin, (PCHG_MENU )Mchg_Menu, select_id);
                }
            }

            break;

        case GUI_ID_EDIT0:
        case GUI_ID_EDIT1:
        case GUI_ID_EDIT2:
        case GUI_ID_EDIT3:
        case GUI_ID_EDIT4:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                temp_id = Id - GUI_ID_EDIT0;
                Set_Output_Callback(Start_Offtemp_Get);
                Set_Input_Len(4);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT5:
        case GUI_ID_EDIT6:
        case GUI_ID_EDIT7:
        case GUI_ID_EDIT8:
        case GUI_ID_EDIT9:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                temp_id = Id - GUI_ID_EDIT5;
                Set_Output_Callback(Start_Freq_Get);
                Set_Input_Len(3);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT10:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(Target_Temp_Get);
                Set_Input_Len(4);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT11:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(Min_FanP_Get);
                Set_Input_Len(2);
                Input_Task(hWin);
            }

            break;

//        case GUI_ID_EDIT12:
//            if (NCode == WM_NOTIFICATION_RELEASED) {
//                Set_Output_Callback(Stop_Sec_Get);
//                Set_Input_Len(3);
//                Input_Task(hWin);
//            }

//            break;

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
WM_HWIN MDev_APWM_FAN;
void MDev_APWM_FAN_Task(WM_HWIN hParent, U8 num, U8 chg_flag)
{
    select_id = 0;
    select_num = num;
	select_chg_flag = chg_flag;	
    MDev_APWM_FAN = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    Current_HWIN = MDev_APWM_FAN;
}

