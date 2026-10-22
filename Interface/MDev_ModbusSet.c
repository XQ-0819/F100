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
#include "recode.h"
#include "SourceCode.h"
//EndofEventsFunctionList


static U8 select_num = 0;//选择哪个
static U8 select_type = 0;//选择哪个

/*********************************************************************
*
*       static data
*
**********************************************************************
*/
#define GUI_ID_TEXT10   (GUI_ID_USER+1)
#define GUI_ID_TEXT11   (GUI_ID_USER+2)
#define GUI_ID_TEXT12   (GUI_ID_USER+3)
#define GUI_ID_TEXT13   (GUI_ID_USER+4)
#define GUI_ID_TEXT14   (GUI_ID_USER+5)

#define GUI_ID_DROPDOWN10 (GUI_ID_USER+50)
#define GUI_ID_DROPDOWN11 (GUI_ID_USER+51)
#define GUI_ID_DROPDOWN12 (GUI_ID_USER+52)
#define GUI_ID_DROPDOWN13 (GUI_ID_USER+53)
#define GUI_ID_DROPDOWN14 (GUI_ID_USER+54)
#define GUI_ID_DROPDOWN15 (GUI_ID_USER+55)
#define GUI_ID_DROPDOWN16 (GUI_ID_USER+56)
#define GUI_ID_DROPDOWN17 (GUI_ID_USER+57)

#define X  40
#define Y1  50
#define Y2  205
#define X_Space     260
#define Y_Space     50

#define X2_Space    80

static const u16 Lang_Buf[14] = {Lang_Serial_port_inf,Lang_Uart_num,Lang_Baud_rate,Lang_Data_bits,Lang_Stop_bit,Lang_CRC_bit,
                                Lang_Data_inf,Lang_Addr,Lang_Format,Lang_Format_Encode,Lang_Register_address,Lang_Data_Len,Lang_Decimal,Lang_Addr};
/*********************************************************************
*
*       Dialog resource
*
* This table conatins the info required to create the dialog.
* It has been created by ucGUIbuilder.
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect,    "",               0,                       0,  0,  800, 480, 0, 0},
    { TEXT_CreateIndirect,      "设备管理-详细设置",  	GUI_ID_TEXT0,           280, 4, 240, 42, 0, 0},
    { TEXT_CreateIndirect,      "串口信息",  			GUI_ID_TEXT1,           X - 20 , 				Y1, 140, 36, 0, 0},
    { TEXT_CreateIndirect,      "串口号:",  			GUI_ID_TEXT2,           X - 20, 				Y1 + Y_Space, X2_Space, 36, 0, 0},
    { TEXT_CreateIndirect,      "波特率:",  			GUI_ID_TEXT3,           X + X_Space  - 20, 	Y1 + Y_Space, X2_Space, 36, 0, 0},
    { TEXT_CreateIndirect,      "数据位:",  			GUI_ID_TEXT4,           X - 20, 				Y1 + Y_Space * 2, X2_Space+20, 36, 0, 0},
    { TEXT_CreateIndirect,      "停止位:",  			GUI_ID_TEXT5,           X + X_Space - 20, 	Y1 + Y_Space * 2, X2_Space, 36, 0, 0},
    { TEXT_CreateIndirect,      "校验位:",  			GUI_ID_TEXT6,           X + X_Space * 2 - 40 , Y1 + Y_Space * 2, X2_Space+10, 36, 0, 0},

    { TEXT_CreateIndirect,      "数据信息",  			GUI_ID_TEXT7,           X, 				Y2, 120, 36, 0, 0},
    { TEXT_CreateIndirect,      "设备地址1:",  			GUI_ID_TEXT8,           X, 				Y2 + Y_Space, 120, 36, 0, 0},
    { TEXT_CreateIndirect,      "存储格式:",  			GUI_ID_TEXT9,           X, 				Y2 + Y_Space * 2, X2_Space + 20, 36, 0, 0},
    { TEXT_CreateIndirect,      "编码格式:",  			GUI_ID_TEXT10,          X + X_Space - 20, 	Y2 + Y_Space * 2, X2_Space + 20, 36, 0, 0},
    { TEXT_CreateIndirect,      "寄存器地址:",  		GUI_ID_TEXT11,          X, 				Y2 + Y_Space * 3, X2_Space +48, 36, 0, 0},
    { TEXT_CreateIndirect,      "数据长度:",  			GUI_ID_TEXT12,          X + X_Space - 20, 	Y2 + Y_Space * 3, X2_Space + 44, 36, 0, 0},
    { TEXT_CreateIndirect,      "小数位:",  			GUI_ID_TEXT13,          X + X_Space * 2 - 20, Y2 + Y_Space * 3, X2_Space +44, 36, 0, 0},
    { TEXT_CreateIndirect,      "设备地址2:",  			GUI_ID_TEXT14,          X + X_Space + X2_Space, 	Y2 + Y_Space, 120, 36, 0, 0},
    { EDIT_CreateIndirect,      "",           			GUI_ID_EDIT4,         	X + X_Space + X2_Space * 2 + 40, 	Y2 + Y_Space, 190, 36, 0, 0},

    { EDIT_CreateIndirect,      "",           			GUI_ID_EDIT0,         	X + X2_Space + 40, 	Y2 + Y_Space, 190, 36, 0, 0},
    { EDIT_CreateIndirect,      "",            			GUI_ID_EDIT1,          	X + X2_Space + 48, 	Y2 + Y_Space * 3, 90, 36, 0, 0},
    { EDIT_CreateIndirect,      "",            			GUI_ID_EDIT2,          	X + X_Space + X2_Space + 24, 	Y2 + Y_Space * 3, 90, 36, 0, 0},
    { EDIT_CreateIndirect,      "",            			GUI_ID_EDIT3,          	X + X_Space * 2 + X2_Space+24, 	Y2 + Y_Space * 3, 90, 36, 0, 0},

    { DROPDOWN_CreateIndirect,  "",          			GUI_ID_DROPDOWN10,       X + X2_Space, 	Y1 + Y_Space, 120, 110, 0, 0},
    { DROPDOWN_CreateIndirect,  "",          			GUI_ID_DROPDOWN11,       X + X_Space + X2_Space, 	Y1 + Y_Space, 120, 145, 0, 0},
    { DROPDOWN_CreateIndirect,  "",          			GUI_ID_DROPDOWN12,       X + X2_Space, 	Y1 + Y_Space * 2, 120, 110, 0, 0},
    { DROPDOWN_CreateIndirect,  "",          			GUI_ID_DROPDOWN13,       X + X_Space + X2_Space, 	Y1 + Y_Space * 2, 120, 110, 0, 0},
    { DROPDOWN_CreateIndirect,  "",          			GUI_ID_DROPDOWN14,       X + X_Space * 2 + X2_Space - 20, 	Y1 + Y_Space * 2, 160, 110, 0, 0},
    { DROPDOWN_CreateIndirect,  "",          			GUI_ID_DROPDOWN15,       X + X2_Space + 24, Y2 + Y_Space * 2, 120, 110, 0, 0},
    { DROPDOWN_CreateIndirect,  "",          			GUI_ID_DROPDOWN16,       X + X_Space + X2_Space + 24, Y2 + Y_Space * 2, 120, 145, 0, 0},

    { BUTTON_CreateIndirect,    "料塔标定",          GUI_ID_BUTTON0,          MENU_BACK_X1, MENU_BACK_Y, 120, 50, 0, 0},
    { BUTTON_CreateIndirect,    "风机检测",          GUI_ID_BUTTON1,          MENU_BACK_X2, MENU_BACK_Y, 120, 50, 0, 0},
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
    GUI_SetColor(GUI_LINE_COLOR);
//    GUI_DrawRoundedRect(20, 68, 775, 200, 10);
//    GUI_DrawRoundedRect(20, 223, 775, 405, 10);

    My_DrawRoundedRect(20, 68, 775, 200, 10, 45, 150);
    My_DrawRoundedRect(20, 223, 775, 405, 10, 45, 150);
}
static void Update_Value(WM_HWIN hWin)
{
    int i = 0;

    if (ext_inf.din.pro_data[0].energy_sta == PL_DL645_97 || ext_inf.din.pro_data[0].energy_sta == PL_DL645_07) {
        for (i = 0; i < sizeof(ext_inf.din.pro_data[0].energy_addr); i++) {
            if (ext_inf.din.pro_data[0].energy_addr[i] < 0x30 || ext_inf.din.pro_data[0].energy_addr[i] > 0x39) {
                break;
            }
        }

        memcpy(show_buf, ext_inf.din.pro_data[0].energy_addr, sizeof(ext_inf.din.pro_data[0].energy_addr));
        show_buf[i] = 0;
        EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT0 ), show_buf);

        for (i = 0; i < sizeof(ext_inf.din.pro_data[1].energy_addr); i++) {
            if (ext_inf.din.pro_data[1].energy_addr[i] < 0x30 || ext_inf.din.pro_data[1].energy_addr[i] > 0x39) {
                break;
            }
        }

        memcpy(show_buf, ext_inf.din.pro_data[1].energy_addr, sizeof(ext_inf.din.pro_data[1].energy_addr));
        show_buf[i] = 0;
        EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT4 ), show_buf);
    } else {
        sprintf(show_buf, "%d", ext_inf.din.pro_data[0].energy_addr[0]);
        EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT0 ), show_buf);

        sprintf(show_buf, "%d", ext_inf.din.pro_data[1].energy_addr[0]);
        EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT4 ), show_buf);
    }

    sprintf(show_buf, "%d", ext_inf.din.pro_data[0].energy_ct);
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT1 ), show_buf);
}
static void Set_OptValue(WM_HWIN hWin, U8 sid)
{
    U8 i = 0;
    //增加是否启用

    switch (sid) {
    case ENERGY_MODE_MORE:
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT9));

        for (i = 0; i < 4; i++) {
            WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT10 + i));
        }

        for (i = 0; i < 3; i++) {
            WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT1 + i));
        }

        for (i = 0; i < 2; i++) {
            WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN15 + i));
        }

        for (i = 0; i < sizeof(ext_inf.din.pro_data[0].water_addr); i++) {
            if (ext_inf.din.pro_data[0].water_addr[i] < 0x30 || ext_inf.din.pro_data[0].water_addr[i] > 0x39) {
                break;
            }
        }

        WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT11));

        TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT11), GUI_LANG_GetTextEx(Lang_CT_Ratio, Language_Bit));

        WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT1));

        Update_Value(hWin);
        break;

    case WATER_MODE_CJT:
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT9));

        for (i = 0; i < 5; i++) {
            WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT10 + i));
        }

        for (i = 0; i < 3; i++) {
            WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT1 + i));
        }

        for (i = 0; i < 2; i++) {
            WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN15 + i));
        }

        for (i = 0; i < sizeof(ext_inf.din.pro_data[0].water_addr); i++) {
            if (ext_inf.din.pro_data[0].water_addr[i] < 0x30 || ext_inf.din.pro_data[0].water_addr[i] > 0x39) {
                break;
            }
        }

        memcpy(show_buf, ext_inf.din.pro_data[0].water_addr, sizeof(ext_inf.din.pro_data[0].water_addr));
        show_buf[i] = 0;
        EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT0), show_buf);
        break;

    case WATER_MODE_MB:
        sprintf(show_buf, "%d",  ext_inf.din.pro_data[0].water_addr[0]);
        EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT0), show_buf);

        break;

    case FOOD_MODE_MB:
        sprintf(show_buf, "%d",  ext_inf.din.pro_data[0].food_addr);
        EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT0), show_buf);

        break;

    default:
        break;

    }

    if (sid != ENERGY_MODE_MORE) {
        sprintf(show_buf, "%d",  ext_inf.din.mb_data[select_type].reg_addr);
        EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT1), show_buf);
    }

    sprintf(show_buf, "%d",  ext_inf.din.mb_data[select_type].dx.len);
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT2), show_buf);
    sprintf(show_buf, "%d",  ext_inf.din.mb_data[select_type].dx.point);
    EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT3), show_buf);


    DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN11), ext_inf.din.mb_data[select_type].baud);
    DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN12), ext_inf.din.mb_data[select_type].ux.dbits);
    DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN13), ext_inf.din.mb_data[select_type].ux.stopb);
    DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN14), ext_inf.din.mb_data[select_type].ux.parity);

    DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN15), ext_inf.din.mb_data[select_type].dx.mem);
    DROPDOWN_SetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN16), ext_inf.din.mb_data[select_type].dx.encode);
}

static void Get_OptValue(WM_HWIN hWin, U8 sid)
{
    ext_inf.din.mb_data[select_type].baud = DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN11));
    ext_inf.din.mb_data[select_type].ux.dbits = DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN12));
    ext_inf.din.mb_data[select_type].ux.stopb = DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN13));
    ext_inf.din.mb_data[select_type].ux.parity = DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN14));

    ext_inf.din.mb_data[select_type].dx.mem = DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN15));
    ext_inf.din.mb_data[select_type].dx.encode = DROPDOWN_GetSel(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN16));
}

static void DL645_Addr_Get(void)
{
    unsigned char *time_buf = Get_Input_Text();
    unsigned char len = strlen((const char *)time_buf);

    if (len == 0 || len > 12) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        return;
    }

//    if (stemp > 255) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = 255;
//    }

//    if (stemp < 0) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = 0;
//    }

    memcpy(ext_inf.din.pro_data[0].energy_addr, time_buf, len);

    if (len != 12)ext_inf.din.pro_data[0].energy_addr[len] = 0;

    Update_Value(Current_HWIN);
}

static void Modbus_Addr_Get(void)
{
    float ftemp = Get_Input_Value();
    unsigned short stemp = (unsigned short)ftemp;

    if (stemp > 255) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 255;
    }

//    if (stemp < 0) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = 0;
//    }
    ext_inf.din.pro_data[0].energy_addr[0] = stemp;
    Update_Value(Current_HWIN);
}

static void DL645_Addr2_Get(void)
{
    unsigned char *time_buf = Get_Input_Text();
    unsigned char len = strlen((const char *)time_buf);

    if (len == 0 || len > 12) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        return;
    }

//    if (stemp > 255) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = 255;
//    }

//    if (stemp < 0) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = 0;
//    }

    memcpy(ext_inf.din.pro_data[1].energy_addr, time_buf, len);

    if (len != 12)ext_inf.din.pro_data[1].energy_addr[len] = 0;

    Update_Value(Current_HWIN);
}

static void Modbus_Addr2_Get(void)
{
    float ftemp = Get_Input_Value();
    unsigned short stemp = (unsigned short)ftemp;

    if (stemp > 255) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 255;
    }

//    if (stemp < 0) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = 0;
//    }
    ext_inf.din.pro_data[1].energy_addr[0] = stemp;
    Update_Value(Current_HWIN);
}

static void CT_Get(void)
{
    float ftemp = Get_Input_Value();
    unsigned short stemp = (unsigned short)ftemp;

    if (stemp > 255) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 255;
    }

    if (stemp <= 0) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 1;
    }

    ext_inf.din.pro_data[0].energy_ct = stemp;
    Update_Value(Current_HWIN);
}
static void CJT_Addr_Get(void)
{
    unsigned char *time_buf = Get_Input_Text();
    unsigned char len = strlen((const char *)time_buf);
    unsigned char i = 0;

    if (len == 0 || len > 14) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        return;
    }

//    if (stemp > 255) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = 255;
//    }

//    if (stemp < 0) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = 0;
//    }

    memcpy(ext_inf.din.pro_data[0].water_addr, time_buf, len);

    if (len != 14)ext_inf.din.pro_data[0].water_addr[len] = 0;

    for (i = 0; i < sizeof(ext_inf.din.pro_data[0].water_addr); i++) {
        if (ext_inf.din.pro_data[0].water_addr[i] < 0x30 || ext_inf.din.pro_data[0].water_addr[i] > 0x39) {
            break;
        }
    }

    memcpy(show_buf, ext_inf.din.pro_data[0].water_addr, sizeof(ext_inf.din.pro_data[0].water_addr));
    show_buf[i] = 0;
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, GUI_ID_EDIT0), show_buf);
}

static void Water_Addr_Get(void)
{
    float ftemp = Get_Input_Value();
    unsigned short stemp = (unsigned short)ftemp;

    if (stemp > 255) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 255;
    }

//    if (stemp < 0) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = 0;
//    }
    ext_inf.din.pro_data[0].water_addr[0] = stemp;
    sprintf(show_buf, "%d",  ext_inf.din.pro_data[0].water_addr[0]);
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, GUI_ID_EDIT0), show_buf);
}

static void Food_Addr_Get(void)
{
    float ftemp = Get_Input_Value();
    unsigned short stemp = (unsigned short)ftemp;

    if (stemp > 255) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 255;
    }

//    if (stemp < 0) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = 0;
//    }
    ext_inf.din.pro_data[0].food_addr = stemp;
    sprintf(show_buf, "%d",  ext_inf.din.pro_data[0].food_addr);
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, GUI_ID_EDIT0), show_buf);
}

static void Reg_Addr_Get(void)
{
    float ftemp = Get_Input_Value();
    unsigned short stemp = (unsigned short)ftemp;

//    if (stemp > 9) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = 9;
//    }

//    if (stemp < 0) {
//        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
//        stemp = 0;
//    }

    ext_inf.din.mb_data[select_type].reg_addr = stemp;
    sprintf(show_buf, "%d",  ext_inf.din.mb_data[select_type].reg_addr);
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, GUI_ID_EDIT1), show_buf);
}

static void Data_Len_Get(void)
{
    float ftemp = Get_Input_Value();
    short stemp = (short)ftemp;

    if (stemp > 9) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 9;
    }

    if (stemp < 0) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 0;
    }

    ext_inf.din.mb_data[select_type].dx.len = stemp;
    sprintf(show_buf, "%d",  ext_inf.din.mb_data[select_type].dx.len);
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, GUI_ID_EDIT2), show_buf);
}

static void Data_Point_Get(void)
{
    float ftemp = Get_Input_Value();
    short stemp = (short)ftemp;

    if (stemp > 9) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 9;
    }

    if (stemp < 0) {
        MWarning_Task(Current_HWIN, Get_Tips(TIPS_LIMIT_ER));
        stemp = 0;
    }

    ext_inf.din.mb_data[select_type].dx.point = stemp;
    sprintf(show_buf, "%d",  ext_inf.din.mb_data[select_type].dx.point);
    EDIT_SetText(WM_GetDialogItem(Current_HWIN, GUI_ID_EDIT3), show_buf);
}

static void InitDialog(WM_MESSAGE * pMsg)
{
    WM_HWIN hWin = pMsg->hWin;
    int MDev_ID = 0;
    WM_HWIN hItem;

    for (MDev_ID = GUI_ID_BUTTON2; MDev_ID <= GUI_ID_BUTTON2; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, Cut_cbButton);
    }

    for (MDev_ID = GUI_ID_BUTTON0; MDev_ID <= GUI_ID_BUTTON1; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Button_SetHZ24(hItem);
    }

    WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON0));

    for (MDev_ID = GUI_ID_TEXT0; MDev_ID <= GUI_ID_TEXT9; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24L(hItem);

        if (MDev_ID == GUI_ID_TEXT0 || MDev_ID == GUI_ID_TEXT1 || MDev_ID == GUI_ID_TEXT7) {
            TEXT_SetTextAlign(hItem,  GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetTextColor(hItem, GUI_WHITE);
        }

        if (MDev_ID == GUI_ID_TEXT0) {
            sprintf(show_buf, "%s-%s", GUI_LANG_GetTextEx(Lang_Device, Language_Bit), GUI_LANG_GetTextEx(Lang_Detail_Set, Language_Bit));
            TEXT_SetText(hItem, show_buf); // 设备管理-详细设置
            continue;
        }

        if ((MDev_ID != GUI_ID_TEXT8)) {
            sprintf(show_buf, "%s:", GUI_LANG_GetTextEx(Lang_Buf[MDev_ID - GUI_ID_TEXT1], Language_Bit));
        } else {
            sprintf(show_buf, "%s%d:", GUI_LANG_GetTextEx(Lang_Buf[MDev_ID - GUI_ID_TEXT1], Language_Bit), 1); // 设备地址1
        }
        TEXT_SetText(hItem, show_buf);
    }


    for (MDev_ID = GUI_ID_TEXT10; MDev_ID <= GUI_ID_TEXT14; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Text_SetHZ24L(hItem);
        if ((MDev_ID != GUI_ID_TEXT14)) {
            sprintf(show_buf, "%s:", GUI_LANG_GetTextEx(Lang_Buf[MDev_ID - GUI_ID_TEXT10 + 9], Language_Bit));
        } else {
            sprintf(show_buf, "%s%d:", GUI_LANG_GetTextEx(Lang_Buf[MDev_ID - GUI_ID_TEXT10 + 9], Language_Bit), 2); // 设备地址1
        }
        TEXT_SetText(hItem, show_buf);
    }

    for (MDev_ID = GUI_ID_EDIT0; MDev_ID <= GUI_ID_EDIT4; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Edit_SetHZ24(hItem);
    }

    EDIT_SetMaxLen(WM_GetDialogItem(hWin, GUI_ID_EDIT0), 15);
    EDIT_SetMaxLen(WM_GetDialogItem(hWin, GUI_ID_EDIT4), 15);

    for (MDev_ID = GUI_ID_DROPDOWN10; MDev_ID <= GUI_ID_DROPDOWN16; MDev_ID++) {
        hItem = WM_GetDialogItem(hWin, MDev_ID);
        Dropdown_SetHZ24(hItem);
        Login_Show(hItem, LOGIN_ADMIN);
    }

    sprintf(show_buf, "%s%d", GUI_LANG_GetTextEx(Lang_UART, Language_Bit), 3);
    DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN10), show_buf);

#if USE_GD == 1
    if (basic_inf.comm_info.ux.port == 1) {
        sprintf(show_buf, "%s%d", GUI_LANG_GetTextEx(Lang_UART, Language_Bit), 1);
        DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN10), show_buf);
    }
#endif

    WM_DisableWindow(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN10));

    DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN11), "1200");
    DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN11), "2400");
    DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN11), "4800");
    DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN11), "9600");
    DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN11), "19200");
    DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN11), "38400");
    DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN11), "57600");
    DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN11), "115200");

    DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN12), "7");
    DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN12), "8");

    DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN13), "1");
    DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN13), "1.5");
    DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN13), "2");

    DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN14), GUI_LANG_GetTextEx(Lang_NO_check, Language_Bit));   // 无效验
    DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN14), GUI_LANG_GetTextEx(Lang_Odd_check, Language_Bit));  // 奇效验
    DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN14), GUI_LANG_GetTextEx(Lang_Even_check, Language_Bit)); // 偶效验

    DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN15), "B1234");
    DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN15), "B4321");
    DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN15), "B2143");

    DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN16), "HEX");
    DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN16), "BCD");
    DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN16), "FLOAT");
    DROPDOWN_AddString(WM_GetDialogItem(hWin, GUI_ID_DROPDOWN16), "ASCII");

    switch (select_num) {
    case WATER_MODE_CJT:
        BUTTON_SetText(WM_GetDialogItem(hWin, GUI_ID_BUTTON1), GUI_LANG_GetTextEx(Lang_Fan_Sta, Language_Bit));
    case WATER_MODE_MB:
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT14));
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT4));
        BUTTON_SetText(WM_GetDialogItem(hWin, GUI_ID_BUTTON1), GUI_LANG_GetTextEx(Lang_More_Settings, Language_Bit));
        select_type = MB_DATA_WATER;
        break;

    case FOOD_MODE_MB:
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT14));
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_EDIT4));

        BUTTON_SetText(WM_GetDialogItem(hWin, GUI_ID_BUTTON0), GUI_LANG_GetTextEx(Lang_Feed_Bins_ADJ, Language_Bit));
        BUTTON_SetText(WM_GetDialogItem(hWin, GUI_ID_BUTTON1), GUI_LANG_GetTextEx(Lang_More_Settings, Language_Bit));

        if (ext_inf.din.pro_data[0].food_sta == FL_MB_XK3102) {
            WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON0));
        }

        select_type = MB_DATA_FOOD;
        break;

    case ENERGY_MODE_MORE:
        BUTTON_SetText(WM_GetDialogItem(hWin, GUI_ID_BUTTON1), GUI_LANG_GetTextEx(Lang_Fan_Sta, Language_Bit));
        select_type = MB_DATA_ENERGY;
        break;

    default:
        break;
    }

    Set_OptValue(hWin, select_num);
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
                if (select_num == ENERGY_MODE_MORE) {
                    if (ext_inf.din.pro_data[0].energy_sta == PL_DL645_97 || ext_inf.din.pro_data[0].energy_sta == PL_DL645_07) {
                        Set_Output_Callback(DL645_Addr_Get);
                        Set_Input_Len(12);
                    } else {
                        Set_Output_Callback(Modbus_Addr_Get);
                        Set_Input_Len(3);
                    }
                } else if (select_num == WATER_MODE_CJT) {
                    Set_Output_Callback(CJT_Addr_Get);
                    Set_Input_Len(14);
                } else if (select_num == WATER_MODE_MB) {
                    Set_Output_Callback(Water_Addr_Get);
                    Set_Input_Len(3);
                } else {
                    Set_Output_Callback(Food_Addr_Get);
                    Set_Input_Len(3);
                }

                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT1:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                if (select_num == ENERGY_MODE_MORE) {
                    Set_Output_Callback(CT_Get);
                    Set_Input_Len(3);
                    Input_Task(hWin);
                    break;
                }

                Set_Output_Callback(Reg_Addr_Get);
                Set_Input_Len(5);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT2:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(Data_Len_Get);
                Set_Input_Len(1);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT3:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Set_Output_Callback(Data_Point_Get);
                Set_Input_Len(1);
                Input_Task(hWin);
            }

            break;

        case GUI_ID_EDIT4:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                if (select_num == ENERGY_MODE_MORE) {
                    if (ext_inf.din.pro_data[0].energy_sta == PL_DL645_97 || ext_inf.din.pro_data[0].energy_sta == PL_DL645_07) {
                        Set_Output_Callback(DL645_Addr2_Get);
                        Set_Input_Len(12);
                    } else {
                        Set_Output_Callback(Modbus_Addr2_Get);
                        Set_Input_Len(3);
                    }
                }

                Input_Task(hWin);
            }

            break;

        case GUI_ID_BUTTON0:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                if (select_num == FOOD_MODE_MB) {
                    MDev_FoodADJ_Task(hWin, 0);
                }
            }

            break;

        case GUI_ID_BUTTON1:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                if (select_num == WATER_MODE_CJT || select_num == WATER_MODE_MB) {
                    MDev_ProduceMore_Task(hWin, select_num);
                } else if (select_num == FOOD_MODE_MB) {
                    MDev_ProduceMore_Task(hWin, select_num);
                } else {
                    MDev_FanChk_Task(hWin);
                }
            }

            break;

        case GUI_ID_BUTTON2:
            if (NCode == WM_NOTIFICATION_RELEASED) {
                Get_OptValue(hWin, select_num);

                if (Config_Save(SDIN_ID) < 0) {
                    MWarning_Task(WM_GetParent(hWin), Get_Tips(TIPS_SAVE_ER));
                }

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
WM_HWIN MDev_ModbusSet;
void MDev_ModbusSet_Task(WM_HWIN hParent, U8 id)
{
    select_num = id;//选择哪个
    MDev_ModbusSet = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hParent, 0, 0);
    Current_HWIN = MDev_ModbusSet;
}
