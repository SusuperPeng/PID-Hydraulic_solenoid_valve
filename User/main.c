#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "tim2.h" //Encoder正交编码器时钟
#include "tim3.h" //10ms周期中断测编码器次数时钟
#include "PWM.h"
#include "stdio.h"      //包含需要的头文件
#include "stdarg.h"		//包含需要的头文件 
#include "string.h"     //包含需要的头文件
#include "stdlib.h" 

int main()
{
	int i=0;
	
	delay_init();  //延时初始化
	Usart1_Init(115200);
	LED_Init();    //LED初始化
	Encoder_TIM2_Init();
	Timer3_Init_Encoder_100ms();
	PWM_Init();
	LED2_ON;
	TIM_Cmd(TIM3, DISABLE); 
	
	while(1)
	{	
		//延时启动，防震动
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
		i=1;   //退出延时启动模式
		LED2_OFF;
		TIM_Cmd(TIM3, ENABLE); 
		LED4_OFF;	
			}
	
		
		if(USART1_RX_FLAG == 1)
		{		
			u1_printf("%s",USART1_RXBUFF);
			
			//找到字符串中第一个回车或换行符，替换为字符串结束符'\0'，实现字符串截断去除换行
			USART1_RXBUFF[strcspn(USART1_RXBUFF, "\r\n")] = '\0';

			if (sscanf(USART1_RXBUFF, "%f", &target) != 1)
			{
				u1_printf("错误\r\n");		// 解析失败，打印错误提示
				}
					
			USART1_RX_FLAG=0;   //清除接收标志
			USART1_RX_LENG=0;	// 重置接收长度
			memset(USART1_RXBUFF,0,256);	// 清空缓冲区
			
			}

		u1_printf("%f,      %d,      %f\r\n",target,encoder_speed,pid.output);
		}	
	
	}

	

