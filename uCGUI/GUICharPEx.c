#include <stddef.h>           /* needed for definition of NULL */
#include "GUI_Private.h"
#include "ff.h"
#include "diskio.h"
#include <string.h>
#include "w25qxx.h"

//#include "sdio_sdcard.h"

#define USE_FLASH 	1// 使用外部FLASH作为字库
#define USE_GBK		0//
#define FONT_MAXNUM 6


const char font_path[FONT_MAXNUM][24] = {
    "fonts/ASCII16.bin",
    "fonts/ASCII24.bin",
    "fonts/ASCII32.bin",
    "fonts/GB16.bin",
    "fonts/GB24.bin",
    "fonts/GB32.bin"
};
U32 font_addr[FONT_MAXNUM] = {
    NULL,
    ascii_24_addr,
    ascii_32_addr,
    NULL,
    gbk_24_addr,
    gbk_32_addr
};




//字模数据的暂存数组,以单个字模的最大字节数为设定值
#define BYTES_PER_FONT      128
static U8 GUI_FontDataBuf[BYTES_PER_FONT];
/*---------------------------------------------------------------------------*/
/*字库外部函数部分-----------------------------------------------------------*/
void GUI_X_GetFontData(char* font, U32 oft, U8 *ptr, U8 bytes)
{
    FIL fsrc;        // 定义文件操作类
    FRESULT res;  // 定义操作结果变量
    UINT br;         // 定义读写数量变量
    res = f_open(&fsrc, font, FA_OPEN_EXISTING | FA_READ);   //打开字库文件

    if (res != FR_OK) {

    }

    res = f_lseek(&fsrc, oft); //找到首地址
    res = f_read(&fsrc, ptr, bytes, &br); //读取32个字库点阵数据
    res = f_close(&fsrc); //关闭字体
}

void GUI_X_GetFontDataFlash(char* font, U32 oft, U8 *ptr, U8 bytes)
{
    U8 	i = 0;
    U32 hznum = oft;

    for (i = 0; i < FONT_MAXNUM; i++) {

        if (strcmp(&font_path[i][0], font) == 0) {
            hznum += font_addr[i];
        }
    }

    SPI_Flash_Read(ptr, hznum, bytes);
}




static void GUI_GetDataFromMemory(const GUI_FONT_PROP GUI_UNI_PTR *pProp, U16P c)
{
    U8 BytesPerFont;
    U32 oft;
    char *font = (char *)pProp->paCharInfo->pData;
    BytesPerFont = (*GUI_pContext).pAFont->YSize * pProp->paCharInfo->BytesPerLine; //每个字模的数据字节数

    if (BytesPerFont > BYTES_PER_FONT) {
        BytesPerFont = BYTES_PER_FONT;
    }

    if (c < 0x80)                                                                  {
        oft = (c - 0x20) * BytesPerFont; //英文字符地址偏移算法
    } else {
//#if USE_GBK == 1
//        oft = ((((c >> 8) - 0x40) - ((c >> 8) / 128)) + ((c & 0xFF) - 0x81) * 190) * BytesPerFont; //中文字符地址偏移算法包括符号
//#else
//        oft = ((((c >> 8) - 0xA1)) + ((c & 0xFF) - 0xA1) * 94) * BytesPerFont; //中文字符地址偏移算法包括符号
//#endif
#if USE_GBK == 1
        oft = ((((c & 0xFF) - 0x40) - ((c & 0xFF) / 128)) + ((c >> 8) - 0x81) * 190) * BytesPerFont; //中文字符地址偏移算法包括符号
#else
        oft = ((((c & 0xFF) - 0xA1)) + ((c >> 8) - 0xA1) * 94) * BytesPerFont; //中文字符地址偏移算法包括符号
#endif
    }

#if USE_FLASH == 1
    GUI_X_GetFontDataFlash(font, oft, GUI_FontDataBuf, BytesPerFont);
#else
    GUI_X_GetFontData(font, oft, GUI_FontDataBuf, BytesPerFont);
#endif
}

void GUIPROP_X_DispChar(U16P c)
{
    int BytesPerLine;
    GUI_DRAWMODE DrawMode = (*GUI_pContext).TextMode;
    const GUI_FONT_PROP GUI_UNI_PTR *pProp = (*GUI_pContext).pAFont->p.pProp;

    //搜索定位字库数据信息
    for (; pProp; pProp = pProp->pNext) {
        if ((c >= pProp->First) && (c <= pProp->Last))break;
    }

    if (pProp) {
        GUI_DRAWMODE OldDrawMode;
        const GUI_CHARINFO GUI_UNI_PTR * pCharInfo = pProp->paCharInfo;
        GUI_GetDataFromMemory(pProp, c);//取出字模数据
        BytesPerLine = pCharInfo->BytesPerLine;
        OldDrawMode  = LCD_SetDrawMode(DrawMode);
        LCD_DrawBitmap((*GUI_pContext).DispPosX, (*GUI_pContext).DispPosY,
                       pCharInfo->XSize, (*GUI_pContext).pAFont->YSize,
                       (*GUI_pContext).pAFont->XMag, (*GUI_pContext).pAFont->YMag,
                       1,     /* Bits per Pixel */
                       BytesPerLine,
                       &GUI_FontDataBuf[0],
                       &LCD_BKCOLORINDEX
                      );

        /* Fill empty pixel lines */
        if ((*GUI_pContext).pAFont->YDist > (*GUI_pContext).pAFont->YSize) {
            int YMag = (*GUI_pContext).pAFont->YMag;
            int YDist = (*GUI_pContext).pAFont->YDist * YMag;
            int YSize = (*GUI_pContext).pAFont->YSize * YMag;

            if (DrawMode != LCD_DRAWMODE_TRANS) {
                LCD_COLOR OldColor = GUI_GetColor();
                GUI_SetColor(GUI_GetBkColor());
                LCD_FillRect((*GUI_pContext).DispPosX, (*GUI_pContext).DispPosY + YSize,
                             (*GUI_pContext).DispPosX + pCharInfo->XSize,
                             (*GUI_pContext).DispPosY + YDist);
                GUI_SetColor(OldColor);
            }
        }

        LCD_SetDrawMode(OldDrawMode); /* Restore draw mode */
        (*GUI_pContext).DispPosX += pCharInfo->XDist * (*GUI_pContext).pAFont->XMag;
    }
}
/*********************************************************************
*
*       GUIPROP_GetCharDistX
*/
int GUIPROP_X_GetCharDistX(U16P c, int * pSizeX)
{
    const GUI_FONT_PROP GUI_UNI_PTR * pProp = (*GUI_pContext).pAFont->p.pProp;

    for (; pProp; pProp = pProp->pNext) {
        if ((c >= pProp->First) && (c <= pProp->Last))break;
    }

    return (pProp) ? (pProp->paCharInfo)->XSize * (*GUI_pContext).pAFont->XMag : 0;
}

