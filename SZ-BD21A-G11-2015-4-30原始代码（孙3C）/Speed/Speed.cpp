/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												肖遥电子（广州）研发中心
$										Shawyoo Electronics (GuangZhou) R&D Center
$                       	http://www.shawyoo.com		 Email: autoele@qq.com
$								 QQ群：9069471/9069433/9524253		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      文件描述
	文件名: 	  Speed.c
	功能描述： 速度相关处理.停车前20秒数据；疲劳驾驶数据；超速数据；360小时数据；
	编译环境：Keil for ARM + ARTX1.10
	目标CPU:  LPC2000
	作者：
	创建时间：								 
	建议编辑环境：UltraEdit 11.00a+  制表符宽度为三个字符，程序可读性更好。
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/					  

#define			SPEEDRD_GLOBALS			1

#include		   <string.h>
#include		   <stdlib.h>
#include       "..\flash\DFF_25xxx.h"
#include       "..\flash\DFE_25xxx.h"
#include			"..\UART\UART0.H"

#include			"..\RTC\RTC.h"
#include			"..\GPS\GPS.h"
#include			"Speed.h"
#include			"..\Common\FLASH_Addr.h"	
#include			"..\Common\GB_T19056.h"	
#include			"..\KeyDisp\KeyDisp.h"
#include			"..\common\Algol.H"
#include			"..\GSM_GPRS\GSM_GPRS.h"	
#include			"..\GSM_GPRS\JT_T808.h" 
//#include		"GB.h"


// $$$$$$$$$$$$$$$$$$ <<< Use Configuration Wizard in Context Menu >>> $$$$$$$$$$$$$$$$$
// <h>速度相关配置


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>TEST_STATUS_TO_PC: 把速度和开关量原始数据送PC  <0=> 不发送 <1=> 发送
//   	<i> Default: 不发送
*/
#define		TEST_STATUS_TO_PC		0				// 把速度和开关量原始数据送PC

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>MAX_SPEED_KM_H: 最大允许时速值 <1800-3800>
//   	<i> Default: 3000
*/
#define		MAX_SPEED_KM_H			300

// <h>速度值处理相关

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>SPEED_FILTER: 选择是否需要速度突变滤波  <0=> 不滤波 <1=> 滤波
//   	<i> Default: 滤波
*/
#define		SPEED_FILTER			1

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>SPEED_ADJUST: 选择是否需要校正速度  <0=> 不校正 <1=> 校正
//   	<i> Default: 校正
*/
#define		SPEED_ADJUST			0


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>ADD1_OVER_FIVE: 速度值四舍五入处理  <0=> 不处理 <1=> 要四舍五入
//   	<i> Default: 要四舍五入
*/
#define		ADD1_OVER_FIVE			1

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>MAX_ACC_KM: 加速度的限制值(每秒变化的时速/10) <1-100>
//   	<i> Default: 50
*/
#define		MAX_ACC_KM				80	  			// 最大加速度，即每秒速度变化不超过40km

// </h>

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>CHECK_PREQUENCY: 速度采集的频率 <4-5>
//   	<i> Default: 5
*/
#define  	CHECK_PREQUENCY		5       		// 速度检测频度，5次/秒


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>SENDOR_FACTOR: 速度传感器传感系数，即每转多少脉冲 <1-100>
//   	<i> Default: 8
*/
#define		SENDOR_FACTOR			8			// 速度传感器每转的脉冲数

// <h>连续驾驶时间上限和休息时间下限
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>INTERVAL_RS: 选择间隔时间为固定值  <0=> 上位机设定 <1=> 固定为下面值
//   	<i> Default: 固定为下面值
*/
#define		INTERVAL_RS				0

#define		TEST_CODE_FOR_SPEED		1

// </h>

// </h>

// $$$$$$$$$$$$$$$$$$$ <<< end of configuration section >>> $$$$$$$$$$$$$$$$$$$$$$$$$$$$


#define	DEBUG_SPEED_INFO_OUPTPUT	1  //  1: 启用速度相关调试信息输出		0: 禁用速度相关调试信息输出

uint32		SpeedSecond; // 有速度计时


__task		void			TaskSpeed(void)	
{
	uint16	Speed;
	SpeedC.DrivingDataInit(SysReset);
	//IC_Card_Out = TRUE;
	gCnt.InsertIC_Card = 0;	
	
	while(1)
	{
		NOP();	NOP();	NOP();	NOP();
		TC.Speed = 0;
	   SpeedTackCnt++;
		os_evt_wait_or( 0x0001, (2*100) );		// 正常每秒执行1次

		if( (U0_Iap_Flag!=0)|| (GG_Status.Flag.Bits.UpdateRemoteSet == 1))  continue; // 远程升级
		
		
		if( SamlePeriod1S || TestAccuracy || TestBlind || TestCanBus )		continue;	// 精度测试或盲区测试，不处理下面程序

		if(TD.MainTask==tTASK_RUN_FLAG)	
		{
			Uart.puts("Speed Task run\r\n");
		}

		//if((SpeedTackCnt%50==0)&&(StopFlag))
		if((VehicleStatus.VS1.Bits.MainPowerLost==0)&&(ICCardBusing==FALSE))
		{
		   NOP();	NOP();
			ICCardBusing = TRUE;
		 	SpeedC.CheckDriverCard();
			ICCardBusing = FALSE;
		}

		SpeedC.SpeedStatusLogFun();

		Speed = SpeedC.DrivingDataRecording(uiGPS_Speed);

		SpeedC.FatigueDrivingFun(Speed);
		
		SpeedC.OverSpeedFun(Speed);
		
		
		NOP();	NOP();	NOP();	NOP();		
	}
}



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$			
	函数名称：GetSpeedValue
   功能描述：处理停车数据、48小时速度数据、360小时位置信息				 
   入口参数：无
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
	 
*/ 
uchar			OnlyUsePulseSpeed,OnlyUseGPS_Speed;
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
uint16			SpeedCls::DrivingDataRecording(uint16	Speed)      //每秒钟执行一次
{
	uchar SaveStopFlag = FALSE;
	static uchar FirstRun = 0xaa;
	static uchar  ValidPosMin;			  // 定位的分钟
	static uint32 NoMoveSecond = 0;
	static uint32 StopSecond = 0xff, DrivingSecond = 0;			  // 这里初始化StopSecond 为0xff是为了刚开始可能就产生事故疑点
	uint32		i,tl,Addr;
	RTC_STR		tRtc;
	pRTC_STR		pRtc;
	pSAVE_STOP_DATA		pSaveStopData;
	union
	{
		uchar		Buffer[256+4];	   
		uint16	B16[128+2];
		uint32	B32[64+1];
	}tPage;
	union
	{
		uchar		Buffer[64];
		uint16	B16[32];
		uint32	B32[16];
	}Tp;	
	uchar			SwitchStatus;
	uchar *pData = NULL;


	pRtc = (pRTC_STR)&tRtc;						// 按实时时钟读取速度值，每秒处理秒平均速度，每分钟处理分钟平均速度
	Rtc.GetRTC(pRtc);
	
	uiSpeed = Pulse.GetSecSpeed(Tp.Buffer);	// 读取1秒数据速度和开关量数据
	SwitchStatus = Tp.Buffer[1];


	if ((uiSpeed<5)&&(SpeedStatusJudge.Status==SPEED_STATUS_UNNORMAL)&&(GPSValidFlag == GPS_VALID_FLAG))
	{
		Speed = 	uiGPS_Speed;

		// 过滤掉大于200km/h的速度
		if (Speed>2000)
		{
			Speed = 0;
		}
		
		// 用于事故疑点
		for (i=0; i<10;i += 2)
		{
			Tp.Buffer[i] = Speed/10;
		}

	}
	else
	{
		Speed = uiSpeed;
	}
	

	
	//if ((Speed > 20)&&(gCnt.SystemSecond > 30)&&(SpeedSecond>10))		  // 速度大于5km/h	并且系统启动30秒后	清停止标志
	if ((Speed > 20)&&(gCnt.SystemSecond > 30))
	{
		if (++DrivingSecond > 10)
		{
			StopFlag = 0;
			StopSecond = 0;
		
			if ((DriverCardFlag==0))
			{
				if (((AlertLogonTime==0)||(AlertLogonTime==5) ||(AlertLogonTime==10) ||
					((AlertLogonTime % 300)==0) || ((AlertLogonTime % 305)==0) || ((AlertLogonTime % 310)==0)) )
				{
					if ((SpeedStatusJudge.Status!=SPEED_STATUS_UNNORMAL)&&(JT808Flag.VS5.Bits.OverSpeedAlarm==0))
					{  KeyDisp.SendDataToLCM_YH(eLCM_U_CENTER_TMP_MSG,(7*2+1),(uchar*)"\x47请插入驾驶员卡");	}
				}
				AlertLogonTime++;
			}

		}
		
	}
	else
	{	
		if(++StopSecond>10)
		{
			DrivingSecond = 0;
		}	
		AlertLogonTime = 0;
		StopFlag = 1;
		

	
	}	


	
	///////////////////////////////////////////////////////////////////// 最近5秒速度数据，用于判断停车
	Last5Speed[4] = Last5Speed[3];		Last5Speed[3] = Last5Speed[2];
	Last5Speed[2] = Last5Speed[1];		Last5Speed[1] = Last5Speed[0];
	Last5Speed[0] = Speed;

	//Uart.puts("Last5Speed Test Data:"); Uart.putb(1,10,(uchar *)&Last5Speed[0]); Uart.puts("\r\n"); 

  
	/// 急加速、急减速判断
	if( Last5Speed[0] < Last5Speed[1] )			// 减速度
	{											 
		Tp.Buffer[0] = Last5Speed[0]/10;
		Tp.Buffer[1] = Last5Speed[1]/10;				 
		for(i=0;i<5;i++)
		{
			if(  (AccelPara[0].Start[i] < Tp.Buffer[0])  && (   Tp.Buffer[0] < AccelPara[0].End[i] )		)
			{
				if( (Tp.Buffer[1]-Tp.Buffer[0])> AccelPara[0].Value[i] )
				{
					if(JT808ExtenVsFlag.VS7.Bits.AccelDec==0)	
					{
						JT808ExtenVsFlag.VS7.Bits.AccelDec = 1;
						JT808ExtenVsFlag.VS7.Bits.AccelAdd = 0;
						Uart.puts("急减速上报 ");
						GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_POS_REPORT,0,NULL);
					}
					
				}
				else
				{
					JT808ExtenVsFlag.VS7.Bits.AccelDec = 0;
				}
			}
		}
	}
	else 	// 加速
	{
		Tp.Buffer[0] = Last5Speed[0]/10;
		Tp.Buffer[1] = Last5Speed[1]/10;
		for(i=0;i<5;i++)
		{
			if(  (AccelPara[1].Start[i] < Tp.Buffer[0])  && (   Tp.Buffer[0] < AccelPara[1].End[i] )		)
			{
				if( (Tp.Buffer[0]-Tp.Buffer[1])> AccelPara[1].Value[i] )
				{
					if(JT808ExtenVsFlag.VS7.Bits.AccelAdd==0)
					{
						JT808ExtenVsFlag.VS7.Bits.AccelAdd = 1;
						JT808ExtenVsFlag.VS7.Bits.AccelDec = 0;
						GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_POS_REPORT,0,NULL);
					  	Uart.puts("急加速上报 ");
					}
					
				}
				else
					JT808ExtenVsFlag.VS7.Bits.AccelAdd = 0;
			}
		}
	}
		  												 
	// 存30秒 0.2秒的瞬时速度 
	for(i=29;i>0;i--)
	{
		memmove( (uchar*)&StopData[i],(uchar*)&StopData[i-1],10);
	}
	memmove( (uchar*)&StopData[0],Tp.Buffer,10);		// 最近1秒数据，保存在低地址

	//Uart.puts("speed Test Data:"); Uart.putb(1,300,(uchar*)&StopData[0]); Uart.puts("\r\n");

	//Uart.puts("sec speed dot 2 Test Data:"); Uart.putb(1,10,Tp.Buffer); Uart.puts("\r\n");

   
	/// 定位有效，行驶10秒，10秒位置没变，视为事故疑点	 可认为是翻车，轮子在转，位置不变
	if (GPSValidFlag == GPS_VALID_FLAG)
	{	 
		//if (SpeedSecond<10)	  // 视为未行驶
		if (SpeedSecond==0)
		{
			NoMoveSecond = 0;
			SaveStopFlag = FALSE;
		}
				

		//Uart.puts("GB last Lon Test Data:"); Uart.putb(1,4, (uchar *)&GPS.LastGBPack.Lon);Uart.puts("\r\n"); 
		//Uart.puts("GB Lon Test Data:"); Uart.putb(1,4, (uchar *)&GPS.GBPack.Lon);Uart.puts("\r\n"); 
		//Uart.puts("GB last Lat Test Data:"); Uart.putb(1,4, (uchar *)&GPS.LastGBPack.Lat);Uart.puts("\r\n"); 
		//Uart.puts("GB  Lat Test Data:"); Uart.putb(1,4, (uchar *)&GPS.GBPack.Lat);Uart.puts("\r\n"); 

		Tp.B32[0] = SwapINT32(GPS.LastGBPack.Lon);
		Tp.B32[1] = SwapINT32(GPS.GBPack.Lon);
		Tp.B32[2] = SwapINT32(GPS.LastGBPack.Lat);
		Tp.B32[3] = SwapINT32(GPS.GBPack.Lat);
			
	   // 赤道处的经度 1853米/分，越靠北极或南极时，每分经度的距离越来越小，每3分小1米
		// 纬度3706米/分	
		// 对纬度为说，+10表示偏3米， 对经度来说，+20 偏3米															
		if ((((Tp.B32[0]<Tp.B32[1]+100)&&(Tp.B32[0]+100>Tp.B32[1])) && 	  // 经度
				((Tp.B32[2]<Tp.B32[3]+100)&&(Tp.B32[2]+100>Tp.B32[3]))))			// 纬度
		//if ((GPS.LastGBPack.Lat==GPS.GBPack.Lat) &&(GPS.LastGBPack.Lon==GPS.GBPack.Lon))
		{
			//Uart.puts("位置不变");
	  		if ((SpeedSecond>10)&&(++NoMoveSecond==10))// 行驶10s
			{
				SaveStopFlag = TRUE;
				//#if (DEBUG_SPEED_INFO_OUPTPUT == 1)
				Uart.puts("产生事故疑点三");		 
				//#endif
			}
		}
		else
		{
			NoMoveSecond = 0;
			SaveStopFlag = FALSE;
			
		}
		GPS.LastGBPack = GPS.GBPack;
	}
	else
	{
		SaveStopFlag = FALSE;
		NoMoveSecond = 0;
	} 

	if (((SpeedSecond>=10)&&((Last5Speed[0])==0)&&(Last5Speed[1] !=0)) || (SaveStopFlag==TRUE))
	{
		if (SaveStopFlag==FALSE)
		{
			//#if (DEBUG_SPEED_INFO_OUPTPUT == 1)
			Uart.puts("产生事故疑点一或二");
			//#endif
		}

		SetBufferValue(tPage.Buffer,256,0);
		pSaveStopData = (pSAVE_STOP_DATA)&tPage;
		//Rtc.GetRTC_BCD((pRTC_STR)Tp.Buffer);
		if (SaveStopFlag==FALSE)
		{
			//tl = Rtc.DT.RtcSecond - 2;			// 使用 Rtc.DT.RtcSecond ，运行48小时后，产生的事故点少5，6秒（发现不准）
			tl = Rtc.DateTimeToSecond(pRtc); 
			tl -= 2;
		}
		else
		{
			//tl = Rtc.DT.RtcSecond;
			tl = Rtc.DateTimeToSecond(pRtc);
		}
		Rtc.SecondToDateTime_BCD(tl, (pRTC_STR)Tp.Buffer);
		
		memmove( &(pSaveStopData->DateTime[0]),(uchar*)&Tp.Buffer[0],6);							// 停车时刻年月日时分秒

		//Uart.puts("stop time Test Data:"); Uart.putb(1,6,&(pSaveStopData->DateTime[0])); Uart.puts("\r\n");

	  	memmove( &(pSaveStopData->DriverID[0]),&CurrentDriverInfo.ID[0],18);			// 当前驾驶员驾驶证号码
		//Uart.puts("CurrentDriverInfo.ID Test Data:"); Uart.putb(1,18,&CurrentDriverInfo.ID[0]); Uart.puts("\r\n");
		//memmove( &(pSaveStopData->Data.Buffer[0]),(uchar*)&StopData[10],200);			// 停车前20秒数据
		if (VehicleStatus.VS1.Bits.MainPowerLost==1)
		{
			memmove( &(pSaveStopData->Data.Buffer[0]),(uchar*)&StopData[2],200);		
		
		}
		else if ((uiSpeed<5)&&(SpeedStatusJudge.Status==SPEED_STATUS_UNNORMAL))
		{
			 memmove( &(pSaveStopData->Data.Buffer[0]),(uchar*)&StopData[0],200);
			 if (pSaveStopData->Data.Buffer[0] != 0)  pSaveStopData->Data.Buffer[0] = 0; //  确保停车一点是为0		
			 Uart.puts("保存速度异常时的事故疑点");
		}
		else
		{
			memmove( &(pSaveStopData->Data.Buffer[0]),(uchar*)&StopData[1],200);	
		}
		//memmove( &(pSaveStopData->Pos),(uchar*)&GPS.GBPack,10);	// 当前有效位置
		memmove( &(pSaveStopData->Pos),(uchar*)&ValidLocationInfo,10);			 // 最近一次有效位置

		//Uart.puts("save stop  Test Data:"); Uart.putb(1,200,(uchar *)&pSaveStopData->Data.Buffer[0]);	Uart.puts("\r\n");
		////////////////////////////////////////////////////////////////////////// 存储停车数据到Flash
		if( (FlashPageCnt.ParkingDataSave%FF_PAGE_PER_SECTOR)==0)
		{
			Addr = FF_PARKING_DATA_START_PAGE;
			Addr += FlashPageCnt.ParkingDataSave%FF_PARKING_DATA_TOTAL_PAGE;
			
			DFF.SectorEraseWaitOK(Addr/FF_PAGE_PER_SECTOR,10);
			
		}
		Addr = FF_PARKING_DATA_START_PAGE;
		Addr += FlashPageCnt.ParkingDataSave%FF_PARKING_DATA_TOTAL_PAGE;		//得到实际存储的页地址
		DFF.PageProgramWaitOK( Addr,tPage.Buffer,3);
		FlashPageCnt.ParkingDataSave++;
		#if (DEBUG_SPEED_INFO_OUPTPUT == 1)
		Uart.puts("存储停车数据到Flash ");
		#endif	
		
		//////////////////////////// 停车前15分钟每分钟平均速度

		DFE.Read( FM_15MIN_SPEED,15,Tp.Buffer);				// 读取已存储的停车前15分钟速度值	
		DFE.Write( FM_STOP_LAST15_SPEED,15,Tp.Buffer);
		Rtc.GetRTC_BCD((RTC_STR *)&Tp.Buffer[0]);	
		DFE.Write( FM_STOP_TIME,6,(uchar*)&Tp.Buffer[0]);
			
	}

	///// 重新上电开机第一次运行时，要检查一下上次的位置信息时间是否当前小时，不是，则保存，并且清空铁电中的位置信息
	if (FirstRun==0xaa)
	{
		FirstRun = 0;

		#if (DEBUG_SPEED_INFO_OUPTPUT == 1)
		Uart.puts("chek and save last pos info");
		#endif

		Save360HDataToFlashAndClearFMPosData(tPage.Buffer,Tp.Buffer);
		
	}
	

	///////////////////////////////////////////////////// 单位分钟内每秒钟的速度和开关量
	if(SpeedSwMinFlag==0)
	{
		SpeedSwPerMin[0].SpeedSw[tRtc.Sec].Speed = Speed/10;
		SpeedSwPerMin[0].SpeedSw[tRtc.Sec].Sw = SwitchStatus;
	}
	else
	{
		SpeedSwPerMin[1].SpeedSw[tRtc.Sec].Speed = Speed/10;
		SpeedSwPerMin[1].SpeedSw[tRtc.Sec].Sw = SwitchStatus;
	}

	///////////////////////////////////////////////////// 
	GPS.GB19056_DataPack(Tp.Buffer);
	memmove( (uchar*)&LocationInfo,(uchar*)&Tp.Buffer[0],LEN_LOCATION_INFO);
	if(GPSValidFlag == GPS_VALID_FLAG)
	{
		ValidPosMin = tRtc.Min;		  // 记录定位的分钟
		memmove( (uchar*)&ValidLocationInfo,(uchar*)Tp.Buffer,LEN_LOCATION_INFO);
	}

	
	
	if(tRtc.Sec==59)				//  如果0秒时处理，则分要减1，所以这里使用59秒
	{
		///////////////////////////////////////////////////// 单位分钟内每秒钟的速度和开关量的起始时间		
		if(SpeedSwMinFlag==0)
		{
			for (i=0; i<60;i++)
			{
				if (SpeedSwPerMin[0].SpeedSw[i].Speed != 0) break;					// 速度不为0
				
				if ((i<59)&&(SpeedSwPerMin[0].SpeedSw[i].Sw != SpeedSwPerMin[0].SpeedSw[i].Sw)) break;		// 开关量有变化
			}
			if (i==60)
			{
			//  不需要存储该分数速度和开关量
				SpeedSwMinFlag = 2;		  
			}
			else
			{
				//memmove( (uchar*)&SpeedSwPerMin[0].StartTime[0], (uchar*)&tRtc,6);
				Rtc.RTC_FormatToBCD(&tRtc,Tp.Buffer);
				memmove((uchar *)&SpeedSwPerMin[0],(uchar*)&Tp.Buffer[0],6);	//  年月日时分秒

				SpeedSwMinFlag = 1;

			}
		}
		else
		{
			for (i=0; i<60;i++)
			{
				if (SpeedSwPerMin[1].SpeedSw[i].Speed != 0) break;					// 速度不为0
				
				if ((i<59)&&(SpeedSwPerMin[1].SpeedSw[i].Sw != SpeedSwPerMin[1].SpeedSw[i].Sw)) break;		// 开关量有变化
			}

			if ((i!=60) || (SpeedSwMinFlag==1))
			{	
				///////////////////////////////////////////////////// 存储上一分钟”单位分钟内每秒钟的速度和开关量”到Flash
				///////////////////////////////////////////////////// 即48小速度数据，每次存储有速度或开关量变化的数据
				/////////////////////////////////////////////////////  就把2分钟存储1次数据到1页Flash
				///////////////////////////////////////////////////// 这里无需存储到铁电，备用电池存储超过2分钟时间
					//memmove( (uchar*)&SpeedSwPerMin[1].StartTime[0], (uchar*)&tRtc,6);
				Rtc.RTC_FormatToBCD(&tRtc,Tp.Buffer);
				memmove( (uchar*)&SpeedSwPerMin[1], Tp.Buffer,6);

				if( (FlashPageCnt.Speed48HSave%FF_PAGE_PER_SECTOR)==0)
				{
					Addr = FF_48H_SPEED_START_PAGE;
					Addr += FlashPageCnt.Speed48HSave%FF_48H_SPEED_TOTAL_PAGE;
					
					DFF.SectorEraseWaitOK(Addr/FF_PAGE_PER_SECTOR,10);
				}
				Addr = FF_48H_SPEED_START_PAGE;
				Addr += FlashPageCnt.Speed48HSave%FF_48H_SPEED_TOTAL_PAGE;		//得到实际存储的页地址
				DFF.PageProgramWaitOK( Addr,(uchar*)&SpeedSwPerMin,3);
				FlashPageCnt.Speed48HSave++;

				DFE.Write( FM_FLASH_PAGE_CNT, LEN_FLASH_PAGE_CNT, (uchar*)&FlashPageCnt);

				#if (DEBUG_SPEED_INFO_OUPTPUT == 1)
				//Uart.puts("存储48小时行驶数据的2分钟数据到Flash ");		
				#endif
			}
			SpeedSwMinFlag = 0;
			
		}
		////////////////////////////////////////////////////////// 单位小时内，每分钟的位置和平均速度
		////////////////////////////////////////////////////////// 铁电满1小时就存到3页的flash中
		////////////////////////////////////////////////////////// 每小时下一秒更新铁电时间，并清除数据，更新为当前时间和数据
		Save360HDataToFlashAndClearFMPosData(tPage.Buffer,Tp.Buffer);
		
		
		if (ValidPosMin==tRtc.Min)		 // 单位分钟位置信息取该分钟范围内首个有效的位置信息，如该分钟范围内无有效的位置信息，则该分钟位置信息为7FFFFFFFH；
		{
			memmove(Tp.Buffer,(uchar*)&ValidLocationInfo,LEN_LOCATION_INFO);	
		}
		else
		{
			 memmove(Tp.Buffer,(uchar*)&LocationInfo,LEN_LOCATION_INFO);
			 ValidPosMin=tRtc.Min; 
		}

		tl = Pulse.GetMinPlus();					// 
		tl *= 60;		// 
		Tp.Buffer[10] = tl/VehiclePulseKV;		// 速度值存储在位置信息后面

		if((Tp.Buffer[10]<1)&&(SpeedStatusJudge.Status==SPEED_STATUS_UNNORMAL))
		{
			Tp.Buffer[10] = GPS.Get1MinAvgSpeed()/10;		//  无脉冲速度，并且速度异常，取GPS速度
			
		}
	


		Addr = FM_360H_LOCATION_INFO;
		Addr += ((tRtc.Min%60)*LEN_LOCATOIN_INFO_PER_MIN);
		DFE.Write(Addr,LEN_LOCATOIN_INFO_PER_MIN,Tp.Buffer);
		NOP();NOP();NOP();

		/////////////////// 最近15分钟每分钟平均速度
		DFE.Read( FM_15MIN_SPEED,15,&Tp.Buffer[33]);		
		Tp.Buffer[32] = Tp.Buffer[10];		
		DFE.Write( FM_15MIN_SPEED,15,&Tp.Buffer[32]);

		///////////  存储360小时数据  
		#if (DEBUG_SPEED_INFO_OUPTPUT == 1)
		Uart.puts("存储360小时之1分钟位置信息到铁电 ");
		//Uart.puts("location Test Data:"); Uart.putb(1,LEN_LOCATION_INFO,(uchar*)&LocationInfo);  Uart.puts("\r\n");
		#endif

		 
		     
		// 写入flash
		if( (tRtc.Min==59) )
		{
			Save360HDataToFlash(tPage.Buffer);

			#if (DEBUG_SPEED_INFO_OUPTPUT == 1)
			Uart.puts("存储360小时之60分钟位置信息到Flash ");
		   #endif
		}
		
	}
	return Speed;				 
}

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：Save360HDataToFlash
   功能描述：保存360小时数据到flash				 
   入口参数：缓存
   出口参数：无
   全局变量: 无				
   创建时间：
   修改时间： 
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void			SpeedCls::Save360HDataToFlash(uchar *pBuffer)
{
	uchar i;
	uint32 Addr;
	// 写入flash
	for (i=0; i<3; i++)
	{
		if( (FlashPageCnt.Location360HSave%FF_PAGE_PER_SECTOR)==0)
		{
			Addr = FF_360H_LOCATION_START_PAGE;
			Addr += FlashPageCnt.Location360HSave%FF_360H_LOCATION_TOTAL_PAGE;
			
			DFF.SectorEraseWaitOK(Addr/FF_PAGE_PER_SECTOR,10);
		}
		
		// 读取
		SetBufferValue(&pBuffer[0], 256, 0);
		if (i==0)	 
			DFE.Read(FM_360H_LOCATION_TIME,226,&pBuffer[0]);	 // 从有时间的地址开始读取
		else			
			DFE.Read(FM_360H_LOCATION_INFO+i*220,220,&pBuffer[6]);		//  从实际地址开始读取	
	
		///////////////////////////////////////////存储到Flash
		Addr = FF_360H_LOCATION_START_PAGE;
		Addr += FlashPageCnt.Location360HSave%FF_360H_LOCATION_TOTAL_PAGE;
		DFF.PageProgramWaitOK( Addr,pBuffer,3);
		FlashPageCnt.Location360HSave++;
	}
	DFE.Write(FM_FLASH_PAGE_CNT, LEN_FLASH_PAGE_CNT, (uchar*)&FlashPageCnt);
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：Save360HDataToFlashAndClearFMPosData
   功能描述：保存360小时数据到flash,并清除铁电上的位置数据				 
   入口参数：256字节的缓存，64字节缓存
   出口参数：无
   全局变量: 无				
   创建时间：
   修改时间： 
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void			SpeedCls::Save360HDataToFlashAndClearFMPosData(uchar *p256ByteBuffer, uchar *p64ByteBuffer)
{
	uchar *pData;

	DFE.Read(FM_360H_LOCATION_TIME,6,p64ByteBuffer); 
	Rtc.GetRTC_BCD((RTC_STR  *)&p64ByteBuffer[6]);  
	#if (DEBUG_SPEED_INFO_OUPTPUT == 1)
	Uart.puts("Check pos time Test Data:"); Uart.putb(1,12,p64ByteBuffer); Uart.puts("\r\n");
	#endif
	if ((memcmp(p64ByteBuffer,p64ByteBuffer+6, 4) !=0)&&(FlashPageCnt.Location360HSave>0))
	{	 // 如果铁电中不是当前小时数，先保存之前的数据到flash，并清除单位小时数据，重新存储  
	  	#if (DEBUG_SPEED_INFO_OUPTPUT == 1)
		Uart.puts("保存之前位置数据到flash");
		#endif
		Save360HDataToFlash(p256ByteBuffer);

		pData = NULL;
		while(pData==NULL)
		{
			pData = MyMalloc(666);
			os_dly_wait(1);		
		}

		SetBufferValue(pData,666,0);
		p64ByteBuffer[6+4] = p64ByteBuffer[6+5] = 0; // 分，秒为0
		memmove(pData, p64ByteBuffer+6,6); 

		DFE.Write(FM_360H_LOCATION_TIME,666,pData);
		NOP();NOP();NOP();
	
		//Uart.puts("up pos info time Test Data:");Uart.putb(1,6,pData); Uart.puts("\r\n");

		MyFree(pData);

		#if (DEBUG_SPEED_INFO_OUPTPUT == 1)
		Uart.puts("更新铁电中的位置时间\r\n");
		#endif
		
	}
	
}



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：FatigueDriving
   功能描述：疲劳驾驶处理				 
   入口参数：1分钟的速度
   出口参数：无
   全局变量: 无				
   创建时间：
   修改时间： 
*/ 	

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void			SpeedCls::FatigueDrivingFun(uint16	Speed)
{
	//static   uint32 SpeedSecond = 0;
	uchar 	i, SaveOverTimeDataFlag = 0;	 // 保存疲劳驾驶标志： 0：不保存， 1：保存到铁电，更新铁电计数，条件满足，更新flash  2: 临时保存,应付检测中心上位机实时采集
	uint32	tl,Len,Addr;
	union
	{
		uchar 	Buffer[256];
		uint16	B16[128];
		uint32	B32[64];
	} Tp;
	RTC_STR		tRtc;
	pRTC_STR		pRtc;
	pSAVE_FATIGUE_DRIVING_INFO		pSave;
	//pDRIVER_INFO_TIME pDriverInfo;
	NOP();	NOP();


	#if(TEST_CODE_FOR_SPEED==1)
	if( TD.SW == tSPEED_DRIVE_PARA )
	{
		SetBufferValue( Tp.Buffer,128,0);
		strcpy((char*)Tp.Buffer,"Cnt: ");
		Len = 5;
		Int2Str( CurrentDriverInfoTime.TravelTimeSec,(char*)&Tp.Buffer[Len]);		
		strcat((char*)Tp.Buffer," ");		Len = strlen((char*)Tp.Buffer);
		Int2Str( CurrentDriverInfoTime.RestTimeSec,(char*)&Tp.Buffer[Len]);		
		strcat((char*)Tp.Buffer," ");		Len = strlen((char*)Tp.Buffer);
		Int2Str( CurrentDriverInfoTime.OneDayTravelTimeSec,(char*)&Tp.Buffer[Len]);		
		strcat((char*)Tp.Buffer," ");		Len = strlen((char*)Tp.Buffer);
		Int2Str( CurrentDriverInfoTime.ParkingTimeSec,(char*)&Tp.Buffer[Len]);
		strcat((char*)Tp.Buffer,"\r\n");
		Uart.puts((char*)Tp.Buffer);
		
		SetBufferValue( Tp.Buffer,128,0);
		strcpy((char*)Tp.Buffer,"Limit: ");
		Len = 7;
		Int2Str( LimitTime.TravelTimeSec,(char*)&Tp.Buffer[Len]);		
		strcat((char*)Tp.Buffer," ");		Len = strlen((char*)Tp.Buffer);
		Int2Str( LimitTime.RestTimeSec,(char*)&Tp.Buffer[Len]);		
		strcat((char*)Tp.Buffer," ");		Len = strlen((char*)Tp.Buffer);
		Int2Str( LimitTime.OneDayTravelTimeSec,(char*)&Tp.Buffer[Len]);		
		strcat((char*)Tp.Buffer," ");		Len = strlen((char*)Tp.Buffer);
		Int2Str( LimitTime.ParkingTimeSec,(char*)&Tp.Buffer[Len]);
		strcat((char*)Tp.Buffer," ");		Len = strlen((char*)Tp.Buffer);
		Int2Str( LimitTime.PreAlarmTime,(char*)&Tp.Buffer[Len]);
		Uart.puts((char*)Tp.Buffer);
	}
	#endif
	
	pRtc = (pRTC_STR)&tRtc;						// 按实时时钟读取速度值，每秒处理秒平均速度，每分钟处理分钟平均速度
	Rtc.GetRTC(pRtc);	

	
	// 统计不在位的前一位驾驶员的不在位时间
	for (i=0; i<2; i++)
	{
		if ((RecentDriverInfoTime[i].Number>=DRIVER_NUMBER_MIN)&&(RecentDriverInfoTime[i].Number!=CurrentDriverInfoTime.Number))
		{
			 if (RecentDriverInfoTime[i].OffTime<0xffff)
			 {
			 	RecentDriverInfoTime[i].OffTime++;
			 }
			 break;
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////
	if(Speed>=5)		// 大于1km/h，开始计时驾驶时间，单位为0.1km/h.
	{
		if (SpeedSecond==0)
		{
			Uart.puts("检测到开始速度:"); Int2Str(Speed,(char *)Tp.Buffer); Uart.puts((char *)Tp.Buffer); Uart.puts(" (0.1km/h)\r\n");
		}
		++SpeedSecond;


		if ((SpeedSecond>10)&&(LimitTime.EnableCheckFatigue==TRUE)) 
		{				
			if( CurrentDriverInfoTime.TravelTimeSec == 0 ) 	// 第0秒，为驾驶开始时间
			{
				
				CurrentDriverInfoTime.TravelTimeSec += 10; // 加上判断10S的时间
		
				JT808Flag.VS6.Bits.FatiguePreAlarm = 0;
				JT808Flag.VS5.Bits.FatigueDriving = 0;
				JT808_GPRS_TempPack = TRUE;
		
				#if (DEBUG_SPEED_INFO_OUPTPUT == 1)
				Uart.puts("驾驶计时开始 ");
				#endif
	
				//CurrentDriverInfoTime.StartTimeSec = Rtc.DT.RtcSecond - 10;//Rtc.DateTimeToSecond(pRtc);
				CurrentDriverInfoTime.StartTimeSec = Rtc.DateTimeToSecond(pRtc) - 10;
				memmove( (uchar*)&CurrentDriverInfoTime.StartLocatoin, (uchar*)&ValidLocationInfo,LEN_LOCATION_INFO);
				DFE.Write(FM_CURRENT_DRIVER_INFO_TIME,LEN_DRIVER_INFO_TIME,(uchar*)&CurrentDriverInfoTime);			 	
			}
			CurrentDriverInfoTime.ParkingTimeSec = 0;	
			CurrentDriverInfoTime.TravelTimeSec++;
			CurrentDriverInfoTime.OneDayTravelTimeSec++;	
			////////////////////////////////////////// 休息时间小于规定的时间，算连续驾驶时间
			if(CurrentDriverInfoTime.RestTimeSec>0)
			{
				if( (CurrentDriverInfoTime.RestTimeSec<LimitTime.RestTimeSec))
				{
					//CurrentDriverInfoTime.TravelTimeSec += CurrentDriverInfoTime.RestTimeSec;
					CurrentDriverInfoTime.TravelTimeSec += CurrentDriverInfoTime.RestTimeSec + 10;// 有10s秒的判断行驶，所以要多加10秒
					#if (DEBUG_SPEED_INFO_OUPTPUT == 1)
					Uart.puts("休息时间不足，算驾驶时间 ");
					#endif
				}
				CurrentDriverInfoTime.RestTimeSec = 0;
				CurrentDriverInfoTime.ParkingTimeSec = 0;
			}
		
		}
		JT808Flag.VS7.Bits.StopOverTime = 0;
		CurrentDriverInfoTime.ParkingTimeSec = 0;
	}
	else 
	{	
		SpeedSecond = 0;
	
		if (CurrentDriverInfoTime.TravelTimeSec>0)	 // 要有驾驶时间，没速度时计数休息时间
		{
			if(CurrentDriverInfoTime.RestTimeSec==0)
			{
				JT808_GPRS_TempPack = TRUE;
				#if (DEBUG_SPEED_INFO_OUPTPUT == 1)
				Uart.puts("休息计时开始 ");
				#endif
			}

			CurrentDriverInfoTime.RestTimeSec++;	
		
			if( CurrentDriverInfoTime.RestTimeSec == LimitTime.RestTimeSec )	// 休息时间大于规定时间，才能清0驾驶时间
			{	
				for(i=0;i<2;i++)	// 清除所有驾驶员驾驶时间
				{
					SetBufferValue(Tp.Buffer, LEN_DRIVER_INFO_TIME,0);
					DFE.Write (FM_RECENT_DRIVER_INFO_TIME+LEN_DRIVER_INFO_TIME*i, LEN_DRIVER_INFO_TIME, (uchar*)&Tp.Buffer);
					NOP(); NOP();
				}

				if( CurrentDriverInfoTime.TravelTimeSec>=LimitTime.TravelTimeSec-3)
				{	
						SaveOverTimeDataFlag = 1;
						CurrentDriverInfoTime.TravelTimeSec = 0;
						JT808Flag.VS6.Bits.FatiguePreAlarm = 0;
						JT808Flag.VS5.Bits.FatigueDriving = 0;

						DFE.Write( FM_CURRENT_DRIVER_INFO_TIME,LEN_DRIVER_INFO_TIME,(uchar*)&CurrentDriverInfoTime);

						#if (DEBUG_SPEED_INFO_OUPTPUT == 1)
						Uart.puts("休息时间大于规定时间 1，清驾驶时间");
						#endif

						// 疲劳驾驶分两步更新连续结束驾驶时间：第一步是停电10秒就更新一次，计数器加1
						// 第二步是，停时达到休息时间后，更新疲劳驾驶信息，以及计数，	这个FM_FATIGUE_DRIVING_CNT
						// 计影响上位机实时查询有没有疲劳驾驶
						// 为了找到相同的存储位置，所以下面的计数要减1
						OverTimeDriveUpateFlag= 0;
						DFE.Write(EE_OVERTIME_DRIVE_UPDATE_FLAG,1,&OverTimeDriveUpateFlag);
						NOP();NOP();
		  				DFE.Read(FM_FATIGUE_DRIVING_CNT,4,(uchar*)&tl);	 	// 
						if (tl>0) tl--;
						DFE.Write(FM_FATIGUE_DRIVING_CNT,4,(uchar*)&tl);

						goto _SAVE_FAITGUE_DATA;
				}

				CurrentDriverInfoTime.TravelTimeSec = 0;
				JT808Flag.VS6.Bits.FatiguePreAlarm = 0;
				JT808Flag.VS5.Bits.FatigueDriving = 0;	 
				JT808_GPRS_TempPack = TRUE;

				DFE.Write( FM_CURRENT_DRIVER_INFO_TIME,LEN_DRIVER_INFO_TIME,(uchar*)&CurrentDriverInfoTime);

				
				#if (DEBUG_SPEED_INFO_OUPTPUT == 1)
				Uart.puts("休息时间大于规定时间 2，清驾驶时间");
				#endif	
			}
			else if (CurrentDriverInfoTime.RestTimeSec==10)			 //  休息有10s说明是停车了	 
			{
				if( CurrentDriverInfoTime.TravelTimeSec>=LimitTime.TravelTimeSec-3)
				{
					 #if (DEBUG_SPEED_INFO_OUPTPUT == 1)
					 Uart.puts("更新连续驾驶时的结束时间\r\n");
					 #endif

					 if (OverTimeDriveUpateFlag==0)
					 {
						OverTimeDriveUpateFlag= 1;
						DFE.Write(EE_OVERTIME_DRIVE_UPDATE_FLAG,1,&OverTimeDriveUpateFlag);
					 }
					 else
					 {
					 	 DFE.Read(FM_FATIGUE_DRIVING_CNT,4,(uchar*)&tl);	 	// 
						 if (tl>0) tl--;
						 DFE.Write(FM_FATIGUE_DRIVING_CNT,4,(uchar*)&tl);
					 }

					 SaveOverTimeDataFlag = 2; // 临时更新到铁电，还没有算是疲劳驾驶结束		 应付无锡检测，停车10S
					 goto _SAVE_FAITGUE_DATA;
				}
				
			}
			

		}
		
		//////////////////////////////////////////// 停车超时处理
		if( (LimitTime.EnableCheckParking==TRUE) && (LimitTime.ParkingTimeSec>0) )
		{
			if(CurrentDriverInfoTime.ParkingTimeSec==0)
			{
				JT808Flag.VS7.Bits.StopOverTime = 0;
				JT808_GPRS_TempPack = TRUE;
				#if (DEBUG_SPEED_INFO_OUPTPUT == 1)
				Uart.puts("停车计时开始 ");
				#endif
			}
			CurrentDriverInfoTime.ParkingTimeSec++;				// // 实际测试，未加1时，快了1秒
			if( CurrentDriverInfoTime.ParkingTimeSec == (LimitTime.ParkingTimeSec+1) )
			{
				JT808Flag.VS7.Bits.StopOverTime = 1;
				JT808_GPRS_TempPack = TRUE;
				#if (DEBUG_SPEED_INFO_OUPTPUT == 1)
				Uart.puts("停车超时报警 ");
				#endif
				KeyDisp.SendDataToLCM_YH(eLCM_U_CENTER_TMP_MSG,(10*2+1),(uchar*)"\x47停车超时，请提高效率！");			
			}		
			if( CurrentDriverInfoTime.ParkingTimeSec > LimitTime.ParkingTimeSec )
			{
				if( ((CurrentDriverInfoTime.RestTimeSec - LimitTime.ParkingTimeSec )%180)==0 )	// 3分钟提醒1次
				{
					JT808Flag.VS7.Bits.StopOverTime = 1;
					JT808_GPRS_TempPack = TRUE;
					#if (DEBUG_SPEED_INFO_OUPTPUT == 1)
					Uart.puts("停车超时报警 ");
					#endif
					KeyDisp.SendDataToLCM_YH(eLCM_U_CENTER_TMP_MSG,(10*2+1),(uchar*)"\x47停车超时，请提高效率！");	
				
				}
			}
		}
		else
		{
			JT808Flag.VS7.Bits.StopOverTime = 0;		
			CurrentDriverInfoTime.ParkingTimeSec = 0;	
		}
		return;			// 停车，不再判断后面的疲劳驾驶
	}
	
	//////////////////////////////////////////////////////////////// 未使能，直接返回

	if( LimitTime.EnableCheckFatigue!=TRUE )		return;

	////////////////////////////////////////// 每分钟存储1次驾驶时间
	if( tRtc.Sec==59)
	{
		DFE.Write( FM_CURRENT_DRIVER_INFO_TIME,LEN_DRIVER_INFO_TIME,(uchar*)&CurrentDriverInfoTime);	
		NOP();NOP();
		DFE.Write(FM_RECENT_DRIVER_INFO_TIME,(LEN_DRIVER_INFO_TIME*2), (uchar*)&RecentDriverInfoTime[0]);		 	
	}
	

	////////////////////////////////////////////////////////// 疲劳驾驶预警
	if((LimitTime.TravelTimeSec > CurrentDriverInfoTime.TravelTimeSec)&&
		(CurrentDriverInfoTime.TravelTimeSec>=LimitTime.TravelTimeSec - LimitTime.PreAlarmTime) )
	{	 
		tl = CurrentDriverInfoTime.TravelTimeSec - (LimitTime.TravelTimeSec - LimitTime.PreAlarmTime);
		if(((tl==0)||(tl==5)||(tl==10))||((tl%(5*60))==0) || ((tl%(5*60+5))==0)||((tl%(5*60+10))==0)) 
		{
			JT808Flag.VS6.Bits.FatiguePreAlarm = 1;
			JT808Flag.VS7.Bits.StopOverTime = 0;
			JT808_GPRS_TempPack = TRUE;
			#if (DEBUG_SPEED_INFO_OUPTPUT == 1)
			Uart.puts("疲劳驾驶预警 ");
			#endif
			KeyDisp.SendDataToLCM_YH(eLCM_U_CENTER_TMP_MSG,(8*2+1),(uchar*)"\x47请注意，疲劳预警");
			BEEP(9,0,10,0,30);			 //
			
		}
	}
	
	if(( CurrentDriverInfoTime.TravelTimeSec >= LimitTime.TravelTimeSec )&&
		(( CurrentDriverInfoTime.TravelTimeSec<LimitTime.TravelTimeSec+30*60 )))
	{
		tl = CurrentDriverInfoTime.TravelTimeSec - LimitTime.TravelTimeSec;
		if(((tl==0)||(tl==5)||(tl==10)||((tl%(5*60))==0)) || ((tl%(5*60+5))==0)||((tl%(5*60+10))==0)) 	  	// 5分钟提醒1次  提示30分钟
		{
			JT808Flag.VS5.Bits.FatigueDriving = 1;
			//pSave = (pSAVE_FATIGUE_DRIVING_INFO)&Tp.Buffer;
			KeyDisp.SendDataToLCM_YH(eLCM_U_CENTER_TMP_MSG,(17*2+1),(uchar*)"\x47安全驾驶，疲劳驾驶警告，请注意休息");
		  	BEEP(9,0,10,0,30);			 //进入疲劳驾驶阶段每分钟鸣叫15次
			JT808_GPRS_TempPack = TRUE;
			#if (DEBUG_SPEED_INFO_OUPTPUT == 1)
		  	Uart.puts("疲劳驾驶警告1 ");
			#endif
				
		}		
	}	

_SAVE_FAITGUE_DATA:
	//////////// 保存疲劳驾驶数据 /////////////////
	if (SaveOverTimeDataFlag != 0)
	{
			//SaveData = FALSE;

			pSave = (pSAVE_FATIGUE_DRIVING_INFO)&Tp.Buffer;
			//Rtc.SecondToDateTime(CurrentDriverInfoTime.EndTimeSec, pRtc);		// 更新结束时间和结束位置信息
			// 更新结束时间和结束位置信息
			CurrentDriverInfoTime.EndTimeSec = Rtc.DateTimeToSecond(pRtc);
			if (SaveOverTimeDataFlag==1)
			{
				CurrentDriverInfoTime.EndTimeSec -= LimitTime.RestTimeSec; // 停车到休息时间才算保存，所以这里减去掉休息时间
			}
			else
			{
				CurrentDriverInfoTime.EndTimeSec -= 10;
			}
		  	memmove( (uchar*)&(pSave->EndLocation), (uchar*)&ValidLocationInfo,LEN_LOCATION_INFO);
		  	DFE.Write( FM_CURRENT_DRIVER_INFO_TIME,LEN_DRIVER_INFO_TIME,(uchar*)&CurrentDriverInfoTime);	
		  	
		  	///////////////////////////////////////////////////////////////// 存储当前疲劳驾驶信息到铁电
		  	
		  	memmove ( pSave->ID, &CurrentDriverInfoTime.ID[0],18);		  	
		  	pRtc = (pRTC_STR)&tRtc;	
		  	Rtc.SecondToDateTime(CurrentDriverInfoTime.StartTimeSec , pRtc);
		  	//memmove( pSave->StartTime, (uchar*)&tRtc,6);
			Rtc.RTC_FormatToBCD(&tRtc, pSave->StartTime);
		  	Rtc.SecondToDateTime(CurrentDriverInfoTime.EndTimeSec, pRtc);
		  	//memmove( pSave->EndTime, (uchar*)&tRtc,6);
			Rtc.RTC_FormatToBCD(&tRtc, pSave->EndTime);
		  	memmove( (uchar*)&(pSave->StartLocation), (uchar*)&CurrentDriverInfoTime.StartLocatoin,LEN_LOCATION_INFO);
		  	memmove( (uchar*)&(pSave->EndLocation), (uchar*)&ValidLocationInfo,LEN_LOCATION_INFO);
		  	
		  	DFE.Read(FM_FATIGUE_DRIVING_CNT,4,(uchar*)&tl);
		  	Addr = tl%5;
		  	Addr *= LEN_SAVE_FATIGUE_DRIVING_INFO;
		  	Addr += FM_SAVE_FATIGUE_DRIVING;		  	
		  	DFE.Write( Addr, LEN_SAVE_FATIGUE_DRIVING_INFO,Tp.Buffer);
		   tl++;
		  	DFE.Write(FM_FATIGUE_DRIVING_CNT,4,(uchar*)&tl);	 	// 疲劳驾驶计数器加1后存储到铁电	

			#if (DEBUG_SPEED_INFO_OUPTPUT == 1)	
		  	Uart.puts("更新疲劳驾驶记录到铁电 "); 
			#endif 

			if (SaveOverTimeDataFlag==2)
			{
				SaveOverTimeDataFlag = 0;
				return;  //
			}

			if ((tl)&&(tl%5)==0)
			{
				// 铁电充满近一页的Flash的空间，则放到Flash上
				if( (FlashPageCnt.FatigueDrivingSave%FF_PAGE_PER_SECTOR)==0)
				{
					Addr = FF_TIRE_DRIVE_START_PAGE;
					Addr += FlashPageCnt.FatigueDrivingSave%FF_TIRE_DRIVE_TOTAL_PAGE;
					
					DFF.SectorEraseWaitOK(Addr/FF_PAGE_PER_SECTOR,10);
				}				
				///////////////////////////////////////////存储到Flash  这样1页flash存有5条疲劳驾驶记录
				SetBufferValue(Tp.Buffer,256,0);
				DFE.Read(FM_SAVE_FATIGUE_DRIVING,LEN_SAVE_FATIGUE_DRIVING_INFO*5,Tp.Buffer); // 

				Addr = FF_TIRE_DRIVE_START_PAGE;
				Addr += FlashPageCnt.FatigueDrivingSave%FF_TIRE_DRIVE_TOTAL_PAGE;
				DFF.PageProgramWaitOK(Addr,Tp.Buffer,3);
				FlashPageCnt.FatigueDrivingSave++;

				DFE.Write( FM_FLASH_PAGE_CNT, LEN_FLASH_PAGE_CNT, (uchar*)&FlashPageCnt);

				#if (DEBUG_SPEED_INFO_OUPTPUT == 1)	
		  		Uart.puts("保存疲劳驾驶记录到Flash"); 
				#endif 
			}

			SaveOverTimeDataFlag = 0; // 清0
	}



} // End of <eTaskTiredDrive> function






/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：OverSpeed
   功能描述：超速驾驶处理				 
   入口参数：无
   出口参数：无
   全局变量: 无				
   创建时间：						
   修改时间： 
*/ 	
///         OVER_SPEED_MAX_CNT		LEN_OVER_SPEED_1REC		FM_OVER_SPEED_REC
//void			Delay_mS(unsigned char  c);
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void			SpeedCls::OverSpeedFun(uint16   Speed)
{
	uint32	Len,tmp;
	union
	{
		uchar 	Buffer[128];
		uint16	B16[64];
		uint32	B32[32];
	} Tp;
	static	uint16		PreAlarmCnt;

	NOP();	NOP();	
	Speed /= 10;
	if( (OverSpeed.Enable==FALSE)||(OverSpeed.HighSpeedLimit==0)||(Speed<5)||(Speed>380) )	// 小于3公里/小时或大于380公里/，不计超速
	{
		JT808Flag.VS5.Bits.OverSpeedAlarm = 0;
		VehicleStatus.VS3.Bits.OverSpeed = 0;
		OverSpeed.SecondCnt = 0;
		return;
	}
	

	/////////////////////////////////////////////////////////////////////////////  超速
	if( Speed > OverSpeed.HighSpeedLimit)
	{  
		if( OverSpeed.SecondCnt == 0)
		{
			#if (DEBUG_SPEED_INFO_OUPTPUT == 1)
			Uart.puts("超速计时开始 ");
			#endif
			JT808Flag.VS5.Bits.OverSpeedAlarm = 0;
			VehicleStatus.VS3.Bits.OverSpeed = 0;
			JT808_GPRS_TempPack = TRUE;
		}
		OverSpeed.SecondCnt++;
		PreAlarmCnt = 0;
	
		tmp = (OverSpeed.EnableTime+1);
		if((OverSpeed.SecondCnt == tmp)||( OverSpeed.SecondCnt==tmp+5)||(OverSpeed.SecondCnt==tmp+10))	// 实际测试，未加1时，快了1秒
		{
			JT808Flag.VS5.Bits.OverSpeedAlarm = 1;
			VehicleStatus.VS3.Bits.OverSpeed = 1;
			JT808_GPRS_TempPack = TRUE;
			#if (DEBUG_SPEED_INFO_OUPTPUT == 1)
			Uart.puts("超速报警 ");
			#endif
			BEEP(9,0,2,0,10);
			
			//KeyDisp.SendDataToLCM_YH(eLCM_U_CENTER_TMP_MSG,(16*2+1),(uchar*)"\x47安全驾驶，超速警告，请减速行驶！");
			KeyDisp.SendDataToLCM_YH(eLCM_U_CENTER_TMP_MSG,(8*2+1),(uchar*)"\x47您已超速，请减速");
		} 
	  	if(OverSpeed.SecondCnt > OverSpeed.EnableTime )
		{
		
			tmp = OverSpeed.SecondCnt-OverSpeed.EnableTime;
			if( (tmp%(5*60)==0) || (tmp % (5*60+5)==0)	||  (tmp % (5*60+10)==0))		// 5分钟提醒三次，每2秒提示一次
			{
				BEEP(9,0,2,0,10);
				#if (DEBUG_SPEED_INFO_OUPTPUT == 1)
				Uart.puts("超速报警1 ");
				#endif
			
				JT808Flag.VS5.Bits.OverSpeedAlarm = 1;
				VehicleStatus.VS3.Bits.OverSpeed = 1;
				JT808_GPRS_TempPack = TRUE;
				KeyDisp.SendDataToLCM_YH(eLCM_U_CENTER_TMP_MSG,(8*2+1),(uchar*)"\x47您已超速，请减速");
			}
		}	     
	}
	else 
	{
		/////////////////////////////////////////////////////////////////////////////  超速预警
		#if (WUXI_TEST != 1)
		if( (Speed+OverSpeed.PreAlarmDifference/10) >= OverSpeed.HighSpeedLimit ) 
		{
			PreAlarmCnt++;
			if( (PreAlarmCnt==1)||((PreAlarmCnt%60)==0))
			{
				JT808Flag.VS6.Bits.OverSpeedPreAlarm = 1;
				JT808_GPRS_TempPack = TRUE;
				#if (DEBUG_SPEED_INFO_OUPTPUT == 1)
				Uart.puts("超速预警 ");
				#endif
				KeyDisp.SendDataToLCM_YH(eLCM_U_CENTER_TMP_MSG,(16*2+1),(uchar*)"\x47安全驾驶，超速预警，请勿加速！");
			}
		}
		else					  
		{
			JT808Flag.VS6.Bits.OverSpeedPreAlarm = 0;
			PreAlarmCnt = 0;
		}
		#endif
		OverSpeed.SecondCnt = 0;
	}

	if ((JT808Flag.VS5.Bits.OverSpeedAlarm==1) && ( Speed < OverSpeed.HighSpeedLimit) )
	{
		JT808Flag.VS5.Bits.OverSpeedAlarm = 0;
		VehicleStatus.VS3.Bits.OverSpeed = 0;
		JT808_GPRS_TempPack = TRUE;
		#if (DEBUG_SPEED_INFO_OUPTPUT == 1)
		Uart.puts("速度降低，清除超速报警标志 ");
		#endif
	}
	 

	#if(TEST_CODE_FOR_SPEED==1)
	if ( TD.SW == tSPEED_OVER_SPEED_PARA )
	{
		SetBufferValue( Tp.Buffer,128,0);
		if( OverSpeed.Enable==TRUE)
			strcpy((char*)Tp.Buffer,"OVP,Enable ");
		else
			strcpy((char*)Tp.Buffer,"OVP,Disable ");
		Len = 12;
		Int2Str( OverSpeed.HighSpeedLimit,(char*)&Tp.Buffer[Len]);		
		strcat((char*)Tp.Buffer," ");		Len = strlen((char*)Tp.Buffer);
		Int2Str( OverSpeed.LowSpeedLimit,(char*)&Tp.Buffer[Len]);		
		strcat((char*)Tp.Buffer," ");		Len = strlen((char*)Tp.Buffer);
		Int2Str( OverSpeed.PreAlarmDifference,(char*)&Tp.Buffer[Len]);		
		strcat((char*)Tp.Buffer," ");		Len = strlen((char*)Tp.Buffer);
		Int2Str( OverSpeed.EnableTime,(char*)&Tp.Buffer[Len]);
		strcat((char*)Tp.Buffer," ");		Len = strlen((char*)Tp.Buffer);
		Int2Str( OverSpeed.SecondCnt,(char*)&Tp.Buffer[Len]);
		strcat((char*)Tp.Buffer,"\r\n");
		Uart.puts((char*)Tp.Buffer);
	}
	#endif

} // End of <OverSpeed> function





/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：
   功能描述：初始化				 
   入口参数：无
   出口参数：无
   全局变量: 无				
   创建时间：
   修改时间： 
*/	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void				SpeedCls::DrivingDataInit(uchar PowerOn)
{
	uchar t;
	NOP();	NOP();

	PulsPerMin = 0;
	StopFlag = 1;

	DFE.Read( FM_VEHICLE_PULSE_KV,4,(uchar*)&VehiclePulseKV);
	if( (VehiclePulseKV < 10 ) || (VehiclePulseKV>10000) )
	{
		VehiclePulseKV = 3600;
		Uart.puts("脉冲系数错误，取默认值3600 ");
		DFE.Write( FM_VEHICLE_PULSE_KV,4,(uchar*)&VehiclePulseKV);
	}
	
	DFE.Read( FM_OVER_SPEED_PARA,LEN_OVER_SPEED_PARA,(uchar*)&OverSpeed);
	
	DFE.Read( FM_DRIVING_PARA,LEN_DRIVING_TIME_LIMIT,(uchar*)&LimitTime);
	
	DFE.Read( FM_CURRENT_DRIVER_INFO_TIME,LEN_DRIVER_INFO_TIME,(uchar*)&CurrentDriverInfoTime);

	memmove(CurrentDriverInfo.ID,CurrentDriverInfoTime.ID, 18);

	DFE.Read(EE_JT_DRIVER_NAME_LEN,1,(uchar *)&t); 	// 姓名长度
	if(t>10)	t = 10;
	DFE.Read(EE_JT_DRIVER_NAME,t,CurrentDriverInfo.Name);
	
	#if (1)//#if (WUXI_TEST==1)
	OverSpeed.Enable = TRUE;					  // 使能超速报警
	LimitTime.EnableCheckFatigue =  TRUE;		// 使能超速报警
	#endif

	DFE.Read(FM_SPEED_STAUTS_JUDGE,LEN_SPEED_STATUS_JUDGE,(uchar *)&SpeedStatusJudge);

	DFE.Read(EE_OVERTIME_DRIVE_UPDATE_FLAG,1,&OverTimeDriveUpateFlag);

	DFE.Read( FM_RECENT_DRIVER_INFO_TIME, (LEN_DRIVER_INFO_TIME*2), (uchar*)&RecentDriverInfoTime[0]);


	
}	

				
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：
   功能描述：
   入口参数：无
   出口参数：无
   全局变量: 无				
   创建时间：
   修改时间： 
*/	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用				
void			SpeedCls::GetCurrentDriver(uchar *pDriver)
{
	
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：
   功能描述：
   入口参数：无
   出口参数：无
   全局变量: 无
   创建时间：
   修改时间：
*/	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用	
void			SpeedCls::SetGetPara(uchar Cmd,uchar Len,uchar *pData)
{	
	NOP();	NOP();
	switch(Cmd)
	{
		case eSPEED_CMD_SET_DRIVING_TIME:			// 疲劳驾驶相关参数
			////////////// 设置参数时，重新计时
			CurrentDriverInfoTime.TravelTimeSec = 0;
			CurrentDriverInfoTime.RestTimeSec = 0;
			CurrentDriverInfoTime.OneDayTravelTimeSec = 0;			
			CurrentDriverInfoTime.ParkingTimeSec = 0;
			JT808Flag.VS6.Bits.FatiguePreAlarm = 0;
			JT808Flag.VS5.Bits.FatigueDriving = 0;
			JT808Flag.VS7.Bits.StopOverTime = 0;
			memmove( (uchar*)&LimitTime,pData,LEN_DRIVING_TIME_LIMIT);		
			DFE.Write( FM_DRIVING_PARA,LEN_DRIVING_TIME_LIMIT,(uchar*)&LimitTime);						
		break;
		case eSPEED_CMD_GET_DRIVING_TIME:			// 疲劳驾驶相关参数					
			memmove( pData, (uchar*)&LimitTime, LEN_DRIVING_TIME_LIMIT);
		break;


		case eSPEED_CMD_SET_OVER_SPEED:	  		// 超速相关参数
			OverSpeed.SecondCnt = 0;
			memmove( (uchar*)&OverSpeed,pData,LEN_OVER_SPEED_PARA);
			DFE.Write( FM_OVER_SPEED_PARA,LEN_OVER_SPEED_PARA,(uchar*)&OverSpeed );
		break;
		case eSPEED_CMD_GET_OVER_SPEED:	  		// 超速相关参数
			memmove( pData, (uchar*)&OverSpeed, LEN_OVER_SPEED_PARA);
		break;


		case eSPEED_CMD_DISABLE_CNT:

			if( *pData == 0 )	
			{
				OverSpeed.Enable = 0;
				LimitTime.EnableCheckFatigue = 0;
				LimitTime.EnableCheckParking = 0;
				CurrentDriverInfoTime.TravelTimeSec = 0;
				CurrentDriverInfoTime.RestTimeSec = 0;
				CurrentDriverInfoTime.OneDayTravelTimeSec = 0;						
				CurrentDriverInfoTime.ParkingTimeSec = 0;
	
				JT808Flag.VS6.Bits.FatiguePreAlarm = 0;
				JT808Flag.VS5.Bits.FatigueDriving = 0;
				JT808Flag.VS7.Bits.StopOverTime = 0;
				Uart.puts("禁止疲劳驾驶、超速、停车超时报警 ");
			}
			else if( *pData == 1 )
			{
				OverSpeed.Enable = TRUE;
				Uart.puts("使能超速报警 ");
			}
			else if( *pData == 2 ) 	
			{
				LimitTime.EnableCheckFatigue = TRUE;
				Uart.puts("使能疲劳驾驶报警 ");
			}
			else if( *pData == 3 ) 	
			{
				LimitTime.EnableCheckParking = TRUE;
				Uart.puts("使能超时停车报警 ");
			}
		break;


		default: break;
	}
	
}

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：
   功能描述：
   入口参数：无
   出口参数：无
   全局变量: 无
   创建时间：
   修改时间：
*/	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用	
uint16			SpeedCls::SpeedStatusLogFun(void)
{
	static uchar Aert = 0;
	static uint32 SpeedUnnormalSecond = 0, SpeednormalSecond = 0, AlertSecond = 0;
	uchar Save = 0;
	uint16 FSpeed;
	uint32 Addr;
	union
	{
		uchar		Buffer[256];
		uint16	B16[128];
		uint32	B32[64];
	}Tp;

	int32 tl, Second;
	
   
	//uiGPS_Speed = 401; // test


	if ((GPSValidFlag == GPS_VALID_FLAG)&&(uiGPS_Speed>400))	 // 定位，GPS速度大于40KM/H
	{	 
		Rtc.GetRTC_BCD((pRTC_STR)Tp.Buffer);

		// 年，月，日 有一个不同，就认为是下个日历天
		if ((Tp.Buffer[0] != SpeedStatusJudge.LastTime[0]) || (Tp.Buffer[1] != SpeedStatusJudge.LastTime[1])||
			(Tp.Buffer[2] != SpeedStatusJudge.LastTime[2]))
		{
			
			SpeedStatusJudge.LastTime[0] = Tp.Buffer[0];	 SpeedStatusJudge.LastTime[1] = Tp.Buffer[1];
			SpeedStatusJudge.LastTime[2] = Tp.Buffer[2];
			SpeedStatusJudge.Flag = 0;
			DFE.Write(FM_SPEED_STAUTS_JUDGE,LEN_SPEED_STATUS_JUDGE,(uchar *)&SpeedStatusJudge);
			
			#if (DEBUG_SPEED_INFO_OUPTPUT == 1)
			Uart.puts("新一个日历天，重新判定");
			#endif

		}

		if(SpeedStatusJudge.Flag==0) // 未没有判定
		{	
			// 5分钟内，每秒的GPS速度与记录仪速度对比，如果这时间内大于-/+11%，就记这5分钟时时间作为开始时间
			// 5分钟那个点作为结束时间
		
	  		FSpeed = Pulse.GetSecSpeed(Tp.Buffer);	

			
			tl = 	uiGPS_Speed - FSpeed;

			if (tl<0) tl = -tl;
			tl *= 100;			  // 放大100倍 

			//Uart.puts("uiGPS_Speed Test Data:"); Uart.putb(1,2,(uchar *)&uiGPS_Speed); Uart.puts("\r\n");
			//Uart.puts("FSpeed Test Data:"); Uart.putb(1,2,(uchar *)&FSpeed); Uart.puts("\r\n");
			//Uart.puts("tl Test Data:"); Uart.putb(1,4,(uchar *)&tl); Uart.puts("\r\n");

			if ((tl / uiGPS_Speed) >= 11)	 // 超过±11%时, 上面放大100倍，所以这里大于11就可以
			{
				SpeednormalSecond = 0;

				if (SpeedUnnormalSecond==0)
				{
					Rtc.GetRTC_BCD((pRTC_STR)SpeedStatusLog.StartTime);	
					//#if (DEBUG_SPEED_INFO_OUPTPUT == 1)  
					Uart.puts("速度异常开始");
					//#endif
				}

				//Uart.puts("速度异常计数");

				if (SpeedUnnormalSecond<60)
				{
					SpeedStatusLog.Speed[SpeedUnnormalSecond].RecordSpeed = FSpeed / 10;
					SpeedStatusLog.Speed[SpeedUnnormalSecond].RefSpeed = uiGPS_Speed / 10;

					//Uart.puts("FSpeed Test Data:"); Uart.putb(1,2,(uchar *)&FSpeed);Uart.puts("\r\n");
					//Uart.puts("uiGPS_Speed Test Data:"); Uart.putb(1,2,(uchar *)&uiGPS_Speed); Uart.puts("\r\n");
					//Uart.puts("SpeedStatusLog.Speed Test Data:"); Uart.putb(1,2,&SpeedStatusLog.Speed[SpeedUnnormalSecond].RecordSpeed);	Uart.puts("\r\n");
					//Uart.puts("SpeedUnnormalSecond Test Data:");	Uart.putb(1,4,(uchar *)&SpeedUnnormalSecond);Uart.puts("\r\n");
				}
				//else if (SpeedUnnormalSecond == 62)		// test
				else if (SpeedUnnormalSecond == 300)		 // 连续5分内	  300
				{
				 	// 速度异常
					/*SpeedStatusJudge.Flag = 1;
					SpeedStatusJudge.Status = SPEED_STAUTS_UNNORMAL;

					Second = Rtc.BDC_DateTimeToSecond((pRTC_STR)SpeedStatusLog.StartTime);
					Second += 300;
					Rtc.SecondToDateTime_BCD(Second,(pRTC_STR)SpeedStatusLog.EndTime);*/
					KeyDisp.SendDataToLCM_YH(eLCM_U_CENTER_TMP_MSG,(16*2+1),(uchar*)"\x47速度异常");
					Save = SPEED_STATUS_UNNORMAL;
					AlertSecond = 0;
					Aert = TRUE;
					//#if (DEBUG_SPEED_INFO_OUPTPUT == 1)
					Uart.puts("速度异常");
					//#endif

					
				}
				//Uart.puts("SpeedUnnormalSecond Test Data:");	Uart.putb(1,4,(uchar *)&SpeedUnnormalSecond);Uart.puts("\r\n");
				
				SpeedUnnormalSecond++;				
			}
			else
			{
				SpeedUnnormalSecond = 0;

			
				
				//if (SpeedStatusJudge.Status==SPEED_STATUS_UNNORMAL)
				{	
					if (SpeednormalSecond==0)
					{
						Rtc.GetRTC_BCD((pRTC_STR)SpeedStatusLog.StartTime);	
						//#if (DEBUG_SPEED_INFO_OUPTPUT == 1)  
						Uart.puts("速度正常计数开始");
						//#endif
						
					}
								  
					if (SpeednormalSecond<60)
					{			 
						SpeedStatusLog.Speed[SpeednormalSecond].RecordSpeed = FSpeed / 10;
						SpeedStatusLog.Speed[SpeednormalSecond].RefSpeed = uiGPS_Speed / 10;


						//Uart.puts("FSpeed Test Data:"); Uart.putb(1,2,(uchar *)&FSpeed);Uart.puts("\r\n");
						//Uart.puts("uiGPS_Speed Test Data:"); Uart.putb(1,2,(uchar *)&uiGPS_Speed); Uart.puts("\r\n");
						//Uart.puts("SpeedStatusLog.Speed Test Data:"); Uart.putb(1,2,&SpeedStatusLog.Speed[SpeednormalSecond].RecordSpeed);	Uart.puts("\r\n");
						//Uart.puts("SpeednormalSecond Test Data:");	Uart.putb(1,4,(uchar *)&SpeednormalSecond);Uart.puts("\r\n");
					}
					else if (SpeednormalSecond==300)		 
					{				
					  	Save = SPEED_STATUS_NORMAL;
						AlertSecond = 0;
						Aert = TRUE;

						KeyDisp.SendDataToLCM_YH(eLCM_U_CENTER_TMP_MSG,(4*2+1),(uchar*)"\x47速度正常");	 // 

						Uart.puts("速度正常");
					}
					//Uart.puts("SpeednormalSecond Test Data:");	Uart.putb(1,4,(uchar *)&SpeednormalSecond);Uart.puts("\r\n");
					SpeednormalSecond++;
				}
			}

			if (Save)
			{
				SpeedStatusJudge.Flag = 1;
				SpeedStatusJudge.Status = Save;
				SpeedStatusLog.Status = Save;
				DFE.Write(FM_SPEED_STAUTS_JUDGE,LEN_SPEED_STATUS_JUDGE,(uchar *)&SpeedStatusJudge);

				//Uart.puts("speed log 1 Test Data:"); Uart.putb(1,LEN_SPEED_STATUS_LOG,	(uchar *)&SpeedStatusLog); Uart.puts("\r\n");

				Second = Rtc.BDC_DateTimeToSecond((pRTC_STR)SpeedStatusLog.StartTime);
				Second += 300;
				//Rtc.SecondToDateTime_BCD(Second,(pRTC_STR)SpeedStatusLog.EndTime);	  // 奇怪，用这个，SpeedStatusLog 第3秒的脉冲速度不对
				Rtc.SecondToDateTime_BCD(Second,(pRTC_STR)Tp.Buffer);
				memmove(SpeedStatusLog.EndTime,Tp.Buffer,6);

				//Uart.puts("speed log 11 Test Data:"); Uart.putb(1,LEN_SPEED_STATUS_LOG,	(uchar *)&SpeedStatusLog); Uart.puts("\r\n");
				//Uart.puts("SpeedStatusLog.StartTime Test Data:"); Uart.putb(1,6,(uchar *)&SpeedStatusLog.StartTime);Uart.puts("\r\n");
				//Uart.puts("SpeedStatusLog.EndTime Test Data:"); Uart.putb(1,6,(uchar *)&SpeedStatusLog.EndTime);Uart.puts("\r\n");

				////////////////////////////  保存速度状态日志
				if( (FlashPageCnt.SpeedStatusSave%FF_PAGE_PER_SECTOR)==0)
				{
					Addr = FF_SPEED_STATUS_START_PAGE;
					Addr += FlashPageCnt.SpeedStatusSave%FF_SPEED_STATUS_TOTAL_PAGE;
					
					DFF.SectorEraseWaitOK(Addr/FF_PAGE_PER_SECTOR,10);
				}

				Addr = FF_SPEED_STATUS_START_PAGE;
				Addr += FlashPageCnt.SpeedStatusSave%FF_SPEED_STATUS_TOTAL_PAGE;

				SetBufferValue(Tp.Buffer,256,0);
				memmove(Tp.Buffer,(uchar *)&SpeedStatusLog,LEN_SPEED_STATUS_LOG);

				//Uart.puts("speed log 2 Test Data:"); Uart.putb(1,LEN_SPEED_STATUS_LOG,	(uchar *)&SpeedStatusLog); Uart.puts("\r\n");
				
				DFF.PageProgramWaitOK( Addr,Tp.Buffer,3);
				FlashPageCnt.SpeedStatusSave++;

				DFE.Write( FM_FLASH_PAGE_CNT, LEN_FLASH_PAGE_CNT, (uchar*)&FlashPageCnt);

				#if (DEBUG_SPEED_INFO_OUPTPUT == 1)
				Uart.puts("存储速度状态日志到Flash ");
				#endif
				
			}
			
		}
	}
	else
	{
		 SpeedUnnormalSecond = 0;
		 SpeednormalSecond = 0;

	}


	if ((Aert==TRUE)&&((++AlertSecond==6) || (AlertSecond==11) ))
	{
		 if (SpeedStatusJudge.Status==SPEED_STATUS_UNNORMAL)
		 {
		 	KeyDisp.SendDataToLCM_YH(eLCM_U_CENTER_TMP_MSG,(4*2+1),(uchar*)"\x47速度异常");
		 }
		 else	if (SpeedStatusJudge.Status==SPEED_STATUS_NORMAL)
		 {
		 	  KeyDisp.SendDataToLCM_YH(eLCM_U_CENTER_TMP_MSG,(4*2+1),(uchar*)"\x47速度正常");		// 晚上调试，先停
		 }
	}



	return 0;
}





SpeedCls::SpeedCls()
{

}



/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */

