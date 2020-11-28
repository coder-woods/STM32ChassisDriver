#ifndef __LED_H
#define __LED_H
#include "sys.h"

#define LED_PORT_TIMER      RCC_AHB1Periph_GPIOG
#define LED_PORT            GPIOG
#define LED_PORT_PIN        GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15
#define BEEP_PORT_TIMER     RCC_AHB1Periph_GPIOG
#define BEEP_PORT           GPIOG
#define BEEP_PORT_PIN       GPIO_Pin_7

#define LED0 PGout(13)
#define LED1 PGout(14)
#define LED2 PGout(15)
#define BEEP PGout(7)

void LED_Init(void);
void LED_Light(void);
void BEEP_Init(void);
void BEEP_Call(void);
#endif
