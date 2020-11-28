#ifndef __RS485_H
#define __RS485_H			 
#include "sys.h" 

#define RS485_TX_EN		                    PGout(6)	           //RS485模式控制.0,接收;1,发送.
                                       
#define RS485_USART                         USART6
#define RS485_USART_TIMER                   RCC_APB2Periph_USART6
#define RS485_USART_NVIC_IRQChannel         USART6_IRQn
#define RS485_USART_IRQHandler              USART6_IRQHandler
#define RS485_USART_CONTROL_PORT_TIMER      RCC_AHB1Periph_GPIOG
#define RS485_USART_CONTROL_PORT            GPIOG
#define RS485_USART_CONTROL_PIN             GPIO_Pin_6
#define RS485_USART_PORT                    GPIOC
#define RS485_USART_PORT_TIMER              RCC_AHB1Periph_GPIOC
#define RS485_USART_PORT_GPIO_PinSource_TX  GPIO_PinSource6
#define RS485_USART_PORT_GPIO_PinSource_RX  GPIO_PinSource7
#define RS485_USART_PORT_GPIO_AF            GPIO_AF_USART6
#define RS485_USART_PORT_TX                 GPIO_Pin_6
#define RS485_USART_PORT_RX                 GPIO_Pin_7

#define RS485_PRE_PRIORITY        0x02
#define RS485_SUB_PRIORITY        0x00

void RS485_Init(u32 bound);
void RS485_Send_Data(volatile const u8* TX_buff,u8 length);

#endif	   
















