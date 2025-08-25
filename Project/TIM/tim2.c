#include "tim2.h"



/**************************************************************************
�������ܣ���TIM2��ʼ��Ϊ�������ӿ�ģʽ
��ڲ�������
����  ֵ����
**************************************************************************/
void Encoder_TIM2_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;     
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;  // A�� B��
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   // ���븡��
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); // ��ʼ���ṹ��ΪĬ��ֵ
    TIM_TimeBaseStructure.TIM_Prescaler = 0;     // ��Ԥ��Ƶ
    TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD;  // �Զ���װ�����
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  //Ĭ�ϲ���Ƶ����
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 
    TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12,TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);// ����Ϊ������ģʽ��ģʽ3��TI1��TI2��,4��Ƶ����
	
	//���벶���˲�����������	
	TIM_ICStructInit(&TIM_ICInitStructure); // ���ýṹ��ΪĬ��ֵ
	TIM_ICInitStructure.TIM_ICFilter = 10;  // �����ź��˲�����ֵԽ��Խ�����ţ�����Ӧ����
	TIM_ICInit(TIM2, &TIM_ICInitStructure);

	//�����Ҫ�� CNT ���/��תʱ�����������Ȧ��������Ҫ��������жϡ�
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);      // ��������жϱ�־
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); // ���������жϣ�����Ҫ���޴���ʱʹ�ã�

    // ��ռ�����
    TIM_SetCounter(TIM2, 0);

    // ������ʱ��
    TIM_Cmd(TIM2, ENABLE);	
}
/**************************************************************************
�������ܣ���λʱ���ȡ����������
��ڲ�������ʱ��
����  ֵ��������
**************************************************************************/
int Read_Encoder()
{
	int Encoder_TIM;    
	Encoder_TIM= (short)TIM2 -> CNT; // ��ȡ��ǰ����ֵ
	TIM2 -> CNT=0;	// ���������´�����

	return Encoder_TIM;
}
/**************************************************************************
�������ܣ�TIM2�жϷ�����
��ڲ�������
����  ֵ����
	����㽫��ʱ������ �������ӿ�ģʽ�������ڴ��ж���ʵ�� Ȧ���ۼ�/����ʶ��
**************************************************************************/
void TIM2_IRQHandler(void)
{ 		    		  			    
	if(TIM2->SR & 0x0001) // �ж��Ƿ��Ǹ��£�������ж�
	{    				   				     	    	
		// TODO: ����������ﴦ������¼��������ۼ�Ȧ����
	}				   

	TIM2->SR &= ~(1 << 0); // ��������жϱ�־λ�����룩
}


