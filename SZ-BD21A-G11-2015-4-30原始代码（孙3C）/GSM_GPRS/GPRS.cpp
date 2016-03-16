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

#include		"..\Config\RTOS.h"

#if(USE_MOBILE_MODEL)

#include		"ATCommand.h"
#include		"GSM_GPRS.h"

#include		<stdlib.h>
#include		<string.h>
#define		DOWN_UP_CMD_GLOBALS		1
#include		"DownUploadCmd.h"
#include		"CmdPrototype.h"
#include		"..\Common\Algol.h"
#include		"..\UART\UART0.h"
#include		"..\UART\UART1.h"
#include		"..\Common\EEPROM_Addr.h"
#include		"..\KeyDisp\KeyDisp.h"
#include		"..\RTC\RTC.h"



void			GPRSCls::ReDialFlag(void)
{
	GG_Evt.GPRS.ReDial = TRUE;
}


/**************************************************************************************************************
** �������ƣ�Close()
** ����������ǿ�йر�GPRS
** ��ڲ�������
** ���ڲ�������
** ȫ�ֱ���: ��
** �޸�ʱ�䣺
**************************************************************************************************************/ 
void			GPRSCls::Close(void)
{
//	uchar		s;
	NOP();	NOP();	
	GG_Status.Flag.Word = GG_FLAG_INIT_DATA;	
	
} // End of <GPRSCls::Close> function




/**************************************************************************************************************
** �������ƣ�Dial(uchar s)
** �������������GPRS������״̬����������ߣ����������ߣ����ţ�
** ��ڲ�������
** ���ڲ���������״̬
** ȫ�ֱ���: ��
** �޸�ʱ�䣺
*************************************************************************************************************/ 
uchar		GPRSCls::Dial(uchar s)	  
{

	static		char		DialBuffer[80]={0};	
	static		char		sPort[32] = {0};
	uchar		i,Len;	
	NOP();	NOP();	
	////////////////////////////////////////////////// �ȹر�		
	GG_Evt.GPRS.Dial = TRUE;
	
	GG_Status.GPRS_SendStatus = GPRS_SEND_IDLE;
	GG_Status.GPRS_SendEnable = ERROR;
	Close();
	os_dly_wait(100);
	//////////////////////////////////////////////////
	for(i=0;i<80;i++)	{ DialBuffer[i] = 0;	}
	if(GG_Status.LinkType==TCP_LINK)
	{
		strcpy((char *)DialBuffer,"AT+CIPSTART=\"TCP\",\"\0");
		Uart.puts("GPRS Dial TCP \0");
	}
	else
	{
		strcpy((char *)DialBuffer,"AT+CIPSTART=\"UDP\",\"\0");
		Uart.puts("GPRS Dial UDP \0");
	}
				
	strcpy((char *)&DialBuffer[19],(char *)sIP_Port);
	Len = strlen((char*)DialBuffer);
	for(i=19;i<Len;i++)			 // AT+CIPSTART="UDP","10.64.62.138","67*7678678784467678"\r\n
	{
		//// IP��ַ�Ͷ˿ںſ��ö��Ż�ð�Ÿ���
		if((DialBuffer[i]==',')||(DialBuffer[i]==':'))
		{	// �˿ں��ƶ�sPort
			strcpy((char*)sPort,(char*)&DialBuffer[i+1]);						
			break;
		}
	}	
	for(;i<Len;i++)
	{
		DialBuffer[i] = 0;
	}
	strcat((char *)DialBuffer,"\",\"\0");			// SIMCOMģ�飬�ɣкͶ˿ڷָ�ʹ�ö���
	strcat((char *)DialBuffer,(char*)sPort);		// ׷�Ӷ˿�	
	strcat((char *)DialBuffer,"\"\r\n\0");	
		
	Uart.puts(&DialBuffer[19]);		
	////////////////////////////////////////////////// ����
	Uart1.ClearReceiveFlag(GPRS_WAIT_RET_STATUS,1);	
	//strcpy(DialBuffer,"AT+CIPSTART=\"TCP\",\"61.235.163.15\",\"8000\"\r\n");
	Uart1.puts((char *)DialBuffer);	
	os_dly_wait(50);
	SystemLog.GPRS_DialCnt++;
	return OK;
}// End of <GprsStart> function


	
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�GPRSCls::SendNBytesData
   �����������Թ̶����ȷ�������
   ��ڲ��������ȣ�����ָ��
   ���ڲ��������η�������ʱ�䣺��
   ȫ�ֱ���: 
   �޸�ʱ�䣺
*/ 
uchar		SendCmd[46];
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
uchar			GPRSCls::SendNBytesData(uint16 Len,uchar *pData)
{
	uchar		i;
	RtcCls	Rtc;

	NOP();	NOP();
	if(Len>1024)
		return ERROR;
	if(Len==0)
		return OK;
	for(i=0;i<46;i++)	{ SendCmd[i] = 0;	}
	
	///////////////////////////////////////////////////
	
	GG_Evt.GPRS.SendData = TRUE;
	GG_Status.GPRS_SendStatus =GPRS_SEND_CMD;		
	KeyDisp.SetLED_Status(LED_GPRS,LED_GPRS_SEND_DATA);
	strcpy((char *)SendCmd,"AT+CIPSEND=\0");
	Int16ToDecChar(Len,(char*)&SendCmd[11]);	
	strcat((char *)SendCmd,"\r\0");	
	Uart1.ClearReceiveFlag(GPRS_WAIT_RET_STATUS,1);
	Uart1.puts((char *)SendCmd);				// ��������
	os_dly_wait(20);
	///////////////////////////////////////////////////
	GG_Status.GPRS_SendStatus =GPRS_SEND_DATA;	
	//UserEvtFlag.GSM_GPRS_Transmit = 0;
	Uart1.ClearReceiveFlag(GPRS_WAIT_RET_STATUS,1);
	Uart1.putb(1,Len,pData);						// ����GPRS����
	os_dly_wait(10);	
	//////////////// �ڴ����ж��м�ⲻ��<\r\nSEND OK\r\n>�еĵڶ����س���???
	return OK;
}






/**********************************************************************************************
** ��������: ClearAligBuffer()
** ��������: ������ͻ���
** �������: ��
** �������: ��
** ȫ�ֱ���: ��
** �޸�ʱ��: 2009.07.18
************************************************************************************************/
void			GPRSCls::ClearAligBuffer()
{
	 SendAlig.Length = 0;
}





/**********************************************************************************************
** ��������: Init()
** ��������: 
** �������: ��
** �������: ��
** ȫ�ֱ���: ��
** �޸�ʱ��: 2009.08.26
************************************************************************************************/
void			GPRSCls::Init(void)
{
	SendAlig.Length = 0;
	SendAlig.Busy = 0;
	UploadCmdBusing = 0;
}



GPRSCls::GPRSCls(void)
{
	AllPackIndex = 0x8000;
}


#endif

/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */
