/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												Фң���ӣ����ݣ��з�����
$									Shawyoo Electronics (GuangZhou) R&D Center
$                      	http://www.shawyoo.com		 Email: autoele@qq.com
$						 QQȺ��8502321/9069471/9069433/9524253		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      �ļ�����
	�ļ���: 	 DriverCard.c
	����������˾��������
	���뻷����
	Ŀ��CPU:  
	���ߣ�    
	����ʱ�䣺
	����༭������UltraEdit 11.00a+  �Ʊ�����Ϊ�����ַ�������ɶ��Ը��á�
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/					  
#include			"..\IIC\IIC0.h"
#include			<string.h>
#include			<stdlib.h>
#include			"..\RTC\RTC.h"
#include			"Speed.h"
#include			"..\KeyDisp\KeyDisp.h"
#include       "..\flash\DFE_25xxx.h"
#include       "..\flash\DFF_25xxx.h"
#include			"..\Common\GB_T19056.h"
#include			"..\UART\UART0.H"
#include			"..\GSM_GPRS\GSM_GPRS.H"
#include			"..\GSM_GPRS\JT_T808.H"
#include			"..\GPS\GPS.h"
#include			"..\Common\FLASH_ADDR.H"

// ��������ַ�ĸ�5 λ�̶�Ϊ10100 ��������2 λA1 A0 Ϊ�����ĵ�ַλ
// ���� 500382197911253837
// ���� 500382197611243839

// IIC��USB����һ������ӿڣ�ͨ��CD4052�л�
//#define			IIC_ENABLE			{ IODIR0 |= (1<<18); IOSET0 = (1<<18);}
//#define			USB_ENABLE			{ IODIR0 |= (1<<18); IOCLR0 = (1<<18);}

#define			CHECK_DRIVER_ID_VALID			1

#define        LEN_IC_CARD_INFO              256

#define    	   DRIVER_LOG_IN                  1
#define        DRIVER_LOG_OUT                 2
#define        CHANGE_DRIVER                  3
#define        SAME_DRIVER                    4
#define        NO_ACTION                      0xff

#define		   LEN_JT_DRIVER_INFO				 100

#define			USE_IIC_IC_CARD_MODULE				1


#define        IC_CARD_SIZE                    128
#define        NOACTION                      0xff

#define			DRIVER_IN							0x1
#define			DRIVER_OUT							0x2



extern uchar IC_CardAuthFailure;	
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�CheckNewDriver
   ��������������Ƿ�����˾�������֤(�Ƿ���ǰһ����֤��ͬһ˾��)
   ��ڲ������ޣ�ʹ�����ڽṹ����CurrentDriverInfoTime
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ ///////////////////////////////////////////////////////////////////////////////////////
uchar			SpeedCls::CheckNewDriver(void)
{
	#if(USE_IIC_IC_CARD_MODULE==1)
	uchar		s,NewCard, Ret;
	uint32	i,Addr,tl;
	union
	{
		uchar		Buffer[32];
		uint16	B16[16];
		uint32	B32[8];
	}Tp;
	//DRIVER_INFO_TIME		DriverInfoTime[2];
	
	
	NewCard = TRUE;		// ����Ϊ�¿�
	for(i=0;i<2;i++)
	{
		//Uart.puts("i Test Data:"); Uart.putb(1,4,(uchar *)&i); Uart.puts("\r\n");
		//Uart.puts("RecentDriverInfoTime[i] Test Data:"); Uart.putb(1, LEN_DRIVER_INFO_TIME,(uchar *)&RecentDriverInfoTime[i]); Uart.puts("\r\n");

		s = MyStrCmpNB( 18, (char*)&CurrentDriverInfoTime.ID,(char*)&RecentDriverInfoTime[i].ID);
		if(s==0)			// �ҵ���ͬID�Ŀ�����ǰ����Ŀ�Ϊ�ɿ�
		{
			Uart.puts("�ҵ���ͬ��ID��");
			NewCard = 0;

			if(( RecentDriverInfoTime[i].TravelTimeSec != 0)&&	// ��ʻʱ���0��˵����Ϣʱ�䲻���������²忨ʱ��ͼ�ʻʱ��
				((RecentDriverInfoTime[i].OffTime+ RecentDriverInfoTime[i].RestTimeSec)<LimitTime.RestTimeSec))// ����λʱ�����֮ǰ����ϢʱС����С��Ϣʱ��
			{
				RecentDriverInfoTime[i].RestTimeSec += RecentDriverInfoTime[i].OffTime;
				RecentDriverInfoTime[i].OffTime = 0;

				KeyDisp.SendDataToLCM_YH(eLCM_U_CENTER_TMP_MSG,(22*2+1),(uchar*)"\x47��ע�⣺��Ϣʱ�䲻�㣬�ظ��忨����Ϊ������ʻ");	
				memmove( (uchar*)&CurrentDriverInfoTime, (char*)&RecentDriverInfoTime[i], LEN_DRIVER_INFO_TIME );

				Ret = DRIVER_IN;
			}
			else		// ���²忨ʱ�䣬��ʻʱ��
			{				
				CurrentDriverInfoTime.Number = RecentDriverInfoTime[i].Number;		// ʹ��ԭ���ı��
				Addr = LEN_DRIVER_INFO_TIME;
				Addr *= i;
				Addr += FM_RECENT_DRIVER_INFO_TIME;
				DFE.Write ( Addr, LEN_DRIVER_INFO_TIME, (uchar*)&CurrentDriverInfoTime );

				Ret = DRIVER_IN;
			}
			///////////////////////////// �޸�
			DFE.Write ( FM_CURRENT_DRIVER_INFO_TIME, LEN_DRIVER_INFO_TIME, (uchar*)&CurrentDriverInfoTime );
			break;
		}
	}

	if(NewCard==TRUE)		// δ�ҵ���ͬID�Ŀ�����ǰ����Ŀ�Ϊ�¿�
	{
		Uart.puts("�µ�ID��");
		DFE.Read ( FM_DRIVER_SERIAL_NUMBER,4,Tp.Buffer);		// �Ա�ţ�5λ��
		if( Tp.B32[0]<DRIVER_NUMBER_MIN)	Tp.B32[0] = DRIVER_NUMBER_MIN;
		Tp.B32[0]++;
		DFE.Write ( FM_DRIVER_SERIAL_NUMBER,4,Tp.Buffer);		// �Ա�ż�1��洢
		NOP(); NOP();
		DFE.Write(FM_DRIVER_NUMBER, 4, Tp.Buffer);
		NOP(); NOP();
		CurrentDriverInfoTime.Number = Tp.B16[0];
		DFE.Write ( FM_CURRENT_DRIVER_INFO_TIME, LEN_DRIVER_INFO_TIME, (uchar*)&CurrentDriverInfoTime );
		
		Ret = DRIVER_IN;

		for(i=0;i<2;i++)
		{
			if( RecentDriverInfoTime[i].Number <DRIVER_NUMBER_MIN+1)		// �洢��δʹ�õĿռ�
			{
				Addr = LEN_DRIVER_INFO_TIME;
				Addr *= i;
				Addr += FM_RECENT_DRIVER_INFO_TIME;
				DFE.Write ( Addr, LEN_DRIVER_INFO_TIME, (uchar*)&CurrentDriverInfoTime );
				RecentDriverInfoTime[i] = CurrentDriverInfoTime;
				NewCard = 0;
				break;
			}
		}
		if( NewCard==TRUE)		// δ�洢�����޿ռ䣬�踲����ɵĿ������忨ʱ�������Ϊ��ɵģ�
		{
			tl = RecentDriverInfoTime[0].StartTimeSec;
			for(i=1;i<2;i++)	// �����ҳ�����忨��ʱ��
			{
				if(tl>RecentDriverInfoTime[i].StartTimeSec)	tl = RecentDriverInfoTime[i].StartTimeSec;
			}
			for(i=0;i<2;i++)
			{
				if(tl==RecentDriverInfoTime[i].StartTimeSec)	break;		// �ҵ�������
			}
			Addr = LEN_DRIVER_INFO_TIME;
			Addr *= i;
			Addr += FM_RECENT_DRIVER_INFO_TIME;
			DFE.Write ( Addr, LEN_DRIVER_INFO_TIME, (uchar*)&CurrentDriverInfoTime );
			RecentDriverInfoTime[i] = CurrentDriverInfoTime;

		}		
	}
	#endif
	return Ret;
}
//////////////////////////////////////////////// 













static uchar 	Connect_Specify_Center(uchar Action, uchar Center)
{
	uchar i, s, TcpIndex = 0;

	if (Center==CONNECT_MAIN_CENTER)	   	TcpIndex =  eFLAG_TCP;
	else if (Center==CONNECT_AUTH_CENTER)  TcpIndex =  eFLAG_TCP2;

	if (Action==CHECK_CENTER_ONLINE)
	{
		if (sIP.Online[TcpIndex]== ENABLE_FLAG) {	return OK; }
		else  { goto WAIT_FOR_SPECIFY_CENTER;	 }
	}

	if (Center==CONNECT_MAIN_CENTER)
	{
		JT808_Ctr_IP_DNS.Ctr = 1;
	 	s = 0;
	 	DFE.Write(EE_ENABLE_TCP2,1,(uchar*)&s);
		NOP(); 	NOP(); 	NOP(); 
		DFE.Write(EE_JT_NET_CTR ,1,(uchar*)&JT808_Ctr_IP_DNS.Ctr);
		NOP(); NOP(); NOP(); 
		JT808_Ctr_IP_DNS.Net_Time = 0;
		DFE.Write(EE_JT_NET_CTR_NET_TIME,2,(uchar*)&JT808_Ctr_IP_DNS.Net_Time);
		NOP(); NOP(); NOP();  
		
			
	}
	else if (Center==CONNECT_AUTH_CENTER)
	{
		SetBufferValue((uchar *)&JT808_Ctr_IP_DNS, sizeof(JT808_Ctr_IP_DNS), 0);
		SetBufferValue(&JT808_Ctr_IP_DNS.C_ANP[0], LEN_IPorDNS, 0);
		memmove(&JT808_Ctr_IP_DNS.C_ANP[0], &JT808_IP_DNS.M_APN[0], LEN_IPorDNS);
		DFE.Read(EE_JT_IC_AUTH_M_IP_DNS, LEN_IPorDNS, &JT808_Ctr_IP_DNS.C_IPorDNS[0]);
		NOP();
		DFE.Write(EE_JT_NET_CTR_IP_DNS, LEN_IPorDNS,&JT808_Ctr_IP_DNS.C_IPorDNS[0]);
		NOP();NOP();NOP(); 
		DFE.Read(EE_JT_IC_AUTH_TCP_PORT, 4, (uchar *)&JT808_Ctr_IP_DNS.C_TCP_Port);
		NOP();
		DFE.Write(EE_JT_NET_CTR_TCP_PORT, 2, (uchar *)&JT808_Ctr_IP_DNS.C_TCP_Port);
		NOP();NOP();NOP(); 	
		DFE.Read(EE_JT_IC_AUTH_UDP_PORT, 4, (uchar *)&JT808_Ctr_IP_DNS.C_UDP_Port);
		NOP();
		DFE.Write(EE_JT_NET_CTR_UDP_PORT, 2, (uchar *)&JT808_Ctr_IP_DNS.C_UDP_Port);
		NOP();NOP();NOP(); 
		// ������ʱ����д������
	   JT808_Ctr_IP_DNS.Net_Time = 0; // 
		JT808_Ctr_IP_DNS.Ctr = 1; // 
	   DFE.Write(EE_JT_NET_CTR, LEN_JT_CTR_IP_DNS,(uchar *)&JT808_Ctr_IP_DNS);
		NOP(); NOP(); NOP();
		JT808_Ctr_IP_DNS.Net_Time = 2; 		// 2����
		JT808_Ctr_IP_DNS.Ctr = 0; 				// ��ʱ������TCP2	
		gCnt.Timer[eTMR_TMP_SERVER] = 1;
	}

	 sIP.EnableFlag[TcpIndex] = ENABLE_FLAG;					//JT808

	 DisConectSomeSocket = 1;				// �Ͽ���������
	 os_dly_wait(100);					 	// ��ʱΪ����GSM���������κ�

	if (Action!=CONNECT_CENTER_WAIT) return OK;

WAIT_FOR_SPECIFY_CENTER:

	i = 0;
	while (sIP.Online[TcpIndex]!= ENABLE_FLAG)	   // �ȴ�����
	{
		
		i++;	
		if (i > 300)	return ERROR;			 // 30�볬ʱ

		os_dly_wait(10);
	}
	return OK;
}







///////////////////////////////////////////////

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ� CheckDriverCard
   ���������� ����Ƿ���˾����(��ʻԱ��ݿ��ͺ�24C256)
   ��ڲ�����
   ���ڲ����� 
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
uchar			SpeedCls::CheckDriverCard(void)
{
	static uint32 CheckCnt = 0;
	uchar       RetS, Len, i, s;
	//OS_RESULT 	result;
	//uchar		UserName[32+1];
	uchar	tBuffer[16];

	union 
	{
		
		uchar    ICCardAllData[IC_CARD_SIZE];
		struct
		{
		  uchar     CardTypeInfo[5];				//����������	 �������Ϊ�� ��GPSBD��, Ϊ��ͨ��������Ϊ442��
		  uchar     ProductResv[11];				//оƬ���̱���
		  uchar		DriverName[16];				//��ʻԱ����
		  uchar		DriverCardID[18];				//��ʻ֤����
		  uchar		DriverCarVaildYear;        //��ʻ֤��Ч��-��	  BCD
		  uchar		DriverCarVaildMonth;       //��ʻ֤��Ч��-��	  BCD
		  uchar		DriverCarVaildDate;        //��ʻ֤��Ч��-��	  BCD
		  uchar     CertificateID[18];  			//��ҵ�ʸ�֤��
		  uchar     StandardResv[56];				//��׼��չԤ��
		  uchar     CheckSum;						//���У���
	   }ICCardData;
	}Tmp;
	uchar		CheckSum;
	pDRIVER_INFO_TIME pDriverInfo;
	NOP();	NOP();	
	//IIC_ENABLE;					// ѡͨIICͨ��
	RetS = 0;			   // �ȼ���û�м�ʻԱ�����֤��
	//os_dly_wait(1);			// ��ʱһ������		
	//UserEvtFlag.USB_Enum = 0;
	//RetS=(I2C0.ReadEEPROM(CARD_DEVICE_ADDR,51,8,Tmp.ICCardAllData));

	//if (((DriverCardFlag==0))&&(StopFlag==0))	// ���뿨��ûͣ����û�м�ʻ����־

	if((GET_IC_CARD_STATUS)==0)		// IC���Ѳ��� �����ڳ���������  	  // �����޸ĵĻ���CheckIC_Card��ҲҪ�޸�
	//if((GET_IC_CARD_STATUS)!=0)		// IC���Ѳ���	(���ڳ��տ���)
	{
		if ((StopFlag==0)&&(CheckCnt++ % 300) == 0)
		{
			if (DriverCardFlag==0)
			{
				KeyDisp.SendDataToLCM_YH(eLCM_U_CENTER_TMP_MSG,(8*2+1),(uchar*)"\x47����ͣ�����ٲ忨");
			}
			else
			{
				//KeyDisp.SendDataToLCM_YH(eLCM_U_CENTER_TMP_MSG,(9*2+1),(uchar*)"\x47������ʻ���ο���Ч");
			}
		}
	}
	else
	{
		if (StopFlag == 1)
		{
			CheckCnt = 0;
		}

	}

	if (StopFlag == 0) return 0;				  //   ��ͣ�����������������

	I2C0.CheckIC_Card();				 //����Ƿ��п�

	//
	//if(RetS==ERROR)					 //��IC��Ȩ�ޣ��ɹ�����IC��
	if(IC_CardType==0)
	{		 
	 	//Uart.putb(1, 8,(uchar *)Tmp.ICCardAllData); // ("Driver Card Test1!\r\n\0");
		SetCarFlat = 0;
		if((DriverCardFlag==1)&&(IC_Card_Insert==TRUE))				//�Ƿ��ʻ��	 &&(StopFlag==1)
		{
		    DriverCardFlag = 0; 
	 	    DFE.Write(FM_ICCardInOrOut_FLAG,1,&DriverCardFlag);
			BEEP(9,0,4,0,5);
			NOP();
			RecodeInOut(DRIVER_OUT);				
			IC_Card_Insert = FALSE;	
			CheckCnt = 0;

			// 
			for(i=0;i<2;i++)
			{
				//pDriverInfo = (pDRIVER_INFO_TIME)&Tmp;
				//DFE.Read(FM_RECENT_DRIVER_INFO_TIME+i*LEN_DRIVER_INFO_TIME, LEN_DRIVER_INFO_TIME, (uchar*)pDriverInfo );
				//s = MyStrCmpNB( 18, (char*)&CurrentDriverInfoTime.ID,(char*)&pDriverInfo->ID);
				//if(s==0)			// �ҵ���ͬID�Ŀ�����ǰ����Ŀ�Ϊ�ɿ�
				if (RecentDriverInfoTime[i].Number==CurrentDriverInfoTime.Number)
				{
					Uart.puts("���浱ǰ��ʻԱ����Ϣ");
					CurrentDriverInfoTime.OffTime = 0;
					RecentDriverInfoTime[i] = CurrentDriverInfoTime; 
					DFE.Write (FM_RECENT_DRIVER_INFO_TIME+LEN_DRIVER_INFO_TIME*i, LEN_DRIVER_INFO_TIME, (uchar*)&CurrentDriverInfoTime );
					NOP(); NOP();
					//Uart.puts("i Test Data:"); Uart.putb(1,4,(uchar *)&i); Uart.puts("\r\n");
					//Uart.puts("save RecentDriverInfoTime[i] Test Data:"); Uart.putb(1, LEN_DRIVER_INFO_TIME,(uchar *)&RecentDriverInfoTime[i]); Uart.puts("\r\n");
					break;
				}		
			}
			SetBufferValue((uchar*)&CurrentDriverInfoTime, LEN_DRIVER_INFO_TIME, 0);
			DFE.Write (FM_CURRENT_DRIVER_INFO_TIME,LEN_DRIVER_INFO_TIME,(uchar*)&CurrentDriverInfoTime); // ��յ�ǰ��ʻԱ��Ϣ

			GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_DRIVER_INFO_REPORT,0,NULL);
	   //
															                 
	   }
//	   return FALSE;																					               //�������ɹ�����IC��
	}
	else
	{	 
		if (DriverCardFlag) 	  //�������
		{
			IC_Card_Insert = TRUE;
			return 0;
		}

		SetBufferValue(Tmp.ICCardAllData, 128,0);
		if(IC_CardType==IC_AT24C_CARD)
		{
		   I2C0.ReadEEPROM(CARD_DEVICE_ADDR,0,128,Tmp.ICCardAllData);
		}
		else
		{
		  I2C0.PinFor4442(1);
		  os_dly_wait(1);
		  I2C0.Reset4442Card();
		  I2C0.Read4442Card(0,128,Tmp.ICCardAllData);
		  I2C0.PinFor4442(0);
		  Uart.putb(1, 256,Tmp.ICCardAllData);
		}


		if ((Tmp.ICCardData.DriverCardID[0]<'0') || (Tmp.ICCardData.DriverCardID[0]>'9') || 
		   (Tmp.ICCardData.DriverCardID[1]<'0') || (Tmp.ICCardData.DriverCardID[1]>'9'))		//  ��ʻ֤���Ƿ����
		{
			return 0;
		} 

		RetS = Tmp.ICCardAllData[0];
		CheckSum = CalXorSum(127, Tmp.ICCardAllData);

		//Uart.puts("IC Test Data:"); Uart.putb(1,128, Tmp.ICCardAllData); Uart.puts("\r\n");
		//Uart.puts("Check Test Data:");	Uart.putb(1,1, &CheckSum); Uart.puts("\r\n");
		

		if (CheckSum == Tmp.ICCardData.CheckSum)  // У�����Ƿ���ȷ
		{	
				if(DriverCardFlag==0)																						     //�Ƿ��²��뿨
				{
					SetBufferValue((uchar *)&CurrentDriverInfoTime,LEN_DRIVER_INFO_TIME,0);
					Len = 18;
					memmove(CurrentDriverInfo.ID, Tmp.ICCardData.DriverCardID, Len);
					memmove(CurrentDriverInfoTime.ID, Tmp.ICCardData.DriverCardID, Len);
					DFE.Write(FM_DRIVER_CODE, Len, CurrentDriverInfo.ID);  // 18λ��ҵ�ʸ�֤�� ������ǰ�Ĵ洢
					NOP(); NOP();
					DFE.Write(EE_JT_DRIVER_CARD_ID, Len, CurrentDriverInfo.ID);

					// added 2015.1.19
					memmove((uchar*)&CurrentDriverInfoTime.StartLocatoin,(uchar*)&ValidLocationInfo,LEN_LOCATION_INFO);
					
					
					memmove(CurrentDriverInfo.Name, Tmp.ICCardData.DriverName, Len);
					CurrentDriverInfo.Name[15] = 0;	  // ǿ���������һ�ֽ���0�� Ϊ�����strlen
					Len = strlen((char *)CurrentDriverInfo.Name);
					DFE.Write(EE_JT_DRIVER_NAME_LEN, 1, &Len);
					NOP();
					DFE.Write(EE_JT_DRIVER_NAME, Len, CurrentDriverInfo.Name);

					IC_CardAuthFailure = 0; // ��֤�ɹ�

					Len = 0;
					DFE.Write(EE_JT_CERTIFICATE_NAME_LEN, 1, (uchar *)&Len);

					SetBufferValue(tBuffer,16,0);
					tBuffer[0] = 0x20;
					memmove(&tBuffer[1], &Tmp.ICCardData.DriverCarVaildYear,3);
					DFE.Write(EE_JT_DRIVER_CARD_VALID_DATE, 4, &tBuffer[0]);

					RetS = CheckNewDriver();																  //�ж��Ƿ��¿�������¼��ؼ�ʻ����
					if(RetS==NOACTION)
					{  
					   //Uart.puts("Driver Card Error!\r\n\0");
						return ERROR;
				   }
					else
					{	
					    IC_Card_Insert = TRUE;
						 GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_DRIVER_INFO_REPORT,0,NULL); 
						//GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_I2C_DRIVER_INFO_REPORT,0,NULL);		//I2C�������ļ�ʻԱ��Ϣ�ɼ��ϱ�
						BEEP(9,0,2,0,10);
					  //Uart.puts("Driver Card 0K!\r\n\0");
						DriverCardFlag = 1; 
				 	   DFE.Write(FM_ICCardInOrOut_FLAG,1,&DriverCardFlag);
						RecodeInOut(RetS);												      //��¼��¼��¼		
						
					}
				}
			}

						
	    
	}	
	NOP();	NOP();
	//USB_ENABLE;
	NOP();	NOP();
	return RetS;
} // End of <CheckDriverCard> function

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ� RecodeInOut
   ���������� ��¼��¼�˳���¼
   ��ڲ�����
   ���ڲ����� 
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void			SpeedCls::RecodeInOut(uchar InOrOut)
{	
	uint32 tl;
	uint32 Addr;
	DRIVER_IN_OUT_STATUS Status;
	uchar tBuffer[256];


	Rtc.GetRTC_BCD((RTC_STR *)tBuffer);
	memmove(Status.Time,tBuffer,6);
	memmove(Status.ID,CurrentDriverInfoTime.ID,18);
	Status.EvenType = InOrOut;

	DFE.Read(FM_DRIVER_OUT_IN_CNT,4,(uchar*)&tl);
	Addr = tl%10;
	Addr *= LEN_DRIVER_IN_OUT_STATUS;
	Addr += FM_DRIVER_OUT_IN;		  	
	DFE.Write( Addr, LEN_DRIVER_IN_OUT_STATUS,(uchar *)&Status);
	tl++;
	DFE.Write(FM_DRIVER_OUT_IN_CNT,4,(uchar*)&tl);	 	// ��ʻԱ����ǳ���������1��洢������	
	//
	Uart.puts("���±����ʻ����ǳ���¼������ "); 
	if ((tl)&&(tl%10)==0)
	{
		// ���������һҳ��Flash�Ŀռ䣬��ŵ�Flash��
		if( (FlashPageCnt.DriverLogInOutSave%FF_PAGE_PER_SECTOR)==0)
		{
			Addr = FF_DRIVER_IN_OUT_START_PAGE;
			Addr += FlashPageCnt.DriverLogInOutSave%FF_DRIVER_IN_OUT_TOTAL_PAGE;
			
			DFF.SectorEraseWaitOK(Addr/FF_PAGE_PER_SECTOR,10);
		}				
		///////////////////////////////////////////�洢��Flash  ����1ҳflash����10����ʻԱ����ǳ���¼
		DFE.Read(FM_DRIVER_OUT_IN,LEN_DRIVER_IN_OUT_STATUS*10,tBuffer); // 

		Addr = FF_DRIVER_IN_OUT_START_PAGE;
		Addr += FlashPageCnt.DriverLogInOutSave%FF_DRIVER_IN_OUT_TOTAL_PAGE;
		DFF.PageProgramWaitOK(Addr,tBuffer,3);
		FlashPageCnt.DriverLogInOutSave++;

		DFE.Write(FM_FLASH_PAGE_CNT,LEN_FLASH_PAGE_CNT,(uchar*)&FlashPageCnt);

  		Uart.puts("�����ʻ����ǳ���¼��Flash"); 
		
	}
					
}





/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ� SendDataToRFID
   ���������� �������ݵ�RFIDģ��
   ��ڲ�����
   ���ڲ����� 
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������

#define		RFID_I2C_ADDR		0x50

uchar		SpeedCls::SendDataToRFID(uchar Cmd, uchar *pData, uint16 Len)
{
	uchar tBuffer[128];
	uchar  i;
	uint32	CheckSum;
	JT808_DEVICE_PROTOCOL_HEAD DeviceCom;

	Uart.puts(" дIC������S1 ");

	if (Len > 128)	return ERROR;
	
	
	DeviceCom.VersionNo = 0;//0x0101;
	DeviceCom.ManufacturerNo = 0;//0x1201;    // δ֪������,��ʱ��һ��ֵ	
	DeviceCom.DeviceTypeNo = 0x0B;     // JT808 2013
	DeviceCom.Cmd	= Cmd;

	SetBufferValue(tBuffer, 128, 0);
	tBuffer[0] = 0x7e;			// ��ʶ�ֽ�

	memmove(&tBuffer[2], (uchar *)&DeviceCom.VersionNo, 6);	   // �Ӱ汾�ſ�ʼ ��6���ֽ�
	if ((Len != 0) && (pData != NULL) )
		memmove(&tBuffer[8], pData, Len);

	CheckSum = 0;
	for (i = 0; i < Len + 6; i++)
	{
		CheckSum += tBuffer[i+2];  
	}


	tBuffer[1] = DeviceCom.CheckSum = CheckSum & 0xff;	   // ���У����

	i = EscapeCharEncode(Len+7, &tBuffer[1]);  // ��У����

	tBuffer[i+1] = 0x7e;		  // ��ʶ�ֽ�

	i += 2;     // ͷβ������ʶ�ֽ�

	if (I2C_Busy_Now_Flag == 1) { os_dly_wait(10); }
	I2C_Busy_Now_Flag = 1;

		
	I2C0.Write(RFID_I2C_ADDR, 0, i, tBuffer);
	os_dly_wait(1);

	I2C_Busy_Now_Flag = 0;

	Uart.puts(" дIC������S2 ");
	
   return OK;

	 
	
}	


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�GetDataFromRFID
   ���������� ����RFIDģ�������
   ��ڲ�����
   ���ڲ����� 
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
uchar		SpeedCls::GetDataFromRFID(uchar Cmd, uchar *pData, uint16 Len)
{
	uchar tBuffer[200];
	uchar  i, j, Length,*pS, *pE;
	uint32	CheckSum;

	if(Cmd != 0xff)
	{	
		Uart.puts(" ��ȡIC������S1 ");
		if (I2C_Busy_Now_Flag == 1) { os_dly_wait(10); }
		I2C_Busy_Now_Flag = 1;
	
		SetBufferValue(tBuffer, 128, 0);
		I2C0.Read(RFID_I2C_ADDR, 0, 128, tBuffer);	
		os_dly_wait(20);
	
		I2C_Busy_Now_Flag = 0;
	}
	else
	{
		memmove( tBuffer,pData,Len);
	}


	i =0x7E;
	pS = SearchArray(128, tBuffer,1, &i);

	if (pS == NULL)	return 0xff;
	Uart.puts(" ��ȡIC������S2 ");

	Length = pS - &tBuffer[0];
	Length = 128 - Length - 1 - 7;				   // ����ʣ�����ݵĳ��ȣ�JT808���������7E֮��������7���ֽ�,  �ټ�ȥ��7e"һ���ֽ�
	pE = SearchArray(Length , pS+8, 1, &i);	   // ��JT808����Э�飬pS+8ָ���û����ݻ� 7E

	//Uart.puts("CCC1"); os_dly_wait(1);

	if (pE == NULL)		return 0xff;			 // û�ҵ��ڶ���7E,������Ч

	Uart.puts(" ��ȡIC������S3 ");
    
	Length = pE - pS - 1;

	Length = EscapeCharDecode(Length, pS+1);       // ת�廹ԭ 

	// У������
	CheckSum = 0;
	j = Length;
	for (i = 1; i < j; i++)		 //  
	{
		CheckSum += *(pS+i+1);  
	}

	if ((CheckSum & 0xff) != *(pS+1))  
	{
		 return 0xff;
	}
	Uart.puts(" ��ȡIC������S4 ");
	if (pData != NULL)
	{
		j =  Length + 2;  // ����RFID������

		memmove(pData, pS, j); 
		return j;
			
	}

   Uart.puts(" ��ȡIC������S5 ");
	return 0;

}





