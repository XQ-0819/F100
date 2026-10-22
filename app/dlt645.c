/*
** brief : impletment dlt64597/07 protocol
** author: commsky
** date  : 2019-4-1
*/

#include <stdio.h>
#include <string.h>

#include "dlt645.h"
#include "energy.h"

#define WAKE_CHAR		(0xFE)
#define DLT645_STX		(0x68)
#define DLT645_ETX		(0x16)
#define DLT645_KEY		(0x33)

#define DLT645_BERR_CTRL	(0xC0)
#define DLT645_BDIR			(0x80)
#define DLT645_BERR_ACK		(0x40)
#define DLT645_BCOND		(0x20)

#define FUN_READ_DATA	(0x11)	// 读功能码
#define FUN_READ_COND	(0x12)	// 读后续数据功能码
#define FUN_READ_ADDR	(0x13)	// 读通讯地址
#define FUN_WRITE_DATA	(0x14)	// 写数据
#define FUN_WRITE_ADDR	(0x15)	// 写通讯地址
#define FUN_SNAPSHOT	(0x16)	// 冻结命令
#define FUN_CHANGE_BAUD	(0x17)	// 更改通讯速率
#define FUN_CHANGE_PWD	(0x18)	// 修改密码
#define FUN_DEMAND_CLR	(0x19)	// 需量清零
#define FUN_ENERGY_CLR	(0x1A)	// 电量清零
#define FUN_EVENT_CLR	(0x1B)	// 事件清零

/*
* brief : sum of data
* param : data[in]:data buffer
*         n[in]:data number
* result: -1:param error
*        >=0:sum(only LSB byte)
*/
int SumOfData( BYTE* data,int n )
{
	int		i;
	BYTE	sum;
	
	if( data==NULL || n<1 ){
		return -1;
	}
	
	for( sum=0,i=0; i<n; i++ ){
		sum += data[i];
	}
	
	return sum;
}

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
int DLT645_PackMsg( int nLead,BYTE proto,BYTE* addr,BYTE func,BYTE* di,BYTE* buf,int size )
{
	int		i,it;
	PDLT645_H	pMsgH;
	
    if( addr==NULL || di==NULL ||
    	buf==NULL || nLead<0 ||
    	size<(nLead+sizeof(TDLT645_07_RD)) ){
    	return -1;
    }
	
    memset( buf,WAKE_CHAR,nLead );
    it=nLead;
    pMsgH=(PDLT645_H)&buf[it];
    pMsgH->stx1=DLT645_STX;
    for( i=0; i<DLT645_ADDR_SIZE; i++ ){
    	pMsgH->addr[i]=addr[i];
    }
    pMsgH->stx2=DLT645_STX;
    pMsgH->ctrl.all=func;
    if( proto==TYPE_DL645_97){
    	pMsgH->len=sizeof(TDI_97);
    }else if( proto==TYPE_DL645_07 ){
    	pMsgH->len=sizeof(TDI_07);
    }else{
    	return -2;
    }
    
    it += sizeof(TDLT645_H);
    for( i=0; i<pMsgH->len; i++ ){
    	buf[it++]=di[i]+DLT645_KEY;
    }
    buf[it]=SumOfData( &buf[nLead],it-nLead );
    buf[++it]=DLT645_ETX;
    it++;
    
    return it;
}

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
int DLT645_GetDatSeg( BYTE proto,BYTE* addr,BYTE func,BYTE* di,BYTE* buf,int msgLn )
{
	int		i,n,it,itSum;
	BYTE	sum;
	BYTE*	pData;
	PDLT645_H	pMsgH;
	
	if( addr==NULL || di==NULL ||
		buf==NULL || msgLn<sizeof(TDLT645_ERR) ){
		return -1;
	}
	
	for( it=0; it<msgLn; it++ ){
		if( buf[it]==DLT645_STX ){
			break;
		}
	}
	if( (msgLn-it)<sizeof(TDLT645_ERR) ){
		return -2;
	}
	pMsgH=(PDLT645_H)&buf[it];
	// check address
	if( memcmp(pMsgH->addr,addr,DLT645_ADDR_SIZE)!=0 ){
		return -3;
	}
	// check stx2
	if( pMsgH->stx2!=DLT645_STX ){
		return -4;
	}
	// check sum
	itSum=sizeof(TDLT645_H)+pMsgH->len;
	sum=SumOfData( &buf[it],itSum );
	if( sum!=buf[it+itSum] ){
		return -5;
	}
	// check dir and control code
	if( pMsgH->ctrl.bits.dir==0 ||
		pMsgH->ctrl.bits.err!=0 ||
		pMsgH->ctrl.bits.cond!=0 ||
		pMsgH->ctrl.bits.fun!=func ){
		return -6;
	}
	// check di
	pData=&buf[it+sizeof(TDLT645_H)];
	for( i=0; i<pMsgH->len; i++ ){
		pData[i] -= DLT645_KEY;
	}
	n=(proto!=TYPE_DL645_97)? sizeof(TDI_07) : sizeof(TDI_97);
	if( memcmp(pData,di,n)!=0 ){
		return -7;
	}
	// data
	pData += n;
	n=pMsgH->len-n;
	for( i=0; i<n; i++ ){
		buf[i]=pData[i];
	}
	
	return n;
}


