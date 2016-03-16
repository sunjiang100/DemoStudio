/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												Фң���ӣ����ݣ��з�����
$										Shawyoo Electronics (GuangZhou) R&D Center
$                       	http://www.shawyoo.com		 Email: autoele@qq.com
$								 QQȺ��9069471/9069433/9524253		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      �ļ�����
	�ļ���: 	  Speed.c
	���������� �ٶ���ش���.ͣ��ǰ20�����ݣ�ƣ�ͼ�ʻ���ݣ��������ݣ�360Сʱ���ݣ�
	���뻷����Keil for ARM + ARTX1.10
	Ŀ��CPU:  LPC2000
	���ߣ�
	����ʱ�䣺								 
	����༭������UltraEdit 11.00a+  �Ʊ�����Ϊ�����ַ�������ɶ��Ը��á�
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
// <h>�ٶ��������


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>TEST_STATUS_TO_PC: ���ٶȺͿ�����ԭʼ������PC  <0=> ������ <1=> ����
//   	<i> Default: ������
*/
#define		TEST_STATUS_TO_PC		0				// ���ٶȺͿ�����ԭʼ������PC

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>MAX_SPEED_KM_H: �������ʱ��ֵ <1800-3800>
//   	<i> Default: 3000
*/
#define		MAX_SPEED_KM_H			300

// <h>�ٶ�ֵ�������

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>SPEED_FILTER: ѡ���Ƿ���Ҫ�ٶ�ͻ���˲�  <0=> ���˲� <1=> �˲�
//   	<i> Default: �˲�
*/
#define		SPEED_FILTER			1

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>SPEED_ADJUST: ѡ���Ƿ���ҪУ���ٶ�  <0=> ��У�� <1=> У��
//   	<i> Default: У��
*/
#define		SPEED_ADJUST			0


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>ADD1_OVER_FIVE: �ٶ�ֵ�������봦��  <0=> ������ <1=> Ҫ��������
//   	<i> Default: Ҫ��������
*/
#define		ADD1_OVER_FIVE			1

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>MAX_ACC_KM: ���ٶȵ�����ֵ(ÿ��仯��ʱ��/10) <1-100>
//   	<i> Default: 50
*/
#define		MAX_ACC_KM				80	  			// �����ٶȣ���ÿ���ٶȱ仯������40km

// </h>

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>CHECK_PREQUENCY: �ٶȲɼ���Ƶ�� <4-5>
//   	<i> Default: 5
*/
#define  	CHECK_PREQUENCY		5       		// �ٶȼ��Ƶ�ȣ�5��/��


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>SENDOR_FACTOR: �ٶȴ���������ϵ������ÿת�������� <1-100>
//   	<i> Default: 8
*/
#define		SENDOR_FACTOR			8			// �ٶȴ�����ÿת��������

// <h>������ʻʱ�����޺���Ϣʱ������
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>INTERVAL_RS: ѡ����ʱ��Ϊ�̶�ֵ  <0=> ��λ���趨 <1=> �̶�Ϊ����ֵ
//   	<i> Default: �̶�Ϊ����ֵ
*/
#define		INTERVAL_RS				0

#define		TEST_CODE_FOR_SPEED		1

// </h>

// </h>

// $$$$$$$$$$$$$$$$$$$ <<< end of configuration section >>> $$$$$$$$$$$$$$$$$$$$$$$$$$$$


#define	DEBUG_SPEED_INFO_OUPTPUT	1  //  1: �����ٶ���ص�����Ϣ���		0: �����ٶ���ص�����Ϣ���

uint32		SpeedSecond; // ���ٶȼ�ʱ


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
		os_evt_wait_or( 0x0001, (2*100) );		// ����ÿ��ִ��1��

		if( (U0_Iap_Flag!=0)|| (GG_Status.Flag.Bits.UpdateRemoteSet == 1))  continue; // Զ������
		
		
		if( SamlePeriod1S || TestAccuracy || TestBlind || TestCanBus )		continue;	// ���Ȳ��Ի�ä�����ԣ��������������

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
	�������ƣ�GetSpeedValue
   ��������������ͣ�����ݡ�48Сʱ�ٶ����ݡ�360Сʱλ����Ϣ				 
   ��ڲ�������
   ���ڲ�������
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
	 
*/ 
uchar			OnlyUsePulseSpeed,OnlyUseGPS_Speed;
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
uint16			SpeedCls::DrivingDataRecording(uint16	Speed)      //ÿ����ִ��һ��
{
	uchar SaveStopFlag = FALSE;
	static uchar FirstRun = 0xaa;
	static uchar  ValidPosMin;			  // ��λ�ķ���
	static uint32 NoMoveSecond = 0;
	static uint32 StopSecond = 0xff, DrivingSecond = 0;			  // �����ʼ��StopSecond Ϊ0xff��Ϊ�˸տ�ʼ���ܾͲ����¹��ɵ�
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


	pRtc = (pRTC_STR)&tRtc;						// ��ʵʱʱ�Ӷ�ȡ�ٶ�ֵ��ÿ�봦����ƽ���ٶȣ�ÿ���Ӵ������ƽ���ٶ�
	Rtc.GetRTC(pRtc);
	
	uiSpeed = Pulse.GetSecSpeed(Tp.Buffer);	// ��ȡ1�������ٶȺͿ���������
	SwitchStatus = Tp.Buffer[1];


	if ((uiSpeed<5)&&(SpeedStatusJudge.Status==SPEED_STATUS_UNNORMAL)&&(GPSValidFlag == GPS_VALID_FLAG))
	{
		Speed = 	uiGPS_Speed;

		// ���˵�����200km/h���ٶ�
		if (Speed>2000)
		{
			Speed = 0;
		}
		
		// �����¹��ɵ�
		for (i=0; i<10;i += 2)
		{
			Tp.Buffer[i] = Speed/10;
		}

	}
	else
	{
		Speed = uiSpeed;
	}
	

	
	//if ((Speed > 20)&&(gCnt.SystemSecond > 30)&&(SpeedSecond>10))		  // �ٶȴ���5km/h	����ϵͳ����30���	��ֹͣ��־
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
					{  KeyDisp.SendDataToLCM_YH(eLCM_U_CENTER_TMP_MSG,(7*2+1),(uchar*)"\x47������ʻԱ��");	}
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


	
	///////////////////////////////////////////////////////////////////// ���5���ٶ����ݣ������ж�ͣ��
	Last5Speed[4] = Last5Speed[3];		Last5Speed[3] = Last5Speed[2];
	Last5Speed[2] = Last5Speed[1];		Last5Speed[1] = Last5Speed[0];
	Last5Speed[0] = Speed;

	//Uart.puts("Last5Speed Test Data:"); Uart.putb(1,10,(uchar *)&Last5Speed[0]); Uart.puts("\r\n"); 

  
	/// �����١��������ж�
	if( Last5Speed[0] < Last5Speed[1] )			// ���ٶ�
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
						Uart.puts("�������ϱ� ");
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
	else 	// ����
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
					  	Uart.puts("�������ϱ� ");
					}
					
				}
				else
					JT808ExtenVsFlag.VS7.Bits.AccelAdd = 0;
			}
		}
	}
		  												 
	// ��30�� 0.2���˲ʱ�ٶ� 
	for(i=29;i>0;i--)
	{
		memmove( (uchar*)&StopData[i],(uchar*)&StopData[i-1],10);
	}
	memmove( (uchar*)&StopData[0],Tp.Buffer,10);		// ���1�����ݣ������ڵ͵�ַ

	//Uart.puts("speed Test Data:"); Uart.putb(1,300,(uchar*)&StopData[0]); Uart.puts("\r\n");

	//Uart.puts("sec speed dot 2 Test Data:"); Uart.putb(1,10,Tp.Buffer); Uart.puts("\r\n");

   
	/// ��λ��Ч����ʻ10�룬10��λ��û�䣬��Ϊ�¹��ɵ�	 ����Ϊ�Ƿ�����������ת��λ�ò���
	if (GPSValidFlag == GPS_VALID_FLAG)
	{	 
		//if (SpeedSecond<10)	  // ��Ϊδ��ʻ
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
			
	   // ������ľ��� 1853��/�֣�Խ���������ϼ�ʱ��ÿ�־��ȵľ���Խ��ԽС��ÿ3��С1��
		// γ��3706��/��	
		// ��γ��Ϊ˵��+10��ʾƫ3�ף� �Ծ�����˵��+20 ƫ3��															
		if ((((Tp.B32[0]<Tp.B32[1]+100)&&(Tp.B32[0]+100>Tp.B32[1])) && 	  // ����
				((Tp.B32[2]<Tp.B32[3]+100)&&(Tp.B32[2]+100>Tp.B32[3]))))			// γ��
		//if ((GPS.LastGBPack.Lat==GPS.GBPack.Lat) &&(GPS.LastGBPack.Lon==GPS.GBPack.Lon))
		{
			//Uart.puts("λ�ò���");
	  		if ((SpeedSecond>10)&&(++NoMoveSecond==10))// ��ʻ10s
			{
				SaveStopFlag = TRUE;
				//#if (DEBUG_SPEED_INFO_OUPTPUT == 1)
				Uart.puts("�����¹��ɵ���");		 
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
			Uart.puts("�����¹��ɵ�һ���");
			//#endif
		}

		SetBufferValue(tPage.Buffer,256,0);
		pSaveStopData = (pSAVE_STOP_DATA)&tPage;
		//Rtc.GetRTC_BCD((pRTC_STR)Tp.Buffer);
		if (SaveStopFlag==FALSE)
		{
			//tl = Rtc.DT.RtcSecond - 2;			// ʹ�� Rtc.DT.RtcSecond ������48Сʱ�󣬲������¹ʵ���5��6�루���ֲ�׼��
			tl = Rtc.DateTimeToSecond(pRtc); 
			tl -= 2;
		}
		else
		{
			//tl = Rtc.DT.RtcSecond;
			tl = Rtc.DateTimeToSecond(pRtc);
		}
		Rtc.SecondToDateTime_BCD(tl, (pRTC_STR)Tp.Buffer);
		
		memmove( &(pSaveStopData->DateTime[0]),(uchar*)&Tp.Buffer[0],6);							// ͣ��ʱ��������ʱ����

		//Uart.puts("stop time Test Data:"); Uart.putb(1,6,&(pSaveStopData->DateTime[0])); Uart.puts("\r\n");

	  	memmove( &(pSaveStopData->DriverID[0]),&CurrentDriverInfo.ID[0],18);			// ��ǰ��ʻԱ��ʻ֤����
		//Uart.puts("CurrentDriverInfo.ID Test Data:"); Uart.putb(1,18,&CurrentDriverInfo.ID[0]); Uart.puts("\r\n");
		//memmove( &(pSaveStopData->Data.Buffer[0]),(uchar*)&StopData[10],200);			// ͣ��ǰ20������
		if (VehicleStatus.VS1.Bits.MainPowerLost==1)
		{
			memmove( &(pSaveStopData->Data.Buffer[0]),(uchar*)&StopData[2],200);		
		
		}
		else if ((uiSpeed<5)&&(SpeedStatusJudge.Status==SPEED_STATUS_UNNORMAL))
		{
			 memmove( &(pSaveStopData->Data.Buffer[0]),(uchar*)&StopData[0],200);
			 if (pSaveStopData->Data.Buffer[0] != 0)  pSaveStopData->Data.Buffer[0] = 0; //  ȷ��ͣ��һ����Ϊ0		
			 Uart.puts("�����ٶ��쳣ʱ���¹��ɵ�");
		}
		else
		{
			memmove( &(pSaveStopData->Data.Buffer[0]),(uchar*)&StopData[1],200);	
		}
		//memmove( &(pSaveStopData->Pos),(uchar*)&GPS.GBPack,10);	// ��ǰ��Чλ��
		memmove( &(pSaveStopData->Pos),(uchar*)&ValidLocationInfo,10);			 // ���һ����Чλ��

		//Uart.puts("save stop  Test Data:"); Uart.putb(1,200,(uchar *)&pSaveStopData->Data.Buffer[0]);	Uart.puts("\r\n");
		////////////////////////////////////////////////////////////////////////// �洢ͣ�����ݵ�Flash
		if( (FlashPageCnt.ParkingDataSave%FF_PAGE_PER_SECTOR)==0)
		{
			Addr = FF_PARKING_DATA_START_PAGE;
			Addr += FlashPageCnt.ParkingDataSave%FF_PARKING_DATA_TOTAL_PAGE;
			
			DFF.SectorEraseWaitOK(Addr/FF_PAGE_PER_SECTOR,10);
			
		}
		Addr = FF_PARKING_DATA_START_PAGE;
		Addr += FlashPageCnt.ParkingDataSave%FF_PARKING_DATA_TOTAL_PAGE;		//�õ�ʵ�ʴ洢��ҳ��ַ
		DFF.PageProgramWaitOK( Addr,tPage.Buffer,3);
		FlashPageCnt.ParkingDataSave++;
		#if (DEBUG_SPEED_INFO_OUPTPUT == 1)
		Uart.puts("�洢ͣ�����ݵ�Flash ");
		#endif	
		
		//////////////////////////// ͣ��ǰ15����ÿ����ƽ���ٶ�

		DFE.Read( FM_15MIN_SPEED,15,Tp.Buffer);				// ��ȡ�Ѵ洢��ͣ��ǰ15�����ٶ�ֵ	
		DFE.Write( FM_STOP_LAST15_SPEED,15,Tp.Buffer);
		Rtc.GetRTC_BCD((RTC_STR *)&Tp.Buffer[0]);	
		DFE.Write( FM_STOP_TIME,6,(uchar*)&Tp.Buffer[0]);
			
	}

	///// �����ϵ翪����һ������ʱ��Ҫ���һ���ϴε�λ����Ϣʱ���Ƿ�ǰСʱ�����ǣ��򱣴棬������������е�λ����Ϣ
	if (FirstRun==0xaa)
	{
		FirstRun = 0;

		#if (DEBUG_SPEED_INFO_OUPTPUT == 1)
		Uart.puts("chek and save last pos info");
		#endif

		Save360HDataToFlashAndClearFMPosData(tPage.Buffer,Tp.Buffer);
		
	}
	

	///////////////////////////////////////////////////// ��λ������ÿ���ӵ��ٶȺͿ�����
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
		ValidPosMin = tRtc.Min;		  // ��¼��λ�ķ���
		memmove( (uchar*)&ValidLocationInfo,(uchar*)Tp.Buffer,LEN_LOCATION_INFO);
	}

	
	
	if(tRtc.Sec==59)				//  ���0��ʱ�������Ҫ��1����������ʹ��59��
	{
		///////////////////////////////////////////////////// ��λ������ÿ���ӵ��ٶȺͿ���������ʼʱ��		
		if(SpeedSwMinFlag==0)
		{
			for (i=0; i<60;i++)
			{
				if (SpeedSwPerMin[0].SpeedSw[i].Speed != 0) break;					// �ٶȲ�Ϊ0
				
				if ((i<59)&&(SpeedSwPerMin[0].SpeedSw[i].Sw != SpeedSwPerMin[0].SpeedSw[i].Sw)) break;		// �������б仯
			}
			if (i==60)
			{
			//  ����Ҫ�洢�÷����ٶȺͿ�����
				SpeedSwMinFlag = 2;		  
			}
			else
			{
				//memmove( (uchar*)&SpeedSwPerMin[0].StartTime[0], (uchar*)&tRtc,6);
				Rtc.RTC_FormatToBCD(&tRtc,Tp.Buffer);
				memmove((uchar *)&SpeedSwPerMin[0],(uchar*)&Tp.Buffer[0],6);	//  ������ʱ����

				SpeedSwMinFlag = 1;

			}
		}
		else
		{
			for (i=0; i<60;i++)
			{
				if (SpeedSwPerMin[1].SpeedSw[i].Speed != 0) break;					// �ٶȲ�Ϊ0
				
				if ((i<59)&&(SpeedSwPerMin[1].SpeedSw[i].Sw != SpeedSwPerMin[1].SpeedSw[i].Sw)) break;		// �������б仯
			}

			if ((i!=60) || (SpeedSwMinFlag==1))
			{	
				///////////////////////////////////////////////////// �洢��һ���ӡ���λ������ÿ���ӵ��ٶȺͿ���������Flash
				///////////////////////////////////////////////////// ��48С�ٶ����ݣ�ÿ�δ洢���ٶȻ򿪹����仯������
				/////////////////////////////////////////////////////  �Ͱ�2���Ӵ洢1�����ݵ�1ҳFlash
				///////////////////////////////////////////////////// ��������洢�����磬���õ�ش洢����2����ʱ��
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
				Addr += FlashPageCnt.Speed48HSave%FF_48H_SPEED_TOTAL_PAGE;		//�õ�ʵ�ʴ洢��ҳ��ַ
				DFF.PageProgramWaitOK( Addr,(uchar*)&SpeedSwPerMin,3);
				FlashPageCnt.Speed48HSave++;

				DFE.Write( FM_FLASH_PAGE_CNT, LEN_FLASH_PAGE_CNT, (uchar*)&FlashPageCnt);

				#if (DEBUG_SPEED_INFO_OUPTPUT == 1)
				//Uart.puts("�洢48Сʱ��ʻ���ݵ�2�������ݵ�Flash ");		
				#endif
			}
			SpeedSwMinFlag = 0;
			
		}
		////////////////////////////////////////////////////////// ��λСʱ�ڣ�ÿ���ӵ�λ�ú�ƽ���ٶ�
		////////////////////////////////////////////////////////// ������1Сʱ�ʹ浽3ҳ��flash��
		////////////////////////////////////////////////////////// ÿСʱ��һ���������ʱ�䣬��������ݣ�����Ϊ��ǰʱ�������
		Save360HDataToFlashAndClearFMPosData(tPage.Buffer,Tp.Buffer);
		
		
		if (ValidPosMin==tRtc.Min)		 // ��λ����λ����Ϣȡ�÷��ӷ�Χ���׸���Ч��λ����Ϣ����÷��ӷ�Χ������Ч��λ����Ϣ����÷���λ����ϢΪ7FFFFFFFH��
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
		Tp.Buffer[10] = tl/VehiclePulseKV;		// �ٶ�ֵ�洢��λ����Ϣ����

		if((Tp.Buffer[10]<1)&&(SpeedStatusJudge.Status==SPEED_STATUS_UNNORMAL))
		{
			Tp.Buffer[10] = GPS.Get1MinAvgSpeed()/10;		//  �������ٶȣ������ٶ��쳣��ȡGPS�ٶ�
			
		}
	


		Addr = FM_360H_LOCATION_INFO;
		Addr += ((tRtc.Min%60)*LEN_LOCATOIN_INFO_PER_MIN);
		DFE.Write(Addr,LEN_LOCATOIN_INFO_PER_MIN,Tp.Buffer);
		NOP();NOP();NOP();

		/////////////////// ���15����ÿ����ƽ���ٶ�
		DFE.Read( FM_15MIN_SPEED,15,&Tp.Buffer[33]);		
		Tp.Buffer[32] = Tp.Buffer[10];		
		DFE.Write( FM_15MIN_SPEED,15,&Tp.Buffer[32]);

		///////////  �洢360Сʱ����  
		#if (DEBUG_SPEED_INFO_OUPTPUT == 1)
		Uart.puts("�洢360Сʱ֮1����λ����Ϣ������ ");
		//Uart.puts("location Test Data:"); Uart.putb(1,LEN_LOCATION_INFO,(uchar*)&LocationInfo);  Uart.puts("\r\n");
		#endif

		 
		     
		// д��flash
		if( (tRtc.Min==59) )
		{
			Save360HDataToFlash(tPage.Buffer);

			#if (DEBUG_SPEED_INFO_OUPTPUT == 1)
			Uart.puts("�洢360Сʱ֮60����λ����Ϣ��Flash ");
		   #endif
		}
		
	}
	return Speed;				 
}

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�Save360HDataToFlash
   ��������������360Сʱ���ݵ�flash				 
   ��ڲ���������
   ���ڲ�������
   ȫ�ֱ���: ��				
   ����ʱ�䣺
   �޸�ʱ�䣺 
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void			SpeedCls::Save360HDataToFlash(uchar *pBuffer)
{
	uchar i;
	uint32 Addr;
	// д��flash
	for (i=0; i<3; i++)
	{
		if( (FlashPageCnt.Location360HSave%FF_PAGE_PER_SECTOR)==0)
		{
			Addr = FF_360H_LOCATION_START_PAGE;
			Addr += FlashPageCnt.Location360HSave%FF_360H_LOCATION_TOTAL_PAGE;
			
			DFF.SectorEraseWaitOK(Addr/FF_PAGE_PER_SECTOR,10);
		}
		
		// ��ȡ
		SetBufferValue(&pBuffer[0], 256, 0);
		if (i==0)	 
			DFE.Read(FM_360H_LOCATION_TIME,226,&pBuffer[0]);	 // ����ʱ��ĵ�ַ��ʼ��ȡ
		else			
			DFE.Read(FM_360H_LOCATION_INFO+i*220,220,&pBuffer[6]);		//  ��ʵ�ʵ�ַ��ʼ��ȡ	
	
		///////////////////////////////////////////�洢��Flash
		Addr = FF_360H_LOCATION_START_PAGE;
		Addr += FlashPageCnt.Location360HSave%FF_360H_LOCATION_TOTAL_PAGE;
		DFF.PageProgramWaitOK( Addr,pBuffer,3);
		FlashPageCnt.Location360HSave++;
	}
	DFE.Write(FM_FLASH_PAGE_CNT, LEN_FLASH_PAGE_CNT, (uchar*)&FlashPageCnt);
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�Save360HDataToFlashAndClearFMPosData
   ��������������360Сʱ���ݵ�flash,����������ϵ�λ������				 
   ��ڲ�����256�ֽڵĻ��棬64�ֽڻ���
   ���ڲ�������
   ȫ�ֱ���: ��				
   ����ʱ�䣺
   �޸�ʱ�䣺 
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void			SpeedCls::Save360HDataToFlashAndClearFMPosData(uchar *p256ByteBuffer, uchar *p64ByteBuffer)
{
	uchar *pData;

	DFE.Read(FM_360H_LOCATION_TIME,6,p64ByteBuffer); 
	Rtc.GetRTC_BCD((RTC_STR  *)&p64ByteBuffer[6]);  
	#if (DEBUG_SPEED_INFO_OUPTPUT == 1)
	Uart.puts("Check pos time Test Data:"); Uart.putb(1,12,p64ByteBuffer); Uart.puts("\r\n");
	#endif
	if ((memcmp(p64ByteBuffer,p64ByteBuffer+6, 4) !=0)&&(FlashPageCnt.Location360HSave>0))
	{	 // ��������в��ǵ�ǰСʱ�����ȱ���֮ǰ�����ݵ�flash���������λСʱ���ݣ����´洢  
	  	#if (DEBUG_SPEED_INFO_OUPTPUT == 1)
		Uart.puts("����֮ǰλ�����ݵ�flash");
		#endif
		Save360HDataToFlash(p256ByteBuffer);

		pData = NULL;
		while(pData==NULL)
		{
			pData = MyMalloc(666);
			os_dly_wait(1);		
		}

		SetBufferValue(pData,666,0);
		p64ByteBuffer[6+4] = p64ByteBuffer[6+5] = 0; // �֣���Ϊ0
		memmove(pData, p64ByteBuffer+6,6); 

		DFE.Write(FM_360H_LOCATION_TIME,666,pData);
		NOP();NOP();NOP();
	
		//Uart.puts("up pos info time Test Data:");Uart.putb(1,6,pData); Uart.puts("\r\n");

		MyFree(pData);

		#if (DEBUG_SPEED_INFO_OUPTPUT == 1)
		Uart.puts("���������е�λ��ʱ��\r\n");
		#endif
		
	}
	
}



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�FatigueDriving
   ����������ƣ�ͼ�ʻ����				 
   ��ڲ�����1���ӵ��ٶ�
   ���ڲ�������
   ȫ�ֱ���: ��				
   ����ʱ�䣺
   �޸�ʱ�䣺 
*/ 	

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void			SpeedCls::FatigueDrivingFun(uint16	Speed)
{
	//static   uint32 SpeedSecond = 0;
	uchar 	i, SaveOverTimeDataFlag = 0;	 // ����ƣ�ͼ�ʻ��־�� 0�������棬 1�����浽���磬��������������������㣬����flash  2: ��ʱ����,Ӧ�����������λ��ʵʱ�ɼ�
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
	
	pRtc = (pRTC_STR)&tRtc;						// ��ʵʱʱ�Ӷ�ȡ�ٶ�ֵ��ÿ�봦����ƽ���ٶȣ�ÿ���Ӵ������ƽ���ٶ�
	Rtc.GetRTC(pRtc);	

	
	// ͳ�Ʋ���λ��ǰһλ��ʻԱ�Ĳ���λʱ��
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
	if(Speed>=5)		// ����1km/h����ʼ��ʱ��ʻʱ�䣬��λΪ0.1km/h.
	{
		if (SpeedSecond==0)
		{
			Uart.puts("��⵽��ʼ�ٶ�:"); Int2Str(Speed,(char *)Tp.Buffer); Uart.puts((char *)Tp.Buffer); Uart.puts(" (0.1km/h)\r\n");
		}
		++SpeedSecond;


		if ((SpeedSecond>10)&&(LimitTime.EnableCheckFatigue==TRUE)) 
		{				
			if( CurrentDriverInfoTime.TravelTimeSec == 0 ) 	// ��0�룬Ϊ��ʻ��ʼʱ��
			{
				
				CurrentDriverInfoTime.TravelTimeSec += 10; // �����ж�10S��ʱ��
		
				JT808Flag.VS6.Bits.FatiguePreAlarm = 0;
				JT808Flag.VS5.Bits.FatigueDriving = 0;
				JT808_GPRS_TempPack = TRUE;
		
				#if (DEBUG_SPEED_INFO_OUPTPUT == 1)
				Uart.puts("��ʻ��ʱ��ʼ ");
				#endif
	
				//CurrentDriverInfoTime.StartTimeSec = Rtc.DT.RtcSecond - 10;//Rtc.DateTimeToSecond(pRtc);
				CurrentDriverInfoTime.StartTimeSec = Rtc.DateTimeToSecond(pRtc) - 10;
				memmove( (uchar*)&CurrentDriverInfoTime.StartLocatoin, (uchar*)&ValidLocationInfo,LEN_LOCATION_INFO);
				DFE.Write(FM_CURRENT_DRIVER_INFO_TIME,LEN_DRIVER_INFO_TIME,(uchar*)&CurrentDriverInfoTime);			 	
			}
			CurrentDriverInfoTime.ParkingTimeSec = 0;	
			CurrentDriverInfoTime.TravelTimeSec++;
			CurrentDriverInfoTime.OneDayTravelTimeSec++;	
			////////////////////////////////////////// ��Ϣʱ��С�ڹ涨��ʱ�䣬��������ʻʱ��
			if(CurrentDriverInfoTime.RestTimeSec>0)
			{
				if( (CurrentDriverInfoTime.RestTimeSec<LimitTime.RestTimeSec))
				{
					//CurrentDriverInfoTime.TravelTimeSec += CurrentDriverInfoTime.RestTimeSec;
					CurrentDriverInfoTime.TravelTimeSec += CurrentDriverInfoTime.RestTimeSec + 10;// ��10s����ж���ʻ������Ҫ���10��
					#if (DEBUG_SPEED_INFO_OUPTPUT == 1)
					Uart.puts("��Ϣʱ�䲻�㣬���ʻʱ�� ");
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
	
		if (CurrentDriverInfoTime.TravelTimeSec>0)	 // Ҫ�м�ʻʱ�䣬û�ٶ�ʱ������Ϣʱ��
		{
			if(CurrentDriverInfoTime.RestTimeSec==0)
			{
				JT808_GPRS_TempPack = TRUE;
				#if (DEBUG_SPEED_INFO_OUPTPUT == 1)
				Uart.puts("��Ϣ��ʱ��ʼ ");
				#endif
			}

			CurrentDriverInfoTime.RestTimeSec++;	
		
			if( CurrentDriverInfoTime.RestTimeSec == LimitTime.RestTimeSec )	// ��Ϣʱ����ڹ涨ʱ�䣬������0��ʻʱ��
			{	
				for(i=0;i<2;i++)	// ������м�ʻԱ��ʻʱ��
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
						Uart.puts("��Ϣʱ����ڹ涨ʱ�� 1�����ʻʱ��");
						#endif

						// ƣ�ͼ�ʻ��������������������ʻʱ�䣺��һ����ͣ��10��͸���һ�Σ���������1
						// �ڶ����ǣ�ͣʱ�ﵽ��Ϣʱ��󣬸���ƣ�ͼ�ʻ��Ϣ���Լ�������	���FM_FATIGUE_DRIVING_CNT
						// ��Ӱ����λ��ʵʱ��ѯ��û��ƣ�ͼ�ʻ
						// Ϊ���ҵ���ͬ�Ĵ洢λ�ã���������ļ���Ҫ��1
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
				Uart.puts("��Ϣʱ����ڹ涨ʱ�� 2�����ʻʱ��");
				#endif	
			}
			else if (CurrentDriverInfoTime.RestTimeSec==10)			 //  ��Ϣ��10s˵����ͣ����	 
			{
				if( CurrentDriverInfoTime.TravelTimeSec>=LimitTime.TravelTimeSec-3)
				{
					 #if (DEBUG_SPEED_INFO_OUPTPUT == 1)
					 Uart.puts("����������ʻʱ�Ľ���ʱ��\r\n");
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

					 SaveOverTimeDataFlag = 2; // ��ʱ���µ����磬��û������ƣ�ͼ�ʻ����		 Ӧ��������⣬ͣ��10S
					 goto _SAVE_FAITGUE_DATA;
				}
				
			}
			

		}
		
		//////////////////////////////////////////// ͣ����ʱ����
		if( (LimitTime.EnableCheckParking==TRUE) && (LimitTime.ParkingTimeSec>0) )
		{
			if(CurrentDriverInfoTime.ParkingTimeSec==0)
			{
				JT808Flag.VS7.Bits.StopOverTime = 0;
				JT808_GPRS_TempPack = TRUE;
				#if (DEBUG_SPEED_INFO_OUPTPUT == 1)
				Uart.puts("ͣ����ʱ��ʼ ");
				#endif
			}
			CurrentDriverInfoTime.ParkingTimeSec++;				// // ʵ�ʲ��ԣ�δ��1ʱ������1��
			if( CurrentDriverInfoTime.ParkingTimeSec == (LimitTime.ParkingTimeSec+1) )
			{
				JT808Flag.VS7.Bits.StopOverTime = 1;
				JT808_GPRS_TempPack = TRUE;
				#if (DEBUG_SPEED_INFO_OUPTPUT == 1)
				Uart.puts("ͣ����ʱ���� ");
				#endif
				KeyDisp.SendDataToLCM_YH(eLCM_U_CENTER_TMP_MSG,(10*2+1),(uchar*)"\x47ͣ����ʱ�������Ч�ʣ�");			
			}		
			if( CurrentDriverInfoTime.ParkingTimeSec > LimitTime.ParkingTimeSec )
			{
				if( ((CurrentDriverInfoTime.RestTimeSec - LimitTime.ParkingTimeSec )%180)==0 )	// 3��������1��
				{
					JT808Flag.VS7.Bits.StopOverTime = 1;
					JT808_GPRS_TempPack = TRUE;
					#if (DEBUG_SPEED_INFO_OUPTPUT == 1)
					Uart.puts("ͣ����ʱ���� ");
					#endif
					KeyDisp.SendDataToLCM_YH(eLCM_U_CENTER_TMP_MSG,(10*2+1),(uchar*)"\x47ͣ����ʱ�������Ч�ʣ�");	
				
				}
			}
		}
		else
		{
			JT808Flag.VS7.Bits.StopOverTime = 0;		
			CurrentDriverInfoTime.ParkingTimeSec = 0;	
		}
		return;			// ͣ���������жϺ����ƣ�ͼ�ʻ
	}
	
	//////////////////////////////////////////////////////////////// δʹ�ܣ�ֱ�ӷ���

	if( LimitTime.EnableCheckFatigue!=TRUE )		return;

	////////////////////////////////////////// ÿ���Ӵ洢1�μ�ʻʱ��
	if( tRtc.Sec==59)
	{
		DFE.Write( FM_CURRENT_DRIVER_INFO_TIME,LEN_DRIVER_INFO_TIME,(uchar*)&CurrentDriverInfoTime);	
		NOP();NOP();
		DFE.Write(FM_RECENT_DRIVER_INFO_TIME,(LEN_DRIVER_INFO_TIME*2), (uchar*)&RecentDriverInfoTime[0]);		 	
	}
	

	////////////////////////////////////////////////////////// ƣ�ͼ�ʻԤ��
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
			Uart.puts("ƣ�ͼ�ʻԤ�� ");
			#endif
			KeyDisp.SendDataToLCM_YH(eLCM_U_CENTER_TMP_MSG,(8*2+1),(uchar*)"\x47��ע�⣬ƣ��Ԥ��");
			BEEP(9,0,10,0,30);			 //
			
		}
	}
	
	if(( CurrentDriverInfoTime.TravelTimeSec >= LimitTime.TravelTimeSec )&&
		(( CurrentDriverInfoTime.TravelTimeSec<LimitTime.TravelTimeSec+30*60 )))
	{
		tl = CurrentDriverInfoTime.TravelTimeSec - LimitTime.TravelTimeSec;
		if(((tl==0)||(tl==5)||(tl==10)||((tl%(5*60))==0)) || ((tl%(5*60+5))==0)||((tl%(5*60+10))==0)) 	  	// 5��������1��  ��ʾ30����
		{
			JT808Flag.VS5.Bits.FatigueDriving = 1;
			//pSave = (pSAVE_FATIGUE_DRIVING_INFO)&Tp.Buffer;
			KeyDisp.SendDataToLCM_YH(eLCM_U_CENTER_TMP_MSG,(17*2+1),(uchar*)"\x47��ȫ��ʻ��ƣ�ͼ�ʻ���棬��ע����Ϣ");
		  	BEEP(9,0,10,0,30);			 //����ƣ�ͼ�ʻ�׶�ÿ��������15��
			JT808_GPRS_TempPack = TRUE;
			#if (DEBUG_SPEED_INFO_OUPTPUT == 1)
		  	Uart.puts("ƣ�ͼ�ʻ����1 ");
			#endif
				
		}		
	}	

_SAVE_FAITGUE_DATA:
	//////////// ����ƣ�ͼ�ʻ���� /////////////////
	if (SaveOverTimeDataFlag != 0)
	{
			//SaveData = FALSE;

			pSave = (pSAVE_FATIGUE_DRIVING_INFO)&Tp.Buffer;
			//Rtc.SecondToDateTime(CurrentDriverInfoTime.EndTimeSec, pRtc);		// ���½���ʱ��ͽ���λ����Ϣ
			// ���½���ʱ��ͽ���λ����Ϣ
			CurrentDriverInfoTime.EndTimeSec = Rtc.DateTimeToSecond(pRtc);
			if (SaveOverTimeDataFlag==1)
			{
				CurrentDriverInfoTime.EndTimeSec -= LimitTime.RestTimeSec; // ͣ������Ϣʱ����㱣�棬���������ȥ����Ϣʱ��
			}
			else
			{
				CurrentDriverInfoTime.EndTimeSec -= 10;
			}
		  	memmove( (uchar*)&(pSave->EndLocation), (uchar*)&ValidLocationInfo,LEN_LOCATION_INFO);
		  	DFE.Write( FM_CURRENT_DRIVER_INFO_TIME,LEN_DRIVER_INFO_TIME,(uchar*)&CurrentDriverInfoTime);	
		  	
		  	///////////////////////////////////////////////////////////////// �洢��ǰƣ�ͼ�ʻ��Ϣ������
		  	
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
		  	DFE.Write(FM_FATIGUE_DRIVING_CNT,4,(uchar*)&tl);	 	// ƣ�ͼ�ʻ��������1��洢������	

			#if (DEBUG_SPEED_INFO_OUPTPUT == 1)	
		  	Uart.puts("����ƣ�ͼ�ʻ��¼������ "); 
			#endif 

			if (SaveOverTimeDataFlag==2)
			{
				SaveOverTimeDataFlag = 0;
				return;  //
			}

			if ((tl)&&(tl%5)==0)
			{
				// ���������һҳ��Flash�Ŀռ䣬��ŵ�Flash��
				if( (FlashPageCnt.FatigueDrivingSave%FF_PAGE_PER_SECTOR)==0)
				{
					Addr = FF_TIRE_DRIVE_START_PAGE;
					Addr += FlashPageCnt.FatigueDrivingSave%FF_TIRE_DRIVE_TOTAL_PAGE;
					
					DFF.SectorEraseWaitOK(Addr/FF_PAGE_PER_SECTOR,10);
				}				
				///////////////////////////////////////////�洢��Flash  ����1ҳflash����5��ƣ�ͼ�ʻ��¼
				SetBufferValue(Tp.Buffer,256,0);
				DFE.Read(FM_SAVE_FATIGUE_DRIVING,LEN_SAVE_FATIGUE_DRIVING_INFO*5,Tp.Buffer); // 

				Addr = FF_TIRE_DRIVE_START_PAGE;
				Addr += FlashPageCnt.FatigueDrivingSave%FF_TIRE_DRIVE_TOTAL_PAGE;
				DFF.PageProgramWaitOK(Addr,Tp.Buffer,3);
				FlashPageCnt.FatigueDrivingSave++;

				DFE.Write( FM_FLASH_PAGE_CNT, LEN_FLASH_PAGE_CNT, (uchar*)&FlashPageCnt);

				#if (DEBUG_SPEED_INFO_OUPTPUT == 1)	
		  		Uart.puts("����ƣ�ͼ�ʻ��¼��Flash"); 
				#endif 
			}

			SaveOverTimeDataFlag = 0; // ��0
	}



} // End of <eTaskTiredDrive> function






/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�OverSpeed
   �������������ټ�ʻ����				 
   ��ڲ�������
   ���ڲ�������
   ȫ�ֱ���: ��				
   ����ʱ�䣺						
   �޸�ʱ�䣺 
*/ 	
///         OVER_SPEED_MAX_CNT		LEN_OVER_SPEED_1REC		FM_OVER_SPEED_REC
//void			Delay_mS(unsigned char  c);
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
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
	if( (OverSpeed.Enable==FALSE)||(OverSpeed.HighSpeedLimit==0)||(Speed<5)||(Speed>380) )	// С��3����/Сʱ�����380����/�����Ƴ���
	{
		JT808Flag.VS5.Bits.OverSpeedAlarm = 0;
		VehicleStatus.VS3.Bits.OverSpeed = 0;
		OverSpeed.SecondCnt = 0;
		return;
	}
	

	/////////////////////////////////////////////////////////////////////////////  ����
	if( Speed > OverSpeed.HighSpeedLimit)
	{  
		if( OverSpeed.SecondCnt == 0)
		{
			#if (DEBUG_SPEED_INFO_OUPTPUT == 1)
			Uart.puts("���ټ�ʱ��ʼ ");
			#endif
			JT808Flag.VS5.Bits.OverSpeedAlarm = 0;
			VehicleStatus.VS3.Bits.OverSpeed = 0;
			JT808_GPRS_TempPack = TRUE;
		}
		OverSpeed.SecondCnt++;
		PreAlarmCnt = 0;
	
		tmp = (OverSpeed.EnableTime+1);
		if((OverSpeed.SecondCnt == tmp)||( OverSpeed.SecondCnt==tmp+5)||(OverSpeed.SecondCnt==tmp+10))	// ʵ�ʲ��ԣ�δ��1ʱ������1��
		{
			JT808Flag.VS5.Bits.OverSpeedAlarm = 1;
			VehicleStatus.VS3.Bits.OverSpeed = 1;
			JT808_GPRS_TempPack = TRUE;
			#if (DEBUG_SPEED_INFO_OUPTPUT == 1)
			Uart.puts("���ٱ��� ");
			#endif
			BEEP(9,0,2,0,10);
			
			//KeyDisp.SendDataToLCM_YH(eLCM_U_CENTER_TMP_MSG,(16*2+1),(uchar*)"\x47��ȫ��ʻ�����پ��棬�������ʻ��");
			KeyDisp.SendDataToLCM_YH(eLCM_U_CENTER_TMP_MSG,(8*2+1),(uchar*)"\x47���ѳ��٣������");
		} 
	  	if(OverSpeed.SecondCnt > OverSpeed.EnableTime )
		{
		
			tmp = OverSpeed.SecondCnt-OverSpeed.EnableTime;
			if( (tmp%(5*60)==0) || (tmp % (5*60+5)==0)	||  (tmp % (5*60+10)==0))		// 5�����������Σ�ÿ2����ʾһ��
			{
				BEEP(9,0,2,0,10);
				#if (DEBUG_SPEED_INFO_OUPTPUT == 1)
				Uart.puts("���ٱ���1 ");
				#endif
			
				JT808Flag.VS5.Bits.OverSpeedAlarm = 1;
				VehicleStatus.VS3.Bits.OverSpeed = 1;
				JT808_GPRS_TempPack = TRUE;
				KeyDisp.SendDataToLCM_YH(eLCM_U_CENTER_TMP_MSG,(8*2+1),(uchar*)"\x47���ѳ��٣������");
			}
		}	     
	}
	else 
	{
		/////////////////////////////////////////////////////////////////////////////  ����Ԥ��
		#if (WUXI_TEST != 1)
		if( (Speed+OverSpeed.PreAlarmDifference/10) >= OverSpeed.HighSpeedLimit ) 
		{
			PreAlarmCnt++;
			if( (PreAlarmCnt==1)||((PreAlarmCnt%60)==0))
			{
				JT808Flag.VS6.Bits.OverSpeedPreAlarm = 1;
				JT808_GPRS_TempPack = TRUE;
				#if (DEBUG_SPEED_INFO_OUPTPUT == 1)
				Uart.puts("����Ԥ�� ");
				#endif
				KeyDisp.SendDataToLCM_YH(eLCM_U_CENTER_TMP_MSG,(16*2+1),(uchar*)"\x47��ȫ��ʻ������Ԥ����������٣�");
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
		Uart.puts("�ٶȽ��ͣ�������ٱ�����־ ");
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
	�������ƣ�
   ������������ʼ��				 
   ��ڲ�������
   ���ڲ�������
   ȫ�ֱ���: ��				
   ����ʱ�䣺
   �޸�ʱ�䣺 
*/	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
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
		Uart.puts("����ϵ������ȡĬ��ֵ3600 ");
		DFE.Write( FM_VEHICLE_PULSE_KV,4,(uchar*)&VehiclePulseKV);
	}
	
	DFE.Read( FM_OVER_SPEED_PARA,LEN_OVER_SPEED_PARA,(uchar*)&OverSpeed);
	
	DFE.Read( FM_DRIVING_PARA,LEN_DRIVING_TIME_LIMIT,(uchar*)&LimitTime);
	
	DFE.Read( FM_CURRENT_DRIVER_INFO_TIME,LEN_DRIVER_INFO_TIME,(uchar*)&CurrentDriverInfoTime);

	memmove(CurrentDriverInfo.ID,CurrentDriverInfoTime.ID, 18);

	DFE.Read(EE_JT_DRIVER_NAME_LEN,1,(uchar *)&t); 	// ��������
	if(t>10)	t = 10;
	DFE.Read(EE_JT_DRIVER_NAME,t,CurrentDriverInfo.Name);
	
	#if (1)//#if (WUXI_TEST==1)
	OverSpeed.Enable = TRUE;					  // ʹ�ܳ��ٱ���
	LimitTime.EnableCheckFatigue =  TRUE;		// ʹ�ܳ��ٱ���
	#endif

	DFE.Read(FM_SPEED_STAUTS_JUDGE,LEN_SPEED_STATUS_JUDGE,(uchar *)&SpeedStatusJudge);

	DFE.Read(EE_OVERTIME_DRIVE_UPDATE_FLAG,1,&OverTimeDriveUpateFlag);

	DFE.Read( FM_RECENT_DRIVER_INFO_TIME, (LEN_DRIVER_INFO_TIME*2), (uchar*)&RecentDriverInfoTime[0]);


	
}	

				
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�
   ����������
   ��ڲ�������
   ���ڲ�������
   ȫ�ֱ���: ��				
   ����ʱ�䣺
   �޸�ʱ�䣺 
*/	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������				
void			SpeedCls::GetCurrentDriver(uchar *pDriver)
{
	
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�
   ����������
   ��ڲ�������
   ���ڲ�������
   ȫ�ֱ���: ��
   ����ʱ�䣺
   �޸�ʱ�䣺
*/	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������	
void			SpeedCls::SetGetPara(uchar Cmd,uchar Len,uchar *pData)
{	
	NOP();	NOP();
	switch(Cmd)
	{
		case eSPEED_CMD_SET_DRIVING_TIME:			// ƣ�ͼ�ʻ��ز���
			////////////// ���ò���ʱ�����¼�ʱ
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
		case eSPEED_CMD_GET_DRIVING_TIME:			// ƣ�ͼ�ʻ��ز���					
			memmove( pData, (uchar*)&LimitTime, LEN_DRIVING_TIME_LIMIT);
		break;


		case eSPEED_CMD_SET_OVER_SPEED:	  		// ������ز���
			OverSpeed.SecondCnt = 0;
			memmove( (uchar*)&OverSpeed,pData,LEN_OVER_SPEED_PARA);
			DFE.Write( FM_OVER_SPEED_PARA,LEN_OVER_SPEED_PARA,(uchar*)&OverSpeed );
		break;
		case eSPEED_CMD_GET_OVER_SPEED:	  		// ������ز���
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
				Uart.puts("��ֹƣ�ͼ�ʻ�����١�ͣ����ʱ���� ");
			}
			else if( *pData == 1 )
			{
				OverSpeed.Enable = TRUE;
				Uart.puts("ʹ�ܳ��ٱ��� ");
			}
			else if( *pData == 2 ) 	
			{
				LimitTime.EnableCheckFatigue = TRUE;
				Uart.puts("ʹ��ƣ�ͼ�ʻ���� ");
			}
			else if( *pData == 3 ) 	
			{
				LimitTime.EnableCheckParking = TRUE;
				Uart.puts("ʹ�ܳ�ʱͣ������ ");
			}
		break;


		default: break;
	}
	
}

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�
   ����������
   ��ڲ�������
   ���ڲ�������
   ȫ�ֱ���: ��
   ����ʱ�䣺
   �޸�ʱ�䣺
*/	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������	
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


	if ((GPSValidFlag == GPS_VALID_FLAG)&&(uiGPS_Speed>400))	 // ��λ��GPS�ٶȴ���40KM/H
	{	 
		Rtc.GetRTC_BCD((pRTC_STR)Tp.Buffer);

		// �꣬�£��� ��һ����ͬ������Ϊ���¸�������
		if ((Tp.Buffer[0] != SpeedStatusJudge.LastTime[0]) || (Tp.Buffer[1] != SpeedStatusJudge.LastTime[1])||
			(Tp.Buffer[2] != SpeedStatusJudge.LastTime[2]))
		{
			
			SpeedStatusJudge.LastTime[0] = Tp.Buffer[0];	 SpeedStatusJudge.LastTime[1] = Tp.Buffer[1];
			SpeedStatusJudge.LastTime[2] = Tp.Buffer[2];
			SpeedStatusJudge.Flag = 0;
			DFE.Write(FM_SPEED_STAUTS_JUDGE,LEN_SPEED_STATUS_JUDGE,(uchar *)&SpeedStatusJudge);
			
			#if (DEBUG_SPEED_INFO_OUPTPUT == 1)
			Uart.puts("��һ�������죬�����ж�");
			#endif

		}

		if(SpeedStatusJudge.Flag==0) // δû���ж�
		{	
			// 5�����ڣ�ÿ���GPS�ٶ����¼���ٶȶԱȣ������ʱ���ڴ���-/+11%���ͼ���5����ʱʱ����Ϊ��ʼʱ��
			// 5�����Ǹ�����Ϊ����ʱ��
		
	  		FSpeed = Pulse.GetSecSpeed(Tp.Buffer);	

			
			tl = 	uiGPS_Speed - FSpeed;

			if (tl<0) tl = -tl;
			tl *= 100;			  // �Ŵ�100�� 

			//Uart.puts("uiGPS_Speed Test Data:"); Uart.putb(1,2,(uchar *)&uiGPS_Speed); Uart.puts("\r\n");
			//Uart.puts("FSpeed Test Data:"); Uart.putb(1,2,(uchar *)&FSpeed); Uart.puts("\r\n");
			//Uart.puts("tl Test Data:"); Uart.putb(1,4,(uchar *)&tl); Uart.puts("\r\n");

			if ((tl / uiGPS_Speed) >= 11)	 // ������11%ʱ, ����Ŵ�100���������������11�Ϳ���
			{
				SpeednormalSecond = 0;

				if (SpeedUnnormalSecond==0)
				{
					Rtc.GetRTC_BCD((pRTC_STR)SpeedStatusLog.StartTime);	
					//#if (DEBUG_SPEED_INFO_OUPTPUT == 1)  
					Uart.puts("�ٶ��쳣��ʼ");
					//#endif
				}

				//Uart.puts("�ٶ��쳣����");

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
				else if (SpeedUnnormalSecond == 300)		 // ����5����	  300
				{
				 	// �ٶ��쳣
					/*SpeedStatusJudge.Flag = 1;
					SpeedStatusJudge.Status = SPEED_STAUTS_UNNORMAL;

					Second = Rtc.BDC_DateTimeToSecond((pRTC_STR)SpeedStatusLog.StartTime);
					Second += 300;
					Rtc.SecondToDateTime_BCD(Second,(pRTC_STR)SpeedStatusLog.EndTime);*/
					KeyDisp.SendDataToLCM_YH(eLCM_U_CENTER_TMP_MSG,(16*2+1),(uchar*)"\x47�ٶ��쳣");
					Save = SPEED_STATUS_UNNORMAL;
					AlertSecond = 0;
					Aert = TRUE;
					//#if (DEBUG_SPEED_INFO_OUPTPUT == 1)
					Uart.puts("�ٶ��쳣");
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
						Uart.puts("�ٶ�����������ʼ");
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

						KeyDisp.SendDataToLCM_YH(eLCM_U_CENTER_TMP_MSG,(4*2+1),(uchar*)"\x47�ٶ�����");	 // 

						Uart.puts("�ٶ�����");
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
				//Rtc.SecondToDateTime_BCD(Second,(pRTC_STR)SpeedStatusLog.EndTime);	  // ��֣��������SpeedStatusLog ��3��������ٶȲ���
				Rtc.SecondToDateTime_BCD(Second,(pRTC_STR)Tp.Buffer);
				memmove(SpeedStatusLog.EndTime,Tp.Buffer,6);

				//Uart.puts("speed log 11 Test Data:"); Uart.putb(1,LEN_SPEED_STATUS_LOG,	(uchar *)&SpeedStatusLog); Uart.puts("\r\n");
				//Uart.puts("SpeedStatusLog.StartTime Test Data:"); Uart.putb(1,6,(uchar *)&SpeedStatusLog.StartTime);Uart.puts("\r\n");
				//Uart.puts("SpeedStatusLog.EndTime Test Data:"); Uart.putb(1,6,(uchar *)&SpeedStatusLog.EndTime);Uart.puts("\r\n");

				////////////////////////////  �����ٶ�״̬��־
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
				Uart.puts("�洢�ٶ�״̬��־��Flash ");
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
		 	KeyDisp.SendDataToLCM_YH(eLCM_U_CENTER_TMP_MSG,(4*2+1),(uchar*)"\x47�ٶ��쳣");
		 }
		 else	if (SpeedStatusJudge.Status==SPEED_STATUS_NORMAL)
		 {
		 	  KeyDisp.SendDataToLCM_YH(eLCM_U_CENTER_TMP_MSG,(4*2+1),(uchar*)"\x47�ٶ�����");		// ���ϵ��ԣ���ͣ
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

