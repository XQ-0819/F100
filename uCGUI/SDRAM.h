/**
 *    文件名：	SDRAM.h
 *    说明：	包含了所有可用到的头文件
 *    			
 *    			
 *    创建时间：2014-02-23
 */




#ifndef   __SDRAM_H
#define   __SDRAM_H



#include "stm32f4xx.h"


/* Private defines -----------------------------------------------------------*/
#define SDRAM_BANK_ADDR                 ((uint32_t)0xC0000000)

/* #define SDRAM_MEMORY_WIDTH            FMC_SDMemory_Width_8b  */
#define SDRAM_MEMORY_WIDTH            FMC_SDMemory_Width_16b
/* #define SDRAM_MEMORY_WIDTH               FMC_SDMemory_Width_32b */ 

#if USE_GD == 1
#define SDCLOCK_PERIOD                   FMC_SDClock_Period_3
#else	
#define SDCLOCK_PERIOD                   FMC_SDClock_Period_2
#endif

#define SDRAM_TIMEOUT     ((uint32_t)0xFFFF) 

#define SDRAM_MODEREG_BURST_LENGTH_1             ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2             ((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4             ((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8             ((uint16_t)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL      ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2              ((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3              ((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD    ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000) 
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((uint16_t)0x0200) 
  

void SDRAM_FMC_Config(void);
void SDRAM_WriteBuffer(uint32_t* pBuffer, uint32_t uwWriteAddress, uint32_t uwBufferSize);
void SDRAM_ReadBuffer(uint32_t* pBuffer, uint32_t uwReadAddress, uint32_t uwBufferSize);

#endif
