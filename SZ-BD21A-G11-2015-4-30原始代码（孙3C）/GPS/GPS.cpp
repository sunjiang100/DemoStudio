/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												 
$									
$                       	 		
$								  		
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      文件描述
	文件名:    GPS.cpp
	功能描述： 
	编译环境： 
	目标CPU:  (char*)
	作者：
	创建时间：
	建议编辑环境：UltraEdit 11.00a+  制表符宽度为三个字符，程序可读性更好。
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/	

#include			"..\Config\RTOS.h"
#define			GPS_GLOBALS		1
#include			"..\GPS\GPS.h"
#include			"..\UART\Uart3.h"
#include			"..\UART\Uart0.h"
#include			"..\RTC\RTC.h"
#include			"..\KeyDisp\KeyDisp.h"
#include			"..\GSM_GPRS\GSM_GPRS.h"
#include			<stdlib.h>
#include			<string.h>
#include			<math.h>
#include			"..\FLASH\DFE_25xxx.H"
#include			"..\FLASH\DFF_25xxx.H"
#include			"..\Common\FLASH_ADDR.H"
#include			"..\Switch\switch.h"
#include			"..\Common\GB_T19056.h"
#include			"..\GSM_GPRS\JT_T808.h"



extern		uchar		RecordTaskRun,PicDataUpLoadFlag;
uchar			TestRoadSecNo;

extern		uchar		jt_auth_ok_flag;
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：TaskGPS_RecOK
   功能描述：成功收到一帧GPS数据的处理任务
   入口参数：无
   出口参数：无
   全局变量: 
   创建时间：							 113.34127166
   修改时间：
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/
uchar			GPS_CoolStart;
uchar			OnlyUseBD_GPS;
__task		void        TaskGPS(void)	
{	
	NOP();	NOP();
	static	uint32	Cnt=0;

	NOP();	NOP();
	
	NOP();	NOP();
	GPS.GPSValidCnt = 5;
	while(GPS.GPSValidCnt)
	{
		GPS_POWER_OFF;	
		os_dly_wait(100);
		GPS.GPSValidCnt--;
	}
	
	GPS_POWER_ON;
	os_dly_wait(300);
	
	/*GPS.GPSValidCnt = 20;
	while(GPS.GPSValidCnt)
	{
		Uart3.puts("$PCAS10,2*1E\r\n");
		os_dly_wait(20);
		GPS.GPSValidCnt--;
	}	  */
	
	NOP();	NOP();
	GPS.Init(0);
	NOP();	NOP();
	GPS.GPSValidCnt = 0;
	TestRoadSecNo = 0;
	
	GPS_CoolStart = 0;
	
	NOP();	NOP();
	while(1)
	{
		Cnt++;
		TC.GPS = 0;
		NOP();	NOP();
		
		if(GPS_CoolStart)
		{
			os_dly_wait(100);
			GPS_CoolStart++;
			if(GPS_CoolStart<30)
			{	GPS_POWER_OFF;	}
			else
			{
				GPS_POWER_ON;
				Uart3.puts("$PCAS10,2*1E\r\n");
				Uart.puts("$PCAS10,2*1E\r\n");				
				if(GPS_CoolStart>45)	GPS_CoolStart = 0;
			}
			continue;
		}
		
		
		os_evt_wait_or( 0x0001, (2*100) );		// 正常每秒执行1次
		
		if(TD.GPS==tTASK_RUN_FLAG)			Uart.puts("GPS Task 1\r\n\0");	

		if( (GG_Status.Flag.Bits.UpdateRemoteSet == 1)||(U0_Iap_Flag!=0) )   continue; // 升级

		if( (GG_Status.Flag.Bits.Online == 1) && ( jt_auth_ok_flag != 0 )  && ((Cnt%120) == 0) && (PicDataUpLoadFlag == 0) ) //上报图片数据								// 心跳包
		{
			Uart.puts("心跳包1 ");
			GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_HEARTBEAT,0,NULL);		
		}
		//省电模式下上传位置信息，劫警，盗警，拖车报警将唤醒GPS模块		
		//if((IDEL_For_Acc_OFF==TRUE)&&(( set_pos_send_time  - count_second_for_send)>60)&&(JT808Flag.VS5.Bits.EmergencyAlarm==0)&&  		
		//  	(JT808Flag.VS8.Bits.StealAlarm==0)&&(JT808Flag.VS8.Bits.IllegalMove==0) )
		#if (WUXI_TEST==1)
		if (0)
		#else
		if((IDEL_For_Acc_OFF==TRUE) )
		#endif
		{			
		  	GPS.GPS_PowerOn = FALSE;
		  	GPS_POWER_OFF; 
		  	GPS.LastJTPack.Date[0] = HexToBcd(Rtc.DT.Year);
	      GPS.LastJTPack.Date[1] = HexToBcd(Rtc.DT.Month);
	      GPS.LastJTPack.Date[2] = HexToBcd(Rtc.DT.Day);
			//////////////////////////////////////////////////////////////////// RTC时间					
	      GPS.LastJTPack.Time[0] = HexToBcd(Rtc.DT.Hour);
	      GPS.LastJTPack.Time[1] = HexToBcd(Rtc.DT.Min);
	      GPS.LastJTPack.Time[2] = HexToBcd(Rtc.DT.Sec);
		  	KeyDisp.SetLED_Status(LED_GPS,LED_TURN_OFF);

			////////////////////////////////////////////////////////		休眠发送位置信息				
			if( (GG_Status.Flag.Bits.Online == 1) && ( jt_auth_ok_flag != 0 )  )	
			
			{
				if( Cnt%(2*60/2) == 0 )			// 5分钟发送1条，Cnt每2秒加1
				{
					GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_POS_REPORT,0,NULL);
					//GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_HEARTBEAT,0,NULL);	
					Uart.puts("休眠，定时发送位置信息 ");
				}
			}
			else
			{
			    GPS.LocationInfoPushAlignment();
			}
		}
		else
		{
			    GPS.LocationInfoPushAlignment();
				os_dly_wait(10);
			 	GPS.eTask();
		}
	}			
}// End of <TaskGPS> function
				  




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：GPSCls::eTask
   功能描述：GPS任务函数
   入口参数：无
   出口参数：无
   全局变量: 
   创建时间：2015-10-10 12:25
   修改时间：	0f 64 af cb
	*GY,1018000002,V1,0,0,0,0,0,0,<0,0,20,0,0,0,09>
*/ 
/*
//uchar		  const	 CofigANT[12+1] = {0xB5,0x62,0x06,0x13,0x04,0x00,0x1f,0x00,0x8b,0x29,0xf0,0x88,0x00};
uchar		  const	 CofigANT[12+1] = {0xB5,0x62,0x06,0x13,0x04,0x00,0x1f,0x00,0x0f,0x64,0xaf,0xcb,0x00};
			 // \xB5 \x62 \x06 \x13 \x04 \x00 \x1f \x00 \x0f \x64 \xaf \xcb
uchar		  const	 GetCofigANT[8+1] = {0xb5,0x62,0x0a,0x09,0x00,0x00,0x13,0x43,0x00};
uchar		  const	 PollCofigANT[8+1] = {0xb5,0x62,0x06,0x13,0x00,0x00,0x19,0x51,0x00};
*/

#define		TEST_ROAD  0

#if (TEST_ROAD)
static uint32 lonS,latS,lonE,latE,lonQ,latQ;
#endif 

