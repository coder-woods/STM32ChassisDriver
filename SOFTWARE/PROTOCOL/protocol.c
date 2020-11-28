#include "protocol.h"
#include "RS485.h"	
#include "usart.h"	

/*************************************
*        接收控制指令
*************************************/
struct SysStatCtrl_def      SysStatCtrl;
struct ChassisDrvCtrl_def   ChassisDrvCtrl;
struct LEDCtrl_def          LEDCtrl;
struct BotArmCtrl_def       BotArmCtrl;

/*************************************
*        调试信息
*************************************/
struct PID_Ctrl_def         PID_Ctrl;

/****************************************************************************
* 名    称: void protocol_RS232(volatile const u8* RX_buff, u8 length)
* 功    能：RS232协议解析
* 入口参数：RX_buff 接收数组首地址 length 数组长度
* 返回参数：无
* 说    明：
* 作    者：嵌入式小白Woods
****************************************************************************/	
void protocol_RS232(volatile const u8* RX_buff, u8 length)
{
//    uint8_t check_sum = 0;
    int16_t temp_AWH_v = 0, temp_BWH_v = 0;
//    check_sum = SerialMsgChecksum(RX_buff,(length+2));
//    WOODS_DBG_PRINTF("RX_buff = %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x check_sum=%2x!\r\n",
//                     RX_buff[0],RX_buff[1],RX_buff[2],RX_buff[3],RX_buff[4],RX_buff[5],RX_buff[6],RX_buff[7],
//                     RX_buff[8],RX_buff[9],RX_buff[10],RX_buff[11],RX_buff[12],RX_buff[13],RX_buff[14],check_sum);
    if( RX_buff[length+2] == SerialMsgChecksum(RX_buff,(length+2)))
    {
        switch(RX_buff[3])
        {
            case SYS_STAT_CMD_ID : 
                    SysStatCtrl.SystemStatus     = RX_buff[4];
                    SysStatCtrl.LensFocus        = RX_buff[5];
                    SysStatCtrl.BotArmFunction   = RX_buff[6];
                    SysStatCtrl.BotGraspingForce = RX_buff[7];
//                    WOODS_DBG_PRINTF("RS232 protocol recived system_status_control !\r\n");
                    break;
            case CHASSIS_DRV_CMD_ID : 
                    temp_AWH_v = (int16_t)((RX_buff[4] << 8) + RX_buff[5]);
                    temp_AWH_v =(temp_AWH_v*42.17/(100*0.83));
                    if(temp_AWH_v >= 7800)
                    {
                       ChassisDrvCtrl.AWH_v = 7800;
                    }
                    else if(temp_AWH_v <= -7800)
                    {
                        ChassisDrvCtrl.AWH_v = -7800;
                    }
                    else
                        ChassisDrvCtrl.AWH_v = (int16_t)temp_AWH_v;
                    temp_BWH_v = (int16_t)((RX_buff[6] << 8) + RX_buff[7]);
                    temp_BWH_v = (temp_BWH_v*42.17/(100*0.83));
                    if(temp_BWH_v >= 7800)
                    {
                       ChassisDrvCtrl.BWH_v = 7800; 
                    }
                    else if(temp_BWH_v <= -7800)
                    {
                        ChassisDrvCtrl.BWH_v = -7800;
                    }
                    else
                        ChassisDrvCtrl.BWH_v = (int16_t)temp_BWH_v;
//                    WOODS_DBG_PRINTF("RS232 protocol recived chassis_driver_control !\r\n"); 
                    break;
            case LED_CMD_ID : 
                    LEDCtrl.LED1Mode  = RX_buff[4];
                    LEDCtrl.LED1R_Lux = RX_buff[5];
                    LEDCtrl.LED1G_Lux = RX_buff[6];
                    LEDCtrl.LED1B_Lux = RX_buff[7];
                    LEDCtrl.LED2Mode  = RX_buff[8];
                    LEDCtrl.LED2R_Lux = RX_buff[9];
                    LEDCtrl.LED2G_Lux = RX_buff[10];
                    LEDCtrl.LED2B_Lux = RX_buff[11];
                    WOODS_DBG_PRINTF("RS232 protocol recived light_control !\r\n");                
                    break;
            case BOT_ARM_CMD_ID : 
                    BotArmCtrl.SH_Ctrl       = RX_buff[4];
                    BotArmCtrl.EL_Ctrl       = RX_buff[5];
                    BotArmCtrl.WR_Ctrl       = RX_buff[6];
                    BotArmCtrl.ClawTrun_Ctrl = RX_buff[7];
                    BotArmCtrl.ClawOpen_Ctrl = RX_buff[8];
                    BotArmCtrl.Body_Ctrl     = RX_buff[9];
                    BotArmCtrl.RUArm_Ctrl    = RX_buff[10];
                    BotArmCtrl.RDArm_Ctrl    = RX_buff[11];
                    BotArmCtrl.LUArm_Ctrl    = RX_buff[12];
                    BotArmCtrl.LDArm_Ctrl    = RX_buff[13];
                    WOODS_DBG_PRINTF("RS232 protocol recived robot_arm_control !\r\n");
                    break;
            case PID_CMD_ID : 
                    PID_Ctrl.Kp = (double)((RX_buff[4] << 8) + RX_buff[5]);
                    PID_Ctrl.Ki = (double)((RX_buff[6] << 8) + RX_buff[7]);
                    PID_Ctrl.Kd = (double)((RX_buff[8] << 8) + RX_buff[9]);
                    WOODS_DBG_PRINTF("RS232 protocol recived PID_control !\r\n");                 
                    break;
            default:
                    WOODS_DBG_PRINTF("RS232 protocol control ID ERROR !\r\n");
                    break;
        }
    }
    else
    {
        WOODS_DBG_PRINTF("RS232 protocol checksum ERROR !\r\n");
    }
}

/****************************************************************************
* 名    称: void protocol_RS485(volatile const u8* RX_buff)
* 功    能：RS485协议解析
* 入口参数：RX_buff 接收数组首地址 length 数组长度
* 返回参数：无
* 说    明：
* 作    者：嵌入式小白Woods
****************************************************************************/
void protocol_RS485(volatile const u8* RX_buff)
{
    uint8_t check_CRC_H = 0, check_CRC_L = 0;
    uint16_t check_CRC = 0;
    if(received_RS485_len != 0)
    {
        WOODS_DBG_PRINTF("len = %d !\r\n",received_RS485_len);
        check_CRC = modbus_crc16( RX_buff, (uint16_t)(received_RS485_len-2));
        check_CRC_H = (uint8_t)( check_CRC >> 8 );
        check_CRC_L = (uint8_t)check_CRC;
        WOODS_DBG_PRINTF("RX_buff = %2x %2x %2x %2x %2x %2x %2x check_CRC=%4x!\r\n",
                     RX_buff[0],RX_buff[1],RX_buff[2],RX_buff[3],RX_buff[4],RX_buff[5],RX_buff[6],check_CRC);
        if((RX_buff[received_RS485_len-1] == check_CRC_H)&&(RX_buff[received_RS485_len-2] == check_CRC_L))
        {
            if(RX_buff[0] == MODBUS_FAC1_Add)                     //modbus设备1接收
            {   
                if(RX_buff[1] == FUNC_INQUIRE_ID)
                {
                    WOODS_DBG_PRINTF("FACILITY1 recived 03 ! \r\n"); 
                }
                else if(RX_buff[1] == FAC_SET_ID)
                {
                    WOODS_DBG_PRINTF("FACILITY1 recived 06 ! \r\n"); 
                }
                else
                    WOODS_DBG_PRINTF("MODBUS_FUNCTION_ID error \r\n");
            }
            else if(RX_buff[0] == MODBUS_FAC2_Add)                 //modbus设备2接收
            {
                if(RX_buff[1] == FUNC_INQUIRE_ID)
                {
                    WOODS_DBG_PRINTF("FACILITY2 recived 03 ! \r\n"); 
                }
                else if(RX_buff[1] == FAC_SET_ID)
                {
                    WOODS_DBG_PRINTF("FACILITY2 recived 06 ! \r\n"); 
                }
                else
                    WOODS_DBG_PRINTF("MODBUS_FUNCTION_ID error \r\n");                
            }
            else
                WOODS_DBG_PRINTF("MODBUS_FACILITY_Add error \r\n");                
        }
        else
            WOODS_DBG_PRINTF("modbus_crc16 error \r\n");
        received_RS485_len = 0; 
    }     
}

/****************************************************************************
* 名    称: void modbus_send(void)
* 功    能：MODBUS发送数据
* 入口参数：
* 返回参数：无
* 说    明：
* 作    者：嵌入式小白Woods
****************************************************************************/
void modbus_send(void)
{
    uint16_t check_CRC;

    RS485_Txbuf[0] = 0x01;
    RS485_Txbuf[1] = 0x03;
    RS485_Txbuf[2] = 0x00;
    RS485_Txbuf[3] = 0x08;
    RS485_Txbuf[4] = 0x00;
    RS485_Txbuf[5] = 0x01;
    check_CRC = modbus_crc16(RS485_Txbuf,6);
    RS485_Txbuf[6] = (uint8_t)check_CRC;
    RS485_Txbuf[7] = (uint8_t)(check_CRC >> 8);
    RS485_Send_Data(RS485_Txbuf,8);
    WOODS_DBG_PRINTF("MODBUS TX_buff = %2x %2x %2x %2x %2x %2x %2x %2x !\r\n",
        RS485_Txbuf[0],RS485_Txbuf[1],RS485_Txbuf[2],RS485_Txbuf[3],RS485_Txbuf[4],RS485_Txbuf[5],RS485_Txbuf[6],RS485_Txbuf[7]);
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

/****************************************************************************
* 名    称: static uint16_t modbus_crc16 (volatile const uint8_t *bufData, volatile uint16_t buflen)
* 功    能：CRC校验函数
* 入口参数：bufData 接收数组首地址 buflen 数组长度
* 返回参数：无
* 说    明：
* 作    者：嵌入式小白Woods
****************************************************************************/
static uint16_t modbus_crc16 (volatile const uint8_t *bufData, volatile uint16_t buflen)
{
    uint16_t CRC_check = 0xffff;
    uint16_t POLYNOMIAL = 0xa001;
    uint8_t i, j;
   
    for (i = 0; i < buflen; i++)
    {
        CRC_check ^= bufData[i];
        for (j = 0; j < 8; j++)
        {
            if ((CRC_check & 0x0001) != 0)
            {
                CRC_check >>= 1;
                CRC_check ^= POLYNOMIAL;
            }
            else
            {
                CRC_check >>= 1;
            }
        }
    }
    return CRC_check;
}


