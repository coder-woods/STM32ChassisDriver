#include "timer.h"

uint8_t LED_Scan    = 0;
uint8_t IMU_Scan    = 0;
uint8_t MOTOR_Scan  = 0;
uint8_t MPU_Scan    = 0;
uint8_t IRQ_Scan    = 0;
uint8_t Batt_Scan   = 0;
uint8_t ANO_Scan    = 0;
uint8_t MODBUS_Scan = 0;

/****************************************************************************
* 名    称: void SYS_BASE_TIMER_INIT(void)
* 功    能：通用定时器5中断初始化
* 入口参数：无
* 返回参数：无
* 说    明：定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us. 
            Ft=定时器工作频率,单位:Mhz
* 作    者：嵌入式小白Woods
****************************************************************************/	
void SYS_BASE_TIMER_INIT(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(SYS_BASE_TIMER_CLK,ENABLE);                  //使能定时器时钟
	
    TIM_TimeBaseInitStructure.TIM_Period = arr; 	                    //自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;                        //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;       //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(SYS_BASE_TIMER,&TIM_TimeBaseInitStructure);        //初始化定时器
	
	TIM_ITConfig(SYS_BASE_TIMER,TIM_IT_Update,ENABLE);                  //允许定时器更新中断
	NVIC_InitStructure.NVIC_IRQChannel=SYS_BASE_TIMER_NVIC_IRQChannel;  //定时器中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=PRE_PRIORITY;  //抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=SUB_PRIORITY;         //子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);

    TIM_Cmd(SYS_BASE_TIMER,ENABLE);                                     //使能定时器
}

/****************************************************************************
* 名    称: void SYS_BASE_TIMER_IRQ(void)
* 功    能：定时器的中断函数
* 入口参数：无
* 返回参数：无
* 说    明：系统基时时钟
* 作    者：嵌入式小白Woods
****************************************************************************/	
void SYS_BASE_TIMER_IRQ(void)
{
    static uint16_t ms5 = 0, ms10 = 0, ms40 = 0, ms50 = 0,ms100 = 0, ms200 = 0, ms500 = 0; //分频系数
	if(TIM_GetITStatus(SYS_BASE_TIMER,TIM_IT_Update)==SET) //溢出中断
	{
		ms5++;
		ms10++;
        ms40++;
        ms50++;
		ms100++;
		ms200++;
		ms500++;
        
		ANO_Scan = 1;
		if(ms5 >= 5)            //200Hz
		{
			ms5 = 0;
			MPU_Scan = 1;
		}
		if(ms10 >= 10)          //100Hz
		{
			ms10 = 0;
			IMU_Scan = 1;
		}
        if(ms40 >= 40)          //25Hz
        {
            ms40 =0;
            MOTOR_Scan =1;
        }
		if(ms50 >= 50)          //20Hz
		{
			ms50 = 0;
		}
		if(ms100 >= 100)        //10Hz
		{
			ms100 = 0;
            MODBUS_Scan = 1;
		}
		if(ms200 >= 200)        //5Hz
		{
			ms200 = 0;
		}
		if(ms500 >= 500)        //2Hz
		{
			ms500 = 0;
			LED_Scan = 1;
		}
	}
	TIM_ClearITPendingBit(SYS_BASE_TIMER,TIM_IT_Update);  //清除中断标志位
}
