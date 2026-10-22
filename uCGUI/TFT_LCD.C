

#include "TFT_LCD.h"
#include "stm324x9i_eval_lcd.h"
#include  <stdlib.h>
#include "fonts.h"
#define TFT_LCD_TEST 0
#define TFT_LCD_XSIZE 800
#define TFT_LCD_YSIZE 480
#define TFT_DrawPoint(x1,y1,color)   TFT_SetPoint(x1,y1,color)
//#define TFT_SetCursor(x,y) RA8875_SetTextCursor(x,y)

/****************************************************************************
* 名    称：u16 GUI_Color565(u32 RGB)
* 功    能：将RGB颜色转换为16位颜色。
* 入口参数：RGB  颜色值
* 出口参数：返回16位RGB颜色值。
* 说    明：
* 调用方法：i=GUI_Color565(0xafafaf);
****************************************************************************/
u16 GUI_Color565(u32 RGB)
{
    u8  r, g, b;

    b = ( RGB >> (0+3) ) & 0x1f;		// 取B色的高5位
    g = ( RGB >> (8+2) ) & 0x3f;		// 取G色的高6位
    r = ( RGB >> (16+3)) & 0x1f;		// 取R色的高5位

    return( (r<<11) + (g<<5) + (b<<0) );
}
//画线
//x1,y1:起点坐标
//x2,y2:终点坐标
void TFT_DrawLine(u16 x1,u16 y1,u16 x2,u16 y2,u16 color)
{
    unsigned int  t;
    signed int x, y;
    if((x1==x2)&&(y1==y2))
    {
        TFT_DrawPoint(x1, y1,color);
    }
    else if(abs(y2-y1)>abs(x2-x1))//斜率大于1
    {
        if(y1>y2)
        {
            t=y1;
            y1=y2;
            y2=t;
            t=x1;
            x1=x2;
            x2=t;
        }
        for(y=y1; y<y2; y++) //以y轴为基准
        {
            x=(y-y1)*(x2-x1)/(y2-y1)+x1;
            TFT_DrawPoint((unsigned int)x,(unsigned int)y,color);
        }
    }
    else     //斜率小于等于1
    {
        if(x1>x2)
        {
            t=y1;
            y1=y2;
            y2=t;
            t=x1;
            x1=x2;
            x2=t;
        }
        for(x=x1; x<=x2; x++) //以x轴为基准
        {
            y =(x-x1)*(y2-y1)/(x2-x1)+y1;
            TFT_DrawPoint((unsigned int)x,(unsigned int)y,color);
        }
    }
}
/****************************************************************************
* 名    称：void TFT_draw_circle(u16 cx,u16 cy,u16 r,u16 color,u8 fill)
* 功    能：在指定座标画圆，可填充
* 入口参数：
* 出口参数：
* 说    明：
* 调用方法：
****************************************************************************/
void TFT_draw_circle(u16 cx,u16 cy,u16 r,u16 color,u8 fill)
{
    u16 x,y;
    s16 delta;
    x=0;
    y=r;
    delta=3-(r<<1);

    while(y>=x)
    {
        if(fill)
        {
            TFT_DrawLine(cx+x,cy+y,cx-x,cy+y,color);
            TFT_DrawLine(cx+x,cy-y,cx-x,cy-y,color);
            TFT_DrawLine(cx+y,cy+x,cx-y,cy+x,color);
            TFT_DrawLine(cx+y,cy-x,cx-y,cy-x,color);
        }
        else
        {
            TFT_DrawPoint(cx+x,cy+y,color);
            TFT_DrawPoint(cx-x,cy+y,color);
            TFT_DrawPoint(cx+x,cy-y,color);
            TFT_DrawPoint(cx-x,cy-y,color);
            TFT_DrawPoint(cx+y,cy+x,color);
            TFT_DrawPoint(cx-y,cy+x,color);
            TFT_DrawPoint(cx+y,cy-x,color);
            TFT_DrawPoint(cx-y,cy-x,color);
        }
        x++;
        /***使用Bresenham算法画圆**/
        if(delta<0)
        {
            delta +=4*x+6;
        }
        else
        {
            delta +=10+4*(x-y);
            y--;
        }
    }
}

/****************************************************************************
* 名    称：void GUI_Rectangle(u16 x0, u16 y0, u16 x1, u16 y1,u16 color,u8 fill)
* 功    能：在指定区域画矩形，可填充颜色
* 入口参数：
* 出口参数：
* 说    明：
* 调用方法：
****************************************************************************/
void TFT_draw_Rectangle(u16 x0, u16 y0, u16 x1, u16 y1,u16 color,u8 fill)
{
    if(fill)
    {
        u16 i;
        if(x0>x1)
        {
            i=x1;
            x1=x0;
        }
        else
        {
            i=x0;
        }
        for(; i<=x1; i++)
        {
            TFT_DrawLine(i,y0,i,y1,color);
        }
        return;
    }
    TFT_DrawLine(x0,y0,x0,y1,color);
    TFT_DrawLine(x0,y1,x1,y1,color);
    TFT_DrawLine(x1,y1,x1,y0,color);
    TFT_DrawLine(x1,y0,x0,y0,color);
}
/****************************************************************************
* 名    称：void  TFT_draw_Square(u16 x0, u16 y0, u16 with, u16 color,u8 fill)
* 功    能：在指定区域画正方形，可填充颜色
* 入口参数：
* 出口参数：
* 说    明：
* 调用方法：
****************************************************************************/
void  TFT_draw_Square(u16 x0, u16 y0, u16 with, u16 color,u8 fill)
{
    TFT_draw_Rectangle(x0, y0, x0+with, y0+with, color,fill);
}


#if USE_429 == 1
extern u8 *HZ_X_X_ptr_color;
#else
u8 *HZ_X_X_ptr_color;
#endif
/**********************************************
ASCII(8*16/16*24/16*32) 及 汉字(16*16/24*24/32*32) 混合字符串显示函数，带反白功能
x,y显示字符串的左上角xy坐标.x..8点一字节位置(0-29); y..一条扫描线定位(0-239)
*ptr...字符串指针，本函数所指的是flash字串
*           LineColor : 字符的颜色
*           FillColor   : 字符背景颜色
*           mod        :TRANSP (字体显示的时候不带背景颜色)
				 NORMAL (字体显示的时候有背景颜色)

返回参数：输出字串长度，留意汉字一个算两个
其它假定：调用时汉字必须在字库数组中已经存在，否则将输出不正确的结果
***********************************************/
void TFT_DispString(u16 x,u16 y,unsigned char *pstr,u8 dotwidth,u16 LineColor,u16 FillColor, u8 Mod)
{
    u32 count;

    u32 i=0,k=0;
    u32 str;
    INT32U strh;
    while(*pstr)
    {
        if(*pstr>0xa0)
        {

            switch(dotwidth)
            {
            case 16://写16*16的汉字
            {
                count=0;
                HZ_X_X_ptr_color=(	u8 *)&(HZ16_16_struct[0].Index[0]);
                while((HZ_X_X_ptr_color[count]!=*pstr)||(HZ_X_X_ptr_color[count+1]!=*(pstr+1)))
                {
                    count=count+34;
                    if(HZ_X_X_ptr_color[count]==0)//没有找到
                    {
                        break;
                    }
                }

                count+=2;

                for (i=0; i<16; i++)		 //列控制
                {
                    str = (HZ_X_X_ptr_color[2*i+count]<<8)|(HZ_X_X_ptr_color[2*i+count+1]);
                    for (k=0; k<16; k++)
                    {
                        if ( (str >> 15-k) & 0x01 == 0x01)
                        {
                            TFT_DrawPoint(x+k,y+i,LineColor); // 字符颜色
                        }
                        else
                        {
                            if(Mod == 1)
                            {
                                TFT_DrawPoint(x+k,y+i,FillColor); // 背景颜色
                            }
                        }
                    }

                }

                pstr+=2;

                x+=16;
            }
            break;

            case 24://写24*24的汉字
            {

                count=0;
                HZ_X_X_ptr_color=(	u8 *)&(HZ24_24_struct[0].Index[0]);
                while((HZ_X_X_ptr_color[count]!=*pstr)||(HZ_X_X_ptr_color[count+1]!=*(pstr+1)))
                {
                    count=count+74;
                    if(HZ_X_X_ptr_color[count]==0)//没有找到
                    {
                        //count=0xffff;
                        break;
                    }
                }

                count+=2;
                for (i=0; i<24; i++)		 //列控制
                {

                    //str = (uchar)HzLib[addr].Msk[i*2]<<8|(uchar)HzLib[addr].Msk[i*2+1];
                    str = (HZ_X_X_ptr_color[3*i+count]<<16)|(HZ_X_X_ptr_color[3*i+count+1]<<8)|(HZ_X_X_ptr_color[3*i+count+2]);
                    for (k=0; k<24; k++)
                    {
                        if ( (str >> 23-k) & 0x01 == 0x01)
                        {
                            TFT_DrawPoint(x+k,y+i,LineColor); // 字符颜色
                        }
                        else
                        {
                            if(Mod == 1)
                            {
                                TFT_DrawPoint(x+k,y+i,FillColor); // 背景颜色
                            }
                        }
                    }

                }

                pstr+=2;

                x+=24;

            }
            break;
            case 32://写32*32的汉字
            {
                count=0;
                HZ_X_X_ptr_color=(	u8 *)&(HZ32_32_struct[0].Index[0]);
                while((HZ_X_X_ptr_color[count]!=*pstr)||(HZ_X_X_ptr_color[count+1]!=*(pstr+1)))
                {
                    count=count+130;
                    if(HZ_X_X_ptr_color[count]==0)//没有找到
                    {
                        break;
                    }
                }

                count+=2;
                for (i=0; i<32; i++)		 //列控制
                {

                    str = (HZ_X_X_ptr_color[4*i+count]<<24)|(HZ_X_X_ptr_color[4*i+count+1]<<16)|(HZ_X_X_ptr_color[4*i+count+2]<<8)|(HZ_X_X_ptr_color[4*i+count+3]);
                    for (k=0; k<32; k++)
                    {
                        if ( (str >> 31-k) & 0x01 == 0x01)
                        {
                            TFT_DrawPoint(x+k,y+i,LineColor); // 字符颜色
                        }
                        else
                        {
                            if(Mod == 1)
                            {
                                TFT_DrawPoint(x+k,y+i,FillColor); // 背景颜色
                            }
                        }
                    }

                }

                pstr+=2;

                x+=32;
            }
            break;
            case 48://д48*48�ĺ���
            {

                count=0;
                HZ_X_X_ptr_color=(	u8 *)&(HZ48_48_struct[0].Index[0]);
                while((HZ_X_X_ptr_color[count]!=*pstr)||(HZ_X_X_ptr_color[count+1]!=*(pstr+1)))
                {
                    count=count+290;
                    if(HZ_X_X_ptr_color[count]==0)//没有找到
                    {
                        break;
                    }
                }

                count+=2;
                for (i=0; i<48; i++)		 //�п���
                {
                    strh = (HZ_X_X_ptr_color[6*i+count]<<8)|(HZ_X_X_ptr_color[6*i+1+count]);
                    str = (HZ_X_X_ptr_color[6*i+2+count]<<24)|(HZ_X_X_ptr_color[6*i+3+count]<<16)|(HZ_X_X_ptr_color[6*i+4+count]<<8)|(HZ_X_X_ptr_color[6*i+5+count]);
                    for (k=0; k<48; k++)
                    {
                        if(k<16)
                        {
                            if ( (strh >> 15-k) & 0x01 == 0x01)
                            {
                                TFT_SetPoint(x+k,y+i,LineColor); // �ַ���ɫ
                            }
                            else
                            {
                                if(Mod ==1)
                                {
                                    TFT_SetPoint(x+k,y+i,FillColor); // ������ɫ
                                }
                            }
                        }
                        else
                        {
                            if ( (str >> 47-k) & 0x01 == 0x01)
                            {
                                TFT_SetPoint(x+k,y+i,LineColor); // �ַ���ɫ
                            }
                            else
                            {
                                if(Mod ==1)
                                {
                                    TFT_SetPoint(x+k,y+i,FillColor); // ������ɫ
                                }
                            }
                        }
                    }
                }
                pstr+=2;
                x+=48;
            }
            break;
            }
        }
        else
        {
            switch(dotwidth)
            {
            case 16://写8*16的数字
            {

                count=0;
                HZ_X_X_ptr_color=(  u8 *)&(ASC_8_16_struct[0].Index[0]);
                while(HZ_X_X_ptr_color[count]!=*pstr)
                {
                    count=count+17;
                    if(HZ_X_X_ptr_color[count]==0)//没有找到
                    {
                        break;
                    }
                }

                count+=1;
                for (i=0; i<16; i++)		 //列控制
                {

                    str = HZ_X_X_ptr_color[i+count];


                    for (k=0; k<8; k++)
                    {
                        if ( (str >> 7-k) & 0x01 == 0x01)
                        {
                            TFT_DrawPoint(x+k,y+i,LineColor); // 字符颜色
                        }
                        else
                        {
                            if(Mod == 1)
                            {
                                TFT_DrawPoint(x+k,y+i,FillColor); // 背景颜色
                            }
                        }
                    }

                }
                pstr+=1;

                x+=8;

            }
            break;


            case 24://写16*24的数字
            {

                count=0;
                HZ_X_X_ptr_color=(	u8 *)&(ASC_12_24_struct[0].Index[0]);
                while(HZ_X_X_ptr_color[count]!=*pstr)
                {
                    count=count+49;
                    if(HZ_X_X_ptr_color[count]==0)//没有找到
                    {
                        break;
                    }
                }

                count+=1;
                for (i=0; i<24; i++)		 //列控制
                {
                    str = (HZ_X_X_ptr_color[2*i+count]<<8)|(HZ_X_X_ptr_color[2*i+count+1]);
                    for (k=0; k<16; k++)
                    {
                        if ( (str >> 15-k) & 0x01 == 0x01)
                        {
                            TFT_DrawPoint(x+k,y+i,LineColor); // 字符颜色
                        }
                        else
                        {
                            if(Mod == 1)
                            {
                                TFT_DrawPoint(x+k,y+i,FillColor); // 背景颜色
                            }
                        }
                    }

                }
                pstr+=1;
                x+=12;
            }
            break;

            case 32://写32*32的数字
            {
                count=0;
                HZ_X_X_ptr_color=(	u8 *)&(ASC_16_32_struct[0].Index[0]);
                while(HZ_X_X_ptr_color[count]!=*pstr)
                {
                    count=count+65;
                    if(HZ_X_X_ptr_color[count]==0)//没有找到
                    {
                        break;
                    }
                }

                count+=1;
                for (i=0; i<32; i++)		 //列控制
                {

                    str = (HZ_X_X_ptr_color[2*i+count]<<8)|(HZ_X_X_ptr_color[2*i+count+1]);


                    for (k=0; k<16; k++)
                    {
                        if ( (str >> 15-k) & 0x01 == 0x01)
                        {
                            TFT_DrawPoint(x+k,y+i,LineColor); // 字符颜色
                        }
                        else
                        {
                            if(Mod == 1)
                            {
                                TFT_DrawPoint(x+k,y+i,FillColor); // 背景颜色
                            }
                        }
                    }

                }
                pstr+=1;
                x+=16;
            }
            break;
            }
        }
        if(x >= TFT_LCD_XSIZE-1) // 光标后移
        {
            switch(dotwidth)
            {
            case 16:
            {
                y += 16; // Y坐标
            }
            break;
            case 24:
            {
                y += 24; // Y坐标
            }
            break;
            case 32:
            {
                y += 32; // Y坐标
            }
            break;
            }
            x = 0; // X坐标
            if( y >= TFT_LCD_YSIZE-1 )
                y = 0;
        }
        TFT_SetCursor(x,y);
    }
}



//画一个触摸点
//用来校准用的
void TFT_drow_touch_point(unsigned int x,unsigned int y,u16 color)
{
    TFT_DrawLine(x-12,y,x+13,y,color);//横线
    TFT_DrawLine(x,y-12,x,y+13,color);//竖线
    TFT_DrawPoint(x+1,y+1,color);
    TFT_DrawPoint(x-1,y+1,color);
    TFT_DrawPoint(x+1,y-1,color);
    TFT_DrawPoint(x-1,y-1,color);
    TFT_draw_circle(x,y,6,color,0);//画中心圈
}


#ifdef TFT_LCD_TEST
void TFT_LCD_Test(void)
{
    u16 i,j;
    while(1)
    {
        for(i =0; i<4000; i++)
            for(j=0; j<5600; j++)
            {}
        TFT_draw_Rectangle(0,0,799,479,White,1);
        for(i =0; i<4000; i++)
            for(j=0; j<5600; j++)
            {}
        TFT_DrawLine(20,20,100,100,Red);
        TFT_draw_Rectangle(20, 20, 100, 100, GREEN,0);
        TFT_draw_Rectangle(110, 20, 190, 100, Blue,1);
        TFT_draw_circle(250,60,50,Red,0);
        TFT_draw_circle(350,60,50,Red,1);
        for(i =0; i<4000; i++)
            for(j=0; j<5600; j++)
            {}
        TFT_draw_Rectangle(0,0,799,479,Magenta,1);
        for(i =0; i<4000; i++)
            for(j=0; j<5600; j++)
            {}
        TFT_DispString(30,30,"ABCD1234",16,White,Black, NORMAL);
        TFT_DispString(30,60,"ABCD1234",24,White,Black, NORMAL);
        TFT_DispString(30,90,"ABCD1234",32,White,Black, NORMAL);

        TFT_DispString(30,140,"青岛科技1235ABCDabcd",16,Blue,Yellow, NORMAL);
        TFT_DispString(30,190,"青岛科技1235ABCD",24,Blue,Yellow, NORMAL);
        TFT_DispString(760,190,"青岛科技1235ABCD",24,Blue,Yellow, NORMAL);
        TFT_drow_touch_point(300,300,Red);
        for(i =0; i<4000; i++)
            for(j=0; j<5600; j++)
            {}
        TFT_draw_Rectangle(0,0,799,479,GREEN,1);
        for(i =0; i<4000; i++)
            for(j=0; j<5600; j++)
            {}
    }

}
#else
void TFT_LCD_Test(void)
{

}
#endif
