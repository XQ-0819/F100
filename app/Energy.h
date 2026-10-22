#ifndef _ENERGY_H_
#define _ENERGY_H_

#include "stm32f4xx.h"


#define UART_TIME_OUT 1000
#define UART_TIMES		30
#define CUR_CHK_TIMES		3
#define WATER_INTERVAL (60)



#define TYPE_MODBUS					(0)	//modbus
#define TYPE_DL645_97				(1)	//DL645-97
#define TYPE_DL645_07				(2)	//DL645-07
#define TYPE_CJT_188				(3)	//CJT188


#define FOOD_ADJUST_SYN				(0)	//握手
#define FOOD_ADJUST_ZERO			(1)	//零点
#define FOOD_ADJUST_NUM				(2)	//定值









extern u8 energy_enable[];
#pragma pack(push,1)

typedef struct {
    unsigned char		enable;		// 是否采集
    unsigned char		type;		// 采集类型 0-modbus 1-dlt97 2dlt07
    unsigned char		addr[7];	// 地址
    unsigned long 		reg;
    unsigned char 		reg_num;
} TSET_PARM, *PSET_PARM;

typedef struct {
    unsigned char		addr;		// 地址
    unsigned short		reg;		// 寄存器地址
    unsigned short		data;		// 写入数据
} TWR_PAPM, *PWR_PAPM;
#pragma pack(pop)




void Uart2_Rec_Task(void * pdata);
void Sensors_Task(void * pdata);

void food_adjust_int(unsigned char id);
int food_adjust_proc(unsigned char id, unsigned char type, unsigned short value);

#endif



