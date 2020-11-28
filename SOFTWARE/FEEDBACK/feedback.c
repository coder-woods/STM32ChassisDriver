#include "feedback.h"
#include "usart.h"	

struct FlagType_def flag = {0,0,0,0,0,0,0};   //数据反馈标志
static u16 feedback_frame_id = 0;

/*************************************
*        反馈状态信息
*************************************/
struct SysStatFB_def    SysStatFB;
struct ChassisDrvFB_def ChassisDrvFB;
struct IMUFB_def        IMUFB;
struct SonarFB_def      SonarFB;
struct GPSFB_def        GPSFB;
struct BotArmFB_def     BotArmFB;
struct LEDFB_def        LEDFB;

/****************************************************************************
* 名    称: void feedback_RS232(void)
* 功    能：定时反馈数据至上位机
* 入口参数：无
* 返回参数：无
* 说    明：
* 作    者：嵌入式小白Woods
****************************************************************************/
void feedback_RS232(void)
{
    static u8 SendSysStatCnt    = 0;       //发送频率相同时用于错开时间
	static u8 SendChassisDrvCnt = 2;
	static u8 SendIMUCnt 	    = 4;
	static u8 SendSonarCnt	    = 6;
	static u8 SendGPSCnt	    = 8;
    static u8 SendARMCnt        = 10;
    static u8 SendLEDCnt        = 12;
    
	SendSysStatCnt++;
	SendChassisDrvCnt++;
//    SendIMUCnt++;
//    SendSonarCnt++;
//    SendGPSCnt++;
//    SendARMCnt++;
//    SendLEDCnt++;
    
    if(SendSysStatCnt >= SEND_SYS_STAT_TIME)
	{
		SendSysStatCnt = 0;
		flag.SendSysStat = 1;
//        WOODS_DBG_PRINTF("send_system_status !\r\n");
	}
	if(SendChassisDrvCnt >= SEND_CHASSIS_DRV_TIME)
	{
		SendChassisDrvCnt = 0;
		flag.SendChassisDrv = 1;
//        WOODS_DBG_PRINTF("send_chassis_driver !\r\n");
	}
	if(SendIMUCnt >= SEND_IMU_TIME)
	{
		SendIMUCnt = 0;
		flag.SendIMU = 1;
//        WOODS_DBG_PRINTF("send_imu !\r\n");
	}
	if(SendSonarCnt >= SEND_SONAR_TIME)
	{
		SendSonarCnt = 0;
		flag.SendSonar = 1;
//        WOODS_DBG_PRINTF("send_sonar !\r\n");
	}
	if(SendGPSCnt >= SEND_GPS_TIME)
	{
		SendGPSCnt = 0;
		flag.SendGPS = 1;
//        WOODS_DBG_PRINTF("send_gps !\r\n");
	}
    if(SendARMCnt >= SEND_ARM_TIME)
	{
		SendARMCnt = 0;
		flag.SendARM = 1;
//        WOODS_DBG_PRINTF("send_arm !\r\n");
	}
    if(SendLEDCnt >= SEND_LED_TIME)
	{
		SendLEDCnt = 0;
		flag.SendLED = 1;
//        WOODS_DBG_PRINTF("send_light !\r\n");
	}
    if(flag.SendSysStat)
    {
        flag.SendSysStat = 0;
        SendSysStatFunc(&SysStatFB);
    }
    if(flag.SendChassisDrv)
    {
        flag.SendChassisDrv = 0;
        SendChassisDrvFunc(&ChassisDrvFB);
    }
    if(flag.SendIMU)
    {
        flag.SendIMU = 0;
        SendIMUFunc(&IMUFB);
    }
    if(flag.SendSonar)
    {
        flag.SendSonar = 0;
        SendSonarFunc(&SonarFB);
    }
    if(flag.SendGPS)
    {
        flag.SendGPS = 0;
        SendGPSFunc(&GPSFB);
    }
    if(flag.SendARM)
    {
        flag.SendARM = 0;
        SendBotArmFunc(&BotArmFB);
    }
    if(flag.SendLED)
    {
        flag.SendLED = 0;
        SendLEDFBFunc(&LEDFB);
    }
}

/****************************************************************************
* 名    称: void SendSysStatFunc(const struct SysStatFB_def* SysStatFB_ptr)
* 功    能：反馈系统状态数据
* 入口参数：System_Status_Feedback_ptr 系统状态数据结构体指针
* 返回参数：无
* 说    明：
* 作    者：嵌入式小白Woods
****************************************************************************/
void SendSysStatFunc(const struct SysStatFB_def* SysStatFB_ptr)
{
    RS232_Txbuf[0]  = HEAD1;
    RS232_Txbuf[1]  = HEAD2;
    RS232_Txbuf[2]  = 0x0C;
    RS232_Txbuf[3]  = SYS_STAT_FB_ID;
    RS232_Txbuf[4]  = SysStatFB_ptr->SysStat;
    RS232_Txbuf[5]  = (u8)(SysStatFB_ptr->VBATT >> 8);
    RS232_Txbuf[6]  = (u8)SysStatFB_ptr->VBATT;
    RS232_Txbuf[7]  = SysStatFB_ptr->BatSOC;
    RS232_Txbuf[8]  = SysStatFB_ptr-> BotErrCode1;
    RS232_Txbuf[9]  = SysStatFB_ptr->BotErrCode2;
    RS232_Txbuf[10] = SysStatFB_ptr->BatFETstat;
    RS232_Txbuf[11] = SysStatFB_ptr->BatProtectStat;
    RS232_Txbuf[12] = (u8)(feedback_frame_id >> 8);
    RS232_Txbuf[13] = (u8)feedback_frame_id;
    RS232_Txbuf[14] = SerialMsgChecksum(RS232_Txbuf, 0x0C + 2);
    
    RS232_Send_Data(RS232_Txbuf, 0x0C + 3);
    
    feedback_frame_id++;
}

/****************************************************************************
* 名    称: void SendChassisDrvFunc(const struct ChassisDrvFB_def* ChassisDrvFB_ptr)
* 功    能：反馈底盘轮速数据
* 入口参数：Chassis_Driver_Feedback_ptr 底盘轮速数据结构体指针
* 返回参数：无
* 说    明：
* 作    者：嵌入式小白Woods
****************************************************************************/
void SendChassisDrvFunc(const struct ChassisDrvFB_def* ChassisDrvFB_ptr)
{
    RS232_Txbuf[0]  = HEAD1;
    RS232_Txbuf[1]  = HEAD2;
    RS232_Txbuf[2]  = 0x0C;
    RS232_Txbuf[3]  = CHASSIS_DRV_EB_ID;
    RS232_Txbuf[4]  = (u8)(ChassisDrvFB_ptr->A_WH_v >> 8);
    RS232_Txbuf[5]  = (u8)ChassisDrvFB_ptr->A_WH_v;
    RS232_Txbuf[6]  = (u8)(ChassisDrvFB_ptr->B_WH_v >> 8);
    RS232_Txbuf[7]  = (u8)ChassisDrvFB_ptr->B_WH_v;
    RS232_Txbuf[8]  = (u8)(ChassisDrvFB_ptr->C_WH_v >> 8);
    RS232_Txbuf[9]  = (u8)ChassisDrvFB_ptr->C_WH_v;
    RS232_Txbuf[10] = (u8)(ChassisDrvFB_ptr->D_WH_v >> 8);
    RS232_Txbuf[11] = (u8)ChassisDrvFB_ptr->D_WH_v;
    RS232_Txbuf[12] = (u8)(feedback_frame_id >> 8);
    RS232_Txbuf[13] = (u8)feedback_frame_id;
    RS232_Txbuf[14] = SerialMsgChecksum(RS232_Txbuf, 0x0C + 2);
    
    RS232_Send_Data(RS232_Txbuf, 0x0C + 3);
    
    feedback_frame_id++;
}

/****************************************************************************
* 名    称: void SendIMUFunc(const struct IMUFB_def* IMUFB_ptr)
* 功    能：反馈IMU数据
* 入口参数：IMU_Feedback_ptr IMU数据结构体指针
* 返回参数：无
* 说    明：
* 作    者：嵌入式小白Woods
****************************************************************************/
void SendIMUFunc(const struct IMUFB_def* IMUFB_ptr)
{
    RS232_Txbuf[0]  = HEAD1;
    RS232_Txbuf[1]  = HEAD2;
    RS232_Txbuf[2]  = 0x10;
    RS232_Txbuf[3]  = IMU_FB_ID;
    RS232_Txbuf[4]  = (u8)(IMUFB_ptr->GyrX >> 8);
    RS232_Txbuf[5]  = (u8)IMUFB_ptr->GyrX;
    RS232_Txbuf[6]  = (u8)(IMUFB_ptr->GyrY >> 8);
    RS232_Txbuf[7]  = (u8)IMUFB_ptr->GyrY;
    RS232_Txbuf[8]  = (u8)(IMUFB_ptr->GyrZ >> 8);
    RS232_Txbuf[9]  = (u8)IMUFB_ptr->GyrZ;
    RS232_Txbuf[10] = (u8)(IMUFB_ptr->AccX >> 8);
    RS232_Txbuf[11] = (u8)IMUFB_ptr->AccX;
    RS232_Txbuf[12] = (u8)(IMUFB_ptr->AccY >> 8);
    RS232_Txbuf[13] = (u8)IMUFB_ptr->AccY;    
    RS232_Txbuf[14] = (u8)(IMUFB_ptr->AccZ >> 8);
    RS232_Txbuf[15] = (u8)IMUFB_ptr->AccZ;
    RS232_Txbuf[16] = (u8)(feedback_frame_id >> 8);
    RS232_Txbuf[17] = (u8)feedback_frame_id;
    RS232_Txbuf[18] = SerialMsgChecksum(RS232_Txbuf, 0x10 + 2);
    
    RS232_Send_Data(RS232_Txbuf, 0x10 + 3);
    
    feedback_frame_id++;
}

/****************************************************************************
* 名    称: void SendSonarFunc(const struct SonarFB_def* SonarFB_ptr)
* 功    能：反馈超声波数据
* 入口参数：Sonar_Feedback_ptr 超声波数据结构体指针
* 返回参数：无
* 说    明：
* 作    者：嵌入式小白Woods
****************************************************************************/
void SendSonarFunc(const struct SonarFB_def* SonarFB_ptr)
{
    RS232_Txbuf[0]  = HEAD1;
    RS232_Txbuf[1]  = HEAD2;
    RS232_Txbuf[2]  = 0x0C;
    RS232_Txbuf[3]  = SONAR_FB_ID;
    RS232_Txbuf[4]  = SonarFB_ptr->Sonar0;
    RS232_Txbuf[5]  = SonarFB_ptr->Sonar1;
    RS232_Txbuf[6]  = SonarFB_ptr->Sonar2;
    RS232_Txbuf[7]  = SonarFB_ptr->Sonar3;
    RS232_Txbuf[8]  = SonarFB_ptr->Sonar4;
    RS232_Txbuf[9]  = SonarFB_ptr->Sonar5;
    RS232_Txbuf[10] = SonarFB_ptr->Sonar6;
    RS232_Txbuf[11] = SonarFB_ptr->Sonar7;
    RS232_Txbuf[12] = (u8)(feedback_frame_id >> 8);
    RS232_Txbuf[13] = (u8)feedback_frame_id;
    RS232_Txbuf[14] = SerialMsgChecksum(RS232_Txbuf, 0x0C + 2);
    
    RS232_Send_Data(RS232_Txbuf, 0x0C + 3);
    
    feedback_frame_id++;    
}

/****************************************************************************
* 名    称: void SendGPSFunc(const struct GPSFB_def* GPSFB_ptr)
* 功    能：反馈GPS数据
* 入口参数：GPS_Feedback_ptr GPS数据结构体指针
* 返回参数：无
* 说    明：
* 作    者：嵌入式小白Woods
****************************************************************************/
void SendGPSFunc(const struct GPSFB_def* GPSFB_ptr)
{
    RS232_Txbuf[0]  = HEAD1;
    RS232_Txbuf[1]  = HEAD2;
    RS232_Txbuf[2]  = 0x0C;
    RS232_Txbuf[3]  = GPS_FB_ID;
    RS232_Txbuf[4]  = GPSFB_ptr->LatDeg;
    RS232_Txbuf[5]  = GPSFB_ptr->LatMin;
    RS232_Txbuf[6]  = GPSFB_ptr->LatSec;
    RS232_Txbuf[7]  = (u8)(GPSFB_ptr->LongDeg >> 8);
    RS232_Txbuf[8]  = (u8)(GPSFB_ptr->LongDeg);
    RS232_Txbuf[9]  = GPSFB_ptr->LongMin;
    RS232_Txbuf[10] = GPSFB_ptr->LongSec;
    RS232_Txbuf[11] = 0x00;
    RS232_Txbuf[12] = (u8)(feedback_frame_id >> 8);
    RS232_Txbuf[13] = (u8)feedback_frame_id;
    RS232_Txbuf[14] = SerialMsgChecksum(RS232_Txbuf, 0x0C + 2);
    
    RS232_Send_Data(RS232_Txbuf, 0x0C + 3);
    
    feedback_frame_id++;    
}

/****************************************************************************
* 名    称: void SendBotArmFunc(const struct BotArmFB_def* BotArmFB_ptr)
* 功    能：反馈机械臂数据
* 入口参数：Robot_Arm_Feedback_ptr 机械臂数据结构体指针
* 返回参数：无
* 说    明：
* 作    者：嵌入式小白Woods
****************************************************************************/
void SendBotArmFunc(const struct BotArmFB_def* BotArmFB_ptr)
{
    RS232_Txbuf[0]  = HEAD1;
    RS232_Txbuf[1]  = HEAD2;
    RS232_Txbuf[2]  = 0x17;
    RS232_Txbuf[3]  = BOT_ARM_FB_ID;
    RS232_Txbuf[4]  = (u8)(BotArmFB_ptr->SH_Deg >> 8);
    RS232_Txbuf[5]  = (u8)BotArmFB_ptr->SH_Deg;
    RS232_Txbuf[6]  = (u8)(BotArmFB_ptr->EL_Deg >> 8);
    RS232_Txbuf[7]  = (u8)BotArmFB_ptr->EL_Deg;
    RS232_Txbuf[8]  = (u8)(BotArmFB_ptr->WR_Deg >> 8);
    RS232_Txbuf[9]  = (u8)BotArmFB_ptr->WR_Deg;
    RS232_Txbuf[10] = (u8)(BotArmFB_ptr->ClawTrun_Deg >> 8);
    RS232_Txbuf[11] = (u8)BotArmFB_ptr->ClawTrun_Deg;
    RS232_Txbuf[12] = BotArmFB_ptr->ClawOpen_Len;
    RS232_Txbuf[13] = (u8)(BotArmFB_ptr->Body_Deg >> 8);
    RS232_Txbuf[14] = (u8)BotArmFB_ptr->Body_Deg;
    RS232_Txbuf[15] = (u8)(BotArmFB_ptr->RUArm_Deg >> 8);
    RS232_Txbuf[16] = (u8)BotArmFB_ptr->RUArm_Deg;
    RS232_Txbuf[17] = (u8)(BotArmFB_ptr->RDArm_Deg >> 8);
    RS232_Txbuf[18] = (u8)BotArmFB_ptr->RDArm_Deg;
    RS232_Txbuf[19] = (u8)(BotArmFB_ptr->LUArm_Deg >> 8);
    RS232_Txbuf[20] = (u8)BotArmFB_ptr->LUArm_Deg;
    RS232_Txbuf[21] = (u8)(BotArmFB_ptr->LDArm_Deg >> 8);
    RS232_Txbuf[22] = (u8)BotArmFB_ptr->LDArm_Deg;
    RS232_Txbuf[23] = (u8)(feedback_frame_id >> 8);
    RS232_Txbuf[24] = (u8)feedback_frame_id;
    RS232_Txbuf[25] = SerialMsgChecksum(RS232_Txbuf, 0x17 + 2);
    
    RS232_Send_Data(RS232_Txbuf, 0x17 + 3);
    
    feedback_frame_id++;     
}

/****************************************************************************
* 名    称: void SendLEDFBFunc(const struct LEDFB_def* LEDFB_ptr)
* 功    能：反馈LED灯数据
* 入口参数：Light_Feedback_ptr LED灯数据结构体指针
* 返回参数：无
* 说    明：
* 作    者：嵌入式小白Woods
****************************************************************************/
void SendLEDFBFunc(const struct LEDFB_def* LEDFB_ptr)
{
    RS232_Txbuf[0]  = HEAD1;
    RS232_Txbuf[1]  = HEAD2;
    RS232_Txbuf[2]  = 0x0C;
    RS232_Txbuf[3]  = LED_FB_ID;
    RS232_Txbuf[4]  = LEDFB_ptr->LED1Mode;
    RS232_Txbuf[5]  = LEDFB_ptr->LED1R_Lux;
    RS232_Txbuf[6]  = LEDFB_ptr->LED1G_Lux;
    RS232_Txbuf[7]  = LEDFB_ptr->LED1B_Lux;
    RS232_Txbuf[8]  = LEDFB_ptr->LED2Mode;
    RS232_Txbuf[9]  = LEDFB_ptr->LED2R_Lux;
    RS232_Txbuf[10] = LEDFB_ptr->LED2G_Lux;
    RS232_Txbuf[11] = LEDFB_ptr->LED2B_Lux;
    RS232_Txbuf[12] = (u8)(feedback_frame_id >> 8);
    RS232_Txbuf[13] = (u8)feedback_frame_id;
    RS232_Txbuf[14] = SerialMsgChecksum(RS232_Txbuf, 0x0C + 2);
    
    RS232_Send_Data(RS232_Txbuf, 0x0C + 3);
    
    feedback_frame_id++;     
}

/****************************************************************************
* 名    称: static u8 SerialMsgChecksum(volatile const u8 *data, u8 length) 
* 功    能：和校验函数
* 入口参数：buff 接收数组首地址 length 数组长度
* 返回参数：无
* 说    明：
* 作    者：嵌入式小白Woods
****************************************************************************/
static u8 SerialMsgChecksum(volatile const u8 *buff, u8 length) 
{ 
    uint8_t i =0;
    uint8_t sum =0;
    uint8_t checksum =0;
  
    for(i=0; i<length; i++)
    {
        sum += *buff++;
    }
    checksum = sum &0xff;
    return checksum;
}
