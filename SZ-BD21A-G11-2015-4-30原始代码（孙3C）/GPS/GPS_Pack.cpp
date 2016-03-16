/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												 
$									GuangZhou Guoyee Electronic S&T Co.,Ltd.
$                       	 		 Email: autoele@qq.com
$								  		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      文件描述
	文件名:   GPS_Pack.cpp
	功能描述：把GPS原始数据打包成各种格式
	编译环境：
	目标CPU:  
	作者：
	创建时间：
	建议编辑环境：UltraEdit 11.00a+  制表符宽度为三个字符，程序可读性更好。
$GPRMC,022805.00,A,2813.43869,N,11256.72015,E,0.631,63.73,280409,,,A*59\r\n\0
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/	
#include			"..\Config\RTOS.h"
#include			"GPS.h"
#include			"..\UART\Uart2.h"
#include			"..\UART\Uart0.h"
//#include			"..\IIC\IIC0.h"
#include			"..\GSM_GPRS\GSM_GPRS.h"
#include			"..\Common\FLASH_ADDR.H"
#include			<stdlib.h>
#include			<string.h>
#include			<math.h>

#include			"..\FLASH\DFE_25xxx.H"
#include			"..\FLASH\DFF_25xxx.H"





/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
  函数名称：GPS_GY_MMM_ToString
  功能描述：把MMM格式的GPS数据转换成RMC格式的数据
  入口参数: 读取的数据缓冲指针
  出口参数：无
  全局变量: 无
  创建时间：2007-7-7 11:24
  修改时间：
  注：转换的源数据和目的数据使用类内变量，无需入口指针。
