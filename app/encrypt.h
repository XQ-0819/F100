#ifndef _ENCRYPT_H_
#define _ENCRYPT_H_

#define USE_F1 			(0)
#define USE_BOOT 		(0)

// chipID地址

#if USE_F1 == 1
// F1系列
#include "stm32f10x.h"
#define UID0 0X1F
#define UID1 0XFF
#define UID2 0XF7
#define UID3 0XE8
//根据实际情况选择
#define FLASH_SECTOR_SIZE 0x800 /* Sector  size: 2 Kbytes */
#define ADDR_FLASH_SECTOR ((uint32_t)0x08006000) /* Base @ of Sector  2 Kbytes */
#else
// F4系列
#include "stm32f4xx.h"
#define UID0 0X1F
#define UID1 0XFF
#define UID2 0X7A
#define UID3 0X10

//根据实际情况选择
#define FLASH_SECTOR_SIZE 0x4000 /* Sector  size: 16 Kbytes */
#define ADDR_FLASH_SECTOR ((uint32_t)0x08004000) /* Base @ of Sector  16 Kbytes */
#define ADDR_FLASH_REAL	  (FLASH_Sector_1) 
#endif





// chipID地址在crc_tmp_data中的存储坐标
#define UID0_OFFSET 0x22
#define UID1_OFFSET 0x31
#define UID2_OFFSET 0x43
#define UID3_OFFSET_1 0x52
#define UID3_OFFSET_2 0x63
#define UID3_OFFSET_3 0x74
// 通用密码
#define eID0 0xA3
#define eID1 0x2E
#define eID2 0x55
#define eID3 0x6F
#define eID4 0x1B
#define eID5 0xC0
#define eID6 0xE3
#define eID7 0x81
#define eID8 0x45
#define eID9 0x50
#define eID10 0xA3
#define eID11 0x27
// 通用密码在crc_tmp_data中的存储坐标
#define eID0_OFFSET 0x07
#define eID1_OFFSET 0x19
#define eID2_OFFSET 0x29
#define eID3_OFFSET 0x36
#define eID4_OFFSET 0x48
#define eID5_OFFSET 0x69
#define eID6_OFFSET 0x0A
#define eID7_OFFSET 0x1A
#define eID8_OFFSET 0x2C
#define eID9_OFFSET 0x37
#define eID10_OFFSET 0x4A
#define eID11_OFFSET 0x5A

int Check_ID(void);
int NoWrite_Check_ID(void);
uint8_t * Get_McuID(void);
uint32_t Get_McuID8(uint8_t id);
uint8_t Get_McuID_Len(void);

#endif

