#include "usart.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "stm32f4xx.h"
#include "ucos_ii.h"



typedef struct {
    WORD	wIn;
    WORD	wOut;
    BYTE	full;
    BYTE	buf[BUF_SIZE_256];
} TDBuf_256, *PDBuf_256;

typedef struct {
    WORD	wIn;
    WORD	wOut;
    BYTE	full;
    BYTE	buf[BUF_SIZE_2K];
} TDBuf_2K, *PDBuf_2K;

typedef struct {
    TDBuf_256	rx;
    TDBuf_256	tx;
} TUART_BUF256, *PUART_BUF256;
typedef struct {
    TDBuf_2K	rx;
    TDBuf_2K	tx;
} TUART_BUF2K, *PUART_BUF2K;

#define INDEX_UART1	(0)
#define INDEX_UART6	(1)
#define INDEX_UART3	(2)
TUART_BUF2K		uartBuf2K[NUM_UART_COMM];


//#define INDEX_UART3	(0)	// rs485-1
#define INDEX_UART4	(1)	// rs485-2
#define INDEX_UART5	(2)	// rs485-3
//#define INDEX_UART6	(3)	// rs485-4
#define INDEX_UART7	(4)	// rs485-5
#define INDEX_UART8	(5)	// rs485-6

TUART_BUF256	uart485[NUM_UART_485];


#define INDEX_UART2	(0)
TUART_BUF256   uartHMI[NUM_UART_HMI];

/*
* function: PutDataToDBuf
* brief   : put data to dbuf
* param   : pDBuf[in]:dbuf
*           wSize[in]:dbuf size
*           ucDat[in]:data
* result  : -1:param error
*           >0:put ok
*/
int PutDataToDBuf( PDBuf_256 pDBuf, unsigned short wSize, unsigned char ucDat )
{
    if ( pDBuf == NULL || wSize < 1 ) {
        return -1;
    }

    if ( pDBuf->full > 0 ) {
        pDBuf->wOut = (pDBuf->wOut + 1) % wSize;
    }

    pDBuf->buf[pDBuf->wIn] = ucDat;
    pDBuf->wIn = (pDBuf->wIn + 1) % wSize;

    if ( pDBuf->wIn == pDBuf->wOut ) {
        pDBuf->full = 1;
    }

    return 1;
}

/*
* function: GetDataFromDBuf
* brief   : get data from dbuf
* param   : pDBuf[in]:dbuf
*           wSize[in]:dbuf size
*           pDat[in/out]:data buffer
* result  : -1:param error
*            0:no data
*           >0:get ok
*/
int GetDataFromDBuf( PDBuf_256 pDBuf, unsigned short wSize, unsigned char* pDat )
{
    if ( pDBuf == NULL || wSize < 1 ||
            pDat == NULL ) {
        return -1;
    }

    if ( pDBuf->wOut == pDBuf->wIn &&
            pDBuf->full == 0 ) {
        return 0;
    }

    *pDat = pDBuf->buf[pDBuf->wOut];
    pDBuf->wOut = (pDBuf->wOut + 1) % wSize;
    pDBuf->full = 0;

    return 1;
}

/*
* function: ClearDBuf
* brief   : clear dbuf
* param   : pDBuf[in]:dbuf
*           wSize[in]:dbuf size
* result  : -1:param error
*            0:ok
*/
int ClearDBuf( PDBuf_256 pDBuf, unsigned short wSize )
{
    if ( pDBuf == NULL || wSize < 1 ) {
        return -1;
    }

    pDBuf->wIn = 0;
    pDBuf->wOut = 0;
    pDBuf->full = 0;
    pDBuf->buf[0] = 0;

    return 0;
}

/*
* function: GetUartData
* brief   : get all data from uart
* param   : uartID[in]:uart id
*           buf[in]:buffer for storing data
*           size[in]:buffer size
* result  : <0:param error
*          >=0:data number
*/
int GetUartData( int uartID, unsigned char *buf, int size )
{
    int		n, ires;
    unsigned char	bData;
    unsigned short	wSize;
    PDBuf_256	pDbuf;

    if ( buf == NULL || size < 1 ) {
        return -1;
    }

    switch ( uartID ) {
    case UART1_CON:
        pDbuf = (PDBuf_256)&uartBuf2K[INDEX_UART1].rx;
        wSize = BUF_SIZE_2K;
        break;

    case UART3_SENSOR:
        pDbuf = (PDBuf_256)&uartBuf2K[INDEX_UART3].rx;
        wSize = BUF_SIZE_2K;
        break;

    case UART2_HMI:
        pDbuf = (PDBuf_256)&uartHMI[INDEX_UART2].rx;
        wSize = BUF_SIZE_256;
        break;

    case UART4_485:
        pDbuf = (PDBuf_256)&uart485[INDEX_UART4].rx;
        wSize = BUF_SIZE_256;
        break;

    case UART5_485:
        pDbuf = (PDBuf_256)&uart485[INDEX_UART5].rx;
        wSize = BUF_SIZE_256;
        break;

    case UART6_COMM:
        pDbuf = (PDBuf_256)&uartBuf2K[INDEX_UART6].rx;
        wSize = BUF_SIZE_2K;
        break;

    case UART7_485:
        pDbuf = (PDBuf_256)&uart485[INDEX_UART7].rx;
        wSize = BUF_SIZE_256;
        break;

    case UART8_485:
        pDbuf = (PDBuf_256)&uart485[INDEX_UART8].rx;
        wSize = BUF_SIZE_256;
        break;

    default:
        pDbuf = NULL;
        wSize = 0;
        break;
    }

    if ( pDbuf == NULL ) {
        return -2;
    }

    for ( n = 0; n < size; n++ ) {
        ires = GetDataFromDBuf( pDbuf, wSize, &bData );

        if ( ires < 1 ) {
            break;
        }

        buf[n] = bData;
    }

    return n;
}

/*
* function: ClearUartRx
* brief   : clear uart rx buffer
* param   : uartID[in]:uart-id
* result  : -1:param error
*            0:ok
*/
int ClearUartRx( int uartID )
{
    int		ires;

    ClearUartIdle(uartID);

    ires = 0;

    switch ( uartID ) {
    case UART1_CON:
        ClearDBuf( (PDBuf_256)&uartBuf2K[INDEX_UART1].rx, BUF_SIZE_2K );
        break;

    case UART3_SENSOR:
        ClearDBuf( (PDBuf_256)&uartBuf2K[INDEX_UART3].rx, BUF_SIZE_2K );
        break;

    case UART2_HMI:
        ClearDBuf( (PDBuf_256)&uartHMI[INDEX_UART2].rx, BUF_SIZE_256 );
        break;

    case UART4_485:
        ClearDBuf( (PDBuf_256)&uart485[INDEX_UART4].rx, BUF_SIZE_256 );
        break;

    case UART5_485:
        ClearDBuf( (PDBuf_256)&uart485[INDEX_UART5].rx, BUF_SIZE_256 );
        break;

    case UART6_COMM:
        ClearDBuf( (PDBuf_256)&uartBuf2K[INDEX_UART6].rx, BUF_SIZE_2K );
        break;

    case UART7_485:
        ClearDBuf( (PDBuf_256)&uart485[INDEX_UART7].rx, BUF_SIZE_256 );
        break;

    case UART8_485:
        ClearDBuf( (PDBuf_256)&uart485[INDEX_UART8].rx, BUF_SIZE_256 );
        break;

    default:
        ires = -1;
        break;
    }

    if ( ires < 0 ) {
        return -1;
    }

    return 0;
}

/*
* function: SendUartData
* brief   : send data to uart
* param   : uartID[in]:uart id
*           buf[in]:buffer for storing data
*           iNum[in]:data number
* result  : <0:param error
*          >=0:data number
*/
int SendUartData( int uartID, unsigned char *buf, int iNum )
{
    int		i, ires;

    if ( buf == NULL || iNum < 1 ) {
        return -1;
    }

    ires = 0;

    switch ( uartID ) {
    case UART1_CON:
        for ( i = 0; i < iNum; i++ ) {
            PutDataToDBuf( (PDBuf_256)&uartBuf2K[INDEX_UART1].tx, BUF_SIZE_2K, buf[i] );
        }

        USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
        break;

    case UART3_SENSOR:
        for ( i = 0; i < iNum; i++ ) {
            PutDataToDBuf( (PDBuf_256)&uartBuf2K[INDEX_UART3].tx, BUF_SIZE_2K, buf[i] );
        }

        USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
        break;

    case UART2_HMI:
        for ( i = 0; i < iNum; i++ ) {
            PutDataToDBuf( (PDBuf_256)&uartHMI[INDEX_UART2].tx, BUF_SIZE_256, buf[i] );
        }

        USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
        break;

    case UART4_485:
        for ( i = 0; i < iNum; i++ ) {
            PutDataToDBuf( (PDBuf_256)&uart485[INDEX_UART4].tx, BUF_SIZE_256, buf[i] );
        }

        USART_ITConfig(UART4, USART_IT_TXE, ENABLE);
        break;

    case UART5_485:
        for ( i = 0; i < iNum; i++ ) {
            PutDataToDBuf( (PDBuf_256)&uart485[INDEX_UART5].tx, BUF_SIZE_256, buf[i] );
        }

        USART_ITConfig(UART5, USART_IT_TXE, ENABLE);
        break;

    case UART6_COMM:
        for ( i = 0; i < iNum; i++ ) {
            PutDataToDBuf( (PDBuf_256)&uartBuf2K[INDEX_UART6].tx, BUF_SIZE_2K, buf[i] );
        }

        USART_ITConfig(USART6, USART_IT_TXE, ENABLE);
        break;

    case UART7_485:
        for ( i = 0; i < iNum; i++ ) {
            PutDataToDBuf( (PDBuf_256)&uart485[INDEX_UART7].tx, BUF_SIZE_256, buf[i] );
        }

        USART_ITConfig(UART7, USART_IT_TXE, ENABLE);
        break;

    case UART8_485:
        for ( i = 0; i < iNum; i++ ) {
            PutDataToDBuf( (PDBuf_256)&uart485[INDEX_UART8].tx, BUF_SIZE_256, buf[i] );
        }

        USART_ITConfig(UART8, USART_IT_TXE, ENABLE);
        break;

    default:
        ires = -1;
        break;
    }

    if ( ires < 0 ) {
        return -2;
    }

    return 1;
}

/*
* function: SendUartStr
* brief   : send string via uart
* param   : uartID[in]:uart-id
*           str[in]:string
* result  : <0:send error
*            0:ok
*/
int SendUartStr( int uartID, char* str )
{
    int		ires;

    if ( str == NULL ) {
        return -1;
    }

    ires = SendUartData( uartID, (unsigned char*)str, strlen(str) );

    if ( ires < 0 ) {
        return -2;
    }

    return 0;
}

static char text[256];

/*
* function: SendUartStrX
* brief   : send string via uart
* param   : uartID[in]:uart-id
*           fmt[in]:string
*           ...
* result  : <0:send error
*            0:ok
*/
int SendUartStrX( int uartID, char* fmt, ... )
{
    int		ires;
    va_list	argPtr;

    memset( text, 0, sizeof(text) );
    va_start( argPtr, fmt );
    vsnprintf( text, sizeof(text) - 1, fmt, argPtr );
    va_end( argPtr );
    ires = SendUartData( uartID, (unsigned char*)text, strlen(text) );

    if ( ires < 0 ) {
        return -2;
    }

    return 0;
}


OS_EVENT*	semModemLink;
typedef struct {
    WORD		wTick;
    OS_EVENT*	sem;
} TUART_EVT, *PUART_EVT;
TUART_EVT	uartEvt[UART_NUM];

/*
* function: InitUartEvent
* brief   : init uart event
* param   : void
* result  : 0:ok
*/
int InitUartEvent( void )
{
    int		i;

    for ( i = 0; i < UART_NUM; i++ ) {
        uartEvt[i].wTick = 0;
        uartEvt[i].sem = OSSemCreate( 0 );
    }

    //semModemLink=OSSemCreate( 0 );

    return 0;
}


void USART_Config( void )
{
    GPIO_InitTypeDef	GPIO_InitStructure;
    USART_InitTypeDef	USART_InitStructure;
    NVIC_InitTypeDef	NVIC_InitStructure;

    NVIC_PriorityGroupConfig( NVIC_PriorityGroup_1 );

#if USART1_SWITCH
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE);
    /* Connect PXx to USARTx_Tx*/
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
    /* Connect PXx to USARTx_Rx*/
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
    /* Configure USART Tx as alternate function  */
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    /* Configure USART Rx as alternate function  */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init( USART1, &USART_InitStructure );

    USART_ClearFlag( USART1, USART_FLAG_TC );
    USART_ITConfig( USART1, USART_IT_RXNE, ENABLE );
    USART_ITConfig( USART1, USART_IT_TC, ENABLE );
    USART_ITConfig( USART1, USART_IT_ERR, ENABLE );//开启这个中断才能清除ORE

    USART_Cmd( USART1, ENABLE );
#endif

#if USART2_SWITCH
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    /* Connect PXx to USARTx_Tx*/
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);
    /* Connect PXx to USARTx_Rx*/
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);
    /* Configure USART Tx as alternate function  */
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    /* Configure USART Rx as alternate function  */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);

    USART_ClearFlag(USART2, USART_FLAG_TC);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    USART_ITConfig(USART2, USART_IT_TC, ENABLE);
    USART_ITConfig( USART2, USART_IT_ERR, ENABLE );//开启这个中断才能清除ORE

    USART_Cmd( USART2, ENABLE );
#endif

#if USART3_SWITCH

#if USE_GD == 1
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC , ENABLE);
    /* Connect PXx to USARTx_Tx*/
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_USART3);
    /* Connect PXx to USARTx_Rx*/
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART3);
    /* Configure USART Tx as alternate function  */
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    /* Configure USART Rx as alternate function  */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
#else
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB , ENABLE);
    /* Connect PXx to USARTx_Tx*/
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
    /* Connect PXx to USARTx_Rx*/
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);
    /* Configure USART Tx as alternate function  */
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    /* Configure USART Rx as alternate function  */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif

    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART3, &USART_InitStructure);

    USART_ClearFlag(USART3, USART_FLAG_TC);
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    USART_ITConfig(USART3, USART_IT_TC, ENABLE);
    USART_ITConfig( USART3, USART_IT_ERR, ENABLE );//开启这个中断才能清除ORE

    USART_Cmd(USART3, ENABLE);
#endif

#if UART4_SWITCH
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC , ENABLE);
    /* Connect PXx to USARTx_Tx*/
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_UART4);
    /* Connect PXx to USARTx_Rx*/
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_UART4);
    /* Configure USART Tx as alternate function  */
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    /* Configure USART Rx as alternate function  */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(UART4, &USART_InitStructure);

    USART_ClearFlag(UART4, USART_FLAG_TC);
    USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
    USART_ITConfig(UART4, USART_IT_TC, ENABLE);
    USART_ITConfig( UART4, USART_IT_ERR, ENABLE );//开启这个中断才能清除ORE

    USART_Cmd(UART4, ENABLE);
#endif

#if UART5_SWITCH
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD, ENABLE);
    /* Connect PXx to USARTx_Tx*/
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_UART5);
    /* Connect PXx to USARTx_Rx*/
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_UART5);
    /* Configure USART Tx as alternate function  */
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    /* Configure USART Rx as alternate function  */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(UART5, &USART_InitStructure);

    USART_ClearFlag(UART5, USART_FLAG_TC);
    USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
    USART_ITConfig(UART5, USART_IT_TC, ENABLE);
    USART_ITConfig( UART5, USART_IT_ERR, ENABLE );//开启这个中断才能清除ORE

    USART_Cmd(UART5, ENABLE);
#endif

#if USART6_SWITCH
#if USE_GD == 1
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
    /* Connect PXx to USARTx_Tx*/
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource9, GPIO_AF_USART6);
    /* Connect PXx to USARTx_Rx*/
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource14, GPIO_AF_USART6);
    /* Configure USART Tx as alternate function  */
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOG, &GPIO_InitStructure);

    /* Configure USART Rx as alternate function  */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_Init(GPIOG, &GPIO_InitStructure);
#else
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    /* Connect PXx to USARTx_Tx*/
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);
    /* Connect PXx to USARTx_Rx*/
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);
    /* Configure USART Tx as alternate function  */
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* Configure USART Rx as alternate function  */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
#endif

    NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART6, &USART_InitStructure);

    USART_ClearFlag(USART6, USART_FLAG_TC);
    USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);
    USART_ITConfig(USART6, USART_IT_TC, ENABLE);

    USART_ITConfig( USART6, USART_IT_ERR, ENABLE );//开启这个中断才能清除ORE

    USART_Cmd(USART6, ENABLE);
#endif

#if UART7_SWITCH
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART7, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
    /* Connect PXx to USARTx_Tx*/
    GPIO_PinAFConfig(GPIOF, GPIO_PinSource6, GPIO_AF_UART7);
    /* Connect PXx to USARTx_Rx*/
    GPIO_PinAFConfig(GPIOF, GPIO_PinSource7, GPIO_AF_UART7);
    /* Configure USART Tx as alternate function  */
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOF, &GPIO_InitStructure);
    /* Configure USART Rx as alternate function  */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_Init(GPIOF, &GPIO_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = UART7_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(UART7, &USART_InitStructure);

    USART_ClearFlag(UART7, USART_FLAG_TC);
    USART_ITConfig(UART7, USART_IT_RXNE, ENABLE);
    USART_ITConfig(UART7, USART_IT_TC, ENABLE);
    USART_ITConfig( UART7, USART_IT_ERR, ENABLE );//开启这个中断才能清除ORE

    USART_Cmd(UART7, ENABLE);
#endif

#if UART8_SWITCH
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART8, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
    /* Connect PXx to USARTx_Tx*/
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource1, GPIO_AF_UART8);
    /* Connect PXx to USARTx_Rx*/
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource0, GPIO_AF_UART8);
    /* Configure USART Tx as alternate function  */
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    /* Configure USART Rx as alternate function  */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = UART8_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(UART8, &USART_InitStructure);

    USART_ClearFlag(UART8, USART_FLAG_TC);
    USART_ITConfig(UART8, USART_IT_RXNE, ENABLE);
    USART_ITConfig(UART8, USART_IT_TC, ENABLE);
    USART_ITConfig( UART8, USART_IT_ERR, ENABLE );//开启这个中断才能清除ORE

    USART_Cmd(UART8, ENABLE);
#endif
}


const DWORD mapBaud[] = {1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200};
int UartInit( BYTE port, BYTE baud, BYTE datBits, BYTE parity, BYTE stopBits )
{
    int		nItem;
    USART_InitTypeDef USART_InitStructure;

    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    //
    nItem = sizeof(mapBaud) / sizeof(DWORD);
    USART_InitStructure.USART_BaudRate = (baud >= nItem) ? 9600 : mapBaud[baud];

    //
    switch ( datBits ) {
    case UDATA_7B://必须有校验
        USART_InitStructure.USART_WordLength = USART_WordLength_8b;
        break;

    case UDATA_8B:
        if ( PARITY_NONE == parity ) {
            USART_InitStructure.USART_WordLength = USART_WordLength_8b;
        } else {
            USART_InitStructure.USART_WordLength = USART_WordLength_9b;
        }

        break;

    default:
        USART_InitStructure.USART_WordLength = USART_WordLength_8b;
        break;
    }

    //
    switch ( parity ) {
    case PARITY_NONE:
        USART_InitStructure.USART_Parity = USART_Parity_No;
        break;

    case PARITY_ODD:
        USART_InitStructure.USART_Parity = USART_Parity_Odd;
        break;

    case PARITY_EVEN:
        USART_InitStructure.USART_Parity = USART_Parity_Even;
        break;

    default:
        USART_InitStructure.USART_Parity = USART_Parity_No;
        break;
    }

    //
    switch ( stopBits ) {
    case STOP_BIT1:
        USART_InitStructure.USART_StopBits = USART_StopBits_1;
        break;

    case STOP_BIT1X5:
        USART_InitStructure.USART_StopBits = USART_StopBits_1_5;
        break;

    case STOP_BIT2:
        USART_InitStructure.USART_StopBits = USART_StopBits_2;
        break;

    default:
        USART_InitStructure.USART_StopBits = USART_StopBits_1;
        break;
    }

    //
    switch ( port ) {
    case UART1_CON:
        USART_Init( USART1, &USART_InitStructure );
        USART_Cmd( USART1, ENABLE );
        break;

    case UART2_HMI:
        USART_Init( USART2, &USART_InitStructure );
        USART_Cmd( USART2, ENABLE );
        break;

    case UART3_SENSOR:
        USART_Init( USART3, &USART_InitStructure );
        USART_Cmd( USART3, ENABLE );
        break;

    case UART4_485:
        USART_Init( UART4, &USART_InitStructure );
        USART_Cmd( UART4, ENABLE );
        break;

    case UART5_485:
        USART_Init( UART5, &USART_InitStructure );
        USART_Cmd( UART5, ENABLE );
        break;

    case UART6_COMM:
        USART_Init( USART6, &USART_InitStructure );
        USART_Cmd( USART6, ENABLE );
        break;

    case UART7_485:
        USART_Init( UART7, &USART_InitStructure );
        USART_Cmd( UART7, ENABLE );
        break;

    case UART8_485:
        USART_Init( UART8, &USART_InitStructure );
        USART_Cmd( UART8, ENABLE );
        break;

    default:
        USART_Init( USART2, &USART_InitStructure );
        USART_Cmd( USART2, ENABLE );
        break;
    }

    return 0;
}

void USART1_IRQHandler( void )
{
    int		ires;
    unsigned char	bData;

#ifdef OS_TICKS_PER_SEC
    OSIntEnter();
#endif

    if (USART_GetFlagStatus(USART1, USART_FLAG_PE) != RESET) {
        USART_ReceiveData(USART1);
        USART_ClearFlag(USART1, USART_FLAG_PE);
    }

    if (USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET) {
        USART_ReceiveData(USART1);
        USART_ClearFlag(USART1, USART_FLAG_ORE);
    }

    if (USART_GetFlagStatus(USART1, USART_FLAG_FE) != RESET) {
        USART_ReceiveData(USART1);
        USART_ClearFlag(USART1, USART_FLAG_FE);//清除溢出中断
    }

    // 接收
    if ( USART_GetITStatus(USART1, USART_IT_RXNE) != RESET ) {
        // 进来必须先清除中断
        USART_ClearITPendingBit( USART1, USART_IT_RXNE );
        /* Read one byte from the receive data register */
        bData = USART_ReceiveData( USART1 );
        PutDataToDBuf( (PDBuf_256)&uartBuf2K[INDEX_UART1].rx, BUF_SIZE_2K, bData );
        uartEvt[UART1_CON].wTick = UART_TICK_XNOW;
    }

    // 发送
    if ( USART_GetITStatus(USART1, USART_IT_TXE) != RESET ) {
        ires = GetDataFromDBuf( (PDBuf_256)&uartBuf2K[INDEX_UART1].tx, BUF_SIZE_2K, &bData );

        if ( ires < 1 ) {
            USART_ITConfig( USART1, USART_IT_TXE, DISABLE );
        } else {
            USART_SendData( USART1, bData );
        }
    }

    if ( USART_GetITStatus(USART1, USART_IT_TC) != RESET ) {
        /* Clear the USART1 transmit interrupt */
        USART_ClearITPendingBit( USART1, USART_IT_TC );
    }

#ifdef OS_TICKS_PER_SEC
    OSIntExit();
#endif
}

void USART2_IRQHandler( void )
{
    int		ires;
    unsigned char	bData;

#ifdef OS_TICKS_PER_SEC
    OSIntEnter();
#endif

    if (USART_GetFlagStatus(USART2, USART_FLAG_PE) != RESET) {
        USART_ReceiveData(USART2);
        USART_ClearFlag(USART2, USART_FLAG_PE);
    }

    if (USART_GetFlagStatus(USART2, USART_FLAG_ORE) != RESET) {
        USART_ReceiveData(USART2);
        USART_ClearFlag(USART2, USART_FLAG_ORE);
    }

    if (USART_GetFlagStatus(USART2, USART_FLAG_FE) != RESET) {
        USART_ReceiveData(USART2);
        USART_ClearFlag(USART2, USART_FLAG_FE);//清除溢出中断
    }

    //接收
    if ( USART_GetITStatus(USART2, USART_IT_RXNE) != RESET ) {
        // 进来必须先清除中断
        USART_ClearITPendingBit( USART2, USART_IT_RXNE );

        /* Read one byte from the receive data register */
        bData = USART_ReceiveData( USART2 );
        PutDataToDBuf( (PDBuf_256)&uartHMI[INDEX_UART2].rx, BUF_SIZE_256, bData );

        uartEvt[UART2_HMI].wTick = UART_TICK_XNOW;
    }

    //发送
    if ( USART_GetITStatus(USART2, USART_IT_TXE) != RESET ) {
        ires = GetDataFromDBuf( (PDBuf_256)&uartHMI[INDEX_UART2].tx, BUF_SIZE_256, &bData );

        if ( ires < 1 ) {
            USART_ITConfig( USART2, USART_IT_TXE, DISABLE );
        } else {
            USART_SendData( USART2, bData );
        }
    }

    if ( USART_GetITStatus(USART2, USART_IT_TC) != RESET ) {
        /* Clear the USART2 transmit interrupt */
        USART_ClearITPendingBit( USART2, USART_IT_TC );
    }

#ifdef OS_TICKS_PER_SEC
    OSIntExit();
#endif
}

void USART3_IRQHandler( void )
{
    int		ires;
    unsigned char	bData;

#ifdef OS_TICKS_PER_SEC
    OSIntEnter();
#endif

    if (USART_GetFlagStatus(USART3, USART_FLAG_PE) != RESET) {
        USART_ReceiveData(USART3);
        USART_ClearFlag(USART3, USART_FLAG_PE);
    }

    if (USART_GetFlagStatus(USART3, USART_FLAG_ORE) != RESET) {
        USART_ReceiveData(USART3);
        USART_ClearFlag(USART3, USART_FLAG_ORE);
    }

    if (USART_GetFlagStatus(USART3, USART_FLAG_FE) != RESET) {
        USART_ReceiveData(USART3);
        USART_ClearFlag(USART3, USART_FLAG_FE);//清除溢出中断
    }

    //接收
    if ( USART_GetITStatus(USART3, USART_IT_RXNE) != RESET ) {
        //进来必须先清除中断
        USART_ClearITPendingBit(USART3, USART_IT_RXNE);

        /* Read one byte from the receive data register */
        bData = USART_ReceiveData( USART3 );
        PutDataToDBuf( (PDBuf_256)&uartBuf2K[INDEX_UART3].rx, BUF_SIZE_2K, bData );
        uartEvt[UART3_SENSOR].wTick = UART_TICK_XNOW;
    }

    //发送
    if ( USART_GetITStatus(USART3, USART_IT_TXE) != RESET ) {
        ires = GetDataFromDBuf( (PDBuf_256)&uartBuf2K[INDEX_UART3].tx, BUF_SIZE_2K, &bData );

        if ( ires < 1 ) {
            USART_ITConfig( USART3, USART_IT_TXE, DISABLE );
        } else {
            USART_SendData( USART3, bData );
        }
    }

    if ( USART_GetITStatus(USART3, USART_IT_TC) != RESET ) {
        /* Clear the USART3 transmit interrupt */
        USART_ClearITPendingBit(USART3, USART_IT_TC);
    }

#ifdef OS_TICKS_PER_SEC
    OSIntExit();
#endif
}

void UART4_IRQHandler( void )
{
    int		ires;
    unsigned char	bData;

#ifdef OS_TICKS_PER_SEC
    OSIntEnter();
#endif

    if (USART_GetFlagStatus(UART4, USART_FLAG_PE) != RESET) {
        USART_ReceiveData(UART4);
        USART_ClearFlag(UART4, USART_FLAG_PE);
    }

    if (USART_GetFlagStatus(UART4, USART_FLAG_ORE) != RESET) {
        USART_ReceiveData(UART4);
        USART_ClearFlag(UART4, USART_FLAG_ORE);
    }

    if (USART_GetFlagStatus(UART4, USART_FLAG_FE) != RESET) {
        USART_ReceiveData(UART4);
        USART_ClearFlag(UART4, USART_FLAG_FE);//清除溢出中断
    }

    //接收
    if ( USART_GetITStatus(UART4, USART_IT_RXNE) != RESET ) {
        //进来必须先清除中断
        USART_ClearITPendingBit(UART4, USART_IT_RXNE);

        /* Read one byte from the receive data register */
        bData = USART_ReceiveData( UART4 );
        PutDataToDBuf( (PDBuf_256)&uart485[INDEX_UART4].rx, BUF_SIZE_256, bData );
        uartEvt[UART4_485].wTick = UART_TICK_XNOW;
    }

    //发送
    if ( USART_GetITStatus(UART4, USART_IT_TXE) != RESET ) {
        ires = GetDataFromDBuf( (PDBuf_256)&uart485[INDEX_UART4].tx, BUF_SIZE_256, &bData );

        if ( ires < 1 ) {
            USART_ITConfig( UART4, USART_IT_TXE, DISABLE );
        } else {
            USART_SendData( UART4, bData );
        }
    }

    if ( USART_GetITStatus(UART4, USART_IT_TC) != RESET ) {
        /* Clear the UART4 transmit interrupt */
        USART_ClearITPendingBit(UART4, USART_IT_TC);
    }

#ifdef OS_TICKS_PER_SEC
    OSIntExit();
#endif
}

void UART5_IRQHandler( void )
{
    int		ires;
    unsigned char	bData;

#ifdef OS_TICKS_PER_SEC
    OSIntEnter();
#endif

    if (USART_GetFlagStatus(UART5, USART_FLAG_PE) != RESET) {
        USART_ReceiveData(UART5);
        USART_ClearFlag(UART5, USART_FLAG_PE);
    }

    if (USART_GetFlagStatus(UART5, USART_FLAG_ORE) != RESET) {
        USART_ReceiveData(UART5);
        USART_ClearFlag(UART5, USART_FLAG_ORE);
    }

    if (USART_GetFlagStatus(UART5, USART_FLAG_FE) != RESET) {
        USART_ReceiveData(UART5);
        USART_ClearFlag(UART5, USART_FLAG_FE);//清除溢出中断
    }

    //接收
    if ( USART_GetITStatus(UART5, USART_IT_RXNE) != RESET ) {
        //进来必须先清除中断
        USART_ClearITPendingBit(UART5, USART_IT_RXNE);

        /* Read one byte from the receive data register */
        bData = USART_ReceiveData( UART5 );
        PutDataToDBuf( (PDBuf_256)&uart485[INDEX_UART5].rx, BUF_SIZE_256, bData );
        uartEvt[UART5_485].wTick = UART_TICK_XNOW;
    }

    //发送
    if ( USART_GetITStatus(UART5, USART_IT_TXE) != RESET ) {
        ires = GetDataFromDBuf( (PDBuf_256)&uart485[INDEX_UART5].tx, BUF_SIZE_256, &bData );

        if ( ires < 1 ) {
            USART_ITConfig( UART5, USART_IT_TXE, DISABLE );
        } else {
            USART_SendData( UART5, bData );
        }
    }

    if ( USART_GetITStatus(UART5, USART_IT_TC) != RESET ) {
        /* Clear the UART5 transmit interrupt */
        USART_ClearITPendingBit(UART5, USART_IT_TC);
    }

#ifdef OS_TICKS_PER_SEC
    OSIntExit();
#endif
}

void USART6_IRQHandler( void )
{
    int		ires;
    unsigned char	bData;

#ifdef OS_TICKS_PER_SEC
    OSIntEnter();
#endif

    if (USART_GetFlagStatus(USART6, USART_FLAG_PE) != RESET) {
        USART_ReceiveData(USART6);
        USART_ClearFlag(USART6, USART_FLAG_PE);
    }

    if (USART_GetFlagStatus(USART6, USART_FLAG_ORE) != RESET) {
        USART_ReceiveData(USART6);
        USART_ClearFlag(USART6, USART_FLAG_ORE);
    }

    if (USART_GetFlagStatus(USART6, USART_FLAG_FE) != RESET) {
        USART_ReceiveData(USART6);
        USART_ClearFlag(USART6, USART_FLAG_FE);//清除溢出中断
    }

    if ( USART_GetITStatus(USART6, USART_IT_RXNE) != RESET ) {
        USART_ClearITPendingBit( USART6, USART_IT_RXNE );

        /* Read one byte from the receive data register */
        bData = USART_ReceiveData( USART6 );
        PutDataToDBuf( (PDBuf_256)&uartBuf2K[INDEX_UART6].rx, BUF_SIZE_2K, bData );
        uartEvt[UART6_COMM].wTick = UART_TICK_XNOW;
    }

    if ( USART_GetITStatus(USART6, USART_IT_TXE) != RESET ) {
        ires = GetDataFromDBuf( (PDBuf_256)&uartBuf2K[INDEX_UART6].tx, BUF_SIZE_2K, &bData );

        if ( ires < 1 ) {
            USART_ITConfig( USART6, USART_IT_TXE, DISABLE );
        } else {
            USART_SendData( USART6, bData );
        }
    }

    if ( USART_GetITStatus(USART6, USART_IT_TC) != RESET ) {
        /* Clear the USART6 transmit interrupt */
        USART_ClearITPendingBit( USART6, USART_IT_TC );
    }

#ifdef OS_TICKS_PER_SEC
    OSIntExit();
#endif
}

void UART7_IRQHandler( void )
{
    int		ires;
    unsigned char	bData;

#ifdef OS_TICKS_PER_SEC
    OSIntEnter();
#endif

    if (USART_GetFlagStatus(UART7, USART_FLAG_PE) != RESET) {
        USART_ReceiveData(UART7);
        USART_ClearFlag(UART7, USART_FLAG_PE);
    }

    if (USART_GetFlagStatus(UART7, USART_FLAG_ORE) != RESET) {
        USART_ReceiveData(UART7);
        USART_ClearFlag(UART7, USART_FLAG_ORE);
    }

    if (USART_GetFlagStatus(UART7, USART_FLAG_FE) != RESET) {
        USART_ReceiveData(UART7);
        USART_ClearFlag(UART7, USART_FLAG_FE);//清除溢出中断
    }

    if ( USART_GetITStatus(UART7, USART_IT_RXNE) != RESET ) {
        USART_ClearITPendingBit(UART7, USART_IT_RXNE);

        /* Read one byte from the receive data register */
        bData = USART_ReceiveData( UART7 );
        PutDataToDBuf( (PDBuf_256)&uart485[INDEX_UART7].rx, BUF_SIZE_256, bData );
        uartEvt[UART7_485].wTick = UART_TICK_XNOW;
    }

    if ( USART_GetITStatus(UART7, USART_IT_TXE) != RESET ) {
        ires = GetDataFromDBuf( (PDBuf_256)&uart485[INDEX_UART7].tx, BUF_SIZE_256, &bData );

        if ( ires < 1 ) {
            USART_ITConfig( UART7, USART_IT_TXE, DISABLE );
        } else {
            USART_SendData( UART7, bData );
        }
    }

    if ( USART_GetITStatus(UART7, USART_IT_TC) != RESET ) {
        /* Clear the UART7 transmit interrupt */
        USART_ClearITPendingBit(UART7, USART_IT_TC);
    }

#ifdef OS_TICKS_PER_SEC
    OSIntExit();
#endif
}

void UART8_IRQHandler( void )
{
    int		ires;
    unsigned char	bData;

#ifdef OS_TICKS_PER_SEC
    OSIntEnter();
#endif

    if (USART_GetFlagStatus(UART8, USART_FLAG_PE) != RESET) {
        USART_ReceiveData(UART8);
        USART_ClearFlag(UART8, USART_FLAG_PE);
    }

    if (USART_GetFlagStatus(UART8, USART_FLAG_ORE) != RESET) {
        USART_ReceiveData(UART8);
        USART_ClearFlag(UART8, USART_FLAG_ORE);
    }

    if (USART_GetFlagStatus(UART8, USART_FLAG_FE) != RESET) {
        USART_ReceiveData(UART8);
        USART_ClearFlag(UART8, USART_FLAG_FE);//清除溢出中断
    }

    if ( USART_GetITStatus(UART8, USART_IT_RXNE) != RESET ) {
        USART_ClearITPendingBit(UART8, USART_IT_RXNE);

        /* Read one byte from the receive data register */
        bData = USART_ReceiveData( UART8 );
        PutDataToDBuf( (PDBuf_256)&uart485[INDEX_UART8].rx, BUF_SIZE_256, bData );
        uartEvt[UART8_485].wTick = UART_TICK_XNOW;
    }

    if ( USART_GetITStatus(UART8, USART_IT_TXE) != RESET ) {
        ires = GetDataFromDBuf( (PDBuf_256)&uart485[INDEX_UART8].tx, BUF_SIZE_256, &bData );

        if ( ires < 1 ) {
            USART_ITConfig( UART8, USART_IT_TXE, DISABLE );
        } else {
            USART_SendData( UART8, bData );
        }
    }

    if ( USART_GetITStatus(UART8, USART_IT_TC) != RESET ) {
        /* Clear the UART8 transmit interrupt */
        USART_ClearITPendingBit(UART8, USART_IT_TC);
    }

#ifdef OS_TICKS_PER_SEC
    OSIntExit();
#endif
}


extern u8 modemLinkFlag;
void Time_Proc( void )
{
    int		i;

    for ( i = 0; i < UART_NUM; i++ ) {
        if ( uartEvt[i].wTick < 1 ) {
            continue;
        }

        uartEvt[i].wTick--;

        if ( uartEvt[i].wTick == UART_TICK_OVER ) {
            switch ( i ) {
            case UART2_HMI:
                OSSemPost( uartEvt[i].sem );
                break;

            case UART3_SENSOR:
                OSSemPost( uartEvt[i].sem );
                break;

            case UART6_COMM:
//				if( modemLinkFlag == 0 ){
//					OSSemPost( semModemLink );
//				}else if( modemLinkFlag == 1 ){
//					OSSemPost( uartEvt[i].sem );
//				}
                OSSemPost( uartEvt[i].sem );
                break;

            default:
                OSSemPost( uartEvt[i].sem );
                break;
            }
        }
    }
}

/*
* function: WaitUartIdle
* brief   : wait uart bus idle
* param   : uartID[in]:uart-id
*           wToutTick[in]:timeout tick
* result  : -1:param error
*        other:event error
*/
int WaitUartIdle( int uartID, WORD wToutTick )
{
    INT8U	evtErr;

    if ( uartID < 0 || uartID >= UART_NUM ) {
        return -1;
    }

    OSSemPend( uartEvt[uartID].sem, wToutTick, &evtErr );

    return evtErr;
}

int ClearUartIdle( int uartID )
{
    INT8U	evtErr;

    if ( uartID < 0 || uartID >= UART_NUM ) {
        return -1;
    }

    OSSemSet( uartEvt[uartID].sem, 0, &evtErr );

    return evtErr;
}

int SetGprsLinkSem( int cnt )
{
    INT8U	osErr;

    OSSemSet( semModemLink, cnt, &osErr );

    return osErr;
}

/*
* function: WaitGprsLink
* brief   : wait gprs-link
* param   : wToutTick[in]:timeout tick
* result  : -1:param error
*        other:event error
*/
int WaitGprsLink( WORD wToutTick )
{
    INT8U	evtErr;

    OSSemPend( semModemLink, wToutTick, &evtErr );

    return evtErr;
}


