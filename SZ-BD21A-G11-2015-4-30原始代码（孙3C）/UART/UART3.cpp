/***************************************  Copyright (c)  **************************************
**										 
**									
**                      
**	  
****************************************  Copyright (c)  **************************************
**                                         �ļ�����
**	�ļ���:   Uart3.cpp
**	����������UART3�ײ�����.
**	���뻷����Keil for ARM uVision3
**	Ŀ��CPU:  LPC3378
**	���ߣ�	 
**	����ʱ�䣺
** �޸�ʱ��: 3009��07��17��
**	����༭������UltraEdit 11.00a+  �Ʊ�����Ϊ�����ַ�������ɶ��Ը��á�
*************************************************************************************************/	


#define		UART3_GLOBALS			1

#include    "UART3.h"


#include    "UART0.h"



//#include		"..\ExBus\ExBus.h"
#include			<string.h>


#include 	"../GPS/GPS.h"

#include		"..\Config\RTOS.h"
#include		"..\Common\CppGlobals.h"
#include		"..\KeyDisp\KeyDisp.h"
#include			"..\common\Algol.H"




#define		TEST_UART3				0

#define		UART3_TRIGGER_WITH_FIFO		1

extern	uchar StartBDUpdate;

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�
   ����������
   ��ڲ�����
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
uchar			Uart3Cls::GetRecBuffer(char *pGPS,char *pANT,char *pSatel)			  //10ms0
{
	//uchar		cBD_SatelNum,GPS_SatelNum;
	uchar		i,s,GPS_RecOK,GPS_ANT_Check;
	uchar		*pHead,*pEnter;	
	uint16	Len;
	uchar		AntBuf[16];
  	uint32	tl;
	

	NOP();	NOP();	
		
	
	GPS_RecOK = GPS_ANT_Check = FALSE;
		
	/////////////////////////////////////									// GPS���ϼ��(100ms)
	if(Rec.Counter<50)											
	{
		ErrCnt++;
		if(ErrCnt>10)																// 1s
		{
			VehicleStatus.VS2.Bits.GPS_ModuleError = 1;	
			ErrCnt = 0;
		}	
	}
	else
	{
		VehicleStatus.VS2.Bits.GPS_ModuleError = 0;
		ErrCnt = 0;
	}

	///////////////////////////////////////////////////////////////////////
	if(pSatel != NULL)
	{
		pHead = SearchArray(Rec.Counter,(uchar *)U3_RecBuffer,6,(uchar *)"$BDGSV\0");		 // ���� ����
		if (pHead != NULL)
		{	
			GPS.BD_SatelNum = Str2Int( (char*)(pHead+11));				
		}
		
		pHead = SearchArray(Rec.Counter,(uchar *)U3_RecBuffer,6,(uchar *)"$GPGSV\0");	// GPS
		if(pHead != NULL)
		{
			GPS.GPS_SatelNum = Str2Int( (char*)(pHead+11));
		}	
	}
	
	// /////////////////////////////////////////RMC����
	if(pGPS != NULL)
	{
		pHead = SearchArray(Rec.Counter,(uchar *)U3_RecBuffer,6,(uchar *)"$GNRMC\0");
		if (pHead==NULL)
		{				
			pHead = SearchArray(Rec.Counter,(uchar *)U3_RecBuffer,6,(uchar *)"$GPRMC\0");
		}
		if(pHead != NULL)
		{					
			NoDataCnt = 0;
			pEnter = SearchArray(Rec.Counter,(uchar*)pHead,2,(uchar*)"\r\n\0");
			if(pEnter != NULL)
			{		 
				Len = pEnter - pHead + 2;
				memmove((uchar*)pGPS,(uchar*)pHead,Len);
				GPS_RecOK = TRUE;					
				for(i=6;i<25;i++)
				{
					if( (*(pGPS+i))=='A')
					{
						GPS_Valid = 'A';
						KeyDisp.SetLED_Status(LED_GPS,LED_8T_PER_1SEC);
						break;
					}
					else if( (*(pGPS+i))=='V')
					{
						GPS_Valid = 'V';
						KeyDisp.SetLED_Status(LED_GPS,LED_1T_PER_1SEC);
						break;
					}
				}
				if(TD.GPS == tGPS_ORIGINAL_DATA)					// ���ԭʼGPS����
				{	
					Uart.puts(pGPS);
					os_dly_wait(2);
				}					
			}
		}	 
		else
		{		 
			NoDataCnt++;
			if(NoDataCnt>12)
			{
				NoDataCnt = 0;
				KeyDisp.SetLED_Status(LED_GPS,LED_4T_PER_1SEC);
				if(U3_Baudrate == 9600)
				{
					U3_Baudrate = 4800;
					Uart.puts("GPS baute 4800");						
				}
				else if(U3_Baudrate == 4800)
				{
					U3_Baudrate = 38400;
					Uart.puts("GPS baute 38400");
				}	
				else if(U3_Baudrate == 38400)
				{
					U3_Baudrate = 9600;
					Uart.puts("GPS baute 9600");
				}
//				else if(U3_Baudrate == 19200)
//				{
//					U3_Baudrate = 9600;
//				}
							
				//Init(U3_Baudrate);			// ���³�ʼ������
				if(TD.MainTask==tMAIN_UART)
				{
					SetBufferValue(AntBuf,16,0);
					strcpy((char*)AntBuf,"Baud:");
					Int16ToDecChar(U3_Baudrate,(char*)&AntBuf[5]);
					strcat((char*)AntBuf,"\r\n\0");
					Uart.puts((char*)AntBuf);
				}				
			}
		}
	}
	
	// �������
	if(pANT != NULL)
	{
		pHead = SearchArray(Rec.Counter,(uchar *)U3_RecBuffer,6,(uchar *)"$GPTXT\0");
		if(pHead != NULL)
		{
			pEnter = SearchArray(Rec.Counter,(uchar*)pHead,2,(uchar*)"\r\n\0");
			if(pEnter != NULL)
			{
				Len = pEnter - pHead + 2;
				memmove((uchar*)pANT,(uchar*)pHead,Len);
				GPS_ANT_Check = TRUE;
				if(TD.GPS == tGPS_ORIGINAL_DATA)					// ���ԭʼGPS����
				{	
					Uart.puts(pANT);
					os_dly_wait(2);
				}
			}
		}
		else
		{
			*pANT = 0;
		}
		SetBufferValue(AntBuf,16,0);
		AntBuf[0] = 0xb5;		AntBuf[1] = 0x62;
		AntBuf[2] = 0x0a;		AntBuf[3] = 0x09;
		AntBuf[4] = 0;

		if(TD.GPS == tGPS_ANT_STATUS)
		{
			pHead = SearchArray(Rec.Counter,(uchar*)U3_RecBuffer,2,(uchar*)AntBuf);
			if(pHead != NULL)
				Uart.putb(1,80,(uchar*)pHead);
		}

		pHead = SearchArray(Rec.Counter,(uchar*)U3_RecBuffer,4,(uchar*)AntBuf);
		if(pHead != NULL)
		{
			
			GPS_ANT_Check = *(pHead+26);
			if(GPS_ANT_Check==0)
				strcpy(pANT,"ANTSTATUS=INIT\r\n0");
			else if(GPS_ANT_Check==1)
				strcpy(pANT,"ANTSTATUS=DONTKNOW\r\n0");
			else if(GPS_ANT_Check==2)
				strcpy(pANT,"ANTENNA OK\0");
			else if(GPS_ANT_Check==3)
				strcpy(pANT,"ANTENNA SHORT\0");
			else if(GPS_ANT_Check==4)
				strcpy(pANT,"ANTENNA OPEN\0");
			else
				strcpy(pANT,"\0");
		}
	}				
	/////////////////////////////////// GPGGA  ��ȡˮƽ���ȣ� ���β��� 
	//$GPGGA,100900.000,3959.9999,N,11600.0000,E,1,22,0.6,48.7,M,0.0,M,,*59
	pHead = SearchArray(Rec.Counter,(uchar *)U3_RecBuffer,6,(uchar *)"$GPGGA\0");
	if(pHead != NULL)
	{
		pEnter = SearchArray(Rec.Counter,(uchar *)pHead,2,(uchar *)"\r\n\0");
		if(pEnter != NULL)
		{
			s = pEnter-pHead;
			for(Len=0,i=0;i<s;i++)
			{
				if( *(pHead+i) == ',')	  Len++;
				if( Len == 8 )
				{
					i++;
					tl = Str2Int((char *)(pHead+i));
					tl <<= 16;
					tl &= 0x00ff0000;
					SystemLog.GG1.Speed_Info |=  tl;  		  			// ˮƽ����
				}
				else if(Len == 9 )
				{
					i++;
					GPS_Altitude = Str2Int( (char*)(pHead+i));		// ���θ߶�
					//if(GPS_Altitude>7000)	GPS_Altitude = 50;
					if (GPS_Valid != 'A')	  GPS_Altitude = 0xffff;
						
					break;
				}
			}
		}
	}
	// ��������
	if( (TD.GPS == tGPS_ALL_DATA)||((TD.GPS==tTASK_RUN_FLAG)&&(TD.Value==0))	)
	{
		if(Rec.Counter)
		{
			//Uart.puts(U3_RecBuffer);
			Uart.putb(1,Rec.Counter,(uchar *)U3_RecBuffer);
			//os_dly_wait(6);
			Rec.Counter = 0;
		}
	}		
	return (GPS_RecOK||GPS_ANT_Check);
}


