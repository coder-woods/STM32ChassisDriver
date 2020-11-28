#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 

#define DEBUG_USART                         USART3
#define DEBUG_USART_TIMER                   RCC_APB1Periph_USART3
#define DEBUG_USART_NVIC_IRQChannel         USART3_IRQn
#define DEBUG_USART_IRQHandler              USART3_IRQHandler
#define DEBUG_USART_PORT                    GPIOB
#define DEBUG_USART_PORT_TIMER              RCC_AHB1Periph_GPIOB
#define DEBUG_USART_PORT_GPIO_PinSource_TX  GPIO_PinSource10
#define DEBUG_USART_PORT_GPIO_PinSource_RX  GPIO_PinSource11
#define DEBUG_USART_PORT_GPIO_AF            GPIO_AF_USART3
#define DEBUG_USART_PORT_TX                 GPIO_Pin_10
#define DEBUG_USART_PORT_RX                 GPIO_Pin_11

#define WOODS_DBG_PRINTF(fmt, args...)  \
    do\
    {\
        printf("¡¾Woods_DBG File:%s Line:%d Function:%s¡¿", __FILE__, __LINE__, __FUNCTION__);\
        printf(fmt, ##args);\
    }while(0)  

void DEBUG_USART_Init(u32 bound);
void DEBUG_USART_Send(const u8* send_buff,u8 length);
#endif


