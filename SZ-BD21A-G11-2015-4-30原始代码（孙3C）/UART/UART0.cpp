/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												 
$										 
$                       	 		  
$								  		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      �ļ�����
	�ļ���: 	 Uart.c
	����������UART0�ײ�������������·�������ѭ������ʻ��¼�ǹ��꣨GB/T19056-2003��
	���뻷����Keil for ARM uVision3
	Ŀ��CPU:  LPC2000
	���ߣ�	 
	����ʱ�䣺
	����༭������UltraEdit 11.00a+  �Ʊ�����Ϊ�����ַ�������ɶ��Ը��á�
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/				


#define		UART0_GLOBALS			1

#include    "..\UART\UART0.h"
#include		<string.h>
//#include		"..\Common\CppGlobals.h"
//#include		"..\Displaylcd\DisplayLcd.h"


#define		TEST_UART0				0
#define		TRIGGER_WITH_FIFO		0

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�GetRecBuffer
   ������������ȡ����
   ��ڲ������ṹָ��
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
uint16		UartCls::GetRecBuffer(uchar *pR)
{
	uint16	Length;
	if(++Rec.OverTimeCnt==5) 	// ����ͨѶ��50ms ����
	{
		if(Rec.Counter)
		{
			Length = Rec.Counter;
			memmove(pR,U0_RecBuffer,Rec.Counter);
			Rec.Counter = 0;
			return Length;
		}
	}
	return 0;
}


uchar		UartCls::GetSendBusyFlag(void)
{
	return Txd.Sending;
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�PutString
   �����������Ӵ���0�����Խ�����'\0'�������ַ���
   ��ڲ����������͵��ַ�������ָ��
   ���ڲ��������ͳɹ�����־���ɹ�Ϊ0�����ɹ�Ϊ��0 
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 	

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
uchar         UartCls::puts(char  *p)
{
	uint16	Len;	
	if( GetFlashData.TotalPage)	return 0;
	Len = strlen(p);
	return putb(1,Len,(uchar*)p);		
}   // End of <PutString> Function



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�PutBuffer
   �����������Ӵ���0����ָ�����ȵ�16������
   ��ڲ�������ʼ����ͣ����ݳ��ȣ������͵�����ָ��
   ���ڲ��������ͳɹ�����־���ɹ�Ϊ0�����ɹ�Ϊ��0
   ȫ�ֱ���: 
   ����ʱ�䣺
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
uchar         UartCls::putb(uchar InitSum,uint16 Len,uchar *p)
{
	NOP();	NOP();	NOP();
	if( GetFlashData.TotalPage)	return 0;
	while(SendAlig.Busy)	{	os_dly_wait(1);	}
	SendAlig.Busy = TRUE;
	if(Len <= (UART0_SEND_ALIG_LENGTH-SendAlig.Length))
	{
		memmove(&U0_SendAligBuffer[SendAlig.Length],(uchar*)p,Len);	
		SendAlig.Length += Len;	
		SendAlig.Busy = 0;	
		return OK;
	}
	else
	{
		SendAlig.Busy = 0;	
		return ERROR;
	}	
}  // End of <PutBuffer> Function




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�
   ����������
   ��ڲ�����
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺2009-5-12 18:37
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
uchar         UartCls::PopAlignmentSend(void)
{
	uint16	SendLen;
	NOP();	NOP();	
	if( GetFlashData.TotalPage)	return 0;
	if(SendAlig.Length == 0)
		return OK;
	while(SendAlig.Busy)	
	{	
		os_dly_wait(1);	
	}
	SendAlig.Busy = TRUE;
	while(Txd.Sending)	
	{
		os_dly_wait(1);	
	}
	if(SendAlig.Length<=UART0_TXD_LENGTH)
		SendLen = SendAlig.Length;
	else
		SendLen = UART0_TXD_LENGTH;
	memmove(U0_TxdBuffer,U0_SendAligBuffer,SendLen);
	memmove(U0_SendAligBuffer,&U0_SendAligBuffer[SendLen],(SendAlig.Length-SendLen) );
	SendAlig.Length -= SendLen;
	Txd.Length = SendLen;
	pTxd = U0_TxdBuffer;
	Txd.Length--;
	LPC_UART0->THR  = *pTxd++;						// ��������
	SendAlig.Busy = 0;
	Txd.Sending = TRUE;
	return OK;
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�UART0_INT
   ����������UART0�жϺ�����ʵ��UART0������·������ͨѶ
   ��ڲ�������
   ���ڲ�������
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 	

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
__inline		void			UartCls::Interrupt(void)    // UART0 interrupt function
{
   uchar   	volatile		IIR,rc;
   uint32	volatile		i,Len;
   NOP();   NOP();	
	IIR = LPC_UART0->IIR;
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
				for(i=0;i<Len;i++)	{	LPC_UART0->THR = *pTxd++;	}
				Txd.Length -= Len;
				Txd.Sending = TRUE;
     		}
			else
			{
     			ClearTransmitFlag();							
			}
		break;	//	  
		case 0x0c:		// ����������Ч����ʱ��ע�����ﲻ����break���						
	  	case 0x04:     // ����������Ч $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
			NOP();	NOP();
			Rec.OverTimeCnt = 0;			
	   	while((LPC_UART0->LSR&0x01) == 1)		 //  ���14�ֽ�
	   	{
	   		rc = LPC_UART0->RBR;	  
	   		if(Rec.Counter<UART0_REC_LENGTH)
					U0_RecBuffer[Rec.Counter++] = rc;
	   	}
	  		NOP();	NOP();	
		break; 
		default: break;
	}	// End of Switch  		  		
	NOP();	NOP();
}  // End of UART0 Interrupt function



extern		"C"		void		UART0_IRQHandler(void)		__irq
{
	Uart.Interrupt();
	//VICVectAddr = 0;	
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�ClearReceiveFlag
   �������������������ر�־
   ��ڲ�������
   ���ڲ�������
   ȫ�ֱ���: ��
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void			UartCls::ClearReceiveFlag(uint16 c)
{ 	
	NOP();	NOP();  	
	Rec.OverTimeCnt = 100;
	Rec.Counter = 0;
	if(c)
	{	
		for(c=0;c<UART0_REC_LENGTH;c++)	{ U0_RecBuffer[c] = 0; }
	}
} // End of Function				 




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�ClearTransmitFlag
   ����������������ͻ���������ر�־
   ��ڲ�������
   ���ڲ�������
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 	
void			Delay_mS(unsigned char  c);
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void			UartCls::ClearTransmitFlag(void)
{
	
	NOP();	NOP();	 	
	Txd.CheckSum = Txd.Sending = 0;	
	Txd.Length = 0;		
} // End of Function				  







/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�UART0_Init
   ������������ʼ��UART0
   ��ڲ�������
   ���ڲ�������
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 	

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������


bool			UartCls::Init(uint32	bps)
{
	//uint16  	Fdiv;	
	uint32	Fclk;	
	///////////////  							UART0����������ر�־����
	NOP();	NOP();	
	ClearReceiveFlag(0);	
	ClearTransmitFlag();
	if((bps<110)||(bps>115200))
		bps = 115200;
	////////////////////////////////  	UART0 Initialize	
	UART0_PIN_SELECT;		              // Enable RxD0 and TxD0  
    LPC_UART0->LCR = 0x83;                       
	
	Fclk = LPC_SC->PCLKSEL0;
	Fclk &= (3UL<<6);					// B.6.7
	Fclk >>= 6;
	Fclk = PeriClock[Fclk];

	/*Fdiv = (Fclk / 16) / bps;    // set baudrate
   LPC_UART0->DLM = Fdiv / 256;							
	LPC_UART0->DLL = Fdiv % 256;*/

	if (bps == 115200)
	{
		// pclk = 100MHz, bps = 115200				GSM    ��ֲע���޸�
	   LPC_UART0->FDR = (14 << 4)  | 9;          // С�� 
		LPC_UART0->DLM = 0;							 
		LPC_UART0->DLL = 33;
		
	}
	else 
	{
		// pclk = 100MHz, bps = 38400				U0    ��ֲע���޸�
	   LPC_UART0->FDR = (15 << 4)  | 7;          // С�� 
		LPC_UART0->DLM = 0;							 
		LPC_UART0->DLL = 111;
		
	}
		 
	//#if (WUXI_TEST == 1)  
	LPC_UART0->LCR = ((U0_DATA_BIT)|(U0_STOP_BIT<<2)|(U0_CHECK_BIT_EN<<3)|(U0_CHECK_BIT<<4)); 		// ʹ��У��
	//#else
	//LPC_UART0->LCR = ((U0_DATA_BIT)|(U0_STOP_BIT<<2)|(0<<3)|(U0_CHECK_BIT<<4)); 	  // ��ʹ��У��
	//#endif
	LPC_UART0->FCR = 0xc7;                       // Eable FIFO
		

	NVIC_SetPriority(UART0_IRQn, UART0_INT_PRIORITY);     
   NVIC_EnableIRQ(UART0_IRQn); 

	LPC_UART0->IER = 0x03;
	return 	TRUE; 
}  // End of UART0 initialize function



UartCls::UartCls(void)
{
	
}

/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */








