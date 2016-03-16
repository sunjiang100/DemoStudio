/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$									广州国业电子科技有限公司
$									
$                       	http://www.guoyee.com		 Email: guoyegps@126.com	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      文件描述
文件名:   gsm_send.cPP
功能描述：GSM发送任务
编译环境：
目标CPU: 
作者：						 
创建时间：
建议编辑环境：UltraEdit 11.00a+  制表符宽度为三个字符，程序可读性更好。
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
	函数名称：GSMCls::GSM_Transmit_Task
   功能描述: 
   入口参数：无
   出口参数：无
   全局变量: 
   创建时间：2015-10-10 12:29
   修改时间：	
*/ 

//static   uchar FlagBak;
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
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

	ReSend.OverTime = ReSend.Times = 0;	  // 应答超时间 10s
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

		if( (JT_Ctr_PowerDown == 1)||(U0_Iap_Flag!=0) )		  //关机
		{
			TC.GPRS_LongTimeNoSend = 0;
			TC.GG_Rec = TC.GG_Send = 0;
			os_dly_wait(100);
			continue;
		}
		/*				  //delete By Sun
		if(JT_ShutMobileCnt)	 	// 测试软件规定，进入围栏后，不能上报任何信息
		{
			os_dly_wait(100);


			#if (WUXI_TEST != 1)
			if(JT_ShutMobileCnt==3)
			{
				GG_Status.Flag.Word = GG_FLAG_INIT_DATA;	
				OneFrame[0] = OneFrame[1] = OneFrame[2] = OneFrame[3] = OneFrame[4] = 0;
				Int2Str(sIP.Socket[eFLAG_TCP],(char*)OneFrame);
				GSM.AT_CommandWaitRetStatus(eAT_QICLOSE,OneFrame,NULL);	// 断开Socket[eFLAG_TCP1]
			}
			TC.GSM = 0;
			TC.GG_Send = 0;
			TC.GG_Rec = 0;
			TC.GPRS_LongTimeNoSend = 0;			
			TickCnt.GG_Send++;
			if(TickCnt.GG_Send%10==0)		Uart.puts("终端被控，已关闭GPRS模块 ");			
			GG_Status.InitStatus = 0;		// if(JT_ShutMobileCnt==0)时，重启模块
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

		CLEAR_RTS;	 		// MCU永远处于准备状态

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
	函数名称：
   功能描述:  发送注册包和心跳包
   入口参数：无
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：	
*/ 

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void		GSMCls::AutoPeriodSend(void)
{
	
}















/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：
   功能描述: 
   入口参数：无
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：	
*/ 

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
uchar		GSMCls::WaitGSM_Free(void)
{
	uint32	i;
	for(i=0;i<(30*10UL);i++)
	{
		os_dly_wait(10);
		if(GSM_Busing==FALSE)	return OK;
		if( (i%(5*10))==0 )		
			Uart.puts("GSM模块流控忙\r\n\0");
	}
	GG_Status.InitStatus = 0;
	return ERROR;	
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：
   功能描述: 
   入口参数：无
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：	
*/ 

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
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
	函数名称：
   功能描述: 
   入口参数：无
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：	
*/ 

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
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
		strcpy((char *)atBuffer,"AT+URDBLOCK=");								 //找到下载的文件,读出文件
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
	   Uart1.WaitRecBuffer((30*100),UART1_REC_BUF_LENGTH,TransparentBuffer);	  		// 等待返回	
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
			pS = MyStrStr((char *)TransparentBuffer,",\"\0");	 // 找数据开始地址
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
		GSM.AT_CommandWaitRetStatus(eAT_UDELFILE,atBuffer,NULL);			 //删除文件
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
	函数名称：
   功能描述: 
   入口参数：无
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：	
*/ 

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
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
			Uart.puts(" GSM忙，放弃接收 ");
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
			   PhoneCall.KeyHang = TRUE;	  		if( (TD.GPRS == tGSM_PHONE_STATUS))	Uart.puts(" 持机1 ");
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
		pS = MyStrStr((char*)tBuffer,(char*)"SEND OK\0");	 // 可能模块反应慢，在这里再检测上一次发送是否成功
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
			
		

		 DisposeCalling(tBuffer);    // 处理来电

		 SetBufferValue(U1_RecBuffer, Len, 0);
		 Uart1.ClearReceiveFlag(0);
		 
	}

	TC.GG_Rec = 0;

	//if( (GG_Status.Flag.Bits.Online==0)||(GSM_Busing))	return;



	/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
		读取GPRS数据
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
	
	if(Uart1.Rec.Counter)	return;		 	// 无其它数据时，才处理短信

	/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
		读取所有短信
	*/
	ListSMS = FALSE;
	Uart1.ClearReceiveFlag(0);
	Uart1.puts("AT+CMGL\r\n\0");	
	os_dly_wait(20);
	//Uart.puts("读取短信 ");
	SetBufferValue(TransparentBuffer, UART1_REC_BUF_LENGTH, 0);
	Len = Uart1.WaitRecBuffer((10*100),UART1_REC_BUF_LENGTH,TransparentBuffer);	  		// 等待返回
	if( (Len>10)&&(Len<UART1_REC_BUF_LENGTH))
	{	
		//Uart.putb(1, Len, TransparentBuffer);
		//os_dly_wait(10);	// test
		pE = (char*)&TransparentBuffer;
		pS = pE;
		while(pS != NULL)
		{
			pS = MyStrStr(pE,(char*)"CMGL:\0");		//包括CMGL及之后的数据
			
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
				if(ch<4)			 //PDU格式的短信							\x0D \x0A +CMGL: 1,0,,38	\x0D \x0A   0891683108200005F0040D91683185404790F700081121621100322312672C91D14E8662115FD95F97
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
					else if(MyStrStr(pSMS->TP_UD,"GUOYEE")!=NULL)		// 非授权号码，则需要固定密码
						c = TRUE;
					else if(MyStrStr(pSMS->TP_UD,"SHSZ")!=NULL)		// 非授权号码，则需要固定密码
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
					if(pSMS->TP_DCS!=SMS_8BIT)				//非数据信息全部发送到手柄		//KeyDisp.SendDataToLCM( eLCM_U_CENTER_MSG, Len, (uchar *)pSMS->TP_UD);
					{ 
						//Uart.putb(1, 360, (uchar *)tBuffer);     os_dly_wait(10); 
						SetBufferValue(TransparentBuffer, 512, 0);
						

					   TransparentBuffer[0] = 0x4E;			// 表示电话号码
					   t = 0;
					   if ((pSMS->TPA[0] == '8') &&  (pSMS->TPA[1] == '6')) 	t = 2;

					   strcat((char *)TransparentBuffer, (char*)&pSMS->TPA[t]);	
					   i = strlen((char *)TransparentBuffer);	
					   t = strlen((char*)&pSMS->TPA[t]);
					   if (t < 20)			  // YH调度屏需要20位电话号，不够，用FF补
					   {
					   	  for (c=20-t; c>0; c--)
						  {
						  	TransparentBuffer[i++] = 0xFF;
						  }
					   }
					   
					   TransparentBuffer[i++] = 0x54; // 表示时间
					   //memmove(&TransparentBuffer[i], &pSMS->TP_SCTS[0],12);		 //年月日 时分 
					   for (c=0;c<12;c+=2)	 // 整理成YH调度屏的时间格式
					   {
					   		TransparentBuffer[i+c] = pSMS->TP_SCTS[c+1];
							TransparentBuffer[i+c+1] = pSMS->TP_SCTS[c];		
					   }
					   i += 12;
					   if (pSMS->TP_DCS != SMS_7BIT)   	t = 0x43; // 表示中文短信
					   else							   		t = 0x45; // 英文短信

					   TransparentBuffer[i++] = t;  
					   memmove((char *)&TransparentBuffer[i], (char*)&pSMS->TP_UD[0], Len);	  //信息内容
					   Len = i + Len;
					   KeyDisp.SendDataToLCM_YH(eLCM_U_REC_PDU_SMS, Len, TransparentBuffer);

					   //Uart.puts("CCC1"); Uart.putb(1, Len, TransparentBuffer);	 os_dly_wait(1);
						

					}
					pE = MyStrStr(pE+2,(char*)"\r\n\0"); 
			
				}
				else				//test格式的短信
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
					else if(MyStrStr(pS,"GUOYEE")!=NULL)		// 非授权号码，则需要固定密码
						c = TRUE;
					else if(MyStrStr(pS,"SHSZ")!=NULL)		// 非授权号码，则需要固定密码
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
		t = AT_CommandWaitRetStatus(eAT_CMGD_ALL,NULL,NULL);			// 删除所有短信
		if((t!=OK)&&(TD.GPRS==tAT_CMD_ERROR))	Uart.puts("ER. eAT_CMGD_ALL\r\n\0");
	}

	
}








