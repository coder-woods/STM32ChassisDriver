#include "pwm.h"
 
 /****************************************************************************
* 名    称: void PWM_INIT(u32 arr,u32 psc)
* 功    能：PWM初始化
* 入口参数：arr：自动重装值 psc：时钟预分频数
* 返回参数：
* 说    明：
* 作    者：嵌入式小白Woods
****************************************************************************/	
void PWM_INIT(u32 arr,u32 psc)
{		 					 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(PWM_TIM_CLK,ENABLE);  	                //TIM时钟使能    
	RCC_AHB1PeriphClockCmd(PWM_PORT_TIMER, ENABLE); 	            //使能PORT时钟	

	GPIO_PinAFConfig(PWM_PORT,PWM_PIN_SOURCE1,PWM_PIN_AF);          //GPIO复用为定时器

	GPIO_InitStructure.GPIO_Pin = PWM_PIN1;                         //GPIO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                    //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	            //速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                  //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                    //上拉
	GPIO_Init(PWM_PORT,&GPIO_InitStructure);                   
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;                        //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;       //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;                           //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(PWM_TIM,&TIM_TimeBaseStructure);               //初始化定时器3
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;               //初始化TIM3 Channel 1 PWM模式 选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;        //输出极性:TIM输出比较极性低
	TIM_OC1Init(PWM_TIM, &TIM_OCInitStructure);                     //根据T指定的参数初始化外设TIM3OC1
	TIM_OC1PreloadConfig(PWM_TIM, TIM_OCPreload_Enable);            //使能TIM3在CCR1上的预装载寄存器
 
    TIM_ARRPreloadConfig(PWM_TIM,ENABLE);                           //ARPE使能 
	
	TIM_Cmd(PWM_TIM, ENABLE);              		  
}    

