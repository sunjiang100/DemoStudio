/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												 
$										 
$                       	 		  
$								  		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      文件描述
	文件名: 	 Uart.c
	功能描述：UART0底层驱动，数据链路层程序，遵循汽车行驶记录仪国标（GB/T19056-2003）
	编译环境：Keil for ARM uVision3
	目标CPU:  LPC2000
	作者：	 
	创建时间：
	建议编辑环境：UltraEdit 11.00a+  制表符宽度为三个字符，程序可读性更好。
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
	函数名称：GetRecBuffer
   功能描述：读取数据
   入口参数：结构指针
   出口参数：
   全局变量: 
   创建时间：
   修改时间：
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
uint16		UartCls::GetRecBuffer(uchar *pR)
{
	uint16	Length;
	if(++Rec.OverTimeCnt==5) 	// 有线通讯，50ms 即可
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
	函数名称：PutString
   功能描述：从串口0发送以结束符'\0'结束的字符串
   入口参数：待发送的字符串数据指针
   出口参数：发送成功与否标志，成功为0，不成功为非0 
   全局变量: 
   创建时间：
   修改时间：
*/ 	

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
uchar         UartCls::puts(char  *p)
{
	uint16	Len;	
	if( GetFlashData.TotalPage)	return 0;
	Len = strlen(p);
	return putb(1,Len,(uchar*)p);		
}   // End of <PutString> Function



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：PutBuffer
   功能描述：从串口0发送指定长度的16进制数
   入口参数：初始检验和，数据长度，待发送的数据指针
   出口参数：发送成功与否标志，成功为0，不成功为非0
   全局变量: 
   创建时间：
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
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
	函数名称：
   功能描述：
   入口参数：
   出口参数：
   全局变量: 
   创建时间：
   修改时间：2009-5-12 18:37
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
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
	LPC_UART0->THR  = *pTxd++;						// 发送数据
	SendAlig.Busy = 0;
	Txd.Sending = TRUE;
	return OK;
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：UART0_INT
   功能描述：UART0中断函数，实现UART0数据链路层数据通讯
   入口参数：无
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
*/ 	

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
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
		case 0x0c:		// 接收数据有效，超时。注：这里不能用break语句						
	  	case 0x04:     // 接收数据有效 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
			NOP();	NOP();
			Rec.OverTimeCnt = 0;			
	   	while((LPC_UART0->LSR&0x01) == 1)		 //  最多14字节
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
	函数名称：ClearReceiveFlag
   功能描述：清除接收相关标志
   入口参数：无
   出口参数：无
   全局变量: 无
   创建时间：
   修改时间：
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
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
	函数名称：ClearTransmitFlag
   功能描述：清除发送缓冲区及相关标志
   入口参数：无
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
*/ 	
void			Delay_mS(unsigned char  c);
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void			UartCls::ClearTransmitFlag(void)
{
	
	NOP();	NOP();	 	
	Txd.CheckSum = Txd.Sending = 0;	
	Txd.Length = 0;		
} // End of Function				  







/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：UART0_Init
   功能描述：初始化UART0
   入口参数：无
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
*/ 	

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用


bool			UartCls::Init(uint32	bps)
{
	//uint16  	Fdiv;	
	uint32	Fclk;	
	///////////////  							UART0缓冲区及相关标志清零
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
		// pclk = 100MHz, bps = 115200				GSM    移植注意修改
	   LPC_UART0->FDR = (14 << 4)  | 9;          // 小数 
		LPC_UART0->DLM = 0;							 
		LPC_UART0->DLL = 33;
		
	}
	else 
	{
		// pclk = 100MHz, bps = 38400				U0    移植注意修改
	   LPC_UART0->FDR = (15 << 4)  | 7;          // 小数 
		LPC_UART0->DLM = 0;							 
		LPC_UART0->DLL = 111;
		
	}
		 
	//#if (WUXI_TEST == 1)  
	LPC_UART0->LCR = ((U0_DATA_BIT)|(U0_STOP_BIT<<2)|(U0_CHECK_BIT_EN<<3)|(U0_CHECK_BIT<<4)); 		// 使用校验
	//#else
	//LPC_UART0->LCR = ((U0_DATA_BIT)|(U0_STOP_BIT<<2)|(0<<3)|(U0_CHECK_BIT<<4)); 	  // 不使用校验
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








