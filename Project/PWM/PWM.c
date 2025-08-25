#include "PWM.h"

/**********************
PWM配置函数：
备注：TIM1为PWM波输出
**********************/
void PWM_Init()
{
	/******************************开启时钟*****************************/	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);	//开启定时器1时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//开启GPIO时钟
	/*****************************定义结构体变量******************************/	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	/****************************配置引脚的模式*******************************/	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/****************************配置时基单元*******************************/	
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//1/2/4分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;		//向上计数
	TIM_TimeBaseInitStructure.TIM_Period = 1000-1;		//ARR：自动重装载寄存器的值 Freq = CK_PSC/(PSC+1)/(ARR+1)
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72-1;	//PSC：预分频系数，72MHz，那么可以给PSC=0，36MHz计数时，那么就给PSC=1
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;//TIM_RepetitionCounter：重复计数，也就是重复多少次才发生计数溢出中断。官方注释里面这个数只与TIM1和TIM8有关
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);////高级定时器专属--MOE主输出使能
	/******************************输出比较单元配置*****************************/		
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//比较输出模式选择
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//比较输出极性，可选为高电平有效或低电平有效
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; //比较输出脉冲宽度，实际设定比较寄存器 CCR 的值，决定脉冲宽度。可设置范围为 0 至 65535。PWM占空比：Duty = CCR / (ARR + 1) = CCR / 100
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);//结构体初始化    
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);//结构体初始化
	
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);//使能或者失能 TIMx 在 CCRx 上的预装载寄存器
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);//使能或者失能 TIMx 在 CCRx 上的预装载寄存器
	TIM_Cmd(TIM1, ENABLE);//开启定时器

}


void TIM1_PWM_SetCompare1(uint16_t Compare)//写入TIM1通道1的CCR(占空比)
{
	TIM_SetCompare1(TIM1,Compare);
}
