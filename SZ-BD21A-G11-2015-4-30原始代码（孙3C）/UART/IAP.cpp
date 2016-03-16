/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												 
$										 
$                       	 		  
$								  		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      �ļ�����
	�ļ���: 
	����������
	���뻷����
	Ŀ��CPU: 
	���ߣ�
	����ʱ�䣺
	����༭������UltraEdit 11.00a+  �Ʊ�����Ϊ�����ַ�������ɶ��Ը��á�
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/					

#include    	"UART0.h"
#include    	"UART1.h"


#include			"..\Common\GB_T19056.h"
#include			"..\Common\GY_T19056.h"


#include			"..\GSM_GPRS\GSM_GPRS.h"
#include			"..\GSM_GPRS\JT_T808.h"
#include			"..\GPS\GPS.h"
#include			"..\Common\EEPROM_Addr.h"		  
#include			"..\Common\FLASH_ADDR.H"

#include			"..\Common\Algol.h"

#include			"..\RTC\RTC.h"
#include			"..\Uart\UART2.h"
#include			"..\Uart\UART3.h"

#include			<string.h>
#include			<stdlib.h>
#include			<stdio.h>
#include			"..\FLASH\DFE_25xxx.H"
#include			"..\FLASH\DFF_25xxx.H"
#include			"..\Switch\switch.h"
#include			"..\SPI\SSP.h"
#include			"..\IIC\IIC0.h"

#include			"..\KeyDisp\KeyDisp.h"
#include			"..\Speed\Speed.h"
#include			"..\common\Algol.h"
#include		   "..\PWM\PWM.h"
#include			"..\UserSys.h"
//#include			"..\KeyDisp\CN_Code.h"





#pragma		pack(1)
typedef		struct 
{
	uint32   Head;  			  //��ͷ GIAP
	uint32	Check; 			  //��У���
	uchar		Cmd;				  //����
   uchar 	Image; 			  //
   uint16 	IndexKB;		  	  // д��ַ��KBΪ��λ���軯��ҳ��
   uint16	StartPage;		  //��KB
   uint16	DataLen;			  //���ݳ���
   uchar		DataBuffer[1024];	// 
}WRITE_FLASH,*pWRITE_FLASH;




/************************************************************************************************
** �������ƣ�IAPprocess()
** ����������IAP���������
** ��ڲ�������
** ���ڲ�����
** ȫ�ֱ���: ��
** ȫ�ֺ���: 
** ����ʱ�䣺
** �޸�ʱ�䣺2011.09.17
**************************************************************************************************/
uchar		ConfigParaFlag;
#if(USE_BD_UPDATE)
static   uchar		BD_up = FALSE;
#endif
void     UartCls::IAPprocess(void)
{
	union
	{
		uchar		Buffer[4];
		uint16	B16[2];
		uint32	B32;
	}Tp;
	uchar   s, Error=0;
	uint16  Index,Length = 0;
	uint32   t,sum,Addr;
	IAPHAED  *IapN;
	WRITE_FLASH		*pWF;
	uchar		*pData;
	SetBufferValue(TxdTBuffer,(256+16),0);	
	IapN = (IAPHAED *)U0_RecBuffer;
	pWF  = (WRITE_FLASH*)U0_RecBuffer;
	sum = CalAddingSum32(IapN->DataLen + 8, &U0_RecBuffer[8]);
	U0_IapOverTime = 0;
	if(sum==IapN->SumCRC)
	{
		switch(IapN->Cmd)
		{
		   case	 eIAP_CMD_HEAD_SHAKE1:            //��һ������
		   case   eIAP_CMD_HEAD_SHAKE2:      		//�ڶ�������
	
				ConfigParaFlag = 0;
			 if((U0_Iap_Flag == 0)&&(IapN->Cmd==eIAP_CMD_HEAD_SHAKE1))
			 {
				 U0_Iap_Flag = IAP_HAND_SHANE_STEP_1;
			 }
			 else if(U0_Iap_Flag == IAP_HAND_SHANE_STEP_1)
			 {
				 U0_Iap_Flag = IAP_HAND_SHANE_STEP_2;
				 
			 }
			 else if(U0_Iap_Flag == IAP_HAND_SHANE_STEP_2)
			 {
			 	 U0_Iap_Flag = IAP_HAND_SHANE_STEP_2;
				 
			 }
			 else
			 {
			 	 Error = 1;
			 }
			 if(Error == 0)
			 {
			 	 Sys.GetVersion((char *)&TxdTBuffer[IAP_HEAD_LEN]);
			 	
				U0_Iap_cnt = 1000;
				//Length = IAP_HEAD_LEN + 16;
				Length = IAP_HEAD_LEN + 28;
			 }
			 break;
		   case   eIAP_CMD_SET_FIRMWARE:      		//�·���������	  ����1���ݱ�����0~256K�ⲿflash�� ����2���ݱ�����257K~512K�ⲿFlash
				
			 if( (IapN->image == '1')||(IapN->image == '2')	)
			 {
			 		
			 		Error = 0;
			 		Addr = IapN->BagNumber;
			 		Addr--;							// ��λ�����ʹ�1��ʼ
			 		Addr <<= 2;					  	// KBת��Ϊҳ��ַ
			 		if(IapN->image == '1')		Addr += FF_IAP1_CODE_START_PAGE;
			 		else								Addr += FF_IAP2_CODE_START_PAGE;
			 	  	
			 	  	
			 	  	if( Addr%FF_PAGE_PER_SECTOR==0)
			 	  	{
			 	  		DFF.SectorEraseWaitOK( Addr/FF_PAGE_PER_SECTOR ,10);
			 		}	
			 		for(Length=0;Length<1024;Length+=256)
			 		{
			 			s = DFF.PageProgramWaitOK(Addr,(uchar*)&(pWF->DataBuffer[Length]),5);
			 			Addr++;
			 			if(s!=OK)	Error = 3;
			 		}
			 		U0_Iap_cnt = 1000;
					U0_Iap_Flag = IAP_DOWN_LOAD;
					Length = IAP_HEAD_LEN;
			 					 	
				 
					 if(IapN->BagNumber==IapN->BagSum)
					 {
						 	memmove(TxdTBuffer, IapN, IAP_HEAD_LEN);
							strncpy((char *)&TxdTBuffer[IAP_HEAD_LEN], "OK\0", 2);
							Length = IAP_HEAD_LEN + 2;
			            sum = CalAddingSum32(Length - 8, &TxdTBuffer[8]);
			           	memmove(&TxdTBuffer[4], (uchar *)&sum, 4);
		     				putb(0,Length,TxdTBuffer);	
							Uart.PopAlignmentSend();
			 				os_dly_wait(10);
	
							NVIC_SystemReset();	 //				//����������
					 }
			 }
			 else	if(IapN->image == '5')			// дFlash
			 {
			 		Error = 0;
			 	  	Addr = IapN->BagNumber;		
			 	  	Addr <<= 2;						// 4P/KB��ת��Ϊҳ��ַ
			 	  	sum = IapN->BagSum;
			 	  	Addr += sum;

					if(ConfigParaFlag==0)
					{
						ConfigParaFlag = TRUE;
						if(Addr==FF_GPS_MESSAGE_START_PAGE)							// ���Ȳ�������
						{
							Tp.B16[0] = PARA_BLOCK_CONFIG_FLAG;
							DFE.Write(EE_PARA_BLOCK_STATUS,2,Tp.Buffer);
						}
						else if(Addr==FF_GPS_BLIND_MESSAGE_START_PAGE)			// ä����������
						{
							Tp.B16[0] = PARA_BLOCK_CONFIG_FLAG;
							DFE.Write(EE_PARA_BLOCK_STATUS+2,2,Tp.Buffer);
						}
						else if(Addr == FF_VDR_BASE_ADDR)			 				// ��¼�ǲ�������
						{
							Tp.B16[0] = PARA_BLOCK_CONFIG_FLAG;
							DFE.Write(EE_PARA_BLOCK_STATUS+4,2,Tp.Buffer);
						}
					}

			 	  	if( Addr%FF_PAGE_PER_SECTOR==0)
			 	  	{
			 	  		DFF.SectorEraseWaitOK( Addr/FF_PAGE_PER_SECTOR ,10);
			 		}	
			 		for(Length=0;Length<1024;Length+=256)
			 		{
			 			s = DFF.PageProgramWaitOK(Addr,(uchar*)&(pWF->DataBuffer[Length]),5);
			 			Addr++;
			 			if(s!=OK)	Error = 3;
			 		}
			 		U0_Iap_cnt = 1000;
					Length = IAP_HEAD_LEN;

					#if(USE_BD_UPDATE)
					if (IapN->BagNumber==0)					// ��һ��
					{
						 if ((pWF->DataBuffer[0]	== 'A') && 	(pWF->DataBuffer[1]	== 'T'))  //˵�����������ļ�
						 {
						 		BD_up = TRUE;
						 }
					}
					#endif

			 }
			 break;


			/////////////////////////////////////////////////////////////////
		   case   eIAP_CMD_GET_FIRMWARE:     		//�ϴ���������			����Ͳ����л���������	 Ҳ��������   �Ƿ���Ҫ��ָ���أ��ͻ�Ҳ���Զ�������
			 	
				if(IapN->image == '5')			// ��Flash����
				{
					
			 	  	Addr = Index = IapN->BagNumber;
			 	  	Addr <<= 2;								// 4P/KB��ת��Ϊҳ��ַ
			 	  	sum = IapN->BagSum;
			 	  	Addr += sum;				// ��ƫ��
			 	  	GetFlashData.TotalPage = 0xffffffff;
			 	  	//////////////////////////////////////////////////////////////////////////
			 	  	SetBufferValue( U0_SendAligBuffer, (IAP_Firmware_Bag_LEN+IAP_HEAD_LEN),0);
		
					IapN = (IAPHAED *)&U0_SendAligBuffer;
					IapN->Head = 0x50414947;	// GIAP
					IapN->Cmd = eIAP_CMD_GET_FIRMWARE;
					IapN->image = '5';
								
					IapN->BagNumber = Index;
					IapN->BagSum = sum;				// ÿ�ζ�1K������������λ������
					IapN->DataLen = IAP_Firmware_Bag_LEN;
					for(t=0; t<4; t++)
					{
						DFF.PageRead(Addr, &U0_SendAligBuffer[IAP_HEAD_LEN + t*FF_BYTES_PER_PAGE]);						 //
					   DFF.WaitDFFok(200);
						Addr++;
				   }
					
					sum = CalAddingSum32(IAP_Firmware_Bag_LEN + 8, &U0_SendAligBuffer[8]);
				   memmove(&U0_SendAligBuffer[4], (uchar *)&sum, 4);
			
					Txd.Length = IAP_Firmware_Bag_LEN+IAP_HEAD_LEN;
					pTxd = U0_SendAligBuffer;
					Txd.Length--;
					LPC_UART0->THR  = *pTxd++;						// ��������		
					Txd.Sending = TRUE;	
					U0_Iap_cnt = 1000;				
					return;
			 	  	
				}
			 break;

			//  ȫ��һ���ȡ
			case eIAP_CMD_GET_FLASH_DATA_N_PAGE:
				
				if( IapN->image == '3')
				{
					GetFlashData.TotalPage = FF_GPS_BLIND_MESSAGE_TOTAL_PAGE;
					GetFlashData.StartPage = FF_GPS_BLIND_MESSAGE_START_PAGE;
				}
				else if( IapN->image == '4')
				{
					GetFlashData.TotalPage = 128;				
					GetFlashData.StartPage = FF_GPS_MESSAGE_START_PAGE;
				}
				else
				{
					memmove( (uchar*)&GetFlashData, &U0_RecBuffer[IAP_HEAD_LEN],8);
					Length = IAP_HEAD_LEN;
					U0_Iap_Flag = IAP_Up_LOAD;
				}
				
			break;

			//////////////////////////////////////////////////////////////
		   case   eIAP_CMD_CRE_FIRMWARE:     		//У���û�����		   ���Ҳ�����л���������  Ҳ��������
	 		 
			 Error = 5;
			 break;


			case  eIAP_CMD_FINISH:

				U0_Iap_Flag = 0;
				U0_Iap_cnt = 0;
				GetFlashData.TotalPage = 0;
				Length = IAP_HEAD_LEN;

				Uart.Init(UART0_BPS);
				os_dly_wait(10);

				#if(USE_BD_UPDATE)
				if (BD_up==TRUE)
				{
					BD_up = FALSE;
					GPS.BD_UpDateFlag = TRUE;
				}
				#endif

			break;

			 default :
			   Error = 6;
			   break;
		 }
	 }
	 else
	 {
	 	Error = 7;
	 }


   // ������Ϣ����λ��
	
	if(Error!=0)
	{
		strncpy((char *)&TxdTBuffer[IAP_HEAD_LEN], "ER\0", 2);
		TxdTBuffer[IAP_HEAD_LEN+2] = Error+'0';
		Length = IAP_HEAD_LEN + 3;
	}
	else
	{
		if(Length==IAP_HEAD_LEN)
	   {
		   strncpy((char *)&TxdTBuffer[IAP_HEAD_LEN], "OK,\0", 3);
		   pData = (uchar*)&TxdTBuffer[IAP_HEAD_LEN];
		   s = strlen( (char*)pData);
		   Int2Str( IapN->BagNumber, (char*)&TxdTBuffer[IAP_HEAD_LEN+s] );
		   strcat( (char*)pData,",");
		   s = strlen( (char*)pData);
		   Int2Str( IapN->BagSum, (char*)&TxdTBuffer[IAP_HEAD_LEN+s] );
		   s = strlen( (char*)pData);
			Length = IAP_HEAD_LEN + s;
		}
	}
	
	if(Length)
   {
	  memmove(TxdTBuffer, IapN, IAP_HEAD_LEN);
	  sum = CalAddingSum32(Length - 8, &TxdTBuffer[8]);
	  memmove(&TxdTBuffer[4], (uchar *)&sum, 4);
     putb(0,Length,TxdTBuffer);
	}

	
//	if((IapN->Cmd==eIAP_CMD_HEAD_SHAKE1)&&(U0_Iap_Flag == IAP_HAND_SHANE_STEP_1))	// �����Ĳ����ʺ�ʹ�õĲ�����һ���������л�
//	{
//		 Uart.PopAlignmentSend();
//		 os_dly_wait(30);
//		 Uart.Init(UART0_BPS_IAP);
//	}
	
}