uint16		Uart3Cls::WaitRecBuffer(uint16 WaitTick,uint16 Length,uchar *pR)
{
	uint16	RetLen;
	uint32	i;
	if(WaitTick<100)	WaitTick = 100;	//  ���ٵȴ�100���ģ���1��
	for(i=0;i<WaitTick;i++)		// 10��
	{
		if(++Rec.OverTimeCnt >= 10)
		{
			if(Rec.Counter)
			{ 	// ֻ��ȡָ����󳤶ȣ��ò���ͨ����AT����ķ���״̬
				RetLen = Rec.Counter;
				if(Rec.Counter<Length)			
					Length =  Rec.Counter;
				memmove(pR,U3_RecBuffer,Length);	
				Rec.Counter = 0;
				return RetLen;
			}
		}
		os_dly_wait(1);
	}
	return 0;
}




void			Uart3Cls::ClearRecBuffer(void)
{
	SetBufferValue((uchar*)U3_RecBuffer,LEN_U3_REC_BUFFER,0);
	Rec.Counter=0;
}


void			Uart3Cls::GetAllGPS_Data(char *pData)
{
	strcpy( pData, (char*)U3_RecBuffer);	
}	








uchar		Uart3Cls::puts(char *p)			 // ����OK
{
	putb( strlen(p),(uchar*)p);
	return 0;
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�putb()
   �����������Ӵ���3����ָ�����ȵ�16������
   ��ڲ�������ʼ����ͣ����ݳ��ȣ������͵�����ָ��
   ���ڲ��������ͳɹ�����־���ɹ�Ϊ0�����ɹ�Ϊ��0
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺2009-10-23 
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
uchar         Uart3Cls::putb(uint16 Len,uchar *p)
{
	uint16	i;
	if(Len<1)	return OK;
	for(i=0;i<Len;i++)	{ Txd.Buffer[i] = *(p+i);	}
	Txd.Sending = TRUE;
	Txd.Length = Len-1;
	pTxd = Txd.Buffer;
	LPC_UART3->THR  = *pTxd++;	
	return OK;

}  // End of <PutBuffer> Function


__inline		void			Uart3Cls::Interrupt(void)    // UART3 interrupt function
{
   uchar	   	volatile		IIR,rc;
 	uint32		volatile		i,Len;
	IIR = LPC_UART3->IIR;
	switch (IIR & 0x0e)		  
	{
     	case 0x02:		    // Transmit interrupt $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$	  
			NOP();	NOP();
			if(Txd.Length)
			{
     			if(Txd.Length>FIFO_LENGTH)	 	
					Len = FIFO_LENGTH;
				else					 	
					Len = Txd.Length;
				for(i=0;i<Len;i++)	{	LPC_UART3->THR = *pTxd++;	}
				Txd.Length -= Len;
				Txd.Sending = TRUE;
     		}
			else
			{
     			Txd.Sending = FALSE;
			}
			break;
		case 0x0c:		// ����������Ч����ʱ��ע�����ﲻ����break���			
	  	case 0x04:     // ����������Ч $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
			NOP();	NOP();
			Rec.OverTimeCnt = 0;
	   	while((LPC_UART3->LSR&0x01) == 1)		 //  ���14�ֽ�
	   	{
	   	   rc = LPC_UART3->RBR;	

				if (StartBDUpdate	== TRUE)		// FOR TEST
				{
					LPC_UART0->THR = rc;
				}

				if(Rec.Counter<LEN_U3_REC_BUFFER)
					U3_RecBuffer[Rec.Counter++] = rc;				

				// $GPZDA
				EndData[0] = EndData[1];  	  EndData[1] = EndData[2];  	
				EndData[2] = rc;
				if( (EndData[0]=='Z')&&(EndData[1]=='D')&&(EndData[2]=='A') )
				{
					isr_evt_set( 0x0001, TaskID.GPS );
				}
	   	}	   	
	  		NOP();	NOP();	  		
	   	break;
		default: 
			break;
	} 		  		
	NOP();	NOP();
	//VICSoftIntClr |= (1 << UART3_INT_VECT);//
}// End of UART3 Interrupt function



extern		"C"		void		UART3_IRQHandler(void)		__irq
{
	Uart3.Interrupt();
	//VICVectAddr = 0;	
}
			




bool			Uart3Cls::Init(uint32	bps)
{
	uint16  Fdiv;	
	uint32	Fclk;	
	///////////////  							UART3����������ر�־����
	NOP();	NOP();
	////////////////////////////////  	UART3 Initialize	
	UART3_PIN_SELECT;	             	// Enable RxD3 and TxD3  
	U3_Baudrate = bps;
   LPC_UART3->LCR = 0x83;   
	
	Fclk = LPC_SC->PCLKSEL1;
	Fclk &= (3UL<<18);					// B.18.19
	Fclk >>= 18;
	Fclk = PeriClock[Fclk];
	                    
	Fdiv = (Fclk / 16) / U3_Baudrate;    // set baudrate
   LPC_UART3->DLM = Fdiv / 256;							
	LPC_UART3->DLL = Fdiv % 256;	    
	LPC_UART3->LCR = ((U3_DATA_BIT)|(U3_STOP_BIT<<2)|(U3_CHECK_BIT_EN<<2)|(U3_CHECK_BIT<<4)); 
	LPC_UART3->FCR = 0xc7;                       // Eable FIFO
		
	NVIC_SetPriority(UART3_IRQn, UART3_INT_PRIORITY);     
   NVIC_EnableIRQ(UART3_IRQn); 
		
	LPC_UART3->IER = 0x03;	

   NOP();	NOP();
	
	return 	TRUE; 
	//os_dly_wait(2);
}  // End of UART3 initialize function



Uart3Cls::Uart3Cls(void)
{
	//
}

/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */
