#include "spi.h"
#include "delay.h"

/****************************************************************************
* 名    称: void SPI2_Init(void)
* 功    能：初始化SPI
* 入口参数：无
* 返回参数：无
* 说    明：
* 作    者：嵌入式小白Woods
****************************************************************************/
void SPI2_Init(void)
{	 
    GPIO_InitTypeDef  GPIO_InitStructure;
    SPI_InitTypeDef  SPI_InitStructure;
	
    RCC_AHB1PeriphClockCmd(SPI_PORT_TIMER, ENABLE);                             //使能GPIOB时钟
    RCC_APB1PeriphClockCmd(SPI_TIMER, ENABLE);                                  //使能SPI2时钟
 
    GPIO_InitStructure.GPIO_Pin = SPI_PORT_PIN;                                 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                                //复用功能
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                              //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;                          //100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                                //上拉
    GPIO_Init(SPI_PORT, &GPIO_InitStructure);                                   //初始化
	
	GPIO_PinAFConfig(SPI_PORT,SPI_PORT_GPIO_PinSource_SCL,SPI_PORT_GPIO_AF);    //复用为 SPI SCL
	GPIO_PinAFConfig(SPI_PORT,SPI_PORT_GPIO_PinSource_MISO,SPI_PORT_GPIO_AF);   //复用为 SPI MISO
	GPIO_PinAFConfig(SPI_PORT,SPI_PORT_GPIO_PinSource_MOSI,SPI_PORT_GPIO_AF);   //复用为 SPI MOSI
 
	RCC_APB1PeriphResetCmd(SPI_TIMER,ENABLE);                                   //复位SPI2
	RCC_APB1PeriphResetCmd(SPI_TIMER,DISABLE);                                  //停止复位SPI2

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		                //设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		            //设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		                    //串行同步时钟的空闲状态为高电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	                    //串行同步时钟的第二个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		                    //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16; //定义波特率预分频的值:波特率预分频值为16
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB;	                //指定数据传输从MSB位还是LSB位开始:数据传输从lSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	                        //CRC值计算的多项式
	SPI_Init(SPI, &SPI_InitStructure);                                  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
	SPI_Cmd(SPI, ENABLE);                                               //使能SPI外设	 
}   

/****************************************************************************
* 名    称: u8 SPI2_ReadByte(u8 TxData)
* 功    能：SPI读取一个字节
* 入口参数：TxData 写入数据内容
* 返回参数：无
* 说    明：
* 作    者：嵌入式小白Woods
****************************************************************************/
u8 SPI2_ReadByte(u8 TxData)
{		 			 
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET){}    //等待发送区空  
	SPI_I2S_SendData(SPI2, TxData);                                     //通过外设SPIx发送一个byte  数据
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET){}   //等待接收完一个byte	
	return SPI_I2S_ReceiveData(SPI2);                                   //返回通过SPIx最近接收的数据		    	
}

/****************************************************************************
* 名    称: void SPI2_WriteByte(u8 TxData)
* 功    能：SPI写入一个字节
* 入口参数：TxData 写入数据内容
* 返回参数：无
* 说    明：
* 作    者：嵌入式小白Woods
****************************************************************************/
void SPI2_WriteByte(u8 TxData)
{		 			 
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET){}    //等待发送区空  
	SPI_I2S_SendData(SPI2, TxData);                                     //通过外设SPIx发送一个byte  数据
}

/****************************************************************************
* 名    称: void SPI2_Write(u8 TxData1 ,u16 TxData2)
* 功    能：SPI写入特定数据
* 入口参数：TxData1 TxData2 写入数据的内容
* 返回参数：无
* 说    明：
* 作    者：嵌入式小白Woods
****************************************************************************/
void SPI2_Write(u8 TxData1 ,u16 TxData2)
{
	SPI2_CS=1;
    SPI2_WriteByte(TxData1);    //A0~A5  C0:0Write 1Read   C1:0  D0~D15
	SPI2_WriteByte(TxData2);
	SPI2_WriteByte(TxData2>>8);
	delay_us(5);
	SPI2_CS=0;
}

/****************************************************************************
* 名    称: u16 SPI2_Read(u8 TxData)
* 功    能：SPI读取特定数据
* 入口参数：TxData 需读取地址
* 返回参数：无
* 说    明：
* 作    者：嵌入式小白Woods
****************************************************************************/
u16 SPI2_Read(u8 TxData)
{
	u16 RxData=0,RxData1=0,RxData2=0;
	SPI2_CS=1;
	SPI2_WriteByte(TxData|0x40);
	RxData1=SPI2_ReadByte(0xFF);
	RxData2=SPI2_ReadByte(0xFF);
	RxData=RxData1|(RxData2<<8);
	delay_us(5);
	SPI2_CS=0;	
	return RxData;
}






