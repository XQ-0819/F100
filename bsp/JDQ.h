#ifndef _JDQ_H_
#define _JDQ_H_
#include "stm32f4xx.h"


void JDQ_Init(void);
void JDQ_Control(u8 id , u8 status);
u8 get_jdq_ctrl_state(void);
void JDQ_Control_Hard(void);

#endif

