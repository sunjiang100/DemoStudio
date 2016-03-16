/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												 
$										 
$                       	 		  
$								  		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      �ļ�����
	�ļ���: 	 GPRS.c
	����������GRPS��غ���
	���뻷����
	Ŀ��CPU: 
	���ߣ�
	����ʱ�䣺
	����༭������UltraEdit 11.00a+  �Ʊ�����Ϊ�����ַ�������ɶ��Ը��á�
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/					


/******************************** End of File ***************************************************************/

#include			<stdlib.h>
#include			<string.h>
#include			"..\GPS\GPS.h"
#include			"GSM_GPRS.h"  
#include			"ATCmd.h"
#include			"..\Common\FLASH_ADDR.H"
#include			"..\ADC\ADC.h"
#include			"..\Config\RTOS.h"
#include			"..\UART\UART1.h"
#include			"..\UART\UART0.h"
#include			"..\Switch\switch.h"
#include			"..\Common\Algol.h"
#include			"..\camera\camera.h"

#include			"..\KeyDisp\KeyDisp.h"
#include			"..\FLASH\DFE_25xxx.H"
#include			"..\FLASH\DFF_25xxx.H"
#include			"..\PWM\PWM.h"
#include			"JT_T808.h"
#include			"..\UserSys.h"
#include			"..\Common\GB_T19056.h"
#include 		<math.h>










/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�GetCommaPosition
   �����������ҳ��ַ������Զ���Ϊ�ָ�����ÿ�����ݵĿ�ʼλ�úͳ���
   ��ڲ�����EndChar:��������pChar:�ַ�����pComma:����λ��
   ���ڲ������ܹ��ж��������ݣ������ٸ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
ע����ڲ����У����ű�������ĳ���Ҫ����ÿ�����ݵ�λ��=����λ��+1
pComma�Ľṹ��  ��ʼλ�ã����ȸ�1�ֽڣ��������һά���顣(���ö�ά�����ʾ�������Ļ�ʱ)
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
uchar			GPRSCls::GetCommaPositionAndDataLen(uchar EndChar,uchar	*pChar,uchar *pComma)
{
	uchar		c,Number,ZeroCnt;
	uint16	i,j,Pos,Len;

	////////////////////////////////////////////////////////
	i = 0;	j = 1;	Pos = 0;		
	ZeroCnt = 0; Number = 0;
	*pComma = 0;		// ��һ�����ݵĿ�ʼλ�ú�Ϊ0	
	c = *pChar;
	for(Len=0;Len<LEN_ONE_FRAME;Len++)
	{
		if((c == ',')||(c == EndChar))
		{
			* (pComma+j) = i-Pos;		// ����
			j++;
			Pos = i+1;						// ��һ�����ݵĿ�ʼλ��
			 * (pComma+j) = Pos;
			j++;
			Number++;
			/////////////////
			if(c==EndChar)
				return Number;			
		}
		i++;
		c = *(pChar+i);
		/////////////////////////////////// ���������,ֱ�ӽ���	  �ո�
		if(c)	ZeroCnt = 0;
		else	ZeroCnt++;
		//if(ZeroCnt>4)
		if(ZeroCnt>10)			   // ʹ�ò���ָ��ʱ��Ϊ0�������10�� 
		{
			*(pChar+Len-4) = '#';
			return Number;
		}
	}	
	return Number;
}













/**************************************************************************************************
** �������ƣ�LocationQuery()		D1
** ��������������λ�÷��ͼ��(ȱʡ�㱨�����߻㱨)		  
** ��ڲ�����
** ���ڲ�����
** ȫ�ֱ���: 
** �޸�ʱ�䣺
// *GY,000,D1,12356,60,300# 
**************************************************************************************************/ 
void		GPRSCls::LocationQuery(uchar *pPS)		 // ������ "D1"
{
	uchar		buf[16];	
	uint32	tmp,tmp1;
	pGPS_SEND	pSd;
	
	tmp = My_atol((char*)&OneFrame[*(pPS+(4<<1))]);
	tmp1 = My_atol((char*)&OneFrame[*(pPS+(5<<1))]);
	if((tmp==0)||(tmp>65535)||(tmp1==0)||(tmp1>65535))
	{
		AckBuffer.Error = TRUE;
		return ;
	}	

	SendInterval.DefauletTime = tmp;
	DFE.Write(EE_JT_SEND_TIME_DEFAULET, 4,(uchar *)&SendInterval.DefauletTime);
	SendInterval.IDEL_ModeTime = tmp1;
	DFE.Write(EE_JT_SEND_TIME_IDEL, 4,(uchar *)&SendInterval.IDEL_ModeTime);
                              

//	pSd = (pGPS_SEND)&buf;
//	GPS.GetSendConfig(pSd);
//	//pSd-> Interval = My_atol((char*)&OneFrame[*(pPS+(4<<1))]);
//	//pSd-> StopInterval = My_atol((char*)&OneFrame[*(pPS+(4<<1))]);
//	pSd-> Str.Times = 0xffffffff;
//   pSd-> Str.T_Interval = My_atol((char*)&OneFrame[*(pPS+(4<<1))]);
//	pSd->Str.T_Time	= My_atol((char*)&OneFrame[*(pPS+(5<<1))]);
//	pSd->Str.Distance = 1000;
//	pSd-> Str.Type = 0x55;		
//	GPS.SetSendConfig(pSd,1);
//	GPRS.ClearAligBuffer();						// �巢�ͻ�����,������ǰδ���͵�D1��������(��Ϊ��ʵʱ��λ)
	
	AckBuffer.GPS = TRUE;
}


/*************************************************************************************************
** ��������: GetSystemLog()
** ��������:  ��ѯ��־ G1 
** �������: ָ��*pPS
** �������: ��
** ȫ�ֱ���: ��
** ����ʱ�䣺2009.12.05
** �޸�ʱ��: 
************************************************************************************************/
#define		LEN_SYS_LOG_PER_GROUP		32					 // ÿ��32���ֽ�
void		GPRSCls::GetSystemLog(uchar *pPS)
{
	uchar		Type;
	uint32	Len = 0;
	


	Type = OneFrame[PSC[4<<1]];		// ȡ����
	if (Type == 'A')						// �Զ��ϱ�ȫ����־
	{
	  AckBuffer.Data[0] = Type; 
	  AckBuffer.Data[1] = ',';
	  memmove(&AckBuffer.Data[2], (uchar *)&SystemLog, LEN_SYSTEM_LOG);
	  Len = LEN_SYSTEM_LOG;
	}
	else if ((Type >= '1') && (Type <= '8'))			 // 8����������һ��
	{
		AckBuffer.Data[0] = Type; 
		AckBuffer.Data[1] = ',';
		Type -= '1';
		memmove(&AckBuffer.Data[2], (uchar *)&SystemLog + Type * 32, LEN_SYS_LOG_PER_GROUP);
	  	Len = LEN_SYS_LOG_PER_GROUP;
	}
	else
	{
		////////// Ӧ�� /////////////////////////////
		SetBufferValue((uchar*)AckBuffer.Data,128,0);
		/////////////////////////////////////////////////4
		strcpy(AckBuffer.Data,"Rst,");
		Int32ToDecChar(SystemLog.ResetCnt.SystemRestartCnt,&AckBuffer.Data[4]);
	
	
		strcat(AckBuffer.Data,",GPS,");
		Len = strlen(AckBuffer.Data);
		/////////////////////////////////////////////////4
		Int32ToDecChar(SystemLog.GPS.PackTotalNumber,&AckBuffer.Data[Len]);
		strcat(AckBuffer.Data,",");
		Len = strlen(AckBuffer.Data);
		/////////////////////////////////////////////////4
		Int32ToDecChar(SystemLog.GPS.PackValidNumber,&AckBuffer.Data[Len]);
		strcat(AckBuffer.Data,",");
		Len = strlen(AckBuffer.Data);
		/////////////////////////////////////////////////4
		//Int32ToDecChar(SystemLog.GPS.PackInvalidNumber,&AckBuffer.Data[Len]);
		Int32ToDecChar(SystemLog.GPS.RestartCnt,&AckBuffer.Data[Len]);
		strcat(AckBuffer.Data,",");
		Len = strlen(AckBuffer.Data);
		/////////////////////////////////////////////////4
		//Int32ToDecChar(SystemLog.GPS.PackTotalBytes,&AckBuffer.Data[Len]);
		Int32ToDecChar(SystemLog.GPS.ValidTime,&AckBuffer.Data[Len]);
		strcat(AckBuffer.Data,",");
		Len = strlen(AckBuffer.Data);
		/////////////////////////////////////////////////4
		Int2Str(SystemLog.GPS.DataErrorCnt,&AckBuffer.Data[Len]);
		strcat(AckBuffer.Data,",");
		Len = strlen(AckBuffer.Data);
		/////////////////////////////////////////////////4
			
	
		strcat(AckBuffer.Data,",GPRS,");
		Len = strlen(AckBuffer.Data);
	//	////////////////////////////////////////////////4
	//	Int32ToDecChar(SystemLog.GPRS.SendTotalBytes,&AckBuffer.Data[Len]);
	//	strcat(AckBuffer.Data,",");
	//	Len = strlen(AckBuffer.Data);
		/////////////////////////////////////////////////2
		Int32ToDecChar(SystemLog.GPRS.RecTotalPack,&AckBuffer.Data[Len]);
		strcat(AckBuffer.Data,",");
		Len = strlen(AckBuffer.Data);
		/////////////////////////////////////////////////4
		Int32ToDecChar(SystemLog.GPRS.RecTotalBytes,&AckBuffer.Data[Len]);
		strcat(AckBuffer.Data,",");
		Len = strlen(AckBuffer.Data);
		//////////////////////////////////////////////////4
		Int32ToDecChar(SystemLog.GPRS.PushTotalBytes,&AckBuffer.Data[Len]);
		strcat(AckBuffer.Data,",");
		Len = strlen(AckBuffer.Data);
		/////////////////////////////////////////////////4
		Int32ToDecChar(SystemLog.GPRS.SendTotalBytes,&AckBuffer.Data[Len]);
		strcat(AckBuffer.Data,",");
		Len = strlen(AckBuffer.Data);
		/////////////////////////////////////////////////4
		Int32ToDecChar(SystemLog.GPRS.SendTotalPack,&AckBuffer.Data[Len]);
		strcat(AckBuffer.Data,",");
		Len = strlen(AckBuffer.Data);
		/////////////////////////////////////////////////4
		Int32ToDecChar(SystemLog.GSM.RecSms,&AckBuffer.Data[Len]);
		strcat(AckBuffer.Data,",");
		Len = strlen(AckBuffer.Data);
		/////////////////////////////////////////////////
		Int32ToDecChar(SystemLog.GSM.SendSms,&AckBuffer.Data[Len]);
		strcat(AckBuffer.Data,",");
		Len = strlen(AckBuffer.Data);
		/////////////////////////////////////////////////
		Int32ToDecChar(SystemLog.GSM.RegServer,&AckBuffer.Data[Len]);
		strcat(AckBuffer.Data,",");
		Len = strlen(AckBuffer.Data);
		/////////////////////////////////////////////////
		Int32ToDecChar(SystemLog.GSM.RegServerOK,&AckBuffer.Data[Len]);
	
		strcat(AckBuffer.Data,",OE,");
		Len = strlen(AckBuffer.Data);			
		/////////////////////////////////////////////////4
		Int32ToDecChar(SystemLog.GPRS.OnLineTime,&AckBuffer.Data[Len]);
		strcat(AckBuffer.Data,",");
		Len = strlen(AckBuffer.Data);
		/////////////////////////////////////////////////4
		Int32ToDecChar(SystemLog.GSM.EbbSignalTime,&AckBuffer.Data[Len]);
		strcat(AckBuffer.Data,",");
		Len = strlen(AckBuffer.Data);
		/////////////////////////////////////////////////4
		Int2Str(SystemLog.GPRS.DialCnt,&AckBuffer.Data[Len]);
		strcat(AckBuffer.Data,",");
		Len = strlen(AckBuffer.Data);
		/////////////////////////////////////////////////4
		Int2Str(SystemLog.GPRS.DialOK,&AckBuffer.Data[Len]);
	
	
		strcat(AckBuffer.Data,",ME,");
		Len = strlen(AckBuffer.Data);
		/////////////////////////////////////////////////2
		Int32ToDecChar(SystemLog.GSM.CoolBoot,&AckBuffer.Data[Len]);
		strcat(AckBuffer.Data,",");
		Len = strlen(AckBuffer.Data);
		/////////////////////////////////////////////////2 
		Int32ToDecChar(SystemLog.GSM.SimErrorTimes,&AckBuffer.Data[Len]);
		strcat(AckBuffer.Data,",");
		Len = strlen(AckBuffer.Data);
		/////////////////////////////////////////////////
		Int2Str(SystemLog.VDR.SystemWorkTime,&AckBuffer.Data[Len]);
	
	
		strcat(AckBuffer.Data,",TEL,");
		Len = strlen(AckBuffer.Data);
		//////////////////////////////////////////////////////////
		Int32ToDecChar(SystemLog.GG.CallerTimes,&AckBuffer.Data[Len]);
		strcat(AckBuffer.Data,",");
		Len = strlen(AckBuffer.Data);
		/////////////////////////////////////////////////////////
		Int32ToDecChar(SystemLog.GG.CallerTime,&AckBuffer.Data[Len]);
		strcat(AckBuffer.Data,",");
		Len = strlen(AckBuffer.Data);
		/////////////////////////////////////////////////////////
		Int32ToDecChar(SystemLog.GG.CalledTimes,&AckBuffer.Data[Len]);
		strcat(AckBuffer.Data,",");
		Len = strlen(AckBuffer.Data);
		/////////////////////////////////////////////////////////
		Int32ToDecChar(SystemLog.GG.CalledTime,&AckBuffer.Data[Len]);
	
		Len = strlen(AckBuffer.Data);
		//AckBuffer.Length = Len;
	}

	AckBuffer.Length = Len;
}





/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�
   ����������		G20  ���ø�IP��ַ
   ��ڲ�����
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺 
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void		GPRSCls::Set3IP(uchar *pPS)
{
	uchar		IP_Port[LEN_IP_PORT] = {0};		// 123.456.789.012:12345 ���21�ֽ�
	uchar		i,c,t,Cnt,Type;
	char     *p;
	uint32   k;
	
	NOP();	NOP();
	Type = OneFrame[PSC[4<<1]];

	if( Type=='D' )
	{
		SetBufferValue(IP_Port,LEN_IP_PORT,0);
		c = OneFrame[PSC[5<<1]];
		
		if(c=='0')	
		{
			DFE.Write(EE_JT_M_IP_DNS,32,IP_Port);		       //JT808 
			//DFE.Write(EE_JT_TCP_PORT,4,IP_Port);	 
			DFE.Write(EE_IP_PORT_TCP,LEN_IP_PORT,IP_Port);
			SetBufferValue(sIP.UDP,LEN_IP_PORT,0);		
			sIP.EnableFlag[eFLAG_TCP] = 0;
			t = 0;
		   DFE.Write(EE_ENABLE_TCP,1,(uchar*)&t);
			sIP.Online[eFLAG_TCP] = 0;
			sIP.Socket[eFLAG_TCP] = 0;

		}
		if(c=='1')	
		{
			DFE.Write(EE_JT_M_IP_DNS,32,IP_Port);		       //JT808 
			//DFE.Write(EE_JT_UDP_PORT,4,IP_Port);	 
			DFE.Write(EE_IP_PORT_UDP,LEN_IP_PORT,IP_Port);
			SetBufferValue(sIP.UDP,LEN_IP_PORT,0);		
			sIP.EnableFlag[eFLAG_UDP] = 0;
			t = 0;
		   DFE.Write(EE_ENABLE_UDP,1,(uchar*)&t);
			sIP.Online[eFLAG_UDP] = 0;
			sIP.Socket[eFLAG_UDP] = 0;

		}
		if(c=='2')	
		{
			DFE.Write(EE_JT_B_IP_DNS,32,IP_Port);		        //JT808 
			//DFE.Write(EE_JT_TCP_PORT,4,IP_Port);	
			DFE.Write(EE_IP_PORT_TCP1,LEN_IP_PORT,IP_Port);	
			SetBufferValue(sIP.TCP1,LEN_IP_PORT,0);
			sIP.EnableFlag[eFLAG_TCP1] = 0;
			t = 0;
		   DFE.Write(EE_ENABLE_TCP1,1,(uchar*)&t);
			sIP.Online[eFLAG_TCP] = 0;
			sIP.Socket[eFLAG_TCP] = 0;	
		}
		if(c=='3')	
		{
			DFE.Write(EE_JT_B_IP_DNS,32,IP_Port);		        //JT808
			//DFE.Write(EE_JT_UDP_PORT,4,IP_Port);	 
			DFE.Write(EE_IP_PORT_UDP1,LEN_IP_PORT,IP_Port);
			SetBufferValue(sIP.UDP1,LEN_IP_PORT,0);	
			sIP.EnableFlag[eFLAG_UDP1] = 0;
			t = 0;
		   DFE.Write(EE_ENABLE_UDP1,1,(uchar*)&t);
			sIP.Online[eFLAG_UDP1] = 0;
			sIP.Socket[eFLAG_UDP1] = 0;
		}
		AckBuffer.GPS = TRUE;
		return;
	}
	else if( Type=='A' )
	{	
		t = PSC[6<<1];
		for(Cnt=0,i=0;i<LEN_IP_PORT;i++)
		{
			c = OneFrame[t++];
			if( (c == ',')||(c=='.')||(c==':'))
			{
				Cnt++;
				if(Cnt<=3)
				{
					c = '.';
				}
				else if(Cnt==4)
				{
					if(c==',')
					{
						c = ':';
					}
				}			
			}		
			if(c=='#')	c = 0;
			if(Cnt<5)
				IP_Port[i] = c;
			else
				IP_Port[i] = 0;
		}	
		////////////////////////////////// ���IP�ĺϷ���
		c = OneFrame[PSC[5<<1]];
		t = ValidIP_Port(IP_Port);
		if( ((c!='1')&&(c!='2')&&(c!='3'))  || (t!=OK)  )
		{
			AckBuffer.Error = TRUE;
			return;
		}	
		if(c=='1')
		{
			DFE.Write(EE_IP_PORT_UDP,LEN_IP_PORT,IP_Port);
			strcpy((char*)sIP.UDP,(char*)IP_Port);	
			p = MyStrChar((char *)IP_Port, ':');			  //JT808 
		   *p = 0;
		   DFE.Write(EE_JT_M_IP_DNS,32,IP_Port);		   //JT808 
		   *p = ':';
			sIP.EnableFlag[eFLAG_UDP] = ENABLE_FLAG;
		   t = ENABLE_FLAG;
		   DFE.Write(EE_ENABLE_UDP,1,(uchar*)&t);

			p = MyStrChar((char *)IP_Port, ':');			     //JT808 		  
		   k = My_atol(p+1);
		   DFE.Write(EE_JT_UDP_PORT,4,(uchar *)&k);		
		}
		if(c=='2')	
		{
			DFE.Write(EE_IP_PORT_TCP1,LEN_IP_PORT,IP_Port);
			strcpy((char*)sIP.TCP1,(char*)IP_Port);
			p = MyStrChar((char *)IP_Port, ':');			  //JT808 
			*p = 0;
		   DFE.Write(EE_JT_B_IP_DNS,32,IP_Port);		        //JT808 
		   *p = ':';
			sIP.EnableFlag[eFLAG_TCP1] = ENABLE_FLAG;
		   t = ENABLE_FLAG;
		   DFE.Write(EE_ENABLE_TCP1,1,(uchar*)&t);
			
			
			p = MyStrChar((char *)IP_Port, ':');			     //JT808 
		   k = My_atol(p+1);
		   DFE.Write(EE_JT_TCP_PORT,4,(uchar *)&k);	
		}
		if(c=='3')	
		{
			DFE.Write(EE_IP_PORT_UDP1,LEN_IP_PORT,IP_Port);
			strcpy((char*)sIP.UDP1,(char*)IP_Port);
			p = MyStrChar((char *)IP_Port, ':');			  //JT808 
			*p = 0;
		    DFE.Write(EE_JT_B_IP_DNS,32,IP_Port);		        //JT808 
		   *p = ':';
			sIP.EnableFlag[eFLAG_UDP1] = ENABLE_FLAG;
		   t = ENABLE_FLAG;
		   DFE.Write(EE_ENABLE_UDP1,1,(uchar*)&t);
			
			p = MyStrChar((char *)IP_Port, ':');			    //JT808 
		   k = My_atol(p+1);
		   DFE.Write(EE_JT_UDP_PORT,4,(uchar *)&k);		
		}
		GG_Status.Flag.Word = GG_FLAG_INIT_DATA;	
		Uart.puts("GRRS Close\r\n\0");

		AckBuffer.GPS = TRUE;
	}
}	



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�SystemCoolBoot			 R1
   ����������ϵͳ������
   ��ڲ�����
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺 
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void		GPRSCls::SystemCoolBoot(uchar *pPS)						  // "R1,"
{
	NOP();	NOP();	NOP();	NOP();
	
	
	
	Uart.puts("System cool boot\r\n\0");
	os_dly_wait(100);	
	/////////////////////////////////// 	
	Switch.SaveVehicleStatus();			// 
	DisableIRQ();
	NOP();	NOP();	NOP();	NOP();
	NVIC_SystemReset();
	NOP();	NOP();	NOP();	NOP();
	for(;;)
	{
		while(1);	
	}
	
}






 /* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�GPRSCls::SetCenterNumber
   ���������� �������ĺ���						S2
   ��ڲ�����
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺 
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void		GPRSCls::SetCenterNumber(uchar *pPS)
{
	NOP();	NOP();
	if( (PSC[(4<<1)+1]>16)||(PSC[(5<<1)+1]>16) )
	{
		AckBuffer.Error = TRUE;
		return;
	}
	SetBufferValue((uchar*)sTelNum.Center,16,0);
	SetBufferValue((uchar*)sTelNum.Center1,16,0);
	
	memmove(sTelNum.Center,&OneFrame[PSC[4<<1]],PSC[(4<<1)+1]);
	sTelNum.Center1[16] = 0;
	DFE.Write(EE_CENTER_NUMBER,16,(uchar*)sTelNum.Center);	

	memmove(sTelNum.Center1,&OneFrame[PSC[5<<1]],PSC[(5<<1)+1]);
	sTelNum.Center1[16] = 0;
	DFE.Write(EE_CENTER_NUMBER1,16,(uchar*)sTelNum.Center1);	


	GSM.SetSMS_Buffer((char *)sTelNum.Center, (char *)sTelNum.Center);
	
	AckBuffer.GPS = TRUE;	 
}







/*************************************************************************************************
** ��������: RemoteOilControl()	  S20
** ��������: Զ����·����
** �������: ��
** �������: ��
** ȫ�ֱ���: ��
** ����ʱ�䣺2009.08.21
** �޸�ʱ��: 
*GY,0000000000,S20,134108,1,0#
*GY,0000000000,S20,134108,1#
**************************************************************************************************/
void		GPRSCls::RemoteOilControl(uchar *pPS)
{
	uchar 	force,ctr,type;

	
	///// ȡ���� ////////////////

	type = OneFrame[PSC[4<<1]+1];

	if(type=='#')	  	// *GY,0000000000,S20,134108,1#
	{
		ctr = OneFrame[ *(pPS+(4<<1)) ];	
	
		if(ctr=='0')			// ����������ʱ������
			VehicleControl.Lock = LOCK_VEHICLE_ENABLE_INV;
		else
			VehicleControl.Lock = LOCK_VEHICLE_ENABLE;
			
		DFE.Write(EE_LOCK_FLAG,LEN_LOCK_VEHICLE_FLAG,(uchar *)&VehicleControl);
	}
	else					// *GY,0000000000,S20,134108,1,0#
	{
		ctr = OneFrame[PSC[4<<1]];
		force = OneFrame[PSC[5<<1]];

		if(((ctr!='0')&&(ctr!='1'))||((force!='0')&&(force!='1')))										
		{
			AckBuffer.Error = TRUE;
			return ;
		}

		if(ctr=='1')												 		// ����·
		{
			if(force=='1')											 		// ��̬����·
			{
				VehicleControl.ForceLock = LOCK_VEHICLE_ENABLE;			
			}
			else 																// ���Ͷϵ�
			{		
				VehicleControl.ForceLock = 0x00;
				VehicleControl.Lock = LOCK_VEHICLE_ENABLE;		// ��̬����·
				gCnt.PulsControl = 30;	  									// 1����
			}		
			DFE.Write(EE_LOCK_FLAG,4,(uchar *)&VehicleControl);	 	// ���ͱ�־д������
		}
		else																	// �ָ���·
		{
			VehicleControl.Lock = 0x00;
			VehicleControl.ForceLock = 0x00; 			  			// 
			DFE.Write(EE_LOCK_FLAG,4,(uchar *)&VehicleControl);	 	// ���ͱ�־д������
		}
	}
	///// Ӧ�� //////////////////
	AckBuffer.GPS = TRUE;

}	


									  


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�
   ����������	  S23	  ��������IP��ַ
   ��ڲ�����
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺 
	*GY,000,S23,123456,113.108.150.103,8899,5#
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void		GPRSCls::SetIP_Port(uchar *pPS)
{	
	uchar		IP_Port[LEN_IP_PORT] = {0};		// 123.456.789.012:12345 ���21�ֽ�
	uchar		i,c,t,Cnt ;
	char     *p;
	uint32   k;


	NOP();	NOP();
	t = PSC[4<<1];
	c = OneFrame[t];

	if( (c>='0')&&(c<='9') )	  	// IP��ַ
	{				 	
		for(Cnt=0,i=0;i<LEN_IP_PORT;i++)
		{
			c = OneFrame[t++];
			if( (c == ',')||(c=='.')||(c==':'))
			{
				Cnt++;
				if(Cnt<=3)
				{
					c = '.';
				}
				else if(Cnt==4)
				{
					if(c==',')
					{
						c = ':';
					}
				}			
			}		
			if(c=='#')	c = 0;
			if(Cnt<5)
				IP_Port[i] = c;
			else
				IP_Port[i] = 0;
		}
		
		////////////////////////////////// ���IP�ĺϷ���
		t = ValidIP_Port(IP_Port);
					
		if(t == OK)
		{
			NOP();	NOP();	NOP();	NOP();	
			DFE.Write(EE_IP_PORT_TCP,32,IP_Port);
			strcpy((char*)sIP.TCP,(char*)IP_Port);	
			p = MyStrChar((char *)IP_Port, ':');			    //JT808 
			*p = 0;
			DFE.Write(EE_JT_M_IP_DNS,32,IP_Port);		       //JT808 
			*p = ':';									  
			sIP.EnableFlag[eFLAG_TCP] = ENABLE_FLAG;
			t = ENABLE_FLAG;
			DFE.Write(EE_ENABLE_TCP,1,(uchar*)&t);
	
			p = MyStrChar((char *)IP_Port, ':');			    //JT808 
			k = My_atol(p+1);
			DFE.Write(EE_JT_TCP_PORT,4,(uchar *)&k);
	
			os_dly_wait(1);		
			GG_Status.Flag.Word = GG_FLAG_INIT_DATA;	
			Uart.puts("GRRS Close\r\n\0");
			
		}
	}
	else	 	// if( (c<'0')||(c>'9') )		// ����
	{
		NOP();	NOP();	NOP();	NOP();
		t = PSC[4<<1];
		Cnt = PSC[(4<<1)+1];
		memmove( IP_Port, (uchar*)&OneFrame[t],Cnt);	
		DFE.Write(EE_IP_PORT_TCP,32,IP_Port);
		strcpy((char*)sIP.TCP,(char*)IP_Port);	
		DFE.Write(EE_JT_M_IP_DNS,32,IP_Port);

		sIP.EnableFlag[eFLAG_TCP] = ENABLE_FLAG;
		t = ENABLE_FLAG;
		DFE.Write(EE_ENABLE_TCP,1,(uchar*)&t);

		t = PSC[5<<1];
		k = Str2Int( (char*)&OneFrame[t] );
		DFE.Write(EE_JT_TCP_PORT,4,(uchar *)&k);

		os_dly_wait(1);		
		GG_Status.Flag.Word = GG_FLAG_INIT_DATA;	
		Uart.puts("GRRS Close\r\n\0");
		AckBuffer.GPS = TRUE;

	}

	AckBuffer.GPS = TRUE;

	t = OneFrame[PSC[6<<1]];
	if(t=='0')
		GPRS.UnRegTerminal();
}




// *GY,000,S24,123456,0,APN_NAME#

void		GPRSCls::SetAPN(uchar *pPS)
{
	uchar	t;
   t = OneFrame[*(pPS+(4<<1))];
	if(t=='G')
	{
		strcpy((char*)AckBuffer.Data,(char*)JT808_IP_DNS.M_APN);		
		AckBuffer.Length = strlen(AckBuffer.Data);
		AckBuffer.GPS = 0;
	}
	else if(t=='S')
	{
		SetBufferValue(JT808_IP_DNS.M_APN,LEN_IPorDNS,0);
		memmove(JT808_IP_DNS.M_APN,&OneFrame[*(pPS+(5<<1))],*(pPS+(5<<1)+1));
		DFE.Write(EE_JT_M_APN, LEN_IPorDNS, (uchar *)&JT808_IP_DNS.M_APN[0]);
		AckBuffer.GPS = TRUE;
	}
}


/*************************************************************************************************
** ��������: RemoteUpgrade()		 S52
** ��������: Զ������
** �������: ָ��*pPS
** �������: ��
** ȫ�ֱ���: ��
** ����ʱ�䣺2009.11.05
** �޸�ʱ��: 
SHSZ*GY,000,S52,123456,IAP1,P,61.152.94.126,7071,szjly,123456,g11bd.gyup,..#
************************************************************************************************/
void		GPRSCls::RemoteUpgrade(uchar *pPS)
{ 
	uchar		CmdTime[36] = {0}, t;
	uint16	map=0;

	memmove(CmdTime,&OneFrame[*(pPS+(4<<1))],*(pPS+(4<<1)+1));
	
	if((CmdTime[0]=='I')&&(CmdTime[1]=='A')&&(CmdTime[2]=='P'))//&&(GG_Status.LinkType==TCP_LINK))
	{
		Uart.puts("S52 Cmd");
		UpgradeFlag = 1;
		UpDataCount = 0;
		GPRS.ClearAligBuffer();						  	// ������ͻ���
		Uart1.ClearReceiveFlag(1);							// ������ջ���
		KeyDisp.SetLED_Status(LED_GPRS,LED_GPRS_ON_LINE);

		memmove(&t, &OneFrame[*(pPS+(5<<1))],*(pPS+(5<<1)+1));
		if(t=='D')
		{
			if(CmdTime[3]=='1')
				DFF.SomeBlockErase(IAP_Firmware1_Base_Block_Add, 4);
			else 
				DFF.SomeBlockErase(IAP_Firmware2_Base_Block_Add, 4); 
		}
		else if(t=='T')		 //TCP����
		{
		    NOP();
		}
		else if(t=='P')		 //FTP����
		{
			SetBufferValue((uchar *)&UpdatePara,UpdatePara_Len, 0);
			memmove(UpdatePara.RemoteAddr, &OneFrame[*(pPS+(6<<1))],*(pPS+(6<<1)+1));
			memmove(UpdatePara.RemotePort, &OneFrame[*(pPS+(7<<1))],*(pPS+(7<<1)+1));
			memmove(UpdatePara.FTPname, &OneFrame[*(pPS+(8<<1))],*(pPS+(8<<1)+1));
         memmove(UpdatePara.FTPpassword, &OneFrame[*(pPS+(9<<1))],*(pPS+(9<<1)+1));
			memmove(UpdatePara.FileName, &OneFrame[*(pPS+(10<<1))],*(pPS+(10<<1)+1));
			memmove(UpdatePara.FileList, &OneFrame[*(pPS+(11<<1))],*(pPS+(11<<1)+1));		 //Ŀ¼
			GG_Status.Flag.Bits.Updating = 0; 	
			GG_Status.Flag.Bits.UpdateRemoteSet = 0;	
			GG_Status.Flag.Bits.UpdateOnLine = 0;
			UpdatePara.Enable = ENABLE_FLAG;			//ʹ��
			GG_Status.Flag.Word = GG_FLAG_INIT_DATA;	
	   	Uart.puts("GRRS Close\r\n\0");
		}
		else if(t=='0')    //TCPģʽ������ʼ��
		{
		    if(CmdTime[3]=='1')
			 {
			    DFE.Read(EE_UP_DATA_FRAME_CNT1,2,(uchar*)&gCnt.UpDateFrame1);
			    map = gCnt.UpDateFrame1;
			 }
			 else
			 {
				 DFE.Read(EE_UP_DATA_FRAME_CNT2,2,(uchar*)&gCnt.UpDateFrame2);
				 map = gCnt.UpDateFrame2;
			 }
		}

		if(CmdTime[3]=='1')
		{
			UpdatePara.Frame = IAP_Firmware1;
		}
		else if(CmdTime[3]=='2')
		{
			UpdatePara.Frame = IAP_Firmware2;
		}
				
		if((t=='P')||(t=='D')||(t=='T'))
		{
			strcpy(AckBuffer.Data, "OK\0");
			AckBuffer.Length = 2;
		}
		else if(t=='C')	  //��ѯ�汾��Ϣ
		{
//			 U0_Iap_Flag = 9;
//			 Sys.GetVersion((char *)AckBuffer.Data);
//			 U0_Iap_Flag = 0;
//			 AckBuffer.Length = strlen(AckBuffer.Data);
			if(UpdatePara.Frame == IAP_Firmware1)
			{
			  U0_Iap_Flag = 9;
			  Sys.GetVersion((char *)AckBuffer.Data);
			  U0_Iap_Flag = 0;
			}
			else if (UpdatePara.Frame == IAP_Firmware2)
			{
				GPS.QueryGNSSVersion(AckBuffer.Data);
			}
			else
			{
				AckBuffer.Data[0] = '0';
				AckBuffer.Data[1]	 = 0;
			}

			AckBuffer.Length = strlen(AckBuffer.Data);
		}
		else if(t == 'S')
		{
			strcpy(AckBuffer.Data, "UpdateSucceed\0");
			AckBuffer.Length = strlen(AckBuffer.Data);
		}
		else if(t == 'F')
		{
			strcpy(AckBuffer.Data, "UpdateFail\0");
			AckBuffer.Length = strlen(AckBuffer.Data);
		}
		else
		{
			Int16ToDecChar(map,AckBuffer.Data);
		   AckBuffer.Length = strlen(AckBuffer.Data);
		}
	}
	else
	{
		UpgradeFlag	= 0;
		AckBuffer.Error = TRUE;
		Uart.puts("S52 Error");
	}

}





/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�GPRSCls::ClearWorkTime		  R81
   ������������ȡIP��ַ���˿ںź�ʱ��ɼ�ʱ����
   ��ڲ�����
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺 
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void			GPRSCls::ReadIP_PortAndInterval(uchar *pPS)
{
	uchar		Len;
	//pGPS_SEND	pSd;
	uchar		CmdTime[16] = {0};
	uchar		tBuffer[16]; 	
	NOP();	NOP();	NOP();

	
	//pSd = (pGPS_SEND)tBuffer;
	//GPS.GetSendConfig(pSd);
			
	CmdTime[0] = ',';
	Int32ToDecChar(SendInterval.DefauletTime,(char*)&CmdTime[1]);					// 
	strcat((char*)CmdTime,",");
	Len = strlen((char*)CmdTime);
	Int32ToDecChar(SendInterval.IDEL_ModeTime,(char*)&CmdTime[Len]);		// 
				
	strcpy((char*)AckBuffer.Data,(char*)sIP.TCP);
	strcat((char*)AckBuffer.Data,(char*)CmdTime);
	strcat((char*)AckBuffer.Data,",");
	strcat((char*)AckBuffer.Data,(char*)JT808_IP_DNS.M_APN);	
	AckBuffer.Length = strlen(AckBuffer.Data);
	
		
}



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�GPRSCls::ReadSMS_Number
   ������������ȡ���Ķ��ź���
   ��ڲ�����
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺 
*GY,0706200002,V4,R82,170305,081956,13800200500,13800200501,13800200502,13800200503,13800200504
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void			GPRSCls::ReadSMS_Number(uchar *pPS)
{
	uint16		Len;
	
	NOP();	NOP();	NOP();
	Len = 0;
	
	DFE.Read(EE_CENTER_NUMBER,11,(uchar*)&AckBuffer.Data[Len]);
	Len += 11;		AckBuffer.Data[Len] = ',';	Len += 1;
	
	DFE.Read(EE_CENTER_NUMBER1,11,(uchar*)&AckBuffer.Data[Len]);
	Len += 11;		AckBuffer.Data[Len] = ',';	Len += 1;
	
	
	Len += 11;	
	
	AckBuffer.Length = Len;
	
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�GPRSCls::ReadSMS_Number
   ������������ȡ���Ķ��ź���
   ��ڲ�����
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺 
*GY,0706200002,V4,R82,170305,081956,13800200500,13800200501,13800200502,13800200503,13800200504
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������

void			GPRSCls::Calling_GPS(uchar *pPS)
{
	NOP();	NOP();
	AckBuffer.GPS = TRUE;		
}








/*************************************************************************************************
** ��������: RestoreFactorySetting()
** ��������: �ָ���������		  S25
** �������: ָ��*pPS
** �������: ��
** ȫ�ֱ���: ��
** ����ʱ�䣺2009.08.25
** �޸�ʱ��: 
************************************************************************************************/
void		GPRSCls::RestoreFactorySetting(uchar *pPS)
{
	uchar 	i;
	//pGPS_SEND		pSd;
	
	#pragma		pack(1)
	union
	{
		uchar		B8[32];
		uint16	B16[16];
		uint32	B32[8];
		unsigned  long long  B64;
	}Tp;
	
	uint32	tl;		
	
	//tmp = 0;
	Tp.B16[0] = 0;
	

	Sys.FirstPowerOn(0);

	if(MyStrStr((char*)OneFrame,"CLR_G1\0")!=NULL)
	{
		SetBufferValue((uchar*)&SystemLog,LEN_SYSTEM_LOG,0);
		tl = EE_SYSTEM_LOG_BASE;
		for(i=0;i<SYSTEM_LOG_EE_PAGE;i++)
		{							
			DFE.Write(tl,EE_PAGE_LEN,(uchar*)&Tp.B8);
			tl += EE_PAGE_LEN;
		}
	}	

	if(MyStrStr((char*)OneFrame,"CLR_ALL\0")!=NULL)
		Sys.FirstPowerOn(TRUE);
	
		
	////////////////// Ӧ�� //////////////////////
	AckBuffer.GPS = TRUE;		
}








/*************************************************************************************************
** ��������: SetReturnTime()
** ��������: ����ȱʡ�ش�ʱ��ʱ��		S17
** �������: ָ��*pPS
** �������: ��
** ȫ�ֱ���: ��
** ����ʱ�䣺2009.08.21
** �޸�ʱ��: 
**************************************************************************************************/
void	GPRSCls::SetReturnTime(uchar *pPS)
{

	 uchar		buf[20];	
	 uint32		ch;
	 //pGPS_SEND	pSd;
	
	
	ch = My_atol((char*)&OneFrame[*(pPS+(4<<1))]);

	if((ch>0)&&(ch<=65535))
	{
		SendInterval.DefauletTime = ch;
		DFE.Write(EE_JT_SEND_TIME_DEFAULET, 4,(uchar *)&SendInterval.DefauletTime);
	}
	else
	{
		AckBuffer.Error = TRUE;
		return ;
	}


//	pSd = (pGPS_SEND)&buf;
//	GPS.GetSendConfig(pSd);
//	pSd-> Str.Interval = My_atol((char*)&OneFrame[*(pPS+(4<<1))] );
//	pSd-> Str.StopInterval = My_atol((char*)&OneFrame[*(pPS+(4<<1))] );
//	pSd-> Str.Times =  0xffffffff;
//	GPS.SetSendConfig(pSd,1);			  			// ���籣��
	 
	 ///// Ӧ�� //////////////////
	 AckBuffer.GPS = TRUE;

}



/*************************************************************************************************
** ��������: DialHelpPhone()
** ��������: ���������绰
** �������: ָ��*pPS
** �������: ��
** ȫ�ֱ���: ��
** ����ʱ�䣺2009.09.02
** �޸�ʱ��: 
************************************************************************************************/
void		GPRSCls::DialHelpPhone()
{
	uchar	len;
		
	len = strlen((char *)sTelNum.User);
	
	PhoneCall.DialFlag = TRUE;
	PhoneCall.Length = len;
	memmove((uchar*)&PhoneCall.Buffer[0],(uchar*)sTelNum.User,len);
	AckBuffer.GPS = TRUE;
}











/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�GPRSCls::Set_Or_Get_Accel_Sub_Para
   ���������� ���û��ȡ���ٶȴ������������
   ��ڲ�����
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
	*GY,000,G3,123456,Cfg,Para#
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������

#include 	"..\Accelerometer\kionix_kxtf9_driver.h" 




typedef	 uchar (*FunVoid)(void);
typedef	 uchar (*FunInt)(int);


uchar GPRSCls::SetOrGetAccelSub(uchar (*pFunVoid)(void), uchar (*pFunInt)(int), uchar *pos, 
													uchar Type, uchar Cmd, uchar Index, uchar Len, uint16 SaveAddr)
{


	return 0;
	
	
}
























uchar GPRSCls::Set_Or_Get_Accel_Sub_Para(uchar Cmd, uchar sOrg)
{
	return 0;
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�GPRSCls::SetOrGetAccelerometerPara
   ���������� ���û��ȡ���ٶȴ���������
   ��ڲ�����
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
	*GY,000,S102,123456,S, R,Para#
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������

void	GPRSCls::SetOrGetAccelerometerPara(uchar *pPS)
{
	
}








/*************************************************************************************************
** ��������: RemoteMonitor()
** ��������: Զ�̼���
** �������: ָ��*pPS
** �������: ��
** ȫ�ֱ���: ��
** ����ʱ�䣺2009.08.23
** �޸�ʱ��: 2009.08.31
************************************************************************************************/
void	GPRSCls::RemoteMonitor(uchar *pPS)
{
	uchar 	i,err=0,len,num[16];

	////////////// ȡ���� ////////////////
	memmove(num,&OneFrame[*(pPS+(4<<1))],*(pPS+(4<<1)+1));
	len = *(pPS+(4<<1)+1);
	
	if(len>16)
	{
		err=1;
	}
	else
	{
		for(i=0;i<len;i++)
		{
		 	if((num[i]<'0')||(num[i]>'9'))
			{
				err=1;
			}
		}
	}

	if(err==1)
	{
	 	AckBuffer.Error = TRUE;
		return ;
	}

	if(TD.GPRS==tGPRS_PACK_DATA)
	{
		Uart.putb(1,len,num);
	}

		
	///////////// Ӧ���� //////////////////
	if(PhoneCall.ListeningFlag == TRUE)												// ��������ڼ���
	{
		AckBuffer.Error = TRUE;	
	}
	else
	{
	 	PhoneCall.ListeningFlag = TRUE;
		PhoneCall.DialFlag = TRUE;
		PhoneCall.Length = len;
		memmove((uchar*)&PhoneCall.Buffer[0],num,len);	
		PhoneCall.CenterCall = TRUE;			
	}	
	AckBuffer.GPS = TRUE;

}






/*************************************************************************************************
** ��������: PhoneSchedulFunction()
** ��������: �绰���ȹ���
** �������: ָ��*pPS
** �������: ��
** ȫ�ֱ���: ��
** ����ʱ�䣺2009.09.12
** �޸�ʱ��: 
	*GY,000,S43,123456,13922280586#
************************************************************************************************/
void	GPRSCls::PhoneSchedulFunction(uchar *pPS)
{
	uchar 	err=0,len,num[16];	// i

	
	////////////// ȡ���� ////////////////
	memmove(num,&OneFrame[*(pPS+(4<<1))],*(pPS+(4<<1)+1));	
	len = *(pPS+(4<<1)+1);


   err = CprDataValue(len, 12 ,(uchar) '0', (uchar) '9', num);


	if(err==1)
	{
		AckBuffer.Error = TRUE;
		return ;
	}
	
	///////////// Ӧ���� //////////////////
	if(PhoneCall.ListeningFlag == TRUE)					// ���ڼ���,���߽�ֹ����
	{
		AckBuffer.Error = TRUE;
	}
	else
	{	
		PhoneCall.DialFlag = TRUE;
		PhoneCall.Length = len;
		memmove((uchar*)&PhoneCall.Buffer[0],num,len);
		AckBuffer.GPS = TRUE;
		PhoneCall.CenterCall = TRUE;
	}	

	
}




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�GPRSCls::SetOrGetTermialPhoneNumber		 
   ������������ȡ�������ն��ֻ���		 S88
   ��ڲ�����
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺2014.04.11
   �޸�ʱ�䣺 
*GY,000,S88,123456,G#
*GY,000,S88,123456,S,13800000000#
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void			GPRSCls::SetOrGetTermialPhoneNumber(uchar *pPS)
{
	uchar Type,s,Len; 
	uchar Num[17];

	Type = OneFrame[PSC[4<<1]];

	if (Type=='G')
	{
		 SetBufferValue((uchar*)&AckBuffer.Data[0], 20,0);
		 DFE.Read(EE_SIM_NUMBER,16,(uchar*)&AckBuffer.Data[0]);
		 AckBuffer.Length = strlen(AckBuffer.Data);
	}
	else if (Type=='S')
	{
		if(PSC[(5<<1)+1]>16)
		{
			AckBuffer.Error = TRUE;
			return;
		}
		
		SetBufferValue(Num,16,0);
		Len =   *(pPS+(5<<1)+1);
		memmove(Num, &OneFrame[PSC[5<<1]], Len);	
		s = CprDataValue(Len, 12 ,(uchar) '0', (uchar) '9', Num);
		if (s==1)
		{
			Uart.puts("�ֻ��������");
			AckBuffer.Error = TRUE;
			return ;
		}

		SetBufferValue(sTelNum.Sim, 16, 0);
		sTelNum.Sim[0] = 0x30;
	   memmove((uchar*)&sTelNum.Sim[1],&Num[0],11);
		s = strlen((char *)&sTelNum.Sim[0]);
		DFE.Write(EE_SIM_NUMBER,16,(uchar*)&sTelNum.Sim[0]);
		NOP(); NOP(); NOP();
	
		DecCharToPressBCD(s,(char *)sTelNum.Sim,SimNumberBCD);
		memmove(sTelNum.SimBCD_JT, SimNumberBCD, 6);	
		AckBuffer.GPS = TRUE;
	}
	else
	{
		AckBuffer.Error = TRUE;
	}


}



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�GPRSCls::SetRegInfo		 
   ����������
   ��ڲ�����
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺2014.04.11
   �޸�ʱ�䣺 
*GY,000,S60,123456,0#
*GY,000,S60,140450,1,44,01,0,��A16888,L0705171414510583#
*GY,000,S60,123456,1,44,01,0,��A16888,L708031423110001#
*GY,000,S60,162600,1,44,01,0,l�A16888,L0705171414510583#
*GY,000,S60,162600,1,44,01,0,l�A16888,L0705171414510583#
*GY,000,S60,140450,1,44,01,0,l�A16888,L0705171414510583#
*/ 
unsigned short		UniCode2CN(unsigned short   uCode);
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void			GPRSCls::SetRegInfo(uchar *pPS)
{
	
	uchar		Len;	
	char		tBuffer[32]; 
	union
	{
		uchar		Buffer[64];
		uint16	B16[32];
		uint32	B32[16];
	}Tp;

	
	Tp.Buffer[0] = OneFrame[*(pPS+(4<<1))];
	if( Tp.Buffer[0] == 'G' )			// ��ȡע�����
	{
		SetBufferValue((uchar*)AckBuffer.Data,128,0);

		// ʡ����ID
		DFE.Read(EE_PROVINCE_REGION_ID, 2, Tp.Buffer);
		Tp.Buffer[2] = Tp.Buffer[3] = 0;
		Int2Str( Tp.B32[0], AckBuffer.Data);
		strcat( AckBuffer.Data,",");
		Len = strlen( (char*)AckBuffer.Data);

		// ������	  		
		DFE.Read(EE_PREFECTURE_REGION_ID, 2, Tp.Buffer);
		Tp.Buffer[2] = Tp.Buffer[3] = 0;
		Int2Str( Tp.B32[0], (char*)&AckBuffer.Data[Len]);
		strcat( AckBuffer.Data,",");
		Len = strlen( (char*)AckBuffer.Data);
	
		// ������ID,		
		DFE.Read(EE_VENDOR_ID, 5, (uchar*)&AckBuffer.Data[Len]);	
		strcat( AckBuffer.Data,",");
		Len = strlen( (char*)AckBuffer.Data); 

		// �ն��ͺ�
		DFE.Read(EE_VENDOR_ID+5,20, (uchar*)&AckBuffer.Data[Len]);	
		strcat( AckBuffer.Data,",");
		Len = strlen( (char*)AckBuffer.Data); 
			
		
		// ������ɫ
		DFE.Read(FM_CAR_COLOUR, 1, (uchar*)&AckBuffer.Data[Len]);
		AckBuffer.Data[Len] += '0';
		strcat( AckBuffer.Data,",");
		Len = strlen( (char*)AckBuffer.Data); 
			 
		
		DFE.Read(FM_VEHICLE_CODE, 9, (uchar*)&AckBuffer.Data[Len]);	  // ���ƺ�
		strcat( AckBuffer.Data,",");
		Len = strlen( (char*)AckBuffer.Data); 
		
		// ȡVIN��		
		DFE.Read(FM_VEHICLE_VIN, LEN_VEHICLE_VIN, (uchar*)&AckBuffer.Data[Len]);  // ����VIN��	 
		strcat( AckBuffer.Data,",");
		AckBuffer.Length = strlen( (char*)AckBuffer.Data); 		
	}
	else if( Tp.Buffer[0] == 'S' )
	{
		Uart.puts((char*)OneFrame);			os_dly_wait(10);
		// ʡ��ID
		Tp.B32[0] = Str2Int((char*)&OneFrame[*(pPS+(5<<1))]);
		DFE.Write(EE_PROVINCE_REGION_ID, 2, Tp.Buffer);
		SetBufferValue((uchar*)tBuffer,32,0);
		memmove( tBuffer, (char*)&OneFrame[*(pPS+(5<<1))],2);
		strcat( tBuffer,"\r\n\0");
		Uart.puts("ʡ��ID��"); 
		Uart.puts(tBuffer);	
		
		// ������
		Tp.B32[0] = Str2Int((char*)&OneFrame[*(pPS+(6<<1))]);
		DFE.Write(EE_PREFECTURE_REGION_ID, 2, Tp.Buffer);
		SetBufferValue((uchar*)tBuffer,32,0);
		memmove( tBuffer, (char*)&OneFrame[*(pPS+(6<<1))],2);
		strcat( tBuffer,"\r\n\0");
		Uart.puts("����ID��"); 
		Uart.puts(tBuffer);		
	
		// ������ɫ
		Tp.Buffer[0] = OneFrame[*(pPS+(7<<1))];			//Uart.puts("T1 ");		os_dly_wait(10);
		Tp.Buffer[0] -= '0';									 	//Uart.puts("T2 ");		os_dly_wait(10);
		DFE.Write(FM_CAR_COLOUR, 1, Tp.Buffer);			//Uart.puts("T3 ");		os_dly_wait(10);

		SetBufferValue((uchar*)tBuffer,32,0);				//Uart.puts("T4 ");		os_dly_wait(10);
		tBuffer[0] = Tp.Buffer[0]+'0';						//Uart.puts("T5 ");		os_dly_wait(10);
		strcat( tBuffer,"\r\n\0");								//Uart.puts("T6 ");		os_dly_wait(10);
		Uart.puts("Color: "); 									//os_dly_wait(10);
		Uart.puts(tBuffer);										//os_dly_wait(10);
		
		// ���ƺ�
		Len = *(pPS+(8<<1)+1);
		SetBufferValue((uchar*)tBuffer,32,0);
		SetBufferValue( Tp.Buffer,64,0);
		memmove(Tp.Buffer,(uchar*)&OneFrame[*(pPS+(8<<1))],Len);
		Uart.puts("VCar Test Data:");
		Uart.putb(1,Len,Tp.Buffer);
		Uart.puts("\r\n\0");
		if(PDU_SMS.Ack == TRUE)
		{	// UniCode ת��������
			Tp.B16[0] = UniCode2CN(Tp.B16[0]);
			if ( Tp.B16[0]==0)
			{
				AckBuffer.Error = TRUE;
				Uart.puts(" �������ó��ƺŴ��� ");
				memmove( Tp.Buffer,"��",2);
				//return;
			}
		}
		else
			Uart.puts(" GPRS���ó��ƺ� ");
		Tp.Buffer[Len] = Tp.Buffer[Len+1] = Tp.Buffer[Len+2] = Tp.Buffer[Len+3] =  0;
		DFE.Write(FM_VEHICLE_CODE, LEN_VEHICLE_CODE, Tp.Buffer);	  // ���ƺ�

		
		strcat( (char*)Tp.Buffer,"\r\n\0");
		Uart.puts("���ƺ��룺"); 
		Uart.putb(1,Len,(uchar*)Tp.Buffer);

		DFE.Write(FM_VEHICLE_VIN, LEN_VEHICLE_VIN, (uchar*)&OneFrame[*(pPS+(9<<1))]);  // ����VIN��	 
		SetBufferValue((uchar*)tBuffer,32,0);
		memmove( tBuffer, (char*)&OneFrame[*(pPS+(9<<1))],*(pPS+(9<<1)+1));
		strcat( tBuffer,"\r\n\0");
		Uart.puts("����VIN�ţ�"); 
		Uart.puts(tBuffer);
		
		AckBuffer.GPS = TRUE;
	}
}





/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�GPRSCls::SetNormalPara		 
   ����������
   ��ڲ�����
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
	�ն��ֻ��š������ֻ��š����ĺ��롢�ٶ�ϵ��Kֵ
*GY,000,S61,123456,S,15901839135,13922280586,13800138000,2555,0,20001800#
*GY,000,S61,123456,G#
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void			GPRSCls::SetNormalPara(uchar *pPS)
{
	uchar Type,s,Len; 
	uchar Num[17];

	Type = OneFrame[PSC[4<<1]];

	if (Type=='G')
	{
		 SetBufferValue((uchar*)&AckBuffer.Data[0], 20,0);
		 DFE.Read(EE_SIM_NUMBER,16,(uchar*)&AckBuffer.Data[0]);
		 strcat( AckBuffer.Data,",");

		 strcat( AckBuffer.Data,(char*)sTelNum.User);
		 strcat( AckBuffer.Data,",");	  
		 

		 strcat( AckBuffer.Data,(char*)sTelNum.Center);
		 strcat( AckBuffer.Data,",");

		 Len = strlen(AckBuffer.Data);

		 Int2Str( VehiclePulseKV , (char*)&AckBuffer.Data[Len]);
		 

		 if(UseGPS_Mil)			 	strcat( AckBuffer.Data,",P,");
		 else 							strcat( AckBuffer.Data,",G,");

		 // ����������
		 SetBufferValue(Num,16,0);
		 IntToHexChar(JT808_Alarm_Word.Shield,8,(char*)Num);
		 strcat( AckBuffer.Data,(char*)Num); 


		 if(EnableSysSleep)			 	strcat( AckBuffer.Data,",1");
		 else 								strcat( AckBuffer.Data,",0");

		 if(EnableVDR_Upload)			strcat( AckBuffer.Data,",1,");
		 else 								strcat( AckBuffer.Data,",0,");

		 // ��ȡ8λ�ߵ͵�ƽ��Ч��־��1Ϊ����Ч��0Ϊ����Ч
		 Num[0] = Num[1] = Num[2] = Num[3] = 0;
		ByteToHexChar(HighLowLevel,(char*)Num);
		strcat( AckBuffer.Data,(char*)Num);

		 AckBuffer.Length = strlen(AckBuffer.Data);


	}
	else if (Type=='S')
	{
		if(PSC[(5<<1)+1]>16)
		{
			AckBuffer.Error = TRUE;
			return;
		}
		
		SetBufferValue(Num,16,0);
		Len =   *(pPS+(5<<1)+1);
		memmove(Num, &OneFrame[PSC[5<<1]], Len);	
		s = CprDataValue(Len, 12 ,(uchar) '0', (uchar) '9', Num);
		if (s==1)
		{
			Uart.puts("�ֻ��������");
			AckBuffer.Error = TRUE;
			return ;
		}

		SetBufferValue(sTelNum.Sim, LEN_SMS_NUMBER, 0);
		sTelNum.Sim[0] = 0x30;
	   memmove((uchar*)&sTelNum.Sim[1],&Num[0],11);
		s = strlen((char *)&sTelNum.Sim[0]);
		DFE.Write(EE_SIM_NUMBER,16,(uchar*)&sTelNum.Sim[0]);
		NOP(); NOP(); NOP();
	
		DecCharToPressBCD(s,(char *)sTelNum.Sim,SimNumberBCD);
		memmove(sTelNum.SimBCD_JT, SimNumberBCD, 6);	
		AckBuffer.GPS = TRUE;

		// �û�����
		SetBufferValue( sTelNum.User,LEN_SMS_NUMBER,0);
		memmove(sTelNum.User,&OneFrame[PSC[6<<1]],PSC[(6<<1)+1]);
		strcpy( (char*)JT808_Number.Center,(char*)sTelNum.Center);
		DFE.Write(EE_USER_NUMBER,16,(uchar*)sTelNum.User);
		DFE.Write(EE_JT_CAR_OWNER_PHONE_NUM, 16,(uchar*)sTelNum.User);

		// ���ĺ���
		SetBufferValue( sTelNum.Center,LEN_SMS_NUMBER,0);
		memmove(sTelNum.Center,&OneFrame[PSC[7<<1]],PSC[(7<<1)+1]);
		strcpy( (char*)JT808_Number.Center,(char*)sTelNum.Center);
		DFE.Write(EE_CENTER_NUMBER,16,(uchar*)sTelNum.Center);
		DFE.Write(EE_JT_CENTER_NUMBER,16,(uchar*)sTelNum.Center);

		// ����ϵ��
		VehiclePulseKV = Str2Int ( (char*)&OneFrame[PSC[8<<1]] );
		DFE.Write( FM_VEHICLE_PULSE_KV,4,(uchar*)&VehiclePulseKV);
		
		// ���ͳ����Դ	
		Num[0] = OneFrame[PSC[9<<1]];
		if( Num[0]=='G')			UseGPS_Mil = Num[0] = 0;	
		else if( Num[0]=='P')	UseGPS_Mil = Num[0] = 1;		
		DFE.Write( FM_USE_GPS_MIL, 1, Num );

		//// ����������
		memmove(Num,&OneFrame[PSC[10<<1]],8);
		Num[8] = Num[9] = 0;
		JT808_Alarm_Word.Shield = HexCharToInt((char*)Num);
		DFE.Write(EE_JT_SHIELD_WORD, 16,(uchar *)&JT808_Alarm_Word.Shield);

		// ���߱�ʶ
		Num[0] = OneFrame[PSC[11<<1]];
		if( Num[0]>'0')			EnableSysSleep = Num[0] = TRUE;	
		else 							EnableSysSleep = Num[0] = FALSE;		
		DFE.Write( FM_ENABLE_SYS_SLEEP, 1, Num );

		// �Զ��ϱ���¼�����ݱ�ʶ
		Num[0] = OneFrame[PSC[12<<1]];
		if( Num[0]>'0')			EnableVDR_Upload = Num[0] = TRUE;	
		else 							EnableVDR_Upload = Num[0] = FALSE;		
		DFE.Write( FM_ENABLE_VDR_UPLOAD, 1, Num );

		// ����8λ�ߵ͵�ƽ��Ч��־��1Ϊ����Ч��0Ϊ����Ч
		Num[0] = OneFrame[PSC[13<<1]];
		Num[1] = OneFrame[PSC[13<<1]+1];
		Num[2] = 0;
		HighLowLevel  = HexCharToInt((char*)Num);
		DFE.Write(EE_HIGH_LOW_LEVEL,2,(uchar*)&HighLowLevel);


	}
	
}






