#ifndef DLT645_H
#define DLT645_H

#include "types.h"


#pragma pack(push)
#pragma pack(1)

#define DLT645_ADDR_SIZE	(6)
// ±¨ÎÄÍ·
typedef struct{
	BYTE	stx1;
	BYTE	addr[DLT645_ADDR_SIZE];
	BYTE	stx2;
	union{
		BYTE	all;
		struct{
			BYTE	fun:5;	// function code
			BYTE	cond:1;	// have continued message
			BYTE	err:1;	// error bit
			BYTE	dir:1;	// dir bit
		}bits;
	}ctrl;
	BYTE	len;
}TDLT645_H,*PDLT645_H;

typedef struct{
	BYTE	x[2];
}TDI_97,*PDI_97;

typedef struct{
	BYTE	x[4];
}TDI_07,*PDI_07;

typedef struct{
	TDLT645_H	head;
	TDI_07		di;
	BYTE		sum;
	BYTE		etx;
}TDLT645_07_RD,*PDLT645_07_RD;

typedef struct{
	TDLT645_H	head;
	BYTE		err;
	BYTE		sum;
	BYTE		etx;
}TDLT645_ERR,*PDLT645_ERR;

#pragma pack(pop)



/*
* brief : package dlt645 message
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
int DLT645_PackMsg( int nLead,BYTE proto,BYTE* addr,BYTE func,BYTE* di,BYTE* buf,int size );

/*
* brief : get dlt645 data segment
* param : proto[in]:device protocol
*         addr[in]:device address
*         func[in]:fun code
*         di[in]:data item
*         buf[in/out]:message buffer
*         msgLn[in]:message length
* result: -1:param error
*        >=0:data field bytes
*/
int DLT645_GetDatSeg( BYTE proto,BYTE* addr,BYTE func,BYTE* di,BYTE* buf,int msgLn );



#endif

