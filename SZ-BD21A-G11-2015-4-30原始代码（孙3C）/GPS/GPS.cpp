/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												 
$									
$                       	 		
$								  		
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      �ļ�����
	�ļ���:    GPS.cpp
	���������� 
	���뻷���� 
	Ŀ��CPU:  (char*)
	���ߣ�
	����ʱ�䣺
	����༭������UltraEdit 11.00a+  �Ʊ�����Ϊ�����ַ�������ɶ��Ը��á�
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
	�������ƣ�TaskGPS_RecOK
   �����������ɹ��յ�һ֡GPS���ݵĴ�������
   ��ڲ�������
   ���ڲ�������
   ȫ�ֱ���: 
   ����ʱ�䣺							 113.34127166
   �޸�ʱ�䣺
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
		
		
		os_evt_wait_or( 0x0001, (2*100) );		// ����ÿ��ִ��1��
		
		if(TD.GPS==tTASK_RUN_FLAG)			Uart.puts("GPS Task 1\r\n\0");	

		if( (GG_Status.Flag.Bits.UpdateRemoteSet == 1)||(U0_Iap_Flag!=0) )   continue; // ����

		if( (GG_Status.Flag.Bits.Online == 1) && ( jt_auth_ok_flag != 0 )  && ((Cnt%120) == 0) && (PicDataUpLoadFlag == 0) ) //�ϱ�ͼƬ����								// ������
		{
			Uart.puts("������1 ");
			GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_HEARTBEAT,0,NULL);		
		}
		//ʡ��ģʽ���ϴ�λ����Ϣ���پ����������ϳ�����������GPSģ��		
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
			//////////////////////////////////////////////////////////////////// RTCʱ��					
	      GPS.LastJTPack.Time[0] = HexToBcd(Rtc.DT.Hour);
	      GPS.LastJTPack.Time[1] = HexToBcd(Rtc.DT.Min);
	      GPS.LastJTPack.Time[2] = HexToBcd(Rtc.DT.Sec);
		  	KeyDisp.SetLED_Status(LED_GPS,LED_TURN_OFF);

			////////////////////////////////////////////////////////		���߷���λ����Ϣ				
			if( (GG_Status.Flag.Bits.Online == 1) && ( jt_auth_ok_flag != 0 )  )	
			
			{
				if( Cnt%(2*60/2) == 0 )			// 5���ӷ���1����Cntÿ2���1
				{
					GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_POS_REPORT,0,NULL);
					//GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_HEARTBEAT,0,NULL);	
					Uart.puts("���ߣ���ʱ����λ����Ϣ ");
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
	�������ƣ�GPSCls::eTask
   ����������GPS������
   ��ڲ�������
   ���ڲ�������
   ȫ�ֱ���: 
   ����ʱ�䣺2015-10-10 12:25
   �޸�ʱ�䣺	0f 64 af cb
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
		if((TaskTickCnt%3)==0)  //3��   ��Ϊ��������  os_evt_wait_or( 0x0001, (2*100) );	 
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

	if (InvalidGPSValidTime>180)	  // 3���Ӳ���λ������ģ��
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


// tett		����·��
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
		Uart.puts("��·��");
	}
	else
	{
			Uart.puts("��·��");
	}

	return; // test
#endif

//
				
	
	
	/////////// ����GPS���߿�·��� ///////////////////////////////
	

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
		SystemLog.GG1.Speed_Info |=  (GetSatelliteStatus(GPS_SatelStatus) & 0xff) << 24 ;			// 	���Ǹ���
	}
	else			// û��GPS�����ź�
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
		if(GPS_InvalidCnt>6*10)									  //6��
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
			  		if (JT808_Alarm_Word.SendTextSMS & (0x01 << Index))		     								// ���Ͷ���
			  		{
						GSM.SetSMS_Buffer((char *)&JT808_Number.TextSMS[0],"GPS Model Error Alarm!!!");		
			  		}
			  		//if (JT808_Alarm_Word.TakePhoto & (0x01 << Index))	Camera.SetTakeNumber(2,TRUE);		  // ����
					if (JT808_Alarm_Word.TakePhoto & (0x01 << Index))
					{
			  			if (!(JT808_Alarm_Word.SavePhoto & (0x01 << Index)))	Jt_photo_upload = 1;		  // �ϴ���ƽ̨
						MediaEventFlag =eBB_MEDIA_EVENT_OTHER;
			  			Camera.SetTakeNumber(2,TRUE);		  // ���� 
			  		}
				}
			}
		   SelfTestResust |= eST_ER_GPS;
		}
	}
	if( (TD.GPS==tTASK_RUN_FLAG)&&(TD.Value<1) ) return;
	
	////////////////////////////////////////////////////////////////////////////////// ����GPS����	
	NOP();	NOP();	
	if(GPS_RecDataOK)
	{		
		NOP();	NOP();
		DataCompleteAnalys(GPS_String);		// ���������Լ��				
		if( (TD.GPS==tTASK_RUN_FLAG)&&(TD.Value<2) ) return;
		///////////////////////////////////////////////////// ���ݴ�� ��ע�����ǰ�����������Լ�⣩		
		GY_DataPack((uchar*)NULL);				//  GPS���ݰ�
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
			if (DriftingFlag==TRUE)			 // ����Ư��
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
		if(TD.GPS == tGPS_HEX_PACK)				// ��� GPS���ݰ�
		{
			Uart.putb(1,LEN_GY_GPS_PACK,(uchar*)&GYPack);
		}				
		
      
		//////////////////////////////////////////// ����Χ�����
		NOP();	NOP();	NOP();	NOP();
		CheckRectangleFence(GYPack.Longitude,GYPack.Latitude);			 //���ε���Χ�����
		
		NOP();	NOP();	NOP();	NOP();
		CheckRoadLineFence(GYPack.Longitude,GYPack.Latitude);
		
		/////////////////////////////////////////////////////////////////////λ����Ϣ�������
		NOP();	NOP();	NOP();	NOP();
		//LocationInfoPushAlignment();			// ����λʱ������������ģ�黵�˾Ͳ��ܽ����ˣ�
		
		if( (GPSValidFlag == GPS_VALID_FLAG)&& (DriftingFlag==FALSE) ) 
		{
			memmove((uchar*)&LastGYPack,(uchar*)&GYPack,LEN_GY_GPS_PACK);
		   memmove((uchar*)&LastJTPack,(uchar*)&JTPack,LEN_JT808_GPS_PACK);
		}		
	}

	//LocationInfoPushAlignment();
	////////////////////////////////////////////////////////////////////////
	NOP();	NOP();	NOP();	NOP();
	


	////////////////////////////////////////////////////////////////// RTCУ��
	
	if( (GPSValidFlag==GPS_VALID_FLAG)||(VirtaulGPS_Valid == TRUE))
	{			
		GPSValidCnt++;
		if( ( (GPSValidCnt%60)==0)|| (GPSValidCnt==15) )
		{
			#if (WUXI_TEST != 1)
			Rtc.SetRTC_DT(gCnt.GPS_RtcSecond+8*60*60);			  						 // ����RTC���ú���	
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
	�������ƣ�DataValidAnaly
   ����������GPRMC���������Է���
   ��ڲ�����GPRMCԭʼ�����ַ���
   ���ڲ�����������־
   ȫ�ֱ���: 
   ����ʱ�䣺2009-03-06 11:00
   �޸�ʱ�䣺
	ע���������ȷ�Ҷ�λ��Ч����������Ч��������Ч���ݡ�
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
uchar			GPSCls::DataCompleteAnalys(char	*pGPS)
{
	
	uchar		c,Sum,Sum1,Valid;
	uint32	i,j,Len;
	//////////////////////////////////////////	
	Len = strlen(pGPS);
	Valid = 0;
	JT808Flag.VS1.Bits.GPS = 0;		
	GPSValidFlag = GPS_INVALID_FLAG;
	i = 1;		 // ������'$'
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
	///////////////////////////////////////// ������Ч�������������λ��
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
// ���أ�
uint32			GPSCls::GetSatelliteStatus(char *pSatel)
{
	union
	{
		uint32	B32;
		struct
		{
			uchar		GPS;		 	// GPS
			uchar		BD;			// ����
			uchar		Glo;
			uchar		Geli;
		}N;
	}Num;
	
	// ��ʾ��
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
	�������ƣ�
   ����������
   ��ڲ�����
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺 
$GPTXT,01,01,02,ANTSTATUS=OPEN*2B \x0D 09:21:18�� \x0A
$GPTXT,01,01,02,ANTSTATUS=OK*3B \x0D 09:21:22�� \x0A
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
#define	NORAM_GPS_TYPE			0
void			GPSCls::GPS_AntCheck(char *pANT)
{
	//static	uint32	LastTime;
	//static	uchar		flag;
	uint32		Short,Open,AntOK;
	uchar			Index;
	
	#if(GPS_ANT_CHECK_COMMAND)	
	Short = Open = AntOK = 0;		// Ĭ������
	if( MyStrStr(pANT,"OPEN\0") )
		Open = TRUE;
	if( MyStrStr(pANT,"SHORT\0") )
		Short = TRUE;
	if( MyStrStr(pANT,"OK\0") )
		AntOK = TRUE;
	#else
	uint32	Data;
	Short = Open = 0;		// Ĭ������
	GET_GPS_ANT_INIT;	  //	return;

	#if (NORAM_GPS_TYPE)
	Data = GET_GPS_ANT_OPEN_STATUS;
	if((Data == 0) && (!(JT808_Alarm_Word.Shield & (0x01 << 5))))
		Open = TRUE;
	Data = GET_GPS_ANT_SHORT_STATUS;
	if((Data != 0)	&& (!(JT808_Alarm_Word.Shield & (0x01 << 6))))
		Short = TRUE;
	
	#else 				 // ����

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

	if(Open)		// ���߿�·
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
		  	if (JT808_Alarm_Word.SendTextSMS & (0x01 << Index))		     								// ���Ͷ���
		  	{
			 	GSM.SetSMS_Buffer((char *)&JT808_Number.TextSMS[0],"Antennae Open Alarm!!!");		
		  	}
		  	#if ( TEST_CODE_JUST_FOR_TEST_LUOYANG != 1)
			if (JT808_Alarm_Word.TakePhoto & (0x01 << Index))
			{
	  			if (!(JT808_Alarm_Word.SavePhoto & (0x01 << Index)))	Jt_photo_upload = 1;		  // �ϴ���ƽ̨
				MediaEventFlag =eBB_MEDIA_EVENT_OTHER;
	  			Camera.SetTakeNumber(2,TRUE);		  // ���� 
	  		}
			#endif	// Ӧ���������ľ������������
		}
	}
	else if(AntOK)
	{ 
	   //if(VehicleStatus.VS4.Bits.GPS_ANT_Open == 1)		  //GPS���ߴӿ�·�лָ�����һ�α�׼��Ϣ
	    if(JT808Flag.VS5.Bits.AntOpen == 1)
		 {
	    	SelfTestResust &= ~eST_ER_ANT_OPEN;
			JT808_GPRS_TempPack = TRUE; 
			Uart.puts("TP ANT Open to OK-TP\r\n\0");      
	    }
	    VehicleStatus.VS4.Bits.GPS_ANT_Open = 0;
		 JT808Flag.VS5.Bits.AntOpen = 0;	
	}



	if(Short)		// ���߶�·
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
		  	if (JT808_Alarm_Word.SendTextSMS & (0x01 << Index))		     								// ���Ͷ���
		  	{
			 	GSM.SetSMS_Buffer((char *)&JT808_Number.TextSMS[0],"Antennae Short Alarm!!!");		
		  	}
		  	//if (JT808_Alarm_Word.TakePhoto & (0x01 << Index))	Camera.SetTakeNumber(2,TRUE);		  // ����
			#if ( TEST_CODE_JUST_FOR_TEST_LUOYANG != 1)
			if (JT808_Alarm_Word.TakePhoto & (0x01 << Index))
			{
	  			if (!(JT808_Alarm_Word.SavePhoto & (0x01 << Index)))	Jt_photo_upload = 1;		  // �ϴ���ƽ̨
				MediaEventFlag =eBB_MEDIA_EVENT_OTHER;
	  			Camera.SetTakeNumber(2,TRUE);		  // ���� 
	  		}
			#endif	// Ӧ���������ľ������������
		}	

	
		AntShortTimeCnt++;
		if(AntShortTimeCnt>5) 	// ���߶�·10�룬�ر�GPS��Դ (3�����Զ��򿪣�
			GPS_PowerOn = 0;
	}
	else if(AntOK)
	{
	    //if(VehicleStatus.VS4.Bits.GPS_ANT_Short == 1)		  //GPS���ߴӶ�·�лָ�����һ�α�׼��Ϣ
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
	�������ƣ�LonLatToUint32
   ������������RMC��ʽ�ľ��Ȼ�γ��ת��Ϊ�Ŵ���10^7��������(��)
   ��ڲ�����GPS�ַ���ָ�룬
   ���ڲ�����������
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
   ע��������DecCharToUint32��������ʹ��Str2Int��atoi���棬��ΪΪ����С����
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
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
	if(i<3)	return 0x7fffffff;					// ��γ��û��0x7fffffff;
	ptDms->D = DecCharToUint32((i-2),(char*)ptGPS);		// �ȵ���������	
	ptDms->M = atoi((char *)(ptGPS+i-2));					// �ֵ���������
	ptDms->S = DecCharToUint32(4,(char *)(ptGPS+i+1));	// �ֵ�С������	
	
	ptDms->MMM = (ptDms->D*60 + ptDms->M)*10000 + ptDms->S;	// ȫ����Ϊ��(�Ŵ���10^4��)
		
	tl  =  DecCharToUint32((2+1+4),(ptGPS+i-2));	// 2λ����+4λС��(�Ŵ���10000��)
	tl *= 1000;												// �Ŵ���1000��
	tl /= 60;												// �ȵ�С�����֣��ܹ��Ŵ���10^7��
	
	tl += ((ptDms->D)*10000000L);
	
	if(Flag<0)		// 
	{	// ��������γΪ��
		ptDms->MMM |= 0x80000000;
		tl |= 0x80000000;
	}
	
	#if (0)	
	ptDms->dbD = tl;		// ����������
	ptDms->dbD /= 10000000L;
	#endif	
	return tl;
}

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�Calculate2DotDistance
   �����������������㾭γ�ȵľ���
   ��ڲ��������㾭γ�����꣬�Ŵ�10^7���Ķ�)
   ���ڲ��������������룬��λΪ��
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺

$GPRMC,100433.124,A,2251.0206,N,10816.1908,E,0.00,271.31,060406,,*0D
*/ 

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
uint32		GPSCls::Calculate2DotDistance(int32 Lon1,int32 Lat1,int32 Lon2,int32 Lat2)
{	
	uint32	tl,LongUnit;
	uint32	Longitude,Latitude;
	long long 	tp1;
	// ��ڲ���Ϊ�Ŵ�10^7���Ķȣ�����Ҫת��Ϊ�Ŵ���10^4�ķ֣������ȳ���60���ٳ���1000
	tp1 = Lon1;		tp1 *= 60;		Lon1 = tp1/1000;
	tp1 = Lat1;		tp1 *= 60;		Lat1 = tp1/1000;
	tp1 = Lon2;		tp1 *= 60;		Lon2 = tp1/1000;
	tp1 = Lat2;		tp1 *= 60;		Lat2 = tp1/1000;
	
	////////////////////////////////////////////////////////////////////////////////
	
	/*  �ý����㷨������㾭γ��֮��ľ��룬����Ϊƽ�棨ʵΪ���棩
		 ������ľ��� 1853��/�֣�Խ���������ϼ�ʱ��ÿ�־��ȵľ���Խ��ԽС��ÿ3��С1��
		 γ��3706��/�֣�����һ��
	*/
	// ����γ�Ⱦ��룬��λΪ��	
	Latitude = ((abs(Lat1 - Lat2))*3706/10000UL);
	
	tl = Lat1 + Lat2;
	tl >>= 1;		// ����2		// ȡ�����м�γ��
	tl /= 10000;	// ֻȡ�ֵ���������
	LongUnit = 1853 - (tl/3);
	// �Ȱ�������ĵ�λ������㣬��λΪ��
	Longitude = ((abs(Lon1 - Lon2))*LongUnit/10000UL);
   		
	// add by xzb

	if (Lon1 == Lon2)				// ������ͬ,�������Ϊ����γ�Ⱦ���
	{
		tl = Latitude;
	}
	else if (Lat1 == Lat2)		// γ����ͬ���������Ϊ�����Ⱦ���
	{
		tl = Longitude;
	}
	else		 // �������Σ�������㾭γ�ȵľ���	// �ۼӾ���
	{	
		tl = sqrt((Longitude*Longitude  + Latitude*Latitude) * 1.0);	
	}
	// end

	return  tl;
	
}



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�Calculate2DotSphericalDistance
   �����������������㾭γ���������
   ��ڲ��������㾭γ������(˫���ȸ�����)
   ���ڲ���: 
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 

#define			PI			3.14159265358979
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
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
	return (Angle*6371004L);					// ����	
	#else
	return 0;
	#endif
}               
					


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
  �������ƣ�GPS_GY_MMM_ToString
  ������������MMM��ʽ��GPS����ת����RMC��ʽ������
  ��ڲ���: MMM���ݣ��ַ�������
  ���ڲ�������
  ȫ�ֱ���: ��
  ����ʱ�䣺2007-7-7 11:24
  �޸�ʱ�䣺
