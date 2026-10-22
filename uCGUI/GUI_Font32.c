#include "GUI.h"


extern int GUIPROP_X_GetCharDistX(U16P c, int * pSizeX);//声明部分
extern void GUIPROP_X_DispChar(U16P c) ;  
 

GUI_CONST_STORAGE GUI_CHARINFO GUI_FontHZ32_CharInfo[2] =  
{     
    { 16, 16, 2, (void *)"fonts/ASCII32.bin"  },      //&ASC_ROM_8*16 字库的路径
    { 32, 32, 4,(void*)"fonts/GB32.bin"},      //&HZ_ROM_16X16字库的路径 
}; 
GUI_CONST_STORAGE GUI_FONT_PROP GUI_FontHZ32_PropHZ= { 
      0xA1A1, //第一个字符
      0xfefe, //最后一个字符
      &GUI_FontHZ32_CharInfo[1], //字符信息
      (void *)0,  
}; 
GUI_CONST_STORAGE  GUI_FONT_PROP GUI_FontHZ32_PropASC= { 
      0x0000, //第一个字符 
      0x007F, //最后一个字符
      &GUI_FontHZ32_CharInfo[0], //字符信息
      (void GUI_CONST_STORAGE *)&GUI_FontHZ32_PropHZ,  
}; 
GUI_CONST_STORAGE  GUI_FONT GUI_FontHZ32 =  
{ 
      GUI_FONTTYPE_PROP_X,
      32, //xsize
      32, //yszie
      1,  //x方向放大倍数
      1,  //y方向放大倍数
      (void GUI_CONST_STORAGE *)&GUI_FontHZ32_PropASC 
};
