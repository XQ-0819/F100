#include <string.h>
#include "menuinc.h"
#include "w25qxx.h"

#define ICON_MAX	(156)
typedef struct {
    const char 	file[24];
    int		size;
    int		addr;
} TICON, *PICON;
TICON icon_file[ICON_MAX] = {

    "00bmMPic_Timer1", 0x000009D4, 0x00000000,
    "01bmMPic_Timer2", 0x000009D4, 0x000009D4,
    "02bmMPic_Timer_Dis", 0x000009D4, 0x000013A8,
    "03bmMPic_Fan1", 0x000009D4, 0x00001D7C,
    "04bmMPic_Fan2", 0x000009D4, 0x00002750,
    "05bmMPic_Fan_Dis", 0x000009D4, 0x00003124,
    "06bmMPic_Home", 0x00001738, 0x00003AF8,
    "07bmMpic_logout", 0x000017D4, 0x00005230,
    "08bmMpic_login", 0x000017D4, 0x00006A04,
    "09bmMPic_Mala", 0x00000854, 0x000081D8,
    "10bmMPic_Mnol", 0x00000854, 0x00008A2C,
    "11bmMpic_MtempLOW", 0x000042C8, 0x00009280,
    "12bmMPic_MtempHIGH", 0x000042C8, 0x0000D548,
    "13bmMPic_MtempNOL", 0x000042C8, 0x00011810,
    "14bmMPic_AlarmR", 0x00001AA4, 0x00015AD8,
    "15bmMPic_CtrlR", 0x00001AF8, 0x0001757C,
    "16bmMPic_DevR", 0x00001C48, 0x00019074,
    "17bmMPic_DataR", 0x00001B4C, 0x0001ACBC,
    "18bmMPic_SysR", 0x00001B4C, 0x0001C808,
    "19bmMPic_UserR", 0x00001AF8, 0x0001E354,
    "20bmMPic_UserG", 0x00001AF8, 0x0001FE4C,
    "21bmMPic_AlarmG", 0x00001AA4, 0x00021944,
    "22bmMPic_DataG", 0x00001B4C, 0x000233E8,
    "23bmMPic_SysG", 0x00001B4C, 0x00024F34,
    "24bmMPic_CtrlG", 0x00001AF8, 0x00026A80,
    "25bmMPic_DevG", 0x00001C48, 0x00028578,
    "26bmMPic_Heat1", 0x000009D4, 0x0002A1C0,
    "27bmMPic_Heat2", 0x000009D4, 0x0002AB94,
    "28bmMPic_Heat_Dis", 0x000009D4, 0x0002B568,
    "29bmMPic_Wet1", 0x000009D4, 0x0002BF3C,
    "30bmMPic_Wet2", 0x000009D4, 0x0002C910,
    "31bmMPic_Wet_Dis", 0x000009D4, 0x0002D2E4,
    "32bmMLogin_YES", 0x000009D4, 0x0002DCB8,
    "33bmMLogin_NO", 0x000009D4, 0x0002E68C,
    "34bmMWIFI_YES", 0x000009D4, 0x0002F060,
    "35bmMWIFI_NO", 0x000009D4, 0x0002FA34,
    "36bmMPic_Copy", 0x00000E20, 0x00030408,
    "37bmMPic_Detail", 0x00000E20, 0x00031228,
    "38bmMPic_Dis", 0x00000910, 0x00032048,
    "39bmMPic_En", 0x00000910, 0x00032958,
    "40bmMPic_Fan1", 0x00000910, 0x00033268,
    "41bmMPic_Fan2", 0x00000910, 0x00033B78,
    "42bmMPic_Fan3", 0x00000910, 0x00034488,
    "43bmMPic_Up", 0x00002410, 0x00034D98,
    "44bmMPic_Down", 0x00002410, 0x000371A8,
    "45MPic_Chick", 0x00000DFC, 0x000395B8,
    "46bmMPic_Htemp_NOL", 0x00002710, 0x0003A3B4,
    "47bmMPic_Htemp_LOW", 0x00002650, 0x0003CAC4,
    "48bmMPic_Htemp_HIGH", 0x00002710, 0x0003F114,
    "49bmMPic_Homing", 0x0000079C, 0x00041824,
    "50bmMPic_Outhome", 0x000007F8, 0x00041FC0,
    "51Mpic_ReturenBack", 0x00006478, 0x000427B8,
    "52MPic_Hum", 0x00001220, 0x00048C30,
    "53MPic_Vac", 0x00001450, 0x00049E50,
    "54MPic_Pig", 0x00001010, 0x0004B2A0,
    "55MPic_duck", 0x00001010, 0x0004C2B0,
    "56Mpic_KeyBack", 0x0000A8C4, 0x0004D2C0,
    "57Mpic_Curve", 0x00000E20, 0x00057B84,
    "58iconDev_Sensor", 0x00000BB0, 0x000589A4,
    "59iconDev_Pwm", 0x00001010, 0x00059554,
    "60iconDev_Enviro", 0x00001190, 0x0005A564,
    "61iconDev_Input", 0x00001010, 0x0005B6F4,
    "62iconDev_JDQ", 0x00001010, 0x0005C704,
    "63iconDev_Product", 0x00001010, 0x0005D714,
    "64iconCtrl_TempCurve", 0x00000F90, 0x0005E724,
    "65iconCtrl_LV", 0x00000B34, 0x0005F6B4,
    "66iconCtrl_MinFan", 0x00000B34, 0x000601E8,
    "67iconCtrl_CtrlData", 0x00001010, 0x00060D1C,
    "68iconCtrl_Age", 0x00000F90, 0x00061D2C,
    "69iconCtrl_Time", 0x00000F90, 0x00062CBC,
    "70iconCtrl_Out", 0x00000F90, 0x00063C4C,
    "71iconCtrl_HumCtrl", 0x00001310, 0x00064BDC,
    "72iconCtrl_CO2Ctrl", 0x00000BB0, 0x00065EEC,
    "73iconCtrl_NH3Ctrl", 0x00000B34, 0x00066A9C,
    "74iconCtrl_VACCtrl", 0x00000F14, 0x000675D0,
    "75iconSys_Home", 0x00000E1C, 0x000684E4,
    "76iconSys_Time", 0x00000F14, 0x00069300,
    "77iconSys_Age", 0x00000F14, 0x0006A214,
    "78iconSys_Number", 0x00000F14, 0x0006B128,
    "79iconSys_DataEx", 0x00000F14, 0x0006C03C,
    "80iconSys_SetEx", 0x00000F14, 0x0006CF50,
    "81iconSys_SetIm", 0x00000F14, 0x0006DE64,
    "82iconSys_SysSet", 0x00000F14, 0x0006ED78,
    "83iconSys_Test", 0x00000DA0, 0x0006FC8C,
    "84iconSys_Reset", 0x00000DA0, 0x00070A2C,
    "85iconAll_Return", 0x00001010, 0x000717CC,
    "86iconAll_NextPage", 0x00001010, 0x000727DC,
    "87iconAll_UpPage", 0x00001010, 0x000737EC,
    "88MPic_HumLow", 0x00004BD0, 0x000747FC,
    "89MPic_HumNol", 0x00004BD0, 0x000793CC,
    "90MPic_HumHigh", 0x00004BD0, 0x0007DF9C,
    "91bmMPic_Home", 0x00002120, 0x00082B6C,
    "92bmMpic_logout", 0x00002120, 0x00084C8C,
    "93bmMpic_login", 0x00002120, 0x00086DAC,
    "94bmMPic_CO2", 0x00001220, 0x00088ECC,
    "95bmMPic_Cut", 0x00001290, 0x0008A0EC,
    "96bmMPic_Hum", 0x00001450, 0x0008B0EC,
    "97bmMPic_OutTemp", 0x000009D4, 0x0008C53C,
    "98English_AlarmG", 0x00001868, 0x0008CF10,
    "99English_AlarmR", 0x00001868, 0x0008E778,
    "100English_CtrG", 0x00001868, 0x0008FFE0,
    "101English_CtrR", 0x00001868, 0x00091848,
    "102English_DataG", 0x00001868, 0x000930B0,
    "103English_DataR", 0x00001868, 0x00094918,
    "104English_DevG", 0x00001868, 0x00096180,
    "105English_DevR", 0x00001868, 0x000979E8,
    "106English_SysG", 0x00001868, 0x00099250,
    "107English_SysR", 0x00001868, 0x0009AAB8,
    "108English_UserG", 0x00001868, 0x0009C320,
    "109English_UserR", 0x00001868, 0x0009DB88,
    "99Mpic_Hum", 0x00001010, 0x0009F3F0,
    "99Mpic_Set", 0x00000E20, 0x000A0400,
    "99Mpic_Speed", 0x000009D4, 0x000A1220,
    "99Mpic_Vac", 0x00000650, 0x000A1BF4,
    "99Mpic_Down", 0x00001910, 0x000A2244,
    "99Mpic_left", 0x00001910, 0x000A3B54,
    "99Mpic_Right", 0x00001910, 0x000A5464,
    "99Mpic_Up", 0x00001910, 0x000A6D74,

    "00heatr", 0x00000AA0, 0x000A8684,
    "00light", 0x00000AA0, 0x000A9124,
    "00spray", 0x00000AA0, 0x000A9BC4,
    "00wet", 0x00000AA0, 0x000AA664,
    "00more", 0x00000C10, 0x000AB104,
    "00top", 0x00001010, 0x000ABD14,
    "00top_cancle", 0x00001010, 0x000ACD24,
    "00clock_in", 0x00001010, 0x000ADD34,

    "01Rus_ALA", 0x00001B4C, 0x000AED44,
    "01Rus_ALAG", 0x00001B4C, 0x000B0890,
    "01Rus_Control", 0x00001B4C, 0x000B23DC,
    "01Rus_ControlG", 0x00001B4C, 0x000B3F28,
    "01Rus_Devic", 0x00001B4C, 0x000B5A74,
    "01Rus_DevicG", 0x00001B4C, 0x000B75C0,
    "01Rus_His", 0x00001B4C, 0x000B910C,
    "01Rus_HisG", 0x00001B4C, 0x000BAC58,
    "01Rus_Sys", 0x00001B4C, 0x000BC7A4,
    "01Rus_SysG", 0x00001B4C, 0x000BE2F0,
    "01Rus_User", 0x00001B4C, 0x000BFE3C,
    "01Rus_UserG", 0x00001B4C, 0x000C1988,

    "bmMpic_alaG16", 0x00000350, 0x000C34D4,
    "bmMpic_alaW16", 0x00000350, 0x000C3824,
    "bmMpic_CtrlG16", 0x00000390, 0x000C3B74,
    "bmMpic_CtrlW16", 0x00000390, 0x000C3F04,
    "bmMpic_DevG16", 0x00000450, 0x000C4294,
    "bmMpic_DevW16", 0x00000450, 0x000C46E4,
    "bmMpic_HistoryG16", 0x000003D0, 0x000C4B34,
    "bmMpic_HistoryW16", 0x000003D0, 0x000C4F04,
    "bmMpic_loginG16", 0x000003D0, 0x000C52D4,
    "bmMpic_loginW16", 0x000003D0, 0x000C56A4,
    "bmMpic_SysG16", 0x00000394, 0x000C5A74,
    "bmMpic_SysW16", 0x000003D0, 0x000C5E08,
    "09bmMPic_MalaON", 0x00000B74, 0x000C61D8,
    "09bmMPic_MalaOFF", 0x00000C4C, 0x000C6D4C,
    

};

#if USE_VS == 1
void GUI_defDrawBitmap(const char * file, int x0, int y0)
{
    FILE *fp;
    GUI_ALLOC_DATATYPE hMem;
    char *_acBuffer;
    int i, size, addr;

    // find file
    for (i = 0;  i < ICON_MAX; i++) {
        if (strcmp(icon_file[i].file, file) == 0) {
            size = icon_file[i].size;
            addr = icon_file[i].addr;
            break;
        }
    }

    if (i >= ICON_MAX)return;

    fp = fopen("Sample\\PureWater\\FlashPic\\iconPic.bin", "rb");      /*只读方式打开流文件*/

    if (NULL != fp) {    /*检测打开的结果*/
        fseek( fp, addr, SEEK_SET ); /*重新设置移动位置*/
        hMem = GUI_ALLOC_AllocZero(size); /*申请和文件大小一样的内存*/
        _acBuffer = GUI_ALLOC_h2p(hMem);  /*把申请到的内存转换成指针*/
        fread(_acBuffer, 1, size, fp); /*把文件读取到缓存*/
        fclose(fp);/*关闭文件*/
        GUI_DrawStreamedBitmapAuto(_acBuffer, x0, y0);/*直接显示出来*/
        GUI_ALLOC_Free(hMem);/*释放申请的内存*/
    }
}
#else
void GUI_defDrawBitmap(const char * file, int x0, int y0)
{
    char *_acBuffer;
    int i, addr;

// find file
    for (i = 0;  i < ICON_MAX; i++) {
        if (strcmp(icon_file[i].file, file) == 0) {
            addr = SDRAM_ICON_ADDR + icon_file[i].addr;
            break;
        }
    }

    if (i >= ICON_MAX)return;

    _acBuffer = (char *)addr;
    GUI_DrawStreamedBitmapAuto(_acBuffer, x0, y0);/*直接显示出来*/
}
#endif

void GUI_defCreateBitmapFromStream(const char * file, GUI_BITMAP * pBMP)
{
    char *_acBuffer;
    int i, addr;
    GUI_LOGPALETTE Palette;

    for (i = 0;  i < ICON_MAX; i++) {
        if (strcmp(icon_file[i].file, file) == 0) {
            addr = SDRAM_ICON_ADDR + icon_file[i].addr;
            break;
        }
    }

    if (i >= ICON_MAX)return;

    _acBuffer = (char *)addr;
    i = GUI_CreateBitmapFromStream(pBMP, &Palette, _acBuffer);
}






