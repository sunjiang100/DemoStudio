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



//////////////////////////////////////////////////// ������2100���Ժ�
#define			TOTAL_YEAR				100
#define			START_YEAR				2000
#define			END_YEAR					(START_YEAR+TOTAL_YEAR)

#define			ONE_DAY_SECOND			(24*60*60L)

//////// ƽ��������������Ҫ��1
#define			ONE_YEAR_DAY			365

//////// ƽ��������������Ҫ��24*60*60
#define			ONE_YEAR_SECOND		(365*24*60*60L)



#pragma		pack(1)

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	����ʱ�����ݽṹ����
*/
#pragma		pack(1)
typedef		struct	
{	
	unsigned char		Year;		// ��(���ֽ�)
	unsigned char		Month;	// ��
	unsigned char		Day;		// ��
	unsigned char		Hour;		// ʱ
	unsigned char		Min;		// ��
	unsigned char		Sec;		// ��
	unsigned short	wYear;	// ��(˫�ֽ�)		
	unsigned short   yDay;		// һ���еĵ�N��
	unsigned char		Week;		// ����
	unsigned char		Leap;		// �����־
	unsigned long  RtcSecond;	// ��������2000��1��1��00:00:00Ϊ��0��
} RTC_STR,*pRTC_STR;


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	R8025�Ĵ���
*/
#pragma		pack(1)
typedef		struct	
{	
	unsigned char		Sec;					// ��
	unsigned char		Min;					// ��
	unsigned char		Hour;					// ʱ
	unsigned char		Week;					// week
	unsigned char		Day;					// ��
	unsigned char		Month;				// ��
	unsigned char		Year;					// ��
	unsigned char   	DigitalOffset;		// һ���еĵ�N��
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
	RTC��
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