/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：GPRS_Connect
   功能描述：GPRS拨号链接到目的IP地址（先建立Socket通道，再根据通道号建立GPRS链接）
   入口参数：TCP/UDP类型，IP地址和端口号
   出口参数：成功标志
   全局变量: 
   创建时间：2011-9-11 22:46
   修改时间：2012-8-7 14:40 
	注：
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用

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
		Uart.puts(" 非法IP地址！\n");
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
**	函数名称：SetSMS_Buffer()
** 功能描述: 设置TEXT格式信息缓存
** 入口参数：电话号码，指向要发送内容的指针
** 出口参数：操作状态
** 全局变量: 无,但要设置信息中心号码
** 创建时间：2015.11.11
** 修改时间：	
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
			TextSMS.Buffer[TextSMS.Length] = 0x1a;	  // 发送短信结束符
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
**	函数名称：SendTextMessage()
** 功能描述: 发送TEXT格式信息
** 入口参数：电话号码，指向要发送内容的指针
** 出口参数：操作状态
** 全局变量: 无,但要设置信息中心号码
** 创建时间：2009.11.11
** 修改时间：	
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
**	函数名称：DisposeCalling
** 功能描述: 处理来电可能漏掉的GSM提示信息
** 入口参数：
** 出口参数：
** 全局变量: 
** 创建时间：2011.10.15
** 修改时间：	
***************************************************************************************/ 

void GSMCls::DisposeCalling(uchar *tBuffer)
{  
	char *pS, *pOK;
	uint16	Len;
	uchar t, ch;
		
	pS = MyStrStr((char*)tBuffer,(char*)"RING\0");		// 来电
	if(pS != NULL)		
	{
	  //Uart.puts("RINGING\r\n\0");
	  PhoneCall.RingFlag = TRUE;
	  //*(pS+1) = 0;
	}
	////////////////////////////////////////////////////////	+CLIP: 13580474097     \x0D \x0A     	 GPRS在拨号时来电将没有提示，出现来电不能提醒	或GPRS正在传输数据时会延时提示的输出
	pS = MyStrStr((char*)tBuffer,(char*)"+CLIP\0");		// 来电
	if(pS != NULL)		
	{
		  IDEL_For_Acc_OFF = FALSE;					 //来电会退出休眠模式
		  IDEL_Dly_Cnt = 0;
		  PhoneCall.RingCnt++;
		  //*(pS+1) = 0;
		  if(MyStrStr(pS+7,(char*)JT808_Number.Reset)!=NULL)
		  {	
		      //DisableIRQ();
				if (PhoneCall.RingCnt >= 5)
				{
					PhoneCall.KeyHang = TRUE;	  	if( (TD.GPRS == tGSM_PHONE_STATUS))	Uart.puts(" 挂机2 ");
					Uart.puts("G11 Rest... ");
					os_dly_wait(10);
					NVIC_SystemReset();
			  	   //for(;;)
					{
						while(1);								//来电复位
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
		  	 	Sys.FirstPowerOn(FALSE);  				//来电恢复出厂设置
				NVIC_SystemReset();
			 }
		  }
		  else 
		  {
			  if((JT808_Number.GetCallMode==0)&&(PhoneCall.RingCnt>=5))			//振铃5次自动接听
			  {
			  	  PhoneCall.CalledAnswer = TRUE;	 // 
			  }
			  else if((JT808_Number.GetCallMode==1)&&(JT808Flag.VS1.Bits.ACC==1)&&(PhoneCall.RingCnt>=5))  //Acc 开 振铃5次自动接听
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


		// add by xzb    防止漏掉SOCKET信息
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
	函数名称：
   功能描述: 
   入口参数：无
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
	*GY,0000000000,S52,153621,IAP1,P,113.108.150.103,21,gye,123321,GD11R.gyup,..# 	
	*GY,000,S52,153621,IAP1,C,113.108.150.103,21,gye,123321,G11BD_New.gyup,..#
*/ 

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用

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

			 //  关掉 TCP,
			t = AT_CommandWaitRetStatus(eAT_QICLOSE,(uchar *)"0",NULL);	// 断开Socket[eFLAG_TCP]
			if (t == OK)
			{	
				sIP.Online[eFLAG_TCP] = 0;		
				Uart.puts("GPRS CLOSE 1\r\n\0");
			}
			else
			{
				AT_CommandWaitRetStatus(eAT_QICLOSE,(uchar *)"0",NULL);	// 断开Socket[eFLAG_TCP1]  // 再关一次
			}

			UpDateError = 0;
			Uart.puts("FTP Start\r\n\0");
			os_dly_wait(10);

			return;
	  }
	  ////////////开始FTP升级
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
		 GPRS.ProcessFrameData(0,t,(uchar *)tBuffer);			// 反馈到主TCP连接的服务器
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
		  
		  SetBufferValue((uchar *)tBuffer, 38, 0);				  //设置连接的用户名
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
		  
		  SetBufferValue((uchar *)tBuffer, 38, 0);				  //设置连接的密码
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
		  
		  
		  Uart1.puts("AT+QFTPCFG=4,\"/COM/\"\r\n\0");    //设置数据输出到COM口
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

		  SetBufferValue((uchar *)tBuffer, 38, 0);				   //连接FTP服务器
		  strcpy(tBuffer,"AT+QFTPOPEN=\"");
		  strcat(tBuffer,(char *)&UpdatePara.RemoteAddr);
		  //strcat(tBuffer,"\",21\r\n\0");						         //默认FTP端口为21				 固件
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
	
		  Uart1.puts("AT+QFTPPATH=\"/\"\r\n\0");          //设置文件路径		 默认根目录
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
					GPS.BD_UpDateFlag = TRUE;		// 镜像2下载成功，升级北斗模块
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
	函数名称：
   功能描述: 
   入口参数：无
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：	
*/ 

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
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

		k = FtpFileBytes / 4096;	 // 4K一个扇区
		t = FtpFileBytes % 4096;
		if (t) k += 1;
		//DFF.SomeBlockErase(n, 5);
		DFF.SectorEraseWaitOK(n*16 ,k);		  // 一块有16 扇区
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
	SetBufferValue((uchar *)tBuffer, 40, 0);				   //取文件
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
				if(UpdatePara.Frame == IAP_Firmware2)		  // 镜像2存北斗升级文件
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
			if(UpdatePara.Frame == IAP_Firmware2)		  // 镜像2存北斗升级文件
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
	Uart1.puts("+++");					       //退出数据模式
	os_dly_wait(100);
	Uart1.puts("+++");
	os_dly_wait(100);
	Uart1.ClearReceiveFlag(1);
	return flag;
}


/*************************************************************************************************
** 函数名称: GetModelIMEIForPrint()
** 函数功能: 查询IMEI号和SIM卡串号, 以便从串口打印
** 输入参数: 指针*pPS
** 输出参数: 无
** 全局变量: 无
** 创建时间：2009.11.05
** 修改时间: 

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
		Uart1.puts("AT+GSN\r\n\0");					 // 模块IMEI号
		RESET_WATCH_DOG;
		Uart1.WaitRecBuffer(300, 30, (uchar *)tBuffer);	  // 一般IMIE号15位
		RESET_WATCH_DOG;



		pTmp = MyStrStr((char *)tBuffer, (char *)"\r\n\0");
		if (pTmp != NULL)				  // 把回车换行替换成空格
		{
			pTmp2 = MyStrStr((char *)&tBuffer[15], (char *)"\r\0");
			if (pTmp2 != NULL)				  // 把回车换行替换成空格
			{
				Len = pTmp2 - pTmp - 2;
				Uart.puts("IMEI:");
				Uart.putb(1, Len, (uchar *)(pTmp+2));
				//Uart.puts((char *)",");
				// 写入铁电
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
		Uart1.puts("AT+QCCID\r\n\0");			 // SIM卡串号
		RESET_WATCH_DOG;
		os_dly_wait(50);
		RESET_WATCH_DOG;
		Uart1.WaitRecBuffer(500, 30, (uchar *)&tBuffer[0]); // 一般SIM串号20位
		RESET_WATCH_DOG;

  
		pTmp = MyStrStr((char *)tBuffer, (char *)"\r\n\0");
		if (pTmp != NULL)				  // 把回车换行替换成空格
		{
			pTmp2 = MyStrStr((char *)&tBuffer[15], (char *)"\r\0");
			if (pTmp2 != NULL)				  // 把回车换行替换成空格
			{
				Len = pTmp2 - pTmp - 2;
				Uart.puts("CCID:");
				Uart.putb(1, Len, (uchar *)(pTmp+2));

				// 写入铁电
				if (Len > LEN_SIM_CARD_ICCID)	Len = LEN_SIM_CARD_ICCID;
				DFE.Write(EE_SIM_CARD_ICCID, Len, (uchar *)(pTmp+2));
				NOP(); NOP();NOP();NOP();NOP();NOP();
			    //
			}
		}
		
		



		#if (GET_PHONE_NUM_FROM_SIM)
	    // 获取手机号，有些SIM没有存手机号就获取不到   +CNUM: "","13539961594",129, 
		RESET_WATCH_DOG;
		os_dly_wait(300);
		RESET_WATCH_DOG;
		SetBufferValue((uchar *)tBuffer, LEN_GSM_SIM_INFO, 0);
		Uart1.ClearReceiveFlag(1);
		Uart1.puts("AT+CNUM\r\n\0");					 // 手机号
		RESET_WATCH_DOG;
		Len = Uart1.WaitRecBuffer(300, 30, (uchar *)tBuffer);	  // 一般IMIE号15位
		RESET_WATCH_DOG;

		//if (Len > 0) Uart.putb(1, Len, (uchar *)tBuffer);

		pTmp = MyStrStr((char *)tBuffer, (char *)"+CNUM:");
		if (pTmp != NULL)				  // 把回车换行替换成空格
		{
			pTmp += 6;
			pTmp2 = MyStrStr((char *)pTmp, (char *)",");		  //

			if (pTmp2 != NULL)				  // 把回车换行替换成空格
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
	else	  // 从eeprom或铁电读出
	{


	  
	}
}




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：
   功能描述：
   入口参数：
   出口参数：
   全局变量:   
   创建时间：
   修改时间：
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
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
	
	s = AT_CommandWaitRetStatus(eAT_E0,NULL,tBuffer);			//	 关掉回显
	if(s!=OK)		Uart.puts("ER. eAT_E0\r\n\0");
	
	s = AT_CommandWaitRetStatus(eAT_IFC,(uchar *)"2,2",tBuffer);			//		使用硬件流控
	if(s!=OK)		Uart.puts("ER. eAT_IFC\r\n\0");
	
	s = AT_CommandWaitRetStatus(eAT_IFC,(uchar *)"2,2",tBuffer);			//		使用硬件流控
	if(s!=OK)		Uart.puts("ER. eAT_IFC\r\n\0");
		
	s= AT_CommandWaitRetStatus(eAT_CIPHEAD,NULL,tBuffer);			//  使用头信息
	if(s!=OK)	Uart.puts("ER. eAT_CIPHEAD\r\n\0");

	
	s = AT_CommandWaitRetStatus(eAT_QIPROMPT,NULL,tBuffer);			//	设置发送数据不返回'>'
	if(s!=OK)	Uart.puts("ER. eAT_QIPROMPT\r\n\0");

	s = AT_CommandWaitRetStatus(eAT_CMGF0,NULL,tBuffer);	 	// 短信格式PDU
	if(s!=OK)	Uart.puts("ER. eAT_CMFG0\r\n\0");

	//s = AT_CommandWaitRetStatus(eAT_CSCS,NULL,tBuffer);	  		// 选择TE字符集		  默认采用 GSM 字符集
	//if(s!=OK)	Uart.puts("ER. eAT_CSCS\r\n\0");

	s = AT_CommandWaitRetStatus(eAT_CPMS,NULL,tBuffer);	  		// 选择短消息存储器
	if(s!=OK)	Uart.puts("ER. eAT_CPMS\r\n\0");

	//s = AT_CommandWaitRetStatus(eAT_CNMI,NULL,tBuffer);	  		// 短信主动读入
	//if(s!=OK)	Uart.puts("ER. eAT_CNMI\r\n\0");

	s = AT_CommandWaitRetStatus(eAT_CLIP,NULL,tBuffer);	 		// 来电显示
	if(s!=OK)	Uart.puts("ER. eAT_CLIP\r\n\0");

	//s = AT_CommandWaitRetStatus(eAT_CRC,NULL,tBuffer);	 		// 来电未接提示
	//if(s!=OK)	Uart.puts("ER. eAT_CRC\r\n\0");

	s = AT_CommandWaitRetStatus(eAT_CLVL_SET,(char*)"90\0",tBuffer);	// 音量			// 90
	if(s!=OK)	Uart.puts("ER. eAT_CLVL_SET\r\n\0");

	s = AT_CommandWaitRetStatus(eAT_CPIN,NULL,tBuffer);	  	// PIN
	if(s!=OK)	Uart.puts("ER. eAT_CPIN\r\n\0");

	s = AT_CommandWaitRetStatus(eAT_CSQ,NULL,tBuffer);	  	// 信号值
	if(s!=OK)	Uart.puts("ER. eAT_CSQ\r\n\0");

	s = AT_CommandWaitRetStatus(eAT_CREG,NULL,tBuffer);		// 注册GSM状态
	if(s!=OK)	Uart.puts("ER. eAT_CREG\r\n\0");

	s = AT_CommandWaitRetStatus(eAT_CNUM,NULL,tBuffer);		// 手机号
	if(s!=OK)	Uart.puts("ER. eAT_CNUM\r\n\0");

	s = AT_CommandWaitRetStatus(eAT_QCCID,NULL,tBuffer);		// SIM卡串号
	if(s!=OK)	Uart.puts("ER. eAT_QCCID\r\n\0");

	s = AT_CommandWaitRetStatus(eAT_CGSN,NULL,tBuffer);		// 模块IMEI号
	if(s!=OK)	Uart.puts("ER. eAT_CGMR \r\n\0");

	s = AT_CommandWaitRetStatus(eAT_CGMR,NULL,tBuffer);		// 模块软件版本
	if(s!=OK)	Uart.puts("ER. eAT_CGMR\r\n\0");

	s = AT_CommandWaitRetStatus(eAT_QMIC,NULL,tBuffer);		//设置MIC增益
	if(s!=OK)	Uart.puts("ER. eAT_QMIC\r\n\0");

	s = AT_CommandWaitRetStatus(eAT_QIFGCNT,NULL,tBuffer);		// 设置场景
	if(s!=OK)	Uart.puts("ER. eAT_QIFGNT\r\n\0");

	s=ERROR;
	for(i=0;i<10;i++)
	{	//{"1,\"gzjtxx03.gd\"\r\n\0"};  // eAT_QICSGP 	{"AT+QICSGP=1,\"\0"},
		SetBufferValue(tBuffer,64,0);	 	// JT808_IP_DNS.M_APN
		strcpy( (char*)tBuffer,(char*)JT808_IP_DNS.M_APN);
		strcat( (char*)tBuffer,"\"\r\n\0");
		s = AT_CommandWaitRetStatus(eAT_QICSGP,tBuffer,tBuffer);		// 设置APN
		if(s!=OK)	Uart.puts("ER. eAT_QICSGP_APN\r\n\0");
		else			break;
	}

	s = AT_CommandWaitRetStatus(eAT_QIMUX,NULL,tBuffer);		//设置多路TCP/UDP
	if(s!=OK)	Uart.puts("ER. eAT_QIMUX\r\n\0");

	//s = AT_CommandWaitRetStatus(eAT_QIMODE,NULL,tBuffer);		// 设置非透传模式		 默认非透传，不需设
	//if(s!=OK)	Uart.puts("ER. eAT_QIMODE\r\n\0");

   s = AT_CommandWaitRetStatus(eAT_QIREGAPP,NULL,tBuffer);		// 注册TCP/UPD协议栈
	if(s!=OK)	Uart.puts("ER. eAT_QIREGAPP\r\n\0");

	//s = AT_CommandWaitRetStatus(eAT_QIDNSIP,NULL,tBuffer);		// 设置使用IP连接服务器		 默认IP连接
	//if(s!=OK)	Uart.puts("ER. eAT_QIDNSIP\r\n\0");

	s = AT_CommandWaitRetStatus(eAT_QINDI,(uchar *)"1",tBuffer);		// 设置使用缓存接收数据
	if(s!=OK)	Uart.puts("ER. eAT_QINDI\r\n\0");

	s = AT_CommandWaitRetStatus(eAT_QISTAT,NULL,tBuffer);		// 查询GSM连接状态
	if(s!=OK)	Uart.puts("ER. eAT_QISTAT\r\n\0");

    s = AT_CommandWaitRetStatus(eAT_QIACT,NULL,tBuffer);		// 激活GPRS场景
	if(s!=OK)	Uart.puts("ER. eAT_QIACT\r\n\0");

	//os_dly_wait(300);	   
	while(AT_Command_Busing)  { os_dly_wait(1); }
	GSM.GetModelIMEIForPrint('R');		  // 输出IMEI号和SIM卡串号	

	while(AT_Command_Busing)  { os_dly_wait(1); }
	JT808_HANDLE_AUDIO_ON_CONTROL;

	//s = AT_CommandWaitRetStatus(eAT_CMGD_ALL,NULL,tBuffer);			// 删除所有短信
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
	SendInterval.NoDriverTime = 300;				//驾驶员未登录时时间回报间隔	  s
	SendInterval.IDEL_ModeTime = 300;      //休眠时时间回报间隔				s
	SendInterval.AlarmModeTime = 30;			//紧急报警时时间回报间隔			s
	SendInterval.DefauletTime = 30;			//缺省时间回报间隔     s
	SendInterval.DefauletDistance = 1000;	//缺省距离回报间隔     s
	SendInterval.NoDriverDistance = 1000;	//驾驶员未登录时距离回报间隔s
	SendInterval.IDEL_ModeDistance = 500;  //休眠时距离回报间隔s
	SendInterval.AlarmModeDistance = 500;	//紧急报警时距离回报间隔s
	SendInterval.MCA.Byte.Mode = 0 ;              	//位置回报策略 0：定时回报  1：定距回报  2：定时和定距
	SendInterval.MCA.Byte.Condition = 0;				//位置回报方案0 根据ACC状态 1：根据登录状态和ACC状态，先判断登录状态，若登录再根据ACC状态	
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
			strcat((char*)tBuffer,"512\r\n\0");			// 读取的长度必须小于缓冲长度(UART1_REC_BUF_LENGTH)
			Uart1.puts((char*)tBuffer);
			os_dly_wait(20);	
	  
			SetBufferValue(TransparentBuffer, UART1_REC_BUF_LENGTH, 0);
			Len = Uart1.WaitRecBuffer((30*100),UART1_REC_BUF_LENGTH,TransparentBuffer);	  		// 等待返回

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
// 语音播报
uchar		GSMCls::TTS_Opreation(uint16 Len, uchar *pData)
{
		uint16 L;
		uchar tBuffer[128+32];

		if (Len>128)
		{
			return ERROR;
		}
		while(AT_Command_Busing)  { os_dly_wait(1); }

		// 打开用于TTS的语音通道，打开喇叭功放
		Uart1.puts("AT+QAUDCH=2\r\n\0"); 	os_dly_wait(10);
		SPEAKER_POWER_ON
		
		//Uart.puts(" TTS 测试");
		//Uart1.puts("AT+QTTS=2,\"你好\",2\r\n\0");	  // TEST OK
		Uart.puts("语音报播内容: ");
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


