/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												 
$										 
$                       	 		  
$								  		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      文件描述
	文件名: 	 RTC.pcc
	功能描述：
	编译环境：Keil for ARM + RTX
	目标CPU:  LPC2000
	作者：
	创建时间：
	建议编辑环境：UltraEdit 11.00a+  制表符宽度为三个字符，程序可读性更好。
	注：LPC2000系列的RTC寄存器是16进制格式，PCF8563的RTC寄存器是BCD格式
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

//////////////////////////////////////////////////// 不考虑2100年以后
#define			TOTAL_YEAR			100
#define			START_YEAR			2000
#define			END_YEAR				(START_YEAR+TOTAL_YEAR)

#define			ONE_DAY_SECOND			(24*60*60L)

//////// 平年总天数，润年要加1
#define			ONE_YEAR_DAY			365

//////// 平年总秒数，润年要加24*60*60
#define			ONE_YEAR_SECOND		(365*24*60*60L)

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	平年中，每个月的天数.月是从1开始
*/
signed char const DayOfMonth[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	从START_YEAR开始，总计TOTAL_YEAR，每年的润年标志，润年为1，否则为0
	(也可用1字节代表8年)
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
	函数名称：DateTimeValid
   功能描述：判断日期、时间的合法性
   入口参数：RTC结构指针	          
   出口参数：正确或错误代码
   全局变量: 无
   修改时间：
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
	函数名称：DateTimeToSecond
   功能描述：把日期和时间转换为秒(2000年1月1日0时0分0秒时刻为0秒)
   入口参数：RTC结构指针	          
   出口参数：秒
   全局变量: 无
   修改时间：
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
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
	//wYear = pRTC->Year;					 //这样不更好
	/////////////////////////////////// 计算出已过去的整年总共秒数
	for(i=0,ySec=0;i<wYear;i++)
	{
		ySec += ONE_YEAR_SECOND;
		if(LeapYearFlag[i])
			ySec += ONE_DAY_SECOND;
	}
	/////////////////////////////////// 计算出当年已过去的整天数
	LeapYear = LeapYearFlag[wYear];
	Day = DayOfYear[pRTC->Month-1];
	Day += pRTC->Day;
	Day -= 1;
	if((LeapYear!=0)&&(pRTC->Month>2))
		Day += 1;
	/////////////////////////////////// 计算出当天已过去的总秒数
	Sec = (pRTC->Hour)*3600L + (pRTC->Min)*60L + pRTC->Sec;
	Sec += ySec;
	Sec += (Day*ONE_DAY_SECOND);
	/////////////////////////////////// 
   return Sec;
}

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：BDC_DateTimeToSecond
   功能描述：把BCD格式的日期和时间转换为秒(2000年1月1日0时0分0秒时刻为0秒)
   入口参数：RTC结构指针	          
   出口参数：秒
   全局变量: 无
   修改时间：
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
	函数名称：SecondToDateTime_BCD
   功能描述：以压缩BCD格式读取RTC
   入口参数：
   出口参数：
   全局变量: 
   创建时间：
   修改时间：
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
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
	函数名称：SecondToDateTime
   功能描述：把秒(2000年1月1日0时0分0秒时刻为0秒)转换为年月日时分秒
   入口参数：秒，RTC结构指针	          
   出口参数：成功标志，成功为OK，不成功为ERROR
   全局变量: 无
   修改时间：
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
uchar			RtcCls::SecondToDateTime(uint32	Second, RTC_STR  *pRTC)
{
	uchar		i,Month,Day;
   int16  	LeapYear,wYear;
   uint32 	Sec; 	
 	/////////////////////////////////// 计算出已过去的整年数
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
	/////////////////////////////////// 计算出当年已过去的整月数
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
	/////////////////////////////////// 计算出当月已过去的整天数
	pRTC->Day = Second/ONE_DAY_SECOND;
	pRTC->Day += 1;
	/////////////////////////////////// 计算出当天已过去的整小时数
	Second %= ONE_DAY_SECOND;
	pRTC->Hour = Second/3600L;
	/////////////////////////////////// 计算出分钟、秒
	Second %= 3600L;
	pRTC->Min = Second/60;
	pRTC->Sec = Second%60;
	/////////////////////////////////// 星期
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
	函数名称：DecManySec
   功能描述：时间减m秒
   入口参数：要减的秒数，RTC结构指针	          
   出口参数：成功标志OK
   全局变量: 无
   修改时间：
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
uchar			RtcCls::DecManySec(uint32	mSecond, RTC_STR  *pRTC)
{

	uint32  SecondTime;
   
	SecondTime = DateTimeToSecond(pRTC);

	SecondTime -=  mSecond;

	SecondToDateTime(SecondTime, pRTC);

	return OK;
}





/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：SetRTC
   功能描述：设置实时时钟
   入口参数：时钟结构体
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
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
	函数名称：SetRTC_BCD
   功能描述：以压缩BCD形式设置RTC
   入口参数：时钟结构体
   出口参数：
   全局变量: 
   创建时间：
   修改时间：
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
#include		"..\Common\Algol.h"
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
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
	函数名称：SetRTC_DT;
   功能描述：设置实时时钟
   入口参数：时钟结构体
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
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
	SetRTC(pRtc);									// 设置MCU内部时钟RTC

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
	SetRtcR8025(pRtc);							// 设备外部时钟芯片RTC

	NOP();	NOP();
}	// End of <SetRTC> function










