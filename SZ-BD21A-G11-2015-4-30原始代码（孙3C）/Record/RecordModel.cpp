/**************************************Copyright (c)*****************************************
**								���ݹ�ҵ���Ӽ������޹�˾
**								
**                     	
**							  
***************************************Copyright (c)*****************************************/


#define		RECORD_MODEL_GLOBALS			1	

#include 	"RecordModel.h"
#include			"..\GSM_GPRS\JT_T808.h"
#include 	"..\Common\Algol.h"
#include	"..\UART\UART2.h"
#include	"..\UART\UART0.h"
#include			"..\GSM_GPRS\GSM_GPRS.h"
#include	"..\GPS\GPS.h"


#define	USE_RECORD_MODEL			1




uchar		RecordTaskRun;



extern   uchar  *pShareMem;

extern	uchar		jt_auth_ok_flag;

void 		RecordCls::RecordTask(void)			   // ��Camera�������
{
	#if (USE_CARMERA_MODEL==1)
	uchar s, tBuffer[32];
	
	static	uint32	Cnt;
	uint16	i, Len;

	Cnt++;

	if ((Cnt % 100) == 0)	   
	{	
		//Uart.puts("R");
		//Uart.putb(1, 2, (uchar *)&TD.MainTask); 
	}

	if (TD.MainTask == tMAIN_RECORD_START)	// ��ʼ¼��
	{
		TD.MainTask = 0;
		Uart.puts("debug start record\r\n");
		os_dly_wait(10);

		StartRecord(20);	
	}

	if (TD.MainTask == tMAIN_RECORD_STOP)	 // ����¼��
	{
		TD.MainTask = 0;
	   Uart.puts("debug stop record\r\n");
	   os_dly_wait(10);

		StopRecord();	
	}



	

	if (TD.MainTask == tMAIN_RECORD_UPLOAD)	 // UP¼��
	{
		TD.MainTask = 0;
	    Uart.puts("debug upload record\r\n");
	    os_dly_wait(10);

		UploadDataToCenerFlag = 1;

		//UploadDataToCenerFlag = TRUE;
		//SendCmdToRecord(DOWN_CMD_RECORD_UPLOAD, 0, NULL);  //
	}

	if (TD.MainTask == tMAIN_RECORD_PLAY)	 // ����¼��
	{
		TD.MainTask = 0;
	    Uart.puts("debug play record file\r\n");
	    os_dly_wait(10);

		SendCmdToRecord(DOWN_CMD_MP3_PLAY, 0,NULL);  // ����¼��

	}


	if (Record_On == TRUE)
	{
		//Record_On = FALSE;
		if (Init_Rs232==0)
		{
			
			//UseRecordNow = TRUE;
			RecordDataReady = FALSE;
			EXT_5V_POWER_ON;

			Init_Rs232 = 1;
			//Uart.puts("Init RS232 115200...\r\n");
			os_dly_wait(100);

	    	Uart2.Init(115200UL,U2_FOR_AUDIO);
			os_dly_wait(10);
			Uart.puts("��ʼ�����ڣ�����¼�� ");
			SendCmdToRecord(DOWN_CMD_RECORD_ON, 0,NULL);  // ¼��
			s = WaitForRecordData(NULL,NULL,NULL,NULL);
			if (s == ERROR)
			{
				SendCmdToRecord(DOWN_CMD_RECORD_ON, 0,NULL);  // ¼��
				s = WaitForRecordData(NULL,NULL,NULL,NULL);	
				if (s == ERROR)	
				{
					Uart2.Init(Baudrate.U2_232,U2_FOR_DISPLAY);	// �л��ֱ��Ĳ�����					
					Init_Rs232 = 0;
					Uart.puts("U2�л��ص����� ");
					return;
					
				}
			}

		}
			
	}
	
	if (Record_Off == TRUE)
	{
		Record_Off = FALSE;
		Uart.puts("����¼�� ");
		SendCmdToRecord(DOWN_CMD_RECORD_OFF, 0, NULL);  // ֹͣ¼��
		s = WaitForRecordData(NULL,NULL,NULL,NULL);
		if (s == ERROR)
		{
			SendCmdToRecord(DOWN_CMD_RECORD_OFF, 0, NULL);  // ֹͣ¼��
			s = WaitForRecordData(NULL,NULL,NULL,NULL);
			if (s == ERROR)
			{
				Uart2.Init(Baudrate.U2_232,U2_FOR_DISPLAY);	// �л��ֱ��Ĳ�����
				Init_Rs232 = 0;
				Uart.puts("U2�л��ص����� ");
				return;
				
			}
			
		}

		UploadDataToCenerFlag = 1;

		//SendCmdToRecord(CTRL_MP3_UPLOAD, 0, NULL);  // ֹͣ¼��
	
	}
	
	if (UploadDataToCenerFlag == 1)
	{
		//UploadDataToCenerFlag  = 0;
		if (RecordDataReady == TRUE) // ¼������׼����
		{
			SetBufferValue(tBuffer, 32, 0);
			SendCmdToRecord(DOWN_CMD_GET_TOTAL_PACK, 0, NULL);  // ��ȡ�ܰ�
			s = WaitForRecordData(&Len,tBuffer,NULL,NULL);
			if (s != ERROR)	return;

			RecordTotalPack = tBuffer[0] | (tBuffer[1]<<8);

						
			for (i= 1; i <= RecordTotalPack; i++)
			{
				TC.Camera = 0;
				SetBufferValue(tBuffer, 32, 0);
				strcpy((char*)tBuffer,"AudioPack: ");
				Int2Str( i, (char*)&tBuffer[11]);
				strcat( (char*)tBuffer,",");
				Len = strlen( (char*)tBuffer);
				Int2Str( RecordTotalPack,(char*)&tBuffer[Len]);
				strcat( (char*)tBuffer,"\r\n");
				Uart.puts((char*)tBuffer);
				while(jt_auth_ok_flag==0)	{ os_dly_wait(100); }
				os_dly_wait(10);		// ��Ҫ��̫��
				s = ERROR;
				while(s==ERROR)
				{
					s = UploadPackRecordData(NULL, i, RECORD_ALL_PACK_UPLOAD);
				}
			}

			RecordDataReady = FALSE;
			UploadDataToCenerFlag = 0;
			Uart2.Init(Baudrate.U2_232,U2_FOR_DISPLAY);	// �л��ֱ��Ĳ�����
			Init_Rs232 = 0;
			RecordTaskRun = 0;
			Uart.puts("�������������ݵ���������U2�л��ص����� ");
		}	
	}
	else   if (UploadDataToCenerFlag == 2)	   // �ش������ϴ�ָ��һЩ���ݰ��ϴ�
	{
		UploadDataToCenerFlag  = 0;
		//UploadPackRecordData("voice.wav", 1, RECORD_SOME_PACK_UPLOAD);
	}

	#endif
}



void RecordCls::SendCmdToRecord(uint16 Cmd, uint16 DataLen, uchar *pData)
{	
	#if (USE_CARMERA_MODEL==1)
	uint16	CheckSum, Len;
	static uint32	Index;
	pGY_COMM_PROTOCOL 	pGyComm;

	SetBufferValue(SendBuffer, LEN_RECORD_SEND_BUFFER, 0);

	pGyComm = (pGY_COMM_PROTOCOL)SendBuffer;

	pGyComm->Head = 0x3A59472A;  // *GY:
	pGyComm->Cmd = Cmd;
	pGyComm->Index	= Index++;

	//Uart.putb(1, DataLen, pData);	  // for debug
	//os_dly_wait(1);

	if (DataLen > LEN_RECORD_SEND_BUFFER/2 )	return;		  // ���ݶε����ݲ��ܳ���32�ֽڣ���Ϊ�ǿ���¼��ģ�飬�������ݶεĻ��治��̫��32�ֽ��㹻

	pGyComm->Attr.Bits.Len = DataLen & 0xff;
	pGyComm->Attr.Bits.LenH = (DataLen >> 8) & 0x07;
	pGyComm->Attr.Bits.Encry = 0;	   // ������				
	 
	pGyComm->Attr.Bits.Pack = 0;		
	Len = 14; 

	if (pData != NULL)
	{
		memmove(&pGyComm->Data.Buffer[0], pData, DataLen); // 	���ݶ�
		Len += DataLen;
	}	

	pGyComm->Attr.Bits.Check = 0x02;		 // CRC16 У�鷽ʽ

	CheckSum = CalCRC16((uchar *)&pGyComm->Cmd, Len - LEN_GY_HEAD_TO_CHECK);

	pGyComm->Check = CheckSum;	 // ���У����

	//Uart.puts(" send to u2 ");	 Uart.putb(1,Len, SendBuffer);
	os_dly_wait(1);

	

	Uart2.ClearRecFlag();
	Uart2.putb(1,Len,SendBuffer);		
	Uart2.SendTask();	
	os_dly_wait(3); 	
	
	#endif	
}



void  	RecordCls::UploadRecordData(uchar UpType)
{
	#if (USE_CARMERA_MODEL==1)
		UploadDataToCenerFlag = UpType;
	#endif	
}




uchar  	RecordCls::UploadPackRecordData(char *pFile, uint16 PackIndex, uchar UpType)	  // �ϴ��ְ�¼������
{
	#if (USE_CARMERA_MODEL==1)

	TYPE_UP_PACK_RECORD_DATA UpRecordPack;
	uchar s, Offset;
	uint16 DataLen, TotalPack;
	uint16  i;
	uchar AudioData[548];   // 512 + 36


	if (pFile == NULL)
	{
		strcpy(UpRecordPack.File, (char *)"voice.wav");	
	}														
	
	//Uart.puts("Up Pack Record Data\r\n");
	//os_dly_wait(1);

	Offset = 0;
	if (PackIndex==1)  Offset = 36;


	{
		UpRecordPack.PackIndex = PackIndex;
		SendCmdToRecord(DOWN_CMD_RECORD_PACK_DATA, sizeof(UpRecordPack), (uchar *)&UpRecordPack);	
										
		// ��������	, ���ϴ�
		SetBufferValue(AudioData, 512, 0);
		DataLen = 0;
		
		s = WaitForRecordData(&DataLen, &AudioData[Offset], &TotalPack, &PackIndex);
		if (s == ERROR)
		{
			s = WaitForRecordData(&DataLen, &AudioData[Offset], &TotalPack, &PackIndex);
			if (s == ERROR)	return ERROR;
		}

	   //if (RecordTotalPack == 1)		RecordTotalPack = TotalPack;
	
		// JT808��������
		if (DataLen > 0)
		{
			if (PackIndex == 1) // ��һ������
			{
				AudioId++;
				MultiMediaReport.MediaId = SwapINT32(AudioId);	// ��֤��ý��ID����0
		  	 	MultiMediaReport.MediaType = 1;			        // ���ͣ���Ƶ	  
		 		MultiMediaReport.MediaFormat = 3;		     // ��ʽ��WAV
			 	MultiMediaReport.MediaEvent = 0;
				MultiMediaReport.MediaChannel = 0;
	
				memmove(&AudioData[0], (uchar *)&MultiMediaReport, 8);
				memmove(&AudioData[8], (uchar*)&GPS.JTPack, LEN_JT808_GPS_PACK); 	
			}
	
			
			for(i=0;i<(30*100UL);i++)		// �����ʱ30��
			{
				if( (SendAudioData)||(jt_auth_ok_flag==0) )
					os_dly_wait(1);
				else
					break;		
			}
			SendAudioData = TRUE;

			//Uart.puts("Record Send To GPRS\r\n");	os_dly_wait(1);
	
			GPRS.JT808_DataPackPushAlignment(eBB_gCMD_TER_MEDIA_DATA_UPLOAD,eBB_gCMD_TER_MEDIA_DATA_UPLOAD, 
														DataLen + Offset, (TotalPack | (PackIndex << 16)),  AudioData);
			
			//if (UpType != RECORD_ALL_PACK_UPLOAD)	return;	

			if (TotalPack == PackIndex)
			{
				Uart.puts("Finish all record data to gprs\r\n");

				//Uart2.Init(Baudrate.U2_232,U2_PIN_FOR_RS232);	// �л��ֱ��Ĳ�����
				
				return OK;
				
			}
		}
	}
	
	return OK;

	#else

	return OK;

	#endif

}



uchar   RecordCls::WaitForRecordData(uint16 *pDataLen, uchar *pData, uint16  *pTotalPack, uint16 *pPackIndex)
{
	#if (USE_CARMERA_MODEL==1)
	uchar	*pFrame = NULL;
	uchar   HeadFlag[4], PackInfoLen;
	uint16 Length;	  
	uint16 DataLen;		  // ���2048�ֽ�
	uint16 CheckSum = 0;          // ����ļ���
	pGY_COMM_PROTOCOL pGyPro;
	
		
	Length = Uart2.WaitGetRecBuffer(RecordBuffer);	 	
	if( (Length>0) )
	{
		//Uart.puts("U2 rec record data:"); Uart.putb(1, Length, RecordBuffer);  os_dly_wait(1);

		HeadFlag[0] = '*'; HeadFlag[1] = 'G'; HeadFlag[2] = 'Y'; HeadFlag[3] = ':'; 
		pFrame = (uchar*)SearchArray(Length, RecordBuffer,4,(uchar*)HeadFlag);
		if (pFrame != NULL)
		{
			// ��������
			pGyPro = (pGY_COMM_PROTOCOL)pFrame;
			
			DataLen = pGyPro->Attr.Bits.Len | (pGyPro->Attr.Bits.LenH << 8);
			if (DataLen == 2047)  DataLen = 2048;
		
			PackInfoLen = 0;
			if (pGyPro->Attr.Bits.Pack)	PackInfoLen = LEN_GY_PACK_INFO;
		
			// У��
			if (pGyPro->Attr.Bits.Check == 0x02)  // CRC16  У�鷽ʽ
			{
				CheckSum = CalCRC16((uchar *)&pGyPro->Cmd, DataLen + LEN_GY_CMD_TO_ATTR + PackInfoLen);	
			}
	
			if (CheckSum != pGyPro->Check)
			{
				Uart.puts("Record Model CheckSum Error\r\n");
				os_dly_wait(1);
	
		    	SendCmdToRecord(DOWN_CMD_CHECK_ERROR, 2, (uchar *)"ER");
	
				return ERROR;
			}

	
			switch (pGyPro->Cmd)
			{
			case	UP_CMD_RECORD_ON:
				if (pGyPro->Data.Buffer[PackInfoLen] == 'O' && (pGyPro->Data.Buffer[PackInfoLen+1] == 'K'))
				{
					Uart.puts("Start Record ...");
					os_dly_wait(1);
				}
				else
				{
					Uart.puts("Record Error: No SD Card!");
					os_dly_wait(1);	
				}
	
				break;
			
			case	UP_CMD_RECORD_OFF:
				Uart.puts("Stop record");
				os_dly_wait(1);
				RecordDataReady = TRUE;
	
				break;
	
			case	UP_CMD_RECORD_UPLOAD:		 // ��1KΪһ���������ϴ�
				//
				SendCmdToRecord(DOWN_CMD_RECORD_UPLOAD, 2, (uchar *)"OK");
				//memmove(pData, &pGyPro->Data.Buffer[PackInfoLen],DataLen);
				//*pDataLen = DataLen;
	
				break;
			case	UP_CMD_CHECK_ERROR:   		 //  ¼��ģ�鱨����λ��ǰһ��ָ��У�����
				Uart.puts("Upper Down Pre Cmd Checksum Error");
				os_dly_wait(1);
	
				break;
			case	UP_CMD_RECORD_PACK_DATA:		 // ��512�ֽ�Ϊһ��
				if ((pData != NULL) && (DataLen == 512))
				{
					if (pTotalPack != NULL)	  *pTotalPack  = pGyPro->Data.Pack.Total;
					if (pPackIndex != NULL)	  *pPackIndex  = pGyPro->Data.Pack.Index;
					SendCmdToRecord(DOWN_CMD_RECORD_UPLOAD, 2, (uchar *)"OK");
					memmove(pData, &pGyPro->Data.Buffer[PackInfoLen], DataLen);
					*pDataLen = DataLen;
				}
				else 
				{
					Uart.puts("Audio File Name Invalid");
					return  ERROR;		
				}
	
				break;	
			case	UP_CMD_GET_TOTAL_PACK:				// ����ܰ���
				if (DataLen == 2)
				{
					//RecordTotalPack = pGyPro->Data.Buffer[PackInfoLen] | (pGyPro->Data.Buffer[PackInfoLen+1]<<8);
					Uart.puts("Record Total Pack:"); //Uart.putb(1, 2, (uchar *)&RecordTotalPack); os_dly_wait(1);
					memmove(pData, &pGyPro->Data.Buffer[PackInfoLen], 2);
					*pDataLen = 2;
				}
				else
				{
					Uart.puts("Get TotalPack Error\r\n");
				}
				
			
			case	UP_CMD_MP3_PLAY	:
				Uart.puts("Play Record File...\r\n");
			
			default:
				Uart.puts("Record Other Data:");
				Uart.putb(1, Length, RecordBuffer); 
				return ERROR;
				
				break;			
			}
		}
		else
		{
			Uart.puts(" U2δ�ҵ�֡ͷ\r\n");
			Uart.putb(1, Length, RecordBuffer); 
			return ERROR;
		}	
			
	}
	else 
	{
		Uart.puts("U2������");
		return ERROR;
	}

	return OK;

	#else
	return OK;
	#endif

}





void		RecordCls::CounterFun(void)
{
	#if (USE_CARMERA_MODEL==1)
	if(RecordTime)	
	{
		RecordTime--;
		if( (RecordTime==0)&&(Record_On==TRUE))
		{
			StopRecord();
		}
	}
	#endif

}





void 	RecordCls::Init(void)
{
	#if (USE_CARMERA_MODEL==1)

	Record_On = Record_On = FALSE;
	UploadDataToCenerFlag = FALSE;
	AudioId = 0;
	SendAudioData = FALSE;
	RecordDataReady = FALSE;
	RecordTotalPack = 1;
	RecordTaskRun = 0;
	Init_Rs232 = 0;
	#endif
}





void	   RecordCls::StartRecord(uint16	Time)
{
	#if (USE_CARMERA_MODEL==1)

	Init();
	Record_On = TRUE;
	RecordTime = Time;
	RecordTaskRun = TRUE;
	U2PinSelDevice = U2_FOR_AUDIO;

	#endif
}


void	   RecordCls::StopRecord(void)
{
	 #if (USE_CARMERA_MODEL==1)
		//if (Record_On == TRUE)
		{
			Record_On = FALSE;
			Record_Off = TRUE;
		}

	 #endif
}




RecordCls::RecordCls(void)
{
	#if (USE_CARMERA_MODEL==1)
	RecordBuffer = pShareMem;
	#endif
}
