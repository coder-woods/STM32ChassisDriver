#include "can.h"
#include "feedback.h"
#include "usart.h"	

u8 CAN_Txbuf[CAN_BUFF_LEN_MAX];

/****************************************************************************
* 名    称: void CAN1_Configuration(void)
* 功    能：CAN初始化
* 入口参数：无  
* 返回参数：无
* 说    明：mode Mode_Normal,普通模式;CAN_Mode_LoopBack,回环模式;
            baud = 42M / (CAN_SJW + CAN_BS1 + CAN_BS2) / CAN_Prescaler
* 作    者：嵌入式小白Woods
****************************************************************************/
void CAN1_Configuration(void)
{
    GPIO_InitTypeDef       GPIO_InitStructure;
    CAN_InitTypeDef        CAN_InitStructure;
  	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
    NVIC_InitTypeDef       NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);       //使能PORTB时钟	                   											 
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);        //使能CAN1时钟	
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8| GPIO_Pin_9;       //初始化GPIO
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                //复用功能
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;              //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;          //100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                //上拉
    GPIO_Init(GPIOB, &GPIO_InitStructure);                      //初始化PB8,PB9
	
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_CAN1);      //GPIOB8引脚复用为CAN1H
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource9,GPIO_AF_CAN1);      //GPIOB9引脚复用为CAN1L
	  
   	CAN_InitStructure.CAN_TTCM=DISABLE;	                        //非时间触发通信模式     	//CAN单元设置
  	CAN_InitStructure.CAN_ABOM=DISABLE;	                        //软件自动离线管理	  
  	CAN_InitStructure.CAN_AWUM=DISABLE;                         //睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
  	CAN_InitStructure.CAN_NART=ENABLE;	                        //禁止报文自动传送 
  	CAN_InitStructure.CAN_RFLM=DISABLE;	                        //报文不锁定,新的覆盖旧的  
  	CAN_InitStructure.CAN_TXFP=DISABLE;	                        //优先级由报文标识符决定 
    
  	CAN_InitStructure.CAN_Mode= CAN_Mode_Normal;                //模式设置 
  	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;	                    //重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位 CAN_SJW_1tq~CAN_SJW_4tq
  	CAN_InitStructure.CAN_BS1=CAN_BS1_7tq;                     //时间段1的时间单元.  Tbs1范围CAN_BS1_1tq ~CAN_BS1_16tq
  	CAN_InitStructure.CAN_BS2=CAN_BS2_6tq;                      //时间段2的时间单元.  Tbs2范围CAN_BS2_1tq ~	CAN_BS2_8tq
  	CAN_InitStructure.CAN_Prescaler=6;                          //分频系数(Fdiv)为brp+1	 1 - 1024
  	CAN_Init(CAN1, &CAN_InitStructure);                         //初始化CAN2
    
    CAN_FilterInitStructure.CAN_FilterNumber=0;	                        //过滤器0配置
  	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 
  	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;      //32位 
  	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;                    //32位ID
  	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;                //32位MASK
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
   	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;  //过滤器0关联到FIFO0
  	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;                //激活过滤器0
  	CAN_FilterInit(&CAN_FilterInitStructure);                           //滤波器初始化
    
    CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
    NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;           //主优先级为2
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;                  //次优先级为2
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
}

/****************************************************************************
* 名    称: u8 CAN1_Send_Msg(u16 stdid,u8 ext_en u16 extid,u8 remote_en, u8* msg, u8 len)
* 功    能：can发送一组数据(标准帧,数据帧)
* 入口参数：stdid 标准标识符 extid 扩展标示符 len:数据长度(最大为8)  msg:数据指针,最大为8个字节
* 返回参数：0,成功;其他,失败;
* 说    明：
* 作    者：嵌入式小白Woods
****************************************************************************/
u8 CAN1_Send_Msg(u16 stdid, u8 ext_en, u16 extid, u8 remote_en, u8* msg, u8 len)
{	
    u8 mbox;
    u16 i=0;
    CanTxMsg TxMessage;
    TxMessage.StdId=stdid;	                    // 标准标识符
    if(ext_en)
    {
        TxMessage.ExtId=extid;	                // 扩展标示符（29位）
        TxMessage.IDE=CAN_ID_EXT;		        // 使用拓展标识符
    }
    else
    {
        TxMessage.IDE=CAN_ID_STD;		        // 使用标准标识符        
    }
    if(remote_en)
    {
        TxMessage.RTR=CAN_RTR_Remote;		    // 消息类型为遥控帧
    }
    else
    {
        TxMessage.RTR=CAN_RTR_DATA;		        // 消息类型为数据帧，一帧8位        
    }

    TxMessage.DLC=len;						
    for(i=0;i<len;i++)
    {
        TxMessage.Data[i]=msg[i];				// 第一帧信息          
    }
    mbox= CAN_Transmit(CAN1, &TxMessage);   
    i=0;
    while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//等待发送结束
    if(i>=0XFFF)return 1;
    return 0;			
}

/****************************************************************************
* 名    称: void CAN1_RX0_IRQHandler(void)
* 功    能：CAN1接收中断函数
* 入口参数：无
* 返回参数：无
* 说    明：
* 作    者：嵌入式小白Woods
****************************************************************************/
void CAN1_RX0_IRQHandler(void)
{
    int32_t temp;
    CanRxMsg RxMessage;
	CAN_Receive(CAN1,CAN_FIFO0, &RxMessage);  /* 此函数包含释放提出报文了的,在非必要时,不需要自己释放 */
	if(RxMessage.StdId == 0x181)
    {
        if(RxMessage.DLC == 6)
        {
            temp = (int32_t)(RxMessage.Data[2]+(RxMessage.Data[3]<<8)+(RxMessage.Data[4]<<16)+(RxMessage.Data[5]<<24));
            ChassisDrvFB.A_WH_v = (int16_t)(temp*100*0.83/42.17);
        }
        else
            WOODS_DBG_PRINTF("RX.StdId = 0x181 DLC error !\r\n");
    }
    else if(RxMessage.StdId == 0x182)
    {
        if(RxMessage.DLC == 6)
        {
            temp = (int32_t)(RxMessage.Data[2]+(RxMessage.Data[3]<<8)+(RxMessage.Data[4]<<16)+(RxMessage.Data[5]<<24));
            ChassisDrvFB.B_WH_v = (int16_t)(-temp*100*0.83/42.17);
        }
        else
            WOODS_DBG_PRINTF("RX.StdId = 0x182 DLC error !\r\n");
    }
    else if(RxMessage.StdId == 0x183)
    {
        if(RxMessage.DLC == 6)
        {
            temp = (int32_t)(RxMessage.Data[2]+(RxMessage.Data[3]<<8)+(RxMessage.Data[4]<<16)+(RxMessage.Data[5]<<24));
            BotArmFB.RDArm_Deg = (uint16_t)(temp%415000);
        }
        else
            WOODS_DBG_PRINTF("RX.StdId = 0x183 DLC error !\r\n");        
    }
    else if(RxMessage.StdId == 0x184)
    {
        if(RxMessage.DLC == 6)
        {
            temp = (int32_t)(RxMessage.Data[2]+(RxMessage.Data[3]<<8)+(RxMessage.Data[4]<<16)+(RxMessage.Data[5]<<24));
            BotArmFB.LDArm_Deg = (uint16_t)(temp%415000);  
        }
        else
            WOODS_DBG_PRINTF("RX.StdId = 0x184 DLC error !\r\n");        
    }
	CAN_ClearITPendingBit(CAN1,CAN_IT_FMP0);  /* 清除挂起中断 */
}