void			GPSCls::eTask(void)
{
	uchar		GPS_RecDataOK;
	
	#pragma		pack(1)
	union
	{
		uchar		Buffer[16];
		uint16	B16[8];
		uint32	B32[4];
		unsigned long long B64[2];
	} Tp;
	
	

	//uint32	tl;
	uchar		Index;


	TaskTickCnt++;	

	if(TD.MainTask==tTASK_RUN_FLAG)	  Uart.puts("GPS Task Run");

	if((TD.TestMode==TEST_MODE)||(TD.TestMode==TEST_MODE_AUTO))
	{
		//if(TaskTickCnt%(3*10)==0)  
		if((TaskTickCnt%3)==0)  //3秒   因为任务有了  os_evt_wait_or( 0x0001, (2*100) );	 
		{
			SetBufferValue((uchar*)Tp.Buffer,16,0);
			strcpy((char*)Tp.Buffer,"GY-TD-GG");
			Tp.Buffer[8] = GPS_SatelNum;
			Tp.Buffer[9] = GG_Status.Signal;
			Tp.Buffer[10] = '\r';
			Tp.Buffer[11] = '\n';
			Uart.putb(1,12,Tp.Buffer);
		}
	}

	if (InvalidGPSValidTime>180)	  // 3分钟不定位就重启模块
	{
		GPS_PowerOn = 0;
		InvalidGPSValidTime = 0;
	}
	
	if(GPS_PowerOn)
	{
		GPS_POWER_ON;	
		//IODIR0 &= ~(1UL<<8);
		//IODIR1 &= ~(1UL<<18);
	}
	else
	{	
		if(TD.GPS==tTASK_RUN_FLAG)
			Uart.puts("GPS Power Off 3S \r\n\0");
		GPS_POWER_OFF;	
		DataUpDate  = 0;
		os_dly_wait(300);
		GPS_POWER_ON;
		GPS_InvalidCnt = 0;
		GPS_PowerOn = TRUE;
		SystemLog.GPS.RestartCnt++;
		os_dly_wait(100);			
	}


// tett		测试路线
//	lonS = 1133711530UL;	latS = 231411510UL;		 // S
//	lonE = 1133899660UL;	latE = 231357970UL;		 // E
//	lonQ = 1133689483UL;	latQ = 231201483UL;		 // Q			231201483				 // W
//
//
//
#if (TEST_ROAD)
   lonS = 52888608UL;	latS = 1359085568UL;		 // S
	lonE = 2030047488UL;	latE = 18876416UL;		 // E
	lonQ = 1213034966UL;	latQ = 311141583UL;		 // Q			231201483				 // W


				  
  	if (CheckIsOutRoad(lonS,latS,lonE,latE,lonQ,latQ,0) == TRUE)
	{
		Uart.puts("在路外");
	}
	else
	{
			Uart.puts("在路内");
	}

	return; // test
#endif

//
				
	
	
	/////////// 激活GPS天线开路检测 ///////////////////////////////
	

	NOP();	NOP();	

	if(TD.GPS==tTASK_RUN_FLAG)			Uart.puts("GPS Task 2\r\n\0");	

	SetBufferValue((uchar*)GPS_String,LEN_GPS_STRING,0);
	SetBufferValue((uchar*)GPS_AntString,LEN_GPS_ANT_STRING,0);
	GPS_RecDataOK = Uart3.GetRecBuffer(GPS_String,GPS_AntString,GPS_SatelStatus);
				
	NOP();	NOP();	
	if(TD.GPS == tGPS_CONST_DATA)
	{
		GPS_RecDataOK = 1;
		//Rtc.GetLocalTimeString(0,Tp.Buffer);				// 113.3681866
		strcpy(GPS_String,"$GPRMC,064321.00,V,,,,,,,,,,*7F\r\n\0");
		strcpy(GPS_String,"$GPRMC,064321.00,A,2308.35334,N,11322.07557,E,1.026,270.50,120610,,,D*7F\r\n\0");
		strcpy(GPS_String,"$GPRMC,064321.00,A,908.35334,N,9522.07557,E,10.026,12.56,120610,,,D*7F\r\n\0");
		//strcpy(GPS_String,"$GPRMC,092052.00,A,2308.35444,N,11322.08014,E,43.106,282.54,151009,,,A*64\r\n\0");	
	}	
	
	NOP();	NOP();
	if(GPS_RecDataOK)
	{		
		NOP();	NOP();
		SelfTestResust &= ~eST_ER_GPS;
		GPS_InvalidCnt = 0;			
		GPS_Working = TRUE;
		NOP();	NOP();	

		if( (BD_SatelNum>= 6) || (GPS_SatelNum>= 6) )
		{
			InvalidToValid( GPS_String );		 // 'V' -> 'A'			
		}

		GPS_AntCheck(GPS_AntString);
		VehicleStatus.VS2.Bits.GPS_ModuleError = 0;
		if (JT808Flag.VS5.Bits.ANT_Error == 1)
		{
			JT808_GPRS_TempPack = TRUE;
			Uart.puts("TP.JT808Flag.VS5.Bits.ANT_Error = 1\0"); 
		}	
		JT808Flag.VS5.Bits.ANT_Error = 0;
		SetBufferValue((uchar*)GPS_AntString,LEN_GPS_ANT_STRING,0);	
		SystemLog.GG1.Speed_Info &=  ~(0xffUL << 24) ;
		SystemLog.GG1.Speed_Info |=  (GetSatelliteStatus(GPS_SatelStatus) & 0xff) << 24 ;			// 	卫星个数
	}
	else			// 没有GPS数据信号
	{
		GPS_InvalidCnt++;		   
		GPS_Working = 0;
		if(GPS_InvalidCnt>2*10)
		{
		   
			KeyDisp.SetLED_Status(LED_GPS,LED_TURN_OFF);
			if((TD.TestMode==TEST_MODE)||(TD.TestMode==TEST_MODE_AUTO))
			{
				if(TaskTickCnt%(3*10)==0)
					Uart.puts("\r\n\r\nGPS No Data\r\n\0");
			}
			VehicleStatus.VS2.Bits.GPS_ModuleError = 1;
				
		}
		if(GPS_InvalidCnt>6*10)									  //6秒
		{
			GPS_PowerOn = 0;			
		}
		if(GPS_InvalidCnt>10*10)
		{
			if (!(JT808_Alarm_Word.Shield & (0x01 << 4)))
			{
				if(JT808Flag.VS5.Bits.ANT_Error == 0)
				{
					JT808Flag.VS5.Bits.ANT_Error = 1;
	
					Index = 4;
			  		if (JT808_Alarm_Word.SendTextSMS & (0x01 << Index))		     								// 发送短信
			  		{
						GSM.SetSMS_Buffer((char *)&JT808_Number.TextSMS[0],"GPS Model Error Alarm!!!");		
			  		}
			  		//if (JT808_Alarm_Word.TakePhoto & (0x01 << Index))	Camera.SetTakeNumber(2,TRUE);		  // 拍照
					if (JT808_Alarm_Word.TakePhoto & (0x01 << Index))
					{
			  			if (!(JT808_Alarm_Word.SavePhoto & (0x01 << Index)))	Jt_photo_upload = 1;		  // 上传到平台
						MediaEventFlag =eBB_MEDIA_EVENT_OTHER;
			  			Camera.SetTakeNumber(2,TRUE);		  // 拍照 
			  		}
				}
			}
		   SelfTestResust |= eST_ER_GPS;
		}
	}
	if( (TD.GPS==tTASK_RUN_FLAG)&&(TD.Value<1) ) return;
	
	////////////////////////////////////////////////////////////////////////////////// 处理GPS数据	
	NOP();	NOP();	
	if(GPS_RecDataOK)
	{		
		NOP();	NOP();
		DataCompleteAnalys(GPS_String);		// 数据完整性检测				
		if( (TD.GPS==tTASK_RUN_FLAG)&&(TD.Value<2) ) return;
		///////////////////////////////////////////////////// 数据打包 （注：打包前必须作完整性检测）		
		GY_DataPack((uchar*)NULL);				//  GPS数据包
		////////////////////////////	
		//if( (TD.GPS==tTASK_RUN_FLAG)&&(TD.Value<30) ) return;	

		SetBufferValue((uchar*)GPS_CharPack,LEN_GPS_CHAR_PACK,0);
		TH_DataPack((char*)NULL);
		
		//if( (TD.GPS==tTASK_RUN_FLAG)&&(TD.Value<40) ) return;	
		NOP();	NOP();
		//TH_BCD_Pack((uchar*)NULL);
		NOP();	NOP();
		//LCM_BCD_Pack((uchar*)NULL);
		NOP();	NOP();
		GB19056_DataPack((uchar*)NULL);
		NOP();	NOP();
		JT808_DataPack((uchar*)NULL);		
		
		if(GPSValidFlag == GPS_VALID_FLAG)
		{	
			if (DriftingFlag==TRUE)			 // 发生漂移
			{	
				SetBufferValue((uchar*)GPS_CharPack,LEN_GPS_CHAR_PACK,0);
				strcpy(GPS_CharPack,Last_GPS_CharPack);	

				memmove((uchar*)&JTPack,(uchar*)&LastJTPack,LEN_JT808_GPS_PACK);

			}	
			else
			{	
				SetBufferValue((uchar*)Last_GPS_CharPack,LEN_GPS_CHAR_PACK,0);
				strcpy(Last_GPS_CharPack,GPS_CharPack);

				memmove((uchar*)&LastJTPack,(uchar*)&JTPack,LEN_JT808_GPS_PACK);
			}
		}
		 
		NOP();	NOP();
		if(TD.GPS==tGPS_PACK_FLAG)		Uart.puts(GPS_CharPack);	
		
		
		//////////////////////////////////////////////////////////////
		//if( (TD.GPS==tTASK_RUN_FLAG)&&(TD.Value<50) ) return;		
		if(TD.GPS == tGPS_HEX_PACK)				// 输出 GPS数据包
		{
			Uart.putb(1,LEN_GY_GPS_PACK,(uchar*)&GYPack);
		}				
		
      
		//////////////////////////////////////////// 电子围栏检测
		NOP();	NOP();	NOP();	NOP();
		CheckRectangleFence(GYPack.Longitude,GYPack.Latitude);			 //方形电子围栏检测
		
		NOP();	NOP();	NOP();	NOP();
		CheckRoadLineFence(GYPack.Longitude,GYPack.Latitude);
		
		/////////////////////////////////////////////////////////////////////位置信息打包进队
		NOP();	NOP();	NOP();	NOP();
		//LocationInfoPushAlignment();			// 不定位时，进队正常吗？模块坏了就不能进队了？
		
		if( (GPSValidFlag == GPS_VALID_FLAG)&& (DriftingFlag==FALSE) ) 
		{
			memmove((uchar*)&LastGYPack,(uchar*)&GYPack,LEN_GY_GPS_PACK);
		   memmove((uchar*)&LastJTPack,(uchar*)&JTPack,LEN_JT808_GPS_PACK);
		}		
	}

	//LocationInfoPushAlignment();
	////////////////////////////////////////////////////////////////////////
	NOP();	NOP();	NOP();	NOP();
	


	////////////////////////////////////////////////////////////////// RTC校正
	
	if( (GPSValidFlag==GPS_VALID_FLAG)||(VirtaulGPS_Valid == TRUE))
	{			
		GPSValidCnt++;
		if( ( (GPSValidCnt%60)==0)|| (GPSValidCnt==15) )
		{
			#if (WUXI_TEST != 1)
			Rtc.SetRTC_DT(gCnt.GPS_RtcSecond+8*60*60);			  						 // 调用RTC设置函数	
			NOP();	NOP();
			#endif
		}			
	}
	else
	{
		GPSValidCnt = 0;
	}
	
	if((TD.TestMode==TEST_MODE)||(TD.TestMode==TEST_MODE_AUTO))
	{
		if(TaskTickCnt%(5*10)==0)
		{
			if(GPSValidFlag=='A')
				Uart.puts("\r\nGPS OK!\r\n\0");
		}
	}
	Uart3.ClearRecBuffer();
}



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：DataValidAnaly
   功能描述：GPRMC数据完整性分析
   入口参数：GPRMC原始数据字符串
   出口参数：完整标志
   全局变量: 
   创建时间：2009-03-06 11:00
   修改时间：
	注：检验和正确且定位有效，则数据有效，否则无效数据。
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
uchar			GPSCls::DataCompleteAnalys(char	*pGPS)
{
	
	uchar		c,Sum,Sum1,Valid;
	uint32	i,j,Len;
	//////////////////////////////////////////	
	Len = strlen(pGPS);
	Valid = 0;
	JT808Flag.VS1.Bits.GPS = 0;		
	GPSValidFlag = GPS_INVALID_FLAG;
	i = 1;		 // 不计算'$'
	Sum = 0;
	c = 0;
	while( (c!='*')&&(i<Len) )
	{		
		Sum ^= c;
		c = *(pGPS+i);					
		i++;
		if((c=='A')||(c=='V'))
		{
			if(i<25)	Valid = c;
		}
	}
	c = *(pGPS+i);		i++;
	Sum1 = HalfByteHexCharToInt(c);
	Sum1 <<= 4;
	c = *(pGPS+i);		i++;
	Sum1 |= HalfByteHexCharToInt(c);
	
	//if(((Sum!=Sum1)&&(Len<50))||(Valid!='A'))
	if(((Sum!=Sum1)&&(Len<20))||(Valid!='A'))
	{
		//if( (Sum!=Sum1) || (Len<50) )
		if( (Sum!=Sum1) || (Len<20) )
			SystemLog.GPS.DataErrorCnt++;	

		return ERROR;				// 
	}
	///////////////////////////////////////// 数据有效，计算各组数据位置
	i = j = 0;
	c = *pGPS;
	while((c != '*')&&(c != '\0'))
	{
		if(c == ',')
		{
			PS[j++] = i+1;
		}
		i++;
		c = *(pGPS+i);			
	}
	GPSValidFlag = GPS_VALID_FLAG;
	JT808Flag.VS1.Bits.GPS = 1;	
	return OK;	
}


/*
$GPGSV,4,1,16,02,26,295,,03,25,046,,05,68,009,,11,68,009,*70
$GPGSV,4,2,16,13,38,128,28,14,38,128,49,16,25,215,49,18,56,221,*78
$GPGSV,4,3,16,20,26,295,49,21,68,009,,23,13,312,27,25,25,046,50*79
$GPGSV,4,4,16,29,28,082,49,30,56,221,51,31,68,009,49,32,43,261,36*73
$BDGSV,3,1,11,201,37,145,39,202,37,215,42,203,43,188,43,204,26,123,39*6B
$BDGSV,3,2,11,205,17,114,41,206,38,221,42,208,56,299,42,209,30,059,42*6E
$BDGSV,3,3,11,210,16,117,43,211,44,285,42,212,41,207,42*60
*/
// 返回：
uint32			GPSCls::GetSatelliteStatus(char *pSatel)
{
	union
	{
		uint32	B32;
		struct
		{
			uchar		GPS;		 	// GPS
			uchar		BD;			// 北斗
			uchar		Glo;
			uchar		Geli;
		}N;
	}Num;
	
	// 显示用
	if( BD_SatelNum )
	{
		NumberOfSatlice =   ((BD_SatelNum/10)+'0')<<8;			//( (*(pS+11)) << 8);
		NumberOfSatlice |=  ((BD_SatelNum%10)+'0');					////(*(pS+12) );
	}
	else 
	{
		NumberOfSatlice =   ((GPS_SatelNum/10)+'0')<<8;			//( (*(pS+11)) << 8);
		NumberOfSatlice |=  ((GPS_SatelNum%10)+'0');					////(*(pS+12) );
	}
   Num.N.GPS = GPS_SatelNum;
	Num.N.BD = BD_SatelNum;

	return Num.B32;
	
}	// End of





/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：
   功能描述：
   入口参数：
   出口参数：
   全局变量: 
   创建时间：
   修改时间： 
$GPTXT,01,01,02,ANTSTATUS=OPEN*2B \x0D 09:21:18★ \x0A
$GPTXT,01,01,02,ANTSTATUS=OK*3B \x0D 09:21:22★ \x0A
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
#define	NORAM_GPS_TYPE			0
void			GPSCls::GPS_AntCheck(char *pANT)
{
	//static	uint32	LastTime;
	//static	uchar		flag;
	uint32		Short,Open,AntOK;
	uchar			Index;
	
	#if(GPS_ANT_CHECK_COMMAND)	
	Short = Open = AntOK = 0;		// 默认正常
	if( MyStrStr(pANT,"OPEN\0") )
		Open = TRUE;
	if( MyStrStr(pANT,"SHORT\0") )
		Short = TRUE;
	if( MyStrStr(pANT,"OK\0") )
		AntOK = TRUE;
	#else
	uint32	Data;
	Short = Open = 0;		// 默认正常
	GET_GPS_ANT_INIT;	  //	return;

	#if (NORAM_GPS_TYPE)
	Data = GET_GPS_ANT_OPEN_STATUS;
	if((Data == 0) && (!(JT808_Alarm_Word.Shield & (0x01 << 5))))
		Open = TRUE;
	Data = GET_GPS_ANT_SHORT_STATUS;
	if((Data != 0)	&& (!(JT808_Alarm_Word.Shield & (0x01 << 6))))
		Short = TRUE;
	
	#else 				 // 北斗

	Data = GET_GPS_ANT_OPEN_STATUS;
	if((Data != 0) && (!(JT808_Alarm_Word.Shield & (0x01 << 5))))
	{
		Data = GET_GPS_ANT_SHORT_STATUS;
		if(Data != 0)
			Open = TRUE;
	}

	Data = GET_GPS_ANT_OPEN_STATUS;
	if((Data == 0) && (!(JT808_Alarm_Word.Shield & (0x01 << 6))))
	{
		Data = GET_GPS_ANT_SHORT_STATUS;
		if(Data == 0)
			Short = TRUE;
	}
	#endif 

	#endif	

	if(Open)		// 天线开路
	{
		//if(VehicleStatus.VS4.Bits.GPS_ANT_Open != 1)
		if(JT808Flag.VS5.Bits.AntOpen != 1)
		{
			//GPRS_TempPack = TRUE;
			JT808_GPRS_TempPack = TRUE;
			Uart.puts("TP ANT Open\r\n\0");  
			SelfTestResust |= eST_ER_ANT_OPEN;
			VehicleStatus.VS4.Bits.GPS_ANT_Open = 1;
			JT808Flag.VS5.Bits.AntOpen = 1;
	
			Index = 5;
		  	if (JT808_Alarm_Word.SendTextSMS & (0x01 << Index))		     								// 发送短信
		  	{
			 	GSM.SetSMS_Buffer((char *)&JT808_Number.TextSMS[0],"Antennae Open Alarm!!!");		
		  	}
		  	#if ( TEST_CODE_JUST_FOR_TEST_LUOYANG != 1)
			if (JT808_Alarm_Word.TakePhoto & (0x01 << Index))
			{
	  			if (!(JT808_Alarm_Word.SavePhoto & (0x01 << Index)))	Jt_photo_upload = 1;		  // 上传到平台
				MediaEventFlag =eBB_MEDIA_EVENT_OTHER;
	  			Camera.SetTakeNumber(2,TRUE);		  // 拍照 
	  		}
			#endif	// 应付测试中心旧软件，不拍照
		}
	}
	else if(AntOK)
	{ 
	   //if(VehicleStatus.VS4.Bits.GPS_ANT_Open == 1)		  //GPS天线从开路中恢复发送一次标准信息
	    if(JT808Flag.VS5.Bits.AntOpen == 1)
		 {
	    	SelfTestResust &= ~eST_ER_ANT_OPEN;
			JT808_GPRS_TempPack = TRUE; 
			Uart.puts("TP ANT Open to OK-TP\r\n\0");      
	    }
	    VehicleStatus.VS4.Bits.GPS_ANT_Open = 0;
		 JT808Flag.VS5.Bits.AntOpen = 0;	
	}



	if(Short)		// 天线短路
	{
		//if(VehicleStatus.VS4.Bits.GPS_ANT_Short != 1)
		if(JT808Flag.VS5.Bits.AntShort != 1)
		{
			//GPRS_TempPack = TRUE;
			JT808_GPRS_TempPack = TRUE;
			Uart.puts("TP.ANT Shoft");
			VehicleStatus.VS4.Bits.GPS_ANT_Short = 1;
			JT808Flag.VS5.Bits.AntShort = 1;
			SelfTestResust |= eST_ER_ANT_SHORT;
			Index = 6;
		  	if (JT808_Alarm_Word.SendTextSMS & (0x01 << Index))		     								// 发送短信
		  	{
			 	GSM.SetSMS_Buffer((char *)&JT808_Number.TextSMS[0],"Antennae Short Alarm!!!");		
		  	}
		  	//if (JT808_Alarm_Word.TakePhoto & (0x01 << Index))	Camera.SetTakeNumber(2,TRUE);		  // 拍照
			#if ( TEST_CODE_JUST_FOR_TEST_LUOYANG != 1)
			if (JT808_Alarm_Word.TakePhoto & (0x01 << Index))
			{
	  			if (!(JT808_Alarm_Word.SavePhoto & (0x01 << Index)))	Jt_photo_upload = 1;		  // 上传到平台
				MediaEventFlag =eBB_MEDIA_EVENT_OTHER;
	  			Camera.SetTakeNumber(2,TRUE);		  // 拍照 
	  		}
			#endif	// 应付测试中心旧软件，不拍照
		}	

	
		AntShortTimeCnt++;
		if(AntShortTimeCnt>5) 	// 天线短路10秒，关闭GPS电源 (3秒后会自动打开）
			GPS_PowerOn = 0;
	}
	else if(AntOK)
	{
	    //if(VehicleStatus.VS4.Bits.GPS_ANT_Short == 1)		  //GPS天线从短路中恢复发送一次标准信息
	     if(JT808Flag.VS5.Bits.AntShort == 1)
		  {
	   	 SelfTestResust &= ~eST_ER_ANT_SHORT;
			 JT808_GPRS_TempPack = TRUE; 
			 Uart.puts("TP.ANT Shift to OK-TP");
		  }	
		VehicleStatus.VS4.Bits.GPS_ANT_Short = 0;
		JT808Flag.VS5.Bits.AntShort = 0;
		AntShortTimeCnt = 0;		
	}
	NOP();	NOP();

}



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：LonLatToUint32
   功能描述：把RMC格式的经度或纬度转换为放大了10^7倍的整数(度)
   入口参数：GPS字符串指针，
   出口参数：长整数
   全局变量: 
   创建时间：
   修改时间：
   注：函数“DecCharToUint32”，不能使用Str2Int或atoi代替，因为为处理小数点
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
uint32			GPSCls::GPRMC_LonLatToDu32(int32 Flag,char *ptGPS, GPS_DMS *ptDms)
{
	uchar		i;
	int32		tl;
	NOP();	NOP();
	for(i=0;i<5;i++)
	{
		if(*(ptGPS+i)=='.')	
		{ break;	}
	}
	if(i<3)	return 0x7fffffff;					// 经纬度没有0x7fffffff;
	ptDms->D = DecCharToUint32((i-2),(char*)ptGPS);		// 度的整数部分	
	ptDms->M = atoi((char *)(ptGPS+i-2));					// 分的整数部分
	ptDms->S = DecCharToUint32(4,(char *)(ptGPS+i+1));	// 分的小数部分	
	
	ptDms->MMM = (ptDms->D*60 + ptDms->M)*10000 + ptDms->S;	// 全部化为分(放大了10^4倍)
		
	tl  =  DecCharToUint32((2+1+4),(ptGPS+i-2));	// 2位整数+4位小数(放大了10000倍)
	tl *= 1000;												// 放大了1000倍
	tl /= 60;												// 度的小数部分，总共放大了10^7倍
	
	tl += ((ptDms->D)*10000000L);
	
	if(Flag<0)		// 
	{	// 西经或南纬为负
		ptDms->MMM |= 0x80000000;
		tl |= 0x80000000;
	}
	
	#if (0)	
	ptDms->dbD = tl;		// 浮点数，度
	ptDms->dbD /= 10000000L;
	#endif	
	return tl;
}

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：Calculate2DotDistance
   功能描述：计算两点经纬度的距离
   入口参数：两点经纬度坐标，放大10^7倍的度)
   出口参数：长整数距离，单位为米
   全局变量: 
   创建时间：
   修改时间：

$GPRMC,100433.124,A,2251.0206,N,10816.1908,E,0.00,271.31,060406,,*0D
*/ 

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
uint32		GPSCls::Calculate2DotDistance(int32 Lon1,int32 Lat1,int32 Lon2,int32 Lat2)
{	
	uint32	tl,LongUnit;
	uint32	Longitude,Latitude;
	long long 	tp1;
	// 入口参数为放大10^7倍的度，这里要转换为放大了10^4的分，所以先乘以60，再除以1000
	tp1 = Lon1;		tp1 *= 60;		Lon1 = tp1/1000;
	tp1 = Lat1;		tp1 *= 60;		Lat1 = tp1/1000;
	tp1 = Lon2;		tp1 *= 60;		Lon2 = tp1/1000;
	tp1 = Lat2;		tp1 *= 60;		Lat2 = tp1/1000;
	
	////////////////////////////////////////////////////////////////////////////////
	
	/*  用近似算法算出两点经纬度之间的距离，近似为平面（实为球面）
		 赤道处的经度 1853米/分，越靠北极或南极时，每分经度的距离越来越小，每3分小1米
		 纬度3706米/分，到处一样
	*/
	// 计算纬度距离，单位为米	
	Latitude = ((abs(Lat1 - Lat2))*3706/10000UL);
	
	tl = Lat1 + Lat2;
	tl >>= 1;		// 除以2		// 取两点中间纬度
	tl /= 10000;	// 只取分的整数部分
	LongUnit = 1853 - (tl/3);
	// 先按赤道处的单位距离计算，单位为米
	Longitude = ((abs(Lon1 - Lon2))*LongUnit/10000UL);
   		
	// add by xzb

	if (Lon1 == Lon2)				// 经度相同,两点距离为两点纬度距离
	{
		tl = Latitude;
	}
	else if (Lat1 == Lat2)		// 纬度相同，两点距离为两经度距离
	{
		tl = Longitude;
	}
	else		 // 解三角形，算出两点经纬度的距离	// 累加距离
	{	
		tl = sqrt((Longitude*Longitude  + Latitude*Latitude) * 1.0);	
	}
	// end

	return  tl;
	
}



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：Calculate2DotSphericalDistance
   功能描述：计算两点经纬度球面距离
   入口参数：两点经纬度坐标(双精度浮点数)
   出口参数: 
   全局变量: 
   创建时间：
   修改时间：
*/ 

#define			PI			3.14159265358979
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
uint32			GPSCls::Calculate2DotSphericalDistance(double	Lon1,double Lat1,double	Lon2,double Lat2)
{
	#if(0)
	double	Angle,s1,s2,c1,c2,c3;
	NOP();	NOP();
	s1 = sin(Lat1*PI/180);		//
	s2 = sin(Lat2*PI/180);
	c1 = cos(Lat1*PI/180);
	c2 = cos(Lat2*PI/180);
	c3 = cos((Lon1-Lon2)*PI/180);	
	Angle = acos ( s1*s2 + c1*c2*c3 );		// 
	return (Angle*6371004L);					// 返回	
	#else
	return 0;
	#endif
}               
					


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
  函数名称：GPS_GY_MMM_ToString
  功能描述：把MMM格式的GPS数据转换成RMC格式的数据
  入口参数: MMM数据，字符串数据
  出口参数：无
  全局变量: 无
  创建时间：2007-7-7 11:24
  修改时间：
*/ 	
void				Int16ToDecChar(uint16 Data,char *pBuffer);
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void				GPSCls::GY_MMM_ToRMC(uint32 MMM,uchar *pRmc)
{
	uint32	tl;
	uchar		Len,d,m;	
	NOP();	NOP();
	tl = MMM/10000;				// MMM是放大了10^4倍，除以10000，得到度*60+分
	
	d = tl/60;						// 度
	m = tl%60;						// 分	
	
	Int16ToDecChar(d,(char*)pRmc);
	Len = strlen((char*)pRmc);
	
	*(pRmc+Len) = m/10+'0';		Len++;
	*(pRmc+Len) = m%10+'0';		Len++;
	
	*(pRmc+Len) = '.';			Len++;
		
	tl = MMM%10000;					// 分的小数部分
	
	*(pRmc+Len) = tl/1000+'0';			Len++;
	*(pRmc+Len) = tl%1000/100+'0';	Len++;
	*(pRmc+Len) = tl%100/10+'0';		Len++;
	*(pRmc+Len) = tl%10+'0';			Len++;
	
}


#define		USE_CONST_RECT				0

#define		USE_CONST_ROAD_LINE		0

uchar			EnableErrorModify = 0;


#define		LEN_RF_ALARM_MSG			76
#define		LEN_RL_ALARM_MSG			72


#if(USE_CONST_RECT==1)

const		uint32		EnterRectFenceLat[4] = {0x0261eb2f*10,0x02621aad*10,0x0261fb03*10,0x02620ad8*10};
const		uint32		ExitRectFenceLat[3] 	= {0x02619c07*10,0x0261bbb0*10,0x0261abdb*10};

const		uint32		EnterRectFenceTime[4] = {0x18C8827E,0x18C88224, 0x18C88260,0x18C88242};
const		uint32		ExitRectFenceTime[3]	 = {0x18C88314,0x18C882D8,0x18C882F6};
uchar		SendGnssDataEnable;
#else
uchar		LenInOutLoc,SendGnssDataEnable;
uchar		SaveInOutRectFenceLoc[128];
#endif



#if(USE_CONST_ROAD_LINE==1)
// 13 03 05 18 15 32  本地时间0x18C8F2C4，GPS时间，13 03 05 10 15 32		0x18C88244
// 13 03 05 18 15 54  本地时间0x18C8F2DA，GPS时间，13 03 05 10 15 54		0x18C8825A
const		uchar		RoadLineAlarmMsg[13][LEN_RL_ALARM_MSG+1] = 
{
{0x00,0x80,0x00,0x00,0x00,0x0C,0x20,0x03,0x02,0x62,0x0A,0x32,0x06,0xEA,0x05,0x92,0x00,0x2F,0x02,0x21,0x00,0xB1,0x13,0x03,0x05,0x18,0x15,0x32,0x01,0x04,0x00,0x00,0xDB,0xE9,0x02,0x02,0x01,0xC4,0x03,0x02,0x00,0x00,0x12,0x06,0x04,0x00,0x00,0x00,0x64,0x01,0x25,0x04,0x00,0x00,0x00,0x00,0x2A,0x02,0x00,0x00,0x2B,0x04,0x09,0x01,0x6B,0xC6,0x30,0x01,0x1F,0x31,0x01,0x0B},
{0x00,0x80,0x00,0x00,0x00,0x0C,0x20,0x03,0x02,0x62,0x0A,0x97,0x06,0xEA,0x15,0x54,0x00,0x2E,0x02,0x1F,0x00,0x59,0x13,0x03,0x05,0x18,0x15,0x54,0x01,0x04,0x00,0x00,0xDB,0xEB,0x02,0x02,0x01,0xC4,0x03,0x02,0x00,0x00,0x12,0x06,0x04,0x00,0x00,0x00,0x64,0x01,0x25,0x04,0x00,0x00,0x00,0x00,0x2A,0x02,0x00,0x00,0x2B,0x04,0x09,0x48,0x6A,0xFC,0x30,0x01,0x1F,0x31,0x01,0x0B},
{0x00,0x80,0x00,0x00,0x00,0x0C,0x20,0x03,0x02,0x62,0x0A,0xA0,0x06,0xEA,0x29,0xE1,0x00,0x2F,0x02,0x1E,0x00,0x5A,0x13,0x03,0x05,0x18,0x16,0x24,0x01,0x04,0x00,0x00,0xDB,0xEF,0x02,0x02,0x01,0xC4,0x03,0x02,0x00,0x00,0x12,0x06,0x04,0x00,0x00,0x00,0x64,0x01,0x25,0x04,0x00,0x00,0x00,0x00,0x2A,0x02,0x00,0x00,0x2B,0x04,0x09,0x59,0x6A,0xE1,0x30,0x01,0x1F,0x31,0x01,0x0B},
{0x00,0x80,0x00,0x00,0x00,0x0C,0x20,0x03,0x02,0x62,0x0A,0xAD,0x06,0xEA,0x3E,0x72,0x00,0x30,0x02,0x1C,0x00,0x59,0x13,0x03,0x05,0x18,0x16,0x54,0x01,0x04,0x00,0x00,0xDB,0xF2,0x02,0x02,0x01,0xC4,0x03,0x02,0x00,0x00,0x12,0x06,0x04,0x00,0x00,0x00,0x64,0x01,0x25,0x04,0x00,0x00,0x00,0x00,0x2A,0x02,0x00,0x00,0x2B,0x04,0x08,0x37,0x6B,0x16,0x30,0x01,0x1F,0x31,0x01,0x0B},
{0x00,0x80,0x00,0x00,0x00,0x0C,0x20,0x03,0x02,0x62,0x0A,0x9B,0x06,0xEA,0x53,0x0A,0x00,0x2F,0x02,0x1C,0x00,0x59,0x13,0x03,0x05,0x18,0x17,0x24,0x01,0x04,0x00,0x00,0xDB,0xF5,0x02,0x02,0x01,0xC4,0x03,0x02,0x00,0x00,0x12,0x06,0x04,0x00,0x00,0x00,0x64,0x01,0x25,0x04,0x00,0x00,0x00,0x00,0x2A,0x02,0x00,0x00,0x2B,0x04,0x09,0xCC,0x6B,0x16,0x30,0x01,0x1F,0x31,0x01,0x0B},
{0x00,0x80,0x00,0x00,0x00,0x0C,0x20,0x03,0x02,0x61,0xFE,0x58,0x06,0xEA,0x57,0xC2,0x00,0x2F,0x02,0x14,0x00,0xB3,0x13,0x03,0x05,0x18,0x17,0x54,0x01,0x04,0x00,0x00,0xDB,0xFD,0x02,0x02,0x01,0xC4,0x03,0x02,0x00,0x00,0x12,0x06,0x04,0x00,0x00,0x00,0x64,0x01,0x25,0x04,0x00,0x00,0x00,0x00,0x2A,0x02,0x00,0x00,0x2B,0x04,0x09,0x13,0x6A,0xD8,0x30,0x01,0x1F,0x31,0x01,0x0B},
{0x00,0x80,0x00,0x00,0x00,0x0C,0x20,0x03,0x02,0x61,0xEE,0x83,0x06,0xEA,0x57,0xC4,0x00,0x2F,0x02,0x1B,0x00,0xB4,0x13,0x03,0x05,0x18,0x18,0x24,0x01,0x04,0x00,0x00,0xDC,0x06,0x02,0x02,0x01,0xC4,0x03,0x02,0x00,0x00,0x12,0x06,0x04,0x00,0x00,0x00,0x64,0x01,0x25,0x04,0x00,0x00,0x00,0x00,0x2A,0x02,0x00,0x00,0x2B,0x04,0x09,0x2D,0x6A,0xA4,0x30,0x01,0x1F,0x31,0x01,0x0B},
{0x00,0x80,0x00,0x00,0x00,0x0C,0x20,0x03,0x02,0x61,0xDE,0xE0,0x06,0xEA,0x57,0xB0,0x00,0x32,0x02,0x1D,0x00,0xB4,0x13,0x03,0x05,0x18,0x18,0x54,0x01,0x04,0x00,0x00,0xDC,0x0F,0x02,0x02,0x01,0xC4,0x03,0x02,0x00,0x00,0x12,0x06,0x04,0x00,0x00,0x00,0x64,0x01,0x25,0x04,0x00,0x00,0x00,0x00,0x2A,0x02,0x00,0x00,0x2B,0x04,0x09,0x3F,0x6B,0xBD,0x30,0x01,0x1F,0x31,0x01,0x0B},
{0x00,0x80,0x00,0x00,0x00,0x0C,0x20,0x03,0x02,0x61,0xCF,0x00,0x06,0xEA,0x57,0xB5,0x00,0x31,0x02,0x20,0x00,0xB3,0x13,0x03,0x05,0x18,0x19,0x24,0x01,0x04,0x00,0x00,0xDC,0x18,0x02,0x02,0x01,0xC4,0x03,0x02,0x00,0x00,0x12,0x06,0x04,0x00,0x00,0x00,0x64,0x01,0x25,0x04,0x00,0x00,0x00,0x00,0x2A,0x02,0x00,0x00,0x2B,0x04,0x08,0xA0,0x6A,0x66,0x30,0x01,0x1F,0x31,0x01,0x0B},
{0x00,0x80,0x00,0x00,0x00,0x0C,0x20,0x03,0x02,0x61,0xCA,0xFC,0x06,0xEA,0x48,0x2C,0x00,0x30,0x02,0x1C,0x01,0x0E,0x13,0x03,0x05,0x18,0x19,0x54,0x01,0x04,0x00,0x00,0xDC,0x1C,0x02,0x02,0x01,0xC4,0x03,0x02,0x00,0x00,0x12,0x06,0x04,0x00,0x00,0x00,0x64,0x01,0x25,0x04,0x00,0x00,0x00,0x00,0x2A,0x02,0x00,0x00,0x2B,0x04,0x09,0x24,0x6A,0x92,0x30,0x01,0x1F,0x31,0x01,0x0B},
{0x00,0x80,0x00,0x00,0x00,0x0C,0x20,0x03,0x02,0x61,0xCB,0x0D,0x06,0xEA,0x33,0x93,0x00,0x30,0x02,0x23,0x01,0x0E,0x13,0x03,0x05,0x18,0x20,0x24,0x01,0x04,0x00,0x00,0xDC,0x20,0x02,0x02,0x01,0xC4,0x03,0x02,0x00,0x00,0x12,0x06,0x04,0x00,0x00,0x00,0x64,0x01,0x25,0x04,0x00,0x00,0x00,0x00,0x2A,0x02,0x00,0x00,0x2B,0x04,0x09,0x2D,0x6B,0x39,0x30,0x01,0x1F,0x31,0x01,0x0B},
{0x00,0x80,0x00,0x00,0x00,0x0C,0x20,0x03,0x02,0x61,0xCA,0xF9,0x06,0xEA,0x1F,0x0A,0x00,0x30,0x02,0x1C,0x01,0x0D,0x13,0x03,0x05,0x18,0x20,0x54,0x01,0x04,0x00,0x00,0xDC,0x23,0x02,0x02,0x01,0xC4,0x03,0x02,0x00,0x00,0x12,0x06,0x04,0x00,0x00,0x00,0x64,0x01,0x25,0x04,0x00,0x00,0x00,0x00,0x2A,0x02,0x00,0x00,0x2B,0x04,0x08,0xD5,0x6B,0x16,0x30,0x01,0x1F,0x31,0x01,0x0B},
{0x00,0x80,0x00,0x00,0x00,0x0C,0x20,0x03,0x02,0x61,0xCB,0x0B,0x06,0xEA,0x0A,0x6F,0x00,0x31,0x02,0x1B,0x01,0x0E,0x13,0x03,0x05,0x18,0x21,0x24,0x01,0x04,0x00,0x00,0xDC,0x26,0x02,0x02,0x01,0xC4,0x03,0x02,0x00,0x00,0x12,0x06,0x04,0x00,0x00,0x00,0x64,0x01,0x25,0x04,0x00,0x00,0x00,0x00,0x2A,0x02,0x00,0x00,0x2B,0x04,0x09,0x7C,0x6A,0x89,0x30,0x01,0x1F,0x31,0x01,0x0B},
};

#endif



//const		uchar		RoadLineStartAlarmMsg[LEN_RL_ALARM_MSG+1] = {0x00,0x80,0x00,0x00,0x00,0x0C,0x20,0x03,0x02,0x62,0x0A,0x32,0x06,0xEA,0x05,0x92,0x00,0x2F,0x02,0x21,0x00,0xB1,0x13,0x03,0x05,0x18,0x15,0x32,0x01,0x04,0x00,0x00,0xDB,0xE9,0x02,0x02,0x01,0xC4,0x03,0x02,0x00,0x00,0x12,0x06,0x04,0x00,0x00,0x00,0x64,0x01,0x25,0x04,0x00,0x00,0x00,0x00,0x2A,0x02,0x00,0x00,0x2B,0x04,0x09,0x01,0x6B,0xC6,0x30,0x01,0x1F,0x31,0x01,0x0B};



/************************************************************************************************
** 函数名称：CheckEleRail()
** 函数功能：电子围栏检测
** 入口参数：经纬度
** 出口参数：电子围栏号(no不为0时,表示在第no号围栏内;no为0时,表示不在所有的围栏内)
** 全局变量：
** 修改时间：
*************************************************************************************************/
void			GPSCls::CheckRectangleFence(int32 iLon,int32 iLat)
{		
	#define		LEN_TP_BUF		160
	
	uint16	i; 
	uchar		t,Len=0;
	union
	{
		uchar		Buffer[LEN_TP_BUF];
		uint16	B16[LEN_TP_BUF/2];
		uint32	B32[LEN_TP_BUF/4];
	}Tp;
	union
	{
		uchar		Buffer[256];
		uint16	B16[128];
		uint32	B32[64];		
	}tPage; 
	uint32		Addr;
	uchar			Enable,EnableCnt,k;
	pELE_FENCE_STRUCT		pFence=NULL;
	
	
	if((GPSValidFlag!=GPS_VALID_FLAG))
		return ;
	
	if(RectFenceAttr.Total==0)																   // 有围栏才检测		
	{
		VehicleStatus.VS4.Bits.InAreaAlarm = 0;
		VehicleStatus.VS4.Bits.OutAreaAlarm = 0;
		JT808Flag.VS7.Bits.InOutArea = 0;
		if(RectFenceAttr.CurrentNo)
		{
			SetBufferValue( (uchar*)&RectFenceAttr,LEN_EE_FENCE_ATTR,0);
		}
		if(TD.GPS==tGPS_RECT_FENCE)		Uart.puts("未设置矩形电子围栏 ");
		return ;
	}
	
	
	
	SetBufferValue( Tp.Buffer,LEN_TP_BUF,0);
	if(RectFenceAttr.CurrentNo==0)
	{		
		EnableCnt=0;
		for(i=0;i<RectFenceAttr.Total;i++)
		{
			Enable = 0;
			for(;EnableCnt<MAX_FENCE_NUM;EnableCnt++)
			{
				k = EnableCnt>>3;		// 除以8
				t = EnableCnt%8;
				if(RectFenceAttr.Enable[k]&(1<<t))	
				{
					Enable = TRUE;		break;
				}
			}
			NOP();	NOP();
			if( Enable != TRUE )	
			{
				if(TD.GPS==tGPS_RECT_FENCE)		Uart.puts("未使能矩形围栏有效标志 ");
				break;	 	// 未找到使能的围栏
			}
			Addr = FF_RECTANGLE_FENCE_START_PAGE;
			Addr += (EnableCnt/8);
			DFF.PageRead(Addr,tPage.Buffer);
			
			if(TD.GPS==tGPS_RECT_FENCE)
			{
				SetBufferValue( Tp.Buffer,80,0);
				strcpy((char*)Tp.Buffer,"页地址：");
				Int2Str( Addr,(char*)&Tp.Buffer[8]);
				strcat((char*)Tp.Buffer,"，围栏号：");
				Len = strlen( (char*)Tp.Buffer);
				Int2Str( EnableCnt,(char*)&Tp.Buffer[Len]);
				strcat((char*)Tp.Buffer," ");
				Uart.puts((char*)Tp.Buffer);
			}
			
			k = EnableCnt%8;
			pFence = (pELE_FENCE_STRUCT)&tPage.Buffer[k*LEN_ELE_FENCE_STRUCT];
			EnableCnt++;		//								
			
			/////////////////////////// 左上角和右下角，在边上算出围栏（在边上不算进围栏）
			if(  (iLon>(pFence->Lon1))&&(iLon<(pFence->Lon2))  &&   (iLat>(pFence->Lat2))&&(iLat<(pFence->Lat1))   )				
			{
				// EnableCnt从0开始，而当前围栏号从1开始，所以先加1，再赋值
				RectFenceAttr.CurrentNo 	= EnableCnt;
				RectFenceAttr.RemoteID	= pFence->ID;		// 原设置ID
				RectFenceAttr.Lon1 		= pFence->Lon1;
				RectFenceAttr.Lat1 		= pFence->Lat1;
				RectFenceAttr.Lon2 		= pFence->Lon2;
				RectFenceAttr.Lat2 		= pFence->Lat2;
				RectFenceAttr.InTimeCnt	= 0;
				RectFenceAttr.OutTimeCnt= 0;
				
				DFE.Write(EE_RECTANGLE_FENCE_ATTR,LEN_EE_FENCE_ATTR,(uchar*)&RectFenceAttr);
				
				InOutAreaType = eBB_AREA_ALARM_RECT;			// 报警使用
				InOrOutArea = eBB_IN_AREA;
				FenceOrRoadAlarmID = RectFenceAttr.RemoteID;
				
				SendGnssDataEnable = 0;
				
				TestRectFence = TRUE;	
				
				
				JT808Flag.VS7.Bits.InOutArea = 1;			
				
				SetBufferValue( Tp.Buffer,LEN_TP_BUF,0);
				strcpy((char*)Tp.Buffer,"进入");
				Int2Str( RectFenceAttr.CurrentNo,(char*)&Tp.Buffer[4]);
				strcat((char*)Tp.Buffer,"号矩形围栏，原ID：");	
				Len = strlen( (char*)Tp.Buffer);
				Int2Str( RectFenceAttr.RemoteID,(char*)&Tp.Buffer[Len]);
				strcat((char*)Tp.Buffer," ");	
				Uart.puts((char*)Tp.Buffer);
				
				break;													
			}
		}
		if(RectFenceAttr.CurrentNo==0)
		{			
			if(RectFenceAttr.OutTimeCnt)	
			{
				if( (RectFenceAttr.OutTimeCnt==3) )	  	// 出围栏三秒后，发送报警信息（进围栏时刻信息）
				{
					Uart.puts("上报出围栏报警信息\r\n\0");
					GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_POS_REPORT,0,NULL);
				}				
				RectFenceAttr.InTimeCnt = 0;
				RectFenceAttr.OutTimeCnt++;					
							
			}
			if(TD.GPS==tGPS_RECT_FENCE)	
			{
				SetBufferValue(Tp.Buffer,LEN_TP_BUF,0);
				strcpy((char*)Tp.Buffer,"\r\n未检测到矩形围栏,Lon1: ");
				Len = strlen( (char*)Tp.Buffer);
				Int2Str( pFence->Lon1,(char*)&Tp.Buffer[Len]);

				strcat((char*)Tp.Buffer,",Lat1: ");	
				Len = strlen( (char*)Tp.Buffer);
				Int2Str( pFence->Lat1,(char*)&Tp.Buffer[Len]);

				strcat((char*)Tp.Buffer,",Lon2: ");	
				Len = strlen( (char*)Tp.Buffer);
				Int2Str( pFence->Lon2,(char*)&Tp.Buffer[Len]);

				strcat((char*)Tp.Buffer,",Lat2: ");	
				Len = strlen( (char*)Tp.Buffer);
				Int2Str( pFence->Lat2,(char*)&Tp.Buffer[Len]);

				strcat((char*)Tp.Buffer,",当前坐标,Lon: ");	
				Len = strlen( (char*)Tp.Buffer);
				Int2Str( iLon,(char*)&Tp.Buffer[Len]);

				strcat((char*)Tp.Buffer,",Lat: ");	
				Len = strlen( (char*)Tp.Buffer);
				Int2Str( iLat,(char*)&Tp.Buffer[Len]);

				Uart.puts((char*)Tp.Buffer);
			}
		}
	}
	else	//if(RectFenceAttr.Current)
	{		// 左上角和右下角，在边上算出围栏（在边上不算进围栏）
		if( (iLon>=RectFenceAttr.Lon1)&&(iLon<=RectFenceAttr.Lon2)
			&&(iLat>=RectFenceAttr.Lat2)&&(iLat<=RectFenceAttr.Lat1)  )
		{
			RectFenceAttr.InTimeCnt++;
			
			if(RectFenceAttr.InTimeCnt==3)	// 进围栏三秒后，发送报警信息（进围栏时刻信息）
			{				
				//SendGnssDataEnable = TRUE;		//  
				//JT808Flag.VS7.Bits.InOutArea = TRUE;	
				GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_POS_REPORT,0,NULL);
				Uart.puts("上报进围栏报警信息\r\n\0");
			}
			
			//RectFenceAttr.OutTimeCnt = 0;	进入时清零
			if(TD.GPS==tGPS_RECT_FENCE)
			{
				strcpy((char*)Tp.Buffer,"当前在");
				Int2Str( RectFenceAttr.CurrentNo,(char*)&Tp.Buffer[6]);
				strcat((char*)&Tp.Buffer[Len],"号矩形围栏之内，原ID：");	
				Len = strlen( (char*)Tp.Buffer);
				Int2Str( RectFenceAttr.RemoteID,(char*)&Tp.Buffer[Len]);
				strcat((char*)&Tp.Buffer[Len]," ");	
				Uart.puts((char*)Tp.Buffer);				
			}
		}
		else
		{			
			RectFenceAttr.PreNo = RectFenceAttr.CurrentNo;
			
			InOutAreaType = eBB_AREA_ALARM_RECT;			// 报警使用
			InOrOutArea = eBB_OUT_AREA;
			FenceOrRoadAlarmID = RectFenceAttr.RemoteID;
			
			
			JT808Flag.VS7.Bits.InOutArea = 1;
							
			TestRectFence = 0;
			RectFenceAttr.CurrentNo = 0;
			//RectFenceAttr.InTimeCnt = 0;		// 进入时清零
			RectFenceAttr.OutTimeCnt++;
			Uart.puts("已出围栏 ");	
			
		}
	} 
	
	#undef	LEN_TP_BUF
}



