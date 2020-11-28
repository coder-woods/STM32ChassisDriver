#ifndef __BOTARM_H
#define __BOTARM_H
#include "sys.h"

#define BOTARM_PORT_TIMER     RCC_AHB1Periph_GPIOE
#define BOTARM_PORT           GPIOE
#define BOTARM_PORT_PIN       GPIO_Pin_14

#define BOTARM_POWER_EN       PEout(14)

extern u8 CAN_Txbuf[CAN_BUFF_LEN_MAX];
extern struct BotArmCtrl_def BotArmCtrl;

void MOTOR_Init(void);
void SetBotArm_Speed(void);
uint8_t SetBotArm_speed(uint8_t botarm_name, int32_t speed);
uint8_t SetBotArm_position(uint8_t botarm_name,int32_t position);
uint8_t SetBotArm_EN(uint8_t botarm_name);
#endif
