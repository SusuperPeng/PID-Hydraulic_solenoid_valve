#include "tim2.h"   //��������ʱ��
#include "tim3.h"	//���ٶ�ʱ��
#include "PWM.h"
#include "usart.h"
void Timer3_Init_Encoder_100ms(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); // ���� TIM3 ʱ��

    // ���ö�ʱ��ʱ��������ϵͳʱ��72MHz��
    TIM_TimeBaseStructure.TIM_Period = 1000 - 1;    // �Զ���װ��ֵ��10,000��Ϊ1s,1000Ϊ100ms��100Ϊ10ms
    TIM_TimeBaseStructure.TIM_Prescaler = 7200 - 1;  // Ԥ��Ƶ��72MHz / 7200 = 10kHz������0.1ms��
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); // ʹ�ܸ����ж�
    TIM_Cmd(TIM3, ENABLE);                     // ���� TIM3

    // �ж����ȼ�����
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
    
	pid->measured = measured;   //��ǰ�ٶ�ֵ�����ṹ����
	pid->target = target;   //��ǰ�ٶ�ֵ�����ṹ����
    pid->error = pid->target - pid->measured;  //���㵱ǰ�ٶ���Ŀ���ٶȵ����

    pid->integral += pid->error;	//�ٶ����Ļ���ֵ

    float derivative = pid->error - pid->last_error;//�ٶ�б�ʣ���΢����

    pid->output = pid->Kp * pid->error
                + pid->Ki * pid->integral
                + pid->Kd * derivative;

    pid->last_error = pid->error;

    // ����޷������� PWM ���ֵ��
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
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update); // ����жϱ�־

        encoder_cnt = Read_Encoder(); // ��ȡ�������ٶȣ���λ100ms����������
		encoder_speed = encoder_cnt;

		PID_Calc(&pid,encoder_speed,target);
		
		TIM1_PWM_SetCompare1((uint16_t)(pid.output)); 	// ���Ƶ��
    }
}

