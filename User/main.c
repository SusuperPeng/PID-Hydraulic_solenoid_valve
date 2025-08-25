#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "tim2.h" //Encoder����������ʱ��
#include "tim3.h" //10ms�����жϲ����������ʱ��
#include "PWM.h"
#include "stdio.h"      //������Ҫ��ͷ�ļ�
#include "stdarg.h"		//������Ҫ��ͷ�ļ� 
#include "string.h"     //������Ҫ��ͷ�ļ�
#include "stdlib.h" 

int main()
{
	int i=0;
	
	delay_init();  //��ʱ��ʼ��
	Usart1_Init(115200);
	LED_Init();    //LED��ʼ��
	Encoder_TIM2_Init();
	Timer3_Init_Encoder_100ms();
	PWM_Init();
	LED2_ON;
	TIM_Cmd(TIM3, DISABLE); 
	
	while(1)
	{	
		//��ʱ����������
		if(i==0)
		{
			for(i=0;i<=250;i=i+10)
			{
				TIM1_PWM_SetCompare1(i);
				LED4_ON;
				delay_ms(100);
				LED4_OFF;
				delay_ms(100);
				}
		i=1;   //�˳���ʱ����ģʽ
		LED2_OFF;
		TIM_Cmd(TIM3, ENABLE); 
		LED4_OFF;	
			}
	
		
		if(USART1_RX_FLAG == 1)
		{		
			u1_printf("%s",USART1_RXBUFF);
			
			//�ҵ��ַ����е�һ���س����з����滻Ϊ�ַ���������'\0'��ʵ���ַ����ض�ȥ������
			USART1_RXBUFF[strcspn(USART1_RXBUFF, "\r\n")] = '\0';

			if (sscanf(USART1_RXBUFF, "%f", &target) != 1)
			{
				u1_printf("����\r\n");		// ����ʧ�ܣ���ӡ������ʾ
				}
					
			USART1_RX_FLAG=0;   //������ձ�־
			USART1_RX_LENG=0;	// ���ý��ճ���
			memset(USART1_RXBUFF,0,256);	// ��ջ�����
			
			}

		u1_printf("%f,      %d,      %f\r\n",target,encoder_speed,pid.output);
		}	
	
	}

	

