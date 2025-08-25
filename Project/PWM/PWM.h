#ifndef __TIM1_PWM_H
#define __TIM1_PWM_H

#include "stm32f10x.h"  // STM32F10x 标准外设库头文件

void PWM_Init();
void TIM1_PWM_SetCompare1(uint16_t Compare);

#endif 