uchar			SpeedCls::U2_SendDataToRFID(uchar Cmd, uchar *pData, uint16 Len)
{
	uchar tBuffer[128];
	uchar  i;
	uint32	CheckSum;
	JT808_DEVICE_PROTOCOL_HEAD DeviceCom;

	Uart.puts(" U2дIC������S1 ");

	if (Len > 128)	return ERROR;
	
	
	DeviceCom.VersionNo = 0;//0x0101;
	DeviceCom.ManufacturerNo = 0;//0x1201;    // δ֪������,��ʱ��һ��ֵ	
	DeviceCom.DeviceTypeNo = 0x0B;     // JT808 2013
	DeviceCom.Cmd	= Cmd;

	SetBufferValue(tBuffer, 128, 0);
	tBuffer[0] = 0x7e;			// ��ʶ�ֽ�

	memmove(&tBuffer[2], (uchar *)&DeviceCom.VersionNo, 6);	   // �Ӱ汾�ſ�ʼ ��6���ֽ�
	if ((Len != 0) && (pData != NULL) )
		memmove(&tBuffer[8], pData, Len);

	CheckSum = 0;
	for (i = 0; i < Len + 6; i++)
	{
		CheckSum += tBuffer[i+2];  
	}


	tBuffer[1] = DeviceCom.CheckSum = CheckSum & 0xff;	   // ���У����

	i = EscapeCharEncode(Len+7, &tBuffer[1]);  // ��У����

	tBuffer[i+1] = 0x7e;		  // ��ʶ�ֽ�

	i += 2;     // ͷβ������ʶ�ֽ�

	Uart2.putb(1,i,tBuffer);
	Uart2.SendTask();

	Uart.puts(" U2дIC������S2 ");
	
   return OK;
}







//7E 89 00 00 19 00 01 38 86 08 88 04 02 0B 00 01 5B F7 DD E8 FF 1D 2D 48 00 00 00 00 00 00 00 00 00 00 00 00 B0 AF A2 7E 
uchar				WaitIC_CardAuthTime,IC_CardAuthFailure;

void				SpeedCls::U2_ForRFID_Fun(void)
{
	uint16		i,s,Len;
	uchar			Sum,Index,tBuffer[200];
	pJT808_EXT_DEVICE_DATA	pRFID;
	RTC_STR		Rtc1;
	#pragma		pack(1)
	struct	
	{
		uchar		DriverNum[18];				//��ʻ֤����
		uchar		DriverName[20];			//��ʻԱ����
		uint32	DriverCode;					//��ʻԱ����
	
	}DriverInfo;

	static		uchar Get_Driver_Info_OK = FALSE;

	tBuffer[0] = tBuffer[1] = tBuffer[2] = 0;
	Len = Uart2.RFID_ReceiveData(tBuffer);
	if( (Len == 0)||(tBuffer[0]!=0x7e) )		return;

	if(TD.MainTask==tMAIN_IC_CARD_DATA)
	{
		Uart.puts("\r\nRFID Test Data:");
		Uart.putb(1,Len,tBuffer);
		Uart.puts("\r\n");
	}

	////////////////////////////////////////////////
	if(Len>200)
	{
		Uart.puts(" RFID���ݴ���200�ֽ� ");
		Len = 200;
	}

	Len = EscapeCharDecode( Len-1, &tBuffer[1]);

  	if(TD.MainTask==tMAIN_IC_CARD_DATA)
	{
		Uart.puts("\r\nRFID Test Data:");
		Uart.putb(1,Len,tBuffer);
		Uart.puts("\r\n");
	}

	for(Sum=0,i=4;i<Len;i++)
	{
		Sum += tBuffer[i];		
	}
	if( Sum != tBuffer[1] )		Uart.puts(" RFID����У����� ");

	pRFID = (pJT808_EXT_DEVICE_DATA)&tBuffer[0];
	if( (pRFID->Cmd==0x40)&&(pRFID->Data.Buffer[0]==0) )		// ����
	{
		IC_Card_Auth_GPRS = TRUE;
		WaitIC_CardAuthTime = 90;
		IC_Card_Auth_Flag = 0;
		GPRS.InitAlig('A');	

		Rtc.GetRTC_BCD((RTC_STR *)&Rtc1);
		DFE.Write(FM_RUN_TIME_START,6,(uchar*)&Rtc1);
		NOP(); 	NOP(); NOP(); NOP(); NOP(); NOP(); 
		
		s = Connect_Specify_Center(CONNECT_AUTH_WAIT, CONNECT_AUTH_CENTER); // ������֤����
		os_dly_wait(100);
		if(s!=OK)
		{
			Uart.puts("  ����IC����֤���ķ�����ʧ�� ");
			return;
		}
		/////////////////////////////////// �������ݵ�����
		pRFID->Data.Buffer[0] = 0x0b;
		for (i=0; i<2; i++)			  // �����֤����û�лظ����ط�һ��
		{
			Uart.puts("͸����ʻԱ��Ϣ����֤���ģ���� ");			
	   	GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_DATA_TRAN_UPLOAD,(64+1),pRFID->Data.Buffer);
			for (s=0; s<(60*100); s++)		  // 10s��ʱ
			{
				os_dly_wait(1);
				if( (s%100)==0)
				{
					if(WaitIC_CardAuthTime)	WaitIC_CardAuthTime--;
				}	
				if (IC_Card_Auth_Flag==TRUE)	
				{
					WaitIC_CardAuthTime = 0;
					break;		// ��֤�����ѷ���								
				}
			}
			if (IC_Card_Auth_Flag==TRUE)		
			{
				WaitIC_CardAuthTime = 0;
				break;
			}
		} 
	}
	else if( (pRFID->Cmd==0x41) )			// ��֤���	
	{
		if( pRFID->Data.Buffer[0]==0 )		// �ɹ�
		{
			IC_CardAuthFailure = 0;
			Uart.puts("\r\nIC����֤�ɹ� ");
			U2_SendDataToRFID(0x41, NULL, 0);		// �ظ��ɹ�
			WaitIC_CardAuthTime = 0;
			Get_Driver_Info_OK = TRUE;
      	
			DriverCardFlag = TRUE; 
   		DFE.Write(FM_ICCardInOrOut_FLAG,1,&DriverCardFlag);
      	
			Index = JT_DRIVER_NAME_LEN_INDEX;	
			Len =  tBuffer[Index];			   //  ��ʻԱ��������
			//RetS = Len;
			if (Len > 20) s = 20;	  			// ֻ�ܴ�20���ֽ�
			else		  	  s = Len;
			DFE.Write(EE_JT_DRIVER_NAME_LEN, 1+s, &tBuffer[JT_DRIVER_NAME_LEN_INDEX]);	     // ��¼��ʻԱ�������ȼ�����
			NOP(); NOP();NOP();NOP();NOP();NOP();NOP();
			memmove(DriverInfo.DriverName, &tBuffer[JT_DRIVER_NAME_LEN_INDEX], 1+s);
      	
			// add 2013.6.21
			memmove(CurrentDriverInfo.Name, &tBuffer[JT_DRIVER_NAME_LEN_INDEX], 1+s);
			//
			
			Index += (Len + 1);
			DFE.Write(EE_JT_DRIVER_CARD_ID, 20, &tBuffer[Index]);  // 20λ��ҵ�ʸ�֤��
			NOP(); NOP();NOP();NOP();NOP();NOP();NOP();
			DFE.Write(FM_DRIVER_CODE, 18, &tBuffer[Index]);  // 18λ��ҵ�ʸ�֤�� ������ǰ�Ĵ洢
			NOP(); NOP();NOP();NOP();NOP();NOP();NOP();
			memmove(DriverInfo.DriverNum, &tBuffer[Index], 18);
			
			// add 2013.6.21
			memmove(CurrentDriverInfo.ID, &tBuffer[Index], 18);
			//
			
			
			// ��JT808û��ʻԱ���룬��ʱ�Զ��尴���涨��ʻԱ����
			DriverInfo.DriverCode = (DriverInfo.DriverNum[17]-0x30)+(DriverInfo.DriverNum[16]-0x30) * 10+(DriverInfo.DriverNum[15]-0x30) * 100;
			DriverInfo.DriverCode = SwapINT32(DriverInfo.DriverCode); 
			DFE.Write(FM_DRIVER_NUMBER, 4, (uchar *)&DriverInfo.DriverCode); 
			NOP(); NOP();NOP();
			
			// add 2013.6.21
			CurrentDriverInfo.Number = DriverInfo.DriverCode;
			//
			
			Index += 20;
			Len =  tBuffer[Index];
			if (Len > 60) s = 60;                   // ֻ�ܴ�60���ֽ�
			else		  s = Len;
			DFE.Write(EE_JT_CERTIFICATE_NAME_LEN, s+1, &tBuffer[Index]);  // ��¼��֤�������Ƴ��Ⱥ�����
			NOP(); NOP();NOP();NOP();NOP();NOP();NOP();
      	
			Index += (Len + 1);
			DFE.Write(EE_JT_DRIVER_CARD_VALID_DATE, 4, &tBuffer[Index]);  // 20λ��ҵ�ʸ�֤��
			NOP(); NOP();NOP();NOP();NOP();NOP();NOP();
      	
      	Uart.puts(" ͸����ʻԱ��Ϣ����֤���ģ����2 ");
			GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_DATA_TRAN_UPLOAD,(64+1),pRFID->Data.Buffer);
      	      	
			os_dly_wait(500);
			
			IC_Card_Auth_GPRS = 0;
			IC_Card_Insert = TRUE;
			Connect_Specify_Center(CONNECT_CENTER_WAIT, CONNECT_MAIN_CENTER);	   // ���Ӽ������,�޵ȴ�
			os_dly_wait(200);
			Uart.puts(" ͸����ʻԱ��Ϣ��������ģ���� ");
			GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_DRIVER_INFO_REPORT,0,NULL);
			
		}
		else //if( pRFID->Data.Buffer[0]==0 ) 1:ʧ�ܣ����벻�ԣ�2��ʧ�ܣ���Ƭ������3��ʧ�ܡ���Ƭ���γ���4��ʧ�ܣ�����У�����
		{
			IC_CardAuthFailure = pRFID->Data.Buffer[0];
	
			IC_Card_Insert = TRUE;
			Connect_Specify_Center(CONNECT_CENTER_WAIT, CONNECT_MAIN_CENTER);	   // ���Ӽ������,�޵ȴ�
			os_dly_wait(300);
			Uart.puts(" ����ʧ�ܣ�֪ͨ������� ");
			GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_DRIVER_INFO_REPORT,0,NULL);
			
		}

	}	//7E 4E 00 01 01 00 0B 42 7E 
	else if(	(pRFID->Cmd==0x42) )	  			// IC���γ�
	{
			IC_Card_Insert = FALSE;	
			DriverCardFlag = 0; 
	 	   DFE.Write(FM_ICCardInOrOut_FLAG,1,&DriverCardFlag);
			BEEP(9,0,4,0,5);
			NOP();NOP();NOP();
			os_dly_wait(5);
			WaitIC_CardAuthTime = 0;			
			U2_SendDataToRFID(0x42, NULL, 0);	  // �ο�֪ͨ	   	ֱ�ӹص��ˣ�����Ҫ֪ͨRFID
			os_dly_wait(10);
			U2_SendDataToRFID(0x42, NULL, 0);	  // �ο�֪ͨ
			os_dly_wait(10); 
			
			if (Get_Driver_Info_OK == TRUE)
			{
				Uart.puts("�ϴ���ʻԱ��֤����1����� ");
				GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_DRIVER_INFO_REPORT,0,NULL);	  // ����������
				os_dly_wait(500);
	
				IC_Card_Auth_GPRS = TRUE;
				if (ERROR == Connect_Specify_Center(CONNECT_CENTER_WAIT, CONNECT_AUTH_CENTER)) // ������֤���ģ�������ʱ�ȴ����ߣ�
				{
					Uart.puts("Can not Connect Auth Center 2\r\n");
					os_dly_wait(1);
				}
				else
				{
			   	GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_DRIVER_INFO_REPORT,0,NULL);
			  		os_dly_wait(500);
					Uart.puts("�ϴ���ʻԱ��֤���棬��� ");
				}
				IC_Card_Auth_GPRS = 0;
				// ��ռ�ʻԱ��Ϣ
				SetBufferValue(tBuffer, 128, 0);
				DFE.Write(EE_JT_DRIVER_NAME_LEN, LEN_JT_DRIVER_IC_CARD_INFO, tBuffer);
				NOP(); 	NOP(); NOP(); NOP(); NOP(); NOP(); NOP(); 
				
			}
			Get_Driver_Info_OK = FALSE;
			
			Connect_Specify_Center(CONNECT_CENTER_NO_WAIT, CONNECT_MAIN_CENTER);	   // ���Ӽ������,�޵ȴ�	
	}
	else if(	pRFID->Cmd==0x05 )	
	{
		
	}

}







uchar			SpeedCls::RFID_ReaderSeltTest(void)
{
	EXT_5V_POWER_ON;
	os_dly_wait(100);
	U2PinSelDevice = U2_FOR_RFID;
	os_dly_wait(100);
	Uart2.Init( 9600, U2_FOR_RFID);
	os_dly_wait(100);
	U2_SendDataToRFID(0x05, NULL, 0);			// �Լ�
	
	return OK;
}













/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */

