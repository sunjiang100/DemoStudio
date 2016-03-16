/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$											���ݹ�ҵ���ӿƼ����޹�˾
$							GuangZhou GuoYe Electronics Sciense&Technology Co.,LTD.
$                   	http://www.guoyee.com		 Email: guoyegps@126.com
$						 	QQȺ��9069471/9069433/9524253		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      �ļ�����
	�ļ���: 	 	Plus.h
	����������	�ٶ����崦���������崦��
	���뻷����	
	Ŀ��CPU: 	LPC176x
	���ߣ�
	����ʱ�䣺
	����༭������UltraEdit 11.00a+  �Ʊ�����Ϊ�����ַ�������ɶ��Ը��á�
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
	uint32	OneHour;			// 1Сʱ������
	uint32	OneMin;			// 1����������
	uint16	OneSec;			// 1��������
	uint16	D2Sec;			// 0.2S��������0.2���������Ϊ0ʱ����0.2���ƽ���ٶ�ȡ��һ���ƽ���ٶȳ���5	
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
	������
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
	uint32		SpeedPlusPeriod,InsPlusPeriod;		// �ٶ��������ڣ�˲ʱ�������ڣ�0.2��˲ʱ��
	//uint32		PeriodSum,tPeriodCnt,tPeriodTotalCnt;//,tPeriod[PERIOD_SAMPE_TIMES];
	uint16		SecSpeed,uiLowSpeed[5];
	uchar			RitIntCnt;
	uint32		Timer3Clock;
	SP_SW			SpeedSw[5];
	
};

PULSE_EXT		__noinit__		PulseCls		Pulse;		//  	



#endif


