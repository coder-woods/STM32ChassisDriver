#include "rs232.h"
#include "protocol.h"

volatile u8 RS232_Rxbuf[RS232_BUFF_LEN_MAX], RS232_Txbuf[RS232_BUFF_LEN_MAX], received_RS232_len = 0;

/****************************************************************************
* 名    称: void RS232_Init(u32 bound)
* 功    能：RS232总线初始化
* 入口参数：bound 波特率
* 返回参数：无
* 说    明：
* 作    者：嵌入式小白Woods
****************************************************************************/	
void RS232_Init(u32 bound)
{ 
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RS232_USART_PORT_TIMER,ENABLE);     //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RS232_USART_TIMER,ENABLE);          //使能USART1时钟
 
	GPIO_PinAFConfig(RS232_USART_PORT,RS232_USART_PORT_GPIO_PinSource_TX,RS232_USART_PORT_GPIO_AF);  //GPIOA9引脚复用为USART1 TX
	GPIO_PinAFConfig(RS232_USART_PORT,RS232_USART_PORT_GPIO_PinSource_RX,RS232_USART_PORT_GPIO_AF);  //GPIOA10引脚复用为USART1 RX
	
    GPIO_InitStructure.GPIO_Pin = RS232_USART_PORT_TX | RS232_USART_PORT_RX;       //RS232_USART1引脚配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                                   //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	                           //速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                                 //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                                   //上拉
	GPIO_Init(RS232_USART_PORT,&GPIO_InitStructure);                               //初始化PA9|PA10
	
	USART_InitStructure.USART_BaudRate = bound;                                     //RS232_USART1初始化设置 波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;                     //字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;                          //一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;                             //无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	                //收发模式
    USART_Init(RS232_USART, &USART_InitStructure);   

    USART_Cmd(RS232_USART, ENABLE);                                                //使能RS232_USART1
	USART_ClearFlag(RS232_USART, USART_FLAG_TC);
    
    USART_ITConfig(RS232_USART, USART_IT_RXNE, ENABLE);                            //USART NVIC 配置  中断使能
    NVIC_InitStructure.NVIC_IRQChannel = RS232_USART_NVIC_IRQChannel;              //使能RS232_USART1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=RS232_PRE_PRIORITY;       //抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =RS232_SUB_PRIORITY;		       //子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			                       //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	                                               //根据指定的参数初始化NVIC寄存器
}

/****************************************************************************
* 名    称: void RS232_USART_IRQHandler(void)   
* 功    能：USART1接收中断服务函数
* 入口参数：无
* 返回参数：无
* 说    明： 
* 作    者：嵌入式小白Woods
****************************************************************************/
void RS232_USART_IRQHandler(void)                	
{
	u8 Res;
    static u8 RxCnt = 0;
	if(USART_GetITStatus(RS232_USART, USART_IT_RXNE) != RESET)   //接收中断
	{
		Res = USART_ReceiveData(RS232_USART);                     //(USART->DR);	//读取接收到的数据 
        RS232_Rxbuf[ RxCnt ] = Res;
        if(RxCnt == 0)
        {
            RxCnt = (HEAD1 != Res) ? 0 : RxCnt + 1;
        }
        else if(RxCnt == 1)
        {
            RxCnt = (HEAD2 != Res) ? 0 : RxCnt + 1;
        }
        else if(RxCnt == 2)
        {
            received_RS232_len = Res;
            RxCnt++;
        }
        else if(RxCnt > 2)
        {
            RxCnt++;
            if(RxCnt == received_RS232_len + 3)
            {
                protocol_RS232(RS232_Rxbuf,received_RS232_len);
                received_RS232_len = 0;
                RxCnt = 0;
            }     
        }   
    } 
}

/****************************************************************************
* 名    称: void RS232_Send_Data(volatile const u8* send_buff,u8 length)
* 功    能：RS485发送length个字节
* 入口参数：无
* 返回参数：无
* 说    明：(为了和本代码的接收匹配,这里建议数据长度不要超过128个字节)     
* 作    者：嵌入式小白Woods
****************************************************************************/	
void RS232_Send_Data(volatile const u8* TX_buff,u8 length)
{	   
	u8 t;
  	for(t=0;t<length;t++)		                                         //循环发送数据
	{
        while(USART_GetFlagStatus(RS232_USART,USART_FLAG_TC) == RESET); //等待发送结束		
        USART_SendData(RS232_USART, TX_buff[t]);                        //发送数据
	}	 
	while(USART_GetFlagStatus(RS232_USART,USART_FLAG_TC) == RESET);     //等待发送结束		     
}  
