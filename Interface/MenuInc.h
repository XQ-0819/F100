#ifndef _MENUINC_H
#define _MENUINC_H
//#include "Var_Init.h"
#include <stdio.h>

#include <stddef.h>
#include "GUI.h"
#include "DIALOG.h"


#include "WM.h"
#include "BUTTON.h"
#include "CHECKBOX.h"
#include "DROPDOWN.h"
#include "EDIT.h"
#include "FRAMEWIN.h"
#include "LISTBOX.h"
#include "MULTIEDIT.h"
#include "RADIO.h"
#include "SLIDER.h"
#include "TEXT.h"
#include "PROGBAR.h"
#include "SCROLLBAR.h"
#include "LISTVIEW.h"
#include "parameters.h"
#include "mmessage.h"
#include "app.h"
#include "language.h"
#define USE_VS				(0)

#define SWIN_TYPE		(0)
#define WIN_TYPE		(1)
#define PWM_FAN_TYPE	(2)
#define JDQ_FAN_TYPE	(3)

#define USER_BUF		(8)

#define POINTS_ER	(-99)
#define SNUM_FLAG	(64)
#define TEST_HOLD_TIME		(300000)
#define GUI_MENU_COLOR 		GUI_GREEN
#define GUI_LINE_COLOR 		GUI_DARKCYAN
#define GUI_BACK_COLOR		0x00FCF8F7 //0xd4d6b6 F7F8FC
#define GUI_BUTTON_COLOR 	0x00a69e40
#define GUI_FOUCE_COLOR 	GUI_YELLOW
#define GUI_DEF_GRAY 		0x00EFEFEF
#define GUI_DEF_LGREEN 		0x00FFFFF0
#define GUI_DEF_LRED 		0x00E1E3FF

#define GUI_DEF_LGRAY 		0x00F0F0F0

#define GUI_DEF_RED			0x00553BDB
#define GUI_DEF_BLUE		0x00917927
#define GUI_DEF_GREEN		0x0020C38F
#define GUI_DEF_LGREENB		0x0095EED1
#define GUI_DEF_DUCKGREEN   0x001b6d0c

//????¡§?????
#define GUI_DEF_BUTTON      0x0086511E

#define CHECK_SIZE			(30)

extern BUTTON_SKINFLEX_PROPS BUTTON_Props_Def_EN;
extern BUTTON_SKINFLEX_PROPS BUTTON_Props_Def_PR;
extern BUTTON_SKINFLEX_PROPS BUTTON_Props_Set_EN;
extern BUTTON_SKINFLEX_PROPS BUTTON_Props_Set_PR;

extern GUI_CONST_STORAGE  GUI_FONT GUI_FontYH_24;
extern GUI_CONST_STORAGE  GUI_FONT GUI_FontYH_16;
extern GUI_CONST_STORAGE  GUI_FONT GUI_FontYH_36;
//extern GUI_CONST_STORAGE  GUI_FONT GUI_FontYH_32;


#define GUI_USER_HZ36 	GUI_FontYH_36
//#define GUI_USER_HZ32 	GUI_FontYH_32
#define GUI_USER_HZ24 	GUI_FontYH_24
#define GUI_USER_HZ16 	GUI_FontYH_16
#define GUI_FontBC_16 	GUI_FontYH_36

#define MHIDE			(0)
#define MSHOW			(1)
#define MEXIT			(0xF) //????

#define GUI_ID_BUTTON1_0   (GUI_ID_USER+201)
#define GUI_ID_BUTTON1_1   (GUI_ID_USER+202)
#define GUI_ID_BUTTON1_2   (GUI_ID_USER+203)
#define GUI_ID_BUTTON1_3   (GUI_ID_USER+204)
#define GUI_ID_BUTTON1_4   (GUI_ID_USER+205)
#define GUI_ID_BUTTON1_5   (GUI_ID_USER+206)
#define GUI_ID_BUTTON1_6   (GUI_ID_USER+207)



#define TYPE_NUM		(4)
#define TYPE_BUTTON		(0)
#define TYPE_TEXT		(1)
#define TYPE_EDIT		(2)
#define TYPE_CHECK		(3)

#define PAGE_RETURN     (0)
#define PAGE_DOWN       (1)
#define PAGE_UP         (2)

extern void (*Set_Text[TYPE_NUM])();
typedef struct {
    U8 			isshow;
    U8			type;
    WM_HMEM 		hMem;
    char			text[48];
    int			value;
    float			scale;
} TCHG_MENU, *PCHG_MENU;
void MChange_Menu(void (*Value_Init)(WM_HMEM, U8), WM_HMEM hWin, PCHG_MENU pmenu, U8 sid);
void _cbButtonNext(WM_MESSAGE * pMsg);
void _cbButtonBack(WM_MESSAGE * pMsg);

#define TIPS_MAX			(12)
#define TIPS_SAVE_ER		(0)
#define TIPS_LIMIT_ER		(1)
#define TIPS_TIME_OK		(2)
#define TIPS_PWD_ER			(3)
#define TIPS_PWD_IN			(4)
#define TIPS_PWD_OUT		(5)
#define TIPS_NO_LOGIN		(6)
#define TIPS_NOT_CHG		(7)
#define TIPS_CAL_OK			(8)
#define TIPS_CAL_ER			(9)
#define TIPS_RESET			(10)
#define TIPS_CLOCK			(11)


#define MENU_BACK_Y		(425)
#define MENU_BACK_W		(130)
#define MENU_BACK_H		(60)
#define MENU_BACK_X1	(100)
#define MENU_BACK_X2	(340)
#define MENU_BACK_X3	(600)
typedef enum {
    Main_Buffer_Home = 0,
    Main_Buffer_Dev,
    Main_Buffer_Ctrl,
    Main_Buffer_Sys,
    Main_Buffer_History,
    Main_Buffer_Alarm,
    Main_Buffer_User,
} Main_Buffer_Type;
extern u16 Main_Buffer;
extern void Main_cbButton(WM_MESSAGE *pMsg);
extern u16 Get_UTF8TEXT_LEN(const char *_Str);
const char *Get_Tips(unsigned char ID);

void Button_SetHZ24(BUTTON_Handle hObj);
void Text_SetHZ24(TEXT_Handle hObj);
void Button_SetHZ32(BUTTON_Handle hObj);
void Text_SetHZ32(TEXT_Handle hObj);
void Edit_SetHZ24(EDIT_Handle hObj);
void Text_SetHZ24L(TEXT_Handle hObj);
void Check_SetHZ24(CHECKBOX_Handle hObj);
void Dropdown_SetHZ24(DROPDOWN_Handle hObj);
void Show_MBackPic(void);
void Show_INBackPic(void);
int Config_Save(U8 id);
void Login_Show(WM_HWIN hWin, U8 NeedType);
int get_login_permiss(U8 NeedType);
void Disp_House(void);
void Button_CurShow(WM_HWIN hWin,U8 sid,U8 cur);
void GUI_defDrawBitmap(const char * file, int x0, int y0);
void GUI_defCreateBitmapFromStream(const char * file,GUI_BITMAP * pBMP);
void Detail_cbButton(WM_MESSAGE * pMsg);
void Curve_cbButton(WM_MESSAGE * pMsg);
void next_cbButton(WM_MESSAGE * pMsg);
void Cut_cbButton(WM_MESSAGE * pMsg);
void set_cbButton(WM_MESSAGE * pMsg);

extern WM_HWIN input_hWin;
void Input_Task(WM_HWIN hParent);
void Set_Input_Len(unsigned char len);
void Set_Display_Buff(char *buf);
float Get_Input_Value(void);
unsigned char * Get_Input_Text(void);
void Set_Output_Callback(void (*p)(void));

void Button_Menu(int Id, WM_HWIN hWin, int CurID);
void ButtonText_Show(BUTTON_Handle pObj, const char * s, int x, int y);

extern WM_HWIN Current_HWIN;
extern char show_buf[256];
extern const u16 room_type[ROOM_TYPE_NUM];
extern const char fan_size[][8];
extern int old_Id;
extern const u16 Lang_timer_type[4];


void pluse_water_chk_alarm(void);
void alarm_chk_pluse_water(void);

extern WM_HWIN HBasic_Main;
void BK_Init(void);
void Creat_Basic_Main(void);
void Creat_Basic_Main2(void);

extern WM_HWIN HMain;
void Main_Task(WM_HWIN hWin);


extern WM_HWIN MDev_Man;
void MDev_Man_Task(WM_HWIN hParent);

extern WM_HWIN MDev_SetSensor;
void MDev_SetSensor_Task(WM_HWIN hParent);

extern WM_HWIN MDev_SignalInput;
void MDev_SignalInput_Task(WM_HWIN hParent);

extern WM_HWIN hWinWarning;
void MWarning_Task(WM_HWIN hWin, const char *pDisText);

extern WM_HWIN MDev_Regulatingfan;
void MDev_Regulatingfan_Task(WM_HWIN hParent, unsigned char select);

extern WM_HWIN MDev_SetLight;
void MDev_SetLight_Task(WM_HWIN hParent);

extern WM_HWIN MDev_SetWindow;
extern U8 get_win_adjust_sta(void);
void MDev_SetWindow_Task(WM_HWIN hParent,U8 sid);

extern WM_HWIN MDev_Heater;
void MDev_Heater_Task(WM_HWIN hParent, U8 num, U8 chg_flag);

extern WM_HWIN MDev_FixedSpeedfan;
void MDev_FixedSpeedfan_Task(WM_HWIN hParent, unsigned char Select);

extern WM_HWIN MDev_Spray;
void MDev_Spray_Task(WM_HWIN hParent);

extern WM_HWIN MDev_Wet;
void MDev_Wet_Task(WM_HWIN hParent,U8 num, U8 chg_flag);

extern WM_HWIN MCtrl_Man;
void MCtrl_Man_Task(WM_HWIN hParent);

extern WM_HWIN MCtrl_TempCurve;
void MCtrl_TempCurve_Task(WM_HWIN hParent);

extern WM_HWIN Mctrl_MinAir;
void Mctrl_MinAir_Task(WM_HWIN hParent);
void Update_MinAir(WM_HWIN hWin);

extern WM_HWIN MCtrl_LVPWMFan;
void MCtrl_LVPWMFan_Task(WM_HWIN hParent);
void LV_Add(void);
void LV_Reduce(void);

extern WM_HWIN MCtrl_LVWin;
void MCtrl_LVWin_Task(WM_HWIN hParent);

extern WM_HWIN MCtrl_LVJDQFan;
void MCtrl_LVJDQFan_Task(WM_HWIN hParent);

extern WM_HWIN MSys_Man;
void MSys_Man_Task(WM_HWIN hParent);

extern WM_HWIN MSys_Day;
void MSys_Day_Task(WM_HWIN hParent);

extern WM_HWIN MSys_Total;
void MSys_Total_Task(WM_HWIN hParent);

extern WM_HWIN MSys_Room;
void MSys_Room_Task(WM_HWIN hParent);

extern WM_HWIN MAla_Record;
void MAla_Record_Task(WM_HWIN hParent);

extern WM_HWIN MAla_Record;
void MAla_Record_Task(WM_HWIN hParent);

extern WM_HWIN MSys_HardTS;
void MSys_HardTS_Task(WM_HWIN hParent);

extern WM_HWIN MSys_JDQTS;
void MSys_JDQTS_Task(WM_HWIN hParent);

extern WM_HWIN MSys_PWMTS;
void MSys_PWMTS_Task(WM_HWIN hParent);

extern WM_HWIN MData_Record;
void MData_Record_Task(WM_HWIN hParent);

extern WM_HWIN MUser_Man;
void MUser_Man_Task(WM_HWIN hParent);

extern WM_HWIN MUser_PwdCG;
void MUser_PwdCG_Task(WM_HWIN hParent,U8 id);

extern WM_HWIN MDev_Circle;
void MDev_Circle_Task(WM_HWIN hParent);

extern WM_HWIN MDev_Timer;
void MDev_Timer_Task(WM_HWIN hParent, U8 sid, U8 chg_flag);

void Set_OTSensor_Type(U8 type);
extern WM_HWIN MCtrl_OTSensor;
void MCtrl_OTSensor_Task(WM_HWIN hParent);


#define AS_TYPE_DAY		(0)
#define AS_TYPE_TIME	(1)
#define AS_TYPE_OUT		(2)
void Set_Assist_Type(U8 type);
extern WM_HWIN MCtrl_Assist;
void MCtrl_Assist_Task(WM_HWIN hParent);


extern WM_HWIN MDev_Produce;
void MDev_Produce_Task(WM_HWIN hParent);

extern WM_HWIN MDev_ProduceHis;
void MDev_ProduceHis_Task(WM_HWIN hParent, U8 id);

extern WM_HWIN MDev_ProduceCur;
void MDev_ProduceCur_Task(WM_HWIN hParent);

extern WM_HWIN MDev_ENCInf;
void MDev_ENCInf_Task(WM_HWIN hParent);

extern WM_HWIN MCtrl_VAC;
void MCtrl_VAC_Task(WM_HWIN hParent);

extern WM_HWIN MSys_AlaTS;
void MSys_AlaTS_Task(WM_HWIN hParent);

extern WM_HWIN MSys_TimeSet;
void MSys_TimeSet_Task(WM_HWIN hParent);
void Reset_Sync_Time(void);

extern WM_HWIN MSys_SLTS;
void MSys_SLTS_Task(WM_HWIN hParent);

extern WM_HWIN MDev_ASSensor;
void MDev_ASSensor_Task(WM_HWIN hParent);

extern WM_HWIN MCtrl_Var;
void MCtrl_Var_Task(WM_HWIN hParent);

extern WM_HWIN MCtrl_LVView;
void cal_list_type(void);
void MCtrl_LVView_Task(WM_HWIN hParent,U8 xid,U8 yid);

extern WM_HWIN Temp_View;
void Temp_View_Task(WM_HWIN hParent);

extern WM_HWIN MCtrl_LVPFAN_SET;
void MCtrl_LVFAN_SET_Task(WM_HWIN hParent, unsigned char lv,unsigned char Row, unsigned char id);

extern WM_HWIN HisTemp_View;
void HisTemp_View_Task(WM_HWIN hParent);
extern WM_HWIN DataTemp_View;
void DataTemp_View_Task(WM_HWIN hParent);
extern WM_HWIN MSys_SysSet;
void MSys_SysSet_Task(WM_HWIN hParent);

extern WM_HWIN MDev_ModbusSet;
void MDev_ModbusSet_Task(WM_HWIN hParent, U8 id);

extern WM_HWIN MDev_PHeater;
void MDev_PHeater_Task(WM_HWIN hParent, U8 num);

extern WM_HWIN MDev_FuncView;
void MDev_FuncView_Task(WM_HWIN hParent,U8 id);

extern WM_HWIN MSys_CommSet;
void MSys_CommSet_Task(WM_HWIN hParent);

extern WM_HWIN MDev_Tempc;
void MDev_Tempc_Task(WM_HWIN hParent);
extern WM_HWIN MDev_Tempc_Set;
void MDev_Tempc_Set_Task(WM_HWIN hParent,U8 mode);

extern WM_HWIN MDev_APWM_FAN;
void MDev_APWM_FAN_Task(WM_HWIN hParent, U8 num, U8 chg_flag);

extern WM_HWIN MDev_FanChk;
void MDev_FanChk_Task(WM_HWIN hParent);

extern WM_HWIN MStart_Record;
void MStart_Record_Task(WM_HWIN hParent);

extern WM_HWIN MDev_ProduceMore;
void MDev_ProduceMore_Task(WM_HWIN hParent, U8 id);

extern WM_HWIN MCtrl_exMinAir;
void MCtrl_exMinAir_Task(WM_HWIN hParent);

extern WM_HWIN MDev_FoodADJ;
void MDev_FoodADJ_Task(WM_HWIN hParent, U8 id);

extern WM_HWIN MCtrl_HumFix;
void MCtrl_HumFix_Task(WM_HWIN hParent);

extern WM_HWIN MCtrl_HumFix_Set;
void MCtrl_HumFix_Set_Task(WM_HWIN hParent);

extern WM_HWIN MCtrl_TempCurve_Set;
void MCtrl_TempCurve_Set_Task(WM_HWIN hParent);

extern WM_HWIN MCtrl_LVView_Set;
void MCtrl_LVView_Set_Task(WM_HWIN hParent);

extern WM_HWIN MSys_ParMana;
void MSys_ParMana_Task(WM_HWIN hParent);

extern WM_HWIN MDev_Ala_En;
void MDev_Ala_En_Task(WM_HWIN hParent);

extern WM_HWIN MCtrl_Defrost;
void MCtrl_Defrost_Task(WM_HWIN hParent);

extern WM_HWIN MSys_SensorRG;
void MSys_SensorRG_Task(WM_HWIN hParent);

extern WM_HWIN MDev_SetSWin;
void MDev_SetSWin_Task(WM_HWIN hParent, U8 sid);

extern WM_HWIN MDev_Window_Set;
void MDev_Window_Set_Task(WM_HWIN hParent);

extern WM_HWIN MCtrl_VAC_Set;
void MCtrl_VAC_Set_Task(WM_HWIN hParent);

extern WM_HWIN MSys_InSel;
U8 MSys_InSel_Task(WM_HWIN hParent);

extern WM_HWIN MDev_SetSensor_Set;
void MDev_SetSensor_Set_Task(WM_HWIN hParent);

extern WM_HWIN MDev_Tempc_Pwm;
void MDev_Tempc_Pwm_Task(WM_HWIN hParent);

extern WM_HWIN MCtrl_Man_More;
void MCtrl_Man_More_Task(WM_HWIN hParent,U8 type);

extern WM_HWIN MMain_SS;
void MMain_SS_Task(WM_HWIN hParent);

extern WM_HWIN MClock_Record;
void MClock_Record_Task(WM_HWIN hParent);

extern WM_HWIN MCtrl_DayTemp;
void MCtrl_DayTemp_Task(WM_HWIN hParent);

extern WM_HWIN MDev_Water_Set;
void MDev_Water_Set_Task(WM_HWIN hParent);

#endif



