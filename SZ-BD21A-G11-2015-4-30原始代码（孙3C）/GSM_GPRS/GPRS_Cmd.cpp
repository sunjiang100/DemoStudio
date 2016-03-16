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
	����GPRS����֡ͷ��
*/
char		const		FrameHeadTab[DOWNLOAD_FRAME_HEAD_TOTAL_NUM][5] = 
{	
	{"*GY\0"},		// ��sPackHeadȡ��
	{"*GY\0"},	
	{"*SZ\0"},
	{"*TH\0"},
	{0x7E,0},	
};




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�ProcessFrameData(uchar Channel,uint16 DataLen,uchar *pData)
   ������������������֡���ݼ�͸������
   ��ڲ�������
   ���ڲ�������
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void			GPRSCls::ProcessFrameData(uchar Channel,uint16 DataLen,uchar *pData)
{
	uint16	Len,FrameLen,CurrentLen;
	uchar		CmdCnt,ValidCmd;//,Tmp;		// ���ص���������
	uchar		t,HeadFlag,NoEndFlag;
	uchar		FrameCmd[16];
	uchar		*pHead,*pEnd;		


	uint16	i, MsgAttr, isPack ;
	pJT808_MSG_HEAD	pBB;
	void		(GPRSCls::*pExecuteCmd)(uchar *pPS);		// ���庯��ָ��	
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
			if(pHead != NULL)		// �ҵ�֡ͷ
			{												//////////////////				 */
				NOP();	NOP();	NOP();		
				Len = 0;	CmdCnt++;
				if(pHead != pData)		// ��͸������
				{
					Len = pHead - pData;
					if(DataLen>=Len)	DataLen -= Len;
					if(Uart0CommandFlag)	 // �������ݣ�͸����GPRS
					{
						//PushAlignment(Len,pData);
					}
					else	  	 // �Ǵ������ݣ�͸��������
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

				case BB_DATA_PACK:		 // JT808����Э��
					NOP();	NOP();	
					pData += Len;						// Len=0��͸������	
				  ///////////////////////////
				  //��Ӳ���Э�����
				   
					///EscapeCharDecode(DataLen, pData);       // ת�廹ԭ

				   //pEnd = (uchar*)SearchArray(DataLen - Len - 1,&pHead[1],1,FrameCmd);
					if(DataLen<14)		 // �����㲿����С����
					{
						GG_Status.FrameDataBusing = FALSE;
						return;
					}
				
					pEnd = (uchar*)SearchArray(DataLen - 1,&pHead[1],1,FrameCmd);
					if (pEnd == NULL) 
					{
						pData = pHead + 1;  // ����ָ��0x7e�ĵ�ַ
						DataLen -= 1;
							
						break;
					}
				 

            		Len = pEnd - pHead + 1;
					CurrentLen = Len;
					EscapeCharDecode(Len, pData);       // ת�廹ԭ


					pBB = (pJT808_MSG_HEAD)&pHead[1];		//  ָ����Ϣͷ
					MsgAttr = SwapINT16(pBB->Attr.B16);
					Len = MsgAttr & 0x3FF;           	// ��ȡ���ݳ���
					isPack = (MsgAttr >> 13) & 0x01;           //  ��ȡ��װ��־


					if(Len < LEN_ONE_FRAME)				
					{						
						FrameLen = 	(Len+ (isPack ? LEN_BB_PACK_HEAD : LEN_BB_PACK_HEAD - 4 ) + 2);	// ��ʶλ1�ֽڣ�����λ1�ֽ�

						memmove((uchar*)OneFrame,(uchar*)pHead,FrameLen);
						
						DataLen -= CurrentLen;
             		if (DataLen <= 0)   DataLen = 0;
             		else                pData = pEnd + 1; 

						SystemLog.GPRS.RecTotalPack++;
						SystemLog.GPRS.RecTotalBytes += FrameLen;	

						/////////////////////////////////////// ��������ڣ�����ͨ��16������ʾ	
						if (TD.GPRS==tGPRS_TMP_TEST)
						{	
					    			
							Uart.puts("Center Data:\0");
							Uart.putb(1, FrameLen, OneFrame);
							Uart.puts("\r\n");
						}
						//os_dly_wait(5);
						
						if((BBJT808_GPRS_CmdExe(Len))!=OK)
						{
							Uart.puts(" ����ָ��δִ��: ");
						   Uart.putb(1, pEnd - pHead + 1, pHead);		  //;	���ûִ�е�ָ��
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
				///////////////////////////////////////////////////// ����Э��
							
				
				///////////////////////////////////////////////////// GYЭ�飬��ʹ�ò�ͬ�İ�ͷ	
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
						pData += Len;						// Len=0��͸������
						Len = pEnd - pHead + 1;	  		// #�ţ�Ҫ��1
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
					
						for(i=0;i<8;i++)	{	FrameCmd[i] = 0;	}						// ��ʼ��֡����
						memmove(FrameCmd,&OneFrame[PSC[2<<1]],PSC[(2<<1)+1] ); 	// ��ȡ����
						SetBufferValue((uchar*)OEM,16,0);
						memmove(OEM,&OneFrame[PSC[1<<1]],PSC[(1<<1)+1] ); 			// ��ȡ���к�
   					if(TD.GPRS == tGPRS_GSM_ALL_DATA)		Uart.puts((char*)OneFrame);
						for(i=0;i<DOWNLOAD_CMD_TOTAL_NUM;i++)
						{
							// �������б��е��������Ƚ�
							if(MyStrCmp((char*)FrameCmd,(char*)&DownloadCmdTab[i][0])==0)
							{									
								SetBufferValue((uchar*)AckBuffer.Data,LEN_ACK_BUFFER,0);
								AckBuffer.Length = 0;
								AckBuffer.Error = 0;
								AckBuffer.GPS = 0;
								pExecuteCmd = DownloadCmdProcessTab[i];								
								memmove(CtrlSendCmdTime,&OneFrame[PSC[3<<1]],PSC[(3<<1)+1] );			  // ���ķ��������ʱ��
								CtrlSendCmdTime[6] = 0;	
								NOP();	NOP();	NOP();	NOP();
								(GPRS.*pExecuteCmd)(PSC); 
								NOP();	NOP();	NOP();	NOP();
								ValidCmd = TRUE;	 
								break;
							}
						}
						/////////////////////////////////// Ӧ��								
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
							SystemLog.GPRS.PushTotalBytes += Len;			// ���ӳ����ۼ�
						}
						ValidCmd = 0;	
					}
					else
					{
						pHead += 3; 			// ����"*GY,000000,S1,130305,"û��"#"
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
				
				if(Uart0CommandFlag)	 	// �������ݣ�͸����GPRS
				{
					PushAlignment(0,0,DataLen,pData);
					DataLen = 0;
				}
				else			// �Ǵ������ݣ�͸��������
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
			DataLen = 0;		// �Ҳ���֡ͷ��ȫ�����ݵ�͸��
		}
	}				  
	GG_Status.FrameDataBusing = FALSE;		
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





void		GPRSCls::ReDialFlag(void)
{
	
}




GPRSCls::GPRSCls()
{
}











	
/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */
