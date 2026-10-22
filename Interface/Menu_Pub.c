#include "menuinc.h"
#include "w25qxx.h"
#include <string.h>

#if  USE_VS == 0
#include "DataSave.h"
#endif /* #if USE_VS == 0 */

WM_HWIN Current_HWIN;
char show_buf[256];
const u16 room_type[ROOM_TYPE_NUM] = {Lang_Boar, Lang_Reserve, Lang_Pregnancy, Lang_Childbirth, Lang_Child_care, Lang_Fatten, Lang_Sow,
                                      Lang_Breeding_hen, Lang_Layer, Lang_Broiler, Lang_Brood, Lang_Breed, Lang_Adult_chicken,
                                      Lang_Meat_duck, Lang_Laying_duck, Lang_BreedDuck, Lang_Laying, Lang_Quail};
const char fan_size[FAN_SIZE_NUM][8] = {"18", "24", "36", "50", "51", "54"};
int old_Id;//防止点击下拉列表的同时点击了edit

//返回上一页下一页按键
static const u16 Lang_KEY_NAME[] = {Lang_Return, Lang_PgDown, Lang_PgUp};


static const char * KEY_PIC[] =  {"85iconAll_Return", "86iconAll_NextPage",

                                  "87iconAll_UpPage"
                                 };
static unsigned char PAGE[3] =   {PAGE_UP, PAGE_DOWN, PAGE_RETURN};
//函数指针用来修改显示文本
void (*Set_Text[TYPE_NUM])() = {
    (void (*)())BUTTON_SetText,
    (void (*)())TEXT_SetText,
    EDIT_SetText,
    CHECKBOX_SetText
};
const char number_id[11][8] = {"1.", "1.", "2.", "3.", "4.", "5.", "6.", "7.", "8.", "9.", "10."};
static char menu_show_buf[48];
void MChange_Menu(void (*Value_Init)(WM_HMEM, U8), WM_HMEM hWin, PCHG_MENU pmenu, U8 sid)
{
    U8 i = 0;
    PCHG_MENU menu = (PCHG_MENU) &pmenu[i];

    if (menu == NULL && Value_Init == NULL && hWin == 0)
        return;

    Value_Init(hWin, sid);

    while (1) {
        if (menu->isshow == MEXIT) {
            break;
        }

        if (menu->isshow == MHIDE) {
            WM_HideWindow(WM_GetDialogItem(hWin, menu->hMem));
            menu = (PCHG_MENU) &pmenu[++i];
            continue;
        }

        WM_ShowWindow(WM_GetDialogItem(hWin, menu->hMem));

        if (menu->scale == 1) { //原数据
            sprintf(menu_show_buf, (const char *) menu->text, menu->value);
        } else if (menu->scale == SNUM_FLAG) {
            sprintf(menu_show_buf, (const char *) menu->text, number_id[menu->value]);
        } else {
            sprintf(menu_show_buf, (const char *) menu->text, (float)(menu->value * 1.0f / menu->scale));
        }

        Set_Text[menu->type](WM_GetDialogItem(hWin, menu->hMem), menu_show_buf);
        menu = (PCHG_MENU) &pmenu[++i];
    }

}
const int tips_menu[TIPS_MAX] = {
    Lang_Tips0,  // 数据存储错误
    Lang_Tips1,  // 数据超出限制
    Lang_Tips2,  // 时间设置成功
    Lang_Tips3,  // 密码错误
    Lang_Tips4,  // 登陆成功
    Lang_Tips5,  // 退出登陆
    Lang_Tips6,  // 请登陆后操作
    Lang_Tips7,  // 参数不能修改
    Lang_Tips9,  // 校准成功
    Lang_Tips10, // 校准失败
    Lang_Tips13,
    Lang_Tips14
};


const char *Get_Tips(unsigned char ID)
{
    return GUI_LANG_GetTextEx(tips_menu[ID], Language_Bit);
}

//void Button_SetHZ32(BUTTON_Handle hObj)
//{
//    BUTTON_SetTextAlign(hObj,  GUI_TA_HCENTER | GUI_TA_VCENTER);
//    BUTTON_SetBkColor(hObj, BUTTON_CI_UNPRESSED, GUI_BUTTON_COLOR);
//    BUTTON_SetFont(hObj, &GUI_USER_HZ32);
//}
//void Text_SetHZ32(TEXT_Handle hObj)
//{
//    TEXT_SetTextAlign(hObj,  GUI_TA_HCENTER | GUI_TA_VCENTER);
//    TEXT_SetFont(hObj, &GUI_USER_HZ32);
//}
void Button_SetHZ24(BUTTON_Handle hObj)
{
    BUTTON_SetTextAlign(hObj,  GUI_TA_HCENTER | GUI_TA_VCENTER);
    BUTTON_SetBkColor(hObj, BUTTON_CI_UNPRESSED, GUI_BUTTON_COLOR);
    BUTTON_SetFont(hObj, &GUI_USER_HZ24);
    BUTTON_SetTextColor(hObj, BUTTON_CI_UNPRESSED, GUI_WHITE);

    if (Language_Bit_original == LANGUAGE_ENGLISH) {
        BUTTON_SetFont(hObj, &GUI_USER_HZ16);
    }else if(Language_Bit_original>= LANGUAGE_RUSSIAN) {
        BUTTON_SetFont(hObj, &GUI_FontBC_16);
	}
}
void Text_SetHZ24(TEXT_Handle hObj)
{
    TEXT_SetTextAlign(hObj,  GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetFont(hObj, &GUI_USER_HZ24);
    TEXT_SetTextColor(hObj, GUI_WHITE);
    WM_DisableWindow(hObj);

    if (Language_Bit_original == LANGUAGE_ENGLISH) {
        TEXT_SetFont(hObj, &GUI_USER_HZ16);
    }
	if(Language_Bit_original >= LANGUAGE_RUSSIAN) {
        TEXT_SetFont(hObj, &GUI_FontBC_16);		
	}	
}
void Text_SetHZ24L(TEXT_Handle hObj)
{
    TEXT_SetTextAlign(hObj,  GUI_TA_LEFT | GUI_TA_VCENTER);
    TEXT_SetFont(hObj, &GUI_USER_HZ24);
    TEXT_SetTextColor(hObj, GUI_WHITE);
    WM_DisableWindow(hObj);

    if (Language_Bit_original== LANGUAGE_ENGLISH) {
        TEXT_SetFont(hObj, &GUI_USER_HZ16);
    } else if(Language_Bit_original >= LANGUAGE_RUSSIAN) {
        TEXT_SetFont(hObj, &GUI_FontBC_16);
	}
}

void Check_SetHZ24(CHECKBOX_Handle hObj)
{
    CHECKBOX_SetTextColor(hObj, GUI_WHITE);
    CHECKBOX_SetFont(hObj, &GUI_USER_HZ24);
    CHECKBOX_SetSkinFlexButtonSize(hObj, 28);

    if (Language_Bit_original == LANGUAGE_ENGLISH) {
        CHECKBOX_SetFont(hObj, &GUI_USER_HZ16);
    } else if(Language_Bit_original >= LANGUAGE_RUSSIAN) {
        CHECKBOX_SetFont(hObj, &GUI_FontBC_16);
	}
}
void Edit_SetHZ24(EDIT_Handle hObj)
{
    EDIT_SetBkColor(hObj, 0, GUI_BACK_COLOR);
    EDIT_SetFont(hObj, &GUI_USER_HZ24);
    EDIT_SetTextAlign(hObj, GUI_TA_VCENTER | GUI_TA_CENTER);

    if (Language_Bit_original == LANGUAGE_ENGLISH) {
        EDIT_SetFont(hObj, &GUI_USER_HZ16);
    } else if(Language_Bit_original >= LANGUAGE_RUSSIAN) {
        EDIT_SetFont(hObj, &GUI_USER_HZ16);
	}
}

void Dropdown_SetHZ24(DROPDOWN_Handle hObj)
{
    DROPDOWN_SetTextHeight(hObj, 28);
    DROPDOWN_SetItemSpacing(hObj, 5);
    DROPDOWN_SetBkColor(hObj, 0, GUI_BACK_COLOR);
    DROPDOWN_SetFont(hObj, &GUI_USER_HZ24);
    DROPDOWN_SetAutoScroll(hObj, 1);
    DROPDOWN_SetTextAlign(hObj, GUI_TA_VCENTER | GUI_TA_CENTER);
    DROPDOWN_SetScrollbarWidth(hObj, 24);

    if (Language_Bit_original == LANGUAGE_ENGLISH) {
        DROPDOWN_SetFont(hObj, &GUI_USER_HZ16);
        DROPDOWN_SetItemSpacing(hObj, 15);
    }
	if(Language_Bit_original >= LANGUAGE_RUSSIAN) {
        DROPDOWN_SetFont(hObj, &GUI_FontBC_16);
        DROPDOWN_SetItemSpacing(hObj, 8);
	}	
}


GUI_CONST_STORAGE GUI_BITMAP hBakpic = {
    800, 	// xSize
    480, 	// ySize
    1600,	// BytesPerLine
    16, 	// BitsPerPixel
    (const unsigned char *)(SDRAM_PIC_ADDR + (BACK_ID - 2) * PIC_SIZE),
    NULL, // Pointer to palette
    GUI_DRAW_BMPM565
};

void Show_MBackPic(void)
{
    GUI_DrawBitmap(&hBakpic, 0, 0);
}

GUI_CONST_STORAGE GUI_BITMAP InBakpic = {
    800, 	// xSize
    480, 	// ySize
    1600,	// BytesPerLine
    16, 	// BitsPerPixel
    (const unsigned char *)(SDRAM_PIC_ADDR + (WBG_ID - 1) * PIC_SIZE),
    NULL, // Pointer to palette
    GUI_DRAW_BMPM565
};

void Show_INBackPic(void)
{
    GUI_DrawBitmap(&InBakpic, 0, 0);
}

int Config_Save(U8 id)
{
    int ires = 0;
#if USE_VS == 0
    ires = EEPROM_SaveData(id);
#endif

    if (ires == 0) ires = -1; //特殊处理结果0

    return ires;
}
void Login_Show(WM_HWIN hWin, U8 NeedType)
{
    if (sys_sta.login_type == LOGIN_GUEST) {
        WM_DisableWindow(hWin);
    } else if (NeedType == LOGIN_SUPADMIN && sys_sta.login_type != LOGIN_SUPADMIN) {
        WM_DisableWindow(hWin);
    } else {
        WM_EnableWindow(hWin);
    }
}

int get_login_permiss(U8 NeedType)
{
    if (sys_sta.login_type == LOGIN_GUEST) {
        return -1;
    } else if (NeedType == LOGIN_SUPADMIN && sys_sta.login_type != LOGIN_SUPADMIN) {
        return -1;
    } else {
        return 1;
    }
}

void Button_Menu(int Id, WM_HWIN hWin, int CurID)
{
    if (Id == CurID) {
        return;
    }

    if (sys_sta.trial_end == STA_ON && Id != GUI_ID_BUTTON1_0) {
        return;
    }

    switch (Id) {

    case GUI_ID_BUTTON1_0:
        Main_Buffer = Main_Buffer_Home;
        Current_HWIN = WM_GetParent(hWin);
        GUI_EndDialog(hWin, 0);
        break;

    case GUI_ID_BUTTON1_1:
        GUI_EndDialog(hWin, 0);
        MDev_Man_Task(WM_GetParent(hWin));

        break;

    case GUI_ID_BUTTON1_2:
        GUI_EndDialog(hWin, 0);
        MCtrl_Man_Task(WM_GetParent(hWin));

        break;

    case GUI_ID_BUTTON1_3:
        GUI_EndDialog(hWin, 0);
        MSys_Man_Task(WM_GetParent(hWin));

        break;

    case GUI_ID_BUTTON1_4:
        GUI_EndDialog(hWin, 0);
        MData_Record_Task(WM_GetParent(hWin));

        break;

    case GUI_ID_BUTTON1_5:
        GUI_EndDialog(hWin, 0);
        MAla_Record_Task(WM_GetParent(hWin));

        break;

    case GUI_ID_BUTTON1_6:
        GUI_EndDialog(hWin, 0);
        MUser_Man_Task(WM_GetParent(hWin));

        break;

    default:
        break;
    }
}
void Disp_House(void)
{
    GUI_defDrawBitmap("17bmMPic_HouseG", 21, 427);
}

void Button_CurShow(WM_HWIN hWin, U8 sid, U8 cur_num)
{
    if (hWin == NULL || cur_num == 0) {
        return ;
    }

    if (sid == 0) {
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON0));
        WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON1));
    } else if (sid == cur_num - 1) {
        WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON1));
        WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON0));
    } else {
        WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON0));
        WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON1));
    }
}

