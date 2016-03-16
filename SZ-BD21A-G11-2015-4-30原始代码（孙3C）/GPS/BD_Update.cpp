
#define				BD_UPDATE_GLOBALS					1



#include	 	"BD_Update.h"
#include		"..\Common\Algol.h"
#include		"..\FLASH\DFE_25xxx.H"
#include		"..\FLASH\DFF_25xxx.H"
#include		"..\UART\Uart0.h"
#include		"..\UART\Uart3.h"
#include		"..\Common\FLASH_ADDR.H"		




	

#define		GNSS_UPDATE_FILE_IN_FLASH_PAGE_ADDR		FF_IAP2_CODE_START_PAGE				// �����ļ���FLASH�е�ҳ��ַ



extern uchar *pShareMem; 

uchar		StartBDUpdate = FALSE;



#if(USE_BD_UPDATE)

static uint16 AT_BUP_EscapeCharDecode(uint16 Len, uchar *pBuf)		 // ��ת��
{

	uchar		c,c1;
	uchar		*pMal=NULL;
	uint16	i=0,j;
	NOP();	NOP();	
	
	while(pMal==NULL)
	{
		pMal = MyMalloc(Len);
		os_dly_wait(1);		
	}
	for(j=0,i=0;i<Len;i++)
	{
		c = *(pBuf+i);
		c1 = *(pBuf+i+1);
		if( (c == 0xdc)&&(c1==0xcb)	)	  	// 0xdc 0xcb -> 0xdb
		{
			*(pMal+j) = 0xdb;		j++;	
			i++;		
		}							
		else if( (c == 0xdc)&&(c1==0xcc)	)	// 0xdc 0xcc -> 0xdc
		{
			*(pMal+j) = 0xdc;		j++;			
			i++;
		}
		else if ( (c == 0xdc)&&(c1==0xce)	)		 	// 0xdc 0xce -> 0xde
		{
			*(pMal+j) = 0xde;		j++;			
			i++;
		}
		else
		{
			*(pMal+j) = c;		j++;
		}
	}	
	memmove(pBuf,pMal,j);
	MyFree(pMal);
	return j;
	
		 
}


static uint16 AT_BUP_EscapeCharEncode(uint16 Len, uchar *pBuf)	  //  ת��
{
	uchar		c;
	uchar		*pMal=NULL;
	uint16	i=0,j;
	NOP();	NOP();	
	
	while(pMal==NULL)
	{
		pMal = MyMalloc(Len);
		os_dly_wait(1);		
	}
	for(j=0,i=0;i<Len;i++)
	{
		c = *(pBuf+i);
		if(c == 0xdb)		  	// 0xde -> 0xdc 0xcb
		{
			*(pMal+j) = 0xdc;		j++;
			*(pMal+j) = 0xcb;		j++;
		}							
		else if(c == 0xdc)	// 0xdc -> 0xdc 0xcc
		{
			*(pMal+j) = 0xdc;		j++;
			*(pMal+j) = 0xcc;		j++;
		}
		else if (c == 0xde)		 // 0xde -> 0xdc 0xce
		{
			*(pMal+j) = 0xdc;		j++;
			*(pMal+j) = 0xce;		j++;	
		}
		else
		{
			*(pMal+j) = c;		j++;
		}
	}	
	memmove(pBuf,pMal,j);
	MyFree(pMal);
	return j;
		 
}



static uint16 	AT_BUP_Pack(uchar Cmd, uint16 DataLen, uchar *pData, uchar *pReturnPackData)		 // ���
{
	uchar Check;
	AT_BUP_PROTOCOL_HEAD_TYPE	 AtBup;
	uint16 Len;
	
	AtBup.Head = 0xdb;
	AtBup.Len = DataLen + 3;
	AtBup.DevId = 0x01;
	AtBup.Cmd = Cmd;

	Len =  sizeof(AT_BUP_PROTOCOL_HEAD_TYPE);
	memmove(pReturnPackData, (uchar *)&AtBup, Len);

	memmove(pReturnPackData+Len, pData, DataLen);

	Len += DataLen;

	Check = CalXorSum(Len-1, pReturnPackData+1); // ����ͷ0xdb

	//Uart.puts("Check Test Data:"); Uart.putb(1, 1, &Check);Uart.puts("\r\n");
											
	pReturnPackData[Len++] = Check;

	Len = AT_BUP_EscapeCharEncode(Len-1, pReturnPackData+1); // ת�岻����ͷ
	
	Len += 1;
	pReturnPackData[Len++] = 0xde;


	return 	Len;
		
}


static uchar 	AT_BUP_UpPack_Proccess(uint16 DataLen, uchar *pData)	// ���
{
	uchar Head = 0xdb, End = 0xde;
	uchar *pHead, *pEnd;
	uchar Len, Check;
	pAT_BUP_PROTOCOL_HEAD_TYPE	 pAtBup;
	char tBuffer[8];
	uint16	Tmp;

	pHead = (uchar *)SearchArray(DataLen, pData, 1, (uchar *)&Head);

	if (pHead != NULL)
	{
		Len =  DataLen - (pHead - pData) - 1;
		pEnd = (uchar *)SearchArray(Len, pHead, 1, &End);

		if (pEnd != NULL)
		{
			Len = pEnd - pHead + 1;
			AT_BUP_EscapeCharDecode(Len, pHead); // 

			pAtBup = (pAT_BUP_PROTOCOL_HEAD_TYPE)pHead;

			Check = CalXorSum(Len-3, pHead+1);

			if (Check != pHead[Len-2])
			{
				  Uart.puts("У�����\r\n");
				  return ERROR;
			}



			switch(pAtBup->Cmd)
			{
			case	AT_BUP_UP_UPDATE_BAUDRATE_CMD:
				if (pHead[AT_BUP_DATA_INDEX+1] == 0x00) // ACK
				{
					Uart.puts("���ò����ʳɹ�");
					return OK;
				}
				else if ((pHead[AT_BUP_DATA_INDEX+1] == 0x01))
				{
					Uart.puts("��֧�ָò�����");
					
				}
				else if ((pHead[AT_BUP_DATA_INDEX+1] == 0x10))
				{
					Uart.puts("�������,���ط�");
				
				}
				else
				{
					Uart.puts("��������");
					
				}
					

			break;

			case	AT_BUP_SET_FIRM_UPATE_PARA_CMD:
				Uart.puts("ģ��֧�ֵ�������ݰ�����:");

				Tmp =   pHead[AT_BUP_DATA_INDEX] | (pHead[AT_BUP_DATA_INDEX+1]<<8); 
				Int2Str(Tmp, tBuffer);
				Uart.puts(tBuffer);
				Uart.puts("\r\n");
				
				if (pHead[AT_BUP_DATA_INDEX+2] == 0x00) // ACK
				{
					Uart.puts("֧�ֵ�ǰ�̼�������");
					return OK;
				}
				else if ((pHead[AT_BUP_DATA_INDEX+2] == 0x01))
				{
					Uart.puts("�������ʹ���");
					
				}
				else if ((pHead[AT_BUP_DATA_INDEX+2] == 0x02))
				{
					Uart.puts("���볤�ȴ��󣬴��볤�ȱ���С��256K");
					
				}
				else if ((pHead[AT_BUP_DATA_INDEX+2] == 0x10))
				{
					Uart.puts("�������,���ط�");
					
				}

			break;

			case	AT_BUP_SEND_DATA_PACK_CMD:
				Tmp =   pHead[AT_BUP_DATA_INDEX] | (pHead[AT_BUP_DATA_INDEX+1]<<8); 
				Int2Str(Tmp, tBuffer);
				
				if (pHead[AT_BUP_DATA_INDEX+2] == 0x00) // ACK
				{
					Uart.puts("�������ݰ�:");
					Uart.puts(tBuffer);
					Uart.puts("\r\n");

					return OK;
				}
				else if ((pHead[AT_BUP_DATA_INDEX+2] == 0x01))
				{
					Uart.puts("������š��汾�ŵȲ�������");
				}
				else if ((pHead[AT_BUP_DATA_INDEX+2] == 0x02))
				{
					Uart.puts("�汾��û�б仯");
					
				}
				else if ((pHead[AT_BUP_DATA_INDEX+2] == 0x10))
				{
					Uart.puts("�������,���ط�");
					
				}

				break;

			case	AT_BUP_FIRM_UPDATE_FINISH:
				if (pHead[AT_BUP_DATA_INDEX] == 0x00) // ACK
				{
					Uart.puts("�����ɹ�");

					return OK;
				}
				else if ((pHead[AT_BUP_DATA_INDEX] == 0x01))
				{
					Uart.puts("���յ��Ĵ������ݴ��� ");
				}
				else if ((pHead[AT_BUP_DATA_INDEX] == 0x02))
				{
					Uart.puts("������д���� ");
					
				}
				else if ((pHead[AT_BUP_DATA_INDEX] == 0x03))
				{
					Uart.puts("������֤����");
					
				}
			

			break;

			case	AT_BUP_UPDATE_OK_RETSET_CMD:
				if (pHead[AT_BUP_DATA_INDEX] == 0x00) // ACK
				{
					Uart.puts("���յ���ȷ����������");

					return OK;
				}
				else if ((pHead[AT_BUP_DATA_INDEX] == 0x01))
				{
					Uart.puts("���������Ҫ�ط�");
				}
		

			break;
																								  

			default:
				break;
				
			}


			
		}
	}

	  	
	 return ERROR;
}






/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
  �������ƣ�  GNSSUpdateProccess
  ����������  ��λģ����������
  ��ڲ���: 
  ���ڲ�����
  ȫ�ֱ���: 
  ����ʱ�䣺
  �޸�ʱ�䣺
*/
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
static uchar tBuffer[256+4];
SET_GNSS_FIRW_UPDATE_PARA SetUpPara;
void GPSCls::GNSSUpdateProccess(void)
{
	
	uchar j,LeftLen,Pages, RSendTime;
	pBD_UPDATE_FILE_HEAD_TYPE pBD;
	pSEND_BD_UP_DATA_PACK pSendPack;
	uint16 FirmPages, TotalPack;
	uint32 Len, FirmLen;
	uint32 FirmInFlashPageAddr;
	uchar *pBuf, *pSendBuf;
	uint32	i;

	if (pShareMem == NULL)
	{
		Uart.puts("pShareMem �����ռ�\r\n");
	}

	pBuf = pShareMem;
	pSendBuf = pShareMem + 2048;  // ע��pMem��4K�Ŀռ�


	// �������Ļ�ȡ������Ϣ
	SetBufferValue(tBuffer, 256,0);
	DFF.PageRead(GNSS_UPDATE_FILE_IN_FLASH_PAGE_ADDR,tBuffer);

	pBD =  (pBD_UPDATE_FILE_HEAD_TYPE)tBuffer;
	// ��ȡATGMģ��Ӳ����Ϣ

	//Uart.puts("BDUP Test Data:"); Uart.putb(1, sizeof(BD_UPDATE_FILE_HEAD_TYPE),tBuffer); Uart.puts("\r\n");

	// ����ͷ��Ϣ
	if (pBD->Head != 0x5441)	// "AT"
	{
		Uart.puts("FLASH�����ڱ����̼�����\r\n");
		return;
	}

	 // ����
	if ((pBD->FirmStartAddr==NAVIGATION_CODE_ADDR) && (pBD->FirmType==NAVIGATION_CODE_TYPE))		//  ��������	 
	{
		Uart.puts("��������\r\n");

		SetUpPara.CodeType = pBD->FirmType;
		SetUpPara.Length = pBD->FirmLength;
		SetUpPara.StAdd = 0x00;	// �� CodeType=3 ʱ�� StAdd=0x3e000������ֵʱ�� StAdd=0 

		FirmLen = pBD->FirmLength;

		 FirmPages = pBD->FirmLength / 256;				 // ����̼���FLASH����ҳ
		 LeftLen = pBD->FirmLength % 256;
		 if (LeftLen != 0)
		 {
		 	FirmPages += 1;
		 }

		 FirmInFlashPageAddr = GNSS_UPDATE_FILE_IN_FLASH_PAGE_ADDR + pBD->FirmInFileStartAddr / 256;	// һҳFLASH��256�ֽ� ���ļ�����������FLASH�еĹ̼���ʼ��ַ������ҳ���룬�Ȳ����ǲ���������

		 //SetBufferValue(tBuffer, 256,0);
		 //DFF.PageRead(FirmInFlashAddr,tBuffer);

		 //Uart.puts("BD Firm Test Data:"); Uart.putb(1, 256, tBuffer); Uart.puts("\r\n");
		 os_dly_wait(10);

		 //Uart3.puts("$PCAS06,1*1A\r\n");	//   ģ��Ӳ����Ϣ

		 // ����������������
		 RSendTime = 3;				 // �ط�3��
		 while(RSendTime)
		 {
			 //StartBDUpdate	= TRUE;
			 		 
			 SetBufferValue(tBuffer, 128,0);
			 Uart3.ClearRecBuffer();
	
			 Uart3.puts("$PCAS20*03\r\n");		
	
			 Uart3.WaitRecBuffer(700, 128,tBuffer);	  //  ���7��
			 
			 Uart.puts((char *)tBuffer);
			 os_dly_wait(1);
	
			 if (SearchArray(128, tBuffer, 7, (uchar *)"$PCAS30") == NULL)
			 {
			 	Uart.puts("\r\n������������ʧ��\r\n");
				RSendTime--;
				if (RSendTime==0)
				{ 
					Uart3.Init(9600);
					return;	  // ��������
				}
			
			 }
			 else
			 {
			 	Uart.puts("\r\n�������������ɹ�\r\n");
			 	break;
			 }
		 }

		 // ����������
		 Uart.puts("����������\r\n");
		 pBuf[0] = 0x05;		// 0x01: 9600, 0x05: 115200   
		 Len = AT_BUP_Pack(AT_BUP_UP_UPDATE_BAUDRATE_CMD, 1, pBuf, pSendBuf);	

	
		 SetBufferValue(tBuffer, 128,0);
		 Uart3.ClearRecBuffer();

		 Uart.puts("BAUD  Test Data:"); Uart.putb(1, Len, pSendBuf); Uart.puts("\r\n");
		 Uart3.putb(Len, pSendBuf);
		 
		 Uart3.WaitRecBuffer(700, 128,tBuffer);	  //  ���7��
		 
		 Uart.puts("Ack BAUD Test Data:"); Uart.putb(1, 64, tBuffer); Uart.puts("\r\n");
		 os_dly_wait(1);


		 if (AT_BUP_UpPack_Proccess(128, tBuffer)	!= OK)
		 {
		 	  	 Uart.puts("����������ʧ��\r\n");
				 return;
		 }
	
		 Uart.puts("\r\n�л���������\r\n");
		 Uart3.Init(115200);						//  �����л�115200������
		 os_dly_wait(20);		      // ��ʱ

		 // ���ù̼���������	 /////////////////////////////////////////////////
		 Uart.puts("���ù̼���������\r\n");

		Len =  sizeof(SET_GNSS_FIRW_UPDATE_PARA);
		memmove(pBuf, (uchar *)&SetUpPara, Len); 
		Len = AT_BUP_Pack(AT_BUP_SET_FIRM_UPATE_PARA_CMD, Len, pBuf, pSendBuf);	

		RSendTime = 3;				 // �ط�3��
		 while(RSendTime)
		 {
		 	 SetBufferValue(tBuffer, 128,0);
			 Uart3.ClearRecBuffer();

			 Uart.puts("SetPara  Test Data:"); Uart.putb(1, Len, pSendBuf); Uart.puts("\r\n");
			 Uart3.putb(Len, pSendBuf);
			 
			 
			 Uart3.WaitRecBuffer(700, 128,tBuffer);	  //  ���7��
			 
			 Uart.puts("Ack SetPara Test Data:"); Uart.putb(1, 64, tBuffer); Uart.puts("\r\n");
			 os_dly_wait(1);
	
			 if (AT_BUP_UpPack_Proccess(128, tBuffer)	!= OK)
			 {
			 	  	 Uart.puts("���ù̼���������ʧ��\r\n");
	
					 //Uart3.Init(9600);
					 os_dly_wait(100);
					 RSendTime--;
					 //return;
					 if (RSendTime==0) return;	  // ��������
			 }
			 else
			 {
			 	break;
			 }
		 }

		 // �������ݰ�	 //////////////////////////////////////////////////////////////////
		 Uart.puts("\r\n���͹̼����ݰ�\r\n");

		 TotalPack =  FirmLen / LEN_BD_UP_PACK_DATA;
		
		 if ((FirmLen % LEN_BD_UP_PACK_DATA) != 0)
		 {
		 	TotalPack += 1;
		 }

		 pSendPack = (pSEND_BD_UP_DATA_PACK)pBuf;

		 pSendPack->TotalPk = TotalPack;
			
		 //Uart.puts("TotalPack Test Data:"); Uart.putb(1, 2, (uchar *)&TotalPack); Uart.puts("\r\n"); 
		 //Uart.puts("FirmLen Test Data:"); Uart.putb(1, 4, (uchar *)&FirmLen); Uart.puts("\r\n"); 
		 //os_dly_wait(10);

		 // test 
		 //SetBufferValue(tBuffer, 256,0);
		 //DFF.PageRead(FirmInFlashPageAddr+0,(uchar *)&pSendPack->Data[0] );
		 //Uart.puts("BD Firm Test Data:"); Uart.putb(1, 256, (uchar *)&pSendPack->Data[0]); Uart.puts("\r\n");
		 //

		 

		 for (i = 1; i<=TotalPack; i++)			  // �����1��ʼ 
		 {
		 	pSendPack->PkNo = i;
			if (i==TotalPack)			 // ���һ��
			{
				Len = FirmLen % LEN_BD_UP_PACK_DATA;		 // ����LEN_BD_UP_PACK_DATA�ֽڵ����ݳ���

				// Uart.puts("Left Len Test Data:"); Uart.putb(1, 2, (uchar *)&Len); Uart.puts("\r\n");

				pSendPack->PkSize = Len;
	
				Pages = Len / 256;		
				LeftLen = Len % 256;		  // ����256���ݵĳ���
	
				if (LeftLen != 0)
				{
					Pages += 1;			 // ��Ҫ��FLASH����ҳ�����һ�����ݶ�ȫ	
				}

				Len += 6;		  // PkNo TotalPk PkSize
			}
			else
			{
				pSendPack->PkSize = LEN_BD_UP_PACK_DATA;
				Pages = 4;
				Len =  sizeof(SEND_BD_UP_DATA_PACK);
			}


			for (j=0; j<Pages; j++)
			{
				DFF.PageRead(FirmInFlashPageAddr+j,(uchar *)&pSendPack->Data[j*256] );
				os_dly_wait(1);
			}
			FirmInFlashPageAddr += Pages;			// ����ҳ��ַ
			
//			if (i==TotalPack)
//			{
//			 	Uart.puts("FirmInFlashPageAddr Test Data:"); Uart.putb(1, 4, (uchar *)&FirmInFlashPageAddr); Uart.puts("\r\n");
//				Uart.puts("Pages Test Data:"); Uart.putb(1, 1, (uchar *)&Pages); Uart.puts("\r\n");
//				Uart.puts("Send Len Test Data:"); Uart.putb(1, 2, (uchar *)&Len); Uart.puts("\r\n");
//				Uart.puts("��ȡ���һ������ Test Data:"); Uart.putb(1, 256, (uchar *)&pSendPack->Data[768]); Uart.puts("\r\n");
//			}				 
		 	
		 	Len = AT_BUP_Pack(AT_BUP_SEND_DATA_PACK_CMD, Len, (uchar *)pSendPack, pSendBuf);	 

//			if (i==TotalPack)
//			{
//			 	Uart.puts("Send Len Test Data:"); Uart.putb(1, 2, (uchar *)&Len); Uart.puts("\r\n");
//			}
		

			RSendTime = 3;				 // �ط�3��
			while(RSendTime)
			{
			 	SetBufferValue(tBuffer, 128,0);
			 	Uart3.ClearRecBuffer();
			
//				Uart.puts("������������������ Test Data:"); Uart.putb(1, Len, pSendBuf); Uart.puts("\r\n");
//			
//				if (i==TotalPack)
//				{
//			 		Uart.puts("�������һ������ Test Data:"); Uart.putb(1, Len, pSendBuf); Uart.puts("\r\n");
//			 	}

			 	Uart3.putb(Len, pSendBuf);
			
				Uart3.WaitRecBuffer(700, 128,tBuffer);	  //  ���7��
			 
			 	//Uart.puts("Ack SetPara Test Data:"); Uart.putb(1, 20, tBuffer); Uart.puts("\r\n");
			   
				//os_dly_wait(1);			 // ��ʱ
			
				 if (AT_BUP_UpPack_Proccess(128, tBuffer)	!= OK)
				 {
				 	  	 Uart.puts("����������ʧ��\r\n");
			
						 //Uart3.Init(9600);
						 os_dly_wait(100);
			
						 RSendTime--;
			
						 if(RSendTime==0)
						 {
							 Uart3.Init(9600);
						 	 return;
						 }
				 }
				 else
				 {
				 	 break;
				 }
			}

		 }

		 // ��ѯ�Ƿ����������֪ͨ	 ////////////////////////////////////////
		 //Uart3.ClearRecBuffer();
		 //SetBufferValue(tBuffer, 128,0);

		 AT_BUP_UpPack_Proccess(128, tBuffer+10); //		���һ��Ӧ����������֪ͨ����ճ��, tBuffer Ҫ10�ֽ�

		 //Uart3.WaitRecBuffer(1500, 128,tBuffer);	  //  ���15��
		 RSendTime = 3;				 // �ز�3��
		 while(RSendTime)
		 {
			 Uart3.ClearRecBuffer(); 
			 SetBufferValue(tBuffer, 128,0);
	
			 Uart3.WaitRecBuffer(1500, 128,tBuffer);	  //  ���15��
			 if (AT_BUP_UpPack_Proccess(128, tBuffer)	!= OK)
			 {
		 	 	Uart.puts("��ѯ�����������֪ͨ\r\n");
			 	//Uart3.Init(9600);
			 	os_dly_wait(10);

			
				 RSendTime--;
	
				 if(RSendTime==0)
				 {
					 Uart3.Init(9600);
				 	 return;
				 }

				
			 }
			 else
			 {
			 	break;
			 }
			 
		 }
		 
		 // ������ɣ�����ģ��
		 Uart.puts("������ɣ�����ģ��\r\n");
		 Len =  sizeof(SET_GNSS_FIRW_UPDATE_PARA);
		 Len = AT_BUP_Pack(AT_BUP_UPDATE_OK_RETSET_CMD, 0, pBuf, pSendBuf);	
		 
		 SetBufferValue(tBuffer, 128,0);
		 Uart3.ClearRecBuffer();

		 Uart.puts("SetPara  Test Data:"); Uart.putb(1, Len, pSendBuf); Uart.puts("\r\n");
		 Uart3.putb(Len, pSendBuf);
		 
		 Uart3.WaitRecBuffer(700, 128,tBuffer);	  //  ���7��
		 
		 Uart.puts("Ack SetPara Test Data:"); Uart.putb(1, 32, tBuffer); Uart.puts("\r\n");
		 os_dly_wait(1);

		 if (AT_BUP_UpPack_Proccess(128, tBuffer)	!= OK)
		 {
		 	  	 Uart.puts("����ģ�����\r\n");

				 Uart3.Init(9600);
				 os_dly_wait(10);
				 return;
		 }

		 NVIC_SystemReset();	 //				//�����������ϵͳ����


		 //StartBDUpdate	= FALSE;

		
		
	}


	
}

#endif


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
  �������ƣ�  GNSSUpdateProccess
  ����������  ��λģ��
  ��ڲ���: 
  ���ڲ�����
  ȫ�ֱ���: 
  ����ʱ�䣺
  �޸�ʱ�䣺
*/
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������

/// $GPTXT,01,01,02,HW=ATGM331C,0003112105070*1C
/// $GPTXT,01,01,02,SW=URANUS2,V2.4.3.0*19\r\n



static uchar  GetGNNSVer(char *pCmdStr, char *pSearch,	char *pVerBuf)
{
	 uchar tBuffer[128];
	 uchar Len, i = 0, RetryTimes;
	 uchar *pS, *pE;
	 
	 RetryTimes = 3;
	 while (RetryTimes) 
	 {

		 SetBufferValue(tBuffer, 128,0);
		 Uart3.ClearRecBuffer();
	
		 Uart3.puts(pCmdStr);			// �鱱��Ӳ���汾
	
		 Uart3.WaitRecBuffer(300, 128,tBuffer);	  //  ���3��
	
		 pS = (uchar *)SearchArray(128, tBuffer, strlen(pSearch), (uchar *)pSearch);
		 if (pS != NULL)
		 {
		 	Len = pS - &tBuffer[0] + 1;
		 	pE = (uchar *)SearchArray(128-Len, pS, 2, (uchar *)"\r\n");
			if (pE != NULL)
			{
				Len = pE - pS - 3;				  // 	  ���������3���ַ�   ��  *19
				memmove(pVerBuf, pS, Len);				  // ��ȡ�汾��Ϣ  	SW=URANUS2,V2.4.3.0
	
				i = Len;

				break;
			}							
			 
		 }
		 else
		 {
		 	RetryTimes--;

			if (RetryTimes==0)
			{
				 pVerBuf[i++]	= '0';
		 		 pVerBuf[i++]	= ',';
		 		 pVerBuf[i++]	= '0';

				 break;
				
			}
		 }
	 }


	 return i;

}

uchar GPSCls::QueryGNSSVersion(char *pVer)
{
	 uchar Len, i;

	 if (pVer == NULL) return ERROR;

	  i = GetGNNSVer("$PCAS06,0*1B\r\n", "SW=", pVer);	// ����汾
	  Len = i;
	  pVer[Len++] = ',';												// �ָ���
	  i = GetGNNSVer("$PCAS06,1*1A\r\n", "HW=",pVer+Len);		//	Ӳ���汾	
	  Len += i;
	  pVer[Len]	 = 0;
	  
	  return OK;					
	
}