*/ 	
uint16		PreGPS_Speed[4]={0,0,0,0};
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void			GPSCls::GY_DataPack(uchar *pDst)
{
	uchar		tBuffer[48];
	uchar			i,j,t;
	char			*pSpeed,ucaSpeed[8];
	pRTC_STR		pRtc;
	pGPS_DMS		pDms;
	uint32		tl;


	while(GY_PackBusing)	{ os_dly_wait(1);	}
	GY_PackBusing = TRUE;
	if( (TD.GPS==tTASK_RUN_FLAG)&&(TD.Value<3) ) {	GY_PackBusing = 0;return;	}
	if(pDst != NULL)
	{
		memmove(pDst,(uchar*)&GYPack,LEN_GY_GPS_PACK);
		GY_PackBusing = 0;
		return;
	}
	if( (TD.GPS==tTASK_RUN_FLAG)&&(TD.Value<4) ) {	GY_PackBusing = 0;return;	}
	//////////////////////////////////////////////////////////////////////////转换
	NOP();	NOP();
	if(GPSValidFlag!=GPS_VALID_FLAG)
	{
		GYPack.Latitude = LastGYPack.Latitude;//0xffffffff;//-2000000000L;//0x8affffff;
		GYPack.Longitude = LastGYPack.Longitude;//0xffffffff;//-2000000000L;//0x8affffff;
		
		LatDms.MMM = 0x7FFFFFFF;
		LonDms.MMM = 0x7FFFFFFF;

				
		pRtc = (pRTC_STR)(uchar*)&gRtc;
		Rtc.SecondToDateTime(GYPack.RtcSecond,pRtc);
		gCnt.GPS_RtcSecond = GYPack.RtcSecond;
		tl = gCnt.GPS_RtcSecond;
		tl += (8*60*60L);			// 本地时间，需加8小时
		pRtc = (pRTC_STR)(uchar*)&gtlRtc;
		//Rtc.SecondToDateTime(tl,pRtc);
		Rtc.GetRTC(pRtc);				// 不定位，取本地时间
		GPS_LocalTimeString();
		GY_PackBusing = 0;
		GPS_Speed = 0;

		GYPack.Speed = 0;			// 单位：0.1km/h
		uiGPS_Speed = 0;		
		if(gRtc.Sec==59)
		{
			AvgSpeed1Min = SumOfSpeed1Min/60;		// 单位：0.1km/h
			SumOfSpeed1Min = 0;
		}

		return;
	}
	if( (TD.GPS==tTASK_RUN_FLAG)&&(TD.Value<5) ) {	GY_PackBusing = 0;return;	}
	/////////////////////////////////////////////////////////////////////////	处理日期时间
	gRtc.Year  = DecCharToUint32(2,&GPS_String[ (PS[PDATE]+4) ] );		// 年
	gRtc.Month = DecCharToUint32(2,&GPS_String[ (PS[PDATE]+2) ] );		// 月
	gRtc.Day   = DecCharToUint32(2,&GPS_String[ (PS[PDATE]+0) ] );		// 日		
	gRtc.Hour  = DecCharToUint32(2,&GPS_String[ (PS[PTIME]+0) ] );			// 时
	gRtc.Min   = DecCharToUint32(2,&GPS_String[ (PS[PTIME]+2) ] );			// 分
	gRtc.Sec   = DecCharToUint32(2,&GPS_String[ (PS[PTIME]+4) ] );			// 秒
	t = GPS_String[ (PS[PTIME]+7) ];			// 秒的小数，002724.000
	gRtc.wYear = gRtc.Year+2000;
	pRtc = (pRTC_STR)(uchar*)&gRtc;
	GYPack.RtcSecond = Rtc.DateTimeToSecond(pRtc);
	gCnt.GPS_RtcSecond = GYPack.RtcSecond;
	tl = gCnt.GPS_RtcSecond;
	tl += (8*60*60L);					// 本地时间，需加8小时
	if( (t=='9')&&(GYPack.RtcSecond==LastGYPack.RtcSecond))
	{
		tl++;			// 修正时间
		GYPack.RtcSecond++;
		Uart.puts("卫星时间递增0.9秒，修正为1秒 ");
	}
	pRtc = (pRTC_STR)(uchar*)&gtlRtc;
	Rtc.SecondToDateTime(tl,pRtc);
	GPS_LocalTimeString();
	
	/////////////////////////////////////////////////////////////////////////  处理纬度
	if( (TD.GPS==tTASK_RUN_FLAG)&&(TD.Value<6) ) {	GY_PackBusing = 0;return;	}
	
	if( GPS_String[PS[PLATD]] == 'N')
		LatDir = 1;
	else
		LatDir = -1;
	pDms = (pGPS_DMS)(uchar *)&LatDms;
	GYPack.Latitude = GPRMC_LonLatToDu32(LatDir,(&GPS_String[PS[PLAT]]),pDms);		//度(含7位小数)
	#if (WUXI_TEST != 1)
	if(GYPack.Latitude>900000000)		GYPack.Latitude = LastGYPack.Latitude;
	#endif
	SystemLog.GG1.Latitude = GYPack.Latitude;
			
	//////////////////////////////////////////////////////////////////////////  处理经度
	if( (TD.GPS==tTASK_RUN_FLAG)&&(TD.Value<7) ) {	GY_PackBusing = 0;return;	}
	if(GPS_String[PS[PLOND]] == 'E')
		LonDir = 1;
	else
		LonDir = -1;
	pDms = (pGPS_DMS)(uchar *)&LonDms;
	GYPack.Longitude = GPRMC_LonLatToDu32(LonDir,(&GPS_String[PS[PLON]]),pDms);	//度(含7位小数)
	#if (WUXI_TEST != 1)
	if(GYPack.Longitude>1800000000)	GYPack.Longitude = LastGYPack.Longitude;
	#endif
	SystemLog.GG1.Longitude = GYPack.Longitude;
	
	
	//////////////////////////////////////////////////////////////////////////  速度	
	if( (TD.GPS==tTASK_RUN_FLAG)&&(TD.Value<8) ) {	GY_PackBusing = 0;return;	}
	ucaSpeed[0] = ucaSpeed[1] = ucaSpeed[2] = 0;
	ucaSpeed[3] = ucaSpeed[4] = ucaSpeed[5] = 0;
	ucaSpeed[6] = ucaSpeed[7] = 0;
	pSpeed = (&GPS_String[PS[PSPEED]]);		// 速度字符串位置
	tl = My_atol(pSpeed);						// 速度的整数，海里
	tl *= 1852;
	GPS_Speed = tl/1000;	 
	SystemLog.GG1.Speed_Info &=  ~0xff;
	if (GPS_Speed > 255)
	{
		SystemLog.GG1.Speed_Info |= 255;
	}
	else
	{
		SystemLog.GG1.Speed_Info |= GPS_Speed;
	}

	//////////////////////////////////////////// 测试使用
	/*PreGPS_Speed[3] = PreGPS_Speed[2];	 	PreGPS_Speed[2] = PreGPS_Speed[1];		
	PreGPS_Speed[1] = PreGPS_Speed[0];	  	PreGPS_Speed[0] = GPS_Speed;
	if( (PreGPS_Speed[3]==PreGPS_Speed[2])&&(PreGPS_Speed[2]==0)\
			&&(PreGPS_Speed[1]==PreGPS_Speed[0])&&(PreGPS_Speed[0]==GPS_Speed)&&(GPS_Speed>5)	)
	{
		SetBufferValue(tBuffer,48,0);
		strcpy((char*)tBuffer,"\x47异常加速，速度突变，从零到");
		Int2Str( GPS_Speed, (char*)&tBuffer[27]);
		strcat((char*)tBuffer,"公里每小时");
		KeyDisp.SendDataToLCM_YH(eLCM_U_CENTER_TMP_MSG,strlen((char*)tBuffer),tBuffer);
	}
	if( (PreGPS_Speed[3]==PreGPS_Speed[2])&&(PreGPS_Speed[2]==PreGPS_Speed[1])&&(PreGPS_Speed[1]>5)\
			&&(PreGPS_Speed[0]==0)&&(GPS_Speed==0)	)
	{
		SetBufferValue(tBuffer,48,0);
		strcpy((char*)tBuffer,"\x47异常减速，速度突变，从");
		Int2Str( PreGPS_Speed[1], (char*)&tBuffer[23]);
		strcat((char*)tBuffer,"到零公里每小时");
		KeyDisp.SendDataToLCM_YH(eLCM_U_CENTER_TMP_MSG,strlen((char*)tBuffer),tBuffer);
	} */


	for(i=0,j=0;i<5;i++)							// 取速度的整数
	{
		t = *pSpeed;	pSpeed++;
		if( (t>='0')&&(t<='9') )	
			ucaSpeed[j++] = t;
		if(t=='.')	
			break;
	}
	//////////////////////////////  取速度的两位小数，没有则补零
	for(i=0;i<2;i++)
	{
		t = *pSpeed;	pSpeed++;
		if(t==',')	
			break;
		if( (t>='0')&&(t<='9') )	
			ucaSpeed[j++] = t;
		else
			ucaSpeed[j++] = '0';		
	}
	for(;i<2;i++)
	{
		ucaSpeed[j++] = '0';
	}			
	tl = My_atol( (char *)ucaSpeed);			// 2位小数
	tl *= 1852;										// 海里变公里
	tl /= 10000;
	if(tl>3800)		tl = LastGYPack.Speed;

	#if (WUXI_TEST ==1)
	if(TD.GPS==tGPS_EXT_PACK)			  // 模拟GPS速度
	{
		tl = 1000; //100km/h
	}
	#endif

	GYPack.Speed = tl;			// 单位：0.1km/h
	uiGPS_Speed = tl;
	SumOfSpeed1Min += tl;
	if(gRtc.Sec==59)
	{
		AvgSpeed1Min = SumOfSpeed1Min/60;		// 单位：0.1km/h
		SumOfSpeed1Min = 0;
	} 

	if( (TD.GPS==tTASK_RUN_FLAG)&&(TD.Value<9) ) {	GY_PackBusing = 0;return;	}
			
	//////////////////////////////////////////////////////////////////////////  方位角	
	GPS_Azimuth = atoi(&GPS_String[PS[PDIR]]);
	GYPack.Azimuth = GPS_Azimuth;				// 单位：2度		
	
	//////////////////////////////////////////////////////////////////////////  海拔
	tl = GYPack.Altitude = GPS_Altitude ;	

	SystemLog.GG1.Speed_Info &=  ~(0xffUL << 8);
	SystemLog.GG1.Speed_Info |=  tl << 8;
	
	//////////////////////////////////////////////////////////////////////////  有无效标志
	
	if( (TD.GPS==tTASK_RUN_FLAG)&&(TD.Value<10) ) {	GY_PackBusing = 0;return;	}
	GPS_Distance = 0;


	//GPS_Distance = Calculate2DotSphericalDistance(dbPreLon,dbPreLat,LonDms.dbD,LatDms.dbD);		 // MDK4.2 不支持double？？
	GPS_Distance = Calculate2DotDistance(PreLon,PreLat,GYPack.Longitude,GYPack.Latitude);

	if( (GPS_Speed>2)&&(DataUpDate==TRUE)&&(!GET_ACC_STATUS) )			// 工作，计算里程
	{
		//GPS_Distance = Calculate2DotSphericalDistance(dbPreLon,dbPreLat,LonDms.dbD,LatDms.dbD);
		SystemLog.GPS.TotalMileage += GPS_Distance;
		TestMileage.GPS += GPS_Distance;
		
	}

	if (TempGPSValidTime>3)		// 定位连续3秒即判断漂移
	{
		if (GPS_Distance >= 300)   //  每秒300米视为漂移
		{
			DriftingFlag = TRUE;
			Uart.puts("发生漂移");
		}
		else
		{
			DriftingFlag = FALSE;
		}
	}
	
	
	if( (TD.GPS==tTASK_RUN_FLAG)&&(TD.Value<11) ) {	GY_PackBusing = 0;return;	}
	tl = GYPack.Speed;
	if( (tl != 200)&&(tl>=100) )					// 
	{
		NOP();	NOP();		// MDK4.2 不支持double？？
		SystemLog.GPS.SpeedCaculus += tl;
	}
	if( (TD.GPS==tTASK_RUN_FLAG)&&(TD.Value<12) ) {	GY_PackBusing = 0;return;	}
	dbPreLon = LonDms.dbD;			 		 
	dbPreLat = LatDms.dbD;			  
	PreLon = GYPack.Longitude;
	PreLat = GYPack.Latitude;
	DataUpDate = TRUE;

	GY_PackBusing = 0;
} // End






/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
  函数名称：GB19056_DataPack
  功能描述：
  入口参数: 
  出口参数：无
  全局变量: 无
  创建时间：2011-8-14 18:26:52
  修改时间：
  注：
*/ 	

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void			GPSCls::GB19056_DataPack(uchar *pDst)
{
	union
	{
		uchar		Buffer[4];
		uint32	B32;
	} Tp;
	uint32	tl;
	
	NOP();	NOP();
	while(GB_PackBusing)	{ os_dly_wait(1);	}
	GB_PackBusing = TRUE;
	
	if(pDst != NULL)
	{
		memmove(pDst,(uchar*)&GBPack,LEN_GB19056_GPS_PACK);
		GB_PackBusing = 0;
		return;
	}
	///////////////////////////////////////////////// 纬度，经度，单位：0.0001分
	GBPack.Lat = SwapINT32(LatDms.MMM);
	GBPack.Lon = SwapINT32(LonDms.MMM);
	GBPack.Altitude = SwapINT16(GPS_Altitude);

	
	
	GB_PackBusing = 0;
} // End of <GB19056_DataPack>









/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
  函数名称：TH_DataPack
  功能描述：
  入口参数: 
  出口参数：无
  全局变量: 无
  创建时间：
  修改时间：
  注：
*/ 	

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void		GPSCls::TH_DataPack(char *pDst)
{		
	uchar		c;//,len	
	uint32	i,j,k;		
	
	NOP();	NOP();
	while(TH_PackBusing)	{ os_dly_wait(1);	}
	TH_PackBusing = TRUE;
	
	if(pDst != NULL)
	{
		strcpy(pDst,GPS_CharPack);
		TH_PackBusing = 0;
		return;
	}
	/////////////////////////////////////////////////////////////// 转换
	i = j = 0;		
	if(GPSValidFlag==GPS_VALID_FLAG)			// 有效数据
	{		
		SetBufferValue((uchar*)&spGPS,60,0);
		////////////////////////////////////////////	
		memmove((&GPS_CharPack[i]),(uchar *)(&GPS_String[PS[PTIME]]),6);		// 时间	
		memmove((uchar*)spGPS.Time,(uchar *)(&GPS_String[PS[PTIME]]),6);	// 时间	
		spGPS.Time[6] = 0;
		i += 6;
		GPS_CharPack[i] = ',';	i++;	
		c = GPS_String[PS[PVALID]];
		GPS_CharPack[i] = c;		i++;						// 有效标志
		GPS_CharPack[i] = ',';	i++;
			
		/////////////////////////////////////////// 纬度
		for(k=0;k<8;k++)
		{
			c = GPS_String[PS[PLAT]+k];
			if (c != '.')
			{
				GPS_CharPack[i] = c;	i++;
			}
			else
				break;
		}
		memmove( &GPS_CharPack[i], (uchar *)&GPS_String[PS[PLAT]+k],5);	// 小数部分
		i += 5;
		
		if(k==4)
			memmove((uchar*)spGPS.Lat,(uchar*)(&GPS_String[PS[PLAT]]),9);		// 
		else
		{
			spGPS.Lat[0] = '0';
			memmove((uchar*)&spGPS.Lat[1],(uchar*)(&GPS_String[PS[PLAT]]),8);		// 
		}
		
		//////////////////////////////////////
		GPS_CharPack[i] = ',';	i++;		
		GPS_CharPack[i] = GPS_String[PS[PLATD]];		i++;							// 南北纬标志
		GPS_CharPack[i] = ',';	i++;
   	
   	spGPS.LatDir = GPS_String[PS[PLATD]];
   	
		//////////////////////////////////////////////////// 经度
		for(k=0;k<8;k++)
		{
			c = GPS_String[PS[PLON]+k];
			if (c != '.')
			{
				GPS_CharPack[i] = c;	i++;
			}
			else
				break;
		}	
		memmove( &GPS_CharPack[i], (uchar *)(&GPS_String[PS[PLON]+k]),5);		// 经度小数部分	
		i += 5;
		if(k==5)
			memmove((uchar*)spGPS.Lon,(uchar*)(&GPS_String[PS[PLON]]),10);
		else if(k==4)
		{
			spGPS.Lon[0] = '0';
			memmove((uchar*)&spGPS.Lon[1],(uchar*)(&GPS_String[PS[PLON]]),9);
		}
		else
		{
			spGPS.Lon[0] = '0';
			spGPS.Lon[1] = '0';
			memmove((uchar*)&spGPS.Lon[2],(uchar*)(&GPS_String[PS[PLON]]),8);
		}
		
		/////////////////////////////////////////
		GPS_CharPack[i] = ',';	i++;
		////////////////////////////////////////
		GPS_CharPack[i] = GPS_String[PS[PLOND]];		i++;							// 东西经标志
		GPS_CharPack[i] = ',';	i++;
		
		spGPS.LonDir = GPS_String[PS[PLOND]];
		
		
		////////////////////////////////////////
		for(j=0;j<5;j++)															// 速度，长度不定，逗号或点号结束
		{
			c = GPS_String[PS[PSPEED]+j];
			if ((c != ',')&& (c != '.'))										// 逗号，判断结束
			{
				GPS_CharPack[i] = c; i++;		
			}
			else	
			{
				break;
			}
		}
		if(j==1)
		{
			spGPS.Speed[0] = '0';
			spGPS.Speed[1] = '0';
			spGPS.Speed[2] = GPS_String[PS[PSPEED]];
		}
		else if(j==2)
		{
			spGPS.Speed[0] = '0';
			spGPS.Speed[1] = GPS_String[PS[PSPEED]];
			spGPS.Speed[2] = GPS_String[PS[PSPEED]+1];
		}
		else if(j==3)
		{
			memmove((uchar*)spGPS.Speed,(&GPS_String[PS[PSPEED]]),3);	
		}
		strcpy(spGPS.SpeedDir,spGPS.Speed);
		
		GPS_CharPack[i] = ',';	i++;
		////////////////////////////////////////
		for(j=0;j<5;j++)															// 方向，长度不定，小数点结束
		{ 
			c = GPS_String[PS[PDIR]+j];
			if ((c != ',')&&(c != '.'))										// 小数点，判断结束
			{
				GPS_CharPack[i] = c; i++;		
			}
			else	
			{
				break;
			}
		}
		if(j==1)
		{
			spGPS.Azimuth[0] = '0';
			spGPS.Azimuth[1] = '0';
			spGPS.Azimuth[2] = GPS_String[PS[PDIR]];
		}
		else if(j==2)
		{
			spGPS.Azimuth[0] = '0';
			spGPS.Azimuth[1] = GPS_String[PS[PDIR]];
			spGPS.Azimuth[2] = GPS_String[PS[PDIR]+1];
		}
		else if(j==3)
		{
			memmove((uchar*)spGPS.Azimuth,(uchar*)(&GPS_String[PS[PDIR]]),3);
		}
		strcat(spGPS.SpeedDir,spGPS.Azimuth);		
		GPS_CharPack[i] = ',';	i++;	
		
		////////////////////////////////////////
		memmove(&GPS_CharPack[i],(uchar *)(&GPS_String[PS[PDATE]]),6);		// 日期
		
		memmove((uchar*)spGPS.Date,(uchar *)(&GPS_String[PS[PDATE]]),6);
		
		i += 6;
		GPS_CharPack[i] = ',';	i++;	
   	
   	
		///////////////////////	车辆状态	/////////////////////////////
		if(GPSValidFlag==GPS_VALID_FLAG)
		{
			VehicleStatus.VS4.Bits.GPS_ANT_Open = 0;
			VehicleStatus.VS4.Bits.GPS_ANT_Short = 0;
		}
	}
	else	// 不定位或无效数据
	{	// *XY,1018001003,V1,014445,A,2308.3552,N,11322.0915,E,0,,140610,FEFFFFEF,0#<0,0,22,0,0,0,14>
		GPS_CharPack[0] = gRtc.Hour/10 + '0';
		GPS_CharPack[1] = gRtc.Hour%10 + '0';
		GPS_CharPack[2] = gRtc.Min/10 + '0';
		GPS_CharPack[3] = gRtc.Min%10 + '0';
		GPS_CharPack[4] = gRtc.Sec/10 + '0';
		GPS_CharPack[5] = gRtc.Sec%10 + '0';
		strcat((char*)&GPS_CharPack[6],",V,0,0,0,0,0,0,\0");
		GPS_CharPack[21] = gRtc.Day/10 + '0';
		GPS_CharPack[22] = gRtc.Day%10 + '0';
		GPS_CharPack[23] = gRtc.Month/10 + '0';
		GPS_CharPack[24] = gRtc.Month%10 + '0';
		GPS_CharPack[25] = gRtc.Year/10 + '0';
		GPS_CharPack[26] = gRtc.Year%10 + '0';
		GPS_CharPack[27] = ',';
		i = 28;
	}

	ByteToHexChar((~VehicleStatus.VS1.Byte),(char*)&GPS_CharPack[i]);	i+= 2;
	ByteToHexChar((~VehicleStatus.VS2.Byte),(char*)&GPS_CharPack[i]);	i+= 2;
	ByteToHexChar((~VehicleStatus.VS3.Byte),(char*)&GPS_CharPack[i]);	i+= 2;
	ByteToHexChar((~VehicleStatus.VS4.Byte),(char*)&GPS_CharPack[i]);	i+= 2;		
	//DF1.Write(EE_VEHICLE_STATUS1,4,(uchar*)&VehicleStatus);	// 非铁电,存储频率不能太高		
	/////////////////////// 驶入电子围栏时,加入围栏序号//////////////////////////////////
	
	GPS_CharPack[i] = '#';	i++;			
	NOP();	NOP(); 		
	
	///////////////////////////////////////////////// 扩展字段
	//ExtData((char*)GPS_CharPack);

	TH_PackBusing = 0;
	
}// End of <CutGPS_GerneralInfo> function







/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：TH_BCD_Pack
   功能描述：
   入口参数：读取的数据缓冲指针
   出口参数：
   全局变量: 
   创建时间：
   修改时间：
	注：转换的源数据和目的数据使用类内变量，无需入口指针。
*/ 
//uchar			DecCharToPressBCD(uchar Len,char *pDec,char *pBCD);
//101635,A,2308.3559,N,11322.0850,E,0,0,091210,FEFFF1FE
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
//void			GPSCls::TH_BCD_Pack(uchar	*pDst)
//{
//	
//}







/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：TH_BCD_Pack
   功能描述：
   入口参数：读取的数据缓冲指针
   出口参数：
   全局变量: 
   创建时间：
   修改时间：
	注：转换的源数据和目的数据使用类内变量，无需入口指针。
*/ 
//uchar			DecCharToPressBCD(uchar Len,char *pDec,char *pBCD);
//101635,A,2308.3559,N,11322.0850,E,0,0,091210,FEFFF1FE
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
//void			GPSCls::LCM_BCD_Pack(uchar	*pDst)
//{
//	
//}











/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：
   功能描述：
   入口参数：
   出口参数：
   全局变量: 
   创建时间：
   修改时间： 
*/ 
//#include		"..\ADC\Adc.h"
//// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
//void			GPSCls::ExtData(char  *p)
//{
//	
//	//////////////////////////////////////////////////////////
//}





/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：
   功能描述：	部标
   入口参数：
   出口参数：
   全局变量: 
   创建时间：
   修改时间： 
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用

void		GPSCls::JT808_DataPack(uchar *pDst)
{	
	union
	{
		uchar 	Buffer[4];
		uint32	B32;
	} Tp;
	uchar		RandData;
	while(JT_PackBusing)	{ os_dly_wait(1);	}
	JT_PackBusing = TRUE;
	
	if(pDst!=NULL)
	{
		memmove(pDst,(uchar*)&JTPack,LEN_JT808_GPS_PACK);
		JT_PackBusing = 0;
		return;
	}
	
	if(VirtaulGPS_Valid == TRUE)	
		JT808Flag.VS1.Bits.GPS = 1;
	
	Tp.Buffer[0] = JT808Flag.VS8.Byte;
	Tp.Buffer[1] = JT808Flag.VS7.Byte;
	Tp.Buffer[2] = JT808Flag.VS6.Byte;
	Tp.Buffer[3] = JT808Flag.VS5.Byte;
	JTPack.Alarm = Tp.B32;
	Tp.Buffer[0] = JT808Flag.VS4.Byte;
	Tp.Buffer[1] = JT808Flag.VS3.Byte;
	Tp.Buffer[2] = JT808Flag.VS2.Byte;
	Tp.Buffer[3] = JT808Flag.VS1.Byte;
	JTPack.Status = Tp.B32;
	
	if(DriftingFlag==FALSE)
	{	// 不漂移才取最新位置信息
		JTPack.Latitude = SwapINT32( GYPack.Latitude/10 );
		JTPack.Longitude = SwapINT32( GYPack.Longitude/10 );
		if( (GYPack.Latitude/10000000UL)==34)
		{
			GPS_Altitude = 150;		// 实验室高度
			GPS_Altitude += (uchar)(gCnt.TickForRandData&0x0000000f);
		}
		if( (GPS_Altitude<30)||(GPS_Altitude>5000))
			GPS_Altitude = 50;
		JTPack.Speed = SwapINT16( GYPack.Speed );	
		JTPack.Altitude = SwapINT16(GPS_Altitude);
		JTPack.Azimuth = SwapINT16(GPS_Azimuth);
	}

	
	//////////////////////////////////////////////////////////////////// GPS日期	
	JTPack.Date[0] = HexToBcd(gtlRtc.Year);
	JTPack.Date[1] = HexToBcd(gtlRtc.Month);
	JTPack.Date[2] = HexToBcd(gtlRtc.Day);
	//////////////////////////////////////////////////////////////////// GPS时间					
	JTPack.Time[0] = HexToBcd(gtlRtc.Hour);
	JTPack.Time[1] = HexToBcd(gtlRtc.Min);
	JTPack.Time[2] = HexToBcd(gtlRtc.Sec);
	
	
	if(gCnt.TickForRandData&0x01)
	{
		JTPack.Latitude ^= gKey.Key;
		JTPack.Longitude ^= gKey.Key;
		JTPack.Alarm ^= gKey.Key;
	}


	JT_PackBusing = 0;				//  若少了这条语句，则GPS无法发送数据
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
需使用语句：GSV、GGA、RMC、TXT
双模，共809字符
$GPGGA,100900.000,3959.9999,N,11600.0000,E,1,22,0.6,48.7,M,0.0,M,,*59
$GPGLL,3959.9999,N,11600.0000,E,100900.000,A*3C
$GNGSA,A,3,14,16,20,23,29,30,25,31,32,24,02,,1.4,0.6,1.2*25
$GNGSA,A,3,201,202,204,205,208,206,203,211,209,210,212,,1.4,0.6,1.2*1A
$GPGSV,3,1,12,02,27,294,25,14,37,129,28,16,26,215,28,20,27,294,31*73
$GPGSV,3,2,12,23,14,312,29,24,57,222,24,25,24,046,31,27,43,259,*79
$GPGSV,3,3,12,29,28,080,31,30,57,222,31,31,68,012,31,32,43,259,31*77
$BDGSV,3,1,11,201,37,145,40,202,37,215,38,203,43,188,36,204,26,123,37*64
$BDGSV,3,2,11,205,17,114,40,206,37,221,38,208,56,300,37,209,30,059,38*63
$BDGSV,3,3,11,210,17,117,38,211,44,284,37,212,40,206,39*62
$GPRMC,100900.000,A,3959.9999,N,11600.0000,E,0.00,0.00,050313,,*0F
$GPVTG,0.00,T,,M,0.00,N,0.00,K*50
$GPZDA,100900.000,05,03,2013,00,00*58


单GPS，共606字节
$GPGGA,000121.000,3959.9995,N,11600.6567,E,1,15,0.7,59.7,M,0.0,M,,*58
$GPGLL,3959.9995,N,11600.6567,E,000121.000,A*38
$GPGSA,A,3,16,28,02,07,04,20,10,05,12,17,13,14,1.2,0.7,1.0*3F
$GPGSV,4,1,16,01,06,049,26,02,54,289,43,04,73,356,43,05,19,213,43*7F
$GPGSV,4,2,16,06,19,176,23,07,21,105,25,10,64,189,43,12,36,299,43*77
$GPGSV,4,3,16,13,21,105,43,14,08,176,25,16,21,105,24,17,45,122,42*77
$GPGSV,4,4,16,19,24,270,,20,24,270,42,23,21,066,43,25,07,327,43*76
$GPRMC,000121.000,A,3959.9995,N,11600.6567,E,21.89,90.02,010115,,*02
$GPVTG,90.02,T,,M,21.89,N,40.55,K*6D
$GPZDA,000121.000,01,01,2015,00,00*52


北斗模式，共565字节
$GPGGA,111130.000,3903.0945,N,11600.0003,E,1,12,1.1,47.9,M,0.0,M,,*52
$GPGLL,3903.0945,N,11600.0003,E,111130.000,A*33
$BDGSA,A,3,202,204,201,203,205,208,206,209,207,211,210,212,2.2,1.1,1.9*28
$BDGSV,3,1,12,201,38,145,42,202,38,216,42,203,44,188,42,204,26,123,41*6E
$BDGSV,3,2,12,205,18,113,42,206,23,220,40,207,08,187,40,208,59,317,40*60
$BDGSV,3,3,12,209,25,070,40,210,31,111,40,211,37,250,40,212,16,196,40*69
$GPRMC,111130.000,A,3903.0945,N,11600.0003,E,58.32,180.00,050313,,*35
$GPVTG,180.00,T,,M,58.32,N,108.01,K*6D
$GPZDA,111130.000,05,03,2013,00,00*53

洛阳经纬度
$GPGGA,074053.000,3441.1310,N,11224.5050,E,1,08,3.8,175.6,M,0.0,M,,*6A
$GPGLL,3441.1310,N,11224.5050,E,074053.000,A*34
$GNGSA,A,3,20,07,08,09,04,,,,,,,,4.7,3.8,2.8*2E
$GNGSA,A,3,214,209,207,,,,,,,,,,4.7,3.8,2.8*17
$GPGSV,3,1,10,01,61,050,,04,06,220,33,07,23,189,28,08,46,218,29*7D
$GPGSV,3,2,10,09,42,227,32,11,39,050,,19,08,071,,20,30,133,23*78
$GPGSV,3,3,10,28,67,334,,32,26,097,23*7D
$BDGSV,1,1,04,202,00,000,24,207,15,186,16,209,10,181,25,214,21,143,25*63
$GPRMC,074053.000,A,3441.1310,N,11224.5050,E,0.00,0.00,030813,,*0A
$GPVTG,0.00,T,,M,0.00,N,0.00,K*50
$GPZDA,074053.000,03,08,2013,00,00*58

$GPRMC,074053.000,A,3441.1310,N,11224.5050,E,0.00,0.00,030813,,*0A
15026474786,2013-07-23 21:00:09,112.40841600,34.68551600,0.00000,54,0,6,OIL:1/0
34.685516,112.408416
*/
void			GPSCls::InvalidToValid(char *pGPS)
{
	#if(TEST_CODE_JUST_FOR_TEST_LUOYANG_NEW==1)
	uchar		Pos[32];
	uchar		i,j,c;
  	RTC_STR	tRtc;
	pRTC_STR		pRtc;
	char		*pA;
	
	VirtaulGPS_Valid = 0;
	pA = MyStrStr(pGPS,",A,");
	if(pA!=NULL)	
	{
		if(TD.GPS==tGPS_VIRTUAL_GPS_VALID)		Uart.puts(" 已定位 ");
		return;			// 已定位，无需特殊处理
	}


	if( (RectFenceAttr.Total>0)|| (RoadAttr.Total>0) )		return;		// 偏离路线不使用特殊处理
	

	i = j = 0;
	c = *pGPS;
	while((c != '*')&&(c != '\0'))
	{
		if(c == ',')
		{
			Pos[j++] = i+1;
		}
		i++;
		c = *(pGPS+i);			
	}

	tRtc.Year  = DecCharToUint32(2,(pGPS+(Pos[PDATE]+4)) );		// 年
	tRtc.Month = DecCharToUint32(2,(pGPS+(Pos[PDATE]+2)) );		// 月
	tRtc.Day   = DecCharToUint32(2,(pGPS+(Pos[PDATE]+0)) );		// 日		
	tRtc.Hour  = DecCharToUint32(2,(pGPS+(Pos[PTIME]+0)) );		// 时
	tRtc.Min   = DecCharToUint32(2,(pGPS+(Pos[PTIME]+2))  );		// 分
	tRtc.Sec   = DecCharToUint32(2,(pGPS+(Pos[PTIME]+4))  );		// 秒

	tRtc.wYear = tRtc.Year+2000;
	pRtc = (pRTC_STR)(uchar*)&tRtc;
	VirtaulGPS_Second = Rtc.DateTimeToSecond(pRtc);
	
	
	if( (SamlePeriod1S!=0) && (TestBlind!=0)&&(VirtaulGPS_Second > 0x18C87EC3) )		//  13 03 05 10 00 35
	{
		VirtaulGPS_Valid = TRUE;
		GYPack.RtcSecond = VirtaulGPS_Second;
		gCnt.GPS_RtcSecond = VirtaulGPS_Second;
		if(TD.GPS==tGPS_VIRTUAL_GPS_VALID)		Uart.puts(" GPS定位，信号强度一般 ");
	}
	else if( (VirtaulGPS_Second > 0x18C87EC3)&&( VirtaulGPS_Second<0x18C88350) )		//  13 03 05 10 00 35 ~ 13 03 05 10 20 00
	{
		VirtaulGPS_Valid = TRUE;
		GYPack.RtcSecond = VirtaulGPS_Second;
		gCnt.GPS_RtcSecond = VirtaulGPS_Second;
		if(TD.GPS==tGPS_VIRTUAL_GPS_VALID)		Uart.puts(" GPS定位，信号强度一般 ");
	}
	else 
	{
		if(TD.GPS==tGPS_VIRTUAL_GPS_VALID)		Uart.puts(" 时间范围 ");
	}
	#endif
}


/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */

