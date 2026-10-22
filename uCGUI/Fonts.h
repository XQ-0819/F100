/*
  ******************************************************************************
  * FILE:    ZK.h 
  * AUTHOR:  zouw96
  * VERSION: V1.0
  * DATE:    2011-12-20
  * BRIEF:   workbook
  ******************************************************************************
  * ATTENTION:
  *
  *    常用字符大小的结构体
  *
  *  COPYRIGHT 2011 , All rights reserved.
  ******************************************************************************
*/
#ifndef __ZK_H
#define __ZK_H
#include "stm32f4xx.h"

typedef struct         // 汉字字模数据结构
{
  	u8 Index[2];
  	u8  Msk[32];
}typFNT_GB16;


typedef struct         // 汉字字模数据结构
{
  	u8 Index[2];
  	u8  Msk[72];
}typFNT_GB24;


typedef struct         // 汉字字模数据结构
{
  	u8 Index[2];
  	u8  Msk[128];
}typFNT_GB32;
typedef struct         // 汉字字模数据结构
{
  	u8 Index[2];
  	u8  Msk[288];
}typFNT_GB48;

typedef struct                  												// 汉字字模数据结构
{
	u8 Index[1];               												// 汉字内码索引
	u8 Msk[16]; // 点阵码数据
}typFNT_GB16_ascii;

typedef struct                  												// 汉字字模数据结构
{
	u8 Index[1];               												// 汉字内码索引
	u8 Msk[48]; // 点阵码数据
}typFNT_GB24_ascii;


typedef struct                  												// 汉字字模数据结构
{
	u8 Index[1];               												// 汉字内码索引
	u8 Msk[64]; // 点阵码数据
}typFNT_GB32_ascii;





extern const typFNT_GB32_ascii ASC_16_32_struct[];

extern const typFNT_GB24_ascii ASC_12_24_struct[];


extern const typFNT_GB16_ascii ASC_8_16_struct[];



extern const typFNT_GB16 HZ16_16_struct[];
extern const typFNT_GB24 HZ24_24_struct[];
extern const typFNT_GB32 HZ32_32_struct[];
extern const typFNT_GB48 HZ48_48_struct[];

extern const unsigned char gImage_123[115000] ;




#endif





