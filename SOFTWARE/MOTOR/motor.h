#ifndef __MOTOR_H
#define __MOTOR_H
#include "sys.h"

#define MOTOR_PORT_TIMER     RCC_AHB1Periph_GPIOE
#define MOTOR_PORT           GPIOE
#define MOTOR_PORT_PIN       GPIO_Pin_14

#define MOTOR_POWER_EN       PEout(14)

extern u8 CAN_Txbuf[CAN_BUFF_LEN_MAX];
extern struct ChassisDrvCtrl_def ChassisDrvCtrl;

void MOTOR_Init(void);
void SetMotor_Speed(void);
uint8_t SetMotor_speed(uint8_t wheel_name, int32_t speed);
uint8_t SetMotor_position(uint8_t wheel_name,int32_t position);
uint8_t SetMotor_EN(uint8_t wheel_name);
#endif

