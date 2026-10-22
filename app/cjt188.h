/*
* author: lss
* date  : 2018/9/28
* brief : implement cj/t188-2004 interface
*
* message format:
* 帧起始符 仪表类型 地址域  控制码 数据域长度 数据域  校验码 结束符
*   0x68      T     A0..A6   ctrl     len      data     cs    0x16
*
* remark:
* len:when read data,<=0x64
*         write data,<=0x32
* cs: sum starting from 0x68 to data ending
* M-bus: positive logical,
*        12V < Ubus <= 42V, 
*        Islave<=2mA,Imaster-drive:<=64,
*        Baud: 300bps~9600bps
*/


#ifndef CJT188_H
#define CJT188_H

#include "types.h"


#define CJT_LEAD	(0xFE)
#define CJT188_STX	(0x68)
#define CJT188_ETX	(0x16)
#define CJT_ADDR_SIZE	(7)
#define CJT_DI_SIZE		(2)
#define CJT_BDIR_ACK	(0x80)	// 通讯方向
#define CJT_BERR		(0x40)	// 

#define CJT188_TIMEOUT_MS	(1000)	// 单位: 毫秒,based baud 1200
#define CJT188_RETRY		(3)		// 重试次数

// WATER METER: 0x10..0x19
#define TYPE_WATER_COLD		(0x10)	// 冷水水表
#define TYPE_WATER_HOT		(0x11)	// 生活热水水表
#define TYPE_WATER_DRINK	(0x12)	// 直饮水水表
#define TYPE_WATER_GREY		(0x13)	// 中水水表
// HEAT METER: 0x20..0x29
#define TYPE_HEAT_HOT		(0x20)	// 计热表
#define TYPE_HEAD_COLD		(0x21)	// 计冷表
// GAT METER: 0x30..0x39
#define TYPE_GAS			(0x30)	// 燃气表
// OTHER METER: 0x40
#define TYPE_AMMETER		(0x40)	// 电度表

#define ADDR_BCAST			(0xAA)	// 广播地址

//
#define CJT188_FUNC_RD			(0x01)	// 读数据 
#define CJT188_FUNC_RD_ADDR		(0x03)	// 读地址(表号)
#define CJT188_FUNC_WR			(0x04)	// 写数据
#define CJT188_FUNC_RD_KEYVER	(0x09)	// 读密钥版本号
#define CJT188_FUNC_WR_ADDR		(0x15)	// 写地址(表号)
#define CJT188_FUNC_WR_INIT		(0x16)	// 写机电同步数(置表底数)
#define CJT188_FUNC_USER		(0x20)	// 厂商自定义

// 单位编码
#define CJT188_UNIT_J		(0x01)
#define CJT188_UNIT_KJ		(0x0B)
#define CJT188_UNIT_MJ		(0x0E)
#define CJT188_UNIT_GJ		(0x11)
#define CJT188_UNIT_GJX100	(0x13)

#define CJT188_UNIT_W		(0x14)
#define CJT188_UNIT_KW		(0x17)
#define CJT188_UNIT_MW		(0x1A)

#define CJT188_UNIT_WH		(0x02)
#define CJT188_UNIT_KWH		(0x05)
#define CJT188_UNIT_MWH		(0x08)
#define CJT188_UNIT_MWHX100	(0x0A)

#define CJT188_UNIT_L		(0x29)
#define CJT188_UNIT_M3		(0x2C)
#define CJT188_UNIT_LH		(0x32)	// L/h
#define CJT188_UNIT_M3H		(0x35)	// m3/h



#pragma pack(push)
#pragma pack(1)

typedef struct{
	BYTE	stx;	// 帧边界定位符
	BYTE	type;	// 仪表类型
	BYTE	addr[CJT_ADDR_SIZE];// 仪表地址: A0..A6
	union{
		BYTE	all;
		struct{
			BYTE	func:6;	// 功能定义
			BYTE	err:1;	// 异常
			BYTE	dir:1;	// 传输方向
		}bits;
	}ctrl;	// 控制码
	BYTE	len;	// 数据长度
}TCJT188_HEAD,*PCJT188_HEAD;

typedef struct{
	TCJT188_HEAD	head;
	BYTE	cs;
	BYTE	etx;
}TCJT188_DNULL,*PCJT188_DNULL;

typedef struct{
	union{
		WORD	all;
		BYTE	x[2];
	}di;
	BYTE	ser;
}TCJT188_DIX,*PCJT188_DIX;
typedef struct{
	TCJT188_HEAD	head;
	TCJT188_DIX		diX;
	BYTE	cs;
	BYTE	etx;
}TCJT188_RDX,*PCJT188_RDX;

// ST状态
typedef union{
	WORD	all;
	struct{
		BYTE	swClosed:2;	// 阀门状态: 0b:opened,1b:closed,11b:error
		BYTE	uLoss:1;	// 电池电压: 0:正常,1:欠压
		BYTE	:5;
		BYTE	:8;
	}bits;
}TCJT188_ST,*PCJT188_ST;
typedef struct{
	TCJT188_HEAD	head;
	BYTE			seq;
	TCJT188_ST		st;
	BYTE	cs;
	BYTE	etx;
}TCJT188_ERR,*PCJT188_ERR;


#pragma pack(pop)


/*
* brief : package cjt188 message
* param : nLead[in]:lead char
*         proto[in]:proto
*         addr[in]:address
*         func[in]:func code
*         di[in]:data-id
*         buf[in]:message buffer
*         size[in]:buffer size
* result: -1:param error
*         >0:message length
*/
int CJT188_PackMsg( int nLead,BYTE type,BYTE* addr,BYTE func,BYTE* di,BYTE* buf,int size );

/*
* brief : package cjt188 message
* param : proto[in]:proto
*         addr[in]:address
*         func[in]:func code
*         di[in]:data-id
*         buf[in]:message buffer
*         msgLn[in]:message length
* result: -1:param error
*         >0:message length
*/
int CJT188_GetDatSeg( BYTE type,BYTE* addr,BYTE func,BYTE* di,BYTE* buf,int msgLn );





#endif


