/********************************************************************************************************************
   项 目 名   : STM32底盘驱动程序 
   版 本 号   : v1.0
   作    者   : 嵌入式小白Woods
   邮    箱   ：zyn805672690@126.com
   生成日期   : 2020-10-27
   修改内容   : 创建工程
   说    明   :  —— —— —— —— —— —— —— —— —— —— —— —— —— —— —— —— ——— —— —— —— —— —— —— —— —— —— —— —— —— —— —— ——
                |  接口类型  |            引脚定义及功能        |  中断优先级  |              备注               |
                |   RS485    |           TX PC6 | RX PC7        |      20      |         USART6 ctrl PG6         |
                |   RS232    |           TX PA9 | RX PA10       |      21      |            USART1               |
                |            |           TX PA2 | RX PA3        |              |            USART2（待使用）     |
                |    CAN     |        CAN_H PB8 | CAN_L PB9     |      22      |             CAN1                |
                |   DEBUG    |          TX PB10 | RX PB11       |      32      |            USART3               |
                |   TIMER5   |            系统基时时钟          |      33      |         系统基时时钟            |
                |   TIMER4   |               USMART             |      13      |            USMART               | 
                |    EXTI    |             PF6 PF7 PF8          |      01      |                                 |                
                | PWM(TIMER3)|                 PA6              |              |              CH1                |
                |    LED     |            PG13 PG14 PG15        |              |                                 |
                |    ADC1    |                 PA5              |              |    CH5、CH16(内部温度传感器)    |
                |    I2C     |          SCL PB6 | SDA PB7       |              |         软件模拟I2C             |
                |    SPI     | SCL PB13 | MISO PB14 | MOSI PB15 |              |             SPI2                |
                 —— —— —— —— —— —— —— —— —— —— —— —— —— —— —— —— ——— —— —— —— —— —— —— —— —— —— —— —— —— —— —— ——
                 IWDG：溢出时间4S
*********************************************************************************************************************/
#include "main.h"

int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置系统中断优先级分组2
	delay_init(168);                                //初始化延时函数
    
	DEBUG_USART_Init(115200);                       //初始化串口波特率为115200
    SYS_BASE_TIMER_INIT(1000-1,84-1);               //84M/84/1000=1KHz time=1ms
    LED_Init();
    BEEP_Init();
    EXTIX_Init();
    PWM_INIT(500-1,84-1);	                        //84M/84/500=2KHz
    RS232_Init(115200);
    RS485_Init(9600);
    IIC_Init();
    SPI_INIT();
    Adc_Init();	
    CAN1_Configuration();
    MOTOR_Init();
    BOTARM_Init();
    usmart_dev.init(84); 	                        //初始化USMART
    IWDG_Init(6,500);                               //预分频数为256,重载值为500,溢出时间为4s
    TIM_SetCompare1(TIM3,125);	                    //修改比较值，修改占空比
    while(1)
	{
        if(ANO_Scan)                                //1KHz
        {
            ANO_Scan=0;
            feedback_RS232();
            //BEEP_Call();
        }
        if(IMU_Scan)                                //100Hz
		{
			IMU_Scan  = 0;
		}
        if(MOTOR_Scan)                              //25Hz
		{
			MOTOR_Scan  = 0;
            SetMotor_Speed();
//            SetBotArm_Speed();
		}
        if(MODBUS_Scan)                             //10Hz
		{
			MODBUS_Scan  = 0;
            //modbus_recived(RS485_Rxbuf);
            //modbus_send();
		}
		if(LED_Scan)                                //2Hz
		{
			LED_Scan = 0;
            LED_Light();
		}
        IWDG_Feed();
	}
}
