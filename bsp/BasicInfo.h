/**
 *    文件名：	BasicInfo.h
 *    说明：  	所有文件都可能用到的一些重定义类型,
 *    			被包含于stm32f10x_conf.h   
 *    创建时间：2013-01-07
 */

#ifndef   __BASICINFO_H
#define   __BASICINFO_H


/* 重新定义一些类型，防止由于各种平台和编译器的不同，而产生的类型字节数差异*/
#define  INT8U u8												// Unsigned  8 bit quantity 	
#define  INT8S s8     										// Signed    8 bit quantity  	
#define  INT16U u16   	    											// Unsigned  16 bit quantity  
#define  INT16S s16   	     										// Signed    16 bit quantity   
#define  INT32U u32  	    											// Unsigned  32 bit quantity  	
#define  INT32S s32  	   
#define  INT64S long  long  	   

#define wrong 0
#define right 1

#define false 0
#define true  1

#define NULL 0

#define setbit(x,y)  		(x|=((uint64_t)(1)<<y))
#define clrbit(x,y)  		(x&=~((uint64_t)(1)<<y))
#define reversebit(x,y)  	(x^=(1<<y))
#define getbit(x,y)   		(((x) >> (y))&1)


///////////////////////////////////////////////////////////////
//位带操作,实现51类似的GPIO控制功能
//具体实现思想,参考<<CM3权威指南>>第五章(87页~92页).
//IO口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr&0xFFFFF)<<5)+(bitnum<<2))
#define MEM_ADDR(addr) *((volatile unsigned long *)(addr))
#define BIT_ADDR(addr, bitnum) MEM_ADDR(BITBAND(addr, bitnum))
//F4XXIO 口地址映射
#define GPIOA_ODR_Addr (GPIOA_BASE+20) //0x40020014
#define GPIOB_ODR_Addr (GPIOB_BASE+20) //0x40020414
#define GPIOC_ODR_Addr (GPIOC_BASE+20) //0x40020814
#define GPIOD_ODR_Addr (GPIOD_BASE+20) //0x40020C14
#define GPIOE_ODR_Addr (GPIOE_BASE+20) //0x40021014
#define GPIOF_ODR_Addr (GPIOF_BASE+20) //0x40021414
#define GPIOG_ODR_Addr (GPIOG_BASE+20) //0x40021814
#define GPIOH_ODR_Addr (GPIOH_BASE+20) //0x40021C14
#define GPIOI_ODR_Addr (GPIOI_BASE+20) //0x40022014
#define GPIOA_IDR_Addr (GPIOA_BASE+16) //0x40020010
#define GPIOB_IDR_Addr (GPIOB_BASE+16) //0x40020410
#define GPIOC_IDR_Addr (GPIOC_BASE+16) //0x40020810
#define GPIOD_IDR_Addr (GPIOD_BASE+16) //0x40020C10
#define GPIOE_IDR_Addr (GPIOE_BASE+16) //0x40021010
#define GPIOF_IDR_Addr (GPIOF_BASE+16) //0x40021410
#define GPIOG_IDR_Addr (GPIOG_BASE+16) //0x40021810
#define GPIOH_IDR_Addr (GPIOH_BASE+16) //0x40021C10
#define GPIOI_IDR_Addr (GPIOI_BASE+16) //0x40022010
//F4XXIO 口操作 , 只对单一的 IO 口 ,n 的值范围 [0-15]
#define PAout(n) BIT_ADDR(GPIOA_ODR_Addr,n) //GPIOA 某一位输出
#define PAin(n) BIT_ADDR(GPIOA_IDR_Addr,n) //GPIOA 某一位输入
#define PBout(n) BIT_ADDR(GPIOB_ODR_Addr,n) //GPIOB 某一位输出
#define PBin(n) BIT_ADDR(GPIOB_IDR_Addr,n) //GPIOB 某一位输入
#define PCout(n) BIT_ADDR(GPIOC_ODR_Addr,n) //GPIOC 某一位输出
#define PCin(n) BIT_ADDR(GPIOC_IDR_Addr,n) //GPIOC 某一位输入
#define PDout(n) BIT_ADDR(GPIOD_ODR_Addr,n) //GPIOD 某一位输出
#define PDin(n) BIT_ADDR(GPIOD_IDR_Addr,n) //GPIOD 某一位输入
#define PEout(n) BIT_ADDR(GPIOE_ODR_Addr,n) //GPIOE 某一位输出
#define PEin(n) BIT_ADDR(GPIOE_IDR_Addr,n) //GPIOE 某一位输入
#define PFout(n) BIT_ADDR(GPIOF_ODR_Addr,n) //GPIOF 某一位输出
#define PFin(n) BIT_ADDR(GPIOF_IDR_Addr,n) //GPIOF 某一位输入
#define PGout(n) BIT_ADDR(GPIOG_ODR_Addr,n) //GPIOG 某一位输出
#define PGin(n) BIT_ADDR(GPIOG_IDR_Addr,n) //GPIOG 某一位输入
#define PHout(n) BIT_ADDR(GPIOH_ODR_Addr,n) //GPIOH 某一位输出
#define PHin(n) BIT_ADDR(GPIOH_IDR_Addr,n) //GPIOH 某一位输入
#define PIout(n) BIT_ADDR(GPIOI_ODR_Addr,n) //GPIOI 某一位输出
#define PIin(n) BIT_ADDR(GPIOI_IDR_Addr,n) //GPIOI 某一位输入
/////////////////////////////////////////////////////////////////
#endif
