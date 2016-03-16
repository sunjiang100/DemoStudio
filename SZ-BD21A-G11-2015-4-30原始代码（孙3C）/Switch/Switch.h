/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												 
$										 
$                       	 		  
$								  		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      文件描述
	文件名: 	  Switch.h
	功能描述： 开关量输入输出
	编译环境： Keil for ARM + ARTX
	目标CPU:   LPC2000
	作者：	  
	创建时间： 
	建议编辑环境：UltraEdit 11.00a+  制表符宽度为三个字符，程序可读性更好。
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/					  

#ifndef        __SWITCH_H__
#define			__SWITCH_H__

#ifdef      	SWITCH_GLOBALS
#define     	SWITCH_EXT
#else
#define     	SWITCH_EXT    extern
#endif

#include			"..\Config\RTOS.h"



#define			__SWITCH_DEBUG__		1

/// 16个IO定义
#define			swUNLOCK_DOOR			(1<<0)										// 开门检测(SP1.0)
#define			swLOCK_DOOR				(1<<1)										// 锁门检测(SP1.1)
#define			swDOOR2OPEN 			(1<<2)										// 后尾箱
#define			swSHAKE					(1<<3)										// 震动检测
#define			swBRAKE					(1<<4)										// 刹车检测(SP1.2)
#define			swRSV1_6				(1<<5)										// 
#define			swRSV1_7				(1<<6)										// 
#define			swRSV1_8				(1<<7)										// 

#define			swRSV2_1				(1<<8)									   // 保留
#define			swRSV2_2				(1<<9)									
#define			swRSV2_3				(1<<10)										
#define			swRSV2_4				(1<<11)										
#define			swRSV2_5				(1<<12)
#define			swRSV2_6				(1<<13)
#define			swRSV2_7				(1<<14)										
#define			swRSV2_8				(1<<15)

////////////// 开关量输出控制、输入检测、ADC检测数据结构 /////////////////////////

#define			CHECK_PREQUENCY		5
#define			LEN_SW_SEND				10
#define			LEN_SW_REC				48
#define			LEN_SW_AND_ADC			58

#pragma		pack(1)
typedef			struct		_SW_AD_RT_IIC
{
	struct
	{
		uchar		Comm;
		uchar		Comm2;
		uint16	Frequency[4];
	} Send;
	struct
	{
		uchar		Comm;
		uchar		Comm2;
		uint16	Frequency[4];					// Comm,Comm2,Frequency原样返回
		uint16	SPD[CHECK_PREQUENCY];
		uint32	PulsPerMin;						// 
		uint16	PulsPerSec;
		uchar		SW[CHECK_PREQUENCY];	
		uchar		Rsv;
		uint16	ADC[8];
	} Rec;
} SW_AD_TR_IIC,*pSW_AD_TR_IIC;

SWITCH_EXT		SW_AD_TR_IIC	SwADC;


#pragma		pack(2)
static		struct
{
	uint16		ACC;
	uint16		T1;
	uint16		T2;
	uint16		P1;
	uint16		P2;
	uint16		MainPower;
	uint16		PowerLow;
	uint16		Rsv2;
}volatile	 	AlarmCnt;
#pragma		pack(1)

// 10ms加1，故需乘以10*10
#define		ALARM_DELAY_TIME		(5*10*10L)




//////////// Switch类 /////////////////
#pragma		pack(1)
class			SwitchCls
{
public:
	SwitchCls(void);
	void			Init(void);
	void			eTask(void);
	uint16		ReadIOState(void);
	
	void			CheckSwitchAlarm(uint16  SwStatus);
	void			CheckMotoStop(void);
	void			CheckMotoRunOrNot(void);
	void			CheckParkOverTimeAlarm();
	//void			CheckSpeedLimitAlarm();
	void			CheckRobAlarm();
	void			CheckStealAlarm();
	void			CheckTrailCarAlarm();
	void			CheckNoSetScanAlarm();
	void			CheckHelpAlarm();
//	void			CheckLowVoltageAlarm();
	void			CheckTireDriveAlarm();
	void			CheckIllegalOpenDoor();
	void			CheckIllegalMakeFire();
	void			CheckMachineSelfTest();
	void			CheckShakeAlarm();
	void			CheckLoginAlarm();
	void			CheckMainPower();
	void			CheckRadioControl(uchar sleep);
	void			CheckUnlockAlarm();
	void			CheckHandleWatchCar();
	void			PowerSaveMode();
	void			WakeUp(void);
	void			AutoLockDoor();
	void			SaveVehicleStatus();
	void			GetVehicleStatus();
	#if(USE_TH_GPRS_DATA_PACK_PROTOCOL)	
	void			TH_VehicleControlOutput(void);
	#endif
	
	////////////////////////////////////
	void			OpenSPK();
	void			CloseSPK();
	void			OpenAlarmLight();
	void			CloseAlarmLight();
private:
	#pragma		pack(1)
	uint32		volatile		MotoRotateSpeedCnt;
	uint32		volatile		TickCnt;	
	//uchar			SwBuf[13+5][3];
	struct
	{
		uint32	Door;
		uint32	Acc;
		uint32	Auto;
	} SwCnt;	
	//uchar			SP_Status,SG1_Status;//,SG2_Status;		// 定义两组IO口的状态（SP、SG1、SG2），其中SG2只有低6位有效
	uint16		volatile		IO_Status;							// 16位的IO状态,方便使用.(高8位为SG1口状态,低8位为SP口状态)		盗警
	uchar			volatile		ACC_Status,MOTO_Status,DOOR_Status,Button_Status,MainPower_Status, StealAlarmStatus, CrashAlarmStatus;
	uint16		TrailRadius;
};


SWITCH_EXT	__noinit__		SwitchCls		Switch;

#endif

/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */
