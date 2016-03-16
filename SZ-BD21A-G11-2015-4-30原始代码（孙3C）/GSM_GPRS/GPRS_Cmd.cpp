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
#include			<stdlib.h>
#include			<string.h>


#define			CMD_GLOBALS			1
#include			"CmdPrototype.h"
#include			"CmdPrototypeTab.h"
#include			"DownUploadCmd.h"
#include			"DownUploadDataStruct.h"
#include			"..\GPS\GPS.h"
#include			"GSM_GPRS.h"
#include			"..\Config\RTOS.h"
#include			"..\UART\UART1.h"
#include			"..\UART\UART0.h"
#include			"..\Common\Algol.h"
#include			"JT_T808.h"

#include			"GB_GPRS_Cmd.h"	  

#define			LEN_CMD_END_FLAG			1
  
char		const   DownloadCmdTab[DOWNLOAD_CMD_TOTAL_NUM][6] =
{ 
	{"D1\0"},
	{"G1\0"},
	{"G20\0"},
	{"R1\0"},
	{"S2\0"},
	{"S17\0"},
	{"S20\0"},
	{"S23\0"},
	{"S24\0"},
	{"S25\0"},
	{"S52\0"},
	{"S60\0"},
	{"S61\0"},
	{"R81\0"},
	{"R82\0"},
	{"R83\0"},
	{"S102\0"},
	{"R8\0"},
	{"S43\0"},
	{"S88\0"},
};



enum
{
	GY_DATA_PACK0 = 0,
	GY_DATA_PACK,
	SZ_DATA_PACK,
	TH_DATA_PACK,
	BB_DATA_PACK,
} DATA_PACK_HEAD;


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	定义GPRS数据帧头表
*/
char		const		FrameHeadTab[DOWNLOAD_FRAME_HEAD_TOTAL_NUM][5] = 
{	
	{"*GY\0"},		// 用sPackHead取代
	{"*GY\0"},	
	{"*SZ\0"},
	{"*TH\0"},
	{0x7E,0},	
};




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：ProcessFrameData(uchar Channel,uint16 DataLen,uchar *pData)
   功能描述：处理命令帧数据及透传数据
   入口参数：无
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
*/ 

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void			GPRSCls::ProcessFrameData(uchar Channel,uint16 DataLen,uchar *pData)
{
	uint16	Len,FrameLen,CurrentLen;
	uchar		CmdCnt,ValidCmd;//,Tmp;		// 返回的命令条数
	uchar		t,HeadFlag,NoEndFlag;
	uchar		FrameCmd[16];
	uchar		*pHead,*pEnd;		


	uint16	i, MsgAttr, isPack ;
	pJT808_MSG_HEAD	pBB;
	void		(GPRSCls::*pExecuteCmd)(uchar *pPS);		// 定义函数指针	
	///////////////////////////////////////////////////////
	NOP();	NOP();	NOP();	NOP();		
	CmdCnt = ValidCmd = NoEndFlag = 0;

	if(DataLen<10)		return;

	while(GG_Status.FrameDataBusing==TRUE)		{ os_dly_wait(1);	}
	GG_Status.FrameDataBusing = TRUE;
	
	NOP();	NOP();	NOP();	NOP();		
	while(DataLen)
	{
		NOP();	NOP();
		CmdCnt = 0;
		for(HeadFlag=0;HeadFlag<DOWNLOAD_FRAME_HEAD_TOTAL_NUM;HeadFlag++)
		{
			NOP();	NOP();		
			SetBufferValue((uchar*)FrameCmd,16,0);
			if(HeadFlag==0)
				strcpy((char*)FrameCmd,(char*)sPackHead);
			else
				strcpy((char*)FrameCmd,(char*)&FrameHeadTab[HeadFlag][0]);
			Len = strlen((char*)FrameCmd);
			pHead = (uchar*)SearchArray(DataLen,pData,Len,FrameCmd);
			if(pHead != NULL)		// 找到帧头
			{												//////////////////				 */
				NOP();	NOP();	NOP();		
				Len = 0;	CmdCnt++;
				if(pHead != pData)		// 有透传数据
				{
					Len = pHead - pData;
					if(DataLen>=Len)	DataLen -= Len;
					if(Uart0CommandFlag)	 // 串口数据，透传到GPRS
					{
						//PushAlignment(Len,pData);
					}
					else	  	 // 非串口数据，透传到串口
					{
						t = TRUE;		
						while( t )
						{
							os_dly_wait(1);
							t = Uart.GetSendBusyFlag() ;
						}
						Uart.putb(1,Len,pData);						
					} //					
				}	  //
				SetBufferValue((uchar*)OneFrame,LEN_ONE_FRAME,0);
				switch(HeadFlag)
				{			

				case BB_DATA_PACK:		 // JT808部标协议
					NOP();	NOP();	
					pData += Len;						// Len=0或透传长度	
				  ///////////////////////////
				  //添加部标协议解析
				   
					///EscapeCharDecode(DataLen, pData);       // 转义还原

				   //pEnd = (uchar*)SearchArray(DataLen - Len - 1,&pHead[1],1,FrameCmd);
					if(DataLen<14)		 // 不满足部标最小长度
					{
						GG_Status.FrameDataBusing = FALSE;
						return;
					}
				
					pEnd = (uchar*)SearchArray(DataLen - 1,&pHead[1],1,FrameCmd);
					if (pEnd == NULL) 
					{
						pData = pHead + 1;  // 跳过指向0x7e的地址
						DataLen -= 1;
							
						break;
					}
				 

            		Len = pEnd - pHead + 1;
					CurrentLen = Len;
					EscapeCharDecode(Len, pData);       // 转义还原


					pBB = (pJT808_MSG_HEAD)&pHead[1];		//  指向消息头
					MsgAttr = SwapINT16(pBB->Attr.B16);
					Len = MsgAttr & 0x3FF;           	// 获取数据长度
					isPack = (MsgAttr >> 13) & 0x01;           //  获取封装标志


					if(Len < LEN_ONE_FRAME)				
					{						
						FrameLen = 	(Len+ (isPack ? LEN_BB_PACK_HEAD : LEN_BB_PACK_HEAD - 4 ) + 2);	// 标识位1字节，检验位1字节

						memmove((uchar*)OneFrame,(uchar*)pHead,FrameLen);
						
						DataLen -= CurrentLen;
             		if (DataLen <= 0)   DataLen = 0;
             		else                pData = pEnd + 1; 

						SystemLog.GPRS.RecTotalPack++;
						SystemLog.GPRS.RecTotalBytes += FrameLen;	

						/////////////////////////////////////// 输出到串口，串口通用16进制显示	
						if (TD.GPRS==tGPRS_TMP_TEST)
						{	
					    			
							Uart.puts("Center Data:\0");
							Uart.putb(1, FrameLen, OneFrame);
							Uart.puts("\r\n");
						}
						//os_dly_wait(5);
						
						if((BBJT808_GPRS_CmdExe(Len))!=OK)
						{
							Uart.puts(" 中心指令未执行: ");
						   Uart.putb(1, pEnd - pHead + 1, pHead);		  //;	输出没执行的指令
						}
					}
					else
					{
						DataLen = 0;
					}
					//Uart.puts("Center Data:\0");
					WithoutAckOfServer=0;
					RestartWithoutAckOfServer=0;
					NOP();	NOP(); 
				  break;			
				///////////////////////////////////////////////////// 国标协议
							
				
				///////////////////////////////////////////////////// GY协议，可使用不同的包头	
				case GY_DATA_PACK0:
				case GY_DATA_PACK:
				case SZ_DATA_PACK:
				case TH_DATA_PACK:
				
					//pEnd = (uchar*)MyStrStr((char*)pHead,"#\0");						
					pEnd = (uchar*)SearchArray(DataLen,(uchar*)pHead,1,(uchar*)"#\0");	
					if(pEnd == NULL)	
					{		
						*(pHead+DataLen) = '#';
						pEnd = pHead+DataLen;
					}
										 
				   if(pEnd != NULL)
					{	
						pData += Len;						// Len=0或透传长度
						Len = pEnd - pHead + 1;	  		// #号，要加1
						memmove((uchar*)OneFrame,(uchar*)pHead,Len);
						GetCommaPositionAndDataLen('#',(uchar*)OneFrame,PSC);
						pData += Len;	
						if(DataLen>=Len)	DataLen -= Len;
						else					DataLen = 0;
						
						if (Uart0CommandFlag != TRUE)
						{   	
							SystemLog.GPRS.RecTotalPack++;
							SystemLog.GPRS.RecTotalBytes += Len;	
						}
					
						for(i=0;i<8;i++)	{	FrameCmd[i] = 0;	}						// 初始化帧命令
						memmove(FrameCmd,&OneFrame[PSC[2<<1]],PSC[(2<<1)+1] ); 	// 读取命令
						SetBufferValue((uchar*)OEM,16,0);
						memmove(OEM,&OneFrame[PSC[1<<1]],PSC[(1<<1)+1] ); 			// 读取序列号
   					if(TD.GPRS == tGPRS_GSM_ALL_DATA)		Uart.puts((char*)OneFrame);
						for(i=0;i<DOWNLOAD_CMD_TOTAL_NUM;i++)
						{
							// 跟命令列表中的命令作比较
							if(MyStrCmp((char*)FrameCmd,(char*)&DownloadCmdTab[i][0])==0)
							{									
								SetBufferValue((uchar*)AckBuffer.Data,LEN_ACK_BUFFER,0);
								AckBuffer.Length = 0;
								AckBuffer.Error = 0;
								AckBuffer.GPS = 0;
								pExecuteCmd = DownloadCmdProcessTab[i];								
								memmove(CtrlSendCmdTime,&OneFrame[PSC[3<<1]],PSC[(3<<1)+1] );			  // 中心发送命令的时间
								CtrlSendCmdTime[6] = 0;	
								NOP();	NOP();	NOP();	NOP();
								(GPRS.*pExecuteCmd)(PSC); 
								NOP();	NOP();	NOP();	NOP();
								ValidCmd = TRUE;	 
								break;
							}
						}
						/////////////////////////////////// 应答								
						SetBufferValue((uchar*)OneFrame,LEN_ONE_FRAME,0);								
						strcpy((char*)OneFrame,(char*)sPackHead);
						strcat((char*)OneFrame,",");
						strcat((char*)OneFrame,(char *)sOEM_ID);
						strcat((char*)OneFrame,",V4,");
						strcat((char*)OneFrame,(char*)FrameCmd);
						strcat((char*)OneFrame,",");
						strcat((char*)OneFrame,CtrlSendCmdTime);
						Len = strlen((char*)OneFrame);		  
						if(ValidCmd==TRUE)   
						{	
							if(AckBuffer.Length)
							{
								strcat((char*)OneFrame,",");
								strcat((char*)OneFrame,sNowTimeString);
								strcat((char*)OneFrame,",");
								Len = strlen((char*)OneFrame);
								memmove(&OneFrame[Len],AckBuffer.Data,AckBuffer.Length);
								Len += AckBuffer.Length;
								OneFrame[Len] = '#';
								Len++;												
							}
							else if(AckBuffer.GPS)
							{
								strcat((char*)OneFrame,",");
								Len = strlen((char*)OneFrame);
								GPS.TH_DataPack((char*)&OneFrame[Len]);
								Len = strlen((char*)OneFrame);							
							}
							else if(AckBuffer.Error)
							{
								strcat((char*)OneFrame,",ERROR,");
								Len = strlen((char*)OneFrame);
								GPS.TH_DataPack((char*)&OneFrame[Len]);
								Len = strlen((char*)OneFrame);
							}	
							else
							{								
								strcat((char*)OneFrame,",ERROR#"); 
								Len += 7;
							}
						}
						else
						{	
							strcat((char*)OneFrame,",CMD_ERROR#"); 
							Len += 11;
						}
						///////////////////////////////////////
						if(TextSMS.Ack==TRUE)
						{
							GSM.SetSMS_Buffer(SMS_SendNumber,(char*)OneFrame);
							TextSMS.Ack = FALSE;
						}
						else if(Uart0CommandFlag==TRUE)
						{
							Uart.putb(1,Len,(uchar*)OneFrame);
						}						
						else
						{
							//GPRS.SendData(Channel,Len,(uchar*)OneFrame);
							//PushAlignment(Len, (uchar*)OneFrame);
							SystemLog.GPRS.SendTotalPack++;
							SystemLog.GPRS.PushTotalBytes += Len;			// 进队长度累加
						}
						ValidCmd = 0;	
					}
					else
					{
						pHead += 3; 			// 发送"*GY,000000,S1,130305,"没有"#"
						NoEndFlag = TRUE;
					}
					NOP();	NOP();			
				break;
   	
				default:	
					break;
				}
			}
		}
		if( (CmdCnt==0)||(NoEndFlag==TRUE))
		{
			NOP();	NOP();  NOP();	NOP();
			if(DataLen)
			{
				if(TD.GPRS == tGPRS_GSM_ALL_DATA)
				{
					SetBufferValue((uchar*)OneFrame,LEN_ONE_FRAME,0);
					Int2Str(DataLen,(char*)OneFrame);
					strcat((char*)OneFrame,"  DataLen\r\n\0");
					Uart.puts((char*)OneFrame);
				}
				
				if(Uart0CommandFlag)	 	// 串口数据，透传到GPRS
				{
					PushAlignment(0,0,DataLen,pData);
					DataLen = 0;
				}
				else			// 非串口数据，透传到串口
				{
					t = TRUE;		
					while( t )
					{
						os_dly_wait(1);
						t = Uart.GetSendBusyFlag();
					}
					Uart.putb(1,DataLen,pData);
					Uart2.putb(1,DataLen,pData);					
					DataLen = 0;
				} 				
			}
			DataLen = 0;		// 找不到帧头，全部数据当透传
		}
	}				  
	GG_Status.FrameDataBusing = FALSE;		
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





void		GPRSCls::ReDialFlag(void)
{
	
}




GPRSCls::GPRSCls()
{
}











	
/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */
