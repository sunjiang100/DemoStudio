/***************************************  Copyright (c)  **************************************
**										 
**									
**                      
**	  
****************************************  Copyright (c)  **************************************
**                                         文件描述
**	文件名:   Uart2.cpp
**	功能描述：Uart2底层驱动.
**	编译环境：Keil for ARM uVision3
**	目标CPU:  LPC2378
**	作者：	 
**	创建时间：
** 修改时间: 2009年07月17日
**	建议编辑环境：UltraEdit 11.00a+  制表符宽度为三个字符，程序可读性更好。
*************************************************************************************************/				

#define			UART2_GLOBALS			1

#include    	"UART2.h"
#include    	"UART0.h"

#include		"..\UserSys.h"	

#include			"..\KeyDisp\KeyDisp.h"
#include			"..\Speed\Speed.h"
#include			"..\GSM_GPRS\GSM_GPRS.h"
#include			"..\GSM_GPRS\JT_T808.h"




Uart2Cls::Uart2Cls(void)
{
	
}//End of Uart2cls function

/*
uchar		const		VedioData[523] = 
{
	0x90,0xEB,0x01,0x31,0x03,0x02,0x00,0x00,0x00,0xFF,0xD8,0xFF,0xE0,0x00,0x11,0x4A,
	0x46,0x49,0x46,0x00,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x0A,0xFF,0xDB,
	0x00,0x43,0x00,0x08,0x06,0x06,0x07,0x06,0x05,0x08,0x07,0x07,0x07,0x09,0x09,0x08,
	0x0A,0x0C,0x14,0x0D,0x0C,0x0B,0x0B,0x0C,0x19,0x12,0x13,0x0F,0x14,0x1D,0x1A,0x1F,
	0x1E,0x1D,0x1A,0x1C,0x1C,0x20,0x24,0x2E,0x27,0x20,0x22,0x2C,0x23,0x1C,0x1C,0x28,
	0x37,0x29,0x2C,0x30,0x31,0x34,0x34,0x34,0x1F,0x27,0x39,0x3D,0x38,0x32,0x3C,0x2E,
	0x33,0x34,0x32,0xFF,0xDB,0x00,0x43,0x01,0x09,0x09,0x09,0x0C,0x0B,0x0C,0x18,0x0D,
	0x0D,0x18,0x32,0x21,0x1C,0x21,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,
	0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,
	0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,
	0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0xFF,0xC4,0x00,0x1F,0x00,0x00,0x01,0x05,
	0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x02,
	0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0xFF,0xC4,0x00,0xB5,0x10,0x00,0x02,
	0x01,0x03,0x03,0x02,0x04,0x03,0x05,0x05,0x04,0x04,0x00,0x00,0x01,0x7D,0x01,0x02,
	0x03,0x00,0x04,0x11,0x05,0x12,0x21,0x31,0x41,0x06,0x13,0x51,0x61,0x07,0x22,0x71,
	0x14,0x32,0x81,0x91,0xA1,0x08,0x23,0x42,0xB1,0xC1,0x15,0x52,0xD1,0xF0,0x24,0x33,
	0x62,0x72,0x82,0x09,0x0A,0x16,0x17,0x18,0x19,0x1A,0x25,0x26,0x27,0x28,0x29,0x2A,
	0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x53,
	0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x73,
	0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x92,
	0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,
	0xAA,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,
	0xC8,0xC9,0xCA,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xE1,0xE2,0xE3,0xE4,
	0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,
	0xFF,0xC4,0x00,0x1F,0x01,0x00,0x03,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
	0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,
	0x0B,0xFF,0xC4,0x00,0xB5,0x11,0x00,0x02,0x01,0x02,0x04,0x04,0x03,0x04,0x07,0x05,
	0x04,0x04,0x00,0x01,0x02,0x77,0x00,0x01,0x02,0x03,0x11,0x04,0x05,0x21,0x31,0x06,
	0x12,0x41,0x51,0x07,0x61,0x71,0x13,0x22,0x32,0x81,0x08,0x14,0x42,0x91,0xA1,0xB1,
	0xC1,0x09,0x23,0x33,0x52,0xF0,0x15,0x62,0x72,0xD1,0x0A,0x16,0x24,0x34,0xE1,0x25,
	0xF1,0x17,0x18,0x19,0x1A,0x26,0x27,0x28,0x29,0x2A,0x35,0x36,0x37,0x38,0x39,0x3A,
	0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,
	0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x73,0x6D,0x41
};	*/



void		Uart2Cls::ReceiveProcessData(void)								  //暂时不搞外接显示屏
{
	//uint16	Len;
	NOP();	NOP();	NOP();

	if(++RecOverTimeCnt != 3)	return;
	
	if((TD.Display==tDISP_TEST_FOR_EXT))
	{		
		Uart.puts(" U2 Rec Test Data:");
		Uart.putb(1,RecLength,RecBuffer);  
		Uart.puts("\r\n");
	}

	if ((RecBuffer[0]==0x55) && (RecBuffer[1]==0xaa))
	{
		KeyDisp.TPMS_ReceiveProcessData(RecLength,RecBuffer);		
	}
	else if( RecBuffer[0]==0x0f)		// YouHao LCM
	{
		KeyDisp.YH_ReceiveProcessData();
	}
	else
	{
		//GPRS.PushAlignment(0,0,RecLength,RecBuffer);	
		GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_DATA_TRAN_UPLOAD,RecLength,RecBuffer);			
	}	

	RecLength = 0; // 
}



uint16		Uart2Cls::RFID_ReceiveData(uchar *pData)
{
	uint16	Length;
	if(++RecOverTimeCnt != 3)	return 0;	
	if(RecLength)
	{
		Length = RecLength;
		memmove(pData,RecBuffer,Length);	 //
		RecLength = 0;	
		return Length;
	}
	return 0;
}





uint16		Uart2Cls::WaitGetRecBuffer(uchar *pData)
{
	uint16	Length;
	uint32	i;


	for(i=0;i<(2*100);i++)		// 最久等2秒
	//for(i=0;i<(3*100);i++)		// 最久等3秒		新摄像头
	{
		os_dly_wait(1);
		//if(++RecOverTimeCnt==5)
		if(++RecOverTimeCnt>=5)
		{
			if(RecLength)
			{
				Length = RecLength;
				memmove(pData,RecBuffer,Length);	 //
				RecLength = 0;	
				return Length;
			}
		}
	}
	return 0;
}


uint16		Uart2Cls::GetRecBuffer(uchar *pData)
{
	uint16	Length;
	if(RecLength)
	{
		Length = RecLength;
		memmove(pData,RecBuffer,Length);	 //
		RecLength = 0;	
		return Length;
	}
	return 0;
}




/**********************************************************************************
** 函数名称：Puts()
** 功能描述：从串口2发送字符串,以结束符'\0'结束.
** 入口参数：待发送的字符串数据指针
** 出口参数：发送成功与否标志，成功为0，不成功为非0 
** 全局变量: 无
** 创建时间：
** 修改时间：2009.05.19
*************************************************************************************/ 	
uchar		Uart2Cls::puts(char *p)
{
	uint16		i,len,DlyCnt=0;
	uchar			*pTxd;

	while(TxdBufUsing)				// 正在操作发送缓冲区，需要等待
	{ 
		os_dly_wait(1);
		DlyCnt++;
		if(DlyCnt%50)		Uart.puts("U2进队超时 ");
		if(DlyCnt>300)
		{
			Uart2.Init(Baudrate.U2_232,U2_FOR_DISPLAY);	// 串口
			os_dly_wait(100);
			break;
		}
	} 

	len = strlen((char*)p);	
	if(len > UART2_TXD_LENGTH-TxdLength)
		return (1);							//写发送缓冲区错误，剩余的发送缓冲区空间不够.

	
	TxdBufUsing = 1;
	
	pTxd = (uchar*)p;
	for(i=0;i<len;i++)
	{
		TxdBuffer[TxdLength+i] = *pTxd;
		pTxd++;
	}
	TxdLength += len;
	TxdBufUsing = 0;
	return  0;

}// End of <PutString> Function


/********************************************************************************
** 函数名称：Putb()
** 功能描述：从串口2发送指定长度的16进制数
** 入口参数：初始检验和，数据长度，待发送的数据指针
** 出口参数：发送成功与否标志，成功为0，不成功为非0
** 全局变量: 
** 创建时间：
** 修改时间：2009.05.19
***********************************************************************************/ 	
uchar		Uart2Cls::putb(uchar InitSum,uint16 L,uchar *p)
{
	uchar			*pTxd,CheckSum;
	uint16		i,len,DlyCnt=0;

	if(L==0)		return 0;				  			// 长度为0，即可返回
			
	
	while(TxdBufUsing)				// 正在操作发送缓冲区，需要等待
	{ 
		os_dly_wait(1);
		DlyCnt++;
		if(DlyCnt%50)		Uart.puts("U2进队超时 ");
		if(DlyCnt>300)
		{
			Uart2.Init(Baudrate.U2_232,U2_FOR_DISPLAY);	// 串口
			os_dly_wait(100);
			break;
		}
	}  					
	
	len = L;	
	if(len > UART2_TXD_LENGTH-TxdLength)
	{
		Uart.puts("U2发送，长度出错 ");
		return (1);							//写发送缓冲区错误，剩余的发送缓冲区空间不够.
	}
	
	TxdBufUsing = 1;
	
	
								  
	pTxd = (uchar*)p;	
	if(!InitSum)					  			// 如果调用本函数前已计算校验和，则这里不再计算
	{
		CheckSum = 0;
		for(i=0;i<len;i++)
		{
	   		CheckSum ^= *p;
	   		p++;
		}
		*p = CheckSum;			  				// Add ChechSum at the end of the buffer
		len += 1;           	
	}	
	for(i=0;i<len;i++)
	{
		TxdBuffer[TxdLength+i] = *pTxd; 
		pTxd++;
	}
	TxdLength += len;
	TxdBufUsing = 0;
	return  0;
	
}// End of <PutBuffer> Function

extern	"C"		void       Delay_mS(unsigned char  c);
/**************************************************************************************
** 函数名称：UART2.Interrupt()
** 功能描述：UART2中断函数
** 入口参数：无
** 出口参数：无
** 全局变量: 
** 创建时间：
** 修改时间：2009.05.19
**************************************************************************************/ 	
__inline	void	Uart2Cls::Interrupt(void)    // UART0 interrupt function
{
	uchar   	volatile		IIR,rc;//,c;
	
	uint32	volatile		i,Len;
	
	NOP();   NOP();	
	IIR = LPC_UART2->IIR;
	switch (IIR & 0x0e)		  
	{
     	case 0x02:		    					 	// Transmit interrupt  
     		NOP();	NOP();
			if(TxdLength)
			{
     			if(TxdLength>FIFO_LENGTH)	 	
					Len = FIFO_LENGTH;
				else					 	
					Len = TxdLength;
				RS485_TRANSMIT_ENABLE;
				NOP();NOP();NOP();NOP();NOP();
				for(i=0;i<Len;i++)	
					LPC_UART2->THR = TxdBuffer[Counter+i];
				TxdLength = TxdLength - Len ;
				Counter = Counter + i;								
     		}
			else
			{
				Counter=0;
				TxdLength = 0;
				TxdBufUsing = 0;
				Sys.Delay_uS(500);
				RS485_RECEIVE_ENABLE;
				NOP();NOP();NOP();NOP();NOP();
			}
			break;		  
		case 0x0c:									// 接收数据有效，超时。注：这里不能用break语句
			//Rec.OverTime = 1;
	  	case 0x04:     		 					// 接收数据有效
	   		RecOverTimeCnt=0;
			while((LPC_UART2->LSR&0x01)==0x01)		 
	   		{		
					rc = LPC_UART2->RBR;						
					if(RecLength < UART2_REC_LENGTH)			
						RecBuffer[RecLength++] = rc;	//接收数据	  
	   								
	   		}			  
			break;
		default: break;
	}		  		
	NOP();	NOP();								  
	//VICSoftIntClr |= (1 << UART0_INT_VECT);
}// End of UART0 Interrupt function


extern	"C"	void	UART2_IRQHandler(void)  __irq
{
	Uart2.Interrupt();
	//VICVectAddr = 0;	
}

