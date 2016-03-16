/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												 
$										 
$                       	 		  
$								  		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      文件描述
	文件名: 	 c
	功能描述：UART1
	编译环境：Keil for ARM + ARTX1.10
	目标CPU:  LPC2000
	作者：
	创建时间：
	建议编辑环境：UltraEdit 11.00a+  制表符宽度为三个字符，程序可读性更好。
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/				
#define		UART1_GLOBALS			1

#include    "UART1.h"
#include			"..\common\Algol.H"


#include		<string.h>

#define			TRIGGER_WITH_FIFO1		0


#define			TEST_UART1			0



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
	if(WaitTick<100)	WaitTick = 100;	//  最少等待100节拍，即1秒
	for(i=0;i<WaitTick;i++)		// 10秒
	{
		if(++Rec.OverTimeCnt >= 10)
		{
			if(Rec.Counter)
			{ 	// 只读取指定最大长度，该操作通常是AT命令的返回状态
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
	函数名称：Uart1Cls::PutString
   功能描述：从串口1发送一块数据（字符串）
   入口参数：待发送的字符串数据指针
   出口参数：发送成功与否标志，成功为0，不成功为非0 
   全局变量: 
   创建时间：
   修改时间：
	注：拷贝缓冲
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
uchar         Uart1Cls::puts(char  *p)
{ 
	uint16	Len;	
	Len = strlen(p);
	return putb(1,Len,(uchar*)p);	
}   // End of <PutString> Function


//void			OP_Recorder(uint16  Addr);
//uchar			CheckNewDriver(uchar  *pNew);

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：UART1_PutBuffer
   功能描述：从串口1发送一块数据（16进制）
   入口参数：初始检验和，数据长度，待发送的数据指针
   出口参数：发送成功与否标志，成功为0，不成功为非0
   全局变量: 
   创建时间：
   注：不拷贝缓冲
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
uchar         Uart1Cls::putb(uchar  InitSum,uint16 Len,uchar *pData)
{
	uchar		c;
	
	NOP();	NOP();
	
	while(Txd.Sending==TRUE)  	 { 	 os_dly_wait(1);	}
	
	Txd.Sending = TRUE;

	if(Len==0)					// 长度为0，即可返回
	{
		ClearTransmitFlag();					
		return 0;
	}

	Txd.Length = Len;
	if(pData!=NULL)	memmove(U1_TxdBuffer,pData,Txd.Length);	  // pData==NULL时，外部直接使用Txd.Buffer
	pTxd = U1_TxdBuffer;

	Txd.Length--;
	c = *pTxd++;
	LPC_UART1->THR = c;	
	return 0;
}  // End of <PutBuffer> Function





/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：UART1_INT
   功能描述：UART1中断函数，实现UART0数据链路层数据通讯
   入口参数：无
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
*/ 	

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
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
		case 0x0c:		// 接收数据有效，超时。注：这里不能用break语句			
	  	case 0x04:     // 接收数据有效 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
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
							   
				  	if(Rec.Counter<UART1_REC_BUF_LENGTH)		// 不能超出缓冲区长度
					  	U1_RecBuffer[Rec.Counter++] = rc;	
						
					 UpdataCounter++;  // 升级时记数字节*/
					 
					 rc = LPC_UART1->RBR;
					 //if (FTPdowningflag == TRUE)
					 //	LPC_UART0->THR = rc;   // for debug	
					
					 if(Rec.Counter>=UART1_REC_BUF_LENGTH)
					  	Rec.Counter = 0;
					 
					 
					 U1_RecBuffer[Rec.Counter] = rc;
					 Rec.Counter++;
	
					UpdataCounter++;  // 升级时记数字节			
		   	}

			}
			else
			{
				while((LPC_UART1->LSR&0x01) == 1)
		   	{
		   		rc = LPC_UART1->RBR;	 
					//LPC_UART0->THR = rc; // for debug	
							   
				  	if(Rec.Counter<UART1_REC_BUF_LENGTH)		// 不能超出缓冲区长度
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
	函数名称：UART1_ClearReceiveFlag
   功能描述：清除接收相关标志
   入口参数：无
   出口参数：无
   全局变量: 无
   创建时间：
   修改时间：
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
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
	函数名称：UART1_ClearTransmitFlag
   功能描述：清除发送缓冲区及相关标志
   入口参数：无
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
*/ 	
void			Uart1Cls::ClearTransmitFlag(void)
{
	Txd.Sending = 0;	
	Txd.Length = 0;	
	CLEAR_RTS;
} // End of Function


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：UART1_Init
   功能描述：初始化UART1
   入口参数：无
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
*/ 	
extern		"C"		unsigned long	 install_irq( unsigned long IntNumber, void *HandlerAddr,
									  unsigned long Priority );
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
bool			Uart1Cls::Init(uint32  bps)
{
	//uint16  Fdiv;
	uint32	Fclk;		
	///////////////  UART1缓冲区及相关标志清零
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

	// pclk = 100MHz, bps = 115200				GSM    移植注意修改
   LPC_UART1->FDR = (14 << 4)  | 9;          // 小数 
	LPC_UART1->DLM = 0;							 
	LPC_UART1->DLL = 33;

	// 57600
	/*LPC_UART1->FDR = (13 << 4)  | 4;          // 小数 
	LPC_UART1->DLM = 0;							 
	LPC_UART1->DLL = 83;*/

	// 38400
	/*LPC_UART1->FDR = (15 << 4)  | 7;          // 小数 
	LPC_UART1->DLM = 0;							 
	LPC_UART1->DLL = 111;*/

	
		    
   LPC_UART1->LCR = 0x03;                       // 1 start bit,8 bits, No Check, 1 Stop bit 
   LPC_UART1->FCR = 0xc7;                       // Enable FIFO			 modify by xzb

	NVIC_SetPriority(UART1_IRQn, UART1_INT_PRIORITY);     
   NVIC_EnableIRQ(UART1_IRQn);  

	LPC_UART1->IER = 0x03;
	
	return 	OK;								// 保证函数<UART1_SendChar>正常
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




