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

#ifndef   	__RTC_H__
#define   	__RTC_H__

#include    "..\Config\UserConfig.h"


#ifdef      RTC_GLOBALS
#define     RTC_EXT
#else
#define     RTC_EXT    extern
#endif



#define			CLKEN				0
#define			CTCRST			1
#define			CTTEST0			2
#define			CTTEST1			3
#define			CLKSRC			4



//////////////////////////////////////////////////// 不考虑2100年以后
#define			TOTAL_YEAR				100
#define			START_YEAR				2000
#define			END_YEAR					(START_YEAR+TOTAL_YEAR)

#define			ONE_DAY_SECOND			(24*60*60L)

//////// 平年总天数，润年要加1
#define			ONE_YEAR_DAY			365

//////// 平年总秒数，润年要加24*60*60
#define			ONE_YEAR_SECOND		(365*24*60*60L)



#pragma		pack(1)

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	日期时间数据结构定义
*/
#pragma		pack(1)
typedef		struct	
{	
	unsigned char		Year;		// 年(单字节)
	unsigned char		Month;	// 月
	unsigned char		Day;		// 日
	unsigned char		Hour;		// 时
	unsigned char		Min;		// 分
	unsigned char		Sec;		// 秒
	unsigned short	wYear;	// 年(双字节)		
	unsigned short   yDay;		// 一年中的第N天
	unsigned char		Week;		// 星期
	unsigned char		Leap;		// 润年标志
	unsigned long  RtcSecond;	// 总秒数，2000年1月1日00:00:00为第0秒
} RTC_STR,*pRTC_STR;


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	R8025寄存器
*/
#pragma		pack(1)
typedef		struct	
{	
	unsigned char		Sec;					// 秒
	unsigned char		Min;					// 分
	unsigned char		Hour;					// 时
	unsigned char		Week;					// week
	unsigned char		Day;					// 日
	unsigned char		Month;				// 月
	unsigned char		Year;					// 年
	unsigned char   	DigitalOffset;		// 一年中的第N天
	unsigned char   	Alarm_wMin;			// alarm mininter of week
	unsigned char   	Alarm_wHour;		//   
	unsigned char   	Alarm_wDay;			//  
	unsigned char   	Alarm_dMin;			// alarm miniter of day   
	unsigned char   	Alarm_dHour;		//   
	unsigned char   	Rsv;					//   
	unsigned char   	Ctrl1;
	unsigned char   	Ctrl2;	
} RTC_REG8025,*pRTC_REG8025;



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	RTC类
*/
class			RtcCls
{
	//////////////////////////////////////////	
public:
	unsigned long	Second;
	RTC_STR		DT, Rtc8025;
public:
	RtcCls(void);
	bool			Init(uchar);	
	__inline		void		Interrupt(void);
	///////////////////////////////////////////
	void			GetNowTimeString(uchar Colon,RTC_STR  *pDT,uchar *p);
	void			GetLocalTimeString(uchar Colon,uchar *p);
	uchar			DateTimeValid(RTC_STR  *pRTC);
	
	void			SetRTC_DT(uint32 Sec);

	void			SetRTC(RTC_STR   *pRTC);
	void			GetRTC(RTC_STR   *pRTC);
	void			SetRTC_BCD(RTC_STR   *pRTC);
	void			GetRTC_BCD(RTC_STR   *pRTC);
	uint32		GetRtcSecond(void);							
	void			GetRTC0183Date(char *p);
	void			CounterAddSub(void);
	void			GetNowTime(uchar Bcd,uchar *p);
	
	uint32		DateTimeToSecond(RTC_STR  *);
	uchar			SecondToDateTime(uint32	Second, RTC_STR  *pRTC);
	
	void			GetNowDateTimeString(uchar Separator,RTC_STR  *pDT,uchar *p);
	
	void			GetTimeString(uchar Colon,uchar *p,RTC_STR  *pRTC);
	void			GetDateTimeString(uchar Separator,uchar *p,RTC_STR  *pRTC);
	void			CheckWorkStatus(void);

	uchar			DecManySec(uint32	mSecond, RTC_STR  *pRTC);

	uint32      BDC_DateTimeToSecond(RTC_STR  *);

	void       SecondToDateTime_BCD(uint32	Second, RTC_STR  *pRTC);

	void			ReadR8025(RTC_STR   *pRTC,uchar *pData);
	void			SetRtcR8025(RTC_STR   *pRTC);
	void			WriteR8025(uchar Addr,uchar Len,uchar *pData);
	unsigned int	KeilDateTimeToSecond(char *pDate,char *pTime);
	uchar			RTC_FormatToBCD(RTC_STR  *pRTC, uchar *pBCD);
	
private:
	uint32		volatile		RtcStopCnt,RtcClockSource;	
	uchar			R8025_Buffer[18];	
};


RTC_EXT	__noinit__		RtcCls		Rtc;		//



#endif    // 
/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */

