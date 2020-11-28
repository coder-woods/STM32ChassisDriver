#ifndef __FEEDBACK_H
#define __FEEDBACK_H	
#include "sys.h" 

#define SEND_SYS_STAT_TIME      40
#define SEND_CHASSIS_DRV_TIME   40
#define SEND_IMU_TIME           40
#define SEND_SONAR_TIME         40
#define SEND_GPS_TIME           40
#define SEND_ARM_TIME           40
#define SEND_LED_TIME           40

#define HEAD1 0xAA
#define HEAD2 0x55

#define SYS_STAT_FB_ID      0x81
#define CHASSIS_DRV_EB_ID   0x82
#define IMU_FB_ID           0x83
#define SONAR_FB_ID         0x84
#define GPS_FB_ID           0x85
#define BOT_ARM_FB_ID       0x86
#define LED_FB_ID           0x87

struct FlagType_def
{
	uint8_t SendSysStat;
    uint8_t SendChassisDrv;
	uint8_t SendIMU;
	uint8_t SendSonar;
	uint8_t SendGPS;
    uint8_t SendARM;
    uint8_t SendLED;
};

/*************************************
*        ·´À¡×´Ì¬ÐÅÏ¢
*************************************/
struct SysStatFB_def
{
    u8  SysStat;
    u16 VBATT;
    u8  BatSOC;
    u8  BotErrCode1;
    u8  BotErrCode2;
    u8  BatFETstat;
    u8  BatProtectStat;
};

struct ChassisDrvFB_def 
{
    s16 A_WH_v;
    s16 B_WH_v;
    s16 C_WH_v;
    s16 D_WH_v;
};

struct IMUFB_def
{
    s16 GyrX;
    s16 GyrY;
    s16 GyrZ;
    s16 AccX;
    s16 AccY;
    s16 AccZ;
};

struct SonarFB_def 
{
    u8 Sonar0;
    u8 Sonar1;
    u8 Sonar2;
    u8 Sonar3;
    u8 Sonar4;
    u8 Sonar5;
    u8 Sonar6;
    u8 Sonar7;
};

struct GPSFB_def
{
    s8  LatDeg;
    u8  LatMin;
    u8  LatSec;
    s16 LongDeg;
    u8  LongMin;
    u8  LongSec;
};

struct BotArmFB_def
{   
    u16 SH_Deg;
    u16 EL_Deg;
    u16 WR_Deg;
    u16 ClawTrun_Deg;
    u8  ClawOpen_Len;
    s16 Body_Deg;
    u16 RUArm_Deg;
    u16 RDArm_Deg;
    u16 LUArm_Deg;
    u16 LDArm_Deg;
};

struct LEDFB_def
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

extern volatile u8 RS232_Txbuf[RS232_BUFF_LEN_MAX];

extern void RS232_Send_Data(volatile const u8* TX_buff,u8 length);

void feedback_RS232(void);
void SendSysStatFunc(const struct SysStatFB_def* SysStatFB_ptr);
void SendChassisDrvFunc(const struct ChassisDrvFB_def* ChassisDrvFB_ptr);
void SendIMUFunc(const struct IMUFB_def* IMUFB_ptr);
void SendSonarFunc(const struct SonarFB_def* SonarFB_ptr);
void SendGPSFunc(const struct GPSFB_def* GPSFB_ptr);
void SendBotArmFunc(const struct BotArmFB_def* BotArmFB_ptr);
void SendLEDFBFunc(const struct LEDFB_def* LEDFB_ptr);
static u8 SerialMsgChecksum(volatile const u8 *buff, u8 length);
#endif
