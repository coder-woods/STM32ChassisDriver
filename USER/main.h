#ifndef __MAIN_H
#define __MAIN_H
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "pwm.h"
#include "rs485.h"	
#include "rs232.h"	
#include "timer.h"
#include "iwdg.h"
#include "adc.h"
#include "can.h"
#include "myiic.h"
#include "spi.h"
#include "exti.h"
#include "protocol.h"
#include "feedback.h"
#include "usmart.h"

#include "motor.h"
#include "botarm.h"

#define SYS_BASE_TIMER_INIT     TIM5_Init
#define SPI_INIT                SPI2_Init
#define modbus_recived          protocol_RS485

extern uint8_t LED_Scan ;
extern uint8_t IMU_Scan ;
extern uint8_t MPU_Scan ;
extern uint8_t IRQ_Scan ;
extern uint8_t Batt_Scan;
extern uint8_t ANO_Scan ;
extern uint8_t MODBUS_Scan;
extern uint8_t MOTOR_Scan;
extern volatile u8 RS485_Rxbuf[RS485_BUFF_LEN_MAX];

extern void SYS_BASE_TIMER_INIT(u16 arr,u16 psc);
extern void MOTOR_Init(void);
extern void BOTARM_Init(void);
extern void SetMotor_Speed(void);
#endif


