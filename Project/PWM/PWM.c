#include "PWM.h"

/**********************
PWM���ú�����
��ע��TIM1ΪPWM�����
**********************/
void PWM_Init()
{
	/******************************����ʱ��*****************************/	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);	//������ʱ��1ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//����GPIOʱ��
	/*****************************����ṹ�����******************************/	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	/****************************�������ŵ�ģʽ*******************************/	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//�����������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/****************************����ʱ����Ԫ*******************************/	
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//1/2/4��Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;		//���ϼ���
	TIM_TimeBaseInitStructure.TIM_Period = 1000-1;		//ARR���Զ���װ�ؼĴ�����ֵ Freq = CK_PSC/(PSC+1)/(ARR+1)
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72-1;	//PSC��Ԥ��Ƶϵ����72MHz����ô���Ը�PSC=0��36MHz����ʱ����ô�͸�PSC=1
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;//TIM_RepetitionCounter���ظ�������Ҳ�����ظ����ٴβŷ�����������жϡ��ٷ�ע�����������ֻ��TIM1��TIM8�й�
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);////�߼���ʱ��ר��--MOE�����ʹ��
	/******************************����Ƚϵ�Ԫ����*****************************/		
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//�Ƚ����ģʽѡ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//�Ƚ�������ԣ���ѡΪ�ߵ�ƽ��Ч��͵�ƽ��Ч
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0; //�Ƚ���������ȣ�ʵ���趨�ȽϼĴ��� CCR ��ֵ�����������ȡ������÷�ΧΪ 0 �� 65535��PWMռ�ձȣ�Duty = CCR / (ARR + 1) = CCR / 100
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);//�ṹ���ʼ��    
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);//�ṹ���ʼ��
	
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);//ʹ�ܻ���ʧ�� TIMx �� CCRx �ϵ�Ԥװ�ؼĴ���
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);//ʹ�ܻ���ʧ�� TIMx �� CCRx �ϵ�Ԥװ�ؼĴ���
	TIM_Cmd(TIM1, ENABLE);//������ʱ��

}


void TIM1_PWM_SetCompare1(uint16_t Compare)//д��TIM1ͨ��1��CCR(ռ�ձ�)
{
	TIM_SetCompare1(TIM1,Compare);
}
