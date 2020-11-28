#include "exti.h"
#include "delay.h" 
	   
/****************************************************************************
* 名    称: void EXTIX_Init(void)
* 功    能：外部中断初始化程序
* 入口参数：无
* 返回参数：无
* 说    明：
* 作    者：嵌入式小白Woods
****************************************************************************/
void EXTIX_Init(void)
{
    GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);              //使能SYSCFG时钟
    RCC_AHB1PeriphClockCmd(EXTI_GPIO_TIMER, ENABLE);                    //使能EXTI_PORT时钟
 
    GPIO_InitStructure.GPIO_Pin = EXTI_PORT_PIN;                        //KEY对应引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;                        //普通输入模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;                  //100M
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                        //上拉
    GPIO_Init(EXTI_PORT, &GPIO_InitStructure);                          //初始化EXTI_PORT_PIN
	
	SYSCFG_EXTILineConfig(EXTI_PORT_SOURCE_GPIOx, EXTI_PIN_SOURCE1);    //PF6 连接到中断线6
	SYSCFG_EXTILineConfig(EXTI_PORT_SOURCE_GPIOx, EXTI_PIN_SOURCE2);    //PF7 连接到中断线7
	SYSCFG_EXTILineConfig(EXTI_PORT_SOURCE_GPIOx, EXTI_PIN_SOURCE3);    //PF8 连接到中断线8
	
	EXTI_InitStructure.EXTI_Line = EXTI_LINE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;                 //中断事件
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;             //下降沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;                           //中断线使能
    EXTI_Init(&EXTI_InitStructure);                                     //配置
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;                          //EXTI9_5_IRQn
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = EXTI_PRE_PRIORITY;   //抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = EXTI_SUB_PRIORITY;          //子优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                             //使能外部中断通道
    NVIC_Init(&NVIC_InitStructure);                                             //配置	   
}

/****************************************************************************
* 名    称: void EXTI9_5_IRQHandler(void)
* 功    能：外部中断服务程序
* 入口参数：无
* 返回参数：无
* 说    明：延时 10 ms软件消抖
* 作    者：嵌入式小白Woods
****************************************************************************/
void EXTI9_5_IRQHandler(void)
{
	delay_ms(10);	//消抖

	if(KEY2 == 1)	                         //当按键按下后就是IO从高电平变为低电平触发外部中断
	{
		EXTI_ClearITPendingBit(EXTI_Line8);  //清除LINE8上的中断标志位  KEY1 对应IO口PF8
	}	
	
	if(KEY1 == 0)	                         //当按键按下后就是IO从高电平变为低电平触发外部中断
	{
		EXTI_ClearITPendingBit(EXTI_Line7);  //清除LINE7上的中断标志位  KEY2 对应IO口PF7
	}	
	
	if(KEY0 == 0)	                         //当按键按下后就是IO从高电平变为低电平触发外部中断
	{
		EXTI_ClearITPendingBit(EXTI_Line6);  //清除LINE6上的中断标志位  KEY3 对应IO口PF6
	}
	
}

