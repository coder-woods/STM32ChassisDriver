#include "usart.h"	

u8 Debug_rx_buff[DEBUG_USART_LEN_MAX],USART_RX_STA; //接收缓冲,最大DEBUG_USART_LEN_MAX个字节.末字节为换行符 

#pragma import(__use_no_semihosting)        //加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
         
struct __FILE                               //标准库需要的支持函数        
{ 
    int handle; 
}; 

FILE __stdout;       

_sys_exit(int x)                            //定义_sys_exit()以避免使用半主机模式    
{ 
    x = x; 
} 

int fputc(int ch, FILE *f)                  //重定义fputc函数 
{ 	
    while((DEBUG_USART->SR&0X40)==0);            //循环发送,直到发送完毕   
    DEBUG_USART->DR = (u8) ch;   
	return ch;
}

/****************************************************************************
* 名    称: void RS485_Init(u32 bound)
* 功    能：RS485总线初始化
* 入口参数：bound 波特率
* 返回参数：无
* 说    明：
* 作    者：嵌入式小白Woods
****************************************************************************/	
void DEBUG_USART_Init(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(DEBUG_USART_PORT_TIMER,ENABLE);     //使能DEBUG_USART_PORT时钟
	RCC_APB1PeriphClockCmd(DEBUG_USART_TIMER,ENABLE);          //使能DEBUG_USART时钟 
 
    GPIO_PinAFConfig(DEBUG_USART_PORT,DEBUG_USART_PORT_GPIO_PinSource_TX,DEBUG_USART_PORT_GPIO_AF);   //GPIOB10引脚复用为USART3 TX DEBUG_USART
	GPIO_PinAFConfig(DEBUG_USART_PORT,DEBUG_USART_PORT_GPIO_PinSource_RX,DEBUG_USART_PORT_GPIO_AF);   //GPIOB11引脚复用为USART3 RX DEBUG_USART
    
    GPIO_InitStructure.GPIO_Pin = DEBUG_USART_PORT_TX | DEBUG_USART_PORT_RX;    //DEBUG_USART_PORT配置 TX PB10 RX PB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                                //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	                        //速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                              //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                                //上拉
	GPIO_Init(DEBUG_USART_PORT,&GPIO_InitStructure);                            //初始化 

	USART_InitStructure.USART_BaudRate = bound;                                      //DEBUG_USART 初始化设置 波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;                      //字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;                           //一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;                              //无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	                 //收发模式
    USART_Init(DEBUG_USART, &USART_InitStructure);                                   //初始化DEBUG_USART
	
    USART_Cmd(DEBUG_USART, ENABLE);                                         //DEBUG_USART
	USART_ClearFlag(DEBUG_USART, USART_FLAG_TC);
	
    USART_ITConfig(DEBUG_USART, USART_IT_RXNE, ENABLE);                     //DEBUG_USART NVIC 配置  中断使能
    NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_NVIC_IRQChannel;       //USART3-DEBUG_USART中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;                 //抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		                //子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			                //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	                                        //根据指定的参数初始化NVIC寄存器
}

/****************************************************************************
* 名    称: void DEBUG_USART_IRQHandler(void)   
* 功    能：DEBUG_USART接收中断服务函数
* 入口参数：无
* 返回参数：无
* 说    明：  
* 作    者：嵌入式小白Woods
****************************************************************************/	
void DEBUG_USART_IRQHandler(void)                	                //DEBUG_USART中断服务程序
{
	u8 Res;
	if(USART_GetITStatus(DEBUG_USART, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res =USART_ReceiveData(DEBUG_USART);                    //(DEBUG_USART->DR);读取接收到的数据  	
		if((USART_RX_STA&0x8000)==0)                            //接收未完成
		{
			if(USART_RX_STA&0x4000)                             //接收到了0x0d
			{
				if(Res!=0x0a)USART_RX_STA=0;                    //接收错误,重新开始
				else USART_RX_STA|=0x8000;	                    //接收完成了 
			}
			else //还没收到0X0D
			{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
				{
					Debug_rx_buff[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(DEBUG_USART_LEN_MAX-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}   		         
    } 
} 
/****************************************************************************
* 名    称: void DEBUG_USART_Send(const u8* send_buff,u8 length)
* 功    能：debug发送length个字节
* 入口参数：send_buff 数组首地址 length 发送数据长度
* 返回参数：无
* 说    明：(为了和本代码的接收匹配,这里建议数据长度不要超过128个字节)  
* 作    者：嵌入式小白Woods
****************************************************************************/	
void DEBUG_USART_Send(const u8* TX_buff,u8 length)
{
	u8 t;
  	for(t=0;t<length;t++)		                                      //循环发送数据
	{
        while(USART_GetFlagStatus(DEBUG_USART,USART_FLAG_TC)==RESET); //等待发送结束		
        USART_SendData(DEBUG_USART,TX_buff[t]);                       //发送数据
	}	 
	while(USART_GetFlagStatus(DEBUG_USART,USART_FLAG_TC)==RESET);     //等待发送结束		  
}
 



