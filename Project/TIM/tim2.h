#ifndef __TIM2_H
#define __TIM2_H	

#include "stm32f10x.h"

#define ENCODER_TIM_PERIOD (u16)(65535)   //103的定时器是16位 2的16次方最大是65536

void Encoder_TIM2_Init(void);
int Read_Encoder();


#endif



