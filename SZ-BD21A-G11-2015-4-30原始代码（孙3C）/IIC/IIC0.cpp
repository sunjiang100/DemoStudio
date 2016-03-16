/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												广州国业电子科技有限公司
$										Shawyoo Electronics (GuangZhou) R&D Center
$                       	http://www.guoyee.com		 Email: guoyegps@126.com
$								 QQ群：9069471/9069433/9524253		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      文件描述
	文件名: 	  IIC0.c
	功能描述： IIC0底层驱动程序
	编译环境： Keil for ARM + ARTX
	目标CPU:   LPC2000
	作者：	  肖建辉
	创建时间： 
	建议编辑环境：UltraEdit 11.00a+  制表符宽度为三个字符，程序可读性更好。
					  Keil中可用配置工具
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/					  

#define		IIC0_GLOBALS	1

#include		"IIC0.h"

#include		"..\UART\UART0.h"

#define		NO_USE_IIC0				0





/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：IIC_Read
   功能描述：按指定的从机地址的当前存储地址和长度读取数据
   入口参数：从机地址，任务命令，读取的长度，接收缓冲
   出口参数：无
   全局变量: 无
   创建时间：
   修改时间：
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
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
		if (i++ > 100*15)  return ERROR; // 15S	超时
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
	函数名称：IIC_Write
   功能描述：写数据到从机
   入口参数：从机地址，任务命令，长度，写入的数据区指针
   出口参数：
   全局变量: 
   创建时间：
   修改时间：
	注：调用些函数的入口参数pBuffer如果是局部变量，调用后需延时。
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
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
		if (i++ > 100*15)  return ERROR; // 15S	超时
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
	函数名称：IIC_WriteRead
   功能描述：按指定的从机地址的随机存储地址读取数据
   入口参数：器件地址，任务命令，写长度和写数据缓冲指针，读长度和数据缓冲指针
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
	注：调用些函数的入口参数pW、pR如果是局部变量，调用后需延时。
*/ 	
//void			Delay_mS(unsigned char  c);
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
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
		if (i++ > 100*15)  return ERROR; // 15S	超时
		os_dly_wait(1);	
	}
	I2C_Busy = TRUE;		
	SlaverAddress = SAddr;
	Command = Cmd;
	WriteLength = WLen;						// 写长度
	ReadWrite = TW_WRITE;
	RW_Type = IIC_RW_COMM;	
	pWriteBuffer = pW;				// 写缓冲
	////////////////////////////////////////	
	ReadLength = RLen;						// 读长度
	pReadBuffer  = pR;	  			// 读缓冲
	TW_Start();								// 启动IIC
	return OK;
}	// End of <IIC_WriteRead> function

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：ReadEEPROM
   功能描述：读EEPROM
   入口参数：器件地址，存储器地址，读长度，读取存储指针
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
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
			sAddr[0] = RAddr;			// 铁电和EEPROM都是24C04
			if(RAddr>=1024)			DAddr |= 0x08;
			if((RAddr%1000)>=512)	DAddr |= 0x04;
			if((RAddr%512)>=256)		DAddr |= 0x02;
			WriteRead(DAddr,0,1,(uchar*)sAddr,RLen,pData); 
			os_dly_wait(1+(RLen/(I2C_FREQUENCY/1000)));			// 以适应读数据时延时
		}
		else 
		{
			sAddr[0] = RAddr>>8;
			sAddr[1] = RAddr;
			WriteRead(DAddr,0,2,(uchar*)sAddr,RLen,pData);
			os_dly_wait(1+(RLen/(I2C_FREQUENCY/1000)));			// 以适应读数据时延时
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
	函数名称：EEPROM_Write
   功能描述：写EEPROM
   入口参数：器件地址，页长度，写存储地址，写长度，待写数据指针
   出口参数：输入成功与否（只判断输入EEPROM，不判断是否写成功），返回0表示成功
   全局变量: 
   创建时间：
   修改时间：
*/ 
#include		<string.h>
static	uchar			PageBuffer[128+8];
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
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
		if(DAddr == EE_DA_PARAMETER_CFG) 		// 写铁电存储器
		{
			PageBuffer[0] = WAddr;					// 写长度不能超过128字节，否则出错
			if(WAddr>=1024)			DAddr |= 0x08;
			if((WAddr%1000)>=512)	DAddr |= 0x04;
			if((WAddr%512)>=256)		DAddr |= 0x02;
			memmove(&PageBuffer[1],pData,WLen);
			if(ERROR == Write(DAddr,0,(WLen+1),PageBuffer)) return ERROR;
			os_dly_wait(1);		
		}
		else if( (DAddr == EE_DA_AT24C01_16_IC_CARD))
		{
			PageBuffer[0] = WAddr;				// 写长度不能超过16字节，否则出错
			if(WAddr>=1024)			DAddr |= 0x08;
			if((WAddr%1000)>=512)	DAddr |= 0x04;
			if((WAddr%512)>=256)		DAddr |= 0x02;
			memmove(&PageBuffer[1],pData,WLen);
			if (ERROR == Write(DAddr,0,(WLen+1),PageBuffer)) return ERROR;
			os_dly_wait(1);	
		}
		else if (DAddr == IIC_KIONIX_KXTF9)			 // 加速度传感器
		{
			if(TD.MainTask==tMAIN_IIC)		{ Uart.puts("write Accelerometer...\r\n\0");}
			PageBuffer[0] = WAddr;
			memmove(&PageBuffer[1],pData,WLen);
			if(ERROR == Write(DAddr,0,(WLen+1),PageBuffer)) return ERROR;
			os_dly_wait(1);	

		}
		else											// 写EEPROM，双字节存储地址
		{	
			if(PageLen%8)		return ERROR;		// 页长度必须为8的倍数	
			////////////////////////////////////// 写首址
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
				if(WAddr%PageLen)									// 不是从整页地址开始写
				{
					owLen = PageLen-WAddr%PageLen;			// 第一轮写的长度
					memmove(&PageBuffer[2],pData,owLen);	// 第一轮写的数据
					if(ERROR == Write(DAddr,0,(owLen+2),PageBuffer)) return ERROR;	// 输入EEPROM
					os_dly_wait(1);
					WAddr += owLen;				// 下一页地址
					pData += owLen;
					WLen -= owLen;	
				}
				/////////////////////////////////////////////////////////////
				while(WLen)
				{
					PageBuffer[0] = WAddr>>8;
					PageBuffer[1] = WAddr;							// 写地址
					if(WLen>=PageLen)
						owLen = PageLen;
					else
						owLen = WLen;
					memmove(&PageBuffer[2],pData,owLen);		// 取数据到缓冲
					if(ERROR == Write(DAddr,0,(owLen+2),PageBuffer)) return ERROR;		// 输入EEPROM
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
	函数名称：I2C0_INT
   功能描述：IIC0中断服务程序
   入口参数：无
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
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
					LPC_I2C0->I2CONCLR = (TW_INT_FLAG);    // 清除IIC中断标志					
					TW_Start();							// 再次启动IIC
					return;
				}
				else			// 单写完成
				{
					//UserEvtFlag.IIC = IIC_WRITE_OK_EVT_FLAG;
					TW_Stop();        //  结束总线 
					I2C_Busy = FALSE;
				}
				//isr_evt_set(IIC_WRITE_EVT_OK,tidGetSpeedValue);
			}
			break;
		case TW_MT_DATA_NACK:	// 发送了数据没收到应答
			if(WriteLength)
				Success = NOT_OK;
			TW_Stop();			
			I2C_Busy = FALSE;
			break;	
		///&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
		// Master Receive Mode  //////////////////////////////////////////////////
		case TW_MR_SLA_ACK:		//	SLA+R has been transmitted; ACK has been received			
			if(ReadLength>1)
				TW_Ack();       	// 下次发送应答信号
			else
				TW_NoAck();      	// 下次发送非应答信号				
			break;		
		case TW_MR_DATA_ACK:		// Data byte has been received,ACK has been returned
			*pReadBuffer++ = LPC_I2C0->I2DAT;	
			if(ReadLength)					
				ReadLength--;
			if(ReadLength==1)			
			{
				ReadLength = 0;
				TW_NoAck();      	// 下次发送非应答信号
			}
			break;		
		case TW_MR_DATA_NACK:	// Data byte has been received,NOT ACK has been returned
			*pReadBuffer++ = LPC_I2C0->I2DAT;
			/////////////////////////////////////////////////////////	
			if(ReadLength)
				Success = NOT_OK;					
			////////////////////////////////////////////////////////
			NOP();
			TW_Stop();           //  结束总线 			
			I2C_Busy = FALSE;
			break;
		///&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
		// Master 异常处理  //////////////////////////////////////////////////
		case TW_MR_SLA_NACK:		// 发送了地址没收到应答
		case TW_MT_SLA_NACK:		// 发送了地址没收到应答
		
		case TW_MT_ARB_LOST:		// 丢失仲裁
		default: 
			Success = NOT_OK;
			TW_Stop();			
			I2C_Busy = FALSE;
			break;		
	}
	LPC_I2C0->I2CONCLR = (TW_INT_FLAG);    // 清除中断标志
	NOP();	NOP();	
}	// End of <I2C0_INT> function




extern		"C"		void		I2C0_IRQHandler(void)		__irq
{
	NOP();	NOP();
	I2C0.Interrupt();
	//VICVectAddr = 0;	
}



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：I2cInit
   功能描述：I2c0初始化函数
   入口参数：无
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void		IICCls::Init(void)
{
	uint32	Fclk;
	I2C0_PIN_SELECT;

	LPC_I2C0->I2CONSET = 0x6C;                      // 清除控制寄存器 
   NVIC_DisableIRQ(I2C0_IRQn);  
	
	Fclk = LPC_SC->PCLKSEL0;
	Fclk &= (0x0000003<<14);								// B.13.12
	Fclk >>= 14;
	Fclk = PeriClock[Fclk]; 	   
	
		
   LPC_I2C0->I2SCLH = (Fclk / I2C_FREQUENCY + 1) / 2;    	// 设置高电平时间 
   LPC_I2C0->I2SCLL = (Fclk / I2C_FREQUENCY) / 2;        	// 设置低电平时间               
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
