#include "iwdg.h"

/****************************************************************************
* 名    称: void IWDG_Init(u8 prer,u16 rlr)
* 功    能：初始化独立看门狗
* 入口参数：prer:分频数:0~7 rlr:自动重装载值,0~0XFFF
* 返回参数：无
* 说    明：分频因子=4*2^prer.但最大值只能是256!
            时间计算(大概):Tout=((4*2^prer)*rlr)/32 (ms).
* 作    者：嵌入式小白Woods
****************************************************************************/	
void IWDG_Init(u8 prer,u16 rlr)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); //使能对IWDG->PR IWDG->RLR的写
	IWDG_SetPrescaler(prer);                      //设置IWDG分频数:0~7
	IWDG_SetReload(rlr);                          //设置IWDG自动重装载值,0~0XFFF
	IWDG_ReloadCounter();                         //reload
	IWDG_Enable();                                //使能看门狗
}

/****************************************************************************
* 名    称: void IWDG_Feed(void)
* 功    能：喂狗
* 入口参数：无
* 返回参数：无
* 说    明：
* 作    者：嵌入式小白Woods
****************************************************************************/	
void IWDG_Feed(void)
{
	IWDG_ReloadCounter();
}