void Detail_cbButton(WM_MESSAGE * pMsg)
{
    WM_HWIN  hWin;
    hWin  = pMsg->hWin;

    switch (pMsg->MsgId) {
    case WM_PAINT:
        if (BUTTON_IsPressed(hWin)) {
            GUI_SetBkColor(GUI_WHITE);
            GUI_SetAlpha(0xb0);
            GUI_defDrawBitmap("37bmMPic_Detail", 25, 10);
            GUI_SetAlpha(0);
        } else {
            GUI_defDrawBitmap("37bmMPic_Detail", 25, 10);
        }

        break;

    default:
        BUTTON_Callback(pMsg);
    }
}

void Curve_cbButton(WM_MESSAGE * pMsg)
{
    WM_HWIN  hWin;
    hWin  = pMsg->hWin;

    switch (pMsg->MsgId) {
    case WM_PAINT:
        if (BUTTON_IsPressed(hWin)) {
            GUI_SetBkColor(GUI_WHITE);
            GUI_SetAlpha(0xb0);
            GUI_defDrawBitmap("57Mpic_Curve", 25, 10);
            GUI_SetAlpha(0);
        } else {
            GUI_defDrawBitmap("57Mpic_Curve", 25, 10);
        }

        break;

    default:
        BUTTON_Callback(pMsg);
    }
}

void next_cbButton(WM_MESSAGE * pMsg)
{
    WM_HWIN  hWin;
    hWin  = pMsg->hWin;

    switch (pMsg->MsgId) {
    case WM_PAINT:
        if (BUTTON_IsPressed(hWin)) {
            GUI_SetBkColor(GUI_WHITE);
            GUI_SetAlpha(0xb0);
            GUI_defDrawBitmap("95bmMPic_Cut", 30, 10);
            GUI_SetAlpha(0);
        } else {
            GUI_defDrawBitmap("95bmMPic_Cut", 30, 10);
        }

        break;

    default:
        BUTTON_Callback(pMsg);
    }
}

void set_cbButton(WM_MESSAGE * pMsg)
{
    WM_HWIN  hWin;
    hWin  = pMsg->hWin;

    switch (pMsg->MsgId) {
    case WM_PAINT:
        if (BUTTON_IsPressed(hWin)) {
            GUI_SetBkColor(GUI_WHITE);
            GUI_SetAlpha(0xb0);
            GUI_defDrawBitmap("99Mpic_Set", 30, 10);
            GUI_SetAlpha(0);
        } else {
            GUI_defDrawBitmap("99Mpic_Set", 30, 10);
        }

        break;

    default:
        BUTTON_Callback(pMsg);
    }
}

void Cut_cbButton(WM_MESSAGE * pMsg)
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
            GUI_defDrawBitmap("51Mpic_ReturenBack", 0, 0);
            GUI_defDrawBitmap(KEY_PIC[PAGE[Id - GUI_ID_BUTTON0]], 5, 10);

            ButtonText_Show(WM_GetDialogItem(hWin, Id), GUI_LANG_GetTextEx(Lang_KEY_NAME[PAGE[Id - GUI_ID_BUTTON0]], Language_Bit), 40, 10);

            GUI_SetAlpha(0);
        } else {
            GUI_defDrawBitmap("51Mpic_ReturenBack", 0, 0);
            GUI_defDrawBitmap(KEY_PIC[PAGE[Id - GUI_ID_BUTTON0]], 5, 10);

            ButtonText_Show(WM_GetDialogItem(hWin, Id), GUI_LANG_GetTextEx(Lang_KEY_NAME[PAGE[Id - GUI_ID_BUTTON0]], Language_Bit), 40, 10);
        }

        break;

    default:
        BUTTON_Callback(pMsg);
    }
}

void _cbButtonBack(WM_MESSAGE * pMsg)
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
            GUI_defDrawBitmap("51Mpic_ReturenBack", 0, 0);
            GUI_defDrawBitmap(KEY_PIC[PAGE[Id - GUI_ID_BUTTON0]], 5, 10);

            ButtonText_Show(WM_GetDialogItem(hWin, Id), GUI_LANG_GetTextEx(Lang_KEY_NAME[PAGE[Id - GUI_ID_BUTTON0]], Language_Bit), 40, 10);
            GUI_SetAlpha(0);
        } else {
            GUI_defDrawBitmap("51Mpic_ReturenBack", 0, 0);
            GUI_defDrawBitmap(KEY_PIC[PAGE[Id - GUI_ID_BUTTON0]], 5, 10);

            ButtonText_Show(WM_GetDialogItem(hWin, Id), GUI_LANG_GetTextEx(Lang_KEY_NAME[PAGE[Id - GUI_ID_BUTTON0]], Language_Bit), 40, 10);
        }

        break;

    default:
        BUTTON_Callback(pMsg);
    }
}

void ButtonText_Show(BUTTON_Handle pObj, const char * s, int x, int y)
{
    GUI_RECT r = {0};

    if (s == NULL)
        return;

    WM_GetClientRect(&r);

    r.x0 += x;
    r.y0 += y;

    if (Language_Bit_original >= LANGUAGE_RUSSIAN) {
        GUI_SetFont(&GUI_FontBC_16);
    } else if (Language_Bit_original == LANGUAGE_ENGLISH) {
        GUI_SetFont(&GUI_USER_HZ16);
    } else {
        GUI_SetFont(&GUI_USER_HZ24);
    }

    GUI_SetColor(GUI_WHITE);
    GUI_SetTextMode(GUI_TM_TRANS);
    GUI_DispStringInRect(s, &r, GUI_TA_TOP | GUI_TA_CENTER);

}

static void Main_ButtonText_Show(BUTTON_Handle pObj, const char *s, int x, int y, u32 color)
{
    GUI_RECT r = {0};

    if (s == NULL)
        return;

    WM_GetClientRect(&r);

    r.x0 += x;
    r.y0 += y;

    if (Language_Bit_original >= LANGUAGE_RUSSIAN) {
        GUI_SetFont(&GUI_FontBC_16);
		r.y0 -= 5;
    } else {
        GUI_SetFont(&GUI_USER_HZ16);
    }

    GUI_SetColor(color);
    GUI_SetTextMode(GUI_TM_TRANS);
    GUI_DispStringInRect(s, &r, GUI_TA_TOP | GUI_TA_LEFT);
}

const char *Main_MPIC[2][7] = {
    {"50bmMPic_Outhome", "bmMpic_DevW16", "bmMpic_CtrlW16", "bmMpic_SysW16", "bmMpic_HistoryW16", "bmMpic_alaW16", "bmMpic_loginW16"},
    {"49bmMPic_Homing", "bmMpic_DevG16", "bmMpic_CtrlG16", "bmMpic_SysG16", "bmMpic_HistoryG16", "bmMpic_alaG16", "bmMpic_loginG16"}};
u16 Lang_MainKEY_NAME[6] = {Lang_Device,Lang_Control,Lang_System,Lang_History,Lang_ALA_Record,Lang_User};
u16 Main_Buffer = 0;
void Main_cbButton(WM_MESSAGE *pMsg)
{
    WM_HWIN hWin;
    int Id;
    hWin = pMsg->hWin;
    Id = WM_GetId(hWin);

    switch (pMsg->MsgId) {
    case WM_PAINT:
        if (BUTTON_IsPressed(hWin)) {
            GUI_SetBkColor(GUI_WHITE);
            GUI_SetAlpha(0xb0);

            if (Main_Buffer == (Id - GUI_ID_BUTTON1_0)) {
                if (Id > GUI_ID_BUTTON1_0) {
                    Main_ButtonText_Show(WM_GetDialogItem(hWin, Id), GUI_LANG_GetTextEx(Lang_MainKEY_NAME[Id - GUI_ID_BUTTON1_1], Language_Bit), 27, 45, GUI_GREEN);
                    GUI_defDrawBitmap(Main_MPIC[1][(Id - GUI_ID_BUTTON1_0)], 5, 47);
                } else {
                    GUI_defDrawBitmap(Main_MPIC[1][(Id - GUI_ID_BUTTON1_0)], 5, 45);
                }
            } else {
                if (Id > GUI_ID_BUTTON1_0) {
                    Main_ButtonText_Show(WM_GetDialogItem(hWin, Id), GUI_LANG_GetTextEx(Lang_MainKEY_NAME[Id - GUI_ID_BUTTON1_1], Language_Bit), 27, 45, GUI_WHITE);
                    GUI_defDrawBitmap(Main_MPIC[0][(Id - GUI_ID_BUTTON1_0)], 5, 47);
                } else {
                    GUI_defDrawBitmap(Main_MPIC[0][(Id - GUI_ID_BUTTON1_0)], 5, 45);
                }
            }
            GUI_SetAlpha(0);
        } else {
            if (Main_Buffer == (Id - GUI_ID_BUTTON1_0)) {
                if (Id > GUI_ID_BUTTON1_0) {
                    Main_ButtonText_Show(WM_GetDialogItem(hWin, Id), GUI_LANG_GetTextEx(Lang_MainKEY_NAME[Id - GUI_ID_BUTTON1_1], Language_Bit), 27, 45, GUI_GREEN);
                    GUI_defDrawBitmap(Main_MPIC[1][(Id - GUI_ID_BUTTON1_0)], 5, 47);
                } else {
                    GUI_defDrawBitmap(Main_MPIC[1][(Id - GUI_ID_BUTTON1_0)], 5, 45);
                }
            } else {
                if (Id > GUI_ID_BUTTON1_0) {
                    Main_ButtonText_Show(WM_GetDialogItem(hWin, Id), GUI_LANG_GetTextEx(Lang_MainKEY_NAME[Id - GUI_ID_BUTTON1_1], Language_Bit), 27, 45, GUI_WHITE);
                    GUI_defDrawBitmap(Main_MPIC[0][(Id - GUI_ID_BUTTON1_0)], 5, 47);
                } else {
                    GUI_defDrawBitmap(Main_MPIC[0][(Id - GUI_ID_BUTTON1_0)], 5, 45);
                }
            }
        }

        break;

    default:
        BUTTON_Callback(pMsg);
    }
}

u16 Get_UTF8TEXT_LEN(const char *_Str)
{
    u16 i = 0, textlen = 0;
    for (i = 0; i < strlen(_Str); i++) {
        if (_Str[i] <= 0x7F) {
            textlen++;
        } else {
            textlen++;
            i++;
        }
    }
    return textlen;
}
