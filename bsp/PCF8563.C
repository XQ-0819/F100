#include <stdio.h>
#include <time.h>
#include <string.h>
#include "stm32f4xx.h"
#include "ucos_ii.h"
#include "PCF8563.h"
#include "app.h"


#define WRITE_PCF8563	(0xA2)	// 写入地址
#define READ_PCF8563	(0xA3)	// 读出地址 

#define PCF8563_CTR1	(0x00)	// control/status register1
#define PCF8563_CTR2	(0x01)	// control/status register2
#define PCF8563_SEC		(0x02)	// second,bit0-6:bcd(0x00-0x59)
#define PCF8563_MIN		(0x03)	// minute,bit0-6:bcd(0x00-0x59)
#define PCF8563_HOUR	(0x04)	// hour,bit0-5:bcd(0x00-0x59)
#define PCF8563_DAY		(0x05)	// day,bit0-5:bcd(0x01-0x31)
#define PCF8563_WEEK	(0x06)	// week,bit0-2(0-6)
#define PCF8563_MON		(0x07)	// month,bit0-4,bcd(0x01-0x12)
#define PCF8563_YEAR	(0x08)	// year,bit0-7,bcd(0x00-0x99)

#if USE_GD == 1
#define PCF8563_SCL1				GPIO_Pin_13
#define PCF8563_SDA1				GPIO_Pin_7
#define PCF8563_SCL_GPIO_PORT1	GPIOG
#define PCF8563_SCL_GPIO_CLK1	RCC_AHB1Periph_GPIOG
#define PCF8563_SDA_GPIO_PORT1	GPIOD
#define PCF8563_SDA_GPIO_CLK1	RCC_AHB1Periph_GPIOD
#else
#define PCF8563_SCL1				GPIO_Pin_1
#define PCF8563_SDA1				GPIO_Pin_5
#define PCF8563_SCL_GPIO_PORT1	GPIOB
#define PCF8563_SCL_GPIO_CLK1	RCC_AHB1Periph_GPIOB
#define PCF8563_SDA_GPIO_PORT1	GPIOC
#define PCF8563_SDA_GPIO_CLK1	RCC_AHB1Periph_GPIOC


#define PCF8563_SCL2				GPIO_Pin_3
#define PCF8563_SDA2				GPIO_Pin_5
#define PCF8563_SCL_GPIO_PORT2	GPIOG
#define PCF8563_SCL_GPIO_CLK2	RCC_AHB1Periph_GPIOG
#define PCF8563_SDA_GPIO_PORT2	GPIOC
#define PCF8563_SDA_GPIO_CLK2	RCC_AHB1Periph_GPIOC
#endif

static u32 PCF8563_SCL_GPIO_CLK = PCF8563_SCL_GPIO_CLK1;
static u32 PCF8563_SDA_GPIO_CLK = PCF8563_SDA_GPIO_CLK1;
static GPIO_TypeDef *PCF8563_SCL_GPIO_PORT = PCF8563_SCL_GPIO_PORT1;
static GPIO_TypeDef *PCF8563_SDA_GPIO_PORT = PCF8563_SDA_GPIO_PORT1;
static u16 PCF8563_SCL = PCF8563_SCL1;
static u16 PCF8563_SDA = PCF8563_SDA1;


#define PCF8563_SCL_H         GPIO_SetBits(PCF8563_SCL_GPIO_PORT, PCF8563_SCL)
#define PCF8563_SCL_L         GPIO_ResetBits(PCF8563_SCL_GPIO_PORT, PCF8563_SCL) 
   
#define PCF8563_SDA_H         GPIO_SetBits(PCF8563_SDA_GPIO_PORT, PCF8563_SDA)
#define PCF8563_SDA_L         GPIO_ResetBits(PCF8563_SDA_GPIO_PORT, PCF8563_SDA) 

#define PCF8563_SCL_read      GPIO_ReadInputDataBit(PCF8563_SCL_GPIO_PORT, PCF8563_SCL) 
#define PCF8563_SDA_read      GPIO_ReadInputDataBit(PCF8563_SDA_GPIO_PORT, PCF8563_SDA) 

#pragma pack(push)
#pragma pack(1)

typedef struct{
	u8	sec;	// 0x0-59
	u8	min;	// 0x0-59
	u8	hour;	// 0x0-23
	u8	day;	// 0x1-31
	u8	week;	// 0x0-6
	u8	mon;	// 0x1-12
	u8	year;	// 0x0-99
}HW_DATE_TIME,*PHW_DATE_TIME;

#pragma pack(pop)

extern OS_EVENT      *eeprom_Sem;

static void I2C_delay( void )
{	
   volatile u8 i = 80;
   
   while( i ){
     i--; 
   }
}

void I2C_Start( void )
{
	PCF8563_SDA_H;
	I2C_delay( );
	PCF8563_SCL_H;
	I2C_delay( );
	PCF8563_SDA_L;
	I2C_delay( );
	PCF8563_SCL_H;
	I2C_delay( );
}

void I2C_Stop( void )
{
	PCF8563_SCL_L;
	I2C_delay( );
	PCF8563_SDA_L;
	I2C_delay( );
	PCF8563_SCL_H;
	I2C_delay( );
	PCF8563_SDA_H;
	I2C_delay( );
}

void I2C_Ack( void )
{	
	PCF8563_SCL_L;
	I2C_delay( );
	PCF8563_SDA_L;
	I2C_delay( );
	PCF8563_SCL_H;
	I2C_delay( );
	PCF8563_SCL_L;
	I2C_delay( );
}

void I2C_NoAck( void )
{	
	PCF8563_SCL_L;
	I2C_delay( );
	PCF8563_SDA_H;
	I2C_delay( );
	PCF8563_SCL_H;
	I2C_delay( );
	PCF8563_SCL_L;
	I2C_delay( );
}

//返回为:=1有ACK,=0无ACK
void I2C_WaitAck( void )
{
	u8 ucErrTime=0;	
	
	PCF8563_SCL_L;
	I2C_delay( );
	PCF8563_SDA_H;			
	I2C_delay( );
	PCF8563_SCL_H; // SDA应答信号是由时钟芯片发出的，CPU先拉高，应该是由时钟芯片拉低
	I2C_delay( );
	if( PCF8563_SDA_read )
	{
		ucErrTime++;
		if( ucErrTime>250 ){
			I2C_Stop();
		}
	}
	PCF8563_SCL_L;
}

void I2C_SendByte( u8 SendByte )
{
    u8 i=8;
    
    while( i-- )
    {
        PCF8563_SCL_L;
        I2C_delay( );
		if( SendByte&0x80 ){
			PCF8563_SDA_H;
		}else{
			PCF8563_SDA_L;
		}
        SendByte <<= 1;
        I2C_delay( );
		PCF8563_SCL_H;
        I2C_delay( );
    }
    PCF8563_SCL_L;
    I2C_delay( );
}

u8 I2C_ReceiveByte( void )
{ 
    u8 i=8;
    u8 ReceiveByte=0;
    
	PCF8563_SCL_L;
	I2C_delay();
    PCF8563_SDA_H;				
	while( i-- )
    {
		ReceiveByte <<= 1;      
		PCF8563_SCL_L;
		I2C_delay( );
		PCF8563_SCL_H;
		I2C_delay( );	
		if( PCF8563_SDA_read ){
			ReceiveByte |= 0x01;
		}
    }
    PCF8563_SCL_L;
    
    return ReceiveByte;
}

u8 PCF8563_Read_State(void)
{
	u8 state;
	I2C_Start();
	I2C_SendByte(WRITE_PCF8563);
	I2C_WaitAck();
	I2C_SendByte(PCF8563_CTR1);
	I2C_WaitAck();
	I2C_Start();
	I2C_SendByte(READ_PCF8563);
	I2C_WaitAck();
	state=I2C_ReceiveByte();
	I2C_Ack();
	I2C_NoAck();
	I2C_Stop();
	return state;
}

void PCF8563_Write_State(void)
{		

	I2C_Start();
	I2C_SendByte(WRITE_PCF8563);      
	I2C_WaitAck();
    
    I2C_SendByte(PCF8563_CTR1);
	I2C_WaitAck();
   
	I2C_SendByte(0);//    
	I2C_WaitAck();	
	I2C_Stop();	
}

/*
* function: PCF8563_Init
* brief   : pcf8563 init
* param   : void
* result  : void
*/
void PCF8563_Init( void )
{
	GPIO_InitTypeDef  GPIO_InitStructure;

    if (get_hardware_ver() >= HARD_V4) {
		PCF8563_SCL_GPIO_CLK = PCF8563_SCL_GPIO_CLK2;
		PCF8563_SDA_GPIO_CLK = PCF8563_SDA_GPIO_CLK2;
		PCF8563_SCL_GPIO_PORT = PCF8563_SCL_GPIO_PORT2;
		PCF8563_SDA_GPIO_PORT = PCF8563_SDA_GPIO_PORT2;
		PCF8563_SCL = PCF8563_SCL2;
		PCF8563_SDA = PCF8563_SDA2;
    } else {
		PCF8563_SCL_GPIO_CLK = PCF8563_SCL_GPIO_CLK1;
		PCF8563_SDA_GPIO_CLK = PCF8563_SDA_GPIO_CLK1;
		PCF8563_SCL_GPIO_PORT = PCF8563_SCL_GPIO_PORT1;
		PCF8563_SDA_GPIO_PORT = PCF8563_SDA_GPIO_PORT1;
		PCF8563_SCL = PCF8563_SCL1;
		PCF8563_SDA = PCF8563_SDA1;
    }

	
	RCC_AHB1PeriphClockCmd(PCF8563_SCL_GPIO_CLK | PCF8563_SDA_GPIO_CLK, ENABLE);
    
	GPIO_InitStructure.GPIO_Pin = PCF8563_SCL ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(PCF8563_SCL_GPIO_PORT, &GPIO_InitStructure);
	
 	GPIO_InitStructure.GPIO_Pin = PCF8563_SDA ;
	GPIO_Init(PCF8563_SDA_GPIO_PORT, &GPIO_InitStructure); 
	PCF8563_Write_State();
	
	SyncSysTime( );
}

/*设置地址的内容*/ 
void PCF8563_Write( u8 addr,u8 Data )
{
	I2C_Start( );
	I2C_SendByte( WRITE_PCF8563 );
	I2C_WaitAck( );
	I2C_SendByte( addr );
	I2C_WaitAck( );
	I2C_SendByte( Data );
	I2C_WaitAck( );
	I2C_Stop( );
}

/*读取地址的内容*/ 
u8 PCF8563_Read( u8 addr )
{	
	u8	data;

	I2C_Start( );
	I2C_SendByte( WRITE_PCF8563 );  
	I2C_WaitAck( );
	I2C_SendByte( addr );
	I2C_WaitAck( );
	I2C_Start( );
	I2C_SendByte( READ_PCF8563 );  
	I2C_WaitAck( );
	data = I2C_ReceiveByte( );
	I2C_Stop( );
	
	return data;
}

void PCF8563_CLKOUT_1s( void )
{
	PCF8563_Write(0x01, 0);		//禁止定时器输出，闹铃输出
	PCF8563_Write(0x0e, 0);		//关闭定时器等等
	PCF8563_Write(0x0d, 0x83);	//打开输出脉冲
}

/*
* function: PCF8563_GetDT
* brief   : get date-time
* param   : pDT[in/out]:date-time buffer
* result  : -1:param error
*            0:ok
*/
int PCF8563_GetDT( PDATE_TIME pDT )
{
	u8		i;
	INT8U	err;
	BYTE*	dtBuf;
	HW_DATE_TIME hwDT;
	
	if( pDT==NULL ){
		return -1;
	}
	OSSemPend( eeprom_Sem,0,&err );

	I2C_Start( );
	I2C_SendByte( WRITE_PCF8563 );
	I2C_WaitAck( );
	I2C_SendByte( PCF8563_SEC );
	I2C_WaitAck( );
	I2C_Start( );
	I2C_SendByte( READ_PCF8563 );
	I2C_WaitAck( );

	dtBuf=(BYTE*)&hwDT;
	for( i=0; i<sizeof(HW_DATE_TIME); i++ ){
		dtBuf[i]=I2C_ReceiveByte();
		if( i!=6 ){ // 最后一个数据不应答
			I2C_Ack( );
		}
	}
	I2C_NoAck( );
	I2C_Stop( );

	OSSemPost( eeprom_Sem );

	pDT->year= hwDT.year;
	pDT->mon = hwDT.mon & 0x1F;
	pDT->day = hwDT.day & 0x3F;
	pDT->hour= hwDT.hour & 0x3F;
	pDT->min = hwDT.min & 0x7F;
	pDT->sec = hwDT.sec & 0x7F;

	return 0;
}

static volatile time_t	tSysTime;
/*
* function: Bcd2Hex
* brief   : convert bcd to hex
* param   : bcd[in]:bcd code
* result  : -1:bcd invalid
*          >=0:hex value
*/
int Bcd2Hex( BYTE bcd )
{
	int		tens,units;
	
	tens=(bcd>>4) & 0x0f;
	units=bcd & 0x0f;
	if( tens>9 || units>9 ){
		return -1;
	}
	
	return (tens*10+units);
}
int Hex2Bcd( BYTE hex )
{
	int		tens,units;
	if(hex>99){
		return -1;
	}
	tens = hex/10;
	units = hex%10;

	return (tens*0x10+units);
}
/*
* function: DT2SysTm
* brief   : datatime to system time
* param   : pDT[in]:date-time
* result  : -1:param error
*         else:system time
*/
time_t DT2SysTm( PDATE_TIME pDT )
{
	time_t		t;
	struct tm	tt;
	
	if( pDT==NULL ){
		return (time_t)(-1);
	}
	
	memset( &tt,0,sizeof(tt) );
	tt.tm_year=100+Bcd2Hex(pDT->year);
	tt.tm_mon=Bcd2Hex(pDT->mon)-1;
	tt.tm_mday=Bcd2Hex( pDT->day );
	tt.tm_hour=Bcd2Hex( pDT->hour );
	tt.tm_min=Bcd2Hex( pDT->min );
	tt.tm_sec=Bcd2Hex( pDT->sec );
	t=mktime( &tt );
	
	return t;
}

/*
* function: PCF8563_SetDT
* brief   : set date-time
* param   : pDT[in]:data-time
* result  : -1:param error
*            0:ok
*/
int PCF8563_SetDT( PDATE_TIME pDT )
{
	time_t		t;
	struct tm	tt;
	INT8U		err;
	
	if( pDT==NULL )	{
		return -1;
	}
	
	memset( &tt,0,sizeof(tt) );
	tt.tm_year=100+Bcd2Hex(pDT->year);
	tt.tm_mon=Bcd2Hex(pDT->mon)-1;
	tt.tm_mday=Bcd2Hex( pDT->day );
	tt.tm_hour=Bcd2Hex( pDT->hour );
	tt.tm_min=Bcd2Hex( pDT->min );
	tt.tm_sec=Bcd2Hex( pDT->sec );
	t=mktime( &tt );
	if( t==(time_t)(-1) ){
		return -2;
	}
	tSysTime=t;
	_localtime_r( (const time_t *)&tSysTime,&tt );
	
	OSSemPend( eeprom_Sem,0,&err );

	I2C_Start( );
	I2C_SendByte( WRITE_PCF8563 );
	I2C_WaitAck( );
    
    // 设置写起始地址
    I2C_SendByte( PCF8563_SEC );
	I2C_WaitAck( );
    
	I2C_SendByte( pDT->sec );
	I2C_WaitAck( );	
	I2C_SendByte( pDT->min );
	I2C_WaitAck( );
	// hour,二十四小时制
	I2C_SendByte( pDT->hour );
	I2C_WaitAck( );	
	I2C_SendByte( pDT->day );
	I2C_WaitAck( );
	// week
	I2C_SendByte( tt.tm_wday );
	I2C_WaitAck( );
	I2C_SendByte( pDT->mon );
	I2C_WaitAck( );	
	I2C_SendByte( pDT->year );
	I2C_WaitAck( );	
	I2C_Stop( );
	
	OSSemPost( eeprom_Sem );

	return 0;
}

/*
* function: IncSecond
* brief   : increment system time
* param   : void
* result  : void
*/
void IncSecond( void )
{
	tSysTime++;
}

/*
* function: GetSysTime
* brief   : get system time
* param   : void
* result  : system time
*/
time_t GetSysTime( void )
{
	return tSysTime;
}

/*
* function: SyncSysTime
* brief   : sync system time
* param   : void
* result  : void
*/
void SyncSysTime( void )
{
	struct tm	tt;
	TDATE_TIME	dt;
	
	PCF8563_GetDT( &dt );
	memset( &tt,0,sizeof(tt) );
	tt.tm_year=100+Bcd2Hex(dt.year);
	tt.tm_mon=Bcd2Hex(dt.mon)-1;
	tt.tm_mday=Bcd2Hex( dt.day );
	tt.tm_hour=Bcd2Hex( dt.hour );
	tt.tm_min=Bcd2Hex( dt.min );
	tt.tm_sec=Bcd2Hex( dt.sec );
	tSysTime=mktime( &tt );
}



