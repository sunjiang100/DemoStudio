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


#include			"ATCmd.h"

#include			"GSM_GPRS.h"
#include			"JT_T808.h"
#include			"ATCmd.h"
#include			"..\Common\GB_T19056.h"
#include			"..\ADC\ADC.h"
#include			"..\GPS\GPS.h"
#include       ".\UserSys.h"

uint32	RetryTimeForListen;					  // 监听重拔时间
uchar		DialForListenCnt;					  // 监听重拔次数
uchar		CallOutSuccess;					  // 主叫成功标志



extern 	uchar		jt_auth_ok_flag;
extern   uint32	count_second;
extern   uint32   count_second_for_send;
extern 	uchar	   jt_recv_flag;
//extern	uint16	set_default_send_time;
extern	uint32	set_pos_send_time;
//extern	uchar		set_retry_time;
extern	uint16	jt_send_cmd_flag;
extern	uchar		jt_pic_send_flag;
extern	uchar		jt_save_pos_flag;
extern	uint32    set_pos_send_distant;			// 设置位置上报距离
extern	uchar	   jt_dis_reg_flag;

extern	char	   jt_auth_code[LEN_AUTH_CODE];
extern   uchar    DialForListenCnt;
extern	uchar		CallOutSuccess;

uint32	gprs_online_time,gprs_noline_time;


__task		void		TaskGPRS_SendPhoneCall(void)
{
	uchar 	s;
	uchar 	CallDir = 0;  // 呼叫方向： 主叫， 1， 被叫： 2
	uchar    CallNow = 0;  // 正在拔入或拔出标志
	uint32	TickCnt=0;
	DialForListenCnt = 0;
	PhoneCall.CenterCall = 0;
	RetryTimeForListen = 0;
	CallOutSuccess = FALSE;							
	NOP();	NOP();
	
	NOP();	NOP();
	while(1)
	{
		
		os_dly_wait(10);
		
		TickCnt++;
		if(PhoneCall.CenterCall)
		{
			if(PhoneCall.CenterCall==TRUE)		
			{
				TickCnt = 0;
				PhoneCall.CenterCall = 0x5a;
			}
			if(TickCnt>(5*60*10) )					
				PhoneCall.CenterCall = 0;
			
		}
		// 精度测试或盲区测试，不处理下面程序		
		if( SamlePeriod1S || TestAccuracy || TestBlind || JT_ShutMobileCnt || TestCanBus )		
		{
			os_dly_wait(90);
			continue;
		}
		
		if( (GG_Status.Flag.Bits.UpdateRemoteSet == 1)||(U0_Iap_Flag!=0) )   continue; // 远程升级
		
		
		
		if(TD.MainTask==tTASK_RUN_FLAG)	
		{
			if(TickCnt%10==0)	Uart.puts("Phone task run\r\n");
		}
					
		if((PhoneCall.Busing==TRUE)&&(PhoneCall.CenterCall==0))				 //(PhoneCall.DialFlag==TRUE)||(PhoneCall.RingFlag==TRUE)||
		{
			VehicleStatus.VS8.Bits.PhoneCalling = 1;

			if(GSM.OneMonthSec!=Rtc.DT.Month)						//判断月份是否有变化
			{
			   GSM.OneMonthSec = Rtc.DT.Month;
				DFE.Write(EE_TIME_MONTH, 4, (uchar *)&GSM.OneMonthSec);
				GSM.CallTimeMonth = 0;	
			}
			if(GSM.CallTimeMonth>JT808_Number.CallTimeMonth)
			{
			   PhoneCall.KeyHang = TRUE;		if( (TD.GPRS == tGSM_PHONE_STATUS))	Uart.puts(" 持机3 ");
				PhoneCall.DialFlag = 0;
				KeyDisp.SendDataToLCM_YH(eLCM_U_CENTER_MSG, 14, (uchar *)&CallOverTimeT[1][0]);		  //月度通话超时
				os_dly_wait(500);
			}
			else //if(PhoneCall.Busing==TRUE)
			  GSM.CallTimeMonth++;

			if(GSM.CallingTimeCnt>JT808_Number.CallTime)
			{
				PhoneCall.KeyHang = TRUE;		  	if( (TD.GPRS == tGSM_PHONE_STATUS))	Uart.puts(" 持机4 ");
				PhoneCall.DialFlag = 0;
				KeyDisp.SendDataToLCM_YH(eLCM_U_CENTER_MSG, 14, (uchar *)&CallOverTimeT[0][0]);		  //本次通话超时
			   os_dly_wait(500);
			}
			else //if(PhoneCall.Busing==TRUE) 
			  GSM.CallingTimeCnt++;
		}

		if (PhoneCall.ListeningFlag == TRUE)			// 监听
		{
			
			if (DialForListenCnt < 3)
			{
			  if((RetryTimeForListen > 30) && (PhoneCall.Dialing != TRUE) )			// 如果未接通30秒后再拔
			  {
			  		DialForListenCnt++;
			  		RetryTimeForListen = 0;
					PhoneCall.DialFlag	= TRUE;		  
			  }
			}
			else
			{
				DialForListenCnt = 0;
				PhoneCall.ListeningFlag = 0;
			}
		}


		if(PhoneCall.DialFlag)				// 主叫
		{
			if (CallNow == 0)
			{
				CallNow = 2;
				SystemLog.GG.CallerTimes++;
			}
			CallDir = 1;
			s = GSM.DialPhoneFun(PhoneCall.Length,PhoneCall.Buffer);
			if(s==OK)	PhoneCall.DialFlag = 0;
			else
			{
				if(GG_Status.Registration)	
				{
					//os_dly_wait(3000);				//拨号未成功，30秒后再打
					//if((++PhoneCall.DialCnt>3) &&  (PhoneCall.ListeningFlag == TRUE))	//3次未成功，停止拨号
					{
						PhoneCall.DialFlag = 0;	
						while(AT_Command_Busing)  { os_dly_wait(1); }
						Uart1.puts("ATH\r\n\0");					// 主动挂机
						KeyDisp.SendDataToLCM_YH(0xa6,0,(uchar*)NULL);		// 挂机后通知手柄
					}
				}
				else
				{
					PhoneCall.DialFlag = 0;
					KeyDisp.SendDataToLCM_YH(0xa6,0,(uchar*)NULL);		// 挂机后通知手柄
					os_dly_wait(100);
					//KeyDisp.SendDataToLCM(eLCM_U_CENTER_MSG,(3*2),(uchar*)"无信号\0");
					KeyDisp.SendDataToLCM_YH(eLCM_U_CENTER_TMP_MSG,(3*2+1),(uchar*)"\x43\x65\xE0\x4F\xE1\x53\xF7\0");	 // 无信号
				}
			}				
		}
		else if(PhoneCall.RingFlag)		// 被叫
		{
			
			if (CallNow == 0)
			{
				CallNow = 1;	
				SystemLog.GG.CalledTimes++;
			}
			CallDir = 2;


		}
		
		if(PhoneCall.CalledAnswer)
		{
			PhoneCall.CalledAnswer = 0;
			PhoneCall.RingCnt = 0;
			PhoneCall.Busing = TRUE;
			PhoneCall.RingFlag = 0;
			
			s = eLCM_U_ANSWER_CALL;
			KeyDisp.SendDataToLCM_YH(0x01,1,&s);	
			
			while(AT_Command_Busing)  { os_dly_wait(1); }
			Uart1.puts("ATA\r\n\0");		// 应答
			Uart.puts("Phone call answer ATA");
		}

		if (CallOutSuccess == TRUE)				// 主叫成功通话
		{
			 SystemLog.GG.CallerTime++;    // 1秒计一次 
		}

		if( ((PhoneCall.KeyHang)||(PhoneCall.GsmHang))	)	//&&(PhoneCall.Busing))
		{
			if(PhoneCall.KeyHang)
			{
				os_dly_wait(100);
				while(AT_Command_Busing)  { os_dly_wait(1); }
				Uart1.puts("ATH\r\n\0");		// 主动挂机
				os_dly_wait(100);
				while(AT_Command_Busing)  { os_dly_wait(1); }
				Uart1.puts("ATH\r\n\0");		// 主动挂机

				Uart.puts("ATH ");
			}
			//KeyDisp.SendDataToLCM_YH(0xa6,0,(uchar*)NULL);		// 挂机后通知手柄
			s = eLCM_U_RING_OFF;
			KeyDisp.SendDataToLCM_YH(0x01,1,&s);			// 挂机后通知手柄
		   
			
			if (CallDir == 2)
			{
				SystemLog.GG.CalledTime += GSM.CallingTimeCnt;
			}


			PhoneCall.KeyHang = 0;
			PhoneCall.GsmHang= 0;
			PhoneCall.DialFlag = 0;
			PhoneCall.RingFlag = 0;
			PhoneCall.Busing = 0;			
			PhoneCall.Answer = 0;
			PhoneCall.CalledAnswer = 0;
			PhoneCall.RingCnt = 0;
			PhoneCall.Dialing = 0; 
			PhoneCall.RingingCnt = 0;	
			JT808_GPRS_TempPack = TRUE;
			GSM.CallingTimeCnt = 0;		
			PhoneCall.RING = 0; 
			SPEAKER_POWER_OFF;
			CallDir = 0;
			CallNow = 0;
			CallOutSuccess = FALSE;
			Uart.puts("TP. Call off");

			//PhoneCall.ListeningFlag = 0;
			DFE.Write(EE_CALL_TIME_MONTH, 4, (uchar *)&GSM.CallTimeMonth);	
			VehicleStatus.VS8.Bits.PhoneCalling = 0;			
		}
		
		
		if((PhoneCall.RingFlag == TRUE)||(PhoneCall.Busing == TRUE)||(PhoneCall.Dialing == TRUE)||(GG_Status.InitStatus==0) ) 
		{
			GSM.PhoneCallStatus();
		}
		
		
		

		if(TD.GPRS == tGSM_PHONE_STATUS )
		{
			if((TickCnt%30)==0)
			{
				Uart.puts("PhoneCall struct Test Data:");
				Uart.putb(1,LEN_PHONE_CALL_STRUCT,(uchar*)&PhoneCall);
				Uart.puts("\r\n");
			}
		}
	}
}





void		GSMCls::ClearPhoneCallFlag(void)
{
	PhoneCall.KeyHang = 0;
	PhoneCall.GsmHang= 0;
	PhoneCall.DialFlag = 0;
	PhoneCall.RingFlag = 0;
	PhoneCall.Busing = 0;			
	PhoneCall.Answer = 0;
	PhoneCall.CalledAnswer = 0;
	PhoneCall.RingCnt = 0;
	PhoneCall.Dialing = 0; 
	PhoneCall.RingingCnt = 0;	
	JT808_GPRS_TempPack = TRUE;
	GSM.CallingTimeCnt = 0;		
	PhoneCall.RING = 0; 
	
	CallOutSuccess = FALSE;
	Uart.puts("Clea phone call flag");

	//PhoneCall.ListeningFlag = 0;
	DFE.Write(EE_CALL_TIME_MONTH, 4, (uchar *)&GSM.CallTimeMonth);	
	VehicleStatus.VS8.Bits.PhoneCalling = 0;	
}	







/*************************************************************************************************************
**	函数名称：PhoneCall()
** 功能描述：拨号
** 入口参数：号码长度len,指向号码的指针pNum
** 出口参数：成功返回0
** 全局变量: 无
** 创建时间：2009.08.31
** 修改时间；
**************************************************************************************************************/
uchar		GSMCls::DialPhoneFun(uchar len,char *pNum)
{
	//uchar		i, s;
	uchar	s;

	char		tBuffer[64], *pS;

	if(PhoneCall.Busing)				return ERROR;		// 	
	
	PhoneCall.Busing = TRUE;
	while(AT_Command_Busing)  { os_dly_wait(1); }
	Uart1.puts((char*)"AT+COLP=1\r\n");
	os_dly_wait(30);
	while(AT_Command_Busing)  { os_dly_wait(1); }
	//Uart1.puts("AT+CLVL=60\r\n\0");
	if(PhoneCall.ListeningFlag)
	{
		Uart1.puts("AT+CLVL=0\r\n\0");
	}
	else
	{
		Uart1.puts("AT+CLVL=90\r\n\0");
	}
	os_dly_wait(30);


	if(PhoneCall.ListeningFlag)
	{
		RetryTimeForListen = 0;	
		while(AT_Command_Busing)  { os_dly_wait(1); }
		JT808_HANDLE_AUDIO_OFF_CONTROL;	
		SPEAKER_POWER_OFF;
		os_dly_wait(30);
		while(AT_Command_Busing)  { os_dly_wait(1); }
		//Uart1.puts("AT+UMGC=1,12,20000\r\n\0");	
		Uart1.puts("AT+QMIC=2,12\r\n\0");	
		os_dly_wait(30);
		Uart.puts("Lis HON\r\n\0");
	}
	else
	{	
		while(AT_Command_Busing)  { os_dly_wait(1); }	  // 设置麦
		JT808_HANDLE_AUDIO_ON_CONTROL;
		SPEAKER_POWER_ON;
		os_dly_wait(30);
		while(AT_Command_Busing)  { os_dly_wait(1); }
		//Uart1.puts("AT+UMGC=0,12,20000\r\n\0");
		Uart1.puts("AT+QMIC=2,12\r\n\0");
		os_dly_wait(30);
		//AT_CommandWaitRetStatus(eAT_QMIC, (uchar *)tBuffer, NULL);
		Uart.puts("Dia HON\r\n\0");
	}

	SetBufferValue((uchar*)tBuffer,64,0);
	while(AT_Command_Busing)  { os_dly_wait(1); }
	strcpy(tBuffer,"ATD\0");
	//strcat(tBuffer,pNum);
	s = strlen(tBuffer);
	memmove(tBuffer+s, pNum, len);
	strcat(tBuffer,";\r\n\0");
	
	Uart.puts(tBuffer);
	if(GG_Status.Registration == TRUE)
	{
		//Uart.puts((char*)tBuffer);	 os_dly_wait(10);	
		//if(AT_CommandWaitRetStatus(eAT_D, (uchar *)tBuffer, NULL)==OK)
		Uart1.ClearReceiveFlag(0);		
		while(AT_Command_Busing)  { os_dly_wait(1); }
		Uart1.puts(tBuffer);
		SetBufferValue((uchar *)tBuffer, 32, 0);  
		os_dly_wait(20);   // add new
		PhoneCall.Dialing = TRUE; 
		
		PhoneCall.DialFlag = 0;
		return OK;
	}
	else
	{		
		Uart.puts("Dial failure.unregister GSM\r\n\0");		
		PhoneCall.Busing = 0;
	}
	
	
 	return NOT_OK;
}


















void			GSMCls::PhoneCallStatus(void)
{
	uint16	Len;
	uchar		tBuffer[256];
	char		*pS,*pOK;
	SetBufferValue(tBuffer,256,0);	//64

	Len = Uart1.GetRecBuffer(tBuffer);
		
		////////////////////////////////////////////////////////	+CMTI: "ME",5\x0D \x0A     	
	if(Len)		//+COLP: "13922280586",129,"",0
	{
		if(TD.GPRS == tGSM_PHONE_STATUS )		Uart.puts((char*)tBuffer);
		if(PhoneCall.Dialing == TRUE) 
		{
			//pOK = MyStrStr((char *)tBuffer,(char*)"OK\0");
			pS = MyStrStr((char *)tBuffer,(char*)"+COLP\0");
			if( (pS != NULL) )
			{
				//KeyDisp.SendDataToLCM_YH(0x04,1,(uchar*)"\x30");		// 摘机后通知手柄
				CallOutSuccess = TRUE;
				if (PhoneCall.ListeningFlag == TRUE)
				{
					PhoneCall.ListeningFlag = 0;
					DialForListenCnt = 0;
				}
				else
				{
					//KeyDisp.SendDataToLCM(0xa7,0,(uchar*)NULL);		      // 摘机后通知手柄
					KeyDisp.SendDataToLCM_YH(0xa7,0,(uchar*)NULL);
				}
				PhoneCall.DialFlag = 0;
				PhoneCall.Dialing = 0;
				PhoneCall.Busing = TRUE;

				//*(pS+1) = 0;
			}
			pS = MyStrStr((char*)tBuffer,(char*)"NO CARRIER\0");		// 连接不成功
			if(pS != NULL)		
			{
				Uart.puts("NoC HON\r\n\0");
				KeyDisp.SendDataToLCM_YH(0xa6,0,(uchar*)NULL);		// 挂机后通知手柄
				PhoneCall.GsmHang = TRUE;
				//ClearPhoneCallFlag();
	
			}
			pS = MyStrStr((char*)tBuffer,(char*)"BUSY\0");		// 线路忙碌
			if(pS != NULL)		
			{
			   Uart.puts("Busy HON\r\n\0");
				KeyDisp.SendDataToLCM_YH(0xa6,0,(uchar*)NULL);		// 挂机后通知手柄
				PhoneCall.GsmHang = TRUE;
				//ClearPhoneCallFlag();
			}
			
			pS = MyStrStr((char*)tBuffer,(char*)"NO ANSWER\0");		// 线路忙碌
			if(pS != NULL)		
			{
			   Uart.puts("No Answer HON\r\n\0");
				KeyDisp.SendDataToLCM_YH(0xa6,0,(uchar*)NULL);		// 挂机后通知手柄
				PhoneCall.GsmHang = TRUE;
				//ClearPhoneCallFlag();
			} 

		}
		else if((PhoneCall.RingFlag==TRUE)||(PhoneCall.Busing == TRUE))
		{
			pS = MyStrStr((char*)tBuffer,(char*)"NO CARRIER\0");		// 连接不成功
			if(pS != NULL)		
			{
				Uart.puts("NoC HON\r\n\0");
				KeyDisp.SendDataToLCM_YH(0xa6,0,(uchar*)NULL);		// 挂机后通知手柄
				PhoneCall.GsmHang = TRUE;
				//*(pS+1) = 0;
			}
			pS = MyStrStr((char*)tBuffer,(char*)"ERROR\0");		// 连接不成功
			if(pS != NULL)		
			{
			   PhoneCall.GsmHang = TRUE;
				//*(pS+1) = 0;
			}

		}
		else
		{
			pS = MyStrStr((char*)tBuffer,(char*)"ATH\r\n\0");		// 挂机
			if(pS != NULL)		
			{
			   Uart.puts("ATH HON\r\n\0");
				//KeyDisp.SendDataToLCM(0xa6,0,(uchar*)NULL);		      // 挂机后通知手柄
				PhoneCall.GsmHang = TRUE;
			}  
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
//static   uint32   gprs_noline_time, gprs_online_time;
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void			GSMCls::eTaskSend(void)
{
	char		*pS;
	uchar		i,s;
	union
	{
		uchar		Buffer[128];
		uint16	B16[64];
		uint32	B32[32];
	}Tp;
	//uchar		*pData;
	static uchar	TCP_IP_Valid = FALSE;
	static uchar 	TCP1_IP_Valid = FALSE;

	uint32   tmp;
	NOP();	NOP();
	TickCnt.GG_Send++;

	TC.GG_Send = 0;
	if(TD.MainTask==tTASK_RUN_FLAG)	
	{
		if(TickCnt.GG_Send%100==0)		Uart.puts("GSM Send Task run\r\n");
	}
	
	if((TickCnt.GG_Send)%6==0)
	{										  //连接临时服务器时间控制
		if( (gCnt.Timer[eTMR_TMP_SERVER]%60==0)&&(JT808_Ctr_IP_DNS.Net_Time!=0))		//分钟单位计数						  &&(GG_Status.Flag.Bits.Online==1)
		{
			  Uart.puts("Net_Time--\r\n\0");	
			  if((--JT808_Ctr_IP_DNS.Net_Time)==0)
			  {
			  	  s = 0;
				  DFE.Write(EE_ENABLE_TCP2,1,(uchar*)&s);
				  JT808_Ctr_IP_DNS.Ctr=1;
				  DFE.Write(EE_JT_NET_CTR ,1,(uchar*)&JT808_Ctr_IP_DNS.Ctr);
			     GG_Status.Flag.Word = GG_FLAG_INIT_DATA;	
				  IC_Card_Auth_GPRS = 0;
			     Uart.puts("GRRS Close As Temp's Server\r\n\0");	
			  }
			  DFE.Write(EE_JT_NET_CTR_NET_TIME, 2, (uchar *)&JT808_Ctr_IP_DNS.Net_Time);
		}
	}
	if(Uart.Rec.Counter && (JT_ShutCommunication == 0))	   return;	

		
	SetBufferValue(Tp.Buffer,64,0);
	/////////////////////////////////////////////
	if(GG_Status.Flag.Bits.SimOK == 0)
	{	// 此标志在下面的函数里修改，这里无需修改
		
		s = AT_CommandWaitRetStatus(eAT_CPIN,NULL,Tp.Buffer);	  	// PIN	 SIM卡状态
		if(s!=OK)
		{
			KeyDisp.SetLED_Status(LED_GPRS,LED_MOBILE_NO_SIM);
			if(++GG_Status.NoSimCardCnt>(15*1))
			{
				#if (WUXI_TEST != 1)
				GG_Status.Signal = 0;
				GG_Status.InitStatus = 0;
				SystemLog.GSM.SimErrorTimes++;
				VehicleStatus.VS5.Bits.SIMcardStatus = 0;
				#else
				SystemLog.GSM.SimErrorTimes++;
				VehicleStatus.VS5.Bits.SIMcardStatus = 0;
				JT_ShutMobileCnt = 1;  // 在串口采集记录仪数据时，如果串口其他数据，影响采集 ，所以找不到卡

				Uart.puts("多次找不到sim卡，关闭GSM模块操作");
				#endif
				
			}
		}
		else
		{
			VehicleStatus.VS5.Bits.SIMcardStatus = 1;
		}

		return;	
	}
	
	/////////////////////////////////////////////
	if(GG_Status.Flag.Bits.SignalOK == 0)
	{	// 此标志在下面的函数里修改，这里无需修改

		AT_CommandWaitRetStatus(eAT_CSQ,NULL,Tp.Buffer);	  	// CSQ	 信号质量
		if(GG_Status.Flag.Bits.SignalOK == 0)
		{
		   if((GG_Status.ATCMDtimes++)>ATResendTimes)	
	      {
				GG_Status.InitStatus = 0;
	            GG_Status.ATCMDtimes = 0;
		   }
		}
		else
		  GG_Status.ATCMDtimes = 0; 
	}
	/////////////////////////////////////////////
	if((GG_Status.Flag.Word&0x001c)==0)
	{	// 此标志在下面的函数里修改，这里无需修改
		AT_CommandWaitRetStatus(eAT_COPS,NULL,Tp.Buffer);	  	// COPS	  检测运营商
		os_dly_wait(100);
		if((GG_Status.Flag.Word&0x001c)==0)
		{
		   if((GG_Status.ATCMDtimes++)>ATResendTimes)	
	      {
				GG_Status.InitStatus = 0;
	            GG_Status.ATCMDtimes = 0;
		   }
		}
		else
		{
		  GG_Status.ATCMDtimes = 0; 
		}
		return;
	}
 
	/////////////////////////////////////////////
	if( (GG_Status.Flag.Bits.RegGSM == 0)&&(GG_Status.Flag.Bits.Roaming==0) )
	{	// 此标志在下面的函数里修改，这里无需修改
		AT_CommandWaitRetStatus(eAT_CREG,NULL,Tp.Buffer);	  	// eAT_CREG	  注册网络状态
		SystemLog.Flag.UnReg = TRUE;
		if((GG_Status.Flag.Bits.RegGSM == 0)&&(GG_Status.Flag.Bits.Roaming==0))
		{
		   if((GG_Status.ATCMDtimes++)>ATResendTimes)	
	      {
				GG_Status.InitStatus = 0;
	         GG_Status.ATCMDtimes = 0;
		   }
	   }
	   else
	     GG_Status.ATCMDtimes = 0; 
		return;
	}

	if(GG_Status.Flag.Bits.SetCASA==0)
	{
		SetBufferValue(Tp.Buffer, 64, 0);
		s = GSM.AT_CommandWaitRetStatus(eAT_CSCA_GET,NULL,Tp.Buffer);	  //读取中心号码
		//Uart.puts("\nCenter Number:");
		//Uart.puts((char *)tBuffer);	
		//os_dly_wait(150);	
										
		if(s==OK)
		{
			GG_Status.Flag.Bits.SetCASA = 1;
			if (strlen((char *)Tp.Buffer) > 10)
			{	
				//Uart.puts("Find Center Number in MT");	os_dly_wait(1);
				GG_Status.Flag.Bits.SetCASA = 1;
				s = AT_CommandWaitRetStatus(eAT_CMGD_ALL,NULL,Tp.Buffer);			// 删除所有短信
				if(s!=OK)  { Uart.puts("Delete all MSG er");   }
				AT_CommandWaitRetStatus(eAT_CIMI,NULL,NULL);	     	// ID	
			}
			else	if (memcmp(sTelNum.SmsCenter, "\0\0\0", 3))  //   存在铁电的短信中心号码有设置，不为0
			{
				//Uart.puts("No find Center Number in MT");	os_dly_wait(1);
				
				SetBufferValue(Tp.Buffer, 64, 0);
				Tp.Buffer[0] = '"';
			   Tp.Buffer[1] = '+';
				strcpy((char*)&Tp.Buffer[2],(char*)sTelNum.SmsCenter);
				strcat((char*)Tp.Buffer,"\"\0");
				//strcat((char*)Tp.Buffer,",129\0"); 
					 
				s = AT_CommandWaitRetStatus(eAT_CSCA_SET,Tp.Buffer,NULL);	  	//
				if(s==OK)
				{
					GG_Status.ATCMDtimes = 0;
					GG_Status.Flag.Bits.SetCASA = 1;
					s = AT_CommandWaitRetStatus(eAT_CMGD_ALL,NULL,Tp.Buffer);			// 删除所有短信
					if(s!=OK)  { Uart.puts("Delete all MSG er");   }
					AT_CommandWaitRetStatus(eAT_CIMI,NULL,NULL);	     	// ID
			   }
				else
				{
					if((GG_Status.ATCMDtimes++)>ATResendTimes)	
			      {
					  GG_Status.InitStatus = 0;
					  GG_Status.ATCMDtimes = 0;
					}
				}
			 }
			 else 		 //  退出设置
			 {
			 	 GG_Status.ATCMDtimes = 0;
				 GG_Status.Flag.Bits.SetCASA = 1;
			 	
			 }
		 }
		 else
		 {
			if((GG_Status.ATCMDtimes++)>ATResendTimes)	
	      {
			  GG_Status.InitStatus = 0;
			  GG_Status.ATCMDtimes = 0;
			}
		 }
		 os_dly_wait(20);		
		
		//return;
	}

	SystemLog.Flag.UnReg = 0;
	GG_Status.NoSimCardCnt = 0;
	KeyDisp.SetLED_Status(LED_GPRS,LED_MOBILE_REG);

	if(TextSMS.SendFlag)		 
	{
		s = SendShortMessage(TextSMS.dNumber,TextSMS.Buffer); 
		if(s==OK)	
		{
		  TextSMS.SendFlag = 0;
		  TextSMS.SendTimes = 0;
		}
		else
		{
		  if((TextSMS.SendTimes--)==0)
		    TextSMS.SendFlag = 0;
		}
	}
	
	if(PDU_SMS.PDU_SendFlag==TRUE)		 
	{
		s = SMS.SendPduMessage(&PDU_SMS, PDU_SMS.TP_UD_Len);
		if(s==OK)	
		{
		  PDU_SMS.PDU_SendFlag = 0;
		  TextSMS.SendTimes = 0;
		}
		else
		{
		  if((PDU_SMS.SendTimes--)==0)
		    PDU_SMS.PDU_SendFlag = 0;
		}
	}

	
	/////////////////////////////////////////////
   if(GG_Status.ConnectFailCnt>10)						  //所有连接不在线连接10次后重新拨号，有一个连接在现在，连接60次后重新拨号
	{
	   if((GG_Status.Flag.Bits.Online==0)||(GG_Status.ConnectFailCnt>60))
		{
			GG_Status.ConnectFailCnt = 0;
			GG_Status.Flag.Word = GG_FLAG_INIT_DATA;	
			
			ReSend.OverTime = ReSend.Times = 0;	  // 应答超时间 10s
			ReSend.PreOverTime = SendOVT.TCP_ACK_OverTime; 

			if(JT808_Ctr_IP_DNS.Ctr==0)		  	// 正在使用临时连接服务器
			{
				
				JT808_Ctr_IP_DNS.Ctr = 1;
				sIP.EnableFlag[eFLAG_TCP] = ENABLE_FLAG;  // 主服务器使能

            s = 0;
	         DFE.Write(EE_ENABLE_TCP2,1,(uchar*)&s);
				DFE.Write(EE_JT_NET_CTR ,1,(uchar*)&JT808_Ctr_IP_DNS.Ctr);
				JT808_Ctr_IP_DNS.Net_Time = 0;
				DFE.Write(EE_JT_NET_CTR ,2,(uchar*)&JT808_Ctr_IP_DNS.Net_Time);
			}

			Uart.puts("GPRS Close at connet over times\r\n\0");
		}
	}
	

	
  //	LPC_UART0->THR = 'a'; LPC_UART0->THR = '6';	 NOP(); NOP();NOP();		 // debug
	#if (1)
	if( (TickCnt.GG_Send%(10*100UL)==0))//&&(GPRS.SendAlig.Length==0) )	 // 每隔10秒钟，查询本地IP
	{
		//if(TD.GPRS == tGPRS_GSM_ALL_DATA)
		{
			s = AT_CommandWaitRetStatus(eAT_QISTAT,NULL,Tp.Buffer);		// 查询状态	 查询GSM状态
			if (s != OK)
			{
				GG_Status.Flag.Bits.Online = 0;
				NetConnectCnt++;
				if (( NetConnectCnt > 3) )
				{
					GG_Status.Flag.Bits.DeAttachGPRS = 1;
					Uart.puts("Can't Find TCP Or UDP Connect\n");
					
					ReSend.OverTime = ReSend.Times = 0;	  // 应答超时间 10s
					ReSend.PreOverTime = SendOVT.TCP_ACK_OverTime; 
				}
				
				if (NetConnectCnt > 6)			  // GPRS已断开， 重启模块
				{
					GG_Status.Flag.Word = GG_FLAG_INIT_DATA;
					GG_Status.InitStatus = 0;
					//GPRS.UnRegTerminal();	 //addBySun
					return;
				}
			}
		}

		#if (0)
		SetBufferValue(tBuffer, 32, 0); 
		s = AT_CommandWaitRetStatus(eAT_QILOCIP,NULL,tBuffer);
		if(s==OK)
		{
			GG_Status.Flag.Bits.LocalIP = 1;
		}
		else
		{	// 无IP，GPRS已断开， 重启模块
			//GG_Status.Flag.Word = GG_FLAG_INIT_DATA;
			if (++GG_Status.InitStatus > 10)
			{
				GG_Status.Flag.Word = GG_FLAG_INIT_DATA;
				GG_Status.InitStatus = 0;	
				
				return;						
			}	
			Uart.puts("GRRS Close As No Local IP\r\n\0");
			//return;
		}
		#endif
	 }
	 #endif

	/////////////////////////////////////////////  断开所有连接
	if(GG_Status.Flag.Bits.DeAttachGPRS == 1)
	{	// 	  	
		for(i=0;i<7;i++)
		{
			sIP.Socket[i] = 0;
			sIP.Online[i] = 0;
		}
		//if(PhoneCall.DialFlag)
		{
			//for(i=0;i<8;i++)	 			// 
			for (i = 0; i < 6; i++)			// M35最多有6路
			{
				Tp.Buffer[0] = Tp.Buffer[1] = Tp.Buffer[2] = 0;
				Int2Str(i,(char*)Tp.Buffer);
				AT_CommandWaitRetStatus(eAT_QICLOSE,Tp.Buffer,NULL);	// 断开Socket 0 ~ 5				
			}
		}
		s = AT_CommandWaitRetStatus(eAT_QIDEACT,NULL,Tp.Buffer);			// 关闭GPRS场景		

		GG_Status.Flag.Bits.Online = 0;
		GG_Status.Flag.Bits.DeAttachGPRS = 0;	
		DomainErCnt = 0;	
		return;
	}

	//////////////////////		 关闭存在的socket 特殊用途 added 2013.4
	if(DisConectSomeSocket == 1)
	{		  	
		// 
		//Uart.puts("HHH1");os_dly_wait(1);
		for (i = 0; i < 6; i++)			// M35最多有6路
		{
			if (sIP.Online[i] != 0)
			{
				//Uart.puts("HHH2");os_dly_wait(1);

				sIP.Socket[i] = 0;
				sIP.Online[i]= 0;

				Tp.Buffer[0] = Tp.Buffer[1] = Tp.Buffer[2] = 0;
				Int2Str(i,(char*)Tp.Buffer);
				CloseSocketStatus =  AT_CommandWaitRetStatus(eAT_QICLOSE,Tp.Buffer,NULL);	// 断开Socket 0 ~ 5	
				if (CloseSocketStatus != OK)
				{
					//Uart.puts("HHH3");os_dly_wait(1);
					CloseSocketStatus = AT_CommandWaitRetStatus(eAT_QICLOSE,Tp.Buffer,NULL);
				}
			}			
		}	

		GG_Status.Flag.Bits.Online = 0;
		DisConectSomeSocket = 0;	
		DomainErCnt = 0;	
		return;
	}
   //

	
		 

	/////////////////////////////////////////////
	if(GG_Status.Flag.Bits.SeviceNet == 0)
	{	// G100模块，如果已操作CMNET，第二次之后的操作均返回ERROR，所以不作返回OK或ERROR判断
		// 返回ERROR表明已加网络服务，不能再加
		if((JT808_Ctr_IP_DNS.Ctr==0)||(JT808_Ctr_IP_DNS.Ctr==2))
		{
			Tp.Buffer[0] = '"';
			strcpy((char*)&Tp.Buffer[1],(char*)&JT808_Ctr_IP_DNS.C_ANP[0]);
			strcat((char*)Tp.Buffer,"\"\0");
		}
		else if((sIP.EnableFlag[eFLAG_APN]==ENABLE_FLAG)&&(sIP.EnableFlag[eFLAG_TCP]==ENABLE_FLAG))
		{
			Tp.Buffer[0] = '"';
			//strcpy((char*)&tBuffer[1],(char*)sIP.APN);
			strcpy((char*)&Tp.Buffer[1],(char*)JT808_IP_DNS.M_APN); 
			strcat((char*)Tp.Buffer,"\"\0");
		}
		else if((sIP.EnableFlag[eFLAG_APN]==ENABLE_FLAG)&&(sIP.EnableFlag[eFLAG_TCP1]==ENABLE_FLAG))
		{
			Tp.Buffer[0] = '"';
			//strcpy((char*)&tBuffer[1],(char*)sIP.APN);
			strcpy((char*)&Tp.Buffer[1],(char*)JT808_IP_DNS.B_APN); 
			strcat((char*)Tp.Buffer,"\"\0");
		}
		else
			strcpy((char*)Tp.Buffer,(char*)"\"CMNET\"\0");

		s = AT_CommandWaitRetStatus(eAT_QICSGP,Tp.Buffer,NULL);		 //运营商网络，国内使用"CMNET"
		
		GG_Status.Flag.Bits.SeviceNet = 1;		
		return;
	}


	if(GG_Status.Flag.Bits.AttachGPRS == 0)
	{	// G100模块，如果已操作CMNET，第二次之后的操作均返回ERROR，所以不作返回OK或ERROR判断
		// 返回ERROR表明已激活状态，不能重新激活
		
		for(i=0;i<7;i++)
		{
			sIP.Socket[i] = 0;
			sIP.Online[i] = 0;
		}
		GG_Status.Flag.Bits.AttachGPRS = 1;

		//gprs_noline_time = Cnt.SystemSecond;
	   //gprs_online_time = Cnt.SystemSecond;				
		//return;
	}
	
	 
	 if( (TickCnt.GG_Send%(15*10UL)==0) )
	 {
	   s = AT_CommandWaitRetStatus(eAT_CPIN,NULL,NULL);	  	// PIN
	 	//if(s!=OK) 
		  return;
	 }
	 if( (TickCnt.GG_Send%(5*10UL)==0) )
	 {
	 	s = AT_CommandWaitRetStatus(eAT_CSQ,NULL,NULL);	  	// 信号值
		//if(s!=OK) 
		 //return;
	 }
	 	
	/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 
		FTP升级处理
	*/
	//if((UpdatePara.Enable)&&(GPRS.SendAlig.Length==0)&&(GG_Status.GPRS_SendStatus==GPRS_SEND_IDLE))
	if((UpdatePara.Enable == ENABLE_FLAG) )// &&(GPRS.SendAlig.Length==0))
	{
		Uart.puts("Begin to update..."); os_dly_wait(100);
		
		 IAP_UpDate();
		 return;
	}
	//GG_Status.Flag.Bits.UpdateOnLine = 0;

	
	//////////////////////////////////
		
	////// // 应付测试，检测中心软件不是判断3分钟，而是三次
	//#if(TEST_CODE_JUST_FOR_TEST_LUOYANG_NEW==1)
	#if(1)
	NOP();	NOP();

	#else			
	//////主服务器连接3分钟不成功切换到备份服务器，在备份服务器连接期间1分钟连接一次主服务器，成功后断开备份服务器
	if((sIP.Online[eFLAG_TCP]!=ENABLE_FLAG)&&(sIP.Online[eFLAG_TCP1]!=ENABLE_FLAG))
	{
		gprs_online_time = gCnt.SystemSecond;
		if(((gCnt.SystemSecond - gprs_noline_time) > 3*60)&& (sIP.EnableFlag[eFLAG_TCP1] != ENABLE_FLAG))	  //3分钟不在线或主服务器被禁用，切换到备份服务器
		{
			// 暂时屏蔽掉备份服务器的链接
		   //sIP.EnableFlag[eFLAG_TCP1] = ENABLE_FLAG;
			//sIP.EnableFlag[eFLAG_TCP] = 0;
			sIP.EnableFlag[eFLAG_TCP] = ENABLE_FLAG;
			sIP.EnableFlag[eFLAG_TCP1] = 0;
			if(((gCnt.SystemSecond - gprs_noline_time) > 5*60))
			   gprs_noline_time = gCnt.SystemSecond - 1*60;		//2分钟后备份服务器还连接不上，切换回连接主服务器连接2分钟，轮流连接
		}
		else //if(sIP.EnableFlag[eFLAG_TCP1]!=ENABLE_FLAG)
		{
			sIP.EnableFlag[eFLAG_TCP] = ENABLE_FLAG;
			sIP.EnableFlag[eFLAG_TCP1] = 0;
		}
	}
	else
	{
		gprs_noline_time = gCnt.SystemSecond;
		if((sIP.Online[eFLAG_TCP]==ENABLE_FLAG)&&(sIP.Online[eFLAG_TCP1]==ENABLE_FLAG))	  //两个服务器同时在线关闭备份服务器，重新连接
		{
		   sIP.EnableFlag[eFLAG_TCP1] = 0;
			//sIP.EnableFlag[eFLAG_UDP1] = 0;
			//GG_Status.Flag.Bits.DomainNameB = 0;
			//GG_Status.Flag.Bits.DomainNameM = 0;
			GG_Status.Flag.Word = GG_FLAG_INIT_DATA;	
			Tp.Buffer[0] = Tp.Buffer[1] = Tp.Buffer[2] = 0;
			Int2Str(sIP.Socket[eFLAG_TCP1],(char*)Tp.Buffer);
			// add by xzb
			s = AT_CommandWaitRetStatus(eAT_QICLOSE,Tp.Buffer,NULL);	// 断开Socket[eFLAG_TCP1]
			if (s == OK)
			{	
				sIP.Online[eFLAG_TCP1] = 0;		
				Uart.puts("GPRS Close 1 Successful\r\n\0");
			}
			else 
			{
				Tp.Buffer[0] = Tp.Buffer[1] = Tp.Buffer[2] = 0;
			    Int2Str(s,(char*)Tp.Buffer);
				Uart.puts("GPRS Close 1 Failure\r\n\0");
				Uart.puts((char *)Tp.Buffer);
			}
			// end add
		}
		else if(((gCnt.SystemSecond - gprs_online_time) > 1*60)&&(sIP.Online[eFLAG_TCP1]==ENABLE_FLAG))  //当前连接的是备份服务器则1分钟连接一次主服务器
		{
		   sIP.EnableFlag[eFLAG_TCP] = ENABLE_FLAG;
			//sIP.EnableFlag[eFLAG_UDP] = ENABLE_FLAG;
			if((gCnt.SystemSecond - gprs_online_time) < 61)
			  GG_Status.Flag.Bits.DomainNameM = 0;
			if((gCnt.SystemSecond - gprs_online_time) > 2*60)		//20秒钟连接不上 重新等待1分钟
			  gprs_online_time = gCnt.SystemSecond;
		}
		else if(sIP.Online[eFLAG_TCP1]==ENABLE_FLAG)
		{
			sIP.EnableFlag[eFLAG_TCP] = 0;
			//sIP.EnableFlag[eFLAG_UDP] = 0;
			//GG_Status.Flag.Bits.DomainNameB = 0;
		}
	
	}
	#endif 
	
	/////////////////////////////////////////////将域名解析为IP地址，为下一步连接域名做准备
	if( ((sIP.EnableFlag[eFLAG_TCP]==ENABLE_FLAG)||(sIP.EnableFlag[eFLAG_UDP]==ENABLE_FLAG))&&
	     (GG_Status.Flag.Bits.DomainNameM==0)&&(JT808_Ctr_IP_DNS.Ctr==1))  //主服务器解析
	{
		//if((GG_Status.Flag.Bits.LocalIP==1)&&(GG_Status.Signal>7))				
		{	
			SetBufferValue(Tp.Buffer,64,0);
			SetBufferValue((uchar *)sIP.TCP, 32, 0);
			strcpy((char*)Tp.Buffer,"0,\"\0");
			DFE.Read(EE_JT_M_IP_DNS,32,(uchar*)&Tp.Buffer[3]);	// 读取域名地址
			s = ChekDNSorIP((uchar*)&Tp.Buffer[3]);
			if(s==ERROR)
			{
			   sIP.EnableFlag[eFLAG_TCP] = 0;
				sIP.EnableFlag[eFLAG_UDP] = 0;
				GG_Status.Flag.Bits.DomainNameM = 1;
			}
			else if(s==IP)
			{
			   strcpy((char*)sIP.TCP, (char*)&Tp.Buffer[3]);                //直接IP地址
			   s=OK;
			}
			else if(s==DNS)
			{
			   SetBufferValue(Tp.Buffer,64,0);
			   strcpy((char*)Tp.Buffer,"\"\0");
			   DFE.Read(EE_JT_M_IP_DNS,32,(uchar*)&Tp.Buffer[1]);
			   strcat((char*)Tp.Buffer,"\"\0");
			   s = AT_CommandWaitRetStatus(eAT_QIDNSGIP,Tp.Buffer,sIP.TCP);	  	// eAT_DNS
			   if (s == OK)		DomainErCnt = 0;

			  ///江苏ip屏蔽
				pS = MyStrStr((char*)sIP.TCP,(char*)"183.207.227\0");		// 解析到到江苏IP 
				if(pS != NULL)
				{
				 strcpy((char*)sIP.TCP, (char*)"58.83.210.131\0");
				// GPRS.UnRegTerminal(); 
				}
				pS = NULL;
			   
			}
			memmove(sIP.UDP, sIP.TCP, 32);
			if(s==OK)
			{
				SetBufferValue(Tp.Buffer,64,0);
				DFE.Read(EE_JT_TCP_PORT,4,(uchar*)&tmp);
				Int2Str(tmp,(char*)&Tp.Buffer[0]);
				strcat((char*)sIP.TCP,":\0");
				strcat((char*)sIP.TCP,(char*)&Tp.Buffer[0]);
	
				SetBufferValue(Tp.Buffer,64,0);
				DFE.Read(EE_JT_UDP_PORT,4,(uchar*)&tmp);
				Int2Str(tmp,(char*)&Tp.Buffer[0]);
				strcat((char*)sIP.UDP,":\0");
				strcat((char*)sIP.UDP,(char*)&Tp.Buffer[0]);
				if(ValidIP_Port(sIP.TCP)==OK)
				{
					GG_Status.Flag.Bits.DomainNameM = 1;
					TCP_IP_Valid = TRUE;
				}
				else
				{
					//sIP.EnableFlag[eFLAG_TCP] = 0;
				    //sIP.EnableFlag[eFLAG_UDP] = 0;
					TCP_IP_Valid = FALSE;
				}		
			}
			else if(++DomainErCnt>5)
			{
				sIP.EnableFlag[eFLAG_TCP] = 0;
				sIP.EnableFlag[eFLAG_UDP] = 0;
				GG_Status.Flag.Bits.DomainNameM = 1;
			}
		}
		os_dly_wait(10);
		if(GG_Status.Flag.Bits.Online==0)
			return;
	}

	if( ((sIP.EnableFlag[eFLAG_TCP1]==ENABLE_FLAG)||(sIP.EnableFlag[eFLAG_UDP1]==ENABLE_FLAG))&&
	     (GG_Status.Flag.Bits.DomainNameB==0)&&(JT808_Ctr_IP_DNS.Ctr==1) )  //备份服务器解析
	{
		SetBufferValue(Tp.Buffer,64,0);
		SetBufferValue((uchar *)sIP.TCP1, 32, 0);
		strcpy((char*)Tp.Buffer,"0,\"\0");
		DFE.Read(EE_JT_B_IP_DNS,32,(uchar*)&Tp.Buffer[3]);	// 读取域名地址
		
		s = ChekDNSorIP((uchar*)&Tp.Buffer[3]);
		if(s==ERROR)
		{
		   sIP.EnableFlag[eFLAG_TCP1] = 0;
			sIP.EnableFlag[eFLAG_UDP1] = 0;
			GG_Status.Flag.Bits.DomainNameB = 1;
		}
		else if(s==IP)
		{
		   strcpy((char*)sIP.TCP1, (char*)&Tp.Buffer[3]);                //直接IP地址
		   s=OK;
		}
		else if(s==DNS)
		{
			strcat((char*)Tp.Buffer,"\"\0");
			s = AT_CommandWaitRetStatus(eAT_QIDNSGIP,Tp.Buffer,sIP.TCP1);	  	// eAT_DNS
			if (s == OK)		DomainErCnt = 0;
		}
		memmove(sIP.UDP1, sIP.TCP1, 32);
		if(s==OK)
		{
			SetBufferValue(Tp.Buffer,64,0);
			DFE.Read(EE_JT_TCP_PORT,4,(uchar*)&tmp);			  // TCP1固定连TCP端口
			Int2Str(tmp,(char*)&Tp.Buffer[0]);
			strcat((char*)sIP.TCP1,":\0");
			strcat((char*)sIP.TCP1,(char*)Tp.Buffer);

			/*SetBufferValue(tBuffer,64,0);
			DFE.Read(EE_JT_UDP_PORT,4,(uchar*)&tmp);			  //测试，TCP1固定连UDP端口
			Int2Str(tmp,(char*)&tBuffer[0]);
			strcat((char*)sIP.UDP1,":\0");
			strcat((char*)sIP.UDP1,(char*)tBuffer);  */
			if(ValidIP_Port(sIP.TCP1)==OK)
			{
				GG_Status.Flag.Bits.DomainNameB = 1;
				TCP1_IP_Valid = TRUE;
			}
			else
			{
				//sIP.EnableFlag[eFLAG_TCP1] = 0;
			   //sIP.EnableFlag[eFLAG_UDP1] = 0;
				TCP1_IP_Valid = FALSE;
			}		
		}
		else if(++DomainErCnt>5)
		{
			sIP.EnableFlag[eFLAG_TCP1] = 0;
			sIP.EnableFlag[eFLAG_UDP1] = 0;
			GG_Status.Flag.Bits.DomainNameB = 1;
		}
		//Uart.putb(1, 32, (uchar *)sIP.TCP1);	 	os_dly_wait(5);
		os_dly_wait(10);
		if(GG_Status.Flag.Bits.Online==0)
			return;
	}


	if( ((sIP.EnableFlag[eFLAG_TCP2]==ENABLE_FLAG)||(sIP.EnableFlag[eFLAG_UDP2]==ENABLE_FLAG))&&
	    ((GG_Status.Flag.Bits.DomainNameC==0) && (JT808_Ctr_IP_DNS.Ctr==0)) )  //临时连接或升级服务器解析
	{
		//Uart.puts("FFF1");os_dly_wait(1);
		SetBufferValue(Tp.Buffer,64,0);
		SetBufferValue((uchar *)sIP.TCP2, 32, 0);
		strcpy((char*)Tp.Buffer,"0,\"\0");
		DFE.Read(EE_JT_NET_CTR_IP_DNS,32,(uchar*)&Tp.Buffer[3]);	// 读取域名地址
		s = ChekDNSorIP((uchar*)&Tp.Buffer[3]);
		if(s==ERROR)
		{
		   goto TURN_TO_MAIN_SERVER;
		}
		if(s==IP)
		{
		   strcpy((char*)sIP.TCP2, (char*)&Tp.Buffer[3]);                //直接IP地址
		   s=OK;
		}
		else if(s==DNS)
		{
		   strcat((char*)Tp.Buffer,"\"\0");
		   s = AT_CommandWaitRetStatus(eAT_QIDNSGIP,Tp.Buffer,sIP.TCP2);	  	// eAT_DNS
		}
		memmove(sIP.UDP2, sIP.TCP2, 32);
		if(s==OK)
		{
			SetBufferValue(Tp.Buffer,64,0);
			tmp = 0;
			DFE.Read(EE_JT_NET_CTR_TCP_PORT,2,(uchar*)&tmp);
			Int2Str(tmp,(char*)&Tp.Buffer[0]);
			strcat((char*)sIP.TCP2,":");
			strcat((char*)sIP.TCP2,(char*)Tp.Buffer);
			SetBufferValue(Tp.Buffer,64,0);
			DFE.Read(EE_JT_NET_CTR_UDP_PORT,2,(uchar*)&tmp);
			Int2Str(tmp,(char*)&Tp.Buffer[0]);
			strcat((char*)sIP.UDP2,":");
			strcat((char*)sIP.UDP2,(char*)Tp.Buffer);
			if(ValidIP_Port(sIP.TCP2)==OK)
			{
				//Uart.puts("FFF2");os_dly_wait(1);
				GG_Status.Flag.Bits.DomainNameC = 1;
			}
			else
			{
				//Uart.puts("FFF3");os_dly_wait(1);
				//sIP.EnableFlag[eFLAG_TCP2] = 0;
			   //sIP.EnableFlag[eFLAG_UDP2] = 0;
				goto TURN_TO_MAIN_SERVER;
			}		
		}
		else if(++DomainErCnt>5)
		{
			
TURN_TO_MAIN_SERVER:
			JT808_Ctr_IP_DNS.Ctr = 1;
			sIP.EnableFlag[eFLAG_TCP] = ENABLE_FLAG;  // 切换到主服务器使能

         s = 0;
         DFE.Write(EE_ENABLE_TCP2,1,(uchar*)&s);
			DFE.Write(EE_JT_NET_CTR ,1,(uchar*)&JT808_Ctr_IP_DNS.Ctr);
			JT808_Ctr_IP_DNS.Net_Time = 0;
			DFE.Write(EE_JT_NET_CTR_NET_TIME ,2,(uchar*)&JT808_Ctr_IP_DNS.Net_Time);
			return;
		}
		//Uart.putb(1, 32, (uchar *)sIP.TCP2);		os_dly_wait(5);
		os_dly_wait(10);

		if(GG_Status.Flag.Bits.Online==0)
			return;
	}

	
	///////////////////////////////////////////////////////////////////		GPRS连接	
	if((sIP.EnableFlag[eFLAG_TCP]==ENABLE_FLAG)&&(JT808_Ctr_IP_DNS.Ctr==1) && (TCP_IP_Valid == TRUE))
	{
		if(sIP.Online[eFLAG_TCP]!=ENABLE_FLAG)
		{
			s = GPRS_Connect(eAT_QIOPEN_TCP,sIP.TCP,eFLAG_TCP);
			if (s == OK)
			{
				sIP.Socket[eFLAG_TCP] = eFLAG_TCP;
				sIP.Online[eFLAG_TCP]= ENABLE_FLAG;
				GG_Status.Flag.Bits.Online = 1;
				GG_Status.ConnectFailCnt = 0;
				GG_Status.DialFailCnt[eFLAG_TCP] = 0;
				Uart.puts("Dial TCP OK\r\n");
				GG_Status.SendTotalBytes = 0;
				
				if(NeedRegServer == ENABLE_FLAG)	
					TempNeedRegServer = TRUE;		// 需重新注册
				jt_auth_ok_flag = 0;         		// 需重新鉴权				
				DialOK_NeedAuthFlag = TRUE;		// 
				IC_Card_Auth_GPRS = 0;				// 
				ReSend.OverTime = ReSend.Times = 0;	  // 应答超时间 10s
				ReSend.PreOverTime = SendOVT.TCP_ACK_OverTime;  				
			}
			else
			{
				
		  		GG_Status.ConnectFailCnt++;
				GG_Status.DialFailCnt[eFLAG_TCP]++;
		  		Uart.puts("Dial TCP Failure\r\n");

				Tp.Buffer[0] = Tp.Buffer[1] = Tp.Buffer[2] = 0;
				Tp.Buffer[0] = eFLAG_TCP + 0x30; // '0'
				//AT_CommandWaitRetStatus(eAT_QICLOSE,tBuffer,NULL);
				if (OK != AT_CommandWaitRetStatus(eAT_QICLOSE,Tp.Buffer,NULL))
				{
					AT_CommandWaitRetStatus(eAT_QICLOSE,Tp.Buffer,NULL);	
				}
			}	 	 
			os_dly_wait(50);
			
		}
	}
	
	if((sIP.EnableFlag[eFLAG_UDP]==ENABLE_FLAG)&&(JT808_Ctr_IP_DNS.Ctr==1)&&(sIP.Online[eFLAG_TCP]== ENABLE_FLAG)) 
	{
		if(sIP.Online[eFLAG_UDP]!=ENABLE_FLAG)
		{
			s = GPRS_Connect(eAT_QIOPEN_UDP,sIP.UDP,eFLAG_UDP);
			if (s == OK)
			{
				sIP.Socket[eFLAG_UDP] = eFLAG_UDP;
				sIP.Online[eFLAG_UDP]= ENABLE_FLAG;
				//GG_Status.Flag.Bits.Online = 1;
			   GG_Status.ConnectFailCnt = 0;	
				GG_Status.DialFailCnt[eFLAG_UDP] = 0;
			}
			else
			{
			  //GG_Status.ConnectFailCnt++;				 //服务器以TCP为主，只要TCP连接成功UDP可以不断的连接直到连接成功为止，不影响其他
			  //GG_Status.DialFailCnt[eFLAG_UDP]++;
			} 

			Uart.puts("Dial UDP..\r\n");	 	 
			os_dly_wait(20);
		}
	}

	if((sIP.EnableFlag[eFLAG_TCP1]==ENABLE_FLAG)&&(JT808_Ctr_IP_DNS.Ctr==1) && (TCP1_IP_Valid == TRUE)) 
	{
		if( (sIP.Online[eFLAG_TCP]!=ENABLE_FLAG)&&(sIP.Online[eFLAG_TCP1]!=ENABLE_FLAG) )
		{	// 
			s = GPRS_Connect(eAT_QIOPEN_TCP,sIP.TCP1, eFLAG_TCP1);
			if (s == OK)
			{
			  	sIP.Socket[eFLAG_TCP1] = eFLAG_TCP1;
				sIP.Online[eFLAG_TCP1]= ENABLE_FLAG;
				GG_Status.Flag.Bits.Online = 1;
			    GG_Status.ConnectFailCnt = 0;
				 GG_Status.DialFailCnt[eFLAG_TCP1] = 0;
				Uart.puts("Dial TCP1 OK\r\n");
				jt_auth_ok_flag = 0;
				ReSend.OverTime = ReSend.Times = 0;	  // 应答超时间 10s
				ReSend.PreOverTime = SendOVT.TCP_ACK_OverTime;  
				
			}
			else
			{
				
			  	GG_Status.ConnectFailCnt++;
				GG_Status.DialFailCnt[eFLAG_TCP1]++;
			  	Uart.puts("Dial TCP1 Failure\r\n");	
				Tp.Buffer[0] = Tp.Buffer[1] = Tp.Buffer[2] = 0;
				Tp.Buffer[0] = eFLAG_TCP1 + 0x30;;
				//AT_CommandWaitRetStatus(eAT_QICLOSE,tBuffer,NULL);
				if (OK != AT_CommandWaitRetStatus(eAT_QICLOSE,Tp.Buffer,NULL))
				{
					AT_CommandWaitRetStatus(eAT_QICLOSE,Tp.Buffer,NULL);	
				}
			}
			os_dly_wait(50);
		}
	}
	if((sIP.EnableFlag[eFLAG_UDP1]==ENABLE_FLAG)&&(JT808_Ctr_IP_DNS.Ctr==1)&&(sIP.Online[eFLAG_TCP1]== ENABLE_FLAG)) 
	{
		if(sIP.Online[eFLAG_UDP1]!=ENABLE_FLAG)
		{
			s = GPRS_Connect(eAT_QIOPEN_UDP,sIP.UDP1,eFLAG_UDP1);
			if (s == OK)
			{
				sIP.Socket[eFLAG_UDP1] = eFLAG_UDP1;
				sIP.Online[eFLAG_UDP1]= ENABLE_FLAG;
				//GG_Status.Flag.Bits.Online = 1;
			   GG_Status.ConnectFailCnt = 0;	
				GG_Status.DialFailCnt[eFLAG_UDP1] = 0;
			}
			else
			{
				 //GG_Status.ConnectFailCnt++;
				 //GG_Status.DialFailCnt[eFLAG_UDP1]++;
			}
			Uart.puts("Dial UDP1..\r\n");	 	 
			os_dly_wait(50);
		}
	}
	
	if((sIP.EnableFlag[eFLAG_TCP2]==ENABLE_FLAG)&&((JT808_Ctr_IP_DNS.Ctr==0))) 							 //||(JT808_Ctr_IP_DNS.Ctr==2)
	{
		if( (sIP.Online[eFLAG_TCP]!=ENABLE_FLAG)&&(sIP.Online[eFLAG_TCP1]!=ENABLE_FLAG)&&(sIP.Online[eFLAG_TCP2]!=ENABLE_FLAG) )
		{
			//Uart.puts("FFF4"); os_dly_wait(1);
			s = GPRS_Connect(eAT_QIOPEN_TCP,sIP.TCP2,eFLAG_TCP2);
			if (s == OK)
		 	{
				//Uart.puts("FFF5"); os_dly_wait(1);

				sIP.Socket[eFLAG_TCP2] = eFLAG_TCP2;
				sIP.Online[eFLAG_TCP2]= ENABLE_FLAG;
				GG_Status.Flag.Bits.Online = 1;
			    GG_Status.ConnectFailCnt = 0;
				 GG_Status.DialFailCnt[eFLAG_TCP2] = 0;
				Uart.puts("Dial TCP2 OK\r\n");
				jt_auth_ok_flag = 0;
				ReSend.OverTime = ReSend.Times = 0;	  // 应答超时间 10s
				ReSend.PreOverTime = SendOVT.TCP_ACK_OverTime;  
				
			}
			else
			{
				Uart.puts("Dial TCP2 Failure\r\n");	
				GG_Status.ConnectFailCnt++;
				GG_Status.DialFailCnt[eFLAG_TCP2]++;
				Tp.Buffer[0] = Tp.Buffer[1] = Tp.Buffer[2] = 0;
				Tp.Buffer[0] = eFLAG_TCP2 + 0x30;
				if (OK != AT_CommandWaitRetStatus(eAT_QICLOSE,Tp.Buffer,NULL))
				{
					AT_CommandWaitRetStatus(eAT_QICLOSE,Tp.Buffer,NULL);	
				}
				
			}
			os_dly_wait(50);
		}		
	}

	if((sIP.EnableFlag[eFLAG_UDP2]==ENABLE_FLAG)&&((JT808_Ctr_IP_DNS.Ctr==0))) 							//||(JT808_Ctr_IP_DNS.Ctr==2)	  
	{
		if(sIP.Online[eFLAG_UDP2]!=ENABLE_FLAG)
		{
			s = GPRS_Connect(eAT_QIOPEN_UDP,sIP.UDP2,eFLAG_UDP2);
			if (s == OK)
			{
				sIP.Socket[eFLAG_UDP2] = eFLAG_UDP2;
				sIP.Online[eFLAG_UDP2]= ENABLE_FLAG;
				//GG_Status.Flag.Bits.Online = 1;
			   GG_Status.ConnectFailCnt = 0;	
				GG_Status.DialFailCnt[eFLAG_UDP2] = 0;
			}
			else
			{
				 //GG_Status.ConnectFailCnt++;
				 //GG_Status.DialFailCnt[eFLAG_UDP2]++;
			}
			Uart.puts("Dial UDP2..\r\n");	 	 
			os_dly_wait(50);

		}		
	}

	// add by xzb 
	for(s=0;s<TOTAL_IP_NUMBER;s++)
	{	
		if(sIP.Online[s]==ENABLE_FLAG)
		{
			//AvailableSocketNumber =	 sIP.Socket[s];	    // 记录当前可用socket通道
			VehicleStatus.VS5.Bits.GPRSonlineStatus = 1;
			break;
		}
	}
	if (s == TOTAL_IP_NUMBER)
	{
		GG_Status.Flag.Bits.Online = 0;
		VehicleStatus.VS5.Bits.GPRSonlineStatus = 0;
	}
		 

	if(GG_Status.Flag.Bits.Online==0)		return;
	
	KeyDisp.SetLED_Status(LED_GPRS,LED_GPRS_ON_LINE);

	SystemLog.Flag.Online = TRUE;
	GG_Status.DialErCnt = 0;
	

	if(ReSend.OverTime)
	{
		if(TickCnt.GG_Send%100==0)	
		{
			SetBufferValue(Tp.Buffer,32,0);
			strcpy((char*)Tp.Buffer,"发送超时倒计 ");
			Int2Str( ReSend.OverTime, (char*)&Tp.Buffer[14]);
			if (TD.GPRS==tGPRS_TMP_TEST) { Uart.puts((char*)Tp.Buffer);	  }
		}		
	}
	
	if(TD.GPRS == tGPRS_TMP_TEST )
	{
		if(TickCnt.GG_Send%50==0)	
		{
			SetBufferValue(Tp.Buffer,128,0);
			strcpy((char*)Tp.Buffer,"发送超时倒计 ");
			Int2Str( ReSend.OverTime, (char*)&Tp.Buffer[14]);	 		// 超时计数器
			strcat( (char*)Tp.Buffer," ");
			i = strlen( (char*)Tp.Buffer );
			Int2Str( ReSend.Times,  (char*)&Tp.Buffer[i]);	 			// 第N次
			strcat( (char*)Tp.Buffer," ");
			i = strlen( (char*)Tp.Buffer );		 
			Int2Str( ReSend.PreOverTime,  (char*)&Tp.Buffer[i]);		// 前次允许超时值
			if(IC_Card_Auth_GPRS==TRUE)	strcat( (char*)Tp.Buffer," IC卡占用GPRS通道");
			else									strcat( (char*)Tp.Buffer," IC卡未占用GPRS通道");
			if(jt_auth_ok_flag)				strcat( (char*)Tp.Buffer," 已鉴权");
			else									strcat( (char*)Tp.Buffer," 未鉴权");
			if(DialOK_NeedAuthFlag)			strcat( (char*)Tp.Buffer," 需重新鉴权");
			else									strcat( (char*)Tp.Buffer," 无需再鉴权");

			Uart.puts((char*)Tp.Buffer);
		}
	}
	
	
	
	/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
		发送GPRS数据
	*/
	s = GPRS.PopAlignmentSend(GG_Status.Signal);
	if(s!=OK)
	{
		if(++GG_Status.SendDataErCnt>5)
		{
			VehicleStatus.VS1.Bits.GPRS_Delay = 1;
			GG_Status.Flag.Word = GG_FLAG_INIT_DATA;	 	// 重新初始化
			os_dly_wait(150);

			Uart1.puts("+++");
			os_dly_wait(2);
			Uart.puts("连续发送五次失败，重新链接 ");os_dly_wait(1);
		}
	}
	else
	{
		VehicleStatus.VS1.Bits.GPRS_Delay = 0;
		GG_Status.SendDataErCnt = 0;
	}
	
			 

} // End of <GSMCls::RSend> 





void    GSMCls::CalculateSendInterval(uint32 tInterval)
{	
	NOP();	NOP();
	if( (tInterval>9)&&(tInterval<0xffffffff) )
		SendInterval.DefauletTime = tInterval;
	if((SendInterval.MCA.Byte.Mode==0)||(SendInterval.MCA.Byte.Mode==2))		//定时上报
	{
		SendInterval.Second = SendInterval.DefauletTime;
		if(JT808Flag.VS5.Bits.EmergencyAlarm==1)		//紧急报警时
		{ 
		  SendInterval.Second = SendInterval.AlarmModeTime;
		}
		else if(SendInterval.MCA.Byte.Condition==0)  // 根据ACC 即休眠   
		{
		   if(JT808Flag.VS1.Bits.ACC==0)		//ACC
			   SendInterval.Second = SendInterval.IDEL_ModeTime;
		}
		else if(SendInterval.MCA.Byte.Condition==1)		//根据驾驶员登录和ACC
		{
			if(DriverCardFlag==0)  // 根据ACC 即休眠  驾驶员未登录
		   {
			   SendInterval.Second	= SendInterval.NoDriverTime;
			}
			else if(DriverCardFlag==TRUE)						 //驾驶员登录则根据ACC
			{
			   if(JT808Flag.VS1.Bits.ACC==0)		//ACC关
			     SendInterval.Second = SendInterval.IDEL_ModeTime;
			}
		}
	}
	else
	{
	  SendInterval.Second =  0xffffffff;
	}
	if((SendInterval.MCA.Byte.Mode==1)||(SendInterval.MCA.Byte.Mode==2))		//定距上报
	{
	   SendInterval.Distance = SendInterval.DefauletDistance;
		if(JT808Flag.VS5.Bits.EmergencyAlarm==1)	  //紧急报警时
		{ 
		  SendInterval.Distance = SendInterval.AlarmModeDistance;
		}
		else if(SendInterval.MCA.Byte.Condition==0)  // 根据ACC 即休眠   
		{
		   if(JT808Flag.VS1.Bits.ACC==0)		//ACC关
			   SendInterval.Distance = SendInterval.IDEL_ModeDistance;
		}
		else if(SendInterval.MCA.Byte.Condition==1)		//根据驾驶员登录和ACC
		{
			if(DriverCardFlag==0)  // 根据ACC 即休眠  驾驶员未登录
		   {
			   SendInterval.Distance	= SendInterval.NoDriverDistance;
			}
			else if(DriverCardFlag==TRUE)						 //驾驶员登录则根据ACC
			{
			   if(JT808Flag.VS1.Bits.ACC==0)		//ACC关
			     SendInterval.Distance = SendInterval.IDEL_ModeDistance;
			}
		}
	}
	else
	{
		SendInterval.Distance = 0xffffffff;
	}	
	//拐角补传
	if(((GPS.JTPack.Azimuth - LastAzimuth)>SendInterval.MCA.Byte.Angle)&&(JT808Flag.VS1.Bits.ACC==1)
		 &&((GPS.JTPack.Azimuth - LastAzimuth)<180))	    		 
	{
	  if((LastAzimuthCnt++)>20);			  //延时4秒确认真正产生拐角
	  {
	    JT808_GPRS_TempPack = TRUE;
		 LastAzimuth = GPS.JTPack.Azimuth;
	  }
	} 
	else
	  LastAzimuthCnt = 0; 	


	if(SendInterval.Second==0)		 	SendInterval.Second = 60;
	if(SendInterval.Distance==0)		SendInterval.Distance = 1000;

}




void			GSMCls::SetDefaultSendInterval(uint32 tInterval)
{
	SendOVT.TCP_ACK_OverTime		= 10;
	SendOVT.TCP_ResendTimes       = 3;
	SendOVT.UDP_ACK_OverTime      = 10;
	SendOVT.UDP_ResendTimes       = 3;
	SendOVT.SMS_ACK_OverTime      = 20;
	SendOVT.SMS_ResendTimes       = 3;
	DFE.Write(EE_JT_RESEND_MACHINING,LEN_SEND_OVT_STRU,(uchar *)&SendOVT);
	
	SendInterval.HeartbeatTime			= 60;
	SendInterval.NoDriverTime			= 30;
	SendInterval.IDEL_ModeTime       = 300;
	SendInterval.AlarmModeTime		   = 30;
	SendInterval.DefauletTime			= tInterval;
	SendInterval.DefauletDistance	   = 1000;
	SendInterval.NoDriverDistance	   = 1000;
	SendInterval.IDEL_ModeDistance   = 1000;
	SendInterval.AlarmModeDistance   = 1000;
	
	if(SendInterval.DefauletTime==0)	SendInterval.DefauletTime = 30;
	
	SendInterval.MCA.B32 = 0;
	
	SendInterval.T_Second				= 0;	  		
	SendInterval.T_ContinuedTime		= 0;
	SendInterval.Second					= 30;
	SendInterval.Distance				= 1000;
	DFE.Write(EE_SEND_INTERVAL_BASE,LEN_SEND_INTERVAL_STRU,(uchar *)&SendInterval);
	
	GSM.CalculateSendInterval(0);
}






/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */


