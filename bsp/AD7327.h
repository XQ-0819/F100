  
/******************************************************************************
  Copyright (c), 2000-2020, Froxy Company
  
  File: AD7321.h
  Description: 
  Function List: //List main functions
  Revision:
         1. Author: yang   yangchongxin@126.com
            Date:  2017-8-29 21:53:51
            Changes: First create file
 ******************************************************************************/


#ifndef _AD7321_H_
#define _AD7321_H_


#ifdef __cplusplus
 extern "C" {
#endif

/****************************************************************************** 
 *                                 INCLUDE                                      * 
 ******************************************************************************/
#include "stm32f4xx.h"
 


void AD7327_Init(void);
void AD7327_Proc(void);
void AD7327_WIN_Proc(void);
int Get_AD_Value(u8 sid);

/****************************************************************************** 
 *                                 END                                        * 
 ******************************************************************************/


#ifdef __cplusplus
}
#endif

#endif /* _AD7321_H_ */



