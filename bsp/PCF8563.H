#ifndef PCF8563_H_
#define PCF8563_H_

#include <time.h>
#include "types.h"

#define DAY_TO_MIN		(1440)
#define DAY_TO_SEC		(86400)
#define HOUR_TO_SEC		(3600)
#define MIN_TO_SEC		(60)
#define DAY_TO_HOUR		(24)


#pragma pack(push)
#pragma pack(1)

// 
typedef struct{
	BYTE	year;	// current year minus 2000
	BYTE	mon;	// 1-12
	BYTE	day;	// 1-31
	BYTE	hour;	// 0-23
	BYTE	min;	// 0-59
	BYTE	sec;	// 0-59
}TDATE_TIME,*PDATE_TIME;

#pragma pack(pop)

int Bcd2Hex( BYTE bcd );
int Hex2Bcd( BYTE hex );
time_t DT2SysTm( PDATE_TIME pDT );
/*
* function: IncSecond
* brief   : increment system time
* param   : void
* result  : void
*/
void IncSecond( void );

/*
* function: GetSysTime
* brief   : get system time
* param   : void
* result  : system time
*/
time_t GetSysTime( void );

/*
* function: SyncSysTime
* brief   : sync system time
* param   : void
* result  : void
*/
void SyncSysTime( void );

/*
* function: PCF8563_Init
* brief   : pcf8563 init
* param   : void
* result  : void
*/
void PCF8563_Init( void );

/*
* function: PCF8563_GetDT
* brief   : get date-time
* param   : pDT[in/out]:date-time buffer
* result  : -1:param error
*            0:ok
*/
int PCF8563_GetDT( PDATE_TIME pDT );

/*
* function: PCF8563_SetDT
* brief   : set date-time
* param   : pDT[in]:data-time
* result  : -1:param error
*            0:ok
*/
int PCF8563_SetDT( PDATE_TIME pDT );


#endif
