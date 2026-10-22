/*
*---------------------------------------------------------------
*                        emWin字体工具                           
*                                                               
* 注:支持unicode/GBK编码切换,支持抗锯齿                             
* 注:本字体文件由emWin字体工具 V0.4 生成                             
* 作者:阿里(qq:617622104)                                         
*---------------------------------------------------------------
*/


#include "GUI.h"


#include "GUI_Private.h"
#include <string.h>
#include <stdint.h>


#include "def.h"
#include "w25qxx.h"




typedef struct{
    U16 min;
    U16 max;
    U8 reserved[4];
}x_header_t;//头
typedef struct{
    U16 size;
    U8 pos[4];
}x_table_t;//位置信息
typedef struct{
    U8 XSize;
    U8 XDist;
    U16 bpl;
}x_info_t;//字体信息


static x_header_t x_header;
static unsigned char g_font_buf[364];//软件已计算好BUFF大小，无需更改BUFF大小

//用户实现函数，用来读取字模数据
//offset：地址偏移
//size：读取的字节大小
static unsigned char *GUI_FontYH_24_getData(int offset, int size){
    //字模保存在SPIFLASH示例 SPIFLASH_Read(g_font_buf,pFontAddr+offset,size);
    //字模已加载到SDRAM示例  memcpy(g_font_buf,pFontAddr+offset,size);
#if FONT_SDRAM == 1
	memcpy(g_font_buf,(uint8_t *)(SDRAM_GBK24_ADDR+offset),size);
#else
	SPI_Flash_Read(g_font_buf, gbk_24_addr + offset, size);
#endif   
    return g_font_buf;
}

static x_table_t* GUI_FontYH_24_GetTable(U16P c) {
    x_table_t *pTable = (x_table_t *)GUI_FontYH_24_getData((c-x_header.min)*sizeof(x_table_t)+sizeof(x_header_t),sizeof(x_table_t));
    return pTable;
}

static void GUI_FontYH_24_Draw(int x0, int y0, int XSize, int YSize, int BytesPerLine, const U8 *pData) {
    int x, y;
    tLCD_SetPixelAA* pfSetPixelAA;
    pfSetPixelAA = (GUI_pContext->TextMode & GUI_TM_TRANS) ?
    LCD_SetPixelAA4_Trans : LCD_SetPixelAA4_NoTrans;
    for (y=0; y<YSize; y++) {
        const U8*pData0 = pData;
        for (x=0; x<XSize-1; x+=2) {
            (*pfSetPixelAA)(x+x0,y0+y,   (*pData0)>>4);
            (*pfSetPixelAA)(x0+x+1,y0+y, (*pData0++)&15);
        }
        if (XSize&1) {
            (*pfSetPixelAA)(x0+x,y0+y, (*pData0)>>4);
        }
        pData+=BytesPerLine;
    }
}

static void GUI_FontYH_24_DispChar(U16P c) {
    int BytesPerLine;
    GUI_DRAWMODE DrawMode = GUI_pContext->TextMode;
    x_table_t *pTable = GUI_FontYH_24_GetTable(c);

    if ( pTable->size ) {
        GUI_DRAWMODE OldDrawMode;
        int pos = 0;
        memcpy(&pos, pTable->pos, 4);
        U8 *pMapData = GUI_FontYH_24_getData(pos,pTable->size);
        x_info_t *pInfo = (x_info_t *)pMapData;
        U8 *pData = pMapData+sizeof(x_info_t);
        BytesPerLine = pInfo->bpl;
        OldDrawMode  = LCD_SetDrawMode(DrawMode);
        GUI_FontYH_24_Draw( GUI_pContext->DispPosX,
            GUI_pContext->DispPosY,
            pInfo->XSize,
            GUI_pContext->pAFont->YSize,
            BytesPerLine,
            pData);
        LCD_SetDrawMode(OldDrawMode);
        GUI_pContext->DispPosX += pInfo->XDist;
    }
}

static int GUI_FontYH_24_GetCharDistX(U16P c, int * pSizeX) {
    static char x_header_once = 1;
    if(x_header_once){
        x_header_once = 0;
        x_header_t *pHeader = (x_header_t *)GUI_FontYH_24_getData(0,sizeof(x_header_t));
        memcpy(&x_header, pHeader, sizeof(x_header_t));
    }
    if ((c>x_header.max) || (c<x_header.min)) return 0;
	x_table_t *pTable = GUI_FontYH_24_GetTable(c);
    if ( pTable->size ){
        int pos = 0;
        memcpy(&pos, pTable->pos, 4);
        x_info_t *pInfo = (x_info_t *)GUI_FontYH_24_getData(pos,sizeof(x_info_t));
        return pInfo->XSize;
    }
    return 0;
}

static void GUI_FontYH_24_GetFontInfo(const GUI_FONT * pFont, GUI_FONTINFO* pfi) {
    pfi->Flags = GUI_FONTINFO_FLAG_PROP | GUI_FONTINFO_FLAG_AA4;
}

static char GUI_FontYH_24_IsInFont(const GUI_FONT * pFont, U16 c) {
    return 1;
}


//字体类型：外部字体,类似XBF,最优
//微软雅黑,Regular,17
//字模高度：30,垂直偏移：-1
GUI_CONST_STORAGE GUI_FONT GUI_FontYH_24 = {
GUI_FontYH_24_DispChar,
GUI_FontYH_24_GetCharDistX,
GUI_FontYH_24_GetFontInfo,
GUI_FontYH_24_IsInFont,
(GUI_GETCHARINFO  *)0,
(tGUI_ENC_APIList *)0,
30, /* height of font  */
30, /* space of font y */
1,
1,
(void*)0,
};