/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：GetRTC
   功能描述：
   入口参数：
   出口参数：
   全局变量: 
   创建时间：
   修改时间：
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
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
	函数名称：GetRTC_BCD
   功能描述：以压缩BCD格式读取RTC
   入口参数：
   出口参数：
   全局变量: 
   创建时间：
   修改时间：
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
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
	函数名称：RTC_FormatToBCD
   功能描述：RTC格式（不含星期）时间转为BCD码
   入口参数：
   出口参数：
   全局变量: 
   创建时间：
   修改时间：
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
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
	函数名称：GetNowDateTimeString
   功能描述：以YYYY-MM-DD HH:MM:SS  格式读取时间字符串
   入口参数：取冒号标志，指针
   出口参数：
   全局变量: 
   创建时间：
   修改时间：
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
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
	函数名称：GetNowTime
   功能描述：读取时间
   入口参数：
   出口参数：
   全局变量: 
   创建时间：
   修改时间：
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void			RtcCls::GetNowTime(uchar Bcd,uchar *p)
{
	//////////////////////////////////////////////	时间字符串，以'\0'结束
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
	函数名称：GetNowTimeString
   功能描述：以HH:MM:SS格式读取时间字符串
   入口参数：取冒号标志，指针
   出口参数：
   全局变量: 
   创建时间：
   修改时间：
*/ 

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void			RtcCls::GetNowTimeString(uchar Colon,RTC_STR  *pDT,uchar *p)
{
	uchar		i = 0;
	//////////////////////////////////////////////	时间字符串，以'\0'结束
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
	函数名称：GetLocalTimeString
   功能描述：以HH:MM:SS格式读取时间字符串
   入口参数：取冒号标志，指针
   出口参数：
   全局变量: 
   创建时间：
   修改时间：
*/ 

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void			RtcCls::GetLocalTimeString(uchar Colon,uchar *p)
{
	uchar		i = 0;
	//////////////////////////////////////////////	时间字符串，以'\0'结束
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
		gCnt.Timer[t]++;		// 临时使用的定时器，可在需要的时候清零
	}		
	if (PhoneCall.ListeningFlag == TRUE)		RetryTimeForListen++;
	if (GG_Status.Flag.Bits.Online == 1)		SystemLog.GPRS.OnLineTime++;		// 在线总时间
	if (GG_Status.Flag.Bits.SignalOK == 0)		SystemLog.GSM.EbbSignalTime++;	// 信号弱总时间
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
	函数名称：RtcCls::Interrupt
   功能描述：RTC中断服务，时间进1秒
   入口参数：无
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
*/ 	
extern		SystemCls	Sys;


// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
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
		if(!DT.Sec)		// 每分钟进行一次润年运算
		{
			if(((t%4==0)&&(t%100!=0))||(t%400==0))
				DT.Leap = 1;
			else
				DT.Leap = 0;
		}	
		////////////////////////////////////////////////////////////需要加减的计数器
		CounterAddSub();

	}
	if(s&0x02)
	{
		LPC_RTC->ILR = 0x02;			
	}		
	NOP();   NOP();
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：SystemCls::RtcInit
   功能描述：初始化RTC
   入口参数：无
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
extern	"C"	void		RTC_IRQHandler(void) 		__irq
{
	Rtc.Interrupt();
	//VICVectAddr = 0;
}



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：SystemCls::RtcInit
   功能描述：初始化RTC
   入口参数：无
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
bool			RtcCls::Init(uchar	Clock)
{	
	uint16	t;
	LPC_RTC->CCR = 1;					//打开时钟校准
	LPC_RTC->CIIR = 0x01;			// 每秒中断一次
	LPC_RTC->AMR = 0xff;				// 
	
	#if ( USE_R8025_RTC )
	ReadR8025((RTC_STR *)NULL,(uchar *)NULL);
	t = 0;
	if ((Rtc8025.Month == 0x00) && (Rtc8025.Day == 0x00))  // 读出的日期有误,可能I2C忙，再读一次
	{	
		ReadR8025((RTC_STR *)NULL,(uchar *)NULL);
		if ((Rtc8025.Month == 0x00) && (Rtc8025.Day == 0x00))	t = 1;
		else 																	t = 0;
	}
	if (t==0)  
	{
		SetRTC_BCD((RTC_STR *)&Rtc8025);				//如果日期正确，重启后用时钟芯片初始化RTC
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


	LPC_RTC->CCR  = 0x01;         // 启动RTC

	return OK;
}











/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：
   功能描述：检测RTC是否正常工作，本函数只能供外部任务调用，不能在RTC内部调用
   入口参数：无
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
*/ 	

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void				RtcCls::CheckWorkStatus(void)		// 每秒调用1次
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
	函数名称：
   功能描述：
   入口参数：无
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
*/ 	
#include			"..\IIC\IIC0.h"
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
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
	pR->Year		= pR->Year;			// 年(单字节)
	pR->Month		= pR->Month & 0x1f;		// 月
	pR->Day			= pR->Day & 0x3f;			// 日
	pR->Hour		= pR->Hour & 0x3f ;			// 时
	pR->Min			= pR->Min & 0x7f;			// 分
	pR->Sec			= pR->Sec & 0x7f;			// 秒
	pR->Week		= pR->Week & 0x03;			// 星期	
	
	if(pData != NULL)
	{
		memmove(pData,&R8025_Buffer[1],16);
	}
	
	pR = (pRTC_REG8025)&R8025_Buffer[1];	
	Rtc8025.Year		= pR->Year;			// 年(单字节)
	Rtc8025.Month		= pR->Month;		// 月
	Rtc8025.Day			= pR->Day;			// 日
	Rtc8025.Hour		= pR->Hour;			// 时
	Rtc8025.Min			= pR->Min;			// 分
	Rtc8025.Sec			= pR->Sec;			// 秒
	Rtc8025.wYear		= pR->Year+2000;	// 年(双字节)		
	//Rtc8025.yDay		= pR->yDay;			// 一年中的第N天
	Rtc8025.Week		= pR->Week;			// 星期		
	
	
	
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
	函数名称：
   功能描述：
   入口参数：无
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
46 01 00  00  00 01 40   00 00 00 00 10 00 00 00 50           00    
*/ 	

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void			RtcCls::SetRtcR8025(RTC_STR  *pRTC)
{
	#if ( USE_R8025_RTC )
	pRTC_REG8025	pR;
	SetBufferValue(R8025_Buffer,17,0);
	Rtc8025.Year		= pRTC->Year;			// 年(单字节)
	Rtc8025.Month		= pRTC->Month;			// 月
	Rtc8025.Day			= pRTC->Day;			// 日
	Rtc8025.Hour		= pRTC->Hour;			// 时
	Rtc8025.Min			= pRTC->Min;			// 分
	Rtc8025.Sec			= pRTC->Sec;			// 秒
	Rtc8025.wYear		= pRTC->Year+2000;	// 年(双字节)		
	//Rtc8025.yDay		= pRTC->yDay;			// 一年中的第N天
	Rtc8025.Week		= pRTC->Week;			// 星期		
		
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

	
	
   R8025_Buffer[0] = 0;	// 写开始地址
   I2C0.Write(RTC_DEVICE_ADDR_R8025,0,17,R8025_Buffer);
   os_dly_wait(5);

	#endif
	
}




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：
   功能描述：
   入口参数：无
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
46 01 00  00  00 01 40   00 00 00 00 10 00 00 00 50           00    
*/ 	

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
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



