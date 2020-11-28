#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"

#define SYS_BASE_TIMER_INIT             TIM5_Init
#define SYS_BASE_TIMER_IRQ              TIM5_IRQHandler
#define SYS_BASE_TIMER_CLK              RCC_APB1Periph_TIM5
#define SYS_BASE_TIMER                  TIM5
#define SYS_BASE_TIMER_NVIC_IRQChannel  TIM5_IRQn

#define PRE_PRIORITY        0x03
#define SUB_PRIORITY        0x03

void SYS_BASE_TIMER_INIT(u16 arr,u16 psc);
#endif