*/ 	
void				Int16ToDecChar(uint16 Data,char *pBuffer);
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void				GPSCls::GY_MMM_ToRMC(uint32 MMM,uchar *pRmc)
{
	uint32	tl;
	uchar		Len,d,m;	
	NOP();	NOP();
	tl = MMM/10000;				// MMM�ǷŴ���10^4��������10000���õ���*60+��
	
	d = tl/60;						// ��
	m = tl%60;						// ��	
	
	Int16ToDecChar(d,(char*)pRmc);
	Len = strlen((char*)pRmc);
	
	*(pRmc+Len) = m/10+'0';		Len++;
	*(pRmc+Len) = m%10+'0';		Len++;
	
	*(pRmc+Len) = '.';			Len++;
		
	tl = MMM%10000;					// �ֵ�С������
	
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
// 13 03 05 18 15 32  ����ʱ��0x18C8F2C4��GPSʱ�䣬13 03 05 10 15 32		0x18C88244
// 13 03 05 18 15 54  ����ʱ��0x18C8F2DA��GPSʱ�䣬13 03 05 10 15 54		0x18C8825A
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
** �������ƣ�CheckEleRail()
** �������ܣ�����Χ�����
** ��ڲ�������γ��
** ���ڲ���������Χ����(no��Ϊ0ʱ,��ʾ�ڵ�no��Χ����;noΪ0ʱ,��ʾ�������е�Χ����)
** ȫ�ֱ�����
** �޸�ʱ�䣺
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
	
	if(RectFenceAttr.Total==0)																   // ��Χ���ż��		
	{
		VehicleStatus.VS4.Bits.InAreaAlarm = 0;
		VehicleStatus.VS4.Bits.OutAreaAlarm = 0;
		JT808Flag.VS7.Bits.InOutArea = 0;
		if(RectFenceAttr.CurrentNo)
		{
			SetBufferValue( (uchar*)&RectFenceAttr,LEN_EE_FENCE_ATTR,0);
		}
		if(TD.GPS==tGPS_RECT_FENCE)		Uart.puts("δ���þ��ε���Χ�� ");
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
				k = EnableCnt>>3;		// ����8
				t = EnableCnt%8;
				if(RectFenceAttr.Enable[k]&(1<<t))	
				{
					Enable = TRUE;		break;
				}
			}
			NOP();	NOP();
			if( Enable != TRUE )	
			{
				if(TD.GPS==tGPS_RECT_FENCE)		Uart.puts("δʹ�ܾ���Χ����Ч��־ ");
				break;	 	// δ�ҵ�ʹ�ܵ�Χ��
			}
			Addr = FF_RECTANGLE_FENCE_START_PAGE;
			Addr += (EnableCnt/8);
			DFF.PageRead(Addr,tPage.Buffer);
			
			if(TD.GPS==tGPS_RECT_FENCE)
			{
				SetBufferValue( Tp.Buffer,80,0);
				strcpy((char*)Tp.Buffer,"ҳ��ַ��");
				Int2Str( Addr,(char*)&Tp.Buffer[8]);
				strcat((char*)Tp.Buffer,"��Χ���ţ�");
				Len = strlen( (char*)Tp.Buffer);
				Int2Str( EnableCnt,(char*)&Tp.Buffer[Len]);
				strcat((char*)Tp.Buffer," ");
				Uart.puts((char*)Tp.Buffer);
			}
			
			k = EnableCnt%8;
			pFence = (pELE_FENCE_STRUCT)&tPage.Buffer[k*LEN_ELE_FENCE_STRUCT];
			EnableCnt++;		//								
			
			/////////////////////////// ���ϽǺ����½ǣ��ڱ������Χ�����ڱ��ϲ����Χ����
			if(  (iLon>(pFence->Lon1))&&(iLon<(pFence->Lon2))  &&   (iLat>(pFence->Lat2))&&(iLat<(pFence->Lat1))   )				
			{
				// EnableCnt��0��ʼ������ǰΧ���Ŵ�1��ʼ�������ȼ�1���ٸ�ֵ
				RectFenceAttr.CurrentNo 	= EnableCnt;
				RectFenceAttr.RemoteID	= pFence->ID;		// ԭ����ID
				RectFenceAttr.Lon1 		= pFence->Lon1;
				RectFenceAttr.Lat1 		= pFence->Lat1;
				RectFenceAttr.Lon2 		= pFence->Lon2;
				RectFenceAttr.Lat2 		= pFence->Lat2;
				RectFenceAttr.InTimeCnt	= 0;
				RectFenceAttr.OutTimeCnt= 0;
				
				DFE.Write(EE_RECTANGLE_FENCE_ATTR,LEN_EE_FENCE_ATTR,(uchar*)&RectFenceAttr);
				
				InOutAreaType = eBB_AREA_ALARM_RECT;			// ����ʹ��
				InOrOutArea = eBB_IN_AREA;
				FenceOrRoadAlarmID = RectFenceAttr.RemoteID;
				
				SendGnssDataEnable = 0;
				
				TestRectFence = TRUE;	
				
				
				JT808Flag.VS7.Bits.InOutArea = 1;			
				
				SetBufferValue( Tp.Buffer,LEN_TP_BUF,0);
				strcpy((char*)Tp.Buffer,"����");
				Int2Str( RectFenceAttr.CurrentNo,(char*)&Tp.Buffer[4]);
				strcat((char*)Tp.Buffer,"�ž���Χ����ԭID��");	
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
				if( (RectFenceAttr.OutTimeCnt==3) )	  	// ��Χ������󣬷��ͱ�����Ϣ����Χ��ʱ����Ϣ��
				{
					Uart.puts("�ϱ���Χ��������Ϣ\r\n\0");
					GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_POS_REPORT,0,NULL);
				}				
				RectFenceAttr.InTimeCnt = 0;
				RectFenceAttr.OutTimeCnt++;					
							
			}
			if(TD.GPS==tGPS_RECT_FENCE)	
			{
				SetBufferValue(Tp.Buffer,LEN_TP_BUF,0);
				strcpy((char*)Tp.Buffer,"\r\nδ��⵽����Χ��,Lon1: ");
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

				strcat((char*)Tp.Buffer,",��ǰ����,Lon: ");	
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
	{		// ���ϽǺ����½ǣ��ڱ������Χ�����ڱ��ϲ����Χ����
		if( (iLon>=RectFenceAttr.Lon1)&&(iLon<=RectFenceAttr.Lon2)
			&&(iLat>=RectFenceAttr.Lat2)&&(iLat<=RectFenceAttr.Lat1)  )
		{
			RectFenceAttr.InTimeCnt++;
			
			if(RectFenceAttr.InTimeCnt==3)	// ��Χ������󣬷��ͱ�����Ϣ����Χ��ʱ����Ϣ��
			{				
				//SendGnssDataEnable = TRUE;		//  
				//JT808Flag.VS7.Bits.InOutArea = TRUE;	
				GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_POS_REPORT,0,NULL);
				Uart.puts("�ϱ���Χ��������Ϣ\r\n\0");
			}
			
			//RectFenceAttr.OutTimeCnt = 0;	����ʱ����
			if(TD.GPS==tGPS_RECT_FENCE)
			{
				strcpy((char*)Tp.Buffer,"��ǰ��");
				Int2Str( RectFenceAttr.CurrentNo,(char*)&Tp.Buffer[6]);
				strcat((char*)&Tp.Buffer[Len],"�ž���Χ��֮�ڣ�ԭID��");	
				Len = strlen( (char*)Tp.Buffer);
				Int2Str( RectFenceAttr.RemoteID,(char*)&Tp.Buffer[Len]);
				strcat((char*)&Tp.Buffer[Len]," ");	
				Uart.puts((char*)Tp.Buffer);				
			}
		}
		else
		{			
			RectFenceAttr.PreNo = RectFenceAttr.CurrentNo;
			
			InOutAreaType = eBB_AREA_ALARM_RECT;			// ����ʹ��
			InOrOutArea = eBB_OUT_AREA;
			FenceOrRoadAlarmID = RectFenceAttr.RemoteID;
			
			
			JT808Flag.VS7.Bits.InOutArea = 1;
							
			TestRectFence = 0;
			RectFenceAttr.CurrentNo = 0;
			//RectFenceAttr.InTimeCnt = 0;		// ����ʱ����
			RectFenceAttr.OutTimeCnt++;
			Uart.puts("�ѳ�Χ�� ");	
			
		}
	} 
	
	#undef	LEN_TP_BUF
}



/************************************************************************************************
** �������ƣ�CheckRoundEleRail()
** �������ܣ�Բ�ε���Χ�����
** ��ڲ�������γ��
** ���ڲ���������Χ����(no��Ϊ0ʱ,��ʾ�ڵ�no��Χ����;noΪ0ʱ,��ʾ�������е�Χ����)
** ȫ�ֱ�����
** �޸�ʱ�䣺
*************************************************************************************************/
void			GPSCls::CheckRoundEleFence(int32 iLon,int32 iLat)
{		
	
}




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�Calculate2DotDistance_dm
   �����������������㾭γ�ȵľ��� ��λ������
   ��ڲ��������㾭γ������(FFF.FFFF��ʽ���Ŵ�10^4������С��) ,��λ�� ��
   ���ڲ��������������룬��λΪ��
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺

$GPRMC,100433.124,A,2251.0206,N,10816.1908,E,0.00,271.31,060406,,*0D
*/ 


uint32        Calculate2DotDistance_dm(int32 Lon1,int32 Lat1,int32 Lon2,int32 Lat2)		// ���ص�ֵ�ĵ�λΪ����
{
    uint32    tl,LongUnit;
    uint32    Longitude,Latitude;
    /*  �ý����㷨������㾭γ��֮��ľ��룬����Ϊƽ�棨ʵΪ���棩
         ������ľ��� 1853��/�֣�Խ���������ϼ�ʱ��ÿ�־��ȵľ���Խ��ԽС��ÿ3��С1��
		 γ��3706��/�֣�����һ��
	*/

	// ����γ�Ⱦ��룬��λΪ����
	
	Latitude = (Lat1 > Lat2) ? (Lat1-Lat2):(Lat2 - Lat1);
	
	Latitude = Latitude*3706/1000;
	
	tl = Lat1 + Lat2;
	tl >>= 1;		// ����2		// ȡ�����м�γ��
	tl /= 10000;	// ֻȡ�ֵ���������
	LongUnit = 1853 - (tl/3);
	// �Ȱ�������ĵ�λ������㣬��λΪ����
	//Longitude = ((abs(Lon1 - Lon2))*LongUnit/1000);
	Longitude = (Lon1 > Lon2) ? (Lon1-Lon2):(Lon2 - Lon1);

	Longitude = Longitude *	LongUnit/1000;
   		
	if (Lon1 == Lon2)				// ������ͬ,�������Ϊ����γ�Ⱦ���
	{
		tl = Latitude;

	}
	else if (Lat1 == Lat2)		// γ����ͬ���������Ϊ�����Ⱦ���
	{
		tl = Longitude;
	}
	else		 // �������Σ�������㾭γ�ȵľ���	// �ۼӾ���
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



/* ֱ������ʽ����  	(X-x1)/(x2-x1)  =  (Y-y1)/(y2-y1)
   ����õ�һ��ʽ�� 	X(y2-y1) - Y(x2-x1) - x1(y2-y1)+y1(x2-x1) = 0
   ���ԣ�	A = y2-y1
   			B = -(x2-x1)  = x1-x2
   			C = y1(x2-x1) - x1(y2-y1)
   ���߾��룺D =  |AX+BY+C| / (sqrt((A*A + B*B))
*/
/*
uchar 		CheckIsOutRoad_DL_Distance(int32 LonS, int32 LatS, int32 LonE, int32 LatE,int32 LonQ, int32 LatQ, uchar RoadWidth)
{
	double	A,B,C,td;
	uint32	tl;
	// �ھ�γ�ȷ�Χ��ģ�ֱ���ж�Ϊ��·��
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
	
	
	
	if( LonS==LonE )		// ���ϱ������·��ֻ�㾭�ȼ���뼴��
	{
		tl = Calculate2DotDistance(LonQ,LatQ,LonS,LatQ);
		if( tl > RoadWidth)	return TRUE;
		else						return FALSE;
	}
	else if( LatS==LatE)	// �����������·��ֻ��γ�ȼ���뼴��
	{
		tl = Calculate2DotDistance(LonQ,LatQ,LonQ,LatS);
		if( tl > RoadWidth)	return TRUE;
		else						return FALSE;
	}
	else							// ���������·
	{
		A = LatE;	A -= LatS;
		td = LonE;	td -= LonS;
		B = -td;
		C = td*LatS - A*LonS;
		td = abs( A*LonQ + B*LatQ + C ) / sqrt ( A*A + B*B );
		
		/////////////////// �������ϵת��Ϊ�ף�ԭ��λΪ�Ŵ�10^7�Ķȣ�
		return TRUE;
		
	}

}	*/





/*
��·��ƫ���ж�˼��
��
����.���Գ�����ǰ����ΪԲ�ģ�·��һ��Ϊ�뾶����Բ���Ƿ������յ�������ֱ���н���
		��û�н��㣬���ȷ������ƫ����·��;�н���,����Ҫ���еڣ����жϡ�
����.�����복������Ĺյ���Ϊ��ԳƵ����ĵ㣬�������ԳƵ�
		���ֱ������������Զ�յ�ľ���ͳ����ԳƵ㵽��Զ�յ�ľ���
����.����������ԳƵ㵽��Զ�յ�ľ��롡��С�ڡ���������Զ�յ�ľ��룬����ж�
		��������·���ڣ�������ƫ����·��

*/


uchar 		GPSCls::CheckIsOutRoad(uint32 LonS, uint32 LatS, uint32 LonE, uint32 LatE,uint32 LonQ, uint32 LatQ, uchar RoadWidth)
{
		
    // ����ֱ����Բ�ཻ���жϳ�������·�棬˼�룺���յ�S���յ�E��ֱ��L��������Q�㣬��·���һ��Ϊ�뾶��һ��Բ�������Բ��ֱ���н��㣬��ô�������ڹ�·�У��������·
    // ���������������S�㵽ԭʼ�㣬��S�㣬��X�Ὠ��ƽ��γ�ߣ���Y��ƽ�о��ߵ�ֱ������ϵ����S������Ϊ��0��0�� X,Y��ĵ�λΪ�ף�Q������Ϊ��xQ,yQ),������Q����Բ���뾶Ϊr(·���һ�룩
    // Բ�ķ���: (x-xQ)^2 + (y-yQ)^2 = r^2, ��E������Ϊ��xE,yE), ֱ��SE�ķ��̣�y=kx;
    // ��y=kx����Բ���̣��õ�һԪ���η��̣�����Τ�ﶨ�������ʽ����֪�����b^2 - 4ac > 0�����н⣬��ֱ����Բ�ཻ , 
    // ���õ���(2*xQ+2*yQ*k)^2-4*(1+k^2)*(xQ^2+yQ^2-r^2)
    // ֻҪ�Ƚ� (2a+2bk)^2��4(1+k^2)(a^2+b^2-r^2)��С���ɣ�ֱ֪����Բ�Ƿ��ཻ,�Ӷ��Ƴ������Ƿ񳬹�·��, �ڲ����㸡��Ĵ�������Ҫ������صĲ��� 
    
   uint32	tl;
   int xQ, yQ,xE,yE,r;
	long long tp1, tp2, tp3;//,DistQSPower2, DistQEPower2;//,DistQTPower2;
	char tBuffer[128];
	
	// �ھ�γ�ȷ�Χ��ģ�ֱ���ж�Ϊ��·��
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
	
	Uart.puts("·�η�Χ�ڣ������\r\n");
			
	if( LonS==LonE )		// ���ϱ������·��ֻ�㾭�ȼ���뼴��
	{
		tl = Calculate2DotDistance(LonQ,LatQ,LonS,LatQ);
		if( tl > RoadWidth)	return TRUE;
		else						return FALSE;
	}
	else if( LatS==LatE)	// �����������·��ֻ��γ�ȼ���뼴��
	{
		tl = Calculate2DotDistance(LonQ,LatQ,LonQ,LatS);
		if( tl > RoadWidth)	return TRUE;
		else						return FALSE;
	}
				
	// ��ڲ���Ϊ�Ŵ�10^7���Ķȣ�����Ҫת��Ϊ�Ŵ���10^4�ķ֣������ȳ���60���ٳ���1000
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


   
   ///////////////////////////////////////////////// ��S��E��ɵ�Χ���ڣ����ж��Ƿ���·����
   r = RoadWidth / 2; 			//
	
     // ���Q���ھ�γ�ȵ�ֱ������ϵ��ӳ���Q(xQ,yQ)
   tl = xQ = Calculate2DotDistance_dm(LonS, LatS, LonQ,LatS); //������ƽ��
	if (LonS > LonQ)   
	{		
		xQ = -xQ;
	}
   tl = yQ = Calculate2DotDistance_dm(LonS, LatS, LonS,LatQ);		//����ƽ��   
   if (LatS > LatQ)  
	{		 
		yQ = -yQ;
	}
	    
    // ����ŵ��ھ�γ�ȵ�ֱ������ϵ��ӳ���E(xE,yE)
   tl = xE = Calculate2DotDistance_dm(LonS, LatS, LonE,LatS);		//����ƽ��	
	if (LonS > LonE)   
	{
		xE = -xE;
	}
   tl = yE = Calculate2DotDistance_dm(LonS, LatS, LonS,LatE);   // ����ƽ��   
	
   if (LatS > LatE)  
	{ 
		yE = -yE;
	}
	   
	r = r * 10;	  // תΪ���ף���ΪyQ�ȵȵ�λ�Ƿ���

	// (1+k^2)x - (2x1+2ky1)x + (x1^2+y1^1-r^2) = 0 , ����y1 = yQ, x1 = yQ, k = yE/xE
    
   // k = yE/xE			��yEС��xEʱ����������0����������Ҫ��yE�Ŵ�100��������
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

    if (tp1 > tp2)		// Բ��ֱ���н��㣬��·����
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

	// ��ʵ��ʹ�ô����ж���·�ߣ��洢��Flash��

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
	for(i=0;i<RoadAttr.Total;i++)		// ÿ��·����һ�飿��������
	{
		if(Current)		// ���Ȳ�������߹���·��
		{
			EnableCnt = Current-1;
		}
		else
		{
			Enable = 0;
			for(;EnableCnt<MAX_FENCE_NUM;EnableCnt++)
			{
				k = EnableCnt>>3;		// ����8
				t = EnableCnt%8;
				if(RoadAttr.Enable[k]&(1<<t))	
				{
					Enable = TRUE;		break;
				}
			}
			NOP();	NOP();
			if( Enable != TRUE )	
			{
				if(TD.GPS==tGPS_ROAD_LINE_FENCE)		Uart.puts("δʹ��·����Ч��־ ");
				break;	 	// δ�ҵ�ʹ�ܵ�Χ��
			}
		}
				
		Addr = FF_ROAD_PLAN_START_PAGE;		// 
		Addr += (EnableCnt<<1);					// ÿ��·�߹̶��洢����Ϊ2ҳ

		
		
		DFF.PageRead( Addr, pData );
		DFF.PageRead( Addr+1,pData+FF_BYTES_PER_PAGE );

		if(TD.GPS==	 tGPS_ROAD_LINE_FENCE)
		{
			Uart.puts("·����FLASH��ַ Test Data:");Uart.putb(1,4,(uchar *)&Addr);	 Uart.puts("\r\n");
			Uart.puts("road data Test Data:");Uart.putb(1, 512,pData); Uart.puts("\r\n"); os_dly_wait(10);
		}
		
		//EnableCnt��0��ʼ������ǰΧ���Ŵ�1��ʼ�������ȼ�1���ٸ�ֵ����ǰ���ߺ�
		EnableCnt++;
		
		pRoad = (pROAD_LINE_STRUCT)pData;
		
		TotalDot = (pRoad->DotNumber)-1;		// ��·���� = �յ����� - 1
		for(j=0;j<TotalDot;j++)					// ÿ��·�Σ��յ㣩��һ��
		{				
			
			//////////////////////////////////////////// ���Դ���
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
			if( t != TRUE)			// ���������жϳ�·�ߣ����Է���TRUEΪ��·��
			{	// ����·��
				RoadAttr.CurrentNo = EnableCnt;
				RoadAttr.RemoteID	= pRoad->ID;				// ·��ID
				RoadAttr.SecNo		= pRoad->Dot[j].ID;		// ·��ID
				RoadAttr.Width		= pRoad->Dot[j].Width;	// ·��
				RoadAttr.Lon1		= pRoad->Dot[j].Lon;
				RoadAttr.Lat1		= pRoad->Dot[j].Lat;
				RoadAttr.Lon2		= pRoad->Dot[j+1].Lon;
				RoadAttr.Lat2		= pRoad->Dot[j+1].Lat;
				
				RoadAttr.InTimeCnt	= 0;
				RoadAttr.OutTimeCnt	= 0;
				
				InOutAreaType = eBB_AREA_ALARM_ROAD_SECTION;			// ����ʹ��
				InOrOutArea = eBB_IN_AREA;
				FenceOrRoadAlarmID = RoadAttr.RemoteID;		
				
				DFE.Write(EE_ROAD_ATTR,LEN_EE_FENCE_ATTR,(uchar*)&RoadAttr);
				
				SetBufferValue( Tp.Buffer,LEN_TP_BUF,0);
				strcpy((char*)Tp.Buffer,"\r\n����");
				Int2Str( RoadAttr.CurrentNo,(char*)&Tp.Buffer[6]);
				strcat((char*)Tp.Buffer,"��·�ߣ���");	
				Len = strlen( (char*)Tp.Buffer);
				Int2Str( RoadAttr.SecNo,(char*)&Tp.Buffer[Len]);
				strcat((char*)Tp.Buffer,"·�Σ�ԭID��");	
				Len = strlen( (char*)Tp.Buffer);
				Int2Str( RoadAttr.RemoteID,(char*)&Tp.Buffer[Len]);
				strcat((char*)Tp.Buffer," ");	
				Uart.puts((char*)Tp.Buffer);
				
				JT808Flag.VS7.Bits.InOutLine = TRUE;
				JT808Flag.VS7.Bits.LineDeviate = FALSE;
				
				break;			// ��������·�ε�ѭ��
			}				
		}
		if( RoadAttr.CurrentNo )	break;
		
		Current = 0;			// �´�ѭ�������ҵ�ǰ·��		
	}
	///////////////////////////////////////////////////////////////////////////////////////////
	if( (RoadAttr.CurrentNo==0) && ( RoadAttr.PreNo !=0) )			// δ�ҵ�·�Σ��ѳ�·��
	{				
		
		InOutAreaType = eBB_AREA_ALARM_ROAD_SECTION;			// ����ʹ��
		InOrOutArea = eBB_OUT_AREA;
		FenceOrRoadAlarmID = RoadAttr.RemoteID;			
				
		JT808Flag.VS7.Bits.InOutLine = TRUE;
		JT808Flag.VS7.Bits.LineDeviate = TRUE;
		
		
		SetBufferValue( Tp.Buffer,LEN_TP_BUF,0);
		strcpy((char*)Tp.Buffer,"�ӵ�");
		Int2Str( RoadAttr.PreNo,(char*)&Tp.Buffer[4]);
		strcat((char*)Tp.Buffer,"��·��ƫ�룬ԭID��");	
		Len = strlen( (char*)Tp.Buffer);
		Int2Str( RoadAttr.RemoteID,(char*)&Tp.Buffer[Len]);
		strcat((char*)Tp.Buffer,"��·�κţ�");
		Len = strlen( (char*)Tp.Buffer);
		Int2Str( RoadAttr.SecNo,(char*)&Tp.Buffer[Len]);
		
		strcat((char*)Tp.Buffer," ");	
		Uart.puts((char*)Tp.Buffer);
		
		if(RoadAttr.SecNo)
		{
			RoadAttr.SecNo			= 0;				// ·��ID			
			DFE.Write(EE_ROAD_ATTR,LEN_EE_FENCE_ATTR,(uchar*)&RoadAttr);
		}
		
	}
	MyFree(pData);
	#undef	LEN_TP_BUF
}




/************************************************************************************************
** �������ƣ�CheckRoadFence()
** �������ܣ���·Χ�����
** ��ڲ�������γ��
** ���ڲ���������Χ����(no��Ϊ0ʱ,��ʾ�ڵ�no��Χ����;noΪ0ʱ,��ʾ�������е�Χ����)
** ȫ�ֱ�����
** �޸�ʱ�䣺
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
		
	if(RoadAttr.Total==0)																   // ��Χ���ż��		
	{
		JT808Flag.VS7.Bits.InOutLine = 0;
		if(RoadAttr.CurrentNo)
		{
			SetBufferValue( (uchar*)&RoadAttr,LEN_EE_FENCE_ATTR,0);
		}
		if(TD.GPS==tGPS_ROAD_LINE_FENCE)		Uart.puts("δ����·�� ");
		return ;
	}
	
	
	
	if(RoadAttr.CurrentNo==0)
	{	
		Uart.puts("��������·��\r\n\0");
		SearchRoadSection(0,iLon,iLat);			// ����ȫ��·�ߺ�·��		
	}
	else
	{
		NOP();	NOP();	NOP();	NOP();
		
		//////////////////////////////////////////// ���Դ���
		Uart.puts(" Current Pos S E W Test Data:");
		Tp.B32[0] = iLon;		Tp.B32[1] = iLat;
		Uart.putb(1,8,Tp.Buffer);			
		Uart.putb(1,16,(uchar*)&RoadAttr.Lon1);
		Uart.putb(1,1,(uchar*)&RoadAttr.Width);
		Uart.puts("\r\n\0");
		
		// �Ȳ����Ƿ���Ȼ�ڴ洢�ĵ�ǰ·���ƫ�뵱ǰ·�β���������·��
				
		
		t = CheckIsOutRoad(	RoadAttr.Lon1, RoadAttr.Lat1, RoadAttr.Lon2, RoadAttr.Lat2,
									iLon,iLat, RoadAttr.Width	);
		if( t != TRUE)		// ���������жϳ�·�ߣ����Է���TRUEΪ��·��
		{
			RoadAttr.InTimeCnt++;			
			RoadAttr.PreNo = RoadAttr.CurrentNo;
			if(TD.GPS==tGPS_ROAD_LINE_FENCE)
			{
				SetBufferValue( Tp.Buffer,LEN_TP_BUF,0);
				strcpy((char*)Tp.Buffer,"  ��ǰ��");
				Int2Str( RoadAttr.CurrentNo,(char*)&Tp.Buffer[8]);
				strcat((char*)Tp.Buffer,"��·�ߣ���");	
				Len = strlen( (char*)Tp.Buffer);
				Int2Str( RoadAttr.SecNo,(char*)&Tp.Buffer[Len]);
				strcat((char*)Tp.Buffer,"·�Σ�ԭID��");	
				Len = strlen( (char*)Tp.Buffer);
				Int2Str( RoadAttr.RemoteID,(char*)&Tp.Buffer[Len]);
				strcat((char*)Tp.Buffer," ");	
				Uart.puts((char*)Tp.Buffer);
			}
		}
		else	// �ѳ�·�Σ����²�������·��
		{	
			SearchRoadSection(RoadAttr.CurrentNo,iLon,iLat);
		}
	}
	
	#undef	LEN_TP_BUF
	

}
/************************************************************************************************
** �������ƣ�ChekTimeRand()
** �������ܣ�ʱ�䷶Χ�ڼ�⣬��ʱ�䷶Χ�ڷ���0�����򷵻�1
** ��ڲ����� ��ʼʱ�� St  ����ʱ��Et
** ���ڲ����� 
** ȫ�ֱ�����
** �޸�ʱ�䣺
*************************************************************************************************/
uchar 			GPSCls::ChekTimeRand(uint32 St,uint32 Et)
{
	uchar  result = 1;
	uint32  Ntime;

   if(St<(24*3600))   //����С��һ�죬��Ϊ����ʱ�䣬��ÿ���ĳ��ʱ��
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
 	return GYPack.Speed/100;			// ԭ������100��
}

uint16		GPSCls::Get1MinAvgSpeed(void)
{
 	return AvgSpeed1Min;			// ԭ������100��
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
  �������ƣ�
  ����������
  ��ڲ���: 
  ���ڲ�����
  ȫ�ֱ���: 
  ����ʱ�䣺
  �޸�ʱ��: 
�޸ļ�����
��ȡ�Ѷ�ä��ҳ���� GY S \x00 R \xa0  \x34\x03   \x04\x00   \x00 \x00 \x00 \x00 
GYE-Test \x00 5 \x00 \x00 \x00 \x00 \x00 \x00 \x00 \x00 \x00 \x00 \x00 \x00 
GY S B
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
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
			if(WriteOK ==OK )		strcpy( (char*)tBuffer,"���������ɹ���\0");
			else						strcpy( (char*)tBuffer,"��������ʧ�ܣ�\0");	
		}
		
		//////////////////////////////////////////////////////////////////		
		for(Tp.B16[0]=0,i=0;i<252;i++)	{	Tp.B16[0] += GpsBlindBuffer[i];	}
		Tp.B16[1] = FlashPageCnt.BlindSave;
		memmove( (uchar*)&GpsBlindBuffer[252],Tp.Buffer,4);
		WriteOK = DFF.PageProgramWaitOK(Addr,(uchar*)GpsBlindBuffer,5);				// д��FLASH
		if(WriteOK ==OK )		strcat( (char*)tBuffer,"�洢9��ä�����ݣ�P\0");
		else						strcat( (char*)tBuffer,"�洢9��ä�����ݣ����ݴ���P\0");
		
		Addr = strlen((char*)tBuffer);
		Int2Str( FlashPageCnt.BlindSave, (char*)&tBuffer[Addr] );
		Uart.puts( (char*)tBuffer );	
		FlashPageCnt.BlindSave++;
		GpsBlindCnt = 0;
	}
}



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
  �������ƣ�
  ����������
  ��ڲ���: 
  ���ڲ�����
  ȫ�ֱ���: 
  ����ʱ�䣺
  �޸�ʱ��: 
�ض����Ȳ������ݣ����������� C5   (��Զ�̷���ģ�����ò������ٷ����������Զ���ٷ��봫ָ���󱾵������
GY C \x00 \x00  \x00  \x00  \x00   

�ض����Ȳ������ݣ����������� 24  
GY S  \011 \001  \x00 \x00 \x00 \x00  	\x00 \x00 \x00 \x00   \x00 \x00  
GY S  \014 \002  \x3A \x00 \x00 \x00    \x00 \x00 \x00 \x00   \x00 \x00  

GY F \x00 R D \xE0\x1E \x10\x00  
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void			GPSCls::SaveHistoryData(uchar *pData)
{	
	
	//////////////// ��ʱ���Σ�Ӧ����⣬����ʹ�������ϱ���ÿ��ֻ�ܴ�1����	
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
			if(WriteOK ==OK )		strcpy( (char*)tBuffer,"���������ɹ���\0");
			else						strcpy( (char*)tBuffer,"��������ʧ�ܣ�\0");					
		}		
		//////////////////////////////////////////////////////////////////
		for(Tp.B16[0]=0,i=0;i<252;i++)	{	Tp.B16[0] += GpsSaveBuffer[i];	}
		Tp.B16[1] = FlashPageCnt.HistorySave;
		memmove( (uchar*)&GpsSaveBuffer[252],Tp.Buffer,4);
		WriteOK = DFF.PageProgramWaitOK(Addr,(uchar*)GpsSaveBuffer,5);				// д��FLASH						
		if(WriteOK ==OK )		strcat( (char*)tBuffer,"�洢9����ʷ���� \0");
		else						strcat( (char*)tBuffer,"�洢9����ʷ���ݣ����ݴ��� \0");
		
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
	//////////////////////////////////////////////	ʱ���ַ�������'\0'����
	sNowTimeString[0] = gtlRtc.Hour/10+'0';
	sNowTimeString[1] = gtlRtc.Hour%10+'0';
	sNowTimeString[2] = gtlRtc.Min/10+'0'; 
	sNowTimeString[3] = gtlRtc.Min%10+'0';	
	sNowTimeString[4] = gtlRtc.Sec/10+'0';	
	sNowTimeString[5] = gtlRtc.Sec%10+'0';	
	sNowTimeString[6] = 	0;

}



// �����䴦��
void			GPSCls::SecondHoppingProcess(uchar Blind)
{
	uint32		tl,Sec,ModifyLat,ModifyLon,StartLat,StartLon;
	uint32		ModifySpeed,ModifyAltitude,StartSpeed,StartAltitude;
	RTC_STR		tRtc;
	
	//////////////////////////////////////////// ���Σ��������ģ�ⳡ����������������
	if(  ((GYPack.RtcSecond-1) > LastGYPack.RtcSecond)  &&  ( LastGYPack.RtcSecond > (12*365*24*3600UL) )   )
	{
		//if(TD.GPS==tGPS_MESSAGE_SAVE)		Uart.puts(" ���벹���� ");
		tl = GYPack.RtcSecond - LastGYPack.RtcSecond - 1;	
		if(tl>5)	tl=5;
		//////////////////////////////////////////////////////////////////γ��
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
		//////////////////////////////////////////////////////////////////����
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
		////////////////////////////////////////////////////////////////////�ٶ�
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
		////////////////////////////////////////////////////////////////////����
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
		
		Sec = (LastGYPack.RtcSecond+(8*60*60UL));			// ת���ɱ���ʱ��
		while( tl )
		{
			Sec++;										// ����ʱ��
			Rtc.SecondToDateTime( Sec, (RTC_STR*)&tRtc );
			LastJTPack.Date[0] = HexToBcd(tRtc.Year);
			LastJTPack.Date[1] = HexToBcd(tRtc.Month);
			LastJTPack.Date[2] = HexToBcd(tRtc.Day);
			LastJTPack.Time[0] = HexToBcd(tRtc.Hour);
			LastJTPack.Time[1] = HexToBcd(tRtc.Min);
			LastJTPack.Time[2] = HexToBcd(tRtc.Sec);
			
			StartLat += ModifyLat;					// ����γ��
			LastJTPack.Latitude = SwapINT32( StartLat/10 );
			
			StartLon	+= ModifyLon;					// ��������
			LastJTPack.Longitude = SwapINT32( StartLon/10 );
			
			StartSpeed += ModifySpeed;				// �����ٶ�
			LastJTPack.Speed = SwapINT16( StartSpeed);
			
			StartAltitude += ModifyAltitude;		// �������θ߶�
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
			Uart.puts(" �������� ");
		}
	}
	//////////////////////////////////////////// ���γ������
}




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
  �������ƣ�
  ����������
  ��ڲ���: 
  ���ڲ�����
  ȫ�ֱ���: 
  ����ʱ�䣺
  �޸�ʱ�䣺	
*/

extern		uchar		RecordTaskRun,PicDataUpLoadFlag;
extern   	uchar  	*pShareMem;
extern		uchar		CreateUserFlag;
#include		"..\CAN\CAN.h"
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
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
			Uart.puts("��δ������ ");	
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
	
	////////////////////////////////////////// IC����֤ʱ���������ݲ�����
//	if(IC_Card_Auth_GPRS==TRUE)		return;		
	
	// ����֮ǰ����ȡ���±�־λ״̬�ͱ���״̬
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
//	if(TD.GPS==tGPS_VIRTUAL_GPS_VALID)		Uart.puts(" ���Ӳ���1 ");
//	
//	if( ReadyTestBlind ) 
//	{
//		if((GPSValidFlag==GPS_VALID_FLAG)||(VirtaulGPS_Valid == TRUE))
//		{
//			if(GYPack.RtcSecond >= 0x18C88194 )		// 13 03 05 10 12 36
//			{
//				JT_ShutMobileCnt	= TRUE;
//				ReadyTestBlind = 0;
//				Uart.puts("\r\n��ǰ18���¼ä������\r\n");
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
				Uart.puts("��Ȩ�� ");
				GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_AUTH,0,NULL);
				DialOK_NeedAuthFlag = 0;		// 
			}			
		}
//		else if( (GYPack.RtcSecond % SendInterval.HeartbeatTime) == 0 )								// ������
//		{
//			#if (WUXI_TEST!=1)
//			if( RecordTaskRun||PicDataUpLoadFlag )		return;
//			//Uart.puts("������ ");
//			GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_HEARTBEAT,0,NULL);	
//			#endif	
//		}
	}
	
	
	if( RecordTaskRun||PicDataUpLoadFlag  )		return;		// �ϱ���ý������ʱ��������������
	
	
	if(TD.GPS==tGPS_VIRTUAL_GPS_VALID)		Uart.puts(" ���Ӳ���2 ");
	
	GPRS.RemoteGetVDR_Data();


	GSM.CalculateSendInterval(0);
	
//	if( JT808_GPRS_TempPack == TRUE )					// ��ʱ������������͵����ݣ�ͨ�����б�����־
//	{
//		#if (0)//#if (WUXI_TEST!=1)
//		Uart.puts("������־����������λ����Ϣ ");	 
//		#endif
//		JT808_GPRS_TempPack = 0;
//		JT808_QuicklySendPack = TRUE;
//		#if (0)//#if (WUXI_TEST!=1)
//		if(gCnt.SystemSecond>60)				// ϵͳ����60���ű���
//			GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_POS_REPORT,0,NULL);
//		#endif
//
//	}
//	else if( SendInterval.T_ContinuedTime > 0)						 							// ��ʱ��������Ч
//	{
//		if( (GYPack.RtcSecond % SendInterval.T_Second) == 0 )
//		{
//			Uart.puts("��ʱ�������λ����Ϣ ");	 			// ������ӡ����жϼ�Ȩ�ɹ���ų��ӡ�
//			GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_POS_REPORT,0,NULL);			
//		}
//	}
//	else if( (SendInterval.Second != 0xffffffff)&& (SendInterval.Second != 0) )	 	// ��ʱ���
    if( (SendInterval.Second != 0xffffffff)&& (SendInterval.Second != 0) )	 	// ��ʱ���
	{
		if( (gCnt.GetDataSecond % SendInterval.Second) == 0 )
		{
			//#if (WUXI_TEST!=1)
			Uart.puts("��ʱ����λ����Ϣ ");	 	// ������ӡ����жϼ�Ȩ�ɹ���ų��ӡ�
		//	#endif
			GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_POS_REPORT,0,NULL);			 
		}
	}
//	else if( (SendInterval.Distance != 0xffffffff)&& (SendInterval.Distance != 0) )
//	{
//		if( (SystemLog.GPS.TotalMileage % SendInterval.Distance)==0)						// ��������
//		{
//			Uart.puts("���෢��λ����Ϣ ");	 		// ������ӡ����жϼ�Ȩ�ɹ���ų��ӡ�
//			GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_POS_REPORT,0,NULL);			
//		}
//	}
//	#if (0)//#if (WUXI_TEST!=1)
//	if( (GYPack.RtcSecond % (10*60)) == 0 )
//	{
//		if (TD.GPRS==tGPRS_TMP_TEST) { Uart.puts("��ʱ����ϵͳ��־ ");	 }
//		GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_SYS_LOG_UPLOAD,256,(uchar*)&SystemLog);
//	}
//	#endif
	
	if(TD.GPS==tGPS_VIRTUAL_GPS_VALID)		Uart.puts(" ���Ӳ���5 ");
	

	
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

