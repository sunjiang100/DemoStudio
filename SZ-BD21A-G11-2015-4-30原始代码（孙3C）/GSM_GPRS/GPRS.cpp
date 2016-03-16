/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												 
$										 
$                       	 		  
$								  		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      文件描述
	文件名: 	 GPRS.c
	功能描述：GRPS相关函数
	编译环境：
	目标CPU: 
	作者：
	创建时间：
	建议编辑环境：UltraEdit 11.00a+  制表符宽度为三个字符，程序可读性更好。
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
** 函数名称：Close()
** 功能描述：强行关闭GPRS
** 入口参数：无
** 出口参数：无
** 全局变量: 无
** 修改时间：
**************************************************************************************************************/ 
void			GPRSCls::Close(void)
{
//	uchar		s;
	NOP();	NOP();	
	GG_Status.Flag.Word = GG_FLAG_INIT_DATA;	
	
} // End of <GPRSCls::Close> function




/**************************************************************************************************************
** 函数名称：Dial(uchar s)
** 功能描述：检测GPRS的链接状态，如果不在线，则重新上线（拨号）
** 入口参数：无
** 出口参数：返回状态
** 全局变量: 无
** 修改时间：
*************************************************************************************************************/ 
uchar		GPRSCls::Dial(uchar s)	  
{

	static		char		DialBuffer[80]={0};	
	static		char		sPort[32] = {0};
	uchar		i,Len;	
	NOP();	NOP();	
	////////////////////////////////////////////////// 先关闭		
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
		//// IP地址和端口号可用逗号或冒号隔开
		if((DialBuffer[i]==',')||(DialBuffer[i]==':'))
		{	// 端口号移动sPort
			strcpy((char*)sPort,(char*)&DialBuffer[i+1]);						
			break;
		}
	}	
	for(;i<Len;i++)
	{
		DialBuffer[i] = 0;
	}
	strcat((char *)DialBuffer,"\",\"\0");			// SIMCOM模块，ＩＰ和端口分隔使用逗号
	strcat((char *)DialBuffer,(char*)sPort);		// 追加端口	
	strcat((char *)DialBuffer,"\"\r\n\0");	
		
	Uart.puts(&DialBuffer[19]);		
	////////////////////////////////////////////////// 拨号
	Uart1.ClearReceiveFlag(GPRS_WAIT_RET_STATUS,1);	
	//strcpy(DialBuffer,"AT+CIPSTART=\"TCP\",\"61.235.163.15\",\"8000\"\r\n");
	Uart1.puts((char *)DialBuffer);	
	os_dly_wait(50);
	SystemLog.GPRS_DialCnt++;
	return OK;
}// End of <GprsStart> function


	
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：GPRSCls::SendNBytesData
   功能描述：以固定长度发送数据
   入口参数：长度，数据指针
   出口参数：本次发送所花时间：秒
   全局变量: 
   修改时间：
*/ 
uchar		SendCmd[46];
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
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
	Uart1.puts((char *)SendCmd);				// 发送命令
	os_dly_wait(20);
	///////////////////////////////////////////////////
	GG_Status.GPRS_SendStatus =GPRS_SEND_DATA;	
	//UserEvtFlag.GSM_GPRS_Transmit = 0;
	Uart1.ClearReceiveFlag(GPRS_WAIT_RET_STATUS,1);
	Uart1.putb(1,Len,pData);						// 发送GPRS数据
	os_dly_wait(10);	
	//////////////// 在串口中断中检测不到<\r\nSEND OK\r\n>中的第二个回车符???
	return OK;
}






/**********************************************************************************************
** 函数名称: ClearAligBuffer()
** 函数功能: 清除发送缓冲
** 输入参数: 无
** 输出参数: 无
** 全局变量: 无
** 修改时间: 2009.07.18
************************************************************************************************/
void			GPRSCls::ClearAligBuffer()
{
	 SendAlig.Length = 0;
}





/**********************************************************************************************
** 函数名称: Init()
** 函数功能: 
** 输入参数: 无
** 输出参数: 无
** 全局变量: 无
** 修改时间: 2009.08.26
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