/********************************************************************************
** 函数名称：UART2_Init()
** 功能描述：UART2初始化
** 入口参数：无
** 出口参数：无
** 全局变量: 
** 创建时间：
** 修改时间：2009.05.19
***********************************************************************************/ 	
bool		Uart2Cls::Init(uint32 bps,uchar PinSel)
{
	uchar		tBuffer[16];
	uint32	Fclk;
	//UART2缓冲区及相关标志清零
	NOP();	NOP();	
	
	if((bps<110)||(bps>115200))
		bps = 115200;
	U2PinSelDevice = PinSel;
	if(U2PinSelDevice==U2_FOR_CAMERA)
	{	UART2_PIN_FOR_RS485;	}			// P2.8,P2.9  Enable RxD2 and TxD2 
	else
	{	UART2_PIN_FOR_RS232;	}		  	// P0.10,P0.11  Enable RxD2 and TxD2                  
	LPC_UART2->LCR = 0x83;                
	
	Fclk = LPC_SC->PCLKSEL1;
	Fclk &= (3UL<<16);					// B.16.17
	Fclk >>= 16;
	Fclk = PeriClock[Fclk];
	
	strcpy( (char*)tBuffer,"U2 Init \0\0\0");      
	tBuffer[8] = U2PinSelDevice;
	Uart.puts((char*)tBuffer);
	
	
	if (bps == 9600)	  // pclk = 100MHz, bps = 9600	 yOUHAO								手柄  移植注意修改
	{
		LPC_UART2->FDR = (2 << 4)  | 1;          // 小数 
		LPC_UART2->DLM = 1;							 
		LPC_UART2->DLL = 178;	
	}
		
	else if  (bps == 19200)	  // pclk = 100MHz, bps = 19200	 BJS								手柄  移植注意修改
	{
		LPC_UART2->FDR = (5 << 4)  | 1;          // 小数 
		LPC_UART2->DLM = 1;							 
		LPC_UART2->DLL = 15;	
	} 
	else if (bps==38400)
	{
		LPC_UART2->FDR = (15 << 4)  | 7;          // 小数 
		LPC_UART2->DLM = 0;							 
		LPC_UART2->DLL = 111;
	}
	else if(bps==57600)	// pclk = 100MHz, bps = 57600		摄像头	 移植注意修改
	{
		// 57600
		LPC_UART2->FDR = (13 << 4)  | 4;          // 小数 
		LPC_UART2->DLM = 0;							 
		LPC_UART2->DLL = 83;
	}
	else if (bps == 115200)
	{
		// pclk = 100MHz, bps = 115200				GSM    移植注意修改
    	LPC_UART2->FDR = (14 << 4)  | 9;          // 小数 
		LPC_UART2->DLM = 0;							 
		LPC_UART2->DLL = 33;
	} 
	 
	

	LPC_UART2->LCR = ((U2_DATA_BIT)|(U2_STOP_BIT<<2)|(U2_CHECK_BIT_EN<<3)|(U2_CHECK_BIT<<4));
    LPC_UART2->IER = 0x03;                      // Enable RDA and THRE interrupts 
	LPC_UART2->FCR = 0xc7;           				//c7           // Eable FIFO

	NVIC_SetPriority(UART2_IRQn, UART2_INT_PRIORITY);     
    NVIC_EnableIRQ(UART2_IRQn); 	
   
	LPC_UART2->IER = 0x03;
				
	#if(USE_QUERY_SEND)
       LPC_UART2->THR = 'A';					   // 保证函数<Uart0_SendChar>正常
	#endif
    NOP();	NOP();	

	Counter = 0;
	//TxdBufUsing = RecOverTimeCnt = 100;
	RecOverTimeCnt = 100;
	TxdBufUsing = 0;
	RecLength   = TxdLength   = 0;
	RS485_RECEIVE_ENABLE;
	NOP();NOP();NOP();
	return OK;

}  // End of UART0 initialize function


/********************************************************************************
** 函数名称：UART2_SendTask()
** 功能描述：UART2发送任务
** 入口参数：无
** 出口参数：无
** 全局变量: 
** 创建时间：2009.05.19
** 修改时间：
***********************************************************************************/ 

void Uart2Cls::SendTask()
{
	uchar	c;
	
	while(TxdBufUsing)  { 	 os_dly_wait(1);	}
	
	
	//TxdBufUsing = 1;						//错误位置
	if(TxdLength)
	{	
		RS485_TRANSMIT_ENABLE;
		NOP();NOP();NOP();NOP();NOP();
		TxdBufUsing = 1;					// 这个位置很关键			
		Counter = 0;
		c = TxdBuffer[Counter++];  
		LPC_UART2->THR = c;
		TxdLength--;
		//if( UseRecordNow )	Uart.puts(" U2录音模块通讯 ");					
	}	
}// End of UART0 SendTask function






void			Uart2Cls::ClearRecFlag(void)
{
	//uint16	i;
	Counter = 0;
	RecOverTimeCnt = 100;	
	RecLength  = 0;	
}





void		Uart2Cls::RecEnable(void)
{
	
}







/***********************  End of File *****************************************/
