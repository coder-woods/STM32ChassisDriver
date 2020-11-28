#ifndef __MYIIC_H
#define __MYIIC_H
#include "sys.h" 
 
#define SDA_IN()  {GPIOB->MODER&=~(3<<(7*2));GPIOB->MODER|=0<<7*2;}	//IO方向设置 PB9输入模式
#define SDA_OUT() {GPIOB->MODER&=~(3<<(7*2));GPIOB->MODER|=1<<7*2;} //IO方向设置 PB9输出模式

#define IIC_SCL    PBout(6)             //IO操作函数    SCL
#define IIC_SDA    PBout(7)             //IO操作函数    SDA	 
#define READ_SDA   PBin(7)              //IO操作函数    输入SDA 

#define IIC_PORT_TIMER      RCC_AHB1Periph_GPIOB
#define IIC_PORT            GPIOB
#define IIC_PORT_PIN        GPIO_Pin_6 | GPIO_Pin_7

void IIC_Init(void);                    //初始化IIC的IO口				 
void IIC_Start(void);				    //发送IIC开始信号
void IIC_Stop(void);	  			    //发送IIC停止信号
void IIC_Send_Byte(u8 txd);			    //IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack);    //IIC读取一个字节
u8 IIC_Wait_Ack(void); 				    //IIC等待ACK信号
void IIC_Ack(void);					    //IIC发送ACK信号
void IIC_NAck(void);				    //IIC不发送ACK信号
void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	  
#endif


