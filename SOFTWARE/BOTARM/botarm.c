#include "botarm.h"
#include "protocol.h"
#include "can.h"
#include "delay.h"
#include "usart.h"	

enum BOTARM_NAME
{
    RUArm =0, RDArm = 3, LUArm, LDArm = 4,
}botarm_name;

/****************************************************************************
* 名    称: void BOTARM_Init(void)
* 功    能：初始化机械臂、摇臂电机
* 入口参数：无
* 返回参数：无
* 说    明：对电机进行初始化
* 作    者：嵌入式小白Woods
****************************************************************************/
void BOTARM_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(BOTARM_PORT_TIMER, ENABLE);      //使能PORT时钟
    GPIO_InitStructure.GPIO_Pin = BOTARM_PORT_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;          //普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;         //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;     //100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;         //下拉
    GPIO_Init(BOTARM_PORT, &GPIO_InitStructure);           //初始化GPIO
       
    BOTARM_POWER_EN = 1;
    delay_ms(1500);
    SetBotArm_EN(RDArm);
    SetBotArm_EN(LDArm);
    delay_ms(500);
}

/****************************************************************************
* 名    称: void SetBotArm_Speed(void)
* 功    能：设置机械臂、摇臂电机转速或位置
* 入口参数：无
* 返回参数：无
* 说    明：
* 作    者：嵌入式小白Woods
****************************************************************************/
void SetBotArm_Speed(void)
{
    uint8_t ret_RDArm = 1, ret_LDArm = 1;
    uint8_t temp_BotArm_v = 0;
    BotArmCtrl.RDArm_Ctrl = 0xE0;
    BotArmCtrl.LDArm_Ctrl = 0xE0;
    if((BotArmCtrl.RDArm_Ctrl & 0x80) == 0x80)
    {
        temp_BotArm_v = BotArmCtrl.RDArm_Ctrl & 0x3F;
        if(temp_BotArm_v != 0)
        {
            if((BotArmCtrl.RDArm_Ctrl & 0x40) == 0x40)
            {   
                ret_RDArm = SetBotArm_speed(RDArm,(s32)-2000); 
            }
            else
            {
                ret_RDArm = SetBotArm_speed(RDArm,(s32)2000); 
            }        
        }
        else
        {
            ret_RDArm = SetBotArm_speed(RDArm,0);            
        }
    }
    else
    {
        ret_RDArm = SetBotArm_speed(RDArm,-0);  
    }
    
    if((BotArmCtrl.LDArm_Ctrl & 0x80) == 0x80)
    {
        temp_BotArm_v = BotArmCtrl.LDArm_Ctrl & 0x3F;
        if(temp_BotArm_v != 0)
        {
            if((BotArmCtrl.LDArm_Ctrl & 0x40) == 0x40)
            {
                ret_LDArm = SetBotArm_speed(LDArm,(s32)2000);
            }
            else
                ret_LDArm = SetBotArm_speed(LDArm,(s32)-2000); 
        }
        else 
            ret_LDArm = SetBotArm_speed(LDArm,0); 
    }
    else
        ret_LDArm = SetBotArm_speed(LDArm,0);
//    WOODS_DBG_PRINTF("ret RDArm=%d LDArm=%d !\r\n",ret_RDArm, ret_LDArm);
//    WOODS_DBG_PRINTF("wheel_speed A=%d B=%d !\r\n",ChassisDrvCtrl.AWH_v, ChassisDrvCtrl.BWH_v);
}

/****************************************************************************
* 名    称: uint8_t SetBotArm_speed(uint8_t botarm_name, int32_t speed)
* 功    能：设置机械臂、摇臂电机转速
* 入口参数：无
* 返回参数：无
* 说    明：电机需处于速度模式
* 作    者：嵌入式小白Woods
****************************************************************************/
uint8_t SetBotArm_speed(uint8_t botarm_name, int32_t speed)
{
    uint8_t ret;
    CAN_Txbuf[0] = 0xFF;
    CAN_Txbuf[1] = 0x00;
    CAN_Txbuf[2] = 0x03;
    CAN_Txbuf[3] = (uint8_t)speed;
    CAN_Txbuf[4] = (uint8_t)(speed >> 8);
    CAN_Txbuf[5] = (uint8_t)(speed >> 16);
    CAN_Txbuf[6] = (uint8_t)(speed >> 24);
    ret = CAN1_Send_Msg((0x200 + botarm_name),0,0,0,CAN_Txbuf,7);
    return ret;
}

/****************************************************************************
* 名    称: uint8_t SetBotArm_position(uint8_t botarm_name,int32_t position)
* 功    能：设置机械臂、摇臂电机位置
* 入口参数：无
* 返回参数：无
* 说    明：电机需处于位置模式
* 作    者：嵌入式小白Woods
****************************************************************************/
uint8_t SetBotArm_position(uint8_t botarm_name,int32_t position)
{
    uint8_t ret;
//    CAN_Txbuf[0] = 0xFF;                                           //PD0模式
//    CAN_Txbuf[1] = 0x00;
//    CAN_Txbuf[2] = 0x01;
//    CAN_Txbuf[3] = (uint8_t)position;
//    CAN_Txbuf[4] = (uint8_t)(position >> 8);
//    CAN_Txbuf[5] = (uint8_t)(position >> 16);
//    CAN_Txbuf[6] = (uint8_t)(position >> 24);
//    ret = CAN1_Send_Msg((0x200 + botarm_name),0,0,0,CAN_Txbuf,7);

    CAN_Txbuf[0] = 0x23;                                           //SD0模式
    CAN_Txbuf[1] = 0x7A;
    CAN_Txbuf[2] = 0x60;
    CAN_Txbuf[3] = 0x00;
    CAN_Txbuf[4] = (uint8_t)position;
    CAN_Txbuf[5] = (uint8_t)(position >> 8);
    CAN_Txbuf[6] = (uint8_t)(position >> 16);
    CAN_Txbuf[7] = (uint8_t)(position >> 24);
    ret = CAN1_Send_Msg((0x600 + botarm_name),0,0,0,CAN_Txbuf,8);
    
    delay_ms(10);   
    CAN_Txbuf[0] = 0x2B;                                           
    CAN_Txbuf[1] = 0x40;
    CAN_Txbuf[2] = 0x60;
    CAN_Txbuf[3] = 0x00;
    CAN_Txbuf[4] = 0x7F;
    CAN_Txbuf[5] = 0x00;
    CAN_Txbuf[6] = 0x00;
    CAN_Txbuf[7] = 0x00;
    ret = CAN1_Send_Msg((0x600 + botarm_name),0,0,0,CAN_Txbuf,8);
    return ret;
}

/****************************************************************************
* 名    称: uint8_t SetBotArm_EN(uint8_t botarm_name)
* 功    能：设置机械臂、摇臂电机使能
* 入口参数：无
* 返回参数：无
* 说    明：
* 作    者：嵌入式小白Woods
****************************************************************************/
uint8_t SetBotArm_EN(uint8_t botarm_name)
{
    uint8_t ret;
//    CAN_Txbuf[0] = 0x01;                              //PD0模式
//    CAN_Txbuf[1] = botarm_name;
//    ret = CAN1_Send_Msg(0,0,0,0,CAN_Txbuf,2);
    
    CAN_Txbuf[0] = 0x2B;                               //SD0模式
    CAN_Txbuf[1] = 0x40;
    CAN_Txbuf[2] = 0x60;
    CAN_Txbuf[3] = 0x00;
    CAN_Txbuf[4] = 0x06;
    CAN_Txbuf[5] = 0x00;
    CAN_Txbuf[6] = 0x00;
    CAN_Txbuf[7] = 0x00;
    ret = CAN1_Send_Msg((0x600 + botarm_name),0,0,0,CAN_Txbuf,8);
    
    delay_ms(30);
    CAN_Txbuf[4] = 0x0F;
    ret = CAN1_Send_Msg((0x600 + botarm_name),0,0,0,CAN_Txbuf,8);   

    delay_ms(30);    
    CAN_Txbuf[0] = 0x2F;                               //设置为PPM模式
    CAN_Txbuf[1] = 0x60;
    CAN_Txbuf[2] = 0x60;
    CAN_Txbuf[3] = 0x00;
    CAN_Txbuf[4] = 0x01;
    ret = CAN1_Send_Msg((0x600 + botarm_name),0,0,0,CAN_Txbuf,8);
    
    return ret;
}








