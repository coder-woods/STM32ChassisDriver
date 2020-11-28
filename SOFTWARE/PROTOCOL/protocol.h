#ifndef __PROTOCOL_H
#define __PROTOCOL_H			 
#include "sys.h" 

#define MODBUS_FAC1_Add  0x01
#define MODBUS_FAC2_Add  0x02
#define MODBUS_FAC3_Add  0x03
#define MODBUS_FAC4_Add  0x04
#define MODBUS_FAC5_Add  0x05

#define FUNC_INQUIRE_ID  0x03
#define FAC_SET_ID       0x06

/*************************************
*        上位机指令ID
*************************************/
#define SYS_STAT_CMD_ID     0x01
#define CHASSIS_DRV_CMD_ID  0x02
#define LED_CMD_ID          0x03
#define BOT_ARM_CMD_ID      0x04

#define PID_CMD_ID          0xC1

extern volatile u8 RS485_Txbuf[RS485_BUFF_LEN_MAX],received_RS485_len;
/*************************************
*        接收控制指令
*************************************/
struct SysStatCtrl_def
{
    u8 SystemStatus;
    u8 LensFocus;
    u8 BotArmFunction;
    u8 BotGraspingForce;
};

struct ChassisDrvCtrl_def
{
    s16 AWH_v;
    s16 BWH_v;
    s16 CWH_v;
    s16 DWH_v;
};

struct LEDCtrl_def
{
    u8 LED1Mode;
    u8 LED1R_Lux;
    u8 LED1G_Lux;
    u8 LED1B_Lux;
    u8 LED2Mode;
    u8 LED2R_Lux;
    u8 LED2G_Lux;
    u8 LED2B_Lux;
};

struct BotArmCtrl_def
{
    u8 SH_Ctrl;
    u8 EL_Ctrl;
    u8 WR_Ctrl;
    u8 ClawTrun_Ctrl;
    u8 ClawOpen_Ctrl;
    u8 Body_Ctrl;
    u8 RUArm_Ctrl;
    u8 RDArm_Ctrl;
    u8 LUArm_Ctrl;
    u8 LDArm_Ctrl;
};

/*************************************
*        调试信息
*************************************/
struct PID_Ctrl_def
{
    double Kp;
    double Ki;
    double Kd;
};

void protocol_RS232(volatile const u8* RX_buff,u8 length);
void protocol_RS485(volatile const u8* RX_buff);
void modbus_send(void);
static u8 SerialMsgChecksum(volatile const u8 *data, u8 len);
static uint16_t modbus_crc16 (volatile const uint8_t *bufData, volatile uint16_t buflen);

#endif

