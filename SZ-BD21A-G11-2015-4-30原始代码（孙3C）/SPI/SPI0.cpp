/*********************************** Copyright (c)  *******************************************************
**							    
**										 
**                       	 
**								  		All rights reserved.	  
***********************************  Copyright (c)  *******************************************************
**                                      文件描述
**	文件名:   SPI0.cpp
**	功能描述：SPI0驱动程序。
**	编译环境：Keil uVision3 + ARM
**	目标CPU:  LPC2000
**	作者：
**	创建时间：
******************************************************************************************************************/					  

#define		SPI0_GLOBALS			1

#include		"SPI.h"

#define     SIMULATE_SPI0        1

#include		"..\Config\UserConfig.h"


//使用模拟SPI口,移植时要修改//////////////
#if(SIMULATE_SPI0)
	#define		SPI0_CLK			(1UL<<15)
	#define		SPI0_MISO		(1UL<<17)
	#define		SPI0_MOSI		(1UL<<18)
	#define		SPI0_Delay() {NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();}
#else
	//#define		SPI0_FREQUNCY       1382400
#endif



/***************************************************************************************
** 函数名称：  SPI0.WriteByte() 
** 功能描述： 
** 入口参数：待输出的数据
** 出口参数输出成功与否标志，成功为0，不成功为非0：
** 全局变量: 
** 创建时间：
** 修改时间：
****************************************************************************************/
uchar	SPI0Cls::WriteByte(uchar Data)
{	
	#if(SIMULATE_SPI0)
		uint16		i;
		NOP();	NOP();
		while(Busing)	{os_dly_wait(1);}
		Busing = 1;
		LPC_GPIO0->FIODIR  &= ~(SPI0_MISO);
		LPC_GPIO0->FIODIR  |= SPI0_CLK|SPI0_MOSI;			// out put 
		SPI0_Delay();
		for(i=0;i<8;i++)
		{
			LPC_GPIO0->FIOCLR |= SPI0_CLK;
			if(Data&0x80)
				LPC_GPIO0->FIOSET |= SPI0_MOSI;
			else
				LPC_GPIO0->FIOCLR |= SPI0_MOSI;
			SPI0_Delay();
			LPC_GPIO0->FIOSET |= SPI0_CLK;
			SPI0_Delay();
			Data <<= 1;							// left shift 1 bit
		}	
		Busing = 0;
		return 0;
	#else
		NOP();	NOP();
		while(Busing)	{os_dly_wait(1);}
		Busing = 1;
		SSPDR = Data;
		while((SSPSR&0x01)==0);     			// wait until SPI transmit complete		
		Busing = 0;
		return (SSPDR);	
	#endif
	
}


/***************************************************************************************************
** 函数名称：SPI0.ReadByte()
** 函数功能：从SPI0读取数据
** 入口参数：无
** 出口参数：
** 全局变量: 
** 创建时间：
** 修改时间：
*******************************************************************************************/ 	
uchar	SPI0Cls::ReadByte(void)
{
	#if(SIMULATE_SPI0)
		uint16		i;
		uint32		pin;
		uchar		Data = 0;
		NOP();	NOP();
		while(Busing)	{os_dly_wait(1);}
		Busing = 1;
		LPC_GPIO0->FIODIR  &= ~(SPI0_MISO);
		LPC_GPIO0->FIODIR  |= SPI0_CLK|SPI0_MOSI;			// out put 
		SPI0_Delay();	
		for(i=0;i<8;i++)
		{
			LPC_GPIO0->FIOCLR |= SPI0_CLK;					// Output are driven from the falling edge of SCK		
			SPI0_Delay();
			Data <<= 1;			
			pin = LPC_GPIO0->FIOPIN;
			pin &= (uint32)SPI0_MISO;
			if(pin)
				Data |= 0x01;
			else
				Data &= ~0x01;
			LPC_GPIO0->FIOSET |= SPI0_CLK;		
			NOP();	NOP();
			SPI0_Delay();		
		}
		Busing = 0;
		return Data;
	#else	
		
		NOP();	NOP();
		while(Busing)	{os_dly_wait(1);}
		Busing = 1;

		SSPDR = 0xff;							//空写一次		
		while((SSPSR&0x01)==0);					//等待TFE置位,即发送FIFO空
		//while((SSPSR&0x04)==0);					
		//tmp = SSPDR;							//主动读取一字节外部数据
		Busing = 0;
		return(SSPDR);
	#endif
}


/**********************************************************************************************
** 函数名称：SPI0.ReadByte2PIN()
** 功能描述：
** 入口参数：
** 出口参数：
** 全局变量: 
** 创建时间：
** 修改时间：
************************************************************************************************/
/*uchar	SPI0Cls::SPI0_ReadByte2PIN(void)	  //???
{
	#if(SIMULATE_SPI0)
	uint16		i;
	uint32		pin;
	uchar		Data = 0;	
	IODIR0 &= ~(SPI0_MISO|SPI0_MOSI);	
	IODIR0 |=  SPI0_CLK;					// out put 	
	SPI0_Delay();
	for(i=0;i<4;i++)
	{
		IOCLR0 |= SPI0_CLK;					// Output are driven from the falling edge of SCK		
		SPI0_Delay();				
		pin = IO0PIN;		
		Data <<= 1;	
		if(pin&((uint32)SPI0_MISO))			// B.7.5.3.1
			Data |= 0x01;
		else
			Data &= ~0x01;		
		Data <<= 1;	
		if(pin&((uint32)SPI0_MOSI))			// B.6.4.2.0
			Data |= 0x01;
		else
			Data &= ~0x01;			
		IOSET0 |= SPI0_CLK;		
		NOP();	NOP();
		SPI0_Delay();		
	}
	IODIR0  &= ~(SPI0_MISO);
	IODIR0  |=  SPI0_CLK|SPI0_MOSI;			// out put 	
	return Data;
	
	#else	
	uchar dat;
	while((SSPSR&(0x01<<2)==0));     		// wait until SPI transmit complete
	SSPDR = 0;	
	while((SSPSR&(0x01<<2)==0));     		// wait until SPI transmit complete	
	dat=(uchar)SSPDR;
	return dat;
	#endif
}		 */


 /***************************************************************************************
** 函数名称：SPI0.Init()
** 功能描述：SPI口初始化
** 入口参数：无
** 出口参数：无
** 全局变量: 无
** 创建时间：2009年04月07日
** 修改时间：
******************************************************************************************/ 	
void	SPI0Cls::Init(void)
{	
	#if(SIMULATE_SPI0)
	LPC_PINCON->PINSEL0 &= ~0xc0000000;
	LPC_PINCON->PINSEL1 &= ~0x0000003f;
	LPC_GPIO0->FIODIR  &= ~(SPI0_MISO);
	LPC_GPIO0->FIODIR  |=  SPI0_CLK|SPI0_MOSI;			// out put 			  
	#else	
	PINSEL1|=  0x2a8;   					// config P0.17~P0.19 as SPI port
	SSPCR0  =  (0x01<<8)|  		  			// SCR  串行时钟速率
			   (0x01<<7)|			  		// CPHA 为0，第一个时钟；为1，第二个时钟
			   (0x01<<6)|			  		// CPOL 为0，低电平；为1，高电平
			   (0x00<<4)|			  		// FRF  为0，SPI帧；为1，SSI帧
			   (0x07<<0);			  		// DSS   为0111，8位数据；为1111，16位数据    
	SSPCR1  =(0x00<<3)|  		  			// SOD   为0，从机输出禁止；为1，从机输出使能
			   (0x00<<2)|			  		// MS    为0，主机；为1，从机
			   (0x01<<1)|			  		// SSE   SSP使能，
			   (0x00<<0);			  		// LBM   回写模式					
	SSPCPSR = 0x1a;					  		// pclk分频值，移植时要修改(1M)
	SSPICR  = 0x00;					  	// 中断清除寄存器
	SSPIMSC = 0X00;								   
		
	/*S1SPCR   =(0<<3)|	   					// CPHA=0,数据在SCK的第一个时钟沿采样
				(0<<4)|	   					// CPOL=0,SCK为高有效
				(1<<5)|	   					// MST1=1,SPI处于主模式
				(0<<6)|	  					// LSBF=0，SPI数据传输MSB（bit7）在先
				(0<<7);	   					// SPIE=0,SPI中断被允许
	S1SPCCR = 0xfe&(Fpclk / SPI0_FREQUNCY);	*/	 			 
	
	#endif
}															   


SPI0Cls::SPI0Cls(void)
{

}

/**************************************** End of File *************************************************/
