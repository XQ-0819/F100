/*
* author: lss
* date  : 
* email : commsky@163.com

* brief : 
* 1. rtu mode:implement modbus protocol on serial bus
* |<------------------------- ADU -------------------->|
*   <address>,<function code,data>,<crc lo,crc hi>
*      1..247 |<-------PDU------>|
* 2. ascii mode:
* |<---------------------------------------------------->|
*  start   address  function    data     lrc     end
*    :      2chars   2chars   0..2*252  2chars   cr,lf
* 
* remark:
* 1. big-endian
* 2. address 248..255 reversed
* 3. crc/lrc from addr to crc/lrc
* 4. when <=19200 bps: 3.5*Tbyte for frame,1.5*Tbyte for byte frame,
*    when  >19200 bps: t3.5=1.750ms,t1.5=750us
*    ascii mode Tbyte: 1S
* 5. user function code: 65-72,100-110,1-127(128-255 for exception responses)
* 6. max ADU message length: 256 bytes
*    max PDU message length: 256-server address(1 byte)-crc(2 bytes)=253 bytes
* 7. address space:
*        block      type    property
*    1) 离散量输入  bit    read only
*    2) 线圈        bit    read/write
*    3) 输入寄存器  16bit  read only
*    4) 保持寄存器  16bit  read/write 
* address space/block: 65535
* 
* 
* implement modbus protocol on ethernet
* |<---------------------------- ADU ---------------------------------->|
*   <事务处理标识>,<协议标识符>, <长度>,<单元标识符>,<function code,data>
*      2 bytes       2 bytes    2 bytes   1 bytes    |<------PDU------->|
* max ADU message: 256 bytes
* max PDU message: 256-MBAP(7 bytes)=249 bytes
*/


#ifndef MODBUS_H
#define MODBUS_H

#include "types.h"

#pragma pack(push)
#pragma pack(1)

// ascii字符模式传输
#define ASCII_STX	(0x3A)	// ascii模式起始字符':'
#define ASCII_ETX	(0x0D0A)// ascii模式结束字符'\r\n'


#define ADDR_CAST	(0)		// 广播地址

#define COM_RD_SCOIL		(0x01)	// 读线圈,bit unit,最多0x7D0
#define COM_RD_SIN			(0x02)	// 读取离散量输入,bit unit,最多0x7D0
#define COM_RD_RHOLD		(0x03)	// 读保持寄存器,2*bytes,HI-LO,最多0x7D
#define COM_RD_RIN			(0x04)	// 读输入寄存器,2*bytes,HI-LO,最多0x7D
#define COM_WR_COIL			(0x05)	// 写单个线圈
#define COM_WR_REG			(0x06)	// 写单个寄存器
#define COM_DIAGNOSE		(0x08)	// 串行链路诊断
#define COM_WR_COIL_x		(0x0F)	// 写多个线圈,最多0x7B0
#define COM_WR_REG_x		(0x10)	// 写多个寄存器,最多0x7B
#define COM_RD_FILE_REC		(0x14)	// 读文件记录
#define COM_WR_FILE_REC		(0x15)	// 写文件记录
#define COM_WR_MASK			(0x16)	// 屏蔽写寄存器
#define COM_RW_REG_x		(0x17)	// 读写多个寄存器
#define COM_RD_DEV_ID		(0x2B)	// 读设备识别码

#define ERR_FUNC	(0x01)	// 功能码错误/不支持
#define ERR_ADDR	(0x02)	// 非法地址、非法(地址+数量/长度)
#define ERR_DATA	(0x03)	// 非法数据
#define ERR_FAULT	(0x04)	// 产生不可重新获得的故障
#define ERR_ACK		(0x05)	// 与编程命令一起使用,由于处理时间较长,返回确认命令,防止对端超时
#define ERR_BUSY	(0x06)	// 服务忙
#define ERR_MEM		(0x08)	// 存储存在奇偶错误
#define ERR_GATEWAY_PATH	(0x0A)	// 网关路径错误
#define ERR_GATEWAY_TIMEOUT	(0x0B)	// 网关失去响应

// ----------------------------- read message -------------------------------------
#define ERR_RD_COIL_FUNC_MODBUS	(0x01)	// 功能码不支持
#define ERR_RD_COIL_ADDR_MODBUS	(0x02)	// addr<>ok,addr+num<>ok
#define ERR_RD_COIL_NUM_MODBUS	(0x03)	// 0x0001<=num<=0x07D0
#define ERR_RD_COIL_DATA_MODBUS	(0x04)	// read error
typedef struct{
	BYTE	func;	// 功能码
	union{			// 地址|开始地址,0x0000..0xFFFF,based on 0,HI LO
		WORD	all;
		BYTE	x[2];
	}addr;
	union{			// 线圈数量,1..2000(0x7D0),HI LO
		WORD	all;
		BYTE	x[2];
	}num;
}TPDU_RD,*PPDU_RD;

typedef struct{
	BYTE	func;	// 功能码
	BYTE	bytes;	// 字节数
	BYTE	data;	// 状态数据,多个字节[BIT=1:ON,0:OFF]
}TPDU_RD_A,*PPDU_RD_A;

typedef struct{
	BYTE	addr;
	TPDU_RD	pdu;
	WORD	wCRC;
}TSL_PDU_RD,*PSL_PDU_RD;
typedef struct{
	BYTE		addr;
	TPDU_RD_A	pdu;
}TSL_PDUA_HEAD,*PSL_PDUA_HEAD;

// ------------------------------ write message ------------------------------------
#define WR_SCOIL_ON		(0xFF00)	// 线圈ON
#define WR_SCOIL_OFF	(0x0000)	// 线圈OFF

typedef struct{
	BYTE	func;	// 功能码
	union{			// 地址|开始地址,0x0000..0xFFFF,based on 0,HI LO
		WORD	all;
		BYTE	x[2];
	}addr;
	union{			// 输出值,HI LO
		WORD	all;
		BYTE	x[2];
	}value;
}TPDU_WD,*PPDU_WD;

typedef struct{
	BYTE	func;	// 功能码
	union{			// 地址|开始地址,0x0000..0xFFFF,based on 0,HI LO
		WORD	all;
		BYTE	x[2];
	}addr;
	union{			// 输出值,HI LO
		WORD	all;
		BYTE	x[2];
	}value;
}TPDU_WD_A,*PPDU_WD_A;

typedef struct{
	BYTE	addr;
	TPDU_WD	pdu;
	WORD	wCRC;
}TSL_PDU_WD,*PSL_PDU_WD;
typedef struct{
	BYTE		addr;
	TPDU_WD_A	pdu;
}TSL_PDUA_WHEAD,*PSL_PDUA_WHEAD;

// ------------------------------ wirte multi-data message ------------------------------------
typedef struct{
	BYTE	func;	// 功能码
	union{			// 地址|开始地址,0x0000..0xFFFF,based on 0,HI LO
		WORD	all;
		BYTE	x[2];
	}addr;
	union{			// 输出数量,HI LO[0x0001..0x07B0]
		WORD	all;
		BYTE	x[2];
	}num;
	BYTE	nBytes;	// 字节数
	BYTE	data;
}TPDU_WDX,*PPDU_WDX;

typedef struct{
	BYTE	func;	// 功能码
	union{			// 地址|开始地址,0x0000..0xFFFF,based on 0,HI LO
		WORD	all;
		BYTE	x[2];
	}addr;
	union{			// 输出数量,HI LO[0x0001..0x07B0]
		WORD	all;
		BYTE	x[2];
	}num;
}TPDU_WDX_A,*PPDU_WDX_A;

// --------------------------- read file record message ---------------------------------------
// 异常码02: 参考类型OK,文件号OK,起始地址OK,起始地址+寄存器数量OK
typedef struct{
	BYTE	type;	// 子请求参考类型,0x06
	union{			// 文件号
		WORD	all;// 0x0000..0xFFFF
		BYTE	x[2];
	}fileID;
	union{			// 记录号
		WORD	all;// 0x0000..0x270F
		BYTE	x[2];
	}recID;
	union{			// 记录长度,N(寄存器数量)
		WORD	all;
		BYTE	x[2];
	}recLn;
}TREQ_UNIT,*PREQ_UNIT;
typedef struct{
	BYTE	func;	// 功能码
	BYTE	nByte;	// 字节数,0x07..0xF5
	TREQ_UNIT	reqs;	// 子请求
}TFILE_REC_RD,*PFILE_REC_RD;


typedef struct{
	BYTE	ln;		// 文件响应长度(0x07..0xF5)
	BYTE	type;	// 参考类型,0x06
	union{			// 记录数据,N*2
		WORD	all;
		BYTE	x[2];
	}data;
}TREC_UNIT,*PREC_UNIT;
typedef struct{
	BYTE	func;	// 响应功能码
	BYTE	nByte;	// 响应数据长度
	TREC_UNIT	rec;// 响应数据
}TFILE_REC_RD_A,*PFILE_REC_RD_A;
typedef TFILE_REC_RD_A	 TFILE_REC_WR,*PFILE_REC_WR;
typedef TFILE_REC_WR	 TFILE_REC_WR_A,*PFILE_REC_WR_A;


// ------------------------- wirte mask message -----------------------------------------
// result=(now-data && and_mask) or (or_mask && and_mask)
// if or_mask=0,  result=now-data && and_mask
// if and_mask=0, result=or_mask
typedef struct{
	BYTE	func;
	union{			// 参考地址,0x0000..0xFFFF
		WORD	all;
		BYTE	x[2];
	}addr;
	union{			// and-mask reg,0x0000..0xFFFF
		WORD	all;
		BYTE	x[2];
	}andMask;
	union{			// or-mask reg,0x0000..0xFFFF
		WORD	all;
		BYTE	x[2];
	}orMask;
}TMASK_WR,TMASK_WR_A,*PMASK_WR,*PMASK_WR_A;

// ------------------------- read/write multi-reg message -----------------------------------------
typedef struct{
	BYTE	func;	// 功能码
	union{			// 读起始地址,0x0000..0xFFFF
		WORD	all;
		BYTE	x[2];
	}raddr;
	union{			// 读的数量,0x0001..0x0076
		WORD	all;
		BYTE	x[2];
	}rnum;
	union{			// 写的起始地址,0x0000..0xFFFF
		WORD	all;
		BYTE	x[2];
	}waddr;
	union{			// 写的数量,0x0001..0x0076
		WORD	all;
		BYTE	x[2];
	}wnum;
	BYTE	wBytes;	// 2*N
	union{
		WORD	all;
		BYTE	x[2];
	}data;
}TRWX_REG,*PRWX_REG;

typedef struct{
	BYTE	func;
	BYTE	nBytes;	// 读响应的字节数,2*N
	union{			// 寄存器值
		WORD	all;
		BYTE	x[2];
	}data;
}TRWX_REG_A,*PRWX_REG_A;

// ----------------------------- read device id message -------------------------------------
#define RD_DEVICE_ID_COM	(0x01)	// 请求获得基本设备识别码[流访问]
#define RD_DEVICE_ID_NORM	(0x02)	// 请求获得正常设备识别码[流访问]
#define RD_DEVICE_ID_EXT	(0x03)	// 请求获得扩展设备识别码[流访问]
#define RD_DEVICE_ID_USR	(0x04)	// 请求获得特定识别码对象[专用访问]
//基本,强制的
#define OBJECT_ID_FACTORY_NAME	(0x00)	// 厂商名称,ascii字符串
#define OBJECT_ID_PRODUCT_ID	(0x01)	// 产品代码,ascii字符串
#define OBJECT_ID_VERSION		(0x02)	// 主要修订版本,ascii字符串
//正常,可选的
#define OBJECT_ID_VENDOR_URL	(0x03)	// vendor url,ascii字符串
#define OBJECT_ID_PRODUCT_NAME	(0x04)	// 产品名称,ascii字符串
#define OBJECT_ID_MODE_NAME		(0x05)	// 模式名称,ascii字符串
#define OBJECT_ID_USRAPP_NAME	(0x06)	// 用户应用名称,ascii字符串
//保留
//#define 0x07..0x7F
//扩展的,可选择地定义专用对象,产品有关
//#define 0x80..0xFF

typedef struct{
	BYTE	func;
	BYTE	meiType;	// MEI 类型,0x0E
	BYTE	readDevID;	// ReadDevId码,01/02/03/04
	BYTE	objID;		// 对象id,0x00..0xFF
}TREAD_OBJECT_ID,*PREAD_OBJECT_ID;


#define CONFORMITY_LEVEL_BASIC		(0x01)	// basic identification(stream acess only)
#define CONFORMITY_LEVEL_REGULAR	(0x02)	// regular identification(stream acess only)
#define CONFORMITY_LEVEL_EXT		(0x03)	// extended identification(stream acess only)
#define CONFORMITY_LEVEL_BASIC_X	(0x81)	// basic identification(stream acess and individual access)
#define CONFORMITY_LEVEL_REGULAR_X	(0x82)	// regular identification(stream acess and individual access)
#define CONFORMITY_LEVEL_EXT_X		(0x83)	// extended identification(stream acess and individual access)
typedef struct{
	BYTE	id;	// Object ID
	BYTE	ln;	// Object length
	BYTE	value;	// Object value
}TOBJECT_ID,*POBJECT_ID;
typedef struct{
	BYTE	func;
	BYTE	meiType;	// MEI 类型,0x0E
	BYTE	readDevID;	// ReadDevId码,01/02/03/04
	BYTE	conformity;	// 一致性等级
	BYTE	more;		// more follows
						// case readDevId of 01 02 03
						//   	00:no more object are available,
						//      FF:other?identification?Object?are?available?and?further?Modbus?transactions?are?required
						// case readDevId 0f 04
						//      must?be?set?to?00
	BYTE	nextID;		// next object id
						// case more 0xFF:identification of the next object to be asked for
						// case more 0x00:must be set to 00
	BYTE	objNum;		// number of objects
	TOBJECT_ID	obj;
}TREAD_OBJECT_ID_A,*PREAD_OBJECT_ID_A;

// ----------------------------- serial link diagnose message -------------------------------------
#define COUNT_CLEAR_ALL		(0x0A)	// 清除所有计数计数器
#define COUNT_BUS			(0x0B)	// 总线报文计数
#define COUNT_BUS_ERR		(0x0C)	// 总线通信错误计数
#define COUNT_NODE_ERR		(0x0D)	// 子节点异常错误计数
#define COUNT_NODE			(0x0E)	// 子节点报文计数
#define COUNT_NODE_DUMMY	(0x0F)	// 子节点无响应计数
#define COUNT_NODE_NAK		(0x10)	// 子节点NAK计数
#define COUNT_NODE_BUSY		(0x11)	// 子节点忙计数
#define COUNT_BUS_OVER		(0x12)	// 总线字符溢出计数



// ----------------------------- interface -------------------------------------
// modbus TCP/IP ADU
// |<----------ADU------>|
// MBAP报文头,功能码, 数据
//           |<----PDU-->|

typedef struct{
	union{	// 事务处理标识符 Hi Lo
		WORD	wID;
		BYTE	x[2];
	}taction;
	union{	// 协议标识符
		WORD	wID;	// 0值代表modbus协议
		BYTE	x[2];
	}prot;
	union{	// 长度,下一个域的字节长度
		WORD	wAll;
		BYTE	x[2];
	}len;
	BYTE	unitID;	// 单元标识符,无用时置0xFF
}TMBAP,*PMBAP;

#define ERR_TCPIP_FUNC	(0x01)	// 非法的功能码: 服务器不了解功能码
#define ERR_TCPIP_ADDR	(0x02)	// 非法的数据地址: 与请求有关
#define ERR_TCPIP_DATA	(0x03)	// 非法的数据值: 与请求有关
#define ERR_TCPIP_SRVR	(0x04)	// 服务器故障: 在执行过程中,服务器故障
#define ERR_TCPIP_ACK	(0x05)	// 确认: 服务器接受服务调用,但是需要相对长的时间完成服务.因此,服务器仅返回一个服务调用接收的确认
#define ERR_TCPIP_BUSY	(0x06)	// 服务器繁忙: 服务器不能接受 MODBUS 请求 PDU,客户应用由责任决定是否和何时重发请求
#define ERR_TCPIP_PATH	(0x0A)	// 网关故障: 网关路经是无效的
#define ERR_TCPIP_DEST	(0x0B)	// 网关故障: 目标设备没有响应,网关生成这个异常信息



#pragma pack(pop)

/*
* brief : get register number from data-bytes
* param : pApIt[in]:ap pointer
* result: -1:param error
*        >=0:register number
*/
//int MODBUS_GetRegNum( PAP_PARM pApIt );

/*
* brief   : package modbus message
* param   : addr[in]:dev addr
*           func[in]:control
*           reg[in]:reg
*           regn[in]:reg number
*           buf[in]:buffer
*           size[in]:buffer size
* result  : -1:param error
*           >0:message length
*/
int MODBUS_PackMsg( BYTE addr,BYTE func,BYTE* reg,BYTE regn,BYTE* buf,int size );
int MODBUS_WriteMsg( BYTE addr,BYTE func,BYTE* reg,BYTE* data,BYTE* buf,int size );

/*
* brief   : get modbus data segment
* param   : addr[in]:device address
*           func[in]:fun code
*           buf[in/out]:message buffer
*           msgLn[in]:message length
* result  : <0:error
*           =0:ctrl error
*           >0:data length
*/
int MODBUS_GetDatSeg( BYTE addr,BYTE func,BYTE* buf,int msgLn );
int MODBUS_GetWrAck( BYTE addr,BYTE func,BYTE* reg,BYTE* data,BYTE* buf,int msgLn);


WORD GetCRC16( BYTE *msg, int ln, WORD crc );


#endif

