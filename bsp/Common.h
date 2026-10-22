#ifndef __COMMON_H
#define __COMMON_H 			   
#include "stm32f4xx.h"



void delay_1ms(u16 nms);
void delay_ms(u16 time);
void delay_1us(u32 nus);
void delay_us(u16 time);

u16 CRC16(u8 *puchMsg,u16 usDataLen);
u16 crc16_s(u8 *puchMsg, u16 usDataLen);
u8* mycpy( u8 *dest, u8 *src, u16 count );



#endif

