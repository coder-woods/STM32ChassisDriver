#ifndef __EXTI_H
#define __EXIT_H	 
#include "sys.h"  		

#define EXTI_GPIO_TIMER         RCC_AHB1Periph_GPIOF
#define EXTI_PORT               GPIOF
#define EXTI_PORT_PIN           GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8
#define EXTI_PORT_SOURCE_GPIOx  EXTI_PortSourceGPIOF
#define EXTI_LINE               EXTI_Line6 | EXTI_Line7 | EXTI_Line8
#define EXTI_PIN_SOURCE1        EXTI_PinSource6
#define EXTI_PIN_SOURCE2        EXTI_PinSource7
#define EXTI_PIN_SOURCE3        EXTI_PinSource8

#define EXTI_PRE_PRIORITY        0x00
#define EXTI_SUB_PRIORITY        0x01

#define KEY0 		PFin(6)	 
#define KEY1 		PFin(7)		
#define KEY2 		PFin(8)	
void EXTIX_Init(void);	//外部中断初始化		 					    
#endif

























