#include "RS485.h"	
#include "usart.h"	

volatile u8 RS485_Rxbuf[RS485_BUFF_LEN_MAX], RS485_Txbuf[RS485_BUFF_LEN_MAX], received_RS485_len = 0;    // buffer and flag of RS485

/****************************************************************************
* 名    称: void RS485_Init(u32 bound)
* 功    能：RS485总线初始化
* 入口参数：bound 波特率
* 返回参数：无
* 说    明：
* 作    者：嵌入式小白Woods
****************************************************************************/
void RS485_Init(u32 bound)
{  	 
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RS485_USART_PORT_TIMER | RS485_USART_CONTROL_PORT_TIMER,ENABLE);          //使能RS485_USART_PORT_TIMER时钟
	RCC_APB2PeriphClockCmd(RS485_USART_TIMER,ENABLE);                                                //使能USART6时钟
	
	GPIO_PinAFConfig(RS485_USART_PORT,RS485_USART_PORT_GPIO_PinSource_TX,RS485_USART_PORT_GPIO_AF);  //GPIOC6引脚复用为USART6 TX
	GPIO_PinAFConfig(RS485_USART_PORT,RS485_USART_PORT_GPIO_PinSource_RX,RS485_USART_PORT_GPIO_AF);  //GPIOC7引脚复用为USART6 RX
	  
	GPIO_InitStructure.GPIO_Pin = RS485_USART_PORT_TX | RS485_USART_PORT_RX;        //USART6引脚配置 TX PC6 RX PC7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                                    //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	                            //速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                                  //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                                    //上拉
	GPIO_Init(RS485_USART_PORT,&GPIO_InitStructure);                                //初始化 
	
	GPIO_InitStructure.GPIO_Pin = RS485_USART_CONTROL_PIN;                          
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;                                   //输出
	GPIO_Init(RS485_USART_CONTROL_PORT,&GPIO_InitStructure);                        //初始化Ctrl
	                                                          
	USART_InitStructure.USART_BaudRate = bound;                                     //波特率设置 USART6 初始化设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;                     //字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;                          //一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;                             //无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	                //收发模式
	USART_Init(RS485_USART, &USART_InitStructure);                                  //初始化串口
	
	USART_Cmd(RS485_USART, ENABLE);                                                 //使能串口
	USART_ClearFlag(RS485_USART, USART_FLAG_TC);
	
	USART_ITConfig(RS485_USART, USART_IT_RXNE, ENABLE);                             //开启接受中断
	NVIC_InitStructure.NVIC_IRQChannel = RS485_USART_NVIC_IRQChannel;               //USART6 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=RS485_PRE_PRIORITY;        //抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =RS485_SUB_PRIORITY;		        //子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			                        //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	                                                //根据指定的参数初始化VIC寄存器
	
    RS485_TX_EN=0;				                                                    //初始化默认为接收模式
}

/****************************************************************************
* 名    称: void RS485_USART_IRQHandler(void)   
* 功    能：USART3接收中断服务函数
* 入口参数：无
* 返回参数：无
* 说    明：  
* 作    者：嵌入式小白Woods
****************************************************************************/	
void RS485_USART_IRQHandler(void)                                    
{
	u8 rec_data;	
	if(USART_GetITStatus(RS485_USART, USART_IT_RXNE) != RESET)      //接收到数据
	{	 	
        rec_data = USART_ReceiveData(RS485_USART);                  //读取接收到的数据        
        RS485_Rxbuf[received_RS485_len] = rec_data;
        received_RS485_len++;
	}  
} 

/****************************************************************************
* 名    称: void RS485_Send_Data(volatile const u8* TX_buff,u8 length)
* 功    能：RS485发送length个字节
* 入口参数：length 发送数据长度
* 返回参数：无
* 说    明：(为了和本代码的接收匹配,这里建议数据长度不要超过128个字节)    
* 作    者：嵌入式小白Woods
****************************************************************************/	
void RS485_Send_Data(volatile const u8* TX_buff,u8 length)
{
	u8 t;
	RS485_TX_EN=1;			                                          //设置为发送模式
  	for(t=0;t<length;t++)		                                      //循环发送数据
	{
        while(USART_GetFlagStatus(RS485_USART,USART_FLAG_TC)==RESET); //等待发送结束		
        USART_SendData(RS485_USART,TX_buff[t]);                       //发送数据
	}	 
	while(USART_GetFlagStatus(RS485_USART,USART_FLAG_TC)==RESET);     //等待发送结束		  
	RS485_TX_EN=0;		                                              //发送完设置为接收模式	
}