/************************************************************************************************
** 函数名称：CheckRoundEleRail()
** 函数功能：圆形电子围栏检测
** 入口参数：经纬度
** 出口参数：电子围栏号(no不为0时,表示在第no号围栏内;no为0时,表示不在所有的围栏内)
** 全局变量：
** 修改时间：
*************************************************************************************************/
void			GPSCls::CheckRoundEleFence(int32 iLon,int32 iLat)
{		
	
}




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：Calculate2DotDistance_dm
   功能描述：计算两点经纬度的距离 单位：分米
   入口参数：两点经纬度坐标(FFF.FFFF格式，放大10^4倍，无小数) ,单位： 分
   出口参数：长整数距离，单位为米
   全局变量: 
   创建时间：
   修改时间：

$GPRMC,100433.124,A,2251.0206,N,10816.1908,E,0.00,271.31,060406,,*0D
*/ 


uint32        Calculate2DotDistance_dm(int32 Lon1,int32 Lat1,int32 Lon2,int32 Lat2)		// 返回的值的单位为分米
{
    uint32    tl,LongUnit;
    uint32    Longitude,Latitude;
    /*  用近似算法算出两点经纬度之间的距离，近似为平面（实为球面）
         赤道处的经度 1853米/分，越靠北极或南极时，每分经度的距离越来越小，每3分小1米
		 纬度3706米/分，到处一样
	*/

	// 计算纬度距离，单位为分米
	
	Latitude = (Lat1 > Lat2) ? (Lat1-Lat2):(Lat2 - Lat1);
	
	Latitude = Latitude*3706/1000;
	
	tl = Lat1 + Lat2;
	tl >>= 1;		// 除以2		// 取两点中间纬度
	tl /= 10000;	// 只取分的整数部分
	LongUnit = 1853 - (tl/3);
	// 先按赤道处的单位距离计算，单位为分米
	//Longitude = ((abs(Lon1 - Lon2))*LongUnit/1000);
	Longitude = (Lon1 > Lon2) ? (Lon1-Lon2):(Lon2 - Lon1);

	Longitude = Longitude *	LongUnit/1000;
   		
	if (Lon1 == Lon2)				// 经度相同,两点距离为两点纬度距离
	{
		tl = Latitude;

	}
	else if (Lat1 == Lat2)		// 纬度相同，两点距离为两经度距离
	{
		tl = Longitude;
	}
	else		 // 解三角形，算出两点经纬度的距离	// 累加距离
	{	
		tl = sqrt((Longitude*Longitude  + Latitude*Latitude) * 1.0);	
	}
	// end

	return  tl;

	
}








void			GPSCls::PrintRoadLineLonLat(uint32 LonS, uint32 LatS, uint32 LonE, uint32 LatE,uint32 LonQ, uint32 LatQ, uchar RoadWidth)
{
	char		tBuffer[128];
	uint32	tl;
	if(TD.GPS==tGPS_ROAD_LINE_FENCE)
	{
		SetBufferValue((uchar*)tBuffer,128,0);
		
		strcpy( tBuffer,"Rtc: ");
		tl = strlen( tBuffer);
		Int2Str( GYPack.RtcSecond,&tBuffer[tl]);
		
		strcat( tBuffer,",LonS: ");
		tl = strlen( tBuffer);
		Int2Str( LonS,&tBuffer[tl]);
		
		strcat( tBuffer,",LatS: ");
		tl = strlen( tBuffer);
		Int2Str( LatS,&tBuffer[tl]);		
		
		strcat( tBuffer,",LonE: ");
		tl = strlen( tBuffer);
		Int2Str( LonE,&tBuffer[tl]);
		
		strcat( tBuffer,",LatE: ");
		tl = strlen( tBuffer);
		Int2Str( LatE,&tBuffer[tl]);
		
		strcat( tBuffer,",LonQ: ");
		tl = strlen( tBuffer);
		Int2Str( LonQ,&tBuffer[tl]);
		
		strcat( tBuffer,",LatQ: ");
		tl = strlen( tBuffer);
		Int2Str( LatQ,&tBuffer[tl]);
		
		strcat( tBuffer,",Width: ");
		tl = strlen( tBuffer);
		Int2Str( RoadWidth,&tBuffer[tl]);
		
		
		
		strcat( tBuffer,"\r\n");
		Uart.puts(tBuffer);
		
		SetBufferValue((uchar*)tBuffer,128,0);
		
	}
}



/* 直线两点式方程  	(X-x1)/(x2-x1)  =  (Y-y1)/(y2-y1)
   整理得到一般式： 	X(y2-y1) - Y(x2-x1) - x1(y2-y1)+y1(x2-x1) = 0
   所以：	A = y2-y1
   			B = -(x2-x1)  = x1-x2
   			C = y1(x2-x1) - x1(y2-y1)
   点线距离：D =  |AX+BY+C| / (sqrt((A*A + B*B))
*/
/*
uchar 		CheckIsOutRoad_DL_Distance(int32 LonS, int32 LatS, int32 LonE, int32 LatE,int32 LonQ, int32 LatQ, uchar RoadWidth)
{
	double	A,B,C,td;
	uint32	tl;
	// 在经纬度范围外的，直接判断为出路线
	if( LonS < LonE )
	{
		if( (LonQ<LonS)||(LonQ>LonE) )	return TRUE;		
	}
	else if( LonS > LonE )
	{
		if( (LonQ>LonS)||(LonQ<LonE) )	return TRUE;
	}
	if( LatS < LatE )
	{
		if( (LatQ<LatS)||(LatQ>LatE) )	return TRUE;	
	}
	else if( LatS > LatE )
	{
		if( (LatQ>LatS)||(LatQ<LatE) )	return TRUE;	
	}
	
	
	
	if( LonS==LonE )		// 正南北方向的路，只算经度间距离即可
	{
		tl = Calculate2DotDistance(LonQ,LatQ,LonS,LatQ);
		if( tl > RoadWidth)	return TRUE;
		else						return FALSE;
	}
	else if( LatS==LatE)	// 正东西方向的路，只算纬度间距离即可
	{
		tl = Calculate2DotDistance(LonQ,LatQ,LonQ,LatS);
		if( tl > RoadWidth)	return TRUE;
		else						return FALSE;
	}
	else							// 其它方向的路
	{
		A = LatE;	A -= LatS;
		td = LonE;	td -= LonS;
		B = -td;
		C = td*LatS - A*LonS;
		td = abs( A*LonQ + B*LatQ + C ) / sqrt ( A*A + B*B );
		
		/////////////////// 把坐标关系转化为米（原单位为放大10^7的度）
		return TRUE;
		
	}

}	*/





/*
　路线偏移判断思想
　
　１.　以车辆当前坐标为圆心，路宽一半为半径作的圆，是否与两拐点所作的直线有交点
		　没有交点，则可确定车辆偏移了路线;有交点,则还需要进行第２步判断。
　２.　以离车辆最近的拐点作为点对称的中心点，作车辆对称点
		　分别求出车辆到最远拐点的距离和车辆对称点到最远拐点的距离
　３.　如果车辆对称点到最远拐点的距离　不小于　车辆到最远拐点的距离，则可判断
		　车辆在路线内，否则车辆偏移了路线

*/


uchar 		GPSCls::CheckIsOutRoad(uint32 LonS, uint32 LatS, uint32 LonE, uint32 LatE,uint32 LonQ, uint32 LatQ, uchar RoadWidth)
{
		
    // 采用直线与圆相交，判断车辆超过路面，思想：过拐点S，拐点E作直线L，过车辆Q点，以路宽的一半为半径作一个圆，如果该圆与直线有交点，那么车辆还在公路中，否则出公路
    // 具体作法：如果以S点到原始点，过S点，作X轴建立平行纬线，作Y轴平行经线的直角坐标系，则S点坐标为（0，0） X,Y轴的单位为米，Q的坐标为（xQ,yQ),　，过Q点作圆，半径为r(路宽的一半）
    // 圆的方程: (x-xQ)^2 + (y-yQ)^2 = r^2, 点E的坐标为（xE,yE), 直线SE的方程：y=kx;
    // 把y=kx代入圆方程，得到一元二次方程，根据韦达定理，求根公式，可知，如果b^2 - 4ac > 0，则有解，即直线与圆相交 , 
    // 最后得到：(2*xQ+2*yQ*k)^2-4*(1+k^2)*(xQ^2+yQ^2-r^2)
    // 只要比较 (2a+2bk)^2与4(1+k^2)(a^2+b^2-r^2)大小即可，知直线与圆是否相交,从而推出车辆是否超过路线, 在不能算浮点的处理器，要扩大相关的参数 
    
   uint32	tl;
   int xQ, yQ,xE,yE,r;
	long long tp1, tp2, tp3;//,DistQSPower2, DistQEPower2;//,DistQTPower2;
	char tBuffer[128];
	
	// 在经纬度范围外的，直接判断为出路线
	if( LonS < LonE )
	{
		if( (LonQ<(LonS-10000UL))||(LonQ>(LonE+10000UL)) )	return TRUE;		
	}
	else if( LonS > LonE )
	{
		if( (LonQ>(LonS+10000UL))||(LonQ<(LonE-10000UL)) )	return TRUE;
	}
	if( LatS < LatE )
	{
		if( (LatQ<(LatS-3300UL))||(LatQ>(LatE+3300UL)) )	return TRUE;	
	}
	else if( LatS > LatE )
	{
		if( (LatQ>(LatS+3300UL))||(LatQ<(LatE-3300UL)) )	return TRUE;	
	}
	
	Uart.puts("路段范围内，需计算\r\n");
			
	if( LonS==LonE )		// 正南北方向的路，只算经度间距离即可
	{
		tl = Calculate2DotDistance(LonQ,LatQ,LonS,LatQ);
		if( tl > RoadWidth)	return TRUE;
		else						return FALSE;
	}
	else if( LatS==LatE)	// 正东西方向的路，只算纬度间距离即可
	{
		tl = Calculate2DotDistance(LonQ,LatQ,LonQ,LatS);
		if( tl > RoadWidth)	return TRUE;
		else						return FALSE;
	}
				
	// 入口参数为放大10^7倍的度，这里要转换为放大了10^4的分，所以先乘以60，再除以1000
	tp1 = LonS;		tp1 *= 60;		LonS = tp1/1000;
	tp1 = LatS;		tp1 *= 60;		LatS = tp1/1000;
	tp1 = LonE;		tp1 *= 60;		LonE = tp1/1000;
	tp1 = LatE;		tp1 *= 60;		LatE = tp1/1000;
	tp1 = LonQ;		tp1 *= 60;		LonQ = tp1/1000;
	tp1 = LatQ;		tp1 *= 60;		LatQ = tp1/1000;
	
	////////////////////////////////////////////////////////////////////////////////
	//if(TD.GPS==tGPS_ROAD_LINE_FENCE)
	{
		Uart.puts("\r\nS E Q W: ");
		SetBufferValue((uchar*)tBuffer,128,0);
		Int2Str( LonS,tBuffer);	
		strcat( tBuffer," ");
		tl = strlen( tBuffer);
		
		Int2Str( LatS,&tBuffer[tl]);	
		strcat( tBuffer," ");
		tl = strlen( tBuffer);
		
		Int2Str( LonE,&tBuffer[tl]);	
		strcat( tBuffer," ");
		tl = strlen( tBuffer);
		
		Int2Str( LatE,&tBuffer[tl]);	
		strcat( tBuffer," ");
		tl = strlen( tBuffer);
		
		Int2Str( LonQ,&tBuffer[tl]);	
		strcat( tBuffer," ");
		tl = strlen( tBuffer);
		
		Int2Str( LatQ,&tBuffer[tl]);	
		strcat( tBuffer," ");
		tl = strlen( tBuffer);
		
		Int2Str( RoadWidth,&tBuffer[tl]);	
		strcat( tBuffer,"\r\n");
		
		Uart.puts(tBuffer);
	}


   
   ///////////////////////////////////////////////// 在S、E组成的围栏内，才判断是否在路段里
   r = RoadWidth / 2; 			//
	
     // 求出Q点在经纬度到直角坐标系的映射点Q(xQ,yQ)
   tl = xQ = Calculate2DotDistance_dm(LonS, LatS, LonQ,LatS); //　不用平方
	if (LonS > LonQ)   
	{		
		xQ = -xQ;
	}
   tl = yQ = Calculate2DotDistance_dm(LonS, LatS, LonS,LatQ);		//不用平方   
   if (LatS > LatQ)  
	{		 
		yQ = -yQ;
	}
	    
    // 求出Ｅ点在经纬度到直角坐标系的映射点E(xE,yE)
   tl = xE = Calculate2DotDistance_dm(LonS, LatS, LonE,LatS);		//不用平方	
	if (LonS > LonE)   
	{
		xE = -xE;
	}
   tl = yE = Calculate2DotDistance_dm(LonS, LatS, LonS,LatE);   // 不用平方   
	
   if (LatS > LatE)  
	{ 
		yE = -yE;
	}
	   
	r = r * 10;	  // 转为分米，因为yQ等等单位是分米

	// (1+k^2)x - (2x1+2ky1)x + (x1^2+y1^1-r^2) = 0 , 这里y1 = yQ, x1 = yQ, k = yE/xE
    
   // k = yE/xE			当yE小于xE时，整除后是0，所以下面要把yE放大100倍来处理
	// 2*((100*xQ + yQ*(100*yE/xE)))
    tp3 = yQ;		tp3 *= 100;    tp3 *= yE;		tp3 /= xE;		// yQ*(100*yE/xE)
    tp1 = 100;		tp1 *= xQ;		// 100*xQ
    tp1 += tp3;	
    tp1 <<= 1;
        		 
	// b^2													 
    tp1 = tp1 * tp1 / 10000UL;
	 //if(TD.GPS==tGPS_ROAD_LINE_FENCE)
	 {
	 		Uart.puts("tp1 b^2 Test Data:");
			Uart.putb(1, 8, (uchar *)&tp1);	
			Uart.puts("\r\n");
	 } 
	 

    // 4ac	// (1+(yE/xE)^2
    //tp2 = (10000+(100*yE/xE)*(100*yE/xE)) / 10000UL;	
    tp2 = yE;		tp2 *= 100;			tp2 /= xE;		tp2 *= tp2;
    tp2 += 10000UL;
    tp2 /= 10000UL;
        	
    //tp3 = ((xQ*xQ+yQ*yQ-r*r));
    //tp3 = xQ*xQ;		tp3 += (yQ*yQ);		tp3 -= (r*r);
	 tp3 = xQ;	 tp3 *= xQ; 	tp3 += (yQ*yQ);		tp3 -= (r*r);

    tp2 = 4 * tp2 * tp3;

	 //if(TD.GPS==tGPS_ROAD_LINE_FENCE)
	 {
	 		Uart.puts("tp2 4ac Test Data:");
			Uart.putb(1, 8, (uchar *)&tp2);	
			Uart.puts("\r\n");
	 } 

    if (tp1 > tp2)		// 圆与直线有交点，在路线内
    {
			return 0;
    }
    else
    {
      Uart.puts("out of road 7");
		return 1;
    }
    	
}





uchar			OutOfRoadCnt=0;
uchar			CloseDotFlag = 0;

	// 按实际使用处理，有多条路线，存储在Flash里

void			GPSCls::SearchRoadSection(uchar Current,int32 iLon,int32 iLat)
{
	#define		LEN_TP_BUF		160
	uint32	Addr;
	uint16	i,j,k;
	union
	{
		uchar		Buffer[LEN_TP_BUF];
		uint16	B16[LEN_TP_BUF/2];
		uint32	B32[LEN_TP_BUF/4];
	}Tp;
	uchar			*pData;
	uchar			TotalDot,Enable,EnableCnt,t,Len=0;
	pROAD_LINE_STRUCT		pRoad=NULL;
	
	i = j = k = 0;
	
	pData = NULL;
	while(pData==NULL)	{	pData = MyMalloc(512);		os_dly_wait(1); }
	
	RoadAttr.CurrentNo = 0;
	EnableCnt=0;
	for(i=0;i<RoadAttr.Total;i++)		// 每条路线找一遍？？？？？
	{
		if(Current)		// 优先查找最近走过的路线
		{
			EnableCnt = Current-1;
		}
		else
		{
			Enable = 0;
			for(;EnableCnt<MAX_FENCE_NUM;EnableCnt++)
			{
				k = EnableCnt>>3;		// 除以8
				t = EnableCnt%8;
				if(RoadAttr.Enable[k]&(1<<t))	
				{
					Enable = TRUE;		break;
				}
			}
			NOP();	NOP();
			if( Enable != TRUE )	
			{
				if(TD.GPS==tGPS_ROAD_LINE_FENCE)		Uart.puts("未使能路线有效标志 ");
				break;	 	// 未找到使能的围栏
			}
		}
				
		Addr = FF_ROAD_PLAN_START_PAGE;		// 
		Addr += (EnableCnt<<1);					// 每条路线固定存储长度为2页

		
		
		DFF.PageRead( Addr, pData );
		DFF.PageRead( Addr+1,pData+FF_BYTES_PER_PAGE );

		if(TD.GPS==	 tGPS_ROAD_LINE_FENCE)
		{
			Uart.puts("路线在FLASH地址 Test Data:");Uart.putb(1,4,(uchar *)&Addr);	 Uart.puts("\r\n");
			Uart.puts("road data Test Data:");Uart.putb(1, 512,pData); Uart.puts("\r\n"); os_dly_wait(10);
		}
		
		//EnableCnt从0开始，而当前围栏号从1开始，所以先加1，再赋值给当前践线号
		EnableCnt++;
		
		pRoad = (pROAD_LINE_STRUCT)pData;
		
		TotalDot = (pRoad->DotNumber)-1;		// 总路段数 = 拐点数据 - 1
		for(j=0;j<TotalDot;j++)					// 每个路段（拐点）找一遍
		{				
			
			//////////////////////////////////////////// 测试代码
			Uart.puts(" Current Search S E W Test Data:");	
			SetBufferValue( Tp.Buffer,LEN_TP_BUF,0);
			Tp.B32[0] = iLon;						Tp.B32[1] = iLat;		
			Tp.B32[2] = pRoad->Dot[j].Lon;	Tp.B32[3] = pRoad->Dot[j].Lat;
			Tp.B32[4] = pRoad->Dot[j+1].Lon;	Tp.B32[5] = pRoad->Dot[j+1].Lat;
			Tp.Buffer[24] = pRoad->Dot[j].Width;
			Uart.putb(1,25,Tp.Buffer);
			Uart.puts("\r\n\0");
			
			
			t = CheckIsOutRoad(	pRoad->Dot[j].Lon, pRoad->Dot[j].Lat, pRoad->Dot[j+1].Lon, pRoad->Dot[j+1].Lat,
										iLon,iLat, pRoad->Dot[j].Width	);
			if( t != TRUE)			// 函数名是判断出路线，所以返回TRUE为出路线
			{	// 进入路线
				RoadAttr.CurrentNo = EnableCnt;
				RoadAttr.RemoteID	= pRoad->ID;				// 路线ID
				RoadAttr.SecNo		= pRoad->Dot[j].ID;		// 路段ID
				RoadAttr.Width		= pRoad->Dot[j].Width;	// 路宽
				RoadAttr.Lon1		= pRoad->Dot[j].Lon;
				RoadAttr.Lat1		= pRoad->Dot[j].Lat;
				RoadAttr.Lon2		= pRoad->Dot[j+1].Lon;
				RoadAttr.Lat2		= pRoad->Dot[j+1].Lat;
				
				RoadAttr.InTimeCnt	= 0;
				RoadAttr.OutTimeCnt	= 0;
				
				InOutAreaType = eBB_AREA_ALARM_ROAD_SECTION;			// 报警使用
				InOrOutArea = eBB_IN_AREA;
				FenceOrRoadAlarmID = RoadAttr.RemoteID;		
				
				DFE.Write(EE_ROAD_ATTR,LEN_EE_FENCE_ATTR,(uchar*)&RoadAttr);
				
				SetBufferValue( Tp.Buffer,LEN_TP_BUF,0);
				strcpy((char*)Tp.Buffer,"\r\n进入");
				Int2Str( RoadAttr.CurrentNo,(char*)&Tp.Buffer[6]);
				strcat((char*)Tp.Buffer,"号路线，第");	
				Len = strlen( (char*)Tp.Buffer);
				Int2Str( RoadAttr.SecNo,(char*)&Tp.Buffer[Len]);
				strcat((char*)Tp.Buffer,"路段，原ID：");	
				Len = strlen( (char*)Tp.Buffer);
				Int2Str( RoadAttr.RemoteID,(char*)&Tp.Buffer[Len]);
				strcat((char*)Tp.Buffer," ");	
				Uart.puts((char*)Tp.Buffer);
				
				JT808Flag.VS7.Bits.InOutLine = TRUE;
				JT808Flag.VS7.Bits.LineDeviate = FALSE;
				
				break;			// 跳出查找路段的循环
			}				
		}
		if( RoadAttr.CurrentNo )	break;
		
		Current = 0;			// 下次循环不再找当前路线		
	}
	///////////////////////////////////////////////////////////////////////////////////////////
	if( (RoadAttr.CurrentNo==0) && ( RoadAttr.PreNo !=0) )			// 未找到路段，已出路线
	{				
		
		InOutAreaType = eBB_AREA_ALARM_ROAD_SECTION;			// 报警使用
		InOrOutArea = eBB_OUT_AREA;
		FenceOrRoadAlarmID = RoadAttr.RemoteID;			
				
		JT808Flag.VS7.Bits.InOutLine = TRUE;
		JT808Flag.VS7.Bits.LineDeviate = TRUE;
		
		
		SetBufferValue( Tp.Buffer,LEN_TP_BUF,0);
		strcpy((char*)Tp.Buffer,"从第");
		Int2Str( RoadAttr.PreNo,(char*)&Tp.Buffer[4]);
		strcat((char*)Tp.Buffer,"号路线偏离，原ID：");	
		Len = strlen( (char*)Tp.Buffer);
		Int2Str( RoadAttr.RemoteID,(char*)&Tp.Buffer[Len]);
		strcat((char*)Tp.Buffer,"，路段号：");
		Len = strlen( (char*)Tp.Buffer);
		Int2Str( RoadAttr.SecNo,(char*)&Tp.Buffer[Len]);
		
		strcat((char*)Tp.Buffer," ");	
		Uart.puts((char*)Tp.Buffer);
		
		if(RoadAttr.SecNo)
		{
			RoadAttr.SecNo			= 0;				// 路段ID			
			DFE.Write(EE_ROAD_ATTR,LEN_EE_FENCE_ATTR,(uchar*)&RoadAttr);
		}
		
	}
	MyFree(pData);
	#undef	LEN_TP_BUF
}




/************************************************************************************************
** 函数名称：CheckRoadFence()
** 函数功能：道路围栏检测
** 入口参数：经纬度
** 出口参数：电子围栏号(no不为0时,表示在第no号围栏内;no为0时,表示不在所有的围栏内)
** 全局变量：
** 修改时间：
*************************************************************************************************/
uchar			RoadLineStartDot;
void			GPSCls::CheckRoadLineFence(int32 iLon,int32 iLat)
{	
	#define		LEN_TP_BUF		64
	union
	{
		uchar		Buffer[LEN_TP_BUF];
		uint16	B16[LEN_TP_BUF/2];
		uint32	B32[LEN_TP_BUF/4];
	}Tp;
	uint16		Len = 0;
	uchar			t;
	if((GPSValidFlag!=GPS_VALID_FLAG))
		return ;
		
	if(RoadAttr.Total==0)																   // 有围栏才检测		
	{
		JT808Flag.VS7.Bits.InOutLine = 0;
		if(RoadAttr.CurrentNo)
		{
			SetBufferValue( (uchar*)&RoadAttr,LEN_EE_FENCE_ATTR,0);
		}
		if(TD.GPS==tGPS_ROAD_LINE_FENCE)		Uart.puts("未设置路线 ");
		return ;
	}
	
	
	
	if(RoadAttr.CurrentNo==0)
	{	
		Uart.puts("搜索所有路线\r\n\0");
		SearchRoadSection(0,iLon,iLat);			// 检索全部路线和路段		
	}
	else
	{
		NOP();	NOP();	NOP();	NOP();
		
		//////////////////////////////////////////// 测试代码
		Uart.puts(" Current Pos S E W Test Data:");
		Tp.B32[0] = iLon;		Tp.B32[1] = iLat;
		Uart.putb(1,8,Tp.Buffer);			
		Uart.putb(1,16,(uchar*)&RoadAttr.Lon1);
		Uart.putb(1,1,(uchar*)&RoadAttr.Width);
		Uart.puts("\r\n\0");
		
		// 先查找是否仍然在存储的当前路段里，偏离当前路段才搜索整条路线
				
		
		t = CheckIsOutRoad(	RoadAttr.Lon1, RoadAttr.Lat1, RoadAttr.Lon2, RoadAttr.Lat2,
									iLon,iLat, RoadAttr.Width	);
		if( t != TRUE)		// 函数名是判断出路线，所以返回TRUE为出路线
		{
			RoadAttr.InTimeCnt++;			
			RoadAttr.PreNo = RoadAttr.CurrentNo;
			if(TD.GPS==tGPS_ROAD_LINE_FENCE)
			{
				SetBufferValue( Tp.Buffer,LEN_TP_BUF,0);
				strcpy((char*)Tp.Buffer,"  当前在");
				Int2Str( RoadAttr.CurrentNo,(char*)&Tp.Buffer[8]);
				strcat((char*)Tp.Buffer,"号路线，第");	
				Len = strlen( (char*)Tp.Buffer);
				Int2Str( RoadAttr.SecNo,(char*)&Tp.Buffer[Len]);
				strcat((char*)Tp.Buffer,"路段，原ID：");	
				Len = strlen( (char*)Tp.Buffer);
				Int2Str( RoadAttr.RemoteID,(char*)&Tp.Buffer[Len]);
				strcat((char*)Tp.Buffer," ");	
				Uart.puts((char*)Tp.Buffer);
			}
		}
		else	// 已出路段，重新查找其它路段
		{	
			SearchRoadSection(RoadAttr.CurrentNo,iLon,iLat);
		}
	}
	
	#undef	LEN_TP_BUF
	

}
/************************************************************************************************
** 函数名称：ChekTimeRand()
** 函数功能：时间范围内检测，在时间范围内返回0，否则返回1
** 入口参数： 起始时间 St  结束时间Et
** 出口参数： 
** 全局变量：
** 修改时间：
*************************************************************************************************/
uchar 			GPSCls::ChekTimeRand(uint32 St,uint32 Et)
{
	uchar  result = 1;
	uint32  Ntime;

   if(St<(24*3600))   //秒数小于一天，则为周期时间，即每天的某段时间
	{
	    Ntime = Rtc.DT.Hour*3600 + Rtc.DT.Min*60 + Rtc.DT.Sec;
		 if((St<Ntime)&&(Et>Ntime))
			result = 0;
	}
	else
	{
		Ntime = Rtc.GetRtcSecond();
		if((St<Ntime)&&(Et>Ntime))
			result = 0;
	}
	return result;
}




void			GPSCls::Init(uchar Para)
{
	#pragma		pack(1)
	union
	{
		uchar		Buffer[16];
		uint16	B16[8];
		uint32	B32[4];
		unsigned long long B64[2];
	} Tp;
	NOP();	NOP();

	GpsSaveCnt = GpsBlindCnt = 0;

	
//	DFE.Read(EE_GPS_SEND_CONFIG,LEN_SEND_CFG,(uchar*)&Send);
//	if((Send.Str.Times==0)||(Send.Str.Interval==0))
//	{
//		Send.Str.Times = 0xffffffff;
//		Send.Str.Interval = 60;
//		Send.Str.StopInterval = 60;
//		Send.Str.Distance = 1000;
//		Send.Str.T_Interval = 30;
//		Send.Str.T_Time = 0;
//		Send.Str.Type = 0x55;
//		Send.Str.DelayAlarmTime = 10;
//		DFE.Write(EE_GPS_SEND_CONFIG,LEN_SEND_CFG,(uchar*)&Send);
//	}

	NOP();	NOP();	NOP();	NOP();
	DFE.Read(EE_SYS_GPS_SPEED_CALCULUS,12,Tp.Buffer);
	NOP();	NOP();
	SystemLog.GPS.SpeedCaculus = Tp.B64[0];			// 8
	SystemLog.GPS.TotalMileage = Tp.B32[2];			// 4	
	
	
			
	DataUpDate  = 0;
	NOP();	NOP();
	TaskTickCnt=0;
	
	
	GPS_SatelNum = 0;
	SetBufferValue((uchar*)GpsBlindBuffer,256,0);
	SetBufferValue((uchar*)GpsSaveBuffer,256,0);
	
	
	PackCnt = 0;
	GPS_PowerOn = TRUE;
	
	PreEleRailNo = 0;
	PreSecond  = 0;

	BCD_PackBusing = 0;
	GY_PackBusing = 0;
	TH_PackBusing = 0;

	strcpy(GPS_CharPack,"0,V,0,0,0,0,0,0,0,\0");

	GPS_LocalTimeString();

	SumOfSpeed1Min = 0;
	DriftingFlag = FALSE;

	SetBufferValue((uchar*)Last_GPS_CharPack,LEN_GPS_CHAR_PACK,0);

	
	//GpsSaveFlag = 0;	
	BD_UpDateFlag = FALSE;		
}



//void			GPSCls::SetSendConfig(GPS_SEND  *pSend,uchar Save)
//{
//	NOP();	NOP();
//	Send.Str.Times = pSend->Str.Times;
//	Send.Str.Interval = pSend->Str.Interval;
//	Send.Str.StopInterval = pSend->Str.StopInterval;
//	Send.Str.Distance = pSend->Str.Distance;
//	Send.Str.T_Interval = pSend->Str.T_Interval;
//	Send.Str.T_Time = pSend->Str.T_Time;
//	Send.Str.Type = pSend->Str.Type;
//	Send.Str.DelayAlarmTime = pSend->Str.DelayAlarmTime;
//	if(Save)
//	{
//		DFE.Write(EE_GPS_SEND_CONFIG,LEN_SEND_CFG,(uchar*)pSend);
//		#if(DFE_DEBUG==TRUE)
//		if(TD.MainTask==tMAIN_WRITE_DFE)	Uart.puts("DFE38\r\n\0");
//		#endif
//	}
//}



//void			GPSCls::GetSendConfig(GPS_SEND  *pSend)
//{
//	NOP();	NOP();	
//	pSend->Str.Times = Send.Str.Times ;
//	pSend->Str.Interval = Send.Str.Interval;
//	pSend->Str.StopInterval = Send.Str.StopInterval;
//	pSend->Str.Distance = Send.Str.Distance;
//	pSend->Str.T_Interval = Send.Str.T_Interval;
//	pSend->Str.T_Time = Send.Str.T_Time;
//	pSend->Str.Type = Send.Str.Type;
//	pSend->Str.DelayAlarmTime = Send.Str.DelayAlarmTime;
//}



void			GPSCls::TestGPS(void)
{
	#if(0)	
	GY_GPS_PACK	*pGY;
	strcpy(GPS_String,"$GPRMC,075105.618,A,2306.8053,N,11320.4763,E,000.00,110.19,140605,,*0B\r\n\0");
	
	pGY = (pGY_GPS_PACK)&GYPack;
	GY_DataPack(GPS_String,pGY);	
	#endif
	
}


uchar			GPSCls::GetSpeed(void)
{
 	return GYPack.Speed/100;			// 原来扩大100倍
}

uint16		GPSCls::Get1MinAvgSpeed(void)
{
 	return AvgSpeed1Min;			// 原来扩大100倍
}

uint16		GPSCls::GetSpeedX100(void)
{
	return GYPack.Speed;
}

uint32		GPSCls::GetMileage(void)
{
	return SystemLog.GPS.TotalMileage;
}


void			GPSCls::SetGPSPower(uchar on)
{
	 if(on==0)
	 {
		 GPS_PowerOn = 0;
		 GPS_POWER_OFF;
		 KeyDisp.SetLED_Status(LED_GPS,LED_TURN_OFF);
	 }
	 else
	 {
		 GPS_PowerOn = TRUE;
		 GPS_POWER_ON;
		 KeyDisp.SetLED_Status(LED_GPS,LED_1T_PER_1SEC);
	 }
}






/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
  函数名称：
  功能描述：
  入口参数: 
  出口参数：
  全局变量: 
  创建时间：
  修改时间: 
修改计数器
读取已读盲区页数： GY S \x00 R \xa0  \x34\x03   \x04\x00   \x00 \x00 \x00 \x00 
GYE-Test \x00 5 \x00 \x00 \x00 \x00 \x00 \x00 \x00 \x00 \x00 \x00 \x00 \x00 
GY S B
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void			GPSCls::SaveBlindData(uchar  *pData)
{		
	uchar		WriteOK,tBuffer[64];
	uint32	Addr,i;
	union
	{
		uint32	B32;
		uint16	B16[2];
		uchar		Buffer[4];
	}Tp;
	
	NOP();	NOP();	NOP();	NOP();
	if( GpsBlindCnt >= PACK_NUM_PER_PAGE )
		GpsBlindCnt = 0;


	memmove( &GpsBlindBuffer[GpsBlindCnt*LEN_JT808_GPS_PACK],pData,LEN_JT808_GPS_PACK);
	GpsBlindCnt++;
	
	
	if( GpsBlindCnt == PACK_NUM_PER_PAGE)
	{
		Addr = FF_GPS_BLIND_MESSAGE_START_PAGE;
		Addr += (FlashPageCnt.BlindSave%FF_GPS_BLIND_MESSAGE_TOTAL_PAGE);
		
		WriteOK = OK;
		SetBufferValue(tBuffer,64,0);
		if((Addr%FF_PAGE_PER_SECTOR)==0)
		{
			WriteOK = DFF.SectorEraseWaitOK( Addr/FF_PAGE_PER_SECTOR,10);
			if(WriteOK ==OK )		strcpy( (char*)tBuffer,"擦除扇区成功，\0");
			else						strcpy( (char*)tBuffer,"擦除扇区失败，\0");	
		}
		
		//////////////////////////////////////////////////////////////////		
		for(Tp.B16[0]=0,i=0;i<252;i++)	{	Tp.B16[0] += GpsBlindBuffer[i];	}
		Tp.B16[1] = FlashPageCnt.BlindSave;
		memmove( (uchar*)&GpsBlindBuffer[252],Tp.Buffer,4);
		WriteOK = DFF.PageProgramWaitOK(Addr,(uchar*)GpsBlindBuffer,5);				// 写入FLASH
		if(WriteOK ==OK )		strcat( (char*)tBuffer,"存储9条盲区数据，P\0");
		else						strcat( (char*)tBuffer,"存储9条盲区数据，数据错误，P\0");
		
		Addr = strlen((char*)tBuffer);
		Int2Str( FlashPageCnt.BlindSave, (char*)&tBuffer[Addr] );
		Uart.puts( (char*)tBuffer );	
		FlashPageCnt.BlindSave++;
		GpsBlindCnt = 0;
	}
}



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
  函数名称：
  功能描述：
  入口参数: 
  出口参数：
  全局变量: 
  创建时间：
  修改时间: 
重读精度测试数据，先设置命令 C5   (先远程发送模块设置参数，再发本地清除，远程再发秒传指令，最后本地清除）
GY C \x00 \x00  \x00  \x00  \x00   

重读精度测试数据，再设置命令 24  
GY S  \011 \001  \x00 \x00 \x00 \x00  	\x00 \x00 \x00 \x00   \x00 \x00  
GY S  \014 \002  \x3A \x00 \x00 \x00    \x00 \x00 \x00 \x00   \x00 \x00  

GY F \x00 R D \xE0\x1E \x10\x00  
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void			GPSCls::SaveHistoryData(uchar *pData)
{	
	
	//////////////// 暂时屏蔽，应付检测，不得使用批量上报，每次只能传1条。	
	uchar		WriteOK,tBuffer[64];
	uint32	Addr,i;
	union
	{
		uint32	B32;
		uint16	B16[2];
		uchar		Buffer[4];
	}Tp;
	
	NOP();	NOP();	NOP();	NOP();
	if( GpsSaveCnt >= PACK_NUM_PER_PAGE )
		GpsSaveCnt = 0;

	
	
	memmove( &GpsSaveBuffer[GpsSaveCnt*LEN_JT808_GPS_PACK],pData,LEN_JT808_GPS_PACK);
	GpsSaveCnt++;
	if( GpsSaveCnt == PACK_NUM_PER_PAGE)
	{
		WriteOK = OK;
		Addr = FF_GPS_MESSAGE_START_PAGE;
		Addr += (FlashPageCnt.HistorySave%FF_GPS_MESSAGE_TOTAL_PAGE);
		
		SetBufferValue(tBuffer,64,0);
		if((Addr%FF_PAGE_PER_SECTOR)==0)
		{
			WriteOK = DFF.SectorEraseWaitOK( Addr/FF_PAGE_PER_SECTOR ,10);			
			if(WriteOK ==OK )		strcpy( (char*)tBuffer,"擦除扇区成功，\0");
			else						strcpy( (char*)tBuffer,"擦除扇区失败，\0");					
		}		
		//////////////////////////////////////////////////////////////////
		for(Tp.B16[0]=0,i=0;i<252;i++)	{	Tp.B16[0] += GpsSaveBuffer[i];	}
		Tp.B16[1] = FlashPageCnt.HistorySave;
		memmove( (uchar*)&GpsSaveBuffer[252],Tp.Buffer,4);
		WriteOK = DFF.PageProgramWaitOK(Addr,(uchar*)GpsSaveBuffer,5);				// 写入FLASH						
		if(WriteOK ==OK )		strcat( (char*)tBuffer,"存储9条历史数据 \0");
		else						strcat( (char*)tBuffer,"存储9条历史数据，数据错误 \0");
		
		Addr = strlen((char*)tBuffer);
		Int2Str( FlashPageCnt.HistorySave, (char*)&tBuffer[Addr] );
		Uart.puts( (char*)tBuffer );	
		FlashPageCnt.HistorySave++;
		GpsSaveCnt = 0;
	}
	
}









void			GPSCls::RtcPlusPlus(void)
{
	if(GPSValidFlag!=GPS_VALID_FLAG);
	{
		GYPack.RtcSecond++;
		gCnt.GPS_RtcSecond++;		
		GPS_LocalTimeString();
		
	}
}





/*
*GY,9080010001,V1,080123,A,2813.5721,N,11259.4293,E,0.00,351.12,100609,FFFFFFFF#<0,0,25,0,0,0,10>
*/
uchar			GPSCls::DataPackErrorCheck(void)
{
	uchar		Cnt1,Cnt2;
	uint16	i,Len;
	Len = strlen(TH_GPS_Pack);
	Cnt1 = Cnt2 = 0;
	for(i=0;i<Len;i++)
	{
		if( (TH_GPS_Pack[i] == 'A')||(TH_GPS_Pack[i] == 'V')||\
		    (TH_GPS_Pack[i] == 'N')||(TH_GPS_Pack[i] == 'E')||\
			 (TH_GPS_Pack[i] == '#')||(TH_GPS_Pack[i] == '<')||\
			 (TH_GPS_Pack[i] == '>'))
			 Cnt1++;		
		if(TH_GPS_Pack[i] == ',')
			Cnt2++;
		if(TH_GPS_Pack[i] == 'V')
			return ERROR;
	}
	if( (Cnt1<6)||(Cnt2<12) )
	{
		return ERROR;
	}
	NOP();	NOP();
	return OK;
	
}



void			GPSCls::GPS_LocalTimeString(void)
{	
	//////////////////////////////////////////////	时间字符串，以'\0'结束
	sNowTimeString[0] = gtlRtc.Hour/10+'0';
	sNowTimeString[1] = gtlRtc.Hour%10+'0';
	sNowTimeString[2] = gtlRtc.Min/10+'0'; 
	sNowTimeString[3] = gtlRtc.Min%10+'0';	
	sNowTimeString[4] = gtlRtc.Sec/10+'0';	
	sNowTimeString[5] = gtlRtc.Sec%10+'0';	
	sNowTimeString[6] = 	0;

}



// 秒跳变处理
void			GPSCls::SecondHoppingProcess(uchar Blind)
{
	uint32		tl,Sec,ModifyLat,ModifyLon,StartLat,StartLon;
	uint32		ModifySpeed,ModifyAltitude,StartSpeed,StartAltitude;
	RTC_STR		tRtc;
	
	//////////////////////////////////////////// 无奈，检测中心模拟场景测试有跳秒现象？
	if(  ((GYPack.RtcSecond-1) > LastGYPack.RtcSecond)  &&  ( LastGYPack.RtcSecond > (12*365*24*3600UL) )   )
	{
		//if(TD.GPS==tGPS_MESSAGE_SAVE)		Uart.puts(" 跳秒补数据 ");
		tl = GYPack.RtcSecond - LastGYPack.RtcSecond - 1;	
		if(tl>5)	tl=5;
		//////////////////////////////////////////////////////////////////纬度
		if(GYPack.Latitude>LastGYPack.Latitude)
		{
			StartLat 	= LastGYPack.Latitude;
			ModifyLat 	= (GYPack.Latitude-LastGYPack.Latitude);
		}
		else
		{
			StartLat		= GYPack.Latitude;
			ModifyLat	= (LastGYPack.Latitude-GYPack.Latitude);
		}
		//////////////////////////////////////////////////////////////////经度
		if(GYPack.Longitude>LastGYPack.Longitude)
		{
			StartLon		= LastGYPack.Longitude;
			ModifyLon	= (GYPack.Longitude-LastGYPack.Longitude);
		}
		else
		{
			StartLon		= GYPack.Longitude;
			ModifyLon	= (LastGYPack.Longitude-GYPack.Longitude);
		}
		////////////////////////////////////////////////////////////////////速度
		if( GYPack.Speed > LastGYPack.Speed)
		{
			StartSpeed = LastGYPack.Speed;
			ModifySpeed = (GYPack.Speed - LastGYPack.Speed);
		}
		else
		{
			StartSpeed = GYPack.Speed;
			ModifySpeed = (LastGYPack.Speed - GYPack.Speed);
		}
		////////////////////////////////////////////////////////////////////海拔
		if( GYPack.Altitude > LastGYPack.Altitude )
		{
			StartAltitude = LastGYPack.Altitude;
			ModifyAltitude = (GYPack.Altitude - LastGYPack.Altitude );
		}
		else
		{
			StartAltitude = GYPack.Altitude;
			ModifyAltitude = (LastGYPack.Altitude - GYPack.Altitude );
		}
		
		
		ModifyLat /= (tl+1);
		ModifyLon /= (tl+1);
		ModifySpeed	/= (tl+1);
		ModifyAltitude	/= (tl+1);
		
		Sec = (LastGYPack.RtcSecond+(8*60*60UL));			// 转换成本地时间
		while( tl )
		{
			Sec++;										// 修正时间
			Rtc.SecondToDateTime( Sec, (RTC_STR*)&tRtc );
			LastJTPack.Date[0] = HexToBcd(tRtc.Year);
			LastJTPack.Date[1] = HexToBcd(tRtc.Month);
			LastJTPack.Date[2] = HexToBcd(tRtc.Day);
			LastJTPack.Time[0] = HexToBcd(tRtc.Hour);
			LastJTPack.Time[1] = HexToBcd(tRtc.Min);
			LastJTPack.Time[2] = HexToBcd(tRtc.Sec);
			
			StartLat += ModifyLat;					// 修正纬度
			LastJTPack.Latitude = SwapINT32( StartLat/10 );
			
			StartLon	+= ModifyLon;					// 修正经度
			LastJTPack.Longitude = SwapINT32( StartLon/10 );
			
			StartSpeed += ModifySpeed;				// 修正速度
			LastJTPack.Speed = SwapINT16( StartSpeed);
			
			StartAltitude += ModifyAltitude;		// 修正海拔高度
			LastJTPack.Altitude = SwapINT16(StartAltitude);
			
						
			if(Blind)
			{
				SaveBlindData((uchar*)&LastJTPack);
			}
			else
			{
				SaveHistoryData((uchar*)&LastJTPack);
			}
			if(tl)	tl--;	
			Uart.puts(" 跳秒现象 ");
		}
	}
	//////////////////////////////////////////// 无奈程序结束
}




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
  函数名称：
  功能描述：
  入口参数: 
  出口参数：
  全局变量: 
  创建时间：
  修改时间：	
*/

extern		uchar		RecordTaskRun,PicDataUpLoadFlag;
extern   	uchar  	*pShareMem;
extern		uchar		CreateUserFlag;
#include		"..\CAN\CAN.h"
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void			GPSCls::LocationInfoPushAlignment(void)
{
	union
	{
		uchar 	Buffer[4];
		uint32	B32;
	} Tp;
	
	
	NOP();	NOP();	NOP();	NOP();
	if( CreateUserFlag!=OK)
	{
		if( (GYPack.RtcSecond % 10) == 0 )
		{
			Uart.puts("您未开户！ ");	
		}
		return;
	}
	
//	#if( TEST_CODE_JUST_FOR_TEST_LUOYANG_NEW == 1)
//	if(TD.GPS==tGPS_VIRTUAL_GPS_VALID)
//	{
//		SetBufferValue(tBuffer,64,0);
//		strcpy((char*)tBuffer,"SamlePeriod1S Test Data:");
//		tBuffer[24] = SamlePeriod1S;
//		tBuffer[25] = TestBlind;
//		tBuffer[26] = TestAccuracy;
//		tBuffer[27] = JT_ShutMobileCnt;
//		tBuffer[28] = '\r';
//		tBuffer[29] = '\n';
//		Uart.putb(1,30,tBuffer);
//		
//	}
//	#endif
	
	////////////////////////////////////////// IC卡认证时，其它数据不进队
//	if(IC_Card_Auth_GPRS==TRUE)		return;		
	
	// 进队之前，获取最新标志位状态和报警状态
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
	
//	#if( TEST_CODE_JUST_FOR_TEST_LUOYANG_NEW == 1)
//	if(TD.GPS==tGPS_VIRTUAL_GPS_VALID)		Uart.puts(" 进队测试1 ");
//	
//	if( ReadyTestBlind ) 
//	{
//		if((GPSValidFlag==GPS_VALID_FLAG)||(VirtaulGPS_Valid == TRUE))
//		{
//			if(GYPack.RtcSecond >= 0x18C88194 )		// 13 03 05 10 12 36
//			{
//				JT_ShutMobileCnt	= TRUE;
//				ReadyTestBlind = 0;
//				Uart.puts("\r\n提前18秒记录盲区数据\r\n");
//			}		
//		}
//	}
//	#endif

	
	if(GG_Status.Flag.Bits.Online == 1)	
	{		
		if( NeedRegServer == ENABLE_FLAG )
		{
			if( ((gCnt.GetDataSecond%60)== 0)||(TempNeedRegServer==TRUE) )	
			{
				TempNeedRegServer = 0;
				GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_REG,0,NULL);	
			}
		}
		else if( jt_auth_ok_flag == 0 ) 
		{
			if(DialOK_NeedAuthFlag == TRUE)
			{
				Uart.puts("鉴权码 ");
				GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_AUTH,0,NULL);
				DialOK_NeedAuthFlag = 0;		// 
			}			
		}
//		else if( (GYPack.RtcSecond % SendInterval.HeartbeatTime) == 0 )								// 心跳包
//		{
//			#if (WUXI_TEST!=1)
//			if( RecordTaskRun||PicDataUpLoadFlag )		return;
//			//Uart.puts("心跳包 ");
//			GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_HEARTBEAT,0,NULL);	
//			#endif	
//		}
	}
	
	
	if( RecordTaskRun||PicDataUpLoadFlag  )		return;		// 上报多媒体数据时，不传其它数据
	
	
	if(TD.GPS==tGPS_VIRTUAL_GPS_VALID)		Uart.puts(" 进队测试2 ");
	
	GPRS.RemoteGetVDR_Data();


	GSM.CalculateSendInterval(0);
	
//	if( JT808_GPRS_TempPack == TRUE )					// 临时打包，立即发送的数据，通常带有报警标志
//	{
//		#if (0)//#if (WUXI_TEST!=1)
//		Uart.puts("报警标志，立即发送位置信息 ");	 
//		#endif
//		JT808_GPRS_TempPack = 0;
//		JT808_QuicklySendPack = TRUE;
//		#if (0)//#if (WUXI_TEST!=1)
//		if(gCnt.SystemSecond>60)				// 系统启动60秒后才报警
//			GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_POS_REPORT,0,NULL);
//		#endif
//
//	}
//	else if( SendInterval.T_ContinuedTime > 0)						 							// 临时间隔打包有效
//	{
//		if( (GYPack.RtcSecond % SendInterval.T_Second) == 0 )
//		{
//			Uart.puts("临时间隔发送位置信息 ");	 			// 打包进队。需判断鉴权成功后才出队。
//			GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_POS_REPORT,0,NULL);			
//		}
//	}
//	else if( (SendInterval.Second != 0xffffffff)&& (SendInterval.Second != 0) )	 	// 定时打包
    if( (SendInterval.Second != 0xffffffff)&& (SendInterval.Second != 0) )	 	// 定时打包
	{
		if( (gCnt.GetDataSecond % SendInterval.Second) == 0 )
		{
			//#if (WUXI_TEST!=1)
			Uart.puts("定时发送位置信息 ");	 	// 打包进队。需判断鉴权成功后才出队。
		//	#endif
			GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_POS_REPORT,0,NULL);			 
		}
	}
//	else if( (SendInterval.Distance != 0xffffffff)&& (SendInterval.Distance != 0) )
//	{
//		if( (SystemLog.GPS.TotalMileage % SendInterval.Distance)==0)						// 定距离打包
//		{
//			Uart.puts("定距发送位置信息 ");	 		// 打包进队。需判断鉴权成功后才出队。
//			GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_POS_REPORT,0,NULL);			
//		}
//	}
//	#if (0)//#if (WUXI_TEST!=1)
//	if( (GYPack.RtcSecond % (10*60)) == 0 )
//	{
//		if (TD.GPRS==tGPRS_TMP_TEST) { Uart.puts("定时发送系统日志 ");	 }
//		GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_SYS_LOG_UPLOAD,256,(uchar*)&SystemLog);
//	}
//	#endif
	
	if(TD.GPS==tGPS_VIRTUAL_GPS_VALID)		Uart.puts(" 进队测试5 ");
	

	
}


/*
#define		TEST_ROAD_LINE_CHECK			1

uint32		LonS[10] = {};
uint32		LatS[10] = {};
uint32		LonE[10] = {};
uint32		LatE[10] = {};
uint32		LonQ[10] = {};
uint32		LatQ[10] = {};

void			GPSCls::TestFun(void)
{
	uint32	LonS,LatS,LonE,LatE,LonQ,LatQ;
	
	
}
*/





GPSCls::GPSCls(void)
{


}
















/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */

