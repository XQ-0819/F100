#ifndef __W25QXX_H__
#define __W25QXX_H__


/******************************************************************************

  文 件 名   : W25Q16.h
  版 本 号   : 初稿
  作    者   : 邹稳
  生成日期   : 2013年5月20日星期一
  最近修改   :
  功能描述   : W25Q16.c 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2013年5月20日星期一
    作    者   : 邹稳
    修改内容   : 创建文件

  2.日    期   : 2016年11月22日
    作    者   : 邹稳
    修改内容   : 修改支持W25Q256
******************************************************************************/
#define USE_F4 1

#ifdef USE_F4
#include "stm32f4xx.h"
#else
#include "stm32f10x.h"
#endif

#include "ucos_ii.h"
#include "stm324x9i_eval_lcd.h"

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/
// #define PICLOGO_SIZE  	(115000)// (500 * 115 * 2)
// #define PIC_SIZE  		(770048) 
// #define ICON_SIZE  		(850*1024)//max 752*2
// #define FONT16_SIZE  	(300*1024)
// #define FONT24_SIZE  	(450*1024)
// #define FONT32_SIZE  	(100*1024)
// #define MULTI_LANG_SIZE (100*1024)
#define PICLOGO_SIZE  	(115000)// (500 * 115 * 2)
#define PIC_SIZE  		  (770048) 
#define ICON_SIZE  		  (940*1024)//max 752*2
#define FONT16_SIZE  	  (300*1024)
#define FONT24_SIZE  	  (430*1024)
#define FONT32_SIZE  	  (100*1024)
#define MULTI_LANG_SIZE   (140*1024)


#define SDRAM_PIC_ADDR			(SDRAM_BANK_ADDR + 3 * BUFFER_OFFSET )//3个缓冲图层
#define SDRAM_ICON_ADDR			(SDRAM_PIC_ADDR+ 4 * PIC_SIZE)
#define SDRAM_GBK16_ADDR		(SDRAM_ICON_ADDR + ICON_SIZE)
#define SDRAM_GBK24_ADDR		(SDRAM_GBK16_ADDR + FONT16_SIZE)
#define SDRAM_GBK32_ADDR		(SDRAM_GBK24_ADDR + FONT24_SIZE)

#define SDRAM_LANG_ADDR			(SDRAM_GBK32_ADDR + FONT32_SIZE)

#define WLOGO_ID			(0)
#define WMAIN_ID			(1)
#define WBG_ID				(2)
#define WHOME_ID			(3)
#define WHOME2_ID			(4)
#define BACK_ID             (5)
#define WMAIN_ENG           (6)
#define WPIC_ID				(9)



#define init_addr 			(0)					//752K
#define update_init_addr 	(8272*1024)			//1K
#define soft_bak_addr 		(8273*1024)  		//1024K
#define soft_cur_addr 		(9297*1024) 		//1024K
#define gbk_16_addr 		(10321*1024)  		//1024K
#define gbk_24_addr 		(11345*1024)		//2048K
#define gbk_32_addr 		(13393*1024)		//114K
#define	SEASON_ADDR			(13500*1024)		//
#define his_data_addr 		(13584*1024)		//最后2800K数据存储 

//#define init_addr 			(0)					//752K
//#define update_init_addr 	(20304*1024)		//1K
//#define soft_bak_addr 		(20305*1024)  		//1024K
//#define soft_cur_addr 		(21329*1024) 		//1024K
//#define gbk_16_addr 		(22353*1024)  		//1024K
//#define gbk_24_addr 		(23377*1024)		//2048K
//#define gbk_32_addr 		(25425*1024)		//3072K 

/*****************************************************************************/


//#define W25Q80    0XEF13
//#define W25Q16    0XEF14
//#define W25Q32    0XEF15
//#define W25Q64    0XEF16	
//#define W25Q128   0XEF17	
//#define W25Q80 	0XEF13 	
//#define W25Q16 	0XEF14
//#define W25Q32 	0XEF15
//#define W25Q64 	0XEF16
#define W25Q128	0XEF17
#define W25Q256	0XEF18

#define	SPI_FLASH_CS_L 	GPIO_ResetBits(GPIOA, GPIO_Pin_15)  //选中FLASH					 
#define	SPI_FLASH_CS_H  GPIO_SetBits(GPIOA, GPIO_Pin_15)  //选中FLASH	

//指令表
#define W25X_WriteEnable		0x06
#define W25X_WriteDisable		0x04
#define W25X_ReadStatusReg		0x05
#define W25X_WriteStatusReg		0x01
#define W25X_ReadStatusReg_3	0x15
#define W25X_WriteStatusReg_3	0x11
#define W25X_ReadData			0x03
#define W25X_ReadData_4			0x13
#define W25X_FastReadData		0x0B
#define W25X_FastReadDual		0x3B
#define W25X_PageProgram		0x02
#define W25X_BlockErase			0xD8
#define W25X_SectorErase		0x20
#define W25X_ChipErase			0xC7
#define W25X_PowerDown			0xB9
#define W25X_ReleasePowerDown	0xAB
#define W25X_DeviceID			0xAB
#define W25X_ManufactDeviceID	0x90
#define W25X_JedecDeviceID		0x9F

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

extern u8 Flash_SPIx_ReadWriteByte(u8 TxData);
extern void SPI_Flash_Erase_Chip(void);
extern void SPI_Flash_Erase_Sector(u32 Dst_Addr);
extern void SPI_Flash_Init(void);
extern void SPI_Flash_PowerDown(void);
extern void SPI_Flash_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead);
extern u16 SPI_Flash_ReadID(void);
extern u8 SPI_Flash_ReadSR(void);
extern void SPI_Flash_Wait_Busy(void);
extern void SPI_Flash_WAKEUP(void);
extern u8 SPI_Flash_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
extern void SPI_FLASH_Write_Disable(void);
extern void SPI_FLASH_Write_Enable(void);
extern void SPI_Flash_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
extern void SPI_Flash_Write_Page(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
extern void SPI_FLASH_Write_SR(u8 sr);
extern void SPI_Flash_Enter_4Byte(void);
extern void SPI_Flash_Exit_4Byte(void);
extern u8 SPI_Flash_ReadAdd(void);
extern void SPI_FLASH_WriteAdd(u8 sr);
extern u8 SPI_Flash_ReadSR3(void);
extern void SPI_FLASH_Write_SR3(u8 sr);
extern void SPI_FLASH_DMA_RxConfig(void);
extern void SPI_Flash_DMA_Mode(void);
extern void SPI_Flash_SPI_Mode(void);

extern int W25Qxx_Test(void);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __W25Q16_H__ */
