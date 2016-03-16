/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												���ݹ�ҵ���ӿƼ����޹�˾
$										Shawyoo Electronics (GuangZhou) R&D Center
$                       	http://www.guoyee.com		 Email: guoyegps@126.com
$								 QQȺ��9069471/9069433/9524253		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      �ļ�����
	�ļ���: 	  IIC0.c
	���������� IIC0�ײ���������
	���뻷���� Keil for ARM + ARTX
	Ŀ��CPU:   LPC2000
	���ߣ�	  Ф����
	����ʱ�䣺 
	����༭������UltraEdit 11.00a+  �Ʊ�����Ϊ�����ַ�������ɶ��Ը��á�
					  Keil�п������ù���
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/					  

#define		IIC0_GLOBALS	1

#include		"IIC0.h"

#include		"..\UART\UART0.h"

#define		NO_USE_IIC0				0





/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�IIC_Read
   ������������ָ���Ĵӻ���ַ�ĵ�ǰ�洢��ַ�ͳ��ȶ�ȡ����
   ��ڲ������ӻ���ַ�����������ȡ�ĳ��ȣ����ջ���
   ���ڲ�������
   ȫ�ֱ���: ��
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
bool        IICCls::Read(uchar SAddr,uchar Cmd,uint16 Len,uchar *pBuffer)
{
	uint32 i;

	#if ( NO_USE_IIC0 )
	return 0;
	#endif
	NOP();	NOP();
	if(TD.MainTask==tMAIN_IIC)		{ Uart.puts("I2C read ...\r\n\0");}
	i = 0;
	while(I2C_Busy==TRUE) 
	{  
		if (i++ > 100*15)  return ERROR; // 15S	��ʱ
		os_dly_wait(1);	
	}
	I2C_Busy = TRUE;
	SlaverAddress = SAddr;
	Command = Cmd;
	ReadLength = Len;	
	ReadWrite = TW_READ;
	RW_Type = IIC_READ_COMM;
	pReadBuffer = pBuffer;
	TW_Start();		
	return OK;
}	// End of <IIC_Read> function




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�IIC_Write
   ����������д���ݵ��ӻ�
   ��ڲ������ӻ���ַ������������ȣ�д���������ָ��
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
	ע������Щ��������ڲ���pBuffer����Ǿֲ����������ú�����ʱ��
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
bool        IICCls::Write(uchar SAddr,uchar Cmd,uint16 Len,uchar *pBuffer)
{
	uint32	i;
  	#if ( NO_USE_IIC0 )
	return 0;
	#endif
	NOP();	NOP();
	if(TD.MainTask==tMAIN_IIC)		{ Uart.puts("I2C write...\r\n\0");}
	i = 0;
	while(I2C_Busy==TRUE) 
	{  
		if (i++ > 100*15)  return ERROR; // 15S	��ʱ
		os_dly_wait(1);	
	}
	I2C_Busy = TRUE;
	SlaverAddress = SAddr;
	Command = Cmd;
	WriteLength = Len;	
	ReadWrite = TW_WRITE;	
	RW_Type = IIC_WRITE_COMM;
	pWriteBuffer = pBuffer;
	TW_Start();	
	return OK;
	
}	// End of <IIC_Write> function



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�IIC_WriteRead
   ������������ָ���Ĵӻ���ַ������洢��ַ��ȡ����
   ��ڲ�����������ַ���������д���Ⱥ�д���ݻ���ָ�룬�����Ⱥ����ݻ���ָ��
   ���ڲ�������
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
	ע������Щ��������ڲ���pW��pR����Ǿֲ����������ú�����ʱ��
*/ 	
//void			Delay_mS(unsigned char  c);
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
bool       IICCls::WriteRead(uchar SAddr,uchar Cmd,uint16 WLen,uchar *pW,uint16 RLen,uchar *pR)
{
	uint32	i;
	#if ( NO_USE_IIC0 )
	return 0;
	#endif		  
	NOP();	NOP();
	if(TD.MainTask==tMAIN_IIC)		{ Uart.puts("I2C writeread...\r\n\0");}
	i = 0;
	while(I2C_Busy==TRUE) 
	{  
		if (i++ > 100*15)  return ERROR; // 15S	��ʱ
		os_dly_wait(1);	
	}
	I2C_Busy = TRUE;		
	SlaverAddress = SAddr;
	Command = Cmd;
	WriteLength = WLen;						// д����
	ReadWrite = TW_WRITE;
	RW_Type = IIC_RW_COMM;	
	pWriteBuffer = pW;				// д����
	////////////////////////////////////////	
	ReadLength = RLen;						// ������
	pReadBuffer  = pR;	  			// ������
	TW_Start();								// ����IIC
	return OK;
}	// End of <IIC_WriteRead> function

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�ReadEEPROM
   ������������EEPROM
   ��ڲ�����������ַ���洢����ַ�������ȣ���ȡ�洢ָ��
   ���ڲ�������
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
bool			IICCls::ReadEEPROM(uchar DAddr,uint16 RAddr,uint16 RLen,uchar *pData)
{
	static	volatile	 uchar		sAddr[2];
	uint32		FailureCnt = 0;
	#if ( NO_USE_IIC0 )
	return 0;
	#endif
	NOP();	NOP();
	if(TD.MainTask==tMAIN_IIC)		{ Uart.puts("I2C read eprom...\r\n\0");}	
	IC_CARD_RESET_PIN_HIGH;
	while( (I2C_Busy==TRUE)&&(FailureCnt<100) ) {  os_dly_wait(1);	FailureCnt++;	}
	if(FailureCnt>=100)	{ I2C_Busy = 0;	Uart.puts("I2C busing read...\r\n\0");	return ERROR; }
	Success = NOT_OK;
	FailureCnt = 0;
	while( (Success == NOT_OK)&&((FailureCnt++)<3) )
	{
		NOP();	NOP();
		I2C_Busy = 0;
		if( (DAddr == EE_DA_PARAMETER_CFG)||(DAddr == EE_DA_AT24C01_16_IC_CARD) || (DAddr == IIC_KIONIX_KXTF9) )	
		{
			sAddr[0] = RAddr;			// �����EEPROM����24C04
			if(RAddr>=1024)			DAddr |= 0x08;
			if((RAddr%1000)>=512)	DAddr |= 0x04;
			if((RAddr%512)>=256)		DAddr |= 0x02;
			WriteRead(DAddr,0,1,(uchar*)sAddr,RLen,pData); 
			os_dly_wait(1+(RLen/(I2C_FREQUENCY/1000)));			// ����Ӧ������ʱ��ʱ
		}
		else 
		{
			sAddr[0] = RAddr>>8;
			sAddr[1] = RAddr;
			WriteRead(DAddr,0,2,(uchar*)sAddr,RLen,pData);
			os_dly_wait(1+(RLen/(I2C_FREQUENCY/1000)));			// ����Ӧ������ʱ��ʱ
		}
		NOP();	NOP();
	}
	if(FailureCnt>=3)	
	{
	 	I2C_Busy = 0;
		if(TD.MainTask==tMAIN_IIC)		{ Uart.puts("I2C read eprom failure...\r\n\0");}
		return ERROR;
	}
	return OK;
} // End of <ReadEEPROM> function



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�EEPROM_Write
   ����������дEEPROM
   ��ڲ�����������ַ��ҳ���ȣ�д�洢��ַ��д���ȣ���д����ָ��
   ���ڲ���������ɹ����ֻ�ж�����EEPROM�����ж��Ƿ�д�ɹ���������0��ʾ�ɹ�
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 
#include		<string.h>
static	uchar			PageBuffer[128+8];
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
bool			IICCls::WriteEEPROM(uchar DAddr,uchar PageLen,uint16 WAddr,
                        uint16 WLen,uchar *pData)
{
	
	uint32		FailureCnt=0;
	uchar		owLen;		// One time Write Len
	#if ( NO_USE_IIC0 )
	return 0;
	#endif
	NOP();	NOP();
	if(TD.MainTask==tMAIN_IIC)		{ Uart.puts("I2C write eprom...\r\n\0");}
	IC_CARD_RESET_PIN_HIGH;
	while( (I2C_Busy==TRUE)&&(FailureCnt<100) ) {  os_dly_wait(1);	FailureCnt++;	}
	if(FailureCnt>=100)	{ I2C_Busy = 0;	Uart.puts("I2C busing...\r\n\0");	return FAILURE; }
	//////////////////////////////////////////////////////////////////
	//if(PageLen>MAX_PAGE_LEN)		PageLen = MAX_PAGE_LEN;
	Success = NOT_OK;
   FailureCnt = 0;
	while( (Success == NOT_OK)&&((FailureCnt++)<3) )
	{
		I2C_Busy = 0;
		if(DAddr == EE_DA_PARAMETER_CFG) 		// д����洢��
		{
			PageBuffer[0] = WAddr;					// д���Ȳ��ܳ���128�ֽڣ��������
			if(WAddr>=1024)			DAddr |= 0x08;
			if((WAddr%1000)>=512)	DAddr |= 0x04;
			if((WAddr%512)>=256)		DAddr |= 0x02;
			memmove(&PageBuffer[1],pData,WLen);
			if(ERROR == Write(DAddr,0,(WLen+1),PageBuffer)) return ERROR;
			os_dly_wait(1);		
		}
		else if( (DAddr == EE_DA_AT24C01_16_IC_CARD))
		{
			PageBuffer[0] = WAddr;				// д���Ȳ��ܳ���16�ֽڣ��������
			if(WAddr>=1024)			DAddr |= 0x08;
			if((WAddr%1000)>=512)	DAddr |= 0x04;
			if((WAddr%512)>=256)		DAddr |= 0x02;
			memmove(&PageBuffer[1],pData,WLen);
			if (ERROR == Write(DAddr,0,(WLen+1),PageBuffer)) return ERROR;
			os_dly_wait(1);	
		}
		else if (DAddr == IIC_KIONIX_KXTF9)			 // ���ٶȴ�����
		{
			if(TD.MainTask==tMAIN_IIC)		{ Uart.puts("write Accelerometer...\r\n\0");}
			PageBuffer[0] = WAddr;
			memmove(&PageBuffer[1],pData,WLen);
			if(ERROR == Write(DAddr,0,(WLen+1),PageBuffer)) return ERROR;
			os_dly_wait(1);	

		}
		else											// дEEPROM��˫�ֽڴ洢��ַ
		{	
			if(PageLen%8)		return ERROR;		// ҳ���ȱ���Ϊ8�ı���	
			////////////////////////////////////// д��ַ
			PageBuffer[0] = WAddr>>8;
			PageBuffer[1] = WAddr;
			if((WLen+(WAddr%PageLen))<=PageLen)
			{
				memmove(&PageBuffer[2],pData,WLen);
				if(ERROR == Write(DAddr,0,(WLen+2),PageBuffer)) return ERROR;
				os_dly_wait(1);
			}
			else
			{
				if(WAddr%PageLen)									// ���Ǵ���ҳ��ַ��ʼд
				{
					owLen = PageLen-WAddr%PageLen;			// ��һ��д�ĳ���
					memmove(&PageBuffer[2],pData,owLen);	// ��һ��д������
					if(ERROR == Write(DAddr,0,(owLen+2),PageBuffer)) return ERROR;	// ����EEPROM
					os_dly_wait(1);
					WAddr += owLen;				// ��һҳ��ַ
					pData += owLen;
					WLen -= owLen;	
				}
				/////////////////////////////////////////////////////////////
				while(WLen)
				{
					PageBuffer[0] = WAddr>>8;
					PageBuffer[1] = WAddr;							// д��ַ
					if(WLen>=PageLen)
						owLen = PageLen;
					else
						owLen = WLen;
					memmove(&PageBuffer[2],pData,owLen);		// ȡ���ݵ�����
					if(ERROR == Write(DAddr,0,(owLen+2),PageBuffer)) return ERROR;		// ����EEPROM
					os_dly_wait(1);
					WAddr += owLen;
					pData += owLen;
					WLen  -= owLen;
				}		
			}
		}
	}
	if(FailureCnt>=3)	
	{
	 	I2C_Busy = 0;
		if(TD.MainTask==tMAIN_IIC)		{ Uart.puts("I2C write eprom failure...\r\n\0");}
		return ERROR;
	}
	return OK;
}	// End of <WriteEEPROM> function


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�I2C0_INT
   ����������IIC0�жϷ������
   ��ڲ�������
   ���ڲ�������
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
//void			I2C0_INT(void)   __irq      // IIC0 interrupt function



__inline		void		IICCls::Interrupt(void)
{
	uchar		s;
	NOP();	NOP();
	s = Status = LPC_I2C0->I2STAT;
	Success = OK;
	NOP();	NOP();
	switch(s)
	{			
	   /// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
		/// Start or repeat Start bit has been Transmitted
		case TW_MS_START:			// Start bit OK;
		case TW_MS_REP_START: 	// Repeat Start bit OK				
			LPC_I2C0->I2DAT = (SlaverAddress|ReadWrite);
			break;			
		///&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
		// Master Transmitter Mode ///////////////////////////////////////////////				
		case TW_MT_SLA_ACK:		//	SLA+W has been transmitted; ACK has been received						
			LPC_I2C0->I2DAT = *pWriteBuffer++;			
			break;
		case TW_MT_DATA_ACK:		// Data byte in I2DAT has been transmitted; 
										// ACK has been received			
			if(WriteLength)
				WriteLength--;
			if(WriteLength)
				LPC_I2C0->I2DAT = *pWriteBuffer++;			
			else
			{
				if(RW_Type==IIC_RW_COMM)
				{
					ReadWrite = TW_READ;
					RW_Type = IIC_READ_COMM;
					LPC_I2C0->I2CONCLR = (TW_INT_FLAG);    // ���IIC�жϱ�־					
					TW_Start();							// �ٴ�����IIC
					return;
				}
				else			// ��д���
				{
					//UserEvtFlag.IIC = IIC_WRITE_OK_EVT_FLAG;
					TW_Stop();        //  �������� 
					I2C_Busy = FALSE;
				}
				//isr_evt_set(IIC_WRITE_EVT_OK,tidGetSpeedValue);
			}
			break;
		case TW_MT_DATA_NACK:	// ����������û�յ�Ӧ��
			if(WriteLength)
				Success = NOT_OK;
			TW_Stop();			
			I2C_Busy = FALSE;
			break;	
		///&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
		// Master Receive Mode  //////////////////////////////////////////////////
		case TW_MR_SLA_ACK:		//	SLA+R has been transmitted; ACK has been received			
			if(ReadLength>1)
				TW_Ack();       	// �´η���Ӧ���ź�
			else
				TW_NoAck();      	// �´η��ͷ�Ӧ���ź�				
			break;		
		case TW_MR_DATA_ACK:		// Data byte has been received,ACK has been returned
			*pReadBuffer++ = LPC_I2C0->I2DAT;	
			if(ReadLength)					
				ReadLength--;
			if(ReadLength==1)			
			{
				ReadLength = 0;
				TW_NoAck();      	// �´η��ͷ�Ӧ���ź�
			}
			break;		
		case TW_MR_DATA_NACK:	// Data byte has been received,NOT ACK has been returned
			*pReadBuffer++ = LPC_I2C0->I2DAT;
			/////////////////////////////////////////////////////////	
			if(ReadLength)
				Success = NOT_OK;					
			////////////////////////////////////////////////////////
			NOP();
			TW_Stop();           //  �������� 			
			I2C_Busy = FALSE;
			break;
		///&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
		// Master �쳣����  //////////////////////////////////////////////////
		case TW_MR_SLA_NACK:		// �����˵�ַû�յ�Ӧ��
		case TW_MT_SLA_NACK:		// �����˵�ַû�յ�Ӧ��
		
		case TW_MT_ARB_LOST:		// ��ʧ�ٲ�
		default: 
			Success = NOT_OK;
			TW_Stop();			
			I2C_Busy = FALSE;
			break;		
	}
	LPC_I2C0->I2CONCLR = (TW_INT_FLAG);    // ����жϱ�־
	NOP();	NOP();	
}	// End of <I2C0_INT> function




extern		"C"		void		I2C0_IRQHandler(void)		__irq
{
	NOP();	NOP();
	I2C0.Interrupt();
	//VICVectAddr = 0;	
}



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�I2cInit
   ����������I2c0��ʼ������
   ��ڲ�������
   ���ڲ�������
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void		IICCls::Init(void)
{
	uint32	Fclk;
	I2C0_PIN_SELECT;

	LPC_I2C0->I2CONSET = 0x6C;                      // ������ƼĴ��� 
   NVIC_DisableIRQ(I2C0_IRQn);  
	
	Fclk = LPC_SC->PCLKSEL0;
	Fclk &= (0x0000003<<14);								// B.13.12
	Fclk >>= 14;
	Fclk = PeriClock[Fclk]; 	   
	
		
   LPC_I2C0->I2SCLH = (Fclk / I2C_FREQUENCY + 1) / 2;    	// ���øߵ�ƽʱ�� 
   LPC_I2C0->I2SCLL = (Fclk / I2C_FREQUENCY) / 2;        	// ���õ͵�ƽʱ��               
   LPC_I2C0->I2CONSET = 1<<TW_ENABLE;       	 					// Master model
   /////////////////////////////////////////////

	
	NVIC_SetPriority(I2C0_IRQn, I2C0_INT_PRIORITY);     
   NVIC_EnableIRQ(I2C0_IRQn);  
	
	 
	I2C_Busy = WriteLength = ReadLength = RW_Type = 0;
	Command = SlaverAddress = 0;
} //End of <I2C0_Init> Function



IICCls::IICCls(void)
{
	
}


/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */
