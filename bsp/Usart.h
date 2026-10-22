#ifndef UART_H_
#define UART_H_

#include "types.h"



#define UART_NUM		(8)		// ????????
#define NUM_UART_485	(6)		// 485???? 

#define NUM_UART_HMI	(1)		// ??
#define NUM_UART_COMM	(3)		// 4G -COMM
#define NUM_UART_SENSOR	(1)		// 4G -COMM


#define UART1_CON		(0)		// ???????е??? 
#define UART2_HMI		(1)		// ??????

#define UART3_SENSOR	(2)		// GPRS??
#define UART4_485		(3)		// ??????
#define UART5_485		(4)		// ??????

#define UART6_COMM		(5)		// ??????

#define UART7_485		(6)		// ??????
#define UART8_485		(7)		// ??????

#define UDATA_7B	(0)
#define UDATA_8B	(1)
#define PARITY_NONE	(0)
#define PARITY_ODD	(1)
#define PARITY_EVEN	(2)
#define STOP_BIT1	(0)
#define STOP_BIT1X5	(1)
#define STOP_BIT2	(2)

#define UART_TICK_XNOW	(5) // 
#define UART_TICK_OVER	(2) // 

//??????????,????????????????1
#if USE_GD == 1
#define USART1_SWITCH  0  //
#else	
#define USART1_SWITCH  1  //
#endif
#define USART2_SWITCH  1  //
#define USART3_SWITCH  1  //
#define UART4_SWITCH   0
#define UART5_SWITCH   0
#define USART6_SWITCH  1  
#define UART7_SWITCH   0
#define UART8_SWITCH   0


void USART_Config(void);
void Time_Proc( void );

int UartInit( BYTE port,BYTE baud,BYTE datBits,BYTE parity,BYTE stopBits );

/*
* function: InitUartEvent
* brief   : init uart event
* param   : void
* result  : 0:ok
*/
int InitUartEvent( void );

/*
* function: ClearUartRx
* brief   : clear uart rx buffer
* param   : uartID[in]:uart-id
* result  : -1:param error
*            0:ok
*/
int ClearUartRx( int uartID );

/*
* function: GetUartData
* brief   : get all data from uart
* param   : uartID[in]:uart id
*           buf[in]:buffer for storing data
*           size[in]:buffer size
* result  : <0:param error
*          >=0:data number
*/
int GetUartData( int uartID,unsigned char *buf,int size );

/*
* function: SendUartData
* brief   : send data to uart
* param   : uartID[in]:uart id
*           buf[in]:buffer for storing data
*           iNum[in]:data number
* result  : <0:param error
*          >=0:data number
*/
int SendUartData( int uartID,unsigned char *buf,int iNum );

/*
* function: SendUartStr
* brief   : send string via uart
* param   : uartID[in]:uart-id
*           str[in]:string
* result  : <0:send error
*            0:ok
*/
int SendUartStr( int uartID,char* str );

/*
* function: SendUartStrX
* brief   : send string via uart
* param   : uartID[in]:uart-id
*           fmt[in]:string
*           ...
* result  : <0:send error
*            0:ok
*/
int SendUartStrX( int uartID,char* fmt,... );

/*
* function: WaitUartIdle
* brief   : wait uart bus idle
* param   : uartID[in]:uart-id
*           wToutTick[in]:timeout tick
* result  : -1:param error
*        other:event error
*/
int WaitUartIdle( int uartID,WORD wToutTick );
int ClearUartIdle( int uartID );

int SetGprsLinkSem( int cnt );

/*
* function: WaitGprsLink
* brief   : wait gprs-link
* param   : wToutTick[in]:timeout tick
* result  : -1:param error
*        other:event error
*/
int WaitGprsLink( WORD wToutTick );



#endif
