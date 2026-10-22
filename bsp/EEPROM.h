#ifndef __EEPROM_H
#define __EEPROM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h"



#define ADDR_INIT		(0x0000)
#define ADDR_SETINF 	(0x1000)
#define ADDR_ENERGY 	(0x1600)
#define ADDR_DATA	 	(0x1800)




/***********************************************************************
-------------------------function define   ----------------------------
***********************************************************************/


unsigned char EEPROM_Write(unsigned int WriteAddress,unsigned char* pBuffer,unsigned short NumbyteToWrite);
unsigned char EEPROM_Read(unsigned int ReadAddress,unsigned char* pBuffer,unsigned short NumbyteToWrite);
void EEPROM_Config(void);





#ifdef __cplusplus
}
#endif

#endif
