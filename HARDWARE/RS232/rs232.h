#ifndef __RS232_H
#define __RS232_H		
#include "sys.h" 

#define HEAD1 0xAA
#define HEAD2 0x55

#define RS232_USART                         USART1
#define RS232_USART_TIMER                   RCC_APB2Periph_USART1
#define RS232_USART_NVIC_IRQChannel         USART1_IRQn
#define RS232_USART_IRQHandler              USART1_IRQHandler
#define RS232_USART_PORT                    GPIOA
#define RS232_USART_PORT_TIMER              RCC_AHB1Periph_GPIOA
#define RS232_USART_PORT_GPIO_PinSource_TX  GPIO_PinSource9
#define RS232_USART_PORT_GPIO_PinSource_RX  GPIO_PinSource10
#define RS232_USART_PORT_GPIO_AF            GPIO_AF_USART1
#define RS232_USART_PORT_TX                 GPIO_Pin_9
#define RS232_USART_PORT_RX                 GPIO_Pin_10

#define RS232_PRE_PRIORITY        0x02
#define RS232_SUB_PRIORITY        0x01

void RS232_Init(u32 bound);
void RS232_Send_Data(volatile const u8* TX_buff,u8 length);

#endif
