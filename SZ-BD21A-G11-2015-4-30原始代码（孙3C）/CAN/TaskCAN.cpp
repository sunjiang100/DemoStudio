/*----------------------------------------------------------------------------
 *      R T L    C A N   D r i v e r   E x a m p l e   1
 *----------------------------------------------------------------------------
 *      Name:    CAN_Ex1.c
 *      Purpose: RTX CAN Driver usage example
 *      Rev.:    V3.20
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2008 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/



#define		CAN_GLOBALS			1
#include		"CAN.h"


#include		"..\Uart\Uart0.h"
#include		"..\GSM_GPRS\GSM_GPRS.h"
#include		"..\FLASH\DFE_25xxx.H"
#include		"..\GSM_GPRS\JT_T808.h"	
#include		"..\RTC\RTC.h"
#include			"..\Common\FLASH_ADDR.H"
#include		<string.h>

#define		_PLUS_PLUS_			1



unsigned long	Tx_val=0,Rx_val=0;
volatile uint32 CanBusRecTickCnt;

JT_CAN_DATA_UPLOAD	JT_CanDataUpload;

/*----------------------------------------------------------------------------
 *  Function for converting 1 byte to string in hexadecimal notation
 *---------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------
 *  Functions for init and getting input value
 *---------------------------------------------------------------------------*/





/*----------------------------------------------------------------------------刘工  22644252
 *  Task 1: Sends message with input value in data[0] over CAN periodically
 *---------------------------------------------------------------------------*/
extern		uchar		 jt_auth_ok_flag;
__task		void 	task_send_CAN (void) 
{
	//uint32	TickCnt=0;
	while(1)
	{
		TC.CANSend = 0;
		os_dly_wait(5);		// 
		CanBus.eTaskSend();
			
	}
}




__task		void task_rece_CAN (void)	
{
	uint32	TickCnt=0;
	 CanBusRecTickCnt = 0;
	  while(1)
	  {
	  		TC.CANRecv = 0;
			TickCnt++;
			
			NOP();	NOP();	NOP();	NOP();
			if((JT_ShutCommunication==0) && (GG_Status.Flag.Bits.UpdateRemoteSet == 0))
			{				
			   CanBus.eTaskReceive();
			}
			else
			{
				os_dly_wait(100);
			}
			NOP();	NOP();	NOP();	NOP();
			if(TD.MainTask==tTASK_RUN_FLAG)	
			{
				if(TickCnt%100==0)	Uart.puts("Can rec task run\r\n");
			}
	  }
}




bool			CanBusCls::Init(uchar ch,uint32 Baudrate)
{
	// 暂时屏蔽
	//CAN_setup (ch,Baudrate);                                 					 /// setup CAN Controller #1 
  	//CAN_wrFilter (1, SpecifyCanID[1], EXTENDED_FORMAT);          /// Enable reception of messages 
  	//CAN_start (1);                                  				 /// start CAN Controller #1
	CanBusNumberSID = 0;
	LastPageCanData = 0;
	GetDataCnt = 0;
	GetAllDataCnt = 0;
	ByPassCanCnt = 0;
	GetSpecifyCanIdCnt = 0;
   
	return OK;
  
}







void			CanBusCls::eTaskSend(void)
{
	//uint32	mSec;	
	SendTickCnt++;				// +50ms
 	if( (jt_auth_ok_flag!=0)&&(TestCanBus==TRUE)&&(CanBusNumberSID>0) )
	{	
		
		if( ( SendTickCnt%(JT_CanSet.Can1UploadInterval*20))==0 )
			DataUpLoadToServer();
	}
}

/*----------------------------------------------------------------------------
 *  Task 2: Received CAN message
 *---------------------------------------------------------------------------*/

extern	uchar jt_auth_ok_flag;

void			CanBusCls::eTaskReceive(void)
{
	union
	{
		uchar		Buffer[256];
		uint16	B16[256/2];
		uint32	B32[256/4];
	}Tp;
	OS_RESULT	Result;
	uchar 	SaveFlag,PageCnt;
	uint32	i,Addr;
  		
	SaveFlag = PageCnt = 0;
	Can1RecOverTimeCnt++;
	Result = os_evt_wait_or( 0x0001, 300 );		// 
	if( Result == OS_R_EVT)
	{
		SaveFlag = TRUE;	
		LastPageCanData = 0;	
		PageCnt = 2;	
	}	
	else if(Can1RecOverTimeCnt==5)	 // 超时无指定ID数据，存储最后一页，可能不够21条
	{	 	
		if( GetDataCnt )		// 当GetDataCnt>0时，Can1RecOverTimeCnt为0
		{
			PageCnt = GetDataCnt/CAN_DATA_NUMBER_PER_PAGE;
			LastPageCanData = GetDataCnt%CAN_DATA_NUMBER_PER_PAGE;
			if(LastPageCanData)	PageCnt++;
			SaveFlag = TRUE;
			GetDataCnt = 0;		
			memmove( TemporaryCanData,GetDataBuffer,512);
		}
	}
	if(SaveFlag==TRUE)
	{
		for(i=0;i<PageCnt;i++)
		{
			Addr = FF_CAN_DATA_START_PAGE;
			Addr += (FlashPageCnt.CanBusDataSave%FF_CAN_DATA_TOTAL_PAGE);
	
			if((Addr%FF_PAGE_PER_SECTOR)==0)
				DFF.SectorEraseWaitOK( Addr/FF_PAGE_PER_SECTOR ,10);
			
			memmove( Tp.Buffer,&TemporaryCanData[i*252],252);
			Tp.B16[126]  = FlashPageCnt.CanBusDataSave;
			Tp.B16[127] = CalAddingSum( 254,Tp.Buffer );
			DFF.PageProgramWaitOK(Addr,Tp.Buffer,5);
			FlashPageCnt.CanBusDataSave++;	
		}
		if(TD.CAN==tCAN_TEST_DATA)
		{
			if( PageCnt == 1)		i = 252;
			else						i = 504;
			Uart.puts(" Save Can Test Data:");
			Uart.putb(1,i,TemporaryCanData);
			Uart.puts("\r\n");
		}	
		SetBufferValue(TemporaryCanData,512,0);
	}
	
}






uint32		CanBusCls::GetSpecifyCanData(uchar *pData)
{
	uint32	Len;
	Len = SpecifyNumber<<3;
	memmove(pData,SpecifyCanData,Len);
	return Len;	
}



uint32			CanBusCls::SetGetSpecifyCanID(uchar Type,uchar Number,uint32 *pID)
{
	uint32	i;
	
	NOP();	NOP();	
	if(pID == NULL)		return CanAutoSendInterval; 		// ID指针为空，直接返回时间间隔
	
	if(Type)		//  设置
	{
		for(i=0;i<Number;i++)
		{
			SpecifyCanID[i] = *(pID+i+1);		
		}
		SpecifyNumber = Number;
		CanAutoSendInterval = *pID;		// 第0个为间隔
		DFE.Write(EE_CAN_ID_NUMBER,4,(uchar*)&SpecifyNumber);		
		DFE.Write(EE_CAN_GET_INTERVAL,4,(uchar*)&CanAutoSendInterval);	
		DFE.Write(EE_CAN_SPECIFY_ID,(Number<<2),(uchar*)&SpecifyCanID);		
	}
	else			// 读取
	{
		*(pID+0) = CanAutoSendInterval;
		for(i=0;i<SpecifyNumber;i++)
		{
			*(pID+i+1) = SpecifyCanID[i];
		}		
	}
	return SpecifyNumber;
}


uchar	CanBusCls::JTSetSpecifyCanID(uchar IdIndex, uchar *pID_Set)	  //  设置符合JT808的 CAN总线ID信息
{	
   uchar *p;

	if ((IdIndex > 16) || (pID_Set == NULL))  return ERROR;

	p = pID_Set;
	JTCanSampleInterval[IdIndex] = ByteToINT32(*p, *(p+1),*(p+2),*(p+3));
	SpecifyCanID[IdIndex] = ByteToINT32(*(p+4), *(p+5), *(p+6), *(p+7)); // 含: bit31: CAN通道号，bit30:帧类型，bit29:采集方式，bit28-bit0: CAN总线ID
	SpecifyCanID[IdIndex] &= 0x1fffffff;
	
	DFE.Write((EE_CAN_SPECIFY_ID+(IdIndex<<2)),4,(uchar*)&SpecifyCanID[IdIndex]);	
	NOP(); NOP(); NOP();	
	


	if( JTCanSampleInterval[IdIndex] )
	{
		CAN_setup (1,250000UL);                                 					 /// setup CAN Controller #1 
  		//CAN_wrFilter (1, SpecifyCanID[IdIndex], EXTENDED_FORMAT);          /// Enable reception of messages 
  		CAN_start (1);   
		GetCanID = SpecifyCanID[IdIndex];                               				 /// start CAN Controller #1
		CanBusNumberSID = 0;
		LastPageCanData = 0;
		GetDataCnt = 0;
		GetAllDataCnt = 0;
		ByPassCanCnt = 0;
		GetSpecifyCanIdCnt = 0;
		CanOverTimeCnt = 0;
		Can1RecOverTimeCnt = 0;
		FlashPageCnt.CanBusDataSave = 0;
		FlashPageCnt.CanBusDataRead = 0;
		Uart.puts("Set Can ID,Test Data:");
		Uart.putb(1,1,(uchar*)&IdIndex);
		Uart.putb(1,4,(uchar*)&JTCanSampleInterval[IdIndex]);
		Uart.putb(1,4,(uchar*)&SpecifyCanID[IdIndex]);
		Uart.puts("\r\n");
	}
	else		  	// 采集时间为0，
	{
		ByPassCanID = SpecifyCanID[IdIndex];
	}

	return OK;

}





uint32		CanBusCls::ReadTemporaryCanData(uchar Number,uint32 ID,uchar *pData)
{
	return 0;
}


// 公共内存
extern   	uchar  	*pShareMem;

void			CanBusCls::DataUpLoadToServer(void)
{
	union
	{
		uchar		Buffer[96];
		uint16	B16[32];
		uint32	B32[16];
	}Tp;
	pRTC_STR		pRtc;
	uint32		i,Len,Addr;
	uint16		Total;
	uchar			*pData;
	if(FlashPageCnt.CanBusDataRead<FlashPageCnt.CanBusDataSave)			// 存储时每次4页，所以这里不判断>=4
	{		
		SetBufferValue( Tp.Buffer,64,0);
		strcpy((char*)Tp.Buffer,"CAN BUS数据出队，");
		Len = strlen((char*)Tp.Buffer);
		Int2Str( FlashPageCnt.CanBusDataRead, (char*)&Tp.Buffer[Len]);		
		strcat((char*)Tp.Buffer,",");
		Len = strlen((char*)Tp.Buffer);
		Int2Str( FlashPageCnt.CanBusDataSave, (char*)&Tp.Buffer[Len]);		
		strcat((char*)Tp.Buffer,",");
		Len = strlen((char*)Tp.Buffer);
		Int2Str( GetAllDataCnt, (char*)&Tp.Buffer[Len]);
		Uart.puts((char*)Tp.Buffer);  
		
		Total = 0;
		pData = pShareMem+7;
		for(i=0;i<2;i++)
		{			
			if(FlashPageCnt.CanBusDataRead<FlashPageCnt.CanBusDataSave)
			{
				Addr = FF_CAN_DATA_START_PAGE;
				Addr += (FlashPageCnt.CanBusDataRead%FF_CAN_DATA_TOTAL_PAGE);
				DFF.PageRead( Addr, (pData+i*252) );		// 每页存储21条（252 = 12*21）
				FlashPageCnt.CanBusDataRead++;
				if( (LastPageCanData>0)&&(FlashPageCnt.CanBusDataRead==FlashPageCnt.CanBusDataSave) )
				{
					Total += LastPageCanData;
					SetBufferValue( Tp.Buffer,96,0);

					strcpy((char*)Tp.Buffer,"\r\n指定ID数: ");
					Len = strlen((char*)Tp.Buffer);
					Int2Str( GetSpecifyCanIdCnt, (char*)&Tp.Buffer[Len]);

					strcat((char*)Tp.Buffer,"，上传ID数：");
					Len = strlen((char*)Tp.Buffer);
					Int2Str( CanBusNumberSID, (char*)&Tp.Buffer[Len]);					

					strcat((char*)Tp.Buffer,"，过滤ID数：");
					Len = strlen((char*)Tp.Buffer);
					Int2Str( ByPassCanCnt, (char*)&Tp.Buffer[Len]);	

					strcat((char*)Tp.Buffer,",总条数：");
					Len = strlen((char*)Tp.Buffer);
					Int2Str( GetAllDataCnt, (char*)&Tp.Buffer[Len]);
					
										
					Uart.puts((char*)Tp.Buffer);
				}
				else
				{
					Total += CAN_DATA_NUMBER_PER_PAGE;
				}
			}
			else
			{
				break;
			}
			if(FlashPageCnt.CanBusDataRead>=FlashPageCnt.CanBusDataSave)
				TestCanBus = 0;
		}
		pRtc = (pRTC_STR)&Tp.Buffer;
		Rtc.SecondToDateTime(TestCanStartTime,pRtc);
		pShareMem[0] = Total>>8;
		pShareMem[1] = Total;
		pShareMem[2] = HexToBcd(pRtc->Hour);
		pShareMem[3] = HexToBcd(pRtc->Min);
		pShareMem[4] = HexToBcd(pRtc->Sec);
		pShareMem[5] = 0x10;
		pShareMem[6] = 0x00;
		Len = Total*LEN_ONE_CAN_DATA+7;
		GPRS.JT808_DataPackPushAlignment(eBB_gCMD_TER_CAN_DATA_UPLOAD,eBB_gCMD_TER_CAN_DATA_UPLOAD,Len,0,pShareMem);
		TestCanStartTime += 4;
		
		if(TD.CAN==tCAN_TEST_DATA)
		{
			Uart.puts(" GPRS Can Test Data:");
			Uart.putb(1,Len,pShareMem);
			Uart.puts("\r\n");
		}
		
	}
}











CanBusCls::CanBusCls(void)
{
}




/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
