#ifndef __CAN_H
#define __CAN_H	 
#include "sys.h"	    

extern struct ChassisDrvFB_def ChassisDrvFB;
extern struct BotArmFB_def BotArmFB;

void CAN1_Configuration(void);  //CAN≥ı ºªØ
u8 CAN1_Send_Msg(u16 stdid, u8 ext_en, u16 extid, u8 remote_en, u8* msg, u8 len);

#endif


