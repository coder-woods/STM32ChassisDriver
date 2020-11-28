#include "led.h" 
#include "usart.h"	

/****************************************************************************
* 名    称: void LED_Init(void)
* 功    能：LED初始化
* 入口参数：无
* 返回参数：无
* 说    明：小灯泡初始化
* 作    者：嵌入式小白Woods
****************************************************************************/	
void LED_Init(void)
{    	 
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(LED_PORT_TIMER, ENABLE);     //使能GPIOG时钟

    GPIO_InitStructure.GPIO_Pin = LED_PORT_PIN;         //LED0/LED1/led2对应IO口
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;       //普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  //100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
    GPIO_Init(LED_PORT, &GPIO_InitStructure);           //初始化GPIO
	
    GPIO_SetBits(LED_PORT,LED_PORT_PIN);                //设置高，灯灭
    
    WOODS_DBG_PRINTF("END of LED_Init() !\r\n");
}

/****************************************************************************
* 名    称: void LED_Light(void)
* 功    能：LED闪烁
* 入口参数：无
* 返回参数：无
* 说    明：小灯泡初始化
* 作    者：嵌入式小白Woods
****************************************************************************/	
void LED_Light(void)
{
    static uint8_t count;
    switch (count)
    {
        case 0 :
            {
                LED0 = 0;
                LED1 = 1;
                LED2 = 1;
                count++;   
            }break;
        case 1 :
            {
                LED0 = 1;
                LED1 = 0;
                LED2 = 1;
                count++; 
            } break;
        case 2 :
            {
                LED0 = 1;
                LED1 = 1;
                LED2 = 0;
                count++; 
            } break;
        case 3 :
            {
                LED0 = 1;
                LED1 = 1;
                LED2 = 1;
                count++; 
            } break; 
        case 4 :
            {
                LED0 = 1;
                LED1 = 1;
                LED2 = 0;
                count++; 
            } break;          
        case 5 :
            {
                LED0 = 1;
                LED1 = 0;
                LED2 = 1;
                count++; 
            } break;
        case 6 :
            {
                LED0 = 0;
                LED1 = 1;
                LED2 = 1;
                count++; 
            } break; 
        case 7 :
            {
                LED0 = 1;
                LED1 = 1;
                LED2 = 1;
                count = 0; 
            } break;             
        default :
            break; 
    }
    WOODS_DBG_PRINTF("led is lighting !\r\n");
}

/****************************************************************************
* 名    称: void BEEP_Init(void)
* 功    能：BEEP初始化
* 入口参数：无
* 返回参数：无
* 说    明：蜂鸣器初始化
* 作    者：嵌入式小白Woods
****************************************************************************/	
void BEEP_Init(void)
{    	 
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(BEEP_PORT_TIMER, ENABLE);      //使能BEEP时钟
  
    GPIO_InitStructure.GPIO_Pin = BEEP_PORT_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;         //普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;        //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;    //100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;        //下拉
    GPIO_Init(BEEP_PORT, &GPIO_InitStructure);            //初始化GPIO
	
    GPIO_ResetBits(BEEP_PORT,BEEP_PORT_PIN);              //蜂鸣器对应引脚GPIOG7拉低,关闭蜂鸣器
    
    WOODS_DBG_PRINTF("END of BEEP_Init() !\r\n");
}

/****************************************************************************
* 名    称: void BEEP_Init(void)
* 功    能：BEEP初始化
* 入口参数：无
* 返回参数：无
* 说    明：蜂鸣器初始化
* 作    者：嵌入式小白Woods
****************************************************************************/
void BEEP_Call(void)
{
    BEEP = ~BEEP;       
    WOODS_DBG_PRINTF("BEEP is ring !\r\n");
}



