#include "tim2.h"   //编码器定时器
#include "tim3.h"	//测速定时器
#include "PWM.h"
#include "usart.h"
void Timer3_Init_Encoder_100ms(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); // 开启 TIM3 时钟

    // 设置定时器时基（假设系统时钟72MHz）
    TIM_TimeBaseStructure.TIM_Period = 1000 - 1;    // 自动重装载值：10,000次为1s,1000为100ms，100为10ms
    TIM_TimeBaseStructure.TIM_Prescaler = 7200 - 1;  // 预分频：72MHz / 7200 = 10kHz（周期0.1ms）
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); // 使能更新中断
    TIM_Cmd(TIM3, ENABLE);                     // 启动 TIM3

    // 中断优先级配置
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}



PID_TypeDef pid = {
    .Kp = 0.04f,
    .Ki = 0.0f,
    .Kd = 0.0f
};

void PID_Calc(PID_TypeDef *pid, float measured,float target) {
    
	pid->measured = measured;   //当前速度值赋给结构体里
	pid->target = target;   //当前速度值赋给结构体里
    pid->error = pid->target - pid->measured;  //计算当前速度与目标速度的误差

    pid->integral += pid->error;	//速度误差的积分值

    float derivative = pid->error - pid->last_error;//速度斜率，做微分用

    pid->output = pid->Kp * pid->error
                + pid->Ki * pid->integral
                + pid->Kd * derivative;

    pid->last_error = pid->error;

    // 输出限幅（例如 PWM 最大值）
    if (pid->output > 499) pid->output = 499;
    if (pid->output < 100)   pid->output = 100;
}


int16_t encoder_cnt = 0;
int16_t encoder_speed = 0;
float target;
void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update); // 清除中断标志

        encoder_cnt = Read_Encoder(); // 读取编码器速度（单位100ms内脉冲数）
		encoder_speed = encoder_cnt;

		PID_Calc(&pid,encoder_speed,target);
		
		TIM1_PWM_SetCompare1((uint16_t)(pid.output)); 	// 控制电机
    }
}

