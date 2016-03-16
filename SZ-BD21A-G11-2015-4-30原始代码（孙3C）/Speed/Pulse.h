/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$											广州国业电子科技有限公司
$							GuangZhou GuoYe Electronics Sciense&Technology Co.,LTD.
$                   	http://www.guoyee.com		 Email: guoyegps@126.com
$						 	QQ群：9069471/9069433/9524253		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      文件描述
	文件名: 	 	Plus.h
	功能描述：	速度脉冲处理、高速脉冲处理
	编译环境：	
	目标CPU: 	LPC176x
	作者：
	创建时间：
	建议编辑环境：UltraEdit 11.00a+  制表符宽度为三个字符，程序可读性更好。
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/		


#ifndef		__PULSE_H__
#define		__PULSE_H__


#ifdef      PULSE_GLOBALS
#define     PULSE_EXT
#else
#define     PULSE_EXT    extern
#endif


#include		"..\Config\RTOS.h"


PULSE_EXT    uchar         EMButton_Statu, MOTO_Statu;


typedef		struct
{
	uint32	Total;
	uint32	OneHour;			// 1小时脉冲数
	uint32	OneMin;			// 1分钟脉冲数
	uint16	OneSec;			// 1秒脉冲数
	uint16	D2Sec;			// 0.2S脉冲数，0.2秒的脉冲数为0时，本0.2秒的平均速度取上一秒的平均速度除以5	
}PULSE_CNT,*pPULSE_CNT;


typedef		struct
{
	uchar		Speed;
	uchar		Sw;
}SP_SW,*pSP_SW;



typedef		struct
{
	uint32		TotalPulse;
	uint16		SecSpeed;
	uint16		MinSpeed;	
	uint16		HourSpeed;
	SP_SW			SpeedSwitch[5];	
}SPEED_VALUE,*pSPEED_VALUE;

#define		PERIOD_SAMPE_TIMES			10


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	脉冲类
*/
class			PulseCls
{
public:
	PulseCls(void);
	bool			RitInit(uint32);
	void			Timer0Init(void);
	void			Timer1Init(void);
	void			Timer3Init(void);
	
	__inline		void		RitInterrupt(void);	
	__inline		void		Timer0Interrupt(void);	
	__inline		void		Timer1Interrupt(void);	
	__inline		void		Timer3Interrupt(void);
	
	void			SetPulseInitValue(uint32	 Value);	
	uint16		GetSecSpeed(uchar	 *pSpeedSw);
	uint32		GetMinPlus(void);	
	uint32		GetHourPlus(void);
	uint32		GetTotalPlus(void);	

private:
	
	PULSE_CNT		SpeedPulseCnt,MotoPulseCnt,HighPulseCnt;	
	uint32		SpeedPlusPeriod,InsPlusPeriod;		// 速度脉冲周期，瞬时脉冲周期（0.2秒瞬时）
	//uint32		PeriodSum,tPeriodCnt,tPeriodTotalCnt;//,tPeriod[PERIOD_SAMPE_TIMES];
	uint16		SecSpeed,uiLowSpeed[5];
	uchar			RitIntCnt;
	uint32		Timer3Clock;
	SP_SW			SpeedSw[5];
	
};

PULSE_EXT		__noinit__		PulseCls		Pulse;		//  	



#endif


