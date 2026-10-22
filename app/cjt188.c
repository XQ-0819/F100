/*
** brief : impletment cjt188 protocol
** author: commsky
** date  : 2019-4-1
*/

#include <stdio.h>
#include <math.h>
#include <string.h>
#include "cjt188.h"

typedef struct{
	BYTE	unit;
	int		power;
}TUNIT_POWER,*PUNIT_POWER;
const TUNIT_POWER unitPower[]={
	{CJT188_UNIT_J, -3},
	{CJT188_UNIT_KJ,0},
	{CJT188_UNIT_MJ,3},
	{CJT188_UNIT_GJ,6},
	{CJT188_UNIT_GJX100,8}
};

/*
* brief : sum of data
* param : data[in]:data buffer
*         n[in]:data number
* result: -1:param error
*        >=0:sum(only LSB byte)
*/
extern int SumOfData( BYTE* data,int n );

double GetUnitPower( BYTE unit )
{
	int		i,epwr,nItem;
	double	dres;
	
	nItem=sizeof(unitPower)/sizeof(TUNIT_POWER);
	for( i=0; i<nItem; i++ ){
		if( unitPower[i].unit==unit ){
			break;
		}
	}
	epwr=(i<nItem)? unitPower[i].power : 0;
	dres=pow(10,epwr);
	
	return dres;
}

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
int CJT188_PackMsg( int nLead,BYTE type,BYTE* addr,BYTE func,BYTE* di,BYTE* buf,int size )
{
	int		i,it,itX;
	BYTE	bSum;
	PCJT188_RDX	pCjtRdX;
	
	static BYTE	seq=0;
	
	if( addr==NULL || di==NULL ||
    	buf==NULL || nLead<0 ||
    	size<(nLead+sizeof(TCJT188_RDX)) ){
    	return -1;
    }
    
	memset( buf,CJT_LEAD,nLead );
	it=nLead;
	pCjtRdX=(PCJT188_RDX)&buf[it];
	pCjtRdX->head.stx=CJT188_STX;
	pCjtRdX->head.type=type;
	for( i=0; i<CJT_ADDR_SIZE; i++ ){
		pCjtRdX->head.addr[i]=addr[i];
    }
    pCjtRdX->head.ctrl.all=func;
	pCjtRdX->head.len=sizeof(TCJT188_DIX);
	pCjtRdX->diX.di.x[0]=di[0];
	pCjtRdX->diX.di.x[1]=di[1];
	pCjtRdX->diX.ser=seq++;
	itX=sizeof(TCJT188_HEAD)+sizeof(TCJT188_DIX);
	bSum=SumOfData( &buf[it],itX );
	it += itX;
	buf[it++]=bSum;
	buf[it++]=CJT188_ETX;
	
	return it;
}

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
int CJT188_GetDatSeg( BYTE type,BYTE* addr,BYTE func,BYTE* di,BYTE* buf,int msgLn )
{
	int		it;
	int		nByte;
	BYTE	bSum;
	BYTE*	pData;
	PCJT188_RDX	pRdX;
	
	if( addr==NULL || di==NULL ||
		buf==NULL || msgLn<sizeof(TCJT188_ERR) ){
		return -1;
	}
	
	// stx check
	for( it=0; it<msgLn; it++ ){
		if( buf[it]==CJT188_STX ){
			break;
		}
	}
	if( (msgLn-it)<sizeof(TCJT188_ERR) ){
		return -2;
	}
	
	pRdX=(PCJT188_RDX)&buf[it];
	// check meter type and address
	if( pRdX->head.type!=type ||
		memcmp(pRdX->head.addr,addr,CJT_ADDR_SIZE)!=0 ){
		return -3;
	}
	// check cs
	nByte=sizeof(TCJT188_HEAD)+pRdX->head.len;
	bSum=SumOfData( &buf[it],nByte );
	if( bSum!=buf[it+nByte] ){
		return -4;
	}
	// check ctrl
	if( pRdX->head.ctrl.bits.dir==0 ||
		pRdX->head.ctrl.bits.err!=0 ){
		return 0;
	}
	// check di
	if( memcmp(pRdX->diX.di.x,di,CJT_DI_SIZE)!=0 ){
		return -5;
	}
	// etx check
	if( buf[it+nByte+1]!=CJT188_ETX ){
		return -6;
	}
	
	pData=&buf[it+sizeof(TCJT188_HEAD)+sizeof(TCJT188_DIX)];
	nByte=pRdX->head.len-sizeof(TCJT188_DIX);
	memcpy( buf,pData,nByte );

	return nByte;
}




