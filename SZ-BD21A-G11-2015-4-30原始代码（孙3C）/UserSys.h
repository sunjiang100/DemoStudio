/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												 
$										 
$                       	 		  
$								  		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      文件描述
	文件名: 	 main.cpp
	功能描述：C++主函数，系统初始化
	编译环境：Keil for ARM + RTL
	目标CPU:  LPC2000
	作者：
	创建时间：
	建议编辑环境：UltraEdit 11.00a+  制表符宽度为三个字符，程序可读性更好。
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/	

#ifndef		__MAIN_H__
#define		__MAIN_H__


#ifdef      MAIN_GLOBALS
#define     MAIN_EXT
#else
#define     MAIN_EXT    extern
#endif


//#include		"..\Config\RTOS.h"		__inline

class			SystemCls
{
public:
	SystemCls(void);
	__inline		void		Init(void);
			void		LoadInitValue(void);
	void		FirstPowerOn(unsigned char  Format);
	void		TaskMonitor(void);
	void		InitWatchDog(unsigned int  Clock);
	void		ResetWatchDog(void);
	void		Delay_uS(unsigned short t);
	void		Delay_mS(unsigned short  c);
	void		ReadID_IP(void);
//	void		PowerControl(void);
	void		TestFunction(void);
	unsigned char		CheckTaskRunStatus(void);
	void		GetGYFlagToUart(void);
	void		GetVersion(char *p);
	void		CheckResetSource(void);
	void		IdleMode(void);
	void     	SetFirstPara(void);
	void		PowerControl(unsigned char   Index,unsigned char   Power);
	void		GetGB_Version(char *p);
	void		GetHS_Version(char *p, unsigned char Type);
	void     	OemTest(void);
////////////////////////////////
public:
	unsigned long		volatile		SystemInitFlag;

		
	//unsigned long		volatile		SecondCnt;		
};


MAIN_EXT		SystemCls	Sys;



#endif

/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */
