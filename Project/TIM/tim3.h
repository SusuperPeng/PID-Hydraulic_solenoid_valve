#ifndef __TIM3_H
#define __TIM3_H	 
#include "stm32f10x.h"

typedef struct {
    float Kp;
    float Ki;
    float Kd;

    float target;      // 目标速度
    float measured;    // 当前速度
    float error;
    float last_error;

    float integral;
    float output;
} PID_TypeDef;

void PID_Calc(PID_TypeDef *pid, float measured,float target);
void Timer3_Init_Encoder_100ms(void);

extern int16_t encoder_cnt;
extern int16_t encoder_speed;
extern PID_TypeDef pid;
extern float target;
#endif


