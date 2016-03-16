/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												 
$										 
$                       	 		  
$								  		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ 
                                      �ļ�����
	�ļ���: 	 GSM.c
	����������GSM��غ���
	���뻷����
	Ŀ��CPU: 
	���ߣ�
	����ʱ�䣺
	����༭������UltraEdit 11.00a+  �Ʊ�����Ϊ�����ַ�������ɶ��Ը��á�
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/					  

#include			"..\Config\RTOS.h"
#include			"ATCommand.h"
#include			"GSM_GPRS.h"
#include			"..\GPS\GPS.h"

#define			DOWN_UP_STRU_GLOBALS		1
#include			"DownUpLoadDataStruct.h"
#include			"..\Common\Algol.h"
#include			"..\Common\FLASH_Addr.h"
#include			"..\UART\UART0.h"
#include			"..\UART\UART1.h"
#include			"..\Common\EEPROM_Addr.h"
#include			"..\Common\GB_T19056.h"
#include			"..\Common\GY_T19056.h"

//#include		"..\IIC\IIC0.h"
#include			"..\RTC\RTC.h"
#include			"JT_T808.h"
#include			"ATCmd.h"


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�GPRS_Pack
   ����������GPRS���ݴ������
   ��ڲ������������ͱ�־
   ���ڲ�����
   ȫ�ֱ���: struct GPS, struct T485DataPack.T485Data
   ����ʱ�䣺
   �޸�ʱ�䣺
	ע����Ϊ�������ʹ�õ����ݱȽ���ɢ���ʲ������ָ�룬������ȫ�ֱ���
*XY,1019000031,V1,094642,V,
*/ 

void			GPRSCls::TH_NewDataPack(uchar *pCmd,uchar *pGPS_VS)
{	
	uint16	Length;
	static	uchar		Busing;
	uchar		PackBuffer[256];
	NOP();	NOP();	NOP();
	
	Length = strlen((char*)pGPS_VS);
	if(Length>200)		return;
	
		
	while(Busing)
	{	os_dly_wait(1);	}	
	Busing = 1;	
	sOEM_ID[16] = 0;
	sOEM_ID[17] = 0;
		
	SetBufferValue((uchar*)PackBuffer,256,0);
	strcpy((char*)PackBuffer,(char*)sPackHead);	
	strcat((char*)PackBuffer,",\0");							 // 
	strcat((char*)PackBuffer,(char*)sOEM_ID);				// 
	strcat((char*)PackBuffer,",");
	strcat((char*)PackBuffer,(char*)pCmd);					// 
	strcat((char*)PackBuffer,",");
	Length = strlen((char*)PackBuffer);
	if(pGPS_VS)
		strcat((char*)PackBuffer,(char*)pGPS_VS);		// 
	else
		GPS.TH_DataPack((char*)&PackBuffer[Length]);
	NOP();	NOP();	

	if((TD.GPS==tGPS_PACK_FLAG)&&(pGPS_VS))
		Uart.puts((char*)pGPS_VS);
	///////////////////////////////////////////////// ׷�ӻس�
	strcat((char*)PackBuffer,"\r\n\0");
	LastPackLen = strlen((char*) PackBuffer);					
	memmove((uchar*)LastPackBuffer,(uchar*)PackBuffer,LastPackLen);
	SystemLog.GPRS.PushTotalBytes += LastPackLen;
	//SystemLog.GPS.PackTotalBytes += LastPackLen;
	
	PushAlignment(0,0,LastPackLen,(uchar*)PackBuffer);		// ���			
		
	if(TD.GPRS == tGPRS_PACK_DATA)
	{
		Uart.putb(1,LastPackLen,(uchar*)PackBuffer);
		os_dly_wait(2);
	}		
	Busing = 0;
	NOP();	NOP();
	
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�GB_NewDataPack(uint16 Len,uchar *pData)
   ����������
   ��ڲ�����
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
	
*XY,1019000031,V1,094642,V,
*/ 

void			GPRSCls::GB_NewDataPack(uchar Cmd,uchar Status,uint16 Len,uchar *pData)
{
	
}



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
  �������ƣ�JT808_Packaging
  �������������ݰ�JT808��ʽ���
  ��ڲ���: ָ��ID��Դ���ȡ��ܰ������ְ���Դ���ݡ�Ŀ������
  ���ڲ���������Ŀ�ĳ��ȼ�Ŀ������
  ȫ�ֱ���: 
  ����ʱ�䣺
  �޸�ʱ�䣺
  ע����Pack=0xffffffffʱ������ת��
*/
extern		uchar			jt_auth_ok_flag,CreateUserFlag;
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
uint16		GPRSCls::JT808_Packaging(uint16 Cmd,uint16 SrcLen,uint32 Pack,uchar *pSrcData,uchar *pDstData)
{
	uchar		s;
	uint16	SendIndex;
	union
	{
		uchar		Buffer[4];
		uint16	B16[2];
		uint32	B32;
	} Tp;
	uint16	i,j;
	
	pJT808_MSG_FRAME	pJT;

	pJT = (pJT808_MSG_FRAME)pDstData;

	Tp.B32 = 0;

	////////////////////////////////////////////////// ��Ϣͷ
	pJT->Head 	= 0x7e;
	pJT->Cmd 	= SwapINT16(Cmd);								// ����
	pJT->Attr.B16 	= SrcLen;
	if( (Pack>0)&&(Pack<0xffffffff) )		pJT->Attr.Bits.MultiPack = 1;	
	pJT->Attr.B16 =  SwapINT16(pJT->Attr.B16);			// ���ȡ�����
	memmove( pJT->SimNum,&sTelNum.SimBCD_JT,6);			// �ֻ�����
	SendIndex = SystemLog.Other.UpLoadIndex;
	SystemLog.Other.UpLoadIndex++;
	pJT->Index = SwapINT16(SendIndex);						// ��ˮ��
	i = 0;
	if( (Pack>0)&&(Pack<0xffffffff) )
	{
		Tp.B32 = Pack;
		pJT->Data.PackNo.TotalPack = 	SwapINT16(Tp.B16[0]);
		pJT->Data.PackNo.PackIndex = 	SwapINT16(Tp.B16[1]);
		i += 4;
	}
	memmove(&(pJT->Data.Buffer[i]),pSrcData,SrcLen);
	i += 12;		// ��Ϣͷ���� 12B �� 16B
	i += SrcLen;

	s = CalXorSum( i, (pDstData+1) );
	*(pDstData+i+1) = s;
	i++;

	if(Pack<0xffffffff)
		i = EscapeCharEncode(i,(pDstData+1) );	// ת�崦������У��ͣ�
	
	i++;		// β��ʶ0x73
	*(pDstData+i) = 0x7e;
	i++;
	
	
	if(gCnt.TickForRandData&0x02)
	{
		for(j=0;j<i;j++)
		{
			if((j%3)==0)	*(pDstData+j) ^= gKey.A;
		}
	}

	return i;
}




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�JT_T808_NewDataPack
   ������������JT����T808��ʽ���(�����Ǽ���)
   ��ڲ�����������ȣ��ְ�������
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
	Pack: ��16λΪ�ܰ�������16λΪ����
	ע��������ֻ������Ϣͷ�Ķ��뷽ʽ�����������ݶεĶ��뷽ʽ��
		������ֻ�Ե�λ����Ϣ�������ת���洢��Flash�����Զ�����Ϣ�����������

*/ 
#include		"..\Common\Algol.h"
#define		LEN_APPEND					4			// ����������֡���ݣ����踽����Ϣ�洢��Flash
#define		LEN_JT_DO_PACK		     	1224

#pragma		pack(1)
struct														
{
	uint16	Len;	
	uint16	AckID;	
	uchar		Buffer[LEN_JT_DO_PACK];	
	uchar		Busing;
}gPack,gPackSend;


void			GPRSCls::JT808_DataPackPushAlignment(uint16 AckID,uint16 Cmd,uint16 Len,uint32 Pack,uchar *pData)
{
	uchar		AlarmFlag = 0;
	union
	{
		uchar		Buffer[4];
		uint16	B16[2];
		uint32	B32;
	} Tp;	
	uint16	i;
		
	Tp.B32 = 0;
	while(gPack.Busing)	
	{	
	   os_dly_wait(1);
		if (Tp.B32++ > 2000)	 // ����20�룬����Щ�˷���
		{
			Tp.B32 = 0;
			return;	
		}		
	}	
	gPack.Busing = 1;	

	if(Cmd==eBB_gCMD_TER_POS_REPORT)		// �ж��Ƿ��б�����־
	{
		Tp.Buffer[0] = *(pData+0);
		Tp.Buffer[1] = *(pData+1);
		Tp.Buffer[2] = *(pData+2);
		Tp.Buffer[3] = *(pData+3);
		if(Tp.B32)	AlarmFlag = TRUE;
	}

	SetBufferValue(gPack.Buffer, LEN_JT_DO_PACK, 0);

	gPack.AckID = AckID;	 	// Ӧ��Ƚ�ʹ��

	i = JT808_Packaging(Cmd,Len,Pack,pData,gPack.Buffer);

	gPack.Len = i;			// ת��������֡����

	if (PDU_SMS.Ack==TRUE)
	{
		if(i>140)
		 i = 140;
		SMS.SetPduDataMessage(SMS_8BIT, (char *)SMS_SendNumber,gPack.Buffer, i);
	}
	else if(Uart0CommandFlag==TRUE)
	{
		Uart.putb(1,i,(uchar*)gPack.Buffer);
	}						
	else 
	{
		SystemLog.GPRS.SendTotalPack++;		
		SystemLog.GPRS.PushTotalBytes += i;			// ���ӳ����ۼ�  

		if(  Cmd == eBB_gCMD_TER_POS_REPORT  )
		{
			PushAlignmentToFlash(AlarmFlag);			// ��ӣ��洢��Flash			
		}
		else
		{
			PushAlignment(gPack.AckID,Cmd,gPack.Len,gPack.Buffer);	
		}
		
		if(TD.GPRS == tGPRS_PACK_DATA)
		{
			Uart.putb(1,i+LEN_APPEND,(uchar*)&gPack);
			os_dly_wait(1);	
		}	
	}
	gPack.Busing = 0;
	NOP();	NOP();
	
}






/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�GPRS_PushAlignment
   ������������GPRS�������뷢�Ͷ��У��������������ʧ�����������
   ��ڲ������������ͣ����ȣ�ָ��
   ���ڲ�������ӳɹ���־��0�ɹ�
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/
extern	uchar			jt_auth_ok_flag;
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
uchar			GPRSCls::PushAlignment(uint16 AckID,uint16 MsgID,uint16 Len,uchar *pData)
{
	uint16	FreeLen;
	union
	{
		uchar		Buffer[8];
		uint16	B16[4];
		uint32	B32[2];
	}Tp;
	NOP();	NOP();

	if(Len<3)		return 	0xff;
	while(SendAlig.Busy)	
	{		
		os_dly_wait(1);	
	}
	SendAlig.Busy = TRUE;
	FreeLen = GPRS_ALIG_MAX_LEN-2;		// ����ռ���ֽ�
	FreeLen -= SendAlig.Length;
	////////////////////////////////////////////////////////// �ж϶����Ƿ���
	if(FreeLen<Len)
	{
		SendAlig.Busy = 0;
		return ERROR;			// ���в����ռ�	
	}
	////////////////////////////////////////////////////////// ��Ӵ���// ͨ��Ӧ��ר�û���
	if( (MsgID==eBB_gCMD_TER_ACK)||(MsgID==eBB_gCMD_TER_AUTH)||(MsgID==eBB_gCMD_TER_REG)||(MsgID==eBB_gCMD_TER_DEREG) )		
	{	
		if ((MsgID==eBB_gCMD_TER_DEREG))
		{
			NOP(); 
		}			
		else if( jt_auth_ok_flag == 0 )		// δ��Ȩ��ֻ�����Ȩ��ע��ʹ�øû���
		{
			if( ( MsgID != eBB_gCMD_TER_AUTH )&&(MsgID != eBB_gCMD_TER_REG))	
			{				
				SendAlig.Busy = 0;	return 0;
			}
		}
		if( Len<LEN_PRIORITY_POP_BUFFER)
		{			
			SendAlig.gPriorityLen = Len;
			SendAlig.gPriorityID = MsgID;
			memmove( SendAlig.gPriorityBuffer,pData,Len);			
		}
	}
	else
	{
		Tp.B16[0] = Len;
		Tp.B16[1] = AckID;
		memmove(&SendAlig.Buffer[SendAlig.Length],Tp.Buffer,LEN_APPEND);	// ����
		SendAlig.Length += LEN_APPEND;
		memmove(&SendAlig.Buffer[SendAlig.Length],pData,Len);		// ����
		SendAlig.Length += Len;	
		if(TD.GPRS==tGPRS_U0_GPRS)		Uart.puts("����͸����GPRS����");
	}
	
	if(TD.GPRS==tGPRS_FIFO_DATA)
	{	
		Uart.puts((char*)" ��� ");
		SetBufferValue(Tp.Buffer,8,0);
		Int2Str( SendAlig.Length,(char*)Tp.Buffer);
		Uart.puts((char*)Tp.Buffer);
	}	
	SendAlig.Busy = 0;
	return OK;
}






uchar		GPRSCls::SendData(uchar ch,uint16 DataLen,uchar *pData)
{
	#define		LEN_T_BUFFER		(64+4)
	uchar		t;
	char		atSend[LEN_T_BUFFER];
	char		*pS;

	if( (DataLen==0)||(DataLen>1024)	)		
	{
		Uart.puts("Send Data Len Error Test Data:"); Uart.putb(1,2,(uchar *)&DataLen); Uart.puts("\r\n");
		return OK;
	}
	while (AT_Command_Busing == TRUE)
	{
		os_dly_wait(1);
	}
	AT_Command_Busing = TRUE;
	if (TD.GPRS == tGPRS_AT_ACK_CHECK)			   // for debug
	{
		NOP();	NOP();	//LPC_UART0->THR = 'B'; 	LPC_UART0->THR = '3'; 
	}
	TC.GPRS_LongTimeNoSend = 0;
	GG_Status.SendTotalBytes += DataLen;		// ��ǰSocketͨ�������ֽ���
	KeyDisp.SetLED_Status(LED_GPRS,LED_GPRS_SEND_DATA);
	
	SetBufferValue((uchar*)atSend,LEN_T_BUFFER,0);			
	strcpy((char *)atSend,"AT+QISEND=\0");
	Int2Str(sIP.Socket[ch],&atSend[10]);
	strcat(atSend, ",\0");
	t = strlen(atSend); 
	Int2Str(DataLen,(char*)&atSend[t]);	
	strcat((char *)atSend,"\r\0");
	
	Uart1.WaitSendFree();	// �ȴ����Ϳ���
	Uart1.ClearReceiveFlag(0);
	Uart1.puts((char *)atSend);				// ��������
	if (TD.GPRS==tGPRS_TMP_TEST) { Uart.puts((char*)atSend); }
	os_dly_wait(20);
   
	SetBufferValue((uchar*)atSend,LEN_T_BUFFER,0);					
	Uart1.ClearReceiveFlag(1);
	memmove(U1_TxdBuffer,pData,DataLen);
	Uart1.putb(1,DataLen,(uchar*)NULL);

	if (TD.GPRS==tGPRS_TMP_TEST)
	{
		Uart.puts("QSend Test Data:"); 	Uart.putb(1,DataLen,pData); Uart.puts("\r\n");
	
	}
						
	DataLen = Uart1.WaitRecBuffer(1000,LEN_T_BUFFER,(uchar*)atSend);	 // 
	if(DataLen)
	{	
		pS = MyStrStr(atSend,(char*)"SEND OK\0");
		if( pS != NULL)
		{
			AT_Command_Busing = FALSE;	
			return OK;
		} 
	}
	// �ٵȴ�һ�£�ȷ���Ƿ��ͳɹ�		// added 2013.4	
	DataLen = Uart1.WaitRecBuffer(1000,LEN_T_BUFFER,(uchar*)atSend);
	if (DataLen)
	{
		pS = MyStrStr(atSend,(char*)"SEND OK\0");
		if( pS != NULL)	
		{
			AT_Command_Busing = FALSE;	
			return OK;							
		}
	}
	AT_Command_Busing = FALSE;				
	return ERROR;
}	







/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�GPRS_PushAlignment
   ������������GPRS�������뷢�Ͷ��У��������������ʧ�����������
   ��ڲ������������ͣ����ȣ�ָ��
   ���ڲ�������ӳɹ���־��0�ɹ�
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
uchar			GPRSCls::PushAlignmentToFlash(uchar AlarmFlag)
{
	uint32	i,Len,Addr;
	uchar		tBuffer[64];
	if(AlarmFlag)
	{
		Len = gPack.Len+LEN_APPEND;	
		for(i=0;i<Len;i+=256)
		{
			Addr = FF_ALARM_MSG_START_PAGE;
			Addr += (FlashPageCnt.AlarmMsgSave%FF_ALARM_MSG_TOTAL_PAGE);
			if( Addr%FF_PAGE_PER_SECTOR == 0 )
			{
				DFF.SectorEraseWaitOK(Addr/FF_PAGE_PER_SECTOR,10);			// ��������				
			}		
			DFF.PageProgramWaitOK( Addr,(uchar*)&gPack,3);
			FlashPageCnt.AlarmMsgSave++;
			if(TD.GPRS == tGPRS_TMP_TEST )	strcpy((char*)tBuffer,"\r\n������Ϣ��Flash: ");
		}
	 }
	 else
	 {
		Len = gPack.Len+LEN_APPEND;
	 	for(i=0;i<Len;i+=256)
		{
			Addr = FF_NORMAL_MSG_START_PAGE;
			Addr += (FlashPageCnt.NormalMsgSave%FF_NORMAL_MSG_TOTAL_PAGE);
			if( Addr%FF_PAGE_PER_SECTOR == 0 )
			{
				DFF.SectorEraseWaitOK(Addr/FF_PAGE_PER_SECTOR,10);			// ��������				
			}
			DFF.PageProgramWaitOK( Addr,(uchar*)&gPack,3);
			FlashPageCnt.NormalMsgSave++;
			if(TD.GPRS == tGPRS_TMP_TEST )	strcpy((char*)tBuffer,"\r\n��ͨ��Ϣ��Flash: ");
		}
	 }
	 if(TD.GPRS == tGPRS_TMP_TEST )
	 {
		Len = strlen( (char*)tBuffer);
		Int2Str( FlashPageCnt.AlarmMsgRead,(char*)&tBuffer[Len]);
		
		strcat((char*)tBuffer,",");
		Len = strlen( (char*)tBuffer);
		Int2Str( FlashPageCnt.AlarmMsgSave,(char*)&tBuffer[Len]);
		
		Uart.puts((char*)tBuffer);
	}
	return OK;
}	





/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�GPRS_PopAlignment
   ������������GPRS�������뷢�Ͷ���
   ��ڲ������ź�ǿ�ȣ��ɸ����ź����жϷ��������ֽ���
   ���ڲ��������ͳ���
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/
extern		uchar			SendGnssDataEnable;
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
uint16			GPRSCls::PopAlignmentSend(uchar Signal)
{	
	static	uchar		ReadErCnt[4];
	static	uchar		PageCnt;
	uchar		SendOK=0;
	uint16	ch,Len = 0;
	uint32	i,Addr,Offset;
	union
	{
		uchar		Buffer[768+16];
		uint16	B16[768/2];
		uint32	B32[768/4];
	}Tp;
	uchar		*pData;
	
	NOP();	NOP();
	/////////////////////////////////////////////////////////////////////////////
	if(GSM.WaitGSM_Free()==ERROR)		return ERROR;
	Addr = 0;

	while(gPackSend.Busing)	
	{	
	   os_dly_wait(1);
		if (Addr++ > 2000)	 // ����20�룬����Щ�˷���
		{
			Addr = 0;
			Uart.puts("����æ����ʱ���� ");
			return 0;	
		}		
	}	
	gPackSend.Busing = 1;		
	//KeyDisp.SetLED_Status(LED_GPRS,LED_GPRS_SEND_DATA);
	
	if( ReSend.OverTime )	
	{
		gPackSend.Busing = 0;
		if(TD.GPRS == tGPRS_TMP_TEST )	Uart.puts(" ���ӣ��ȴ�Ӧ�� ");
		return OK;		// δ�յ�Ӧ�𣬷��أ��ȴ���ʱ
	}

	if	( ReSend.Times>=3)	 	// ��෢3��
	{				
		ReSend.Times = 0;	
		
		if( (TestMultiCenter==TRUE)&&(gPackSend.AckID==eBB_gCMD_TER_AUTH) )
		{
			if(PageCnt>=3)		// ���μ�Ȩδ�ɹ������Ӹ�������
			{
				sIP.EnableFlag[eFLAG_TCP1] = ENABLE_FLAG;
				sIP.EnableFlag[eFLAG_TCP] = 0;
				GG_Status.Flag.Word = GG_FLAG_INIT_DATA;	 	// ���³�ʼ��
				Uart.puts("���μ�Ȩδ�ɹ������Ӹ������� ");
			}
		} 
		if(TD.GPRS == tGPRS_TMP_TEST )	Uart.puts(" ���ӣ������η��� ");		
	}
	if( ReSend.Times==0)		//   
	{	
		if(gPackSend.AckID!=eBB_gCMD_TER_AUTH)
		{
			PageCnt = 0;
		}
		ReSend.DataType = 0;
		ReSend.PreOverTime = SendOVT.TCP_ACK_OverTime; 
	}
	ReSend.OverTime = ReSend.PreOverTime*(ReSend.Times+1);	
	ReSend.PreOverTime = ReSend.OverTime;
	
	if( (IC_Card_Auth_GPRS==TRUE)||(Creat_User_Reging == TRUE) )		// IC����֤���ݣ�������ȼ�
	{
		Len = 0;
		gPackSend.Len = 0;
		SetBufferValue(Tp.Buffer,64,0);
		if(SendAlig.Length)		// ���ȷ��Ͳ�������Flash�����ݡ����β��ɹ�����������
		{
			//Uart.puts("����4 ");  
			
			while( SendAlig.Busy )	{ os_dly_wait(1);	}
			SendAlig.Busy = TRUE;
							
			// ��1������֡���ݳ���
			Tp.Buffer[0] = SendAlig.Buffer[0];
			Tp.Buffer[1] = SendAlig.Buffer[1];	
			Len = Tp.B16[0];
			gPackSend.Len = Len;
			
			// Ӧ��ID
			Tp.Buffer[0] = SendAlig.Buffer[2];		// ID����֡�����С�˶���
			Tp.Buffer[1] = SendAlig.Buffer[3];
			gPackSend.AckID = Tp.B16[0];

			// ��ϢID
			Tp.Buffer[62] = SendAlig.Buffer[6];	  	// ֡�������ID����˶���
			Tp.Buffer[63] = SendAlig.Buffer[5];
						
			/////////// ��ȡ������֡����
			memmove(gPackSend.Buffer,&SendAlig.Buffer[LEN_APPEND],Len);

			Uart.puts("IC Card Auth Test Data:");		 	// ��ӡ�����ڣ���λ����16����������ʾ֮
			Uart.putb(1,(Len+LEN_APPEND),(uchar*)&gPackSend);
			Uart.puts("\r\n\0");
			
						
			Len += LEN_APPEND;
			SendAlig.Length -= Len;
			memmove(SendAlig.Buffer,&SendAlig.Buffer[Len],SendAlig.Length);
			
			SetBufferValue(Tp.Buffer,62,0);
			ReSend.DataType = POP_DATA_TYPE_RAM;
			strcpy((char*)Tp.Buffer,"IC����֤��Ϣ���ӣ�ID: ");
			IntToHexChar( Tp.B16[31], 4, (char*)&Tp.Buffer[22]);	
			if( (Tp.B16[31] > 0x8fff )||(gPackSend.Len>=1024) )
			{
				InitAlig('A');
				gPackSend.Len = 0;
			}
			SendAlig.Busy = 0;		
		}
		if(TD.GPRS == tGPRS_TMP_TEST )	Uart.puts(" ���ӣ�IC����֤ ");
		Tp.Buffer[0] = 0;
	}	
	else if(SendAlig.gPriorityLen)		 	// �����ȼ�����
	{
		if( ReSend.Times==0)			// �״η��ͣ���ȡ�����͵����ݣ��ط����ٶ�ȡ
		{
			ReSend.DataType = POP_DATA_TYPE_RAM;
			while( SendAlig.Busy )	{ os_dly_wait(1);	}
			SendAlig.Busy = TRUE;
			
			if( jt_auth_ok_flag == 0 )
			{				
				if( ( SendAlig.gPriorityID != eBB_gCMD_TER_AUTH )&&(SendAlig.gPriorityID!=eBB_gCMD_TER_REG)&&(SendAlig.gPriorityID!=eBB_gCMD_TER_DEREG))	
				{	 // δ��Ȩ��ֻ����ע����Ȩ����
					 SendAlig.Busy = gPackSend.Busing = 0;	
					 return 0;
				}
			}

			if( SendAlig.gPriorityID == eBB_gCMD_TER_AUTH )		PageCnt=0;		// ��Ȩ��������
			
			memmove(gPackSend.Buffer,&SendAlig.gPriorityBuffer,SendAlig.gPriorityLen);
			gPackSend.Len = SendAlig.gPriorityLen;
			gPackSend.AckID = SendAlig.gPriorityID;
			SendAlig.gPriorityLen = 0;
			SendAlig.Busy = 0;
			if( ( SendAlig.gPriorityID != eBB_gCMD_TER_AUTH )&&(SendAlig.gPriorityID!=eBB_gCMD_TER_REG))	
				ReSend.Times = ReSend.OverTime = 0;		// ͨ��Ӧ��������Ӧ��
			strcpy((char*)Tp.Buffer,"�����ȼ����� ");	
			Tp.Buffer[0] = 0;
		}
		if(TD.GPRS == tGPRS_TMP_TEST )	Uart.puts(" ���ӣ���������ȼ� ");
	}
	else if( jt_auth_ok_flag  )
	{			
		if( ReSend.Times==0)			// �״η��ͣ���ȡ�����͵����ݣ��ط����ٶ�ȡ
		{
			Len = 0;
			gPackSend.Len = 0;
			SetBufferValue(Tp.Buffer,64,0);
			if(SendAlig.Length)		// ���ȷ��Ͳ�������Flash�����ݡ����β��ɹ�����������
			{
				//Uart.puts("����4 ");  
				ReSend.DataType = POP_DATA_TYPE_RAM;
				while( SendAlig.Busy )	{ os_dly_wait(1);	}
				SendAlig.Busy = TRUE;
								
				// ��1������֡���ݳ���
				Tp.Buffer[0] = SendAlig.Buffer[0];
				Tp.Buffer[1] = SendAlig.Buffer[1];	
				Len = Tp.B16[0];
				gPackSend.Len = Len;
				
				// Ӧ��ID
				Tp.Buffer[0] = SendAlig.Buffer[2];		// ID����֡�����С�˶���
				Tp.Buffer[1] = SendAlig.Buffer[3];
				gPackSend.AckID = Tp.B16[0];

				// ��ϢID
				Tp.Buffer[62] = SendAlig.Buffer[6];	  	// ֡�������ID����˶���
				Tp.Buffer[63] = SendAlig.Buffer[5];
				
				
				/////////// ��ȡ������֡����								
				memmove(gPackSend.Buffer,&SendAlig.Buffer[LEN_APPEND],Len);
							
				Len += LEN_APPEND;
				SendAlig.Length -= Len;
				memmove(SendAlig.Buffer,&SendAlig.Buffer[Len],SendAlig.Length);
				
				
				
				SetBufferValue(Tp.Buffer,62,0);				
				strcpy((char*)Tp.Buffer," ������Ϣ���ӣ�ID: ");
				IntToHexChar( Tp.B16[31], 4, (char*)&Tp.Buffer[19]);
				strcat((char*)Tp.Buffer," ");
				if( (Tp.B16[31] > 0x8fff )||(gPackSend.Len>=1024) )
				{
					InitAlig('A');
					gPackSend.Len = 0;
				}
				SendAlig.Busy = 0;
				Tp.Buffer[0] = 0;
			}
			
			else if(FlashPageCnt.AlarmMsgRead<FlashPageCnt.AlarmMsgSave)	// ������Ϣ��ÿҳ���Ȳ��̶�
			{	// ������Ϣ����ʱ�Ѵ���ã����Գ���ʱ�����ٴδ��
				ReSend.DataType = POP_DATA_TYPE_ALARM;
				strcpy((char*)Tp.Buffer," ������Ϣ���ӣ�");
				Len = strlen((char*)Tp.Buffer);
				Int2Str( FlashPageCnt.AlarmMsgRead, (char*)&Tp.Buffer[Len]);		
				strcat((char*)Tp.Buffer,",");
				Len = strlen((char*)Tp.Buffer);
				Int2Str( FlashPageCnt.AlarmMsgSave, (char*)&Tp.Buffer[Len]);		
				strcat((char*)Tp.Buffer," ");
   		
				Addr = FF_ALARM_MSG_START_PAGE;
				Addr += (FlashPageCnt.AlarmMsgRead%FF_ALARM_MSG_TOTAL_PAGE);
				DFF.PageRead( Addr, (uchar*)&gPackSend);
				if(gPackSend.Len==0)		 	
				{	// ֡����Ϊ0��ָ����һҳ�����򷢴�ʧ�ܣ���ֹͣ����һҳ��
					FlashPageCnt.AlarmMsgRead++;
					Uart.puts(" ���ӣ���������Ϣ����Ϊ�㣬���� ");
					PageCnt = 0;
				}
				else
				{
					PageCnt = 1;
				}
				Len = gPackSend.Len+LEN_APPEND;
				if(Len > 1024 )
				{
					Len = gPackSend.Len = 250;
				}
								
				if(TD.GPRS == tGPRS_FIFO_DATA )
				{
					Uart.puts("\r\nALARM Test Data:");
					Uart.putb(1,Len,(uchar*)&gPackSend);
					Uart.puts("\r\n");
				}
			}			
			else if( FlashPageCnt.HistoryRead < FlashPageCnt.HistorySave )		// ���Ȳ��ԣ��̶�����252B���ɼ����С��5��ʱ
			{				
				ReSend.DataType = POP_DATA_TYPE_HISTORY;
				strcpy((char*)Tp.Buffer,"\r\n��ʷ��Ϣ���ӣ�");
				Len = strlen((char*)Tp.Buffer);
				Int2Str( FlashPageCnt.HistoryRead, (char*)&Tp.Buffer[Len]);		
				strcat((char*)Tp.Buffer,",");	 
				Len = strlen((char*)Tp.Buffer);
				Int2Str( FlashPageCnt.HistorySave, (char*)&Tp.Buffer[Len]);		
				strcat((char*)Tp.Buffer," ");
				Uart.puts((char*)Tp.Buffer);   
				
				#if(TEST_CODE_JUST_FOR_TEST_LUOYANG_NEW==1)	// Ӧ�����ԣ����������������ϴ���ֻ�ܵ����ϴ�
				os_dly_wait(80);		// ��ʱ1�룬��Ҫ����̫�죬���������
				i = 0;
				Tp.Buffer[1] = Tp.Buffer[2] = Tp.Buffer[3] = 0xff;
				while( ((Tp.Buffer[1]==0xff)&&(Tp.Buffer[2]==0xff)&&(Tp.Buffer[3]==0xff))  )	//  Ϊ0xffʱ����ʧ��	
				{
					Addr = FF_GPS_MESSAGE_START_PAGE;
					Addr += FlashPageCnt.HistoryRead%FF_GPS_MESSAGE_TOTAL_PAGE;
					
					DFF.PageRead( Addr,Tp.Buffer);
					
					// �洢ʱ��9��/ҳ����ȡʱ��Ӧ�����ԣ�ÿ��ֻ�ܷ���1��
					Addr = FlashPageCnt.Numbers%PACK_NUM_PER_PAGE;
					if(Addr)		
					{
						memmove( Tp.Buffer, &Tp.Buffer[Addr*LEN_JT808_GPS_PACK],LEN_JT808_GPS_PACK);					
					}
					i++;
					if(i>5)	
					{
						Uart.puts("����ʷ���ݴ��� ");
						break;
					}
				}		
				FlashPageCnt.Numbers++;
				if( Addr == (PACK_NUM_PER_PAGE-1) )
					PageCnt = 1;
				else
					PageCnt = 0;			
				
				
				///////////////////////////////// 				
				SendOK = gCnt.TickForRandData;				// ��ʱʹ��SendOK�������
				SendOK &= 0x07;
				Tp.Buffer[11] |= SendOK;				// γ��
				Tp.Buffer[15] |= SendOK;				// ����
				Tp.Buffer[17] |= (SendOK&0x03);		// ����
				Tp.Buffer[19] |= SendOK;				// �ٶ�
				Tp.Buffer[21] |= SendOK;				// ����	
				
				Len = get_position_info(0xff,&Tp.Buffer[28],0);	
				Len += 28;		
				
				gPackSend.Len = JT808_Packaging( eBB_gCMD_TER_POS_REPORT,Len,0,Tp.Buffer,(uchar*)&gPackSend.Buffer);	
				gPackSend.AckID = eBB_gCMD_TER_POS_REPORT;
				
				Uart.puts("Test Data:\0");
				Uart.putb(1, Len, Tp.Buffer);
				Uart.puts("\r\n");
								
				#else //////////////////////////////////////////////////////////////////////////////
								
				pData = NULL;
				while( pData == NULL )	{ os_dly_wait(1);	pData = MyMalloc(600);	}
				
				Tp.Buffer[0] = 0;		Tp.Buffer[1] = 18;				// 18��λ����Ϣ
				Tp.Buffer[2] = 0;												// ��ͨλ����Ϣ�����ϱ�
								
				
				Addr = FF_GPS_MESSAGE_START_PAGE;
				Addr += FlashPageCnt.HistoryRead%FF_GPS_MESSAGE_TOTAL_PAGE;
				
				DFF.PageRead( Addr,pData);
				Addr = 3;
				for(i=0;i<PACK_NUM_PER_PAGE;i++)
				{
					Tp.Buffer[Addr++] = 0;
					Tp.Buffer[Addr++] = LEN_JT808_GPS_PACK;
					memmove( &Tp.Buffer[Addr], pData, LEN_JT808_GPS_PACK );
					Addr += LEN_JT808_GPS_PACK;
					pData += LEN_JT808_GPS_PACK;
				}
				
				Addr = FF_GPS_MESSAGE_START_PAGE;
				Addr += (FlashPageCnt.HistoryRead+1)%FF_GPS_MESSAGE_TOTAL_PAGE;
				DFF.PageRead( Addr,pData);
				Addr = ( (LEN_JT808_GPS_PACK+2)*PACK_NUM_PER_PAGE + 3 );
				for(i=0;i<PACK_NUM_PER_PAGE;i++)
				{
					Tp.Buffer[Addr++] = 0;
					Tp.Buffer[Addr++] = LEN_JT808_GPS_PACK;
					memmove( &Tp.Buffer[Addr], pData, LEN_JT808_GPS_PACK );
					Addr += LEN_JT808_GPS_PACK;
					pData += LEN_JT808_GPS_PACK;
				}
				MyFree(pData);			// �ͷ��ڴ�
				
				gPackSend.Len = JT808_Packaging( eBB_gCMD_TER_POS_INFO_MULTI_UPLOAD,Addr,0,Tp.Buffer,(uchar*)&gPackSend.Buffer);	
				gPackSend.AckID = eBB_gCMD_TER_POS_INFO_MULTI_UPLOAD;
				PageCnt = 2;	
				if(ReadErCnt[0]<3)
				{					
					if( ((Tp.Buffer[276]==0xff)&&(Tp.Buffer[277]==0xff)) || ((Tp.Buffer[6]==0xff)&&(Tp.Buffer[7]==0xff) )  )
					{	PageCnt = 0;	ReadErCnt[0]++;	}
					else					ReadErCnt[0] = 0;
				}
				else
				{
					ReadErCnt[0] = 0;
				}
				
				#endif
				Tp.Buffer[0] = 0;
							
			}		// ä��������ÿҳ�̶�����252B���ɼ����С��5��ʱ��ÿ�ζ�2ҳ������<(Save-1)
			else if( (FlashPageCnt.BlindSave>0)&&(FlashPageCnt.BlindRead < (FlashPageCnt.BlindSave-1)) )			
			{	
				os_dly_wait(80);	
				ReSend.DataType = POP_DATA_TYPE_BLIND;
				strcpy((char*)Tp.Buffer," ä����Ϣ���ӣ�");
				Len = strlen((char*)Tp.Buffer);
				Int2Str( FlashPageCnt.BlindRead, (char*)&Tp.Buffer[Len]);		
				strcat((char*)Tp.Buffer,",");
				Len = strlen((char*)Tp.Buffer);
				Int2Str( FlashPageCnt.BlindSave, (char*)&Tp.Buffer[Len]);		
				strcat((char*)Tp.Buffer," ");
				Uart.puts((char*)Tp.Buffer);  
				
				pData = NULL;
				while( pData == NULL )	{ os_dly_wait(1);	pData = MyMalloc(600);	}
				
				Tp.Buffer[0] = 0;		Tp.Buffer[1] = 18;				// 18��λ����Ϣ
				Tp.Buffer[2] = 1;												// ä��λ����Ϣ
								
				
				Addr = FF_GPS_BLIND_MESSAGE_START_PAGE;
				Addr += FlashPageCnt.BlindRead%FF_GPS_BLIND_MESSAGE_TOTAL_PAGE;
				DFF.PageRead( Addr,pData);
				Addr = 3;
				for(i=0;i<PACK_NUM_PER_PAGE;i++)
				{
					Tp.Buffer[Addr++] = 0;
					Tp.Buffer[Addr++] = LEN_JT808_GPS_PACK;
					memmove( &Tp.Buffer[Addr], pData, LEN_JT808_GPS_PACK );
					
					#if(TEST_CODE_JUST_FOR_TEST_LUOYANG_NEW==1)
					SendOK = gCnt.TickForRandData;			// ��ʱʹ��SendOK�������
					SendOK &= 0x07;
					Tp.Buffer[Addr+11] |= SendOK;				// γ��
					Tp.Buffer[Addr+15] |= SendOK;				// ����
					Tp.Buffer[Addr+17] |= (SendOK&0x03);	// ����
					Tp.Buffer[Addr+19] |= SendOK;				// �ٶ�
					Tp.Buffer[Addr+21] |= SendOK;				// ����
					#endif
					
					Addr += LEN_JT808_GPS_PACK;
					pData += LEN_JT808_GPS_PACK;
				}
				
				Addr = FF_GPS_BLIND_MESSAGE_START_PAGE;
				Addr += (FlashPageCnt.BlindRead+1)%FF_GPS_BLIND_MESSAGE_TOTAL_PAGE;
				DFF.PageRead( Addr,pData);
				Addr = ( (LEN_JT808_GPS_PACK+2)*PACK_NUM_PER_PAGE + 3 );
				for(i=0;i<PACK_NUM_PER_PAGE;i++)
				{
					Tp.Buffer[Addr++] = 0;
					Tp.Buffer[Addr++] = LEN_JT808_GPS_PACK;
					memmove( &Tp.Buffer[Addr], pData, LEN_JT808_GPS_PACK );
					
					#if(TEST_CODE_JUST_FOR_TEST_LUOYANG_NEW==1)
					SendOK = gCnt.SystemSecond;			// ��ʱʹ��SendOK�������
					SendOK &= 0x07;
					Tp.Buffer[Addr+11] |= SendOK;				// γ��
					Tp.Buffer[Addr+15] |= SendOK;				// ����
					Tp.Buffer[Addr+17] |= (SendOK&0x03);	// ����
					Tp.Buffer[Addr+19] |= SendOK;				// �ٶ�
					Tp.Buffer[Addr+21] |= SendOK;				// ����
					#endif
					
					Addr += LEN_JT808_GPS_PACK;
					pData += LEN_JT808_GPS_PACK;
				}
				MyFree(pData);			// �ͷ��ڴ�
				
								
				gPackSend.Len = JT808_Packaging( eBB_gCMD_TER_POS_INFO_MULTI_UPLOAD,Addr,0,Tp.Buffer,(uchar*)&gPackSend.Buffer);	
				gPackSend.AckID = eBB_gCMD_TER_POS_INFO_MULTI_UPLOAD;
				PageCnt = 2;	
				if(ReadErCnt[1]<3)
				{					
					if( ((Tp.Buffer[276]==0xff)&&(Tp.Buffer[277]==0xff)) || ((Tp.Buffer[6]==0xff)&&(Tp.Buffer[7]==0xff) )  )
					{	PageCnt = 0;	ReadErCnt[1]++;	}
					else					ReadErCnt[1] = 0;
				}
				else
				{
					ReadErCnt[1] = 0;
				}			
				Tp.Buffer[0] = 0;
			}
			else if(FlashPageCnt.NormalMsgRead<FlashPageCnt.NormalMsgSave)	  	// ��ͨ��Ϣ��ÿҳ���Ȳ��̶�
			{	// ��ͨ��Ϣ����ʱ�Ѵ���ã����Գ���ʱ�����ٴδ��
				ReSend.DataType = POP_DATA_TYPE_NORMAL;
				strcpy((char*)Tp.Buffer," ��ͨ��Ϣ���ӣ�");
				Len = strlen((char*)Tp.Buffer);
				Int2Str( FlashPageCnt.NormalMsgRead, (char*)&Tp.Buffer[Len]);		
				strcat((char*)Tp.Buffer,",");
				Len = strlen((char*)Tp.Buffer);
				Int2Str( FlashPageCnt.NormalMsgSave, (char*)&Tp.Buffer[Len]);		
				strcat((char*)Tp.Buffer," ");
				  		
				Addr = FF_NORMAL_MSG_START_PAGE;
				Addr += (FlashPageCnt.NormalMsgRead%FF_NORMAL_MSG_TOTAL_PAGE);
				DFF.PageRead( Addr, (uchar*)&gPackSend);
				if(gPackSend.Len==0)		 	
				{	// ֡����Ϊ0��ָ����һҳ�����򷢴�ʧ�ܣ���ֹͣ����һҳ��
					FlashPageCnt.NormalMsgRead++;
					PageCnt = 0;
					Uart.puts(" ����Ϊ0������ ");
				}				
				else
				{
					PageCnt = 1;
				}
				Len = gPackSend.Len+LEN_APPEND;
				if(Len > 1024 )
				{
					Len = gPackSend.Len = 250;
				}
				
				if(TD.GPRS == tGPRS_FIFO_DATA )
				{
					Uart.puts("\r\nNormal Test Data:");
					Uart.putb(1,Len,(uchar*)&gPackSend);
					Uart.puts("\r\n");
				}
			}
			else if((JT_RemoteReadRecodMoreDataCmd>=eGB_rCMD_GET_SPEED_PER_SECOND) && 
				(JT_RemoteReadRecodMoreDataCmd<=eGB_rCMD_GET_SPEED_STATUS_LOG))
			{
				
				ReSend.DataType = POP_DATA_TYPE_GET_08H_15H_DATA; 
				// ���� 08H - 15H�Ĵ���
				gPackSend.Len = RemoteGetVDR_Data_08H_15H(gPackSend.Buffer);

				gPackSend.AckID = eBB_gCMD_TER_VDR_DATA_UPLOAD; 
			}
			/*else if( EnableVDR_Upload==TRUE)				// �����ϱ���¼�����ݲ��ϱ�
			{
				if((SendGnssDataEnable==TRUE)&&(FlashPageCnt.GnssDataSave>=4)&&(FlashPageCnt.GnssDataRead<(FlashPageCnt.GnssDataSave-3)))	  	//
				{				
					ReSend.DataType = POP_DATA_TYPE_GNSS;
					strcpy((char*)Tp.Buffer," GNSS��Ϣ���ӣ�");
					Len = strlen((char*)Tp.Buffer);
					Int2Str( FlashPageCnt.GnssDataRead, (char*)&Tp.Buffer[Len]);		
					strcat((char*)Tp.Buffer,",");
					Len = strlen((char*)Tp.Buffer);
					Int2Str( FlashPageCnt.GnssDataSave, (char*)&Tp.Buffer[Len]);		
					strcat((char*)Tp.Buffer," ");
					
					
					pData = NULL;			// ͸��GNSSԭʼ���ݣ�ΪASCII�룬����ת��
					while( pData == NULL )	{ os_dly_wait(1);	pData = MyMalloc(1024);	}
					
					for(i=0;i<4;i++)
					{
						Addr = FF_GNSS_DATA_START_PAGE;
						Addr += ((FlashPageCnt.GnssDataRead+i)%FF_GNSS_DATA_TOTAL_PAGE);
						DFF.PageRead( Addr, (pData+1+i*256) );
					}
					PageCnt = 4;
					*pData = 0;
					Len = strlen( (char*)(pData+1));		// 0xffffffff��ʾ��ת��
					Len += 1;
					gPackSend.Len = JT808_Packaging( eBB_gCMD_TER_DATA_TRAN_UPLOAD,Len,0xffffffff,pData,(uchar*)&gPackSend.Buffer);	
					gPackSend.AckID = eBB_gCMD_TER_DATA_TRAN_UPLOAD;
					MyFree(pData);			// �ͷ��ڴ�
					os_dly_wait(100);
				}
				else if((FlashPageCnt.Speed48HSave>=3)&&(FlashPageCnt.Speed48HRead<(FlashPageCnt.Speed48HSave-2)))
				{
					ReSend.DataType = POP_DATA_TYPE_48H;
					strcpy((char*)Tp.Buffer," 48Сʱ���ݳ��ӣ�");
					Len = strlen((char*)Tp.Buffer);
					Int2Str( FlashPageCnt.Speed48HRead, (char*)&Tp.Buffer[Len]);		
					strcat((char*)Tp.Buffer,",");
					Len = strlen((char*)Tp.Buffer);
					Int2Str( FlashPageCnt.Speed48HSave, (char*)&Tp.Buffer[Len]);		
					strcat((char*)Tp.Buffer," ");
					Uart.puts((char*)Tp.Buffer);  
					
					
					Tp.Buffer[0] = GetVDR_Index[eGB_rCMD_GET_SPEED_PER_SECOND]>>8;
					Tp.Buffer[1] = GetVDR_Index[eGB_rCMD_GET_SPEED_PER_SECOND];		
					Tp.Buffer[2] = eGB_rCMD_GET_SPEED_PER_SECOND;
					
					Tp.Buffer[3] = 0x55;
					Tp.Buffer[4] = 0x7a;
					Tp.Buffer[5] = eGB_rCMD_GET_SPEED_PER_SECOND;
					Tp.Buffer[6] = 0x02;
					Tp.Buffer[7] = 0xf4;
					Tp.Buffer[8] = 0;
					
					for(i=0;i<3;i++)
					{
						Addr = FF_48H_SPEED_START_PAGE;
						Addr += ((FlashPageCnt.Speed48HRead+i)%FF_48H_SPEED_TOTAL_PAGE);
						DFF.PageRead( Addr, &Tp.Buffer[9+i*252] );
					}
									
					Tp.Buffer[9+252*3] = CalXorSum( (9+252*3),Tp.Buffer);
									
					PageCnt = 3;				
					gPackSend.Len = JT808_Packaging( eBB_gCMD_TER_VDR_DATA_UPLOAD,(9+252*3+1),0,Tp.Buffer,(uchar*)&gPackSend.Buffer);	
					gPackSend.AckID = eBB_gCMD_TER_VDR_DATA_UPLOAD;
					
					os_dly_wait(100);
					Tp.Buffer[0] = 0;
				}
				else if((FlashPageCnt.Location360HSave>=3)&&(FlashPageCnt.Location360HRead<(FlashPageCnt.Location360HSave-2)))
				{
					ReSend.DataType = POP_DATA_TYPE_360H;
					strcpy((char*)Tp.Buffer," 360Сʱ���ݳ��ӣ�");
					Len = strlen((char*)Tp.Buffer);
					Int2Str( FlashPageCnt.Location360HRead, (char*)&Tp.Buffer[Len]);		
					strcat((char*)Tp.Buffer,",");
					Len = strlen((char*)Tp.Buffer);
					Int2Str( FlashPageCnt.Location360HSave, (char*)&Tp.Buffer[Len]);		
					strcat((char*)Tp.Buffer," ");
					Uart.puts((char*)Tp.Buffer);  
					
					Tp.Buffer[0] = GetVDR_Index[eGB_rCMD_GET_POSITION_INFO]>>8;
					Tp.Buffer[1] = GetVDR_Index[eGB_rCMD_GET_POSITION_INFO];		
					Tp.Buffer[2] = eGB_rCMD_GET_POSITION_INFO;
					
					Tp.Buffer[3] = 0x55;
					Tp.Buffer[4] = 0x7a;
					Tp.Buffer[5] = eGB_rCMD_GET_POSITION_INFO;
					Tp.Buffer[6] = 0x02;
					Tp.Buffer[7] = 0x9a;
					Tp.Buffer[8] = 0;
									
					for(i=0;i<3;i++)
					{
						if(i==0)			Offset = 9;
						else if(i==1)	Offset = (9+226);
						else if(i==2)	Offset = (9+226+220);
						
						Addr = FF_360H_LOCATION_START_PAGE;
						Addr += ((FlashPageCnt.Location360HRead+i)%FF_360H_LOCATION_TOTAL_PAGE);
						DFF.PageRead( Addr, &Tp.Buffer[Offset] );
						
					}
					
					Tp.Buffer[9+666] = CalXorSum( (9+666),Tp.Buffer);
									
					PageCnt = 3;				
					gPackSend.Len = JT808_Packaging( eBB_gCMD_TER_VDR_DATA_UPLOAD,(9+666+1),0,Tp.Buffer,(uchar*)&gPackSend.Buffer);	
					gPackSend.AckID = eBB_gCMD_TER_VDR_DATA_UPLOAD;				
					os_dly_wait(100);
					Tp.Buffer[0] = 0;
				}
				else if((FlashPageCnt.ParkingDataSave>=2)&&(FlashPageCnt.ParkingDataRead<(FlashPageCnt.ParkingDataSave-1)))
				{
					ReSend.DataType = POP_DATA_TYPE_PARK_DATA;
					strcpy((char*)Tp.Buffer," ͣ�����ݳ��ӣ�");
					Len = strlen((char*)Tp.Buffer);
					Int2Str( FlashPageCnt.ParkingDataRead, (char*)&Tp.Buffer[Len]);		
					strcat((char*)Tp.Buffer,",");
					Len = strlen((char*)Tp.Buffer);
					Int2Str( FlashPageCnt.ParkingDataSave, (char*)&Tp.Buffer[Len]);		
					strcat((char*)Tp.Buffer," ");
					Uart.puts((char*)Tp.Buffer);  
					
					Tp.Buffer[0] = GetVDR_Index[eGB_rCMD_GET_PARKING_DATA]>>8;
					Tp.Buffer[1] = GetVDR_Index[eGB_rCMD_GET_PARKING_DATA];		
					Tp.Buffer[2] = eGB_rCMD_GET_PARKING_DATA;
					
					Tp.Buffer[3] = 0x55;
					Tp.Buffer[4] = 0x7a;
					Tp.Buffer[5] = eGB_rCMD_GET_PARKING_DATA;
					Tp.Buffer[6] = 0x01;
					Tp.Buffer[7] = 0xd4;
					Tp.Buffer[8] = 0;
					
					for(i=0;i<2;i++)
					{
						Addr = FF_PARKING_DATA_START_PAGE;
						Addr += ((FlashPageCnt.ParkingDataRead+i)%FF_PARKING_DATA_TOTAL_PAGE);
						DFF.PageRead( Addr, &Tp.Buffer[9+i*234] );
					}
					
					Tp.Buffer[9+468] = CalXorSum( (9+468),Tp.Buffer);
									
					PageCnt = 2;				
					gPackSend.Len = JT808_Packaging( eBB_gCMD_TER_VDR_DATA_UPLOAD,(9+468+1),0,Tp.Buffer,(uchar*)&gPackSend.Buffer);	
					gPackSend.AckID = eBB_gCMD_TER_VDR_DATA_UPLOAD;				
					os_dly_wait(100);
					Tp.Buffer[0] = 0;
				}
				else if((FlashPageCnt.FatigueDrivingSave>=2)&&(FlashPageCnt.FatigueDrivingRead<(FlashPageCnt.FatigueDrivingSave-1)))
				{
					ReSend.DataType = POP_DATA_TYPE_OVTD;
					strcpy((char*)Tp.Buffer," ƣ�ͼ�ʻ���ӣ�");
					Len = strlen((char*)Tp.Buffer);
					Int2Str( FlashPageCnt.FatigueDrivingRead, (char*)&Tp.Buffer[Len]);		
					strcat((char*)Tp.Buffer,",");
					Len = strlen((char*)Tp.Buffer);
					Int2Str( FlashPageCnt.FatigueDrivingSave, (char*)&Tp.Buffer[Len]);		
					strcat((char*)Tp.Buffer," ");
					Uart.puts((char*)Tp.Buffer);  
					
					Tp.Buffer[0] = GetVDR_Index[eGB_rCMD_GET_OVER_TIME_DRIVE]>>8;
					Tp.Buffer[1] = GetVDR_Index[eGB_rCMD_GET_OVER_TIME_DRIVE];		
					Tp.Buffer[2] = eGB_rCMD_GET_OVER_TIME_DRIVE;
					
					Tp.Buffer[3] = 0x55;
					Tp.Buffer[4] = 0x7a;
					Tp.Buffer[5] = eGB_rCMD_GET_OVER_TIME_DRIVE;
					Tp.Buffer[6] = 0x01;
					Tp.Buffer[7] = 0xf4;
					Tp.Buffer[8] = 0;
					
					for(i=0;i<2;i++)
					{
						Addr = FF_TIRE_DRIVE_START_PAGE;
						Addr += ((FlashPageCnt.FatigueDrivingRead+i)%FF_TIRE_DRIVE_TOTAL_PAGE);
						DFF.PageRead( Addr, &Tp.Buffer[9+i*250] );
					}
					
					Tp.Buffer[9+500] = CalXorSum( (9+500),Tp.Buffer);
									
					PageCnt = 2;				
					gPackSend.Len = JT808_Packaging( eBB_gCMD_TER_VDR_DATA_UPLOAD,(9+500+1),0,Tp.Buffer,(uchar*)&gPackSend.Buffer);	
					gPackSend.AckID = eBB_gCMD_TER_VDR_DATA_UPLOAD;				
					os_dly_wait(100);
					Tp.Buffer[0] = 0;
				}
				else if((FlashPageCnt.DriverLogInOutSave>=2)&&(FlashPageCnt.DriverLogInOutRead<(FlashPageCnt.DriverLogInOutSave-1)))
				{
					ReSend.DataType = POP_DATA_TYPE_LOG_IN_OUT;
					strcpy((char*)Tp.Buffer," ��ʻԱ����ǩ���ӣ�");
					Len = strlen((char*)Tp.Buffer);
					Int2Str( FlashPageCnt.DriverLogInOutRead, (char*)&Tp.Buffer[Len]);		
					strcat((char*)Tp.Buffer,",");
					Len = strlen((char*)Tp.Buffer);
					Int2Str( FlashPageCnt.DriverLogInOutSave, (char*)&Tp.Buffer[Len]);		
					strcat((char*)Tp.Buffer," ");
					Uart.puts((char*)Tp.Buffer);  
					
					Tp.Buffer[0] = GetVDR_Index[eGB_rCMD_GET_GET_LOG_IN_OUT_RECORD]>>8;
					Tp.Buffer[1] = GetVDR_Index[eGB_rCMD_GET_GET_LOG_IN_OUT_RECORD];		
					Tp.Buffer[2] = eGB_rCMD_GET_GET_LOG_IN_OUT_RECORD;
					
					Tp.Buffer[3] = 0x55;
					Tp.Buffer[4] = 0x7a;
					Tp.Buffer[5] = eGB_rCMD_GET_GET_LOG_IN_OUT_RECORD;
					Tp.Buffer[6] = 0x01;
					Tp.Buffer[7] = 0xf4;
					Tp.Buffer[8] = 0;
					
					for(i=0;i<2;i++)
					{
						Addr = FF_DRIVER_IN_OUT_START_PAGE;
						Addr += ((FlashPageCnt.DriverLogInOutRead+i)%FF_DRIVER_IN_OUT_TOTAL_PAGE);
						DFF.PageRead( Addr, &Tp.Buffer[9+i*250] );
					}
									
					Tp.Buffer[9+500] = CalXorSum( (9+500),Tp.Buffer);
									
					PageCnt = 2;				
					gPackSend.Len = JT808_Packaging( eBB_gCMD_TER_VDR_DATA_UPLOAD,(9+500+1),0,Tp.Buffer,(uchar*)&gPackSend.Buffer);	
					gPackSend.AckID = eBB_gCMD_TER_VDR_DATA_UPLOAD;				
					os_dly_wait(100);
					Tp.Buffer[0] = 0;
				}
				else if((FlashPageCnt.PowerOnOffSave>=2)&&(FlashPageCnt.PowerOnOffRead<(FlashPageCnt.PowerOnOffSave-1)))
				{
					ReSend.DataType = POP_DATA_TYPE_PWR;
					strcpy((char*)Tp.Buffer,"  �ϵ��¼���ӣ�");
					Len = strlen((char*)Tp.Buffer);
					Int2Str( FlashPageCnt.PowerOnOffRead, (char*)&Tp.Buffer[Len]);		
					strcat((char*)Tp.Buffer,",");
					Len = strlen((char*)Tp.Buffer);
					Int2Str( FlashPageCnt.PowerOnOffSave, (char*)&Tp.Buffer[Len]);		
					strcat((char*)Tp.Buffer," ");
					Uart.puts((char*)Tp.Buffer);  
					
					Tp.Buffer[0] = GetVDR_Index[eGB_rCMD_GET_POWER_RECORD]>>8;
					Tp.Buffer[1] = GetVDR_Index[eGB_rCMD_GET_POWER_RECORD];		
					Tp.Buffer[2] = eGB_rCMD_GET_POWER_RECORD;
					
					Tp.Buffer[3] = 0x55;
					Tp.Buffer[4] = 0x7a;
					Tp.Buffer[5] = eGB_rCMD_GET_POWER_RECORD;
					Tp.Buffer[6] = 0x01;
					Tp.Buffer[7] = 0xf8;
					Tp.Buffer[8] = 0;
					
					for(i=0;i<2;i++)
					{
						Addr = FF_POWER_ON_OFF_START_PAGE;
						Addr += ((FlashPageCnt.PowerOnOffRead+i)%FF_POWER_ON_OFF_TOTAL_PAGE);
						DFF.PageRead( Addr, &Tp.Buffer[9+i*252] );
					}
					
					Tp.Buffer[9+504] = CalXorSum( (9+504),Tp.Buffer);
									
					PageCnt = 2;				
					gPackSend.Len = JT808_Packaging( eBB_gCMD_TER_VDR_DATA_UPLOAD,(9+504+1),0,Tp.Buffer,(uchar*)&gPackSend.Buffer);	
					gPackSend.AckID = eBB_gCMD_TER_VDR_DATA_UPLOAD;				
					os_dly_wait(100);
					Tp.Buffer[0] = 0;
				}
				else if((FlashPageCnt.ParaModifySave>=3)&&(FlashPageCnt.ParaModifyRead<(FlashPageCnt.ParaModifySave-2)))
				{
					ReSend.DataType = POP_DATA_TYPE_PARA_MDF;
					strcpy((char*)Tp.Buffer,"  �����޸ļ�¼���ӣ�");
					Len = strlen((char*)Tp.Buffer);
					Int2Str( FlashPageCnt.ParaModifyRead, (char*)&Tp.Buffer[Len]);		
					strcat((char*)Tp.Buffer,",");
					Len = strlen((char*)Tp.Buffer);
					Int2Str( FlashPageCnt.ParaModifySave, (char*)&Tp.Buffer[Len]);		
					strcat((char*)Tp.Buffer," ");
					Uart.puts((char*)Tp.Buffer);  
					
					Tp.Buffer[0] = GetVDR_Index[eGB_rCMD_GET_MODIFY_PARA_RECORD]>>8;
					Tp.Buffer[1] = GetVDR_Index[eGB_rCMD_GET_MODIFY_PARA_RECORD];		
					Tp.Buffer[2] = eGB_rCMD_GET_MODIFY_PARA_RECORD;
					
					Tp.Buffer[3] = 0x55;
					Tp.Buffer[4] = 0x7a;
					Tp.Buffer[5] = eGB_rCMD_GET_MODIFY_PARA_RECORD;
					Tp.Buffer[6] = 0x02;
					Tp.Buffer[7] = 0xf4;
					Tp.Buffer[8] = 0;
					
					for(i=0;i<3;i++)
					{
						Addr = FF_PARA_MODIFY_START_PAGE;
						Addr += ((FlashPageCnt.ParaModifyRead+i)%FF_PARA_MODIFY_TOTAL_PAGE);
						DFF.PageRead( Addr, &Tp.Buffer[9+i*252] );
					}
								
					
					Tp.Buffer[9+756] = CalXorSum( (9+756),Tp.Buffer);
									
					PageCnt = 3;				
					gPackSend.Len = JT808_Packaging( eBB_gCMD_TER_VDR_DATA_UPLOAD,(9+756+1),0,Tp.Buffer,(uchar*)&gPackSend.Buffer);	
					gPackSend.AckID = eBB_gCMD_TER_VDR_DATA_UPLOAD;				
					os_dly_wait(100);
					Tp.Buffer[0] = 0;
				}
				else if((FlashPageCnt.SpeedStatusSave>=3)&&(FlashPageCnt.SpeedStatusRead<(FlashPageCnt.SpeedStatusSave-2)))
				{
					ReSend.DataType = POP_DATA_TYPE_SPEED_S;
					strcpy((char*)Tp.Buffer,"  �ٶ�״̬��¼���ӣ�");
					Len = strlen((char*)Tp.Buffer);
					Int2Str( FlashPageCnt.SpeedStatusRead, (char*)&Tp.Buffer[Len]);		
					strcat((char*)Tp.Buffer,",");
					Len = strlen((char*)Tp.Buffer);
					Int2Str( FlashPageCnt.SpeedStatusSave, (char*)&Tp.Buffer[Len]);		
					strcat((char*)Tp.Buffer," ");
					Uart.puts((char*)Tp.Buffer);  
					
					Tp.Buffer[0] = GetVDR_Index[eGB_rCMD_GET_SPEED_STATUS_LOG]>>8;
					Tp.Buffer[1] = GetVDR_Index[eGB_rCMD_GET_SPEED_STATUS_LOG];		
					Tp.Buffer[2] = eGB_rCMD_GET_SPEED_STATUS_LOG;
					
					Tp.Buffer[3] = 0x55;
					Tp.Buffer[4] = 0x7a;
					Tp.Buffer[5] = eGB_rCMD_GET_SPEED_STATUS_LOG;
					Tp.Buffer[6] = 0x02;
					Tp.Buffer[7] = 0x99;
					Tp.Buffer[8] = 0;
					
					for(i=0;i<3;i++)
					{
						Addr = FF_SPEED_STATUS_START_PAGE;
						Addr += ((FlashPageCnt.SpeedStatusRead+i)%FF_SPEED_STATUS_TOTAL_PAGE);
						DFF.PageRead( Addr, &Tp.Buffer[9+i*256] );
					}	
					
					Tp.Buffer[9+665] = CalXorSum( (9+665),Tp.Buffer);
					PageCnt = 3;				
					gPackSend.Len = JT808_Packaging( eBB_gCMD_TER_VDR_DATA_UPLOAD,(9+665+1),0,Tp.Buffer,(uchar*)&gPackSend.Buffer);	
					gPackSend.AckID = eBB_gCMD_TER_VDR_DATA_UPLOAD;				
					os_dly_wait(100);
					Tp.Buffer[0] = 0;
				}
			}*/
		}
	}	  //��Ȩ�ɹ�
			
	//////////////////////////////////////// �����ݷ���
	
	if( gPackSend.Len > 0 )
	{	
		SendOK = ERROR;
		ReSend.Times++;	
		AckBuffer.MsgId = gPackSend.AckID;
		if((TD.GPRS==tGPRS_TMP_TEST)&&Tp.Buffer[0])	Uart.puts((char*)Tp.Buffer);
		//Uart.puts("����6 ");  
		for(ch=0;ch<TOTAL_IP_NUMBER;ch++)
		{
			if(sIP.Online[ch]==ENABLE_FLAG)
			{	
				if( (SendData(ch,gPackSend.Len,gPackSend.Buffer))==OK)		// ��������
				{
					SendOK = OK;
					break;
				}
			}			
		}
		if(SendOK==OK)			
		{
			os_dly_wait(10);		// ��ʱ1�룬��Ҫ����̫�졣
			//gPackSend.Len = 0;
			if( gPackSend.AckID==eBB_gCMD_TER_REG )	 	// 	
			{	  
			   gPackSend.Len = 0;
				PageCnt++;
				strcpy((char*)Tp.Buffer,"��N�γ��ӳɹ����ȴ�ע��Ӧ�� ");
			}
			else if( gPackSend.AckID==eBB_gCMD_TER_AUTH)
			{
				PageCnt++;
				strcpy((char*)Tp.Buffer,"��N�γ��ӳɹ����ȴ���ȨӦ�� ");
				//if(TestMultiCenter!=TRUE)
				{	// 
					PageCnt = 0;
					ReSend.DataType = 0;						
					//ReSend.PreOverTime = SendOVT.TCP_ACK_OverTime; 
				}
			}
			else 
			{	
				gPackSend.Len = 0;	  				
				strcpy((char*)Tp.Buffer,"��N�γ��ӳɹ�������ȴ�Ӧ�� ");	// ���ɹ�ʱ������������
				switch(ReSend.DataType)
				{
					case POP_DATA_TYPE_HISTORY		:	FlashPageCnt.HistoryRead	 		+= PageCnt;		break;		
					case POP_DATA_TYPE_BLIND		:	FlashPageCnt.BlindRead	 			+= PageCnt;    break;
					case POP_DATA_TYPE_ALARM		:	FlashPageCnt.AlarmMsgRead 			+= PageCnt;	   break;
					case POP_DATA_TYPE_NORMAL		:	FlashPageCnt.NormalMsgRead 		+= PageCnt;	   break;
					case POP_DATA_TYPE_GNSS			:	FlashPageCnt.GnssDataRead	 		+= PageCnt;    break;
						                                                            	
					/*case POP_DATA_TYPE_48H			:	FlashPageCnt.Speed48HRead	 		+= PageCnt;    break;
					case POP_DATA_TYPE_360H			:	FlashPageCnt.Location360HRead		+= PageCnt;    break;
					case POP_DATA_TYPE_PARK_DATA	:	FlashPageCnt.ParkingDataRead		+= PageCnt;    break;
					case POP_DATA_TYPE_OVTD			:	FlashPageCnt.FatigueDrivingRead	+= PageCnt;    break;	
					case POP_DATA_TYPE_LOG_IN_OUT	:	FlashPageCnt.DriverLogInOutRead	+= PageCnt;    break;
					case POP_DATA_TYPE_PWR			:	FlashPageCnt.PowerOnOffRead		+= PageCnt;    break;				
					case POP_DATA_TYPE_PARA_MDF	:	FlashPageCnt.ParaModifyRead		+= PageCnt;    break;
					case POP_DATA_TYPE_SPEED_S		:	FlashPageCnt.SpeedStatusRead		+= PageCnt;    break;  */

					case POP_DATA_TYPE_GET_08H_15H_DATA:											 
					 	if (JT_RemoteReadRecodMoreDataCmd != 0)
						{							 
							 if (JT_VehRecord.PackIndex==JT_VehRecord.TotalPack)
							 {										 
							 	  JT_RemoteReadRecodMoreDataCmd = 0;
							 }
							 //JT_VehRecord.PackIndex++;		 // ��������++
						}
						
						break;
				}
				PageCnt = 0;
				ReSend.DataType = 0;
				ReSend.Times = ReSend.OverTime = 0;
				ReSend.PreOverTime = SendOVT.TCP_ACK_OverTime; 

				if (SendAudioData)	SendAudioData = 0;		// 
				if( SendPicData)		SendPicData = 0;
			}							
		}
		else				 			
		{
			strcpy((char*)Tp.Buffer,"��N�γ���ʧ�� ");	
			PageCnt = 0;
			ReSend.DataType = 0;
			ReSend.Times = ReSend.OverTime = 0;
			ReSend.PreOverTime = SendOVT.TCP_ACK_OverTime; 
			GG_Status.Flag.Word = GG_FLAG_INIT_DATA;			// ���ͳ�ʱ10��δ�ɹ����ز���
		}
		
		Tp.Buffer[2] = ReSend.Times+'0';

		if (TD.GPRS==tGPRS_TMP_TEST) { Uart.puts((char*)Tp.Buffer);}
	}
	else if( ReSend.Times==0)		// ����Ϊ0��δ��������
	{
		SendOK = OK;					// δ�������ݣ�����OK
		PageCnt = 0;
		ReSend.Times = ReSend.OverTime = 0;
		ReSend.DataType = 0;
		ReSend.PreOverTime = SendOVT.TCP_ACK_OverTime; 
		
	}
	//////////////////////////////////////////////////////////////////////////
	gPackSend.Busing = 0;
	return SendOK;			// ���ط���ʱ�䣺��
} 







/**********************************************************************************************
** ��������: Init()
** ��������: 
** �������: ��
** �������: ��
** ȫ�ֱ���: ��
** �޸�ʱ��: 2009.08.26
************************************************************************************************/
void			GPRSCls::InitAlig(uchar Clear)
{
	if( Clear=='A')
	{
		SendAlig.Length = 0;
		SendAlig.Busy = 0;
		UploadCmdBusing = 0;
		SendAlig.gPriorityLen = 0;
		SendAlig.gPriorityID = 0;
		SetBufferValue(SendAlig.Buffer,GPRS_ALIG_MAX_LEN,0);
		SetBufferValue(SendAlig.gPriorityBuffer,LEN_PRIORITY_POP_BUFFER,0);
	}
	else if( Clear=='P')
	{
		SendAlig.gPriorityLen = 0;
		SendAlig.gPriorityID = 0;
	}
}



uint16		GPRSCls::GetAligLen(void)
{
	return 0;//SendAlig.Length;
}





void		GPRSCls::ClearAligBuffer(void)
{
	
}








/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */
