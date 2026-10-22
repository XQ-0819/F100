#ifndef _DIN_H_
#define _DIN_H_
#include "stm32f4xx.h"

#define DIN_ON	(1)
#define DIN_OFF	(0)

void DIN_Init(void);
void DIN_Proc(void);
int Get_DIN_State(u8 sid);
int Get_Pluse_Value(u8 sid);
void Clear_Pluse_Value(u8 sid);

#endif

