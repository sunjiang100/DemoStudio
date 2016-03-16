/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												 
$										 
$                       	 		  
$								  		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      �ļ�����
	�ļ���: 	 c
	����������UART1
	���뻷����Keil for ARM + ARTX1.10
	Ŀ��CPU:  LPC2000
	���ߣ�
	����ʱ�䣺
	����༭������UltraEdit 11.00a+  �Ʊ�����Ϊ�����ַ�������ɶ��Ը��á�
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/				
#define		UART1_GLOBALS			1

#include    "UART1.h"
#include			"..\common\Algol.H"


#include		<string.h>

#define			TRIGGER_WITH_FIFO1		0


#define			TEST_UART1			0



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
uint16		Uart1Cls::GetRecBuffer(uchar *pR)
{
	uint16	Length;
	//if(++Rec.OverTimeCnt==5)						//10
	if(++Rec.OverTimeCnt>=10)						//10
	{
		if(Rec.Counter)
		{
			Length = Rec.Counter;
			memmove(pR,U1_RecBuffer,Rec.Counter);
			Rec.Counter = 0;
			return Length;
		}
	}
	return 0;
}



uint16		Uart1Cls::GetRecLength(void)
{
	return Rec.Counter;
}



void		Uart1Cls::WaitSendFree(void)
{
	
	while(Txd.Sending==TRUE)  { 	 os_dly_wait(1);	}
	
}



uint16		Uart1Cls::WaitRecBuffer(uint16 WaitTick,uint16 Length,uchar *pR)
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
				memmove(pR,U1_RecBuffer,Length);	
				Rec.Counter = 0;
				return RetLen;
			}
		}
		os_dly_wait(1);
	}
	return 0;
}
	






/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�Uart1Cls::PutString
   �����������Ӵ���1����һ�����ݣ��ַ�����
   ��ڲ����������͵��ַ�������ָ��
   ���ڲ��������ͳɹ�����־���ɹ�Ϊ0�����ɹ�Ϊ��0 
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
	ע����������
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
uchar         Uart1Cls::puts(char  *p)
{ 
	uint16	Len;	
	Len = strlen(p);
	return putb(1,Len,(uchar*)p);	
}   // End of <PutString> Function


//void			OP_Recorder(uint16  Addr);
//uchar			CheckNewDriver(uchar  *pNew);

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�UART1_PutBuffer
   �����������Ӵ���1����һ�����ݣ�16���ƣ�
   ��ڲ�������ʼ����ͣ����ݳ��ȣ������͵�����ָ��
   ���ڲ��������ͳɹ�����־���ɹ�Ϊ0�����ɹ�Ϊ��0
   ȫ�ֱ���: 
   ����ʱ�䣺
   ע������������
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
uchar         Uart1Cls::putb(uchar  InitSum,uint16 Len,uchar *pData)
{
	uchar		c;
	
	NOP();	NOP();
	
	while(Txd.Sending==TRUE)  	 { 	 os_dly_wait(1);	}
	
	Txd.Sending = TRUE;

	if(Len==0)					// ����Ϊ0�����ɷ���
	{
		ClearTransmitFlag();					
		return 0;
	}

	Txd.Length = Len;
	if(pData!=NULL)	memmove(U1_TxdBuffer,pData,Txd.Length);	  // pData==NULLʱ���ⲿֱ��ʹ��Txd.Buffer
	pTxd = U1_TxdBuffer;

	Txd.Length--;
	c = *pTxd++;
	LPC_UART1->THR = c;	
	return 0;
}  // End of <PutBuffer> Function





/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�UART1_INT
   ����������UART1�жϺ�����ʵ��UART0������·������ͨѶ
   ��ڲ�������
   ���ڲ�������
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 	

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
uint32		U1RecStopCnt;
__inline		void			Uart1Cls::Interrupt(void)    // UART1 interrupt function
{
   uchar   	volatile		IIR,rc;
   
	uint32	volatile		i,Len;
   NOP();   NOP();	
   
	IIR = LPC_UART1->IIR;	
	switch (IIR & 0x0e)		  
	{
     	case 0x02:		    // Transmit interrupt $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$	  
     		NOP();	NOP();			
			if(Txd.Length)
			{
     			if(Txd.Length>UART1_FIFO_LENGTH)	 	
					Len = UART1_FIFO_LENGTH;
				else					 	
					Len = Txd.Length;
				for(i=0;i<Len;i++)	{	LPC_UART1->THR = *pTxd;		pTxd++;	}
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
			//U1RecStopCnt = 0;
			if (FTPdowningflag == TRUE)
			{
				while((LPC_UART1->LSR&0x01) == 1)
		   	{
		   		/*rc = LPC_UART1->RBR;	 
					//LPC_UART0->THR = rc; // for debug	
					//	NOP();	NOP();
							   
				  	if(Rec.Counter<UART1_REC_BUF_LENGTH)		// ���ܳ�������������
					  	U1_RecBuffer[Rec.Counter++] = rc;	
						
					 UpdataCounter++;  // ����ʱ�����ֽ�*/
					 
					 rc = LPC_UART1->RBR;
					 //if (FTPdowningflag == TRUE)
					 //	LPC_UART0->THR = rc;   // for debug	
					
					 if(Rec.Counter>=UART1_REC_BUF_LENGTH)
					  	Rec.Counter = 0;
					 
					 
					 U1_RecBuffer[Rec.Counter] = rc;
					 Rec.Counter++;
	
					UpdataCounter++;  // ����ʱ�����ֽ�			
		   	}

			}
			else
			{
				while((LPC_UART1->LSR&0x01) == 1)
		   	{
		   		rc = LPC_UART1->RBR;	 
					//LPC_UART0->THR = rc; // for debug	
							   
				  	if(Rec.Counter<UART1_REC_BUF_LENGTH)		// ���ܳ�������������
					  	U1_RecBuffer[Rec.Counter++] = rc;		
		   	}
				
			}
				
			
	  		NOP();	NOP();
		break;
		default: break;
	}	// End of Switch  	
	//VICVectAddr = 0;	
	NOP();   NOP();
}  // End of UART1 Interrupt function



extern		"C"		void		UART1_IRQHandler(void)		__irq
{
	Uart1.Interrupt();
	//VICVectAddr = 0;	
}



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�UART1_ClearReceiveFlag
   �������������������ر�־
   ��ڲ�������
   ���ڲ�������
   ȫ�ֱ���: ��
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void			Uart1Cls::ClearReceiveFlag(uchar ClearBuffer)
{
	uint16		i;
	NOP();	NOP();
	Rec.Counter = 0;		
	if(ClearBuffer)
	{	for(i=0;i<UART1_REC_BUF_LENGTH;i++)	{ U1_RecBuffer[i] = 0;	}	}
	CLEAR_RTS;
} // End of Function




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�UART1_ClearTransmitFlag
   ����������������ͻ���������ر�־
   ��ڲ�������
   ���ڲ�������
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 	
void			Uart1Cls::ClearTransmitFlag(void)
{
	Txd.Sending = 0;	
	Txd.Length = 0;	
	CLEAR_RTS;
} // End of Function


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�UART1_Init
   ������������ʼ��UART1
   ��ڲ�������
   ���ڲ�������
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 	
extern		"C"		unsigned long	 install_irq( unsigned long IntNumber, void *HandlerAddr,
									  unsigned long Priority );
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
bool			Uart1Cls::Init(uint32  bps)
{
	//uint16  Fdiv;
	uint32	Fclk;		
	///////////////  UART1����������ر�־����
	if((bps<110)||(bps>115200))
		bps = 57600;
	NOP();	NOP();	
	ClearReceiveFlag(1);
	ClearTransmitFlag();
	////////////////////////////////  UART1 Initialize	
	//PINSEL4 &= ~0x0000000f;
	UART1_PIN_SELECT;
	CLEAR_RTS;
   LPC_UART1->LCR = 0x83;    
	
	Fclk = LPC_SC->PCLKSEL0;
	Fclk &= (3UL<<8);					// B.8.9
	Fclk >>= 8;
	Fclk = PeriClock[Fclk];
	
	                   
	/*Fdiv = (Fclk / 16) / bps;    	// set baudrate
   LPC_UART1->DLM = Fdiv / 256;							
	LPC_UART1->DLL = Fdiv % 256;*/

	// pclk = 100MHz, bps = 115200				GSM    ��ֲע���޸�
   LPC_UART1->FDR = (14 << 4)  | 9;          // С�� 
	LPC_UART1->DLM = 0;							 
	LPC_UART1->DLL = 33;

	// 57600
	/*LPC_UART1->FDR = (13 << 4)  | 4;          // С�� 
	LPC_UART1->DLM = 0;							 
	LPC_UART1->DLL = 83;*/

	// 38400
	/*LPC_UART1->FDR = (15 << 4)  | 7;          // С�� 
	LPC_UART1->DLM = 0;							 
	LPC_UART1->DLL = 111;*/

	
		    
   LPC_UART1->LCR = 0x03;                       // 1 start bit,8 bits, No Check, 1 Stop bit 
   LPC_UART1->FCR = 0xc7;                       // Enable FIFO			 modify by xzb

	NVIC_SetPriority(UART1_IRQn, UART1_INT_PRIORITY);     
   NVIC_EnableIRQ(UART1_IRQn);  

	LPC_UART1->IER = 0x03;
	
	return 	OK;								// ��֤����<UART1_SendChar>����
}  // End of UART1 initialize function



uchar				Uart1Cls::WaitChar(void)
{
	uchar		i;
	char		*pC;
	for(i=0;i<(20*5);i++)
	{
		os_dly_wait(5);
		pC = MyStrStr((char*)U1_RecBuffer,"\r\n>\0");
		if(pC != NULL)
			return OK;
	}
	return ERROR;
}



Uart1Cls::Uart1Cls(void)
{
	//Init(UART0_BPS);
}


/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */




