#include "tim2.h"



/**************************************************************************
函数功能：把TIM2初始化为编码器接口模式
入口参数：无
返回  值：无
**************************************************************************/
void Encoder_TIM2_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;     
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;  // A相 B相
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   // 输入浮空
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); // 初始化结构体为默认值
    TIM_TimeBaseStructure.TIM_Prescaler = 0;     // 无预分频
    TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD;  // 自动重装载最大
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  //默认不分频即可
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 
    TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12,TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);// 设置为编码器模式，模式3（TI1和TI2）,4倍频计数
	
	//输入捕获滤波器（抗抖）	
	TIM_ICStructInit(&TIM_ICInitStructure); // 重置结构体为默认值
	TIM_ICInitStructure.TIM_ICFilter = 10;  // 输入信号滤波器，值越大越抗干扰，但响应变慢
	TIM_ICInit(TIM2, &TIM_ICInitStructure);

	//如果你要在 CNT 溢出/反转时做处理（比如测圈数），就要开启这个中断。
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);      // 清除更新中断标志
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); // 开启更新中断（如需要超限处理时使用）

    // 清空计数器
    TIM_SetCounter(TIM2, 0);

    // 启动定时器
    TIM_Cmd(TIM2, ENABLE);	
}
/**************************************************************************
函数功能：单位时间读取编码器计数
入口参数：定时器
返回  值：脉冲数
**************************************************************************/
int Read_Encoder()
{
	int Encoder_TIM;    
	Encoder_TIM= (short)TIM2 -> CNT; // 读取当前计数值
	TIM2 -> CNT=0;	// 清零用于下次增量

	return Encoder_TIM;
}
/**************************************************************************
函数功能：TIM2中断服务函数
入口参数：无
返回  值：无
	如果你将定时器用于 编码器接口模式，可以在此中断中实现 圈数累计/方向识别：
**************************************************************************/
void TIM2_IRQHandler(void)
{ 		    		  			    
	if(TIM2->SR & 0x0001) // 判断是否是更新（溢出）中断
	{    				   				     	    	
		// TODO: 你可以在这里处理溢出事件（比如累计圈数）
	}				   

	TIM2->SR &= ~(1 << 0); // 清除更新中断标志位（必须）
}


