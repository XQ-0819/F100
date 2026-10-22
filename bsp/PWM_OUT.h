
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _PWM_H_
#define _PWM_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"


void PWM_Init(void);
void PWM_Out(u8 channel ,u8 degree );

#ifdef __cplusplus
}
#endif

#endif /* _PWM_H_ */


