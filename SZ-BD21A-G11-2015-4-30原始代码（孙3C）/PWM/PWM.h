/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												 
$										 
$                       	 
$								  		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      �ļ�����
	�ļ���: 
	����������
	���뻷����
	Ŀ��CPU: 
	���ߣ�
	����ʱ�䣺
	����༭������UltraEdit 11.00a+  �Ʊ�����Ϊ�����ַ�������ɶ��Ը��á�
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


#define     SIMU           0              //1 ����ģ���г�����    0������
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	
*/

typedef		struct	_SIMU_SPEED_
{
	uint32	TimeSlot;
	uint16	Speed;
	uint16	Interval;
} SIMU_SPEED,*pSIMU_SPEED;



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	RTC��
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
	uint32	NowSpeed,TargetSpeed,Acceleration; // ʵ�ʴ������Ȼ����Ƶ��
};


PWM_EXT	__noinit__		PwmCls		Pwm;		//



#endif    // 
/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */

