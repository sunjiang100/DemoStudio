/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$									���ݹ�ҵ���ӿƼ����޹�˾
$									
$                       	http://www.guoyee.com		 Email: guoyegps@126.com	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      �ļ�����
�ļ���:   gsm_send.cPP
����������GSM��������
���뻷����
Ŀ��CPU: 
���ߣ�						 
����ʱ�䣺
����༭������UltraEdit 11.00a+  �Ʊ�����Ϊ�����ַ�������ɶ��Ը��á�
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/		
								 
#define			GSM_GPRS_GLOBALS			1
#include			"GSM_GPRS.h"
#include			"JT_T808.h"
#include			"ATCmd.h"
#include			"..\Common\GB_T19056.h"
#include			"..\ADC\ADC.h"
#include			"..\GPS\GPS.h"
#include       ".\UserSys.h"
#include			"..\Common\flash_addr.h"


extern 	uchar		jt_auth_ok_flag;
extern 	uchar	   jt_recv_flag;
extern	uint16	jt_send_cmd_flag;
extern	uchar		jt_pic_send_flag;
extern	uchar		jt_save_pos_flag;
extern	uchar	   jt_dis_reg_flag;

extern	char	   jt_auth_code[LEN_AUTH_CODE];
extern   uchar    DialForListenCnt;
extern	uchar		CallOutSuccess;




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�GSMCls::GSM_Transmit_Task
   ��������: 
   ��ڲ�������
   ���ڲ�������
   ȫ�ֱ���: 
   ����ʱ�䣺2015-10-10 12:29
   �޸�ʱ�䣺	
*/ 

//static   uchar FlagBak;
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
__task		void				TaskGSM_GPRS_Receive(void)	  
{	
	
	NOP();	NOP();
	TickCnt.GG_Rec = 0;		 //UserGlobel.h
	TickCnt.GG_Send = 0;
	GG_Status.InitStatus = 0;
	GSM_TaskBusing = FALSE;
	GG_Status.FrameDataBusing = FALSE;
	DisConectSomeSocket = 0;
	NetConnectCnt = 0;
	GPRS.InitAlig('A');
	
	KeyDisp.SetLED_Status(LED_GPRS,LED_MOBILE_ERROR);

	DFE.Read(EE_SYSTEM_LOG_BASE,LEN_SYSTEM_LOG,(uchar*)&SystemLog);

	os_dly_wait(10);
	if(GSM.Init()!=OK)
   {
	  SelfTestResust |= eST_ER_GSM;
   } 
	NOP();	NOP();	NOP();	NOP();

	jt_send_cmd_flag = 0;
	jt_recv_flag = 0;
	JT_RemoteReadRecodMoreDataCmd = 0;

	ReSend.OverTime = ReSend.Times = 0;	  // Ӧ��ʱ�� 10s
	ReSend.PreOverTime = SendOVT.TCP_ACK_OverTime;  
	
	ReSend.DataType = 0;
	
	#if (WUXI_TEST==1)
	GSM.JT808_SendPeriodFotJT808Check();
	#endif

	//GSM.CalculateSendInterval(0);

	//GSM.JT808_SendPeriodFotJT808Check();
	
	NOP();	NOP();	

	while(1)
	{
		NOP();	NOP();
		TC.GSM = 0;
		//os_dly_wait(1);		//delete By Sun

		if( (JT_Ctr_PowerDown == 1)||(U0_Iap_Flag!=0) )		  //�ػ�
		{
			TC.GPRS_LongTimeNoSend = 0;
			TC.GG_Rec = TC.GG_Send = 0;
			os_dly_wait(100);
			continue;
		}
		/*				  //delete By Sun
		if(JT_ShutMobileCnt)	 	// ��������涨������Χ���󣬲����ϱ��κ���Ϣ
		{
			os_dly_wait(100);


			#if (WUXI_TEST != 1)
			if(JT_ShutMobileCnt==3)
			{
				GG_Status.Flag.Word = GG_FLAG_INIT_DATA;	
				OneFrame[0] = OneFrame[1] = OneFrame[2] = OneFrame[3] = OneFrame[4] = 0;
				Int2Str(sIP.Socket[eFLAG_TCP],(char*)OneFrame);
				GSM.AT_CommandWaitRetStatus(eAT_QICLOSE,OneFrame,NULL);	// �Ͽ�Socket[eFLAG_TCP1]
			}
			TC.GSM = 0;
			TC.GG_Send = 0;
			TC.GG_Rec = 0;
			TC.GPRS_LongTimeNoSend = 0;			
			TickCnt.GG_Send++;
			if(TickCnt.GG_Send%10==0)		Uart.puts("�ն˱��أ��ѹر�GPRSģ�� ");			
			GG_Status.InitStatus = 0;		// if(JT_ShutMobileCnt==0)ʱ������ģ��
			GG_Status.Flag.Bits.Online = 0;
			GG_Status.OnLine = 0;
			GG_Status.Signal = 0;
			
		
			if(JT_ShutMobileCnt>=10)		{	MOBILE_POWER_OFF;	}
		
			
			continue;

			#else

			continue;
			
			#endif
		}
			*/

		if(GSM.PowerOn == TRUE)
		{	
			MOBILE_POWER_ON;
		}
		else
		{	
			MOBILE_POWER_OFF;
			
		}

		CLEAR_RTS;	 		// MCU��Զ����׼��״̬

		if(GG_Status.InitStatus==0) 
		{
			GSM.Init();
		}
					

		if((PhoneCall.RingFlag == TRUE)||(PhoneCall.Busing == TRUE)||(PhoneCall.Dialing == TRUE)||(GG_Status.InitStatus==0) ) 
		{
			TC.GG_Rec = TC.GG_Send = 0;
			os_dly_wait(100);
			continue;
		}
		
		
		GSM.eTaskReceive();
		GSM.eTaskSend();			
		
		
	}
}	// End of <TaskGSM_GPRS> function








/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�
   ��������:  ����ע�����������
   ��ڲ�������
   ���ڲ�������
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺	
*/ 

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void		GSMCls::AutoPeriodSend(void)
{
	
}















/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�
   ��������: 
   ��ڲ�������
   ���ڲ�������
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺	
*/ 

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
uchar		GSMCls::WaitGSM_Free(void)
{
	uint32	i;
	for(i=0;i<(30*10UL);i++)
	{
		os_dly_wait(10);
		if(GSM_Busing==FALSE)	return OK;
		if( (i%(5*10))==0 )		
			Uart.puts("GSMģ������æ\r\n\0");
	}
	GG_Status.InitStatus = 0;
	return ERROR;	
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�
   ��������: 
   ��ڲ�������
   ���ڲ�������
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺	
*/ 

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
uint32		GSMCls::GetGG_FlagBits(void)
{
	uchar		i,t;
	union
	{
		uint32	B32;
		uint16	B16[2];
		uchar		Buffer[4];
	}Tp;
	Tp.B32 = 0;
	Tp.B16[0] = GG_Status.Flag.Word;
	t = 0;
	for(i=0;i<8;i++)
	{
		t >>= 1;
		if(sIP.EnableFlag[i] == ENABLE_FLAG )
			t |= 0x80;
	}
	Tp.Buffer[2] = t;	
	if(NeedRegServer==ENABLE_FLAG)		Tp.Buffer[2] |= 0x80;

	if(GPRS_SendAligFull)					Tp.Buffer[3] |= 0x01;
	if(DTU_Busing)								Tp.Buffer[3] |= 0x02;
	if(RTU_Busing)								Tp.Buffer[3] |= 0x04;
	if(GSM_Busing)								Tp.Buffer[3] |= 0x08;
	if(UseFlowControl == ENABLE_FLAG)	Tp.Buffer[3] |= 0x10;
	//if(DTU_Busing)								Tp.Buffer[3] |= 0x20;
	//if(DTU_Busing)								Tp.Buffer[3] |= 0x40;
	//if(DTU_Busing)								Tp.Buffer[3] |= 0x80;


	return Tp.B32;
}

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�
   ��������: 
   ��ڲ�������
   ���ڲ�������
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺	
*/ 

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
#if (0)
uchar		GPRSCls::IAP_Get_Firmware_File(uint32 FileLen)
{
	uchar  atBuffer[65];
	char   dBuffer[11];
	uchar  s=ERROR, n, m;
	uint32 t;
	uint16 i, block,fAdd;
	char  *pS;
	
	block = FileLen/IAP_Firmware_Bag_LEN;
	if(FileLen%IAP_Firmware_Bag_LEN!=0)
	  block++;
	for(i=0, m=0; i<block; )
	{
	 	SetBufferValue(atBuffer,64,0);
		strcpy((char *)atBuffer,"AT+URDBLOCK=");								 //�ҵ����ص��ļ�,�����ļ�
		strcat((char*)atBuffer,"\"\0");
		strcat((char*)atBuffer, UpdatePara.FileName);
		strcat((char*)atBuffer,"\",\0");

		t = i*IAP_Firmware_Bag_LEN;
		SetBufferValue((uchar *)dBuffer,10,0);
		Int32ToDecChar(t,dBuffer);
	   strcat((char*)atBuffer,dBuffer);
		strcat((char*)atBuffer,",\0");
		
		t = IAP_Firmware_Bag_LEN;
		SetBufferValue((uchar *)dBuffer,10,0);
		Int32ToDecChar(t,dBuffer);
	   strcat((char*)atBuffer,dBuffer);
		strcat((char*)atBuffer,"\r\n\0");
	
		if(TD.GPRS==tGPRS_GETFILE)
		  Uart.puts((char *)atBuffer);	    
		Uart1.ClearReceiveFlag(0);
		Uart1.puts((char *)atBuffer);
	   SetBufferValue(TransparentBuffer, UART1_REC_BUF_LENGTH, 0);
	   Uart1.WaitRecBuffer((30*100),UART1_REC_BUF_LENGTH,TransparentBuffer);	  		// �ȴ�����	
		if(TD.GPRS==tGPRS_GETFILE)
		   Uart.putb(1, 30, (uchar *)&TransparentBuffer[0]);
		pS = NULL;
		pS = MyStrStr((char *)TransparentBuffer,"+URDBLOCK:\0");
		if(pS!=NULL)
		{
			//Uart.putb(1, UART1_REC_BUF_LENGTH, TransparentBuffer);		os_dly_wait(30);
			if(UpdatePara.Frame == IAP_Firmware2)							
		   {
			   n = IAP_Firmware2_Base_Sector_Add;
			   fAdd = IAP_Firmware2_Base_Add;
			}
			else
			{
				n = IAP_Firmware1_Base_Sector_Add;
				fAdd = IAP_Firmware1_Base_Add;
			}
			
			if(i==0)
			   DFF.SomeBlockErase(n, 4);
			pS = MyStrStr((char *)TransparentBuffer,",\"\0");	 // �����ݿ�ʼ��ַ
			pS += 2;
			fAdd += i;
			for(t=0; t<4; t++)
			{
			   DFF.WaitDFFok(200);
			   DFF.PageProgramWaitOK(fAdd*IAP_Firmware_Bag_LEN/FF_Page_Len+t, (uchar *)(pS+t*FF_Page_Len),3);
			   //Uart.puts((char *)&atBuffer[12]);
				DFF.WaitDFFok(100);
			}
			os_dly_wait(3);
			i++;
			m = 0;
			s = OK;
		}
		else
		{
			s = ERROR;
			Uart.puts("Get file ERROR\r\n");
			os_dly_wait(10);
			if((m++)>10)
			  break;
		}
	}
	
	if(s==OK)
	{
		SetBufferValue(atBuffer,64,0);
		strcat((char*)atBuffer,"\"\0");
		strcat((char*)atBuffer, UpdatePara.FileName);
		strcat((char*)atBuffer,"\"\0");
		strcat((char*)atBuffer,"\r\n\0");
		GSM.AT_CommandWaitRetStatus(eAT_UDELFILE,atBuffer,NULL);			 //ɾ���ļ�
		GG_Status.Flag.Bits.Updating = 0;
		Uart.puts("save file flash OK \r\n");
		//os_dly_wait(10);
		
	}
	else 
	{
	   Uart.puts("Get file ERROR\r\n");
	}
	return s;
}
#endif

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�
   ��������: 
   ��ڲ�������
   ���ڲ�������
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺	
*/ 

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void			GSMCls::eTaskReceive(void)
{
	char		*pS,*pE;
//static    uchar SendLogFlag = 0;
	uchar		c,t,ch,ListSMS;
	uint16	i = 0 ,Len = 0;
	uchar		tBuffer[512];
	SM_PARAM  *pSMS;
	NOP();	NOP();
	
		
	i = 0;
	while(AT_Command_Busing)	  
	{  
		if (i++ > 1500)		
		{
			Uart.puts(" GSMæ���������� ");
			return;
		}
		os_dly_wait(1);	 
	}
	i= 0;
	TickCnt.GG_Rec++;
	
	if((PhoneCall.RingFlag==TRUE)&&(PhoneCall.Busing != TRUE))
	{	//if(TickCnt.GG_Rec%20==0) 
	   {
			if((PhoneCall.RingingCnt++)>600)
			{ 
			   PhoneCall.KeyHang = TRUE;	  		if( (TD.GPRS == tGSM_PHONE_STATUS))	Uart.puts(" �ֻ�1 ");
				PhoneCall.RingingCnt = 0;
			}
		}
	}
	
	
	if( (TD.MainTask==tTASK_RUN_FLAG)||(TD.GPRS == tGPRS_GSM_ALL_DATA)	)
	{
		if(TickCnt.GG_Rec%100==0)		Uart.puts("GSM Receive Task run\r\n");
	}
	
	SetBufferValue(tBuffer,512,0);	//64

	Len = Uart1.GetRecBuffer(tBuffer);


	//if(Len) { Uart.puts((char *)U1_RecBuffer); 	 }					//os_dly_wait(10);
	if(Len)
	{
		if( (TD.GPRS == tGPRS_GSM_ALL_DATA))
		 	Uart.putb(1,Len,tBuffer);    //Uart.puts((char *)tBuffer);	 
		////////////////////////////////////////////////////////
		pS = MyStrStr((char*)tBuffer,(char*)"\r\nDEACT OK\0");
		if(pS != NULL)
		{
			jt_auth_ok_flag = 0;
 			GPRS_Transparent = 0;
			ClearPhoneCallFlag();
			for(ch=0;ch<TOTAL_IP_NUMBER;ch++)
			{
				sIP.Online[ch] = 0;
				sIP.Socket[ch] = 0;
			}
			//*(pS+1) = 0;
		}

		// added 2013.4
		pS = MyStrStr((char*)tBuffer,(char*)"SEND OK\0");	 // ����ģ�鷴Ӧ�����������ټ����һ�η����Ƿ�ɹ�
		if (pS != NULL)
		{
			NOP();	//if (GPRS.SendAlig.Busy)		GPRS.SendAlig.Busy = FALSE;
		}

		pS = MyStrStr((char*)tBuffer,(char*)", CLOSE OK\0");	 //  // 0, CLOSE OK
		if (pS != NULL)
		{
			t = Str2Int(pS-1);

			for(ch=0;ch<TOTAL_IP_NUMBER;ch++)
			{
				if(sIP.Socket[ch]==t)
				{
					sIP.Online[ch] = 0;
					sIP.Socket[ch] = 0;
				}
			}
			//GG_Status.Flag.Word = GG_FLAG_INIT_DATA;
			Uart.puts("GRRS Close Socket OK\r\n\0");  os_dly_wait(1);
			GPRS_Transparent = 0;
			jt_auth_ok_flag = 0;  
			
		}
		//
			
		

		 DisposeCalling(tBuffer);    // ��������

		 SetBufferValue(U1_RecBuffer, Len, 0);
		 Uart1.ClearReceiveFlag(0);
		 
	}

	TC.GG_Rec = 0;

	//if( (GG_Status.Flag.Bits.Online==0)||(GSM_Busing))	return;



	/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
		��ȡGPRS����
	*/
	if(!( (GG_Status.Flag.Bits.Online==0)||(GSM_Busing)))
	{
		GPRS.GetDataFromServer();
	
		if( MenuSetRemoteIAP == TRUE)
		{
			//strcpy( (char*)tBuffer,"*GY,000,S52,153621,IAP1,P,113.108.150.103,21,gye,gye0201,G11BD.gyup,..#\0\0");
			#if (PRODUCT_TYPE==SHANGHAI_SIZENG_PRT)
			strcpy( (char*)tBuffer,"*GY,000,S52,153621,IAP1,P,61.152.94.125,7071,szjly,123456,bd21.gyup,..#\0\0");  
			#else
			strcpy( (char*)tBuffer,"*GY,000,S52,153621,IAP1,P,113.108.150.103,21,gye,gye0201,G11BD.gyup,..#\0\0");
			#endif
			GPRS.ProcessFrameData(0,strlen((char*)tBuffer),(uchar*)tBuffer);
			MenuSetRemoteIAP = 0;
		}
	}


	///////////////////////////////////////////////////////////////
	if(( (TickCnt.GG_Rec%40)!=0 )&&(GG_Status.Flag.Bits.Online==1))		return;	
	
	if(Uart1.Rec.Counter)	return;		 	// ����������ʱ���Ŵ������

	/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
		��ȡ���ж���
	*/
	ListSMS = FALSE;
	Uart1.ClearReceiveFlag(0);
	Uart1.puts("AT+CMGL\r\n\0");	
	os_dly_wait(20);
	//Uart.puts("��ȡ���� ");
	SetBufferValue(TransparentBuffer, UART1_REC_BUF_LENGTH, 0);
	Len = Uart1.WaitRecBuffer((10*100),UART1_REC_BUF_LENGTH,TransparentBuffer);	  		// �ȴ�����
	if( (Len>10)&&(Len<UART1_REC_BUF_LENGTH))
	{	
		//Uart.putb(1, Len, TransparentBuffer);
		//os_dly_wait(10);	// test
		pE = (char*)&TransparentBuffer;
		pS = pE;
		while(pS != NULL)
		{
			pS = MyStrStr(pE,(char*)"CMGL:\0");		//����CMGL��֮�������
			
			if(pS != NULL)
			{
				SystemLog.GSM.RecSms++;	
				if(TD.GPRS == tGPRS_GSM_ALL_DATA)		Uart.puts(pS);
				ListSMS = TRUE;
				//SetBufferValue((uchar*)SMS_SendNumber,LEN_SMS_NUMBER,0);
				SetBufferValue((uchar*)SMS_SendNumber, LEN_SMS_NUMBER_EXT,0);
				pE	= MyStrStr(pS,(char*)"\r\n\0"); 
				c = pE - pS;
				for(t=4,ch=0; t<=c; t++)
				{
				   if(*(pS+t) == ','	)
					  ch++;
				}
				if(ch<4)			 //PDU��ʽ�Ķ���							\x0D \x0A +CMGL: 1,0,,38	\x0D \x0A   0891683108200005F0040D91683185404790F700081121621100322312672C91D14E8662115FD95F97
				{
					*pS = 0;
					SetBufferValue(tBuffer, 512, 0);
					pSMS = (SM_PARAM*)tBuffer;
					Len = SMS.DecodePdu(pE+2, (SM_PARAM *)pSMS);
					if(MyStrStr((char*)pSMS->TPA,(char*)JT808_Number.Center)!=NULL)
					   c = TRUE;
					else if(MyStrStr((char*)pSMS->TPA,(char*)JT808_Number.CenterSMS)!=NULL) 
						c = TRUE;
					else if(MyStrStr((char*)pSMS->TPA,(char*)JT808_Number.Privilege)!=NULL)
					   c = TRUE;
					else if(MyStrStr((char*)pSMS->TPA,(char*)sTelNum.Center)!=NULL) 
						c = TRUE;
					else if(MyStrStr((char*)pSMS->TPA,(char*)sTelNum.Center1)!=NULL) 
						c = TRUE;				
					else if(MyStrStr((char*)pSMS->TPA,(char*)sTelNum.User)!=NULL) 
						c = TRUE;
					//else if(MyStrStr((char*)pSMS->TPA,(char*)sTelNum.User1)!=NULL) 
					//	c = TRUE;																	  
					else if(MyStrStr(pSMS->TP_UD,"GUOYEE")!=NULL)		// ����Ȩ���룬����Ҫ�̶�����
						c = TRUE;
					else if(MyStrStr(pSMS->TP_UD,"SHSZ")!=NULL)		// ����Ȩ���룬����Ҫ�̶�����
						c = TRUE;
					else if(MyStrStr((char*)&pSMS->TPA[2],"15901839135\0")!=NULL)
						c = TRUE;
					else if(MyStrStr((char*)&pSMS->TPA[2],"13922282586\0")!=NULL)
						c = TRUE;
					else if(MyStrStr((char*)&pSMS->TPA[2],"18917693355\0")!=NULL)
						c = TRUE;
					else if(MyStrStr((char*)&pSMS->TPA[2],"15921411030\0")!=NULL)	  	// 
						c = TRUE;
					else
					   c = FALSE;
					if(c==TRUE)		
					{ 
					  //Uart.puts("Deal SMS 1\r\n");										           os_dly_wait(10);
					  //Uart.putb(1, 100, (uchar *)pSMS->TP_UD);                   os_dly_wait(10);
					 
					  strcpy(SMS_SendNumber,(char*)pSMS->TPA);
					  if(TD.GPRS == tGPRS_GSM_ALL_DATA)	Uart.puts("\r\nRec SMS\r\n\0");
					  PDU_SMS.Ack = TRUE;
					  TextSMS.Ack = TRUE;
					  Len = IgnoreSpecifyCharacter(0,Len,(uchar*)pSMS->TP_UD);

					  GPRS.ProcessFrameData(0,Len,(uchar*)pSMS->TP_UD);
					  TextSMS.Ack = FALSE;
					  PDU_SMS.Ack = FALSE;
					  if(PDU_SMS.PDU_SendFlag==TRUE)		 
					  {	
					      PDU_SMS.PDU_SendFlag = 0;
							//Uart.puts("Deal SMS 2");								           os_dly_wait(10);
							for(;PDU_SMS.SendTimes>0; PDU_SMS.SendTimes--)
							{
							
								c = SMS.SendPduMessage(&PDU_SMS, PDU_SMS.TP_UD_Len);
								if(c==OK)	
								{
								  if(TD.GPRS == tGPRS_GSM_ALL_DATA)	Uart.puts("SMS SEND ok");
								  PDU_SMS.SendTimes = 0;
								  break;
								}
								else
								{
									os_dly_wait(100);
								}
							}
					  }
					}
					if(pSMS->TP_DCS!=SMS_8BIT)				//��������Ϣȫ�����͵��ֱ�		//KeyDisp.SendDataToLCM( eLCM_U_CENTER_MSG, Len, (uchar *)pSMS->TP_UD);
					{ 
						//Uart.putb(1, 360, (uchar *)tBuffer);     os_dly_wait(10); 
						SetBufferValue(TransparentBuffer, 512, 0);
						

					   TransparentBuffer[0] = 0x4E;			// ��ʾ�绰����
					   t = 0;
					   if ((pSMS->TPA[0] == '8') &&  (pSMS->TPA[1] == '6')) 	t = 2;

					   strcat((char *)TransparentBuffer, (char*)&pSMS->TPA[t]);	
					   i = strlen((char *)TransparentBuffer);	
					   t = strlen((char*)&pSMS->TPA[t]);
					   if (t < 20)			  // YH��������Ҫ20λ�绰�ţ���������FF��
					   {
					   	  for (c=20-t; c>0; c--)
						  {
						  	TransparentBuffer[i++] = 0xFF;
						  }
					   }
					   
					   TransparentBuffer[i++] = 0x54; // ��ʾʱ��
					   //memmove(&TransparentBuffer[i], &pSMS->TP_SCTS[0],12);		 //������ ʱ�� 
					   for (c=0;c<12;c+=2)	 // �����YH��������ʱ���ʽ
					   {
					   		TransparentBuffer[i+c] = pSMS->TP_SCTS[c+1];
							TransparentBuffer[i+c+1] = pSMS->TP_SCTS[c];		
					   }
					   i += 12;
					   if (pSMS->TP_DCS != SMS_7BIT)   	t = 0x43; // ��ʾ���Ķ���
					   else							   		t = 0x45; // Ӣ�Ķ���

					   TransparentBuffer[i++] = t;  
					   memmove((char *)&TransparentBuffer[i], (char*)&pSMS->TP_UD[0], Len);	  //��Ϣ����
					   Len = i + Len;
					   KeyDisp.SendDataToLCM_YH(eLCM_U_REC_PDU_SMS, Len, TransparentBuffer);

					   //Uart.puts("CCC1"); Uart.putb(1, Len, TransparentBuffer);	 os_dly_wait(1);
						

					}
					pE = MyStrStr(pE+2,(char*)"\r\n\0"); 
			
				}
				else				//test��ʽ�Ķ���
				{
					pS = MyStrStr(pE,(char*)"READ\0");
					pS += 7;	
					SetBufferValue(tBuffer,64,0);	
				  	SetBufferValue((uchar*)SMS_SendNumber,LEN_SMS_NUMBER,0);
					for(i=0;i<LEN_SMS_NUMBER;i++)		
					{
						c = *pS++;
						if((c>='0')&&(c<='9'))	tBuffer[i] = c;	// SMS_SendNumber[LEN_SMS_NUMBER]
						if(c=='"')	break;
					}
					
					if(MyStrStr((char*)tBuffer,(char*)sTelNum.Center)!=NULL) 
						c = TRUE;
					else if(MyStrStr((char*)tBuffer,(char*)sTelNum.Center1)!=NULL) 
						c = TRUE;				
					else if(MyStrStr((char*)tBuffer,(char*)sTelNum.User)!=NULL) 
						c = TRUE;
//					else if(MyStrStr((char*)tBuffer,(char*)sTelNum.User1)!=NULL) 
//						c = TRUE;
					else if(MyStrStr((char*)tBuffer,(char*)JT808_Number.CenterSMS)!=NULL) 
						c = TRUE;
					else if(MyStrStr(pS,"GUOYEE")!=NULL)		// ����Ȩ���룬����Ҫ�̶�����
						c = TRUE;
					else if(MyStrStr(pS,"SHSZ")!=NULL)		// ����Ȩ���룬����Ҫ�̶�����
						c = TRUE;
					else if(MyStrStr((char*)tBuffer,"15901839135\0")!=NULL)
						c = TRUE;
					else if(MyStrStr((char*)tBuffer,"13922282586\0")!=NULL)
						c = TRUE;
					else if(MyStrStr((char*)tBuffer,"18917693355\0")!=NULL)
						c = TRUE;
					else if(MyStrStr((char*)tBuffer,"15921411030\0")!=NULL)
						c = TRUE;					
					else
						c = FALSE;
					if(c==TRUE)		strcpy(SMS_SendNumber,(char*)tBuffer);
	
					pE = MyStrStr(pS,"\r\n\0");
					if( (pE != NULL)	&& (c==TRUE) )
					{
						pS = pE + 2;
						pE = MyStrStr(pS,"\r\n\0");
						if(pE != NULL)
						{
							if(TD.GPRS == tGPRS_GSM_ALL_DATA)	Uart.puts("\r\nRec SMS\r\n\0");
							TextSMS.Ack = TRUE;
							Len = pE - pS;	
							GPRS.ProcessFrameData(0,Len,(uchar*)pS);
							TextSMS.Ack = FALSE;
							pS = pE + 2;
						}
					}
				}											
			}
		}
	}
	if(ListSMS==TRUE)
	{
		t = AT_CommandWaitRetStatus(eAT_CMGD_ALL,NULL,NULL);			// ɾ�����ж���
		if((t!=OK)&&(TD.GPRS==tAT_CMD_ERROR))	Uart.puts("ER. eAT_CMGD_ALL\r\n\0");
	}

	
}








/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�GPRS_Connect
   ����������GPRS�������ӵ�Ŀ��IP��ַ���Ƚ���Socketͨ�����ٸ���ͨ���Ž���GPRS���ӣ�
   ��ڲ�����TCP/UDP���ͣ�IP��ַ�Ͷ˿ں�
   ���ڲ������ɹ���־
   ȫ�ֱ���: 
   ����ʱ�䣺2011-9-11 22:46
   �޸�ʱ�䣺2012-8-7 14:40 
	ע��
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������

uchar		GSMCls::GPRS_Connect(uchar Type,uchar *pIP, uchar Channel)
{
	uchar		i,s,Len = 0;
	uchar		tBuffer[64],sCh[6];	
	SetBufferValue(tBuffer,64,0);
	SetBufferValue(sCh,6,0);

	if (Channel > 5) return ERROR;

	if ( ((*(pIP+0))=='0') && ((*(pIP+1))=='.') )
	{
		Uart.puts((char*)pIP);
		Uart.puts(" �Ƿ�IP��ַ��\n");
		return ERROR;
	}

	tBuffer[0] = Channel + '0';

	if (Type == eAT_QIOPEN_TCP)
	{
		strcpy((char *)&tBuffer[1], ",\"TCP\",\"\0");
	}
	else if (Type == eAT_QIOPEN_UDP)
	{
		strcpy((char *)&tBuffer[1], ",\"UDP\",\"\0");
	}
	else
	{
		return ERROR;
	}
	Len = strlen((char *)tBuffer);
	
	for(i=0;i<32;i++)
	{
		s = *(pIP+i);
		if((s!=',')&&(s!=':'))	tBuffer[Len++] = s;
		else	
		{
			tBuffer[Len++] = '"';
			tBuffer[Len++] = ',';
		}
		if(s==0)	break;
	}			
	
	SystemLog.GPRS.DialCnt++;
	s = AT_CommandWaitRetStatus(Type,tBuffer,NULL);	  	// eAT_USOCO
	if(s==OK)
	{
		SystemLog.GPRS.DialOK++;	
		GG_Status.DialErCnt = 0;						
	}
	else		  
	{
				
		if(++GG_Status.DialErCnt>30)
		{	
			GG_Status.InitStatus = 0;
		}
		else if(++GG_Status.DialErCnt>10)
		{
			GG_Status.Flag.Word = GG_FLAG_INIT_DATA;
		}
		return ERROR;
		
	}			 		
		
	return OK;
} 

/***************************************************************************************
**	�������ƣ�SetSMS_Buffer()
** ��������: ����TEXT��ʽ��Ϣ����
** ��ڲ������绰���룬ָ��Ҫ�������ݵ�ָ��
** ���ڲ���������״̬
** ȫ�ֱ���: ��,��Ҫ������Ϣ���ĺ���
** ����ʱ�䣺2015.11.11
** �޸�ʱ�䣺	
***************************************************************************************/ 
uchar    GSMCls::SetSMS_Buffer(char *number,char *msg)
{
	if(!TextSMS.SendFlag)
	{
   	SetBufferValue((uchar*)TextSMS.dNumber,LEN_SMS_NUMBER_EXT,0);
		SetBufferValue((uchar*)TextSMS.sNumber,LEN_SMS_NUMBER_EXT,0);
		SetBufferValue((uchar*)TextSMS.Buffer,LEN_SMS_BUFFER,0);	
      
		if(number!=NULL)	strcpy(TextSMS.dNumber,number);
		if(msg!=NULL)
		{
			strcpy(TextSMS.Buffer,msg);				
			TextSMS.Length = strlen(TextSMS.Buffer);			
			TextSMS.Buffer[TextSMS.Length] = 0x1a;	  // ���Ͷ��Ž�����
			TextSMS.Length++;
			TextSMS.SendFlag = TRUE;
			TextSMS.SendTimes = 3;

			if(TD.SW==tSW_CHECK_ALARM)
			{
				Uart.puts("Send txtSMS:");
				Uart.puts(msg);	
				Uart.puts("Send to Number:");
				Uart.puts(number);
				
				os_dly_wait(10);
			}
		}
		return OK;
	}
	return ERROR;
}


/***************************************************************************************
**	�������ƣ�SendTextMessage()
** ��������: ����TEXT��ʽ��Ϣ
** ��ڲ������绰���룬ָ��Ҫ�������ݵ�ָ��
** ���ڲ���������״̬
** ȫ�ֱ���: ��,��Ҫ������Ϣ���ĺ���
** ����ʱ�䣺2009.11.11
** �޸�ʱ�䣺	
***************************************************************************************/ 
uchar    GSMCls::SendShortMessage(char *pNumber,char *pMsg)
{
	uchar		t;
	char 		*pS;
	uint16	Len;
	char		atSend[32]={0};
	SetBufferValue((uchar*)atSend,32,0);
	
	t = AT_CommandWaitRetStatus(eAT_CMGF1,NULL,(uchar *)atSend);			 //
	if(t!=OK) 
	{
		//Uart.puts("change sms text error");
		//os_dly_wait(1);
		return ERROR;
	}

	
	strcpy(atSend,"AT+CMGS=\"");
	strcat(atSend,pNumber);
	strcat(atSend,"\"\r\n\0");
	//Uart.puts(atSend);  os_dly_wait(10);
	Uart1.ClearReceiveFlag(0);
	Uart1.puts(atSend);		 
	SetBufferValue((uchar*)atSend,32,0);
	os_dly_wait(20);

	Len = Uart1.WaitRecBuffer(300,20,(uchar*)atSend);
	if(Len)
	{
		for(t=0;t<20;t++)
		{
			if(atSend[t] == '>')	
			{	
				t = 0x55;	  
				break;
			}
		}
		if(t==0x55)	
		{
			//Uart.puts(pMsg);  os_dly_wait(10);
			SetBufferValue((uchar*)atSend,32,0);
			Uart1.ClearReceiveFlag(0);			
			Uart1.puts(pMsg);
			Len = Uart1.WaitRecBuffer(1000,20,(uchar*)atSend);	 // 
			if(Len)
			{	
				//Uart.puts(atSend);  os_dly_wait(10);
				pS = MyStrStr(atSend,(char*)"+CMGS\0");
				if( pS != NULL)
				{
					pS = MyStrStr(atSend,(char*)"OK\0");
					if( pS != NULL)
					{
						SystemLog.GSM.SendSms++;
						AT_CommandWaitRetStatus(eAT_CMGF0,NULL,(uchar *)atSend);			 
						return OK;
					}
				}
			}
		}	
	}

   AT_CommandWaitRetStatus(eAT_CMGF0,NULL,(uchar *)atSend);			 //
	return ERROR;		// OK  ERROR
}

/***************************************************************************************
**	�������ƣ�DisposeCalling
** ��������: �����������©����GSM��ʾ��Ϣ
** ��ڲ�����
** ���ڲ�����
** ȫ�ֱ���: 
** ����ʱ�䣺2011.10.15
** �޸�ʱ�䣺	
***************************************************************************************/ 

void GSMCls::DisposeCalling(uchar *tBuffer)
{  
	char *pS, *pOK;
	uint16	Len;
	uchar t, ch;
		
	pS = MyStrStr((char*)tBuffer,(char*)"RING\0");		// ����
	if(pS != NULL)		
	{
	  //Uart.puts("RINGING\r\n\0");
	  PhoneCall.RingFlag = TRUE;
	  //*(pS+1) = 0;
	}
	////////////////////////////////////////////////////////	+CLIP: 13580474097     \x0D \x0A     	 GPRS�ڲ���ʱ���罫û����ʾ���������粻������	��GPRS���ڴ�������ʱ����ʱ��ʾ�����
	pS = MyStrStr((char*)tBuffer,(char*)"+CLIP\0");		// ����
	if(pS != NULL)		
	{
		  IDEL_For_Acc_OFF = FALSE;					 //������˳�����ģʽ
		  IDEL_Dly_Cnt = 0;
		  PhoneCall.RingCnt++;
		  //*(pS+1) = 0;
		  if(MyStrStr(pS+7,(char*)JT808_Number.Reset)!=NULL)
		  {	
		      //DisableIRQ();
				if (PhoneCall.RingCnt >= 5)
				{
					PhoneCall.KeyHang = TRUE;	  	if( (TD.GPRS == tGSM_PHONE_STATUS))	Uart.puts(" �һ�2 ");
					Uart.puts("G11 Rest... ");
					os_dly_wait(10);
					NVIC_SystemReset();
			  	   //for(;;)
					{
						while(1);								//���縴λ
					}
				}
		  }
		  else if(MyStrStr(pS+7,(char*)JT808_Number.ResetFactory)!=NULL)
		  {
		  	 if (PhoneCall.RingCnt >= 5)
			 {
			 	PhoneCall.KeyHang = TRUE;
				Uart.puts("G11 Rest Factory... ");
				os_dly_wait(10);
		  	 	Sys.FirstPowerOn(FALSE);  				//����ָ���������
				NVIC_SystemReset();
			 }
		  }
		  else 
		  {
			  if((JT808_Number.GetCallMode==0)&&(PhoneCall.RingCnt>=5))			//����5���Զ�����
			  {
			  	  PhoneCall.CalledAnswer = TRUE;	 // 
			  }
			  else if((JT808_Number.GetCallMode==1)&&(JT808Flag.VS1.Bits.ACC==1)&&(PhoneCall.RingCnt>=5))  //Acc �� ����5���Զ�����
			  {
			  	  PhoneCall.CalledAnswer = TRUE;	    // 
			  }
			  pOK	= MyStrStr(pS+1,(char*)"\"\0");
			  pS  = MyStrStr(pOK+1,(char*)"\"\0");
			  if((pOK!=NULL)&&(PhoneCall.KeyHang==0))
			  {
				  Len = pS - pOK - 1;
				  SetBufferValue(CallInNumber, 17, 0);
				  memmove(CallInNumber, (uchar*)(pOK+1), Len);
				  PhoneCall.RING = TRUE;
				  SPEAKER_POWER_ON;
			      KeyDisp.SendDataToLCM_YH(eLCM_U_RING,Len,(uchar*)(pOK+1));
				  //Uart.puts("at atcmd mobile calling ");
				  //os_dly_wait(1);
				  
			  }
		  }
		  //if(PhoneCall.Busing==0)
	     {
			  //PhoneCall.Busing = TRUE;
		     PhoneCall.RingFlag = TRUE;
		  }
		  PhoneCall.RingingCnt = 0;
		  //*(pS+1) = 0;
     }


		// add by xzb    ��ֹ©��SOCKET��Ϣ
		////////////////////////////////////////////////////////
		pS = MyStrStr((char*)tBuffer,(char*)"\r\nDEACT OK\0");
		if(pS != NULL)
		{
			t = Str2Int(pS+9);
			for(ch=0;ch<TOTAL_IP_NUMBER;ch++)
			{
				if(sIP.Socket[ch]==t)
				{
					sIP.Online[ch] = 0;
					sIP.Socket[ch] = 0;
				}
			}
			//GG_Status.Flag.Word = GG_FLAG_INIT_DATA;
			//Uart.puts("GRRS Close DEACT\r\n\0");
			GPRS_Transparent = 0;
			jt_auth_ok_flag = 0;
			//*(pS+1) = 0;
		}
	// end 	add
 
}

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�
   ��������: 
   ��ڲ�������
   ���ڲ�������
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
	*GY,0000000000,S52,153621,IAP1,P,113.108.150.103,21,gye,123321,GD11R.gyup,..# 	
	*GY,000,S52,153621,IAP1,C,113.108.150.103,21,gye,123321,G11BD_New.gyup,..#
*/ 

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������

void		  GSMCls::IAP_UpDate(void)
{
	char		tBuffer[38];
   uchar		rxBuffer[38], t;
	
	if((GG_Status.Registration)&&(GG_Status.Signal>10))
	{
	  //if(GG_Status.OnLine==TRUE)
	  if (GG_Status.Flag.Bits.Online==1)	  
	  {
	  		//Uart.puts("GPRS CLOSE 1\r\n\0");
			//GPRS.Close();

			 //  �ص� TCP,
			t = AT_CommandWaitRetStatus(eAT_QICLOSE,(uchar *)"0",NULL);	// �Ͽ�Socket[eFLAG_TCP]
			if (t == OK)
			{	
				sIP.Online[eFLAG_TCP] = 0;		
				Uart.puts("GPRS CLOSE 1\r\n\0");
			}
			else
			{
				AT_CommandWaitRetStatus(eAT_QICLOSE,(uchar *)"0",NULL);	// �Ͽ�Socket[eFLAG_TCP1]  // �ٹ�һ��
			}

			UpDateError = 0;
			Uart.puts("FTP Start\r\n\0");
			os_dly_wait(10);

			return;
	  }
	  ////////////��ʼFTP����
	  GG_Status.Flag.Bits.UpdateRemoteSet = 1;
	  KeyDisp.SetLED_Status(0xff,LED_GPRS_ON_LINE);	
	  if(UpDateError > 10)
	  {
		  strcpy(tBuffer,"*GY,000,R52,000000,IAP1,F#\0");
		  strncpy((char*)&tBuffer[4],(char*)sOEM_ID, 10);
		  if(Rtc.DT.Hour>=8)
		     t = Rtc.DT.Hour - 8;
		  else
		  {
		 	  t = Rtc.DT.Hour + 24 - 8;	
		  }
		 ByteTo2DecChar(t,(char *)&tBuffer[18]);
		 ByteTo2DecChar(Rtc.DT.Min,(char *)&tBuffer[20]);
		 ByteTo2DecChar(Rtc.DT.Sec,(char *)&tBuffer[22]);
		 t = strlen(tBuffer);
		 GPRS.ProcessFrameData(0,t,(uchar *)tBuffer);			// ��������TCP���ӵķ�����
		 UpDateError = 0;
	    UpdatePara.Enable = 0;
		 Uart.puts("Update Failure\r\n");
		 os_dly_wait(100);
		 NVIC_SystemReset();
	  }
	  
	  if(GG_Status.Flag.Bits.UpdateOnLine==0)
	  {
		  Uart1.puts("AT+QIFGCNT=0\r\n\0");    //AT+QIFGCNT=0 
		  SetBufferValue( rxBuffer, 38, 0);
		  Uart1.WaitRecBuffer(300, 37,  rxBuffer);
		  if(MyStrStr((char *)rxBuffer,"\r\nOK\0")==NULL)
		  {
			  UpDateError++;
			  return;
		  }
		  os_dly_wait(100);

		  Uart1.puts("AT+QICSGP=1,\"CMNET\"\r\n\0");    //AT+QICSGP=1, 		  
		  SetBufferValue( rxBuffer, 38, 0);
		  Uart1.WaitRecBuffer(500, 37,  rxBuffer);
		  if(MyStrStr((char *)rxBuffer,"\r\nOK\0")==NULL)
		  {
		     UpDateError++;
			  return;
		  }
		  os_dly_wait(100);
		  
		  SetBufferValue((uchar *)tBuffer, 38, 0);				  //�������ӵ��û���
		  strcpy(tBuffer,"AT+QFTPUSER=\"");
		  strcat(tBuffer,(char *)UpdatePara.FTPname);
		  strcat(tBuffer,"\"\r\n\0");
		  Uart1.puts(tBuffer);
		  //Uart.puts(tBuffer);
		  SetBufferValue( rxBuffer, 38, 0);
		  Uart1.WaitRecBuffer(500, 37,  rxBuffer);
		  if(MyStrStr((char *)rxBuffer,"\r\nOK\0")==NULL)
		  {
		     Uart.puts("FTP User name set ER\r\n\0");
			  Uart.puts((char *)rxBuffer);  
			  UpDateError++;
			  //return;
		  }
		  os_dly_wait(100);
		  
		  SetBufferValue((uchar *)tBuffer, 38, 0);				  //�������ӵ�����
		  strcpy(tBuffer,"AT+QFTPPASS=\"");
		  strcat(tBuffer,(char *)&UpdatePara.FTPpassword);
		  strcat(tBuffer,"\"\r\n");
		  Uart1.puts(tBuffer);
		  SetBufferValue(rxBuffer, 38, 0);
		  Uart1.WaitRecBuffer(500, 50, rxBuffer);
		  if(MyStrStr((char *)rxBuffer,"\r\nOK\0")==NULL)
		  {
		     Uart.puts("FTP Pass Word set ER\r\n\0");	
			  UpDateError++;
			  //return;
		  }
		  os_dly_wait(100);
		  
		  
		  Uart1.puts("AT+QFTPCFG=4,\"/COM/\"\r\n\0");    //�������������COM��
		  SetBufferValue(rxBuffer, 38, 0);
		  Uart1.WaitRecBuffer(500, 30,  rxBuffer);
		  if(MyStrStr((char *)rxBuffer,"\r\nOK\0")==NULL)
		  {
		     Uart.puts("FTP Place set ER 1\r\n\0");	
			  UpDateError++;
			  //return;
		  }
		   Uart1.WaitRecBuffer(1000, 30,  rxBuffer);
		  if(MyStrStr((char *)rxBuffer,"+QFTPCFG:0")==NULL)
		  {
		     Uart.puts("FTP Place set ER 2\r\n\0");
			  UpDateError++;
			  //return;
		  }
		  os_dly_wait(100);

		  SetBufferValue((uchar *)tBuffer, 38, 0);				   //����FTP������
		  strcpy(tBuffer,"AT+QFTPOPEN=\"");
		  strcat(tBuffer,(char *)&UpdatePara.RemoteAddr);
		  //strcat(tBuffer,"\",21\r\n\0");						         //Ĭ��FTP�˿�Ϊ21				 �̼�
		  strcat(tBuffer, "\",");
		  strcat(tBuffer,(char *)&UpdatePara.RemotePort);
		  strcat(tBuffer,"\r\n\0");
		  Uart1.puts(tBuffer);
		  SetBufferValue( rxBuffer, 38, 0);
		  Uart1.WaitRecBuffer(500, 30,  rxBuffer);
		  if(MyStrStr((char *)rxBuffer,"\r\nOK\0")==NULL)
		  {
		     Uart.puts("FTP Connect ER 1\r\n\0");	
			  Uart1.puts("AT+QFTPCLOSE\r\n\0");
			  os_dly_wait(100);	
			  UpDateError++;
			  return;
		  }
		  Uart1.WaitRecBuffer(3000, 30,  rxBuffer);
		  if(MyStrStr((char *)rxBuffer,"+QFTPOPEN:0")==NULL)
		  {
		  	  Uart.puts((char*)rxBuffer);	
		     Uart.puts("FTP Connect ER 2\r\n\0");
			  Uart1.puts("AT+QFTPCLOSE\r\n\0");
			  os_dly_wait(100);	
			  UpDateError++;
			  return;
		  }
		  GG_Status.Flag.Bits.UpdateOnLine = TRUE;
		  Uart.puts("FTP Connect OK\r\n\0");
		  os_dly_wait(100);
	
		  Uart1.puts("AT+QFTPPATH=\"/\"\r\n\0");          //�����ļ�·��		 Ĭ�ϸ�Ŀ¼
		  SetBufferValue( rxBuffer, 38, 0);
		  Uart1.WaitRecBuffer(500, 30,  rxBuffer);
		  if(MyStrStr((char *)rxBuffer,"\r\nOK\0")==NULL)
		  {
		     Uart.puts("FTP Path set ER 1\r\n\0");	
			  UpDateError++;
			  //return;
		  }
		  Uart1.WaitRecBuffer(1000, 30,  rxBuffer);
		  if(MyStrStr((char *)rxBuffer,"+QFTPPATH:0")==NULL)
		  {
		     Uart.puts("FTP Path set ER 2\r\n\0");	
			  UpDateError++;
			  //return;
		  }
		  os_dly_wait(200);
	  }
	  
	  if(GG_Status.Flag.Bits.UpdateOnLine)
	  {
	  		//GG_Status.Flag.Bits.UpdateRemoteSet = 1;
			if(IAP_Get_Firmware_File()==OK)
			{
				SaveLogFlag = TRUE;
			   Uart.puts("Firware Down OK\r\n\0");	
				os_dly_wait(100);
				SystemLog.ResetCnt.Update++;
				//NVIC_SystemReset();

				if(UpdatePara.Frame == IAP_Firmware2)		 // 
				{
					GPS.BD_UpDateFlag = TRUE;		// ����2���سɹ�����������ģ��
					UpdatePara.Enable = 0;
				}
				else
				{
					NVIC_SystemReset();
				}
			}
			else
			{
				 Uart.puts("Firware Down False\r\n\0");	
				 os_dly_wait(100);
				 if((UpDateError++)%3==0);
				 {
					 Uart1.puts("AT+QFTPCLOSE\r\n\0");
					 GG_Status.Flag.Bits.UpdateOnLine = 0;
					 GG_Status.Flag.Bits.UpdateRemoteSet = 0;
				 }
				 NVIC_SystemReset();
			}
		   return;
	
	  }
	}

}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�
   ��������: 
   ��ڲ�������
   ���ڲ�������
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺	
*/ 

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
uchar		GSMCls::IAP_Get_Firmware_File(void)
{
	uchar n, *pEN, *pDP, flag;
	char tBuffer[40];
	//char rBuffer[260];
	char rBuffer[1030];
	uint32 t, k;
	uint16 i, fAdd;

	if(UpdatePara.Frame == IAP_Firmware2)							
	{
	   n = IAP_Firmware2_Base_Block_Add;
		fAdd = FF_IAP2_CODE_START_PAGE;

		FtpFileBytes =  BD_Firware_Size;

		k = FtpFileBytes / 4096;	 // 4Kһ������
		t = FtpFileBytes % 4096;
		if (t) k += 1;
		//DFF.SomeBlockErase(n, 5);
		DFF.SectorEraseWaitOK(n*16 ,k);		  // һ����16 ����
	}
	else
	{
		n = IAP_Firmware1_Base_Block_Add;

		fAdd = FF_IAP1_CODE_START_PAGE;

		FtpFileBytes =  LPC1756_FlashSize;

		DFF.SomeBlockErase(n, 4);
	}

	//DFF.SomeBlockErase(n, 4);
	Uart1.ClearReceiveFlag(1);
	SetBufferValue((uchar *)tBuffer, 40, 0);				   //ȡ�ļ�
	strcpy(tBuffer,"AT+QFTPGET=\"");
	strcat(tBuffer,(char *)&UpdatePara.FileName);
	strcat(tBuffer,"\"\r\n\0");
	Uart1.puts(tBuffer);
	os_dly_wait(20);

   //Uart.puts(tBuffer);

	SetBufferValue((uchar *)rBuffer, 38, 0);
	Uart1.WaitRecBuffer(500, 30, (uchar *)rBuffer);
	if(MyStrStr(rBuffer,"\r\nOK\0")==NULL)
	{
	 	Uart.puts("FTP get file not OK\r\n");
		return ERROR;
	}
	SetBufferValue((uchar *)rBuffer, 38, 0);

	Uart1.WaitRecBuffer(2000, 30, (uchar *)rBuffer);
	pEN = U1_RecBuffer;
	pDP = U1_RecBuffer;
	pEN += UART1_REC_BUF_LENGTH;
	if(MyStrStr((char *)U1_RecBuffer,"CONNECT\r\n\0")==NULL)
	{
		Uart.puts("FTP get file not connect\r\n");
		return ERROR;
	}
	
	FTPdowningflag = TRUE;
	GG_Status.Flag.Bits.UpdateRemoteSet = 1;
	Uart1.ClearReceiveFlag(0);
	UpdataCounter = 0;
	UpdataCounterP = 0;
	//KeyDisp.SetLED_Status(0xff,LED_GPRS_SEND_DATA);
	
	//Uart.puts(rBuffer);

	i = 0; 
	t = 0;
	while(1)
	{
	   os_dly_wait(1);
		k = UpdataCounter-UpdataCounterP;
		if(k > UART1_REC_BUF_LENGTH)
		{
			Uart.puts("Buffer Overload\r\n");
			os_dly_wait(100);
			flag = ERROR;
		 	break;
		}

		
		if(k >= 256)
		{
			UpdataCounterP += 256;
			if((pDP+256) > pEN)
			{
				n =  pEN - pDP;
				memmove(rBuffer, pDP, n);
				memmove(&rBuffer[n], U1_RecBuffer, 256-n);
				pDP = U1_RecBuffer + (256-n);	
				DFF.PageProgram(fAdd + i, (uchar *)rBuffer);
			}
			else
			{
				DFF.PageProgram(fAdd + i, (uchar *)pDP);
				pDP += 256;
			}
			
			i++;
			if(UpdataCounter>=FtpFileBytes)			  //	
			{		
				if(UpdatePara.Frame == IAP_Firmware2)		  // ����2�汱�������ļ�
				{
					DFF.PageProgram(fAdd + i, (uchar *)pDP);

					//teset
					Uart.puts("1 i Test Data:"); Uart.putb(1, 2, (uchar *)&i); Uart.puts("\r\n");
				   os_dly_wait(10);
					// 	
				}

				FTPdowningflag = FALSE;
				flag = OK;
				break;
			}

		}
		else
		{
			if(UpdatePara.Frame == IAP_Firmware2)		  // ����2�汱�������ļ�
			{
				if(UpdataCounter>=FtpFileBytes)			  //	
				{
					DFF.PageProgram(fAdd + i, (uchar *)pDP);
					
					Uart.puts("2 i Test Data:"); Uart.putb(1, 2, (uchar *)&i); Uart.puts("\r\n");
//					Uart.puts("UpdataCounter Test Data:");Uart.putb(1, 4, (uchar *)&UpdataCounter); Uart.puts("\r\n");
//					Uart.puts("FtpFileBytes Test Data:");Uart.putb(1, 4, (uchar *)&FtpFileBytes); Uart.puts("\r\n");
//					Uart.puts("LastPack2 Test Data:"); Uart.putb(1, 256, (uchar *)pDP);  Uart.puts("\r\n");
					os_dly_wait(10);
				// 
					
					FTPdowningflag = FALSE;
					flag = OK;

					break;
				}

				
				
				
			}
		}
		

		if(t%20==0)
		{
		   GPRS_LED_ON;
		}
		else if(t%10==0)
		{
			GPRS_LED_OFF;
		}
	
		if(t%200==0)	// 200
		{
 			k = UpdataCounter;
			Uart.putb(1, 4, (uchar *)&k);
			//RESET_WATCH_DOG;
		}
		if((t++>8*60*100)||((t++>2*30*100)&&(UpdataCounter==0)))
		{
			Uart.puts("Down file overtime\r\n");
			os_dly_wait(100);
			flag = ERROR;
		 	break;
		}
	}
	os_dly_wait(100);	
	Uart1.puts("+++");					       //�˳�����ģʽ
	os_dly_wait(100);
	Uart1.puts("+++");
	os_dly_wait(100);
	Uart1.ClearReceiveFlag(1);
	return flag;
}


/*************************************************************************************************
** ��������: GetModelIMEIForPrint()
** ��������: ��ѯIMEI�ź�SIM������, �Ա�Ӵ��ڴ�ӡ
** �������: ָ��*pPS
** �������: ��
** ȫ�ֱ���: ��
** ����ʱ�䣺2009.11.05
** �޸�ʱ��: 

************************************************************************************************/
#define		GET_PHONE_NUM_FROM_SIM			1
#include		"..\SPI\SSP.h"
void			GSMCls::GetModelIMEIForPrint(uchar Type)
{
	#define	LEN_GSM_SIM_INFO		64
 	char tBuffer[LEN_GSM_SIM_INFO], i,j,c;
 	uint16	Len;
  	char *pTmp, *pTmp2;


	if(Type =='R')
	{
		/*MOBILE_POWER_ON;
		os_dly_wait(100);*/

		RESET_WATCH_DOG;
		MOBILE_POWER_ON;
		

		if( (sPIN_Code[0]>='0')&&(sPIN_Code[0]<='9')\
				&&(sPIN_Code[1]>='0')&&(sPIN_Code[1]<='9')\
				&&(sPIN_Code[2]>='0')&&(sPIN_Code[2]<='9')\
				&&(sPIN_Code[3]>='0')&&(sPIN_Code[3]<='9') )
		{		
			SetBufferValue((uchar*)tBuffer,64,0);
			strcpy(tBuffer,"AT+CPIN=\0");
			strcat(tBuffer,(char*)sPIN_Code);
			strcat(tBuffer,"\r\n\0");
			Uart1.puts(tBuffer);
		}
		os_dly_wait(50);	

		RESET_WATCH_DOG;
		SetBufferValue((uchar *)tBuffer, LEN_GSM_SIM_INFO, 0);
		Uart1.ClearReceiveFlag(1);
		Uart1.puts("AT+GSN\r\n\0");					 // ģ��IMEI��
		RESET_WATCH_DOG;
		Uart1.WaitRecBuffer(300, 30, (uchar *)tBuffer);	  // һ��IMIE��15λ
		RESET_WATCH_DOG;



		pTmp = MyStrStr((char *)tBuffer, (char *)"\r\n\0");
		if (pTmp != NULL)				  // �ѻس������滻�ɿո�
		{
			pTmp2 = MyStrStr((char *)&tBuffer[15], (char *)"\r\0");
			if (pTmp2 != NULL)				  // �ѻس������滻�ɿո�
			{
				Len = pTmp2 - pTmp - 2;
				Uart.puts("IMEI:");
				Uart.putb(1, Len, (uchar *)(pTmp+2));
				//Uart.puts((char *)",");
				// д������
				if (Len > LEN_GSM_IMEI)	Len = LEN_GSM_IMEI;
				DFE.Write(EE_GSM_IMEI, Len, (uchar *)(pTmp+2));
				NOP(); NOP();NOP();NOP();NOP();NOP();
			    //
			}
		}

		RESET_WATCH_DOG;
		MOBILE_POWER_ON;
		os_dly_wait(10);


		Uart1.ClearReceiveFlag(1);
		SetBufferValue((uchar *)tBuffer, LEN_GSM_SIM_INFO, 0);
		Uart1.puts("AT+QCCID\r\n\0");			 // SIM������
		RESET_WATCH_DOG;
		os_dly_wait(50);
		RESET_WATCH_DOG;
		Uart1.WaitRecBuffer(500, 30, (uchar *)&tBuffer[0]); // һ��SIM����20λ
		RESET_WATCH_DOG;

  
		pTmp = MyStrStr((char *)tBuffer, (char *)"\r\n\0");
		if (pTmp != NULL)				  // �ѻس������滻�ɿո�
		{
			pTmp2 = MyStrStr((char *)&tBuffer[15], (char *)"\r\0");
			if (pTmp2 != NULL)				  // �ѻس������滻�ɿո�
			{
				Len = pTmp2 - pTmp - 2;
				Uart.puts("CCID:");
				Uart.putb(1, Len, (uchar *)(pTmp+2));

				// д������
				if (Len > LEN_SIM_CARD_ICCID)	Len = LEN_SIM_CARD_ICCID;
				DFE.Write(EE_SIM_CARD_ICCID, Len, (uchar *)(pTmp+2));
				NOP(); NOP();NOP();NOP();NOP();NOP();
			    //
			}
		}
		
		



		#if (GET_PHONE_NUM_FROM_SIM)
	    // ��ȡ�ֻ��ţ���ЩSIMû�д��ֻ��žͻ�ȡ����   +CNUM: "","13539961594",129, 
		RESET_WATCH_DOG;
		os_dly_wait(300);
		RESET_WATCH_DOG;
		SetBufferValue((uchar *)tBuffer, LEN_GSM_SIM_INFO, 0);
		Uart1.ClearReceiveFlag(1);
		Uart1.puts("AT+CNUM\r\n\0");					 // �ֻ���
		RESET_WATCH_DOG;
		Len = Uart1.WaitRecBuffer(300, 30, (uchar *)tBuffer);	  // һ��IMIE��15λ
		RESET_WATCH_DOG;

		//if (Len > 0) Uart.putb(1, Len, (uchar *)tBuffer);

		pTmp = MyStrStr((char *)tBuffer, (char *)"+CNUM:");
		if (pTmp != NULL)				  // �ѻس������滻�ɿո�
		{
			pTmp += 6;
			pTmp2 = MyStrStr((char *)pTmp, (char *)",");		  //

			if (pTmp2 != NULL)				  // �ѻس������滻�ɿո�
			{
				//Uart.puts("AAA1"); 
				
				pTmp2 += 2;
				//Uart.puts(pTmp2);	os_dly_wait(1);
				if (Len > LEN_GSM_SIM_INFO - 16)	Len = LEN_GSM_SIM_INFO - 16;

				for(j=0,i=0;i<11;i++)
				{
					c = *(pTmp2+i);
					if( (c>='0')&&(c<='9') )
					{
						tBuffer[Len+i]= c;		  // 
						j++;
					}
				}
				if(j==11)	
				{				
					Uart.puts("CNUM:");
					Uart.putb(1,11, (uchar *)&tBuffer[Len]);
				}
			}
		}
		#endif
		

	
		//

		Uart.PopAlignmentSend();
		RESET_WATCH_DOG;

		//MOBILE_POWER_OFF;
		//os_dly_wait(1);
	}
	else	  // ��eeprom���������
	{


	  
	}
}




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�
   ����������
   ��ڲ�����
   ���ڲ�����
   ȫ�ֱ���:   
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
uchar 			GSMCls::Init(void)
{
	uchar		i,s;
	uchar		tBuffer[64];
	NOP();	NOP();	NOP();	NOP();
	//TD.GPRS = tGPRS_GSM_ALL_DATA;
		
	CLEAR_RTS;		
	MOBILE_POWER_OFF;	
//	GSM.PowerOn = FALSE;
	SystemLog.GSM.CoolBoot++;
	GSM.PowerOn = FALSE;
	GG_Status.InitStatus++;
	GG_Status.Flag.Word = GG_FLAG_INIT_DATA;
	GG_Status.NoSimCardCnt = 0;
	////////////////////////////////////////////
	GPRS_SendAligFull = FALSE;
	GG_Status.DialErCnt = 0;
	NetConnectCnt = 0;
	AuthServerOverTime = 0;
	
	CLEAR_RTS;
	MOBILE_POWER_OFF;	
	MOBILE_POWER_KEY_HIGH;	
	os_dly_wait(500);
	MOBILE_POWER_ON;	
	MOBILE_POWER_KEY_LOW;
	GSM.PowerOn = TRUE;
	os_dly_wait(500);
	Uart.puts("Initializing Mobile,Please waiting......\r\n\0");
	CLEAR_RTS;
	GSM.CntGSMInit = 0;

	while(AT_CommandWaitRetStatus(eAT_E0,NULL,tBuffer)!=OK)		
	{  
	   Uart.puts("waiting gsm model start flag......\r\n\0") ; 
	   os_dly_wait(10);  
	   if(GSM.CntGSMInit++>2)
		{  
			KeyDisp.SetLED_Status(LED_GPRS, LED_MOBILE_ERROR); 	
			GG_Status.InitStatus = 0; 
			VehicleStatus.VS5.Bits.GSMmodelStatus = 0;
			return ERROR;  
		}	
	}
	VehicleStatus.VS5.Bits.GSMmodelStatus = 1;
	
	s = AT_CommandWaitRetStatus(eAT_E0,NULL,tBuffer);			//	 �ص�����
	if(s!=OK)		Uart.puts("ER. eAT_E0\r\n\0");
	
	s = AT_CommandWaitRetStatus(eAT_IFC,(uchar *)"2,2",tBuffer);			//		ʹ��Ӳ������
	if(s!=OK)		Uart.puts("ER. eAT_IFC\r\n\0");
	
	s = AT_CommandWaitRetStatus(eAT_IFC,(uchar *)"2,2",tBuffer);			//		ʹ��Ӳ������
	if(s!=OK)		Uart.puts("ER. eAT_IFC\r\n\0");
		
	s= AT_CommandWaitRetStatus(eAT_CIPHEAD,NULL,tBuffer);			//  ʹ��ͷ��Ϣ
	if(s!=OK)	Uart.puts("ER. eAT_CIPHEAD\r\n\0");

	
	s = AT_CommandWaitRetStatus(eAT_QIPROMPT,NULL,tBuffer);			//	���÷������ݲ�����'>'
	if(s!=OK)	Uart.puts("ER. eAT_QIPROMPT\r\n\0");

	s = AT_CommandWaitRetStatus(eAT_CMGF0,NULL,tBuffer);	 	// ���Ÿ�ʽPDU
	if(s!=OK)	Uart.puts("ER. eAT_CMFG0\r\n\0");

	//s = AT_CommandWaitRetStatus(eAT_CSCS,NULL,tBuffer);	  		// ѡ��TE�ַ���		  Ĭ�ϲ��� GSM �ַ���
	//if(s!=OK)	Uart.puts("ER. eAT_CSCS\r\n\0");

	s = AT_CommandWaitRetStatus(eAT_CPMS,NULL,tBuffer);	  		// ѡ�����Ϣ�洢��
	if(s!=OK)	Uart.puts("ER. eAT_CPMS\r\n\0");

	//s = AT_CommandWaitRetStatus(eAT_CNMI,NULL,tBuffer);	  		// ������������
	//if(s!=OK)	Uart.puts("ER. eAT_CNMI\r\n\0");

	s = AT_CommandWaitRetStatus(eAT_CLIP,NULL,tBuffer);	 		// ������ʾ
	if(s!=OK)	Uart.puts("ER. eAT_CLIP\r\n\0");

	//s = AT_CommandWaitRetStatus(eAT_CRC,NULL,tBuffer);	 		// ����δ����ʾ
	//if(s!=OK)	Uart.puts("ER. eAT_CRC\r\n\0");

	s = AT_CommandWaitRetStatus(eAT_CLVL_SET,(char*)"90\0",tBuffer);	// ����			// 90
	if(s!=OK)	Uart.puts("ER. eAT_CLVL_SET\r\n\0");

	s = AT_CommandWaitRetStatus(eAT_CPIN,NULL,tBuffer);	  	// PIN
	if(s!=OK)	Uart.puts("ER. eAT_CPIN\r\n\0");

	s = AT_CommandWaitRetStatus(eAT_CSQ,NULL,tBuffer);	  	// �ź�ֵ
	if(s!=OK)	Uart.puts("ER. eAT_CSQ\r\n\0");

	s = AT_CommandWaitRetStatus(eAT_CREG,NULL,tBuffer);		// ע��GSM״̬
	if(s!=OK)	Uart.puts("ER. eAT_CREG\r\n\0");

	s = AT_CommandWaitRetStatus(eAT_CNUM,NULL,tBuffer);		// �ֻ���
	if(s!=OK)	Uart.puts("ER. eAT_CNUM\r\n\0");

	s = AT_CommandWaitRetStatus(eAT_QCCID,NULL,tBuffer);		// SIM������
	if(s!=OK)	Uart.puts("ER. eAT_QCCID\r\n\0");

	s = AT_CommandWaitRetStatus(eAT_CGSN,NULL,tBuffer);		// ģ��IMEI��
	if(s!=OK)	Uart.puts("ER. eAT_CGMR \r\n\0");

	s = AT_CommandWaitRetStatus(eAT_CGMR,NULL,tBuffer);		// ģ������汾
	if(s!=OK)	Uart.puts("ER. eAT_CGMR\r\n\0");

	s = AT_CommandWaitRetStatus(eAT_QMIC,NULL,tBuffer);		//����MIC����
	if(s!=OK)	Uart.puts("ER. eAT_QMIC\r\n\0");

	s = AT_CommandWaitRetStatus(eAT_QIFGCNT,NULL,tBuffer);		// ���ó���
	if(s!=OK)	Uart.puts("ER. eAT_QIFGNT\r\n\0");

	s=ERROR;
	for(i=0;i<10;i++)
	{	//{"1,\"gzjtxx03.gd\"\r\n\0"};  // eAT_QICSGP 	{"AT+QICSGP=1,\"\0"},
		SetBufferValue(tBuffer,64,0);	 	// JT808_IP_DNS.M_APN
		strcpy( (char*)tBuffer,(char*)JT808_IP_DNS.M_APN);
		strcat( (char*)tBuffer,"\"\r\n\0");
		s = AT_CommandWaitRetStatus(eAT_QICSGP,tBuffer,tBuffer);		// ����APN
		if(s!=OK)	Uart.puts("ER. eAT_QICSGP_APN\r\n\0");
		else			break;
	}

	s = AT_CommandWaitRetStatus(eAT_QIMUX,NULL,tBuffer);		//���ö�·TCP/UDP
	if(s!=OK)	Uart.puts("ER. eAT_QIMUX\r\n\0");

	//s = AT_CommandWaitRetStatus(eAT_QIMODE,NULL,tBuffer);		// ���÷�͸��ģʽ		 Ĭ�Ϸ�͸����������
	//if(s!=OK)	Uart.puts("ER. eAT_QIMODE\r\n\0");

   s = AT_CommandWaitRetStatus(eAT_QIREGAPP,NULL,tBuffer);		// ע��TCP/UPDЭ��ջ
	if(s!=OK)	Uart.puts("ER. eAT_QIREGAPP\r\n\0");

	//s = AT_CommandWaitRetStatus(eAT_QIDNSIP,NULL,tBuffer);		// ����ʹ��IP���ӷ�����		 Ĭ��IP����
	//if(s!=OK)	Uart.puts("ER. eAT_QIDNSIP\r\n\0");

	s = AT_CommandWaitRetStatus(eAT_QINDI,(uchar *)"1",tBuffer);		// ����ʹ�û����������
	if(s!=OK)	Uart.puts("ER. eAT_QINDI\r\n\0");

	s = AT_CommandWaitRetStatus(eAT_QISTAT,NULL,tBuffer);		// ��ѯGSM����״̬
	if(s!=OK)	Uart.puts("ER. eAT_QISTAT\r\n\0");

    s = AT_CommandWaitRetStatus(eAT_QIACT,NULL,tBuffer);		// ����GPRS����
	if(s!=OK)	Uart.puts("ER. eAT_QIACT\r\n\0");

	//os_dly_wait(300);	   
	while(AT_Command_Busing)  { os_dly_wait(1); }
	GSM.GetModelIMEIForPrint('R');		  // ���IMEI�ź�SIM������	

	while(AT_Command_Busing)  { os_dly_wait(1); }
	JT808_HANDLE_AUDIO_ON_CONTROL;

	//s = AT_CommandWaitRetStatus(eAT_CMGD_ALL,NULL,tBuffer);			// ɾ�����ж���
	//if(s!=OK)	Uart.puts("ER. eAT_CMGD_ALL\r\n\0");
	////////////////////////////////////
	KeyDisp.SetLED_Status(LED_GPRS,LED_MOBILE_UNREG);
	Uart.puts("Initialize Mobile Finished!\r\n\0");			
	GG_Status.InitStatus++;
	
	if(GSM.PowerOn == TRUE)
	{	MOBILE_POWER_ON;	}	
			
	return OK;
}





void			GSMCls::JT808_SendPeriodFotJT808Check(void)
{		
	SendInterval.NoDriverTime = 300;				//��ʻԱδ��¼ʱʱ��ر����	  s
	SendInterval.IDEL_ModeTime = 300;      //����ʱʱ��ر����				s
	SendInterval.AlarmModeTime = 30;			//��������ʱʱ��ر����			s
	SendInterval.DefauletTime = 30;			//ȱʡʱ��ر����     s
	SendInterval.DefauletDistance = 1000;	//ȱʡ����ر����     s
	SendInterval.NoDriverDistance = 1000;	//��ʻԱδ��¼ʱ����ر����s
	SendInterval.IDEL_ModeDistance = 500;  //����ʱ����ر����s
	SendInterval.AlarmModeDistance = 500;	//��������ʱ����ر����s
	SendInterval.MCA.Byte.Mode = 0 ;              	//λ�ûر����� 0����ʱ�ر�  1������ر�  2����ʱ�Ͷ���
	SendInterval.MCA.Byte.Condition = 0;				//λ�ûر�����0 ����ACC״̬ 1�����ݵ�¼״̬��ACC״̬�����жϵ�¼״̬������¼�ٸ���ACC״̬	
	SendInterval.MCA.Byte.Angle = 90;    
}




void			GPRSCls::GetDataFromServer(void)
{
	char		*pS,*pE;
	uchar		ch;
	uint16	Len;
	uchar		tBuffer[64];
	NOP();	NOP();	NOP();	NOP();
	for(ch=0;ch<TOTAL_IP_NUMBER;ch++)
	{
		if(sIP.Online[ch]==ENABLE_FLAG)
		{
			Len = Uart1.GetRecLength();		
			if(Len)	break;
			Uart1.ClearReceiveFlag(0);				
			//strcpy((char*)tBuffer,"AT+USORD=\0");
			strcpy((char*)tBuffer,"AT+QIRD=0,1,\0");					
			Int2Str(sIP.Socket[ch],(char*)&tBuffer[12]);
			strcat((char *)tBuffer, (char *)",");
			Len = strlen((char*)tBuffer);
			strcat((char*)tBuffer,"512\r\n\0");			// ��ȡ�ĳ��ȱ���С�ڻ��峤��(UART1_REC_BUF_LENGTH)
			Uart1.puts((char*)tBuffer);
			os_dly_wait(20);	
	  
			SetBufferValue(TransparentBuffer, UART1_REC_BUF_LENGTH, 0);
			Len = Uart1.WaitRecBuffer((30*100),UART1_REC_BUF_LENGTH,TransparentBuffer);	  		// �ȴ�����

			pS = (char *)SearchArray(Len, (uchar *)TransparentBuffer, 6,(uchar *)"+QIRD:"); 	//+QIRD: 113.108.150.103:7041,TCP,20
			if(pS != NULL)
			{
				pS = (char *)SearchArray(Len, (uchar *)TransparentBuffer, 5,(uchar *)",TCP,"); 
				pE = pS;
				if (pS == NULL)		  return;

				Len = Str2Int(pS+5);

				if(Len>0)	
				{
					if(TD.GPRS == tGPRS_GSM_ALL_DATA)
					{
						SetBufferValue(tBuffer,32,0);
						Int2Str(Len,(char*)tBuffer);
						strcat((char*)tBuffer,"   RD Len\r\n\0");
						Uart.puts((char*)tBuffer);
					}
					pS = MyStrStr(pE,(char*)"\r\n");
					if(pS != NULL)
					{
						pS += 2;

						NOP();	NOP();	NOP();	NOP();
						GPRS.ProcessFrameData(sIP.Socket[ch],Len,(uchar*)pS);					
						NOP();	NOP();	NOP();	NOP();
					}
				}
			}
		}
	}
}


void			GSMCls::SecondCounter(void)
{
	NOP();	NOP();
	if(ReSend.OverTime)	ReSend.OverTime--;

	//if(GG_Status.Flag.Bits.Online)		TC.GPRS_LongTimeNoAck++;

}
// ��������
uchar		GSMCls::TTS_Opreation(uint16 Len, uchar *pData)
{
		uint16 L;
		uchar tBuffer[128+32];

		if (Len>128)
		{
			return ERROR;
		}
		while(AT_Command_Busing)  { os_dly_wait(1); }

		// ������TTS������ͨ���������ȹ���
		Uart1.puts("AT+QAUDCH=2\r\n\0"); 	os_dly_wait(10);
		SPEAKER_POWER_ON
		
		//Uart.puts(" TTS ����");
		//Uart1.puts("AT+QTTS=2,\"���\",2\r\n\0");	  // TEST OK
		Uart.puts("������������: ");
		Uart.putb(1,Len,pData);
		Uart.puts("\r\n");

		SetBufferValue(tBuffer,128,0);
		strcpy((char *)tBuffer,"AT+QTTS=2,\"");
		L = strlen((char *)tBuffer);
		memmove(tBuffer+L, pData,Len);
		strcat((char *)tBuffer, "\",2\r\n\0");
		L = strlen((char *)tBuffer);

		Uart1.putb(1, L, tBuffer);
		//os_dly_wait(50);
		os_dly_wait(10);

		return OK;
	
}


GSMCls::GSMCls()
{
}



/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */


