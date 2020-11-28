#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"

#define PWM_INIT        TIM3_PWM_Init

#define PWM_TIM         TIM3
#define PWM_TIM_CLK     RCC_APB1Periph_TIM3
#define PWM_PORT_TIMER  RCC_AHB1Periph_GPIOA
#define PWM_PORT        GPIOA
#define PWM_PIN_SOURCE1 GPIO_PinSource6
#define PWM_PIN_AF      GPIO_AF_TIM3
#define PWM_PIN1        GPIO_Pin_6

void PWM_INIT(u32 arr,u32 psc);
#endif
