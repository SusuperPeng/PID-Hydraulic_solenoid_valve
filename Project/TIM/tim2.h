#ifndef __TIM2_H
#define __TIM2_H	

#include "stm32f10x.h"

#define ENCODER_TIM_PERIOD (u16)(65535)   //103�Ķ�ʱ����16λ 2��16�η������65536

void Encoder_TIM2_Init(void);
int Read_Encoder();


#endif



