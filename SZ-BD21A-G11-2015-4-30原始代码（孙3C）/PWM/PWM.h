/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												 
$										 
$                       	 
$								  		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      文件描述
	文件名: 
	功能描述：
	编译环境：
	目标CPU: 
	作者：
	创建时间：
	建议编辑环境：UltraEdit 11.00a+  制表符宽度为三个字符，程序可读性更好。
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/					  

#ifndef   	__PWM_H__
#define   	__PWM_H__

#include    "..\Config\UserConfig.h"


#ifdef      PWM_GLOBALS
#define     PWM_EXT
#else
#define     PWM_EXT    extern
#endif


#define     SIMU           0              //1 启用模拟行车代码    0不启用
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	
*/

typedef		struct	_SIMU_SPEED_
{
	uint32	TimeSlot;
	uint16	Speed;
	uint16	Interval;
} SIMU_SPEED,*pSIMU_SPEED;



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	RTC类
*/
class			PwmCls
{
	//////////////////////////////////////////
public:
	PwmCls(void);
	void		Init(uint32);
	void		UnInit(void);	
	void		SimuSpeedProcess(void);
	void		SetTargetSpeed(uint32 S,uint32 Time);
private:
	uint32	NowSpeed,TargetSpeed,Acceleration; // 实际处理是先换算成频率
};


PWM_EXT	__noinit__		PwmCls		Pwm;		//



#endif    // 
/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */

