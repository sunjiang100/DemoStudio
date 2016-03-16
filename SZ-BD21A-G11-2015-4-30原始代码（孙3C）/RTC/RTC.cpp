/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												 
$										 
$                       	 		  
$								  		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      �ļ�����
	�ļ���: 	 RTC.pcc
	����������
	���뻷����Keil for ARM + RTX
	Ŀ��CPU:  LPC2000
	���ߣ�
	����ʱ�䣺
	����༭������UltraEdit 11.00a+  �Ʊ�����Ϊ�����ַ�������ɶ��Ը��á�
	ע��LPC2000ϵ�е�RTC�Ĵ�����16���Ƹ�ʽ��PCF8563��RTC�Ĵ�����BCD��ʽ
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/					  

#define			RTC_GLOBALS		1
#include     	"RTC.h"
#include			"..\Config\RTOS.h"
#include			"..\UserSys.h"
#include			"..\common\Algol.H"
#include			"..\UART\UART0.h"
#include			"..\GPS\GPS.h"
#include	"..\Record\RecordModel.h"
#include			"..\GSM_GPRS\GSM_GPRS.h"


#define			USE_R8025_RTC				0

extern			uint32	RetryTimeForListen;

//////////////////////////////////////////////////// ������2100���Ժ�
#define			TOTAL_YEAR			100
#define			START_YEAR			2000
#define			END_YEAR				(START_YEAR+TOTAL_YEAR)

#define			ONE_DAY_SECOND			(24*60*60L)

//////// ƽ��������������Ҫ��1
#define			ONE_YEAR_DAY			365

//////// ƽ��������������Ҫ��24*60*60
#define			ONE_YEAR_SECOND		(365*24*60*60L)

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	ƽ���У�ÿ���µ�����.���Ǵ�1��ʼ
*/
signed char const DayOfMonth[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	��START_YEAR��ʼ���ܼ�TOTAL_YEAR��ÿ��������־������Ϊ1������Ϊ0
	(Ҳ����1�ֽڴ���8��)
*/
unsigned char const LeapYearFlag[100] = 
{
	1,0,0,0,	1,0,0,0,	1,0,0,0,	1,0,0,0,	1,0,0,0,
	1,0,0,0,	1,0,0,0,	1,0,0,0,	1,0,0,0,	1,0,0,0,
	1,0,0,0,	1,0,0,0,	1,0,0,0,	1,0,0,0,	1,0,0,0,
	1,0,0,0,	1,0,0,0,	1,0,0,0,	1,0,0,0,	1,0,0,0,
	1,0,0,0,	1,0,0,0,	1,0,0,0,	1,0,0,0,	1,0,0,0,
};

#define			LEAP_YEAR_NUM			25
#define			TOTAL_DAY				36525L
#define			TOTAL_YEAR_SECOND		0xbc191380



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  	
*/  
signed short const DayOfYear[13] = { 0,31,59,90,120,151,181,212,243,273,304,334,365  };  

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�DateTimeValid
   �����������ж����ڡ�ʱ��ĺϷ���
   ��ڲ�����RTC�ṹָ��	          
   ���ڲ�������ȷ��������
   ȫ�ֱ���: ��
   �޸�ʱ�䣺
*/ 
uchar				RtcCls::DateTimeValid(RTC_STR  *pRTC)
{
	uchar		Valid = 0;	
	if( (pRTC->Year>99) )									Valid = 1;
	if( (pRTC->Month<1)||(pRTC->Month>12) )			Valid = 2;
	if( (pRTC->Day<1)||(pRTC->Day>31) )					Valid = 3;
	if( (pRTC->Hour>23) )									Valid = 4;
	if( (pRTC->Min>59) )										Valid = 5;
	if( (pRTC->Sec>59) )										Valid = 6;
	if( (pRTC->wYear<START_YEAR)||(pRTC->wYear>END_YEAR) )		
		Valid = 7;
	return	Valid;	
}

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�DateTimeToSecond
   ���������������ں�ʱ��ת��Ϊ��(2000��1��1��0ʱ0��0��ʱ��Ϊ0��)
   ��ڲ�����RTC�ṹָ��	          
   ���ڲ�������
   ȫ�ֱ���: ��
   �޸�ʱ�䣺
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
uint32			RtcCls::DateTimeToSecond(RTC_STR  *pRTC)
{
	uchar		i;
   int16  	LeapYear,wYear;
   uint32 	Day,Sec,ySec;
	RTC_STR  RTC;

	RTC = *pRTC;

	pRTC = &RTC;
 	//////////////////////////////////////////
  	NOP();	NOP();
   if(((pRTC->wYear)<START_YEAR)||((pRTC->wYear)>END_YEAR))
 	{ 		
 		pRTC->wYear = pRTC->Year;
 		pRTC->wYear += START_YEAR;
 	}
 	i = DateTimeValid(pRTC);
 	if(i!=0)		return 0 ;		

	wYear = pRTC->wYear;
   wYear -= START_YEAR;	
	//wYear = pRTC->Year;					 //����������
	/////////////////////////////////// ������ѹ�ȥ�������ܹ�����
	for(i=0,ySec=0;i<wYear;i++)
	{
		ySec += ONE_YEAR_SECOND;
		if(LeapYearFlag[i])
			ySec += ONE_DAY_SECOND;
	}
	/////////////////////////////////// ����������ѹ�ȥ��������
	LeapYear = LeapYearFlag[wYear];
	Day = DayOfYear[pRTC->Month-1];
	Day += pRTC->Day;
	Day -= 1;
	if((LeapYear!=0)&&(pRTC->Month>2))
		Day += 1;
	/////////////////////////////////// ����������ѹ�ȥ��������
	Sec = (pRTC->Hour)*3600L + (pRTC->Min)*60L + pRTC->Sec;
	Sec += ySec;
	Sec += (Day*ONE_DAY_SECOND);
	/////////////////////////////////// 
   return Sec;
}

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�BDC_DateTimeToSecond
   ������������BCD��ʽ�����ں�ʱ��ת��Ϊ��(2000��1��1��0ʱ0��0��ʱ��Ϊ0��)
   ��ڲ�����RTC�ṹָ��	          
   ���ڲ�������
   ȫ�ֱ���: ��
   �޸�ʱ�䣺
*/ 
uint32			RtcCls::BDC_DateTimeToSecond(RTC_STR  *pRTC)
{
	uint16	Year = 2000;
	RTC_STR Rt;

	Year += BcdToHex(pRTC->Year);
   Rt.wYear  = Year;
	Rt.Year  = BcdToHex(pRTC->Year);
   Rt.Month = BcdToHex(pRTC->Month);
   Rt.Hour	= BcdToHex(pRTC->Hour);		 
   Rt.Day   = BcdToHex(pRTC->Day);
   Rt.Min	= BcdToHex(pRTC->Min);
   Rt.Sec	= BcdToHex(pRTC->Sec);


  return DateTimeToSecond(&Rt);
}



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�SecondToDateTime_BCD
   ������������ѹ��BCD��ʽ��ȡRTC
   ��ڲ�����
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void			RtcCls::SecondToDateTime_BCD(uint32	Second, RTC_STR  *pRTC)
{	 
	/*SecondToDateTime(Second, (RTC_STR  *)pRTC);
	pRTC->Year  = HexToBcd(pRTC->Year);
	pRTC->Month = HexToBcd(pRTC->Month);
	pRTC->Day   = HexToBcd(pRTC->Day);
	pRTC->Hour  = HexToBcd(pRTC->Hour);
	pRTC->Min   = HexToBcd(pRTC->Min);
	pRTC->Sec   = HexToBcd(pRTC->Sec);
	pRTC->Week  = HexToBcd(pRTC->Week);	 */

	RTC_STR  Rt;

	Rt = *pRTC;
	SecondToDateTime(Second, &Rt);
	pRTC->Year  = HexToBcd(Rt.Year);
	pRTC->Month = HexToBcd(Rt.Month);
	pRTC->Day   = HexToBcd(Rt.Day);
	pRTC->Hour  = HexToBcd(Rt.Hour);
	pRTC->Min   = HexToBcd(Rt.Min);
	pRTC->Sec   = HexToBcd(Rt.Sec);
	pRTC->Week  = HexToBcd(Rt.Week);



}	// End of <GetRTC> function


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�SecondToDateTime
   ��������������(2000��1��1��0ʱ0��0��ʱ��Ϊ0��)ת��Ϊ������ʱ����
   ��ڲ������룬RTC�ṹָ��	          
   ���ڲ������ɹ���־���ɹ�ΪOK�����ɹ�ΪERROR
   ȫ�ֱ���: ��
   �޸�ʱ�䣺
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
uchar			RtcCls::SecondToDateTime(uint32	Second, RTC_STR  *pRTC)
{
	uchar		i,Month,Day;
   int16  	LeapYear,wYear;
   uint32 	Sec; 	
 	/////////////////////////////////// ������ѹ�ȥ��������
	if(Second>TOTAL_YEAR_SECOND)
		return ERROR;
	for(i=0,wYear=0;i<TOTAL_YEAR;i++)
	{
		Sec = ONE_YEAR_SECOND;
		if(LeapYearFlag[i])
			Sec += ONE_DAY_SECOND;
		if(Second<Sec)	break;
		wYear++;
		Second -= Sec;
	}
	pRTC->Year = wYear;
	pRTC->wYear = wYear+START_YEAR;
	/////////////////////////////////// ����������ѹ�ȥ��������
	LeapYear = LeapYearFlag[wYear];
	for(i=0,Month=1;i<12;i++)
	{
		Sec = DayOfMonth[i];
		Sec *= ONE_DAY_SECOND;
		if((LeapYear!=0)&&(Month==2))
			Sec += ONE_DAY_SECOND;
		if(Second<Sec)	break;
		Month++;
		Second -= Sec;
	}
	pRTC->Month = Month;
	/////////////////////////////////// ����������ѹ�ȥ��������
	pRTC->Day = Second/ONE_DAY_SECOND;
	pRTC->Day += 1;
	/////////////////////////////////// ����������ѹ�ȥ����Сʱ��
	Second %= ONE_DAY_SECOND;
	pRTC->Hour = Second/3600L;
	/////////////////////////////////// ��������ӡ���
	Second %= 3600L;
	pRTC->Min = Second/60;
	pRTC->Sec = Second%60;
	/////////////////////////////////// ����
	wYear = pRTC->wYear;
	Month = pRTC->Month;
	Day   = pRTC->Day;
	if((Month==1)||(Month==2))
	{
  		Month += 12;
  		wYear--;
	}
	pRTC->Week = ((Day+2*Month+3*(Month+1)/5+wYear+wYear/4-wYear/100+wYear/400)+1)%7;
	return OK;
}			


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�DecManySec
   ����������ʱ���m��
   ��ڲ�����Ҫ����������RTC�ṹָ��	          
   ���ڲ������ɹ���־OK
   ȫ�ֱ���: ��
   �޸�ʱ�䣺
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
uchar			RtcCls::DecManySec(uint32	mSecond, RTC_STR  *pRTC)
{

	uint32  SecondTime;
   
	SecondTime = DateTimeToSecond(pRTC);

	SecondTime -=  mSecond;

	SecondToDateTime(SecondTime, pRTC);

	return OK;
}





/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�SetRTC
   ��������������ʵʱʱ��
   ��ڲ�����ʱ�ӽṹ��
   ���ڲ�������
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void			RtcCls::SetRTC(RTC_STR   *pRTC)
{
	uint16	Year = 2000;
	Year += pRTC->Year;
	LPC_RTC->YEAR  = Year;
	LPC_RTC->MONTH = pRTC->Month;
	LPC_RTC->DOM   = pRTC->Day;
	LPC_RTC->HOUR	= pRTC->Hour;
	LPC_RTC->MIN	= pRTC->Min;
	LPC_RTC->SEC	= pRTC->Sec;

}	// End of <SetRTC> function


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�SetRTC_BCD
   ������������ѹ��BCD��ʽ����RTC
   ��ڲ�����ʱ�ӽṹ��
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
#include		"..\Common\Algol.h"
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void			RtcCls::SetRTC_BCD(RTC_STR   *pRTC)
{
	uint16 t;
	uint16	Year = 2000;

	Year += BcdToHex(pRTC->Year);
	LPC_RTC->YEAR  = Year;
	LPC_RTC->MONTH = BcdToHex(pRTC->Month);
	LPC_RTC->HOUR	= BcdToHex(pRTC->Hour);		 
	LPC_RTC->DOM   = BcdToHex(pRTC->Day);
	LPC_RTC->MIN	= BcdToHex(pRTC->Min);
	LPC_RTC->SEC	= BcdToHex(pRTC->Sec);


	t = LPC_RTC->YEAR;
	t &= 0xfff;				 // 0 - 4095
	DT.wYear     = t;
	DT.Year		 = t%100;

	t = 	LPC_RTC->MONTH;
	if ((t>12) || (t ==0))
	{	t = 1;			  }
	DT.Month     = t;

	t =  LPC_RTC->DOM;
	if ((t>31) || (t==0))
	{	t = 1;			}
	DT.Day		 = t;
	DT.Hour		 = LPC_RTC->HOUR % 24;
	DT.Min		 = LPC_RTC->MIN % 60;
	DT.Sec		 = LPC_RTC->SEC % 60;
	DT.Week		 = LPC_RTC->DOW&0x03;		
	DT.RtcSecond = GetRtcSecond();
	SystemLog.GG1.RtcSecond = 	DT.RtcSecond;

	gCnt.RtcSecond = DT.RtcSecond;



}	// End of <SetRTC> function




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�SetRTC_DT;
   ��������������ʵʱʱ��
   ��ڲ�����ʱ�ӽṹ��
   ���ڲ�������
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void			RtcCls::SetRTC_DT(uint32 Sec)
{
	uchar		t;
	pRTC_STR		pRtc;
	RTC_STR		RtcBCD;
	NOP();	NOP();
	pRtc = (pRTC_STR)(uchar*)&DT;
	SecondToDateTime(Sec,pRtc);				// 
	DT.RtcSecond = Sec;
	gCnt.RtcSecond = Sec;
	gCnt.R8025_Second = Sec;
	SetRTC(pRtc);									// ����MCU�ڲ�ʱ��RTC

	RtcBCD.Year  = HexToBcd(DT.Year);
	RtcBCD.Month = HexToBcd(DT.Month);
	RtcBCD.Day   = HexToBcd(DT.Day);
	RtcBCD.Hour  = HexToBcd(DT.Hour);
	RtcBCD.Min   = HexToBcd(DT.Min);
	RtcBCD.Sec   = HexToBcd(DT.Sec);
	if((DT.Month==1)||(DT.Month==2))
	{
  		DT.Month += 12;
  		DT.Year--;
	}
	t = ((DT.Day+2*DT.Month+3*(DT.Month+1)/5+DT.Year+DT.Year/4-DT.Year/100+DT.Year/400)+1)%7;	
	RtcBCD.Week  = HexToBcd(t);

	pRtc = (pRTC_STR)(uchar*)&RtcBCD;
	SetRtcR8025(pRtc);							// �豸�ⲿʱ��оƬRTC

	NOP();	NOP();
}	// End of <SetRTC> function










/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�GetRTC
   ����������
   ��ڲ�����
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void			RtcCls::GetRTC(RTC_STR  *pRTC)
{
	
	pRTC->Year   = DT.Year;
	pRTC->Month  = DT.Month;
	pRTC->Day    = DT.Day;
	pRTC->Hour   = DT.Hour;
	pRTC->Min    = DT.Min;
	pRTC->Sec    = DT.Sec;
	pRTC->wYear  = DT.wYear;
	pRTC->yDay   = DT.yDay;
	pRTC->Week   = DT.Week;
	pRTC->RtcSecond = DateTimeToSecond( (RTC_STR*)&DT );
}	// End of <GetRTC> function



uint32			RtcCls::GetRtcSecond(void)
{
	pRTC_STR		pRtc;	
	pRtc = (pRTC_STR)(uchar*)&DT;
	return DateTimeToSecond(pRtc);
	
}



//$GPRMC,092052.00,A,2308.35444,N,11322.08014,E,43.106,282.54,151009,,,A*64
// *XY,1018001003,V1,014445,A,2308.3552,N,11322.0915,E,0,,140610,FEFFFFEF,0#<0,0,22,0,0,0,14>
void			RtcCls::GetRTC0183Date(char *p)
{
	NOP();	NOP();	
	*(p+0) = DT.Day/10 + '0';
	*(p+1) = DT.Day%10 + '0';
	*(p+2) = DT.Month/10 + '0';
	*(p+3) = DT.Month%10 + '0';
	*(p+4) = DT.Year/10 + '0';
	*(p+5) = DT.Year%10 + '0';	
}



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�GetRTC_BCD
   ������������ѹ��BCD��ʽ��ȡRTC
   ��ڲ�����
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void			RtcCls::GetRTC_BCD(RTC_STR  *pRTC)
{
	pRTC->Year  = HexToBcd(DT.Year);
	pRTC->Month = HexToBcd(DT.Month);
	pRTC->Day   = HexToBcd(DT.Day);
	pRTC->Hour  = HexToBcd(DT.Hour);
	pRTC->Min   = HexToBcd(DT.Min);
	pRTC->Sec   = HexToBcd(DT.Sec);
	pRTC->Week  = HexToBcd(DT.Week);
}	// End of <GetRTC> function


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�RTC_FormatToBCD
   ����������RTC��ʽ���������ڣ�ʱ��תΪBCD��
   ��ڲ�����
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
uchar			RtcCls::RTC_FormatToBCD(RTC_STR  *pRTC, uchar *pBCD)
{
	if ((!pRTC) || (!pBCD))		return ERROR;

	pBCD[0] = HexToBcd(pRTC->Year);  
	pBCD[1] = HexToBcd(pRTC->Month); 
	pBCD[2] = HexToBcd(pRTC->Day); 
	pBCD[3] = HexToBcd(pRTC->Hour);  
	pBCD[4] = HexToBcd(pRTC->Min); 
	pBCD[5] = HexToBcd(pRTC->Sec); 
	
	return 0;  
}	// End of <GetRTC> function

		  



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�GetNowDateTimeString
   ������������YYYY-MM-DD HH:MM:SS  ��ʽ��ȡʱ���ַ���
   ��ڲ�����ȡð�ű�־��ָ��
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void			RtcCls::GetNowDateTimeString(uchar Separator,RTC_STR  *pDT,uchar *p)
{
	uchar		i = 4;
	///////////////////////////////////////////////////////
	Int16ToDecChar(pDT->wYear,(char*)p);
	if(Separator)
	{	*(p+i) = '-';		i++;		}
	*(p+i) = pDT->Month/10+'0';		i++;
	*(p+i) = pDT->Month%10+'0';		i++;
	if(Separator)
	{	*(p+i) = '-';		i++;		}
	*(p+i) = pDT->Day/10+'0';		i++;
	*(p+i) = pDT->Day%10+'0';		i++;
	if(Separator)
	{	*(p+i) = ' ';		i++;		}
	GetNowTimeString(Separator,pDT,(p+i));
}



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�GetNowTime
   ������������ȡʱ��
   ��ڲ�����
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void			RtcCls::GetNowTime(uchar Bcd,uchar *p)
{
	//////////////////////////////////////////////	ʱ���ַ�������'\0'����
	if(Bcd)
	{
		*(p+0) = HexToBcd(DT.Hour);		
		*(p+1) = HexToBcd(DT.Min);
		*(p+2) = HexToBcd(DT.Sec);
	}
	else
	{
		*(p+0) = DT.Hour;
		*(p+1) = DT.Min;
		*(p+2) = DT.Sec;		
	}
}





/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�GetNowTimeString
   ������������HH:MM:SS��ʽ��ȡʱ���ַ���
   ��ڲ�����ȡð�ű�־��ָ��
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void			RtcCls::GetNowTimeString(uchar Colon,RTC_STR  *pDT,uchar *p)
{
	uchar		i = 0;
	//////////////////////////////////////////////	ʱ���ַ�������'\0'����
	*(p+i) = pDT->Hour/10+'0';		i++;
	*(p+i) = pDT->Hour%10+'0';		i++;
	if(Colon)
	{	*(p+i) = ':';	i++;	}
	*(p+i) = pDT->Min/10+'0';  		i++;
	*(p+i) = pDT->Min%10+'0';		i++;
	if(Colon)
	{	*(p+i) = ':';	i++;	}	
	*(p+i) = pDT->Sec/10+'0';		i++;
	*(p+i) = pDT->Sec%10+'0';		i++;
}





/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�GetLocalTimeString
   ������������HH:MM:SS��ʽ��ȡʱ���ַ���
   ��ڲ�����ȡð�ű�־��ָ��
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void			RtcCls::GetLocalTimeString(uchar Colon,uchar *p)
{
	uchar		i = 0;
	//////////////////////////////////////////////	ʱ���ַ�������'\0'����
	*(p+i) = DT.Hour/10+'0';		i++;
	*(p+i) = DT.Hour%10+'0';		i++;
	if(Colon)
	{	*(p+i) = ':';	i++;	}
	*(p+i) = DT.Min/10+'0';  		i++;
	*(p+i) = DT.Min%10+'0';		i++;
	if(Colon)
	{	*(p+i) = ':';	i++;	}	
	*(p+i) = DT.Sec/10+'0';		i++;
	*(p+i) = DT.Sec%10+'0';		i++;
}


void			RtcCls::CounterAddSub(void)
{
	uchar		t;
	NOP();	NOP();
	GPS.RtcPlusPlus();
	gCnt.SystemSecond++;	
	gCnt.RtcSecond++;
	gCnt.GetDataSecond++;
	DT.RtcSecond++;
	for(t=0;t<TIMER_NUMBER;t++)
	{
		gCnt.Timer[t]++;		// ��ʱʹ�õĶ�ʱ����������Ҫ��ʱ������
	}		
	if (PhoneCall.ListeningFlag == TRUE)		RetryTimeForListen++;
	if (GG_Status.Flag.Bits.Online == 1)		SystemLog.GPRS.OnLineTime++;		// ������ʱ��
	if (GG_Status.Flag.Bits.SignalOK == 0)		SystemLog.GSM.EbbSignalTime++;	// �ź�����ʱ��
	if(JT_ShutMobileCnt)								JT_ShutMobileCnt++;
	if(SendInterval.T_ContinuedTime) 			SendInterval.T_ContinuedTime--;

	SystemLog.GG1.RtcSecond++;
	SystemSecondCntLast = TRUE;
	Sys.TaskMonitor();
	GSM.SecondCounter();	
	Record.CounterFun();
   ////////////////////////////////////////////////////////////////////////// 
}



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�RtcCls::Interrupt
   ����������RTC�жϷ���ʱ���1��
   ��ڲ�������
   ���ڲ�������
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 	
extern		SystemCls	Sys;


// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
__inline		void		RtcCls::Interrupt(void)
{
	uchar    s;	
	uint32	t;
	NOP();	NOP();
	NOP();	NOP();
	s = LPC_RTC->ILR;
	if(s&0x01)
	{
		LPC_RTC->ILR = 0x01;	
		RtcStopCnt = 0;
		isr_evt_set(0x0001,TaskID.Speed);
		InRtcIntFlag = TRUE;	

//		t = LPC_RTC->YEAR;
//		t &= 0xfff;				 // 0 - 4095
//		DT.wYear     = t;
//		DT.Year		 = t%100;
//		DT.Month     = LPC_RTC->MONTH&0x1f;
//		DT.Day		 = LPC_RTC->DOM&0x3f;
//		DT.Hour		 = LPC_RTC->HOUR&0x3f;
//		DT.Min		 = LPC_RTC->MIN&0x7f;
//		DT.Sec		 = LPC_RTC->SEC&0x7f;
		t = LPC_RTC->YEAR;
		t &= 0xfff;				 // 0 - 4095
		DT.wYear     = t;
		DT.Year		 = t%100;
	
		t = 	LPC_RTC->MONTH;
		if ((t>12) || (t ==0))
		{	t = 1;			  }
		DT.Month     = t;
	
		t =  LPC_RTC->DOM;
		if ((t>31) || (t==0))
		{	t = 1;			}
		DT.Day		 = t;
		DT.Hour		 = LPC_RTC->HOUR % 24;
		DT.Min		 = LPC_RTC->MIN % 60;
		DT.Sec		 = LPC_RTC->SEC % 60;
		DT.Week		 = LPC_RTC->DOW&0x03;		
		/////////////////////////////////////////////		
		if(!DT.Sec)		// ÿ���ӽ���һ����������
		{
			if(((t%4==0)&&(t%100!=0))||(t%400==0))
				DT.Leap = 1;
			else
				DT.Leap = 0;
		}	
		////////////////////////////////////////////////////////////��Ҫ�Ӽ��ļ�����
		CounterAddSub();

	}
	if(s&0x02)
	{
		LPC_RTC->ILR = 0x02;			
	}		
	NOP();   NOP();
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�SystemCls::RtcInit
   ������������ʼ��RTC
   ��ڲ�������
   ���ڲ�������
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
extern	"C"	void		RTC_IRQHandler(void) 		__irq
{
	Rtc.Interrupt();
	//VICVectAddr = 0;
}



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�SystemCls::RtcInit
   ������������ʼ��RTC
   ��ڲ�������
   ���ڲ�������
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
bool			RtcCls::Init(uchar	Clock)
{	
	uint16	t;
	LPC_RTC->CCR = 1;					//��ʱ��У׼
	LPC_RTC->CIIR = 0x01;			// ÿ���ж�һ��
	LPC_RTC->AMR = 0xff;				// 
	
	#if ( USE_R8025_RTC )
	ReadR8025((RTC_STR *)NULL,(uchar *)NULL);
	t = 0;
	if ((Rtc8025.Month == 0x00) && (Rtc8025.Day == 0x00))  // ��������������,����I2Cæ���ٶ�һ��
	{	
		ReadR8025((RTC_STR *)NULL,(uchar *)NULL);
		if ((Rtc8025.Month == 0x00) && (Rtc8025.Day == 0x00))	t = 1;
		else 																	t = 0;
	}
	if (t==0)  
	{
		SetRTC_BCD((RTC_STR *)&Rtc8025);				//���������ȷ����������ʱ��оƬ��ʼ��RTC
	}
	#endif


	t = LPC_RTC->YEAR;
	t &= 0xfff;				 // 0 - 4095
	DT.wYear     = t;
	DT.Year		 = t%100;

	t = 	LPC_RTC->MONTH;
	if ((t>12) || (t ==0))
	{	t = 1;			  }
	DT.Month     = t;

	t =  LPC_RTC->DOM;
	if ((t>31) || (t==0))
	{	t = 1;			}
	DT.Day		 = t;
	DT.Hour		 = LPC_RTC->HOUR % 24;
	DT.Min		 = LPC_RTC->MIN % 60;
	DT.Sec		 = LPC_RTC->SEC % 60;
	DT.Week		 = LPC_RTC->DOW&0x03;		
	DT.RtcSecond = GetRtcSecond();
	SystemLog.GG1.RtcSecond = 	DT.RtcSecond;

	NVIC_SetPriority(RTC_IRQn, RTC_INT_PRIORITY);     
   NVIC_EnableIRQ(RTC_IRQn); 


	LPC_RTC->CCR  = 0x01;         // ����RTC

	return OK;
}











/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�
   �������������RTC�Ƿ�����������������ֻ�ܹ��ⲿ������ã�������RTC�ڲ�����
   ��ڲ�������
   ���ڲ�������
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 	

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void				RtcCls::CheckWorkStatus(void)		// ÿ�����1��
{
	
	//////////////////////////////////////////////////////////////////////
	if(++RtcStopCnt>2)
	{
		InRtcIntFlag = 2;					// 		
		CounterAddSub();
			
			
		////////////////////////////////////////////////
	}		
}














/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�
   ����������
   ��ڲ�������
   ���ڲ�������
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 	
#include			"..\IIC\IIC0.h"
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void			RtcCls::ReadR8025(RTC_STR  *pRTC,uchar *pData)
{

	#if ( USE_R8025_RTC )
	pRTC_REG8025	pR;
	RTC_STR		Rtc1;
	//uchar   R8025_Buffer1[20];
	SetBufferValue(R8025_Buffer,17,0);
	I2C0.Read(RTC_DEVICE_ADDR_R8025,0,17,R8025_Buffer);
	os_dly_wait(2);

	pR = (pRTC_REG8025)&R8025_Buffer[1];	
	pR->Year		= pR->Year;			// ��(���ֽ�)
	pR->Month		= pR->Month & 0x1f;		// ��
	pR->Day			= pR->Day & 0x3f;			// ��
	pR->Hour		= pR->Hour & 0x3f ;			// ʱ
	pR->Min			= pR->Min & 0x7f;			// ��
	pR->Sec			= pR->Sec & 0x7f;			// ��
	pR->Week		= pR->Week & 0x03;			// ����	
	
	if(pData != NULL)
	{
		memmove(pData,&R8025_Buffer[1],16);
	}
	
	pR = (pRTC_REG8025)&R8025_Buffer[1];	
	Rtc8025.Year		= pR->Year;			// ��(���ֽ�)
	Rtc8025.Month		= pR->Month;		// ��
	Rtc8025.Day			= pR->Day;			// ��
	Rtc8025.Hour		= pR->Hour;			// ʱ
	Rtc8025.Min			= pR->Min;			// ��
	Rtc8025.Sec			= pR->Sec;			// ��
	Rtc8025.wYear		= pR->Year+2000;	// ��(˫�ֽ�)		
	//Rtc8025.yDay		= pR->yDay;			// һ���еĵ�N��
	Rtc8025.Week		= pR->Week;			// ����		
	
	
	
	if(pRTC != NULL)
	{
		pRTC->Year   = Rtc8025.Year;
		pRTC->Month  = Rtc8025.Month;
		pRTC->Day    = Rtc8025.Day;
		pRTC->Hour   = Rtc8025.Hour;
		pRTC->Min    = Rtc8025.Min;
		pRTC->Sec    = Rtc8025.Sec;
		pRTC->wYear  = Rtc8025.wYear;
		//pRTC->yDay   = Rtc8025.yDay;
		pRTC->Week   = Rtc8025.Week;
	}   
	
	Rtc1.Year = BcdToHex(Rtc8025.Year);
	Rtc1.Month = BcdToHex(Rtc8025.Month);
	Rtc1.Day = BcdToHex(Rtc8025.Day);
	Rtc1.Hour = BcdToHex(Rtc8025.Hour);
	Rtc1.Min = BcdToHex(Rtc8025.Min);
	Rtc1.Sec = BcdToHex(Rtc8025.Sec);
	
	gCnt.R8025_Second = DateTimeToSecond((RTC_STR*)&Rtc1);	
	
	#endif
	NOP();	NOP();             
}	





/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�
   ����������
   ��ڲ�������
   ���ڲ�������
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
46 01 00  00  00 01 40   00 00 00 00 10 00 00 00 50           00    
*/ 	

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void			RtcCls::SetRtcR8025(RTC_STR  *pRTC)
{
	#if ( USE_R8025_RTC )
	pRTC_REG8025	pR;
	SetBufferValue(R8025_Buffer,17,0);
	Rtc8025.Year		= pRTC->Year;			// ��(���ֽ�)
	Rtc8025.Month		= pRTC->Month;			// ��
	Rtc8025.Day			= pRTC->Day;			// ��
	Rtc8025.Hour		= pRTC->Hour;			// ʱ
	Rtc8025.Min			= pRTC->Min;			// ��
	Rtc8025.Sec			= pRTC->Sec;			// ��
	Rtc8025.wYear		= pRTC->Year+2000;	// ��(˫�ֽ�)		
	//Rtc8025.yDay		= pRTC->yDay;			// һ���еĵ�N��
	Rtc8025.Week		= pRTC->Week;			// ����		
		
	pR = (pRTC_REG8025)&R8025_Buffer[1];
	pR->Sec					= Rtc8025.Sec;
	pR->Min              = Rtc8025.Min;
	pR->Hour             = Rtc8025.Hour;
	pR->Week             = Rtc8025.Week;
	pR->Day              = Rtc8025.Day;
	pR->Month            = Rtc8025.Month;
	pR->Year             = Rtc8025.Year;

	pR->Ctrl1				= 0x22;
	pR->Ctrl2				= 0x20;

	
	
   R8025_Buffer[0] = 0;	// д��ʼ��ַ
   I2C0.Write(RTC_DEVICE_ADDR_R8025,0,17,R8025_Buffer);
   os_dly_wait(5);

	#endif
	
}




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�
   ����������
   ��ڲ�������
   ���ڲ�������
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
46 01 00  00  00 01 40   00 00 00 00 10 00 00 00 50           00    
*/ 	

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void			RtcCls::WriteR8025(uchar Addr,uchar Len,uchar *pData)
{

	#if ( USE_R8025_RTC )
	
	if((Addr==13)||(Addr>15)||(Len>15))		return;
	R8025_Buffer[0] = (Addr<<4)&0xf0;
	memmove(R8025_Buffer,pData,Len);
	I2C0.Write(RTC_DEVICE_ADDR_R8025,0,Len+1,R8025_Buffer);	
	#endif
}




RtcCls::RtcCls(void)
{
	
}


// ( 2012-03-10  16:11:21)
/*
January February 	March 		April 		May 		June
July 	August 		September 	October 	November 	December
Mar  3 2012
16:26:18

*/

unsigned int		RtcCls::KeilDateTimeToSecond(char *pDate,char *pTime)
{
	RTC_STR		Rtc;
	uchar 	t1,t2;
	uint32		Second;
	Rtc.wYear = Str2Int((char*)(pDate+7));
	Rtc.Year = Rtc.wYear%100;
	if(strstr(pDate,"Jan")!=NULL)
		Rtc.Month = 1;
	else if(strstr(pDate,"Jan")!=NULL)		Rtc.Month = 1;
	else if(strstr(pDate,"Feb")!=NULL)		Rtc.Month = 2;
	else if(strstr(pDate,"Mar")!=NULL)		Rtc.Month = 3;
	else if(strstr(pDate,"Apr")!=NULL)		Rtc.Month = 4;
	else if(strstr(pDate,"May")!=NULL)		Rtc.Month = 5;
	else if(strstr(pDate,"Jun")!=NULL)		Rtc.Month = 6;
	else if(strstr(pDate,"Jul")!=NULL)		Rtc.Month = 7;
	else if(strstr(pDate,"Aug")!=NULL)		Rtc.Month = 8;
	else if(strstr(pDate,"Sep")!=NULL)		Rtc.Month = 9;
	else if(strstr(pDate,"Oct")!=NULL)		Rtc.Month = 10;
	else if(strstr(pDate,"Nov")!=NULL)		Rtc.Month = 12;
	else if(strstr(pDate,"Dec")!=NULL)		Rtc.Month = 12;
	else									Rtc.Month = 12;  
	t1 = *(pDate+4);
	t2 = *(pDate+5);
	Rtc.Day = t2-'0';
	if(t1 == '1')
		Rtc.Day += 10;
	else if(t1 == '2')
		Rtc.Day += 20;
	else if(t1 == '3')
		Rtc.Day += 30;
	///////////////////////////////////////
	Rtc.Hour = Str2Int(pTime);
	Rtc.Min = Str2Int(pTime+3);
	Rtc.Sec = Str2Int(pTime+6);
	
	Second = DateTimeToSecond(&Rtc);
	
	return Second;
}











/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */



